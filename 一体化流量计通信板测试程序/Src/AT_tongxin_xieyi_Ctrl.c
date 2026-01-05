#include "GPIO.h"
#include "AT_tongxin_xieyi_Ctrl.h"
#include "tongxin_xieyi_Ctrl.h"
#include "uart1.h"
#include "uart0.h"
#include "uart4.h"
#include "Test_List.h"
//这里代表着红外通信

uint8_t Send_AT[] = "AT\r\n";
uint8_t Send_ATE0[] = "ATE0\r\n";
uint8_t Send_ATI[] = "ATI\r\n";
uint8_t Send_AT_CPIN[] = "AT+CPIN?\r\n";
uint8_t Get_CPIN_READY[] = "+CPIN: READY\r\n";
uint8_t Send_AT_QCCID[] = "AT+QCCID\r\n";
uint8_t Get_QCCID[] = "+QCCID: ";
uint8_t Send_AT_GSN[] = "AT+GSN\r\n";
uint8_t Send_AT_CIMI[] = "AT+CIMI\r\n";
uint8_t Send_AT_CSQ[] = "AT+CSQ\r\n";
uint8_t Get_CSQ[] = "+CSQ: ";
//模组关机
uint8_t Send_AT_QPOWD[] = "AT+QPOWD=0\r\n";
uint8_t Get_QPOWD[] = "POWERED DOWN\r\n";
uint8_t Get_OK[] = "OK\r\n";


void AT_TONGXIN_xieyijiexi(uint8_t zufuchua[],uint16_t lenth)
{
	uint16_t pHead = 0;
	while(1)
	{
		if(pHead>=lenth)
		{
			break;
		}
		if(pHead+sizeof(Get_OK)-1<=lenth)
		{
			if(bijiao_zifuchuan(&zufuchua[pHead],Get_OK,sizeof(Get_OK)-1))
			{
				Test_quanju_canshu_L.time_softdelay_ms = 0;
				if(test_xieyi_jilu_Rec ==  No_Receive)
				{
						test_xieyi_jilu_Rec = w_OK;
						Test_quanju_canshu_L.time_softdelay_ms = 0;
				}
				if(test_xieyi_jilu_Rec == w_get_CIMI)
				{
					uint16_t xunhuan_diyigezifu=0;
					for(xunhuan_diyigezifu=0;xunhuan_diyigezifu<10;xunhuan_diyigezifu++)
					{
						if(zufuchua[xunhuan_diyigezifu]<0x40&&zufuchua[xunhuan_diyigezifu]>=0x30)
						break;
					}
					memcpy(Test_linshi_cunchushuju_L.IMEI_CHK,&zufuchua[xunhuan_diyigezifu],15);
					test_xieyi_jilu_Rec = w_OK;
					Test_quanju_canshu_L.time_softdelay_ms = 0;
				}
				if(test_xieyi_jilu_Rec ==  w_get_GSN)
				{
					uint16_t xunhuan_diyigezifu=0;
					for(xunhuan_diyigezifu=0;xunhuan_diyigezifu<10;xunhuan_diyigezifu++)
					{
						if(zufuchua[xunhuan_diyigezifu]<0x40&&zufuchua[xunhuan_diyigezifu]>=0x30)
						break;
					}
					memcpy(Test_linshi_cunchushuju_L.IMSI_CHK,&zufuchua[xunhuan_diyigezifu],15);
					test_xieyi_jilu_Rec = w_OK;
					Test_quanju_canshu_L.time_softdelay_ms = 0;
				}
				pHead+=(sizeof(Get_OK)-1);
			}
		}
		if(pHead+sizeof(Get_CPIN_READY)-1<=lenth)
		{
			if(bijiao_zifuchuan(&zufuchua[pHead],Get_CPIN_READY,sizeof(Get_CPIN_READY)-1))
			{
				Test_quanju_canshu_L.time_softdelay_ms = 0;
				test_xieyi_jilu_Rec = w_get_cpin;
				Test_quanju_canshu_L.time_softdelay_ms = 0;
				pHead+=(sizeof(Get_CPIN_READY)-1);
			}
		}
		if(pHead+sizeof(Get_QCCID)-1<=lenth)
		{
			if(bijiao_zifuchuan(&zufuchua[pHead],Get_QCCID,sizeof(Get_QCCID)-1))
			{
				Test_quanju_canshu_L.time_softdelay_ms = 0;
				memcpy(Test_linshi_cunchushuju_L.ICCID_CHK,&zufuchua[pHead+sizeof(Get_QCCID)-1],20);
				test_xieyi_jilu_Rec = w_get_QCCID;
				Test_quanju_canshu_L.time_softdelay_ms = 0;
				pHead+=(sizeof(Get_QCCID)-1);
			}
		}
		if(pHead+sizeof(Get_CSQ)-1<=lenth)
		{
			if(bijiao_zifuchuan(&zufuchua[pHead],Get_CSQ,sizeof(Get_CSQ)-1))
			{
				Test_quanju_canshu_L.time_softdelay_ms = 0;
				Test_linshi_cunchushuju_L.CSQ = zufuchua[pHead+sizeof(Get_CSQ)-1]-'0';
				Test_linshi_cunchushuju_L.CSQ = Test_linshi_cunchushuju_L.CSQ*10;
				Test_linshi_cunchushuju_L.CSQ += zufuchua[pHead+sizeof(Get_CSQ)]-'0';
				test_xieyi_jilu_Rec = w_get_CSQ;
				if(Test_linshi_cunchushuju_L.CSQ>33&&Test_linshi_cunchushuju_L.CSQ<5)
				{
					Test_quanju_canshu_L.time_softdelay_ms = 0;
				}
				pHead+=(sizeof(Get_CSQ)-1);
			}
		}
		if(pHead+sizeof(Get_QPOWD)-1<=lenth)
		{
			if(bijiao_zifuchuan(&zufuchua[pHead],Get_QPOWD,sizeof(Get_QPOWD)-1))
			{
				Test_quanju_canshu_L.time_softdelay_ms = 0;
				
				test_xieyi_jilu_Rec = w_OK;
				pHead+=(sizeof(Get_QPOWD)-1);
			}
		}
		pHead++;
	}
}
void To_send_AT()
{
	Uart4_Tx_Send(Send_AT,sizeof(Send_AT)-1);
}
void To_send_ATE0()
{
	Uart4_Tx_Send(Send_ATE0,sizeof(Send_ATE0)-1);
}
void To_send_ATI()
{
	Uart4_Tx_Send(Send_ATI,sizeof(Send_ATI)-1);
}
void To_send_CPIN()
{
	Uart4_Tx_Send(Send_AT_CPIN,sizeof(Send_AT_CPIN)-1);
}
void To_send_QCCID()
{
	Uart4_Tx_Send(Send_AT_QCCID,sizeof(Send_AT_QCCID)-1);
}
void To_send_GSN()
{
	Uart4_Tx_Send(Send_AT_GSN,sizeof(Send_AT_GSN)-1);
}
void To_send_CIMI()
{
	Uart4_Tx_Send(Send_AT_CIMI,sizeof(Send_AT_CIMI)-1);
}
void To_send_CSQ()
{
	Uart4_Tx_Send(Send_AT_CSQ,sizeof(Send_AT_CSQ)-1);
}
void To_send_QPOWD()
{
	Uart4_Tx_Send(Send_AT_QPOWD,sizeof(Send_AT_QPOWD)-1);
}


