#include "timer_manage.h"
StrTimeinf strTimeinf[MAXTIMERNUM+1];
unsigned char timeNo = 0;
unsigned long sysTime[8] = {0, 0, 0, 0, 0, 0, 0, 0};
void MAINTIMERADDONCE(void)
{
   unsigned char temptimenum;
   for (temptimenum = 1; temptimenum <= timeNo; temptimenum++)
   {
      if ((strTimeinf[temptimenum].timeswitch == TIMERON)&&((strTimeinf[temptimenum].timetype == MAINTIMER)))
      {
         strTimeinf[temptimenum].count++;
      }
   }
}
void SUBTIMERADDONCE(void)
{
   unsigned char temptimenum;
   for (temptimenum = 1; temptimenum <= timeNo; temptimenum++)
   {
      if ((strTimeinf[temptimenum].timeswitch == TIMERON)&&((strTimeinf[temptimenum].timetype == SUBTIMER)))
      {
         strTimeinf[temptimenum].count++;
      }
   }
} 
/**
 * 
 * 
 * @author linyx (2014/11/4)
 * 
 * @param timeType 
 * @param timeNumber 
 * @param timeLen 
 * @param CallBack 
 * 
 * @return unsigned char 
 */
unsigned char User_StartTime(unsigned char timeType, unsigned char timeNumber, unsigned long timeLen, ISR_FUNC_PTR CallBack) 
{
    if(timeType >= TIMERNUM) return FALSE;
    if (timeNumber == 0) return FALSE;
    if (timeNumber <= timeNo)
    {
        if(timeType == MAINTIMER) sysTime[(timeNumber-1)/32] |= ((unsigned long)1<<((timeNumber-1)%32));      //
        strTimeinf[timeNumber].timetype = timeType;
        strTimeinf[timeNumber].timernum = timeNumber;
        strTimeinf[timeNumber].timeswitch = TIMERON;
        strTimeinf[timeNumber].timeLen = timeLen;
        strTimeinf[timeNumber].count = 0;
        strTimeinf[timeNumber].callback = CallBack;
        return TRUE;
    }
    return FALSE;
}
/**
 * 
 * 
 * @author linyx (2014/11/4)
 * 
 * @param timeType 
 * @param timeNumber 
 * 
 * @return unsigned char 
 */
unsigned char User_CloseTime(unsigned char timeNumber)
{
    if (timeNumber == 0) return FALSE;
    if (timeNumber <= timeNo)
    {
        if(strTimeinf[timeNumber].timetype == MAINTIMER) sysTime[(timeNumber-1)/32] &=(~((unsigned long)1<<((timeNumber-1)%32)));      //
        strTimeinf[timeNumber].timetype = 0;
        strTimeinf[timeNumber].timernum = 0;
        strTimeinf[timeNumber].timeswitch = TIMEROFF;
        strTimeinf[timeNumber].timeLen = 0;
        strTimeinf[timeNumber].count = 0;
        strTimeinf[timeNumber].callback = NULL;
        return TRUE;
    }
    return FALSE;
}
unsigned char IsSysTimeRun(void)
{
    unsigned char i, len;
    len = (timeNo%32)?((timeNo/32)+1):(timeNo/32);
    for(i=0; i<len; i++)
    {
        if(sysTime[i]!=0) return TRUE;
    }
    return FALSE;
}
/**
 * 请求一个定时器序号  
 * 
 * @author linyx (2014/11/13)
 * 
 * @return unsigned char 
 */
unsigned char User_AskTimeNumber(void)
{
   if (timeNo < MAXTIMERNUM) {   //定时器号从0开始，每次申请，先加一再返回  
      timeNo++;
      return(timeNo);
   }
   else if (timeNo >= MAXTIMERNUM){ //超过最大定时器设定数则不再添加    
      return 0;
   }
   return 0;
}
/**
 * 
 * 
 * @author linyx (2014/10/31)
 */
void User_InitTimerManage(void)
{
    unsigned char temptimenum;
    for (temptimenum = 0; temptimenum <= MAXTIMERNUM; temptimenum++)
    {
     strTimeinf[temptimenum].timetype = 0;
     strTimeinf[temptimenum].timernum = 0;
     strTimeinf[temptimenum].timeswitch = TIMEROFF;
     strTimeinf[temptimenum].timeLen = 0;
     strTimeinf[temptimenum].count = 0;
     strTimeinf[temptimenum].callback = NULL;
    }
}
/**
 * 
 * 
 * @author linyx (2014/10/31)
 */
void User_TimerManage(void)
{
    unsigned char temptimenum;
    for (temptimenum = 1; temptimenum <= timeNo; temptimenum++)
    {
        if (strTimeinf[temptimenum].timeswitch == TIMERON)
        {
            if (strTimeinf[temptimenum].count >= strTimeinf[temptimenum].timeLen)
            {
                strTimeinf[temptimenum].count = 0;
                strTimeinf[temptimenum].callback();
            }
        }
    }
}

