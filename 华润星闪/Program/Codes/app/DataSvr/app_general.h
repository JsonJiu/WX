#ifndef _APP_GENERAL_H_
#define _APP_GENERAL_H_

#include "include.h"

/*
 * 表具运行的各状态值
 */
typedef enum {

    STATE_REMAIN_NORMAL = 0,        //存量足
    STATE_REMAIN_LOW,           //低存量
    STATE_REMAIN_ZERO,          //零
    STATE_REMAIN_NEGTIVE,       //透支
    STATE_REMAIN_NEGTIVEOVER,   //透支结束

    STATE_MEASURE_NORMAL,       //计量设备正常
    STATE_MEASURE_READING,       //计量设备正在读取
    STATE_MEASURE_BROKEN,       //计量设备损坏

    STATE_FREEZE_NONE,
    STATE_FREEZE_OK,

    STATE_ERR,
}T_StateVal;

typedef enum
{
    SETUP_Precision_001 = 0,
    SETUP_Precision_010,
    SETUP_Precision_100,

    SETUP_MeterType_GAS,
    SETUP_MeterType_MONEY,
    
    SETUP_CloseWarning_OFF,
    SETUP_CloseWarning_ON,

    SETUP_CloseOver_OFF,
    SETUP_CloseOver_ON,

    SETUP_PostPaid_OFF,     //后付费
    SETUP_PostPaid_ON,      //预付费

    SETUP_CntPay_OFF,
    SETUP_CntPay_ON,


    SETUP_OverFlow_OFF,
    SETUP_OverFlow_ON,

    SETUP_UnusedClose_OFF,
    SETUP_UnusedClose_ON,

    SETUP_Freeze_OFF,
    SETUP_Freeze_ON,
    
    SETUP_ON,
    SETUP_OFF,
    
    SETUP_VAL_OTHER,
} T_SetupVal;


typedef enum
{
    STATE_REMAIN = 0,
    STATE_MEASURE,      //计量设备状态
    STATE_FREEZE,       //冻结状态
} T_State;

typedef enum
{
    SETUP_MeterType = 0,            //表类型
    SETUP_PostPaid,                 //预付费
    SETUP_CntPay,                   //阶梯计费
    SETUP_Precision,                //计量精度
    SETUP_OverFlow,                 //过流开关
    SETUP_UnusedClose,              //闲置关阀开关
    SETUP_CloseWarning,             //少额、气关阀
    SETUP_CloseOver,                //透支关阀
    SETUP_Freeze,
} T_Setup;



typedef enum
{
    FAULT_NOFAULT =0,
    FAULT_Magnet,  //       :1;//磁攻击
    FAULT_DoorBroken,//     :1;//阀门故障
    FAULT_RemainEmpty,//    :1;//余量使用完毕
    FAULT_OveruseUp,//      :1;//透支使用完毕
    FAULT_UnusedTimeup,//   :1;//闲置关阀
    FAULT_Warning119,//     :1;//119告警
    FAULT_UnderPowTimeup,// :1;//欠压时间到
    FAULT_OverFlow,//       :1;//过流保护
    FAULT_MemFault,//       :1;//存储故障
    FAULT_WaitInv,//        :1;//等待反插卡
    FAULT_NoUser,//         :1;//无用户状态
    FAULT_Power,
    FAULT_FORCECLOSE,
    FAULT_Other,
} T_FaultVal;

typedef enum
{
    BASEVAL_Amount = 0,                 //累计气量整数(2位小数)
    BASEVAL_PeriodAmountGas,            //周期内的使用气量
    BASEVAL_Price,                      //当前单价(4位小数)
    BASEVAL_Remain,                     //剩余量(2位小数)
    BASEVAL_RemainInt,                  //剩余量整数
    BASEVAL_RemainDec,                  //剩余量小数(小数位数根据DecNum确定)；当剩余量为正，剩余小数=RemainDec；当透支状态时，剩余小数=DecNum - RemainDec;
    BASEVAL_RemainVol,                  //剩余气量(2位小数)
    BASEVAL_Negtiveuse,                 //负用量
    BASEVAL_Totaladd,                   //存入总量(从表生下来开始的充值总量)
    BASEVAL_Fault,                      //错误代码
    
    USERVAL_Maxremain,                  //表存上限
    USERVAL_MaxAmount,                  //最大累计量
    USERVAL_UnusedCloseTime,            //闲置关阀时间（小时）
    USERVAL_Overdraught,                //透支量2位小数
    USERVAL_Warning,                    //报警量2位小数
    USERVAL_ChargeAmount,               //当前存入量
    USERVAL_Maxflow,                    //流量上限
    USERVAL_ChargeIndex,                //充值序号2字节
    USERVAL_PriceIndex,                 //调价序号2字节

    GLOBEVAL_RemainTime,                //2字节 欠压倒计时 分钟
    GLOBEVAL_ForceCloseTimes,           //1字节 强制关阀次数
    GLOBEVAL_UnuseCloseTimes,           //1字节 闲置关阀次数
} T_Value;



