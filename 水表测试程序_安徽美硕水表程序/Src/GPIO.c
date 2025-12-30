#include "GPIO.h"
#include "ZDINA219.h"
void Others_GPIO_Init()
{
	  //LED指示灯 PB4
	  FL_GPIO_InitTypeDef    GPIO_InitStruct;
    GPIO_InitStruct.pin = FL_GPIO_PIN_4;
    GPIO_InitStruct.mode = FL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.pull = FL_DISABLE;
    GPIO_InitStruct.remapPin = FL_DISABLE;
    GPIO_InitStruct.analogSwitch = FL_DISABLE;
    FL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	  //主电源供电引脚 PC5
    GPIO_InitStruct.pin = FL_GPIO_PIN_5;
    GPIO_InitStruct.mode = FL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.pull = FL_DISABLE;
    GPIO_InitStruct.remapPin = FL_DISABLE;
    GPIO_InitStruct.analogSwitch = FL_DISABLE;
    FL_GPIO_Init(GPIOC, &GPIO_InitStruct);
	
	  //NB电源引脚PC4
	  GPIO_InitStruct.pin = FL_GPIO_PIN_4;
    GPIO_InitStruct.mode = FL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.pull = FL_DISABLE;
    GPIO_InitStruct.remapPin = FL_DISABLE;
    GPIO_InitStruct.analogSwitch = FL_DISABLE;
    FL_GPIO_Init(GPIOC, &GPIO_InitStruct);
		
		//主电电压检测控制 PE0
		GPIO_InitStruct.pin = FL_GPIO_PIN_0;
    GPIO_InitStruct.mode = FL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.pull = FL_DISABLE;
    GPIO_InitStruct.remapPin = FL_DISABLE;
    GPIO_InitStruct.analogSwitch = FL_DISABLE;
    FL_GPIO_Init(GPIOE, &GPIO_InitStruct);
		
		//NB电压检测控制 PE1
		GPIO_InitStruct.pin = FL_GPIO_PIN_1;
    GPIO_InitStruct.mode = FL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.pull = FL_DISABLE;
    GPIO_InitStruct.remapPin = FL_DISABLE;
    GPIO_InitStruct.analogSwitch = FL_DISABLE;
    FL_GPIO_Init(GPIOE, &GPIO_InitStruct);
		
		//主电电流检测使能 PE3
		GPIO_InitStruct.pin = FL_GPIO_PIN_3;
    GPIO_InitStruct.mode = FL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.pull = FL_DISABLE;
    GPIO_InitStruct.remapPin = FL_DISABLE;
    GPIO_InitStruct.analogSwitch = FL_DISABLE;
    FL_GPIO_Init(GPIOE, &GPIO_InitStruct);
		
		//备电电流检测使能 PE2
		GPIO_InitStruct.pin = FL_GPIO_PIN_2;
    GPIO_InitStruct.mode = FL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.pull = FL_DISABLE;
    GPIO_InitStruct.remapPin = FL_DISABLE;
    GPIO_InitStruct.analogSwitch = FL_DISABLE;
    FL_GPIO_Init(GPIOE, &GPIO_InitStruct);
		
		//霍尔1控制 PC3
		GPIO_InitStruct.pin = FL_GPIO_PIN_3;
    GPIO_InitStruct.mode = FL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.pull = FL_DISABLE;
    GPIO_InitStruct.remapPin = FL_DISABLE;
    GPIO_InitStruct.analogSwitch = FL_DISABLE;
    FL_GPIO_Init(GPIOC, &GPIO_InitStruct);
		
		//霍尔2控制 PC2
		GPIO_InitStruct.pin = FL_GPIO_PIN_2;
    GPIO_InitStruct.mode = FL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.pull = FL_DISABLE;
    GPIO_InitStruct.remapPin = FL_DISABLE;
    GPIO_InitStruct.analogSwitch = FL_DISABLE;
    FL_GPIO_Init(GPIOC, &GPIO_InitStruct);
		
		
		//119电路
		GPIO_InitStruct.pin = FL_GPIO_PIN_12;
    GPIO_InitStruct.mode = FL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.pull = FL_DISABLE;
    GPIO_InitStruct.remapPin = FL_DISABLE;
    GPIO_InitStruct.analogSwitch = FL_DISABLE;
    FL_GPIO_Init(GPIOB, &GPIO_InitStruct);
		
		//按键电路控制
		GPIO_InitStruct.pin = FL_GPIO_PIN_1;
    GPIO_InitStruct.mode = FL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_OPENDRAIN;
    GPIO_InitStruct.pull = FL_DISABLE;
    GPIO_InitStruct.remapPin = FL_DISABLE;
    GPIO_InitStruct.analogSwitch = FL_DISABLE;
    FL_GPIO_Init(GPIOD, &GPIO_InitStruct);
		
		//计量IO1初始化
		GPIO_InitStruct.pin = FL_GPIO_PIN_3;
    GPIO_InitStruct.mode = FL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.pull = FL_DISABLE;
    GPIO_InitStruct.remapPin = FL_DISABLE;
    GPIO_InitStruct.analogSwitch = FL_DISABLE;
    FL_GPIO_Init(GPIOA, &GPIO_InitStruct);
		
		//计量IO2初始化
		GPIO_InitStruct.pin = FL_GPIO_PIN_2;
    GPIO_InitStruct.mode = FL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.pull = FL_DISABLE;
    GPIO_InitStruct.remapPin = FL_DISABLE;
    GPIO_InitStruct.analogSwitch = FL_DISABLE;
    FL_GPIO_Init(GPIOA, &GPIO_InitStruct);
		
		//工位选择1 PC0
		GPIO_InitStruct.pin = FL_GPIO_PIN_0;
    GPIO_InitStruct.mode = FL_GPIO_MODE_INPUT;
    GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.pull = FL_ENABLE;
    GPIO_InitStruct.remapPin = FL_DISABLE;
    GPIO_InitStruct.analogSwitch = FL_DISABLE;
    FL_GPIO_Init(GPIOC, &GPIO_InitStruct);
		
		//工位选择2 PD12
		GPIO_InitStruct.pin = FL_GPIO_PIN_12;
    GPIO_InitStruct.mode = FL_GPIO_MODE_INPUT;
    GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.pull = FL_ENABLE;
    GPIO_InitStruct.remapPin = FL_DISABLE;
    GPIO_InitStruct.analogSwitch = FL_DISABLE;
    FL_GPIO_Init(GPIOD, &GPIO_InitStruct);
		
		//工位选择3 PB15
		GPIO_InitStruct.pin = FL_GPIO_PIN_15;
    GPIO_InitStruct.mode = FL_GPIO_MODE_INPUT;
    GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.pull = FL_ENABLE;
    GPIO_InitStruct.remapPin = FL_DISABLE;
    GPIO_InitStruct.analogSwitch = FL_DISABLE;
    FL_GPIO_Init(GPIOB, &GPIO_InitStruct);
		
		//工位选择4 PE5
		GPIO_InitStruct.pin = FL_GPIO_PIN_5;
    GPIO_InitStruct.mode = FL_GPIO_MODE_INPUT;
    GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.pull = FL_ENABLE;
    GPIO_InitStruct.remapPin = FL_DISABLE;
    GPIO_InitStruct.analogSwitch = FL_DISABLE;
    FL_GPIO_Init(GPIOE, &GPIO_InitStruct);
		
		
		//开到位 PA12
		GPIO_InitStruct.pin = FL_GPIO_PIN_12;
    GPIO_InitStruct.mode = FL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.pull = FL_DISABLE;
    GPIO_InitStruct.remapPin = FL_DISABLE;
    GPIO_InitStruct.analogSwitch = FL_DISABLE;
    FL_GPIO_Init(GPIOA, &GPIO_InitStruct);
		
		//关到位 PA11
		GPIO_InitStruct.pin = FL_GPIO_PIN_11;
    GPIO_InitStruct.mode = FL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.pull = FL_DISABLE;
    GPIO_InitStruct.remapPin = FL_DISABLE;
    GPIO_InitStruct.analogSwitch = FL_DISABLE;
    FL_GPIO_Init(GPIOA, &GPIO_InitStruct);
		
		
		//上电先关灯
		LED_Off();
		//按键设置为开漏
		anjian_kongzhi_OFF();
		//主电源供电断开
		zhudian_gongdian_OFF();
		//备电电源断开
		beidian_gongdian_OFF();
		//主电电压检测控制关闭
		zhudian_dianya_CHK_CTRL_OFF();
		//二级电源电压检测控制关闭
		erji_dianya_CHK_CTRL_OFF();
		//电流检测使能关闭
		Current_CHK_CTRL_OFF();
		//霍尔1控制 关
		huuoer_1_OFF();
		//霍尔2控制 关
		huuoer_2_OFF();
		//计量IO1置高
    jiliang_IO1_set_Hi();
		//计量IO2置高
		jiliang_IO2_set_Hi();
		//功耗检测的IIC初始化
		INA219_IIC_GPIO_Init();
		//开到位置高
    kaidaowei_set_Hi();
		//关到位置低
		guandaowei_set_Low();
}
//LED开
void LED_On()
{
	//uint8_t buf[2] = {0,1};
	FL_GPIO_ResetOutputPin(GPIOB,FL_GPIO_PIN_4);
}
//LED关
void LED_Off()
{
	FL_GPIO_SetOutputPin(GPIOB,FL_GPIO_PIN_4);
}
//主控板主电供电开
void zhudian_gongdian_On()
{
	FL_GPIO_SetOutputPin(GPIOC,FL_GPIO_PIN_5);
}
//主控板主电供电关
void zhudian_gongdian_OFF()
{
	FL_GPIO_ResetOutputPin(GPIOC,FL_GPIO_PIN_5);
}
//主控板NB电源供电开
void beidian_gongdian_On()
{
	FL_GPIO_SetOutputPin(GPIOC,FL_GPIO_PIN_4);
}
//主控板NB电源供电关
void beidian_gongdian_OFF()
{
	FL_GPIO_ResetOutputPin(GPIOC,FL_GPIO_PIN_4);
}

