#ifndef __GPIO_H__
#define __GPIO_H__
#include "main.h"

#define On 1
#define Off 2

void Main_Voltage_Output_Ctl(uint8_t State);
void Standby_Voltage_Output_Ctl(uint8_t State);
void Main_Current_CHK_CTL(uint8_t State);
void Standby_Current_CHK_CTL(uint8_t State);
void Output_12V_CTL1(uint8_t State);
void Output_12V_CTL2(uint8_t State);
void User_GPIO_Init(void);

#endif
