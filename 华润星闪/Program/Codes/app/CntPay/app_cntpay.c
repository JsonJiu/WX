#include "include.h"

/**************************************** 
使用方法： 
阶梯部分： 
 
 
定时每天0点0分0秒调用 void OneDayGone(void);用于 阶梯价格周期管理； 
定时每小时调用 void OneHourGone(void);用于 新阶梯价格表的更新； 
定时没分钟调用 void OneMinuGone(void);用于 冻结数据的实现； 
 
冻结： 
实现 GetMeterState(unsigned char *buf);用于冻结获取当前表状态； 
 
 
***********************************************/

St_CNTPAY Cntpay;
St_CNTPAYRUN CntpayRun;
unsigned short GasOnMinGo = 0;          //过流计算
unsigned short UnUsedTimeOut = 0;       //闲置时间(小时)
unsigned short LeakTime = 0;            //漏气关阀次数
unsigned short halltimeout;

#define HallAttackTimeOut    5*60       //磁攻击后 n分钟内关闭漏气保护功能


/////////////////////////////////extern/////////////////////////////////////////////

void RemainStateChangeEvent(T_StateVal state);





///////////////////////////////////// 霍尔 开始 ///////////////////////////////////////




unsigned char HallTimeNum;
void HallTimeOut1sTick(void)
{
    CheckHall();
    if(halltimeout)
        halltimeout--;
    else
       User_CloseTime(HallTimeNum);
}

/**
 * 霍尔触发后启动超时功能，防止磁攻击计数
 * 
 * @author xxx (2015/5/28)
 */
void HallTimerStart(void)
{
    halltimeout = HallAttackTimeOut;
    if (HallTimeNum == 0) {
       HallTimeNum = User_AskTimeNumber();
    }
    User_StartTime(SUBTIMER, HallTimeNum, 1, HallTimeOut1sTick);
}



unsigned char CheckHall(void)
{
    if(GpioHallARead() || GpioHallBRead())
    {
        //Reset_ValveControlRight(R_EVENT);
        SetState(STATE_MEASURE, STATE_MEASURE_NORMAL);
        return TRUE;
    }else{
        HallAttackEvent();
        HallTimerStart();
        //SetState(STATE_MEASURE, STATE_MEASURE_BROKEN);
        return FALSE;
    }
}


/**
 * 霍尔A中断服务
 * 
 * @author xsc (2014/11/26)
 */
void HallAInt(void)
{
    DelayMs(1);
    if((GpioHallARead()==0))
    {
        if(CheckHall()==TRUE)
        {
            if (halltimeout && Get_ValveStatus() == CLOSE_STATUS)
            {
                return;
            }
                Cntpay.Hall.HallACount ++;
        }
    }
}

/**
 * 霍尔B中断服务
 * 
 * @author xsc (2014/11/26)
 */
void HallBInt(void)
{
    DelayMs(1);
    if((GpioHallBRead()==0))
    {
        if(CheckHall()==TRUE)
        {
            if (halltimeout && Get_ValveStatus() == CLOSE_STATUS)
            {
                return;
            }
            Cntpay.Hall.HallBCount ++;
            if(Cntpay.Hall.HallACount)
            {
                Cntpay.Hall.HallACount = 0;
                Cntpay.Hall.HallBCount = 0;
                switch(Setup.Precision)
                {
                case SETUP_Precision_010:
                    Meter.ConsumeCount += 10;
                    break;
                case SETUP_Precision_100:
                    Meter.ConsumeCount += 100;
                    break;
                case SETUP_Precision_001:
                default:
                    Meter.ConsumeCount += 1;
                    break;
                }
                Charging();
            }
        }
    }
}


/**
 * 霍尔初始化
 * 
 * @author xsc (2014/11/27)
 */
void HallInit(void)
{
    HallASetInt(1, HallAInt);
    HallBSetInt(1, HallBInt);
    CheckHall();
}
#endif

///////////////////////////////////// 霍尔 结束 ///////////////////////////////////////


///////////////////////////////////// 直读头 开始 /////////////////////////////////////

///调用申明
void ZDInit(void);



///////////////////////////////////// 直读头 结束 /////////////////////////////////////

///////////////////////////////////// 计量统一接口 开始 ////////////////////////////////
/**
 * 计量初始化
 * 
 * @author xsc (2015-01-21)
 */
void CntPayDeviceInit(void)
{
	switch(StuMeter.CntPay_Device)
	{
		case CntPay_HALL:
			HallInit();
			break;
			
		case CntPay_ZD:
			ZDInit();
			break;
			
		default: break;
	}
}

/**
 * 检测计量设备是否损坏
 * 
 * @author xsc (2015-04-21)
 * 
 * @return T_StateVal 
 */
T_StateVal CntPayDeviceCheckErr(void)
{
	switch(StuMeter.CntPay_Device)
	{
		case CntPay_HALL:
			CheckHall();
			break;
			
		case CntPay_ZD:
			break;
			
		default: break;
	}

    return GetState(STATE_MEASURE);
}

t_CntPayDeviceState GetZDState(void)
{
    ZDSTATE state;
    unsigned long temp;
    state = ZDState();
    if(state == ZDSTATE_READED || state == ZDSTATE_ERR)
    {
        if(state == ZDSTATE_ERR)
        {
            SetState(STATE_MEASURE, STATE_MEASURE_BROKEN);
            CntPayDeviceErrEvent();
        }else{
            SetState(STATE_MEASURE, STATE_MEASURE_NORMAL);
//            if(ZDRun.amount > GetValue(BASEVAL_Amount))
//            {
//                temp = ZDRun.amount - GetValue(BASEVAL_Amount);
//            }else{
//                if ((9999999 - GetValue(BASEVAL_Amount)) < 200000) {
//                    temp = 9999999 - GetValue(BASEVAL_Amount) + ZDRun.amount;
//                }else{
//                    temp = 0;
//                }
//            }
//            if(temp > 200000)
//            {
//                ZDRun.amount = (GetValue(BASEVAL_Amount) / 1000000) * 1000000 + ZDRun.amount % 1000000;
//                if(GetValue(BASEVAL_Amount) % 1000000 < ZDRun.amount % 1000000)
//                {
//                    ZDRun.amount += 1000000;
//                }
//                SetValue(BASEVAL_Amount, ZDRun.amount);
//            }else{
//                if (temp != 0) {
                    SetValue(BASEVAL_Amount, ZDRun.amount);
//                }
//            }
        }
        return STATE_Done;
    }else{
        return STATE_Busy;
    }
}

