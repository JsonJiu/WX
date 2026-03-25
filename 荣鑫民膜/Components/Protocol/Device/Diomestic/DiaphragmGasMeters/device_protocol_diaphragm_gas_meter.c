/**
 * @file device_protocol_diaphragm_gas_meter.c
 * @brief 膜式燃气表下位机通信协议实现 (工装板与被测主控板之间)
 * @version 2.0.0
 * @date 2025-01-20
 *
 * @section intro 简介
 * 实现工装板与膜式燃气表主控板之间的通信协议。
 * 支持红外通信和有线串口通信两种方式。
 *
 * @section protocol_format 协议帧格式
 * [0]     68          - 帧头1
 * [1-6]   表号        - 6字节
 * [7]     68          - 帧头2
 * [8]     控制码      - OPT_READ/OPT_WRITE/OPT_INSTALL
 * [9-10]  数据域长度  - 2字节小端
 * [11-16] 时间        - 6字节BCD
 * [17]    设备类型    - 1字节
 * [18-19] 数据标识    - 2字节小端
 * [20]    帧序号      - 1字节
 * [21...] 数据域      - 可变长度
 * [n-1]   校验和      - 1字节
 * [n]     16          - 帧尾
 *
 * @section protocol_reference 协议参考 (PIC工程)
 * - app_irdausart_protocol.c  (红外协议)
 * - app_MeterUsart_protocol.c (有线协议)
 */

#define LOG_TAG "device_protocol_dgm"

#include "device_protocol.h"
#include "device_protocol_diaphragm_gas_meter_events.h"
#include "utility.h"
#include <elog.h>
#include <stdio.h>
#include <string.h>

// 兼容性: 保留旧接口，但不推荐使用
// TODO: 后续完全移除对 Test_List.h 的依赖
#include "Test_List.h"
extern uint8_t water_meter_type;
extern void protocol_debug_print(const uint8_t *data, uint16_t len);

/*============ 协议帧索引定义 (与PIC一致) ============*/

#define INDEX_68FRAME1 0     // 第一个68的起始索引
#define INDEX_METER_ID 1     // 表地址起始索引（6字节）
#define INDEX_68FRAME2 7     // 第二个68起始索引
#define INDEX_CONTROL_CODE 8 // 控制码索引
#define INDEX_DATA_LENGTH 9  // 数据长度起始索引（2字节，小端）
#define INDEX_TIME 11        // 时间的起始索引（6字节）
#define INDEX_DEVICE_TYPE 17 // 设备类型索引
#define INDEX_DATA_MARK 18   // 数据标识索引（2字节，小端）
#define INDEX_FRAME_SEQUENCE 20 // 帧序号索引
#define INDEX_VOLUME_DATA 21    // 数据域起始索引

#define DATA_CMD_LENGTH_FRONT 10 // 数据区命令长度前面固定部分

/*============ 控制码定义 (与PIC一致) ============*/

// 请求控制码 (下行: 工装→被测表)
#define OPT_READ 0x01     // 读操作
#define OPT_WRITE 0x04    // 写操作
#define OPT_INSTALL 0x05  // 安装/配置操作
#define OPT_LOADLINK 0x08 // 加载链接

// 响应控制码 (上行: 被测表→工装, D7=1)
#define OPT_READ_RESP 0x81    // 读响应
#define OPT_WRITE_RESP 0x84   // 写响应
#define OPT_INSTALL_RESP 0x85 // 安装响应

// 异常应答 (D6=1)
#define OPT_READ_ERR 0xC1    // 读异常
#define OPT_WRITE_ERR 0xC4   // 写异常
#define OPT_INSTALL_ERR 0xC5 // 安装异常

// 控制码辅助宏
#define IS_RESPONSE(ctrl) ((ctrl)&0x80) // 检查是否是响应 (D7=1)
#define IS_ABNORMAL(ctrl) ((ctrl)&0x40) // 检查是否异常应答 (D6=1)

/*============ 数据标识定义 (与PIC一致) ============*/

// 基础命令
#define DEV_TIME 0xC621 // 时间

// 测试相关命令
#define DEV_AutoCheckFinish 0x1000 // 自检完成
#define DEV_BoardInfo                                                          \
  0x1001 // 上告开机信息获取，包含表具类型，是否带附件，主控板电压，模块状态，信号强度，连接状态等
#define DEV_SETOUTIOSTATUS 0x1002 // 设置/获取端口输出状态
#define DEV_CLOSE_IR 0x1005       // 关闭红外
#define DEV_CONFIG_IO_STATUS                                                   \
  0x1007 // 配置端口状态，可以配置通讯电池欠压状态，阀门状态，是否进入低功耗，是否复位
#define DEV_READ_CHECK_STATUS 0x1008 // 读取检测状态/星闪MAC

// IMEI/IMSI/ICCID读取
#define DEV_read_IMEI_IMSI 0xC518       // 读取IMEI/IMSI
#define DEV_read_IMEI_IMSI_ICCID 0xC525 // 读取IMEI/IMSI/ICCID

// 上位机测试命令（已移除，相关定义请在上位机代码中维护）

/*============ 设备类型定义 ============*/

#define DEVICE_TYPE 0x08 // RTU类型

/*============ 检测过程标志定义 (与PIC一致) ============*/

#define MASTER_HALT 0
#define MASTER_SELFCHECK_FINISH 1
#define MASTER_CONNCET_CHECK 2
#define MASTER_CHECK_ONE 3
#define MASTER_CHECK_TWO 4
#define MASTER_CHECK_CURRENT 5
#define MASTER_CHECK_END 6
#define MASTER_IR_CLOSED 7

