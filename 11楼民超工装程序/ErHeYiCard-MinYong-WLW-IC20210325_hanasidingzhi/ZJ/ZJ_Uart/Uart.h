#ifndef _UART_H_
#define _UART_H_
#include "r_cg_macrodriver.h"
#include "r_cg_userdefine.h"
#if 1
void Uart0_SendByte(unsigned char TxData);
void Uart0_SendBytes(unsigned char *buff,unsigned int tmp);
void Uart0_SendBytecc(char *buff );


void Uart1_SendByte(unsigned char TxData);
void Uart1_SendBytes(unsigned char *buff,unsigned int tmp);
void Uart1_SendBytecc(char *buff );

void Uart2_SendByte(unsigned char TxData);
void Uart2_SendBytes(unsigned char *buff,unsigned int tmp);
void Uart2_SendBytecc(char *buff );
void RF_SendBytes(unsigned char *rfbuff,unsigned int rftmp);
#endif

void Uart3_SendByte(unsigned char TxData);
void Uart3_SendBytes(unsigned char *buff,unsigned int tmp);
void Uart3_SendBytecc(char *buff );

void Uart_Init();
void Uart3_Config(unsigned char Uart1Baund);
#endif