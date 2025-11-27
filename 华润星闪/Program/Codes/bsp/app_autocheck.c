#include "config.h"
#include "drv_ad.h"
#include "app_ina219.h"
#include "lib_public.h"

unsigned char IDNUM = 0;

//#define DEF_ZDUART_CHECK
#define DEF_VALVE_CHECK
//#define DEF_DAOWEI_CHECK
#define DEF_IRDA


CHECKPRESULT_S CheckResult = {
                                0x01,           //ID
                                0xFF,           //表具类型
                                0xFF,           //是否带附件
                                0xFF,           //主控板电压
                                0xFF,           //主控板电流
                                0xFF,           //信号强度
                                0xFF,           //RTC电池
                                0xFF,           //软件版本号1
                                0xFF,           //软件版本号2
                                0xFF,           //保留
                                0xFFF0,         //状态
                                0xFF,           // 星闪连接状态
                                0xFF,           // 星闪信号强度
                                };

unsigned char SENCOND_TIMERNUM = 0;

DEVICETOOL_S DeviceTool;
CHECKPRESULT_S CheckResult;

/**
 * 工装板的IO口状态
 */
void Init_DeviceToolIoStatus(void)
{
    VALVE_R1_IN;
    VALVE_B1_IN;

    ZD_POW_IN;

    IO_HALL_A_IN;
    IO_HALL_B_IN;
    IO_119_OUT;
    IO_119_1;
//    IO_KEY_1_IN;IO_KEY_2_IN;

    IO_VALVE_DAOWEI1_IN;

}

/**
 * 直读电源中断服务函数
 */
void ZdPower_IntVoid(void)
{
    unsigned char dat[7] = {0xfe, 0x68, 0x00, 0x00, 0x00, 0x00, 0x00};
    
    DelayMs(50);
//    OM_Open_Usart(DEFAULT);
    DelayMs(5);
//    OM_SendUsartData(DEFAULT, dat, sizeof(dat));
//    CloseUsart(DEFAULT);
    DelayMs(5);
//    SetUsartIoOut(DEFAULT, 0);
    DelayMs(5);
    SetUartTxDirIn();
    if(CheckResult.DeviceID == IDNUM)
    {
        OM_SendUsartData(Debug_COM, DebugBuf, sprintf((char*)DebugBuf,"\r\n==直读中断产生！===\r\n"));
    }
}

/**
 * 初始化直读模块
 */
void Init_ZdMorden(void)
{
    //OM_Open_Usart(DEFAULT);
    ZDSetIntIo(ZdPower_IntVoid);
}

/**
 * 获取设备ID
 * @return
 */
unsigned char GetDeviceID(void)
{

    if(GetP2Status() == 0 && GetP1Status() == 0 && GetP0Status() == 1)  return 1;
    if(GetP2Status() == 0 && GetP1Status() == 1 && GetP0Status() == 0)  return 2;
    if(GetP2Status() == 0 && GetP1Status() == 1 && GetP0Status() == 1)  return 3;
    if(GetP2Status() == 1 && GetP1Status() == 0 && GetP0Status() == 0)  return 4;
    return 0;
}
/**
 * 检测主控板在线的标识
 * @return
 */
unsigned long CheckMasterOnLineVolt(void)
{
    unsigned long SysVolt = 0;
     unsigned char R1 = 10, R2  = 1;
    unsigned short VoltMul = (R1 + R2)*100/R2;
//    ADC_InitTypeDef ADC_InitPara_OnLine    = {0x00, 0x00, 0x0A, 0x00};
    ADC_InitTypeDef ADC_InitPara_OnLine    = {0x00, 0x00, 0x0A, 0x01};
    OA_ADC_Init(&ADC_InitPara_OnLine);
    //计算系统电源电压
    SysVolt = (unsigned long)OA_ADC_GetValue(ADCCHANNEL_ONLINE);
        SysVolt *= AdcRefVolt;
    if(ADC_InitPara_OnLine.ADCModeConversionBit == 8)
        SysVolt >>= 8;
    else
        SysVolt >>= 10;
    SysVolt *= VoltMul;
    SysVolt /= 100;
    return SysVolt;
}

/**
 * 检测主控板RTC电压
 * @return
 */
unsigned long CheckMasterRTCVolt(void)
{
    unsigned long SysVolt = 0;
    //   unsigned char R1 = 1, R2  = 1;
     unsigned char R1 = 10, R2  = 1;
    unsigned short VoltMul = (R1 + R2)*100/R2;
 //   ADC_InitTypeDef ADC_InitPara_RTCVolt    = {0x00, 0x00, 0x0A, 0x01};
    ADC_InitTypeDef ADC_InitPara_RTCVolt    = {0x00, 0x01, 0x0A, 0x01};
    OA_ADC_Init(&ADC_InitPara_RTCVolt);
    //计算系统电源电压
    SysVolt = (unsigned long)OA_ADC_GetValue(ADCCHANNEL_RTC);
        SysVolt *= AdcRefVolt;
    if(ADC_InitPara_RTCVolt.ADCModeConversionBit == 8)
        SysVolt >>= 8;
    else
        SysVolt >>= 10;
    SysVolt *= VoltMul;
    SysVolt /= 100;
    return SysVolt;
}


/**
 * 检测备电接口电压
 * @return
 */
