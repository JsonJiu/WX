#ifndef __ADC_CHK_H__
#define __ADC_CHK_H__
#include "main.h"
//初始化
void MF_ADC_PC10_Config_Init(void);
//获取主电位置的电压
uint32_t get_zhudian_weizhi_dianya(void);
//获取二级电源电压
uint32_t get_erjidianyuan_weizhi_dianya(void);
//电磁铁电压
uint32_t get_diancitie_dianya(void);
//主电供电的电压
uint32_t get_zhukongban_gongdian_dianya(void);
//工装的供电电压
uint32_t get_gongzhuang_gongdian_dianya(void);
//NB2的电压
uint32_t get_NB2_weizhi_dianya(void);
//获取NB电源电压
uint32_t get_NB_weizhi_dianya(void);
uint32_t get_famen_2_dianya(void);
uint32_t get_famen_1_dianya(void);
//读取主控电压
uint32_t get_zhukong_dianya(void);
//119供电的电压
uint32_t get_119_gongdian_dianya(void);
#endif
