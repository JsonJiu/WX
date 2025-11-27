#ifndef __GPIO_H__
#define __GPIO_H__
#include "main.h"

void Others_GPIO_Init(void);
void LED_On(void);
void LED_Off(void);

//VBAT电压
void zhudian_gongdian_On(void);
void zhudian_gongdian_OFF(void);

//这里是提供压力传感器检测电压
void beidian_gongdian_On(void);
void beidian_gongdian_OFF(void);

//主电电压检测
void zhudian_dianya_CHK_CTRL_ON(void);
void zhudian_dianya_CHK_CTRL_OFF(void);

//主电功耗检测
void Current_CHK_CTRL_ON(void);
void Current_CHK_CTRL_OFF(void);

//备电电压检测
void erji_dianya_CHK_CTRL_ON(void);
void erji_dianya_CHK_CTRL_OFF(void);

//备电功耗检测，这里不使用
void NB_Current_CHK_CTRL_ON(void);
void NB_Current_CHK_CTRL_OFF(void);

//霍尔1控制 开
void huuoer_1_ON(void);
//霍尔1控制 关
void huuoer_1_OFF(void);
//霍尔2控制 开
void huuoer_2_ON(void);
//霍尔2控制 关
void huuoer_2_OFF(void);

//开到位控制 高
void daowei_OPEN_High(void);
//开到位控制 低
void daowei_OPEN_Low(void);
//关到位控制 高
void daowei_CLOSE_High(void);
//关到位控制 低
void daowei_CLOSE_Low(void);
//开到位
void kai_daowei(void);
//关到位
void guan_daowei(void);
	
//RS2103控制
void RS2103_POWER_ON(void);
void RS2103_POWER_OFF(void);
void RS2103_Sel_wuci(void);
void RS2103_Sel_youci(void);

//PTD有磁控制
void SENSOR_P_High(void);
void SENSOR_P_Low(void);
void SENSOR_D_High(void);
void SENSOR_D_Low(void);
void SENSOR_T_High(void);
void SENSOR_T_Low(void);

//检测压力使能引脚是否正常
uint8_t COR_PRE_CHK(void);
#endif
