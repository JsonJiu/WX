
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef _BOARD_H
#define _BOARD_H
#include "lib_public.h"
///***********需要 BSP提供的函数 ***********/
////端口方向初始化  OUT-输出，IN-输入
//void PowerCtrl_Init(unsigned char mode);
//void GreenLed_Init(unsigned char mode);
//void RedLed_Init(unsigned char mode);
//void KEY_Init(unsigned char mode);
//
//// 输出控制
//void PowerCtrl_Set(void);
//void PowerCtrl_Clr(void);
//
//void GreenLed_Set(void);
//void GreenLed_Clr(void);
//
//void RedLed_Set(void);
//void RedLed_Clr(void);
//
////KEY 获取输入
//unsigned char KEY_Get(void);
//
///***********结束  需要 BSP提供的函数 ***********/

typedef enum
{
   ADCCHANNELVOLT = 0,
   ADCCHANNELCUR,
}ADCChannel_ET;


extern void OA_ADC_CMD(FunctionalState NewState);
extern void OA_ADC_SetConversionChannel(unsigned char channel);
extern unsigned char OA_ADC_GetConversionState(void);
extern unsigned short OA_ADC_GetConversionValue(void);

//调用接口
//void IO_VBB_CHK_EN_DIR_OUT(void);
//void IO_VBB_CHK_EN_1(void);
//void IO_VBB_CHK_EN_0(void);

//应用对外接口
void OA_ADC_Init(ADC_InitTypeDef* ADC_InitStruct);      //调用bsp
unsigned short OA_ADC_GetValue(ADCChannel_ET AdcChannel);


void GPIO_Config(void);
void HardwareInit(void);
unsigned int Get_Voltage(void);
void Printf_DebugMessage(char *str);

void PowerHold(void);
void PowerOff(void);
void RedLed_On(void);
void RedLed_Off(void);
void GreenLed_On(void);
void GreenLed_Off(void);

void StateLEDControl(void);
void Task_StateManage(void);
#endif /*__ __BOARD_H */
