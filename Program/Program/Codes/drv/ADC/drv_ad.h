#ifndef __DRV_AD_H__
#define __DRV_AD_H__
#include "lib_public.h"

//本头文件提供所有对应用开放的接口以及所需全局变量

typedef enum
{
   ADCCHANNEL_ONLINE = 0,
   ADCCHANNEL_RTC,
   ADCCHANNEL_3V3,
   ADCCHANNEL_BAT
}ADCChannel_ET;


extern void OA_ADC_CMD(FunctionalState NewState);
extern void OA_ADC_SetConversionChannel(unsigned char channel);
extern unsigned char OA_ADC_GetConversionState(void);
extern unsigned short OA_ADC_GetConversionValue(void);

//调用接口
void IO_VBB_CHK_EN_DIR_OUT(void);
void IO_VBB_CHK_EN_1(void);
void IO_VBB_CHK_EN_0(void);

//应用对外接口
void OA_ADC_Init(ADC_InitTypeDef* ADC_InitStruct);      //调用bsp
unsigned short OA_ADC_GetValue(ADCChannel_ET AdcChannel);


#endif
