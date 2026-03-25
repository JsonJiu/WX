/**
 * @file NB_18_DiaphragmGas_board_config.c
 * @brief NB-18型膜式燃气表工装板级配置 - 直接实现
 * @version 2.0.0
 * @date 2026-02-07
 *
 * @section intro 简介
 * 所有平台驱动函数直接实现，不包回调间接层。
 * 换板子时只需修改本文件中的函数实现和 g_board_config 数据。
 */

#include "NB_18_DiaphragmGas_board_config.h"
#include "fm33lg0xx_fl.h"
#include "ina219.h"

#define LOG_TAG "board"
#include <elog.h>

/*===== 板级配置数据 =====*/

const BoardConfig g_board_config = {
    .battery_type = BATTERY_TYPE_ALKALINE,
    .features =
        {
            .valve_output = 1,
            .valve_input = 0,
            .hall_output = 1,
            .cover_output = 1,
            .power_measure = 1,
            .gprs_3v6 = 1,
            .alarm_119 = 1,
            .capacitor_ctrl = 1,
            .backup_power = 1,
        },
};

/*===== 外部函数声明 (来自 GPIO.c / ZDINA219.c / ADC_CHK.c) =====*/

extern void daowei_1_ON(void);
extern void daowei_1_OFF(void);
extern void daowei_2_ON(void);
extern void daowei_2_OFF(void);

// INA219 相关 (从 ina219.c)
extern uint16_t Current_CHK_Func(uint8_t channel); // 获取电流 (原始单位)

extern uint32_t get_zhudian_weizhi_dianya(void);
extern uint32_t get_NB_weizhi_dianya(void);
extern uint32_t get_gongzhuang_gongdian_dianya(void);
extern uint32_t get_adc_in9_voltage(void);
extern uint32_t get_adc_in9_voltage(void);
extern uint32_t get_adc_in12_voltage(void);
extern uint32_t get_adc_in2_voltage(void);

extern uint32_t get_famen_1_dianya(void);
extern uint32_t get_famen_2_dianya(void);

/*===== 电源控制 =====*/

void board_main_power_on(void) {
  FL_GPIO_SetOutputPin(GPIOB, FL_GPIO_PIN_10); // 主电使能 Pb10
  FL_GPIO_SetOutputPin(GPIOA, FL_GPIO_PIN_10); // INA219旁路开 PE3 (大电流模式)
}

void board_main_power_off(void) {
  FL_GPIO_ResetOutputPin(GPIOA, FL_GPIO_PIN_10); // INA219旁路关
  FL_GPIO_ResetOutputPin(GPIOB, FL_GPIO_PIN_10); // 主电关
}

void board_main_power_measure_mode(void) {
  FL_GPIO_SetOutputPin(GPIOB, FL_GPIO_PIN_10); // 主电使能
  FL_GPIO_ResetOutputPin(GPIOA,
                         FL_GPIO_PIN_10); // INA219旁路关 -> 电流过采样电阻
}

void board_backup_power_on(void) {
  FL_GPIO_SetOutputPin(GPIOB, FL_GPIO_PIN_9); // 备电使能 PB9
}

void board_backup_power_off(void) {
  FL_GPIO_ResetOutputPin(GPIOB, FL_GPIO_PIN_9);
}

/*===== ADC 读取 =====*/

uint16_t board_read_main_voltage_mv(void) {
  return (uint16_t)get_zhudian_weizhi_dianya();
}

uint16_t board_read_backup_voltage_mv(void) {
  return (uint16_t)get_NB_weizhi_dianya();
}

 uint16_t board_read_board_5v_mv(void) {
   return (uint16_t)get_adc_in8_voltage();
 }

// uint16_t board_read_board_4v7_mv(void) {
//   return (uint16_t)get_adc_in9_voltage(); // ADC IN9 PD4, 5V经二极管后约4.7V
// }

uint16_t board_read_board_3v3_mv(void) {
  return (uint16_t)get_adc_in9_voltage(); // ADC IN9 PD4, 3.3V
}

