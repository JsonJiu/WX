#include "User.h"
#include "Uart.h"
#include "IOIntp.h"
#include "Struct.h"
void CheckZJData()
{
	if(Flag.ZJData)
	{
		Flag.ZJData = 0;
		Uart3_SendBytes(TempUserData.ZJBuff,TempUserData.ZJBuffLen);
		Delay_ms(5);
		TempUserData.ZJBuffLen = 0;
		Start_Intp(ZJ_INTP);
	}
}
void CheckFJData()
{
	if(Flag.FJData)
	{
		Flag.FJData = 0;
		Uart2_SendBytes(TempUserData.FJBuff,TempUserData.FJBuffLen);
		Start_Intp(FJ_INTP);
	}
}