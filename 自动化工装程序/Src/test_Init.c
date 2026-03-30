#include "adc.h"
#include "fm33lg0xx_fl_gpio.h"
#include "gpio.h"
#include "ina219.h"
#include "ir.h"
#include "led.h"
#include "measurement.h"
#include "test.h"

/*————————结构体枚举定义————————*/
struct Test Test_result;
enum Test1 Test_process;
enum Test2 Test_agreement_receive;

/*————————定时器全局变量计时————————*/
uint32_t single_step_timing = 0; // 单步测试时间
uint32_t interval_timing = 0;    // 间隔测试时间

uint8_t repeat_report = 0; // 重复上告计数

/*————————工位检测————————*/
void Workstation_CHK(void)
{
//  if (FL_GPIO_GetInputPin(GPIOC, FL_GPIO_PIN_0) == 0 &&
//      FL_GPIO_GetInputPin(GPIOD, FL_GPIO_PIN_12) == 0 &&
//      FL_GPIO_GetInputPin(GPIOB, FL_GPIO_PIN_15) == 0)
//  {
//    Test_result.Workstation = 0x00;
//  }
//  else if (FL_GPIO_GetInputPin(GPIOC, FL_GPIO_PIN_0) == 0 &&
//           FL_GPIO_GetInputPin(GPIOD, FL_GPIO_PIN_12) == 0 &&
//           FL_GPIO_GetInputPin(GPIOB, FL_GPIO_PIN_15) == 1)
//  {
//    Test_result.Workstation = 0x01;
//  }
//  else if (FL_GPIO_GetInputPin(GPIOC, FL_GPIO_PIN_0) == 0 &&
//           FL_GPIO_GetInputPin(GPIOD, FL_GPIO_PIN_12) == 1 &&
//           FL_GPIO_GetInputPin(GPIOB, FL_GPIO_PIN_15) == 0)
//  {
//    Test_result.Workstation = 0x02;
//  }
//  else if (FL_GPIO_GetInputPin(GPIOC, FL_GPIO_PIN_0) == 0 &&
//           FL_GPIO_GetInputPin(GPIOD, FL_GPIO_PIN_12) == 1 &&
//           FL_GPIO_GetInputPin(GPIOB, FL_GPIO_PIN_15) == 1)
//  {
//    Test_result.Workstation = 0x03;
//  }
//  else if (FL_GPIO_GetInputPin(GPIOC, FL_GPIO_PIN_0) == 1 &&
//           FL_GPIO_GetInputPin(GPIOD, FL_GPIO_PIN_12) == 0 &&
//           FL_GPIO_GetInputPin(GPIOB, FL_GPIO_PIN_15) == 0)
//  {
//    Test_result.Workstation = 0x04;
//  }
//  else if (FL_GPIO_GetInputPin(GPIOC, FL_GPIO_PIN_0) == 1 &&
//           FL_GPIO_GetInputPin(GPIOD, FL_GPIO_PIN_12) == 0 &&
//           FL_GPIO_GetInputPin(GPIOB, FL_GPIO_PIN_15) == 1)
//  {
//    Test_result.Workstation = 0x05;
//  }
  Test_result.Workstation = 0x03;
}

/*————————测试初始化————————*/
/*
1.工位检测
2.主电供电打开
3.备电供电打开
4.主电电流检测关闭
5.备电电流检测关闭
6.电磁铁1供电关闭
7.电磁铁2供电关闭
8.测试流程为等待
9.初始化完成LED闪烁，配合main函数
*/
void Test_Init(void)
{
  Workstation_CHK();
  Main_Voltage_Output_Ctl(On);
  Standby_Voltage_Output_Ctl(On);
  Main_Current_CHK_CTL(Off);
  Standby_Current_CHK_CTL(Off);
  Output_12V_CTL1(Off);
  Output_12V_CTL2(Off);
  Test_process = t_wait;
  LED_Sign_Open();
  interval_timing = 0;
  single_step_timing = 0;
}

/**
 * @brief
 *
 */