//主控板主电电压检测使能开
void zhudian_dianya_CHK_CTRL_ON()
{
	FL_GPIO_SetOutputPin(GPIOE,FL_GPIO_PIN_0);
}
//主控板主电电压检测使能关
void zhudian_dianya_CHK_CTRL_OFF()
{
	FL_GPIO_ResetOutputPin(GPIOE,FL_GPIO_PIN_0);
}
//主控板NB电压检测使能开
void erji_dianya_CHK_CTRL_ON()
{
	FL_GPIO_SetOutputPin(GPIOE,FL_GPIO_PIN_1);
}
//主控板NB电压检测使能关
void erji_dianya_CHK_CTRL_OFF()
{
	FL_GPIO_ResetOutputPin(GPIOE,FL_GPIO_PIN_1);
}

//主电电流检测使能开
void Current_CHK_CTRL_ON()
{
	FL_GPIO_ResetOutputPin(GPIOE,FL_GPIO_PIN_3);
}
//主电电流检测使能关
void Current_CHK_CTRL_OFF()
{
	FL_GPIO_SetOutputPin(GPIOE,FL_GPIO_PIN_3);
}
//NB电流检测使能开
void NB_Current_CHK_CTRL_ON()
{
	FL_GPIO_ResetOutputPin(GPIOE,FL_GPIO_PIN_2);
}
//NB电流检测使能开
void NB_Current_CHK_CTRL_OFF()
{
	FL_GPIO_ResetOutputPin(GPIOE,FL_GPIO_PIN_2);
}

