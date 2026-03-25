/**
 * @file pc_protocol_water_meter.c
 * @brief 国内水表MES系统上位机通信协议实现
 * @version 1.1.0
 * @date 2024-12
 *
 * @section intro 简介
 * 实现与MES系统的通信协议（上位机协议）。
 * 协议格式: 68 CMD LEN DATA... CHECKSUM 16
 *
 * @section decoupling 解耦设计
 * - 工位号通过回调函数 PC_Protocol_GetStationId() 获取
 * - 测试结果数据仍需要 Test_List.h（因为数据结构复杂）
 */

#define LOG_TAG "pc_water_meter"

#include "pc_protocol.h"
#include <elog.h>
#include <stdio.h>
#include <string.h>

// 外部依赖 - 这些需要从原有代码中引入
extern void test_start(void);
extern void protocol_debug_print(const uint8_t *data, uint16_t len);
extern uint8_t Debug_Mode;
extern uint8_t PassThrough_Mode;

// 测试结果结构体 - 需要从Test_List.h引入
#include "Test_List.h"
// Test_jiejuo_jilu 已在 Test_List.h 中声明为 extern
extern uint8_t water_meter_type;
extern uint8_t test_famen_type;

/*============ 内部变量 ============*/

static ProtocolSendFunc s_send_func = NULL;
static ProtocolEventCallback s_event_callback = NULL;

// 发送缓冲区
#define PC_TX_BUF_SIZE 256
static uint8_t s_tx_buffer[PC_TX_BUF_SIZE];

/*============ 协议帧结构 ============*/

// 开始测试命令结构 (接收)
// 协议格式参考: 68 AA 19 05 05 02 50 00 00 77 01 02 00 0F 00 01 01 00 00 00 14
// E6 00 0C 16
#pragma pack(1)
typedef struct {
  uint8_t head;            // [0] 0x68 帧头
  uint8_t cmd;             // [1] 0xAA 命令
  uint8_t length;          // [2] 帧长度
  uint8_t station_id;      // [3] 工位号
  uint8_t meter_number[6]; // [4-9] 表号/模块号 (6字节)
  uint8_t meter_type;      // [10] 表类型: 0=机械表, 1=超声波表
  // 超声波参数 [11-16]
  uint8_t us_meter_type; // [11] 表计类型 (一代管/二代管)
  uint8_t us_transducer; // [12] 换能器类型
  uint16_t us_pipe_type; // [13-14] 管径 (小端, DN15=0x000F)
  uint8_t us_valve_type; // [15] 阀门类型: 0=无阀, 1=5线阀, 2=2线阀
  uint8_t module_type;   // [16] 模组类型: 0=NB, 1=CAT.1, 2=LoRa
  // 机械表参数 [17-22]
  uint16_t mech_pipe_mm;   // [17-18] 机械表管径 (小端)
  uint8_t mech_valve_type; // [19] 机械表阀门类型
  uint8_t mech_timeout;    // [20] 阀门超时时间(秒)
  uint16_t mech_stall_ma;  // [21-22] 堵转电流(mA, 小端)
  uint8_t checksum;        // [23] 校验和
  uint8_t tail;            // [24] 0x16 帧尾
} StartTestFrame;

// 静态断言: 确保结构体大小正确 (25字节)
_Static_assert(sizeof(StartTestFrame) == 25,
               "StartTestFrame size must be 25 bytes!");

// 简单响应帧结构
typedef struct {
  uint8_t head;
  uint8_t cmd;
  uint8_t length;
  uint8_t station_id;
  uint8_t checksum;
  uint8_t tail;
} SimpleResponseFrame;

// 注: ConfigFrame 已移至 pc_protocol_config.c
#pragma pack()

/*============ 内部函数声明 ============*/

static bool mes_init(void);
static ProtocolResult mes_parse(uint8_t *data, uint16_t len);
static bool mes_send_cmd(uint16_t cmd, void *param);
static void mes_on_response(uint16_t code, const uint8_t *data, uint16_t len);
static void mes_set_send_func(ProtocolSendFunc func);
static void mes_set_event_callback(ProtocolEventCallback callback);

