/* 
 * File:   temp.h
 * Author: wb
 *
 * Created on December 8, 2015, 9:08 AM
 */

#ifndef TEMP_H
#define	TEMP_H

#ifdef	__cplusplus
extern "C" {
#endif

/****************************************LED灯**********************************/
    //定时基数为1mS
#define TICK_COUNT(X)		X		//TICK的次数

#define TICK_ON_1MS(X)		X               
#define TICK_ON_1S(X)		(1000*X)

#define TICK_OFF_1MS(X)         X
#define TICK_OFF_1S(X)		(1000*X)

/**
 *  表具参数信息
 */
typedef struct
{
    unsigned char Num[20];          //表具编号
    unsigned char Time[6];          //表具时间
}METERSTATUS_ST;
extern METERSTATUS_ST Meter;

/******************************************************************************/
extern RTC_T    RTC;
extern unsigned char DebugBuf[600];

/********************************定时器号任务申请*************************/

void MCUAllIOSetLow(void);
int Check_Time_Format(RTC_T *pTime);
void Task_ReadRTC(void);
void Init_RTC(void);
unsigned char CheckRtcTime_Programe(unsigned char *time);
unsigned char ReadTimeValue(unsigned char *buf);


#ifdef	__cplusplus
}
#endif

#endif	/* TEMP_H */

