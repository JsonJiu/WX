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
enum Test2 Test_agreement_receive;

/*————————定时器全局变量计时————————*/
uint32_t single_step_timing = 0; //单步测试时间
uint32_t interval_timing = 0; 	 //间隔测试时间


/*————————工位检测————————*/
void Workstation_CHK(void)
{
	if(FL_GPIO_GetInputPin(GPIOB,FL_GPIO_PIN_8) == 0 && FL_GPIO_GetInputPin(GPIOB,FL_GPIO_PIN_7) == 0)
	{
		Test_result.Workstation = 0x00;
	}
	if(FL_GPIO_GetInputPin(GPIOB,FL_GPIO_PIN_8) == 0 && FL_GPIO_GetInputPin(GPIOB,FL_GPIO_PIN_7) == 1)
	{
		Test_result.Workstation = 0x01;
	}
	if(FL_GPIO_GetInputPin(GPIOB,FL_GPIO_PIN_8) == 1 && FL_GPIO_GetInputPin(GPIOB,FL_GPIO_PIN_7) == 0)
	{
		Test_result.Workstation = 0x02;
	}
	if(FL_GPIO_GetInputPin(GPIOB,FL_GPIO_PIN_8) == 1 && FL_GPIO_GetInputPin(GPIOB,FL_GPIO_PIN_7) == 1)
	{
		Test_result.Workstation = 0x03;
	}
}

/*————————双电单电选择————————*/
/*
工位4 IO口加跳线帽为双电，不加为单电
*/
uint8_t Power_Select(void)
{
	uint8_t select = 0;
	if(FL_GPIO_GetInputPin(GPIOB,FL_GPIO_PIN_5) == 1)
	{
		Output_7V2_CTL(On);
		Output_3V6_CTL(Off);
		select = 1;
	}
	if(FL_GPIO_GetInputPin(GPIOB,FL_GPIO_PIN_5) == 0)
	{
		Output_7V2_CTL(On);
		Output_3V6_CTL(On);
		select = 0;
	}
	return select;
}

/*————————测试初始化————————*/
/*
1.单电双电初始化
2.工位检测
3.电磁铁供电关闭，12V供电预留，要用到霍尔时再更新程序
4.舵机角度归0
5.3.6V功耗检测关闭
6.7.2V功耗检测关闭
7.AD1 AD2 AD3 AD4检测全都关闭
8.主控板LED控制关闭
9.12V电磁铁1控制关闭
10.12V电磁铁2控制关闭
11.测试步骤为等待
*/
void Test_Init(void)
{
	Power_Select();
	Workstation_CHK();
	Output_12V_CTL(Off);
	Motor_Angle_Select(0);
	Current_3V6_I_CHK_CTL(Off);
	Current_7V2_I_CHK_CTL(Off);
	ADC_CHK_CTL(AD1,Off);
	ADC_CHK_CTL(AD2,Off);
	ADC_CHK_CTL(AD3,Off);
	ADC_CHK_CTL(AD4,Off);
	Ext_LED_CTL(Off);
	Output_12V_CTL1(Off);
	Output_12V_CTL2(Off);
	Brownout_detect(Off);
	Test_process = t_wait;
}

