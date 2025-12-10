#ifndef __LIB_PUBLIC_H__
#define __LIB_PUBLIC_H__

typedef void(*IRQServerRecvDataFT)(unsigned char Data);
typedef void(*IRQServerSendDataFT)(unsigned char Data);
typedef void(*IRQServerFT)(void);

typedef struct
{
    unsigned char	Year;	//BCD骞翠唤浣庝綅
    unsigned char	Mon;	//BCD
    unsigned char	Day;	//BCD
    unsigned char	Hour;	//BCD
    unsigned char	Min;	//BCD
    unsigned char	Sec;	//BCD
    unsigned char	Week;	//BCD
}RTC_T;

typedef enum UartBaud//琛ㄧず娉㈢壒锟??
{
    UartBaud1200 = 0,
    UartBaud2400,
    UartBaud4800,
    UartBaud9600,
    UartBaud19200,
    UartBaud38400,
    UartBaud57600,
    UartBaud62500,
    UartBaud115200,
}UartBaud;

typedef struct
{
    UartBaud Baud;

    unsigned char Parity : 2;   // 0:鏃犳牎锟?? 1:Odd濂囨牎锟?? 2:Even鍋舵锟??
    unsigned char StopBit : 2;   // 0:鏃犲仠姝綅; 1:1浣嶅仠姝綅; 2:2浣嶅仠姝綅
    unsigned char DataBit : 2;   // 0:7浣嶆暟鎹綅; 1:8浣嶆暟鎹綅; 2:9浣嶆暟鎹綅
    unsigned char Sequence : 1;   // 0:LSB;  1:MSB
}UartMode, *pUartMode;

typedef enum
{
    DISABLE = 0,
    ENABLE = !DISABLE
}FunctionalState;

typedef struct ADCModeS
{
    unsigned char ADCModeOpt : 2;//0:单次//1:连续
    unsigned char ADCModeResolution : 2;//0:8bit//1:10bit//2:12bit
    unsigned char ADCModeVREFU : 2;//0:VDD//1:内部基准//2:外部正基准
    unsigned char ADCModeVREFD : 2;//0:VSS//1:外部负基准
}ADCModeST;

typedef struct
{
    unsigned char ADCModeConversionMode;   //????,0:??,1:??
    unsigned char ADCModeChannel;          //????,0:????,1:????
    unsigned char ADCModeConversionBit;    //??????,8:8?,10:10?;12:12?
    unsigned char ADCRef;                  //????????,0:??1.45V,1:VDD
}ADC_InitTypeDef;


#define BIT0             0x01 /*Port Bit 0*/
#define BIT1             0x02 /*Port Bit 1*/
#define BIT2             0x04 /*Port Bit 2*/
#define BIT3             0x08 /*Port Bit 3*/
#define BIT4             0x10 /*Port Bit 4*/
#define BIT5             0x20 /*Port Bit 5*/
#define BIT6             0x40 /*Port Bit 6*/
#define BIT7             0x80 /*Port Bit 7*/

/*****************************************************************************/

#define PARITY_NONE     0
#define PARITY_ODD      1
#define PARITY_EVEN     2
#define STOP_NONE       0
#define STOP_1          1
#define STOP_2          2

#define DATA_7          0
#define DATA_8          1
#define DATA_9          2
#define LSB             0

#define MSB             1
#ifndef BOOL
#ifndef FALSE
#define FALSE           0
#define TRUE            1
#endif
#endif

extern unsigned short AdcRefVolt;//ADC采集基准,单位mV,只读,不可更改

void DelayUs(unsigned int Us);
void DelayMs(unsigned int Ms);
void MCU_SleepEnter(void);
void MCU_IntEn(void);
void CBB_NOP(void);
void MCUPllCfg(void);
unsigned short GetRandom(void);
int RTCCfg(void);                //返回RUE/FALSE
int RTCReadTime(RTC_T* Rtc);     //返回RUE/FALSE
int RTCWriteTime(RTC_T* Rtc);    //返回RUE/FALSE
//void RegOneSecIRQFun(IRQServerFT CallBack);
#endif

