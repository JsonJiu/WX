/**
 * @file NB_18_DiaphragmGas_port.c
 * @brief NB-18型膜式燃气表业务接口层实现
 * @version 3.0.0
 * @date 2026-02-07
 *
 * @section intro 简介
 * 每个函数只做三件事: 检查初始化 + 检查 feature flag + 转调 board 函数。
 * 不包含任何平台头文件 (fm33lg0xx_fl.h 等)。
 */

#include "NB_18_DiaphragmGas_port.h"
#include "NB_18_DiaphragmGas_board_config.h"
#include <stddef.h>

#define LOG_TAG "port"
#include <elog.h>

/*============ 私有变量 ============*/

static bool s_initialized = false;

/*============ 内部检查宏 ============*/

#define CHECK_INIT()                                                           \
  do {                                                                         \
    if (!s_initialized)                                                        \
      return PORT_ERR_NOT_INIT;                                                \
  } while (0)

#define CHECK_FEATURE(f)                                                       \
  do {                                                                         \
    if (!g_board_config.features.f)                                            \
      return PORT_ERR_FEATURE_DISABLED;                                        \
  } while (0)

#define CHECK_PARAM(ptr)                                                       \
  do {                                                                         \
    if (!(ptr))                                                                \
      return PORT_ERR_NULL_PARAM;                                              \
  } while (0)

/*============ 状态码字符串 ============*/

static const char *s_status_strings[] = {
    [PORT_OK] = "OK",
    [PORT_ERR_NOT_INIT] = "Not initialized",
    [PORT_ERR_NULL_PARAM] = "Null parameter",
    [PORT_ERR_FEATURE_DISABLED] = "Feature disabled",
    [PORT_ERR_INVALID_CHANNEL] = "Invalid channel",
    [PORT_ERR_TIMEOUT] = "Timeout",
    [PORT_ERR_HW_FAULT] = "Hardware fault",
};

const char *port_status_to_string(PortStatus status) {
  if (status < sizeof(s_status_strings) / sizeof(s_status_strings[0])) {
    return s_status_strings[status];
  }
  return "Unknown";
}

/*============ 初始化接口 ============*/

/*============ 主电电压范围接口 ============*/

uint16_t get_main_power_min_voltage_mv(void) {
  return (g_board_config.battery_type == BATTERY_TYPE_ALKALINE)
             ? ALKALINE_BATTERY_MIN_VOLTAGE_MV
             : LITHIUM_BATTERY_MIN_VOLTAGE_MV;
}

uint16_t get_main_power_max_voltage_mv(void) {
  return (g_board_config.battery_type == BATTERY_TYPE_ALKALINE)
             ? ALKALINE_BATTERY_MAX_VOLTAGE_MV
             : LITHIUM_BATTERY_MAX_VOLTAGE_MV;
}

/*============ 初始化接口实现 ============*/

int diaphragm_gas_board_init(void) {
  PortStatus status = diaphragm_port_init();
  if (status != PORT_OK) {
    log_e("膜式燃气表板级初始化失败: %s", port_status_to_string(status));
    return -1;
  }
  log_i("膜式燃气表板级配置初始化成功");
  return 0;
}

PortStatus diaphragm_port_init(void) {
  if (board_hw_init() != 0) {
    return PORT_ERR_HW_FAULT;
  }
  s_initialized = true;
  return PORT_OK;
}

void diaphragm_gas_port_deinit(void) { s_initialized = false; }

bool diaphragm_gas_port_is_init(void) { return s_initialized; }

/*============ 电源控制接口 ============*/

PortStatus power_on_main_power(void) {
  CHECK_INIT();
  board_main_power_on();
  return PORT_OK;
}

PortStatus power_off_main_power(void) {
  CHECK_INIT();
  board_main_power_off();
  return PORT_OK;
}

PortStatus power_on_main_power_measure_mode(void) {
  CHECK_INIT();
  CHECK_FEATURE(power_measure);
  board_main_power_measure_mode();
  return PORT_OK;
}

PortStatus power_on_backup_power(void) {
  CHECK_INIT();
  CHECK_FEATURE(backup_power);
  board_backup_power_on();
  return PORT_OK;
}

PortStatus power_off_backup_power(void) {
  CHECK_INIT();
  CHECK_FEATURE(backup_power);
  board_backup_power_off();
  return PORT_OK;
}

/*============ 电压读取接口 ============*/

PortStatus read_main_voltage_mv(uint16_t *voltage_mv) {
  CHECK_INIT();
  CHECK_PARAM(voltage_mv);
  *voltage_mv = board_read_main_voltage_mv();
  return PORT_OK;
}

PortStatus read_backup_voltage_mv(uint16_t *voltage_mv) {
  CHECK_INIT();
  CHECK_PARAM(voltage_mv);
  *voltage_mv = board_read_backup_voltage_mv();
  return PORT_OK;
}

