#ifndef __APP_CARD1608_H__
#define __APP_CARD1608_H__

#define SYS_HEAD_CODE   LCD_CHAR_P
#define SYS_VER1        1
#define SYS_VER2        0
#define SYS_VER3        0
#define SYS_VER         SYS_VER1*0x1000+SYS_VER2*0x100+SYS_VER3
/*预编译的配置信息*/
//#define UseSetup        1
//#define DELAY_CLOSE_TIME   48*60//单位：分钟

//定义卡类型，需大于0x50
#define SHEZHIKA    		0x56    //设置卡
#define YONGHUKA    		0x5a    //用户卡
#define JIANCEKA	        0x5b    //检测卡
#define QINGCHUKA   		0x57    //清除卡
#define ZHUANCUNKA  		0x58    //转存卡
#define QIEHUANKA           0x38    //计数刻度切换
#define TIAOJIAKA   		0x5c    //调价卡
#define PEIZHIKA    		0x48    //配置卡
#define OLD_TRAN_CARD 	    0x59

#define ERR_MAGNET              1       //磁攻击
#define ERR_REMAIN_FEW          10      //剩余气量不足
#define ERR_REMAIN_EMPTY        11      //剩余气量使用完毕
#define ERR_OVERDRAUGHT_EMPTY   12      //透支用气使用完毕
#define ERR_UNUSEDCLOSE         13      //闲置关阀
#define ERR_DOORBROKEN          14      //强制关阀
#define ERR_MEM                 40      //存储失败
#define ERR_OVERFLOW            15      //过流
#define ERR_119                 119

//#define ERRINVERSIONCARD        50          //未启用时反插卡
#define C_ERR_MAX               2           //充值量大于表存上限
#define C_ERR_METAL             3           //非法卡片 铜片
#define C_ERR_NOSET             4           //未设置过插入非设置卡
#define C_ERR_NOCARD            5           //读卡未完成就拔出卡片
#define C_ERR_CARDID            6           //卡号核对出错
#define C_ERR_DATA              7           //数据校验出错
#define C_ERR_PASS              8           //读卡密码错误
#define C_ERR_OPENFILE          9           //文件打开失败

#define C_ERR_COMPANYCODE       20          //非本机卡片
#define C_ERR_UNKNOW            21          //未知卡片
#define C_ERR_CARDID_RULE       22          //用户卡卡号不对
#define C_ERR_PRICE             23          //单价核对错误
#define C_ERR_PRICETIMES        24          //调价更新次数不对
#define C_ERR_CHARGETIMES       25          //充值序号错误
#define C_ERR_ADD_0             26          //充值总量为0
#define C_ERR_AR                27          //错误的加密方式
#define C_ERR_CMC               28          //错误的cmc
#define C_ERR_CARDTIMES         29          //换卡次数错误
#define C_ERR_NOCLEAN           30          //未清除检测卡
#define C_ERR_TRAN_EMP          31          //转存卡无数据
#define C_ERR_FULLDATA          32          //表中有数据，卡中也有数据（转存时）
#define C_ERR_LOCK              33          //配置卡被锁定
#define C_ERR_METERTYPE         34          //错误的卡类型
#define C_ERR_OLDCARD           35          //非新卡插入新表
#define INVERSIONCARD           51          //正常使用反插卡,错误代码编号应小于此值

