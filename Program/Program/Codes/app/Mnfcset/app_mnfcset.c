/**********************************************************************
功能说明： 
    本应用用于设置表具参数，具体命令集见Command.c
使用指南： 
    系统调用：
        启动        void SetRun(void)
        任务运行     void SettingTask(void)
        定时器1秒    void Set_TimeTick_1s(void)

**********************************************************************/

#include "app_mnfcset.h"
#include "Command.h"
#include "app_datasvr.h"
#include "define.h"
#include "include.h"

Stu_Set SetCache;


void Set_TimeTick_1s(void)
{
    if(SetCache.TimeOut && SetCache.RxOk == 0)
    {
        SetCache.TimeOut--;
        if (SettingTimeOut == 0) {
            SetCache.SetCmdLen = 0;
        }
    }
}

void OV_LcdDispNumWithDot(long num,char dots);
void SetConfigRx(unsigned char d)
{
    if(SetCache.RxOk) return;
    
    SetCache.SetCmd[SetCache.SetCmdLen++] = d;
    SetCache.TimeOut = 2;


    unsigned char result;
    
    result = CheckSysCommand(SetCache.SetCmd, &SetCache.SetCmdLen);
    if (result < 0xE0) {
        copy_memery(SetCache.SetCmd, SetCache.SetCmd + result, (SetCache.SetCmdLen - result));
        SetCache.SetCmdLen -= result;
    }
    if (result == 0xFE) { //命令正确
        SetCache.RxOk = 1;
    }
    if (result == 0xE0) {   //起始位错误
        copy_memery(SetCache.SetCmd, SetCache.SetCmd + 1, --SetCache.SetCmdLen);
    }
    if (SetCache.RxOk) {
        SetCache.State = SetState_RunCmd;
    }
    
    
}

void SettingStart(void)
{
    Uart0ReceiveEnable();
}

void SettingEnd(void)
{
    Uart0ReceiveDisable();
}


void SettingInit(void)
{
    UartMode uartconfig;
  
    uartconfig.Baud = UartBaud9600;
    uartconfig.Parity = 0;     //偶校验
    uartconfig.DataBit = 1;    //8位数据
    uartconfig.StopBit = 1;    //1停止位
    #if UARTTYPE == UARTTYPE_HUART
        Uart0Config((pUartMode)&uartconfig, IrqServerDefaultRS, SetConfigRx);
        SettingEnd();
        //GetTag();
        //Uart_Init(uartconfig, SetConfigRx);
    #else
        Muart_Init(uartconfig, SetConfigRx);
        Muart_RxEn();
    #endif
}


void SettingRun(void)
{
    unsigned long temp;
  
    if(SetCache.RxOk == 1)
    {
        temp = RunSysCommand(SetCache.SetCmd);
        if(temp > 0) {
            //准备发送数据
            temp = MakeSend(SetCache.SetCmd, temp);
            //发送数据
            #if UARTTYPE == UARTTYPE_HUART
                Uart0SendData(SetCache.SetCmd, temp);
            #else
                Muart_Write_Byte();
            #endif
        }
        SetCache.RxOk = 0;
        SetCache.SetCmdLen = 0;
    }

}


    /*
        if (DeviceCommand.SetConfig) {
            temp = RunSysCommand(SetCache.SetCmd, SetCache.SetCmdLen, 0);
            if(temp) {
                BeepMs(10);
                temp = MakeSend(SetCache.SetCmd, temp);
                SetSendBuffer(SetCache.SetCmd, temp);
                SetCache.SetCmdLen = 0;
                SetCache.TimeOut = 0;
            }
            DeviceCommand.SetConfig = 0;
            DeviceRun.SetConfig = 0;
            SetCache.State.start = 0;
            SetStop();
        }
    }else{    //超时未抄作，退出
        DeviceRun.SetConfig = 0;
        SetCache.State.start = 0;
        SetStop();
        DeviceRun.Lcd = 1;
    }*/

