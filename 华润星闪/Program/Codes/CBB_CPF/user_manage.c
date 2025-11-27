#include "user_manage.h"
#include "task_manage.h"
#include "timer_manage.h"

unsigned char test1ms, test1s;

void User_Init(void)
{
   User_InitTimerManage();
   User_InitTaskManage();
   User_TimerCfg(MAINTIMER, TIME_1_MS,   MAINTIMERADDONCE);
   User_TimerCfg(SUBTIMER,  TIME_1_S, SUBTIMERADDONCE);
   User_TimerStart(MAINTIMER);
   User_TimerStart(SUBTIMER);
}
/*
void TASK1(void)
{
    int i;
    for(i=0; i<5; i++);
    //User_CloseTime(test1ms);
}
void TASK2(void)
{
    int i;
    for(i=0; i<5; i++);
    User_CloseTime(test1s);
}
 */
/*
void Example(void)
{
   test1ms = User_AskTimeNumber();    //定时器编号在使用前需申请，如果返回值为0，表明已超出最大定时器数    
   if(test1ms!=0){
       User_StartTime(MAINTIMER, test1ms, 1000, TASK1);
   }
   test1s = User_AskTimeNumber();    //定时器编号在使用前需申请，如果返回值为0，表明已超出最大定时器数
   if(test1s!=0){
       User_StartTime(SUBTIMER, test1s, 5, TASK2);
   }
}
 */

//使用User_Add_Task函数调用      
//使用User_StartTime函数调用    

void LowPowerConsManage(void)
{
     if((IsSysTimeRun()==FALSE)&&(IsSysTaskRun()==FALSE))
     {
             MCU_SleepEnter();
     }
}