/*============ 内部变量 ============*/

static ProtocolSendFunc s_send_func = NULL;
static ProtocolEventCallback s_event_callback = NULL;

// 膜式燃气表专用事件回调 (推荐使用)
static DgmEventCallback s_dgm_event_callback = NULL;

// 发送缓冲区
#define DGM_TX_BUF_SIZE 256
static uint8_t s_tx_buffer[DGM_TX_BUF_SIZE];

// 默认表号
static uint8_t s_meter_number[6] = {0x00, 0x00, 0x00, 0x01, 0x00, 0x00};

// RTC时间
static uint8_t s_rtc_time[6] = {0x25, 0x01, 0x20, 0x10, 0x30, 0x00};

// 检测过程状态
static uint8_t s_check_process = MASTER_HALT;

// 高低电平标志 (用于IO状态检测)
static uint8_t s_high_low_flag = 0;

/*============ 内部函数声明 ============*/

static bool dgm_init(void);
static ProtocolResult dgm_parse(uint8_t *data, uint16_t len);
static bool dgm_send_cmd(uint16_t cmd, void *param);
static void dgm_on_response(uint16_t code, const uint8_t *data, uint16_t len);
static void dgm_set_send_func(ProtocolSendFunc func);
static void dgm_set_event_callback(ProtocolEventCallback callback);

// 响应处理函数
static void handle_read_response(const uint8_t *frame, uint16_t data_mark,
                                 uint16_t data_len);
static void handle_write_response(const uint8_t *frame, uint16_t data_mark);
static void handle_install_response(const uint8_t *frame, uint16_t data_mark);

// 命令发送函数
static uint16_t build_cmd_frame(uint8_t *buf, uint8_t ctrl_code,
                                uint16_t data_mark, const uint8_t *data,
                                uint16_t data_len);
static bool send_read_cmd(uint16_t data_mark);
static bool send_write_cmd(uint16_t data_mark, const uint8_t *data,
                           uint16_t data_len);

// 数据解析辅助函数
static void parse_connect_result(const uint8_t *data);
static void parse_imei_imsi_iccid(const uint8_t *data);
static void parse_io_status(const uint8_t *data, uint8_t high_low);

/*============ 协议接口实例 ============*/

const ProtocolInterface diaphragm_gas_meter_protocol = {
    .name = "diaphragm_gas_meter",
    .init = dgm_init,
    .parse = dgm_parse,
    .send_cmd = dgm_send_cmd,
    .on_response = dgm_on_response,
    .set_send_func = dgm_set_send_func,
    .set_event_callback = dgm_set_event_callback,
    .preamble = NULL, // 膜式燃气表不需要前导
};

/*============ 接口实现 ============*/

static bool dgm_init(void) {
  log_i("膜式燃气表下位机协议初始化");
  s_check_process = MASTER_HALT;
  return true;
}

/**
 * @brief 解析主控板响应
 *
 * 协议帧格式与上位机协议相同
 */
static ProtocolResult dgm_parse(uint8_t *data, uint16_t len) {
  uint16_t pos = 0;
  bool handled = false;

  log_d("膜表协议解析, 长度=%d", len);
  elog_hexdump("DGM_RX", 8, data, len);

  // 最小前缀长度: 11字节 (能读到数据域长度字段)
  // 帧头1(1) + 表号(6) + 帧头2(1) + 控制码(1) + 数据域长度(2)
  while (pos + 11 <= len) {
    // 查找第一个帧头
    if (data[pos + INDEX_68FRAME1] != FRAME_HEAD_68) {
      pos++;
      continue;
    }

    // 验证第二个帧头
    if (data[pos + INDEX_68FRAME2] != FRAME_HEAD_68) {
      pos++;
      continue;
    }

    // 获取数据域长度 (小端模式)
    uint16_t data_field_len = READ_LE_U16(&data[pos + INDEX_DATA_LENGTH]);

    // 验证数据域长度的合理性
    // 最小数据域 = 时间(6) + 设备类型(1) + 数据标识(2) + 帧序号(1) = 10字节
    if (data_field_len < 10) {
      log_w("数据域长度异常: %d (最小应为10)", data_field_len);
      pos++;
      continue;
    }

    // 最大数据域长度限制 (防止异常数据)
    if (data_field_len > 200) {
      log_w("数据域长度过大: %d", data_field_len);
      pos++;
      continue;
    }

    // 计算完整帧长度: 前缀(11) + 数据域 + 校验和(1) + 帧尾(1)
    uint16_t frame_len = 11 + data_field_len + 2;

    // 验证是否有完整的帧
    if (pos + frame_len > len) {
      log_d("帧不完整, 需要%d字节, 当前有%d字节", frame_len, len - pos);
      return PROTOCOL_RESULT_INCOMPLETE;
    }

    // 验证帧尾
    if (data[pos + frame_len - 1] != FRAME_TAIL_16) {
      log_d("帧尾错误: 0x%02X, 期望0x16", data[pos + frame_len - 1]);
      pos++;
      continue;
    }

    // 验证校验和
    uint8_t recv_checksum = data[pos + frame_len - 2];
    uint8_t calc_sum = util_checksum_sum8(&data[pos], frame_len - 2);
    if (recv_checksum != calc_sum) {
      log_e("校验和错误: 计算=0x%02X, 接收=0x%02X", calc_sum, recv_checksum);
      pos++;
      continue;
    }

    // 获取控制码和数据标识
    uint8_t ctrl_code = data[pos + INDEX_CONTROL_CODE];
    uint16_t data_mark = READ_LE_U16(&data[pos + INDEX_DATA_MARK]);

    log_d("收到帧: 控制码=0x%02X, 数据标识=0x%04X", ctrl_code, data_mark);

    // 验证是否是响应帧 (D7=1表示上行/响应)
    if (!IS_RESPONSE(ctrl_code)) {
      log_w("不是响应帧: 控制码=0x%02X", ctrl_code);
      pos++;
      continue;
    }

    // 检查是否异常应答 (D6=1表示异常)
    if (IS_ABNORMAL(ctrl_code)) {
      log_e("收到异常应答: 控制码=0x%02X, 数据标识=0x%04X", ctrl_code,
            data_mark);
      // 触发异常事件
      DgmProtocolEvent event = {0};
      event.type = DGM_EVENT_PARSE_ERROR;
      event.data_mark = data_mark;
      if (s_dgm_event_callback) {
        s_dgm_event_callback(&event);
      }
      pos += frame_len;
      continue;
    }

    // 根据响应码分发处理
    switch (ctrl_code) {
    case OPT_READ_RESP: // 0x81 - 读响应
      handle_read_response(&data[pos], data_mark, data_field_len);
      handled = true;
      break;

    case OPT_WRITE_RESP: // 0x84 - 写响应
      handle_write_response(&data[pos], data_mark);
      handled = true;
      break;

    case OPT_INSTALL_RESP: // 0x85 - 安装响应
      handle_install_response(&data[pos], data_mark);
      handled = true;
      break;

    default:
      log_w("未知响应码: 0x%02X", ctrl_code);
      break;
    }

    pos += frame_len;
  }

  return handled ? PROTOCOL_RESULT_OK : PROTOCOL_RESULT_UNKNOWN_CMD;
}

