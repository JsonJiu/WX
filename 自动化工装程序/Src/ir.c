#include "ir.h"

#include "gpio.h"
#include "jiaoyan.h"
#include "test.h"
#include "uart0.h"
#include "uart1.h"

uint8_t device_ID[8] = {0};
uint8_t start_Wmbus_test_xieyi[9] = {0x68, 0x09, 0x00, 0x5A, 0x9F, 0x55, 0x01, 0xC0, 0x16};
uint8_t set_true_xieyi[9] = {0x68, 0x09, 0x00, 0x5A, 0x33, 0x55, 0x00, 0x53, 0x16};
uint8_t get_version_xieyi[6] = {0x68, 0x06, 0x00, 0x7B, 0xE9, 0x16};
uint8_t find_device_ID_xieyi[6] = {0x68, 0x06, 0x00, 0x7A, 0xE8, 0x16};
uint8_t find_version_xieyi[6] = {0x68, 0x06, 0x00, 0x7B, 0xE9, 0x16};
uint8_t find_result_xieyi[6] = {0x68, 0x06, 0x00, 0x17, 0x85, 0x16};
uint8_t get_gp30_voltage_xieyi[6] = {0x68, 0x06, 0x00, 0x8A, 0xF8, 0x16};
uint8_t report_up_xieyi[15] = {0x68, 0xAA, 0x0F, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x5A, 0xE0, 0x5B, 0x16};
uint8_t report_result_xieyi[15] = {0x68, 0xAA, 0x0F, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x7A, 0xE0, 0xFB, 0x16};
uint8_t self_check_xieyi[15] = {0x68, 0xAA, 0x0F, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x7A, 0xFA, 0x15, 0x16};
uint8_t close_check_xieyi[14] = {0x68, 0x0E, 0x00, 0x5A, 0x07, 0x55, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2C, 0x16};
uint8_t EUI_xieyi[18] = {0x68, 0xFF, 0xEE, 0xDD, 0x55, 0x00, 0x09, 0xFF, 0xAA, 0x81, 0x02, 0x00, 0x59, 0x45, 0xCA, 0x40, 0x64, 0x16};
uint8_t pulse_xieyi[10] = {0x68, 0x0A, 0x00, 0x5A, 0x41, 0x55, 0x02, 0x05, 0x69, 0x16};
uint8_t lierda_EUI_xieyi[7] = {0x68, 0x07, 0x00, 0xC6, 0x01, 0x36, 0x16};
uint8_t find_Lrd_type_xieyi[17] = {0x68, 0xFF, 0xEE, 0xDD, 0x55, 0x00, 0x08, 0x41, 0x54, 0x2B, 0x56, 0x45, 0x52, 0x3F, 0x0D, 0x88, 0x16};
uint8_t config_wmbus_xieyi[15] = {0x68, 0xAA, 0x0F, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x5A, 0xE2, 0xDD, 0x16};
uint8_t find_Wmbus_config_xieyi[15] = {0x68, 0xAA, 0x0F, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x30, 0x7A, 0xE2, 0xFD, 0x16};
uint8_t set_wmbus_key_xieyi[41] = {0x68, 0x29, 0x00, 0x5A, 0x67, 0x55, 0x01, 0x32, 0x42, 0x37, 0x45, 0x31, 0x35, 0x31, 0x36, 0x32, 0x38, 0x41, 0x45, 0x44, 0x32, 0x41, 0x36, 0x41, 0x42, 0x46, 0x37, 0x31, 0x35, 0x38, 0x38, 0x30, 0x39, 0x43, 0x46, 0x34, 0x46, 0x33, 0x43, 0xFA, 0x16};

uint8_t white_module_param_xieyi[200];

extern uint8_t ASCII_EUI[8];
extern uint8_t ASCII_16Byte[16];

#include "ir.h"
#include "jiaoyan.h"
#include "test.h"
#include "uart1.h"

extern struct Test Test_result;
extern enum Test2 Test_agreement_receive;
extern enum Test1 Test_process;
extern uint32_t interval_timing;
extern uint32_t single_step_timing;

/*————————前导码发送————————*/
uint8_t Part_Qiandao_FE[10] = {0xFE, 0xFE, 0xFE, 0xFE, 0xFE,
                               0xFE, 0xFE, 0xFE, 0xFE, 0xFE};
