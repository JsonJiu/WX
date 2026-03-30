#pragma interrupt INTRTC r_rtc_interrupt
#pragma interrupt INTTM00 r_tau0_channel0_interrupt
#pragma interrupt INTTM01 r_tau0_channel1_interrupt
//#pragma interrupt INTTM02 r_tau0_channel2_interrupt
//#pragma interrupt INTTM03 r_tau0_channel3_interrupt
//#pragma interrupt INTTM04 r_tau0_channel4_interrupt
#include "Timer.h"
#include "r_cg_rtc.h"
#include "r_cg_tau.h"
#include "r_cg_sau.h"
#include "Struct.h"
#include "IOIntp.h"
void Timer_Init()
{
  R_RTC_Create();
  R_RTC_Start();
  R_TAU0_Create();
  Stop_Timer(ZJ_TIMER);
  Stop_Timer(FJ_TIMER);
}
void Start_Timer(_TIMER_Def T_TIMER)
{
  switch(T_TIMER)
  {
        case ZJ_TIMER:
             R_TAU0_Channel0_Start();
             break;
	case FJ_TIMER:
             R_TAU0_Channel1_Start();
             break;
  default:
    break;
  }
}
void Stop_Timer(_TIMER_Def T_TIMER)
{
  switch(T_TIMER)
  {
  case ZJ_TIMER:
    R_TAU0_Channel0_Stop();
    break;
	case FJ_TIMER:
		R_TAU0_Channel1_Stop();
		break;
  default:
    break; 
  }
}
void Scan_Rtc()
{
  if(Flag.Rtc_Come == 1)
  {
    Flag.Rtc_Come = 0;
  }
}
/*********************************************
          TIMER00____interrupt;ZJ
**********************************************/
__interrupt static void r_tau0_channel0_interrupt(void)
{
  if(Csb_Timer.ZJTimer < 5)
  {
    Csb_Timer.ZJTimer++;
  }
  else
  {
    ZJ_EN(0);
    Stop_Timer(ZJ_TIMER);
    Flag.ZJData=1;
  }
}
/*********************************************
          TIMER01____interrupt;CARD
**********************************************/
__interrupt static void r_tau0_channel1_interrupt(void)
{
	if(Csb_Timer.FJTimer < 5)
  {
    Csb_Timer.FJTimer++;
  }
  else
  {
    FJ_EN(0);
    Stop_Timer(FJ_TIMER);
    Flag.FJData=1;
  }
}
/*********************************************
          RTC____interrupt
**********************************************/
__interrupt static void r_rtc_interrupt(void)
{
  Flag.Rtc_Come=1;
}