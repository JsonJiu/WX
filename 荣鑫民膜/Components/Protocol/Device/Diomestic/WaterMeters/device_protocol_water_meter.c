/**
 * @file device_protocol_water_meter.c
 * @brief 三川水表协议实现
 * @version 1.0.0
 * @date 2024-12
 *
 * @section intro 简介
 * 实现与三川水表的通信协议。
 * 协议格式: 68 ADDR(6) TYPE VER CTRL LEN(2) DI(2) DATA... CRC(2) 16
 */

#define LOG_TAG "wm_proto"

#include "device_protocol.h"
#include "utility.h"
#include <elog.h>
#include <stdio.h>
#include <string.h>

// 外部依赖
#include "Test_List.h"
// 这些变量已在 Test_List.h 中声明为 extern，这里不需要重复声明
// extern struct Test_jieguo Test_jiejuo_jilu;
// extern struct Test_linshi_cunchushuju Test_linshi_cunchushuju_L;
// extern struct Test_quanju_canshu Test_quanju_canshu_L;
// extern enum Test_liucheng Test_liucheng_L;
// extern enum test_xieyi_jilu test_xieyi_jilu_Rec;
extern uint8_t water_meter_type;
extern void protocol_debug_print(const uint8_t *data, uint16_t len);
extern void hexdump(const uint8_t *data, uint16_t len);

/*============ 内部变量 ============*/

static ProtocolSendFunc s_send_func = NULL;
static ProtocolEventCallback s_event_callback = NULL;

// 协议版本
#define WM_PROTOCOL_VERSION 0x0A // 当前使用新版协议

// 发送缓冲区
#define WM_TX_BUF_SIZE 256
static uint8_t s_tx_buffer[WM_TX_BUF_SIZE];

// 默认表号
static uint8_t s_default_meter_no[6] = {0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA};

/*============ 前导码配置 ============*/

// 主前导码 (0xAA x 50)
static const uint8_t s_preamble_data[50] = {
    0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
    0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
    0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
    0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
    0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
};

// 同步前导码 (0xFE x 10)
static const uint8_t s_sync_preamble[10] = {
    0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE,
};

// 水表前导配置
static const ProtocolPreambleConfig s_water_meter_preamble = {
    .enabled = true,
    .data = s_preamble_data,
    .length = sizeof(s_preamble_data),
    .repeat_count = 32, // 重复32次
    .delay_ms = 3,      // 每次发送后延时3ms
    .sync_data = s_sync_preamble,
    .sync_length = sizeof(s_sync_preamble),
};

/*============ 内部函数声明 ============*/

static bool wm_init(void);
static ProtocolResult wm_parse(uint8_t *data, uint16_t len);
static bool wm_send_cmd(uint16_t cmd, void *param);
static void wm_on_response(uint16_t code, const uint8_t *data, uint16_t len);
static void wm_set_send_func(ProtocolSendFunc func);
static void wm_set_event_callback(ProtocolEventCallback callback);

// 响应处理函数
static void handle_read_response(const uint8_t *data, uint16_t cmd_code,
                                 uint16_t data_len);
static void handle_write_response(const uint8_t *data, uint16_t cmd_code);
static void handle_ctrl_response(const uint8_t *data, uint16_t cmd_code);

// 命令发送函数 (cmd_code 即数据标识 DI)
static bool send_read_cmd(uint16_t cmd_code, const uint8_t *meter_no);
static bool send_write_cmd(uint16_t cmd_code, const uint8_t *meter_no,
                           const uint8_t *data, uint16_t data_len);
static bool send_ctrl_cmd(uint16_t cmd_code, const uint8_t *meter_no,
                          const uint8_t *data, uint16_t data_len);

/*============ 协议接口实例 ============*/

const ProtocolInterface water_meter_protocol = {
    .name = "water_meter",
    .init = wm_init,
    .parse = wm_parse,
    .send_cmd = wm_send_cmd,
    .on_response = wm_on_response,
    .set_send_func = wm_set_send_func,
    .set_event_callback = wm_set_event_callback,
    .preamble = &s_water_meter_preamble, // 水表需要前导唤醒
};

/*============ 接口实现 ============*/

static bool wm_init(void) {
  log_i("水表协议初始化");
  return true;
}