void Send_Qiandao(void)
{
  uint8_t count = 0;
  for (count = 0; count < 120; count++)
  {
    Uart1_Tx_Send(Part_Qiandao_FE, 10);
  }
}

/*————————红外数据接收解析————————*/
void irProtocolParse(uint8_t *Data, uint32_t lenth)
{
  uint8_t pHead = 0;
  uint8_t Data_lenth = 0;
  uint8_t sum_jiaoyan = 0;
  uint8_t sum_jiaoyan_jisuan = 0;
  uint8_t IR_analysis_Data[200] = {0};
  uint8_t Data_sign = 0;
  uint16_t CRC_jiaoyan = 0;
  uint16_t CRC_jiaoyan_jisuan = 0;
  while (1)
  {
    if (pHead + 6 > lenth)
      break;
    if (Data[pHead] == 0x68)
    {
      Data_lenth = Data[pHead + 1];                                   // 取数据中的帧长度数据
      sum_jiaoyan = Data[pHead + Data_lenth - 2];                     // 取数据中的和校验一字节数据
      memcpy(IR_analysis_Data, &Data[pHead], Data_lenth);             // 将数据中有用的完整的这一串数据转存
      sum_jiaoyan_jisuan = Sum_jiaoyan(IR_analysis_Data, Data_lenth); // 将转存的数据进行和校验
      if (sum_jiaoyan == sum_jiaoyan_jisuan)
      {
        Test_result.Infrared = 1;
        Data_sign = Data[pHead + 3];
        switch (Data_sign)
        {
        case 0xEA:
          interval_timing = 0;
          memcpy(device_ID, &Data[pHead + 4], 8);
          memcpy(Test_result.cumulant_Data, &Data[pHead + 20], 8);
          Test_agreement_receive = r_find_device_ID;
          break;
        case 0x7B:
          interval_timing = 0;
          Test_result.Check_Code[0] = Data[pHead + 6];
          Test_result.Check_Code[1] = Data[pHead + 5];
          Test_agreement_receive = r_find_check_code;
          break;
        case 0x8A:
          interval_timing = 0;
          Test_result.GP30_Voltage = Data[pHead + 4] & 0xFF;
          Test_result.GP30_Voltage = (Test_result.GP30_Voltage << 8) + (Data[pHead + 5] & 0xFF);
          Test_agreement_receive = r_get_GP30_voltage;
          break;
        case 0x17: // 协议不一致导致需要分开独立解析
          if (Test_result.module_type == NB)
          {
            // flash检测
            Test_result.flash = Data[pHead + 4];
            // 主电协议获取电压
            Test_result.Voltage_Main_Protocol = Data[pHead + 6];
            Test_result.Voltage_Main_Protocol = Test_result.Voltage_Main_Protocol << 8;
            Test_result.Voltage_Main_Protocol += Data[pHead + 5];
            // 备电/NB/扩频协议获取电压
            Test_result.Voltage_Standby_Protocol = Data[pHead + 8];
            Test_result.Voltage_Standby_Protocol = Test_result.Voltage_Standby_Protocol << 8;
            Test_result.Voltage_Standby_Protocol += Data[pHead + 7];
            // 温压传感器检测
            Test_result.T = Data[pHead + 9];
            Test_result.P = Test_result.T;
            // EEPROM状态读取
            Test_result.EEPROM = Data[pHead + 10];
            // 模组上告检测
            Test_result.GPS_Module = Data[pHead + 12];
            if (Test_process == t_find_result_first)
            {
              // 强磁检测
              Test_result.Strong_magnet_1 = Data[pHead + 11];
              // 开盖检测
              Test_result.Open_Cap_1 = Data[pHead + 13];
            }
            if (Test_process == t_find_result_second)
            {
              Test_result.Strong_magnet_2 = Data[pHead + 11];
              Test_result.Open_Cap_2 = Data[pHead + 13];
              if (Test_result.Strong_magnet_1 == 1 && Test_result.Strong_magnet_2 == 0)
                Test_result.Strong_magnet = 1;
              if (Test_result.Strong_magnet_1 == 1 && Test_result.Strong_magnet_2 == 1)
                Test_result.Strong_magnet = 0;
              if (Test_result.Open_Cap_1 == 1 && Test_result.Open_Cap_2 == 0)
                Test_result.Open_Cap = 1;
              if (Test_result.Open_Cap_1 == 1 && Test_result.Open_Cap_2 == 1)
                Test_result.Open_Cap = 0;
            }
            // 超声瞬时流量读取
            memcpy(Test_result.flow_rate, &Data[pHead + 14], 4);
            // 走水状态读取
            Test_result.flow_state = Data[pHead + 18];
            // GP30模组AD采样检测
            Test_result.GP30_Voltage = Data[pHead + 20];
            Test_result.GP30_Voltage = Test_result.GP30_Voltage << 8;
            Test_result.GP30_Voltage += Data[pHead + 19];
            // IMEI号获取
            memcpy(Test_result.IMEI, &Data[pHead + 21], 15);
            // IMSI号获取
            memcpy(Test_result.IMSI, &Data[pHead + 36], 15);
            // ICCID号获取
            memcpy(Test_result.ICCID, &Data[pHead + 51], 20);
            // CSQ获取
            Test_result.CSQ = Data[pHead + 71]; // 表具传回来的CSQ有两个字节，这里获取低位字节
            // loraEUI
            memcpy(Test_result.LoraEUI, &Data[pHead + 73], 16);
            // 到位1信号
            Test_result.reach_position_1 = Data[pHead + 89];
            // 到位2信号
            Test_result.reach_position_2 = Data[pHead + 90];
            // 计量霍尔1
            Test_result.measurement_hall_1 = Data[pHead + 91];
            // 计量霍尔2
            Test_result.measurement_hall_2 = Data[pHead + 92];
            // 无磁计量流速
            Test_result.non_magnetic_measure_rate = Data[pHead + 93];
            // GP30状态
            Test_result.gp30_state = Data[pHead + 94];
            // lora上告结果
            Test_result.lora_result = Data[pHead + 95];
            // Wmbus自检
            Test_result.Wmbus = Data[pHead + 96];
            if (Test_result.GPS_Module == 1 || Test_process == t_find_result_first)
            {
              interval_timing = 0;
              Test_agreement_receive = r_find_result;
            }
          }
          if (Test_result.module_type == lierda_lora)
          {
            // flash检测
            Test_result.flash = Data[pHead + 4];
            // 主电协议获取电压
            Test_result.Voltage_Main_Protocol = Data[pHead + 5];
            Test_result.Voltage_Main_Protocol = Test_result.Voltage_Main_Protocol << 8;
            Test_result.Voltage_Main_Protocol += Data[pHead + 6];
            // 备电/NB/扩频协议获取电压
            Test_result.Voltage_Standby_Protocol = Data[pHead + 7];
            Test_result.Voltage_Standby_Protocol = Test_result.Voltage_Standby_Protocol << 8;
            Test_result.Voltage_Standby_Protocol += Data[pHead + 8];
            // EEPROM状态读取
            Test_result.EEPROM = Data[pHead + 10];
            // 模组上告检测
            Test_result.GPS_Module = Data[pHead + 12];
            // 超声瞬时流量读取
            memcpy(Test_result.flow_rate, &Data[pHead + 13], 4);
            // 走水状态读取
            Test_result.flow_state = Data[pHead + 17];
            // GP30模组AD采样检测
            Test_result.GP30_Voltage = Test_result.Voltage_Main_Protocol;
            // loraEUI
            memcpy(Test_result.LoraEUI, &Data[pHead + 70], 16);
            // 校验码
            Test_result.Check_Code[0] = Data[pHead + 92];
            Test_result.Check_Code[1] = Data[pHead + 91];
            // lorarssi
            Test_result.Lorarssi[0] = Data[pHead + 94];
            Test_result.Lorarssi[1] = Data[pHead + 93];
            // loraisnr
            Test_result.Loraisnr[0] = Data[pHead + 96];
            Test_result.Loraisnr[1] = Data[pHead + 95];
            interval_timing = 0;
            Test_agreement_receive = r_find_result;
          }
          break;
        case 0x5A:
          switch (Data[pHead + 4])
          {
          case 0x33:
            interval_timing = 0;
            Test_agreement_receive = r_set_true;
            break;
          case 0x9f:
            interval_timing = 0;
            Test_agreement_receive = r_Wmbus_test;
            break;
          case 0x07:
            interval_timing = 0;
            Test_agreement_receive = r_close_commumication;
            break;
          case 0x41:
            interval_timing = 0;
            Test_agreement_receive = r_simulate_pulse;
            break;
          case 0x5F:
            interval_timing = 0;
            if (Data[pHead + 6] == 0x01)
            {
              Test_agreement_receive = r_write_module_param;
            }
            else
            {
              Test_agreement_receive = NO_receive;
            }
            break;
          case 0x60:
            interval_timing = 0;
            Test_agreement_receive = r_set_wmbus_key;
            break;
          default:
            break;
          }
          break;
        case 0xC6:
          switch (Data[pHead + 4])
          {
          case 0x01:
            interval_timing = 0;
            memcpy(Test_result.LoraEUI, &Data[pHead + 5], 16);
            Test_agreement_receive = r_get_LrdEUI;
            break;
          case 0x08:
            interval_timing = 0;
            memcpy(Test_result.freqInfo, &Data[pHead + 5], 8);
            if (Test_result.freqInfo[0] == '9' &&
                Test_result.freqInfo[1] == '2' &&
                Test_result.freqInfo[2] == '3' &&
                Test_result.freqInfo[3] == '2' &&
                Test_result.freqInfo[4] == '9' &&
                Test_result.freqInfo[5] == '2' &&
                Test_result.freqInfo[6] == '3' &&
                Test_result.freqInfo[7] == '4')
            {
              Test_agreement_receive = r_get_frequence;
            }
            else
              Test_agreement_receive = NO_receive;
            break;
          default:
            break;
          }
        default:
          break;
        }
        pHead += Data_lenth;
      }
    }
    if (Data[pHead] == 0x68)
    {
      Data_lenth = 0;
      Data_lenth = Data[pHead + 2];                                   // 取数据中的帧长度数据
      sum_jiaoyan = Data[pHead + Data_lenth - 2];                     // 取数据中的和校验一字节数据
      memcpy(IR_analysis_Data, &Data[pHead], Data_lenth);             // 将数据中有用的完整的这一串数据转存
      sum_jiaoyan_jisuan = Sum_jiaoyan(IR_analysis_Data, Data_lenth); // 将转存的数据进行和校验
      if (sum_jiaoyan == sum_jiaoyan_jisuan)
      {
        Test_result.Infrared = 1;
        Data_sign = Data[pHead + 11];
        switch (Data_sign)
        {
        case 0x5A:
          switch (Data[pHead + 12])
          {
          case 0xE0:
            interval_timing = 0;
            Test_agreement_receive = r_get_report;
            break;
          case 0xE2:
            interval_timing = 0;
            Test_agreement_receive = r_config_wmbus;
            break;
          default:
            break;
          }
          break;
        case 0x7A:
          switch (Data[pHead + 12])
          {
          case 0xE0:
            interval_timing = 0;
            Test_result.commumication = Data[pHead + 13];
            if (Test_result.commumication == 0x01)
            {
              Test_agreement_receive = r_find_report;
            }
            else if (Test_result.commumication == 0x00)
            {
              Test_agreement_receive = r_find_report_ERR;
            }
            break;
          case 0xE2:
            interval_timing = 0;
            if (Data[pHead + 13] == 0x01)
            {
              Test_agreement_receive = r_get_wmbus_state;
            }
            break;
          case 0xFA:
            interval_timing = 0;
            Test_result.self_check_flag = Data[pHead + 14];
            Test_agreement_receive = r_self_check;
            break;
          default:
            break;
          }
          break;
        default:
          break;
        }
        pHead += Data_lenth;
      }
    }
    if (Data[pHead] == 0x2B && Data[pHead + 1] == 0x56 && Data[pHead + 2] == 0x45 && Data[pHead + 3] == 0x52 && Data[pHead + 4] == 0x3A)
    {
      if (Data[pHead + 6] == 0x77)
        Test_result.lierda_module_type = lierda_wmbus;
      if (Data[pHead + 6] == 0x4C)
        Test_result.lierda_module_type = lierda_lorawan;
      Test_agreement_receive = r_get_lerda_module_type;
      pHead += 72;
    }
    if (Data[pHead] == 0xFF && Data[pHead + 1] == 0xAA)
    {
      Data_lenth = 0;
      Data_lenth = Data[pHead + 3] + 7; // 取数据中的帧长度数据
      CRC_jiaoyan = Data[pHead + Data_lenth - 2];
      CRC_jiaoyan = (CRC_jiaoyan << 8) + Data[pHead + Data_lenth - 3];
      memcpy(IR_analysis_Data, &Data[pHead], Data_lenth);      // 将数据中有用的完整的这一串数据转存
      CRC_jiaoyan_jisuan = CRC16_MODBUS(Data, Data_lenth - 3); // 将转存的数据进行CRC校验
      if (CRC_jiaoyan_jisuan == CRC_jiaoyan)
      {
        Test_result.Infrared = 1;
        Data_sign = Data[pHead + 2];
        switch (Data_sign)
        {
        case 0x21:
          interval_timing = 0;
          memcpy(ASCII_EUI, &Data[pHead + 9], 8);
          ASCII8byte_To_ASCII16byte(ASCII_EUI);
          ASCII_To_Hex(ASCII_16Byte, 16);
          Test_agreement_receive = r_get_EUI;
          break;
        default:
          break;
        }
        pHead += Data_lenth;
      }
    }
    pHead++;
  }
}