// 命令处理函数
static void handle_start_test(const uint8_t *data, uint16_t len);
static void handle_query_result(const uint8_t *data, uint16_t len);
// 注: 0xAE (设置配置) 和 0xBE (查询步骤) 已移至 pc_protocol_config.c

// 响应发送函数
static void send_start_test_ack(void);
static void send_test_result(void);

/*============ 协议接口实例 ============*/

const ProtocolInterface water_meter_pc_protocol = {
    .name = "water_meter",
    .init = mes_init,
    .parse = mes_parse,
    .send_cmd = mes_send_cmd,
    .on_response = mes_on_response,
    .set_send_func = mes_set_send_func,
    .set_event_callback = mes_set_event_callback,
};

/*============ 接口实现 ============*/

static bool mes_init(void) {
  log_i("国内水表MES协议初始化");
  return true;
}

static ProtocolResult mes_parse(uint8_t *data, uint16_t len) {
  uint16_t pos = 0;
  uint8_t checksum = 0;
  uint16_t frame_len = 0;
  bool handled = false; // 标记是否处理了有效命令

  log_d("PC协议开始解析, 长度=%d", len);

  while (pos + 5 < len) {
    // 查找帧头
    if (data[pos] != FRAME_HEAD_68) {
      pos++;
      continue;
    }

    uint8_t cmd = data[pos + 1];
    frame_len = data[pos + 2];

    // 验证长度
    if (pos + frame_len > len) {
      log_d("帧不完整, 需要%d字节", frame_len);
      return PROTOCOL_RESULT_INCOMPLETE;
    }

    // 验证帧尾
    if (data[pos + frame_len - 1] != FRAME_TAIL_16) {
      log_d("帧尾错误");
      pos++;
      continue;
    }

    // 处理不同命令
    switch (cmd) {
    case PC_CMD_START_TEST: // 0xAA
      log_d("收到开始测试命令");
      handle_start_test(&data[pos], frame_len);
      handled = true;
      break;

    case PC_CMD_QUERY_RESULT: // 0xAC
      log_d("收到查询结果命令");
      handle_query_result(&data[pos], frame_len);
      handled = true;
      break;

      // 注意: 0xAE (设置配置) 和 0xBE (查询步骤) 命令已移至公共配置协议
      // (pc_protocol_config.c)

    default:
      // 未知命令，不认领此帧，让其他协议处理
      log_d("非MES命令: 0x%02X, 跳过让其他协议处理", cmd);
      return PROTOCOL_RESULT_UNKNOWN_CMD;
    }

    pos += frame_len;
  }

  return handled ? PROTOCOL_RESULT_OK : PROTOCOL_RESULT_UNKNOWN_CMD;
}

static bool mes_send_cmd(uint16_t cmd, void *param) {
  switch (cmd) {
  case PC_CMD_START_TEST_ACK:
    send_start_test_ack();
    return true;

  case PC_CMD_RESULT_RESPONSE:
    send_test_result();
    return true;

    // 注: PC_CMD_SET_CONFIG_ACK 已移至 pc_protocol_config.c

  default:
    log_w("未支持的发送命令: 0x%04X", cmd);
    return false;
  }
}

static void mes_on_response(uint16_t code, const uint8_t *data, uint16_t len) {
  log_d("PC协议收到响应: 0x%04X", code);
}

static void mes_set_send_func(ProtocolSendFunc func) { s_send_func = func; }

static void mes_set_event_callback(ProtocolEventCallback callback) {
  s_event_callback = callback;
}

/*============ 辅助函数 ============*/

/**
 * @brief 获取阀门类型名称
 */
static const char *get_valve_type_name(uint8_t valve_type) {
  switch (valve_type) {
  case 0:
    return "无阀";
  case 1:
    return "5线阀";
  case 2:
    return "2线阀";
  default:
    return "未知";
  }
}

/**
 * @brief 获取模块类型名称
 */
static const char *get_module_type_name(uint8_t module_type) {
  switch (module_type) {
  case 0:
    return "NB-IoT";
  case 1:
    return "CAT.1";
  case 2:
    return "LoRa";
  case 3:
    return "WiFi";
  default:
    return "未知";
  }
}

/**
 * @brief 获取表计类型名称 (超声波)
 */