unsigned long CheckMasterBATVolt(void)
{
    unsigned long SysVolt = 0;
    //   unsigned char R1 = 1, R2  = 1;
     unsigned char R1 = 10, R2  = 1;
    unsigned short VoltMul = (R1 + R2)*100/R2;
 //   ADC_InitTypeDef ADC_InitPara_RTCVolt    = {0x00, 0x00, 0x0A, 0x01};
    ADC_InitTypeDef ADC_InitPara_RTCVolt    = {0x00, 0x02, 0x0A, 0x01};
    OA_ADC_Init(&ADC_InitPara_RTCVolt);
    //计算系统电源电压
    SysVolt = (unsigned long)OA_ADC_GetValue(ADCCHANNEL_BAT);
        SysVolt *= AdcRefVolt;
    if(ADC_InitPara_RTCVolt.ADCModeConversionBit == 8)
        SysVolt >>= 8;
    else
        SysVolt >>= 10;
    SysVolt *= VoltMul;
    SysVolt /= 100;
    return SysVolt;
}

/**
 * 秒定时器任务
 */
void TaskSencondTimer(void)
{
    DeviceTool.FlagMasterOnLineVolt = CheckMasterOnLineVolt()/100;
    CheckResult.RTCVoult = CheckMasterRTCVolt()/100;

    if(DeviceTool.FlagMasterOnLineVolt < 10)
    {
        //Debug(0x01);
        if( ( DeviceTool.EnableDeviceToolCheck != 0 ) || (DeviceTool.StatusCheck != chk_Halt) )
        {    
            IO_HALL_A_OUT;
            IO_HALL_B_OUT;
            IO_HALL_A_0;
            IO_HALL_B_0;
            DeviceTool.EnableDeviceToolCheck = 0;
            DeviceTool.StatusCheck = chk_Halt;
            OM_SendUsartData(Debug_COM, DebugBuf, sprintf((char*)DebugBuf,"\r\n====被测板离线，停止测试！！！====\r\n"));
        }
    }
}

/**
 * 秒定时器任务初始化
 */
void Init_SencondTimer(void)
{
    if(SENCOND_TIMERNUM == 0)
    {
        SENCOND_TIMERNUM = User_AskTimeNumber();
    }
    TaskSencondTimer();
    User_StartTime(SUBTIMER, SENCOND_TIMERNUM, 1, TaskSencondTimer);
}

/**
 * 检测工具定时任务
 */
void DeviceToolCheckTiming_Task(void)
{
    if(DeviceTool.SecondsTiming) DeviceTool.SecondsTiming--;
}

unsigned char DEVICETOOLCHECK_TIMER_NUM = 0;

/**
 * 申请上告定时器
 */
void Ask_DeviceToolsCheck_TimerNum(void)
{
    if(DEVICETOOLCHECK_TIMER_NUM == 0)
    {
        DEVICETOOLCHECK_TIMER_NUM = User_AskTimeNumber();
    }
    User_StartTime(MAINTIMER, DEVICETOOLCHECK_TIMER_NUM, 1000, DeviceToolCheckTiming_Task);
}

/*
 *發送報文
 */
//#define Debug_Mode
unsigned char debug_data[14] = {0x00,0x01,0x02,0x03,0x04,0x05,0x06,0x07,0x08,0x09,0x10,0x11,0x12,0x13};
void Debug(unsigned char data)
{
#ifdef Debug_Mode
    UsartTxConfig(UP_COM, 1);
    DelayMs(200);
    OM_SendUsartData(UP_COM, &data, 1);
    DelayMs(1000);
    UsartTxConfig(UP_COM, 0);
#endif
}

/**
 * 自动检测过程
 */
