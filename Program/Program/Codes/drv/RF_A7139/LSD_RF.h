////////////////////////////////////////////////////////////////////////////////
// 版权:     利尔达科技集团股份有限公司
// 文件名:   LSD_RF.h
// 版本：    v1.0
// 工作环境: IAR  v5.30
// 作者:     于海波
// 生成日期: 2013.11
// 功能:     
// 相关文件: 
// 修改日志：
// LSD4RF-2D717N10   VCC  GND  SDIO  SCK  GPIO2  CK0  GPIO1  SCS  WP  SDA
// LSD4RF-2D717N01   VCC  GND  SCS   SCK  SDIO   SDA  GPIO2  WP
//
////////////////////////////////////////////////////////////////////////////////
#ifndef _LSD_RF_h_
#define _LSD_RF_h_

#define Sint8  signed   char
#define Uint8  unsigned char
#define uint8  unsigned char
#define Uint16 unsigned short
#define uint16 unsigned short
#define Uint32 unsigned long
#define uint32 unsigned long
#define Sint32 signed long

#define RF_Init LSD_RF_InitRF

/*********************************************************************
**     根据平台修改
*********************************************************************/
//SDIO
#define LSD_SDIO_OH       RF_SETA_Set()
#define LSD_SDIO_OL       RF_SETA_Clr()
#define LSD_SDIO_IN       RF_SETA_Get()
#define LSD_SDIO_MI       RF_SETA_Init(IN)
#define LSD_SDIO_MO       RF_SETA_Init(OUT)
//SCK
#define LSD_SCK_MO        RF_RXD_Init(OUT)
#define LSD_SCK_H         RF_RXD_Set()
#define LSD_SCK_L         RF_RXD_Clr()
//GIO2
//CKO
#define LSD_CKO_MO        A7139_CKO_Out()
#define LSD_CKO_H         A7139_CKO_Set()
#define LSD_CKO_L         A7139_CKO_Clr()
//GIO1
#define GIO1_IN           RF_AUX_Get()
#define LSD_GIO1_MI       RF_AUX_Init(IN)
#define IRQ_Int_Init      RF_AUX_IntEn(A7139_GIO1_IRQServer)
#define A7139_GIO1_IntEn  RF_AUX_IntEn(A7139_GIO1_IRQServer)
#define A7139_GIO1_IntDis RF_AUX_IntDis()

//SCS
#define LSD_SCS_MO        RF_TXD_Init(OUT)
#define LSD_SCS_H         RF_TXD_Set()
#define LSD_SCS_L         RF_TXD_Clr()


//DELAY
#define LSD_DELAY         CBB_NOP()


typedef struct
{
  unsigned short RXLen;    //?è???????????§?????¤???????á??RXOK??1
  unsigned char RXOK;     //???????????¨?¤????????
  unsigned short TXLen;    //?è??·??????¤??
}StuA7139Config;

extern StuA7139Config A7139RunConfig;

///*********************************************************************
//**  function Declaration
//*********************************************************************/
void LSD_RF_ByteSend(Uint8 src);
Uint8 LSD_RF_ByteRead(void);
void LSD_RF_StrobeCMD(Uint8 cmd);
void LSD_RF_WriteReg(Uint8 address, Uint16 dataWord);

Uint16 LSD_RF_ReadReg(Uint8 address);
void LSD_RF_WritePageA(Uint8 address, Uint16 dataWord);
Uint16 LSD_RF_ReadPageA(Uint8 address);
void LSD_RF_WritePageB(Uint8 address, Uint16 dataWord);
Uint16 LSD_RF_ReadPageB(Uint8 address);
Uint8 LSD_RF_InitRF(void);
Uint8 LSD_RF_Config(void);
Uint8 LSD_RF_WriteID(void);
void LSD_RF_FreqSet(Uint8 ch);
void LSD_RF_FreqCalSet(Uint8 ch);
Uint8 LSD_RF_Cal(void);
//void LSD_RF_WriteFIFO(void);
void LSD_RF_SendPacket(Uint8 *txBuffer, Uint8 size);
//void LSD_RF_RxPacket(void);
void LSD_RF_RxPacket(Uint8 *cRxBuf, Uint8 cLength);
void LSD_Err_State(void);
void LSD_RF_RCOSC_Cal(void);
void LSD_RF_RXmode(void);
void LSD_RF_Sleepmode(void);

void SendAgain(void);


 void A7139_GIO1_IRQServer(void);

#endif
