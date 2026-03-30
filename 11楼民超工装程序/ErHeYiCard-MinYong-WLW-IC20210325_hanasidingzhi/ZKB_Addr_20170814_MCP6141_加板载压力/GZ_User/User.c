#include "User.h"
#include "Struct.h"
#include "IOIntp.h"
#include "Uart.h"
#include "Irda.h"
#include "r_cg_sau.h"
#include "r_cg_rtc.h"
#include "OneShot_AD.h"
#include "Timer.h"
#include "ZDINA219.h"
#include "BDINA219.h"
#include "MCP4561.h"
#include "string.h"

unsigned char UMUSimulationData[18]={0x02,0x00,0xFF,0x98,0,0,0,0,0,0,0,0,0,0,0,0,0x03,0x66};
unsigned char UMUSimulationData1[14]={0x02,0x01,0xe2,0x3a,0,0,0,0,0,0,0,0,0x03,0xd8};
unsigned char UMUSimulationData2[16]={0x02,0x01,0xe2,0x3a,0,0,0,0,0,0,0,0,0,0,0x03,0xd8};
unsigned char IrdaSimulationData[27]={0xfe,0xfe,0xfe,0xfe,0xfe,
0x68,0xAB,0x57,0x58,0xAA,0xAA,0xAA,0xAA,0xAA,0xF2,0x77,0x88,0x99,0x55
,0x03,0x00,0x03,0xE2,0x0A,0x01,0xE6,0x16
};

unsigned char IrdaSimulationData3[28]={0xfe,0xfe,0xfe,0xfe,0xfe,
0x68,0x00,0x00,0x00,0x01,0x00,0x00,0x68,0x05,0x0A,0x00,0x18,0x01,0x25
,0x08,0x50,0x00,0x16,0x52,0xA6,0x00,0x84,0x16
};

unsigned char IrdaSimulationData1[27]={0xfe,0xfe,0xfe,0xfe,0xfe,
0x68,0xAB,0x57,0x58,0xAA,0xAA,0xAA,0xAA,0xAA,0xF2,0x77,0x88,0x99,0x55
,0x03,0x00,0x03,0xE5,0x05,0x01,0xE4,0x16
};
//C525??
unsigned char IrdaSimulationData_C525[30] = {0xfe,0xfe,0xfe,0xfe,0xfe,
0x68,0xaa,0xaa,0xaa,0xaa,0xaa,0xaa,0x68,0x01,0x0b,0x00,0x21,0x10,0x11,0x09
,0x14,0x13,0x16,0x25,0xc5,0x00,0xda,0x24,0x16,0xfe
};

//C525_2
unsigned char IrdaSimulationData_C525_2[30] = {0xfe,0xfe,0xfe,0xfe,0xfe,
0x68,0x00,0x00,0x00,0x01,0x00,0x00,0x68,0x01,0x0b,0x00,0x21,0x11,0x03,0x16
,0x20,0x14,0x16,0x25,0xc5,0x00,0xda,0x36,0x16,0xfe
};

//C518
unsigned char IrdaSimulationData_C518[28] = {0xfe,0xfe,0xfe,0xfe,0xfe,
0x68,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x68,0x01,0x0A,0x00,0x24,0x09,0x02,
0x16,0x08,0x25,0x02,0x18,0xC5,0x00,0x26,0x16
};

//C519
unsigned char IrdaSimulationData_C519[28] = {0xfe,0xfe,0xfe,0xfe,0xfe,
0x68,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x68,0x01,0x0A,0x00,0x24,0x09,0x02,
0x16,0x12,0x43,0x02,0x19,0xC5,0x00,0x4F,0x16
};

//test
unsigned char debug_test[10]={0x68,0xAB,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,};