void Process_AutoCheck(void)
{
    unsigned char times = 3;
    switch(DeviceTool.StatusCheck)
    {
        case chk_Halt:   //在未检测到命令的时候，空转
            break;
        case chk_Start:  //开始检测
            
            if(DeviceTool.FlagMasterOnLineVolt >= 10)           //当检测到有电压的时候，返回数据：主控板已安放
            {
                DeviceTool.FlagMasterOnLineVolt = CheckMasterOnLineVolt()/100;
                CheckResult.RTCVoult = CheckMasterRTCVolt()/100;
                //unsigned char chatbat = CheckMasterBATVolt()/100;
                unsigned char chatbat = 0x00;
                Debug(0x21);
                if(chatbat<0x0A)
                {
                    CheckResult.charkbat = 0x00;
                }
                else
                {
                    CheckResult.charkbat = 0x01;
                }
                

                LedOrBeepTick((LED_TypeDef)(GREEN_LED) , TICK_COUNT(2), TICK_ON_1MS(500), TICK_OFF_1MS(500));
                OM_SendUsartData(Debug_COM, DebugBuf, sprintf((char*)DebugBuf,"\r\n\r\n\r\n**********************************************************************************\r\n"));
                OM_SendUsartData(Debug_COM, DebugBuf, sprintf((char*)DebugBuf,"\r\n===MasterBoard OnLine !====\r\n"));
                OM_SendUsartData(Debug_COM, DebugBuf, sprintf((char*)DebugBuf,"\r\n RTC电压%d,\r\n", CheckResult.RTCVoult));
                OM_SendUsartData(Debug_COM, DebugBuf, sprintf((char*)DebugBuf,"\r\n 主控板电压%d,\r\n", DeviceTool.FlagMasterOnLineVolt));
                /*********************直读检测用串口*******************/
#ifdef DEF_ZDUART_CHECK
           //     Init_ZdMorden();
#endif
                /************************************************************/
#ifdef DEF_IRDA 
                OM_Close_Usart(Meter_COM);
                OM_Open_Usart(IR_COM);
#endif
                Init_DeviceToolIoStatus();
                Set_MasterLowConsManage(ENABLE);//使能主控板退出低功耗，開啟紅外
                SetCheckConsumptionPower(DISABLE);//断开检测主控板功耗的电源
                DelayMs(100);
                SetMasterBoardPower(ENABLE);//打开主控板电源
                SetIrdaPower(0);
                DeviceTool.FlagMasterBoardSelfCheckFinish = 0;
                DeviceTool.FlagCheckProcess = 0;
                DeviceTool.SecondsTiming = 55;
                DeviceTool.StatusCheck = chk_MasterOnLine;
                Debug(0x22);
            }
            break;
        case chk_MasterOnLine:
            //SetMasterBoardInPut(DeviceTool.FlagSetHightLow);//向主控板提供输入口低电平
            if((DeviceTool.FlagMasterBoardSelfCheckFinish == MASTER_SELFCHECK_FINISH) || (DeviceTool.SecondsTiming == 0))//等待主控板自检完成
            {
                if(DeviceTool.SecondsTiming == 0)  //若超时，则直接结束测试（应该是）
                {
                    DeviceTool.SecondsTiming = 0;
//                    DeviceTool.StatusCheck = chk_end;
                    DeviceTool.StatusCheck = chk_CheckFinish;              //直接结束测试至最后一步
                    memset(&CheckResult.MeterTYP, 0x00, sizeof(CHECKPRESULT_S));   //表具类型清零？
                    OM_SendUsartData(Debug_COM, DebugBuf, sprintf((char*)DebugBuf, "\r\n====Master Board Self Check OverTime!====\r\n"));
                }
                else
                {
                    DeviceTool.SecondsTiming = 30;  //计时30秒
                    DeviceTool.StatusCheck = chk_MasterBoardSelfCheckFinish;       //进行下一项测试
                    Debug(0x31);
                    OM_SendUsartData(Debug_COM, DebugBuf, sprintf((char*)DebugBuf, "\r\n====Master Board Self Check Finish!====\r\n"));
                }

                if(DeviceTool.SecondsTiming != 0)   //若自检完成，则灯闪烁
                    LedOrBeepTick((LED_TypeDef)(GREEN_LED) , TICK_COUNT(3), TICK_ON_1MS(50), TICK_OFF_1MS(50));
                /***********************直读检测用串口********************/
#ifdef DEF_ZDUART_CHECK
//                OM_Close_Usart(DEFAULT);
//                CloseZdPowerInt();
//                SetUartTxDirIn();
#endif
                /***********************直读检测用串口************************/
            }
            break;

        case chk_MasterBoardSelfCheckFinish:        //启动红外
            if((DeviceTool.EnableDeviceToolCheck == 1) || (DeviceTool.SecondsTiming == 0))   //板子离开检测与超时检测
            {
                if(DeviceTool.SecondsTiming != 0)    //若没有超时
                    LedOrBeepTick((LED_TypeDef)(GREEN_LED) , TICK_COUNT(1), TICK_ON_1MS(10), TICK_OFF_1MS(80));
                DeviceTool.StatusCheck = chk_DeviceToolConnectMasterBoard;
                Debug(0x32);
                DeviceTool.CountReapeat = 0;
                DeviceTool.SecondsTiming = 5;                //延时5S
/*****************************红外通讯****************************************/
#ifdef DEF_IRDA
                {
                    OM_SendUsartData(Debug_COM, DebugBuf, sprintf((char*)DebugBuf,"\r\n========启动连接(红外)=========\r\n"));
                    IrdaUartDataFormFrame(OPT_WRITE, DEV_TESTCONNECT);  //组帧，为发送帧组帧(是通过红外发的)
//                    IrdaUartDataFormFrame(OPT_READ, DEV_TIME);
                    CheckResult.Status.Irda = 0;
                }
#else
                MeterUartDataFormFrame(OPT_WRITE, DEV_TESTCONNECT);//启动检测板连接主控板
#endif
/*****************************红外通讯****************************************/
            }
            break;
        case chk_DeviceToolConnectMasterBoard:
            if(DeviceTool.FlagCheckProcess == MASTER_CONNCET_CHECK || (DeviceTool.SecondsTiming == 0))//若读取到返回或是超时
            {
                CheckResult.Status.Irda = 1;
                OM_SendUsartData(Debug_COM, DebugBuf, sprintf((char*)DebugBuf,"\r\n========Connect Ok!=========\r\n"));
#ifdef DEF_IRDA
                DeviceTool.CountReapeat = 0;
                DeviceTool.SecondsTiming = 0;
                DeviceTool.StatusCheck = chk_find_IMEI;  //调试
                Debug(0x33);
                OM_Open_Usart(Meter_COM);
                OM_Close_Usart(IR_COM);
#else
                Set_MasterLowConsManage(DISABLE);//使能主控板进入低功耗
                DeviceTool.SecondsTiming = 8;
                DeviceTool.StatusCheck = chk_CheckCurrent;
#endif  
            }
            else if(DeviceTool.SecondsTiming == 0)   //若是超时进入该状态
            {
                DeviceTool.CountReapeat++;
                if(DeviceTool.CountReapeat > 2 )     //检测两次，两次不通过返回失败
                {
                    DeviceTool.SecondsTiming = 0;
//                    DeviceTool.StatusCheck = chk_end;
                    DeviceTool.StatusCheck = chk_CheckFinish;
                    memset(&CheckResult.MeterTYP, 0x00, sizeof(CHECKPRESULT_S));
                    OM_SendUsartData(Debug_COM, DebugBuf, sprintf((char*)DebugBuf,"\r\n========Connect OverTime!=========\r\n"));
                }
                else                                 //第一次检测失败，再发送一次
                {
                    DeviceTool.SecondsTiming = 5;
                    IrdaUartDataFormFrame(OPT_WRITE, DEV_TESTCONNECT);
                    OM_SendUsartData(Debug_COM, DebugBuf, sprintf((char*)DebugBuf,"\r\n========启动连接(红外)=========\r\n"));
                }
            }
            break;

        case chk_find_IMEI:
            if(DeviceTool.SecondsTiming==0)
            {
                DeviceTool.SecondsTiming = 0; //将延时增加3秒
                MeterUartDataFormFrame(OPT_READ, DEV_read_IMEI_IMSI_ICCID);//发送协议以读取IMEI
                DelayMs(3000);
                DeviceTool.StatusCheck = chk_find_MAC;
                Debug(0x41);
                OM_SendUsartData(Debug_COM, DebugBuf, sprintf((char*)DebugBuf,"\r\n========读取IMEI=========\r\n"));
            }
            break;

        case chk_find_MAC:
            if(DeviceTool.SecondsTiming==0)
            {
                DeviceTool.SecondsTiming = 0; //将延时增加3秒
                MeterUartDataFormFrame(OPT_READ, DEV_READ_CHECK_STATUS);//发送协议以读取星闪MAC
                DelayMs(3000);
                DeviceTool.StatusCheck = chk_MasterBoard_CloseIR;
                OM_SendUsartData(Debug_COM, DebugBuf, sprintf((char*)DebugBuf,"\r\n========读取星闪MAC=========\r\n"));
                Debug(0x42);
            }
            break;
            
        case chk_MasterBoard_CloseIR: 
            if(DeviceTool.SecondsTiming)
            {
                if(DeviceTool.FlagCheckProcess == MASTER_IR_CLOSED)
                {
                    Set_MasterLowConsManage(DISABLE);//使能主控板进入低功耗
                    DelayMs(3000);
                    SetCheckConsumptionPower(ENABLE);//提供检测主控板功耗的电源
                    DelayMs(100);
                    //DelayMs(100);
                    SetMasterBoardPower(DISABLE);//断开主控板电源
                    DelayMs(100);
                    
                    DeviceTool.CountReapeat = 0; //次数重置为第一次
                    DeviceTool.SecondsTiming = 0; //将延时减少3秒
                    DeviceTool.StatusCheck = chk_CheckCurrent;
                    Debug(0x43);
                    OM_SendUsartData(Debug_COM, DebugBuf, sprintf((char*)DebugBuf,"\r\n== IR Closed ===\r\n"));
                    DelayMs(3000);//增加延时
                }
            }
            else
            {
                MeterUartDataFormFrame(OPT_WRITE, DEV_CLOSE_IR);

                DeviceTool.SecondsTiming = 3;
                DeviceTool.CountReapeat++;
                if(DeviceTool.CountReapeat > 2 )
                {
                    Set_MasterLowConsManage(DISABLE);//使能主控板进入低功耗(关闭串口使能，以进入低功耗)
                    DelayMs(3000);
                    SetCheckConsumptionPower(ENABLE);//提供检测主控板功耗的电源
                    DelayMs(100);
                    SetMasterBoardPower(DISABLE);//断开主控板电源
                    DeviceTool.CountReapeat = 0;
                    DeviceTool.SecondsTiming = 0; //减少3
                    DeviceTool.StatusCheck = chk_CheckCurrent;
                    OM_SendUsartData(Debug_COM, DebugBuf, sprintf((char*)DebugBuf,"\r\n== IR Close Over Time ===\r\n"));
                    DelayMs(3000);//增加延时
                }
            }
            break;
            
        case chk_CheckCurrent:                        //检测电流
            if(DeviceTool.SecondsTiming == 0)
            {
                unsigned char i = 0;
                unsigned short temp = 0xFF;
                for(i = 0; i < 10; i++)
                {
                    temp = GetCurrent();
                    if(temp >= 0xFF)
                        temp = 0xFF;

                    if(temp < 15)
                    {
                        CheckResult.MasterCurrent = temp;
                        break;
                    }
                    else
                    {
                        if(CheckResult.MasterCurrent > temp)
                            CheckResult.MasterCurrent = temp;
                    }
                    OM_SendUsartData(Debug_COM, DebugBuf, sprintf((char*)DebugBuf,"\r\n== Current %d = %d uA==\r\n", (i + 1),CheckResult.MasterCurrent));
                    DelayMs(470);
                }
                SetMasterBoardPower(ENABLE);//提供主控板电源
                DelayMs(100);
                SetCheckConsumptionPower(DISABLE);//断开检测主控板功耗的电源
                Set_MasterLowConsManage(ENABLE);  //使能主控板退出低功耗
                OM_SendUsartData(Debug_COM, DebugBuf, sprintf((char*)DebugBuf,"\r\n==Current = %d uA==\r\n", CheckResult.MasterCurrent));
                DeviceTool.SecondsTiming = 0;
#ifdef DEF_ZDUART_CHECK
                DeviceTool.StatusCheck = chk_CheckFinish;
                DeviceTool.SecondsTiming = 0;
#else 
                DeviceTool.StatusCheck = chk_SetMsterBoardOutHight;
                Debug(0x51);
#endif
//                LedOrBeepTick((LED_TypeDef)(GREEN_LED) , TICK_COUNT(8), TICK_ON_1MS(10), TICK_OFF_1MS(80));
            }
            break;

        case chk_SetMsterBoardOutHight:
            if(DeviceTool.SecondsTiming)
            {
                if(DeviceTool.FlagCheckProcess == MASTER_CHECK_ONE)
                {
                    DeviceTool.CountReapeat = 0;
                   
                    DeviceTool.StatusCheck = chk_SetMsterBoardOutLow;
                    DeviceTool.SecondsTiming = 0;
                    OM_SendUsartData(Debug_COM, DebugBuf, sprintf((char*)DebugBuf,"\r\n== Check High Finish ===\r\n"));
//                    LedOrBeepTick((LED_TypeDef)(GREEN_LED) , TICK_COUNT(8), TICK_ON_1MS(10), TICK_OFF_1MS(80));
                    Debug(0x52);
                }
            }
            else
            {
                DelayMs(2000);
                DeviceTool.FlagSetHightLow = 1;//告知主控板的输出IO口，输出高电平
                SetMasterBoardInPut(DeviceTool.FlagSetHightLow);//向主控板提供输入口高电平
                DelayMs(200);
                MeterUartDataFormFrame(OPT_WRITE, DEV_SETOUTIOSTATUS);

                DeviceTool.SecondsTiming = 5;
                DeviceTool.CountReapeat++;
                if(DeviceTool.CountReapeat > 2 )
                {
                    CheckResult.Status.Status = 0;
                    DeviceTool.CountReapeat = 0;
                    DeviceTool.StatusCheck = chk_SetMsterBoardOutLow;
                    Set_MasterLowConsManage(ENABLE);//使能主控板退出低功耗
                    OM_SendUsartData(Debug_COM, DebugBuf, sprintf((char*)DebugBuf,"\r\n== Check High Over Time ===\r\n"));
                }
            }
            break;
        case chk_SetMsterBoardOutLow:
            if(DeviceTool.SecondsTiming)
            {
                if(DeviceTool.FlagCheckProcess == MASTER_CHECK_TWO)
                {
                    DeviceTool.CountReapeat = 0;
                    OM_SendUsartData(Debug_COM, DebugBuf, sprintf((char*)DebugBuf,"\r\n== Check Low Finish ===\r\n"));
                    DeviceTool.StatusCheck = chk_CheckFinish;
                    Debug(0x53);
//                    LedOrBeepTick((LED_TypeDef)(GREEN_LED) , TICK_COUNT(8), TICK_ON_1MS(10), TICK_OFF_1MS(80));
                }
            }
            else
            {
                DeviceTool.FlagSetHightLow = 0;//告知主控板的输出IO口，输出低电平
                SetMasterBoardInPut(DeviceTool.FlagSetHightLow);//向主控板提供输入口低电平
                DelayMs(200);
                MeterUartDataFormFrame(OPT_WRITE, DEV_SETOUTIOSTATUS);
                DeviceTool.SecondsTiming = 5;
                DeviceTool.CountReapeat++;
                if(DeviceTool.CountReapeat > 2)
                {
                    CheckResult.Status.Status = 0;
                    DeviceTool.CountReapeat = 0;
                    DeviceTool.StatusCheck = chk_CheckFinish;
                    OM_SendUsartData(Debug_COM, DebugBuf, sprintf((char*)DebugBuf,"\r\n== Check Low OverTime ===\r\n"));
                }
            }
            break;
        //case chk_KaiGai:
            //if(DeviceTool.SecondsTiming==0)
            //{
               // SetMasterBoardInPut(0);
              //  Set_MasterLowConsManage(ENABLE);
               // DeviceTool.SecondsTiming = 0; //将延时增加3秒
              //  MeterUartDataFormFrame(OPT_READ, DEV_TESTCONNECT);//发送协议以读取开盖电平
             //   DelayMs(3000);
             //   DeviceTool.StatusCheck = chk_CheckFinish;
             //   OM_SendUsartData(Debug_COM, DebugBuf, sprintf((char*)DebugBuf,"\r\n========开盖检测=========\r\n"));
          //  }
          //  break;
        case chk_CheckFinish:
           // Set_MasterLowConsManage(DISABLE);//使能主控板进入低功耗
           //OM_Close_Usart(UP_COM);
           //SetUsbUartTXRXIN();
            IO_HALL_A_0;//关闭电磁铁，长时间通电会发热
            IO_HALL_B_0;//关闭电磁铁，长时间通电会发热
            IO_CARD_ERR_IN;// 外部电路是 【3.3V—51R—22R—IO_CARD_ERR_1】，输出低电平电流会很大
            IO_CARD_XB_IN;
            IO_119_OUT;
            IO_119_1;
            Debug(0x99);
            DeviceTool.StatusCheck = chk_end;
            while(times--)
         {
            if(CheckResult.DeviceID == IDNUM)
            {
                if(CheckResult.DeviceID==1)
                    DelayMs(200);
                else if(CheckResult.DeviceID==2)
                    DelayMs(400);
                else if(CheckResult.DeviceID==3)
                    DelayMs(600);
                else if(CheckResult.DeviceID==4)
                    DelayMs(800);

              }
           // OM_SendUsartData(Debug_COM, DebugBuf, sprintf((char*)DebugBuf,"\r\n=主动上报查询结果===\r\n"));
              //  PCUartDataFormFrame(OPT_READ, DEV_GETCHECK_RESULT);//回应给{
              // OM_SendUsartData(Debug_COM, DebugBuf, sprintf((char*)DebugBuf,"\r\n==结果已出可查询==\r\n")); // add by mjl
            }
         //  if(CheckResult.DeviceID == IDNUM)
           // {
             //   OM_SendUsartData(Debug_COM, DebugBuf, sprintf((char*)DebugBuf,"\r\n=主动上报查询结果===\r\n"));
             //  PCUartDataFormFrame(OPT_READ, DEV_GETCHECK_RESULT);//回应给{
            //   DelayMs(IDNUM * 400);
            //    PCUartDataFormFrame(OPT_READ, DEV_GETCHECK_RESULT);
            //   DelayMs(IDNUM * 400);
            //    PCUartDataFormFrame(OPT_READ, DEV_GETCHECK_RESULT);
           //     DelayMs(IDNUM * 400);
           //     PCUartDataFormFrame(OPT_READ, DEV_GETCHECK_RESULT);

        //   }
            break;
        case  chk_end:
            break;
        default:break;
    }
}


