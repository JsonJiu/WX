#include "Irda.h"
#include "Struct.h"
#include "IOIntp.h"
#include "Uart.h"
#include "User.h"
#include "r_cg_rtc.h"
#include "string.h"
unsigned char Irda_Send_Buff[800];
unsigned char Boot_Wait_Flag;
void Check_IRDA_Data()
{
	if(Flag.IRDAData)//ru guo hongwai shou dao shuju 
	{
		unsigned char Uart2_Irda_Check_i;
		unsigned char Uart2_Irda_Check_cs = 0; 
		Flag.IRDAData = 0;
		//Uart2_SendBytes(TempUserData.IRDABuff,TempUserData.IRDABuffLen);
		if(Flag.IRDAOKData==0)//fangzhichuxianzhuangche
		{
		if(TempUserData.IRDABuffLen >= 18 + TempUserData.IRDABuff[15] + TempUserData.IRDABuff[16])//IR shu ju chang du shi 2 wei de 
		{	
			if((TempUserData.IRDABuff[5]==0xAA&&TempUserData.IRDABuff[6]==0xAA)||(TempUserData.IRDABuff[5]==0x31&&TempUserData.IRDABuff[6]==0x32))
			{
				TempUserData.IRDABuffLen = 19 + TempUserData.IRDABuff[15] + TempUserData.IRDABuff[16];
				TempUserData.IRDABuff_REC_len = TempUserData.IRDABuffLen;
				for(Uart2_Irda_Check_i = 0;Uart2_Irda_Check_i < (TempUserData.IRDABuffLen-2);Uart2_Irda_Check_i++)
                            	{
                                  Uart2_Irda_Check_cs += TempUserData.IRDABuff[Uart2_Irda_Check_i];//CS yanzhen 
                            	}
				if( Uart2_Irda_Check_cs == TempUserData.IRDABuff[TempUserData.IRDABuffLen-2])//ru guo jiao yan tong guo
                          	{
				//Irda_Data_Handling(TempUserData.IRDABuff);
				Flag.IRDAOKData = 1;//guandian wei zhi 1--biaoshi guandian OK
				
                          	}
			}
		}
		}
		else if(Flag.IRDAOKData==2)
		{
		if(TempUserData.IRDABuffLen >= 71)
		{
			//c525
			if(TempUserData.IRDABuff[18]==0x25&&TempUserData.IRDABuff[19]==0xC5)
			{
				if(TempUserData.IRDABuff[71]>=0x01&&TempUserData.IRDABuff[71]<=32)
				{
					memcpy(TempUserData.IMEI_IMSI_ICCID,TempUserData.IRDABuff+21,50);
					if(TempUserData.IRDABuffLen >= 129)
					{
						memcpy(TempUserData.versiontime,TempUserData.IRDABuff+117,6);
						memcpy(TempUserData.platepressure,TempUserData.IRDABuff+123,5);
					}
					else if (TempUserData.IRDABuffLen >= 124 && TempUserData.IRDABuffLen < 129)
					{
						memcpy(TempUserData.versiontime,TempUserData.IRDABuff+117,6);
					}
					else 
					{
						memset(TempUserData.versiontime,0x86,6);//yu fu zhi
						memset(TempUserData.platepressure,0x86,5);//yu fu zhi
					}
					TempUserData.RtcTimer = 9;
				}
				else
				{
				
				}
			}
			else if(TempUserData.IRDABuff[11]==0x25&&TempUserData.IRDABuff[12]==0xC5)//ru guo tou du cuo le
			{
				if(TempUserData.IRDABuff[64]>=0x01&&TempUserData.IRDABuff[64]<=32)
				{
					memcpy(TempUserData.IMEI_IMSI_ICCID,TempUserData.IRDABuff+14,50);
					if(TempUserData.IRDABuffLen >= 122)
					{
						memcpy(TempUserData.versiontime,TempUserData.IRDABuff+110,6);
						memcpy(TempUserData.platepressure,TempUserData.IRDABuff+116,5);
					}
					else if (TempUserData.IRDABuffLen >= 117 && TempUserData.IRDABuffLen <122)
					{
						memcpy(TempUserData.versiontime,TempUserData.IRDABuff+110,6);
					}
					else
					{
						
					}
					TempUserData.RtcTimer = 9;
				}
				else
				{
				
				}
			}
			else
			{
				memset(TempUserData.IMEI_IMSI_ICCID,0x37,50);//yu fu zhi
				memset(TempUserData.versiontime,0x86,6);//yu fu zhi
				memset(TempUserData.platepressure,0x87,5);//yu fu zhi
			}
			
			
		}
		}
		else if(Flag.IRDAOKData==3)
		{
			if(TempUserData.IRDABuffLen >= 71)
			{
				unsigned long len=65;
				//c518
				if(TempUserData.IRDABuff[18]==0x18&&TempUserData.IRDABuff[19]==0xC5)
				{
					len = len+TempUserData.IRDABuff[len];
					len = ++len+TempUserData.IRDABuff[len];
					memcpy(TempUserData.versionid,TempUserData.IRDABuff+len+29,2);
					TempUserData.RtcTimer = 12;
				}
				else if(TempUserData.IRDABuff[11]==0x18&&TempUserData.IRDABuff[12]==0xC5)//ru guo tou du cuo le
				{
					len = len-7;
					len = len+TempUserData.IRDABuff[len];
					len = ++len+TempUserData.IRDABuff[len];
					memcpy(TempUserData.versionid,TempUserData.IRDABuff+len+29,2);
					TempUserData.RtcTimer = 12;
				}
				else	
				{
					memset(TempUserData.versionid,0x78,2);
				}
			}
		}
		else if(Flag.IRDAOKData==4)
		{
			if(TempUserData.IRDABuffLen >= 71)
			{
				//c519
				if(TempUserData.IRDABuff[18]==0x19&&TempUserData.IRDABuff[19]==0xC5)
				{
					memcpy(TempUserData.esamid,TempUserData.IRDABuff+159,8);
					TempUserData.RtcTimer = 15;
				}
				else if(TempUserData.IRDABuff[11]==0x19&&TempUserData.IRDABuff[12]==0xC5)//ru guo tou du cuo le
				{	
					memcpy(TempUserData.esamid,TempUserData.IRDABuff+152,8);
					TempUserData.RtcTimer = 15;
				}
				else
				{
					memset(TempUserData.esamid,0x34,8);
				}
			}
		}
	}
}
unsigned char Irda_Data_Handling(unsigned char *Irda_Buff)
{
	/*if((TempUserData.IRDABuff[17]==)&&(TempUserData.IRDABuff[18]==))
		{
			Flag.MeterStatus = TempUserData.IRDABuff[22];
		}*/
}