/**
 * 获取计费设备状态
 * 
 * @author xsc (2015-01-21)
 * 
 * @return unsigned char 
 */
t_CntPayDeviceState CntPayGetState(void)
{

	switch(StuMeter.CntPay_Device)
	{
		case CntPay_HALL:
			return STATE_Done;
			break;
			
		case CntPay_ZD:
			GetZDState();
			break;
			
		default: break;
	}

}

/**
 * 用于超声波模块扣费接口
 * 
 * @author xsc (2015-04-09)
 * 
 * @param n 超声波的累计气量
 */
void CntPayCharge(unsigned long n)
{
    if (n > GetValue(BASEVAL_Amount)) {
        Meter.ConsumeCount = n - GetValue(BASEVAL_Amount);
    }else{
        if (GetValue(USERVAL_MaxAmount) - GetValue(BASEVAL_Amount) < 200000) {  //离最大累计气量相差2000m2以内才能翻转计算，否则抛弃数据
            Meter.ConsumeCount = n + GetValue(USERVAL_MaxAmount) - GetValue(BASEVAL_Amount);
        }
    }
    Charging();
}


void CntPayPowdown(void)
{
	switch(StuMeter.CntPay_Device)
	{
		case CntPay_HALL:
			return STATE_Done;
			break;
			
		case CntPay_ZD:
			ZDStop();
			break;
			
		default: break;
	}
}


void ReadCntPayDevice(void)
{
    unsigned short i, j;

    for(i = 0;i < 3; i++)	//最多尝试三次
    {
        for(j = 0; j < 30; j++)		//300ms一个周期
        {
            if(CntPayGetState() == STATE_Done)
            {
                CntPayPowdown();
                if(CntPayDeviceCheckErr() == STATE_MEASURE_NORMAL || CntPayDeviceCheckErr() == STATE_MEASURE_BROKEN)
                {
                    return;			//正常工作
                }else{
                    DelayMs(100);	//延时100ms用于直读头彻底掉电
                }
            }else{
                SetState(STATE_MEASURE, STATE_MEASURE_BROKEN);
            }
            DelayMs(10);
        }
    }
    CntPayPowdown();
}




///////////////////////////////////// 计量统一接口 结束 ////////////////////////////////


///////////////////////////////////// 冻结 开始 ///////////////////////////////////////
/**
 * 读冻结数据
 * 
 * @author 谢尚川 (2014-03-31)
 *  
 * @param time 起始时间
 * @param buf 冻结数据缓存
 * @param n   读取倒数几个月的冻结数据
 * 
 * @return unsigned short 返回长度 
 */
unsigned short MeterFreeze_Read(RTC_T time,unsigned char *buf, unsigned char n)
{
    unsigned char len = sizeof(StuFreezeData) - 2;
    unsigned short mon;
    //unsigned short mon;
    StuFreezeData *FreezeData;
    unsigned short i, succ = 0;
    RTC_T temptime;

    if(n > 24)
    {
        return 0;
    }

    FreezeData = (StuFreezeData *)buf;
    for(i = 0; i < n; i++)
    {
        //计算读取的是第几个月数据
        mon = CharBCDtoHEX(time.Year) % 2;
        mon = mon * 12;
        mon = mon + CharBCDtoHEX(time.Mon);
        //mon += i;
        if(mon > 24)
        {
            mon = mon - 24;
        }
        mon = mon * sizeof(StuFreezeData);

        ReadFreezeData(mon, (unsigned char *)FreezeData, sizeof(StuFreezeData));
        if (CrcCheck(FreezeData->Crc16, buf, len ) == TRUE) {
            copy_memery((unsigned char *)&temptime, (unsigned char *)&FreezeData->Date, sizeof(RTC_T));
            if(DateCmp(temptime, time, 2) == 0)
            {
                FreezeData += len;
                succ += len;
            }
        }
        time.Mon = charTobcd(CharBCDtoHEX(time.Mon) - 1);
        if(time.Mon == 0)
        {
            time.Year = charTobcd(CharBCDtoHEX(time.Year) - 1);
            time.Mon = 0x12;
        }
    }
    return succ;

}
/**
 * 修改指定月份冻结数据 ctrl字段为1
 * 
 * @author xsc (2015-01-29)
 * 
 * @param time 
 * 
 * @return unsigned char 
 */
unsigned char MeterFreeze_Ctrl(RTC_T time)
{
    unsigned char len = sizeof(StuFreezeData) - 2;
    unsigned short mon;
    StuFreezeData FreezeData;
    

    //计算读取的是第几个月数据
    mon = CharBCDtoHEX(time.Year) % 2;
    mon = mon * 12;
    mon = mon + CharBCDtoHEX(time.Mon);
    
    mon = mon * sizeof(StuFreezeData);
    ReadFreezeData(mon, (unsigned char *)&FreezeData, sizeof(StuFreezeData));
    if (CrcCheck(FreezeData.Crc16, (unsigned char *)&FreezeData, len ) == TRUE) {  //数据校验
        FreezeData.ctrl = 1;
        FreezeData.Crc16 = Crc16Get((unsigned char *)&FreezeData, len);

        WriteFreezeData(mon, (unsigned char *)&FreezeData, len + 2);
        ReadFreezeData(mon, (unsigned char *)&FreezeData, len + 2);
        if(CrcCheck(FreezeData.Crc16, (unsigned char *)&FreezeData, len) == TRUE)
        {
            return TRUE;
        }else{
            return FALSE;
        }
    }
    return FALSE;
}

/**
 * 刷新冻结状态
 * 
 * @author xsc (2015-04-08)
 */
