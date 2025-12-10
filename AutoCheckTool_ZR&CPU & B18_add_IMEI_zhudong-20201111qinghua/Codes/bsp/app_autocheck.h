/* 
 * File:   app_autocheck.h
 * Author: wb
 *
 * Created on November 7, 2016, 7:26 PM
 */

#ifndef APP_AUTOCHECK_H

#define	APP_AUTOCHECK_H

#ifdef	__cplusplus
extern "C" {
#endif

typedef enum CHECK_STATUS_EU
{
    chk_Halt,
  //  chk_Start_Pre,
    chk_Start,
    chk_MasterOnLine, 
    chk_MasterBoardSelfCheckFinish,
    chk_DeviceToolConnectMasterBoard,
    chk_find_IMEI,
    chk_MasterBoard_CloseIR,
    chk_CheckCurrent,
    chk_SetMsterBoardOutHight,
    chk_SetMsterBoardOutLow,
    chk_CheckFinish,
    chk_end
}CHECK_STATUS_E;
extern CHECK_STATUS_E Check_Status;

//输出接口
#define IO_VALVE        1   //阀门接口
#define IO_BEEP         2   //蜂鸣
#define IO_CARD_SDA     3
#define IO_CARD_SCL     4
#define IO_CARD_RST     5
#define IO_CARD_POW     6
#define IO_GUANDIAN_IN  7   //光电信号
#define IO_GUANDIAN_POW 8   //光电电源
//#define IO_TP_SDA       9
//#define IO_TP_SCL      10
//#define IO_TP_POW      11

typedef union
{
    unsigned int Status;
    struct
    {
        unsigned int Morden :1;                 //模块状态
        unsigned int Connect :1; 		//连接状态
        unsigned int SimCard :1;                //SIM卡状态
        unsigned int EEPROM :1;                 //EE状态
        unsigned int Measure:1;                 //计量状态
        unsigned int Valve:1;                   //阀门状态
        unsigned int Io119:1;                   //119状态
        unsigned int IcCard:1;                  //IC卡状态
        unsigned int Rtc:1;                     //RTC状态
        unsigned int Irda:1;                    //红外
        unsigned int TempPress:1;               //温压
        unsigned int CoverOpen:1;               //开盖检测
        unsigned int Qingxie:1;               //倾斜开关检测
        unsigned int Bluetooth:1;                     //蓝牙
        unsigned int reserve4:1;
        unsigned int reserve5:1;
    };
}U_STATUS_UT;

typedef struct CHECKPRESULT_ST
{
    unsigned char DeviceID;               //表具ID
    unsigned char MeterTYP;               //表具类型
    unsigned char IsOrNoWithIterm;        //是否带附件
    unsigned char MasterVoult;            //主控板电压
    unsigned char MasterCurrent;          //主控板电流
    unsigned char ValueSingal;            //信号强度
    unsigned char RTCVoult;               //RTC电压
    unsigned char SoftVer1;              //软件版本1
    unsigned char SoftVer2;              //软件版本2
    unsigned char Reserve;               //保留
    U_STATUS_UT Status;                  //结果状态
    unsigned char FindIMEI[20];           //存储IMEI
    unsigned char FindIMSI[20];            //存储IMSI
    //unsigned char FindICCID[20];           //存储ICCID
    unsigned char charkbat;
    unsigned char SoftVertime[6];         //软件版本编译时间
    //unsigned char StarMac[12];     // 星闪MAC（12字节ASCII + '\0'）
    //unsigned char ESAMID[8];   // esamid
    //unsigned char BANZAI[4];      // 板载压力
}CHECKPRESULT_S;
extern CHECKPRESULT_S CheckResult;



#define     MASTER_HALT                 0
#define     MASTER_SELFCHECK_FINISH     1
#define     MASTER_CONNCET_CHECK        2
#define     MASTER_CHECK_ONE            3
#define     MASTER_CHECK_TWO            4
#define     MASTER_CHECK_CURRENT        5
#define     MASTER_CHECK_END            6
#define     MASTER_IR_CLOSED            7

typedef struct DEVICETOOL_ST
{
    unsigned short SecondsTiming;
    unsigned char StatusCheck;
    unsigned char CountReapeat;
    unsigned char FlagMasterOnLineVolt;                     //主控板在线标识
    unsigned char FlagMasterBoardSelfCheckFinish;           //主控板自检完成
    unsigned char EnableDeviceToolCheck;                    //工装板使能检测
    unsigned char FlagCheckProcess;                         //检测过程的标识
    unsigned char FlagSetHightLow;                          //设置高低的标识
    unsigned char FlagSetIR;                                //设置红外
}DEVICETOOL_S;
extern DEVICETOOL_S DeviceTool;


//////////////////////////////////////////////////////////////////////////////////////////
void Init_DeviceToolIoStatus(void);
void ZdPower_IntVoid(void);
void Init_ZdMorden(void);
unsigned char GetDeviceID(void);
unsigned long CheckMasterOnLineVolt(void);
unsigned long CheckMasterRTCVolt(void);
void TaskSencondTimer(void);
void Init_SencondTimer(void);
void DeviceToolCheckTiming_Task(void);
void Ask_DeviceToolsCheck_TimerNum(void);
void Process_AutoCheck(void);
unsigned char SetConnectMasterBoard(unsigned char *data);
unsigned char SetMasterOut(unsigned char *data, unsigned char outtype);
unsigned char RecvConnectResult(unsigned char *data);
unsigned char SetMasterBoardInPut(unsigned char type);
unsigned char GetMasterOutIO_Status(unsigned char type);
unsigned char GetMasterInPutIO_Status(unsigned char *data, unsigned char intype);
unsigned char ReadCheckResult(unsigned char *data);
#ifdef	__cplusplus
}
#endif

#endif	/* APP_AUTOCHECK_H */

