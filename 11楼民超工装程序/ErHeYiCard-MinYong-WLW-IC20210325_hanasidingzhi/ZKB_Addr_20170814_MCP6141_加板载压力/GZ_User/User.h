#ifndef _USER_H_
#define _USER_H_
#include "r_cg_macrodriver.h"
#include "Mydefine.h"

/*#define UnderVolt_PIN_OUT     (P0)
#define UnderVolt_PIN_PORT    (BIT5)
#define UnderVolt_HI          (UnderVolt_PIN_OUT |=UnderVolt_PIN_PORT)
#define UnderVolt_LO          (UnderVolt_PIN_OUT &=~UnderVolt_PIN_PORT)
#define UnderVolt_EN(a)       (a)?(UnderVolt_HI):(UnderVolt_LO)*/

#define SuperCAPLink_PIN_OUT     (P1)
#define SuperCAPLink_PIN_PORT    (BIT7)
#define SuperCAPLink_HI          (SuperCAPLink_PIN_OUT |=SuperCAPLink_PIN_PORT)
#define SuperCAPLink_LO          (SuperCAPLink_PIN_OUT &=~SuperCAPLink_PIN_PORT)
#define SuperCAPLink_EN(a)       (a)?(SuperCAPLink_HI):(SuperCAPLink_LO)

#define SuperCAPFD_PIN_OUT     (P5)
#define SuperCAPFD_PIN_PORT    (BIT0)
#define SuperCAPFD_HI          (SuperCAPFD_PIN_OUT |=SuperCAPFD_PIN_PORT)
#define SuperCAPFD_LO          (SuperCAPFD_PIN_OUT &=~SuperCAPFD_PIN_PORT)
#define SuperCAPFD_EN(a)       (a)?(SuperCAPFD_HI):(SuperCAPFD_LO)

#define ZDNoCurrent_PIN_OUT     (P5)
#define ZDNoCurrent_PIN_PORT    (BIT1)
#define ZDNoCurrent_HI          (ZDNoCurrent_PIN_OUT |=ZDNoCurrent_PIN_PORT)
#define ZDNoCurrent_LO          (ZDNoCurrent_PIN_OUT &=~ZDNoCurrent_PIN_PORT)
#define ZDNoCurrent_EN(a)       (a)?(ZDNoCurrent_HI):(ZDNoCurrent_LO)

#define ZDOKCurrent_PIN_OUT     (P5)
#define ZDOKCurrent_PIN_PORT    (BIT5)
#define ZDOKCurrent_HI          (ZDOKCurrent_PIN_OUT |=ZDOKCurrent_PIN_PORT)
#define ZDOKCurrent_LO          (ZDOKCurrent_PIN_OUT &=~ZDOKCurrent_PIN_PORT)
#define ZDOKCurrent_EN(a)       (a)?(ZDOKCurrent_HI):(ZDOKCurrent_LO)

#define ZDBackUpDYCurrent_PIN_OUT     (P1)
#define ZDBackUpDYCurrent_PIN_PORT    (BIT1)
#define ZDBackUpDYCurrent_HI          (ZDBackUpDYCurrent_PIN_OUT |=ZDBackUpDYCurrent_PIN_PORT)
#define ZDBackUpDYCurrent_LO          (ZDBackUpDYCurrent_PIN_OUT &=~ZDBackUpDYCurrent_PIN_PORT)
#define ZDBackUpDYCurrent_EN(a)       (a)?(ZDBackUpDYCurrent_HI):(ZDBackUpDYCurrent_LO)


#define ZDBackUpDY36_PIN_OUT     (P0)
#define ZDBackUpDY36_PIN_PORT    (BIT0)
#define ZDBackUpDY36_HI          (ZDBackUpDY36_PIN_OUT |=ZDBackUpDY36_PIN_PORT)
#define ZDBackUpDY36_LO          (ZDBackUpDY36_PIN_OUT &=~ZDBackUpDY36_PIN_PORT)
#define ZDBackUpDY36_EN(a)       (a)?(ZDBackUpDY36_HI):(ZDBackUpDY36_LO)

#define CardCon1_PIN_OUT     (P4)
#define CardCon1_PIN_PORT    (BIT6)
#define CardCon1_HI          (CardCon1_PIN_OUT |=CardCon1_PIN_PORT)
#define CardCon1_LO          (CardCon1_PIN_OUT &=~CardCon1_PIN_PORT)
#define CardCon1_EN(a)       (a)?(CardCon1_HI):(CardCon1_LO)

//#define CardCon2_PIN_OUT     (P3)
//#define CardCon2_PIN_PORT    (BIT1)
//#define CardCon2_HI          (CardCon2_PIN_OUT |=CardCon2_PIN_PORT)
//#define CardCon2_LO          (CardCon2_PIN_OUT &=~CardCon2_PIN_PORT)
//#define CardCon2_EN(a)       (a)?(CardCon2_HI):(CardCon2_LO)

#define CardOpen_PIN_OUT     (P1)
#define CardOpen_PIN_PORT    (BIT6)
#define CardOpen_HI          (CardOpen_PIN_OUT |=CardOpen_PIN_PORT)
#define CardOpen_LO          (CardOpen_PIN_OUT &=~CardOpen_PIN_PORT)
#define CardOpen_EN(a)       (a)?(CardOpen_HI):(CardOpen_LO)

#define ShfitON_PIN_OUT     (P3)
#define ShfitON_PIN_PORT    (BIT2)
#define ShfitON_HI          (ShfitON_PIN_OUT |=ShfitON_PIN_PORT)
#define ShfitON_LO          (ShfitON_PIN_OUT &=~ShfitON_PIN_PORT)
#define ShfitON_EN(a)       (a)?(ShfitON_HI):(ShfitON_LO)

#define VavleR_PIN_OUT     (P2)
#define VavleR_PIN_PORT    (BIT2)
#define VavleR_InPut       (VavleR_PIN_OUT&VavleR_PIN_PORT)

#define VavleB_PIN_OUT     (P2)
#define VavleB_PIN_PORT    (BIT1)
#define VavleB_InPut        (VavleB_PIN_OUT&VavleB_PIN_PORT)

#define CAPIN_PIN_OUT     (P2)
#define CAPIN_PIN_PORT    (BIT0)
#define CAPIN_InPut       (CAPIN_PIN_OUT&CAPIN_PIN_PORT)

extern unsigned char SystemStart;
void CheckCard(unsigned char CARDEN);
void Check_ZJCMD();
void ScanRtc();
void IOInit();
#endif