void FreshFreezeState(void)
{
    RTC_T temprtc;
    unsigned char t = DateCmp(Cntpay.FreezeTime, RTC, 2);
    //过滤 跨月校时的情况
    if(t != 0)
    {
        SetState(STATE_FREEZE, STATE_FREEZE_NONE);
    }
    if(GetState(STATE_FREEZE) == STATE_FREEZE_OK)
    {
        temprtc.Year = RTC.Year;
        temprtc.Mon = RTC.Mon;
        copy_memery((unsigned char *)&temprtc.Day, (unsigned char *)&Meter.FreezeDate, sizeof(StuFreezeTime));

        //时间相等立即冻结
        unsigned long temp = DateCmp(temprtc, RTC, 5);
        if(temp == 1 || temp == 0)
        {
            SetState(STATE_FREEZE, STATE_FREEZE_NONE);
        }
    }
}

unsigned char cntpaydeviceerrtime = 0;
/**
 * 冻结
 * 
 * @author xsc (2014/11/27)
 */
unsigned char Freeze_Up(void)
{
    //
    if(GetSetupVal(SETUP_Freeze) != SETUP_Freeze_ON || GetState(STATE_FREEZE) == STATE_FREEZE_OK)
        return FALSE;
    StuFreezeData temp;
    //unsigned char tdata;
    unsigned char size = sizeof(StuFreezeData);
    unsigned short mon;
    //RTC_T tempdate;
    

    mon = CharBCDtoHEX(RTC.Year) % 2;
    mon = mon * 12;
    mon = mon + CharBCDtoHEX(RTC.Mon);
    mon = mon * size;
    
//  //判断是否有本月冻结数据
//  ReadFreezeData(mon, (unsigned char *)&temp, size);
//  if(CrcCheck(temp.Crc16, (unsigned char *)&temp, size - 2) == TRUE)
//  {
//      copy_memery((unsigned char *)&tempdate,(unsigned char *)&temp.Date,4);
//      tdata = DateCmp(tempdate, RTC, 4);
//      if (tdata == 1 || tdata == 0) {   //若冻结数据时间大于冻结日
//          SetState(STATE_FREEZE, STATE_FREEZE_OK);
//          return TRUE;
//      }
//  }
    ReadCntPayDevice();
    #if CntPay_Device == CntPay_ZD
        ReadZDEvent();
        if(GetValue(BASEVAL_Amount) == 0xFFFFFFFF && cntpaydeviceerrtime < 5)
        {
			cntpaydeviceerrtime++;
            return FALSE;
		}
		cntpaydeviceerrtime = 0;
    #endif
    unsigned long templong = GetValue(BASEVAL_Amount);
    copy_memery((unsigned char *)&temp.Date, (unsigned char *)&RTC, 4);
    copy_memery((unsigned char *)temp.Data, (unsigned char *)&templong , 4);
    temp.ctrl = 0;

    GetMeterState(temp.State);
    temp.Crc16 = Crc16Get((unsigned char *)&temp, size - 2);

    WriteFreezeData(mon, (unsigned char *)&temp, size);
    ReadFreezeData(mon, (unsigned char *)&temp, size);
    if(CrcCheck(temp.Crc16, (unsigned char *)&temp, size - 2) == TRUE)
    {
        copy_memery((unsigned char *)&Cntpay.FreezeTime, (unsigned char *)&RTC, sizeof(RTC_T));
        SetState(STATE_FREEZE, STATE_FREEZE_OK);
        return TRUE;
    }else{
        return FALSE;
    }
}

/**
 * 检测冻结时间
 * 
 * @author xsc (2015-04-08)
 */
void CheckFreezeTime(void)
{
    RTC_T temprtc;

    if(GetSetupVal(SETUP_Freeze) == SETUP_Freeze_ON)
    {
        if(GetSysPowState() == STATUS_POW_LOW || GetSysPowState() == STATUS_POW_CUT)
        {
            return;
        }
        temprtc.Year = RTC.Year;
        temprtc.Mon = RTC.Mon;
        copy_memery((unsigned char *)&temprtc.Day, (unsigned char *)&Meter.FreezeDate, sizeof(StuFreezeTime));

        //时间相等立即冻结
        unsigned long temp = DateCmp(temprtc, RTC, 5);
        if(temp == 0 || temp == 2)
        {
            Freeze_Up();
            Freeze_Up_Event();
        }
    }
}

///////////////////////////////// 冻结 结束 /////////////////////////////////////

void SetUpDefault(void)
{
    mem_set((unsigned char *) &Setup, sizeof(Setup), 0x00);
    SetSetupVal(SETUP_MeterType, SETUP_MeterType_MONEY);        //默认金额式
    SetSetupVal(SETUP_PostPaid, SETUP_PostPaid_ON);             //预付费开启
    SetSetupVal(SETUP_Precision, SETUP_Precision_001);          //计量精度0.01
    SetSetupVal(SETUP_OverFlow, SETUP_OverFlow_OFF);            //过流关闭
    SetSetupVal(SETUP_CloseWarning, SETUP_CloseWarning_ON);     //关阀告警开启
    SetSetupVal(SETUP_UnusedClose, SETUP_UnusedClose_OFF);      //闲置关闭
    
    SetSetupVal(SETUP_CloseOver, SETUP_CloseOver_ON);           //透支关阀开启
    SetSetupVal(SETUP_Freeze, SETUP_Freeze_ON);                 //冻结开启
    SetSetupVal(SETUP_CntPay, SETUP_CntPay_ON);                 //开启阶梯
}



/**
 * 清闲置时间
 * 
 * @author xsc (2015-01-19)
 */
void ClearUnUsedTime(void)
{
    UnUsedTimeOut = 0;
}

/**
 * 获取漏气保护次数
 * 
 * @author xsc (2015-01-19)
 * 
 * @return unsigned char 
 */
unsigned char GetLeakTime(void)
{
    return LeakTime;
}

/**
 * 清除漏气保护次数
 * 
 * @author xsc (2015-01-19)
 */
void ClearLeakTime(void)
{
    LeakTime = 0;
}


/**
 * 检测更新阶梯模型任务
 * 
 * @author xsc (2015-03-12)
 */
