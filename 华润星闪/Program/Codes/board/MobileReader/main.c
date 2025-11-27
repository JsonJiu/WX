
/* Includes ------------------------------------------------------------------*/

#include "include.h"
#include "main.h"

#include "user_manage.h"
#include "task_manage.h"
#include "timer_manage.h"

extern unsigned short RF_OptDelayTime;
extern unsigned short Debug_in , Debug_out;

#ifdef DEBUG
char DebugMsgBuff[UART_BUFF_LEN];
#endif
unsigned long SYS_IdleTOPowerDownTime = AUTOPOWERDOWNTIME;

void Delay10Us(unsigned short us)
{
    unsigned int t;
    while(us--)
    {
        for( t = 0; t < 10; t++)
            ;
    }
}

void Task_UserTimer(void)
{
    if(COM_Buffer.TimeOver > 0)
        COM_Buffer.TimeOver--;

    if(WiFi_Buffer.TimeOver > 0)
        WiFi_Buffer.TimeOver--;

    if(RF_Buffer.TimeOver > 0)
        RF_Buffer.TimeOver--;

    if(RFTiming)
        RFTiming--;

    if(RF_OptDelayTime)
        RF_OptDelayTime--;

    if(SYS_IdleTOPowerDownTime)
        SYS_IdleTOPowerDownTime--;

   
}

unsigned char TaskNum_WiFi_HW = 0;
unsigned char TaskNum_WiFi_SW = 0;
unsigned char TaskNum_RF_SW = 0;
unsigned char TaskNum_RF_HW = 0;
unsigned char TaskNum_CMD_SW = 0;
unsigned char TaskNum_StateManage = 0;
unsigned char TaskNum_UserTimer = 0;

#define USE_CPF
/*******************************************************************************
  * 功能：  Main program.
  * 参数：  None
  * 返回：  None
*******************************************************************************/
int main(void)
{
    MCUPllCfg();
    MCU_IntEn();
    HardwareInit();               //硬件初始化
    User_Init();
    //Example();
#ifdef USE_CPF
  #ifndef DEBUG
    TaskNum_WiFi_HW = User_AskTimeNumber();    //定时器编号在使用前需申请，如果返回值为0，表明已超出最大定时器数
    if(TaskNum_WiFi_HW != 0)
    {
            User_StartTime(MAINTIMER, TaskNum_WiFi_HW, 100, Task_WiFi_HW);
    }
  #endif
    TaskNum_WiFi_SW = User_AskTimeNumber();    //定时器编号在使用前需申请，如果返回值为0，表明已超出最大定时器数
    if(TaskNum_WiFi_SW != 0)
    {
            User_StartTime(MAINTIMER, TaskNum_WiFi_SW, 5, Task_WiFi_SW);
    }

    TaskNum_RF_SW = User_AskTimeNumber();    //定时器编号在使用前需申请，如果返回值为0，表明已超出最大定时器数
    if(TaskNum_RF_SW != 0)
    {
            User_StartTime(MAINTIMER, TaskNum_RF_SW, 5, Task_RF_SW);
    }

    TaskNum_RF_HW = User_AskTimeNumber();    //定时器编号在使用前需申请，如果返回值为0，表明已超出最大定时器数
    if(TaskNum_RF_HW != 0)
    {
            User_StartTime(MAINTIMER, TaskNum_RF_HW, 10, Task_RF_HW);
    }

//    TaskNum_CMD_SW = User_AskTimeNumber();    //定时器编号在使用前需申请，如果返回值为0，表明已超出最大定时器数
//    if(TaskNum_CMD_SW != 0)
//    {
//            User_StartTime(MAINTIMER, TaskNum_CMD_SW, 50, Task_CMD_SW);
//    }

    TaskNum_StateManage = User_AskTimeNumber();    //定时器编号在使用前需申请，如果返回值为0，表明已超出最大定时器数
    if(TaskNum_StateManage != 0)
    {
            User_StartTime(MAINTIMER, TaskNum_StateManage, 10, Task_StateManage);
    }

    TaskNum_UserTimer = User_AskTimeNumber();    //定时器编号在使用前需申请，如果返回值为0，表明已超出最大定时器数
    if(TaskNum_UserTimer != 0)
    {
            User_StartTime(MAINTIMER, TaskNum_UserTimer, 1, Task_UserTimer);
    }
#endif

#ifdef DEBUG
        Printf_DebugMessage("System Running... \r\n");
#endif
    
   while (1U)
   {
#ifdef USE_CPF
      User_TimerManage();
      User_TaskManage();

//      LowPowerConsManage();
#else
      
    #ifdef DEBUG
        Task_WiFi_HW();
    #endif
        Task_WiFi_SW();
        Task_RF_SW();
        //Task_RF_HW();
        //RF_SW_FSM();
    #ifdef DEBUG
        Task_CMD_SW();
    #endif
        //Task_StateManage();
#endif
   }

}