static ProtocolResult wm_parse(uint8_t *data, uint16_t len) {
  uint16_t pos = 0;
  uint16_t frame_len = 0;
  uint16_t crc_recv = 0;
  uint16_t crc_calc = 0;
  uint16_t cmd_code = 0; // 命令码/数据标识

  log_d("水表协议开始解析, 长度=%d", len);
  // protocol_debug_print(data, len);
  // 使用easylogger系统的函数
  elog_hexdump("WM_RX", 8, data, len);

  while (pos + 17 < len) {
    // 查找帧头
    if (data[pos] != FRAME_HEAD_68) {
      pos++;
      continue;
    }

    // 获取帧长度 (小端模式)
    frame_len = READ_LE_U16(&data[pos + 10]);

    // 验证长度
    if (pos + frame_len > len) {
      log_d("帧不完整");
      return PROTOCOL_RESULT_INCOMPLETE;
    }

    // 验证帧尾
    if (data[pos + frame_len - 1] != FRAME_TAIL_16) {
      pos++;
      continue;
    }

    // 验证CRC
    crc_recv = READ_LE_U16(&data[pos + frame_len - 3]); // 收到的协议里面的CRC
    crc_calc = util_crc16_ccitt(&data[pos], frame_len - 3); // 计算得到的CRC

    if (crc_recv != crc_calc) {
      log_e("CRC错误: 收到=0x%04X, 计算=0x%04X", crc_recv, crc_calc);
      pos++;
      continue;
    }

    // 保存表号
    memcpy(Test_jiejuo_jilu.dangqian_biaohao, &data[pos + 1], 6);

    // 获取控制码和命令码
    uint8_t ctrl = data[pos + 9];
    cmd_code = READ_LE_U16(&data[pos + 12]);

    log_d("收到水表响应: 控制码=0x%02X, 命令码=0x%04X", ctrl, cmd_code);

    // 根据控制码分发处理
    switch (ctrl) {
    case WM_CTRL_READ: // 0x81 - 读响应
      handle_read_response(&data[pos], cmd_code, frame_len - 17);
      break;

    case WM_CTRL_WRITE: // 0x82 - 写响应
      handle_write_response(&data[pos], cmd_code);
      break;

    case WM_CTRL_CTRL: // 0x83 - 控制响应
      handle_ctrl_response(&data[pos], cmd_code);
      break;

    default:
      log_w("未知控制码: 0x%02X", ctrl);
      break;
    }

    pos += frame_len;
  }

  return PROTOCOL_RESULT_OK;
}

static bool wm_send_cmd(uint16_t cmd, void *param) {
  // 命令码高4位表示操作类型，低12位是具体命令码
  uint8_t op_type = (cmd >> 12) & 0x0F;
  uint16_t cmd_code = cmd & 0x0FFF;

  // 使用当前表号或默认表号
  const uint8_t *meter_no = Test_jiejuo_jilu.dangqian_biaohao;
  if (meter_no[0] == 0 && meter_no[1] == 0) {
    meter_no = s_default_meter_no;
  }

  switch (op_type) {
  case 0x1: // 读命令
    return send_read_cmd(cmd_code, meter_no);

  case 0x2: // 写命令
    return send_write_cmd(cmd_code, meter_no, (uint8_t *)param, 0);

  case 0x3: // 控制命令
    return send_ctrl_cmd(cmd_code, meter_no, (uint8_t *)param, 0);

  default:
    // 直接使用cmd作为命令码发送读命令
    return send_read_cmd(cmd, meter_no);
  }
}

static void wm_on_response(uint16_t code, const uint8_t *data, uint16_t len) {
  log_d("水表协议响应通知: 0x%04X", code);
}

static void wm_set_send_func(ProtocolSendFunc func) { s_send_func = func; }

static void wm_set_event_callback(ProtocolEventCallback callback) {
  s_event_callback = callback;
}

/*============ 响应处理实现 ============*/
/**
 * @brief 解析读取的协议，并根据协议设置响应
 *
 * @param frame     完整协议帧数据
 * @param cmd_code  命令码/数据标识 (如 0x2031=表号, 0xF003=综合查询)
 * @param data_len  数据域长度
 */
