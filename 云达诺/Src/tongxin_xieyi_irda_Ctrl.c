#include "GPIO.h"
#include "tongxin_xieyi_irda_Ctrl.h"
#include "uart1.h"
#include "uart4.h"
#include "Test_List.h"
//这里代表着红外通信

uint8_t irad_xieyi_zuzhen[200];
uint8_t irad_kongshujuyu[1];
uint8_t irad_moren_biaohao[6] = {0xAA,0xAA,0xAA,0xAA,0xAA,0xAA};

//这里写比较字符串主要是原生比较会使\0就是结束符。
uint8_t irad_bijiao_zifuchuan(uint8_t bijiao1[],uint8_t bijiao2[],uint16_t lenth)
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

uint16_t irad_user_cmd_get_crc(unsigned char *crcbuf, int len)
{
    uint16_t x;
    uint16_t crc = 0;
	  uint16_t i=0;
    for (i=0; i<len ; i++)
    {
        x = ((crc >> 8) ^ (uint16_t)crcbuf[i]) & 0x0FF;
        x ^= (x >> 4);
        crc = (crc << 8) ^ (x << 12) ^ (x << 5) ^ x;
    }
    return crc & 0x0ffff;
}
//读通信地址  2031H  C=82H  无数据域
//

void irad_TONGXIN_xieyijiexi(uint8_t zufuchua[],uint16_t lenth)
{
	uint16_t pHead = 0;
	uint16_t zhen_lenth = 0;
	uint16_t CRC_jiaoyan = 0;
	uint16_t CRC_jiaoyan_jisuan = 0;
	uint16_t shujubiaoshi = 0;
	while(1)
	{
		//协议帧长度最小为18
		if(pHead+17>lenth)
		{
			break;
		}
		if(zufuchua[pHead]==0x68)
		{
			//确保数据帧完整性，读取帧长度
			zhen_lenth = 0;
			zhen_lenth = zufuchua[pHead+11];
			zhen_lenth = zhen_lenth<<8;
			zhen_lenth += zufuchua[pHead+10];
			//若数据帧长度不足则不进入解析
			if(pHead+zhen_lenth<=lenth)
			{
				//校验最后一个字节是否是帧结束符
				if(zufuchua[pHead+zhen_lenth-1]==0x16)
				{
					//获取协议中的CRC校验值
					CRC_jiaoyan = 0;
					CRC_jiaoyan = zufuchua[pHead+zhen_lenth-2];
					CRC_jiaoyan = CRC_jiaoyan<<8;
					CRC_jiaoyan += zufuchua[pHead+zhen_lenth-3];
					//执行CRC校验
					CRC_jiaoyan_jisuan = irad_user_cmd_get_crc(&zufuchua[pHead],zhen_lenth-3);
					//CRC校验通过，认为协议正常
					if(CRC_jiaoyan == CRC_jiaoyan_jisuan)
					{
						//先记录地址域，用于后续的协议通信
						memcpy(Test_jiejuo_jilu.dangqian_biaohao,&zufuchua[pHead+1],6);
						//协议类型选择
						switch(zufuchua[pHead+7])
						{
							case 0x00:
								//协议版本号选择
							  switch(zufuchua[pHead+8])
								{
									case 0x00:
										//控制码校验
										switch(zufuchua[pHead+9])
										{
											//这里加密与后续标志不解析
											//上传数据返回(也有直接回复的指令)
											case 0x81:
												break;
											//读数据返回
											case 0x82:
												//进入数据标识解析
											  shujubiaoshi = 0;
											  shujubiaoshi = zufuchua[pHead+13];
											  shujubiaoshi = shujubiaoshi<<8;
											  shujubiaoshi += zufuchua[pHead+12];
											  //pHead+15
											  switch(shujubiaoshi)
												{
													//获取表号
													case 0x2031:
														//表号无数据域
													  Test_quanju_canshu_L.time_softdelay_ms = 0;
													  test_xieyi_jilu_irda_rec = h_get_biaohao_chuankou;
														break;
													default:
														break;
												}
												break;
											//读后续数据
											case 0x83:
												break;
											//写数据
											case 0x84:
												//进入数据标识解析
											  shujubiaoshi = 0;
											  shujubiaoshi = zufuchua[pHead+13];
											  shujubiaoshi = shujubiaoshi<<8;
											  shujubiaoshi += zufuchua[pHead+12];
											  //pHead+15
											  switch(shujubiaoshi)
												{
													default:
														break;
												}
												break;
											//远程升级
											case 0x86:
												break;
											default:
												break;
										}
										break;
									default:
										break;
								}
								break;
							default:
								break;
						}
						//协议解析完成后，进入同一帧下一条协议解析
						pHead+=zhen_lenth;
					}
				}
			}
		}
		pHead++;
	}
}
//数据标识 控制码 表号 数据域数据 数据域长度
void irad_TONGXIN_xieyifasong(uint16_t shujubiaoshi,uint8_t kongzhima,uint8_t w_biaohao[],uint8_t shujuyu[],uint16_t shujuyu_changdu)
{
	uint16_t xieyi_zongchangdu = 0;
	uint16_t CRCjianyan = 0;
	//帧起始符
	irad_xieyi_zuzhen[xieyi_zongchangdu++] = 0x68;
	//地址域
	irad_xieyi_zuzhen[xieyi_zongchangdu++] = w_biaohao[0];
	irad_xieyi_zuzhen[xieyi_zongchangdu++] = w_biaohao[1];
	irad_xieyi_zuzhen[xieyi_zongchangdu++] = w_biaohao[2];
	irad_xieyi_zuzhen[xieyi_zongchangdu++] = w_biaohao[3];
	irad_xieyi_zuzhen[xieyi_zongchangdu++] = w_biaohao[4];
	irad_xieyi_zuzhen[xieyi_zongchangdu++] = w_biaohao[5];
	//协议类型
	irad_xieyi_zuzhen[xieyi_zongchangdu++] = 0x00;
	//协议版本号
	irad_xieyi_zuzhen[xieyi_zongchangdu++] = 0x00;
	//控制码
	irad_xieyi_zuzhen[xieyi_zongchangdu++] = kongzhima;
	//帧长度
	irad_xieyi_zuzhen[xieyi_zongchangdu++] = (shujuyu_changdu+18)&0xFF;
	irad_xieyi_zuzhen[xieyi_zongchangdu++] = ((shujuyu_changdu+18)>>8)&0xFF;
	//数据标识
	irad_xieyi_zuzhen[xieyi_zongchangdu++] = (shujubiaoshi&0xFF);
	irad_xieyi_zuzhen[xieyi_zongchangdu++] = ((shujubiaoshi>>8)&0xFF);
	//消息序号，永远是0
	irad_xieyi_zuzhen[xieyi_zongchangdu++] = 0x00;
	//数据域
	memcpy(&irad_xieyi_zuzhen[xieyi_zongchangdu],shujuyu,shujuyu_changdu);
	xieyi_zongchangdu+=shujuyu_changdu;
	//校验码
	CRCjianyan = irad_user_cmd_get_crc(irad_xieyi_zuzhen,xieyi_zongchangdu);
	irad_xieyi_zuzhen[xieyi_zongchangdu++] = CRCjianyan&0xFF;
	irad_xieyi_zuzhen[xieyi_zongchangdu++] = (CRCjianyan>>8)&0xFF;
	//帧结束符
	irad_xieyi_zuzhen[xieyi_zongchangdu++] = 0x16;
	Uart4_Tx_Send_ok(irad_xieyi_zuzhen,xieyi_zongchangdu);
}
//获取当前表号
void irad_find_biaohao_xieyi()
{
	irad_TONGXIN_xieyifasong(0x2031,0x02,irad_moren_biaohao,irad_kongshujuyu,0);
}