void CheckUpdataNewStep(void)
{
    if(Cntpay.NewActive == 0)
    {
        return;
    }//是否到达执行时间
    unsigned char temp;
    RTC_T tempdate;
    copy_memery((unsigned char *)&tempdate, (unsigned char *)&Cntpay.StepPriceNew.Time, sizeof(St_Time));
    temp = DateCmp(RTC, tempdate, 4);
    if(temp == 1 || temp == 0) //当前时间 大于 等于 设定时间
    {
        copy_memery((unsigned char *)&Cntpay.StepPriceNow, (unsigned char *)&Cntpay.StepPriceNew, sizeof(StuStepPrice));
        Cntpay.NewActive = 0;
        SetValue(USERVAL_PriceIndex, Cntpay.StepPriceNow.PriceIndex);
        mem_set((unsigned char *)&Cntpay.NextTime, sizeof(RTC_T), 0);
        RefreshStepNextTime();
        StepCycleCheck();
        CheckStepNow();
        RefreshRemainVal();
        UpdataNewStepEvent();
    }
}

/**
 * 立刻更新阶梯
 * 
 * @author xsc (2014-12-19)
 * 
 * @param p 阶梯表结构体指针
 * 
 * @return unsigned char 更新结果
 */
unsigned char UpdataStepNow(StuStepPrice *p)
{
//  unsigned char i;
    StuStepPrice temp;
    
    copy_memery((unsigned char *)&temp, (unsigned char *)p, sizeof(StuStepPrice));
    //修复内存错位
////#ifdef __ICCRL78__      //增加需修正内存错位的平台预编译
//    for(i = 0; i < 7; i++)
//    {
//        *((unsigned char *)&temp + sizeof(StuStepPrice) - i) = *((unsigned char *)&temp + 53 - i);         //53为阶梯理论占用空间大小
//    }
////#endif
    copy_memery((unsigned char *)&Cntpay.StepPriceNow, (unsigned char *)&temp, sizeof(StuStepPrice));
    SetValue(USERVAL_PriceIndex, Cntpay.StepPriceNow.PriceIndex);
    mem_set((unsigned char *)&Cntpay.NextTime, sizeof(RTC_T), 0);
    RefreshStepNextTime();
    StepCycleCheck();
    CheckStepNow();
    RefreshRemainVal();
    RefreshStepNextTime();
    return TRUE;
}

/**
 * 读取当前/预调阶梯模型
 * 
 * @author xsc (2014-12-22)
 * 
 * @param p 阶梯模型缓存 
 * @param t 0:当前阶梯;1:预调阶梯 
 * 
 * @return unsigned char 结果
 */
unsigned char ReadStep(StuStepPrice *p, unsigned char t)
{
  unsigned char i;
    if(t)
    {
        copy_memery((unsigned char *)p, (unsigned char *)&Cntpay.StepPriceNew, sizeof(StuStepPrice));
    }else{
        copy_memery((unsigned char *)p, (unsigned char *)&Cntpay.StepPriceNow, sizeof(StuStepPrice));
    }
    //copy_memery((unsigned char *)p, (unsigned char *)&Cntpay.StepPriceNew, sizeof(StuStepPrice));
    
    //修复内存错位
    for(i = 0; i < 7; i++)
    {
      *((unsigned char *)p + 53 - 6 + i) = *((unsigned char *)p + sizeof(StuStepPrice) - 6 + i);         //53为阶梯理论占用空间大小
    }
    //copy_memery((unsigned char *)p, (unsigned char *)&temp, 53);
    return TRUE;
}


/**
 * 更新价格阶梯表
 * 
 * @author xsc (2014/11/28)
 * 
 * @param p 阶梯表结构体指针
 * 
 * @return unsigned char 更新结果
 */
unsigned char UpdataStep(StuStepPrice *p)
{
//  unsigned char i;
    StuStepPrice temp;
    
    copy_memery((unsigned char *)&temp, (unsigned char *)p, sizeof(StuStepPrice));
    //修复内存错位
////#ifdef __ICCRL78__      //增加需修正内存错位的平台预编译
//    for(i = 0; i < 7; i++)
//    {
//        *((unsigned char *)&temp + sizeof(StuStepPrice) - i) = *((unsigned char *)&temp + 53 - i);         //53为阶梯理论占用空间大小
//    }
////#endif

    if(temp.PriceIndex > GetValue(USERVAL_PriceIndex))
    {
        Cntpay.NewActive = 1;
        copy_memery((unsigned char *)&Cntpay.StepPriceNew, (unsigned char *)&temp, sizeof(StuStepPrice));

        unsigned char temp;
        RTC_T tempdate;
        copy_memery((unsigned char *)&tempdate, (unsigned char *)&Cntpay.StepPriceNew.Time, sizeof(St_Time));
        temp = DateCmp(RTC, tempdate, 4);
        if(temp == 1 || temp == 0) //当前时间 大于 等于 设定时间
        {
            copy_memery((unsigned char *)&Cntpay.StepPriceNow, (unsigned char *)&Cntpay.StepPriceNew, sizeof(StuStepPrice));
            Cntpay.NewActive = 0;
            SetValue(USERVAL_PriceIndex, Cntpay.StepPriceNow.PriceIndex);
            mem_set((unsigned char *)&Cntpay.NextTime, sizeof(RTC_T), 0);
            RefreshStepNextTime();
            StepCycleCheck();
            CheckStepNow();
            RefreshRemainVal();
        }
        return TRUE;
    }
    return FALSE;
}

/******************************** 阶梯测试 ***************************************/

const unsigned char oldstepprice[] = {
    0x01,   //模型类型
    0x01,   //更新方式
    0x01,   //阶梯周期
    0x14, 0x12, 0x13, 0x00,     //年月日时
    0x00, 0x00, 0x01, 0x00,     //阶梯1价格 2字节整数 2字节小数     1元
    0x63, 0x9f, 0x86, 0x01,     //阶梯1气量 3字节整数 1字节小数     5方
    0x00, 0x00, 0x03, 0x00,     //阶梯2价格 2字节整数 2字节小数     2元
    0x63, 0x3f, 0x42, 0x0f,     //阶梯2气量 3字节整数 1字节小数     10方
    0x00, 0x00, 0x03, 0x00,     //阶梯3价格 2字节整数 2字节小数     2元
    0x63, 0x3f, 0x42, 0x0f,     //阶梯3气量 3字节整数 1字节小数     15方
    0x00, 0x00, 0x03, 0x00,     //阶梯4价格 2字节整数 2字节小数     3元
    0x63, 0x3f, 0x42, 0x0f,     //阶梯4气量 3字节整数 1字节小数     20方
    0x00, 0x00, 0x03, 0x00,     //阶梯5价格 2字节整数 2字节小数     4元
    0x63, 0x3f, 0x42, 0x0f,     //阶梯5气量 3字节整数 1字节小数     25方(无效)
    0x81, 0x1D,                 //阶梯起始日期
    0x01, 0x00, 0x00, 0x00      //调价序号
};

