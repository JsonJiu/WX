#ifndef __TEST__H
#define __TEST__H
#include "main.h"

void Test_Init(void);
void Test_Start(void);
void Test_Function(void);
void Moter_Test(void);
void LED_Test(void);
void UMU_Timing_Send(void);
/*————————测试结果记录————————*/
struct Test
{
	uint8_t Find_Enable;	   // 上位查询使能标志位
	uint8_t Agreement_receive; // 协议交互标志

	uint8_t Workstation;	  // 工位
	uint32_t Voltage_zhudian; // 主电压检测
	uint32_t Current_zhudian; // 主电流检测
	uint32_t Voltage_beidian; // 备电压检测
	uint32_t Current_beidian; // 备电流检测
	uint8_t UMU;			  // UMU状态
	uint8_t UMU_EXTI;		  // UMU_Wake检测
	uint8_t LED;			  // LED状态
	uint8_t Motor_test;		  // 阀门测试状态
	uint8_t Motor_R_flag;
	uint8_t Motor_B_flag;
	uint8_t Infrared;		  // 红外
	uint8_t CAPStatus;		  // 通信板状态
	uint8_t E20A[45];		  // E20A转运号
	uint8_t IMEI[15];		  // IMEI号
	uint8_t IMSI[15];		  // IMSI号
	uint8_t ICCID[20];		  // ICCID号
	uint8_t SOFT_Version[2];  // 软件版本号
	uint8_t Comp_Time[6];	  // 编译时间
	uint8_t Pressure;		  // 压力状态
	uint8_t PlatePressure[4]; // 板载压力状态
	uint8_t ESAMID[8];		  // ESAMID
	uint8_t BD_Diode;		  // 备电二极管
};

/*————————测试流程————————*/
enum Test1
{
	t_wait = 0,
	t_start,
	t_get_gongzhuang_voltage,
	t_get_zhudian_current,
	t_get_beidian_current,
	t_simulate_umu,
	t_motor_test,
	t_start_shanggao,
	t_jiance_shanggao,
	t_read_IMEI_IMSI_ICCID,
	t_get_version,
	t_get_esamid,
	t_end
};

/*————————协议交互成功标志位————————*/
enum Test2
{
	r_umu = 0,
	r_E20A,
	r_1000,
	r_C525,
	r_C518,
	r_C519,
	NO_receive
};

#endif