/* 设置连接端口连接 */
unsigned char SetConnectMasterBoard(unsigned char *data)
{
    unsigned char len = 0;
    memcpy(&data[len], Meter.Num, 7);                                           len += 7;
    memcpy((unsigned char *)&data[len], (unsigned char *)&RTC, sizeof(RTC));    len += 6;
    return len;
}

/* 设置主控板IO输出 */
unsigned char SetMasterOut(unsigned char *data, unsigned char outtype)
{
    unsigned char len = 1;
#ifdef    DEF_VALVE_CHECK
    data[len++] = IO_VALVE; data[len++] = outtype;//停止阀门动作
#endif

    if((CheckResult.IsOrNoWithIterm & 0x02) == 0x02)//带IC卡
    {
        data[len++] = IO_CARD_SDA; data[len++] = outtype;//icKA  sda
        data[len++] = IO_CARD_SCL; data[len++] = outtype;//IC card scl
        data[len++] = IO_CARD_RST; data[len++] = outtype;//IC card rst
        //data[len++] = IO_CARD_POW; data[len++] = outtype;//IC card pow
    }
#if 0
    if((CheckResult.IsOrNoWithIterm & 0x10) == 0x10)//温压
    {
        data[len++] = IO_TP_SDA; data[len++] = outtype;// TP sda
        data[len++] = IO_TP_SCL; data[len++] = outtype;// TP scl
        data[len++] = IO_TP_POW; data[len++] = outtype;// TP pow
    }
#endif
    if(CheckResult.MeterTYP == 1)//如果是光电表
    {
#ifdef DEF_ZDUART_CHECK
        
#else
       // data[len++] = IO_GUANDIAN_IN; data[len++] = outtype;
        data[len++] = IO_GUANDIAN_POW; data[len++] = outtype;
#endif
    }
    data[0] = (len - 1) / 2;//端口数量
    return len;
}