const unsigned char newstepprice[] = {
    0x01,   //模型类型
    0x01,   //更新方式
    0x01,   //阶梯周期
    0x14, 0x12, 0x16, 0x10,     //年月日时
    0x88, 0x13, 0x02, 0x00,     //阶梯1价格 2字节整数 2字节小数     0.5元
    0x63, 0x9f, 0x86, 0x01,     //阶梯1气量 3字节整数 1字节小数     5方
    0x00, 0x00, 0x03, 0x00,     //阶梯2价格 2字节整数 2字节小数     1元
    0x63, 0x3f, 0x42, 0x0f,     //阶梯2气量 3字节整数 1字节小数     10方
    0x00, 0x00, 0x03, 0x00,     //阶梯3价格 2字节整数 2字节小数     2元
    0x63, 0x3f, 0x42, 0x0f,     //阶梯3气量 3字节整数 1字节小数     15方
    0x00, 0x00, 0x03, 0x00,     //阶梯4价格 2字节整数 2字节小数     3元
    0x63, 0x3f, 0x42, 0x0f,     //阶梯4气量 3字节整数 1字节小数     20方
    0x00, 0x00, 0x03, 0x00,     //阶梯5价格 2字节整数 2字节小数     4元
    0x63, 0x3f, 0x42, 0x0f,     //阶梯5气量 3字节整数 1字节小数     25方(无效)
    0x81, 0x1D,                 //阶梯起始日期
    0x02, 0x00, 0x00, 0x00      //调价序号
};

unsigned char rr;




/******************************** 阶梯测试 **************************************/

const St_RegTxt CntpayRegTag[] = {
    {sizeof(Cntpay.StepPriceNow     ), "现阶梯表"},        //
    {sizeof(Cntpay.NewActive        ), "更新阶梯"},        //
    {sizeof(Cntpay.StepPriceNew     ), "新阶梯表"},        //
    {sizeof(Cntpay.StepUseTime      ), "计费时间"},
    {sizeof(Cntpay.NewStep          ), "分价新阶"}, //分价新阶梯补扣标志
    {sizeof(Cntpay.tprice           ), "新阶价差"}, //分价新阶梯补扣标志
    {sizeof(Cntpay.Hall             ), "霍尔计量"},
    {sizeof(Cntpay.NextTime         ), "下周期时"},
    {sizeof(Cntpay.FreezeTime       ), "上次冻时"}
};
/**
 * 阶梯扣费任务初始值
 * 
 * @author xsc (2014-12-17)
 */
void CntpayRegDefault(void)
{
    unsigned char HexToBcd(unsigned char hex);
    unsigned char i;
    StuStepPrice temp;

    mem_set((unsigned char *)&Cntpay, sizeof(Cntpay), 0);
    copy_memery((unsigned char*)&temp, (unsigned char*)oldstepprice, sizeof(oldstepprice));
    for(i = 0; i < 7; i++)
    {
        *((unsigned char *)&temp + sizeof(StuStepPrice) - i) = *((unsigned char *)&temp + 53 - i);         //53为阶梯理论占用空间大小
    }

    UpdataStepNow(&temp);
    RefreshStepNextTime();
}


void CntpayRegDataSvr(void)
{
    St_SAVESTRUCT temp;

    temp.fun = CntpayRegDefault;
    temp.data = (unsigned char *)&Cntpay;
    temp.len = sizeof(Cntpay);
    DataSvrReg(sizeof(CntpayRegTag) / 9, &temp, (St_RegTxt *)&CntpayRegTag);
}

/**
 * 检测当前所处阶梯
 * 
 * @author xsc (2014/11/28)
 */
void CheckStepNow(void)
{
    unsigned char i;
    unsigned long temp, temp2, temp3;


    if(GetSetupVal(SETUP_MeterType) != SETUP_MeterType_MONEY)
    {
        return;
    }
    if(GetSetupVal(SETUP_CntPay) != SETUP_CntPay_ON)
    {
        return;
    }
    for(i = 0; i < 5; i++)
    {
        //当前阶梯
        copy_memery((unsigned char*)&temp, Cntpay.StepPriceNow.table[i].StepAmount.AmountInt, 3);
        temp &= 0xFFFFFF;
        temp *= 100;
        temp += Cntpay.StepPriceNow.table[i].StepAmount.AmountDec;

        if(GetValue(BASEVAL_PeriodAmountGas) < temp || i == 4)
        {
            temp2 = Cntpay.StepPriceNow.table[i].StepPrice.PriceInt[1];
            temp2 <<= 8;
            temp2 += Cntpay.StepPriceNow.table[i].StepPrice.PriceInt[0];
            temp2 *= 10000;
            temp3 = Cntpay.StepPriceNow.table[i].StepPrice.PriceDec[1];
            temp3 <<= 8;
            temp3 += Cntpay.StepPriceNow.table[i].StepPrice.PriceDec[0];
            temp2 += temp3;
            if(temp2 >= GetValue(BASEVAL_Price))
            {
                Cntpay.tprice = temp2 - GetValue(BASEVAL_Price);
            }else{
                Cntpay.tprice = 0;
            }
            SetValue(BASEVAL_Price, temp2);
            //更新下一阶梯剩余量
            CntpayRun.NextStepRemain = temp - GetValue(BASEVAL_PeriodAmountGas);
            Cntpay.NewStep = 1;
            return;
        }
    }
}

