/*
 使用说明：
 * 用户使用电源管理任务，只需调用
 * 1.任务注册函数
 * int App_PowerManageReg(void)
 * 用于给电源管理任务申请欠压点，低压点和掉电点参数空间
 * 2.任务初始化函数
 * int Task_PowerManage_Init(unsigned char R1,unsigned char R2,ADC_InitTypeDef* ADC_InitStruct,PowManageFunST *PowManageFunUser,SysPowManage_ST *pPowAlarmPoint)
 * unsigned char R1,unsigned char R2为系统电源检测分压电阻比值，R2为接地电阻。如果焊接15k，3k，可以R1=15，R2=3，或者R1=5，R2=1
 * ADC_InitTypeDef* ADC_InitStruct为电压检测adc模块配置参数
 * typedef struct
{
    unsigned char ADCModeConversionMode;   //转换模式,0:单次,1:连续(此处选0)
    unsigned char ADCModeChannel;          //检测源,0:系统电压,1:阀门电流
    unsigned char ADCModeConversionBit;    //转换精度,8:8位,10:10位;12:12位(PIC单片机选10，填0x0A)
    unsigned char ADCRef;                  //参考电压,0:内部1.45V,1:VDD(PIC单片机选1)
}ADC_InitTypeDef;
 * PowManageFunST *PowManageFunUser为电源管理任务各阶段回调函数，请参考具体定义
 * 电源管理任务已经extern空函数:void FunPowerManageDefault(void)
 * 初始化参数PowManageFunST *PowManageFunUser时，请务必保证没有漏定义造成电源管理任务初始化时回调函数指向未知
 * 建议不需要的回调任务指向空函数FunPowerManageDefault
 * SysPowManage_ST *pPowAlarmPoint:系统报警电压点和启动电压点配置参数，含义请参考SysPowManage_ST定义
 * 电源管理任务初始化过程示例：
PowManageFunST PowManageFunUser =       //电源管理回调函数初始化
{
    VoltNormalToCut,        //回调函数，具体由用户自己定义，下同
    VoltNormalToLow,
    VoltNormalToUnder,
    VoltUnderToNormal,
    VoltUnderToLow,
    VoltUnderToCut,
    VoltLowToNormal,
    VoltLowToUnder,
    VoltLowToCut,
    VoltCutToNormal,
    VoltCutToUnder,
    VoltCutToLow,
};
void User_PowOnCheck(void)
{
    ADC_InitTypeDef UserADC_InitPara;                               //初始化adc模块配置参数
    SysPowManage_ST UserSysPowManagePara = {3300,3000,1000,3000};   //初始化报警电压和启动电压配置参数,
                                                                    //注意：报警电压会被保存，启动电压不会被保存
                                                                    //此初始化过程只用于表具未配置参数时的初始化，进行参数配置后(假如会配置报警电压)
                                                                    //表具报警电压在配置的参数被保存后，上电会读取并以配置参数为准
    UserADC_InitPara.ADCModeChannel         = 0;//选择系统电源
    UserADC_InitPara.ADCModeConversionBit   = 10;//10位ad
    UserADC_InitPara.ADCModeConversionMode  = 0;//单次转换
    UserADC_InitPara.ADCRef                 = 1;//参考电压VDD
    Task_PowerManage_Init(12,3,&UserADC_InitPara,&PowManageFunUser,&UserSysPowManagePara);//调用电源管理初始化函数
}
 * 电源管理任务已经管理的任务有：
 * 电源变化时保存或者读取参数
 * 电源变化时通知阀门
 * 提供欠压倒计时接口unsigned int GetRemain48Hour(RemainTime_ST *pRemainTime);
 * 提供系统电源状态查询函数SysPowState_ET GetSysPowState(void);
 * 提供查询系统电压值unsigned long GetSysPow(void);
 * 提供查询系统欠压状态int IsSysPowUndering48(void);
 * 提供查询欠压倒计时值unsigned int GetRemain48Hour(RemainTime_ST *RemainTime);
 */
#include "App_PowerManage.h"
#include "drv_ad.h"
#include "timer_manage.h"
#include "app_valve_control.h"
#include "lcd.h"
#include "app_datasvr.h"
#include "string.h"
#include "app_general.h"
#include "lib_public.h"

static unsigned char GPRSIsRunning = 0;
SysPowManage_ST DefaultPowAlarmPoint;

void SetGPRSIsRunning(void)
{
    GPRSIsRunning = 1;
}

