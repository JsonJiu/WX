#include "GPIO.h"
#include "jiliang_xieyi_Ctrl.h"
#include "uart0.h"
#include "uart1.h"
#include "uart3.h"
#include "Test_List.h"
//这里代表着红外通信

uint8_t jiliang_xieyi_zuzhen[200];
uint8_t jiliang_kongshujuyu[1];
uint8_t jiliang_moren_biaohao[6] = {0xAA,0xAA,0xAA,0xAA,0xAA,0xAA};


//读通信地址  2031H  C=82H  无数据域
//

void JILIAN_TONGXIN_xieyijiexi(uint8_t zufuchua[],uint16_t lenth)
{
	uint16_t pHead = 0;
	uint16_t zhen_lenth = 0;
	uint8_t jiaoyanhe_jiaoyan = 0;
	uint8_t jiaoyanhe_jiaoyan_jisuan = 0;
	while(1)
	{
		//协议帧长度最小为18
		if(pHead+6>lenth)
		{
			break;
		}
		if(zufuchua[pHead]==0x68)
		{
			//确保数据帧完整性，读取帧长度
			zhen_lenth = 0;
			zhen_lenth = zufuchua[pHead+2];
			//若数据帧长度不足则不进入解析
			if(pHead+zhen_lenth<=lenth)
			{
				//校验最后一个字节是否是帧结束符
				if(zufuchua[pHead+zhen_lenth-1]==0x16)
				{
					//校验校验和是否为预期校验和
					jiaoyanhe_jiaoyan = 0;
					for(jiaoyanhe_jiaoyan_jisuan=0;jiaoyanhe_jiaoyan_jisuan<zhen_lenth-2;jiaoyanhe_jiaoyan_jisuan++)
					{
						jiaoyanhe_jiaoyan+=zufuchua[pHead+jiaoyanhe_jiaoyan_jisuan];
					}
					//和校验通过，认为协议正常
					if(jiaoyanhe_jiaoyan == zufuchua[pHead+zhen_lenth-2])
					{
						switch(zufuchua[pHead+3])
						{
							case 0:
								//无水状态
							  test_xieyi_jilu_Rec = w_get_kongshui;
							break;
							case 1:
								//满水桩体
							  test_xieyi_jilu_Rec = w_get_manshui;
							break;
							case 2:
								//走水状态
							  test_xieyi_jilu_Rec = w_get_zoushui;
							break;
						}
						//协议解析完成后，解析帧往前走
						pHead+=zhen_lenth;
					}
				}
			}
		}
		pHead++;
	}
}
//数据标识 控制码 表号 数据域数据 数据域长度
void JILIAN_TONGXIN_xieyifasong(uint8_t zhuangtai,uint8_t zoushuidaxiao)
{
	uint16_t xieyi_zongchangdu = 0;
  uint16_t hejianyan_changdu = 0;
	//帧起始符
	jiliang_xieyi_zuzhen[xieyi_zongchangdu++] = 0x68;
	//地址域
	jiliang_xieyi_zuzhen[xieyi_zongchangdu++] = 0xAA;
	//协议长度
	jiliang_xieyi_zuzhen[xieyi_zongchangdu++] = 0x07;
  //进入的状态 无水，有水，走水
	jiliang_xieyi_zuzhen[xieyi_zongchangdu++] = zhuangtai;
	//水流大小
	jiliang_xieyi_zuzhen[xieyi_zongchangdu++] = zoushuidaxiao;
	jiliang_xieyi_zuzhen[xieyi_zongchangdu] = 0;
	//校验码
	for(hejianyan_changdu=0;hejianyan_changdu<5;hejianyan_changdu++)
	{
		jiliang_xieyi_zuzhen[xieyi_zongchangdu]+=jiliang_xieyi_zuzhen[hejianyan_changdu];
	}
	xieyi_zongchangdu++;
	//帧结束符
	jiliang_xieyi_zuzhen[xieyi_zongchangdu++] = 0x16;
	PC_Chuankou_tongxin_Debug_send(jiliang_xieyi_zuzhen,xieyi_zongchangdu);
	Uart3_Tx_Send(jiliang_xieyi_zuzhen,xieyi_zongchangdu);
}
//更改计量参数 空管
void JILIAN_wushui_xieyi()
{
	JILIAN_TONGXIN_xieyifasong(0,0);
}
//更改计量参数 满水
void JILIAN_manshui_xieyi()
{
	JILIAN_TONGXIN_xieyifasong(1,0);
}
//更改计量参数 走水
void JILIAN_zoushui_xieyi()
{
	JILIAN_TONGXIN_xieyifasong(2,2);
}
