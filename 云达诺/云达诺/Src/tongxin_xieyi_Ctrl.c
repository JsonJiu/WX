#include "GPIO.h"
#include "tongxin_xieyi_Ctrl.h"
#include "uart1.h"
#include "uart0.h"
#include "Test_List.h"
#include "crc.h"
//这里代表着红外通信

uint8_t xieyi_zuzhen[200];
uint8_t kongshujuyu[1];
uint8_t moren_biaohao[6] = {0xAA,0xAA,0xAA,0xAA,0xAA,0xAA};

extern struct Test_linshi_cunchushuju Test_linshi_cunchushuju_L;

void TONGXIN_xieyijiexi(uint8_t *Data,uint16_t len)
{
	uint16_t pHead = 0;
	uint16_t zhen_lenth = 0;
	uint16_t CRC_jiaoyan = 0;
	uint16_t CRC_jiaoyan_jisuan = 0;
	uint16_t DID = 0;//数据标识
	uint8_t C = 0; //控制码
	uint8_t dian1 = 0;
	uint8_t dian2 = 0;
	uint8_t dian3 = 0;
	uint16_t jiexi_changdu = 0;
	while(1)
	{
		//最小20个字节
		if(pHead+20>len)
		{
			break;
		}
		if(Data[pHead]==0x68)
		{
			//确保数据帧完整性，读取帧长度
			zhen_lenth = Data[pHead+11];
			zhen_lenth = (zhen_lenth<<8) + Data[pHead+10];
			C = Data[pHead+9];
			DID = Data[pHead+13];
			DID = (DID<<8) + Data[pHead+12];
			//若数据帧长度不足则不进入解析
			if(pHead+zhen_lenth<=len)
			{
				CRC_jiaoyan = 0;
				CRC_jiaoyan = Data[pHead+zhen_lenth-2];
				CRC_jiaoyan = (CRC_jiaoyan<<8) + Data[pHead+zhen_lenth-3];
				CRC_jiaoyan_jisuan = CRC16_XMODEM(Data,zhen_lenth-3);
				if(CRC_jiaoyan_jisuan == CRC_jiaoyan)
				{
					switch(C)
					{
						case 0x82: //读控制码
							switch(DID)
							{
								case 0x2031:  //读取表号协议返回
									Test_quanju_canshu_L.time_softdelay_ms = 0;
									test_xieyi_jilu_Rec = w_get_biaohao;
									Test_jiejuo_jilu.hongwai_jiance = 1;  //红外检测正常
									memcpy(Test_jiejuo_jilu.dangqian_biaohao,&Data[pHead+17],6);    //获取实际表号
									break;
								case 0xF001:  //上告协议返回
									Test_quanju_canshu_L.time_softdelay_ms = 0;
									test_xieyi_jilu_Rec = w_get_shanggao;
									break;
								case 0x2604:
									Test_quanju_canshu_L.time_softdelay_ms = 0;
									Test_jiejuo_jilu.FM_Status = Data[pHead+19];
									test_xieyi_jilu_Rec = w_get_famen;
									break;
								case 0x1212:
									Test_quanju_canshu_L.time_softdelay_ms = 0;
									test_xieyi_jilu_Rec = w_get_yali_status;
									if(zhen_lenth == 20)
									{
										Test_jiejuo_jilu.YaLi_Status = wuyali;
									}
									else if(zhen_lenth == 22)
									{
										Test_jiejuo_jilu.YaLi_Status = youyali;
									}
									break;
								//获取版本号
								case 0x2011:
									//先获取版本号，共32位
									//寻找协议中的三个点
									while(1)
									{
										if(Data[pHead+17+jiexi_changdu]==0)
											break;
										if(dian1==0)
										{
											if(Data[pHead+17+jiexi_changdu]=='.')
											{
												dian1 = jiexi_changdu;
											}
										}
										else if(dian2==0)
										{
											if(Data[pHead+17+jiexi_changdu]=='.')
											{
												dian2 = jiexi_changdu;
											}
										}
										else if(dian3==0)
										{
											if(Data[pHead+17+jiexi_changdu]=='.')
											{
												dian3 = jiexi_changdu;
											}
										}
										jiexi_changdu++;
									}
									if(dian1==1)
									{
										Test_linshi_cunchushuju_L.L_banbenhao[0] = Data[pHead+17]-'0';
									}
									else if(dian1==2)
									{
										Test_linshi_cunchushuju_L.L_banbenhao[0] = (Data[pHead+17]-'0')<<4;
										Test_linshi_cunchushuju_L.L_banbenhao[0] += Data[pHead+18]-'0';
									}
									if((dian2-dian1)==2)
									{
										Test_linshi_cunchushuju_L.L_banbenhao[1] = (Data[pHead+17+dian1+1]-'0')<<4;
									}
									if((jiexi_changdu-dian2)==2)
									{
										Test_linshi_cunchushuju_L.L_banbenhao[1] +=(Data[pHead+17+dian2+1]-'0');
									}
										Test_quanju_canshu_L.time_softdelay_ms = 0;
										test_xieyi_jilu_Rec = w_get_banbenhao;
									break;
								case 0xF003:  //查询协议返回
									//flash获取
									Test_linshi_cunchushuju_L.L_flash_chk = Data[pHead+17];
									//水表3.6V主电协议读取
									Test_linshi_cunchushuju_L.L_zhudian_dianya = Data[pHead+19];
									Test_linshi_cunchushuju_L.L_zhudian_dianya = Test_linshi_cunchushuju_L.L_zhudian_dianya<<8;
									Test_linshi_cunchushuju_L.L_zhudian_dianya += Data[pHead+18];
									//水表3.6V备电协议读取 此表没有备电，默认返回100E
									Test_linshi_cunchushuju_L.L_beidian_dianya = Data[pHead+21];
									Test_linshi_cunchushuju_L.L_beidian_dianya = Test_linshi_cunchushuju_L.L_beidian_dianya<<8;
									Test_linshi_cunchushuju_L.L_beidian_dianya += Data[pHead+20];	
									//压力传感器读取
									Test_linshi_cunchushuju_L.L_yali_CHK = Data[pHead+22];
									//eeprom读取 
									Test_linshi_cunchushuju_L.L_EEPROM_CHK = Data[pHead+23];
									//磁干扰状态读取
									Test_linshi_cunchushuju_L.L_ciganrao_CHK = Data[pHead+24];
									//模组检测
									Test_linshi_cunchushuju_L.L_GPRS_mozu_CHK = Data[pHead+25];
									//开盖状态读取  此表没有，默认返回1
									Test_linshi_cunchushuju_L.L_kaigai_CHK = Data[pHead+26];
									//超声波瞬时流量   此表非超声波，返回0
									memcpy(Test_linshi_cunchushuju_L.L_chaosheng_shunshiliuliang,&Data[pHead+27],4);
									//超声波走水状态  此表非超声波，返回0
									Test_linshi_cunchushuju_L.L_chaosheng_zousuizhuangtai = Data[pHead+31];
									//GP30模组电压检测  此表没有GP30，返回100E
									Test_linshi_cunchushuju_L.L_GP30mozu_AD_CHK = Data[pHead+33];
									Test_linshi_cunchushuju_L.L_GP30mozu_AD_CHK = Test_linshi_cunchushuju_L.L_GP30mozu_AD_CHK<<8;
									Test_linshi_cunchushuju_L.L_GP30mozu_AD_CHK += Data[pHead+32];			
									//IMEI号
									memcpy(Test_linshi_cunchushuju_L.L_IMEI,&Data[pHead+34],15);
									//IMSI号
									memcpy(Test_linshi_cunchushuju_L.L_IMSI,&Data[pHead+49],15);
									//ICCID号
									memcpy(Test_linshi_cunchushuju_L.L_ICCID,&Data[pHead+64],20);
									//CSQ信号强度
									Test_linshi_cunchushuju_L.L_CSQ = Data[pHead+84];
									//lora号  此表不是  默认0
									memcpy(Test_linshi_cunchushuju_L.lorakey,&Data[pHead+85],16);
									//关到位
									Test_linshi_cunchushuju_L.L_daowei1 = Data[pHead+101];
									//开到位
									Test_linshi_cunchushuju_L.L_daowei2 = Data[pHead+102];
									//计量霍尔
									Test_linshi_cunchushuju_L.L_jilianghuoer1 = Data[pHead+103];
									//计量霍尔
									Test_linshi_cunchushuju_L.L_jilianghuoer2 = Data[pHead+104];
									//无磁计量计量值
									Test_linshi_cunchushuju_L.L_wucijiliang = Data[pHead+105];
									
									Test_quanju_canshu_L.time_softdelay_ms = 0;
									test_xieyi_jilu_Rec = w_get_test_zhuanyong;
									break;
									case 0x9010:
									Test_linshi_cunchushuju_L.L_youcijiliang = Data[pHead+20];
									Test_linshi_cunchushuju_L.L_youcijiliang = (Test_linshi_cunchushuju_L.L_youcijiliang<<8) + Data[pHead+19];
									Test_linshi_cunchushuju_L.L_youcijiliang = (Test_linshi_cunchushuju_L.L_youcijiliang<<8) + Data[pHead+18];
									Test_linshi_cunchushuju_L.L_youcijiliang = (Test_linshi_cunchushuju_L.L_youcijiliang<<8) + Data[pHead+17];
									Test_quanju_canshu_L.time_softdelay_ms = 0;
									test_xieyi_jilu_Rec = w_get_jiliang_L;
								default:
									break;
							}		
							break;
						case 0x84: //写控制码
							switch(DID)
							{
								//阀门动作协议
								case 0xC022:
								if(Data[pHead+15]==0x00)
								{
									test_xieyi_jilu_Rec = w_get_famen_dongzuo;
								}
									break;
								case 0x1101:
									Test_quanju_canshu_L.time_softdelay_ms = 0;
									test_xieyi_jilu_Rec = w_get_yali;
									break;
								default:
									break;
							}							
							break;
						default:
							break;
					}
				}
			//协议解析完成后，进入同一帧下一条协议解析
			pHead+=zhen_lenth;
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
	xieyi_zuzhen[xieyi_zongchangdu++] = 0x00;    //协议类型PT
	//协议版本号
	xieyi_zuzhen[xieyi_zongchangdu++] = 0x0A;    //协议版本号0.0
	//控制码
	xieyi_zuzhen[xieyi_zongchangdu++] = kongzhima;  
	//帧长度
	xieyi_zuzhen[xieyi_zongchangdu++] = (shujuyu_changdu+18)&0xFF;   //低8位
	xieyi_zuzhen[xieyi_zongchangdu++] = ((shujuyu_changdu+18)>>8)&0xFF;  //高8位
	//数据标识
	xieyi_zuzhen[xieyi_zongchangdu++] = (shujubiaoshi&0xFF);   //低8位
	xieyi_zuzhen[xieyi_zongchangdu++] = ((shujubiaoshi>>8)&0xFF);  //高8位
	//消息序号，永远是0
	xieyi_zuzhen[xieyi_zongchangdu++] = 0x00;
	//数据域
	memcpy(&xieyi_zuzhen[xieyi_zongchangdu],shujuyu,shujuyu_changdu);
	xieyi_zongchangdu+=shujuyu_changdu;
	//校验码
	CRCjianyan = CRC16_XMODEM(xieyi_zuzhen,xieyi_zongchangdu);   //计算得到校验码
	xieyi_zuzhen[xieyi_zongchangdu++] = CRCjianyan&0xFF;    //取校验码低8位
	xieyi_zuzhen[xieyi_zongchangdu++] = (CRCjianyan>>8)&0xFF;   //取校验码高8位
	//帧结束符
	xieyi_zuzhen[xieyi_zongchangdu++] = 0x16;
	Uart1_Tx_Send_ok(xieyi_zuzhen,xieyi_zongchangdu); //近红外用这个，近红外需要前导码
	//Uart1_Tx_Send(xieyi_zuzhen,xieyi_zongchangdu);
}






//获取当前表号
void find_biaohao_xieyi(void)
{
	TONGXIN_xieyifasong(0x2031,0x02,moren_biaohao,kongshujuyu,0);
}
//上告协议
void send_shanggao_xieyi(void)
{
	TONGXIN_xieyifasong(0xF001,0x02,Test_jiejuo_jilu.dangqian_biaohao,kongshujuyu,0);
}
//阀门动作协议 
void FM_Test_xieyi(void)
{
	uint8_t shujuyu[1] = {0x1D};
	TONGXIN_xieyifasong(0xC022,0x04,Test_jiejuo_jilu.dangqian_biaohao,shujuyu,1);
}
//查询协议
void send_chaxun_xieyi(void)
{
	TONGXIN_xieyifasong(0xF003,0x02,Test_jiejuo_jilu.dangqian_biaohao,kongshujuyu,0);
}
//关阀协议
void guanfa_xieyi(void)
{
	uint8_t shujuyu[1] = {0x1A};
	TONGXIN_xieyifasong(0xC022,0x04,Test_jiejuo_jilu.dangqian_biaohao,shujuyu,1);
}
//开阀协议
void kaifa_xieyi(void)
{
	uint8_t shujuyu[1] = {0x1C};
	TONGXIN_xieyifasong(0xC022,0x04,Test_jiejuo_jilu.dangqian_biaohao,shujuyu,1);
}
//关闭红外通信
void guan_hongwai_xieyi(void)
{
	TONGXIN_xieyifasong(0xC002,0x04,Test_jiejuo_jilu.dangqian_biaohao,kongshujuyu,0);
}

//压力获取协议
void yali_enable_xieyi(void)
{
	TONGXIN_xieyifasong(0x1101,0x04,Test_jiejuo_jilu.dangqian_biaohao,kongshujuyu,0);
}

//有无阀门获取协议
void Get_Famen_Status(void)
{
	TONGXIN_xieyifasong(0x2604,0x02,Test_jiejuo_jilu.dangqian_biaohao,kongshujuyu,0);
}

//有无压力获取协议
void Get_yali_Status(void)
{
	TONGXIN_xieyifasong(0x1212,0x02,Test_jiejuo_jilu.dangqian_biaohao,kongshujuyu,0);
}
//获取当前版本号
void find_banbenhao_xieyi()
{
	TONGXIN_xieyifasong(0x2011,0x02,Test_jiejuo_jilu.dangqian_biaohao,kongshujuyu,0);
}
//查询有磁计量
void find_youci_xieyi()
{
	TONGXIN_xieyifasong(0x9010,0x02,Test_jiejuo_jilu.dangqian_biaohao,kongshujuyu,0);
}
