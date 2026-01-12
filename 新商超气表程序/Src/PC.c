#include "PC.h"
#include "jiaoyan.h"
#include "uart0.h"
#include "test.h"

extern struct Test Test_result;
#define send_lenth 200
uint8_t start_test_agreement_back[7] = {0x68,0xAB,0x07,0x00,0x00,0x19,0x16};
uint8_t search_test_agreement_back[send_lenth];

/*————————开始测试PC协议返回————————*/
void Back_Start_Test(uint8_t *Data, uint32_t lenth)
{
	start_test_agreement_back[3] = Test_result.Workstation;
	start_test_agreement_back[4] = Test_result.Type;
	start_test_agreement_back[5] = Sum_jiaoyan(Data,lenth);
	Uart0_Tx_Send(Data,lenth);
}

/*————————测试数据返回————————*/
void Back_Search_Data(void)
{
	uint16_t lenth = 0;
	uint8_t count = 0;
	memset(search_test_agreement_back,0x00,send_lenth);
	search_test_agreement_back[lenth++] = 0x68;																					//固定帧头
	search_test_agreement_back[lenth++] = 0xBC;																					//功能码
	search_test_agreement_back[lenth++] = 0x00; 																				//先给0，后面重新赋值
	search_test_agreement_back[lenth++] = Test_result.Workstation;  										//工位
	search_test_agreement_back[lenth++] = Test_result.Type;															//类型
	

	search_test_agreement_back[lenth++] = Test_result.Voltage_3V6_Mould & 0xFF; 				//工装3.6V供电
	search_test_agreement_back[lenth++] = (Test_result.Voltage_3V6_Mould>>8) & 0xFF;
	
	search_test_agreement_back[lenth++] = Test_result.Voltage_7V2_Mould; 								//工装7.2V供电
	search_test_agreement_back[lenth++] = (Test_result.Voltage_7V2_Mould>>8) & 0xFF; 
	search_test_agreement_back[lenth++] = Test_result.UMU_Voltage & 0xFF; 							//计量电压协议读取
	search_test_agreement_back[lenth++] = (Test_result.UMU_Voltage>>8) & 0xFF;
	search_test_agreement_back[lenth++] = Test_result.NB_Voltage & 0xFF; 								//通讯电压协议读取
	search_test_agreement_back[lenth++] = (Test_result.NB_Voltage>>8) & 0xFF;
	

	search_test_agreement_back[lenth++] = Test_result.Current_3V6 & 0xFF;								//3.6V功耗
	search_test_agreement_back[lenth++] = (Test_result.Current_3V6>>8) & 0xFF;	
	
	search_test_agreement_back[lenth++] = Test_result.Current_7V2 & 0xFF;								//7.2V功耗
	search_test_agreement_back[lenth++] = (Test_result.Current_7V2>>8) & 0xFF;	
	
	search_test_agreement_back[lenth++] = Test_result.CSQ;															//信号强度
	
	search_test_agreement_back[lenth++] = Test_result.Infrared;													//红外
	search_test_agreement_back[lenth++] = Test_result.UMU_test;													//UMU测试
	search_test_agreement_back[lenth++] = Test_result.NB_test;													//通讯测试
	search_test_agreement_back[lenth++] = Test_result.LED;															//LED测试
	search_test_agreement_back[lenth++] = Test_result.Motor_test;												//阀门测试
	search_test_agreement_back[lenth++] = Test_result.IC_test;													//IC卡测试
	search_test_agreement_back[lenth++] = Test_result.flash;														//flash
	
	search_test_agreement_back[lenth++] = Test_result.eeprom;														//eeprom

	search_test_agreement_back[lenth++] = Test_result.PT;																//温压测试

	search_test_agreement_back[lenth++] = Test_result.KEY_OPEN;													//开盖检测
	search_test_agreement_back[lenth++] = Test_result.Alarm119;													//119检测
	search_test_agreement_back[2] = lenth+2;																						//长度赋值
	for(count=0;count<lenth;count++)   
	{
		search_test_agreement_back[lenth]+=search_test_agreement_back[count];  						//获取校验位的值
	}
	lenth++;
	search_test_agreement_back[lenth++] = 0x16;																						//固定帧尾
	Uart0_Tx_Send(search_test_agreement_back,lenth);
}


/*————————PC协议通讯接收解析————————*/
void PC_Agreement_Analysis(uint8_t *Data, uint32_t lenth)
{
	uint8_t pHead = 0;
	uint16_t frame_lenth = 0;
	uint8_t sum_jiaoyan = 0;
	uint8_t sum_jiaoyan_jisuan = 0;
	uint8_t PC_analysis_Data[200] = {0};
	while(1)
	{
		if(pHead+7>lenth)
			break;
		if(Data[pHead] == 0x68)
		{
			frame_lenth = Data[pHead+2];
			if(pHead+frame_lenth<=lenth)
			{
				memcpy(PC_analysis_Data,Data+pHead,frame_lenth);//取出要分析的数据,从pHead开始的数据进行校验
				sum_jiaoyan = Data[pHead+frame_lenth-2];
				sum_jiaoyan_jisuan = Sum_jiaoyan(PC_analysis_Data,frame_lenth);
				if(sum_jiaoyan == sum_jiaoyan_jisuan)
				{
					switch(Data[pHead+1])
					{
						case 0xAA:
							if(Data[pHead+3] == Test_result.Workstation && Data[pHead+frame_lenth-1] == 0x16)
							{
								Test_result.Type = Data[pHead+4];
								Back_Start_Test(start_test_agreement_back,7);
								Test_Start();
								pHead+=frame_lenth;
							}
							break;
						case 0xBB:
							if(Data[pHead+3] == Test_result.Workstation && Data[pHead+frame_lenth-1] == 0x16 && Test_result.Find_Enable == 1)
							{
								Back_Search_Data();
								pHead+=frame_lenth;
							}
							break;
						default:
							break;
					
					}
				
				}
			}
		
		
		}
		pHead++;
	}
}


