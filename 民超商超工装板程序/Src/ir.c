#include "ir.h"
#include "jiaoyan.h"
#include "test.h"
#include "uart1.h"

extern struct Test Test_result;
extern enum Test2 Test_agreement_receive;

/*————————E20A协议 数据标识E20A————————*/
/*
先是MOTOR_R   4.7V   2s
然后停止 500ms
然后MOTOR_B   4,7V	 2s
后面是读取内容：
流量	计量电压   通讯电压   Flash  UMU(默认，实际用串口测)   刷卡(FF或5A都判断正常)
*/
uint8_t Motor_Agreement[23] = {0x68,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x68,0x01,0x0A,0x00,0x25,0x06,0x18,0x14,0x31,0x52,0x02,0x0A,0xE2,0x00,0x9D,0x16};

/*————————E20B协议 数据标识E20B————————*/
/*
因为主控板放上工装后
需要先进行人工目检	
人工目检项目有：按键  液晶
按键会把液晶背光灯唤醒影响功耗
此协议的作用是关闭液晶的背光灯
*/
uint8_t Close_Screen_LED_Agreement[23] = {0x68,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x68,0x01,0x0A,0x00,0x22,0x01,0x05,0x18,0x31,0x37,0x02,0x0B,0xE2,0x00,0x6C,0x16};

/*————————1005上告协议 数据码1005————————*/
/*
先是发送主动上告协议
接收到返回数据后从通讯串口发送模拟上告成功协议
屏幕显示PD SUCESS并从串口返回数据
*/
uint8_t Start_Shanggao_Agreement[24] = {0x68,0xFF,0xFF,0xFF,0xFF,0xFF,0xFF,0x68,0x01,0x0B,0x00,0x25,0x06,0x19,0x16,0x24,0x04,0x02,0x05,0x10,0x00,0x01,0x70,0x16};
	
/*————————前导码发送————————*/
uint8_t Part_Qiandao_55[10] = {0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55};
uint8_t Part_Qiandao_FE[5] = {0xFE,0xFE,0xFE,0xFE,0xFE};
void Send_Qiandao(void)
{
	uint8_t count = 0;
	for(count=0;count<55;count++)
	{
		Uart1_Tx_Send(Part_Qiandao_55,10);
	}
	Uart1_Tx_Send(Part_Qiandao_FE,5);
}
	
/*————————红外数据接收解析————————*/
void IR_Agreement_Analysis(uint8_t *Data, uint32_t lenth)
{
	uint8_t pHead = 0;
	uint16_t Data_lenth = 0;
	uint8_t sum_jiaoyan = 0;
	uint8_t sum_jiaoyan_jisuan = 0;
	uint8_t IR_analysis_Data[200] = {0};
	uint16_t Data_sign = 0;
	while(1)
	{
		if(pHead+23>lenth)
			break;
		if(Data[pHead]==0x68 && Data[pHead+7]==0x68)
		{
			Data_lenth = Data[pHead+10];
			Data_lenth = (Data_lenth<<8) + Data[pHead+9] + 13;  //取数据中的帧长度两字节数据，这个字节长度只包含数据内容的长度
			sum_jiaoyan = Data[pHead+Data_lenth-2];		 //取数据中的和校验一字节数据
			memcpy(IR_analysis_Data,&Data[pHead],Data_lenth);	//将数据中有用的完整的这一串数据转存
			sum_jiaoyan_jisuan = Sum_jiaoyan(IR_analysis_Data,Data_lenth); //将转存的数据进行和校验
			if(sum_jiaoyan == sum_jiaoyan_jisuan)
			{
				Data_sign = Data[pHead+19];
				Data_sign = (Data_sign<<8) + Data[pHead+18];
				switch(Data_sign)
				{
					case 0x1005:
						Test_agreement_receive = r_1005;
						break;
					case 0xE20A:
						Test_agreement_receive = r_E20A;
						Test_result.UMU_Flow_Rate = Data[pHead+26];
						Test_result.UMU_Flow_Rate = (Test_result.UMU_Flow_Rate<<8) + Data[pHead+27];
						Test_result.UMU_Flow_Rate = (Test_result.UMU_Flow_Rate<<8) + Data[pHead+28];
						Test_result.UMU_Flow_Rate = (Test_result.UMU_Flow_Rate<<8) + Data[pHead+29];
						if(Test_result.UMU_Flow_Rate>0 && Test_result.UMU_TXD == 1)
						{
							Test_result.UMU_test = 1;
						}
						Test_result.UMU_Voltage = Data[pHead+30]*100;
						Test_result.NB_Voltage = Data[pHead+31]*100;
						Test_result.flash = 1-Data[pHead+32];
						if(Data[pHead+Data_lenth-3]!=0)
						{
							Test_result.IC_test = 1;
						}
						break;
					case 0xE20B:
						Test_result.Infrared = 1;
						Test_agreement_receive = r_E20B;
						break;
					default:
						break;
				}
			}
		}
		pHead++;
	}
}




/*————————关液晶背光————————*/
void Close_Screen_LED(void)
{
	Send_Qiandao();
	Uart1_Tx_Send(Close_Screen_LED_Agreement,23);
}

/*————————开阀门并查询————————*/
void Send_E20A(void)
{
	Send_Qiandao();
	Uart1_Tx_Send(Motor_Agreement,23);
}

/*————————启动上告————————*/
void Start_Shanggao(void)
{
	Send_Qiandao();
	Uart1_Tx_Send(Start_Shanggao_Agreement,24);
}