// uint16_t board_read_gprs_3v6_mv(void) {
//   return (uint16_t)get_adc_in12_voltage(); // ADC IN12 PA1, GPRS 3.6V
// }

uint16_t board_read_rtc_battery_mv(void) {
  return (uint16_t)get_adc_in2_voltage(); // ADC IN2 PD3, RTC电池
}

/*===== 阀门电压读取 =====*/

uint16_t board_read_valve_b_voltage_mv(void) {
  return (uint16_t)get_famen_1_dianya();
}

uint16_t board_read_valve_r_voltage_mv(void) {
  return (uint16_t)get_famen_2_dianya();
}

/*===== INA219 电流读取 =====*/

int32_t board_read_main_current_ua(void) {
  return (int32_t)Current_CHK_Func(INA219_CH_MAIN);
}

int32_t board_read_backup_current_ua(void) {
  return (int32_t)Current_CHK_Func(INA219_CH_NB);
}

/*===== GPIO 信号输出 =====*/

void board_valve_out_1(uint8_t level) {
  // 阀位需要推挽/高阻模式切换，由 daowei 函数内部处理
  if (level)
    daowei_1_ON(); // 推挽输出低电平
  else
    daowei_1_OFF(); // 高阻态
}

void board_valve_out_2(uint8_t level) {
  if (level)
    daowei_2_ON();
  else
    daowei_2_OFF();
}

// void board_cover_signal(uint8_t level) {
//   if (level)
//     FL_GPIO_SetOutputPin(GPIOC, FL_GPIO_PIN_2); // 开盖检测 PC2
//   else
//     FL_GPIO_ResetOutputPin(GPIOC, FL_GPIO_PIN_2);
// }

void board_hall_signal_1(uint8_t level) {
  if (level)
  {
    FL_GPIO_SetOutputPin(GPIOB, FL_GPIO_PIN_11);
    FL_GPIO_SetOutputPin(GPIOC, FL_GPIO_PIN_14);
} // 霍尔信号 PC14
  else
    FL_GPIO_ResetOutputPin(GPIOB, FL_GPIO_PIN_11);
    FL_GPIO_ResetOutputPin(GPIOC, FL_GPIO_PIN_14);
}

void board_hall_signal_2(uint8_t level) {
  if (level)
  {
    FL_GPIO_SetOutputPin(GPIOB, FL_GPIO_PIN_11);
    FL_GPIO_SetOutputPin(GPIOC, FL_GPIO_PIN_15);
} // 霍尔信号 PC15
  else
    FL_GPIO_ResetOutputPin(GPIOB, FL_GPIO_PIN_11);
    FL_GPIO_ResetOutputPin(GPIOC, FL_GPIO_PIN_15);
}

void board_alarm_119(uint8_t level) {
  if (level)
    FL_GPIO_SetOutputPin(GPIOE, FL_GPIO_PIN_1); // 119报警 PC14
  else
    FL_GPIO_ResetOutputPin(GPIOE, FL_GPIO_PIN_1);
}

void board_capacitor_disable(void) {
  FL_GPIO_SetOutputPin(GPIOA, FL_GPIO_PIN_2); // 关闭电解电容 PA2
}

// void board_gprs_3v6_enable(uint8_t level) {
//   if (level)
//     FL_GPIO_SetOutputPin(GPIOE, FL_GPIO_PIN_1); // GPRS 3.6V控制 PE1
//   else
//     FL_GPIO_ResetOutputPin(GPIOE, FL_GPIO_PIN_1);
// }

/*===== 延时 =====*/

void board_delay_ms(uint32_t ms) { FL_DelayMs(ms); }
void board_delay_us(uint32_t us) { FL_DelayUs(us); }

/*===== 初始化 =====*/

int board_hw_init(void) {
  // GPIO 初始化在系统 startup 已完成，这里只做状态日志
  log_i("NB-18 膜式燃气表工装硬件初始化完成");
  return 0;
}