void ClrGPRSIsRunning(void)
{
    GPRSIsRunning = 0;
}

static unsigned short VoltMul = 500; //扩大100倍
//ADC设置参数
ADC_InitTypeDef ADC_InitPara = 
{
	0x00,0x00,0x08,0x00
};

//电源电压判断浮动量
#define VOLT_MORE_MV    50     //50毫伏
//电源告警电压点结构体
SysPowManage_ST SysPowManageST;

//系统电源电压
unsigned long SysPow = 0;
unsigned char PowManageTaskNum = 0;
SysPowState_ET SysPowState = STATUS_POW_CUT;
SysPowFlag_ST SysPowFlag = {0x00,0x00,0x00}; 

unsigned char Remain48HourNum = 0;
unsigned long RemainTime = (unsigned long)48*60*60; //48小时倒计时开始时刻(30s误差在13ms，48小时总误差在75s左右)

RemainTime_ST RemainTimeST;
//48小时倒计时
void Remain48Hour(unsigned long* time)
{
    if(*time > 0)
    {
        RemainTimeST.Hour = (*time)/3600;
        RemainTimeST.Min = ((*time)%3600)/60;
        RemainTimeST.Sec = (*time)%60;
        *time -= 1;
    }
    else
    {
        RemainTimeST.Hour = 0;
        RemainTimeST.Min = 0;
        RemainTimeST.Sec = 0;
        SysPowFlag.SysPowUndering48 = 2;
        Set_ValveControlRight(R_VOLTAGE);                   //置阀门控制权         
    }
}

//获取48小时倒计时时间，返回48小时倒计时标识
unsigned int GetRemain48Hour(RemainTime_ST *pRemainTime)
{
    pRemainTime->Hour   = RemainTimeST.Hour;
    pRemainTime->Min    = RemainTimeST.Min;
    pRemainTime->Sec    = RemainTimeST.Sec;
    return IsSysPowUndering48();
}

//48小时倒计时任务
void Task_Remain_48Hour(void)
{
    if( (1 == IsSysPowUndering48()) && (GetSysPowState() == STATUS_POW_UNDER) )
    {
        Remain48Hour(&RemainTime);
    }
    else
    {
        User_CloseTime(Remain48HourNum);                      //取消欠压倒计时任务
        mem_set((unsigned char *)&RemainTimeST,sizeof(RemainTime_ST),0);
        RemainTime = 0;
    }
}

//GPRS低压上告任务
void Task_GPRS1(void)
{

}

//设置系统电压状态
void SetSysPowState(SysPowState_ET State)
{
    SysPowState = State;
}
//查询系统电压状态
SysPowState_ET GetSysPowState(void)
{
    return SysPowState;
}

//查询系统电压是否欠压48小时倒计时
int IsSysPowUndering48(void)
{
    return SysPowFlag.SysPowUndering48;
}

//查询系统电压是否由常压进入欠压
int GetSysPowNormalToUnder(void)
{
    return(SysPowFlag.SysPowNormalToUnder);
}

void ClrSysPowNormalToUnder(void)
{
    SysPowFlag.SysPowNormalToUnder = 0;
}

void SetSysPowNormalToUnder(unsigned char a)
{
    SysPowFlag.SysPowNormalToUnder = a;
}

/**
 * @declare 电源管理任务 
 *  
 * 每3秒打开一次AD，根据获取电压值，判断更新系统电源状态 
 * 并调取相应任务应用 
 * 应用根据系统电源状态，自行判断处理 
 *  
 * @author ZYX (2014/11/25)
 *
 * requirements 
 */