static bool dgm_send_cmd(uint16_t cmd, void *param) {
  // cmd的高8位表示操作类型，低8位是数据标识高字节
  // 或者直接使用预定义的命令码
  switch (cmd) {
  case DEV_BoardInfo:
    return send_write_cmd(DEV_BoardInfo, NULL, 0); // 写上告开机信息

  case DEV_SETOUTIOSTATUS:
    return send_write_cmd(DEV_SETOUTIOSTATUS, (uint8_t *)param, 1);

  case DEV_CLOSE_IR:
    return send_write_cmd(DEV_CLOSE_IR, NULL, 0);

  case DEV_read_IMEI_IMSI_ICCID:
    return send_read_cmd(DEV_read_IMEI_IMSI_ICCID);

  case DEV_READ_CHECK_STATUS:
    return send_read_cmd(DEV_READ_CHECK_STATUS);

  default:
    // 默认作为读命令发送
    return send_read_cmd(cmd);
  }
}

static void dgm_on_response(uint16_t code, const uint8_t *data, uint16_t len) {
  log_d("膜表协议响应: 0x%04X", code);
}

static void dgm_set_send_func(ProtocolSendFunc func) { s_send_func = func; }

static void dgm_set_event_callback(ProtocolEventCallback callback) {
  s_event_callback = callback;
}

/*============ 响应处理实现 ============*/

/**
 * @brief 处理读响应
 */
