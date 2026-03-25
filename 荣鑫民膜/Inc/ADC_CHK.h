#ifndef __ADC_CHK_H__
#define __ADC_CHK_H__
#include "GPIO.h"
#include "main.h"

// ��ʼ��
void MF_ADC_PC10_Config_Init(void);
// ��ȡ����λ�õĵ�ѹ
uint32_t get_zhudian_weizhi_dianya(void);
// ��ȡ������Դ��ѹ
uint32_t get_erjidianyuan_weizhi_dianya(void);
// �������ѹ
uint32_t get_diancitie_dianya(void);
// ���繩��ĵ�ѹ
uint32_t get_zhukongban_gongdian_dianya(void);
// ��װ�Ĺ����ѹ
uint32_t get_gongzhuang_gongdian_dianya(void);
uint32_t get_famen_2_dianya(void);
uint32_t get_famen_1_dianya(void);
uint32_t get_adc_in5_voltage(void); // RTC电池电压 (PA0, ADC IN5)
#endif
