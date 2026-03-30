#pragma interrupt INTP0 r_intc0_interrupt
//#pragma interrupt INTP1 r_intc1_interrupt
//#pragma interrupt INTP2 r_intc2_interrupt
#pragma interrupt INTP3 r_intc3_interrupt
#pragma interrupt INTP4 r_intc4_interrupt
#pragma interrupt INTP5 r_intc5_interrupt
#pragma interrupt INTP6 r_intc6_interrupt
//#pragma interrupt INTP7 r_intc7_interrupt
#include "IOIntp.h"
#include "r_cg_intp.h"
#include "Struct.h"
#include "Timer.h"
#include "r_cg_sau.h"
void INTP_Init()
{
    R_INTC_Create();
    Start_Intp(ZJ_INTP);
  //Start_Intp(CARD_INTP);
    Start_Intp(IRDA_INTP);
    Stop_Intp(UMU_INTP);
	//Stop_Intp(LED_INTP);
    Start_Intp(LED_INTP);
}
void Start_Intp(_Intp_Def Intp_io)
{
  switch(Intp_io)
  {
	case ZJ_INTP://ZJ INTP5
		Flag.ZJFall = 0;
		R_INTC5_Start();
		break;
	case CARD_INTP:
		//Flag.CARDFall = 0;
		//R_INTC0_Start();
		break;
         case UMU_INTP://UMU INTP6
              Flag.UMUFall = 0;
              R_INTC6_Start();
                break;
  case IRDA_INTP://IRDA INTP4
		Flag.IRDAFall = 0;
              R_INTC4_Start();
    break;
  case LED_INTP://LEDIN INTP3
		R_INTC3_Start();
    break;
  default:
    break;
  }
}
void Stop_Intp(_Intp_Def Intp_io)
{
  switch(Intp_io)
  {
	case ZJ_INTP:
		Flag.ZJFall = 1;
		R_INTC5_Stop();
		break;
	case CARD_INTP:
		//Flag.CARDFall = 1;
		//R_INTC0_Stop();
		break;
  case UMU_INTP:
    Flag.UMUFall = 1;
    R_INTC6_Stop();
    break;
  case IRDA_INTP:
		Flag.IRDAFall = 1;
    R_INTC4_Stop();
    break;
  case LED_INTP:
		R_INTC3_Stop();
    break;
  default:
    break;
  }
}
__interrupt static void r_intc3_interrupt(void)
{
	Stop_Intp(LED_INTP);
	Flag.LedFlag = 1;
	NOP();
	NOP();
	Start_Intp(LED_INTP);
}
__interrupt static void r_intc0_interrupt(void)
{
  //Flag.LedFlag = 1;
}
__interrupt static void r_intc6_interrupt(void)
{
  Stop_Intp(UMU_INTP);
  UMU_EN(1);
  Start_Timer(UMU_TIMER);
  Csb_Timer.UMUTimer=0;
	TempUserData.UMUBuffLen=0;
}
__interrupt static void r_intc5_interrupt(void)
{
  Stop_Intp(ZJ_INTP);
	Flag.ZJFall = 1;
  ZJ_EN(1);
  Start_Timer(ZJ_TIMER);
  Csb_Timer.ZJTimer=0;
	TempUserData.ZJBuffLen=0;
}
__interrupt static void r_intc4_interrupt(void)
{
  Stop_Intp(IRDA_INTP);
  IRDA_EN(1);
  Start_Timer(IRDA_TIMER);
  Csb_Timer.IRDATimer=0;
	TempUserData.IRDABuffLen=0;
}