void RefreshStepNextTime(void)
{
    unsigned char temp;
    unsigned short j;
    unsigned char i;
    unsigned long long day1, day2;
    RTC_T temprtc;
    

    //计算两时间差
    day1 = RTCToDay(Cntpay.NextTime, RTC);
    
    mem_set((unsigned char *)&temprtc, sizeof(RTC_T), 0);
    if(Cntpay.StepPriceNow.cycle.MonthOrDay == 0)   //月
    {
        day2 = Cntpay.StepPriceNow.cycle.Time * 30;
    }else{    //天
        day2 = Cntpay.StepPriceNow.cycle.Time;
    }

    if(DateCmp(Cntpay.NextTime, RTC, 3) <= 1 && day1 < day2 && day1 != 0)
    {
        return;
    }

    //if(Cntpay.NextTime.Year == 0 || Cntpay.NextTime.Year > 0x99 || Cntpay.NextTime.Mon == 0 || Cntpay.NextTime.Mon > 0x12 || Cntpay.NextTime.Day == 0 || Cntpay.NextTime.Day > 0x31)
    {
        Cntpay.NextTime.Year = charTobcd(Cntpay.StepPriceNow.StartDay.year);
        Cntpay.NextTime.Mon = charTobcd(Cntpay.StepPriceNow.StartDay.month);
        Cntpay.NextTime.Day = charTobcd(Cntpay.StepPriceNow.StartDay.day);
        Cntpay.NextTime.Hour = 0;
        Cntpay.NextTime.Min = 0;
        Cntpay.NextTime.Sec = 0;
    }
    if(Cntpay.NextTime.Year == 0 || Cntpay.NextTime.Year > 0x99 || Cntpay.NextTime.Mon == 0 || Cntpay.NextTime.Mon > 0x12 || Cntpay.NextTime.Day == 0 || Cntpay.NextTime.Day > 0x31)
        return;
    temp = DateCmp(Cntpay.NextTime, RTC, 3);
    for(j = 0; j < 9999; j++)
    {
        if(temp == 1)
            break;
        if(Cntpay.StepPriceNow.cycle.MonthOrDay == 0)   //月
        {
            Cntpay.NextTime.Year = charTobcd(CharBCDtoHEX(Cntpay.NextTime.Year) + (CharBCDtoHEX(Cntpay.NextTime.Mon) + Cntpay.StepPriceNow.cycle.Time) / 13);
            Cntpay.NextTime.Mon = CharBCDtoHEX(Cntpay.NextTime.Mon) + Cntpay.StepPriceNow.cycle.Time;
            while(Cntpay.NextTime.Mon > 12)
            {
                Cntpay.NextTime.Mon -= 12;
            }
            Cntpay.NextTime.Mon = charTobcd(Cntpay.NextTime.Mon);
            Cntpay.NextTime.Day = charTobcd(Cntpay.StepPriceNow.StartDay.day);
            Cntpay.NextTime.Min = 0;
            Cntpay.NextTime.Sec = 0;
        }else{//天
            for(i = 0; i < Cntpay.StepPriceNow.cycle.Time; i++)
            {
                AddOneDay(&Cntpay.NextTime);
            }
        }
        temp = DateCmp(Cntpay.NextTime, RTC, 3);
    }
}


/**
 * 阶梯周期检测
 * 
 * @author xsc (2015-03-12)
 */
void StepCycleCheck(void)
{
    if(GetSetupVal(SETUP_MeterType) == SETUP_MeterType_GAS)     //以下为金额表功能
    {
        return;
    }
    if(DateCmp(RTC, Cntpay.NextTime, 3) < 2) //当时间到或超过时
    {
        SetValue(BASEVAL_PeriodAmountGas, 0);
        Cntpay.StepUseTime = 0;
        CheckStepNow();
        RefreshStepNextTime();
        NewStepCycleEvent();
    }
}



/**
 * 更新金额状态及刷新剩余气量
 * 
 * @author xsc (2014/12/9)
 */

void RefreshRemainVal(void)
{
long tl, th, tth;
    unsigned char state;

//    if(GetValue(BASEVAL_Negtiveuse) > 0)
//    {
//        if(GetValue(BASEVAL_Negtiveuse) >= GetValue(USERVAL_Overdraught))
//        {
//            SetState(STATE_REMAIN, STATE_REMAIN_NEGTIVEOVER);
//        }else{
//            SetState(STATE_REMAIN, STATE_REMAIN_NEGTIVE);
//        }
//    }else{
//        if(GetValue(BASEVAL_Negtiveuse) == 0 && GetValue(BASEVAL_RemainInt) && GetValue(BASEVAL_RemainDec))
//            SetState(STATE_REMAIN, STATE_REMAIN_ZERO);
//        if(GetValue(BASEVAL_Remain) > GetValue(USERVAL_Warning))
//        {
//            SetState(STATE_REMAIN, STATE_REMAIN_NORMAL);
//        }else{
//            if(GetValue(BASEVAL_Remain) > 0)
//                SetState(STATE_REMAIN, STATE_REMAIN_LOW);
//        }
//    }
    
    if(GetSetupVal(SETUP_PostPaid) == SETUP_PostPaid_OFF)
    {
        SetState(STATE_REMAIN, STATE_REMAIN_NORMAL);
        SetValue(BASEVAL_RemainVol, GetValue(BASEVAL_Remain));
        return;
    }else
    if(GetValue(BASEVAL_Remain) > GetValue(USERVAL_Warning))
    {
        if(GetState(STATE_REMAIN) != STATE_REMAIN_NORMAL)
        {
            SetState(STATE_REMAIN, STATE_REMAIN_NORMAL);
            RemainStateChangeEvent(STATE_REMAIN_NORMAL);
        }
    }else
    if(GetValue(BASEVAL_Remain) <= GetValue(USERVAL_Warning) && (GetValue(BASEVAL_Remain) > 0) && (GetSetupVal(SETUP_CloseWarning) == SETUP_CloseWarning_ON) && (GetState(STATE_REMAIN) != STATE_REMAIN_LOW))
    {//少额告警
        if(GetSetupVal(SETUP_PostPaid) == SETUP_PostPaid_ON)
            Set_ValveControlRight(R_CONSUME);
        SetState(STATE_REMAIN, STATE_REMAIN_LOW);
        RemainStateChangeEvent(STATE_REMAIN_LOW);
    }else
    if(0 - (GetValue(BASEVAL_Remain)) >= GetValue(USERVAL_Overdraught))
    {//透支结束
        if(GetSetupVal(SETUP_PostPaid) == SETUP_PostPaid_ON)
            Set_ValveControlRight(R_CONSUME);
        SetState(STATE_REMAIN, STATE_REMAIN_NEGTIVEOVER);
        RemainStateChangeEvent(STATE_REMAIN_NEGTIVEOVER);
    }else
        if(((GetValue(BASEVAL_Remain) < 0) && (GetState(STATE_REMAIN) < STATE_REMAIN_ZERO)) || ((GetValue(BASEVAL_Remain) == 0) && (GetState(STATE_REMAIN) != STATE_REMAIN_ZERO)))
        {//过零点
            SetState(STATE_REMAIN, STATE_REMAIN_ZERO);
            RemainStateChangeEvent(STATE_REMAIN_ZERO);
            if(GetSetupVal(SETUP_PostPaid) == SETUP_PostPaid_ON)
                Set_ValveControlRight(R_CONSUME);
        }else
            if(GetValue(BASEVAL_Remain) < 0)
                SetState(STATE_REMAIN, STATE_REMAIN_NEGTIVE);


    if(GetValue(BASEVAL_Price) == 0)//过滤单价为0
    {
        return;
    }
    state = GetState(STATE_REMAIN);
    if(state >= STATE_REMAIN_ZERO)
    {
        tl = GetValue(BASEVAL_RemainDec) / (DecNum / 1000000);
        th = 0 - GetValue(BASEVAL_Negtiveuse);//*10000
    }else{
        tl =  GetValue(BASEVAL_RemainDec) / (DecNum / 1000000);
        th = GetValue(BASEVAL_RemainInt) ;//* 10000
    }
    tl = tl / GetValue(BASEVAL_Price);
    tth = (th * 100) / GetValue(BASEVAL_Price) * 10000;
    th = (th * 100) % GetValue(BASEVAL_Price) * 10000;
    tth += th / GetValue(BASEVAL_Price);
    tth = tl + tth;
    
    SetValue(BASEVAL_RemainVol, tth);
}

