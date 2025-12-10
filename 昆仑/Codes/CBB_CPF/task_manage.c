#include "task_manage.h"

StrTaskList strTaskList;
StrTask strTask[MAXTASKNUM];
unsigned char taskType = NOTASK;
unsigned char taskNumber = 0;

/**
 * 任务初始化  
 * 
 * @author linyx (2014/10/31)
 */
void User_InitTaskManage(void)
{
   strTaskList.phead = 0;
   strTaskList.ptail = 0;
   taskType = NOTASK;
   taskNumber = 0;
}
unsigned char IsSysTaskRun(void)
{
    if((strTaskList.phead == strTaskList.ptail)&&(taskType==NOTASK)) return FALSE;
    return TRUE;
}
/**
 * 
 * 
 * @author linyx (2014/11/4)
 * 
 * @return unsigned char 
 */
unsigned char GetTaskSts(void)
{
   return (taskType);
}
/**
 * 
 * 
 * @author linyx (2014/11/4)
 */
void ResetTaskSts(void)
{
   taskType = NOTASK;
}
/**
 * 
 * 
 * @author linyx (2014/11/4)
 * 
 * @return unsigned char 
 */
void User_KillRunTask(void)
{
   ResetTaskSts();
}
/**
 * 
 * 
 * @author linyx (2014/11/4)
 * 
 * @param type 
 * @param CallBack 
 * 
 * @return unsigned char 
 */
unsigned char User_AddTask(ISR_FUNC_PTR CallBack)
{
#ifdef TASKCOVER
   strTask[strTaskList.phead].callback = CallBack;
   strTaskList.phead++;
   if (strTaskList.phead >= MAXTASKNUM)   strTaskList.phead = 0;
   if (strTaskList.phead == strTaskList.ptail)   strTaskList.ptail++;
   if (strTaskList.ptail >= MAXTASKNUM)   strTaskList.ptail = 0;
#endif
#ifdef TASKIGNORE
   if ((strTaskList.phead+1) == strTaskList.ptail)||((strTaskList.phead+1) == (strTaskList.ptail+MAXTASKNUM)) return FALSE;
   strTask[strTaskList.phead].callback = CallBack;
   strTaskList.phead++;
   if (strTaskList.phead >= MAXTASKNUM)   strTaskList.phead = 0;
#endif 
   return TRUE;
}

/**
 * 
 * 
 * @author linyx (2014/11/13)
 * 
 * @return unsigned long 
 */
//unsigned long User_AskTaskBit(void)
//{
//   unsigned char taskno;
//   taskno = taskNumber;
//   if (taskno > 31)  return 0;
//   taskNumber++;
//   return (1<<taskno);
//}
//void User_SetTaskBit(unsigned long type)
//{
//   Systask |= type;
//}
//void User_ClrTaskBit(unsigned long type)
//{
//   Systask &=~ type;
//}
/**
 * 任务执行
 * 
 * @author linyx (2014/10/31)
 */
void User_TaskManage(void)
{ 
//   if (taskType == RUNTASK) return;                      //任务正在执行时，则返回
//   if (strTaskList.phead != strTaskList.ptail) {         //头尾指针不等，表示有任务
//      taskType = RUNTASK;                                //置任务运行标识
//      strTask[strTaskList.ptail].callback();             //执行先添加的任务
//      strTaskList.ptail++;                               //删除先添加的任务
//      if (strTaskList.ptail > MAXTASKNUM){
//          strTaskList.ptail = 0;
//      }
//   }
}      



