#ifndef __TIME_MANAGE_H
#define __TIME_MANAGE_H

#ifndef NULL
#define NULL (void *)0
#endif
#ifndef FALSE
#define FALSE		0
#endif
#ifndef TRUE
#define TRUE		1
#endif

typedef void (*ISR_FUNC_PTR)(void);

void MAINTIMERADDONCE(void);
void SUBTIMERADDONCE(void);
unsigned char User_StartTime(unsigned char timeType, unsigned char timeNumber, unsigned long timeLen, ISR_FUNC_PTR CallBack);
unsigned char User_CloseTime(unsigned char timeNumber);
unsigned char IsSysTimeRun(void);
unsigned char User_AskTimeNumber(void);
void User_InitTimerManage(void);
void User_TimerManage(void);

#define MAXTIMERNUM 				30			//MAX 最大值254，大于254会出错！！！ 

#define AAAAAA 1
#define BBBBBB 2

#define TIMERNUM        2
#define MAINTIMER       0
#define SUBTIMER        1
#define TIMERON     	1
#define TIMEROFF    	0
#define TIME_1_MS	0
#define TIME_100_MS	1
#define TIME_1_S        2
typedef  struct TIMERINF
{
  unsigned char timetype;               //timer type
  unsigned char timernum;      		//timer number
  unsigned char timeswitch;    		//timer switch  on/off
  unsigned long timeLen;       		//timer length
  unsigned long count;         		//timer count
  ISR_FUNC_PTR callback;    		//timer callback;
}StrTimeinf;

extern StrTimeinf strTimeinf[MAXTIMERNUM+1];
extern void User_TimerCfg(unsigned char type, unsigned char len, ISR_FUNC_PTR CallBack); 
extern void User_TimerStart(unsigned char type);
extern void User_TimerStop(unsigned char type);
#endif
