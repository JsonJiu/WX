
#ifndef _RF_TASK_H_
#define _RF_TASK_H_

typedef enum
{
    RF_MODE_DEEPSLEEP = 0,
    RF_MODE_SLEEP,
    RF_MODE_IDLE,
    RF_MODE_SEND,
    RF_MODE_RECIVE,
}T_RFSTATE;


void RFSendBuff(unsigned char *buf, unsigned short len);
unsigned short RFRecive(unsigned char *buf);
void RF_ModeSwitch(T_RFSTATE mode);
void Task_RF_HW(void);//为了兼容其它移动采集器，添加此空任务
#endif

