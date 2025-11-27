#include "exti.h"

uint8_t EXTIFlag = 0;
void MF_EXTI_Common_Init(void)
{
    FL_EXTI_CommonInitTypeDef    Common_InitStruct;

    Common_InitStruct.clockSource = FL_CMU_EXTI_CLK_SOURCE_HCLK;

    FL_EXTI_CommonInit(&Common_InitStruct);    
}

void MF_EXTI_Line13_Init(void)
{
    FL_GPIO_InitTypeDef    GPIO_InitStruct;

    FL_EXTI_InitTypeDef    EXTI_InitStruct;

    /* PD4 EXTI13_Select0 */ 
    GPIO_InitStruct.pin = FL_GPIO_PIN_4;
    GPIO_InitStruct.mode = FL_GPIO_MODE_INPUT;
    GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.pull = FL_ENABLE;
    GPIO_InitStruct.remapPin = FL_DISABLE;
    GPIO_InitStruct.analogSwitch = FL_DISABLE;
    FL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    EXTI_InitStruct.filter = FL_DISABLE;
    EXTI_InitStruct.input = FL_GPIO_EXTI_INPUT_GROUP0;
    EXTI_InitStruct.triggerEdge = FL_GPIO_EXTI_TRIGGER_EDGE_RISING;

    FL_EXTI_Init(FL_GPIO_EXTI_LINE_13, &EXTI_InitStruct);    
}


void MF_NVIC_Init(void)
{
    FL_NVIC_ConfigTypeDef    InterruptConfigStruct;

    InterruptConfigStruct.preemptPriority = 0x00;
    FL_NVIC_Init(&InterruptConfigStruct, GPIO_IRQn);
}

void EXTI_Init(void)
{
    /* Initial EXTI */
    MF_EXTI_Common_Init();
    MF_EXTI_Line13_Init();

    /* Initial NVIC */
    MF_NVIC_Init();
}

void MF_Config_Init(void)
{
    /* Initial EXTI */
    MF_EXTI_Common_Init();
    MF_EXTI_Line13_Init();

    /* Initial NVIC */
    MF_NVIC_Init();
}

/*引脚中断处理函数*/
void GPIO_IRQHandler(void)
{
    if(FL_GPIO_IsActiveFlag_EXTI(GPIO, FL_GPIO_EXTI_LINE_13) == 0x01UL)
    {
        FL_GPIO_ClearFlag_EXTI(GPIO, FL_GPIO_EXTI_LINE_13);
        EXTIFlag = 1;
    }
}