static const char *get_us_meter_type_name(uint8_t type) {
  switch (type) {
  case 0:
    return "一代管";
  case 1:
    return "二代管";
  case 2:
    return "三代管";
  default:
    return "未知";
  }
}

/**
 * @brief 打印开始测试配置摘要
 * @note 根据表类型打印对应的配置信息，格式美观易读
 */
static void print_test_config(const StartTestFrame *frame) {
  // 调试: 打印原始字节以验证解析正确性
  const uint8_t *raw = (const uint8_t *)frame;
  log_d("原始帧数据: %02X %02X %02X [%02X] [%02X %02X %02X %02X %02X %02X] "
        "[%02X] ...",
        raw[0], raw[1], raw[2], raw[3],                 // 头+cmd+len+工位
        raw[4], raw[5], raw[6], raw[7], raw[8], raw[9], // 表号
        raw[10]);                                       // 表类型

  // 验证结构体字段偏移
  log_d("结构体解析: station_id=%d(offset=%d), meter_type=%d(offset=%d)",
        frame->station_id, (int)((uint8_t *)&frame->station_id - raw),
        frame->meter_type, (int)((uint8_t *)&frame->meter_type - raw));
  log_d("表号字段: offset=%d, 值=%02X%02X%02X%02X%02X%02X",
        (int)((uint8_t *)frame->meter_number - raw), frame->meter_number[0],
        frame->meter_number[1], frame->meter_number[2], frame->meter_number[3],
        frame->meter_number[4], frame->meter_number[5]);

  // 分隔线
  log_i("+======================================================+");
  log_i("|             MES 开始测试配置                         |");
  log_i("+======================================================+");

  // 基本信息 - 直接从原始字节读取
  log_i("| 工位号: %d", raw[3]);
  log_i("| 表  号: %02X%02X%02X%02X%02X%02X", raw[4], raw[5], raw[6], raw[7],
        raw[8], raw[9]);

  log_i("+------------------------------------------------------+");

  // 直接从原始字节解析，避免结构体对齐问题
  uint8_t meter_type = raw[10];                      // 表类型
  uint8_t us_meter_type = raw[11];                   // 表计类型
  uint8_t us_transducer = raw[12];                   // 换能器
  uint16_t us_pipe_type = raw[13] | (raw[14] << 8);  // 管径(小端)
  uint8_t us_valve_type = raw[15];                   // 阀门类型
  uint8_t module_type = raw[16];                     // 模组类型
  uint16_t mech_pipe_mm = raw[17] | (raw[18] << 8);  // 机械表管径
  uint8_t mech_valve_type = raw[19];                 // 机械表阀门
  uint8_t mech_timeout = raw[20];                    // 超时
  uint16_t mech_stall_ma = raw[21] | (raw[22] << 8); // 堵转电流

  // 表具类型
  if (meter_type == 0) {
    // 机械表配置
    log_i("| [表具类型] 机械表");
    log_i("+------------------------------------------------------+");
    log_i("| [机械表参数]");
    log_i("|   管径: DN%u mm", mech_pipe_mm);
    log_i("|   阀门: %s", get_valve_type_name(mech_valve_type));
    if (mech_valve_type != 0) {
      log_i("|   阀门超时: %u 秒", mech_timeout);
      log_i("|   堵转电流: %u mA", mech_stall_ma);
    }
  } else {
    // 超声波表配置
    log_i("| [表具类型] 超声波表");
    log_i("+------------------------------------------------------+");
    log_i("| [超声波表参数]");
    log_i("|   表计类型: %s", get_us_meter_type_name(us_meter_type));
    log_i("|   换能器: %u", us_transducer);
    log_i("|   管径: DN%u mm", us_pipe_type);
    log_i("|   阀门: %s", get_valve_type_name(us_valve_type));
    log_i("|   模块: %s", get_module_type_name(module_type));
  }

  log_i("+======================================================+");

  // 测试项汇总 - 显示所有可配置项
  log_i("| [测试项目配置]");
  uint8_t current_valve_type =
      (meter_type == 0) ? mech_valve_type : us_valve_type;
  uint8_t is_lora = (module_type == 2); // LoRa模块

  // 电压相关
  log_i("|   主电电压(供电):    [Y] 测试");
  log_i("|   主电电压(协议):    [Y] 测试");
  log_i("|   备电NB扩频电压:    [N] 不测");
  log_i("|   GP30电压:          [N] 不测");

  // 功耗相关
  log_i("|   静态功耗:          [Y] 测试");
  log_i("|   满水功耗:          [Y] 测试");
  log_i("|   走水功耗:          [Y] 测试");
  log_i("|   备电NB扩频功耗:    [N] 不测");

  // 信号相关
  log_i("|   信号强度(CSQ):     [Y] 测试");
  log_i("|   无磁模组信号:      [N] 不测");
  log_i("|   LoRa信号强度:      %s", is_lora ? "[Y] 测试" : "[N] 不测");
  log_i("|   LoRa信噪比:        %s", is_lora ? "[Y] 测试" : "[N] 不测");

  // 功能检测
  log_i("|   蓝牙检测:          [N] 不测");
  log_i("|   Flash检测:         [Y] 测试");
  log_i("|   EEPROM检测:        [N] 不测");
  log_i("|   计量检测:          [Y] 测试");
  log_i("|   红外通信:          [Y] 测试");
  log_i("|   强磁检测:          [N] 不测");

  // 阀门相关
  log_i("|   阀门测试:          %s",
        current_valve_type != 0 ? "[Y] 测试" : "[N] 不测");
  log_i("|   阀门到位:          %s",
        current_valve_type != 0 ? "[Y] 测试" : "[N] 不测");

  // 其他检测
  log_i("|   唤醒检测:          [N] 不测");
  log_i("|   脉冲输出检测:      [N] 不测");
  log_i("|   中断检测:          [N] 不测");
  log_i("|   外电检测:          [N] 不测");
  log_i("|   485检测:           [N] 不测");
  log_i("|   海外校验码:        [N] 不测");

  log_i("+======================================================+");

  // 配置模式识别 (用于快速确认配置是否正确)
  log_i(">>> 配置模式: %s %s %s <<<", (meter_type == 0) ? "机械表" : "超声波表",
        get_module_type_name(module_type),
        get_valve_type_name(current_valve_type));
}

