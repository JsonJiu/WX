#ifndef __GPIO_H__
#define __GPIO_H__
#include "main.h"

void INA219_IIC_INIT(void);
void Others_GPIO_Init(void);
void LED_On(void);
void LED_Off(void);
void LED_Toggle();
void zhudian_gongdian_On(void);
void zhudian_gongdian_OFF(void);
void beidian_gongdian_On(void);
void beidian_gongdian_OFF(void);
void zhudian_dianya_CHK_CTRL_ON(void);
void zhudian_dianya_CHK_CTRL_OFF(void);
void erji_dianya_CHK_CTRL_ON(void);
void erji_dianya_CHK_CTRL_OFF(void);
void NB_Current_CHK_CTRL_ON(void);
void NB_Current_CHK_CTRL_OFF(void);
void valve_open_on(void);
void valve_open_off(void);
void valve_close_on(void);
void valve_close_off(void);

// ����������ʹ�ܿ�
void Current_CHK_CTRL_ON(void);
// ����������ʹ�ܹ�
void Current_CHK_CTRL_OFF(void);
// 电磁铁开启
void Electromagnet_On(void);
// 电磁铁关闭
void Electromagnet_Off(void);
// 电机启动
void Motor_Start(void);
// 电机停止
void Motor_Stop(void);

void MF_EXTI_Config_Init(void);
void GPIO_IRQHandler(void);
uint8_t board_station_check(void);

// 阀门GPIO模式切换函数
void change_valve_open_to_input_mode(void);
void change_valve_close_to_input_mode(void);
void change_valve_open_to_output_mode(void);
void change_valve_close_to_output_mode(void);

// 阀门状态读取函数
uint8_t read_valve_open_status(void);
uint8_t read_valve_close_status(void);
#endif
