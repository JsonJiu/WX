#ifndef __LIB_RS004_H__
#define __LIB_RS004_H__
#define USE_AND_OR
#include    <p24Fxxxx.h>
#include    <timer.h>
#include    <uart.h>
#include    <PPS.h>
#include    <ports.h>
#include    <rtcc.h>
#include    <adc.h>
#include "lib_public.h"

#define CLR_BIT(d,bit)      ((d) &= (~(bit)))
#define SET_BIT(d,bit)      ((d) |= (bit))

#define DEF_MAIN_CLK       8000000

typedef	enum PortNumE   //è¡¨ç¤ºä¸²å?,å®??ï¿??ADC???....ç«??ï¿??
{
    PortVmNum0,
    PortVmNum1,
    PortVmNum2,
    PortVmNum3,
    PortVmNum4,
    PortVmNum5,
    PortVmNum6,
    PortVmNum7,
    PortVmNum8,
    PortVmNum9,
    PortVmNum10,
    PortVmNum11,
    PortVmNum12,
    PortVmNum13,
    PortVmNum14,
    PortVmNum15,
    PortVmNum16,
    PortVmNum17,
    PortVmNum18,
    PortVmNum19,
    PortVmNum20,
    PortVmNum21,
    PortVmNum22,
    PortVmNum23,
    PortVmNum24,
    PortVmNum25,
    PortVmNum26,
    PortVmNum27,
    PortVmNum28,
    PortVmNum29,
}PortNumET;

typedef	enum TimeSpE
{
    TimeSpMs1,
    TimeSpMs10,
    TimeSpMs20,
    TimeSpMs50,
    TimeSpMs100,
    TimeSpMs500,
    TimeSpMs1000,
    TimeSpMs2000,
}TimeSpET;

void INT0Config(unsigned char Config, IRQServerFT Fuc);
void INT1Config(unsigned char Config, IRQServerFT Fuc);
void INT2Config(unsigned char Config, IRQServerFT Fuc);
void INT3Config(unsigned char Config, IRQServerFT Fuc);
void INT4Config(unsigned char Config, IRQServerFT Fuc);
void INT5Config(unsigned char Config, IRQServerFT Fuc);
void INT6Config(unsigned char Config, IRQServerFT Fuc);
void INT7Config(unsigned char Config, IRQServerFT Fuc);


/***********************************************************************************************************************
***********************************************************************************************************************/

//ç«??ä¸????½®
void INT0Config(unsigned char Config, IRQServerFT Fuc);
void INT1Config(unsigned char Config, IRQServerFT Fuc);
void INT2Config(unsigned char Config, IRQServerFT Fuc);
void INT3Config(unsigned char Config, IRQServerFT Fuc);
void INT4Config(unsigned char Config, IRQServerFT Fuc);
void INT5Config(unsigned char Config, IRQServerFT Fuc);
void INT6Config(unsigned char Config, IRQServerFT Fuc);
void INT7Config(unsigned char Config, IRQServerFT Fuc);
//ä¸²å???½®
void Uart0Config(pUartMode Mode, IRQServerSendDataFT SendFun, IRQServerRecvDataFT RecvFun);
void Uart1Config(pUartMode Mode, IRQServerSendDataFT SendFun, IRQServerRecvDataFT RecvFun);
void Uart2Config(pUartMode Mode, IRQServerSendDataFT SendFun, IRQServerRecvDataFT RecvFun);
void Uart3Config(pUartMode Mode, IRQServerSendDataFT SendFun, IRQServerRecvDataFT RecvFun);
//ä¸²å?ä½¿è??§å?
void Uart0SendEnable(void);
void Uart0SendDisable(void);
void Uart0ReceiveEnable(void);
void Uart0ReceiveDisable(void);
void Uart1SendEnable(void);
void Uart1SendDisable(void);
void Uart1ReceiveEnable(void);
void Uart1ReceiveDisable(void);
void Uart2SendEnable(void);
void Uart2SendDisable(void);
void Uart2ReceiveEnable(void);
void Uart2ReceiveDisable(void);
void Uart3SendEnable(void);
void Uart3SendDisable(void);
void Uart3ReceiveEnable(void);
void Uart3ReceiveDisable(void);

int Uart1SendData(unsigned char*pSendBuf, unsigned int Len);
int Uart2SendData(unsigned char*pSendBuf, unsigned int Len);
int Uart3SendData(unsigned char*pSendBuf, unsigned int Len);
int Uart4SendData(unsigned char*pSendBuf, unsigned int Len);
//MCU?¡ç?
void MCU_SleepEnter(void);
//MCUä¸???§å?
void MCU_IntEn(void);
void MCU_IntDis(void);
void MCU_DogCfg(void);              //???ï¿??
void MCU_DogReset(void);            //???
void MCUPllCfg(void);
void MCU_LcdCfg(void);   //æ¶²æ????ï¿??
void MCU_LcdOpen(void);  //???æ¶²æ?
void MCU_LcdClose(void); //?³é?æ¶²æ?
void MCU_LcdMemSet(unsigned char Reg);
void MCU_LcdAllSet(void); //?³é?æ¶²æ?
void MCU_LcdAllClr(void); //?³é?æ¶²æ?
void MCU_LcdWriteReg(unsigned char Seg, unsigned char Reg);
unsigned char MCU_LcdReadReg(unsigned char Seg);
//å®???¨æ?ï¿??
void MCU_TimerCfg(PortNumET TimeNo, TimeSpET Sp, IRQServerFT TimeServer);
void MCU_TimerStart(PortNumET TimeNo);
void MCU_TimerStop(PortNumET TimeNo);
//ADC?§å?
void MCU_ADCCfg(PortNumET ADCNum, ADCModeST Mode);   //
void MCU_ADCStart(PortNumET ADCNum);
void MCU_ADCStop(PortNumET ADCNum);
unsigned int MCU_ADCRead(void);
//å®???¶é??§å?
int RTCCfg(void);                //è¿??TRUE/FALSE
int RTCReadTime(RTC_T* Rtc);     //è¿??TRUE/FALSE
int RTCWriteTime(RTC_T* Rtc);    //è¿??TRUE/FALSE

#endif

