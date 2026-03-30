#ifndef _MYDEFINE_H_
#define _MYDEFINE_H_
#include "r_cg_macrodriver.h"

#define BIT0             0x01
#define BIT1             0x02
#define BIT2             0x04
#define BIT3             0x08
#define BIT4             0x10
#define BIT5             0x20
#define BIT6             0x40
#define BIT7             0x80

#define TRUE              1
#define FALSE             0

#define Ml_Uint 1800000000.0
#define M3_Uint 1800000000000000.0
#define M3_1_Uint 180000000000000.0

void Delay_us(unsigned int us);
void Delay_ms(unsigned int ms);
#endif