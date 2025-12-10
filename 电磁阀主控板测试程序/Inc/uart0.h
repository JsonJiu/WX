#ifndef __UART0_H__
#define __UART0_H__
#include "main.h"
void UART0_MF_Config_Init(void);
void Uart0_Rx_rec(void);
void UART0_IRQHandler(void);
void Uart0_Tx_Send(uint8_t zufuchua[],uint16_t lenth);
void Uart0_Tx_Send_init(void);
void UART0_TX_state_change(uint8_t send_state);
#endif
