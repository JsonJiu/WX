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
extern uint32_t interval_timing;    // 间隔测试时间

extern uint8_t repeat_report; // 重复上告计数

void Test_Function_Lierda(void)
{
    if (interval_timing > 0)
        return;
    switch (Test_process)
    {
    case t_find_EUI:
        if (single_step_timing > 0)
        {
            if (Test_agreement_receive == r_get_LrdEUI)
            {
                Test_agreement_receive = NO_receive;
                interval_timing = 0;
                if (Test_result.frequence == 923)
                {
                    single_step_timing = 10000;
                    Test_process = t_set_frequence;
                }
                else
                {
                    single_step_timing = 0;
                    Test_process = t_end;
                }
            }
            else
            {
                getLrdLoRaEUI();
                interval_timing = 3000;
            }
        }
        else
        {
            single_step_timing = 0;
            memset(Test_result.LoraEUI, 0x00, 16);
            Test_process = t_end;
        }
        break;
    case t_set_frequence:
        if (single_step_timing > 0)
        {
            if (Test_agreement_receive == r_write_module_param)
            {
                Test_agreement_receive = NO_receive;
                interval_timing = 0;
                single_step_timing = 10000;
                Test_process = t_get_frequence;
            }
            else
            {
                loraSetFreq923();
                interval_timing = 3000;
            }
        }
        else
        {
            single_step_timing = 0;
            memset(Test_result.LoraEUI, 0x00, 16);
            Test_process = t_end;
        }
        break;
    case t_get_frequence:
        if (single_step_timing > 0)
        {
            if (Test_agreement_receive == r_get_frequence)
            {
                Test_agreement_receive = NO_receive;
                interval_timing = 0;
                single_step_timing = 0;
                Test_process = t_end;
            }
            else
            {
                loraGetFreq();
                interval_timing = 3000;
            }
        }
        else
        {
            single_step_timing = 0;
            memset(Test_result.LoraEUI, 0x00, 16);
            Test_process = t_end;
        }
        break;
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
            if (Test_result.Current_Main_Static > 4 && Test_result.Current_Main_Static < 30)
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
            if (Test_result.Current_Full_Water > 4 && Test_result.Current_Full_Water < 30)
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
            if (Test_result.Current_Flow_Water > 4 && Test_result.Current_Flow_Water < 30)
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
        Test_process = t_get_Standby_current;
        single_step_timing = 30000; // 30s时间测试
        break;
    case t_get_Standby_current:
        if (single_step_timing > 0)
        {
            Test_result.Current_Standby = Current_CHK_Func(1);
            if (Test_result.Current_Standby > 4 && Test_result.Current_Standby < 30)
            {
                single_step_timing = 0;
                single_step_timing = 10000; // 10s时间测试
                Test_process = t_find_device_ID;
            }
            else
                interval_timing = 1000;
        }
        else
        {
            single_step_timing = 10000; // 10s时间测试
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
                Test_process = t_write_APPKEY;
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
            Test_process = t_end; // 表号读不到，就说明红外有问题，直接结束测试
        }
        break;
    case t_write_APPKEY:
        if (single_step_timing > 0)
        {
            if (Test_agreement_receive == r_write_module_param)
            {
                Test_agreement_receive = NO_receive;
                interval_timing = 0;
                single_step_timing = 10000;
                Test_process = t_write_APPEUI;
            }
            else
            {
                whiteAppkey();
                interval_timing = 3000;
            }
        }
        else
        {
            single_step_timing = 10000;
            Test_process = t_write_APPEUI;
        }
        break;
    case t_write_APPEUI:
        if (single_step_timing > 0)
        {
            if (Test_agreement_receive == r_write_module_param)
            {
                Test_agreement_receive = NO_receive;
                interval_timing = 0;
                single_step_timing = 10000;
                Test_process = t_lierda_module_select;
            }
            else
            {
                whiteAppEUI();
                interval_timing = 3000;
            }
        }
        else
        {
            single_step_timing = 10000;
            Test_process = t_lierda_module_select;
        }
        break;
    case t_lierda_module_select:
        if (single_step_timing > 0)
        {
            if (Test_agreement_receive == r_get_lerda_module_type)
            {
                Test_agreement_receive = NO_receive;
                interval_timing = 0;
                single_step_timing = 10000;
                if (Test_result.lierda_module_type == lierda_wmbus)
                    Test_process = t_config_wmbus;
                else if (Test_result.lierda_module_type == lierda_lorawan)
                    Test_process = t_write_save;
            }
            else
            {
                findLrdType();
                interval_timing = 3000;
            }
        }
        else
        {
            single_step_timing = 10000;
            Test_process = t_write_save;
        }
        break;
    case t_config_wmbus:
        if (single_step_timing > 0)
        {
            if (Test_agreement_receive == r_config_wmbus)
            {
                Test_agreement_receive = NO_receive;
                interval_timing = 0;
                single_step_timing = 10000;
                Test_process = t_get_wmbus_state;
            }
            else
            {
                configWmbus();
                interval_timing = 3000;
            }
        }
        else
        {
            single_step_timing = 10000;
            Test_process = t_Measurement_test;
        }
        break;
    case t_get_wmbus_state:
        if (single_step_timing > 0)
        {
            if (Test_agreement_receive == r_get_wmbus_state)
            {
                Test_agreement_receive = NO_receive;
                interval_timing = 0;
                single_step_timing = 10000;
                Test_process = t_set_wmbus_key;
            }
            else
            {
                findWmbusConfig();
                interval_timing = 3000;
            }
        }
        else
        {
            single_step_timing = 10000;
            Test_process = t_Measurement_test;
        }
        break;
    case t_set_wmbus_key:
        if (single_step_timing > 0)
        {
            if (Test_agreement_receive == r_set_wmbus_key)
            {
                Test_agreement_receive = NO_receive;
                interval_timing = 0;
                single_step_timing = 10000;
                Test_process = t_write_reset;
            }
            else
            {
                setWmbusKey();
                interval_timing = 3000;
            }
        }
        else
        {
            single_step_timing = 10000;
            Test_process = t_Measurement_test;
        }
        break;
    case t_write_save:
        if (single_step_timing > 0)
        {
            if (Test_agreement_receive == r_write_module_param)
            {
                Test_agreement_receive = NO_receive;
                interval_timing = 0;
                single_step_timing = 10000;
                Test_process = t_write_reset;
            }
            else
            {
                whiteSave();
                interval_timing = 3000;
            }
        }
        else
        {
            single_step_timing = 10000;
            Test_process = t_write_reset;
        }
        break;
    case t_write_reset:
        if (single_step_timing > 0)
        {
            if (Test_agreement_receive == r_write_module_param)
            {
                Test_agreement_receive = NO_receive;
                interval_timing = 0;
                single_step_timing = 10000;
                Test_process = t_restart;
            }
            else
            {
                whiteReset();
                interval_timing = 3000;
            }
        }
        else
        {
            single_step_timing = 10000;
            Test_process = t_restart;
        }
        break;
    case t_restart:
        Standby_Voltage_Output_Ctl(Off);
        FL_DelayMs(2000);
        Standby_Voltage_Output_Ctl(On);
        Test_process = t_manual_report_UP;
        break;
    case t_manual_report_UP:
        if (single_step_timing > 0)
        {
            if (Test_agreement_receive == r_get_report)
            {
                Test_agreement_receive = NO_receive;
                interval_timing = 5000;
                single_step_timing = 10000;
                Test_process = t_find_result_first;
            }
            else
            {
                Manual_Report_UP();
                interval_timing = 3000;
            }
        }
        else
        {
            single_step_timing = 10000;
            Test_process = t_find_result_first;
        }
        break;
    case t_find_result_first:
        if (single_step_timing > 0)
        {
            if (Test_agreement_receive == r_find_result && Test_result.GPS_Module == 1)
            {
                Test_agreement_receive = NO_receive;
                interval_timing = 0;
                single_step_timing = 10000;
                Test_process = t_Measurement_test;
            }
            else
            {
                Find_Result();
                interval_timing = 3000;
            }
        }
        if (single_step_timing == 0 && (repeat_report >= 3 || Test_agreement_receive == NO_receive))
        {
            interval_timing = 0;
            single_step_timing = 10000;
            Test_process = t_Measurement_test;
        }
        if (single_step_timing == 0 && repeat_report < 3 && Test_agreement_receive == r_find_result)
        {
            repeat_report++;
            Test_agreement_receive = NO_receive;
            single_step_timing = 10000;
            Test_process = t_manual_report_UP;
        }
        break;
    case t_Measurement_test:
        if (single_step_timing > 0)
        {
            if (Test_agreement_receive == r_find_device_ID)
            {
                Test_agreement_receive = NO_receive;
                interval_timing = 0;
                single_step_timing = 0;
                Test_result.cumulant2_int = (Test_result.cumulant_Data[0] << 24) | (Test_result.cumulant_Data[1] << 16) | (Test_result.cumulant_Data[2] << 8) | Test_result.cumulant_Data[3];
                Test_result.cumulant2_decimal = (Test_result.cumulant_Data[4] << 24) | (Test_result.cumulant_Data[5] << 16) | (Test_result.cumulant_Data[6] << 8) | Test_result.cumulant_Data[7];
                if (Test_result.cumulant2_int > Test_result.cumulant1_int)
                {
                    Test_result.Measurement = 1;
                }
                if (Test_result.cumulant2_int == Test_result.cumulant1_int)
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
                Test_process = t_end;
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