/*查询表号和累积量*/
void Find_Device_ID(void)
{
  Send_Qiandao();
  Uart1_Tx_Send(find_device_ID_xieyi, 6);
}

/*开始Wmbus检测*/
void Start_Wmbus_Test(void)
{
  Send_Qiandao();
  Uart1_Tx_Send(start_Wmbus_test_xieyi, 9);
}

/*设置真值*/
void Set_true(void)
{
  Send_Qiandao();
  Uart1_Tx_Send(set_true_xieyi, 9);
}

/*查询版本号和校验码*/
void Find_Version(void)
{
  Send_Qiandao();
  Uart1_Tx_Send(find_version_xieyi, 6);
}

/*0x17协议查询结果*/
void Find_Result(void)
{
  Send_Qiandao();
  Uart1_Tx_Send(find_result_xieyi, 6);
}

/*得到GP30电压*/
void Get_GP30_Voltage(void)
{
  Send_Qiandao();
  Uart1_Tx_Send(get_gp30_voltage_xieyi, 6);
}

/*主动上告*/
void Manual_Report_UP(void)
{
  memcpy(&report_up_xieyi[3], &device_ID[0], 8);
  report_up_xieyi[13] = Sum_jiaoyan(report_up_xieyi, 15);
  Send_Qiandao();
  Uart1_Tx_Send(report_up_xieyi, 15);
}