static void handle_read_response(const uint8_t *frame, uint16_t data_mark,
                                 uint16_t data_len) {
  const uint8_t *payload = &frame[INDEX_VOLUME_DATA];
  DgmProtocolEvent event = {0};

  log_d("处理读响应: 数据标识=0x%04X", data_mark);

  switch (data_mark) {
  case DEV_read_IMEI_IMSI_ICCID: // 0xC525 - 网络参数 (107字节)
    // 来源: 《民用物联网表整机测试通讯协议》章节4.6.10
    event.type = DGM_EVENT_IMEI_RECEIVED;
    event.data_mark = data_mark;

    // 主卡 IMEI [0-14] (15字节)
    memcpy(event.data.imei.imei, &payload[0], 15);
    event.data.imei.imei[15] = '\0';

    // 主卡 IMSI [15-29] (15字节)
    memcpy(event.data.imei.imsi, &payload[15], 15);
    event.data.imei.imsi[15] = '\0';

    // 主卡 ICCID [30-49] (20字节)
    memcpy(event.data.imei.iccid, &payload[30], 20);
    event.data.imei.iccid[20] = '\0';

    // CSQ [50] (1字节)
    event.data.imei.csq = payload[50];

    // RSRP [51-52] (小端, 有符号)
    event.data.imei.rsrp = (int16_t)(payload[51] | (payload[52] << 8));

    // SNR [53-54] (小端, 有符号)
    event.data.imei.snr = (int16_t)(payload[53] | (payload[54] << 8));

    // ECL [55] (1字节)
    event.data.imei.ecl = payload[55];

    // CellID [56-59] (小端)
    event.data.imei.cell_id = payload[56] | (payload[57] << 8) |
                              (payload[58] << 16) | (payload[59] << 24);

    // 备卡 ICCID2 [60-79] (20字节)
    memcpy(event.data.imei.iccid2, &payload[60], 20);
    event.data.imei.iccid2[20] = '\0';

    // 备卡 IMSI2 [80-94] (15字节)
    memcpy(event.data.imei.imsi2, &payload[80], 15);
    event.data.imei.imsi2[15] = '\0';

    // 备卡 CSQ2 [95] (1字节)
    event.data.imei.csq2 = payload[95];

    // 软件版本编译时间 [96-101] (6字节BCD)
    memcpy(event.data.imei.build_time, &payload[96], 6);

    // 外部压力传感器状态 [102] (1字节)
    event.data.imei.pressure_status = payload[102];

    // 外部压力传感器值 [103-106] (小端, 2位小数)
    event.data.imei.pressure_value = payload[103] | (payload[104] << 8) |
                                     (payload[105] << 16) |
                                     (payload[106] << 24);

    log_i("=== 读取网络参数 ===");
    log_i("主卡 IMEI: %s", event.data.imei.imei);
    log_i("主卡 IMSI: %s", event.data.imei.imsi);
    log_i("主卡 ICCID: %s", event.data.imei.iccid);
    log_i("主卡 CSQ: %d", event.data.imei.csq);
    log_i("信号 RSRP: %d dBm, SNR: %d dB, ECL: %d", event.data.imei.rsrp,
          event.data.imei.snr, event.data.imei.ecl);
    log_i("小区号: %u", event.data.imei.cell_id);
    log_i("备卡 ICCID2: %s", event.data.imei.iccid2);
    log_i("备卡 IMSI2: %s, CSQ2: %d", event.data.imei.imsi2,
          event.data.imei.csq2);
    log_i("编译时间: 20%02X-%02X-%02X %02X:%02X:%02X", payload[96], payload[97],
          payload[98], payload[99], payload[100], payload[101]);
    log_i("压力传感器: 状态=%s, 值=%u.%02u kPa", payload[102] ? "异常" : "正常",
          event.data.imei.pressure_value / 100,
          event.data.imei.pressure_value % 100);

    // 兼容旧接口 (TODO: 后续移除)
    parse_imei_imsi_iccid(payload);
    Test_quanju_canshu_L.time_softdelay_ms = 0;
    test_xieyi_jilu_Rec = w_get_IMEI;

    // 触发事件回调
    if (s_dgm_event_callback) {
      s_dgm_event_callback(&event);
    }
    break;

  case DEV_READ_CHECK_STATUS: // 0x1008 - 读取检测状态 (17字节)
    // 来源: 《民用物联网表整机测试通讯协议》章节4.6.18
    event.type = DGM_EVENT_STAR_MAC_RECEIVED;
    event.data_mark = data_mark;

    // 主电电压 [0-1] (大端, 单位0.01V)
    event.data.star_mac.voltage = (payload[0] << 8) | payload[1];

    // 星闪MAC地址 [2-13] (12字节ASCII)
    memcpy(event.data.star_mac.mac, &payload[2], 12);
    event.data.star_mac.mac[12] = '\0';

    // 星闪连接状态 [14]
    event.data.star_mac.connected = payload[14];

    // 星闪信号强度 [15] (有符号8bit)
    event.data.star_mac.signal = (int8_t)payload[15];

    // 按键状态 [16]
    event.data.star_mac.key_status = payload[16];

    log_i("读取检测状态:");
    log_i("  主电电压: %d.%02dV", event.data.star_mac.voltage / 100,
          event.data.star_mac.voltage % 100);
    log_i("  星闪MAC: %s", event.data.star_mac.mac);
    log_i("  连接状态: %d, 信号强度: %d, 按键: %d", payload[14],
          (int8_t)payload[15], payload[16]);

    // 兼容旧接口 (TODO: 后续移除)
    memcpy(Test_linshi_cunchushuju_L.L_StarMac, &payload[2], 12);
    Test_quanju_canshu_L.time_softdelay_ms = 0;
    test_xieyi_jilu_Rec = w_get_test_zhuanyong;

    // 触发事件回调
    if (s_dgm_event_callback) {
      s_dgm_event_callback(&event);
    }
    break;

  default:
    log_d("未处理的读响应: 0x%04X", data_mark);
    break;
  }
}

/**
 * @brief 处理写响应
 */
