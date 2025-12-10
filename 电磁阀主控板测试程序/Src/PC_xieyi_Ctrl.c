#include "GPIO.h"
#include "PC_xieyi_Ctrl.h"
#include "Test_List.h"
#include "uart0.h"
#include "uart1.h"
#define send_lenth 200
uint8_t xieyi1_fanhui[5] = {0x68,0xAB,0x00,0x13,0x16};
uint8_t xieyi2_fanhui[send_lenth];
//开始测试设置成功后发送
void PC_xieyifasong_1()
{
	xieyi1_fanhui[2] = Test_jiejuo_jilu.gongwei;
	xieyi1_fanhui[3] = xieyi1_fanhui[2]+0x13;
	Uart1_Tx_Send(xieyi1_fanhui,5);
	PC_Chuankou_tongxin_Debug_send(xieyi1_fanhui,5);
}
//查询结果
void PC_xieyifasong_2()
{
	uint16_t jishu_lenth = 0;
	uint16_t hejiaoyan = 0;
	memset(xieyi2_fanhui,0x00,send_lenth);
	xieyi2_fanhui[jishu_lenth++] = 0x68;
	xieyi2_fanhui[jishu_lenth++] = 0xAD;
	xieyi2_fanhui[jishu_lenth++] = Test_jiejuo_jilu.gongwei;
	xieyi2_fanhui[jishu_lenth++] = ((Test_jiejuo_jilu.zhidian_gongdiandianya/10)>>8)&0xFF;
	xieyi2_fanhui[jishu_lenth++] = (Test_jiejuo_jilu.zhidian_gongdiandianya/10)&0xFF;
	xieyi2_fanhui[jishu_lenth++] = ((Test_jiejuo_jilu.zhidian_caijidianya)>>8)&0xFF;
	xieyi2_fanhui[jishu_lenth++] = (Test_jiejuo_jilu.zhidian_caijidianya)&0xFF;
	xieyi2_fanhui[jishu_lenth++] = (Test_jiejuo_jilu.zhudian_gonghao>>8)&0xFF;
	xieyi2_fanhui[jishu_lenth++] = (Test_jiejuo_jilu.zhudian_gonghao)&0xFF;
	xieyi2_fanhui[jishu_lenth++] = ((Test_jiejuo_jilu.SY_dianya_1_3/10)>>8)&0xFF;
	xieyi2_fanhui[jishu_lenth++] = (Test_jiejuo_jilu.SY_dianya_1_3/10)&0xFF;
	xieyi2_fanhui[jishu_lenth++] = ((Test_jiejuo_jilu.SY_dianya_2_3/10)>>8)&0xFF;
	xieyi2_fanhui[jishu_lenth++] = (Test_jiejuo_jilu.SY_dianya_2_3/10)&0xFF;
	xieyi2_fanhui[jishu_lenth++] = ((Test_jiejuo_jilu.VCC_dianya/10)>>8)&0xFF;
	xieyi2_fanhui[jishu_lenth++] = (Test_jiejuo_jilu.VCC_dianya/10)&0xFF;
	xieyi2_fanhui[jishu_lenth++] = Test_jiejuo_jilu.anjian_state;
	memcpy(&xieyi2_fanhui[jishu_lenth],Test_jiejuo_jilu.zhukongban_xingshan_MAC,12);
	jishu_lenth+=12;
	xieyi2_fanhui[jishu_lenth] = 0;
	for(hejiaoyan=0;hejiaoyan<jishu_lenth;hejiaoyan++)
	{
		xieyi2_fanhui[jishu_lenth]+=xieyi2_fanhui[hejiaoyan];
	}
	jishu_lenth++;
	xieyi2_fanhui[jishu_lenth++] = 0x16;
	PC_Chuankou_tongxin_send(xieyi2_fanhui,jishu_lenth);
}

void PC_xieyijiexi(uint8_t zufuchua[],uint16_t lenth)
{
	uint16_t pHead = 0;
	uint8_t hejiaoyan = 0;
	uint16_t zhenchangdu = 0;
	
	while(1)
	{
		if(pHead+4>lenth)
		{
			break;
		}
		if(zufuchua[pHead]==0x68)
		{
			if(zufuchua[pHead+1]==0xAA&&zufuchua[pHead+2]==Test_jiejuo_jilu.gongwei&&zufuchua[pHead+4]==0x16)
			{
				//进行和校验
				hejiaoyan = 0;
				for(zhenchangdu=0;zhenchangdu<3;zhenchangdu++)
				{
					hejiaoyan+=zufuchua[pHead+zhenchangdu];
				}
				if(hejiaoyan==zufuchua[pHead+3])
				{
					test_start();
					PC_xieyifasong_1();
					pHead+=4;
				}
			}
			else if(zufuchua[pHead+1]==0xAC&&zufuchua[pHead+2]==Test_jiejuo_jilu.gongwei&&zufuchua[pHead+4]==0x16)
			{
				//进行和校验
				hejiaoyan = 0;
				for(zhenchangdu=0;zhenchangdu<3;zhenchangdu++)
				{
					hejiaoyan+=zufuchua[pHead+zhenchangdu];
				}
				if(hejiaoyan==zufuchua[pHead+3])
				{
					if(Test_quanju_canshu_L.test_over==1)
					{
						PC_xieyifasong_2();
					}
					pHead+=3;
				}
			}
		}
		pHead++;
	}
}


