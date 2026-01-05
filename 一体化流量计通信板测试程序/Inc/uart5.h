#ifndef __UART5_H__
#define __UART5_H__
#include "main.h"
void UART5_MF_Config_Init(void);
void Uart5_Rx_rec(void);
void UART5_IRQHandler(void);
void Uart5_Tx_Send(uint8_t zufuchua[],uint16_t lenth);
void Uart5_Tx_Send_init(void);
extern uint16_t chaoshi_dengdai_5;
extern uint16_t uart5_Rec_shuju_time_count;
#endif
