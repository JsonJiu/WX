#include "Test_List.h"
#include "GPIO.h"
#include "ZDINA219.h"
#include "ADC_CHK.h"
#include "uart1.h"
#include "tongxin_xieyi_Ctrl.h"

struct Test_quanju_canshu Test_quanju_canshu_L;
enum Test_liucheng Test_liucheng_L = w_wait;
struct Test_jieguo Test_jiejuo_jilu;
enum test_xieyi_jilu test_xieyi_jilu_Rec = No_Receive;

void test_quanju_canshu_Init()
{
	Test_quanju_canshu_L.time_softdelay_ms = 10;
}
//测试参数初始化
void test_start_Init()
{
}

//测试结果初始化
void test_jieguo_qingling()
{
	Test_jiejuo_jilu.zhudian_gonghao = 0;
}
//开始测试
void test_start()
{
	//IO口恢复
	test_start_Init();
	//测试结果清零
	test_jieguo_qingling();
	Test_liucheng_L = w_gonghao_CHK;
	//最大测试时间90秒
	Test_quanju_canshu_L.time_aroundtest_ms = 10000;
	Test_quanju_canshu_L.time_softdelay_ms = 0;
}
//终止测试
//终止测试的条件是，测试时间过长，或是主控板离开了工装。
void test_testend()
{
	Test_liucheng_L = w_end;
	Test_quanju_canshu_L.time_softdelay_ms = 0;
}
//测试过程中的额外异常事件
void test_err_end_Func()
{
	//测试超时
	if(Test_quanju_canshu_L.time_aroundtest_ms==0)
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
		case w_gonghao_CHK:
			Test_jiejuo_jilu.zhudian_gonghao = Current_CHK_Func(5);
		  DeBug_print("测试电流%d\r\n",Test_jiejuo_jilu.zhudian_gonghao);
			Test_liucheng_L = w_end;
			break;
		case w_end:
			//一切测试都已结束，返回结果
		  //回到第一步
		  Test_liucheng_L = w_wait;
			break;
		default:
			Test_liucheng_L = w_end;
			break;
	}
}
