#include "app_cardhummach.h"
#include "timer_manage.h"
#include "lcd.h"
#include "app_card1608.h"
#include "app_general.h"
#include "App_Powermanage.h"
#include "app_valve_control.h"

LcdStateVal     LcdState;    //液晶状态
unsigned char LcdDispIndex;
unsigned char DisplayTaskNum = 0;
unsigned char TimeOutTaskNum = 0;
unsigned char ChangeDisplayTaskNum = 0;

LcdStateVal GetLcdState(void)
{
    LcdStateVal re;
   
    re = LcdState;

    return re;
}

LcdStateVal SetLcdState(LcdStateVal val)
{
    LcdStateVal re = val;

    LcdState = val;

    return re;
}

void ShowDefaultInfo(void)
{
    RemainTime_ST T_RemainTime;
    unsigned char ShowFault = 0;

//    //显示报警信息
//    if(Fault.Value)
//    {
//        ShowFault = ShowErr();  
//    }
//    else
//    {
//        OV_LcdDispMark(MARK_ERR,0);
//    }
//    //启用后就开始显示余量报警信息
//    if(Fault.NoUser==0)
//    {
//        if(GetState(STATE_REMAIN)==STATE_REMAIN_FEW)
//        {
//            OV_LcdDispMark(MARK_ERR,1);
//        }
//        else
//        {
//            OV_LcdDispMark(MARK_ERR,0);
//        }
//    }

    if(ShowFault == 0)
    {
        switch(GetCardState())
        {
        //case STATUS_METER_UNUSED:     //出厂状态，未初始化，需要写入一些参数，显示程序版本号
        //    LcdDispVer(SYS_HEAD_CODE, SYS_VER);
        //    break;
        case STATUS_METER_INIT:         //出厂状态，已经初始化，未设置，显示hello
            OV_DispHello();
            OV_LcdDispMark(MARK_ERR,0);
            if(GetSetupVal(SETUP_MeterType) == SETUP_MeterType_MONEY)
            {
                OV_LcdDispMark(MARK_MONEY,1);
                OV_LcdDispMark(MARK_GAS,0);
            }
            else if(GetSetupVal(SETUP_MeterType) == SETUP_MeterType_GAS)
            {
                OV_LcdDispMark(MARK_MONEY,0);
                OV_LcdDispMark(MARK_GAS,1);
            }
            break;
        case STATUS_METER_SETUP:        //设置状态
            if(GetSetupVal(SETUP_MeterType) == SETUP_MeterType_GAS)
                OV_DispRemainGas(GetRemainValue(),2,0);
            else
                OV_DispRemainMoney(GetRemainValue(),2,0);
            break;
        case STATUS_METER_NOUSER_USE:   //无用户的状态的检表
        case STATUS_METER_CHECK:       //检表状态
        case STATUS_METER_INUSE:        //正常用户状态
        case STATUS_METER_OUTBAND:      //解除绑定状态，解绑状态下不允许透支。
        case STATUS_METER_OVERUSE:      //透支状态
            if(GetSysPowState()==STATUS_POW_UNDER)
            {
                GetRemain48Hour(&T_RemainTime);
                OV_DispTime(T_RemainTime.Hour,T_RemainTime.Min,T_RemainTime.Sec);
            }
            else
            {
                if(GetSetupVal(SETUP_MeterType) == SETUP_MeterType_GAS)
                    OV_DispRemainGas(GetRemainValue(),2,0);
                else
                    OV_DispRemainMoney(GetRemainValue(),2,0);
            }
            break;
        default:
            break;
        }
    }
}

/*
 * 轮显信息显示相关的信息
 */
