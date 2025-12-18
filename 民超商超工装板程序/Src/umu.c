#include "umu.h"
#include "jiaoyan.h"
#include "uart4.h"
#include "test.h"

extern struct Test Test_result;
extern enum Test2 Test_agreement_receive;

/*————————UMU协议发送计时————————*/
uint16_t UMU_time_count = 0;

/*————————计量协议————————*/
uint8_t UMU_Agreement[51] = {0xFE,0x68,0x32,0x00,0x01,0x00,0x00,0x00,0xEF,0x1F,0x3C,0x00,0x00,0x00,0xEE,0x14,0x3C,0x00,0x00,0x52,
													 0x1B,0x00,0x00,0x52,0x08,0x00,0xD2,0x00,0xD2,0xFF,0xFF,0xFF,0xFF,0x07,0xD0,0x27,0x9E,0x40,0x02,0x00,
													 0x00,0x01,0x6E,0x0D,0x5C,0xFF,0xFF,0x05,0x03,0x46,0x16};

/*————————UMU接收数据解析————————*/
/*
此数据是主控板上告时从UMU发出的数据，
所以只能检测TXD是否正常
而UMU的RXD测试是看E20A返回的数据流量是否大于0
*/
void UMU_Agreement_Analysis(uint8_t *Data, uint32_t lenth)
{
	uint8_t pHead = 0;
	uint8_t Data_lenth = 0;
	uint8_t sum_jiaoyan = 0;
	uint8_t sum_jiaoyan_jisuan = 0;
	uint8_t UMU_analysis_Data[200] = {0};
	while(1)
	{
		if(pHead+12>lenth)
			break;
		if(Data[pHead]==0x68)
		{	
			Data_lenth = Data[pHead+1];
			if(Data[pHead+Data_lenth-1]==0x16)
			{
				sum_jiaoyan = Data[pHead+Data_lenth-2];
				memcpy(UMU_analysis_Data,&Data[pHead],Data_lenth);
				sum_jiaoyan_jisuan = Sum_jiaoyan(UMU_analysis_Data,Data_lenth);
				if(sum_jiaoyan==sum_jiaoyan_jisuan)
				{
					Test_result.UMU_TXD = 1;
				}
			}
		}
		pHead++;
	}
}

/*————————UMU模拟协议发送————————*/
/*
需要再主控板上电10s和UMU建立连接
所以这边每隔两秒进行一次发送
是否建立连接可以通过E20A查询流量
流量只能代表主控板UMU的接收是否完好
而当主控板上电后，会自动发送一串指令
我们接收到这串指令说明主控板UMU的发送完好
*/
void UMU_Simulate(void)
{
	if(UMU_time_count == 0)
	{
		Uart4_Tx_Send(UMU_Agreement,51);
		UMU_time_count = 4000; //2s发一次
	}
}
