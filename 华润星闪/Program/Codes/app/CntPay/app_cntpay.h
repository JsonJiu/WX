#ifndef _APP_CNTPAY_H_
#define _APP_CNTPAY_H_

#include "include.h"

//配置支持小数点位数，1000000000 即1元 = 1.000000000
#define DecNum       1000000000


typedef enum
{
   STATE_IDLE = 0,   //空闲
   STATE_Busy,       //忙碌
   STATE_Done,       //完成
}t_CntPayDeviceState;

typedef enum
{
   STATUS_HALL_NORMAL = 0,    //正常
   STATUS_HALL_SINGLE,        //单边异常
   STATUS_HALL_DUAL,          //双边异常
}t_HALLSTATE;

typedef enum
{
   GATEERR_Null = 0,
   GATEERR_Leak,              //漏气
   GATEERR_OverFlow,          //过流
   GATEERR_UnUsedTimeOver,    //闲置
}t_GATEERR;


typedef struct
{
   t_HALLSTATE       StatusHall;    //霍尔状态
   unsigned char     HallACount;    //霍尔A计数
   unsigned char     HallBCount;    //霍尔B计数
   unsigned char     PulseCount;    //有效脉冲数
}St_Hall;



typedef struct
{
   unsigned char Year;        //年
   unsigned char Month;       //月
   unsigned char Day;         //日
   unsigned char Hours;       //时
}St_Time;

typedef struct
{
    unsigned char    ctrl;
    St_Time          Date;
    unsigned char    Data[4];
    unsigned char    State[2];
    unsigned short   Crc16;
}StuFreezeData;


typedef union
{
   unsigned short value;
   struct
   {
      unsigned short day    : 5;
      unsigned short month  : 4;
      unsigned short year   : 7;
   };
}u_Day;

typedef union
{
   unsigned char value;
   struct
   {
      unsigned char Time         : 5;     //时间
      unsigned char Model        : 2;     //模型
      unsigned char MonthOrDay   : 1;     //月/日计量周期
   };
}u_Cycle;


typedef struct
{
   struct{
      unsigned char PriceDec[2];    //小数
      unsigned char PriceInt[2];    //整数
   }StepPrice;
   struct{
      unsigned char AmountDec;      //小数
      unsigned char AmountInt[3];   //整数
   }StepAmount;
}St_Table;


typedef struct
{
   unsigned char  type;          //模型类型
   unsigned char  mode;          //更新方式
   u_Cycle        cycle;         //阶梯周期
   St_Time        Time;          //执行时间
   St_Table       table[5];      //价格表
   u_Day          StartDay;      //起始日期
   unsigned long  PriceIndex;    //调价序号

} StuStepPrice, *pStuStepPrice;


typedef struct
{
   StuStepPrice      StepPriceNow;     //当前价格阶梯
   unsigned short    NewActive;      //新阶梯是否有效
   StuStepPrice      StepPriceNew;     //新的价格阶梯
   unsigned char     StepUseTime;    //已计费时间-月/日-由阶梯周期决定
   unsigned char     NewStep;
   unsigned long     tprice;           //与上阶梯单价价格差
   St_Hall           Hall;
   RTC_T             NextTime;         //下一次清除时间
   RTC_T             FreezeTime;
}St_CNTPAY;

typedef struct
{
   unsigned long     NextStepRemain;   //下一阶梯剩余气量，带2位小数
}St_CNTPAYRUN;


extern const unsigned char newstepprice[];
  
   
void JustReduce(long tReduceInt,long tReduceDec);
void Charging(void);
void CntPayCharge(unsigned long n);
unsigned short ReCharge(long money, unsigned short index);
void CheckStepNow(void);
unsigned char CheckHall(void);
void HallInit(void);
//计量设备
void CntPayDeviceInit(void);
t_CntPayDeviceState CntPayGetState(void);

//冻结
unsigned short MeterFreeze_Read(RTC_T time,unsigned char *buf, unsigned char n);
unsigned char MeterFreeze_Ctrl(RTC_T time);
unsigned char Freeze_Up(void);
void FreshFreezeState(void);



void Cntpay_Test(void);
void CntpayRegDataSvr(void);
unsigned char UpdataStep(StuStepPrice *p);
void RefreshRemainVal(void);
unsigned char UpdataStepNow(StuStepPrice *p);
unsigned char ReadStep(StuStepPrice *p, unsigned char t);

void SetErrCode(t_GATEERR t);
t_GATEERR GetErrCode(void);
void SetUpDefault(void);
void ClearLeakTime(void);
unsigned char GetLeakTime(void);
void ClearUnUsedTime(void);
void StepCycleCheck(void);
void RePowerUp(void);
void RefreshStepNextTime(void);


void ChangeRTCEvent(void);
void OneSecondGone(void);
void OneMinuGone(void);
void OneHourGone(void);
void OneDayGone(void);
#endif

