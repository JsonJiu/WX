#ifndef _TIMER_H_
#define _TIMER_H_
#include "r_cg_macrodriver.h"

typedef enum 
{
    ZJ_TIMER,  
    CARD_TIMER,
		IRDA_TIMER,
    UMU_TIMER,
}_TIMER_Def;
void Start_Timer(_TIMER_Def T_TIMER);
void Stop_Timer(_TIMER_Def T_TIMER);
void Timer_Init();
void Scan_Rtc();
#endif