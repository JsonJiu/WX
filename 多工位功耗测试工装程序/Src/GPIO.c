#include "GPIO.h"
#include "ZDINA219.h"
void Others_GPIO_Init()
{
	  //LED指示灯 PC4
	  FL_GPIO_InitTypeDef    GPIO_InitStruct;
    GPIO_InitStruct.pin = FL_GPIO_PIN_4;
    GPIO_InitStruct.mode = FL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.pull = FL_DISABLE;
    GPIO_InitStruct.remapPin = FL_DISABLE;
    GPIO_InitStruct.analogSwitch = FL_DISABLE;
    FL_GPIO_Init(GPIOC, &GPIO_InitStruct);

		//上电先关灯
		LED_On();
		//功耗检测的IIC初始化
		INA219_IIC_GPIO_Init();
}
//LED开
void LED_On()
{
	//uint8_t buf[2] = {0,1};
	FL_GPIO_ResetOutputPin(GPIOC,FL_GPIO_PIN_4);

}
//LED关
void LED_Off()
{
	FL_GPIO_SetOutputPin(GPIOC,FL_GPIO_PIN_4);
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