typedef struct
{
    T_SetupVal      MeterType;          //表类型
    T_SetupVal      PostPaid;           //后付费开关
    T_SetupVal      CntPay;             //开启阶梯计费开关
    T_SetupVal      Precision;          //计量精度
    T_SetupVal      OverFlow;           //过流开关
    T_SetupVal      UnusedClose;        //闲置关阀开关
    T_SetupVal      CloseWarning;       //少额、气关阀告警
    T_SetupVal      CloseOver;          //透支关阀告警
    T_SetupVal      Freeze;             //冻结开关
} StuSetup;

typedef struct
{
    unsigned char       Years;              //年
    unsigned char       Months;             //月
    unsigned char       Days;               //日
    unsigned char       Hours;              //时
    unsigned char       Minus;              //分
    unsigned char       Data[4];            //冻结气量
    unsigned char       State[2];
    unsigned char       Crc[2];
} StuFreezeDate;

typedef struct
{
    unsigned char       Days;
    unsigned char       Hours;
    unsigned char       Minus;
//    unsigned char       Seconds;
}StuFreezeTime, *pStuFreezeTime;

typedef enum
{
	CntPay_HALL = 0,	          //霍尔
	CntPay_ZD,		          //直读头
	CntPay_CSB,		          //超声波
}T_CntPay_Device;


/*
 * 表具运行参数，掉电等状态需要保存,上电重新读取
 */
typedef struct
{
    unsigned char       MeterID[20];        //表号
    unsigned char       BootLoader;         //是否进入升级模式
	T_CntPay_Device 	CntPay_Device;		//计量设备
    T_StateVal          State;              //表具状态

    unsigned long       ConsumeCount;           //计数

    unsigned char       UnuseCloseTimes;    //闲置关阀次数

    unsigned long       Amount;          //累计气量(2位小数)
    unsigned long       PeriodAmountGas;    //周期内的使用气量
    unsigned long       Negtiveuse;         //负用量
    unsigned long       Price;              //当前单价(4位小数)
    unsigned long       RemainInt;          //剩余整数
    unsigned long       RemainDec;          //剩余小数（7位）(剩余量为正时，剩余小数为+，透支时，1-)
    long                 RemainVol;          //剩余气量2位小数
    unsigned long       Overdraught;        //透支量
    long                 Totaladd;           //存入总量(从表生下来开始的充值总量)
    long                 ChargeAmount;       //当前存入量

    unsigned long       Warning;            //报警量 2位小数
    unsigned int        Maxflow;            //流量上限
    unsigned long       Maxremain;          //表存上限 2位小数
    unsigned long       MaxAmount;          //最大累计量 2位小数
    unsigned long       UnusedCloseTime;    //闲置关阀时间（小时）
    
    unsigned int        PriceIndex;         //调价序号
    unsigned int        ChargeIndex;        //充值序号
    StuFreezeTime       FreezeDate;          //冻结日
    T_StateVal          Freezeed;           //当月冻结状态
} StuMeter, *pStuMeter;


typedef union
{
    unsigned int Value;
    struct
    {
        unsigned int Magnet         : 1; //磁攻击
        unsigned int DoorBroken     : 1; //阀门故障
        unsigned int RemainEmpty    : 1; //余量使用完毕
        unsigned int OveruseUp      : 1; //透支使用完毕
        unsigned int UnusedTimeup   : 1; //闲置关阀
        unsigned int Warning119     : 1; //119告警
        unsigned int UnderPowTimeup : 1; //欠压时间到
        unsigned int OverFlow       : 1; //过流保护
        unsigned int MemFault       : 1; //存储故障
        unsigned int WaitInv        : 1; //等待反插卡
        unsigned int NoUser         : 1; //无用户状态
        unsigned int Power          : 1; //低压或掉电
        unsigned int ForceClose     : 1; //强制关阀
        unsigned int Other          : 1; //其他错误
    };
} StuMarkFault;


typedef struct
{
    T_StateVal   Remain;        //余量不足报警
    T_StateVal   Door;                  //阀门状态
    T_StateVal   Measure;            //直读头状态
} StuMarkState;






extern StuMeter         Meter;
extern StuSetup         Setup;      //配置参数，上电初始化，不需存储
extern StuMarkState     MeterState;
extern StuMarkState    State;
extern RTC_T           RTC;

extern const unsigned char month[];


long GetValue(T_Value ValueType);
unsigned char SetValue(T_Value ValueType, long val);

T_SetupVal SetSetupVal(T_Setup tSetup, T_SetupVal val);
T_SetupVal GetSetupVal(T_Setup tSetup);

T_StateVal SetState(T_State tstate, T_StateVal val);
T_StateVal GetState(T_State tstate);

unsigned char SetFreezeTime(pStuFreezeTime time);
unsigned char GetFreezeTime(pStuFreezeTime time);

void GetDate(RTC_T* t);
void SetDate(RTC_T date);
void RegGeneralData(void);
unsigned char DaysInThisMonth(unsigned char m);
unsigned char CheckFeb(unsigned long year);
#endif