void ChangeDisp(void)
{
    if(GetLcdState() == STATE_LCD_DISP_CHANGE)
    {
        if(GetCardState() == STATUS_METER_INIT)//未插设置卡HELLO状态和程序版本轮显
        {
            LcdDispIndex += 1;
            if(LcdDispIndex > 2)
                LcdDispIndex = 1;
            switch(LcdDispIndex)
            {
            case 1://显示“HELLO”
                OV_DispHello();
                if(GetSetupVal(SETUP_MeterType) == SETUP_MeterType_MONEY)
                {
                    OV_LcdDispMark(MARK_MONEY,1);
                    OV_LcdDispMark(MARK_GAS,0);
                }
                else if(GetSetupVal(SETUP_MeterType) == SETUP_MeterType_GAS)
                {
                    OV_LcdDispMark(MARK_MONEY,0);
                    OV_LcdDispMark(MARK_GAS,1);
                }
                break;
            case 2://显示程序版本
                LcdDispVer(SYS_HEAD_CODE, SYS_VER);
                break;
            default:
                break;
            }
        }
        else if(GetSetupVal(SETUP_MeterType) == SETUP_MeterType_GAS)//气量表轮显
        {
            LcdDispIndex += 1;
            if(LcdDispIndex > 5)
                LcdDispIndex = 1;
            switch(LcdDispIndex)
            {
            case 1://显示剩余气量
                OV_DispRemainGas(GetRemainValue(),2,0);
                break;
            case 2://显示消费气量
                OV_DispConsumedGas(GetValue(BASEVAL_PeriodAmountGas),2,0);
                break;
            case 3://显示充值气量
                OV_DispChargeGas(GetValue(USERVAL_ChargeAmount),2,0);
                break;
            case 4://显示累计气量
                OV_DispAmountGas(GetValue(BASEVAL_Totaladd),2,0);
                break;
            case 5://显示程序版本
                LcdDispVer(SYS_HEAD_CODE, SYS_VER);
                break;
            default:
                break;
            }
        }
        else                   //金额表轮显
        {
            if(GetConfigValue(CONFIG_UseSigPrice)==1)//单一价
            {
                LcdDispIndex += 1;
                if(LcdDispIndex > 5)
                    LcdDispIndex = 0;
            }
            else                        //阶梯价
            {
                LcdDispIndex += 1;
                if(LcdDispIndex > 15)
                    LcdDispIndex = 0;
            }
            switch(LcdDispIndex)
            {
            case 0://显示程序版本
                LcdDispVer(SYS_HEAD_CODE, SYS_VER);
                break;
            case 1://显示剩余金额
                OV_DispRemainMoney(GetRemainValue(),2,0);
                break;
            case 2://显示消费金额
                OV_DispConsumedMoney(GetValue(BASEVAL_PeriodAmountGas),2,0);
                break;
            case 3://显示充值金额
                OV_DispChargeMoney(GetValue(USERVAL_ChargeAmount),2,0);
                break;
            case 4://显示累计金额
                OV_DispAmountMoney(GetValue(BASEVAL_Totaladd),2,0);
                break;
            case 5://显示单价
                OV_DispPrice(GetValue(BASEVAL_Price),4); 
                break;
            case 6://阶梯1金额
                OV_DispStepMoney(1,4,1);
                break;
            case 7://阶梯1气量
                OV_DispStepGas(1,2,1);
                break;
            case 8://阶梯2金额
                OV_DispStepMoney(1,4,2);
                break;
            case 9://阶梯2气量
                OV_DispStepGas(1,2,2);
                break;
            case 10://阶梯3金额
                OV_DispStepMoney(1,4,3);
                break;
            case 11://阶梯3气量
                OV_DispStepGas(1,2,3);
                break;
            case 12://阶梯4金额
                OV_DispStepMoney(1,4,4);
                break;
            case 13://阶梯4气量
                OV_DispStepGas(1,2,4);
                break;
            case 14://阶梯5金额
                OV_DispStepMoney(1,4,5);
                break;
            case 15://阶梯5气量
                OV_DispStepGas(1,2,5);
                break;
            default:
                break;
            }
        }
        OV_LcdOn();
    }
}

//void DisplayTimeOut(void)
//{
//    if(CardBslRead())
//    {
//        //User_CloseTime(DisplayTaskNum);
//        //User_CloseTime(TimeOutTaskNum);
//        OV_LcdOff();
//    }
//}

