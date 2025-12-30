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
//按键控制 开(拉低)
void anjian_kongzhi_ON(void);
//按键控制 关(拉高)
void anjian_kongzhi_OFF(void);
//计量IO1置高
void jiliang_IO1_set_Hi(void);
//计量IO1置低
void jiliang_IO1_set_Low(void);
//计量IO2置高
void jiliang_IO2_set_Hi(void);
//计量IO2置低
void jiliang_IO2_set_Low(void);
//开到位置高
void kaidaowei_set_Hi(void);
//开到位置低
void kaidaowei_set_Low(void);
//关到位置高
void guandaowei_set_Hi(void);
//关到位置低
void guandaowei_set_Low(void);
void MF_EXTI_Config_Init(void);
void GPIO_IRQHandler(void);
#endif