/**
 * 扣除相应的量
 */
void JustReduce(long tReduceInt,long tReduceDec)
{
    unsigned long tRemainInt,tRemainDec,tRemainNegInt;
    tRemainInt=GetValue(BASEVAL_RemainInt);
    tRemainDec=GetValue(BASEVAL_RemainDec);
    tRemainNegInt=GetValue(BASEVAL_Negtiveuse);
    
    if(tRemainInt>tReduceInt)
    {
        tRemainInt-=tReduceInt;
    }
    else
    {
        tRemainNegInt+=tReduceInt-tRemainInt;
        tRemainInt = 0;
    }
    
    if(tRemainDec<tReduceDec)
    {
        if(tRemainInt>0)
            tRemainInt-=1;
        else
            tRemainNegInt+=1;
        tRemainDec+=DecNum;
        
    }
    tRemainDec-=tReduceDec;

    SetValue(BASEVAL_RemainInt,tRemainInt);
    SetValue(BASEVAL_RemainDec,tRemainDec);
    SetValue(BASEVAL_Negtiveuse,tRemainNegInt);
}

/**
 * 将充值量累加到存量中
 */
void JustCharge(long tCharge, long *RemainInt, long *RemainDec, long *RemainNegInt)
{
    unsigned long tRemainInt,tRemainDec,tRemainNegInt;
    tRemainInt=GetValue(BASEVAL_RemainInt);
    tRemainDec=GetValue(BASEVAL_RemainDec);
    tRemainNegInt=GetValue(BASEVAL_Negtiveuse);
    
    if(tCharge>0)
    {
        tRemainDec += tCharge%100 * (DecNum/100);
        if(tRemainDec >= DecNum)
        {
            tRemainDec -= DecNum;
            tRemainInt += 1;
        }
        tRemainInt += tCharge / 100;
    }
    else if(tCharge<0)
    {
        tCharge=-tCharge;
        tRemainDec += (100-tCharge % 100) * (DecNum/100);
        if(tRemainDec >= DecNum)
        {
            tRemainDec -= DecNum;
            tRemainInt += 1;
        }
        tRemainNegInt += tCharge / 100 + 1;
    }
    
    if(tRemainNegInt > tRemainInt)
    {
        tRemainNegInt -= tRemainInt;
        tRemainInt = 0;
    }
    else
    {
        tRemainInt -= tRemainNegInt;
        tRemainNegInt = 0;
    }       

//  SetValue(BASEVAL_RemainInt,tRemainInt);
//  SetValue(BASEVAL_RemainDec,tRemainDec);
//  SetValue(BASEVAL_Negtiveuse,tRemainNegInt);
    *RemainInt = tRemainInt;
    *RemainDec = tRemainDec;
    *RemainNegInt = tRemainNegInt;
}

/**
 * 充值接口
 * 
 * @author xsc (2014/11/28)
 * 
 * @param money 充值金额，带两位小数
 * @param index 充值序号 
 *  
 * @return 成功/失败
 */
unsigned short ReCharge(long money, unsigned short index)
{
    unsigned char result = FALSE;
    long RemainI, RemainD, RemainNegI;
    long temp;

    if(index==0xffff)
        SetValue(USERVAL_ChargeIndex,0);
    
    if(index > GetValue(USERVAL_ChargeIndex))
    {
        JustCharge(money, &RemainI, &RemainD, &RemainNegI);
        temp = RemainI * 100 + RemainD / (DecNum/100) - RemainNegI * 100;
        if(temp > GetValue(USERVAL_Maxremain))
        {
            return FALSE;
        }
        SetValue(BASEVAL_RemainInt,RemainI);
        SetValue(BASEVAL_RemainDec,RemainD);
        SetValue(BASEVAL_Negtiveuse,RemainNegI);
        SetValue(USERVAL_ChargeIndex, index);
        SetValue(USERVAL_ChargeAmount, money);
        SetValue(BASEVAL_Totaladd, GetValue(BASEVAL_Totaladd) + money);

        RefreshRemainVal();
        result = TRUE;
    }
    return result;
}

/**
 * 漏气、过流、闲置存储
 * 
 * @author xsc (2015-01-08)
 */