void TaskShowInfo(void)
{
    if(GetLcdState() == STATE_LCD_DISP_RELEASE)
    {
        ShowDefaultInfo();   //显示默认信息
    }

    //显示阀门状态
    switch(Get_ValveStatus())
    {
    case OPEN_STATUS:
        OV_LcdDispMark(MARK_OPEN,1);
        OV_LcdDispMark(MARK_CLOSE,0);          
        break;
    case CLOSE_STATUS:
        OV_LcdDispMark(MARK_OPEN,0);
        OV_LcdDispMark(MARK_CLOSE,1);
        break;
    case ERR_STATUS:       
    default:
        OV_LcdDispMark(MARK_OPEN,1);
        OV_LcdDispMark(MARK_CLOSE,1);
        break;
    }
    
    //显示电压状态
    switch(GetSysPowState())
    {
    case STATUS_POW_NORMAL:
        OV_DispBatt(3);
        break;
    case STATUS_POW_UNDER:
        OV_DispBatt(2);
        break;
    case STATUS_POW_LOW:
        OV_DispBatt(1);
        break;
    case STATUS_POW_CUT:
        OV_DispBatt(0);
        break;
    default:
        break;
    }
    
    //检测卡片状态，反插卡轮显
    if(CardBslRead())
    {
        SetLcdState(STATE_LCD_DISP_RELEASE);   //释放显示，显示表具不同状态下的默认值
        LcdDispIndex = 0;
    }
}

void TaskCardMeterDisplay(void)
{ 
    DisplayTaskNum = User_AskTimeNumber();
    if (DisplayTaskNum)
    {
        User_StartTime(SUBTIMER, DisplayTaskNum, 1, TaskShowInfo);  //显示刷新，1*100ms打开一次
    }
    
    ChangeDisplayTaskNum = User_AskTimeNumber();
    if (ChangeDisplayTaskNum)
    {
        User_StartTime(SUBTIMER, ChangeDisplayTaskNum, 30, ChangeDisp);  //轮显，30*100ms打开一次
    }

//    TimeOutTaskNum = User_AskTimeNumber();
//    if (TimeOutTaskNum)
//    {
//        //User_StartTime(SUBTIMER, TimeOutTaskNum, 200, DisplayTimeOut);  //显示超时，200*100ms打开一次
//          User_StartTime(SUBTIMER, TimeOutTaskNum, GetConfigValue(CONFIG_LcdSleepTime), DisplayTimeOut); 
//    }
}

/**
 *  版本号显示 X N-N-N
 *  输入参数：head1 版本信息，字母
 *          ：T_num 版本号
 */
void LcdDispVer(unsigned char Head1, unsigned int T_num)
{
    unsigned int k;
    OV_LcdClearValueMark();
    OV_LcdDispChar(0, Head1);
    OV_LcdDispChar(1, LCD_CHAR_BLANK);
    k = T_num / 0x1000;
    OV_LcdDispChar(2, k);
    OV_LcdDispChar(3, LCD_CHAR_SEPARATE);
    T_num %= 0x1000;
    k = T_num / 0x100;
    OV_LcdDispChar(4, k);
    OV_LcdDispChar(5, LCD_CHAR_SEPARATE);
    T_num %= 0x100;
    OV_LcdDispChar(6, T_num %= 0x10);
}

void LcdDispLoading(void)
{
    unsigned char i;
    OV_LcdClearValueMark();
    for(i = 0; i < 7; i++)
        OV_LcdDispChar(i, LCD_CHAR_SEPARATE);
}

/*
 * 显示错误代码
 */
void LcdDispErr(unsigned char ErrNum)
{
    OV_LcdClearValueMark();
    if(ErrNum > 99)
    {
        OV_LcdDispChar(0, ErrNum / 100);
        ErrNum %= 100;
        OV_LcdDispChar(1, ErrNum / 10);
        OV_LcdDispChar(2, ErrNum % 10);
    }
    else if(ErrNum > 9)
    {
        OV_LcdDispChar(0, LCD_CHAR_BLANK);
        OV_LcdDispChar(1, ErrNum / 10);
        OV_LcdDispChar(2, ErrNum % 10);
    }
    else
    {
        OV_LcdDispChar(0, LCD_CHAR_BLANK);
        OV_LcdDispChar(1, LCD_CHAR_BLANK);
        OV_LcdDispChar(2, ErrNum % 10);
    }
    OV_LcdDispChar(3, LCD_CHAR_SEPARATE);
    OV_LcdDispChar(4, LCD_CHAR_E);
    OV_LcdDispChar(5, LCD_CHAR_r);
    OV_LcdDispChar(6, LCD_CHAR_r);
}
