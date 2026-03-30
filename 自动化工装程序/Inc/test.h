#ifndef __TEST_H__
#define __TEST_H__
#include "main.h"
void Test_Init(void);
void startTestInit(void);
void getLrdEUIInit(void);
void Test_Function_Select(void);
void Test_Function_NB(void);
void Test_Function_CAT1(void);
void Test_Function_Mensi(void);
void Test_Function_Lierda(void);
void Test_Function_None(void);
void Test_Function_Pulse(void);
void Moter_Test(void);

/*模组类型宏定义*/
#define NB 0x00				// NB模组
#define CAT1 0x01			// cat1模组
#define mensi_lora 0x02		// 门思lora模组
#define lierda_lora 0x03	// 利尔达lora模组
#define none 0x04			// 基表
#define pulse 0x05			// 三线脉冲
#define lierda_lorawan 0x06 // 利尔达lorawan模组
#define lierda_wmbus 0x07	// 利尔达wmbus模组

/*三线脉冲方向宏定义*/
#define forward 0x01
#define back 0x02

struct Test
{
	uint8_t Find_Enable; // 上位查询使能标志位

	/*测试结果记录*/
	uint8_t Workstation;			   // 工位
	uint16_t Voltage_Main_Mould;	   // 主电供电电压
	uint16_t Voltage_Main_Protocol;	   // 主电协议获取电压
	uint16_t Current_Main_Static;	   // 静态功耗
	uint16_t Current_Full_Water;	   // 满水功耗
	uint16_t Current_Flow_Water;	   // 走水功耗
	uint16_t Voltage_Standby_Mould;	   // 备电/NB/扩频供电电压
	uint16_t Voltage_Standby_Protocol; // 备电/NB/扩频协议获取电压
	uint16_t Current_Standby;		   // 备电/NB/扩频功耗
	uint8_t Bluetooth;				   // 蓝牙检测
	uint8_t flash;					   // flash检测
	uint8_t Measurement;			   // 计量
	uint8_t Infrared;				   // 红外通信
	uint8_t IMEI[15];				   // IMEI号
	uint8_t IMSI[15];				   // IMSI号
	uint8_t ICCID[20];				   // ICCID号
	uint8_t CSQ;					   // 信号强度
	uint8_t Valve;					   // 阀门
	uint8_t Valve_Position;			   // 阀门到位
	uint8_t EEPROM;					   // EEPROM
	uint16_t GP30_Voltage;			   // GP30电压
	uint8_t LoraEUI[16];			   // LoraEUI号
	uint8_t Strong_magnet;			   // 强磁检测
	uint8_t Open_Cap;				   // 开盖检测
	uint8_t GPS_Module;				   // GPS模组状态
	uint8_t non_magnet_Module_sign;	   // 无磁模组信号强度
	uint8_t Check_Code[2];			   // 校验码
	uint8_t version[2];				   // 程序版本号
	uint8_t T;						   // 水温检测
	uint8_t P;						   // 压力检测
	uint8_t Lorarssi[2];			   // lora模组信号强度
	uint8_t Loraisnr[2];			   // lora模组信噪比
	uint8_t wake;					   // 唤醒检测
	uint8_t Pulse_Output;			   // 脉冲输出检测
	uint8_t Interrupt;				   // 中断检测
	uint8_t Ext_volatge;			   // 外电检测
	uint8_t RS485;					   // 485检测
	uint8_t Three_Pulse;			   // 三线脉冲检测
	uint8_t Wmbus;					   // wmbus检测

	/*额外需要传递的参数*/
	uint8_t Strong_magnet_1;	// 第一次强磁检测电平
	uint8_t Strong_magnet_2;	// 第二次强磁检测电平
	uint8_t Open_Cap_1;			// 第一次开盖电平
	uint8_t Open_Cap_2;			// 第二次开盖电平
	uint32_t cumulant1_int;		// 累积量第一次存储整数部分
	uint32_t cumulant2_int;		// 累积量第二次存储整数部分
	uint32_t cumulant1_decimal; // 累积量第一次存储小数部分
	uint32_t cumulant2_decimal; // 累积量第二次存储小数部分
	uint8_t gp30_state;			// GP30状态
	uint8_t cumulant_Data[8];	// 协议获取到的累积量数据
	uint8_t commumication;		// 协议获取到的上告成功标志位
	uint8_t self_check_flag;	// 自检结果标志位
	uint8_t appkey[32];			// APPKEY值
	uint8_t appeui[16];			// APPEUI
	uint16_t frequence;			// 频点
	uint8_t freqInfo[8];		// 频点信息
	uint8_t lierda_module_type; // 利尔达模组类型

	/*0x17指令暂时先不用的参数*/
	uint8_t flow_rate[4];			   // 瞬时流速
	uint8_t flow_state;				   // 走水状态
	uint8_t reach_position_1;		   // 到位1
	uint8_t reach_position_2;		   // 到位2
	uint8_t measurement_hall_1;		   // 计量霍尔1
	uint8_t measurement_hall_2;		   // 计量霍尔2
	uint8_t non_magnetic_measure_rate; // 无磁计量速度
	uint8_t lora_result;			   // lora上告结果

	/*上位传递的参数*/
	uint8_t module_type; // 模组类型
};

/*————————测试流程————————*/
enum Test1
{
	t_wait,
	t_start,
	t_get_main_voltage,
	t_change_empty_water,
	t_get_static_current,
	t_change_full_water,
	t_get_full_water_current,
	t_change_flow_water,
	t_get_flow_water_current,
	t_get_Standby_voltage,
	t_get_Standby_current,
	t_write_APPKEY,
	t_write_APPEUI,
	t_lierda_module_select,
	t_config_wmbus,
	t_get_wmbus_state,
	t_set_wmbus_key,
	t_write_save,
	t_write_reset,
	t_restart,
	t_simulate_pulse_forward,
	t_simulate_pulse_forward_test,
	t_simulate_pulse_back,
	t_simulate_pulse_back_test,
	t_start_Wmbus_test,
	t_find_result_first,
	t_find_device_ID,
	t_set_true,
	t_find_check_code,
	t_get_GP30_voltage,
	t_mode_select,
	t_close_commumication,
	t_manual_report_UP,
	t_find_report,
	t_find_EUI,
	t_set_frequence,
	t_get_frequence,
	t_self_check,
	t_Measurement_test,
	t_open_magnet,
	t_find_result_second,
	t_end
};

/*————————协议交互成功标志位————————*/
enum Test2
{
	NO_receive,
	r_empty_water,
	r_full_water,
	r_flow_water,
	r_Wmbus_test,
	r_get_lerda_module_type,
	r_config_wmbus,
	r_get_wmbus_state,
	r_set_wmbus_key,
	r_write_module_param,
	r_find_device_ID,
	r_set_true,
	r_find_check_code,
	r_find_result,
	r_close_commumication,
	r_get_report,
	r_find_report,
	r_find_report_ERR,
	r_self_check,
	r_get_GP30_voltage,
	r_get_EUI,
	r_get_LrdEUI,
	r_get_frequence,
	r_simulate_pulse,
};

#endif