/* 接收连接结果 */
unsigned char RecvConnectResult(unsigned char *data)
{
    unsigned char len = 0;
    CheckResult.MeterTYP = data[len++];
    CheckResult.IsOrNoWithIterm = data[len++];
    CheckResult.MasterVoult = data[len++];
    CheckResult.Status.Morden = data[len++];
    CheckResult.ValueSingal = data[len++];
    CheckResult.Status.Connect = 1;//data[len++];  //modify by mjl 
    len++;
    CheckResult.Status.SimCard = data[len++];
    CheckResult.Status.EEPROM = data[len++];
    CheckResult.Status.Measure = data[len++];
#ifdef DEF_ZDUART_CHECK
#else
    if(CheckResult.MeterTYP != 2)
        CheckResult.Status.Measure = 1;
#endif
    CheckResult.SoftVer1 = data[len++];
    CheckResult.SoftVer2 = data[len++];
    CheckResult.Status.Rtc = data[len++];
    CheckResult.Status.TempPress = data[len++];
    len += 10;
     // 解析开盖状态（协议位置24，对应 data[23]）
    //CheckResult.Status.CoverOpen = data[len++];
    //len = 19;
    //Debug(CheckResult.Status.CoverOpen);
    //Debug(0x88);
    //len+=1;
    // 解析蓝牙状态
   // CheckResult.Status.Bluetooth = 1;
    return len;
}
/*接收读取IMEI以及IMSI的结果**/
unsigned char FindIMEIandIMSI_where(unsigned char *data)
{
    unsigned char len = 0;
    unsigned int weizhi1 = 0;
    unsigned int weizhi2 = 0;
    weizhi1 = data[len+44];
    weizhi2 = data[weizhi1+len+45];
    int i = 0;
    memcpy(CheckResult.FindIMEI,&data[weizhi1 + weizhi2 + len +53] , 20);
    memcpy(CheckResult.FindIMSI,&data[weizhi1 + weizhi2 + len +155], 20);
    //CheckResult.charkbat = 0x00;
}
/*接收读取IMEI、IMSI、ICCID和软件版本编译时间的结果**/
unsigned char FindIMEIandIMSIandICCID_where(unsigned char *data)
{
    unsigned int offset = 0;
    memcpy(CheckResult.FindIMEI,&data[offset] ,15);
    offset+=15;
    memcpy(CheckResult.FindIMSI,&data[offset] ,15);
    offset+=15;
    memcpy(CheckResult.FindICCID,&data[offset] ,20);
    offset += 20;
    offset += 46;
    memcpy(CheckResult.SoftVertime,&data[offset] ,6);
}

