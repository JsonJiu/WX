#include "GPIO.h"
#include "PC_xieyi_Ctrl.h"
#include "Test_List.h"
#include "uart0.h"
#include "uart1.h"
#define send_lenth 200
uint8_t xieyi1_fanhui[6] = {0x68,0xAB,0x06,0x00,0x19,0x16};
uint8_t xieyi2_fanhui[send_lenth];
//开始测试设置成功后发送
void PC_xieyifasong_1()
{
	xieyi1_fanhui[3] = Test_jiejuo_jilu.gongwei;
	xieyi1_fanhui[4] = 0x19+xieyi1_fanhui[3];
	//Uart0_Tx_Send(xieyi1_fanhui,5);
	PC_Chuankou_tongxin_send(xieyi1_fanhui,6);
}
//查询结果
void PC_xieyifasong_2()
{
	uint16_t jishu_lenth = 0;
	uint16_t hejiaoyan = 0;
	memset(xieyi2_fanhui,0x00,send_lenth);
	xieyi2_fanhui[jishu_lenth++] = 0x68; //固定帧头  [0]
	xieyi2_fanhui[jishu_lenth++] = 0xAD; //返回测试结果指示位  [1]
	xieyi2_fanhui[jishu_lenth++] = 0x00; //先赋0，后面有具体赋值  [2]
	xieyi2_fanhui[jishu_lenth++] = Test_jiejuo_jilu.gongwei;  //工位  [3]
	xieyi2_fanhui[jishu_lenth++] = (Test_jiejuo_jilu.zhidian_dianya_gongdian)&0xFF;   //主电电压低8位  [4]
	xieyi2_fanhui[jishu_lenth++] = ((Test_jiejuo_jilu.zhidian_dianya_gongdian)>>8)&0xFF;   //主电电压高8位   [5]
	xieyi2_fanhui[jishu_lenth++] = (Test_jiejuo_jilu.zhidian_dianya_huoqu)&0xFF;      //主电电压协议获取低8位  [6]
	xieyi2_fanhui[jishu_lenth++] = ((Test_jiejuo_jilu.zhidian_dianya_huoqu)>>8)&0xFF;  //主电电压协议获取高8位  [7]
  xieyi2_fanhui[jishu_lenth++] = (Test_jiejuo_jilu.zhidian_jingtai_gonghao)&0xFF;    //静态走水功耗低8位   [8]
	xieyi2_fanhui[jishu_lenth++] = ((Test_jiejuo_jilu.zhidian_jingtai_gonghao)>>8)&0xFF;  //静态走水功耗高8位  [9]
	
	Test_jiejuo_jilu.zhidian_manshui_gonghao = 10;
	xieyi2_fanhui[jishu_lenth++] = (Test_jiejuo_jilu.zhidian_manshui_gonghao)&0xFF;   //满水功耗低8位  [10]
	xieyi2_fanhui[jishu_lenth++] = ((Test_jiejuo_jilu.zhidian_manshui_gonghao)>>8)&0xFF;  //满水功耗高8位  [11]
	Test_jiejuo_jilu.zhidian_zoushui_gonghao = 10;
	xieyi2_fanhui[jishu_lenth++] = (Test_jiejuo_jilu.zhidian_zoushui_gonghao)&0xFF;   //走水功耗低8位  [12]
	xieyi2_fanhui[jishu_lenth++] = ((Test_jiejuo_jilu.zhidian_zoushui_gonghao)>>8)&0xFF;   //走水功耗高8位  [13]
	Test_jiejuo_jilu.beidian_dianya_gongdian = 3600;
	xieyi2_fanhui[jishu_lenth++] = (Test_jiejuo_jilu.beidian_dianya_gongdian)&0xFF;    //备点电压低8位  [14]
	xieyi2_fanhui[jishu_lenth++] = ((Test_jiejuo_jilu.beidian_dianya_gongdian)>>8)&0xFF;   //备点电压高8位   [15]
	
	xieyi2_fanhui[jishu_lenth++] = (Test_jiejuo_jilu.beidian_dianya_huoqu)&0xFF;     //备点电压协议获取低8位 [16]
	xieyi2_fanhui[jishu_lenth++] = ((Test_jiejuo_jilu.beidian_dianya_huoqu)>>8)&0xFF;   //备点电压协议获取高8位  [17]
	Test_jiejuo_jilu.beidian_gonghao = 10;
	xieyi2_fanhui[jishu_lenth++] = (Test_jiejuo_jilu.beidian_gonghao)&0xFF;       //备点功耗低8位  [18]
	xieyi2_fanhui[jishu_lenth++] = ((Test_jiejuo_jilu.beidian_gonghao)>>8)&0xFF;   //备点功耗高8位   [19]
	
	xieyi2_fanhui[jishu_lenth++] = Test_jiejuo_jilu.lanya_jiance;     //蓝牙检测  [20]  /  压力检测（因为后续没有蓝牙了）   
	xieyi2_fanhui[jishu_lenth++] = Test_jiejuo_jilu.flash_jiance;     //flash检测   [21]
	xieyi2_fanhui[jishu_lenth++] = Test_jiejuo_jilu.jiliang_jiance;   //计量异常检测  [22]
	xieyi2_fanhui[jishu_lenth++] = Test_jiejuo_jilu.hongwai_jiance;   //红外通信异常检测  [23]
	memcpy(&xieyi2_fanhui[jishu_lenth],Test_jiejuo_jilu.IMEI_CHK,15);  //IMEI号获取  [24]-[38]
	jishu_lenth+=15;
	memcpy(&xieyi2_fanhui[jishu_lenth],Test_jiejuo_jilu.IMSI_CHK,15);  //IMSI号获取   [39]-[53]
	jishu_lenth+=15;
	memcpy(&xieyi2_fanhui[jishu_lenth],Test_jiejuo_jilu.ICCID_CHK,20);  //ICCID号获取  [54]-[73]
	jishu_lenth+=20;
	xieyi2_fanhui[jishu_lenth++] = Test_jiejuo_jilu.CSQ;   //信号强度  [74]
	
	Test_jiejuo_jilu.FM=1;
	Test_jiejuo_jilu.FM_daowei=1;
	xieyi2_fanhui[jishu_lenth++] = Test_jiejuo_jilu.FM;    //阀门异常检测  [75]
	xieyi2_fanhui[jishu_lenth++] = Test_jiejuo_jilu.FM_daowei;   //阀门到位异常检测   [76]
	
	xieyi2_fanhui[jishu_lenth++] = Test_jiejuo_jilu.EEPROM_jiance;   //EEPROM检测   [77]
	
	xieyi2_fanhui[jishu_lenth++] = (Test_jiejuo_jilu.GP30_dianya)&0xFF;   //GP30电压低8位  [78]
	xieyi2_fanhui[jishu_lenth++] = ((Test_jiejuo_jilu.GP30_dianya)>>8)&0xFF;  //GP30电压高8位  [79]

	memcpy(&xieyi2_fanhui[jishu_lenth],Test_jiejuo_jilu.loraEUI,16);     //loraEUI获取  [80]-[95]
	jishu_lenth+=16;
	Test_jiejuo_jilu.qiangci_jiance = 1;
	xieyi2_fanhui[jishu_lenth++] = Test_jiejuo_jilu.qiangci_jiance;  //强磁检测  [96]
	Test_jiejuo_jilu.kaigai_jiance = 1;
	xieyi2_fanhui[jishu_lenth++] = Test_jiejuo_jilu.kaigai_jiance;   //开盖检测  [97]
	
	xieyi2_fanhui[jishu_lenth++] = Test_jiejuo_jilu.GPSmozu_jiacne;  //GPS模组检测  [98]

	xieyi2_fanhui[jishu_lenth++] = 0;	//无磁检测
	memcpy(&xieyi2_fanhui[jishu_lenth],Test_jiejuo_jilu.jiaoyanma,2);//海外用校验码
	jishu_lenth+=2;
	memcpy(&xieyi2_fanhui[jishu_lenth],Test_jiejuo_jilu.banbenhao,2);//程序版本号
	jishu_lenth+=2;
	//对帧长度赋值
	xieyi2_fanhui[2] = jishu_lenth+2;   //加上校验值和帧尾
	xieyi2_fanhui[jishu_lenth] = 0;
	for(hejiaoyan=0;hejiaoyan<jishu_lenth;hejiaoyan++)   
	{
		xieyi2_fanhui[jishu_lenth]+=xieyi2_fanhui[hejiaoyan];   //获取校验位的值  [99]
	}
	jishu_lenth++;
	xieyi2_fanhui[jishu_lenth++] = 0x16;    //固定帧尾  [100]
	PC_Chuankou_tongxin_send(xieyi2_fanhui,jishu_lenth);   //往PC返回数据
}

