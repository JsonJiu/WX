#include "Test_List.h"
#include "LED_CTRL.h"
#include "GPIO.h"
#include "ZDINA219.h"
#include "uart0.h"
#include "uart1.h"
#include "ADC_CHK.h"
#include "tongxin_xieyi_Ctrl.h"
#include "AT_tongxin_xieyi_Ctrl.h"

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
// 测试参数初始化
void test_start_Init()
{
	// 上电先关灯
	LED_Off();
	// 按键设置为开漏
	anjian_kongzhi_OFF();
	// 主电源供电断开
	V3_6_gongdian_kaiguan_Off();
	// 备电电源断开
	NB_dianyuan_gongdian_Off();
	// 主电电压检测控制关闭
	zhudian_dianya_CHK_CTRL_OFF();
	// 二级电源电压检测控制关闭
	erji_dianya_CHK_CTRL_OFF();
	// 电流检测使能关闭
	Current_CHK_CTRL_OFF();
	// 霍尔1控制 关
	huuoer_1_OFF();
	// 霍尔2控制 关
	huuoer_2_OFF();
	// 计量IO1置低
	jiliang_IO1_set_Low();
	// 计量IO2置高
	jiliang_IO2_set_Hi();
	// 脉冲IO12接地脚置低
	maichong_IO12_set_Low();
	// 功耗检测的IIC初始化
	INA219_IIC_GPIO_Init();
	// 开到位置高
	kaidaowei_set_Hi();
	// 关到位置低
	guandaowei_set_Low();
	// 阀控通信引脚置低
	fakong_tongxin_set_Low();
	// 脉冲输出1引脚置高
	maichong_shuchu_1_set_Hi();
	// 脉冲输出2引脚置高
	maichong_shuchu_2_set_Hi();
	// 断线IO3引脚置低
	duanxian_shuchu_IO3_set_Low();
	// 断线IO4引脚置低
	duanxian_shuchu_IO4_set_Low();
	// 3.3V输出引脚拉低
	V3_3_OPT_shuchu_set_Low();
	// 开盖检测输出引脚拉高IO6
	kaigai_OPT_shuchu_set_Hi();
	// 调试口串口通信控制拉低 PB7
	tiaoshi_chuankou_tongxin_set_Low();
	// NB电压读取控制拉低 PB9
	NB_dianya_duqu_set_Low();
	// VDD电压读取控制拉低 PB10
	VDD_dianya_duqu_set_Low();
	// VDD电压读取控制拉低 PB11
	V119_dianya_duqu_set_Low();
	// 通信复位控制拉低 PC13
	tongxin_fuwei_set_Low();
	// 通信唤醒控制拉低 PC14
	tongxin_huanxing_set_Low();
	// 通信电源控制拉低 PC15
	tongxin_dianyuan_set_Low();
	// 屏幕背光控制拉低 PE5
	pingmu_beiguang_set_Low();
	// NB电源供电控制关闭 PC0
	NB_dianyuan_gongdian_Off();
	// 阀控_调试_唤醒关闭 PC2
	FK_tiaoshi_chuankou_huanxing_Off();
	// 阀门断线检测拉低 PC3
	FK_duanxia_CHK_Off();
	// 3.6V供电控制关闭 PC10
	V3_6_gongdian_kaiguan_Off();
	// 阀门负极电压检测控制低 PH15
	FM_fuji_dianyuan_gongdian_Off();
	// LED会亮一下代表初始化完成
	LED_FLAG_Run();
}
// 工位检测
void gongwei_jiance()
{
	Test_jiejuo_jilu.gongwei = 0x00;
	DeBug_print("当前工位为%d\r\n", Test_jiejuo_jilu.gongwei);
}
// 测试结果初始化
void test_jieguo_qingling()
{
	Test_jiejuo_jilu.zhidian_dianya_gongdian = 0;
	Test_jiejuo_jilu.zhidian_jingtai_gonghao = 0;
	memset(Test_jiejuo_jilu.IMEI_CHK, 0x00, 15);
	memset(Test_jiejuo_jilu.IMSI_CHK, 0x00, 15);
	memset(Test_jiejuo_jilu.ICCID_CHK, 0x00, 20);
	Test_jiejuo_jilu.chuankou_tongxin = 0;
	Test_quanju_canshu_L.test_over = 0;
	memset(Test_jiejuo_jilu.banbenhao, 0x00, 2);
	// 接收数据标志位清除
	test_xieyi_jilu_Rec = No_Receive;
}
// 开始测试
void test_start()
{
	// IO口恢复
	test_start_Init();
	// 测试结果清零
	test_jieguo_qingling();
	Test_liucheng_L = w_start;
	// 最大测试时间90秒
	Test_quanju_canshu_L.time_aroundtest_ms = 100000;
	// 单步超时时间
	Test_quanju_canshu_L.danbu_chaoshishijian_ms = 3000;
	Test_quanju_canshu_L.test_over = 0;
	Test_quanju_canshu_L.time_softdelay_ms = 0;
}
// 终止测试
// 终止测试的条件是，测试时间过长，或是主控板离开了工装。
void test_testend()
{
	Test_liucheng_L = w_end;
	Test_quanju_canshu_L.test_over = 1;
	Test_quanju_canshu_L.time_softdelay_ms = 0;
}
// 测试过程中的额外异常事件
void test_err_end_Func()
{
	// 测试超时
	if ((Test_quanju_canshu_L.time_aroundtest_ms == 0 || Test_quanju_canshu_L.danbu_chaoshishijian_ms == 0) && Test_quanju_canshu_L.test_over == 0)
	{
		// 结束测试
		test_testend();
	}
}