/*============ 命令处理实现 ============*/

static void handle_start_test(const uint8_t *data, uint16_t len) {
  // 验证帧长度
  if (len < sizeof(StartTestFrame)) {
    log_e("开始测试帧长度错误: %d < %d", len, sizeof(StartTestFrame));
    return;
  }

  const StartTestFrame *frame = (const StartTestFrame *)data;

  // 计算校验和 (从头到校验和前)
  uint8_t calc_sum = 0;
  for (uint16_t i = 0; i < len - 2; i++) {
    calc_sum += data[i];
  }

  if (calc_sum != frame->checksum) {
    log_e("校验和错误: 计算=0x%02X, 接收=0x%02X", calc_sum, frame->checksum);
    return;
  }

  // 工位匹配检查：只响应与本机工位匹配的命令
  uint8_t local_station = PC_Protocol_GetStationId();
  if (frame->station_id != local_station) {
    log_d("工位不匹配: 命令工位=%d, 本机工位=%d", frame->station_id,
          local_station);
    return; // 不处理不匹配的命令
  }

  log_i("开始测试命令解析成功");

  // 打印配置摘要 (在解析后立即打印，便于调试)
  print_test_config(frame);

  // 保存测试参数到全局结构
  memcpy(Test_jiejuo_jilu.mokuaihao, frame->meter_number, 6);

  // 设置表类型 (同时设置两个变量以兼容阀门测试模块)
  if (frame->meter_type == 0) {
    water_meter_type = 0;
    Test_jiejuo_jilu.biaoju_leixing_famen = 0; // 机械表
  } else {
    water_meter_type = 1;
    Test_jiejuo_jilu.biaoju_leixing_famen = 1; // 超声波表
  }

  // 解析超声波配置
  Test_jiejuo_jilu.ultrasonic_config.pipe_type = frame->us_meter_type;
  Test_jiejuo_jilu.ultrasonic_config.transducer_type = frame->us_transducer;
  Test_jiejuo_jilu.ultrasonic_config.pipe_size_type = frame->us_pipe_type;
  Test_jiejuo_jilu.ultrasonic_config.valve_type = frame->us_valve_type;
  Test_jiejuo_jilu.ultrasonic_config.module_type = frame->module_type;

  // 解析机械表配置
  Test_jiejuo_jilu.mechanical_config.pipe_diameter_mm = frame->mech_pipe_mm;
  Test_jiejuo_jilu.mechanical_config.valve_type = frame->mech_valve_type;
  Test_jiejuo_jilu.mechanical_config.timeout_seconds = frame->mech_timeout;
  Test_jiejuo_jilu.mechanical_config.stall_current_ma = frame->mech_stall_ma;

  // 确定阀门测试类型
  if (water_meter_type == 0) {
    test_famen_type =
        (Test_jiejuo_jilu.mechanical_config.valve_type != 0) ? 1 : 0;
  } else {
    test_famen_type =
        (Test_jiejuo_jilu.ultrasonic_config.valve_type != 0) ? 1 : 0;
  }

  // 启动测试
  test_start();

  // 发送应答
  send_start_test_ack();
}