static void handle_write_response(const uint8_t *frame, uint16_t data_mark) {
  const uint8_t *payload = &frame[INDEX_VOLUME_DATA];
  DgmProtocolEvent event = {0};

  log_d("处理写响应: 数据标识=0x%04X", data_mark);

  switch (data_mark) {
  case DEV_AutoCheckFinish: // 0x1000 - 自检完成响应
    event.type = DGM_EVENT_SELF_CHECK_COMPLETE;
    event.data_mark = data_mark;
    // 解析数据域信号强度值
    event.data.self_check.signal_strength = payload[0];
    log_i("自检完成时的信号强度CSQ=%d", payload[0]);
    if (s_dgm_event_callback) {
      s_dgm_event_callback(&event);
    }
    break;
  case DEV_BoardInfo: // 0x1001 - 上告/开机信息写响应 (26字节状态信息)
    // 解析26字节状态信息到事件数据
    // 注意: payload[0]是表具类型，不是表号！表号在帧头frame[1-6]
    event.type = DGM_EVENT_POWER_ON_INFO_RECEIVED;
    event.data_mark = data_mark;

    // 解析26字节状态信息
    event.data.board_info.meter_type = payload[0];     // [0]  表具类型
    event.data.board_info.has_addon = payload[1];      // [1]  是否带附件
    event.data.board_info.voltage = payload[2];        // [2]  主控板电压
    event.data.board_info.module_status = payload[3];  // [3]  模块状态
    event.data.board_info.signal = payload[4];         // [4]  信号强度
    event.data.board_info.connect_status = payload[5]; // [5]  连接状态
    event.data.board_info.sim_ok = payload[6];         // [6]  SIM卡状态
    event.data.board_info.storage_ic_ok = payload[7];  // [7]  EEPROM状态
    event.data.board_info.measure_ok = payload[8];     // [8]  计量状态
    event.data.board_info.sw_ver1 = payload[9];        // [9]  软件版本1
    event.data.board_info.sw_ver2 = payload[10];       // [10] 软件版本2
    event.data.board_info.rtc_ok = payload[11];        // [11] RTC状态
    event.data.board_info.temp_press_ok = payload[12]; // [12] 温压状态
    // payload[13-22] 预留10字节，跳过
    event.data.board_info.cover_open = payload[23];   // [23] 开盖状态
    event.data.board_info.tilt_ok = payload[24];      // [24] 倾斜状态
    event.data.board_info.bluetooth_ok = payload[25]; // [25] 蓝牙状态

    event.data.board_info.ir_comm_ok = true;

    log_d(
        "上告开机信息: 类型=%d, 附件=0x%02X, 电压=%d.%dV, 信号=%d, 版本=V%d.%d",
        payload[0], payload[1], payload[2] / 10, payload[2] % 10, payload[4],
        payload[9], payload[10]);

    // 解析开盖检测状态
    if (payload[23] == 0 && Test_jiejuo_jilu.kaigai_jiance == 1) {
      Test_jiejuo_jilu.kaigai_jiance = 1; // 开盖检测通过
      log_d("开盖检测: 通过 (cover_open=%d)", payload[23]);
    } else {
      Test_jiejuo_jilu.kaigai_jiance = 0;
      log_d("开盖检测: 失败 (cover_open=%d)", payload[23]);
    }

    // 兼容旧接口 (TODO: 后续移除)
    parse_connect_result(payload);
    s_check_process = MASTER_CONNCET_CHECK;
    Test_jiejuo_jilu.hongwai_jiance = 1;
    Test_quanju_canshu_L.time_softdelay_ms = 0;
    test_xieyi_jilu_Rec = w_get_connect;

    // 触发事件回调
    if (s_dgm_event_callback) {
      s_dgm_event_callback(&event);
    }
    break;

  case DEV_TIME: // 0xC621 - 时间设置响应
    event.type = DGM_EVENT_TIME_SET_OK;
    event.data_mark = data_mark;
    log_d("时间设置成功");
    if (s_dgm_event_callback) {
      s_dgm_event_callback(&event);
    }
    break;

  case DEV_SETOUTIOSTATUS: // 0x1002 - IO状态检测响应 (7字节)
    // 来源: 《民用物联网表整机测试通讯协议》章节4.6.13
    event.type = DGM_EVENT_IO_STATUS;
    event.data_mark = data_mark;
    event.data.io_status.high_low = s_high_low_flag;
    event.data.io_status.open_pos = payload[0];  // [0] 开到位
    event.data.io_status.close_pos = payload[1]; // [1] 关到位
    event.data.io_status.hall1 = payload[2];     // [2] 霍尔1状态
    event.data.io_status.hall2 = payload[3];     // [3] 霍尔2状态
    event.data.io_status.ic_xb = payload[4];     // [4] IC卡XB脚电平
    event.data.io_status.io_119 = payload[5];    // [5] 119电平
    event.data.io_status.ic_err = payload[6];    // [6] IC卡ERR脚电平

    // 计算检测结果 (可选，实际验证由测试层处理)
    if (s_high_low_flag == 1) {
      // 高电平测试: 期望霍尔1=0, 霍尔2=1
      event.data.io_status.hall_ok = (payload[2] == 0 && payload[3] == 1);
    } else {
      // 低电平测试: 期望霍尔1=1, 霍尔2=0
      event.data.io_status.hall_ok = (payload[2] == 1 && payload[3] == 0);
    }
    event.data.io_status.ic_ok =
        (payload[4] == s_high_low_flag && payload[6] == s_high_low_flag);

    log_d(
        "IO状态(%s): open_pos=%d, close_pos=%d, hall1=%d, hall2=%d, hall_ok=%d",
        s_high_low_flag ? "高" : "低", payload[0], payload[1], payload[2],
        payload[3], event.data.io_status.hall_ok);
    log_d("  IC卡: XB=%d, ERR=%d, IC卡OK=%d", payload[4], payload[6],
          event.data.io_status.ic_ok);

    // 兼容旧接口 (TODO: 后续移除)
    if (s_high_low_flag == 1) {
      s_check_process = MASTER_CHECK_ONE;
    } else {
      s_check_process = MASTER_CHECK_TWO;
    }
    parse_io_status(payload, s_high_low_flag);
    Test_quanju_canshu_L.time_softdelay_ms = 0;
    test_xieyi_jilu_Rec = w_get_IO_status;

    // 触发事件回调
    if (s_dgm_event_callback) {
      s_dgm_event_callback(&event);
    }
    break;

  case DEV_CLOSE_IR: // 0x1005 - 关闭红外响应
    event.type = DGM_EVENT_IR_CLOSED;
    event.data_mark = data_mark;
    log_d("红外关闭成功");

    // 兼容旧接口 (TODO: 后续移除)
    s_check_process = MASTER_IR_CLOSED;
    Test_quanju_canshu_L.time_softdelay_ms = 0;
    test_xieyi_jilu_Rec = w_get_close_IR;

    // 触发事件回调
    if (s_dgm_event_callback) {
      s_dgm_event_callback(&event);
    }
    break;
  case DEV_CONFIG_IO_STATUS: // 0x1007 - 配置IO状态响应
    event.type = DGM_EVENT_IO_CONFIGURED;
    event.data_mark = data_mark;
    log_d("设置端口状态成功(1007响应)");

    // 触发事件回调
    if (s_dgm_event_callback) {
      s_dgm_event_callback(&event);
    }
    break;
  default:
    log_d("未处理的写响应: 0x%04X", data_mark);
    break;
  }
}

