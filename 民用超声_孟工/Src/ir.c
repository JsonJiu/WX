#include "ir.h"
#include "jiaoyan.h"
#include "test.h"
#include "uart1.h"

extern struct Test Test_result;
extern enum Test2 Test_agreement_receive;
extern uint32_t interval_timing;

uint8_t Agreement_E20A[22] = {0x68, 0xAB, 0x57, 0x58, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xF2, 0x77, 0x88, 0x99, 0x55, 0x03, 0x00, 0x03, 0xE2, 0x0A, 0x01, 0xE6, 0x16};
uint8_t Agreement_1005[22] = {0x68, 0xAB, 0x57, 0x58, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xF2, 0x77, 0x88, 0x99, 0x55, 0x03, 0x00, 0x03, 0xE5, 0x05, 0x01, 0xE4, 0x16};
uint8_t Agreement_C525[24] = {0x68, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0x68, 0x01, 0x0B, 0x00, 0x21, 0x10, 0x11, 0x09, 0x14, 0x13, 0x16, 0x25, 0xC5, 0x00, 0xDA, 0x24, 0x16};
uint8_t Agreement_C518[23] = {0x68, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x68, 0x01, 0x0A, 0x00, 0x24, 0x09, 0x02, 0x16, 0x08, 0x25, 0x02, 0x18, 0xC5, 0x00, 0x26, 0x16};
uint8_t Agreement_C519[23] = {0x68, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0x68, 0x01, 0x0A, 0x00, 0x24, 0x09, 0x02, 0x16, 0x12, 0x43, 0x02, 0x19, 0xC5, 0x00, 0x4F, 0x16};

/*————————前导码发送————————*/
uint8_t Part_Qiandao_55[10] = {0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55, 0x55};
uint8_t Part_Qiandao_FE[10] = {0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE, 0xFE};
void Send_Qiandao(void)
{
	uint8_t count = 0;
	for (count = 0; count < 60; count++)
	{
		Uart1_Tx_Send(Part_Qiandao_55, 10);
	}
	Uart1_Tx_Send(Part_Qiandao_FE, 10);
}

/*————————红外数据接收解析————————*/
void IR_Agreement_Analysis(uint8_t *Data, uint32_t lenth)
{
	uint16_t pHead = 0;
	uint16_t Data_lenth = 0;
	uint8_t sum_jiaoyan = 0;
	uint8_t sum_jiaoyan_jisuan = 0;
	uint8_t IR_analysis_Data[500] = {0};
	uint16_t Data_sign = 0;
	uint16_t len = 65;
	while (1)
	{
		if (pHead + 43 > lenth)
			break;
		if (Data[pHead] == 0x68 && Data[pHead + 1] == 0xAB)
		{
			Data_lenth = 19 + Data[pHead + 16] + (Data[pHead + 15] << 8); // 帧长度=19+数据长度低字节+数据长度高字节
			memcpy(IR_analysis_Data, Data + pHead, Data_lenth);			  // 取出要分析的数据,从pHead开始的数据进行校验
			sum_jiaoyan = Sum_jiaoyan(IR_analysis_Data, Data_lenth);
			if (sum_jiaoyan == IR_analysis_Data[Data_lenth - 2])
			{
				Test_result.Agreement_receive = r_E20A;
				Test_result.Infrared = 1;
				memcpy(Test_result.E20A, IR_analysis_Data, Data_lenth);
				pHead += Data_lenth;
			}
		}
		if (Data[pHead] == 0x68 && Data[pHead + 7] == 0x68)
		{
			Data_lenth = Data[pHead + 10];
			Data_lenth = (Data_lenth << 8) + Data[pHead + 9] + 13;			// 取数据中的帧长度两字节数据，这个字节长度只包含数据内容的长度
			sum_jiaoyan = Data[pHead + Data_lenth - 2];						// 取数据中的和校验一字节数据
			memcpy(IR_analysis_Data, &Data[pHead], Data_lenth);				// 将数据中有用的完整的这一串数据转存
			sum_jiaoyan_jisuan = Sum_jiaoyan(IR_analysis_Data, Data_lenth); // 将转存的数据进行和校验
			if (sum_jiaoyan == sum_jiaoyan_jisuan)
			{
				Data_sign = Data[pHead + 19];
				Data_sign = (Data_sign << 8) + Data[pHead + 18];
				switch (Data_sign)
				{
				case 0x1000:
					Test_result.Agreement_receive = r_1000;
					interval_timing = 0;
					break;
				case 0xC525:
					Test_result.Agreement_receive = r_C525;
					interval_timing = 0;
					memcpy(Test_result.IMEI, &Data[pHead + 21], 15);
					memcpy(Test_result.IMSI, &Data[pHead + 36], 15);
					memcpy(Test_result.ICCID, &Data[pHead + 51], 20);
					memcpy(Test_result.Comp_Time, &Data[pHead + 117], 6);
					Test_result.Pressure = Data[pHead + 123];
					memcpy(Test_result.PlatePressure, &Data[pHead + 124], 4);
					break;
				case 0xC518:
					Test_result.Agreement_receive = r_C518;
					interval_timing = 0;
					len = len + Data[pHead + len] + 1;
					len = len + Data[pHead + len] + 1;
					memcpy(Test_result.SOFT_Version, Data + pHead + len + 28, 2);
					break;
				case 0xC519:
					Test_result.Agreement_receive = r_C519;
					interval_timing = 0;
					memcpy(Test_result.ESAMID, &Data[pHead + 159], 8);
					break;
				default:
					break;
				}
				pHead += Data_lenth;
			}
		}
		pHead++;
	}
}

/*————————E20A指令发送————————*/
void Send_E20A(void)
{
	Send_Qiandao();
	Uart1_Tx_Send(Agreement_E20A, 22);
}

/*————————启动上告————————*/
void Start_Shanggao(void)
{
	Send_Qiandao();
	Uart1_Tx_Send(Agreement_1005, 22);
}

/*————————读取IMEI,IMSI,ICCID————————*/
void Read_IMEI_IMSI_ICCID(void)
{
	Send_Qiandao();
	Uart1_Tx_Send(Agreement_C525, 24);
}

/*————————读取版本号————————*/
void Read_Version(void)
{
	Send_Qiandao();
	Uart1_Tx_Send(Agreement_C518, 23);
}

/*————————读取ESAMID————————*/
void Read_ESAMID(void)
{
	Send_Qiandao();
	Uart1_Tx_Send(Agreement_C519, 23);
}
