#ifndef __UART0_H__
#define __UART0_H__
#include "main.h"
void UART0_MF_Config_Init(void);
void Uart0_Rx_rec(void);
void UART0_IRQHandler(void);
void Uart0_Tx_Send(uint8_t zufuchua[],uint16_t lenth);
void Uart0_Tx_Send_init(void);
void DeBug_print(const char fmt[], ...);
void PC_Chuankou_tongxin_Debug_send(uint8_t zufuchua[],uint16_t lenth);
extern uint16_t chaoshi_dengdai_0;
extern uint16_t uart0_Rec_shuju_time_count;
extern uint8_t touchuan_flag;
#endif