/**
 * @brief 处理安装响应
 */
static void handle_install_response(const uint8_t *frame, uint16_t data_mark) {
  const uint8_t *payload = &frame[INDEX_VOLUME_DATA];
  DgmProtocolEvent event = {0};

  log_d("处理安装响应: 数据标识=0x%04X", data_mark);

  switch (data_mark) {
  case DEV_AutoCheckFinish: // 0x1000 - 自检完成 (1字节: 信号强度)
    event.type = DGM_EVENT_SELF_CHECK_COMPLETE;
    event.data_mark = data_mark;

    // 解析信号强度
    event.data.self_check.signal_strength = payload[0];

    log_d("主控板自检完成, 信号强度=%d", payload[0]);

    // 兼容旧接口 (TODO: 后续移除)
    s_check_process = MASTER_SELFCHECK_FINISH;
    Test_quanju_canshu_L.time_softdelay_ms = 0;
    test_xieyi_jilu_Rec = w_get_self_check;

    // 触发事件回调
    if (s_dgm_event_callback) {
      s_dgm_event_callback(&event);
    }
    break;

  default:
    log_d("未处理的安装响应: 0x%04X", data_mark);
    break;
  }
}

/*============ 数据解析辅助函数 ============*/

/**
 * @brief 解析连接结果数据 (0x1001响应, 26字节)
 * 对应PIC的RecvConnectResult函数
 *
 * @note 数据域内容:
 *   [0]  表具类型     [1]  是否带附件   [2]  主控板电压
 *   [3]  模块状态     [4]  信号强度     [5]  连接状态
 *   [6]  SIM卡状态    [7]  EEPROM状态   [8]  计量状态
 *   [9]  软件版本1    [10] 软件版本2    [11] RTC状态
 *   [12] 温压状态     [13-22] 预留10字节
 *   [23] 开盖状态     [24] 倾斜状态     [25] 蓝牙状态
 */
static void parse_connect_result(const uint8_t *data) {
  // 兼容旧接口，暂时保留但不做处理
  (void)data;
}

/**
 * @brief 解析IMEI/IMSI/ICCID
 * 对应PIC的FindIMEIandIMSIandICCID_where函数
 */
static void parse_imei_imsi_iccid(const uint8_t *data) {
  // IMEI (15字节)
  memcpy(Test_jiejuo_jilu.IMEI_CHK, &data[0], 15);
  log_d("IMEI: %.15s", Test_jiejuo_jilu.IMEI_CHK);

  // IMSI (15字节)
  memcpy(Test_jiejuo_jilu.IMSI_CHK, &data[15], 15);
  log_d("IMSI: %.15s", Test_jiejuo_jilu.IMSI_CHK);

  // ICCID (20字节)
  memcpy(Test_jiejuo_jilu.ICCID_CHK, &data[30], 20);
  log_d("ICCID: %.20s", Test_jiejuo_jilu.ICCID_CHK);
}

// 注: parse_star_mac 函数已移除，0x1008解析现在直接内联处理

/**
 * @brief 解析IO状态
 * 对应PIC的GetMasterInPutIO_Status函数
 */
static void parse_io_status(const uint8_t *data, uint8_t high_low) {
  uint8_t pos = 0;

  // 到位信号1
  uint8_t daowei1 = data[pos++];
  // 到位信号2
  uint8_t daowei2 = data[pos++];

  // 霍尔1
  uint8_t hall1 = data[pos++];
  // 霍尔2
  uint8_t hall2 = data[pos++];

  // IC卡XB
  uint8_t ic_xb = data[pos++];
  // 119状态
  uint8_t io_119 = data[pos++];
  // IC卡ERR
  uint8_t ic_err = data[pos++];

  log_d("IO状态: 到位1=%d, 到位2=%d, 霍尔1=%d, 霍尔2=%d", daowei1, daowei2,
        hall1, hall2);

  // 检测高电平时
  if (high_low == 1) {
    if (hall1 != 0) {
      Test_jiejuo_jilu.jiliang_jiance = 0; // 霍尔1检测失败
      log_d("霍尔1高电平检测失败");
    }
    if (hall2 != 1) {
      Test_jiejuo_jilu.jiliang_jiance = 0; // 霍尔2检测失败
      log_d("霍尔2高电平检测失败");
    }
  }
  // 检测低电平时
  else {
    if (hall1 != 1) {
      Test_jiejuo_jilu.jiliang_jiance = 0;
      log_d("霍尔1低电平检测失败");
    }
    if (hall2 != 0) {
      Test_jiejuo_jilu.jiliang_jiance = 0;
      log_d("霍尔2低电平检测失败");
    }
  }

  // IC卡检测
  if (ic_xb != high_low || ic_err != high_low) {
    // IC卡检测失败
    log_d("IC卡检测失败");
  }

  // 119检测
  if (high_low == 1) {
    if (io_119 == 0) {
      // 高电平时119为低，等待低电平测试
    } else {
      Test_jiejuo_jilu.qiangci_jiance = 0; // 119检测失败
    }
  } else {
    if (io_119 == 1) {
      Test_jiejuo_jilu.qiangci_jiance = 1; // 119检测通过
    } else {
      Test_jiejuo_jilu.qiangci_jiance = 0;
    }
  }
}

