#pragma interrupt INTRTC r_rtc_interrupt
#pragma interrupt INTTM00 r_tau0_channel0_interrupt
#pragma interrupt INTTM01 r_tau0_channel1_interrupt
#pragma interrupt INTTM02 r_tau0_channel2_interrupt
//#pragma interrupt INTTM03 r_tau0_channel3_interrupt
#pragma interrupt INTTM04 r_tau0_channel4_interrupt
#include "Timer.h"
#include "r_cg_rtc.h"
#include "r_cg_tau.h"
#include "r_cg_sau.h"
#include "Struct.h"
#include "IOIntp.h"
#include "OneShot_AD.h"
#include "User.h"
void Timer_Init()
{
        R_RTC_Create();
        R_RTC_Start();
        R_TAU0_Create();//chu shi hua ding shi qi zhen lie
        Stop_Timer(UMU_TIMER);
        Stop_Timer(ZJ_TIMER);
	Stop_Timer(IRDA_TIMER);
	Stop_Timer(UMU_TIMER);
}
void Start_Timer(_TIMER_Def T_TIMER)
{
  switch(T_TIMER)
  {
  case ZJ_TIMER:
    R_TAU0_Channel0_Start();
    break;
	case CARD_TIMER:
		R_TAU0_Channel1_Start();
		break;
	case UMU_TIMER:
		R_TAU0_Channel2_Start();
		break;
	case IRDA_TIMER:
		R_TAU0_Channel4_Start();
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
	case CARD_TIMER:
		R_TAU0_Channel1_Stop();
		break;
	case UMU_TIMER:
		R_TAU0_Channel2_Stop();
		break;
	case IRDA_TIMER:
		R_TAU0_Channel4_Stop();
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
	if(Csb_Timer.CARDTimer < 5)
  {
    Csb_Timer.CARDTimer++;
  }
  else
  {
    CARD_EN(0);
    Stop_Timer(CARD_TIMER);
    Flag.CARDData=1;
  }
}
/*********************************************
          TIMER02____interrupt;KEY2
**********************************************/
__interrupt static void r_tau0_channel2_interrupt(void)
{
	if(Csb_Timer.UMUTimer < 5)
  {
    Csb_Timer.UMUTimer++;
  }
  else
  {
    UMU_EN(0);
    Stop_Timer(UMU_TIMER);
		if(TempUserData.UMUBuffLen >=1)
		{
			Flag.UMUData=1;
		}
  }
}
/*********************************************
          TIMER04____interrupt;RF
**********************************************/
__interrupt static void r_tau0_channel4_interrupt(void)//0.5s
{
	if(Csb_Timer.IRDATimer < 4)
  {
    Csb_Timer.IRDATimer++;
  }
  else
  {
    Stop_Timer(IRDA_TIMER);
    IRDA_EN(0);
    Flag.IRDAData=1;
  }
}
/*********************************************
          RTC____interrupt
**********************************************/
__interrupt static void r_rtc_interrupt(void)
{
  Flag.Rtc_Come=1;
  Flag.Rtc_time++;
}