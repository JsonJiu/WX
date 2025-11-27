#include "include.h"


StuMarkFault    Fault;      //错误告警，上电初始化，不需要存储
StuMarkState    State;      //表具状态，上电初始化，不需存储
StuSetup        Setup;      //配置参数，上电初始化，不需存储
StuMeter        BakMeter;   //储存参数时校验用 

StuMeter        Meter;      //表具的数据，需要存储

RTC_T           RTC;

const St_RegTxt MeterRegTag[] = {
    {sizeof(Meter.MeterID         ), "表    号"},
    {sizeof(Meter.BootLoader      ), "升级标志"},
	{sizeof(Meter.CntPay_Device	  ), "计量设备"},
    {sizeof(Meter.State           ), "表具状态"},
    {sizeof(Meter.ConsumeCount    ), "待扣费数"},
    
    {sizeof(Meter.UnuseCloseTimes ), "闲置次数"},

    {sizeof(Meter.Amount          ), "累计气量"},
    {sizeof(Meter.PeriodAmountGas ), "周期用量"},
    {sizeof(Meter.Negtiveuse      ), "负用气量"},
    {sizeof(Meter.Price           ), "当前单价"},
    {sizeof(Meter.RemainInt       ), "剩余整数"},
    {sizeof(Meter.RemainDec       ), "剩余小数"},
    {sizeof(Meter.RemainVol       ), "剩余气量"},
    {sizeof(Meter.Overdraught     ), "透支气量"},
    {sizeof(Meter.Totaladd        ), "存入总量"},
    {sizeof(Meter.ChargeAmount    ), "充值气量"},

    {sizeof(Meter.Warning         ), "报警气量"},
    {sizeof(Meter.Maxflow         ), "流量上限"},
    {sizeof(Meter.Maxremain       ), "表存上限"},
    {sizeof(Meter.MaxAmount       ), "累计上限"},
    {sizeof(Meter.UnusedCloseTime ), "闲置时间"},

    {sizeof(Meter.PriceIndex      ), "调价序号"},
    {sizeof(Meter.ChargeIndex     ), "充值序号"},
    
    {sizeof(Meter.FreezeDate      ), "冻结时间"} 
};



void DataSvrDefault(void)
{
    RTC_T   T_RTC;
  
//    Meter.Amount = 0;        //累计气量
//    Meter.PeriodAmountGas = 0;  //周期内的使用气量
//    Meter.Negtiveuse = 0;       //负用量
//    Meter.Price = 10000;            //当前单价(3位小数)
//    Meter.RemainInt = 0;           //剩余量
//    Meter.RemainDec = 0;          //小数（5位）
//    Meter.Totaladd = 0;         //存入总量(从表生下来开始的充值总量)
//
//    Meter.Maxremain = 9999999;                //表存上限
//    Meter.UnusedCloseTime = 0;          //闲置关阀时间（小时）
//    Meter.Overdraught = 0;              //透支量 1位小数
//    Meter.Warning = 0;                  //报警量 1位小数
//    Meter.ChargeAmount = 0;             //当前存入量
//    Meter.PriceIndex = 0;
//    Meter.Maxflow = 0;                  //流量上限
//    Meter.UnuseCloseTimes=0;            //闲置关阀次数
//    Meter.ChargeIndex = 0;

    mem_set((unsigned char *)&Meter, sizeof(Meter), 0x00);
    
    SetValue(BASEVAL_Price, 10000);     //单价
    SetValue(BASEVAL_RemainInt,0);
    SetValue(BASEVAL_Negtiveuse,0);

    SetValue(USERVAL_Warning, 99);
    SetValue(USERVAL_Overdraught, 0);
    SetState(STATE_REMAIN, STATE_REMAIN_NORMAL);
    SetValue(USERVAL_Maxremain, 9999999);
    SetValue(USERVAL_MaxAmount, 9999999);
    T_RTC.Year = 0x14;
    T_RTC.Mon = 0x12;
    T_RTC.Day = 0x20;
    T_RTC.Hour = 0x12;
    T_RTC.Min = 0x12;
    T_RTC.Sec = 0x12;
    SetDate(T_RTC);
    mem_set(Meter.MeterID, 0x11, sizeof(Meter.MeterID));
    Meter.FreezeDate.Days = 0x28;
    Meter.FreezeDate.Hours = 0x23;
    Meter.FreezeDate.Minus = 0x55;
    SetValue(BASEVAL_RemainInt,0);
}