typedef struct
{
    unsigned char blank1[10];           //0x00--0x09
    unsigned char RemoteType;           //0x0a  流量计类型
    unsigned char RemoteID;             //0x0b  流量计ID
    unsigned char Meter485ID;           //0x0c  表序号
    unsigned char Blank11[4];           //0x0d--0x10  
    unsigned char Overuse[3];           //0x11--0x13
    unsigned char Cardid[6];            //0x14--0x19
    unsigned char MaxFlow;              //0x1a
    unsigned char MarkUse;              //0x1b
    unsigned char CMC[4];               //0x1c--0x1f
    
    unsigned char blank2[16];           //0x20--0x2f
    unsigned char PriceV3[5];           //0x30--0x34
    unsigned char PriceIndexV3[2];      //0x35--0x36
    unsigned char MeterPriceV3[5];      //0x37--0x3b
    unsigned char MeterPriceIndexV3[2]; //0x3c--0x3d
    unsigned char blank3[2];            //0x3e--0x3f
    
    unsigned char MeterRemainMoney[6];  //0x40--0x45
    unsigned char TotalUseMoney[5];     //0x46--0x4a
    unsigned char TotalAddMoney[4];     //0x4b--0x4e
    unsigned char CardChangeTimes;      //0x4f
    unsigned char blank4[15];           //0x50--0x5e 表反写充值验证信息
    unsigned char MarkCardIn;           //0x5f
    
    unsigned char ChargeInfo[25];       //0x60--0x78
    unsigned char blank5[7];            //0x79--0x7f 表反写换卡序号
    
    unsigned char ErrCode1;             //0x80
    unsigned char MaxRemain[3];         //0x81--0x83
    unsigned char CardRemain[4];        //0x84--0x87
    unsigned char MeterRemainGas[4];    //0x88--0x8b
    unsigned char TotalUseGas[4];       //0x8c--0x8f
    
    unsigned char Mark3aa3[2];          //0x90--0x91
    unsigned char MarkClockAdjust;      //0x92
    unsigned char ErrCode2;             //0x93
    unsigned char Warning;              //0x94
    unsigned char IdleTime;             //0x95
    unsigned char MeterClock[3];        //0x96--0x98
    unsigned char blank6;               //0x99
    unsigned char Price[3];             //0x9a--0x9c
    unsigned char PriceRatio[3];        //0x9d--0x9f

    unsigned char MeterID[6];           //0xa0--0xa5
    unsigned char blank7[7];            //0xa6--0xac
    unsigned char MarkUsePriceIndex;    //0xad
    unsigned char ErrCode3;             //0xae
    unsigned char MarkNewMeter;         //0xaf
    
    unsigned char ChargeRecode1[16];    //0xb0--0xbf
    unsigned char ChargeRecode2[16];    //0xc0--0xcf
    unsigned char ChargeRecode3[16];    //0xd0--0xdf
    unsigned char ChargeRecode4[16];    //0xe0--0xef
    unsigned char ChargeRecode5[16];    //0xf0--0xff
    
}Stu_UserCard;

typedef struct
{
    unsigned char Blank1[14];           //0x00--0x0d
    unsigned char TranMarkV3;           //0x0e  用于判断新旧转存卡
    unsigned char TranMark;             //0x0f  用于判断新旧转存卡
    unsigned char Blank2[12];           //0x10--0x1b
    unsigned char CMC[4];               //0x1c--0x1f
        
    unsigned char MeterId[6];           //0x20--0x25
    unsigned char ChargeIndex[2];       //0x26--0x27
    unsigned char AmountGas[4];         //0x28--0x2b
    unsigned char Blank3[4];            //0x2c--0x2f
    
    unsigned char Price[4];             //0x30--0x33
    unsigned char PriceRatio[4];        //0x34--0x37
    unsigned char PriceV3[4];           //0x38--0x3b
    unsigned char PriceRatioV3[4];      //0x3c--0x3f
    unsigned char StepPrice[64];           //0x40--0x7f
    
    unsigned char CardId[4];            //0x80--0x83
    unsigned char Blank5[4];            //0x84--0x87
    unsigned char Remain[4];            //0x88--0x8b
    unsigned char Blank6[4];            //0x8c--0x8f
    
    unsigned char RemainMark;           //0x90 0xff-正数 0xfe-负数
    unsigned char RemainV3[12];         //0x91--0x9c
    unsigned char Blank7[51];           //0x9d--0xcf
    
    unsigned char Remain2[4];           //0xd0--0xd3
    unsigned char AmountOld[4];         //0xd4--0xd7
    unsigned char Blank8[1];            //0xd8--0xd8
    unsigned char Overdraught[3];       //0xd9--0xdb
    unsigned char Blank9[4];            //0xdc--0xdf
    
    unsigned char Warning[1];           //0xe0
    unsigned char IdleTime[1];          //0xe1
}Stu_TranCard;

