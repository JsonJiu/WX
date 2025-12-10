#include "GPIO.h"
#include "tongxin_xieyi_Ctrl.h"
#include "uart1.h"
#include "uart0.h"
#include "Test_List.h"
//这里代表着红外通信

uint8_t xieyi_zuzhen[200];
uint8_t kongshujuyu[1];
uint8_t moren_biaohao[6] = {0xAA,0xAA,0xAA,0xAA,0xAA,0xAA};

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

uint16_t user_cmd_get_crc(unsigned char *crcbuf, int len)
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

void TONGXIN_xieyijiexi(uint8_t zufuchua[],uint16_t lenth)
{
	uint16_t pHead = 0;
	uint16_t zhen_lenth = 0;
	uint16_t CRC_jiaoyan = 0;
	uint16_t CRC_jiaoyan_jisuan = 0;
	uint16_t shujubiaoshi = 0;
	uint8_t  lora_changdu = 0;
	uint8_t dian1 = 0;
	uint8_t dian2 = 0;
	uint8_t dian3 = 0;
	uint16_t jiexi_changdu = 0;
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
					CRC_jiaoyan_jisuan = user_cmd_get_crc(&zufuchua[pHead],zhen_lenth-3);
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
													  test_xieyi_jilu_Rec = w_get_biaohao;
														break;
													//用于上告
													case 0xF001:
														//上告信息无数据域
													  Test_quanju_canshu_L.time_softdelay_ms = 0;
													  test_xieyi_jilu_Rec = w_get_shanggao;
														break;
													//获取无磁信号强度
													case 0x1100:
														//这个板子没有flash
														Test_linshi_cunchushuju_L.L_wucijiliang_xinhaoqiangdu = zufuchua[pHead+22];
													  Test_quanju_canshu_L.time_softdelay_ms = 0;
													  test_xieyi_jilu_Rec = w_get_zhukongban_wuci_xinhao;
														break;
													//获取版本号
													case 0x2011:
														//先获取版本号，共32位
															//寻找协议中的三个点
															while(1)
															{
																if(zufuchua[pHead+17+jiexi_changdu]==0)
																	break;
																if(dian1==0)
																{
																	if(zufuchua[pHead+17+jiexi_changdu]=='.')
																	{
																		dian1 = jiexi_changdu;
																	}
																}
																else if(dian2==0)
																{
																	if(zufuchua[pHead+17+jiexi_changdu]=='.')
																	{
																		dian2 = jiexi_changdu;
																	}
																}
																else if(dian3==0)
																{
																	if(zufuchua[pHead+17+jiexi_changdu]=='.')
																	{
																		dian3 = jiexi_changdu;
																	}
																}
																jiexi_changdu++;
															}
															if(dian1==1)
															{
																Test_linshi_cunchushuju_L.L_banbenhao[0] = zufuchua[pHead+17]-'0';
															}
															else if(dian1==2)
															{
																Test_linshi_cunchushuju_L.L_banbenhao[0] = (zufuchua[pHead+17]-'0')<<4;
																Test_linshi_cunchushuju_L.L_banbenhao[0] += zufuchua[pHead+18]-'0';
															}
															if((dian2-dian1)==2)
															{
																Test_linshi_cunchushuju_L.L_banbenhao[1] = (zufuchua[pHead+17+dian1+1]-'0')<<4;
															}
															if((jiexi_changdu-dian2)==2)
															{
																Test_linshi_cunchushuju_L.L_banbenhao[1] +=(zufuchua[pHead+17+dian2+1]-'0');
															}
													  Test_quanju_canshu_L.time_softdelay_ms = 0;
														test_xieyi_jilu_Rec = w_get_banbenhao;
														break;
													//测试专用协议
													case 0xF003:
														//flash检测
													  //这个板子没有flash
														Test_linshi_cunchushuju_L.L_flash_chk = zufuchua[pHead+17];
													  //主电AD采样检测
													  Test_linshi_cunchushuju_L.L_zhudian_dianya = zufuchua[pHead+19];
													  Test_linshi_cunchushuju_L.L_zhudian_dianya = Test_linshi_cunchushuju_L.L_zhudian_dianya<<8;
													  Test_linshi_cunchushuju_L.L_zhudian_dianya += zufuchua[pHead+18];
													  //备电AD采样检测
													  Test_linshi_cunchushuju_L.L_beidian_dianya = zufuchua[pHead+21];
													  Test_linshi_cunchushuju_L.L_beidian_dianya = Test_linshi_cunchushuju_L.L_beidian_dianya<<8;
													  Test_linshi_cunchushuju_L.L_beidian_dianya += zufuchua[pHead+20];
													  //压力传感器读取
													  Test_linshi_cunchushuju_L.L_yali_CHK = zufuchua[pHead+22];
													  //EEPROM状态读取
													  Test_linshi_cunchushuju_L.L_EEPROM_CHK = zufuchua[pHead+23];
													  //霍尔1状态读取
													  Test_linshi_cunchushuju_L.L_huoer1_CHK = zufuchua[pHead+24];
													  //GPRS状态读取
													  Test_linshi_cunchushuju_L.L_GPRS_mozu_CHK = zufuchua[pHead+25];
													  //霍尔2状态读取
													  Test_linshi_cunchushuju_L.L_huoer2_CHK = zufuchua[pHead+26];
													  //超声瞬时流量读取
														memcpy(Test_linshi_cunchushuju_L.L_chaosheng_shunshiliuliang,&zufuchua[pHead+27],4);
														//走水状态读取
													  Test_linshi_cunchushuju_L.L_chaosheng_zousuizhuangtai = zufuchua[pHead+31];
														//GP30状态读取
														Test_linshi_cunchushuju_L.L_GP30mozu_AD_CHK = zufuchua[pHead+33];
														Test_linshi_cunchushuju_L.L_GP30mozu_AD_CHK = Test_linshi_cunchushuju_L.L_GP30mozu_AD_CHK<<8;
														Test_linshi_cunchushuju_L.L_GP30mozu_AD_CHK += zufuchua[pHead+32];
														//IMEI号获取
														memcpy(Test_linshi_cunchushuju_L.L_IMEI,&zufuchua[pHead+34],15);
														//IMSI号获取
														memcpy(Test_linshi_cunchushuju_L.L_IMSI,&zufuchua[pHead+49],15);
														//ICCID号获取
														memcpy(Test_linshi_cunchushuju_L.L_ICCID,&zufuchua[pHead+64],20);
														//CSQ获取
													  Test_linshi_cunchushuju_L.L_CSQ = zufuchua[pHead+84];
														//lorakey(这里存在一些问题，目前只有一位)
														Test_linshi_cunchushuju_L.lorakey[0] = zufuchua[pHead+85];
														lora_changdu = 0;
														//到位1信号
														Test_linshi_cunchushuju_L.L_daowei1 = zufuchua[pHead+86+lora_changdu];
														//到位2信号
														Test_linshi_cunchushuju_L.L_daowei2 = zufuchua[pHead+87+lora_changdu];
														//计量霍尔1信号获取
														Test_linshi_cunchushuju_L.L_jilianghuoer1 = zufuchua[pHead+88+lora_changdu];
														//计量霍尔2信号获取
														Test_linshi_cunchushuju_L.L_jilianghuoer2 = zufuchua[pHead+89+lora_changdu];
													  //无磁计量信号获取
														Test_linshi_cunchushuju_L.L_wucijiliang = zufuchua[pHead+90+lora_changdu];
														if(Test_linshi_cunchushuju_L.L_CSQ<33&&Test_linshi_cunchushuju_L.L_CSQ>10)
														{
															Test_quanju_canshu_L.time_softdelay_ms = 0;
														}
														DeBug_print("查询上告信息返回\r\n");														
														test_xieyi_jilu_Rec = w_get_test_zhuanyong;
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
													//阀门动作协议
													case 0xC022:
														if(zufuchua[pHead+15]==0x00)
														{
															test_xieyi_jilu_Rec = w_get_famen_dongzuo;
														}
														break;
													//获取信号强度到主控板
													case 0x1101:
														if(zufuchua[pHead+15]==0x00)
														{
															test_xieyi_jilu_Rec = w_set_wuci_xinhao_huoqu;
														}
														break;
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
void TONGXIN_xieyifasong(uint16_t shujubiaoshi,uint8_t kongzhima,uint8_t w_biaohao[],uint8_t shujuyu[],uint16_t shujuyu_changdu)
{
	uint16_t xieyi_zongchangdu = 0;
	uint16_t CRCjianyan = 0;
	//帧起始符
	xieyi_zuzhen[xieyi_zongchangdu++] = 0x68;
	//地址域
	xieyi_zuzhen[xieyi_zongchangdu++] = w_biaohao[0];
	xieyi_zuzhen[xieyi_zongchangdu++] = w_biaohao[1];
	xieyi_zuzhen[xieyi_zongchangdu++] = w_biaohao[2];
	xieyi_zuzhen[xieyi_zongchangdu++] = w_biaohao[3];
	xieyi_zuzhen[xieyi_zongchangdu++] = w_biaohao[4];
	xieyi_zuzhen[xieyi_zongchangdu++] = w_biaohao[5];
	//协议类型
	xieyi_zuzhen[xieyi_zongchangdu++] = 0x00;
	//协议版本号
	xieyi_zuzhen[xieyi_zongchangdu++] = 0x00;
	//控制码
	xieyi_zuzhen[xieyi_zongchangdu++] = kongzhima;
	//帧长度
	xieyi_zuzhen[xieyi_zongchangdu++] = (shujuyu_changdu+18)&0xFF;
	xieyi_zuzhen[xieyi_zongchangdu++] = ((shujuyu_changdu+18)>>8)&0xFF;
	//数据标识
	xieyi_zuzhen[xieyi_zongchangdu++] = (shujubiaoshi&0xFF);
	xieyi_zuzhen[xieyi_zongchangdu++] = ((shujubiaoshi>>8)&0xFF);
	//消息序号，永远是0
	xieyi_zuzhen[xieyi_zongchangdu++] = 0x00;
	//数据域
	memcpy(&xieyi_zuzhen[xieyi_zongchangdu],shujuyu,shujuyu_changdu);
	xieyi_zongchangdu+=shujuyu_changdu;
	//校验码
	CRCjianyan = user_cmd_get_crc(xieyi_zuzhen,xieyi_zongchangdu);
	xieyi_zuzhen[xieyi_zongchangdu++] = CRCjianyan&0xFF;
	xieyi_zuzhen[xieyi_zongchangdu++] = (CRCjianyan>>8)&0xFF;
	//帧结束符
	xieyi_zuzhen[xieyi_zongchangdu++] = 0x16;
	Uart1_Tx_Send_ok(xieyi_zuzhen,xieyi_zongchangdu);
}
//获取当前表号
void find_biaohao_xieyi()
{
	TONGXIN_xieyifasong(0x2031,0x02,moren_biaohao,kongshujuyu,0);
}
//上告协议
void send_shanggao_xieyi()
{
	TONGXIN_xieyifasong(0xF001,0x02,Test_jiejuo_jilu.dangqian_biaohao,kongshujuyu,0);
}
//阀门动作协议 
void FM_Open_xieyi()
{
	uint8_t shujuyu[] = {0x1D};
	TONGXIN_xieyifasong(0xC022,0x04,Test_jiejuo_jilu.dangqian_biaohao,shujuyu,1);
}
//测试专用协议
void Test_zhuanyong_xieyi()
{
	TONGXIN_xieyifasong(0xF003,0x02,Test_jiejuo_jilu.dangqian_biaohao,kongshujuyu,0);
}
//获取当前版本号
void find_banbenhao_xieyi()
{
	TONGXIN_xieyifasong(0x2011,0x02,Test_jiejuo_jilu.dangqian_biaohao,kongshujuyu,0);
}
//主控板获取当前无磁采样信号强度
void get_wucicaiyang_xieyi()
{
	TONGXIN_xieyifasong(0x1101,0x04,Test_jiejuo_jilu.dangqian_biaohao,kongshujuyu,0);
}
//获取主控板获取的无磁采样信号强度
void get_wucicaiyang_zhukongban_xieyi()
{
	TONGXIN_xieyifasong(0x1100,0x02,Test_jiejuo_jilu.dangqian_biaohao,kongshujuyu,0);
}