/*查询上告是否成功,不适用于利尔达*/
void Find_Report_Result(void)
{
  memcpy(&report_result_xieyi[3], &device_ID[0], 8);
  report_result_xieyi[13] = Sum_jiaoyan(report_result_xieyi, 15);
  Send_Qiandao();
  Uart1_Tx_Send(report_result_xieyi, 15);
}

/*查询自检结果*/
void Find_Self_Check(void)
{
  memcpy(&self_check_xieyi[3], &device_ID[0], 8);
  self_check_xieyi[13] = Sum_jiaoyan(self_check_xieyi, 15);
  Send_Qiandao();
  Uart1_Tx_Send(self_check_xieyi, 15);
}

/*关闭通讯*/
void Close_Commumication(void)
{
  Send_Qiandao();
  Uart1_Tx_Send(close_check_xieyi, 14);
}

/*查询LoraEUI*/
void Find_LoraEUI(void)
{
  Send_Qiandao();
  Uart1_Tx_Send(EUI_xieyi, 18);
}

/*模拟三线脉冲*/
void simulate3WirePulse(uint8_t Dir)
{
  if (Dir == forward)
  {
    pulse_xieyi[6] = 0x01;
    pulse_xieyi[8] = Sum_jiaoyan(pulse_xieyi, 8);
    Send_Qiandao();
    Uart1_Tx_Send(pulse_xieyi, 10);
  }
  else if (Dir == back)
  {
    pulse_xieyi[6] = 0x02;
    pulse_xieyi[8] = Sum_jiaoyan(pulse_xieyi, 8);
    Send_Qiandao();
    Uart1_Tx_Send(pulse_xieyi, 10);
  }
}

