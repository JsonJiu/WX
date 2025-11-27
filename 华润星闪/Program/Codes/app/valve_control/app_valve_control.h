#ifndef APP_VALVE_CONTROL_H
#define	APP_VALVE_CONTROL_H

typedef enum VALVE_STATUS_E
{
    NO_STARUS,OPEN_STATUS,OPENING_STATUS,CLOSE_STATUS,CLOSING_STATUS,ERR_STATUS
}VALVE_STATUS_EU;

#define R_KEY			0x01//
#define R_VOLTAGE		0x02//
#define R_EVENT           	0x04//
#define R_COMMUNICATION		0x08//
#define R_CONSUME		0x10//


#define OPT_OPEN_VALVE          0
#define OPT_CLOSE_VALVE         1
#define OPT_EXIT_CONTROL        2


typedef enum
{
    BASEVAL_TIMEOPENVALVE = 0,  //开阀时间
    BASEVAL_TIMECLOSEVALVE,     //关阀时间
    BASEVAL_TIMETWOCLOSEVALVE,  //二次关阀时间
    BASEVAL_TIMETWOOPENVALVE,   //二次开阀时间
    BASEVAL_VALUEOVERCRRUNT,    //过流值
    BASEAL_POWERONOPENVALVE,    //获取上电是否开阀的标志
    BASEAL_OWERCLOSEVALVE,      //获取欠压是否关阀的标志
    BASEL_OVERTIMEVALUE,        //商业表阀门超时时间最大值
} T_ValveValue;

typedef enum
{
    POWERON_NOTOPEN = 0,        //上电按键开阀
    POWERON_OPEN,           //上电直接开阀
} F_POWERONOPENVALVE;

typedef enum
{
    OWER_NOTCLOSEVALVE = 0,     //欠压不关阀
    OWER_CLOSEVALVE,        //欠压关阀
} F_OWERCLOSEVALVE;

typedef struct PARAFLGA_S
{
    unsigned char PowerOnOpenValve;                 //上电直接开阀的标志（0：上电需要按键开阀 1：上电直接开阀；）（符合开阀条件时）
    unsigned char OwerVol_CloseValve;               //欠压关阀标志（0：欠压不关阀；1：欠压关阀）
}PARAFLGA_ST;
extern PARAFLGA_ST ParaFlag;

/**
* VALVE_ST 结构体中的时间变量均以100毫秒为单位
*/
typedef struct VALVE_S
{
    VALVE_STATUS_EU Status;
    unsigned char Right_Control;//阀门控制权（按键控制，欠压低压掉电控制，磁攻击，远程控制，欠费控制）
    unsigned char TimeValue_OpenValve;//开阀时间
    unsigned char TimeValue_CloseValve;//关阀时间
    unsigned char TimeValue_TwoCloseValve;//二次关阀的关时间值(为0时，表示无阀，为1时，表示有阀)
    unsigned char TimeValue_TwoOpenValve;//二次开阀的开时间值（为0时，表示非二次关阀，为1时表示二次关阀）
    unsigned short Value_OverCurrent;//阀门过流的值
    unsigned char Flag_Communicate_OpenCloseValve;//（0：强制关阀；1：强制开阀；2：退出强制）
    PARAFLGA_ST ParaFlag;//阀门参数标志
    unsigned short OpenCloseOverTime_Time;      //商业表开关阀超时时间（默认值为300S=5分钟）
    unsigned char CommerceMeter_OptTime;       //商业表阀门操作定时
    unsigned char CheckOverCurrent_Time;       //检测过流的定时
    unsigned char Flag_OverCurrent;
}VALVE_ST;
extern VALVE_ST Valve;



extern unsigned char Flag_BeepAlrm;

extern void RegValveData(void);

extern void OV_OpenValve(void);
extern void OV_CloseValve(void);
extern void OV_ValveControlLogic(void);
extern void Set_ValveControlRight(unsigned char type);
extern void Reset_ValveControlRight(unsigned char type);
extern VALVE_STATUS_EU Get_ValveStatus(void);
extern unsigned char Get_FlagValveOverCurrrent(void);
extern void Set_CommunicateOpenCloseValveFlag(unsigned char flag);
extern unsigned char Get_CommunicateOpenCloseValveFlag(void);
extern unsigned char SetValveParaValue(T_ValveValue valvevalue_type, unsigned short val);
extern unsigned short GetValveParaValue(T_ValveValue valvevalue_type);
extern void Set_BeepAlarm_Flag(void);
extern void ClearBeepAlarm_Flag(void);
extern unsigned char GetBeepAlarm_Flag(void);
extern void OV_OpenValve_Opt(unsigned int ms);
extern void OV_CloseValve_Opt(unsigned int ms);
extern void OV_Valve_Init(unsigned char flag_PowerOnOpenValve, unsigned char flag_OwerVol_CloseValve);

#endif


