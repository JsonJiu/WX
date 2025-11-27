#include "config.h"

unsigned char RTC_TIMER_NUM = 0;
unsigned char USB_SETTIMERNUM = 0;
unsigned char WAKEUP_TIMENUM = 0;

unsigned char DebugBuf[600];

RTC_T   RTC;
METERSTATUS_ST Meter;
/**
 * 设置所有IO口为输出低
 */
void MCUAllIOSetLow(void)
{
    ANSA = 0;   TRISA = 0;  LATA = 0;   ODCA = 0;
    ANSB = 0;   TRISB = 0;  LATB = 0;   ODCB = 0;
                TRISC = 0;  LATC = 0;   ODCC = 0;
    ANSD = 0;   TRISD = 0;  LATD = 0;   ODCD = 0;
    ANSE = 0;   TRISE = 0;  LATE = 0;   ODCE = 0;
                TRISF = 0;  LATF = 0;   ODCF = 0;
    ANSG = 0;   TRISG = 0;  LATG = 0;   ODCG = 0;
}


/**************************************RTC************************************/
/**
 * 不带周校验的时间校验
 * @param pTime:时间
 * @return
 */
int Check_Time_Format(RTC_T *pTime)
{
    if( ( (pTime->Year & 0x0f) > 0x09 ) ||
        ( (pTime->Year       ) > 0x99 ) ||
        ( (pTime->Mon & 0x0f ) > 0x09 ) ||
        ( (pTime->Mon        ) > 0x12 ) ||
        ( (pTime->Mon        ) ==0x00 ) ||
        ( (pTime->Day & 0x0f ) > 0x09 ) ||
        ( (pTime->Day        ) > 0x31 ) ||
        ( (pTime->Day        ) ==0x00 ) ||
        ( (pTime->Hour & 0x0f) > 0x09 ) ||
        ( (pTime->Hour       ) > 0x23 ) ||
        ( (pTime->Min & 0x0f ) > 0x09 ) ||
        ( (pTime->Min        ) > 0x59 ) ||
        ( (pTime->Sec & 0x0f ) > 0x09 ) ||
        ( (pTime->Sec        ) > 0x59 ) )
        return FALSE;
    return TRUE;
}

/**
 * 读取RTC任务
 */
void Task_ReadRTC(void)
{
    RTCReadTime(&RTC);  //内部硬件RTC
    if(Check_Time_Format(&RTC) == FALSE)    //读取时钟格式异常处理,不校验闰年及周
    {
    }
}

/**
 * RTC配置
 */
void Init_RTC(void)
{
    RTCCfg();
    if(RTC_TIMER_NUM == 0)
    {
        RTC_TIMER_NUM = User_AskTimeNumber();
    }
    Task_ReadRTC();
    User_StartTime(SUBTIMER, RTC_TIMER_NUM, 10, Task_ReadRTC);
}


/**
 * 校时处理函数
 * @param time：时间
 */
unsigned char CheckRtcTime_Programe(unsigned char *time)
{
    if( Check_Time_Format((RTC_T *)time) == TRUE )
    {
        memcpy(&RTC, &time[0], 6);
        RTCWriteTime(&RTC);
        return TRUE;
    }
    else
    {
        return FALSE;
    }
}

/******************************************************************************/
/**
 * 读RTC时间值
 * @param buf：时间缓冲区；
 * @return：返回数据长度；
 */
unsigned char ReadTimeValue(unsigned char *buf)
{
    unsigned char len = 0;
    RTCReadTime(&RTC);
    memcpy(&buf[len], (unsigned char *)&RTC.Year, 6);
    len += 6;
    return len;
}

