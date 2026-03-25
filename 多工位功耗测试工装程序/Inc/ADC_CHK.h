#ifndef __ADC_CHK_H__
#define __ADC_CHK_H__
#include "main.h"
//初始化
void MF_ADC_PC10_Config_Init(void);
//获取主电位置的电压
uint32_t get_zhudian_weizhi_dianya(void);
//获取二级电源电压
uint32_t get_erjidianyuan_weizhi_dianya(void);
//获取VCC电源电压
uint32_t get_VCC_weizhi_dianya(void);
//获取升压电路电压
uint32_t get_SY_weizhi_dianya(void);
//获取主电供电电路电压
uint32_t get_zhudian_gongdian_weizhi_dianya(void);
//检测工装自身电路电压
uint32_t get_gongzhuang_MCU_gongdian_weizhi_dianya(void);
#endif
