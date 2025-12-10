#include "config.h"

/**
 * 低功耗配置
 */
void LowConsumeCfg(void)
{
    MCUAllIOSetLow();
}

extern unsigned char IDNUM;
/**
 * 初始化设备
 */
void Init_Device(void)
{
    SetCheckConsumptionPower(DISABLE);//断开检测主控板功耗的电源
    DelayMs(100);
    SetMasterBoardPower(ENABLE);//打开主控板电源

    CheckResult.DeviceID = GetDeviceID();
    IDNUM  =  CheckResult.DeviceID;

    Ask_DeviceToolsCheck_TimerNum();

    LedOrBeepTick((LED_TypeDef)(GREEN_LED) , TICK_COUNT(1), TICK_ON_1MS(500), TICK_OFF_1MS(500));
    if(CheckResult.DeviceID == IDNUM)
    {
        DebugPrintf("\r\n 系统启动2...\r\n");
        //printf("\r\n 系统启动3...\r\n");
    }

    DeviceTool.StatusCheck = chk_Halt;
}

/**
 * MCU外设初始化
 */
void Init_Periphera(void)
{
    Init_RTC();

    Init_DebugUsart();
    Init_UpUsart();

#ifdef DEF_IRDA
    Init_MeterUsart();
#endif
    Init_IrdaUsart();
    Init_DeviceID();
    Init_SencondTimer();

    IO_HALL_A_OUT;
    IO_HALL_B_OUT;
    IO_HALL_A_0;
    IO_HALL_B_0;
}

/**
 * MCU初始化
 */
void MCU_Init(void)
{
    Init_Periphera();
    Init_Device();
}

unsigned char sts_Hall = 0;
void Task_Hall(void)
{
    switch( sts_Hall )
    {
        case 0:
            IO_HALL_A_OUT;
            IO_HALL_B_OUT;
            IO_HALL_A_0;
            IO_HALL_B_0;
            sts_Hall++;
            break;
        case 1:
            IO_HALL_A_1;
            DelayMs(200);
            IO_HALL_A_0;
            sts_Hall++;
            break;
       case 2:
            IO_HALL_B_1;
            DelayMs(200);
            IO_HALL_B_0;
            sts_Hall = 1;
            break;
    }
}

unsigned char Hall_TIMER_NUM = 0;
void Ask_Hall_TimerNum(void)
{
    if(Hall_TIMER_NUM == 0)
    {
        Hall_TIMER_NUM = User_AskTimeNumber();
    }
    User_StartTime(MAINTIMER, Hall_TIMER_NUM, 1000, Task_Hall);
}


/**
 * 主程序入口：main
 * @return
 */
int main(void)
{
   
    MCUPllCfg();
    MCU_IntEn();
    User_Init();
    MCU_Init();
   // Ask_Hall_TimerNum();

    while (1U)
    {
        User_TimerManage();
        User_TaskManage();
        
        Process_AutoCheck();

        MeterUartDataDealProcess();
        PCUartDataDealProcess();
        DebugUartDataDealProcess();
        IrdaUartDataDealProcess();
         
    }
}
