#include "Test_List.h"
#include "LED_CTRL.h"
#include "GPIO.h"
#include "ZDINA219.h"
#include "uart0.h"
#include "uart1.h"
#include "ADC_CHK.h"
#include "tongxin_xieyi_Ctrl.h"

struct Test_quanju_canshu Test_quanju_canshu_L;
enum Test_liucheng Test_liucheng_L = w_wait;
struct Test_jieguo Test_jiejuo_jilu;
struct Test_linshi_cunchushuju Test_linshi_cunchushuju_L;
enum test_xieyi_jilu test_xieyi_jilu_Rec = No_Receive;
uint8_t shangg_chongfa = 0;

uint8_t dayinbiaohaoyong[12];

void test_quanju_canshu_Init()
{
	Test_quanju_canshu_L.time_softdelay_ms = 10;
}
//测试参数初始化
void test_start_Init()
{
	//工装放上就会上电
	zhudian_gongdian_On();
	//备电电源断开
	beidian_gongdian_OFF();
	//主电电压检测控制关闭
	zhudian_dianya_CHK_CTRL_OFF();
	//电机停止动作
  huuoer_1_OFF();
	//二级电源电压检测控制关闭
	erji_dianya_CHK_CTRL_OFF();
	//霍尔1控制 关
	huuoer_1_OFF();
	//霍尔2控制 关
	huuoer_2_OFF();
	//LED会亮一下代表初始化完成
	LED_FLAG_Run();
}
//工位检测
void gongwei_jiance()
{
	/*
	if(FL_GPIO_GetInputPin(GPIOE,FL_GPIO_PIN_0)==0)
	{
		Test_jiejuo_jilu.gongwei = 0x03;
	}
	else if(FL_GPIO_GetInputPin(GPIOE,FL_GPIO_PIN_1)==0)
	{
		Test_jiejuo_jilu.gongwei = 0x02;
	}
	else if(FL_GPIO_GetInputPin(GPIOE,FL_GPIO_PIN_2)==0)
	{
		Test_jiejuo_jilu.gongwei = 0x01;
	}
	else if(FL_GPIO_GetInputPin(GPIOE,FL_GPIO_PIN_3)==0)
	{
		Test_jiejuo_jilu.gongwei = 0x00;
	}
	else
	{
		Test_jiejuo_jilu.gongwei = 0x00;
	}
	*/
	Test_jiejuo_jilu.gongwei = 0x00;
	DeBug_print("当前工位为%d\r\n",Test_jiejuo_jilu.gongwei);
}
//测试结果初始化
void test_jieguo_qingling()
{
	Test_jiejuo_jilu.zhidian_dianya_gongdian = 0;
	Test_jiejuo_jilu.zhidian_dianya_huoqu = 0;
	Test_jiejuo_jilu.zhidian_jingtai_gonghao = 0;
	Test_jiejuo_jilu.zhidian_manshui_gonghao = 0;
	Test_jiejuo_jilu.zhidian_zoushui_gonghao = 0;
	Test_jiejuo_jilu.beidian_dianya_gongdian = 0;
	Test_jiejuo_jilu.beidian_dianya_huoqu = 0;
	Test_jiejuo_jilu.beidian_gonghao = 0;
	Test_jiejuo_jilu.lanya_jiance = 0;
	Test_jiejuo_jilu.flash_jiance = 0;
	Test_jiejuo_jilu.jiliang_jiance = 0;
	Test_jiejuo_jilu.hongwai_jiance = 0;
	memset(Test_jiejuo_jilu.IMEI_CHK,0x00,15);
	memset(Test_jiejuo_jilu.IMSI_CHK,0x00,15);
	memset(Test_jiejuo_jilu.ICCID_CHK,0x00,20);
	Test_jiejuo_jilu.FM1 = 0;
	Test_jiejuo_jilu.FM2 = 0;
	Test_jiejuo_jilu.FM = 0;
	Test_jiejuo_jilu.FM_daowei_1 = 0;
	Test_jiejuo_jilu.FM_daowei_2 = 0;
	Test_jiejuo_jilu.FM_daowei = 0;
	Test_jiejuo_jilu.EEPROM_jiance = 0;
	Test_jiejuo_jilu.GP30_dianya = 0;
	memset(Test_jiejuo_jilu.loraEUI,0x00,16);
	Test_jiejuo_jilu.qiangci_jiance = 0;
	Test_jiejuo_jilu.qiangci_jiance_1 = 0;
	Test_jiejuo_jilu.qiangci_jiance_2 = 0;
	Test_jiejuo_jilu.kaigai_jiance = 0;
	Test_jiejuo_jilu.GPSmozu_jiacne = 0;
	Test_quanju_canshu_L.test_over = 0;
	memset(Test_jiejuo_jilu.dangqian_biaohao,0x00,6);
	Test_jiejuo_jilu.jiliang_1 = 0;
	Test_jiejuo_jilu.jiliang_2 = 0;
	Test_jiejuo_jilu.wucixinhaoqiangdu = 0;
	memset(Test_jiejuo_jilu.jiaoyanma,0x00,2);
	memset(Test_jiejuo_jilu.banbenhao,0x00,2);
	//接收数据标志位清除
	test_xieyi_jilu_Rec = No_Receive;
}
//开始测试
void test_start()
{
	//IO口恢复
	test_start_Init();
	//测试结果清零
	test_jieguo_qingling();
	Test_liucheng_L = w_start;
	//最大测试时间90秒
	Test_quanju_canshu_L.time_aroundtest_ms = 100000;
	//单步超时时间
	Test_quanju_canshu_L.danbu_chaoshishijian_ms = 3000;
	Test_quanju_canshu_L.test_over = 0;
	Test_quanju_canshu_L.time_softdelay_ms = 0;
	DeBug_print("已开始测试，表号为\r\n",Test_jiejuo_jilu.mokuaihao);
	PC_Chuankou_tongxin_Debug_send(Test_jiejuo_jilu.mokuaihao,6);
}
//终止测试
//终止测试的条件是，测试时间过长，或是主控板离开了工装。
void test_testend()
{
	Test_liucheng_L = w_end;
	Test_quanju_canshu_L.test_over = 1;
	Test_quanju_canshu_L.time_softdelay_ms = 0;
}
//测试过程中的额外异常事件
void test_err_end_Func()
{
	//测试超时
	if((Test_quanju_canshu_L.time_aroundtest_ms==0||Test_quanju_canshu_L.danbu_chaoshishijian_ms==0)&&Test_quanju_canshu_L.test_over==0)
	{
		//结束测试
		test_testend();
	}
}