void PC_xieyijiexi(uint8_t zufuchua[],uint16_t lenth)
{
	uint16_t pHead = 0;
	uint8_t hejiaoyan = 0;
	uint16_t zhenchangdu = 0;
	uint16_t linshichangdu = 0;
	
	while(1)
	{
		if(pHead+5>lenth)
		{
			break;
		}
		if(zufuchua[pHead]==0x68)
		{
			if(zufuchua[pHead+1]==0xAA)
			{
				//获取协议长度
				zhenchangdu = zufuchua[pHead+2];
				//长度应低于100个字节
				if(zhenchangdu<100)
				{
					//校验工位与尾帧是否正确
					if(zufuchua[pHead+3]==Test_jiejuo_jilu.gongwei&&zufuchua[pHead+zhenchangdu]==0x16)
					{
						//进行和校验
						hejiaoyan = 0;
						for(linshichangdu=0;linshichangdu<zhenchangdu-1;linshichangdu++)
						{
							hejiaoyan+=zufuchua[pHead+linshichangdu];
						}
						if(hejiaoyan==zufuchua[pHead+pHead+zhenchangdu-1])
						{
							//协议校验完成，进入主程序处理
							memcpy(Test_jiejuo_jilu.mokuaihao,&zufuchua[pHead+4],6);
							test_start();        //开始测试
							PC_xieyifasong_1();  //工装返回开始测试指令
							pHead+=zhenchangdu;
						}
					}
				}
			}
			else if(zufuchua[pHead+1]==0xAC&&zufuchua[pHead+3]==Test_jiejuo_jilu.gongwei&&zufuchua[pHead+5]==0x16)
			{
				if(zufuchua[pHead+1]==0xAC)
				{
					//获取协议长度
					zhenchangdu = zufuchua[pHead+2];
					//长度应低于100个字节
					if(zhenchangdu<100)
					{
						//校验工位与尾帧是否正确
						if(zufuchua[pHead+3]==Test_jiejuo_jilu.gongwei&&zufuchua[pHead+zhenchangdu-1]==0x16)
						{
							//进行和校验
							hejiaoyan = 0;
							for(linshichangdu=0;linshichangdu<zhenchangdu-2;linshichangdu++)
							{
								hejiaoyan+=zufuchua[pHead+linshichangdu];
							}
							if(hejiaoyan==zufuchua[pHead+pHead+zhenchangdu-2])
							{
								//协议校验完成，进入主程序处理
								//如果测试完成，则返回结果，未完成则不处理
								if(Test_quanju_canshu_L.test_over==1)
								{
								  PC_xieyifasong_2();   //发送查询结果
								}
								pHead+=zhenchangdu;
							}
						}
					}
				}
			}
		}
		pHead++;
	}
}