typedef struct
{
    unsigned char Blank1[20];           //0x00--0x13
    unsigned char CardId[2];            //0x14--0x15
    unsigned char Blank2[4];            //0x16--0x19
    unsigned char MaxFlow;              //0x1a
    unsigned char Blank3[1];            //0x1b
    unsigned char CMC[4];               //0x1c--0x1f
    
    unsigned char Blank4[16];           //0x20--0x2f

    unsigned char PriceV3[5];           //0x30--0x34
    unsigned char PriceIndexV3[2];      //0x35--0x36
    unsigned char MeterPriceV3[5];      //0x37--0x3b
    unsigned char MeterPriceIndexV3[2]; //0x3c--0x3d
    unsigned char Blank5[2];            //0x20--0x2f
    
    unsigned char SP_Str1[4];           //0x40--0x43
    unsigned char MeterType;            //0x44
    unsigned char SP_Str2[3];           //0x45--0x47
    unsigned char MeterTime[3];         //0x48--0x4a
    unsigned char Blank6[1];            //0x1b
    unsigned char UserCMCode[4];        //0x4c--0x4f
    
    unsigned char UserKeySeed[4];       //0x50--0x53
    unsigned char IssueGasCMCode[4];    //0x54--0x57
    unsigned char UserSkeyA[4];         //0x58--0x5b
    unsigned char UserSkeyB[4];         //0x5c--0x5f
    unsigned char Crc[2];               //0x60--0x61
    unsigned char SetupCardCode[4];     //0x62--0x65
    unsigned char SetupCardCodeInv[4];  //0x66--0x69
    unsigned char IssueMoneyCMCode[4];  //0x6a--0x6d
    unsigned char Blank7[2];            //0x6e--0x6f
    
    unsigned char Price[3];             //0x70--0x72
    unsigned char PriceTime[3];         //0x73--0x75
    unsigned char PriceIndex;           //0x76
    unsigned char PriceType[2];         //0x77--0x78
    unsigned char PriceUpdate;          //0x79
    unsigned char SP_Str4[23];          //0x7e--0x95
    unsigned char PriceRatio[3];        //0x96--0x98        
}Stu_SetupCard;

typedef union
{
    unsigned char buf[0xff];
    Stu_UserCard UserCard;
    Stu_TranCard TranCard;
    Stu_SetupCard SetupCard; 
}Stu_Card;

typedef struct
{
    unsigned long ChargeAmount;
    unsigned int  ChargeIndex;
    unsigned int  CRC;
}Stu_EncodeInfo;

typedef enum {
    //STATUS_METER_UNUSED = 0,       //出厂状态，未初始化，需要写入一些参数，显示程序版本号SP X-XX
    STATUS_METER_INIT = 0,         //出厂状态，已经初始化，未设置，显示hello
    STATUS_METER_SETUP,        //设置状态
    STATUS_METER_NOUSER_USE,   //无用户的状态的检表
    STATUS_METER_CHECK,        //检表状态
    STATUS_METER_INUSE,        //正常用户状态
    STATUS_METER_OUTBAND,      //解除绑定状态，解绑状态下不允许透支。
    STATUS_METER_OVERUSE,      //透支状态
}
CardStateVal;

typedef enum
{
    CONFIG_OverDraught = 0,             
    CONFIG_LcdSleepTime,           
    CONFIG_UseSetupCard,                    
    CONFIG_HallCheck,                 
    CONFIG_NoHallSigAttact,                 
    CONFIG_CleanTest,                
    CONFIG_NoUserUse,                   
    CONFIG_UseSigPrice,                    
    CONFIG_NoBandUse,                  
    CONFIG_VerifyCompanyCode,   
} ConfigValue;

typedef struct
{
    unsigned char   OverDraught;	      //透支量	                 默认50（0.5方）
    unsigned int    LcdSleepTime;           //液晶休眠时间（100ms)	 默认20s
    unsigned char   UseSetupCard      : 1;  //1--使用设置卡             0--不使用
    unsigned char   HallCheck         : 1;  //1--双霍尔计数             0--单霍尔计数
    unsigned char   NoHallSigAttact   : 1;  //1--不使用单边磁攻击       0--使用
    unsigned char   CleanTest         : 1;  //1--使用检测卡后要清除     0--不清除
    unsigned char   NoUserUse         : 1;  //1--无用户状态下允许透支   0--不允许
    unsigned char   UseSigPrice       : 1;  //1--使用单一价格           0--使用梯级价
    unsigned char   NoBandUse         : 1;  //1--设置卡解绑后不关阀     0--关阀
    unsigned char   VerifyCompanyCode : 1;  //1--验证公司代码           0--不验证公司代码
} StuConfig, *pStuConfig;