/*利尔达loraEUI查询*/
void getLrdLoRaEUI(void)
{
  Send_Qiandao();
  Uart1_Tx_Send(lierda_EUI_xieyi, 7);
}

/*查询利尔达模组的类型*/
void findLrdType(void)
{
  Send_Qiandao();
  Uart1_Tx_Send(find_Lrd_type_xieyi, 17);
}

/*启动Wmbus发送诊断*/
void configWmbus(void)
{
  memcpy(&config_wmbus_xieyi[3], &device_ID[0], 8);
  config_wmbus_xieyi[13] = Sum_jiaoyan(config_wmbus_xieyi, 15);
  Send_Qiandao();
  Uart1_Tx_Send(config_wmbus_xieyi, 15);
}

/*读取Wmbus发送诊断信息*/
void findWmbusConfig(void)
{
  memcpy(&find_Wmbus_config_xieyi[3], &device_ID[0], 8);
  find_Wmbus_config_xieyi[13] = Sum_jiaoyan(find_Wmbus_config_xieyi, 15);
  Send_Qiandao();
  Uart1_Tx_Send(find_Wmbus_config_xieyi, 15);
}

/*设置wmbus密钥*/
void setWmbusKey(void)
{
  Send_Qiandao();
  Uart1_Tx_Send(set_wmbus_key_xieyi, 41);
}

