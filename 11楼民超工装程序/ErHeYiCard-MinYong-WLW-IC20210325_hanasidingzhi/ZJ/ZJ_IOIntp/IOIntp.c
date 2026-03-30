//#pragma interrupt INTP0 r_intc0_interrupt
//#pragma interrupt INTP1 r_intc1_interrupt
//#pragma interrupt INTP2 r_intc2_interrupt
//#pragma interrupt INTP3 r_intc3_interrupt
#pragma interrupt INTP4 r_intc4_interrupt
#pragma interrupt INTP5 r_intc5_interrupt
//#pragma interrupt INTP6 r_intc6_interrupt
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
  Start_Intp(FJ_INTP);
}
void Start_Intp(_Intp_Def Intp_io)
{
  switch(Intp_io)
  {
	case ZJ_INTP:
		Flag.ZJFall = 0;
		R_INTC5_Start();
		break;
	case FJ_INTP:
		Flag.FJFall = 0;
		R_INTC4_Start();
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
	case FJ_INTP:
		Flag.FJFall = 1;
		R_INTC4_Stop();
		break;
  default:
    break;
  }
}
__interrupt static void r_intc5_interrupt(void)
{
	Stop_Intp(ZJ_INTP);
	ZJ_EN(1);
	Start_Timer(ZJ_TIMER);
        Csb_Timer.ZJTimer=0;
	TempUserData.ZJBuffLen=0;
}
__interrupt static void r_intc4_interrupt(void)
{
        Stop_Intp(FJ_INTP);
	FJ_EN(1);
	Start_Timer(FJ_TIMER);
        Csb_Timer.FJTimer=0;
	TempUserData.FJBuffLen=0;
}