void test_Loop_Func()
{
	test_err_end_Func();
	// Test_liucheng_L = w_gonghao_CHK;
	if (Test_quanju_canshu_L.time_softdelay_ms > 0)
		return;
	switch (Test_liucheng_L)
	{
	case w_wait:
		// 仅等待下一次测试
		break;
	case w_start:
		// 开始测试前校验VDD是否有电，从而判断测试是否开始。
		Test_jiejuo_jilu.zhidian_dianya_gongdian = get_zhukongban_gongdian_dianya();
		DeBug_print("VCC电压%d\r\n", Test_jiejuo_jilu.zhidian_dianya_gongdian);
		// 测试合格条件判断
		if (Test_jiejuo_jilu.zhidian_dianya_gongdian > 3200 && Test_jiejuo_jilu.zhidian_dianya_gongdian < 4000)
		{
			// 测试合格，进入下一步
			Test_quanju_canshu_L.time_softdelay_ms = 1000;
			// 重置单步超时时间
			Test_quanju_canshu_L.danbu_chaoshishijian_ms = 10000;
			// 3.6V供电控制开启 PC10
			V3_6_gongdian_kaiguan_On();
			// 阀控通信引脚置高
			fakong_tongxin_set_Hi();
			Test_liucheng_L = w_getAT;
		}
		else
		{
			// 否则1秒复测一次
			Test_quanju_canshu_L.time_softdelay_ms = 1000;
		}
		break;
		// 发送AT指令，查看是否通信
	case w_getAT:
		if (test_xieyi_jilu_Rec == w_OK)
		{
			DeBug_print("获取到OK\r\n");
			Test_jiejuo_jilu.chuankou_tongxin = 1;
			test_xieyi_jilu_Rec = No_Receive;
			Test_quanju_canshu_L.time_softdelay_ms = 0;
			// 重置单步超时时间
			Test_quanju_canshu_L.danbu_chaoshishijian_ms = 10000;
			Test_liucheng_L = w_getATE0;
		}
		else
		{
			// 阀控通信引脚置低
			fakong_tongxin_set_Low();
			To_send_AT();
			DeBug_print("发送AT\r\n");
			test_xieyi_jilu_Rec = No_Receive;
			// 3秒重复发一次
			Test_quanju_canshu_L.time_softdelay_ms = 3000;
		}
		break;
	case w_getATE0:
		if (test_xieyi_jilu_Rec == w_OK)
		{
			DeBug_print("获取到ATE0\r\n");
			test_xieyi_jilu_Rec = No_Receive;
			Test_quanju_canshu_L.time_softdelay_ms = 0;
			// 重置单步超时时间
			Test_quanju_canshu_L.danbu_chaoshishijian_ms = 10000;
			Test_liucheng_L = w_getATI;
		}
		else
		{
			To_send_ATE0();
			DeBug_print("发送ATE0\r\n");
			test_xieyi_jilu_Rec = No_Receive;
			// 3秒重复发一次
			Test_quanju_canshu_L.time_softdelay_ms = 3000;
		}
		break;
	case w_getATI:
		if (test_xieyi_jilu_Rec == w_OK)
		{
			DeBug_print("获取到ATI\r\n");
			test_xieyi_jilu_Rec = No_Receive;
			Test_quanju_canshu_L.time_softdelay_ms = 0;
			// 重置单步超时时间
			Test_quanju_canshu_L.danbu_chaoshishijian_ms = 10000;
			Test_liucheng_L = w_Get_cpin;
		}
		else
		{
			To_send_ATI();
			DeBug_print("发送ATI\r\n");
			test_xieyi_jilu_Rec = No_Receive;
			// 3秒重复发一次
			Test_quanju_canshu_L.time_softdelay_ms = 3000;
		}
		break;
	case w_Get_cpin:
		if (test_xieyi_jilu_Rec == w_get_cpin)
		{
			DeBug_print("获取到cpin\r\n");
			test_xieyi_jilu_Rec = No_Receive;
			Test_quanju_canshu_L.time_softdelay_ms = 0;
			// 重置单步超时时间
			Test_quanju_canshu_L.danbu_chaoshishijian_ms = 10000;
			Test_liucheng_L = w_Get_QCCID;
		}
		else
		{
			To_send_CPIN();
			DeBug_print("发送AT+CPIN?\r\n");
			test_xieyi_jilu_Rec = No_Receive;
			// 3秒重复发一次
			Test_quanju_canshu_L.time_softdelay_ms = 3000;
		}
		break;
	case w_Get_QCCID:
		if (test_xieyi_jilu_Rec == w_get_QCCID)
		{
			memcpy(Test_jiejuo_jilu.ICCID_CHK, Test_linshi_cunchushuju_L.ICCID_CHK, 20);
			DeBug_print("获取到QCCID%s\r\n", Test_jiejuo_jilu.ICCID_CHK);
			test_xieyi_jilu_Rec = No_Receive;
			Test_quanju_canshu_L.time_softdelay_ms = 0;
			// 重置单步超时时间
			Test_quanju_canshu_L.danbu_chaoshishijian_ms = 10000;
			Test_liucheng_L = w_Get_GSN;
		}
		else
		{
			To_send_QCCID();
			DeBug_print("发送QCCID\r\n");
			test_xieyi_jilu_Rec = No_Receive;
			// 3秒重复发一次
			Test_quanju_canshu_L.time_softdelay_ms = 3000;
		}
		break;
	case w_Get_GSN:
		if (test_xieyi_jilu_Rec == w_OK)
		{
			memcpy(Test_jiejuo_jilu.IMSI_CHK, Test_linshi_cunchushuju_L.IMSI_CHK, 15);
			DeBug_print("获取到GSN%s\r\n", Test_jiejuo_jilu.IMSI_CHK);
			test_xieyi_jilu_Rec = No_Receive;
			Test_quanju_canshu_L.time_softdelay_ms = 0;
			// 重置单步超时时间
			Test_quanju_canshu_L.danbu_chaoshishijian_ms = 10000;
			Test_liucheng_L = w_Get_CIMI;
		}
		else
		{
			To_send_GSN();
			DeBug_print("发送GSN\r\n");
			test_xieyi_jilu_Rec = w_get_GSN;
			// 3秒重复发一次
			Test_quanju_canshu_L.time_softdelay_ms = 3000;
		}
		break;
	case w_Get_CIMI:
		if (test_xieyi_jilu_Rec == w_OK)
		{
			memcpy(Test_jiejuo_jilu.IMEI_CHK, Test_linshi_cunchushuju_L.IMEI_CHK, 15);
			DeBug_print("获取到CIMI%s\r\n", Test_jiejuo_jilu.IMEI_CHK);
			test_xieyi_jilu_Rec = No_Receive;
			Test_quanju_canshu_L.time_softdelay_ms = 0;
			// 重置单步超时时间
			Test_quanju_canshu_L.danbu_chaoshishijian_ms = 10000;
			Test_liucheng_L = w_Get_CSQ;
		}
		else
		{
			To_send_CIMI();
			DeBug_print("发送CIMI\r\n");
			test_xieyi_jilu_Rec = w_get_CIMI;
			// 3秒重复发一次
			Test_quanju_canshu_L.time_softdelay_ms = 3000;
		}
		break;
	case w_Get_CSQ:
		if (test_xieyi_jilu_Rec == w_get_CSQ)
		{
			Test_jiejuo_jilu.CSQ = Test_linshi_cunchushuju_L.CSQ;
			DeBug_print("获取到CSQ%d\r\n", Test_jiejuo_jilu.CSQ);
			test_xieyi_jilu_Rec = No_Receive;
			if (Test_jiejuo_jilu.CSQ <= 33 && Test_jiejuo_jilu.CSQ > 5)
			{
				Test_quanju_canshu_L.time_softdelay_ms = 0;
				// 重置单步超时时间
				Test_quanju_canshu_L.danbu_chaoshishijian_ms = 10000;
				Test_liucheng_L = w_Guanji;
			}
			else
			{
				To_send_CSQ();
				DeBug_print("发送CSQ\r\n");
				test_xieyi_jilu_Rec = No_Receive;
				// 3秒重复发一次
				Test_quanju_canshu_L.time_softdelay_ms = 3000;
			}
		}
		else
		{
			To_send_CSQ();
			DeBug_print("发送CSQ\r\n");
			test_xieyi_jilu_Rec = No_Receive;
			// 3秒重复发一次
			Test_quanju_canshu_L.time_softdelay_ms = 3000;
		}
		break;
	case w_Guanji:
		if (test_xieyi_jilu_Rec == w_OK)
		{
			DeBug_print("获取到OK\r\n");
			test_xieyi_jilu_Rec = No_Receive;
			Test_quanju_canshu_L.time_softdelay_ms = 0;
			// 重置单步超时时间
			Test_quanju_canshu_L.danbu_chaoshishijian_ms = 10000;
			Test_quanju_canshu_L.time_softdelay_ms = 2000;
			Test_liucheng_L = w_gonghao_CHK;
		}
		else
		{
			// 阀控通信引脚置低
			fakong_tongxin_set_Low();
			To_send_QPOWD();
			DeBug_print("发送关机\r\n");
			test_xieyi_jilu_Rec = w_guanji;
			// 3秒重复发一次
			Test_quanju_canshu_L.time_softdelay_ms = 3000;
		}
		break;
	case w_gonghao_CHK:
		Test_jiejuo_jilu.zhidian_jingtai_gonghao = Current_CHK_Func(0);
		DeBug_print("供电电流%d\r\n", Test_jiejuo_jilu.zhidian_jingtai_gonghao);
		if (Test_jiejuo_jilu.zhidian_jingtai_gonghao > 2 && Test_jiejuo_jilu.zhidian_jingtai_gonghao < 30)
		{
			// 测试合格，进入下一步
			Test_quanju_canshu_L.time_softdelay_ms = 0;
			// 重置单步超时时间 10秒
			Test_quanju_canshu_L.danbu_chaoshishijian_ms = 10000;
			Test_liucheng_L = w_end;
		}
		else
		{
			// 否则1秒复测一次
			Test_quanju_canshu_L.time_softdelay_ms = 1000;
		}
		break;
	case w_end:
		DeBug_print("测试完成\r\n");
		// 主电源供电断开
		V3_6_gongdian_kaiguan_Off();
		// 电机停止动作
		huuoer_1_OFF();
		// 一切测试都已结束，打开测试返回
		Test_quanju_canshu_L.test_over = 1;
		// 回到第一步
		Test_liucheng_L = w_wait;
		break;
	default:
		Test_liucheng_L = w_end;
		break;
	}
}
// 该事件为测试委托事件，在主函数中运行
void Test_loop_func()
{
	// 在未测试状态下，会每3秒发送一次AT
}
