#ifndef __ADC_CHK_H__
#define __ADC_CHK_H__
#include "main.h"
//初始化
void MF_ADC_Config_Init(void);
//获取主电位置的电压
uint32_t get_zhudian_weizhi_dianya(void);
//获取压力电源电压
uint32_t get_yali_dianya(void);
//电磁铁电压
uint32_t get_diancitie_dianya(void);
//主电供电的电压
uint32_t get_zhukongban_gongdian_dianya(void);
//工装的供电电压
uint32_t get_gongzhuang_gongdian_dianya(void);
uint32_t get_famen_2_dianya(void);
uint32_t get_famen_1_dianya(void);
#endif
