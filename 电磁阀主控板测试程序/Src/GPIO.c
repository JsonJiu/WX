#include "GPIO.h"
#include "ZDINA219.h"
void Others_GPIO_Init()
{
	  //LED指示灯 PD10
	  FL_GPIO_InitTypeDef    GPIO_InitStruct;
    GPIO_InitStruct.pin = FL_GPIO_PIN_8;
    GPIO_InitStruct.mode = FL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.pull = FL_DISABLE;
    GPIO_InitStruct.remapPin = FL_DISABLE;
    GPIO_InitStruct.analogSwitch = FL_DISABLE;
    FL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	  //主电源供电引脚
    GPIO_InitStruct.pin = FL_GPIO_PIN_4;
    GPIO_InitStruct.mode = FL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.pull = FL_DISABLE;
    GPIO_InitStruct.remapPin = FL_DISABLE;
    GPIO_InitStruct.analogSwitch = FL_DISABLE;
    FL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	  //另一路电源引脚
	  GPIO_InitStruct.pin = FL_GPIO_PIN_5;
    GPIO_InitStruct.mode = FL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.pull = FL_DISABLE;
    GPIO_InitStruct.remapPin = FL_DISABLE;
    GPIO_InitStruct.analogSwitch = FL_DISABLE;
    FL_GPIO_Init(GPIOA, &GPIO_InitStruct);
		//主电电压检测控制
		GPIO_InitStruct.pin = FL_GPIO_PIN_2;
    GPIO_InitStruct.mode = FL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.pull = FL_DISABLE;
    GPIO_InitStruct.remapPin = FL_DISABLE;
    GPIO_InitStruct.analogSwitch = FL_DISABLE;
    FL_GPIO_Init(GPIOB, &GPIO_InitStruct);
		//二级电压检测控制
		GPIO_InitStruct.pin = FL_GPIO_PIN_9;
    GPIO_InitStruct.mode = FL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.pull = FL_DISABLE;
    GPIO_InitStruct.remapPin = FL_DISABLE;
    GPIO_InitStruct.analogSwitch = FL_DISABLE;
    FL_GPIO_Init(GPIOA, &GPIO_InitStruct);
		//VCC电压检测控制
		GPIO_InitStruct.pin = FL_GPIO_PIN_6;
    GPIO_InitStruct.mode = FL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.pull = FL_DISABLE;
    GPIO_InitStruct.remapPin = FL_DISABLE;
    GPIO_InitStruct.analogSwitch = FL_DISABLE;
    FL_GPIO_Init(GPIOB, &GPIO_InitStruct);
		//升压电压检测
		GPIO_InitStruct.pin = FL_GPIO_PIN_7;
    GPIO_InitStruct.mode = FL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.pull = FL_DISABLE;
    GPIO_InitStruct.remapPin = FL_DISABLE;
    GPIO_InitStruct.analogSwitch = FL_DISABLE;
    FL_GPIO_Init(GPIOB, &GPIO_InitStruct);
		//电流检测使能
		GPIO_InitStruct.pin = FL_GPIO_PIN_7;
    GPIO_InitStruct.mode = FL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.pull = FL_DISABLE;
    GPIO_InitStruct.remapPin = FL_DISABLE;
    GPIO_InitStruct.analogSwitch = FL_DISABLE;
    FL_GPIO_Init(GPIOC, &GPIO_InitStruct);
		//串口通信使能
		GPIO_InitStruct.pin = FL_GPIO_PIN_15;
    GPIO_InitStruct.mode = FL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.pull = FL_DISABLE;
    GPIO_InitStruct.remapPin = FL_DISABLE;
    GPIO_InitStruct.analogSwitch = FL_DISABLE;
    FL_GPIO_Init(GPIOA, &GPIO_InitStruct);
		//按键1
		GPIO_InitStruct.pin = FL_GPIO_PIN_8;
    GPIO_InitStruct.mode = FL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.pull = FL_DISABLE;
    GPIO_InitStruct.remapPin = FL_DISABLE;
    GPIO_InitStruct.analogSwitch = FL_DISABLE;
    FL_GPIO_Init(GPIOB, &GPIO_InitStruct);
		//按键2
		GPIO_InitStruct.pin = FL_GPIO_PIN_9;
    GPIO_InitStruct.mode = FL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.pull = FL_DISABLE;
    GPIO_InitStruct.remapPin = FL_DISABLE;
    GPIO_InitStruct.analogSwitch = FL_DISABLE;
    FL_GPIO_Init(GPIOB, &GPIO_InitStruct);
		//按键3
		GPIO_InitStruct.pin = FL_GPIO_PIN_10;
    GPIO_InitStruct.mode = FL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.pull = FL_DISABLE;
    GPIO_InitStruct.remapPin = FL_DISABLE;
    GPIO_InitStruct.analogSwitch = FL_DISABLE;
    FL_GPIO_Init(GPIOB, &GPIO_InitStruct);
		//按键4
		GPIO_InitStruct.pin = FL_GPIO_PIN_11;
    GPIO_InitStruct.mode = FL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.pull = FL_DISABLE;
    GPIO_InitStruct.remapPin = FL_DISABLE;
    GPIO_InitStruct.analogSwitch = FL_DISABLE;
    FL_GPIO_Init(GPIOB, &GPIO_InitStruct);
		//119电路
		GPIO_InitStruct.pin = FL_GPIO_PIN_12;
    GPIO_InitStruct.mode = FL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.pull = FL_DISABLE;
    GPIO_InitStruct.remapPin = FL_DISABLE;
    GPIO_InitStruct.analogSwitch = FL_DISABLE;
    FL_GPIO_Init(GPIOB, &GPIO_InitStruct);
		//工位检测
		//工位4 PE0
		GPIO_InitStruct.pin = FL_GPIO_PIN_0;
    GPIO_InitStruct.mode = FL_GPIO_MODE_INPUT;
    GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.pull = FL_ENABLE;
    GPIO_InitStruct.remapPin = FL_DISABLE;
    GPIO_InitStruct.analogSwitch = FL_DISABLE;
    FL_GPIO_Init(GPIOE, &GPIO_InitStruct);
		//工位3 PE1
		GPIO_InitStruct.pin = FL_GPIO_PIN_1;
    GPIO_InitStruct.mode = FL_GPIO_MODE_INPUT;
    GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.pull = FL_ENABLE;
    GPIO_InitStruct.remapPin = FL_DISABLE;
    GPIO_InitStruct.analogSwitch = FL_DISABLE;
    FL_GPIO_Init(GPIOE, &GPIO_InitStruct);
		//工位2 PE2
		GPIO_InitStruct.pin = FL_GPIO_PIN_2;
    GPIO_InitStruct.mode = FL_GPIO_MODE_INPUT;
    GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.pull = FL_ENABLE;
    GPIO_InitStruct.remapPin = FL_DISABLE;
    GPIO_InitStruct.analogSwitch = FL_DISABLE;
    FL_GPIO_Init(GPIOE, &GPIO_InitStruct);
		//工位1 PE3
		GPIO_InitStruct.pin = FL_GPIO_PIN_3;
    GPIO_InitStruct.mode = FL_GPIO_MODE_INPUT;
    GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.pull = FL_ENABLE;
    GPIO_InitStruct.remapPin = FL_DISABLE;
    GPIO_InitStruct.analogSwitch = FL_DISABLE;
    FL_GPIO_Init(GPIOE, &GPIO_InitStruct);
		//检测升压电路电压
	   GPIO_InitStruct.pin           = FL_GPIO_PIN_4;
	   GPIO_InitStruct.mode = FL_GPIO_MODE_INPUT;                                         /*配置GPIO的功能模式*/
	   GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_OPENDRAIN;                               /*配置GPIO的输出模式*/
	   GPIO_InitStruct.pull = FL_ENABLE;                                                  /*配置GPIO输上拉模式*/
	   GPIO_InitStruct.remapPin = FL_DISABLE;                                              /*配置GPIO数字重定向功能*/
	   GPIO_InitStruct.analogSwitch = FL_DISABLE;                                          /*配置GPIO模拟开关功能*/
	   FL_GPIO_Init(GPIOD, &GPIO_InitStruct);
		
		//上电先关灯
		LED_Off();
		//主电源供电断开
		zhudian_gongdian_OFF();
		//备电电源断开
		beidian_gongdian_OFF();
		//主电电压检测控制关闭
		zhudian_dianya_CHK_CTRL_OFF();
		//二级电源电压检测控制关闭
		erji_dianya_CHK_CTRL_OFF();
		//VCC电压检测控制关闭
		VCC_dianya_CHK_CTRL_OFF();
		//升压电压检测控制关闭
		SY_dianya_CHK_CTRL_OFF();
		//电流检测使能关闭
		Current_CHK_CTRL_OFF();
		//主控板串口使能关闭
		Uart_shineng_OFF();
		//按键一抬起
    ANJIAN_1_OFF();
		//按键二抬起
    ANJIAN_2_OFF();
		//按键三抬起
    ANJIAN_3_OFF();
		//按键四抬起
    ANJIAN_4_OFF();
		//119不触发
		dianlu_119_OFF();
		//功耗检测的IIC初始化
		INA219_IIC_GPIO_Init();
}
//LED开
void LED_On()
{
	//uint8_t buf[2] = {0,1};
	FL_GPIO_ResetOutputPin(GPIOA,FL_GPIO_PIN_8);

}
//LED关
void LED_Off()
{
	FL_GPIO_SetOutputPin(GPIOA,FL_GPIO_PIN_8);
}
//主控板主电供电开
void zhudian_gongdian_On()
{
	FL_GPIO_SetOutputPin(GPIOA,FL_GPIO_PIN_4);
}
//主控板主电供电关
void zhudian_gongdian_OFF()
{
	FL_GPIO_ResetOutputPin(GPIOA,FL_GPIO_PIN_4);
}
//主控板备电供电开
void beidian_gongdian_On()
{
	FL_GPIO_SetOutputPin(GPIOA,FL_GPIO_PIN_5);
}
//主控板备电供电关
void beidian_gongdian_OFF()
{
	FL_GPIO_ResetOutputPin(GPIOA,FL_GPIO_PIN_5);
}
//主控板主电电压检测使能开
void zhudian_dianya_CHK_CTRL_ON()
{
	FL_GPIO_SetOutputPin(GPIOB,FL_GPIO_PIN_2);
}
//主控板主电电压检测使能关
void zhudian_dianya_CHK_CTRL_OFF()
{
	FL_GPIO_ResetOutputPin(GPIOB,FL_GPIO_PIN_2);
}
//主控板二级电压检测使能开
void erji_dianya_CHK_CTRL_ON()
{
	FL_GPIO_SetOutputPin(GPIOA,FL_GPIO_PIN_9);
}
//主控板二级电压检测使能关
void erji_dianya_CHK_CTRL_OFF()
{
	FL_GPIO_ResetOutputPin(GPIOA,FL_GPIO_PIN_9);
}
//主控板VCC电压检测使能开
void VCC_dianya_CHK_CTRL_ON()
{
	FL_GPIO_SetOutputPin(GPIOB,FL_GPIO_PIN_6);
}
//主控板VCC电压检测使能关
void VCC_dianya_CHK_CTRL_OFF()
{
	FL_GPIO_ResetOutputPin(GPIOB,FL_GPIO_PIN_6);
}
//主控板升压电压检测使能开
void SY_dianya_CHK_CTRL_ON()
{
	FL_GPIO_SetOutputPin(GPIOB,FL_GPIO_PIN_7);
}
//主控板升压电压检测使能关
void SY_dianya_CHK_CTRL_OFF()
{
	FL_GPIO_ResetOutputPin(GPIOB,FL_GPIO_PIN_7);
}
//主控板电流检测使能开
void Current_CHK_CTRL_ON()
{
	FL_GPIO_ResetOutputPin(GPIOC,FL_GPIO_PIN_7);
}
//主控板电流检测使能关
void Current_CHK_CTRL_OFF()
{
	FL_GPIO_SetOutputPin(GPIOC,FL_GPIO_PIN_7);
}
//主控板串口使能开
void Uart_shineng_ON()
{
	FL_GPIO_SetOutputPin(GPIOA,FL_GPIO_PIN_15);
}
//主控板串口使能关
void Uart_shineng_OFF()
{
	FL_GPIO_ResetOutputPin(GPIOA,FL_GPIO_PIN_15);
}
//按键一抬起
void ANJIAN_1_OFF()
{
	FL_GPIO_SetOutputPin(GPIOB,FL_GPIO_PIN_8);
}
//按键一按下
void ANJIAN_1_ON()
{
	FL_GPIO_ResetOutputPin(GPIOB,FL_GPIO_PIN_8);
}
//按键二抬起
void ANJIAN_2_OFF()
{
	FL_GPIO_SetOutputPin(GPIOB,FL_GPIO_PIN_9);
}
//按键二按下
void ANJIAN_2_ON()
{
	FL_GPIO_ResetOutputPin(GPIOB,FL_GPIO_PIN_9);
}
//按键三抬起
void ANJIAN_3_OFF()
{
	FL_GPIO_SetOutputPin(GPIOB,FL_GPIO_PIN_10);
}
//按键三按下
void ANJIAN_3_ON()
{
	FL_GPIO_ResetOutputPin(GPIOB,FL_GPIO_PIN_10);
}
//按键四抬起
void ANJIAN_4_OFF()
{
	FL_GPIO_SetOutputPin(GPIOB,FL_GPIO_PIN_11);
}
//按键四按下
void ANJIAN_4_ON()
{
	FL_GPIO_ResetOutputPin(GPIOB,FL_GPIO_PIN_11);
}
//119电路不触发
void dianlu_119_OFF()
{
	FL_GPIO_SetOutputPin(GPIOB,FL_GPIO_PIN_12);
}
//119电路触发
void dianlu_119_ON()
{
	FL_GPIO_ResetOutputPin(GPIOB,FL_GPIO_PIN_12);
}
//升压状态获取
int getv4state(void)
{
	if(FL_GPIO_GetInputPin(GPIOD, FL_GPIO_PIN_4)==1)
		return 1;
  else
		return 0;
}
/*
//GPIO中断
void MF_EXTI_Init(void)
{
    FL_EXTI_CommonInitTypeDef    EXTI_InitStruct;

    EXTI_InitStruct.clockSource = FL_CMU_EXTI_CLK_SOURCE_HCLK;

    (void)FL_EXTI_CommonInit(&EXTI_InitStruct);
}
void MF_EXTI_Line2_Init(void)
{
    FL_GPIO_InitTypeDef    GPIO_InitStruct;

    FL_EXTI_InitTypeDef    EXTI_InitStruct;

    GPIO_InitStruct.pin          = FL_GPIO_PIN_10;
    GPIO_InitStruct.mode         = FL_GPIO_MODE_INPUT;
    GPIO_InitStruct.outputType   = FL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.pull         = FL_ENABLE;
    GPIO_InitStruct.remapPin     = FL_DISABLE;
    GPIO_InitStruct.analogSwitch = FL_DISABLE;
    (void)FL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    EXTI_InitStruct.filter      = FL_ENABLE;
    EXTI_InitStruct.input       = FL_GPIO_EXTI_INPUT_GROUP2;
    EXTI_InitStruct.triggerEdge = FL_GPIO_EXTI_TRIGGER_EDGE_FALLING;

    (void)FL_EXTI_Init(FL_GPIO_EXTI_LINE_2, &EXTI_InitStruct);
}
void MF_NVIC_Init(void)
{
    FL_NVIC_ConfigTypeDef    InterruptConfigStruct;

    InterruptConfigStruct.preemptPriority = 0x02;
    FL_NVIC_Init(&InterruptConfigStruct, GPIO_IRQn);
}

void MF_EXTI_Config_Init(void)
{
    MF_EXTI_Init();
    MF_EXTI_Line2_Init();
    MF_NVIC_Init();
}
*/

void GPIO_IRQHandler(void)
{
    if(FL_GPIO_IsActiveFlag_EXTI(GPIO, FL_GPIO_EXTI_LINE_2) == 0x01UL)
    {
        FL_GPIO_ClearFlag_EXTI(GPIO, FL_GPIO_EXTI_LINE_2);
    }
}

