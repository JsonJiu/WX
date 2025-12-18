#ifndef __UART4_H__
#define __UART4_H__
#include "main.h"
void UART4_MF_Config_Init(void);
void Uart4_Rx_rec(void);
void UART4_IRQHandler(void);
void Uart4_Tx_Send(uint8_t zufuchua[],uint16_t lenth);
void Uart4_Tx_Send_ok(uint8_t zufuchua[],uint16_t lenth);
void Uart4_Tx_Send_init(void);
extern uint16_t chaoshi_dengdai_4;
extern uint16_t uart4_Rec_shuju_time_count;
#endif