t_GATEERR gateerr;
/**
 * 漏气、过流、闲置原因标记
 * 
 * @author xsc (2015-01-08)
 */
void SetErrCode(t_GATEERR t)
{
    gateerr = t;
}

/**
 * 漏气、过流、闲置获取
 * 
 * @author xsc (2015-01-08)
 */
t_GATEERR GetErrCode(void)
{
    return gateerr;
}
/**
 * 扣费
 * 
 * @author xsc (2014/11/27)
 */
void Charging(void)
{
    long templ;
    unsigned long tempint,tempdec;

    //后付费 则只为气量式
    if(GetSetupVal(SETUP_PostPaid) == SETUP_PostPaid_OFF)
    {
        SetSetupVal(SETUP_MeterType, SETUP_MeterType_GAS);
    }
    if(GetSetupVal(SETUP_MeterType) != SETUP_MeterType_MONEY)
    {
        SetSetupVal(SETUP_MeterType, SETUP_MeterType_GAS);
    }

    //漏气检测
    if(Get_ValveStatus() == CLOSE_STATUS && LeakTime < 8)
    {
        Set_ValveControlRight(R_EVENT);
        SetErrCode(GATEERR_Leak);
        LeakTime ++;
    }else{
        if(GetErrCode() == GATEERR_Leak)
        {
            Set_ValveControlRight(R_EVENT);
            SetErrCode(GATEERR_Null);
            LeakTime = 0;
        }
    }
    if(GetSetupVal(SETUP_UnusedClose) == SETUP_UnusedClose_ON)
    {
        if(GetErrCode() == GATEERR_UnUsedTimeOver)
        {
            Set_ValveControlRight(R_EVENT);
            SetErrCode(GATEERR_Null);
        }
    }

    while(Meter.ConsumeCount)
    {
        Meter.ConsumeCount--;
        GasOnMinGo++;

        //更新累计气量
        if(GetValue(BASEVAL_Amount) + 1 > GetValue(USERVAL_MaxAmount))
        {
            SetValue(BASEVAL_Amount, GetValue(BASEVAL_Amount) - GetValue(USERVAL_MaxAmount));
        }else{
            SetValue(BASEVAL_Amount, GetValue(BASEVAL_Amount) + 1);
        }
        Set_BeepAlarm_Flag();

        if(GetSetupVal(SETUP_PostPaid) == SETUP_PostPaid_ON)
        {
            if(Cntpay.NewStep == 1 && Cntpay.StepPriceNow.cycle.Model == 1 && Cntpay.tprice)    //分段补扣周期内气量
            {
                tempdec =  (Cntpay.tprice % 1000000) * (DecNum / 1000000);
                tempint =  (Cntpay.tprice / 1000000);
                Cntpay.tprice = GetValue(BASEVAL_PeriodAmountGas);
                while(Cntpay.tprice--)
                {
                    JustReduce(tempint,tempdec);        //扣费
                }
                Cntpay.NewStep = 0;
            }

            //0.01方的价格
            templ = GetValue(BASEVAL_Price);
            tempdec =  (templ % 1000000) * (DecNum / 1000000);
            tempint =  (templ / 1000000);

            JustReduce(tempint,tempdec);        //扣费
            RefreshRemainVal();                 //刷新剩余气量

            if(GetSetupVal(SETUP_MeterType) == SETUP_MeterType_MONEY)
            {
                //更新周期用量
                SetValue(BASEVAL_PeriodAmountGas, GetValue(BASEVAL_PeriodAmountGas) + 1);

                if(CntpayRun.NextStepRemain)
                    CntpayRun.NextStepRemain--;

                //判断距下一阶梯剩余气量
                if(CntpayRun.NextStepRemain == 0)
                {
                    CheckStepNow();
                }
            }
        }
    }
}



/**
 * 一天过去了 
 * 阶梯价格周期管理 
 * 
 * @author xsc (2014/11/27)
 */
void OneDayGone(void)
{
    if(GetSetupVal(SETUP_MeterType) == SETUP_MeterType_GAS)     //以下为金额表功能
    {
        return;
    }
    StepCycleCheck();
}

/**
 * 一小时过去了 
 * 更新阶梯管理、闲置关阀 
 * 
 * @author xsc (2014/11/27)
 */
void OneHourGone(void)
{
    //闲置关阀
    if(GetSetupVal(SETUP_UnusedClose) == SETUP_UnusedClose_ON)
    {
        UnUsedTimeOut++;
        if(GetValue(USERVAL_UnusedCloseTime) < UnUsedTimeOut && GetValue(USERVAL_UnusedCloseTime) != 0)
        {
            Set_ValveControlRight(R_EVENT);
            SetErrCode(GATEERR_UnUsedTimeOver);
            UnUsedTimeOverEvent();
        }
    }

    CheckUpdataNewStep();
}

/**
 * 一分钟过去了
 * 
 * @author xsc (2014/12/9)
 */
void OneMinuGone(void)
{
    FreshFreezeState();
    CheckFreezeTime();

    //过流检测
    if(GetSetupVal(SETUP_OverFlow) == SETUP_OverFlow_ON)
    {
        if(GetValue(USERVAL_Maxflow) < GasOnMinGo && GetValue(USERVAL_Maxflow) != 0)
        {
            Set_ValveControlRight(R_EVENT);
            SetErrCode(GATEERR_OverFlow);
            OverFlowEvent();
        }else{
            if(GetErrCode() == GATEERR_OverFlow)
            {
                Reset_ValveControlRight(R_EVENT);
                SetErrCode(GATEERR_Null);
            }
        }
    }
    GasOnMinGo = 0;
}

void OneSecondGone(void)
{
    //SoftRTC_Ns_Task(1);

}

/**
 * 新上电重
 * 
 * @author xsc (2015-03-12)
 */
void RePowerUp(void)
{
    RePowerUpEvent();
    FreshFreezeState();
    CheckStepNow();
    StepCycleCheck();
    RefreshStepNextTime();
    RefreshRemainVal();
}


/**
 * 更新时钟事件
 * 
 * @author xsc (2015-04-08)
 */
void ChangeRTCEvent(void)
{
    FreshFreezeState();
    CheckFreezeTime();
    CheckUpdataNewStep();
    StepCycleCheck();
}

