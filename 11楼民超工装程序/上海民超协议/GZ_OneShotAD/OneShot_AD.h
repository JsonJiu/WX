#ifndef _ONESHOT_AD_H_
#define _ONESHOT_AD_H_
#include "r_cg_macrodriver.h"
#include "Mydefine.h"
///////AD_Power1µç³Ø
#define AD_POWER_PIN_OUT     (P2)
#define AD_POWER_PIN_PORT    (BIT4)
#define AD_POWER_HI          (AD_POWER_PIN_OUT |=AD_POWER_PIN_PORT)
#define AD_POWER_LO          (AD_POWER_PIN_OUT &=~AD_POWER_PIN_PORT)
#define AD_POWER(a)          (a)?(AD_POWER_HI):(AD_POWER_LO)

#define RF_AD_POWER_PIN_OUT     (P2)
#define RF_AD_POWER_PIN_PORT    (BIT7)
#define RF_AD_POWER_HI          (RF_AD_POWER_PIN_OUT |=RF_AD_POWER_PIN_PORT)
#define RF_AD_POWER_LO          (RF_AD_POWER_PIN_OUT &=~RF_AD_POWER_PIN_PORT)
#define RF_AD_POWER(a)          (a)?(RF_AD_POWER_HI):(RF_AD_POWER_LO)

void AD_Init();
unsigned long CheckPower(unsigned char adtype);
#endif