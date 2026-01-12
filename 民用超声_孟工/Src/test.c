#include "test.h"
#include "gpio.h"
#include "pwm.h"
#include "adc.h"
#include "ZDINA219.h"
#include "umu.h"
#include "NB.h"
#include "ir.h"

/*————————结构体枚举定义————————*/
struct Test Test_result;
enum Test1 Test_process;

/*————————定时器全局变量计时————————*/
uint32_t single_step_timing = 0; // 单步测试时间
uint32_t interval_timing = 0;	 // 间隔测试时间
uint32_t umu_timing = 0;		 // UMU定时发送时间

/*————————工位检测————————*/
void Workstation_CHK(void)
{
	if (FL_GPIO_GetInputPin(GPIOB, FL_GPIO_PIN_8) == 0 && FL_GPIO_GetInputPin(GPIOB, FL_GPIO_PIN_7) == 0)
	{
		Test_result.Workstation = 0x00;
	}
	if (FL_GPIO_GetInputPin(GPIOB, FL_GPIO_PIN_8) == 0 && FL_GPIO_GetInputPin(GPIOB, FL_GPIO_PIN_7) == 1)
	{
		Test_result.Workstation = 0x01;
	}
	if (FL_GPIO_GetInputPin(GPIOB, FL_GPIO_PIN_8) == 1 && FL_GPIO_GetInputPin(GPIOB, FL_GPIO_PIN_7) == 0)
	{
		Test_result.Workstation = 0x02;
	}
	if (FL_GPIO_GetInputPin(GPIOB, FL_GPIO_PIN_8) == 1 && FL_GPIO_GetInputPin(GPIOB, FL_GPIO_PIN_7) == 1)
	{
		Test_result.Workstation = 0x03;
	}
}

/*————————工装供电初始化————————*/
void Power_Init(void)
{
	Output_7V2_CTL(On);
	Output_3V6_CTL(On);
}

/*————————测试初始化————————*/
/*
1.供电初始化
2.工位检测
3.电磁铁供电关闭，12V供电预留，要用到霍尔时再更新程序
4.3.6V功耗检测关闭
5.7.2V功耗检测关闭
6.AD1 AD2 AD3检测全都关闭
7.测试步骤为等待
*/
void Test_Init(void)
{
	Power_Init();
	Workstation_CHK();
	Output_12V_CTL(Off);
	Current_3V6_I_CHK_CTL(Off);
	Current_7V2_I_CHK_CTL(Off);
	ADC_CHK_CTL(AD1, Off);
	ADC_CHK_CTL(AD2, Off);
	ADC_CHK_CTL(AD3, Off);
	Test_process = t_wait;
}

/*————————测试结果初始化————————*/
void Test_Result_Init(void)
{
	Test_result.Find_Enable = 0;
	Test_result.Agreement_receive = NO_receive;

	Test_result.Voltage_zhudian = 0;
	Test_result.Current_zhudian = 0;
	Test_result.Voltage_beidian = 0;
	Test_result.Current_beidian = 0;
	Test_result.UMU = 0;
	Test_result.LED = 0;
	Test_result.Motor_test = 0;
	Test_result.Motor_R_flag = 0;
	Test_result.Motor_B_flag = 0;
	Test_result.Infrared = 0;
	Test_result.CAPStatus = 0;
	memset(Test_result.E20A, 0, sizeof(Test_result.E20A));
	memset(Test_result.IMEI, 0, sizeof(Test_result.IMEI));
	memset(Test_result.IMSI, 0, sizeof(Test_result.IMSI));
	memset(Test_result.ICCID, 0, sizeof(Test_result.ICCID));
	memset(Test_result.SOFT_Version, 0, sizeof(Test_result.SOFT_Version));
	memset(Test_result.Comp_Time, 0, sizeof(Test_result.Comp_Time));
	Test_result.Pressure = 0;
	memset(Test_result.PlatePressure, 0, sizeof(Test_result.PlatePressure));
	memset(Test_result.ESAMID, 0, sizeof(Test_result.ESAMID));
	Test_result.BD_Diode = 0;
}

/*————————开始测试————————*/
void Test_Start(void)
{
	Test_Init();
	Test_Result_Init();
	Test_process = t_start;
}