//0xE505
unsigned char ZJSendDataBuff[200];
unsigned char ZJSendDataBuffLen;
unsigned char SystemStart = 0;
unsigned short Check_ZJCMD_i;
unsigned char cs=0;
unsigned char StopFlag =0 ;
unsigned int chongfuceshi = 0;
void IOInit()
{
	unsigned char IOInit_i;
	UMUSimulationData[17] = 0;
	UMUSimulationData1[13] = 0;
	UMUSimulationData2[15] = 0;
	for(IOInit_i=0;IOInit_i<17;IOInit_i++)
	{
		UMUSimulationData[17] ^=UMUSimulationData[IOInit_i];
	}
	for(IOInit_i=0;IOInit_i<13;IOInit_i++)
	{
		UMUSimulationData1[13] ^=UMUSimulationData1[IOInit_i];
	}
	for(IOInit_i=0;IOInit_i<15;IOInit_i++)
	{
		UMUSimulationData2[15] ^=UMUSimulationData2[IOInit_i];
	}
	SuperCAPLink_EN(0);//p1.7=0
	SuperCAPFD_EN(0);//p5.0=0
	ZDBackUpDYCurrent_EN(1); //P1.1  = 1///bei dian ceshi dianliu shineng yizhi dakai baozheng zhukongbanyoudian
	ZDNoCurrent_EN(1);//p5.1 = 1/////zhudian dianyuan qiehuan zhi zhudian 
	ZDOKCurrent_EN(0);//P5.5 = 0/////zhudian ceshi dianliu shineng guanbi
	ZDBackUpDY36_EN(1);//P0.0 = 1/////yizhi zhigao 
	CheckCard(0);//CARD_OPEN=0
	NOP();
}
void CheckZDCurrent()
{
	unsigned char CheckZDCurrent_i;
	unsigned short minZDCurrent;
	unsigned short tmpZDCurrent[5];
	MCP4561_Write_Data(0,0);///big;
	Delay_ms(100);
	//ZDOKCurrent_EN(1); //P5.5 = 1主电流经过Q6,U2
	Delay_ms(100);
	//ZDNoCurrent_EN(0);  //p5.1 = 1主电流不经过Q4
	//Delay_ms(1000);
	TempUserData.ZDVolt = CheckPower(1);
	Delay_ms(200);
	for(CheckZDCurrent_i=0;CheckZDCurrent_i<5;CheckZDCurrent_i++)
	{
		Delay_ms(100);
		tmpZDCurrent[CheckZDCurrent_i]=ReadZD_Current();
	}
	minZDCurrent = tmpZDCurrent[0];
	for(CheckZDCurrent_i=1;CheckZDCurrent_i<5;CheckZDCurrent_i++)
	{
		if(minZDCurrent>tmpZDCurrent[CheckZDCurrent_i])
		{
			minZDCurrent = tmpZDCurrent[CheckZDCurrent_i];
		}
	}
	TempUserData.ZDCurrent = minZDCurrent;
	//ZDNoCurrent_EN(1);  //p5.1 = 1
	//ZDOKCurrent_EN(0); //P5.5 = 1
	NOP();
}
void CheckBDCurrent()
{
	unsigned char CheckBDCurrent_i;
	unsigned short minBDCurrent;
	unsigned short tmpBDCurrent[3];
	ShfitON_EN(0);
	Delay_ms(200);
	TempUserData.BDVolt = CheckPower(0);//ANI0---LVCAP--beidian dianya
	for(CheckBDCurrent_i=0;CheckBDCurrent_i<3;CheckBDCurrent_i++)
	{
		Delay_ms(100);
		tmpBDCurrent[CheckBDCurrent_i]=ReadBD_Current();;
	}
	minBDCurrent = tmpBDCurrent[0];
	for(CheckBDCurrent_i=1;CheckBDCurrent_i<3;CheckBDCurrent_i++)
	{
		if(minBDCurrent > tmpBDCurrent[CheckBDCurrent_i])
		{
			minBDCurrent = tmpBDCurrent[CheckBDCurrent_i];
		}
	}
	TempUserData.BDCurrent = minBDCurrent;
	NOP();
	ShfitON_EN(1);
}
void CheckCard(unsigned char CARDEN)
{
	if(!CARDEN)
	{
		CardOpen_EN(1);
	}
	else
	{
		CardOpen_EN(0);
	}
}
void Check_ZJCMD()
{
	//debug_test[3]=TempUserData.ZJAddr;
	//Uart2_SendBytes(debug_test,10);
	//Delay_ms(2000);
	if(Flag.ZJData)//ru guo zhuji shu ju yi jin dao lai
	{
		Flag.ZJData = 0;
		cs = 0;
		if(TempUserData.ZJBuffLen >= 6)
		{
			for(Check_ZJCMD_i=0;Check_ZJCMD_i<6;Check_ZJCMD_i++)
			{
				cs +=TempUserData.ZJBuff[Check_ZJCMD_i];//cs jiaoyan
			}
			if(cs ==TempUserData.ZJBuff[6])//ru guo jiaoyan tong guo 
			{
				if((TempUserData.ZJBuff[0]==0x68)&&(TempUserData.ZJBuff[3]==0x01)&&(TempUserData.ZJBuff[4]==0x0A)
			&&(TempUserData.ZJBuff[5]==TempUserData.ZJAddr)&&(TempUserData.ZJBuff[7]==0x16))//ruguo shou dao de shi qi dong ce shi zhi ling 
				{
					SystemStart = 1;//qidong ce shi biaozhi zhi 1
					TempUserData.IRDABuffLen = 0;
					Flag.IRDAOKData = 0;//qingchu IRDAOKData biao zhi wei
					TempUserData.RtcTimer = 0;
					Flag.UMUData = 0;
					StopFlag = 0;
					Start_Intp(UMU_INTP);// qidong waibu zhongduan --  UMU _INTP
					Uart2_SendByte(0x99);
				}
				else if((TempUserData.ZJBuff[0]==0x68)&&(TempUserData.ZJBuff[3]==0x02)&&(TempUserData.ZJBuff[4]==0x0B)&&(TempUserData.ZJBuff[5]==TempUserData.ZJAddr)&&
				(TempUserData.ZJBuff[7]==0x16))//ruguo shoudao duqu gongwei zhuangtai zhiling
				{
					if(StopFlag)//ru guo ce shi wan cheng fa song shu ju 
					{
						//StopFlag = 0;
						ZJSendDataBuffLen=0;
						ZJSendDataBuff[ZJSendDataBuffLen++]=0x68;
						ZJSendDataBuff[ZJSendDataBuffLen++]=0xDB;
						ZJSendDataBuff[ZJSendDataBuffLen++]=0x68;
						ZJSendDataBuff[ZJSendDataBuffLen++]=0x55;
						ZJSendDataBuff[ZJSendDataBuffLen++]=TempUserData.ZJAddr;//gong wei hao
						
						ZJSendDataBuff[ZJSendDataBuffLen++]=3600>>8;
						ZJSendDataBuff[ZJSendDataBuffLen++]=3600;
						
						TempUserData.ZDCurrent=TempUserData.ZDCurrent*10;
						if(TempUserData.ZDCurrent>720)
						{TempUserData.ZDCurrent-=370;}
						else if(TempUserData.ZDCurrent<=720 && TempUserData.ZDCurrent>420)
						{TempUserData.ZDCurrent-=370;}
						ZJSendDataBuff[ZJSendDataBuffLen++]=TempUserData.ZDCurrent>>8;
						ZJSendDataBuff[ZJSendDataBuffLen++]=TempUserData.ZDCurrent;
						
						ZJSendDataBuff[ZJSendDataBuffLen++]=TempUserData.BDVolt>>8;
						ZJSendDataBuff[ZJSendDataBuffLen++]=TempUserData.BDVolt;
						
						ZJSendDataBuff[ZJSendDataBuffLen++]=0x00;//bu ce bei dian le
						ZJSendDataBuff[ZJSendDataBuffLen++]=0x14;
						
						//ZJSendDataBuff[ZJSendDataBuffLen++]=TempUserData.BDCurrent>>8;//bu ce bei dian le
						//ZJSendDataBuff[ZJSendDataBuffLen++]=TempUserData.BDCurrent;
						//Flag.UMUData=0;
						ZJSendDataBuff[ZJSendDataBuffLen++]=Flag.UMUData;//UMU  de TX
						
						Flag.CAPStatus =1;
						
						ZJSendDataBuff[ZJSendDataBuffLen++]=Flag.LedFlag;;//shi ji shi LED Flag
						//Flag.JudgeVavle=1;
						ZJSendDataBuff[ZJSendDataBuffLen++]=Flag.JudgeVavle;//shi ji shi Valve Flag
						
						Flag.IRDAOKData=1;
						ZJSendDataBuff[ZJSendDataBuffLen++]=Flag.IRDAOKData;//shi ji shi IR Flag
						
						ZJSendDataBuff[ZJSendDataBuffLen++]=Flag.CAPStatus;//tong xin ban zhuang tai
						
						//ZJSendDataBuff[ZJSendDataBuffLen++]=Flag.MeterStatus;
						if(TempUserData.IRDABuff_REC_len==0)
						{
							TempUserData.IRDABuff_REC_len = 45;
						}
						
						for(Check_ZJCMD_i = 0;Check_ZJCMD_i<TempUserData.IRDABuff_REC_len;Check_ZJCMD_i++)
						{
							ZJSendDataBuff[ZJSendDataBuffLen++]=TempUserData.IRDABuff[Check_ZJCMD_i];//ba hong wai kou shoudao de shu ju zhuanfa
						}
						
						for(Check_ZJCMD_i =0;Check_ZJCMD_i<50;Check_ZJCMD_i++)
						{
							ZJSendDataBuff[ZJSendDataBuffLen++]=TempUserData.IMEI_IMSI_ICCID[Check_ZJCMD_i];//IMEI,IMSI,ICCID
						}
						
						for(Check_ZJCMD_i =0;Check_ZJCMD_i<2;Check_ZJCMD_i++)
						{
							ZJSendDataBuff[ZJSendDataBuffLen++]=TempUserData.versionid[Check_ZJCMD_i];//ban ben hao
						}
						//
						
						for(Check_ZJCMD_i =0;Check_ZJCMD_i<6;Check_ZJCMD_i++)
						{
							ZJSendDataBuff[ZJSendDataBuffLen++]=TempUserData.versiontime[Check_ZJCMD_i];//bian yi shi jian
						}
						
						for(Check_ZJCMD_i =0;Check_ZJCMD_i<5;Check_ZJCMD_i++)
						{
							ZJSendDataBuff[ZJSendDataBuffLen++]=TempUserData.platepressure[Check_ZJCMD_i];//ban zai ya li zhuang tai he zhi
						}
						
						for(Check_ZJCMD_i =0;Check_ZJCMD_i<8;Check_ZJCMD_i++)
						{
							ZJSendDataBuff[ZJSendDataBuffLen++]=TempUserData.esamid[Check_ZJCMD_i];//ESAMID
						}
						
						ZJSendDataBuff[ZJSendDataBuffLen++]= TempUserData.BDflag; //BD er ji guan
						
						//
						ZJSendDataBuff[ZJSendDataBuffLen] = 0;
						
						for(Check_ZJCMD_i = 0;Check_ZJCMD_i<ZJSendDataBuffLen;Check_ZJCMD_i++)
						{
							ZJSendDataBuff[ZJSendDataBuffLen] +=ZJSendDataBuff[Check_ZJCMD_i];//qiu lei jia he
						}
						ZJSendDataBuffLen++;
						ZJSendDataBuff[ZJSendDataBuffLen++]=0x16;//zhen wei 
						Uart2_SendBytes(ZJSendDataBuff,ZJSendDataBuffLen);
						TempUserData.IRDABuffLen = 0;
						Delay_ms(5);
					}
				}
			}
		}
		TempUserData.ZJBuffLen = 0;
		Start_Intp(ZJ_INTP);
	}
}
void ScanRtc()
{
	if(Flag.Rtc_Come)//rtc zhongduan biao zhi---0.5Szhongduan yici
	{
		Flag.Rtc_Come = 0;
		if(Flag.Rtc_time >= 2)
		{
			Uart1_SendBytes(UMUSimulationData,18);
			Flag.Rtc_time = 0;
		}
		if(SystemStart)//ru guo qidong ceshi 
		{
			TempUserData.RtcTimer++;//RtcTimer jishu jia 1
			switch(TempUserData.RtcTimer)
			{
				case 1://
				//Delay_ms(10000);//deng dai 10s
				//CheckBDCurrent();
				chongfuceshi = 0;
				break;
				case 3:
				CheckCard(1);
				CheckCard(1);
				CheckCard(1);
				Delay_ms(3000);
				CheckCard(0);//card_open==1
				CheckCard(0);
				CheckCard(0);
				break;
				case 2:
				Delay_ms(2000);
				CheckZDCurrent();
				if(TempUserData.ZDCurrent > 80 && chongfuceshi < 6)
				{
					//CheckZDCurrent();
					chongfuceshi++;
					TempUserData.RtcTimer = 1;
				}
				else
				{
					chongfuceshi = 0;
				}
				break;
				case 4:
				Delay_ms(300);
				Uart1_SendBytes(&UMUSimulationData[2],1);//gei UMU fa song shu ju 
				Uart3_SendByte(0xAF);
				for(Check_ZJCMD_i=0;Check_ZJCMD_i<550;Check_ZJCMD_i++)
				{
					Uart3_SendByte(0x55);
				}
				Uart3_SendBytes(IrdaSimulationData3,28);
				Uart3_SendBytes(IrdaSimulationData1,27);//qi dong GPRS zhi ling 
				Delay_ms(2000);
				chongfuceshi++;
				if(chongfuceshi>=2)
				{
				}
				else
				{
					TempUserData.RtcTimer = 4;
				}
				break;
				case 5:
				//Delay_ms(10000);
				break;
				case 6:
				//Delay_ms(10000);
				break;
				case 7:
				//Delay_ms(10000);
				chongfuceshi = 0;
				break;
				case 8:
				memset(TempUserData.IMEI_IMSI_ICCID,0x38,50);//yu fu zhi
				memset(TempUserData.versiontime,0x87,6);//yu fu zhi
				memset(TempUserData.platepressure,0x88,5);//yu fu zhi
				memset(TempUserData.IRDABuff,0x00,200);      // qing kong hong wai huan chong qu
				Delay_ms(300);
				TempUserData.IRDABuffLen = 0;
				Uart1_SendBytes(UMUSimulationData,1);//gei UMU fa song shu ju 
				for(Check_ZJCMD_i=0;Check_ZJCMD_i<550;Check_ZJCMD_i++)
				{
					Uart3_SendByte(0x55);
				}
				Uart3_SendBytes(IrdaSimulationData_C525,30);//IR tong lu ce shi
				Uart3_SendBytes(IrdaSimulationData_C525_2,30);//IR tong lu ce shi
				Start_Intp(IRDA_INTP);
				Flag.IRDAOKData = 2;//xie yi suo
				//TempUserData.IRDABuffLen = 0;          //hong wai jie shou shu liang biao shi
				Delay_ms(1500);
				chongfuceshi++;
				if(chongfuceshi>=10)
				{
				}
				else
				{
					TempUserData.RtcTimer = 7;
				}
				break;
				case 9:
				if(chongfuceshi<=6)
				{
				Delay_ms(300);
				chongfuceshi++;
				TempUserData.RtcTimer = 8;
				}
				break;
				case 10:
				//Delay_ms(500);                       //bi xu zai zhe li jie shou wan shu ju
				chongfuceshi = 0;
				TempUserData.RtcTimer = 10;
				break;
				case 11:
				TempUserData.BDflag = 0;//BD er ji guan yu fu zhi
				memset(TempUserData.versionid,0x77,2);//yu fu zhi
						//memset(TempUserData.versiontime,0x11,6);
				memset(TempUserData.IRDABuff,0x00,400);      // qing kong hong wai huan chong qu
				Delay_ms(300);
				TempUserData.IRDABuffLen = 0;
				Uart1_SendBytes(UMUSimulationData,1);//gei UMU fa song shu ju 
				for(Check_ZJCMD_i=0;Check_ZJCMD_i<550;Check_ZJCMD_i++)
				{
					Uart3_SendByte(0x55);
				}
				Uart3_SendBytes(IrdaSimulationData_C518,28);//IR tong lu ce shi
				Start_Intp(IRDA_INTP);
				Flag.IRDAOKData = 3;//xie yi suo
				Delay_ms(3500);
				chongfuceshi++;
				if(chongfuceshi>=3)
				{
				}
				else
				{
					TempUserData.RtcTimer = 10;
				}				
				break;
				case 12:
				if(chongfuceshi<=2)
				{
				//Delay_ms(500);
				chongfuceshi++;
				TempUserData.RtcTimer = 11;
				}
				break;
				case 13:
				//Delay_ms(500);                       //bi xu zai zhe li jie shou wan shu ju
				chongfuceshi = 0;
				TempUserData.RtcTimer = 13;
				break;
				case 14:
				memset(TempUserData.esamid,0x33,8);//yu fu zhi
				memset(TempUserData.IRDABuff,0x00,200);      // qing kong hong wai huan chong qu
				Delay_ms(300);
				TempUserData.IRDABuffLen = 0;
				Uart1_SendBytes(UMUSimulationData,1);//gei UMU fa song shu ju 
				for(Check_ZJCMD_i=0;Check_ZJCMD_i<550;Check_ZJCMD_i++)
				{
					Uart3_SendByte(0x55);
				}
				Uart3_SendBytes(IrdaSimulationData_C519,28);//IR tong lu ce shi
				Start_Intp(IRDA_INTP);
				Flag.IRDAOKData = 4;//xie yi suo
				break;
				case 15:
				if(chongfuceshi<=2)
				{
				//Delay_ms(500);														
				chongfuceshi++;
				TempUserData.RtcTimer = 14;
				}
				break;
				case 16:
				Delay_ms(500);                       //bi xu zai zhe li jie shou wan shu ju
				chongfuceshi = 0;
				
				TempUserData.RtcTimer = 16;
				break;
				case 17:
				memset(TempUserData.IRDABuff,0x00,200);      // qing kong hong wai huan chong qu
				TempUserData.IRDABuffLen = 0;
				//TempUserData.IRDABuff[0] = TempUserData.IRDABuffLen; //tiao shi yong de
				//Uart2_SendBytes(TempUserData.IRDABuff,2);
				Delay_ms(300);
				Uart1_SendBytes(UMUSimulationData,1);//gei UMU fa song shu ju 
				for(Check_ZJCMD_i=0;Check_ZJCMD_i<550;Check_ZJCMD_i++)
				{
					Uart3_SendByte(0x55);
				}
				Uart3_SendBytes(IrdaSimulationData,27);//IR tong lu ce shi
				Uart3_SendBytes(IrdaSimulationData,27);//IR tong lu ce shi
				Flag.IRDAOKData = 0;//xie yi suo
				Start_Intp(IRDA_INTP);
				//Flag.LedFlag = 0;//LED No jian ce
				Flag.LedFlag = 1;
				TempUserData.VavleRTimer = 0;
				TempUserData.VavleBTimer = 0;
				
				TempUserData.ScanVavleTimer=3000;
				Flag.JudgeVavle = 0;
				//TempUserData.ZDVolt = CheckPower();
//				while(TempUserData.ScanVavleTimer--)//panduan fa men
//				{
//					Delay_ms(1);
//					//if((!VavleR_InPut)&&(VavleB_InPut))
//					if(VavleR_InPut)
//					{
//						TempUserData.VavleRTimer++;
//						if(TempUserData.VavleRTimer>5)
//						{
//							Flag.JudgeVavle = 1;
//							break;
//						}
//					}
//				}
//				TempUserData.ScanVavleTimer=1000;
//				if(Flag.JudgeVavle)
//				{
//					Flag.JudgeVavle = 0;
//					while(TempUserData.ScanVavleTimer--)
//					{
//						Delay_ms(1);
//						//if((VavleB_InPut)&&(!VavleR_InPut))
//						if(!VavleR_InPut)
//						{
//							TempUserData.VavleBTimer++;
//							if(TempUserData.VavleBTimer>5)
//							{
//								Flag.JudgeVavle = 1;
//								break;
//							}
//						}
//					}
//				}
	                      while(TempUserData.ScanVavleTimer--)
				{
					Delay_ms(1);
					if(((!VavleR_InPut)&&(VavleB_InPut))||((!VavleB_InPut)&&(VavleR_InPut)))
					{
						TempUserData.VavleRTimer++;
						if(TempUserData.VavleRTimer>3)
						{
							Flag.JudgeVavle = 1;
							break;
						}
					}
				}
				//TempUserData.ScanVavleTimer=1000;
				TempUserData.ScanVavleTimer=2000;
				if(Flag.JudgeVavle)
				{
					Flag.JudgeVavle = 0;
					while(TempUserData.ScanVavleTimer--)
					{
						Delay_ms(1);
						if(((!VavleR_InPut)&&(VavleB_InPut))||((!VavleB_InPut)&&(VavleR_InPut)))
						{
							TempUserData.VavleBTimer++;
							if(TempUserData.VavleBTimer>3)
							{
								Flag.JudgeVavle = 1;
								break;
							}
						}
					}
				}
				
				Delay_ms(1500);
				Delay_ms(50);
				break;
				case 18:
				if(Flag.JudgeVavle==0 || Flag.IRDAOKData != 1)
				{
					chongfuceshi++;
					if(chongfuceshi<5)
					{
						TempUserData.RtcTimer=16;
					}
				}
				Delay_ms(2000);
				break;
				/*
				case 19:
				break;
				case 20:
				break;
				case 21:
				break;
				case 22:
				break;
				case 23:
				break;
				case 33:
				break;
				case 34:
				break;
				case 35:
				break;
				case 36:
				break;
				case 37:
				break;
				case 38:
				break;
				case 39:
				break;
				case 40:
				break;
				case 41:
				break;
				case 42:
				break;
				case 43:
				break;
				case 44:
				break;
				case 45:
				break;
				case 46:
				break;
				case 47:
				break;
				case 48:
				break;
				case 49:
				break;
				case 50:
				break;
				case 51:
				break;
				case 52:
				break;*/
				default:
				TempUserData.RtcTimer =0;
				SystemStart = 0;
				StopFlag = 1;//ceshi jieshu biao zhi wei zhi 1
				//SystemStart = 1;
				break;
			}
		}
	}
}