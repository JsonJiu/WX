#include "pulse.h"

extern uint8_t pulse_forward_count;
extern uint8_t pulse_back_count;

void MF_EXTI_Common_Init(void)
{
    FL_EXTI_CommonInitTypeDef Common_InitStruct;

    Common_InitStruct.clockSource = FL_CMU_EXTI_CLK_SOURCE_HCLK;

    FL_EXTI_CommonInit(&Common_InitStruct);
}

void MF_EXTI_Line12_Init(void)
{
    FL_GPIO_InitTypeDef GPIO_InitStruct;

    FL_EXTI_InitTypeDef EXTI_InitStruct;

    /* PD1 EXTI12_Select1 */
    GPIO_InitStruct.pin = FL_GPIO_PIN_1;
    GPIO_InitStruct.mode = FL_GPIO_MODE_INPUT;
    GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.pull = FL_ENABLE;
    GPIO_InitStruct.remapPin = FL_DISABLE;
    GPIO_InitStruct.analogSwitch = FL_DISABLE;
    FL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    EXTI_InitStruct.filter = FL_DISABLE;
    EXTI_InitStruct.input = FL_GPIO_EXTI_INPUT_GROUP1;
    EXTI_InitStruct.triggerEdge = FL_GPIO_EXTI_TRIGGER_EDGE_RISING;

    FL_EXTI_Init(FL_GPIO_EXTI_LINE_12, &EXTI_InitStruct);
}

void MF_EXTI_Line0_Init(void)
{
    FL_GPIO_InitTypeDef GPIO_InitStruct;

    FL_EXTI_InitTypeDef EXTI_InitStruct;

    /* PA3 EXTI0_Select3 */
    GPIO_InitStruct.pin = FL_GPIO_PIN_3;
    GPIO_InitStruct.mode = FL_GPIO_MODE_INPUT;
    GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.pull = FL_ENABLE;
    GPIO_InitStruct.remapPin = FL_DISABLE;
    GPIO_InitStruct.analogSwitch = FL_DISABLE;
    FL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    EXTI_InitStruct.filter = FL_DISABLE;
    EXTI_InitStruct.input = FL_GPIO_EXTI_INPUT_GROUP3;
    EXTI_InitStruct.triggerEdge = FL_GPIO_EXTI_TRIGGER_EDGE_RISING;

    FL_EXTI_Init(FL_GPIO_EXTI_LINE_0, &EXTI_InitStruct);
}

void MF_NVIC_Init(void)
{
    FL_NVIC_ConfigTypeDef InterruptConfigStruct;

    InterruptConfigStruct.preemptPriority = 0x00;
    FL_NVIC_Init(&InterruptConfigStruct, GPIO_IRQn);
}

void MF_EXTI_Init(void)
{
    /* Initial EXTI */
    MF_EXTI_Common_Init();
    MF_EXTI_Line12_Init();
    MF_EXTI_Line0_Init();
    /* Initial NVIC */
    MF_NVIC_Init();
}

/*引脚中断处理函数*/
void GPIO_IRQHandler(void)
{
    if(FL_GPIO_IsActiveFlag_EXTI(GPIO, FL_GPIO_EXTI_LINE_0) == 0x01UL)
    {
        FL_GPIO_ClearFlag_EXTI(GPIO, FL_GPIO_EXTI_LINE_0);
        pulse_forward_count++;
    }
		if(FL_GPIO_IsActiveFlag_EXTI(GPIO, FL_GPIO_EXTI_LINE_12) == 0x01UL)
    {
        FL_GPIO_ClearFlag_EXTI(GPIO, FL_GPIO_EXTI_LINE_12);
        pulse_back_count++;
    }
}
