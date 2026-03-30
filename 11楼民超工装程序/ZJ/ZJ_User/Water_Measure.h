#ifndef _WaterMeasure_h
#define _WaterMeasure_h
#include "r_cg_macrodriver.h"
#include "Mydefine.h"

///////////////////PLUSE
#define PULSE_PIN_Out        (P1)
#define PULSE_PIN_Port       (BIT2)
#define PULSE_HI             (PULSE_PIN_Out |=PULSE_PIN_Port)
#define PULSE_LO             (PULSE_PIN_Out &=~PULSE_PIN_Port)
#define PULSE(a)             (a)?(PULSE_HI):(PULSE_LO)

///////////////////GuangDianPower
#define LXD_POWER_PIN_Out        (P12)
#define LXD_POWER_PIN_Port       (BIT7)
#define LXD_POWER_HI             (LXD_POWER_PIN_Out |=LXD_POWER_PIN_Port)
#define LXD_POWER_LO             (LXD_POWER_PIN_Out &=~LXD_POWER_PIN_Port)
#define LXD_POWER(a)             (a)?(LXD_POWER_HI):(LXD_POWER_LO)

///////////////////GuangDianIntpInPut
#define LXD_INPUT_PIN_Out        (P5)
#define LXD_INPUT_PIN_Port       (BIT2)
#define LXD_INPUT                (LXD_INPUT_PIN_Out&LXD_INPUT_PIN_Port)

#define Rref    10000.0
void Check_AM();
void WaterMeasure();
void ScanLXD();
#endif