#include "NB.h"
#include "uart3.h"
#include "jiaoyan.h"
#include "test.h"

extern struct Test Test_result;
extern enum Test2 Test_agreement_receive;
void NB_sendOK(void);
/*————————模拟通讯协议————————*/
/*
等待启动上告后
通过通讯串口向主控板MCU发送上告完成协议
液晶屏会显示PD SUCESS
*/
uint8_t NB_Agreement[9] = {0x68,0x01,0x01,0x03,0x01,0x00,0x00,0x6E,0x16};
uint8_t Get_AT[] = "AT\r";
uint8_t CFUN_0[] = "AT+CFUN=0\r";
uint8_t Return_OK[] = "OK\r";
//这里写比较字符串主要是原生比较会使\0就是结束符。
uint8_t bijiao_zifuchuan(uint8_t bijiao1[],uint8_t bijiao2[],uint16_t lenth)
{
	uint16_t bijiao_lenth = 0;
	for(bijiao_lenth = 0;bijiao_lenth < lenth;bijiao_lenth++)
	{
		if(bijiao1[bijiao_lenth]!=bijiao2[bijiao_lenth])
		{
			return 0;
		}
	}
	return 1;
}
/*————————通讯接收数据解析————————*/
void NB_Agreement_Analysis(uint8_t *Data, uint32_t lenth)
{
	uint8_t pHead = 0;
	while(1)
	{
		if(pHead>=lenth)
		{
		break;
		}
		if(pHead+sizeof(Get_AT)-1<=lenth)
		{
        if(bijiao_zifuchuan(&Data[pHead],Get_AT,sizeof(Get_AT)-1))
		{
       	NB_sendOK();
				pHead += sizeof(Get_AT)-1;
		}
	}
		if(pHead+sizeof(CFUN_0)-1<=lenth)
		{
		if(bijiao_zifuchuan(&Data[pHead],CFUN_0,sizeof(CFUN_0)-1))
		{
		Test_agreement_receive = r_simulate_shanggao;
		Test_result.NB_test = 1;
		pHead += sizeof(CFUN_0)-1;
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
/*————————NB模拟协议发送OK———————*/
void NB_sendOK(void)
{
	Uart3_Tx_Send(Return_OK,sizeof(Return_OK)-1);
}
