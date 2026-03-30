#include "pc.h"
#include "uart0.h"
#include "test.h"
#include "jiaoyan.h"

extern struct Test Test_result;
#define send_lenth 200
uint8_t startTestResContents[6] = {0x68, 0xAB, 0x06, 0x00, 0x19, 0x16};
uint8_t lrdStartTestResContents[6] = {0x68, 0xBD, 0x06, 0x00, 0x2B, 0x16};
uint8_t testDataResContents[send_lenth];
uint8_t lrdEUIResContents[send_lenth];

/*————————开始测试PC协议返回————————*/
void startTestResponse(uint8_t *Data, uint32_t lenth)
{
    startTestResContents[3] = Test_result.Workstation;
    startTestResContents[4] = Sum_jiaoyan(Data, lenth);
    Uart0_Tx_Send(Data, lenth);
}

/*————————利尔达EUI数据返回————————*/
void lrdEUIResponse(void)
{
    uint16_t lenth = 0;
    uint8_t count = 0;
    memset(lrdEUIResContents, 0x00, send_lenth);
    lrdEUIResContents[lenth++] = 0x68;                    // 固定帧头
    lrdEUIResContents[lenth++] = 0xBB;                    // 功能码
    lrdEUIResContents[lenth++] = 0x00;                    // 先给0，后面重新赋值
    lrdEUIResContents[lenth++] = Test_result.Workstation; // 工位
    memcpy(&lrdEUIResContents[lenth], Test_result.LoraEUI, 16);
    lenth += 16;
    lrdEUIResContents[2] = lenth + 2; // 长度赋值
    for (count = 0; count < lenth; count++)
    {
        lrdEUIResContents[lenth] += lrdEUIResContents[count]; // 获取校验位的值
    }
    lenth++;
    lrdEUIResContents[lenth++] = 0x16; // 固定帧尾
    Uart0_Tx_Send(lrdEUIResContents, lenth);
}

/*————————测试数据返回————————*/
void testDataResponse(void)
{
    uint16_t lenth = 0;
    uint8_t count = 0;
    memset(testDataResContents, 0x00, send_lenth);
    testDataResContents[lenth++] = 0x68;                    // 固定帧头
    testDataResContents[lenth++] = 0xAD;                    // 功能码
    testDataResContents[lenth++] = 0x00;                    // 先给0，后面重新赋值
    testDataResContents[lenth++] = Test_result.Workstation; // 工位														//类型

    testDataResContents[lenth++] = Test_result.Voltage_Main_Mould & 0xFF; // 主电供电电压
    testDataResContents[lenth++] = (Test_result.Voltage_Main_Mould >> 8) & 0xFF;

    testDataResContents[lenth++] = Test_result.Voltage_Main_Protocol; // 主电协议获取电压
    testDataResContents[lenth++] = (Test_result.Voltage_Main_Protocol >> 8) & 0xFF;

    testDataResContents[lenth++] = Test_result.Current_Main_Static & 0xFF; // 静态功耗
    testDataResContents[lenth++] = (Test_result.Current_Main_Static >> 8) & 0xFF;

    testDataResContents[lenth++] = Test_result.Current_Full_Water & 0xFF; // 满水功耗
    testDataResContents[lenth++] = (Test_result.Current_Full_Water >> 8) & 0xFF;

    testDataResContents[lenth++] = Test_result.Current_Flow_Water & 0xFF; // 走水功耗
    testDataResContents[lenth++] = (Test_result.Current_Flow_Water >> 8) & 0xFF;

    testDataResContents[lenth++] = Test_result.Voltage_Standby_Mould & 0xFF; // 备电/NB/扩频供电电压
    testDataResContents[lenth++] = (Test_result.Voltage_Standby_Mould >> 8) & 0xFF;

    testDataResContents[lenth++] = Test_result.Voltage_Standby_Protocol & 0xFF; // 备电/NB/扩频协议获取电压
    testDataResContents[lenth++] = (Test_result.Voltage_Standby_Protocol >> 8) & 0xFF;

    testDataResContents[lenth++] = Test_result.Current_Standby & 0xFF; // 备电/NB/扩频功耗
    testDataResContents[lenth++] = (Test_result.Current_Standby >> 8) & 0xFF;

    testDataResContents[lenth++] = Test_result.Bluetooth;   // 蓝牙测试
    testDataResContents[lenth++] = Test_result.flash;       // flash测试
    testDataResContents[lenth++] = Test_result.Measurement; // 计量测试
    testDataResContents[lenth++] = Test_result.Infrared;    // 红外

    memcpy(&testDataResContents[lenth], Test_result.IMEI, 15);
    lenth += 15;
    memcpy(&testDataResContents[lenth], Test_result.IMSI, 15);
    lenth += 15;
    memcpy(&testDataResContents[lenth], Test_result.ICCID, 20);
    lenth += 20;

    testDataResContents[lenth++] = Test_result.CSQ;            // CSQ测试
    testDataResContents[lenth++] = Test_result.Valve;          // 阀门测试
    testDataResContents[lenth++] = Test_result.Valve_Position; // 阀门到位
    testDataResContents[lenth++] = Test_result.EEPROM;         // EEPROM

    testDataResContents[lenth++] = Test_result.GP30_Voltage & 0xFF; // GP30电压
    testDataResContents[lenth++] = (Test_result.GP30_Voltage >> 8) & 0xFF;

    memcpy(&testDataResContents[lenth], Test_result.LoraEUI, 16);
    lenth += 16;

    testDataResContents[lenth++] = Test_result.Strong_magnet;          // 强磁检测
    testDataResContents[lenth++] = Test_result.Open_Cap;               // 开盖检测
    testDataResContents[lenth++] = Test_result.GPS_Module;             // GPS模组检测
    testDataResContents[lenth++] = Test_result.non_magnet_Module_sign; // 无磁模组信号强度

    memcpy(&testDataResContents[lenth], Test_result.Check_Code, 2); // 校验码
    lenth += 2;
    memcpy(&testDataResContents[lenth], Test_result.version, 2); // 程序版本号
    lenth += 2;

    testDataResContents[lenth++] = Test_result.T; // 水温检测
    testDataResContents[lenth++] = Test_result.P; // 压力检测

    memcpy(&testDataResContents[lenth], Test_result.Lorarssi, 2); // lora模组信号强度
    lenth += 2;
    memcpy(&testDataResContents[lenth], Test_result.Loraisnr, 2); // lora模组信噪比
    lenth += 2;

    testDataResContents[lenth++] = Test_result.wake;         // 唤醒检测
    testDataResContents[lenth++] = Test_result.Pulse_Output; // 脉冲输出检测
    testDataResContents[lenth++] = Test_result.Interrupt;    // 中断检测
    testDataResContents[lenth++] = Test_result.Ext_volatge;  // 外电检测
    testDataResContents[lenth++] = Test_result.RS485;        // 485检测
    testDataResContents[lenth++] = Test_result.Three_Pulse;  // 三线脉冲检测
    testDataResContents[lenth++] = Test_result.Wmbus;        // wmbus检测

    testDataResContents[2] = lenth + 2; // 长度赋值
    for (count = 0; count < lenth; count++)
    {
        testDataResContents[lenth] += testDataResContents[count]; // 获取校验位的值
    }
    lenth++;
    testDataResContents[lenth++] = 0x16; // 固定帧尾
    Uart0_Tx_Send(testDataResContents, lenth);
}