//霍尔1控制 开
void huuoer_1_ON()
{
	FL_GPIO_SetOutputPin(GPIOC,FL_GPIO_PIN_3);
}
//霍尔1控制 关
void huuoer_1_OFF()
{
	FL_GPIO_ResetOutputPin(GPIOC,FL_GPIO_PIN_3);
}
//霍尔2控制 开
void huuoer_2_ON()
{
	FL_GPIO_SetOutputPin(GPIOC,FL_GPIO_PIN_2);
}
//霍尔2控制 关
void huuoer_2_OFF()
{
	FL_GPIO_ResetOutputPin(GPIOC,FL_GPIO_PIN_2);
}
//按键控制 开(拉低)
void anjian_kongzhi_ON()
{
	FL_GPIO_ResetOutputPin(GPIOD,FL_GPIO_PIN_1);
}
//按键控制 关(拉高)
void anjian_kongzhi_OFF()
{
	FL_GPIO_SetOutputPin(GPIOD,FL_GPIO_PIN_1);
}
//计量IO1置高
void jiliang_IO1_set_Hi()
{
	FL_GPIO_SetOutputPin(GPIOA,FL_GPIO_PIN_3);
}
//计量IO1置低
void jiliang_IO1_set_Low()
{
	FL_GPIO_ResetOutputPin(GPIOA,FL_GPIO_PIN_3);
}
//计量IO2置高
void jiliang_IO2_set_Hi()
{
	FL_GPIO_SetOutputPin(GPIOA,FL_GPIO_PIN_2);
}
//计量IO2置低
void jiliang_IO2_set_Low()
{
	FL_GPIO_ResetOutputPin(GPIOA,FL_GPIO_PIN_2);
}

//开到位置高
void kaidaowei_set_Hi()
{
	FL_GPIO_SetOutputPin(GPIOA,FL_GPIO_PIN_11);
}
//开到位置低
void kaidaowei_set_Low()
{
	FL_GPIO_ResetOutputPin(GPIOA,FL_GPIO_PIN_11);
}
//关到位置高
void guandaowei_set_Hi()
{
	FL_GPIO_SetOutputPin(GPIOA,FL_GPIO_PIN_12);
}
//关到位置低
void guandaowei_set_Low()
{
	FL_GPIO_ResetOutputPin(GPIOA,FL_GPIO_PIN_12);
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