const St_RegTxt SetupRegTag[] = {
    {sizeof(Setup.MeterType)            ,"表类型"},
    {sizeof(Setup.PostPaid)             ,"计量开关"},
    {sizeof(Setup.CntPay)               ,"阶梯开关"},
    {sizeof(Setup.Precision)            ,"计量精度"},
    {sizeof(Setup.OverFlow)             ,"过流开关"},
    {sizeof(Setup.UnusedClose)          ,"闲置开关"},
    {sizeof(Setup.CloseWarning)         ,"少气关阀"},
    {sizeof(Setup.CloseOver)            ,"透支关阀"},
    {sizeof(Setup.Freeze)               ,"冻结开关"}
};


/**
 * 通用参数申请空间
 * 
 * @author xsc (2014-12-17)
 */
void RegGeneralData(void)
{
    St_SAVESTRUCT temp;
    temp.fun = SetUpDefault;
    temp.data = (unsigned char *)&Setup;
    temp.len = sizeof(Setup);
    DataSvrReg(sizeof(SetupRegTag) / 9, &temp, (St_RegTxt *)&SetupRegTag);

    CntpayRegDataSvr();

    temp.fun = DataSvrDefault;
    temp.data = (unsigned char *)&Meter;
    temp.len = sizeof(Meter);
    DataSvrReg(sizeof(MeterRegTag) / 9, &temp, (St_RegTxt *)&MeterRegTag);
}

/**
 * 获取表号
 * 
 * @author xsc (2014/12/9)
 * 
 * @param id 缓冲区 
 * @param len 
 */
void GetMeterID(unsigned char *id, unsigned char len)
{
    copy_memery(id, Meter.MeterID, len);
}

/**
 * 设置表号
 * 
 * @author xsc (2014/12/9)
 * 
 * @param id 
 * @param len 
 */
void SetMeterID(unsigned char *id, unsigned char len)
{
    copy_memery(Meter.MeterID, id, len);
}



/**
 * 设置表具基础运行值
 * BASEVAL_AmountGas        累计气量    正数，2位小数
 * BASEVAL_Price            单价        正数，3位小数
 * BASEVAL_Remain           剩余量      有符号数，两位小数
 * BASEVAL_Totaladd         总充值量    正数，2位小数
 */
unsigned char SetValue(T_Value ValueType, long val)
{
    unsigned char re = TRUE;
    switch(ValueType)
    {
    case BASEVAL_Amount:     //累计气量整数
        Meter.Amount = val;
        break;
    case BASEVAL_PeriodAmountGas:   //周期内的用气量
        Meter.PeriodAmountGas = val;
        break;
    case BASEVAL_Price:         //当前单价(3位小数)
        Meter.Price = val;
        break;
    case BASEVAL_Remain:        //剩余量(2位小数)
        if(val >= 0)
        {
            Meter.Negtiveuse = 0;
            Meter.RemainInt = val / 100;
            Meter.RemainDec = (val % 100) * (DecNum / 100);
        }
        else
        {
            val *= -1;
            Meter.Negtiveuse = val / 100 + 1;
            Meter.RemainInt = 0;
            Meter.RemainDec = (100 - val % 100) * (DecNum / 100);
        }
        RefreshRemainVal();
        break;
    case BASEVAL_RemainInt:        //剩余量整数
        Meter.RemainInt = val;
        break;

    case BASEVAL_RemainDec:         //剩余量小数
        Meter.RemainDec = val;
        break;
    case BASEVAL_Totaladd:      //存入总量(从表生下来开始的充值总量)
        Meter.Totaladd = val;
        break;
    case BASEVAL_RemainVol:
        Meter.RemainVol = val;
        break;
    case BASEVAL_Negtiveuse:
        Meter.Negtiveuse = val;
        break;
    case USERVAL_Maxremain:     //表存上限
        Meter.Maxremain = val;
        break;

    case USERVAL_MaxAmount:     //累计上限
        Meter.MaxAmount = val;
        break;

    case USERVAL_UnusedCloseTime:   //闲置关阀时间（小时）
        Meter.UnusedCloseTime = val;
        break;
    case USERVAL_Overdraught:   //透支量 2位小数
        Meter.Overdraught = val;
        RefreshRemainVal();
        break;
    case USERVAL_Warning:       //报警量 2位小数
        Meter.Warning = val;
        RefreshRemainVal();
        break;
    case USERVAL_ChargeAmount:  //当前存入量
        Meter.ChargeAmount = val;
        break;
    case USERVAL_Maxflow:       //流量上限
        Meter.Maxflow = val;
        break;
    case USERVAL_ChargeIndex:
        Meter.ChargeIndex = val;
        break;
    case USERVAL_PriceIndex:
        Meter.PriceIndex = val;
        break;

    default:
        re = FALSE;
        break;
    }
    return re;
}


