#ifndef __GPIO__H
#define __GPIO__H
#include "main.h"

#define On 1
#define Off 2
#define High 3
#define Low 4
#define AD1 5
#define AD2 6
#define AD3 7
#define AD4 8

void MF_GPIO_Init(void);

void Current_3V6_I_CHK_CTL(uint8_t State);
void Current_7V2_I_CHK_CTL(uint8_t State);
void Output_3V6_CTL(uint8_t State);
void Output_7V2_CTL(uint8_t State);
void Output_12V_CTL(uint8_t State);
void ADC_CHK_CTL(uint8_t Num,uint8_t State);
void Ext_LED_CTL(uint8_t State);
void A119_CTL(uint8_t State);
void Brownout_detect(uint8_t State);
/*————————12V电磁铁电源输出控制————————*/
void Output_12V_CTL(uint8_t State);
/*————————12V电磁铁2输出控制————————*/
void Output_12V_CTL1(uint8_t State);
void Output_12V_CTL2(uint8_t State);
#endif 