//调用参数
//extern unsigned short AdcRefVolt;//ADC采集基准,单位mV,只读,不可更改
//电源管理任务默认回调函数为空
void FunPowerManageDefault(void)
{
    return ;
}
//设置电源管理任务默认回调函数
PowManageFunST PowManageFun =
{
    FunPowerManageDefault,
    FunPowerManageDefault,
    FunPowerManageDefault,
    FunPowerManageDefault,
    FunPowerManageDefault,
    FunPowerManageDefault,
    FunPowerManageDefault,
    FunPowerManageDefault,
    FunPowerManageDefault,
    FunPowerManageDefault,
    FunPowerManageDefault,
    FunPowerManageDefault,
};
//任务主体
void Task_PowerManage(void)
{
    unsigned long SysVolt;
    OA_ADC_Init(&ADC_InitPara);
    //计算系统电源电压
    SysVolt = (unsigned long)OA_ADC_GetValue(ADCCHANNELVOLT);
    SysVolt *= AdcRefVolt;
    if(ADC_InitPara.ADCModeConversionBit == 8)
        SysVolt >>= 8;
    else
        SysVolt >>= 10;
    SysVolt *= VoltMul;
    SysVolt /= 100;
    //更新系统电压
    SysPow = SysVolt;
    switch(SysPowState)
    {
    case STATUS_POW_NORMAL:
        if( SysVolt <= (SysPowManageST.PowAlarmST.VoltCut-VOLT_MORE_MV) )              //常压到掉电
        {
            SetSysPowState(STATUS_POW_CUT);
            Set_ValveControlRight(R_VOLTAGE);                   //置阀门控制权
            SysPowFlag.SysPowUndering48 = 3;
            PowManageFun.FunNormalToCut();                      //回调函数    
            
            DataSvrSaveAll();                                   //添加掉电保存数据任务
            DelayMs(500);
        }
        else if( SysVolt <= (SysPowManageST.PowAlarmST.VoltLow-VOLT_MORE_MV) )         //常压到低压
        {
            SetSysPowState(STATUS_POW_LOW);
            Set_ValveControlRight(R_VOLTAGE);                   //置阀门控制权 
            SysPowFlag.SysPowUndering48 = 3;
            PowManageFun.FunNormalToLow();                      //回调函数
            
            DataSvrSaveAll();                                   //添加低压保存数据任务
            DelayMs(500);
        }
        else if( SysVolt <= (SysPowManageST.PowAlarmST.VoltUnder-VOLT_MORE_MV) )         //常压到欠压
        {
            if(GPRSIsRunning == 0)
            {
                SetSysPowState(STATUS_POW_UNDER);
                SetSysPowNormalToUnder(1);                          //系统电源标志常压进欠压置位  
                //Set_ValveControlRight(R_VOLTAGE);                   //置阀门控制权

                if( Remain48HourNum == 0 )
                {
                    Remain48HourNum = User_AskTimeNumber();
                }
                RemainTime = (unsigned long)48*60*60;
                User_StartTime(SUBTIMER, Remain48HourNum, 1, Task_Remain_48Hour); //开启欠压倒计时任务，1秒更新一次
                SysPowFlag.SysPowUndering48 = 1;                    //
                PowManageFun.FunNormalToUnder();                    //回调函数
            }
        }
        break;
    case STATUS_POW_UNDER:
        if( SysVolt > (SysPowManageST.PowAlarmST.VoltUnder+VOLT_MORE_MV) )               //欠压到常压
        {
            SetSysPowState(STATUS_POW_NORMAL);
            ClrSysPowNormalToUnder();
            Reset_ValveControlRight(R_VOLTAGE);                 //清除阀门控制权//电压恢复时                     
            User_CloseTime(Remain48HourNum);                      //取消欠压倒计时任务
            mem_set((unsigned char *)&RemainTimeST,sizeof(RemainTime_ST),0);
            RemainTime = 0;
            SysPowFlag.SysPowUndering48 = 0;                    //
            PowManageFun.FunUnderToNormal();                    //回调函数
        }
        else if( SysVolt <= (SysPowManageST.PowAlarmST.VoltLow-VOLT_MORE_MV) )         //欠压到低压
        {
            SetSysPowState(STATUS_POW_LOW);
            Set_ValveControlRight(R_VOLTAGE);                   //置阀门控制权 
            User_CloseTime(Remain48HourNum);                      //取消欠压倒计时任务
            mem_set((unsigned char *)&RemainTimeST,sizeof(RemainTime_ST),0);
            RemainTime = 0;
            SysPowFlag.SysPowUndering48 = 3;                    //
            PowManageFun.FunUnderToLow();                       //回调函数
            
            DataSvrSaveAll();                                   //添加低压保存数据任务
            DelayMs(500);
        }
        else if( SysVolt <= (SysPowManageST.PowAlarmST.VoltCut-VOLT_MORE_MV) )         //欠压到掉电
        {
            SetSysPowState(STATUS_POW_CUT);
            Set_ValveControlRight(R_VOLTAGE);                   //置阀门控制权
            User_CloseTime(Remain48HourNum);                      //取消欠压倒计时任务
            mem_set((unsigned char *)&RemainTimeST,sizeof(RemainTime_ST),0);
            RemainTime = 0;                                     //
            SysPowFlag.SysPowUndering48 = 3;                    //
            PowManageFun.FunUnderToCut();                       //回调函数
            
            DataSvrSaveAll();                                   //添加掉电保存数据任务
            DelayMs(500);
        }
        break;
    case STATUS_POW_LOW:
        if( SysVolt > (SysPowManageST.PowAlarmST.VoltUnder+VOLT_MORE_MV) )               //低压到常压
        {
            SetSysPowState(STATUS_POW_NORMAL);
            //if(SysPowFlag.SysPowPowOn == 1)                     //不是刚上电
            {
#ifdef DEF_GPRS_METER
                    DelayMs(2000);                             //延时20秒
#endif
                DataSvrReadAll();                               //读取所有参数
            }
            ClrSysPowNormalToUnder();
            Reset_ValveControlRight(R_VOLTAGE);                 //清除阀门控制权//电压恢复时  
            User_CloseTime(Remain48HourNum);                      //取消欠压倒计时任务
            mem_set((unsigned char *)&RemainTimeST,sizeof(RemainTime_ST),0);
            RemainTime = 0;   
            SysPowFlag.SysPowUndering48 = 0;                    //
            PowManageFun.FunLowToNormal();                      //回调函数
        }
        else if( SysVolt > (SysPowManageST.PowAlarmST.VoltLow+VOLT_MORE_MV) )          //低压到欠压
        {
            SetSysPowState(STATUS_POW_UNDER);
            
            SetSysPowNormalToUnder(3);
            DataSvrReadAll();                                   //读取所有参数
            SysPowFlag.SysPowUndering48 = 3;
            PowManageFun.FunLowToUnder();                       //回调函数
        }
        else if( SysVolt <= (SysPowManageST.PowAlarmST.VoltCut-VOLT_MORE_MV) )         //低压到掉电
        {
            SetSysPowState(STATUS_POW_CUT);
            SysPowFlag.SysPowUndering48 = 3;
            PowManageFun.FunLowToCut();
        }
        break;
    case STATUS_POW_CUT:
        if( SysVolt > (SysPowManageST.PowAlarmST.VoltUnder+VOLT_MORE_MV) )               //掉电到常压
        {
            SetSysPowState(STATUS_POW_NORMAL);
            //if(SysPowFlag.SysPowPowOn == 1)                     //不是刚上电
            {
#ifdef DEF_GPRS_METER
                    DelayMs(2000);                             //延时20秒
#endif
                DataSvrReadAll();                               //读取所有参数
            }
            //SysPowFlag.SysPowPowOn = 1;    
            User_CloseTime(Remain48HourNum);                      //取消欠压倒计时任务
            mem_set((unsigned char *)&RemainTimeST,sizeof(RemainTime_ST),0);
            RemainTime = 0;
            ClrSysPowNormalToUnder();
            Reset_ValveControlRight(R_VOLTAGE);                 //清除阀门控制权//电压恢复时
            SysPowFlag.SysPowUndering48 = 0;                    //
            PowManageFun.FunCutToNormal();                      //回调函数
        }
        else if( SysVolt > (SysPowManageST.PowAlarmST.VoltLow+VOLT_MORE_MV) )          //掉电到欠压
        {
            SetSysPowState(STATUS_POW_UNDER);
            SetSysPowNormalToUnder(3);
            //SysPowFlag.SysPowPowOn = 1;
            DataSvrReadAll();                                   //读取所有参数
            SysPowFlag.SysPowUndering48 = 3;
            PowManageFun.FunCutToUnder();                       //回调函数
        }
        else if( SysVolt > (SysPowManageST.PowAlarmST.VoltCut-VOLT_MORE_MV) )          //掉电到低压
        {
            //SysPowFlag.SysPowPowOn = 1;
            SetSysPowState(STATUS_POW_LOW);
            SysPowFlag.SysPowUndering48 = 3;
            PowManageFun.FunCutToLow();
        }
        break;
    }
}

