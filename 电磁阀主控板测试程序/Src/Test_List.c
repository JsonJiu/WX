#include "Test_List.h"
#include "GPIO.h"
#include "ZDINA219.h"
#include "ADC_CHK.h"
#include "uart0.h"
#include "uart1.h"
#include "tongxin_xieyi_Ctrl.h"


struct Test_quanju_canshu Test_quanju_canshu_L;
enum Test_liucheng Test_liucheng_L;
struct Test_jieguo Test_jiejuo_jilu;
enum test_xieyi_jilu test_xieyi_jilu_Rec = No_Receive;
uint32_t SY_dianya1;
uint32_t SY_dianya2;
uint32_t SY_dianya3;
uint32_t SY_dianyastate1;
uint32_t SY_dianyastate2;
void test_quanju_canshu_Init()
{
	Test_quanju_canshu_L.time_softdelay_ms = 10;
}
//测试参数初始化
void test_start_Init()
{
		//备电电源关闭
		beidian_gongdian_OFF();
		//主电源供电打开
		zhudian_gongdian_On();
		//主控板串口使能关闭
		Uart_shineng_OFF();
		//按键一抬起
    ANJIAN_1_OFF();
		//按键二抬起
    ANJIAN_2_OFF();
		//按键三抬起
    ANJIAN_3_OFF();
		//按键四抬起
    ANJIAN_4_OFF();
		//119不触发
		dianlu_119_OFF();
}
//工位检测
void gongwei_jiance()
{
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
	DeBug_print("当前工位为%d\r\n",Test_jiejuo_jilu.gongwei);
}
//测试结果初始化
void test_jieguo_qingling()
{
	Test_jiejuo_jilu.zhidian_gongdiandianya = 0;
	Test_jiejuo_jilu.zhidian_caijidianya = 0;
	Test_jiejuo_jilu.zhudian_gonghao = 0;
	Test_jiejuo_jilu.SY_dianya_1_1 = 0;
	Test_jiejuo_jilu.SY_dianya_1_2 = 0;
	Test_jiejuo_jilu.SY_dianya_1_3 = 0;
	Test_jiejuo_jilu.SY_dianya_2_1 = 0;
	Test_jiejuo_jilu.SY_dianya_2_2 = 0;
	Test_jiejuo_jilu.SY_dianya_2_3 = 0;
	Test_jiejuo_jilu.VCC_dianya = 0;
	Test_jiejuo_jilu.anjian_state = 0;
	Test_jiejuo_jilu.anjian_zhuangtai = 0;
	memset(Test_jiejuo_jilu.zhukongban_xingshan_MAC,0x00,12);
}
//开始测试
void test_start()
{
	//IO口恢复
	test_start_Init();
	//测试结果清零
	test_jieguo_qingling();
	Test_liucheng_L = w_start;
	//重置指令接收帧
	test_xieyi_jilu_Rec = No_Receive;
	//最大测试时间90秒
	Test_quanju_canshu_L.time_aroundtest_ms = 90000;
	Test_quanju_canshu_L.test_over = 0;
	Test_quanju_canshu_L.time_softdelay_ms = 0;
	DeBug_print("已开始测试\r\n");
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
	if(Test_quanju_canshu_L.time_aroundtest_ms==0&&Test_quanju_canshu_L.test_over==0)
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
		  Uart_shineng_OFF();
		  ANJIAN_1_OFF();
			break;
		case w_start:
			//开始测试前校验VDD是否有电，从而判断测试是否开始。
		  Test_jiejuo_jilu.VCC_dianya = get_VCC_weizhi_dianya();
		  DeBug_print("VCC电压%d\r\n",Test_jiejuo_jilu.VCC_dianya);
		  //测试合格条件判断
		  if(Test_jiejuo_jilu.VCC_dianya>2600&&Test_jiejuo_jilu.VCC_dianya<4000)
			{
				//测试合格，进入下一步
				Test_quanju_canshu_L.time_softdelay_ms = 10000;
				Test_liucheng_L = w_zhudian_CHK;
			}
			else
			{
				//否则1秒复测一次
				Test_quanju_canshu_L.time_softdelay_ms = 1000;
			}
			break;
		case w_zhudian_CHK:
			Test_jiejuo_jilu.zhidian_gongdiandianya = get_zhudian_gongdian_weizhi_dianya();
		  DeBug_print("供电电压%d\r\n",Test_jiejuo_jilu.zhidian_gongdiandianya);
			if(Test_jiejuo_jilu.zhidian_gongdiandianya>2600&&Test_jiejuo_jilu.zhidian_gongdiandianya<5500)
			{
				//测试合格，进入下一步
				Test_quanju_canshu_L.time_softdelay_ms = 0;
				Test_liucheng_L = w_gonghao_CHK;
			}
			else
			{
				//否则1秒复测一次
				Test_quanju_canshu_L.time_softdelay_ms = 1000;
			}
			break;
		case w_gonghao_CHK:
			//拉低串口的TX
			//UART0_TX_state_change(0);
		  //FL_GPIO_ResetOutputPin(GPIOA,FL_GPIO_PIN_14);
			Test_jiejuo_jilu.zhudian_gonghao = Current_CHK_Func();
		  DeBug_print("测试功耗%d\r\n",Test_jiejuo_jilu.zhudian_gonghao);
		  //UART0_TX_state_change(1);
		  if(Test_jiejuo_jilu.zhudian_gonghao>50)
			{
				Uart_shineng_OFF();
				Test_quanju_canshu_L.time_softdelay_ms = 1000;
				break;
			}
			//Test_quanju_canshu_L.time_softdelay_ms = 1000;
		  //Test_quanju_canshu_L.time_softdelay_ms = 1000;
		  //上拉一下通信用的SET引脚
		  Uart_shineng_ON();
		  Test_liucheng_L = w_SY_DY1_DY2_CHK;
			break;
		//检测第一次两边的电压
		case w_SY_DY1_DY2_CHK:
//			Test_jiejuo_jilu.SY_dianya_1_1 = get_erjidianyuan_weizhi_dianya();
//		  Test_jiejuo_jilu.SY_dianya_2_1 = get_SY_weizhi_dianya();
//		  Test_jiejuo_jilu.SY_dianya_1_3 = Test_jiejuo_jilu.SY_dianya_1_1;
//		  Test_jiejuo_jilu.SY_dianya_2_3 = Test_jiejuo_jilu.SY_dianya_2_1;
		  //获取继电器状态
		  SY_dianya1=getv4state();
			Test_liucheng_L = w_DY1_HIGH_CHANG;
		  Test_quanju_canshu_L.time_softdelay_ms = 3000;
		  //DeBug_print("第一次电压测试点1%d，点2%d\r\n",Test_jiejuo_jilu.SY_dianya_1_1,Test_jiejuo_jilu.SY_dianya_2_1);
//		  if(Test_jiejuo_jilu.SY_dianya_1_1<500&&Test_jiejuo_jilu.SY_dianya_2_1<500)
//			{
//				//测试正常
//				Test_liucheng_L = w_DY1_HIGH_CHANG;
//			}
//			else
//			{
//				Test_quanju_canshu_L.time_softdelay_ms = 1000;
//			}
			break;
		case w_DY1_HIGH_CHANG:
			if(test_xieyi_jilu_Rec != set_get_1008)
			{
				test_xieyi_jilu_Rec = No_Receive;
				TONGXIN_xieyifasong_1008(0,0,0,1);
				//DeBug_print("协议发送\r\n");
				//等待3秒自动重发
				//Test_quanju_canshu_L.time_softdelay_ms = 3000;
				FL_DelayMs(3000);
			}
			else
			{
				//按键一抬起
        ANJIAN_1_OFF();
				SY_dianya2=getv4state();
				if(SY_dianya2!=SY_dianya1)
				{
					SY_dianyastate1=1;
				}
				test_xieyi_jilu_Rec = No_Receive;
				Test_liucheng_L = w_DY2_HIGH_CHANG;
			}
			break;
//		case w_SY2_DY1_DY2_CHK:
//			Test_jiejuo_jilu.SY_dianya_1_2 = get_erjidianyuan_weizhi_dianya();
//		  Test_jiejuo_jilu.SY_dianya_2_2 = get_SY_weizhi_dianya();
//		  Test_jiejuo_jilu.SY_dianya_1_3 = Test_jiejuo_jilu.SY_dianya_1_2;
//		  Test_jiejuo_jilu.SY_dianya_2_3 = Test_jiejuo_jilu.SY_dianya_2_2;
//		  DeBug_print("第二次电压测试点1%d，点2%d\r\n",Test_jiejuo_jilu.SY_dianya_1_2,Test_jiejuo_jilu.SY_dianya_2_2);
//		  if(Test_jiejuo_jilu.SY_dianya_1_2>9000&&Test_jiejuo_jilu.SY_dianya_1_2<15000&&Test_jiejuo_jilu.SY_dianya_2_2<500)
//			{
//				//测试正常
//				Test_liucheng_L = w_DY2_HIGH_CHANG;
//			}
//			else
//			{
//				Test_quanju_canshu_L.time_softdelay_ms = 1000;
//			}
//			break;
		case w_DY2_HIGH_CHANG:
			if(test_xieyi_jilu_Rec != set_get_1008)
			{
				test_xieyi_jilu_Rec = No_Receive;
				TONGXIN_xieyifasong_1008(1,0,0,0);
				DeBug_print("协议发送\r\n");
				//等待3秒自动重发
				//Test_quanju_canshu_L.time_softdelay_ms = 3000;
				FL_DelayMs(3000);
			}
			else
			{
				test_xieyi_jilu_Rec = No_Receive;
				SY_dianya3=getv4state();
				if(SY_dianya3!=SY_dianya2)
				{
					SY_dianyastate2=1;
				}
				if(SY_dianyastate2==1&&SY_dianyastate2==1)
				{
				Test_jiejuo_jilu.SY_dianya_1_3=12000;
				Test_jiejuo_jilu.SY_dianya_2_3=12000;
				}
				ANJIAN_1_ON();
				Test_liucheng_L = w_DY3_ANJIAN_CHK;
			}
			break;
//		case w_SY3_DY1_DY2_CHK:
//			Test_jiejuo_jilu.SY_dianya_1_3 = get_erjidianyuan_weizhi_dianya();
//		  Test_jiejuo_jilu.SY_dianya_2_3 = get_SY_weizhi_dianya();
//		  DeBug_print("第三次电压测试点1%d，点2%d\r\n",Test_jiejuo_jilu.SY_dianya_1_3,Test_jiejuo_jilu.SY_dianya_2_3);
//		  if(Test_jiejuo_jilu.SY_dianya_1_3>9000&&Test_jiejuo_jilu.SY_dianya_1_3<15000&&Test_jiejuo_jilu.SY_dianya_2_3>9000&&Test_jiejuo_jilu.SY_dianya_2_3<15000)
//			{
//				//按键一按下
//				ANJIAN_1_ON();
//				//测试正常
//				Test_liucheng_L = w_DY3_ANJIAN_CHK;
//			}
//			else
//			{
//				Test_quanju_canshu_L.time_softdelay_ms = 1000;
//			}
//			break;
		case w_DY3_ANJIAN_CHK://该步骤用于检测按键
			if(test_xieyi_jilu_Rec != set_get_1008)
			{
				test_xieyi_jilu_Rec = No_Receive;
				TONGXIN_xieyifasong_1008(0,0,0,0);
				DeBug_print("协议发送\r\n");
				//等待3秒自动重发
				Test_quanju_canshu_L.time_softdelay_ms = 3000;
			}
			else
			{
				test_xieyi_jilu_Rec = No_Receive;
				Test_liucheng_L = w_end;
			}
		case w_end:
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
