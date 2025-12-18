#ifndef __UART1_H__
#define __UART1_H__
#include "main.h"
void UART1_MF_Config_Init(void);
void Uart1_Rx_rec(void);
void UART1_IRQHandler(void);
void Uart1_Tx_Send(uint8_t zufuchua[],uint16_t lenth);
void Uart1_Tx_Send_init(void);
extern uint16_t chaoshi_dengdai_1;
extern uint16_t uart1_Rec_shuju_time_count;
void Uart1_Tx_Send_ok(uint8_t zufuchua[],uint16_t lenth);
void PC_Chuankou_tongxin_send(uint8_t zufuchua[],uint16_t lenth);
#endif