void Test_Result_Init(void)
{
  Test_result.Find_Enable = 0;
  Test_agreement_receive = NO_receive;
  Test_result.Voltage_Main_Mould = 0;
  Test_result.Voltage_Main_Protocol = 0;
  Test_result.Current_Main_Static = 0;
  Test_result.Current_Full_Water = 0;
  Test_result.Current_Flow_Water = 0;
  Test_result.Voltage_Standby_Mould = 0;
  Test_result.Voltage_Standby_Protocol = 0;
  Test_result.Current_Standby = 0;
  Test_result.Bluetooth = 0;
  Test_result.flash = 0;
  Test_result.Measurement = 0;
  Test_result.Infrared = 0;
  memset(Test_result.IMEI, 0x00, 15);
  memset(Test_result.IMSI, 0x00, 15);
  memset(Test_result.ICCID, 0x00, 20);
  Test_result.CSQ = 0;
  Test_result.Valve = 0;
  Test_result.Valve_Position = 0;
  Test_result.EEPROM = 0;
  Test_result.GP30_Voltage = 0;
  memset(Test_result.LoraEUI, 0x00, 16);
  Test_result.Strong_magnet = 0;
  Test_result.Strong_magnet_1 = 0;
  Test_result.Strong_magnet_2 = 0;
  Test_result.Open_Cap = 0;
  Test_result.Open_Cap_1 = 0;
  Test_result.Open_Cap_2 = 0;
  Test_result.GPS_Module = 0;
  Test_result.non_magnet_Module_sign = 0;
  memset(Test_result.Check_Code, 0x00, 2);
  memset(Test_result.version, 0x00, 2);
  Test_result.T = 0;
  Test_result.P = 0;
  memset(Test_result.Lorarssi, 0x00, 2);
  memset(Test_result.Loraisnr, 0x00, 2);
  Test_result.wake = 0;
  Test_result.Pulse_Output = 0;
  Test_result.Interrupt = 0;
  Test_result.Ext_volatge = 0;
  Test_result.RS485 = 0;
  Test_result.Three_Pulse = 0;
  Test_result.Wmbus = 0;
  Test_agreement_receive = NO_receive;

  Test_result.cumulant1_int = 0;
  Test_result.cumulant2_int = 0;
  Test_result.cumulant1_decimal = 0;
  Test_result.cumulant2_decimal = 0;
  Test_result.gp30_state = 0;
  memset(Test_result.cumulant_Data, 0x00, 8);
  Test_result.commumication = 0;
  Test_result.self_check_flag = 0;
  // memset(Test_result.appkey, 0x00, 32);
  // memset(Test_result.appeui, 0x00, 16);
  // Test_result.frequence = 0;
  Test_result.lierda_module_type = 0;

  memset(Test_result.flow_rate, 0x00, 4);
  Test_result.flow_state = 0;
  Test_result.reach_position_2 = 0;
  Test_result.measurement_hall_1 = 0;
  Test_result.measurement_hall_2 = 0;
  Test_result.non_magnetic_measure_rate = 0;
  Test_result.lora_result = 0;

  // Test_result.module_type = 0;
  repeat_report = 0;
}

/*————————开始测试————————*/
void startTestInit(void)
{
  Test_Init();
  Test_Result_Init();
  Test_process = t_start;
}

/*————————Lierda查询EUI初始化————————*/
void getLrdEUIInit(void)
{
  Test_Init();
  Test_Result_Init();
  single_step_timing = 10000;
  Test_process = t_find_EUI;
}

/*————————测试流程选择————————*/
void Test_Function_Select(void)
{
  switch (Test_result.module_type)
  {
  case NB:
    Test_Function_NB();
    break;
  case CAT1:
    Test_Function_CAT1();
    break;
  case mensi_lora:
    Test_Function_Mensi();
    break;
  case lierda_lora:
    Test_Function_Lierda();
    break;
  case none:
    Test_Function_Mensi(); // 基表和门思测试逻辑一致，只是少了通讯部分
    break;
  case pulse:
    Test_Function_Pulse();
    break;
  default:
    break;
  }
}
