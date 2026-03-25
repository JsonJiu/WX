/**
 * @file NB_18_DiaphragmGas_port.h
 * @brief NB-18型膜式燃气表业务接口层
 * @version 3.0.0
 * @date 2026-02-07
 *
 * @section architecture 架构设计
 * | 层次         | 内容                           | 文件               |
 * |--------------|--------------------------------|--------------------|
 * | 板级配置     | 引脚、feature flags、驱动函数  | board_config.h/c   |
 * | 业务接口     | power_on、read_voltage 等      | port.h/c (本文件)  |
 * | 测试逻辑     | 测试步骤状态机                  | diaphragme_meter_test.c |
 *
 * port 层只做 feature flag 检查 + 转调 board 函数，不触碰任何平台头文件。
 */

#ifndef NB_18_DIAPHRAGM_GAS_PORT_H
#define NB_18_DIAPHRAGM_GAS_PORT_H

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/*============ 电压范围宏定义 ============*/

/* 碱性电池输入电压范围 (4节1.5V串联) */
#define ALKALINE_BATTERY_MIN_VOLTAGE_MV 6000 // 6.0V
#define ALKALINE_BATTERY_MAX_VOLTAGE_MV 6500 // 6.5V

/* 锂电池输入电压范围 */
#define LITHIUM_BATTERY_MIN_VOLTAGE_MV 3500 // 3.5V
#define LITHIUM_BATTERY_MAX_VOLTAGE_MV 3900 // 3.9V

/* 膜表板上3.6V电压范围 */
#define METER_BOARD_3V6_MIN_VOLTAGE_MV 3400 // 3.4V
#define METER_BOARD_3V6_MAX_VOLTAGE_MV 3800 // 3.8V

/* 膜表板上3.3V电压范围 */
#define METER_BOARD_3V3_MIN_VOLTAGE_MV 3200 // 3.2V
#define METER_BOARD_3V3_MAX_VOLTAGE_MV 3500 // 3.5V

/* 膜表板上5V电压范围 */
#define METER_BOARD_5V_MIN_VOLTAGE_MV 4800 // 4.8V
#define METER_BOARD_5V_MAX_VOLTAGE_MV 5200 // 5.2V

/* 膜表板上4.7V电压范围 (5V经二极管) */
#define METER_BOARD_4V7_MIN_VOLTAGE_MV 4400 // 4.4V
#define METER_BOARD_4V7_MAX_VOLTAGE_MV 5000 // 5.0V

/* 备电电压范围 */
#define BACKUP_POWER_MIN_VOLTAGE_MV 3300 // 3.3V
#define BACKUP_POWER_MAX_VOLTAGE_MV 3500 // 3.5V

/* GPRS 3.6V电压范围 */
#define GPRS_3V6_MIN_VOLTAGE_MV 3200 // 3.2V
#define GPRS_3V6_MAX_VOLTAGE_MV 3800 // 3.8V

/*============ 功耗范围宏定义 ============*/

/* 静态功耗范围 */
#define STATIC_POWER_MIN_CURRENT_UA 10 // 10uA
#define STATIC_POWER_MAX_CURRENT_UA 30 // 30uA

/* 动态功耗范围 (运行时安全保护) */
#define DYNAMIC_POWER_MIN_CURRENT_MA 30  // 30mA
#define DYNAMIC_POWER_MAX_CURRENT_MA 700 // 700mA

/*============ 状态码定义 ============*/

typedef enum {
  PORT_OK = 0,               /**< 操作成功 */
  PORT_ERR_NOT_INIT,         /**< 未初始化 */
  PORT_ERR_NULL_PARAM,       /**< 参数为空 */
  PORT_ERR_FEATURE_DISABLED, /**< 功能未使能 */
  PORT_ERR_INVALID_CHANNEL,  /**< 无效通道 */
  PORT_ERR_TIMEOUT,          /**< 操作超时 */
  PORT_ERR_HW_FAULT,         /**< 硬件故障 */
} PortStatus;

const char *port_status_to_string(PortStatus status);

/*============ 主电电压范围接口 (根据电池类型) ============*/

uint16_t get_main_power_min_voltage_mv(void);
uint16_t get_main_power_max_voltage_mv(void);

/*============ 初始化接口 ============*/

/**
 * @brief 初始化膜式燃气表工装 (board + port)
 * @return 0-成功, 其他-失败
 * @note 此函数应在系统初始化时调用，早于测试逻辑
 */
int diaphragm_gas_board_init(void);

/**
 * @brief 初始化膜表工装业务接口层
 * @return PORT_OK-成功, 其他-失败原因
 */
PortStatus diaphragm_port_init(void);

/**
 * @brief 反初始化
 */
void diaphragm_gas_port_deinit(void);

/**
 * @brief 检查是否已初始化
 */
bool diaphragm_gas_port_is_init(void);

/*============ 电源控制接口 ============*/

PortStatus power_on_main_power(void);
PortStatus power_off_main_power(void);
PortStatus power_on_main_power_measure_mode(void);
PortStatus power_on_backup_power(void);
PortStatus power_off_backup_power(void);

/*============ 电压读取接口 ============*/

PortStatus read_main_voltage_mv(uint16_t *voltage_mv);
PortStatus read_backup_voltage_mv(uint16_t *voltage_mv);
PortStatus read_board_5v_voltage_mv(uint16_t *voltage_mv);
PortStatus read_board_4v7_voltage_mv(uint16_t *voltage_mv);
PortStatus read_board_3v3_voltage_mv(uint16_t *voltage_mv);
PortStatus read_gprs_3v6_voltage_mv(uint16_t *voltage_mv);
PortStatus read_rtc_battery_voltage_mv(uint16_t *voltage_mv);
PortStatus enable_board_4v7_read(uint8_t level);

/*============ 阀门电压读取接口 ============*/

PortStatus read_valve_b_voltage_mv(uint16_t *mv);
PortStatus read_valve_r_voltage_mv(uint16_t *mv);

/*============ 功耗读取接口 ============*/

PortStatus read_main_power_consumption_ua(int32_t *current_ua);
PortStatus read_backup_power_consumption_ua(int32_t *current_ua);

/*============ 阀位信号接口 ============*/

PortStatus output_valve_position_signal_1(uint8_t level);
PortStatus output_valve_position_signal_2(uint8_t level);
PortStatus read_valve_position_signal_1(uint8_t *level);
PortStatus read_valve_position_signal_2(uint8_t *level);

/*============ 开盖信号接口 ============*/

PortStatus output_open_cover_signal(uint8_t level);

/*============ 霍尔信号接口 ============*/

PortStatus output_hall_signal_1(uint8_t level);
PortStatus output_hall_signal_2(uint8_t level);

/*============ 119报警信号接口 ============*/

PortStatus output_alarm_119_signal(uint8_t level);

/*============ 延时接口 ============*/

PortStatus port_delay_ms(uint32_t ms);
PortStatus port_delay_us(uint32_t us);

/*============ 电解电容控制 ============*/

PortStatus diaphragm_board_capacitor_disable(void);

/*============ 辅助检查宏 ============*/

#define PORT_CHECK_RETURN(status)                                              \
  do {                                                                         \
    if ((status) != PORT_OK)                                                   \
      return (status);                                                         \
  } while (0)

#define PORT_CHECK_GOTO(status, label)                                         \
  do {                                                                         \
    if ((status) != PORT_OK)                                                   \
      goto label;                                                              \
  } while (0)

#ifdef __cplusplus
}
#endif

#endif /* NB_18_DIAPHRAGM_GAS_PORT_H */