void test_Loop_Func()
{
	test_err_end_Func();
	//Test_liucheng_L = w_gonghao_CHK;
	if(Test_quanju_canshu_L.time_softdelay_ms>0)
		return;
	switch(Test_liucheng_L)
	{
		case w_wait:
			//仅等待下一次测试
			break;
		case w_start:
			//开始测试前校验VDD是否有电，从而判断测试是否开始。
		  Test_jiejuo_jilu.zhidian_dianya_gongdian = get_zhukongban_gongdian_dianya();
		  DeBug_print("VCC电压%d\r\n",Test_jiejuo_jilu.zhidian_dianya_gongdian);
		  //测试合格条件判断
		  if(Test_jiejuo_jilu.zhidian_dianya_gongdian>3200&&Test_jiejuo_jilu.zhidian_dianya_gongdian<4000)
			{
				//测试合格，进入下一步
				Test_quanju_canshu_L.time_softdelay_ms = 0;
				//重置单步超时时间
	      Test_quanju_canshu_L.danbu_chaoshishijian_ms = 10000;
				Test_liucheng_L = w_gonghao_CHK;
			}
			else
			{
				//否则1秒复测一次
				Test_quanju_canshu_L.time_softdelay_ms = 1000;
			}
			break;
		case w_gonghao_CHK:
			Test_jiejuo_jilu.zhidian_jingtai_gonghao = Current_CHK_Func(0);
		  DeBug_print("供电电流%d\r\n",Test_jiejuo_jilu.zhidian_jingtai_gonghao);
			if(Test_jiejuo_jilu.zhidian_jingtai_gonghao>2&&Test_jiejuo_jilu.zhidian_jingtai_gonghao<30)
			{
				//测试合格，进入下一步
				Test_quanju_canshu_L.time_softdelay_ms = 0;
				//重置单步超时时间 10秒
	      Test_quanju_canshu_L.danbu_chaoshishijian_ms = 10000;
				Test_liucheng_L = w_diancitie_huanxing_irda;
			}
			else
			{
				//否则1秒复测一次
				Test_quanju_canshu_L.time_softdelay_ms = 1000;
			}
			break;
		case w_diancitie_huanxing_irda:
			Test_quanju_canshu_L.time_softdelay_ms = 0;
		  //空流程，因为这里用不到电磁铁了
	    Test_quanju_canshu_L.danbu_chaoshishijian_ms = 10000;
		  Test_liucheng_L = get_biaohao;
			break;
		case get_biaohao:
		  if(test_xieyi_jilu_Rec == w_get_biaohao)
			{
				test_xieyi_jilu_Rec = No_Receive;
				//测试合格，进入下一步
				Test_quanju_canshu_L.time_softdelay_ms = 1000;
				Test_jiejuo_jilu.hongwai_jiance = 1;
				//上告重发不判断，直接重发3次
				shangg_chongfa = 0;
				//重置单步超时时间 10秒
	      Test_quanju_canshu_L.danbu_chaoshishijian_ms = 15000;
				Test_liucheng_L = get_banbenhao;
			}
			else
			{
				DeBug_print("发送读表号协议\r\n");
				//发送协议
				find_biaohao_xieyi();
				//每2秒重发一次（这是远红外，所以可以）
				Test_quanju_canshu_L.time_softdelay_ms = 3500;
			}
			break;
		case get_banbenhao:
		  if(test_xieyi_jilu_Rec == w_get_banbenhao)
			{
				test_xieyi_jilu_Rec = No_Receive;
				memcpy(Test_jiejuo_jilu.banbenhao,Test_linshi_cunchushuju_L.L_banbenhao,2);
				
				//测试合格，进入下一步
				Test_quanju_canshu_L.time_softdelay_ms = 1000;
				//上告重发不判断，直接重发3次
				shangg_chongfa = 0;
				//重置单步超时时间 10秒
	      Test_quanju_canshu_L.danbu_chaoshishijian_ms = 15000;
				Test_liucheng_L = w_zhudong_shangg;
			}
			else
			{
				DeBug_print("发送读版本号协议\r\n");
				//发送协议
				find_banbenhao_xieyi();
				//每2秒重发一次（这是远红外，所以可以）
				Test_quanju_canshu_L.time_softdelay_ms = 2000;
			}
			break;
		case w_zhudong_shangg:
			if(shangg_chongfa>2)
			{
				test_xieyi_jilu_Rec = No_Receive;
				//测试合格，进入下一步
				Test_quanju_canshu_L.time_softdelay_ms = 2000;
				//重置单步超时时间 10秒
	      Test_quanju_canshu_L.danbu_chaoshishijian_ms = 10000;
				Test_liucheng_L = w_shangg_jieguo_start;
			}
			else
			{
				shangg_chongfa++;
				DeBug_print("启动上告\r\n");
				//发送协议
				send_shanggao_xieyi();
				//每2秒重发一次（这是远红外，所以可以）
				Test_quanju_canshu_L.time_softdelay_ms = 3500;
			}
			break;
		case w_shangg_jieguo_start:
			if(test_xieyi_jilu_Rec == w_get_test_zhuanyong)
			{
				test_xieyi_jilu_Rec = No_Receive;
				//这一步需要获取计量初始值
				Test_jiejuo_jilu.jiliang_1 = Test_linshi_cunchushuju_L.L_wucijiliang;
				//测试合格，进入下一步，这里的延时是考虑到下一步通信一致导致的通信异常
				//电机开始工作
				huuoer_1_ON();
				Test_quanju_canshu_L.time_softdelay_ms = 2000;
				//重置单步超时时间 20秒
	      Test_quanju_canshu_L.danbu_chaoshishijian_ms = 60000;
				Test_liucheng_L = w_shangg_jieguo;
			}
			else
			{
				DeBug_print("第一次查询上告信息\r\n");
				//发送协议
				Test_zhuanyong_xieyi();
				//每2秒重发一次（这是远红外，所以可以）
				Test_quanju_canshu_L.time_softdelay_ms = 3500;
			}
			break;
		case w_shangg_jieguo:
			if(test_xieyi_jilu_Rec == w_get_test_zhuanyong)
			{
				test_xieyi_jilu_Rec = No_Receive;
				if(Test_linshi_cunchushuju_L.L_CSQ<33&&Test_linshi_cunchushuju_L.L_CSQ>10)
				{
					//对测试结果进行映射
					//计量1
					Test_jiejuo_jilu.jiliang_2 = Test_linshi_cunchushuju_L.L_wucijiliang;
					//flash
					Test_jiejuo_jilu.flash_jiance = Test_linshi_cunchushuju_L.L_flash_chk;
					//获取的主电电压
					Test_jiejuo_jilu.zhidian_dianya_huoqu = Test_linshi_cunchushuju_L.L_zhudian_dianya;
					//获取的备电电压
					Test_jiejuo_jilu.beidian_dianya_huoqu = Test_linshi_cunchushuju_L.L_beidian_dianya;
					//压力状态（协议里没加，改为温度）
					Test_jiejuo_jilu.lanya_jiance = Test_linshi_cunchushuju_L.L_yali_CHK;
					//EEPROM
					Test_jiejuo_jilu.EEPROM_jiance = Test_linshi_cunchushuju_L.L_EEPROM_CHK;
					//GPRS模组（这里有争议，可能有问题）
					Test_jiejuo_jilu.GPSmozu_jiacne = Test_linshi_cunchushuju_L.L_GPRS_mozu_CHK;
					//GP30电压检测
					Test_jiejuo_jilu.GP30_dianya = Test_linshi_cunchushuju_L.L_GP30mozu_AD_CHK;
					//IMEI
					memcpy(Test_jiejuo_jilu.IMEI_CHK,Test_linshi_cunchushuju_L.L_IMEI,15);
					//IMSI
					memcpy(Test_jiejuo_jilu.IMSI_CHK,Test_linshi_cunchushuju_L.L_IMSI,15);
					//ICCID
					memcpy(Test_jiejuo_jilu.ICCID_CHK,Test_linshi_cunchushuju_L.L_ICCID,20);
					//CSQ
					Test_jiejuo_jilu.CSQ = Test_linshi_cunchushuju_L.L_CSQ;
					//若计量1与计量2值不相同，则认为计量正常
					if(Test_jiejuo_jilu.jiliang_1!=Test_jiejuo_jilu.jiliang_2)
					{
						Test_jiejuo_jilu.jiliang_jiance = 1;
					}
					else
					{
						break;
					}
					
					//上告结束，电机停止工作
					huuoer_1_OFF();
					//测试合格，进入下一步
					Test_quanju_canshu_L.time_softdelay_ms = 0;
					//重置单步超时时间 10秒
					Test_quanju_canshu_L.danbu_chaoshishijian_ms = 10000;
					Test_liucheng_L = w_get_wuci_xinhao1;
				}
				else
				{
					
				}
			}
			else
			{
				DeBug_print("查询上告信息\r\n");
				//发送协议
				Test_zhuanyong_xieyi();
				//每2秒重发一次（这是远红外，所以可以）
				Test_quanju_canshu_L.time_softdelay_ms = 3500;
			}
			break;
		case w_get_wuci_xinhao1:
			if(test_xieyi_jilu_Rec == w_set_wuci_xinhao_huoqu)
			{
				test_xieyi_jilu_Rec = No_Receive;
				//测试合格，进入下一步
				Test_quanju_canshu_L.time_softdelay_ms = 2000;
				//重置单步超时时间 10秒
	      Test_quanju_canshu_L.danbu_chaoshishijian_ms = 10000;
				Test_liucheng_L = w_get_wuci_xinhao2;
			}
			else
			{
				//电机开始工作
				huuoer_1_ON();
				shangg_chongfa++;
				DeBug_print("获取无磁模组信号强度至主控板\r\n");
				//发送协议
				get_wucicaiyang_xieyi();
				//每2秒重发一次（这是远红外，所以可以）
				Test_quanju_canshu_L.time_softdelay_ms = 3500;
			}
			break;
			case w_get_wuci_xinhao2:
			if(test_xieyi_jilu_Rec == w_get_zhukongban_wuci_xinhao)
			{
				test_xieyi_jilu_Rec = No_Receive;
				Test_jiejuo_jilu.wucixinhaoqiangdu = Test_linshi_cunchushuju_L.L_wucijiliang_xinhaoqiangdu;
				//测试合格，进入下一步
				Test_quanju_canshu_L.time_softdelay_ms = 2000;
				//重置单步超时时间 10秒
	      Test_quanju_canshu_L.danbu_chaoshishijian_ms = 10000;
				
				if(Test_jiejuo_jilu.wucixinhaoqiangdu>14)
				{
					huuoer_1_OFF();
					Test_liucheng_L = w_end;
				}
				else
				{
					Test_liucheng_L = w_get_wuci_xinhao1;
				}
				
			}
			else
			{
				shangg_chongfa++;
				DeBug_print("获取无磁模组信号强度\r\n");
				//发送协议
				get_wucicaiyang_zhukongban_xieyi();
				//每2秒重发一次（这是远红外，所以可以）
				Test_quanju_canshu_L.time_softdelay_ms = 3500;
			}
			break;
	  //这板子没有阀门
		case w_famenceshi:
			if(test_xieyi_jilu_Rec != w_get_famen_dongzuo)
			{
				test_xieyi_jilu_Rec = No_Receive;
				//重置单步超时时间 10秒
					Test_quanju_canshu_L.danbu_chaoshishijian_ms = 10000;
				//等待3秒自动重发
				Test_quanju_canshu_L.time_softdelay_ms = 3000;
			}
			else
			{
				DeBug_print("开始阀门检测\r\n");
				FM_Open_xieyi();
				Test_liucheng_L = w_end;
			}
			break;
		case w_end:
			DeBug_print("测试完成\r\n");
			//主电源供电打开
			zhudian_gongdian_On();
			//备电电源关闭
			beidian_gongdian_OFF();
		  
		  //电机停止动作
		  huuoer_1_OFF();
			//一切测试都已结束，打开测试返回
		  Test_quanju_canshu_L.test_over = 1;
		  //回到第一步
		  Test_liucheng_L = w_wait;
			break;
		default:
			Test_liucheng_L = w_end;
			break;
	}
}
//该事件为测试委托事件，在主函数中运行
void Test_loop_func()
{
	if(Test_liucheng_L == w_famenceshi)
	{
		if(get_famen_1_dianya()>1800)
		{
			Test_jiejuo_jilu.FM1 |= 0x01; 
		}
		else
		{
			Test_jiejuo_jilu.FM1 |= 0x02;
		}
		if(get_famen_2_dianya()>1800)
		{
			Test_jiejuo_jilu.FM2 |= 0x01; 
		}
		else
		{
			Test_jiejuo_jilu.FM2 |= 0x02;
		}
		if(Test_jiejuo_jilu.FM1==0x03&&Test_jiejuo_jilu.FM2==0x03)
		{
			Test_quanju_canshu_L.time_softdelay_ms = 0;
			//校验完成后不管协议有没有解析都会结束
			test_xieyi_jilu_Rec = w_get_famen_dongzuo;
		}
	}
}