/*接收读取星闪MAC地址的结果**/
void ParseStarStatusToCheckResult(const unsigned char *data)
{
    unsigned int offset = 0;
    unsigned int mac[12] = {0x38,0x38,0x38,0x38,0x38,0x38,0x38,0x38,0x38,0x38,0x38,0x38};
    // ? 主电电压（2字节，大端，单位0.01V）→ 转为0.1V写入 MasterVoult
    //unsigned short voltage_raw = (data[offset] << 8) | data[offset+1];
    //CheckResult.MasterVoult = (unsigned char)(voltage_raw / 10); // 0.01V → 0.1V
    offset += 2;

    // ? 星闪MAC地址（12字节ASCII）
    memcpy(CheckResult.StarMac, &data[offset], 12);
    if(CheckResult.StarMac[0] == 0 && CheckResult.StarMac[1] == 0 && CheckResult.StarMac[2] == 0)
    {
        memset(CheckResult.StarMac,0x38,12);
    }
    offset += 12;
}
/* 设置主控板输入高低 */
unsigned char SetMasterBoardInPut(unsigned char type)
{
    unsigned char len = 0;
#ifdef    DEF_DAOWEI_CHECK
    IO_VALVE_DAOWEI1_OUT;
    if(type == 1)
    {
        IO_VALVE_DAOWEI1_1;
    }
    else
    {
        IO_VALVE_DAOWEI1_0;
    }
#endif
    // 光电 霍尔 共用
    IO_HALL_A_OUT;
    if(type == 1)
    {
        IO_HALL_A_0;
    }
    else
    {
        IO_HALL_A_1;
    }
    
    if(CheckResult.MeterTYP == 0)//如果是霍尔
    {
        IO_HALL_B_OUT;
        if(type == 1)
        {
            IO_HALL_B_1;
        }
        else
        {
            IO_HALL_B_0;
        }
    }
    
    if( (CheckResult.IsOrNoWithIterm & 0x02) == 0x02)//带IC卡
    {
        IO_CARD_XB_OUT;
        if(type == 1)
        {
            IO_CARD_XB_1;
        }
        else
        {
            IO_CARD_XB_0;
        }
        IO_CARD_ERR_OUT;
        if(type == 1)
        {
            IO_CARD_ERR_1;
        }
        else
        {
            IO_CARD_ERR_0;
        }
    }
    if( (CheckResult.IsOrNoWithIterm & 0x01)  == 0x01)//带119
    {
//        IO_119_OUT;
//        if(type == 1)
//        {
//            IO_119_1;
//        }
//        else
//        {
//            IO_119_0;
//        }
    }
    return len;
}

