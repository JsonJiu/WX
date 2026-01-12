#include "DAC.h"

void MF_OPA_Init(void);
void MF_DAC1_Init(void)
{
    FL_GPIO_InitTypeDef    GPIO_InitStruct;

    FL_DAC_InitTypeDef    DAC_InitStruct;

    GPIO_InitStruct.pin = FL_GPIO_PIN_12;                                                /*配置GPIO的引脚号*/
    GPIO_InitStruct.mode = FL_GPIO_MODE_ANALOG;                                         /*配置GPIO的功能模式*/
    GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;                               /*配置GPIO的输出模式*/
    GPIO_InitStruct.pull = FL_DISABLE;                                                  /*配置GPIO输上拉模式*/
    GPIO_InitStruct.remapPin = FL_DISABLE;                                              /*配置GPIO数字重定向功能*/
    GPIO_InitStruct.analogSwitch = FL_DISABLE;                                          /*配置GPIO模拟开关功能*/
    (void)FL_GPIO_Init(GPIOB, &GPIO_InitStruct);                                        /*GPIO初始化*/

    DAC_InitStruct.triggerMode = FL_ENABLE;                                            /*配置触发模式使能*/
    DAC_InitStruct.triggerSource = FL_DAC_TRGI_SOFTWARE;                                /*配置触发源*/
    DAC_InitStruct.sampleHoldMode = FL_DISABLE;                                         /*配置采样保持使能*/
    DAC_InitStruct.holdTime = 0x0;                                                      /*配置保持时间*/
    DAC_InitStruct.sampleTime = 0x0;                                                    /*配置采样时间*/
    DAC_InitStruct.bufferMode = FL_DISABLE;                                              /*配置buffer模式*/
    DAC_InitStruct.switchMode = FL_DISABLE;                                              /*配置buffer反馈开关*/
    (void)FL_DAC_Init(DAC, &DAC_InitStruct);                                            /*DAC初始化*/
		//对运放进行初始化
		MF_OPA_Init();
}
void MF_OPA_Init(void)
{
    FL_GPIO_InitTypeDef    GPIO_InitStruct;

    FL_OPA_InitTypeDef    OPA_InitStruct;

    GPIO_InitStruct.pin          = FL_GPIO_PIN_12;
    GPIO_InitStruct.mode         = FL_GPIO_MODE_ANALOG;
    GPIO_InitStruct.outputType   = FL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.pull         = FL_DISABLE;
    GPIO_InitStruct.remapPin     = FL_DISABLE;
    GPIO_InitStruct.analogSwitch = FL_DISABLE;
    (void)FL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    OPA_InitStruct.INP           = FL_OPA_INP_SOURCE_DAC;
    OPA_InitStruct.INN           = FL_OPA_INN_SOURCE_INN1;
    OPA_InitStruct.mode          = FL_OPA_MODE_BUFFER;
    OPA_InitStruct.PGAModeSelect = FL_OPA_PGA_MODE_FB_TO_GND;
    OPA_InitStruct.negtiveToPin  = FL_DISABLE;
    OPA_InitStruct.gain          = FL_OPA_GAIN_NOINVERT_X2;
    OPA_InitStruct.lowPowermode  = FL_DISABLE;

    (void)FL_OPA_Init(OPA, &OPA_InitStruct);
		//配置完成后直接打开
    FL_OPA_Enable(OPA);
}

//设置控制电压
//设置电压为mV，设置电压为输出电压，最终电压的计算放在这里
void DAC_SWTrigger(uint32_t dianya)
{
	  uint32_t shezhi_dianya = 0;
	  //设置电压有效性检测
	  if(dianya>8000)
		{
			return;
		}
	  shezhi_dianya = (1221*6-dianya)/5;
		shezhi_dianya = shezhi_dianya*1024/825;
    FL_DAC_Enable(DAC);
    FL_DAC_WriteData(DAC, shezhi_dianya);
    FL_DAC_EnableSoftwareTrigger(DAC);
}
void DAC_Select(void)
{
	uint8_t State;
	State=FL_GPIO_GetInputPin(GPIOB,FL_GPIO_PIN_6);
  switch(State)
	{
		case 1:
      DAC_SWTrigger(7200);
      break;
		 case 0:
	    DAC_SWTrigger(6000);
	    break;
	default:
					break;	
	}
}
