#ifndef __GPIO_H__
#define __GPIO_H__
#include "main.h"

void Others_GPIO_Init(void);
void LED_On(void);
void LED_Off(void);
void zhudian_gongdian_On(void);
void zhudian_gongdian_OFF(void);
void beidian_gongdian_On(void);
void beidian_gongdian_OFF(void);
void zhudian_dianya_CHK_CTRL_ON(void);
void zhudian_dianya_CHK_CTRL_OFF(void);
void erji_dianya_CHK_CTRL_ON(void);
void erji_dianya_CHK_CTRL_OFF(void);
// 霍尔1控制
void huuoer_1_ON(void);
void huuoer_1_OFF(void);
// 霍尔2控制
void huuoer_2_ON(void);
void huuoer_2_OFF(void);

//��λ1���� ��
void daowei_1_ON(void);
//��λ1���� ��
void daowei_1_OFF(void);
//��λ2���� ��
void daowei_2_ON(void);
//��λ2���� ��
void daowei_2_OFF(void);

void MF_EXTI_Config_Init(void);
void GPIO_IRQHandler(void);

// 工位检测函数声明
uint8_t read_gongwei_pin_PC0(void);
uint8_t read_gongwei_pin_PD12(void);
uint8_t read_gongwei_pin_PB15(void);
uint8_t read_gongwei_pin_PE5(void);
uint8_t get_gongwei_number(void);

#endif