/*————————PC协议通讯接收解析————————*/
void pcProtocolParse(uint8_t *Data, uint32_t lenth)
{
    uint8_t pHead = 0;
    uint16_t frame_lenth = 0;
    uint8_t sum_jiaoyan = 0;
    uint8_t sum_jiaoyan_jisuan = 0;
    uint8_t PC_analysis_Data[200] = {0};
    while (1)
    {
        if (pHead + 5 > lenth)
            break;
        if (Data[pHead] == 0x68)
        {
            frame_lenth = Data[pHead + 2];
            if (pHead + frame_lenth <= lenth)
            {
                memcpy(PC_analysis_Data, Data + pHead, frame_lenth); // 取出要分析的数据,从pHead开始的数据进行校验
                sum_jiaoyan = Data[pHead + frame_lenth - 2];
                sum_jiaoyan_jisuan = Sum_jiaoyan(PC_analysis_Data, frame_lenth);
                if (sum_jiaoyan == sum_jiaoyan_jisuan)
                {
                    switch (Data[pHead + 1])
                    {
                    case 0xAA:
                        if (Data[pHead + 3] == Test_result.Workstation && Data[pHead + frame_lenth - 1] == 0x16)
                        {
                            Test_result.module_type = 0;
                            Test_result.frequence = 0;
                            startTestResponse(startTestResContents, 6);
                            Test_result.module_type = Data[pHead + 16];
                            Test_result.frequence = Data[pHead + 26];
                            Test_result.frequence = (Test_result.frequence << 8) + Data[pHead + 27];
                            if (Test_result.module_type == lierda_lora)
                                getLrdEUIInit();
                            else
                                startTestInit();
                        }
                        break;
                    case 0xAC:
                        if (Data[pHead + 3] == Test_result.Workstation && Data[pHead + frame_lenth - 1] == 0x16 && Test_result.Find_Enable == 1)
                        {
                            testDataResponse();
                        }
                        break;
                    case 0xBA:
                        if (Data[pHead + 3] == Test_result.Workstation && Data[pHead + frame_lenth - 1] == 0x16 && Test_result.Find_Enable == 1)
                        {
                            lrdEUIResponse();
                        }
                        break;
                    case 0xBC:
                        if (Data[pHead + 3] == Test_result.Workstation && Data[pHead + frame_lenth - 1] == 0x16)
                        {
                            memset(Test_result.appkey, 0x00, 32);
                            memset(Test_result.appeui, 0x00, 16);
                            startTestResponse(lrdStartTestResContents, 6);
                            memcpy(Test_result.appkey, &Data[pHead + 4], 32);
                            memcpy(Test_result.appeui, &Data[pHead + 36], 16);
                            startTestInit();
                        }
                        break;
                    default:
                        break;
                    }
                    pHead += frame_lenth;
                }
            }
        }
        pHead++;
    }
}
