#include "NB.h"
#include "uart3.h"
#include "jiaoyan.h"
#include "test.h"

extern struct Test Test_result;
extern enum Test2 Test_agreement_receive;
/*————————模拟通讯协议————————*/
/*
等待启动上告后
通过通讯串口向主控板MCU发送上告完成协议
液晶屏会显示PD SUCESS
*/
uint8_t NB_Agreement[9] = {0x68,0x01,0x01,0x03,0x01,0x00,0x00,0x6E,0x16};

/*————————通讯接收数据解析————————*/
void NB_Agreement_Analysis(uint8_t *Data, uint32_t lenth)
{
	uint8_t pHead = 0;
	uint8_t Data_lenth = 0;
	uint8_t sum_jiaoyan = 0;
	uint8_t sum_jiaoyan_jisuan = 0;
	uint8_t NB_analysis_Data[200] = {0};
	while(1)
	{
		if(pHead+33>lenth)
			break;
		if(Data[pHead]==0x68)
		{
			Data_lenth = Data[pHead+3]+6;    //数据帧中的长度字节表示的只是数据内容的长度
			sum_jiaoyan = Data[pHead+Data_lenth-2];
			memcpy(NB_analysis_Data,&Data[pHead],Data_lenth);
			sum_jiaoyan_jisuan = Sum_jiaoyan(NB_analysis_Data,Data_lenth);
			if(sum_jiaoyan==sum_jiaoyan_jisuan)
			{
				Test_result.Agreement_receive = r_simulate_shanggao;
				Test_result.NB_test = 1;
			}
		}
		pHead++;
	}

}

/*————————NB模拟协议发送————————*/
void NB_Simulate(void)
{
	Uart3_Tx_Send(NB_Agreement,9);
}

