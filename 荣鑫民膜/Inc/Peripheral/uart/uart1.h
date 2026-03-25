#ifndef __UART1_H__
#define __UART1_H__
#include "main.h"
#include "uart_common.h"

/*===========================================================================*/
/*                          外部变量声明                                      */
/*===========================================================================*/
/* 接收相关 */
extern volatile uint8_t uart1_rx_data_flag;
extern uint8_t uart1_rx_buffer[];
extern volatile uint16_t uart1_rx_count;

/* 超时计数器 */
extern volatile uint16_t uart1_tx_timeout_counter;
extern volatile uint16_t uart1_rx_frame_timeout;

/* 前导码使能 */
extern uint8_t uart1_preamble_enabled;

/*===========================================================================*/
/*                          函数声明                                          */
/*===========================================================================*/
void UART1_MF_Config_Init(void);
void Uart1_Rx_rec(void);
void UART1_IRQHandler(void);
void Uart1_Tx_Send(uint8_t data[], uint16_t len);
void Uart1_Tx_Send_init(void);
void Uart1_Tx_Send_ok(uint8_t data[], uint16_t len);

#endif /* __UART1_H__ */