PortStatus read_board_5v_voltage_mv(uint16_t *voltage_mv) {
  CHECK_INIT();
  CHECK_PARAM(voltage_mv);
  *voltage_mv = board_read_board_5v_mv();
  return PORT_OK;
}

PortStatus read_board_4v7_voltage_mv(uint16_t *voltage_mv) {
  CHECK_INIT();
  CHECK_PARAM(voltage_mv);
  *voltage_mv = board_read_board_4v7_mv();
  return PORT_OK;
}

PortStatus read_board_3v3_voltage_mv(uint16_t *voltage_mv) {
  CHECK_INIT();
  CHECK_PARAM(voltage_mv);
  *voltage_mv = board_read_board_3v3_mv();
  return PORT_OK;
}

PortStatus read_gprs_3v6_voltage_mv(uint16_t *voltage_mv) {
  CHECK_INIT();
  CHECK_FEATURE(gprs_3v6);
  CHECK_PARAM(voltage_mv);
  *voltage_mv = board_read_gprs_3v6_mv();
  return PORT_OK;
}

PortStatus read_rtc_battery_voltage_mv(uint16_t *voltage_mv) {
  CHECK_INIT();
  CHECK_PARAM(voltage_mv);
  *voltage_mv = board_read_rtc_battery_mv();
  return PORT_OK;
}

PortStatus enable_board_4v7_read(uint8_t level) {
  CHECK_INIT();
  CHECK_FEATURE(gprs_3v6);
  board_gprs_3v6_enable(level);
  return PORT_OK;
}

/*============ 阀门电压读取接口 ============*/

PortStatus read_valve_b_voltage_mv(uint16_t *mv) {
  CHECK_INIT();
  CHECK_PARAM(mv);
  *mv = board_read_valve_b_voltage_mv();
  return PORT_OK;
}

PortStatus read_valve_r_voltage_mv(uint16_t *mv) {
  CHECK_INIT();
  CHECK_PARAM(mv);
  *mv = board_read_valve_r_voltage_mv();
  return PORT_OK;
}

/*============ 功耗读取接口 ============*/

PortStatus read_main_power_consumption_ua(int32_t *current_ua) {
  CHECK_INIT();
  CHECK_PARAM(current_ua);
  CHECK_FEATURE(power_measure);
  *current_ua = board_read_main_current_ua();
  return PORT_OK;
}

PortStatus read_backup_power_consumption_ua(int32_t *current_ua) {
  CHECK_INIT();
  CHECK_PARAM(current_ua);
  CHECK_FEATURE(power_measure);
  *current_ua = board_read_backup_current_ua();
  return PORT_OK;
}

/*============ 阀位信号接口 ============*/

PortStatus output_valve_position_signal_1(uint8_t level) {
  CHECK_INIT();
  CHECK_FEATURE(valve_output);
  board_valve_out_1(level);
  return PORT_OK;
}

PortStatus output_valve_position_signal_2(uint8_t level) {
  CHECK_INIT();
  CHECK_FEATURE(valve_output);
  board_valve_out_2(level);
  return PORT_OK;
}

PortStatus read_valve_position_signal_1(uint8_t *level) {
  CHECK_INIT();
  CHECK_PARAM(level);
  CHECK_FEATURE(valve_input);
  *level = 0; // 当前硬件未实现阀位输入
  return PORT_OK;
}

PortStatus read_valve_position_signal_2(uint8_t *level) {
  CHECK_INIT();
  CHECK_PARAM(level);
  CHECK_FEATURE(valve_input);
  *level = 0;
  return PORT_OK;
}

/*============ 开盖信号接口 ============*/

PortStatus output_open_cover_signal(uint8_t level) {
  CHECK_INIT();
  CHECK_FEATURE(cover_output);
  board_cover_signal(level);
  return PORT_OK;
}

/*============ 霍尔信号接口 ============*/

PortStatus output_hall_signal_1(uint8_t level) {
  CHECK_INIT();
  CHECK_FEATURE(hall_output);
  board_hall_signal_1(level);
  return PORT_OK;
}

PortStatus output_hall_signal_2(uint8_t level) {
  CHECK_INIT();
  CHECK_FEATURE(hall_output);
  board_hall_signal_2(level);
  return PORT_OK;
}

/*============ 119报警信号接口 ============*/

PortStatus output_alarm_119_signal(uint8_t level) {
  CHECK_INIT();
  CHECK_FEATURE(alarm_119);
  board_alarm_119(level);
  return PORT_OK;
}

/*============ 延时接口 ============*/

PortStatus port_delay_ms(uint32_t ms) {
  board_delay_ms(ms);
  return PORT_OK;
}

PortStatus port_delay_us(uint32_t us) {
  board_delay_us(us);
  return PORT_OK;
}

/*============ 电解电容控制 ============*/

PortStatus diaphragm_board_capacitor_disable(void) {
  CHECK_INIT();
  CHECK_FEATURE(capacitor_ctrl);
  board_capacitor_disable();
  return PORT_OK;
}
