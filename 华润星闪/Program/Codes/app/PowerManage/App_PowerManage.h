#ifndef __APP_POWERMANAGE_H__
#define __APP_POWERMANAGE_H__
#include "drv_ad.h"


//电源管理注册结构体
typedef struct PowAlarm_S
{
    unsigned short VoltUnder;       //欠压点
    unsigned short VoltLow;         //低压点
    unsigned short VoltCut;         //掉电点
    unsigned short VoltSysStart;    //启动电压点,系统电压大于此电压点才会启动
}PowAlarm_ST;

typedef struct SysPowManage_S
{   
    PowAlarm_ST PowAlarmST;
}SysPowManage_ST;

extern SysPowManage_ST SysPowManageST;
//系统电源状态
typedef enum SysPowState_E
{
	STATUS_POW_NORMAL = 0,
	STATUS_POW_UNDER,
	STATUS_POW_LOW,
	STATUS_POW_CUT
}SysPowState_ET;

extern SysPowState_ET SysPowState;

//48小时倒计时时间结构体
typedef struct RemainTime_S
{
    unsigned char Hour;
    unsigned char Min;
    unsigned char Sec;
}RemainTime_ST;

extern RemainTime_ST RemainTimeST;

//系统电源标志
typedef struct SysPowFlag_S
{
    unsigned char SysPowNormalToUnder       : 2;	//由常压进入欠压，欠压上告需要 取值 0,1,2,3
    unsigned char SysPowUndering48          : 2;	//电压状态,0:正常，1：48小时倒计时中，2：倒计时结束，3：电压处于非常压状态
    unsigned char SysPowPowOn               : 1;        //系统上电（非掉电到上电）标志
}SysPowFlag_ST;

//定义电源管理任务回调函数指针数组
typedef void (*PowManageFunFT)(void);
typedef struct
{
    PowManageFunFT FunNormalToCut;
    PowManageFunFT FunNormalToLow;
    PowManageFunFT FunNormalToUnder;
    PowManageFunFT FunUnderToNormal;
    PowManageFunFT FunUnderToLow;
    PowManageFunFT FunUnderToCut;
    PowManageFunFT FunLowToNormal;
    PowManageFunFT FunLowToUnder;
    PowManageFunFT FunLowToCut;
    PowManageFunFT FunCutToNormal;
    PowManageFunFT FunCutToUnder;
    PowManageFunFT FunCutToLow;
}PowManageFunST;
extern PowManageFunST PowManageFun;

extern SysPowFlag_ST SysPowFlag;

//引用外部
extern void DelayMs(unsigned int Ms);
  
//应用对外接口
void UpDataSysPowState(void);//更新电压状态
extern unsigned char PowManageTaskNum;
extern ADC_InitTypeDef ADC_InitPara;
void Task_PowerManage(void);
int App_PowerManageReg(void); //任务注册
void FunPowerManageDefault(void);//空函数
int Task_PowerManage_Init(unsigned char R1,unsigned char R2,ADC_InitTypeDef* ADC_InitStruct,PowManageFunST *PowManageFunUser,SysPowManage_ST *pPowAlarmPoint);
SysPowState_ET GetSysPowState(void);
unsigned long GetSysPow(void);
int IsSysPowUndering48(void); 
unsigned int GetRemain48Hour(RemainTime_ST *RemainTime);
//WB专用
int GetSysPowNormalToUnder(void);
void ClrSysPowNormalToUnder(void);
void SetSysPowNormalToUnder(unsigned char a);
void SetGPRSIsRunning(void);
void ClrGPRSIsRunning(void);


#endif