/*============ 命令发送实现 ============*/

/**
 * @brief 构建命令帧
 *
 * @param buf 输出缓冲区
 * @param ctrl_code 控制码
 * @param data_mark 数据标识
 * @param data 数据域内容
 * @param data_len 数据域长度
 * @return 帧总长度
 */
static uint16_t build_cmd_frame(uint8_t *buf, uint8_t ctrl_code,
                                uint16_t data_mark, const uint8_t *data,
                                uint16_t data_len) {
  // NULL指针检查
  if (buf == NULL) {
    log_e("build_cmd_frame: buf为空");
    return 0;
  }

  uint16_t pos = 0;

  // 帧头1
  buf[pos++] = FRAME_HEAD_68;

  // 表号 (6字节)
  memcpy(&buf[pos], s_meter_number, 6);
  pos += 6;

  // 帧头2
  buf[pos++] = FRAME_HEAD_68;

  // 控制码
  buf[pos++] = ctrl_code;

  // 数据域长度 (2字节小端) - 先占位
  uint16_t len_pos = pos;
  pos += 2;

  // 时间 (6字节)
  memcpy(&buf[pos], s_rtc_time, 6);
  pos += 6;

  // 设备类型
  buf[pos++] = DEVICE_TYPE;

  // 数据标识 (2字节小端)
  WRITE_LE_U16(&buf[pos], data_mark);
  pos += 2;

  // 帧序号
  buf[pos++] = 0;

  // 数据域
  if (data != NULL && data_len > 0) {
    memcpy(&buf[pos], data, data_len);
    pos += data_len;
  }

  // 填充数据域长度
  uint16_t data_field_len = pos - 11;
  WRITE_LE_U16(&buf[len_pos], data_field_len);

  // 校验和
  buf[pos] = util_checksum_sum8(buf, pos);
  pos++;

  // 帧尾
  buf[pos++] = FRAME_TAIL_16;

  return pos;
}

/**
 * @brief 发送读命令
 */
static bool send_read_cmd(uint16_t data_mark) {
  if (s_send_func == NULL) {
    log_e("发送函数未设置");
    return false;
  }

  uint16_t frame_len =
      build_cmd_frame(s_tx_buffer, OPT_READ, data_mark, NULL, 0);

  log_d("发送读命令: 数据标识=0x%04X, 长度=%d", data_mark, frame_len);
  elog_hexdump("DGM_TX", 8, s_tx_buffer, frame_len);

  s_send_func(s_tx_buffer, frame_len);
  return true;
}

/**
 * @brief 发送写命令
 */
static bool send_write_cmd(uint16_t data_mark, const uint8_t *data,
                           uint16_t data_len) {
  if (s_send_func == NULL) {
    log_e("发送函数未设置");
    return false;
  }

  // 特殊处理: DEV_BoardInfo需要添加连接数据
  uint8_t connect_data[16] = {0};
  if (data_mark == DEV_BoardInfo && (data == NULL || data_len == 0)) {
    // 填充连接请求数据 (根据PIC的SetConnectMasterBoard函数)
    connect_data[0] = 0x01; // 连接标志
    data = connect_data;
    data_len = 1;
  }

  // 特殊处理: DEV_SETOUTIOSTATUS需要记录高低电平标志
  if (data_mark == DEV_SETOUTIOSTATUS && data != NULL && data_len > 0) {
    s_high_low_flag = data[0];
  }

  uint16_t frame_len =
      build_cmd_frame(s_tx_buffer, OPT_WRITE, data_mark, data, data_len);

  log_d("发送写命令: 数据标识=0x%04X, 长度=%d", data_mark, frame_len);
  elog_hexdump("DGM_TX", 8, s_tx_buffer, frame_len);

  s_send_func(s_tx_buffer, frame_len);
  return true;
}

/*============ 公共API ============*/

/**
 * @brief 发送上告/开机信息获取命令 (0x1001)
 * @note 控制码为 0x04 (写操作)
 */
void DGM_SendBoardInfoRequest(void) { send_write_cmd(DEV_BoardInfo, NULL, 0); }

/**
 * @brief 发送IO状态检测命令
 */
void DGM_SendIOStatusCheck(uint8_t data, uint8_t length) {
  send_write_cmd(DEV_SETOUTIOSTATUS, &data, length);
}

// 1002设置IO输出状态 - 控制码定义
// 0x01 阀门接口, 0x02 蜂鸣器, 0x03 IC卡SDA, 0x04 IC卡SCL,
// 0x05 IC卡RST, 0x06 IC卡POW, 0x07 光电信号, 0x08 光电电源,
// 0x09 阀门接口2（商膜双阀检测使用）
// 控制状态: 0x00 低电平, 0x01 高电平, 阀门接口: 0 开阀, 1 关阀, 2 停止阀门动作

/**
 * @brief 发送设置IO输出状态命令 (1002) - 单个控制
 * @param function 控制码 (0x01~0x09)
 * @param io_status 控制状态 (0x00/0x01/0x02)
 */
void DGM_SendSetOutputIOStatus(uint8_t funtion_number, uint8_t function,
                               uint8_t io_status) {
  // 数据格式: [控制码数量][控制码1][控制状态1]...
  uint8_t data[3];
  data[0] = funtion_number; // 控制码数量 (单个控制时为1)
  data[1] = function;       // 控制码
  data[2] = io_status;      // 控制状态

  send_write_cmd(DEV_SETOUTIOSTATUS, data, 1 + funtion_number * 2);
}

