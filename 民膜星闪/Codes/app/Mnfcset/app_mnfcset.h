#ifndef _SET_TASK_H_
#define _SET_TASK_H_



#ifndef UARTTYPE_MUART
#define UARTTYPE_MUART          1 //模拟串口
#define UARTTYPE_HUART          2 //硬件串口
#endif


/******************** Config *****************************/

//串口类型
#define UARTTYPE         UARTTYPE_HUART
//超时时间
#define SettingTimeOut      10      //秒
//
#define SettingBuffSize     100

/******************** Config *****************************/

typedef enum
{
    SetState_Start = 0, //启动任务
    SetState_WaitCmd,   //等待接收指令
    SetState_RunCmd,    //执行指令
    SetState_End,
}t_SetState;

typedef struct
{
    unsigned char SetCmd[SettingBuffSize];       //缓存
    unsigned char pStart;
    unsigned short SetCmdLen;
    unsigned short pEnd;
    unsigned char TimeOut;
    unsigned char RxOk;
    t_SetState State;
}Stu_Set;



void SetConfigRx(unsigned char d);
void Set_TimeTick_1s(void);
void SettingInit(void);
void SettingRun(void);
void SettingEnd(void);
void SettingStart(void);
#endif

