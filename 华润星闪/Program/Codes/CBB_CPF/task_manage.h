#ifndef __TASK_MANAGE_H
#define __TASK_MANAGE_H

#include "timer_manage.h"
#ifndef FALSE
#define FALSE		0
#endif
#ifndef TRUE
#define TRUE		1
#endif
          
#define NOTASK      0
#define RUNTASK     1
	
typedef struct TASK_LIST
{
  unsigned char phead;      		//task phead
  unsigned char ptail;                  //task ptail 
}StrTaskList;
typedef struct TASK
{
   ISR_FUNC_PTR callback;    		//task callback  
}StrTask;
void User_InitTaskManage(void);
unsigned char GetTaskSts(void);
unsigned char IsSysTaskRun(void);
void ResetTaskSts(void);

#define MAXTASKNUM            16
#define TASKCOVER             //当任务缓存区满了之后，覆盖最早添加的任务       
//#define TASKIGNORE          //当任务缓存区满了之后，不再新增任务 
void User_KillSelfTask(void);
unsigned char User_AddTask(ISR_FUNC_PTR CallBack);
void User_TaskManage(void);
#endif