//电源管理任务参数初始默认值
void App_PowerManageDefault(void)
{
    SysPowManageST.PowAlarmST.VoltUnder     = DefaultPowAlarmPoint.PowAlarmST.VoltUnder;
    SysPowManageST.PowAlarmST.VoltLow       = DefaultPowAlarmPoint.PowAlarmST.VoltLow;
    SysPowManageST.PowAlarmST.VoltCut       = DefaultPowAlarmPoint.PowAlarmST.VoltCut;
    SysPowManageST.PowAlarmST.VoltSysStart  = DefaultPowAlarmPoint.PowAlarmST.VoltSysStart;
}

const St_RegTxt App_PowerManageRegTag[] = 
{
    {sizeof(SysPowManageST.PowAlarmST.VoltUnder     ), "欠压点"},
    {sizeof(SysPowManageST.PowAlarmST.VoltLow       ), "低压点"},
    {sizeof(SysPowManageST.PowAlarmST.VoltCut       ), "掉电点"},
};

//电源管理任务注册
int App_PowerManageReg(void)
{
    St_SAVESTRUCT AppPowerManageRegST;
    AppPowerManageRegST.fun     = App_PowerManageDefault;
    AppPowerManageRegST.data    = (unsigned char *)&SysPowManageST;
    AppPowerManageRegST.len     = sizeof(SysPowManageST) - sizeof(unsigned short);
    DataSvrReg(sizeof(App_PowerManageRegTag)/9, &AppPowerManageRegST,(St_RegTxt *)&App_PowerManageRegTag);
    return TRUE;
}
//更新系统电压状态
void UpDataSysPowState(void)
{
    if( SysPow > SysPowManageST.PowAlarmST.VoltUnder )
        SetSysPowState(STATUS_POW_NORMAL);
    else if( SysPow > SysPowManageST.PowAlarmST.VoltLow )
        SetSysPowState(STATUS_POW_UNDER);
    else if(SysPow >= SysPowManageST.PowAlarmST.VoltCut)
        SetSysPowState(STATUS_POW_LOW);
    else
        SetSysPowState(STATUS_POW_CUT);
}