static void handle_query_result(const uint8_t *data, uint16_t len) {
  // 验证工位和校验和
  uint8_t frame_len = data[2];
  if (frame_len < 6) {
    return;
  }

  uint8_t station = data[3];
  uint8_t local_station = PC_Protocol_GetStationId();
  if (station != local_station) {
    log_d("工位不匹配: 收到%d, 本机%d", station, local_station);
    return;
  }

  // 验证校验和
  uint8_t calc_sum = 0;
  for (uint16_t i = 0; i < frame_len - 2; i++) {
    calc_sum += data[i];
  }

  if (calc_sum != data[frame_len - 2]) {
    log_e("查询结果校验和错误");
    return;
  }

  // 检查测试是否已结束
  if (Test_liucheng_L != w_end) {
    log_d("测试未结束，不发送结果 (当前状态=%d)", Test_liucheng_L);
    return;
  }

  log_i("收到查询结果请求");

  // 发送测试结果
  send_test_result();
}

// 注: handle_set_config 和 handle_query_fail_step 已移至 pc_protocol_config.c

/*============ 响应发送实现 ============*/
// 发送开始测试应答
static void send_start_test_ack(void) {
  SimpleResponseFrame frame = {.head = FRAME_HEAD_68,
                               .cmd = PC_CMD_START_TEST_ACK,
                               .length = 6,
                               .station_id = PC_Protocol_GetStationId(),
                               .checksum = 0,
                               .tail = FRAME_TAIL_16};

  // 计算校验和
  uint8_t *p = (uint8_t *)&frame;
  for (int i = 0; i < 4; i++) {
    frame.checksum += p[i];
  }

  log_d("发送开始测试应答");

  if (s_send_func != NULL && Debug_Mode == 0) {
    s_send_func((uint8_t *)&frame, sizeof(frame));
  }
}

