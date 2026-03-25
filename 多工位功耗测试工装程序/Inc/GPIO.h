#ifndef __GPIO_H__
#define __GPIO_H__
#include "main.h"

void INA219_IIC_INIT(void);
void Others_GPIO_Init(void);
void LED_On(void);
void LED_Off(void);
void MF_EXTI_Config_Init(void);
void GPIO_IRQHandler(void);
#endif