/* 获取主控板输出IO的状态 */
unsigned char GetMasterOutIO_Status(unsigned char type)
{
    VALVE_R1_IN;
    VALVE_B1_IN;
    VALVE_R2_IN;
    VALVE_B2_IN;

    CARD_SDA_IN;
    CARD_SCL_IN;
    CARD_RST_IN;

    TP_SDA_IN;
    TP_SCL_IN;
    TP_POW_IN;

    ZD_POW_IN;
    ZD_SIGNAL_IN;

    DelayMs(100);

#ifdef    DEF_VALVE_CHECK
    if(type == 1)
    {
        if(!VALVE_R1_STATUS)
        {
            CheckResult.Status.Valve = 0;
            OM_SendUsartData(Debug_COM, DebugBuf, sprintf((char*)DebugBuf,"\r\n--->>>Valve - R1-1 Err\r\n"));
        }
        if(VALVE_B1_STATUS)
        {
            CheckResult.Status.Valve = 0;
            OM_SendUsartData(Debug_COM, DebugBuf, sprintf((char*)DebugBuf,"\r\n--->>>Valve - B1-0 Err\r\n"));
        }
    }
    else
    {
        if(VALVE_R1_STATUS)
        {
            CheckResult.Status.Valve = 0;
            OM_SendUsartData(Debug_COM, DebugBuf, sprintf((char*)DebugBuf,"\r\n--->>>Valve - R1-0 Err\r\n"));
        }
        if(!VALVE_B1_STATUS)
        {
            CheckResult.Status.Valve = 0;
            OM_SendUsartData(Debug_COM, DebugBuf, sprintf((char*)DebugBuf,"\r\n--->>>Valve - B1-1 Err\r\n"));
        }
    }
#endif

    if((CheckResult.IsOrNoWithIterm & 0x02) == 0x02)
    {
        if(CARD_SDA_STATUS != type)
        {
            CheckResult.Status.IcCard = 0;
            OM_SendUsartData(Debug_COM, DebugBuf, sprintf((char*)DebugBuf,"\r\n--->>>IC Card - SDA Err\r\n"));
        }
        if(CARD_SCL_STATUS != type)
        {
            CheckResult.Status.IcCard = 0;
            OM_SendUsartData(Debug_COM, DebugBuf, sprintf((char*)DebugBuf,"\r\n--->>>IC Card - SCL Err\r\n"));
        }
        if(CARD_RST_STATUS != type)
        {
            CheckResult.Status.IcCard &= 0;
            OM_SendUsartData(Debug_COM, DebugBuf, sprintf((char*)DebugBuf,"\r\n--->>>IC Card - RST Err\r\n"));
        }
//        if(CARD_POW_STATUS != type)
//        {
//            CheckResult.Status.IcCard = 0;
//        }
    }
#if 0
    if((CheckResult.IsOrNoWithIterm & 0x10) == 0x10) //温压
    {
        if(TP_SDA_STATUS != type)
        {
            CheckResult.Status.TempPress = 0;
            OM_SendUsartData(Debug_COM, DebugBuf, sprintf((char*)DebugBuf,"\r\n--->>>TP - SDA Err\r\n"));
        }
        if(TP_SCL_STATUS != type)
        {
            CheckResult.Status.TempPress = 0;
            OM_SendUsartData(Debug_COM, DebugBuf, sprintf((char*)DebugBuf,"\r\n--->>>TP - SDA Err\r\n"));
        }
        if(TP_POW_STATUS != type)
        {
            CheckResult.Status.TempPress = 0;
            OM_SendUsartData(Debug_COM, DebugBuf, sprintf((char*)DebugBuf,"\r\n--->>>TP - POW Err\r\n"));
        }
    }
#endif

    if(CheckResult.MeterTYP == 1)//如果是光电
    {
#ifdef DEF_ZDUART_CHECK
        
#else
        if(ZD_POW_STATUS != type)
        {
            CheckResult.Status.Measure = 0;
        }
        
//        if(ZD_SIGNAL_STATUS != type)
//        {
//            CheckResult.Status.Measure = 0;
//        }
        
#endif
    }
    return 1;
}

