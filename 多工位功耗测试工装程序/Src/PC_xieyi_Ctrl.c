#include "GPIO.h"
#include "PC_xieyi_Ctrl.h"
#include "Test_List.h"
#include "uart0.h"
#include "uart1.h"
#include "ZDINA219.h"
#define send_lenth 200
uint8_t xieyi1_fanhui[5] = {0x68,0xAB,0x00,0x13,0x16};
uint8_t xieyi2_fanhui[send_lenth];
uint8_t xuceshi_gongwei[6];
void xuceshi_gongwei_qingchu()
{
	memset(xuceshi_gongwei,0x00,6);
}
//开始测试设置成功后发送
void PC_xieyifasong_1(uint8_t dangqian_qingqiugongwei)
{
	xieyi1_fanhui[2] = dangqian_qingqiugongwei;
	xieyi1_fanhui[3] = 0x13+xieyi1_fanhui[2];
	//Uart0_Tx_Send(xieyi1_fanhui,5);
	PC_Chuankou_tongxin_send(xieyi1_fanhui,5);
}
//查询结果
void PC_xieyifasong_2(uint8_t dangqian_qingqiugongwei)
{
	uint16_t jishu_lenth = 0;
	uint16_t hejiaoyan = 0;
	memset(xieyi2_fanhui,0x00,send_lenth);
	xieyi2_fanhui[jishu_lenth++] = 0x68;
	xieyi2_fanhui[jishu_lenth++] = 0xAD;
	xieyi2_fanhui[jishu_lenth++] = dangqian_qingqiugongwei;
	xieyi2_fanhui[jishu_lenth++] = 0x02;
	xieyi2_fanhui[jishu_lenth++] = ((Test_jiejuo_jilu.zhudian_gonghao)>>8)&0xFF;
	xieyi2_fanhui[jishu_lenth++] = (Test_jiejuo_jilu.zhudian_gonghao)&0xFF;
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
			if(zufuchua[pHead+1]==0xAA&&zufuchua[pHead+4]==0x16)
			{
				//进行和校验
				hejiaoyan = 0;
				for(zhenchangdu=0;zhenchangdu<3;zhenchangdu++)
				{
					hejiaoyan+=zufuchua[pHead+zhenchangdu];
				}
				if(hejiaoyan==zufuchua[pHead+3])
				{
					xuceshi_gongwei[zufuchua[pHead+2]] = 0x01;
					PC_xieyifasong_1(zufuchua[pHead+2]);
					pHead+=15;
				}
			}
			else if(zufuchua[pHead+1]==0xAC&&zufuchua[pHead+4]==0x16)
			{
				//进行和校验
				hejiaoyan = 0;
				for(zhenchangdu=0;zhenchangdu<3;zhenchangdu++)
				{
					hejiaoyan+=zufuchua[pHead+zhenchangdu];
				}
				if(hejiaoyan==zufuchua[pHead+3])
				{
					Test_jiejuo_jilu.zhudian_gonghao = 0;
					//直接测试功耗
					Test_jiejuo_jilu.zhudian_gonghao = Current_CHK_Func(zufuchua[pHead+2]);
					PC_xieyifasong_2(zufuchua[pHead+2]);
					pHead+=3;
				}
			}
		}
		pHead++;
	}
}


