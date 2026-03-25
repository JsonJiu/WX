/**
 * @file NB_18_DiaphragmGas_board_config.h
 * @brief NB-18型膜式燃气表工装板级配置头文件
 * @version 2.0.0
 * @date 2026-02-07
 *
 * @section architecture 架构说明
 * 纯数据：电池类型、功能标志
 * + 平台驱动函数（直接实现，不包回调）
 *
 * 换板子时只需修改 board_config.c 中的函数实现和 g_board_config 数据。
 */

#ifndef NB_18_DIAPHRAGM_GAS_BOARD_CONFIG_H
#define NB_18_DIAPHRAGM_GAS_BOARD_CONFIG_H

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/*===== 电池类型 =====*/

typedef enum {
  BATTERY_TYPE_ALKALINE = 0, /**< 碱性电池 (4节1.5V) */
  BATTERY_TYPE_LITHIUM = 1,  /**< 锂电池 (1节3.6V) */
} BatteryType;

/*===== 功能使能标志 =====*/

typedef struct {
  uint8_t valve_output : 1;   /**< 阀位输出 */
  uint8_t valve_input : 1;    /**< 阀位输入 */
  uint8_t hall_output : 1;    /**< 霍尔输出 */
  uint8_t cover_output : 1;   /**< 开盖输出 */
  uint8_t power_measure : 1;  /**< 功耗测量 */
  uint8_t gprs_3v6 : 1;       /**< GPRS 3.6V 电压检测 */
  uint8_t alarm_119 : 1;      /**< 119报警输出 */
  uint8_t capacitor_ctrl : 1; /**< 电解电容控制 */
  uint8_t backup_power : 1;   /**< 备电源控制 */
  uint8_t reserved : 7;
} FeatureFlags;

/*===== 板级配置 (纯数据，没有函数指针) =====*/

typedef struct {
  BatteryType battery_type;
  FeatureFlags features;
} BoardConfig;

/** @brief 全局板级配置实例 (定义在 board_config.c) */
extern const BoardConfig g_board_config;

/*===== 平台驱动函数 (直接声明，不包回调) =====*/

/* 电源控制 */
void board_main_power_on(void);
void board_main_power_off(void);
void board_main_power_measure_mode(void);
void board_backup_power_on(void);
void board_backup_power_off(void);

/* ADC 电压读取 (返回 mV) */
uint16_t board_read_main_voltage_mv(void);
uint16_t board_read_backup_voltage_mv(void);
uint16_t board_read_board_5v_mv(void);
uint16_t board_read_board_4v7_mv(void);
uint16_t board_read_board_3v3_mv(void);
uint16_t board_read_gprs_3v6_mv(void);
uint16_t board_read_rtc_battery_mv(void);

/* INA219 电流读取 */
int32_t board_read_main_current_ua(void);
int32_t board_read_backup_current_ua(void);

/* GPIO 信号输出 */
void board_valve_out_1(uint8_t level);
void board_valve_out_2(uint8_t level);
void board_cover_signal(uint8_t level);
void board_hall_signal_1(uint8_t level);
void board_hall_signal_2(uint8_t level);
void board_alarm_119(uint8_t level);
void board_capacitor_disable(void);
void board_gprs_3v6_enable(uint8_t level);

/* 阀门电压读取 */
uint16_t board_read_valve_b_voltage_mv(void);
uint16_t board_read_valve_r_voltage_mv(void);

/* 延时 */
void board_delay_ms(uint32_t ms);
void board_delay_us(uint32_t us);

/* 硬件初始化 */
int board_hw_init(void);

#ifdef __cplusplus
}
#endif

#endif /* NB_18_DIAPHRAGM_GAS_BOARD_CONFIG_H */