/*————————测试结果初始化————————*/
void Test_Result_Init(void)
{
	Test_result.Find_Enable = 0;
	Test_agreement_receive = NO_receive;

	Test_result.Infrared = 0;
	Test_result.Voltage_7V2_Mould = 0;
	Test_result.Voltage_3V6_Mould = 0;
	Test_result.Infrared = 0;
	Test_result.Current_3V6 = 0;
	Test_result.Current_7V2 = 0;
	Test_result.Voltage_Motor_Power = 0;
	Test_result.Motor_B_flag = 0;
	Test_result.Motor_R_flag = 0;
	Test_result.Motor_test = 0;
	Test_result.UMU_test = 0;
	Test_result.UMU_RXD = 0;
//	Test_result.UMU_TXD = 0; //因为板子上电会发数据，那时候标志位就置1，所以后面开始测试的话会把该标志位清零，这边不清零，放在测试结束再清零
	Test_result.NB_test = 0;
	Test_result.KEY_OPEN = 0;
	
	Test_result.UMU_Flow_Rate = 0;
	Test_result.UMU_Voltage = 0;
	Test_result.NB_Voltage = 0;
	Test_result.flash = 0;
	Test_result.eeprom = 0;
	Test_result.IC_test = 0;
	Test_result.Alarm119 = 0;
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
	if(interval_timing > 0)
		return;
	switch(Test_process)
	{
		case t_wait:
			break;
		case t_start:
			Test_Start();
			Test_process = t_get_7V2_voltage;
			break;
		case t_get_7V2_voltage:  //7.2V电压输出正常才会开始检测
			Test_result.Voltage_7V2_Mould = Get_7V2_Voltage();
			if(Test_result.Voltage_7V2_Mould > 6000 && Test_result.Voltage_7V2_Mould < 7600)
			{
					Test_process = t_close_screen_led;
					single_step_timing = 25000;//10s时间关闭
			}
			else
				Test_process = t_end;
			break;
		case t_close_screen_led:
			if(single_step_timing>0)
			{
				if(Test_agreement_receive == r_E20B)
				{
					Test_agreement_receive = NO_receive;
					single_step_timing = 40000; //20s时间测试
					Test_process = t_get_7V2_current;
				}
				else
				{
					Close_Screen_LED();
					interval_timing = 2000;//两秒发一次
				}
			}
			else
			{
				single_step_timing = 50000; //20s时间测试
				Test_process = t_get_7V2_current;
			}
			break;
		case t_get_7V2_current:
			if(single_step_timing>0)
			{
				Test_result.Current_7V2 = Current_CHK_Func(0);
				if(Test_result.Current_7V2>10 && Test_result.Current_7V2<50)
				{
					single_step_timing = 20000;//10s上告协议发送时间
					if(Power_Select() == 0)
						Test_process = t_get_3V6_voltage;
					else 
						Test_process = t_open_xingshan;
				}
				else
					interval_timing = 1000;//0.5s测一次				
			}
			else
			{
				single_step_timing = 20000;//10s上告协议发送时间
			if(Power_Select() == 0)
					Test_process = t_get_3V6_voltage;
				else 
					Test_process = t_open_xingshan;
			}
			break;
		case t_get_3V6_voltage:
			Test_result.Voltage_3V6_Mould = Get_3V6_Voltage();
			if(Test_result.Voltage_3V6_Mould > 3200 && Test_result.Voltage_3V6_Mould < 4000)
			{
				Test_process = t_get_3V6_current;
				single_step_timing = 40000; //20s时间测试
			}
			else
				Test_process = t_end;
			break;
		case t_get_3V6_current:
			if(single_step_timing>0)
			{
				Test_result.Current_3V6 = Current_CHK_Func(1);
				if(Test_result.Current_3V6>4 && Test_result.Current_3V6<30)
				{
					single_step_timing = 20000;//10s上告协议发送时间
					Test_process = t_open_xingshan;
				}
				else
					interval_timing = 1000;//0.5s测一次				
			}
			else
			{
				single_step_timing = 20000;//10s上告协议发送时间
				Test_process = t_open_xingshan;
			}
			break;
		case t_open_xingshan:  //打开星闪
			if(single_step_timing>0)
			{
				if(Test_agreement_receive == r_A027)
				{				
					Test_agreement_receive = NO_receive;					
					single_step_timing = 20000;
					Test_process = t_IC_test;					
				}
				else
				{
				Open_xingshan();
				interval_timing = 2000;//两秒发一次
				}
			}
			else
				{				
				Test_process = t_IC_test; 
			  }			
			break;	
	  case t_IC_test:  //刷卡得放在上告前，貌似上告后刷卡没反应
				Motor_Angle_Select(90);
				Ext_LED_CTL(On);
				FL_DelayMs(15000);//星闪打开需要时间增加延迟5s
		    single_step_timing = 30000;
				Test_process = t_start_shanggao;
			break;
		case t_start_shanggao:
			if(single_step_timing>0)
			{
				if(Test_agreement_receive == r_1005)
				{
					Test_agreement_receive = NO_receive;
					single_step_timing = 30000;//10串口模拟上告发送时间
					Test_process = t_simulate_shanggao;
				}
				else
				{
					Start_Shanggao();
					interval_timing = 4000;//两秒发一次
				}
			}
			else
				Test_process = t_get_motor_Power; //上告无反应直接跳过NB串口模拟上告
			break;
		case t_simulate_shanggao:
			Motor_Angle_Select(0);
			if(single_step_timing>0)
			{
				if(Test_agreement_receive == r_simulate_shanggao)
				{
					Test_agreement_receive = NO_receive;
					single_step_timing = 0;
					Test_process = t_get_motor_Power;
				}
				else
				{
					//NB_Simulate();
					interval_timing = 1000;//两秒发一次
				}
			}
			else
			{
				single_step_timing = 0;
				Test_process = t_get_motor_Power;
			}
			break;
//		case t_LED_test:
//			Test_result.LED_Voltage = Get_LED_Voltage();
//			if(Test_result.LED_Voltage<500)
//			{
//				Test_result.LED = 1;
//				Test_process = t_get_motor_Power;
//			}
//			else
//				Test_process = t_get_motor_Power;
//			break;
		case t_get_motor_Power:  //阀门供电电压正常才会检测阀门输出电平，否则跳过，默认检测错误
			Ext_LED_CTL(Off);
			Test_result.Voltage_Motor_Power = Get_AD2_Voltage();
			if(Test_result.Voltage_Motor_Power>4500 && Test_result.Voltage_Motor_Power<5200)
			{
				Test_process = t_E20A;
				single_step_timing = 20000; //10s测试时间
			}
			else
				Test_process = t_end;
			break;
		case t_E20A: //包含阀门测试和数据查询
			if(single_step_timing>0)
			{
				if(Test_agreement_receive == r_E20A)
				{
					Test_agreement_receive = NO_receive;
					single_step_timing = 9000; //阀门动作总共需要4.5s
					Test_process = t_motor_test;
				}
				else
				{
					Send_E20A();
					interval_timing = 4000;//两秒发一次
				}
			}
			else
				Test_process = t_end;
			break;
		case t_motor_test:
			if(single_step_timing == 0)
				Test_process = t_end;
			break;
		case t_end:
			Test_process = t_wait;
			Test_result.Find_Enable = 1; //表示应答上位询问数据
			Test_result.UMU_TXD = 0;
			Motor_Angle_Select(0);
			break;
		default:
			Test_process = t_end;
			break;
	}
}

/*————————阀门测试————————*/
/*
阀门测试要放到主函数里实时检测，
阀门动的时间比较短，
如果放到测试步骤E20A后面就会错过测试时间从而导致测试异常
*/
void Moter_Test(void)  
{
	if(Test_process == t_E20A || Test_process == t_motor_test)
	{
		if(Get_Motor_R_Voltage()>4500 && Get_Motor_B_Voltage()<500)
			Test_result.Motor_R_flag = 1;
		if(Get_Motor_B_Voltage()>4500 && Get_Motor_R_Voltage()<500)
			Test_result.Motor_B_flag = 1;
		if(Test_result.Motor_R_flag == 1 && Test_result.Motor_B_flag == 1)
			{
				Test_result.Motor_test = 1;
			}
	}
}
