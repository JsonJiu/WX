#ifndef __UART0_H__
#define __UART0_H__
#include "main.h"
#include "uart_common.h"

/* ============================================================================
 * 公共函数声明
 * ========================================================================== */
void UART0_MF_Config_Init(void);
void Uart0_Rx_rec(void);
void UART0_IRQHandler(void);
void Uart0_Tx_Send(uint8_t data[], uint16_t len);
void Uart0_Tx_Send_init(void);
void PC_Debug_Send(uint8_t data[], uint16_t len);
void PC_Comm_Send(uint8_t data[], uint16_t len);
void protocol_debug_print(uint8_t protocol[], uint16_t length);

/* 测试过程中UART0中断控制 */
void UART0_DisableRxInterrupt(void); /**< 禁用接收中断（测试开始时调用）*/
void UART0_EnableRxInterrupt(void); /**< 启用接收中断（测试结束时调用）*/

/* ============================================================================
 * 公共变量声明（供定时器递减、主循环使用）
 * ========================================================================== */
extern volatile uint8_t uart0_rx_data_flag; /**< 接收数据标志 */
extern volatile uint16_t uart0_rx_count;    /**< 接收数据长度 */
extern volatile uint16_t
    uart0_tx_timeout_counter; /**< 发送超时计数器（ms, 递减）*/
extern volatile uint16_t uart0_rx_frame_timeout; /**< 帧超时计数器（ms, 递减）*/
extern volatile uint16_t
    uart0_bus_idle_counter; /**< 总线空闲计数器（ms, 递减）*/
extern uint8_t uart0_rx_buffer[UART0_BUFFER_SIZE]; /**< 接收缓冲区 */

#endif