//R1,R2:系统电源电压采样分压电阻，R2为采样电阻，分压系数在函数中被扩大100倍
int Task_PowerManage_Init(unsigned char R1,unsigned char R2,ADC_InitTypeDef* ADC_InitStruct,PowManageFunST *PowManageFunUser,SysPowManage_ST *pPowAlarmPoint)
{
    unsigned long SysVolt = 0;
    ADC_InitPara = *ADC_InitStruct;
    VoltMul = (R1 + R2)*100/R2;
    copy_memery( (unsigned char *)&DefaultPowAlarmPoint, (unsigned char *)pPowAlarmPoint, sizeof(SysPowManage_ST) );
    SysPowManageST.PowAlarmST.VoltUnder     = pPowAlarmPoint ->PowAlarmST.VoltUnder;
    SysPowManageST.PowAlarmST.VoltLow       = pPowAlarmPoint ->PowAlarmST.VoltLow;
    SysPowManageST.PowAlarmST.VoltCut       = pPowAlarmPoint ->PowAlarmST.VoltCut;
    SysPowManageST.PowAlarmST.VoltSysStart  = pPowAlarmPoint ->PowAlarmST.VoltSysStart;

    if(PowManageTaskNum == 0)
        PowManageTaskNum = User_AskTimeNumber();
    if( PowManageTaskNum == 0)
        return FALSE;

    //判断系统电压，超过启动电压后才启动，读取保存的参数
    while(SysVolt < SysPowManageST.PowAlarmST.VoltSysStart)
    {
        Set_WatchDog();//喂狗,2015-12-09
        OA_ADC_Init(&ADC_InitPara);
        //计算系统电源电压
        SysVolt = (unsigned long)OA_ADC_GetValue(ADCCHANNELVOLT);
            SysVolt *= AdcRefVolt;
        if(ADC_InitPara.ADCModeConversionBit == 8)
            SysVolt >>= 8;
        else
            SysVolt >>= 10;
        SysVolt *= VoltMul;
        SysVolt /= 100;
        SysPow = SysVolt;
    }
    DataSvrReadAll();
    UpDataSysPowState();
    if( STATUS_POW_NORMAL != GetSysPowState() )
    {
        Set_ValveControlRight(R_VOLTAGE);
        SysPowFlag.SysPowUndering48 = 3;
    }
    else
    {
        Reset_ValveControlRight(R_VOLTAGE);                 //清除阀门电压控制权//电压恢复时
        SysPowFlag.SysPowUndering48 = 0;
    }
    //传递回调函数
    PowManageFun = *PowManageFunUser;
    User_StartTime(SUBTIMER, PowManageTaskNum, 3, Task_PowerManage);//电源管理任务，1*3s打开一次
    return TRUE;
}
//返回系统电源电压
unsigned long GetSysPow(void)
{
    return SysPow;
}