/**
 * 获取表具基础运行值
 *  
 * BASEVAL_AmountGas        累计气量     正数，2位小数
 * BASEVAL_Price            单价        正数，3位小数
 * BASEVAL_Remain           剩余量      有符号数，两位小数
 * BASEVAL_Totaladd         总充值量    正数，2位小数 
 * USERVAL_Maxremain        表存上限 
 * USERVAL_UnusedCloseTime  闲置关阀时间（小时）
 * USERVAL_Overdraught      透支量 2位小数
 * USERVAL_Warning:         //报警量 2位小数 
 * USERVAL_ChargeAmount:    //当前存入量 
 * USERVAL_Maxflow:         //流量上限 
 * USERVAL_ChargeIndex:     //充值序号 
 * USERVAL_PriceIndex:      调价次数 
 */
long GetValue(T_Value ValueType)
{
    long re = 0;
    switch(ValueType)
    {
    case BASEVAL_Amount:     //累计气量(2位小数)
        re = Meter.Amount;
        break;
    case BASEVAL_PeriodAmountGas:   //周期内的用气量
        re = Meter.PeriodAmountGas;
        break;
    case BASEVAL_Price:         //当前单价(3位小数)
        re = Meter.Price; 
        break;

    case BASEVAL_Remain:            //剩余量(2位小数)
        if (Meter.Negtiveuse > 0) {
            re = 0 - (Meter.Negtiveuse * 100 - (Meter.RemainDec) / (DecNum / 100));
        }else{
            re = Meter.RemainInt * 100 + Meter.RemainDec / (DecNum / 100);
        }
        break;

    case BASEVAL_RemainInt:        //剩余量整数
        re = Meter.RemainInt;
        break;
    case BASEVAL_RemainDec:        //剩余量小数
        re = Meter.RemainDec;
        break;
    case BASEVAL_RemainVol:
        RefreshRemainVal();
        re = Meter.RemainVol;
        break;
    case BASEVAL_Negtiveuse:
        re = Meter.Negtiveuse;
        break;
    case BASEVAL_Totaladd:      //存入总量(从表生下来开始的充值总量)
        re = Meter.Totaladd;
        break;
    case BASEVAL_Fault:
        re=Fault.Value;
        break;
    case USERVAL_Maxremain:     //表存上限
        re = Meter.Maxremain;
        break;
    case USERVAL_MaxAmount:     //累计上限
        re = Meter.MaxAmount;
        break;

    case USERVAL_UnusedCloseTime:   //闲置关阀时间（小时）
        re = Meter.UnusedCloseTime;
        break;
    case USERVAL_Overdraught:   //透支量 2位小数
        re = Meter.Overdraught;
        break;
    case USERVAL_Warning:       //报警量 2位小数
        re = Meter.Warning;
        break;
    case USERVAL_ChargeAmount:  //当前存入量
        re = Meter.ChargeAmount;
        break;
    case USERVAL_Maxflow:       //流量上限
        re = Meter.Maxflow;
        break;
    case USERVAL_ChargeIndex:   //充值序号
        re = Meter.ChargeIndex;
        break;
    case USERVAL_PriceIndex:
        re = Meter.PriceIndex;
        break;

    default:
        re = 0;
        break;
    }
    return re;
}