/*利尔达APPKEY参数写入*/
void whiteAppkey(void)
{
  uint16_t lenth = 0;
  uint16_t sum_jiaoyan_lenth = 0;
  white_module_param_xieyi[lenth++] = 0x68;
  white_module_param_xieyi[lenth++] = 0x29;
  white_module_param_xieyi[lenth++] = 0x00;
  white_module_param_xieyi[lenth++] = 0x5A;
  white_module_param_xieyi[lenth++] = 0x5F;
  white_module_param_xieyi[lenth++] = 0x55;
  white_module_param_xieyi[lenth++] = 0x01;
  memcpy(&white_module_param_xieyi[lenth], Test_result.appkey, 32);
  lenth += 32;
  white_module_param_xieyi[lenth] = 0;
  for (sum_jiaoyan_lenth = 0; sum_jiaoyan_lenth < lenth; sum_jiaoyan_lenth++)
  {
    white_module_param_xieyi[lenth] += white_module_param_xieyi[sum_jiaoyan_lenth];
  }
  lenth++;
  white_module_param_xieyi[lenth++] = 0x16;
  Send_Qiandao();
  Uart1_Tx_Send(white_module_param_xieyi, lenth);
}

/*利尔达APPEUI参数写入*/
void whiteAppEUI(void)
{
  uint16_t lenth = 0;
  uint16_t sum_jiaoyan_lenth = 0;
  white_module_param_xieyi[lenth++] = 0x68;
  white_module_param_xieyi[lenth++] = 0x19;
  white_module_param_xieyi[lenth++] = 0x00;
  white_module_param_xieyi[lenth++] = 0x5A;
  white_module_param_xieyi[lenth++] = 0x5F;
  white_module_param_xieyi[lenth++] = 0x55;
  white_module_param_xieyi[lenth++] = 0x02;
  memcpy(&white_module_param_xieyi[lenth], Test_result.appeui, 16);
  lenth += 16;
  white_module_param_xieyi[lenth] = 0;
  for (sum_jiaoyan_lenth = 0; sum_jiaoyan_lenth < lenth; sum_jiaoyan_lenth++)
  {
    white_module_param_xieyi[lenth] += white_module_param_xieyi[sum_jiaoyan_lenth];
  }
  lenth++;
  white_module_param_xieyi[lenth++] = 0x16;
  Send_Qiandao();
  Uart1_Tx_Send(white_module_param_xieyi, lenth);
}