static void handle_read_response(const uint8_t *frame, uint16_t cmd_code,
                                 uint16_t data_len) {
  const uint8_t *payload = &frame[14]; // 数据起始位置 (跳过帧头+表号+控制字等)

  log_d("处理读响应: 命令码=0x%04X, 数据长度=%d", cmd_code, data_len);

  switch (cmd_code) {
  case 0x2031: // 表号
    Test_quanju_canshu_L.time_softdelay_ms = 0;
    test_xieyi_jilu_Rec = w_get_biaohao;
    log_d("获取表号成功");
    break;

  case 0xF003: // 综合查询响应
    // Flash检测结果
    Test_linshi_cunchushuju_L.L_flash_chk = payload[3];

    // 主电电压 (小端)
    Test_linshi_cunchushuju_L.L_zhudian_dianya = READ_LE_U16(&payload[4]);

    // 备电电压
    Test_linshi_cunchushuju_L.L_beidian_dianya = READ_LE_U16(&payload[6]);

    // 压力检测
    Test_linshi_cunchushuju_L.L_yali_CHK = payload[8];

    // EEPROM状态
    Test_linshi_cunchushuju_L.L_EEPROM_CHK = payload[9];

    // 霍尔1状态
    Test_linshi_cunchushuju_L.L_huoer1_CHK = payload[10];

    // GPRS模组状态
    Test_linshi_cunchushuju_L.L_GPRS_mozu_CHK = payload[11];

    // 霍尔2状态
    Test_linshi_cunchushuju_L.L_huoer2_CHK = payload[12];

    // 超声波瞬时流量 (4字节)
    memcpy(Test_linshi_cunchushuju_L.L_chaosheng_shunshiliuliang, &payload[13],
           4);

    // 走水状态
    Test_linshi_cunchushuju_L.L_chaosheng_zousuizhuangtai = payload[17];

    // GP30电压
    Test_linshi_cunchushuju_L.L_GP30mozu_AD_CHK = READ_LE_U16(&payload[18]);

    // IMEI (15字节)
    memcpy(Test_linshi_cunchushuju_L.L_IMEI, &payload[20], 15);

    // IMSI (15字节)
    memcpy(Test_linshi_cunchushuju_L.L_IMSI, &payload[35], 15);

    // ICCID (20字节)
    memcpy(Test_linshi_cunchushuju_L.L_ICCID, &payload[50], 20);

    // CSQ
    Test_linshi_cunchushuju_L.L_CSQ = payload[70];

    // LoRa Key (16字节)
    memcpy(Test_linshi_cunchushuju_L.lorakey, &payload[71], 16);

    // 到位信号
    Test_linshi_cunchushuju_L.L_daowei1 = payload[87];
    Test_linshi_cunchushuju_L.L_daowei2 = payload[88];

    // 计量霍尔
    Test_linshi_cunchushuju_L.L_jilianghuoer1 = payload[89];
    Test_linshi_cunchushuju_L.L_jilianghuoer2 = payload[90];

    // 无磁计量
    Test_linshi_cunchushuju_L.L_wucijiliang = payload[91];

    // 版本号/校验码
    Test_linshi_cunchushuju_L.L_banbenhao[0] = payload[92];
    Test_linshi_cunchushuju_L.L_banbenhao[1] = payload[93];

    // LoRa RSSI
    Test_linshi_cunchushuju_L.L_Lora_rssi[0] = payload[94];
    Test_linshi_cunchushuju_L.L_Lora_rssi[1] = payload[95];

    // LoRa SNR
    Test_linshi_cunchushuju_L.L_lora_isnr[0] = payload[96];
    Test_linshi_cunchushuju_L.L_lora_isnr[1] = payload[97];

    // 水温
    Test_linshi_cunchushuju_L.L_water_temperature[0] = payload[98];
    Test_linshi_cunchushuju_L.L_water_temperature[1] = payload[99];

    log_d("F003综合查询解析完成");
    Test_quanju_canshu_L.time_softdelay_ms = 0;
    test_xieyi_jilu_Rec = w_get_test_zhuanyong; // F003为测试专用指令
    break;

  case 0x9010: // 累计流量
    if (payload[0] == 0x00) {
      memcpy(Test_linshi_cunchushuju_L.L_ultrasonic_accumulated_flux,
             &payload[1], 4);
      test_xieyi_jilu_Rec = w_get_accumulated_flux;
      log_d("读取累计流量成功");
    }
    break;

  case 0xF001: // 上报结果
    Test_quanju_canshu_L.time_softdelay_ms = 0;
    test_xieyi_jilu_Rec = w_get_shanggao;
    break;

  case 0x2011: // 版本号
    // 解析版本号字符串
    Test_quanju_canshu_L.time_softdelay_ms = 0;
    test_xieyi_jilu_Rec = w_get_banbenhao;
    break;

  default:
    log_d("未处理的读响应: 命令码=0x%04X", cmd_code);
    break;
  }
}