typedef struct
{
    unsigned int  EncodeChargeIndex;    //验证区的充值序号
    unsigned char CardCMC[4];
    unsigned char KeySeed[4];
    unsigned char sKeyA[3];
    unsigned char sKeyB[3];
    unsigned char CompanyCode[5];
    unsigned char UserCardIndex[4];
    CardStateVal    CardState;           //卡状态
    StuConfig       Config;              //配置参数
    unsigned char CardId[10];          //卡号
}StuUser,*pStuUser;                      

//typedef struct
//{
//    unsigned char Period;       //阶梯周期
//    unsigned char ActiveTime[4];//执行时间，BCD码，年、月、日、时
//    unsigned char Price1[4];    //更改为HEX格式,低位在前,4位小数,共4字节.
//    unsigned char Amount1[4];   //更改为HEX格式,低位在前,2位小数,共4字节.
//    unsigned char Price2[4];
//    unsigned char Amount2[4];
//    unsigned char Price3[4];
//    unsigned char Amount3[4];
//    unsigned char Price4[4];
//    unsigned char Amount4[4];
//    unsigned char Price5[4];
//    unsigned char Amount5[4];
//    unsigned char PriceSerialNum[4];//价格更新序号，HEX
//    unsigned char PeriodTime[2];    //阶梯周期起始时间，BCD码，年、月、日
//    unsigned char PeriodFlag;       //标志位
//    unsigned char CRC;              //累加和校验
//}StuStepPriceInfo,*pStuStepPriceInfo;

//typedef struct
//{
//    StuUser             UserPara;           //用户数据
//    StuStepPriceInfo    StepPrice;          //阶梯价格表
//    unsigned long       Price;              //当前单价(4位小数)
//    unsigned long       PriceRatio;       //金额系数
//    unsigned long 	    Maxremain;           //表存上限
//    unsigned long 	    UnusedCloseTime;     //闲置关阀时间（小时）
//    unsigned long 	    Overdraught;         //透支量 2位小数
//    unsigned long 	    Warning;             //报警量 2位小数
//    unsigned long 	    Charge;              //当前存入量
//    unsigned long 	    ChargeAmount;        //总充值量
//    unsigned int        PriceIndex;
//    unsigned int        ChargeIndex;
//    T_StateVal          State;               //表具状态
//    unsigned char       CardId[10];
//} StuMeter, *pStuMeter;

void BeepMs(unsigned int ms);
void Card_Init(void);
void LoadDefaultConfig(void);
void RegCardData(void);
void TaskCheckCard(void);
CardStateVal GetCardState(void);
CardStateVal SetCardState(CardStateVal val);
long GetRemainValue(void);
unsigned char SetRemainValue(long val);
unsigned int GetConfigValue(ConfigValue ValueType);
unsigned char card_peizhika_process(void);
unsigned char card_shezhika_process(void);
unsigned char card_yonghuka_process(void);
unsigned char card_jianceka_process(void);
unsigned char card_zhuancunka_process(void);
unsigned char	old_tran_process(void);
unsigned char card_qingchuka_process(void);
unsigned char card_qiehuanka_process(void);

//BSP
extern void BspBeepOn(void);
extern void BspBeepOff(void);
extern unsigned char CardBslRead(void);
extern void CardPowerCheckInput(void);
extern unsigned char CardPowerCheckRead(void);
extern void CardPowerContrlOutput(void);
extern void CardPowerContrlSet(void);
extern void CardPowerContrlClr(void);
extern void CardRstOutput(void);
extern void CardRstClr(void);
extern void CardSclOutput(void);
extern void CardSclClr(void);
extern void CardSdaOutput(void);
extern void CardSdaClr(void);
extern void CardIntConfig(unsigned char c,void(*p)(void));
extern void DelayMs(unsigned int Ms);
//APP
extern void RefreshRemainVal(void);


#endif
