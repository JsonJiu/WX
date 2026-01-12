#include "PC.h"
#include "jiaoyan.h"
#include "uart0.h"
#include "test.h"

extern struct Test Test_result;
#define send_lenth 200
uint8_t search_test_agreement_back[send_lenth];

/*————————测试数据返回————————*/
void Back_Search_Data(void)
{
	uint16_t lenth = 0;
	uint8_t count = 0;
	memset(search_test_agreement_back, 0x00, send_lenth);
	search_test_agreement_back[lenth++] = 0x68;
	search_test_agreement_back[lenth++] = 0xDB;
	search_test_agreement_back[lenth++] = 0x68;
	search_test_agreement_back[lenth++] = 0x55;
	search_test_agreement_back[lenth++] = Test_result.Workstation; // 工位

	search_test_agreement_back[lenth++] = Test_result.Voltage_zhudian >> 8; // 工装主电供电
	search_test_agreement_back[lenth++] = Test_result.Voltage_zhudian;
	
	Test_result.Current_zhudian *= 10;
	search_test_agreement_back[lenth++] = Test_result.Current_zhudian >> 8; // 主电功耗
	search_test_agreement_back[lenth++] = Test_result.Current_zhudian;

	search_test_agreement_back[lenth++] = Test_result.Voltage_beidian >> 8; // 工装备电供电
	search_test_agreement_back[lenth++] = Test_result.Voltage_beidian;
	
	Test_result.Current_beidian *= 10;
	search_test_agreement_back[lenth++] = Test_result.Current_beidian >> 8; // 备电功耗
	search_test_agreement_back[lenth++] = Test_result.Current_beidian;

	search_test_agreement_back[lenth++] = Test_result.UMU; // 此处代表UMU的TX，1为正常

	Test_result.LED = 1;										  // 默认通过
	search_test_agreement_back[lenth++] = Test_result.LED;		  // LED
	search_test_agreement_back[lenth++] = Test_result.Motor_test; // 阀门测试
	search_test_agreement_back[lenth++] = Test_result.Infrared;	  // 红外

	Test_result.CAPStatus = 1;									 // 默认通过
	search_test_agreement_back[lenth++] = Test_result.CAPStatus; // 通信板状态

	memcpy(&search_test_agreement_back[lenth], Test_result.E20A, 45); // E20A转运
	lenth += 45;
	memcpy(&search_test_agreement_back[lenth], Test_result.IMEI, 15); // IMEI号
	lenth += 15;
	memcpy(&search_test_agreement_back[lenth], Test_result.IMSI, 15); // IMSI号
	lenth += 15;
	memcpy(&search_test_agreement_back[lenth], Test_result.ICCID, 20); // ICCID号
	lenth += 20;
	memcpy(&search_test_agreement_back[lenth], Test_result.SOFT_Version, 2); // 软件版本号
	lenth += 2;
//	memcpy(&search_test_agreement_back[lenth], Test_result.Comp_Time, 6); // 编译时间
//	lenth += 6;
//	search_test_agreement_back[lenth++] = Test_result.Pressure;				  // 压力状态
//	memcpy(&search_test_agreement_back[lenth], Test_result.PlatePressure, 4); // 板载压力
//	lenth += 4;
//	memcpy(&search_test_agreement_back[lenth], Test_result.ESAMID, 8); // ESAMID
//	lenth += 8;
//	Test_result.BD_Diode = 0;									// 默认通过
//	search_test_agreement_back[lenth++] = Test_result.BD_Diode; // 备电二极管

	for (count = 0; count < lenth; count++)
	{
		search_test_agreement_back[lenth] += search_test_agreement_back[count]; // 获取校验位的值
	}
	lenth++;
	search_test_agreement_back[lenth++] = 0x16; // 固定帧尾
	Uart0_Tx_Send(search_test_agreement_back, lenth);
}

/*————————PC协议通讯接收解析————————*/
void PC_Agreement_Analysis(uint8_t *Data, uint32_t lenth)
{
	uint8_t pHead = 0;
	uint8_t sum_jiaoyan = 0;
	uint8_t sum_jiaoyan_jisuan = 0;
	uint8_t PC_analysis_Data[200] = {0};
	uint8_t frame_lenth = 8; // 固定帧长度
	while (1)
	{
		if (pHead + 8 > lenth)
			break;
		if (Data[pHead] == 0x68 && Data[pHead + 1] == 0xAB)
		{
			memcpy(PC_analysis_Data, Data + pHead, frame_lenth); // 取出要分析的数据,从pHead开始的数据进行校验
			sum_jiaoyan = Data[pHead + frame_lenth - 2];
			sum_jiaoyan_jisuan = Sum_jiaoyan(PC_analysis_Data, frame_lenth);
			if (sum_jiaoyan == sum_jiaoyan_jisuan)
			{
				switch (Data[pHead + 4])
				{
				case 0x0A:
					if (Data[pHead + 5] == Test_result.Workstation && Data[pHead + frame_lenth - 1] == 0x16)
					{
						Test_Start();
					}
					break;
				case 0x0B:
					if (Data[pHead + 5] == Test_result.Workstation && Data[pHead + frame_lenth - 1] == 0x16 && Test_result.Find_Enable == 1)
					{
						Back_Search_Data();
					}
					break;
				default:
					break;
				}
				pHead += frame_lenth;
			}
		}
		pHead++;
	}
}