// 发送测试结果响应
static void send_test_result(void) {
  uint16_t pos = 0;
  uint8_t checksum = 0;

  // 构建结果帧
  s_tx_buffer[pos++] = FRAME_HEAD_68;
  s_tx_buffer[pos++] = PC_CMD_RESULT_RESPONSE; // 0xAD
  s_tx_buffer[pos++] = 0;                      // 长度占位

  // 工位
  s_tx_buffer[pos++] = PC_Protocol_GetStationId();

  // 主电电压(供电) - 小端
  s_tx_buffer[pos++] = (Test_jiejuo_jilu.zhidian_dianya_gongdian) & 0xFF;
  s_tx_buffer[pos++] = ((Test_jiejuo_jilu.zhidian_dianya_gongdian) >> 8) & 0xFF;

  // 主电电压(协议获取)
  s_tx_buffer[pos++] = (Test_jiejuo_jilu.zhidian_dianya_huoqu) & 0xFF;
  s_tx_buffer[pos++] = ((Test_jiejuo_jilu.zhidian_dianya_huoqu) >> 8) & 0xFF;

  // 静态功耗
  s_tx_buffer[pos++] = (Test_jiejuo_jilu.zhidian_jingtai_gonghao) & 0xFF;
  s_tx_buffer[pos++] = ((Test_jiejuo_jilu.zhidian_jingtai_gonghao) >> 8) & 0xFF;

  // 满水功耗
  s_tx_buffer[pos++] = (Test_jiejuo_jilu.zhidian_manshui_gonghao) & 0xFF;
  s_tx_buffer[pos++] = ((Test_jiejuo_jilu.zhidian_manshui_gonghao) >> 8) & 0xFF;

  // 走水功耗
  s_tx_buffer[pos++] = (Test_jiejuo_jilu.zhidian_zoushui_gonghao) & 0xFF;
  s_tx_buffer[pos++] = ((Test_jiejuo_jilu.zhidian_zoushui_gonghao) >> 8) & 0xFF;

  // 备电电压(供电)
  s_tx_buffer[pos++] = (Test_jiejuo_jilu.beidian_dianya_gongdian) & 0xFF;
  s_tx_buffer[pos++] = ((Test_jiejuo_jilu.beidian_dianya_gongdian) >> 8) & 0xFF;

  // 备电电压(获取) - 默认3600
  Test_jiejuo_jilu.beidian_dianya_huoqu = 3600;
  s_tx_buffer[pos++] = (Test_jiejuo_jilu.beidian_dianya_huoqu) & 0xFF;
  s_tx_buffer[pos++] = ((Test_jiejuo_jilu.beidian_dianya_huoqu) >> 8) & 0xFF;

  // 备电功耗
  s_tx_buffer[pos++] = (Test_jiejuo_jilu.beidian_gonghao) & 0xFF;
  s_tx_buffer[pos++] = ((Test_jiejuo_jilu.beidian_gonghao) >> 8) & 0xFF;

  // 检测项状态
  s_tx_buffer[pos++] = Test_jiejuo_jilu.lanya_jiance;
  s_tx_buffer[pos++] = Test_jiejuo_jilu.flash_jiance;
  s_tx_buffer[pos++] = Test_jiejuo_jilu.jiliang_jiance;
  s_tx_buffer[pos++] = Test_jiejuo_jilu.hongwai_jiance;

  // IMEI (15字节)
  memcpy(&s_tx_buffer[pos], Test_jiejuo_jilu.IMEI_CHK, 15);
  pos += 15;

  // IMSI (15字节)
  memcpy(&s_tx_buffer[pos], Test_jiejuo_jilu.IMSI_CHK, 15);
  pos += 15;

  // ICCID (20字节)
  memcpy(&s_tx_buffer[pos], Test_jiejuo_jilu.ICCID_CHK, 20);
  pos += 20;

  // 信号强度
  s_tx_buffer[pos++] = Test_jiejuo_jilu.CSQ;

  // 阀门
  s_tx_buffer[pos++] = Test_jiejuo_jilu.FM;
  s_tx_buffer[pos++] = Test_jiejuo_jilu.FM_daowei;

  // EEPROM
  s_tx_buffer[pos++] = Test_jiejuo_jilu.EEPROM_jiance;

  // GP30电压
  s_tx_buffer[pos++] = (Test_jiejuo_jilu.GP30_dianya) & 0xFF;
  s_tx_buffer[pos++] = ((Test_jiejuo_jilu.GP30_dianya) >> 8) & 0xFF;

  // LoraEUI (16字节)
  memcpy(&s_tx_buffer[pos], Test_jiejuo_jilu.loraEUI, 16);
  pos += 16;

  // 强磁检测 (默认1)
  Test_jiejuo_jilu.qiangci_jiance = 1;
  s_tx_buffer[pos++] = Test_jiejuo_jilu.qiangci_jiance;

  // 开盖检测 (默认1)
  Test_jiejuo_jilu.kaigai_jiance = 1;
  s_tx_buffer[pos++] = Test_jiejuo_jilu.kaigai_jiance;

  // GPS模组
  s_tx_buffer[pos++] = Test_jiejuo_jilu.GPSmozu_jiacne;

  // 无磁检测
  s_tx_buffer[pos++] = 0;

  // 校验码 (2字节),
  memcpy(&s_tx_buffer[pos], Test_jiejuo_jilu.jiaoyanma, 2);
  pos += 2;

  // 版本号 (2字节),传回去的数据也是小端模式，比如0x01
  // 0x03,实际上位解析成了1.0.3
  memcpy(&s_tx_buffer[pos], Test_jiejuo_jilu.banbenhao, 2);
  pos += 2;

  // 水温
  s_tx_buffer[pos++] = Test_jiejuo_jilu.water_temp;

  // 压力 (默认0),当前的超声户用不测试压力
  Test_jiejuo_jilu.pressure_value = 0;
  s_tx_buffer[pos++] = Test_jiejuo_jilu.pressure_value;

  // 设置长度
  s_tx_buffer[2] = pos + 2; // 加上校验和和帧尾

  // 计算校验和
  checksum = 0;
  for (uint16_t i = 0; i < pos; i++) {
    checksum += s_tx_buffer[i];
  }
  s_tx_buffer[pos++] = checksum;
  s_tx_buffer[pos++] = FRAME_TAIL_16;

  log_d("发送测试结果, 长度=%d", pos);

  if (s_send_func != NULL && Debug_Mode == 0) {
    s_send_func(s_tx_buffer, pos);
  }
}