/*将数据存储*/
void whiteSave(void)
{
  uint16_t lenth = 0;
  uint16_t sum_jiaoyan_lenth = 0;
  white_module_param_xieyi[lenth++] = 0x68;
  white_module_param_xieyi[lenth++] = 0x09;
  white_module_param_xieyi[lenth++] = 0x00;
  white_module_param_xieyi[lenth++] = 0x5A;
  white_module_param_xieyi[lenth++] = 0x5F;
  white_module_param_xieyi[lenth++] = 0x55;
  white_module_param_xieyi[lenth++] = 0x03;
  white_module_param_xieyi[lenth] = 0;
  for (sum_jiaoyan_lenth = 0; sum_jiaoyan_lenth < lenth; sum_jiaoyan_lenth++)
  {
    white_module_param_xieyi[lenth] += white_module_param_xieyi[sum_jiaoyan_lenth];
  }
  lenth++;
  white_module_param_xieyi[lenth++] = 0x16;
  Send_Qiandao();
  Uart1_Tx_Send(white_module_param_xieyi, lenth);
}

/*将模组复位*/
void whiteReset(void)
{
  uint16_t lenth = 0;
  uint16_t sum_jiaoyan_lenth = 0;
  white_module_param_xieyi[lenth++] = 0x68;
  white_module_param_xieyi[lenth++] = 0x09;
  white_module_param_xieyi[lenth++] = 0x00;
  white_module_param_xieyi[lenth++] = 0x5A;
  white_module_param_xieyi[lenth++] = 0x5F;
  white_module_param_xieyi[lenth++] = 0x55;
  white_module_param_xieyi[lenth++] = 0x04;
  white_module_param_xieyi[lenth] = 0;
  for (sum_jiaoyan_lenth = 0; sum_jiaoyan_lenth < lenth; sum_jiaoyan_lenth++)
  {
    white_module_param_xieyi[lenth] += white_module_param_xieyi[sum_jiaoyan_lenth];
  }
  lenth++;
  white_module_param_xieyi[lenth++] = 0x16;
  Send_Qiandao();
  Uart1_Tx_Send(white_module_param_xieyi, lenth);
}

/*设置923频点*/
void loraSetFreq923(void)
{
  uint16_t lenth = 0;
  uint16_t sum_jiaoyan_lenth = 0;
  white_module_param_xieyi[lenth++] = 0x68;
  white_module_param_xieyi[lenth++] = 0x0A;
  white_module_param_xieyi[lenth++] = 0x00;
  white_module_param_xieyi[lenth++] = 0x5A;
  white_module_param_xieyi[lenth++] = 0x5F;
  white_module_param_xieyi[lenth++] = 0x55;
  white_module_param_xieyi[lenth++] = 0x09;
  // 频点具体频段，86代表868,93代表923频段
  white_module_param_xieyi[lenth++] = 0x93;
  white_module_param_xieyi[lenth] = 0;
  for (sum_jiaoyan_lenth = 0; sum_jiaoyan_lenth < lenth; sum_jiaoyan_lenth++)
  {
    white_module_param_xieyi[lenth] += white_module_param_xieyi[sum_jiaoyan_lenth];
  }
  lenth++;
  white_module_param_xieyi[lenth++] = 0x16;
  Send_Qiandao();
  Uart1_Tx_Send(white_module_param_xieyi, lenth);
}

/*查询当前频点*/
void loraGetFreq(void)
{
  uint16_t lenth = 0;
  uint16_t sum_jiaoyan_lenth = 0;
  white_module_param_xieyi[lenth++] = 0x68;
  white_module_param_xieyi[lenth++] = 0x07;
  white_module_param_xieyi[lenth++] = 0x00;
  white_module_param_xieyi[lenth++] = 0xC6;
  white_module_param_xieyi[lenth++] = 0x08;
  white_module_param_xieyi[lenth] = 0;
  for (sum_jiaoyan_lenth = 0; sum_jiaoyan_lenth < lenth; sum_jiaoyan_lenth++)
  {
    white_module_param_xieyi[lenth] += white_module_param_xieyi[sum_jiaoyan_lenth];
  }
  lenth++;
  white_module_param_xieyi[lenth++] = 0x16;
  Send_Qiandao();
  Uart1_Tx_Send(white_module_param_xieyi, lenth);
}