/**
 * 获取状态 
 * STATE_POWER  电源状态 
 * STATE_LCD    液晶状态 
 * STATE_DOOR   阀门状态 
 * STATE_CARD   卡状态 
 * STATE_REMAIN 存量状态 
 * STATE_METER  表状态 
 * @author xsc (2014/11/18)
 * 
 * @param tstate 
 * 
 * @return T_StateVal 
 */
T_StateVal GetState(T_State tstate)
{
    T_StateVal re;
    switch(tstate)
    {
    case STATE_REMAIN:
        re = Meter.State;
        break;
    case STATE_MEASURE:
        re = State.Measure;
        break;
    case STATE_FREEZE:
        re = Meter.Freezeed;
        break;
    default:
        re = STATE_ERR;
        break;
    }
    return re;
}

/**
 * 设置状态 
 * STATE_POWER  电源状态 
 * STATE_LCD    液晶状态 
 * STATE_DOOR   阀门状态 
 * STATE_CARD   卡状态 
 * STATE_REMAIN 存量状态 
 * STATE_METER  表状态 
 * 
 * @author xsc (2014/11/18)
 * 
 * @param tstate 
 * @param val 
 * 
 * @return T_StateVal 
 */
T_StateVal SetState(T_State tstate, T_StateVal val)
{
    T_StateVal re = val;
    switch(tstate)
    {
    case STATE_REMAIN:
        Meter.State = val;
        break;
    case STATE_MEASURE:
        State.Measure = val;
        break;
    case STATE_FREEZE:
        Meter.Freezeed = val;
        break;
    default:
        re = STATE_ERR;
        break;
    }
    return re;
}

/** 
 * 获取表配置信息 
 * SETUP_MeterType 表类型 
 * SETUP_Precision 计量精度 
 * 
 * @author xsc (2014/11/18)
 * 
 * @param tSetup 
 * 
 * @return T_SetupVal 
 */
T_SetupVal GetSetupVal(T_Setup tSetup)
{
    T_SetupVal re = SETUP_VAL_OTHER;
    switch(tSetup)
    {
    case SETUP_MeterType:
        re = Setup.MeterType;
        break;
        
    case SETUP_PostPaid:
        re = Setup.PostPaid;
        break;
        
    case SETUP_CntPay:
        re = Setup.CntPay;
        break;

    case SETUP_Precision:
        re = Setup.Precision;
        break;

    case SETUP_OverFlow:
        re = Setup.OverFlow;
        break;
        
    case SETUP_UnusedClose:
        re = Setup.UnusedClose;
        break;
        
    case SETUP_CloseWarning:
        re = Setup.CloseWarning;
        break;
        
    case SETUP_CloseOver:
        re = Setup.CloseOver;
        break;
        
    case SETUP_Freeze:
        re = Setup.Freeze;
        break;
        
    default:
        break;
    }
    return re;
}

/**
 * 设置表配置信息 
 *  
 * SETUP_MeterType 表类型 SETUP_Precision 计量精度 
 * SETUP_CloseWarning 强制关阀 
 * 
 * @author xsc (2014/11/18)
 * 
 * @param tSetup 
 * @param val 
 * 
 * @return T_SetupVal 
 */