/*————————测试函数————————*/
void Test_Function(void)
{
	if (interval_timing > 0)
		return;
	switch (Test_process)
	{
	case t_wait:
		break;
	case t_start:
		Test_Start();
		Test_process = t_get_gongzhuang_voltage;
		break;
	case t_get_gongzhuang_voltage: // 主电电压和备电电压输出正常才会开始检测，双锂电
		Test_result.Voltage_zhudian = Get_7V2_Voltage();
		Test_result.Voltage_beidian = Get_3V6_Voltage();
		if (Test_result.Voltage_zhudian > 3200 && Test_result.Voltage_zhudian < 4000 && Test_result.Voltage_beidian > 3200 && Test_result.Voltage_beidian < 4000)
		{
			Test_process = t_get_zhudian_current;
			single_step_timing = 15000; // 15s时间关闭
		}
		else
			Test_process = t_end;
		break;
	case t_get_zhudian_current:
		if (single_step_timing > 0)
		{
			Test_result.Current_zhudian = Current_CHK_Func(0);
			if (Test_result.Current_zhudian > 6 && Test_result.Current_zhudian < 50)
			{
				single_step_timing = 15000; // 15s时间关闭
				Test_process = t_get_beidian_current;
			}
			else
				interval_timing = 1000; // 1s测一次
		}
		else
		{
			single_step_timing = 15000; // 15s时间关闭
			Test_process = t_get_beidian_current;
		}
		break;
	case t_get_beidian_current:
		if (single_step_timing > 0)
		{
			Test_result.Current_beidian = Current_CHK_Func(1);
			if (Test_result.Current_beidian > 0 && Test_result.Current_beidian < 10)
			{
				single_step_timing = 50000; // 50s时间关闭
				Test_process = t_start_shanggao;
			}
			else
				interval_timing = 1000; // 1s测一次
		}
		else
		{
			single_step_timing = 50000; // 50s时间关闭
			Test_process = t_start_shanggao;
		}
		break;
	case t_start_shanggao:
		Start_Shanggao();
		interval_timing = 25000; // 等待25s上告完成,并进行解析捕捉1000协议
		Test_process = t_jiance_shanggao;
		break;
	case t_jiance_shanggao:
		if (single_step_timing == 0 || Test_result.Agreement_receive == r_1000)
		{
			Test_result.Agreement_receive = NO_receive;
			single_step_timing = 15000; // 15s时间关闭
			Test_process = t_read_IMEI_IMSI_ICCID;
		}
		break;
	case t_read_IMEI_IMSI_ICCID:
		if (single_step_timing > 0)
		{
			if (Test_result.Agreement_receive == r_C525)
			{
				Test_result.Agreement_receive = NO_receive;
				single_step_timing = 15000; // 15s时间关闭
				Test_process = t_get_version;
			}
			else
			{
				Read_IMEI_IMSI_ICCID();
				interval_timing = 2000; // 两秒发一次
			}
		}
		else
		{
			single_step_timing = 15000; // 15s时间关闭
			Test_process = t_get_version;
		}
		break;
	case t_get_version:
		if (single_step_timing > 0)
		{
			if (Test_result.Agreement_receive == r_C518)
			{
				Test_result.Agreement_receive = NO_receive;
				single_step_timing = 15000; // 15s时间关闭
				Test_process = t_motor_test;
			}
			else
			{
				Read_Version();
				interval_timing = 2000; // 两秒发一次
			}
		}
		else
		{
			single_step_timing = 15000; // 15s时间关闭
			Test_process = t_motor_test;
		}
		break;
	case t_motor_test:
		if (single_step_timing > 0)
		{
			if (Test_result.Agreement_receive == r_E20A)
			{
				Test_result.Agreement_receive = NO_receive;
				single_step_timing = 0;
				Test_process = t_end;
			}
			else
			{
				Send_E20A();
				interval_timing = 4000; // 四秒发一次
			}
		}
		else
		{
			single_step_timing = 0;
			Test_process = t_end;
		}
		break;
		//	case t_get_esamid:
		//		if (single_step_timing > 0)
		//		{
		//			if (Test_result.Agreement_receive == r_C519)
		//			{
		//				Test_result.Agreement_receive = NO_receive;
		//				single_step_timing = 0;
		//				Test_process = t_end;
		//			}
		//			else
		//			{
		//				Read_ESAMID();
		//				interval_timing = 2000; // 两秒发一次
		//			}
		//		}
		//		else
		//		{
		//			single_step_timing = 0;
		//			Test_process = t_end;
		//		}
		//		break;
	case t_end:
		Test_process = t_wait;
		Test_result.Find_Enable = 1; // 表示应答上位询问数据
		break;
	default:
		Test_process = t_end;
		break;
	}
}

/*————————阀门测试————————*/
/*
民超的E20A动阀门速度很快，us级反转，不能用ADC，检测速度太慢检测不到
*/
void Moter_Test(void)
{
	if (Test_process == t_motor_test)
	{
		if (Motor_CHK(MOTOR_B) == 1 && Motor_CHK(MOTOR_R) == 0)
			Test_result.Motor_R_flag = 1;
		if (Motor_CHK(MOTOR_B) == 0 && Motor_CHK(MOTOR_R) == 1)
			Test_result.Motor_B_flag = 1;
		if (Test_result.Motor_R_flag == 1 && Test_result.Motor_B_flag == 1)
		{
			Test_result.Motor_test = 1;
		}
	}
}

/*————————UMU定时发送————————*/
/*
E20A会查询UMU流量
*/
void UMU_Timing_Send(void)
{
	if (umu_timing == 0 && Test_process != t_wait)
	{
		UMU_Simulate();
		umu_timing = 2000; // 2s发送一次
	}
}
