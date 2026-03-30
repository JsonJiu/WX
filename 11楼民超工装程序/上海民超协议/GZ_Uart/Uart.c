#pragma interrupt INTSR0 r_uart0_interrupt_receive
#pragma interrupt INTSR1 r_uart1_interrupt_receive
#pragma interrupt INTSR2 r_uart2_interrupt_receive
#pragma interrupt INTSR3 r_uart3_interrupt_receive
#include "Uart.h"
#include "Struct.h"
#include "r_cg_sau.h"
#include "Mydefine.h"
#include "Timer.h"
#include "User.h"
void Uart_Init()
{
        R_SAU0_Create();
	R_SAU1_Create();
        R_UART3_Start();
	R_UART2_Start();
        R_UART1_Start();
	R_UART0_Start();
}
/*************************************************
          Uart0_TXD
**************************************************/
#if 1
void Uart0_SendByte(unsigned char TxData)
{
	unsigned short CSTimer=65535;
	if ((SRIF0) && (SSR00 != 0))
	{ 
		SRIF0 = 0;      					
		SIR00 |= (SSR00&0x000F);
	}
	TXD0 = TxData;     					
	while ((STIF0 == 0)&&(CSTimer--))
	{
	}
	STIF0 = 0;
}
void Uart0_SendBytes(unsigned char *buff,unsigned int tmp)
{
  unsigned char i;
  for( i=0; i<tmp; i++ )
  {
    Uart0_SendByte( buff[i] );
  }
}
void Uart0_SendBytecc(char *buff )
{
  unsigned char i;
  for( i=0; ; i++ )
  {
    if(buff[i]=='\0')
      break;
    Uart0_SendByte( buff[i] );
  }
}
#endif
/*************************************************
          Uart1_TXD
**************************************************/
#if 1
void Uart1_SendByte(unsigned char TxData)
{
	unsigned short CSTimer=65535;
	if ((SRIF1) && (SSR02 != 0))
	{ 
		SRIF1 = 0;
		SIR02 |= (SSR02&0x000F);
	}
	TXD1 = TxData;
	while ((STIF1 == 0)&&(CSTimer--))
	{
	}
	STIF1 = 0;
}
void Uart1_SendBytes(unsigned char *buff,unsigned int tmp)
{
  unsigned char i;
  for( i=0; i<tmp; i++ )
  {
    Uart1_SendByte( buff[i] );
  }
}
void Uart1_SendBytecc(char *buff )
{
  unsigned char i;
  for( i=0; ; i++ )
  {
    if(buff[i]=='\0')
      break;
    Uart1_SendByte( buff[i] );
  }
}
#endif
/*************************************************
          Uart2_TXD
**************************************************/
#if 1
void Uart2_SendByte(unsigned char TxData)
{
	unsigned long CSTimer=9999999;
	if ((SRIF2) && (SSR10 != 0))
	{ 
		SRIF2 = 0;
		SIR10 |= (SSR10&0x000F);
	}
	TXD2 = TxData;
	while ((STIF2 == 0)&&(CSTimer--))
	{
	}
	STIF2 = 0;
}
void Uart2_SendBytes(unsigned char *buff,unsigned int tmp)
{
  unsigned char i;
  for( i=0; i<tmp; i++ )
  {
    Uart2_SendByte( buff[i] );
  }
}
void Uart2_SendBytecc(char *buff )
{
  unsigned char i;
  for( i=0; ; i++ )
  {
    if(buff[i]=='\0')
      break;
    Uart2_SendByte( buff[i] );
  }
}
void RF_SendBytes(unsigned char *rfbuff,unsigned int rftmp)
{
	Uart2_SendBytes(rfbuff,rftmp);
}
#endif
/*************************************************
          Uart3_TXD
**************************************************/
void Uart3_SendByte(unsigned char TxData)
{
	unsigned long CSTimer=999999999;
	if ((SRIF3) && (SSR12 != 0))
	{ 
		SRIF3 = 0;
		SIR12 |= (SSR12&0x000F);
	}
	TXD3 = TxData;
	while ((STIF3 == 0)&&(CSTimer--))
	{
	}
	STIF3 = 0;
}
void Uart3_SendBytes(unsigned char *buff,unsigned int tmp)
{
  unsigned short i;
  for( i=0; i<tmp; i++ )
  {
    Uart3_SendByte( buff[i] );
  }
}
void Uart3_SendBytecc(char *buff )
{
  unsigned char i;
  for( i=0; ; i++ )
  {
    if(buff[i]=='\0')
      break;
    Uart3_SendByte( buff[i] );
  }
}

/*******************************************
          Uart0_RX
********************************************/
__interrupt static void r_uart0_interrupt_receive(void)
{
  Csb_Timer.CARDTimer=0;
  TempUserData.CARDBuff[TempUserData.CARDBuffLen] = RXD0;
  if(TempUserData.CARDBuff[0] == 0x68)
  {
    TempUserData.CARDBuffLen++;
  }
  if(TempUserData.CARDBuffLen >= 47)
  {
    TempUserData.CARDBuffLen = 0;
  }
}
/*******************************************
          Uart1_RX
********************************************/
__interrupt static void r_uart1_interrupt_receive(void)
{
  Csb_Timer.UMUTimer=0;
  TempUserData.UMUBuff[TempUserData.UMUBuffLen] = RXD1;
  if((TempUserData.UMUBuff[0] == 0x06)||(TempUserData.UMUBuff[0] == 'G')||(TempUserData.UMUBuff[0] == 'g')||(TempUserData.UMUBuff[0] == 'T')||(TempUserData.UMUBuff[0] == 't'))
  {
    TempUserData.UMUBuffLen++;
  }
  if(TempUserData.UMUBuffLen >= 47)
  {
    TempUserData.UMUBuffLen = 0;
  }
}
/*******************************************
          Uart2_RX
********************************************/
__interrupt static void r_uart2_interrupt_receive(void)
{
  Csb_Timer.ZJTimer=0;
  TempUserData.ZJBuff[TempUserData.ZJBuffLen] = RXD2;
  if(TempUserData.ZJBuff[0] == 0x68)
  {
    TempUserData.ZJBuffLen++;
  }
  if(TempUserData.ZJBuffLen >= 47)
  {
    TempUserData.ZJBuffLen = 0;
  }
}
/*******************************************
          Uart3_RX
********************************************/
__interrupt static void r_uart3_interrupt_receive(void)
{
  Csb_Timer.IRDATimer=0;
  TempUserData.IRDABuff[TempUserData.IRDABuffLen] = RXD3;
  if(TempUserData.IRDABuff[0] == 0x68)
  {
    TempUserData.IRDABuffLen++;
  }
  else
  {
    TempUserData.IRDABuffLen = 0;  
  }
  if(TempUserData.IRDABuffLen >= 400)
  {
    TempUserData.IRDABuffLen = 0;
  }
}