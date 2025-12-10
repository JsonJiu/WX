#ifndef __DRV_RFM4463_H__
#define __DRV_RFM4463_H__

#include "lib_public.h"

//本头文件提供所有对应用开放的接口以及所需全局变量
#define RFM4463REVBUFMAXLEN		200

typedef enum
{
   RFM4463_CTRL_DEEPSLEEP = 0,
   RFM4463_CTRL_SLEEP,
   RFM4463_CTRL_IDLE,
   RFM4463_CTRL_CONFIG,
   RFM4463_CTRL_SEND,
   RFM4463_CTRL_RECEIVE,
   RFM4463_CTRL_RESET,
}Rfm4463_CtrlStateET;

extern Rfm4463_CtrlStateET Rfm4463_CtrlState_ET;

extern unsigned char Rfm4463RevBuf[RFM4463REVBUFMAXLEN];

extern UartMode Rfm4463_UartMode_ST;

extern void DelayMs(unsigned int Ms);

//Rfm4463无线模块接口函数
int OR_Rfm4463_Init(void);
int OR_Rfm4463_Control(Rfm4463_CtrlStateET Rfm4463_CtrlState_ET);
unsigned short OR_Rfm4463_RecData(unsigned char *Buf);
int OR_Rfm4463_SendData(unsigned char *Buf,unsigned short SendLen);
int OR_Rfm4463_SetConfigValue(unsigned char *Agc);
int OR_Rfm4463_GetConfigValue(unsigned char *Agc);
void OR_Rfm4463_RfRecvChek(void);
unsigned char GetRFState(void);

//bsp接口
void OR_Rfm4463_RFDIO_DIR_Ctrl(unsigned char dir);
void OR_Rfm4463_RFDIO_Set(void);
void OR_Rfm4463_RFDIO_Reset(void);
void OR_Rfm4463_ACK_DIR_Ctrl(unsigned char dir);
unsigned char OR_Rfm4463_ACK_Read(void);
void OR_Rfm4463_RERCV_Dir_Ctrl(unsigned char dir);
void OR_Rfm4463_RERCV_Set(void);
void OR_Rfm4463_RERCV_Reset(void);
void OR_Rfm4463_SDI_DIR_Ctrl(unsigned char dir);
unsigned char OR_Rfm4463_SDI_Read(void);
void OR_Rfm4463_RFCLK_DIR_Ctrl(unsigned char dir);
unsigned char OR_Rfm4463_RFCLK_Read(void);
void OR_Rfm4463_SDO_DIR_Ctrl(unsigned char dir);
void OR_Rfm4463_SDO_Set(void);
void OR_Rfm4463_SDO_Reset(void);
void OR_Rfm4463_RST_DIR_Ctrl(unsigned char dir);
void OR_Rfm4463_RST_Set(void);
void OR_Rfm4463_RST_Reset(void);
void OR_Rfm4463_SCK_DIR_Ctrl(unsigned char dir);
void OR_Rfm4463_SCK_Set(void);
void OR_Rfm4463_SCK_Reset(void);
void OR_Rfm4463_NSS_DIR_Ctrl(unsigned char dir);
void OR_Rfm4463_NSS_Set(void);
void OR_Rfm4463_NSS_Reset(void);
void OR_Rfm4463_RFTX_DIR_Ctrl(unsigned char dir);
void OR_Rfm4463_RFTX_Set(void);
void OR_Rfm4463_RFTX_Reset(void);
void OR_Rfm4463_RFRX_DIR_Ctrl(unsigned char dir);
void OR_Rfm4463_RFRX_Set(void);
void OR_Rfm4463_RFRX_Reset(void);
void OR_Rfm4463_RSSI_DIR_Ctrl(unsigned char dir);
unsigned char OR_Rfm4463_RSSI_Read(void);
void OR_Rfm4463_Uart_Init(UartMode* UartMode_ST, void(*fun)(unsigned char));
void OR_Rfm4463_Uart_Cmd(FunctionalState NewState);
unsigned char OR_Rfm4463_Uart_ReceiveData(void);
void OR_Rfm4463_Uart_Interrupt(unsigned char RxData);

#endif