T_SetupVal SetSetupVal(T_Setup tSetup, T_SetupVal val)
{
    T_SetupVal re = SETUP_VAL_OTHER;
    switch(tSetup)
    {
    case SETUP_MeterType:
        Setup.MeterType = val;
        if (Setup.MeterType == SETUP_MeterType_GAS) //若为气量式，单价强制设置为1元
        {
            SetValue(BASEVAL_Price, 10000);
        }else{  //若修改为金额式，重新根据阶梯刷新价格
            CheckStepNow();
        }
        break;

    case SETUP_PostPaid:
        Reset_ValveControlRight(R_CONSUME); //修改表类型时，主动清除金额阀门事件；
        Setup.PostPaid = val;
        break;

    case SETUP_CntPay:
        Setup.CntPay = val;
        break;

    case SETUP_Precision:
        Setup.Precision = val;
        break;

    case SETUP_OverFlow:
        if (GetValue(USERVAL_Maxflow) == 0) {
            return Setup.OverFlow;
        }
        Setup.OverFlow = val;
        break;

    case SETUP_UnusedClose:
        if (GetValue(USERVAL_UnusedCloseTime) < (7 * 24)) {
            return Setup.UnusedClose;
        }
        Setup.UnusedClose = val;
        break;

    case SETUP_CloseWarning:
        Setup.CloseWarning = val;
        break;

    case SETUP_CloseOver:
        Setup.CloseOver = val;
        break;

    case SETUP_Freeze:
        Setup.Freeze = val;
        break;

    default:
        break;
    }
    return re;
}

/**
 * 读取冻结时间
 * 
 * @author xsc (2014/12/9)
 * 
 * @param time 
 * 
 * @return unsigned char 
 */
unsigned char GetFreezeTime(pStuFreezeTime time)
{
    copy_memery((unsigned char *)time, (unsigned char *)&Meter.FreezeDate, sizeof(StuFreezeTime));
    return TRUE;
}

/**
 * 设置冻结数据
 * 
 * @author xsc (2014/12/9)
 * 
 * @param time 
 * 
 * @return unsigned char 
 */
unsigned char SetFreezeTime(pStuFreezeTime time)
{
    copy_memery((unsigned char *)&Meter.FreezeDate, (unsigned char *)time, sizeof(StuFreezeTime));
    return TRUE;
}

/**
 * 获取系统时间
 * 
 * @author xsc (2014/11/28)
 * 
 * @return StuDate* 
 */
void GetDate(RTC_T* t)
{
    copy_memery((unsigned char *)t,(unsigned char *)&RTC, sizeof(RTC_T));
}

/**
 * 更新系统时间
 * 
 * @author xsc (2014/11/28)
 * 
 * @param date 
 */
void SetDate(RTC_T date)
{
    copy_memery((unsigned char *)&RTC, (unsigned char *)&date, sizeof(RTC_T));
}

const unsigned char para_month[13]={0,0,3,3,6,1,4,6,2,5,0,3,5};        //星期月参变数
const unsigned char month[12]={31,28,31,30,31,30,31,31,30,31,30,31};

/**
 * 判断2月是 28 还是 29
 * 
 * @author 谢尚川 (2014-03-31)
 * 
 * @param year 判断的年份
 * 
 * @return unsigned char 1:29天；0:28天
 */
unsigned char CheckFeb(unsigned long year)
{
    if ((year % 100) == 0) {
        if ((year % 400) == 0) {
            return TRUE;
        }else{
            return FALSE;
        }
    }else
        if ((year % 4) == 0) {
            return TRUE;
        }else{
            return FALSE;
        }
}

/**
 * 查询月份天数
 * 
 * @author xsc (2015-02-03)
 * 
 * @param m 
 * 
 * @return unsigned char 
 */
unsigned char DaysInThisMonth(unsigned char m)
{
    return month[m];
}



/**
 * 软RTC线程
 * 
 * @author xsc (2014-04-10)
 */
void SoftRTC_Ns_Task(unsigned char t)
{
    unsigned char temp;
    unsigned char i;
    
    for (i = 0; i < t; i++)
    {
        temp = CharBCDtoHEX(RTC.Sec);
        temp ++;
        if ( temp == 60 ) { //秒进位
            RTC.Sec = 0;
            temp = CharBCDtoHEX(RTC.Min);
            temp ++;
            if ( temp == 60 ) {//分进位
                RTC.Min = 0;
                temp = CharBCDtoHEX(RTC.Hour);
                temp ++;
                if (temp == 24) {
                    RTC.Hour = 0;
                    temp = RTC.Mon;
                    AddOneDay(&RTC);
                    
                }else{
                    RTC.Hour = ByteHEXtoBCD(temp);
                }
            }else{
                RTC.Min = ByteHEXtoBCD(temp);
            }
        }else{
            RTC.Sec = ByteHEXtoBCD(temp);
        }
    }
}


