#ifndef __TEST__H
#define __TEST__H
#include "main.h"

void Test_Init(void);
void Test_Start(void);
void Test_Function(void);
void Moter_Test(void);

/*————————测试结果记录————————*/
struct Test
{
	uint8_t Find_Enable; //上位查询使能标志位
	
	uint8_t Workstation; //工位
	uint8_t Type;  			 //类型 00-民超  01-商超
	uint8_t Infrared;    //红外
	uint32_t Voltage_7V2_Mould;//7.2V工装自检
	uint32_t Voltage_3V6_Mould;//3.6V工装自检
	uint16_t Current_7V2;//7.2V电流检测
	uint16_t Current_3V6;//3.6V电流检测
	uint32_t Voltage_Motor_Power;//阀门芯片供电检测
	uint32_t Motor_B_flag;//阀门单端电压检测
	uint32_t Motor_R_flag;//阀门单端电压检测
	uint8_t Motor_test;//阀门总测试
	uint8_t UMU_test;//UMU串口通讯测试
	uint8_t UMU_RXD;//UMU的RXD由主控板的流量反应，E20A读取到流量说明RXD正常
	uint8_t UMU_TXD;//UMU的TXD需要在上告的时候测试，上告时，主控板会从UMU串口发送出来一串数据
	uint8_t NB_test;//NB通讯测试
	
	/*E20A协议读取部分*/
	uint32_t UMU_Flow_Rate; //流速
	uint16_t UMU_Voltage;   //0x24-3.6V  上传给mes需要乘以1000
	uint16_t NB_Voltage;		 //0x49-7.3V  上传给mes需要乘以1000
	uint8_t flash;
	uint8_t eeprom;
	uint8_t IC_test;
	
	uint8_t KEY_OPEN;//开盖检测
	uint8_t CSQ;//信号强度
	uint8_t LED;//LED检测
	uint16_t LED_Voltage;//光敏二极管端电压检测
	uint8_t PT;//温压检测
	uint8_t Alarm119;//119检测
	uint8_t lanya;//星闪（蓝牙）检测
};

/*————————测试流程————————*/
enum Test1
{
	t_wait,
	t_start,
	t_close_screen_led,
	t_get_7V2_voltage,
	t_get_3V6_voltage,
	t_get_7V2_current,
	t_get_3V6_current,
	t_start_shanggao,
	t_simulate_shanggao,
	t_simulate_umu,
	t_IC_test,
	t_LED_test,
	t_get_motor_Power,
	t_E20A,
	t_motor_test,
	t_open_xingshan,
	t_end
};

/*————————协议交互成功标志位————————*/
enum Test2
{
	NO_receive,
	r_E20A,
	r_E20B,
	r_1005,
	r_A027,
	r_simulate_shanggao,
};

#endif