static void handle_write_response(const uint8_t *frame, uint16_t cmd_code) {
  log_d("处理写响应: 命令码=0x%04X", cmd_code);

  switch (cmd_code) {
  case 0x2036: // 超声波表配置
  case 0x2604: // 机械表配置
    Test_quanju_canshu_L.time_softdelay_ms = 0;
    test_xieyi_jilu_Rec = w_set_famen; // 配置写入成功
    log_d("阀门配置写入成功");
    break;

  case 0x9010:                                    // 累计流量重置
    test_xieyi_jilu_Rec = w_set_accumulated_flux; // 累计流量设置成功
    log_d("累计流量重置成功");
    break;

  default:
    log_d("未处理的写响应: 命令码=0x%04X", cmd_code);
    break;
  }
}

static void handle_ctrl_response(const uint8_t *frame, uint16_t cmd_code) {
  log_d("处理控制响应: 命令码=0x%04X", cmd_code);

  switch (cmd_code) {
  case 0xC022: // 阀门控制
    Test_quanju_canshu_L.time_softdelay_ms = 0;
    test_xieyi_jilu_Rec = w_get_famen_dongzuo; // 阀门动作响应
    log_d("阀门控制响应");
    break;

  case 0xC030:                            // 启动上报
    test_xieyi_jilu_Rec = w_get_shanggao; // 上报启动响应
    log_d("上报启动响应");
    break;

  default:
    log_d("未处理的控制响应: 命令码=0x%04X", cmd_code);
    break;
  }
}

/*============ 命令发送实现 ============*/

/**
 * @brief 构建并发送命令帧
 * @param ctrl      控制码 (0x01=读, 0x02=写, 0x03=控制)
 * @param cmd_code  命令码/数据标识 (如 0x2031, 0xF003, 0xC022)
 * @param meter_no  表号 (6字节)
 * @param data      数据域内容
 * @param data_len  数据域长度
 */
static bool build_and_send_frame(uint8_t ctrl, uint16_t cmd_code,
                                 const uint8_t *meter_no, const uint8_t *data,
                                 uint16_t data_len) {
  if (s_send_func == NULL) {
    log_e("发送函数未设置");
    return false;
  }

  uint16_t pos = 0;

  // 帧头
  s_tx_buffer[pos++] = FRAME_HEAD_68;

  // 表号 (6字节)
  memcpy(&s_tx_buffer[pos], meter_no, 6);
  pos += 6;

  // 类型 (固定0x00)
  s_tx_buffer[pos++] = 0x00;

  // 版本
  s_tx_buffer[pos++] = WM_PROTOCOL_VERSION;

  // 控制码
  s_tx_buffer[pos++] = ctrl;

  // 长度占位 (2字节, 小端)
  uint16_t len_pos = pos;
  pos += 2;

  // 数据标识/命令码 (2字节, 小端)
  WRITE_LE_U16(&s_tx_buffer[pos], cmd_code);
  pos += 2;

  // 数据域
  if (data != NULL && data_len > 0) {
    memcpy(&s_tx_buffer[pos], data, data_len);
    pos += data_len;
  }

  // 计算并填充长度 (整帧长度)
  uint16_t frame_len = pos + 3; // 当前位置 + CRC(2) + 帧尾(1)
  WRITE_LE_U16(&s_tx_buffer[len_pos], frame_len);

  // CRC校验
  uint16_t crc = util_crc16_ccitt(s_tx_buffer, pos);
  WRITE_LE_U16(&s_tx_buffer[pos], crc);
  pos += 2;

  // 帧尾
  s_tx_buffer[pos++] = FRAME_TAIL_16;

  // 发送
  log_d("发送水表命令: 控制码=0x%02X, 命令码=0x%04X", ctrl, cmd_code);
  protocol_debug_print(s_tx_buffer, pos);

  if (s_send_func != NULL) {
    s_send_func(s_tx_buffer, pos);
    return true;
  }
  return false;
}

static bool send_read_cmd(uint16_t cmd_code, const uint8_t *meter_no) {
  return build_and_send_frame(WM_CTRL_READ, cmd_code, meter_no, NULL, 0);
}

static bool send_write_cmd(uint16_t cmd_code, const uint8_t *meter_no,
                           const uint8_t *data, uint16_t data_len) {
  return build_and_send_frame(WM_CTRL_WRITE, cmd_code, meter_no, data,
                              data_len);
}

static bool send_ctrl_cmd(uint16_t cmd_code, const uint8_t *meter_no,
                          const uint8_t *data, uint16_t data_len) {
  return build_and_send_frame(WM_CTRL_CTRL, cmd_code, meter_no, data, data_len);
}
