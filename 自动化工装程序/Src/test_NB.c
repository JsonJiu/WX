#include "test.h"
#include "gpio.h"
#include "ina219.h"
#include "ir.h"
#include "measurement.h"
#include "adc.h"
#include "led.h"

/*————————结构体枚举定义————————*/
extern struct Test Test_result;
extern enum Test1 Test_process;
extern enum Test2 Test_agreement_receive;

/*————————定时器全局变量计时————————*/
extern uint32_t single_step_timing; // 单步测试时间
extern uint32_t interval_timing;	// 间隔测试时间

extern uint8_t repeat_report; // 重复上告计数

/*————————测试函数————————*/
void Test_Function_NB(void)
{
	if (interval_timing > 0)
		return;
	switch (Test_process)
	{
	case t_wait:
		break;
	case t_start:
		startTestInit();
		Test_process = t_get_main_voltage;
		break;
	case t_get_main_voltage:
		Test_result.Voltage_Main_Mould = Voltage_Main_Mould_CHK();
		if (Test_result.Voltage_Main_Mould > 3400 && Test_result.Voltage_Main_Mould < 4200)
		{
			Test_process = t_change_empty_water;
			single_step_timing = 10000; // 10s时间测试
		}
		else
			Test_process = t_end;
		break;
	case t_change_empty_water:
		if (single_step_timing > 0)
		{
			if (Test_agreement_receive == r_empty_water)
			{
				Test_agreement_receive = NO_receive;
				interval_timing = 2000;
				single_step_timing = 10000;
				Test_process = t_get_static_current;
			}
			else
			{
				JILIAN_wushui_xieyi();
				interval_timing = 2000; // 两秒发一次
			}
		}
		else
		{
			single_step_timing = 10000;
			Test_process = t_change_full_water;
		}
		break;
	case t_get_static_current:
		if (single_step_timing > 0)
		{
			Test_result.Current_Main_Static = Current_CHK_Func(0);
			if (Test_result.Current_Main_Static > 5 && Test_result.Current_Main_Static < 40)
			{
				single_step_timing = 10000;
				Test_process = t_change_full_water;
			}
			else
				interval_timing = 1000;
		}
		else
		{
			single_step_timing = 10000;
			Test_process = t_change_full_water;
		}
		break;
	case t_change_full_water:
		if (single_step_timing > 0)
		{
			if (Test_agreement_receive == r_full_water)
			{
				Test_agreement_receive = NO_receive;
				interval_timing = 3000;
				single_step_timing = 10000;
				Test_process = t_get_full_water_current;
			}
			else
			{
				JILIAN_manshui_xieyi();
				interval_timing = 2000; // 两秒发一次
			}
		}
		else
		{
			single_step_timing = 10000;
			Test_process = t_change_flow_water;
		}
		break;
	case t_get_full_water_current:
		if (single_step_timing > 0)
		{
			Test_result.Current_Full_Water = Current_CHK_Func(0);
			if (Test_result.Current_Full_Water > 4 && Test_result.Current_Full_Water < 40)
			{
				single_step_timing = 10000;
				Test_process = t_change_flow_water;
			}
			else
				interval_timing = 1000;
		}
		else
		{
			single_step_timing = 10000;
			Test_process = t_change_flow_water;
		}
		break;
	case t_change_flow_water:
		if (single_step_timing > 0)
		{
			if (Test_agreement_receive == r_flow_water)
			{
				Test_agreement_receive = NO_receive;
				interval_timing = 3000;
				single_step_timing = 10000;
				Test_process = t_get_flow_water_current;
			}
			else
			{
				JILIAN_zoushui_xieyi();
				interval_timing = 2000; // 两秒发一次
			}
		}
		else
		{
			single_step_timing = 10000;
			Test_process = t_get_flow_water_current;
		}
		break;
	case t_get_flow_water_current:
		if (single_step_timing > 0)
		{
			Test_result.Current_Flow_Water = Current_CHK_Func(0);
			if (Test_result.Current_Flow_Water > 4 && Test_result.Current_Flow_Water < 40)
			{
				single_step_timing = 10000;
				Test_process = t_get_Standby_voltage;
			}
			else
				interval_timing = 1000;
		}
		else
		{
			single_step_timing = 10000;
			Test_process = t_get_Standby_voltage;
		}
		break;
	case t_get_Standby_voltage:
		Test_result.Voltage_Standby_Mould = Test_result.Voltage_Main_Mould;
		Test_process = t_start_Wmbus_test;
		single_step_timing = 10000; // 10s时间测试
		break;
	case t_start_Wmbus_test: // 因为wmbus检测需要15s，所以放在功耗后第一步开始
		if (single_step_timing > 0)
		{
			if (Test_agreement_receive == r_Wmbus_test)
			{
				Test_agreement_receive = NO_receive;
				interval_timing = 0;
				single_step_timing = 10000;
				Test_process = t_find_result_first;
			}
			else
			{
				Start_Wmbus_Test();
				interval_timing = 3000; // 3秒发一次
			}
		}
		else
		{
			single_step_timing = 10000;
			Test_process = t_find_result_first;
		}
		break;
	case t_find_result_first: // 这边先单独查询强磁和开盖在电磁铁不通电时的状态
		if (single_step_timing > 0)
		{
			if (Test_agreement_receive == r_find_result)
			{
				Test_agreement_receive = NO_receive;
				interval_timing = 0;
				single_step_timing = 10000;
				Test_process = t_find_device_ID;
			}
			else
			{
				Find_Result();
				interval_timing = 3000;
			}
		}
		else
		{
			single_step_timing = 10000;
			Test_process = t_find_device_ID;
		}
		break;
	case t_find_device_ID:
		if (single_step_timing > 0)
		{
			if (Test_agreement_receive == r_find_device_ID)
			{
				Test_agreement_receive = NO_receive;
				interval_timing = 0;
				single_step_timing = 10000;
				Test_result.cumulant1_int = (Test_result.cumulant_Data[0] << 24) | (Test_result.cumulant_Data[1] << 16) | (Test_result.cumulant_Data[2] << 8) | Test_result.cumulant_Data[3];
				Test_result.cumulant1_decimal = (Test_result.cumulant_Data[4] << 24) | (Test_result.cumulant_Data[5] << 16) | (Test_result.cumulant_Data[6] << 8) | Test_result.cumulant_Data[7];
				Test_process = t_set_true;
			}
			else
			{
				Find_Device_ID();
				interval_timing = 3000;
			}
		}
		else
		{
			single_step_timing = 10000;
			Test_process = t_set_true;
		}
		break;
	case t_set_true:
		if (single_step_timing > 0)
		{
			if (Test_agreement_receive == r_set_true)
			{
				Test_agreement_receive = NO_receive;
				interval_timing = 0;
				single_step_timing = 10000;
				Test_process = t_find_check_code;
			}
			else
			{
				Set_true();
				interval_timing = 3000;
			}
		}
		else
		{
			single_step_timing = 10000;
			Test_process = t_find_check_code;
		}
		break;
	case t_find_check_code:
		if (single_step_timing > 0)
		{
			if (Test_agreement_receive == r_find_check_code)
			{
				Test_agreement_receive = NO_receive;
				interval_timing = 0;
				single_step_timing = 10000;
				Test_process = t_Measurement_test;
			}
			else
			{
				Find_Version();
				interval_timing = 3000;
			}
		}
		else
		{
			single_step_timing = 10000;
			Test_process = t_Measurement_test;
		}
		break;
	case t_Measurement_test:
		if (single_step_timing > 0)
		{
			if (Test_agreement_receive == r_find_device_ID)
			{
				Test_agreement_receive = NO_receive;
				interval_timing = 0;
				single_step_timing = 10000;
				Test_result.cumulant2_int = (Test_result.cumulant_Data[0] << 24) | (Test_result.cumulant_Data[1] << 16) | (Test_result.cumulant_Data[2] << 8) | Test_result.cumulant_Data[3];
				Test_result.cumulant2_decimal = (Test_result.cumulant_Data[4] << 24) | (Test_result.cumulant_Data[5] << 16) | (Test_result.cumulant_Data[6] << 8) | Test_result.cumulant_Data[7];
				if ((Test_result.cumulant2_int > Test_result.cumulant1_int) && Test_result.gp30_state == 1)
				{
					Test_result.Measurement = 1;
				}
				if ((Test_result.cumulant2_int == Test_result.cumulant1_int) && Test_result.gp30_state == 1)
				{
					if (Test_result.cumulant2_decimal > Test_result.cumulant1_decimal)
						Test_result.Measurement = 1;
					else
						Test_result.Measurement = 0;
				}
				else
				{
					Test_result.Measurement = 0;
				}
				Test_process = t_open_magnet;
			}
			else
			{
				Find_Device_ID();
				interval_timing = 3000;
			}
		}
		else
		{
			single_step_timing = 0;
			Test_process = t_open_magnet;
		}
		break;
	case t_open_magnet:
		Output_12V_CTL1(On);
		Output_12V_CTL2(On);
		interval_timing = 1000;
		single_step_timing = 40000;
		Test_process = t_find_result_second;
		break;
	case t_find_result_second:
		if (single_step_timing > 0)
		{
			if (Test_agreement_receive == r_find_result)
			{
				Test_agreement_receive = NO_receive;
				interval_timing = 0;
				single_step_timing = 10000;
				Test_process = t_get_Standby_current;
			}
			else
			{
				Find_Result();
				interval_timing = 3000;
			}
		}
		else
		{
			single_step_timing = 10000;
			Test_process = t_get_Standby_current;
		}
		break;
	case t_get_Standby_current: // 这边是开机自动上告，所以需要最后测通讯功耗
		if (single_step_timing > 0)
		{
			Test_result.Current_Standby = Current_CHK_Func(1);
			if (Test_result.Current_Standby > 4 && Test_result.Current_Standby < 30)
			{
				single_step_timing = 0;
				Test_process = t_end;
			}
			else
				interval_timing = 1000;
		}
		else
		{
			single_step_timing = 0;
			Test_process = t_end;
		}
		break;
	case t_end:
		Test_process = t_wait;
		Test_result.Find_Enable = 1; // 表示应答上位询问数据
		Output_12V_CTL1(Off);
		Output_12V_CTL2(Off);
		break;
	default:
		Test_process = t_end;
		break;
	}
}