// 注: send_config_ack 和 send_fail_step_response 已移至 pc_protocol_config.c

/*============ 公共API实现 ============*/

/**
 * @brief 发送测试结果到PC端
 *
 * 替代原来 PC_xieyi_Ctrl.c 中的功能
 */
void PC_SendTestResult(void) { send_test_result(); }

/**
 * @brief 调试模式下分析并打印测试结果
 *
 * 兼容原 PC_test_result_analysis() 函数名
 */
void PC_test_result_analysis(void) {
  if (Debug_Mode == 0) {
    return;
  }

  log_d("\r\n========================================");
  log_d("           水表检测结果汇总");
  log_d("========================================");

  // 测试状态
  log_d("【测试状态】");
  if (Test_quanju_canshu_L.test_status == TEST_SUCCESS) {
    log_d("  ✓ 测试成功");
  } else {
    log_d("  ✗ 测试失败");
  }
  log_d("----------------------------------------");

  // 电压检测
  log_d("【电压检测】");
  log_d("  主电供电电压: %lu mV",
        (unsigned long)Test_jiejuo_jilu.zhidian_dianya_gongdian);
  log_d("  主电协议电压: %lu mV",
        (unsigned long)Test_jiejuo_jilu.zhidian_dianya_huoqu);
  log_d("  备电电压: %lu mV",
        (unsigned long)Test_jiejuo_jilu.beidian_dianya_gongdian);
  log_d("----------------------------------------");

  // 功耗检测
  log_d("【功耗检测】");
  log_d("  静态功耗: %lu uA",
        (unsigned long)Test_jiejuo_jilu.zhidian_jingtai_gonghao);
  log_d("  满水功耗: %lu uA",
        (unsigned long)Test_jiejuo_jilu.zhidian_manshui_gonghao);
  log_d("  走水功耗: %lu uA",
        (unsigned long)Test_jiejuo_jilu.zhidian_zoushui_gonghao);
  log_d("  备电功耗: %lu uA", (unsigned long)Test_jiejuo_jilu.beidian_gonghao);
  log_d("----------------------------------------");

  // 通信检测
  log_d("【通信检测】");
  log_d("  蓝牙: %s", Test_jiejuo_jilu.lanya_jiance ? "通过" : "未通过");
  log_d("  红外: %s", Test_jiejuo_jilu.hongwai_jiance ? "通过" : "未通过");
  log_d("  Flash: %s", Test_jiejuo_jilu.flash_jiance ? "通过" : "未通过");
  log_d("  计量: %s", Test_jiejuo_jilu.jiliang_jiance ? "通过" : "未通过");
  log_d("  EEPROM: %s", Test_jiejuo_jilu.EEPROM_jiance ? "通过" : "未通过");
  log_d("----------------------------------------");

  // 阀门检测
  log_d("【阀门检测】");
  log_d("  阀门状态: %s", Test_jiejuo_jilu.FM ? "通过" : "未通过");
  log_d("  阀门到位: %s", Test_jiejuo_jilu.FM_daowei ? "是" : "否");
  log_d("========================================\r\n");
}
