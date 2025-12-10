#ifndef __GPIO_H__
#define __GPIO_H__
#include "main.h"

void INA219_IIC_INIT(void);
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
void NB_Current_CHK_CTRL_ON(void);
void NB_Current_CHK_CTRL_OFF(void);
//主电电流检测使能开
void Current_CHK_CTRL_ON(void);
//主电电流检测使能关
void Current_CHK_CTRL_OFF(void);
//霍尔1控制 开
void huuoer_1_ON(void);
//霍尔1控制 关
void huuoer_1_OFF(void);
//霍尔2控制 开
void huuoer_2_ON(void);
//霍尔2控制 关
void huuoer_2_OFF(void);

void MF_EXTI_Config_Init(void);
void GPIO_IRQHandler(void);
#endif