/**
 * @brief 发送设置IO输出状态命令 (1002) - 多个控制
 * @param count 控制数量
 * @param controls 控制数组，每个元素包含 [控制码, 控制状态]
 * @note 示例: uint8_t ctrl[][2] = {{0x01, 0x00}, {0x02, 0x01}};
 *       DGM_SendSetOutputIOStatusMulti(2, ctrl);
 */
void DGM_SendSetOutputIOStatusMulti(uint8_t count, uint8_t controls[][2]) {
  if (count == 0 || count > 10)
    return;

  // 数据格式: [控制码数量][控制码1][控制状态1][控制码2][控制状态2]...
  uint8_t data[21]; // 最多支持10个控制: 1 + 10*2 = 21
  data[0] = count;

  for (uint8_t i = 0; i < count; i++) {
    data[1 + i * 2] = controls[i][0];     // 控制码
    data[1 + i * 2 + 1] = controls[i][1]; // 控制状态
  }

  send_write_cmd(DEV_SETOUTIOSTATUS, data, 1 + count * 2);
}

// 1002配置阀门,只配置阀门阀门
void DGM_SendConfigureValve(uint8_t valve_status) {
  // 数据格式: [控制码数量][控制码1][控制状态1]
  uint8_t data[3];
  data[0] = 1;            // 控制码数量 (单个控制时为1)
  data[1] = 0x01;         // 控制码: 阀门接口
  data[2] = valve_status; // 控制状态: 0 开阀, 1 关阀, 2 停止阀门动作

  send_write_cmd(DEV_SETOUTIOSTATUS, data, sizeof(data));
}

/**
 * @brief
 * 发送膜表1007协议，配置IO状态命令，这个当前可能无法使用，是昆仑流量计专用协议
 *
 * @param data 配置数据 (4字节)
 * @param len 数据长度 (应为4)
 */
static void DGM_SendConfigureIOStatus(uint8_t data[4], uint16_t len) {
  // 发送膜表1007协议，配置IO状态命令，进入低功耗模式
  // 数据域：
  // 通讯电池欠压报警状态,0 低电平 1 高电平 2 不设置
  // 阀门状态，0设置为关 1设置为开 2 不设置
  // 是否进入低功耗，0 不进入低功耗 1 进入低功耗
  // 是否复位,0 不复位   1复位

  send_write_cmd(DEV_CONFIG_IO_STATUS, data, len);
}

/**
 * @brief 发送进入低功耗指令,1007协议，这个当前只有昆仑流量计使用，膜表无法使用
 *
 */
void DGM_SendEnterLowPower(void) {
  uint8_t data[4] = {0x02, 0x02, 0x01, 0x00}; // 设置进入低功耗模式
  DGM_SendConfigureIOStatus(data, sizeof(data));
}

void DGM_SendCloseIR(void) {
  // 0 关闭红外 1 开启红外
  uint8_t data = 0;
  send_write_cmd(DEV_CLOSE_IR, &data, 1);
}

/**
 * @brief 发送读取IMEI/IMSI/ICCID命令
 */
void DGM_SendReadIMEI(void) { send_read_cmd(DEV_read_IMEI_IMSI_ICCID); }

/**
 * @brief 发送读取星闪MAC命令
 */
void DGM_SendReadStarMac(void) { send_read_cmd(DEV_READ_CHECK_STATUS); }

/**
 * @brief 获取当前检测过程状态
 */
uint8_t DGM_GetCheckProcess(void) { return s_check_process; }

/**
 * @brief 设置当前检测过程状态
 */
void DGM_SetCheckProcess(uint8_t process) { s_check_process = process; }

/**
 * @brief 设置表号
 */
void DGM_SetMeterNumber(const uint8_t *meter_no) {
  memcpy(s_meter_number, meter_no, 6);
}

/**
 * @brief 设置RTC时间
 */
void DGM_SetRtcTime(const uint8_t *rtc_time) {
  memcpy(s_rtc_time, rtc_time, 6);
}

/**
 * @brief 设置膜式燃气表协议事件回调
 * @param callback 回调函数
 */
void DGM_SetEventCallback(DgmEventCallback callback) {
  s_dgm_event_callback = callback;
}

/**
 * @brief 获取事件类型名称 (用于调试日志)
 * @param type 事件类型
 * @return 事件名称字符串
 */
const char *DGM_GetEventName(DgmEventType type) {
  static const char *event_names[] = {
      [DGM_EVENT_NONE] = "NONE",
      [DGM_EVENT_SELF_CHECK_COMPLETE] = "自检完成",
      [DGM_EVENT_POWER_ON_INFO_RECEIVED] = "上告开机信息获取",
      [DGM_EVENT_CONNECT_FAILED] = "连接失败",
      [DGM_EVENT_IO_STATUS] = "IO状态检测完成",
      [DGM_EVENT_IMEI_RECEIVED] = "收到IMEI",
      [DGM_EVENT_STAR_MAC_RECEIVED] = "收到星闪MAC",
      [DGM_EVENT_IR_CLOSED] = "红外已关闭",
      [DGM_EVENT_TIME_SET_OK] = "时间设置成功",
      [DGM_EVENT_PARSE_ERROR] = "解析错误",
      [DGM_EVENT_CHECKSUM_ERROR] = "校验和错误",
      [DGM_EVENT_TIMEOUT] = "超时",
  };

  if (type < DGM_EVENT_MAX) {
    return event_names[type];
  }
  return "未知事件";
}
