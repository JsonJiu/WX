#ifndef __LPUART2_H__
#define __LPUART2_H__
#include "main.h"
void LPUART2_MF_Config_Init(void);
void LPUART2_Rx_rec(void);
void LPUARTx_IRQHandler(void);
void LPUART2_Tx_Send(uint8_t zufuchua[],uint16_t lenth);
extern uint16_t chaoshi_dengdai_lpuart2;
extern uint16_t lpuart2_Rec_shuju_time_count;
#endif