/* 获取主控板输入IO口的状态 */
unsigned char GetMasterInPutIO_Status(unsigned char *data, unsigned char intype)
{
    unsigned char len = 0;

    if( (DeviceTool.StatusCheck != chk_SetMsterBoardOutLow ) && (DeviceTool.StatusCheck != chk_SetMsterBoardOutHight))
            return 0;

#ifdef DEF_DAOWEI_CHECK
    if(data[len++] != intype)
    {
        if(CheckResult.MeterTYP != 2)//霍尔,光电
        {
            CheckResult.Status.Valve = 0;//开到位
        }
    }
    if(data[len++] != intype)
    {
//        CheckResult.Status.Valve = 0;//关到位
    }
#else                            //add by mjl
    len += 2;
#endif
    if(intype == 1)
    {
        if(data[len++] != 0)
        {
            //if(CheckResult.MeterTYP == 0)//霍尔 光电
            {
                Debug(0x01);
                CheckResult.Status.Measure = 0;//霍尔1
                OM_SendUsartData(Debug_COM, DebugBuf, sprintf((char*)DebugBuf,"\r\n--->>>Hall - H1-1 Err\r\n"));
            }
        }
        if(data[len++] != 1)
        {
            if(CheckResult.MeterTYP == 0)//霍尔
            {
                Debug(0x02);
                CheckResult.Status.Measure = 0;//霍尔2
                OM_SendUsartData(Debug_COM, DebugBuf, sprintf((char*)DebugBuf,"\r\n--->>>Hall - H2-0 Err\r\n"));
            }
        }
    }
    else
    {
        if(data[len++] != 1)
        {
            //if(CheckResult.MeterTYP == 0)//霍尔  光电
            {
                Debug(0x03);
                CheckResult.Status.Measure = 0;//霍尔1
                OM_SendUsartData(Debug_COM, DebugBuf, sprintf((char*)DebugBuf,"\r\n--->>>Hall - H1-0 Err\r\n"));
            }
        }
        if(data[len++] != 0)
        {
            if(CheckResult.MeterTYP == 0)//霍尔
            {
                Debug(0x04);
                CheckResult.Status.Measure = 0;//霍尔2
                OM_SendUsartData(Debug_COM, DebugBuf, sprintf((char*)DebugBuf,"\r\n--->>>Hall - H2-1 Err\r\n"));
            }
        }
    }

    if(data[len++] != intype)
    {
        if((CheckResult.IsOrNoWithIterm & 0x02) == 0x02)
        {
            CheckResult.Status.IcCard = 0;//CARD XB
            OM_SendUsartData(Debug_COM, DebugBuf, sprintf((char*)DebugBuf,"\r\n--->>>IC Card - XB Err\r\n"));
        }
    }
    if((CheckResult.IsOrNoWithIterm & 0x01) == 0x01)
    {
        if(intype == 1)
        {
            if(data[len++] == 0) //这里是工装给高电平时119的检测状态，为低
            {
                IO_119_0;
            }
            else
            {
                CheckResult.Status.Io119 = 0;//119
            }
        }
        else
        {
            if(data[len++] == 1) //这里是工装给低电平时119的检测状态，为高，只有当119检测状态测试过高低时才算正常
            {
                CheckResult.Status.Io119 = 1;//119
            }
            else
            {
                CheckResult.Status.Io119 = 0;//119
            }
        }
    }
/*
    if(data[len++] != intype)
    {
        if((CheckResult.IsOrNoWithIterm & 0x01) == 0x01)
        {
            CheckResult.Status.Io119 = 0;//119
            OM_SendUsartData(Debug_COM, DebugBuf, sprintf((char*)DebugBuf,"\r\n--->>> 119 Err\r\n"));
        }
    }
 */
    if(data[len++] != intype)
    {
        if((CheckResult.IsOrNoWithIterm & 0x02) == 0x02)
        {
            CheckResult.Status.IcCard = 0;//CARD ERR

            if( intype == 1 )
                OM_SendUsartData(Debug_COM, DebugBuf, sprintf((char*)DebugBuf,"\r\n--->>>IC Card - ERR-1  Err\r\n"));
            else
                OM_SendUsartData(Debug_COM, DebugBuf, sprintf((char*)DebugBuf,"\r\n--->>>IC Card - ERR-0  Err\r\n"));
        }
    }
    len += 2;
    return len;
}

/* 读取上告结果 */
unsigned char ReadCheckResult(unsigned char *data)
{
    unsigned char len = 0;
    if(CheckResult.MeterTYP == 0)//如果是霍尔
    {

    }
    else if(CheckResult.MeterTYP == 1)//光电
    {

    }
    
    if((CheckResult.IsOrNoWithIterm & 0x02) != 0x02)//不带IC卡
    {
        CheckResult.Status.IcCard = 1;
    }
    if((CheckResult.IsOrNoWithIterm & 0x01) != 0x01)//不带119
    {
        CheckResult.Status.Io119 = 1;
    }

#if  defined(DEF_DAOWEI_CHECK) || defined(DEF_VALVE_CHECK)

#else
    CheckResult.Status.Valve = 1;
#endif

#ifndef DEF_TEMP_PRESS
    CheckResult.Status.TempPress = 1;
#else

#endif


    memcpy(data, &CheckResult.DeviceID, sizeof(CHECKPRESULT_S));

    len = sizeof(CHECKPRESULT_S);
    return len;
}

