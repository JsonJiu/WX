/**
 * @file    uart_common.h
 * @brief   UART 通用配置头文件
 * @note    集中定义所有UART的中断优先级、缓冲区大小等配置
 */
#ifndef UART_COMMON_H
#define UART_COMMON_H

/*===========================================================================*/
/*                         UART 中断优先级配置                                */
/*===========================================================================*/
/**
 * 中断优先级说明（数值越小优先级越高）：
 * - UART1: 0x00 - 设备通信（水表/膜表），最高优先级
 * - UART0: 0x02 - PC通信，较低优先级
 * - UART3: 0x02 - 计量协议通信
 * - UART4: 0x02 - 远红外检测通信
 */
#define UART0_IRQ_PRIORITY 0x02
#define UART1_IRQ_PRIORITY 0x00
#define UART3_IRQ_PRIORITY 0x02
#define UART4_IRQ_PRIORITY 0x02

/*===========================================================================*/
/*                         UART 缓冲区大小配置                                */
/*===========================================================================*/
/**
 * 缓冲区大小说明：
 * - UART0: 700字节 - PC通信，协议帧较大
 * - UART1: 1024字节 - 膜表通信，数据量最大
 * - UART3: 200字节 - 计量协议
 * - UART4: 200字节 - 红外检测
 */
#define UART0_BUFFER_SIZE 700
#define UART1_BUFFER_SIZE 1024
#define UART3_BUFFER_SIZE 200
#define UART4_BUFFER_SIZE 200

/*===========================================================================*/
/*                         UART 超时配置（毫秒）                              */
/*===========================================================================*/
#define UART0_TX_TIMEOUT_MS 500
#define UART0_RX_FRAME_TIMEOUT_MS 100

#define UART1_TX_TIMEOUT_MS 1000
#define UART1_RX_FRAME_TIMEOUT_MS 100

#define UART3_TX_TIMEOUT_MS 100
#define UART3_RX_FRAME_TIMEOUT_MS 50

#define UART4_TX_TIMEOUT_MS 100
#define UART4_RX_FRAME_TIMEOUT_MS 50

/*===========================================================================*/
/*                         UART 引脚配置                                      */
/*===========================================================================*/
/**
 * UART0: PA13(RX), PA14(TX) - PC通信，TX需要三态切换
 * UART1: PB13(RX), PB14(TX) - 设备通信
 * UART3: PC10(RX), PC11(TX) - 计量协议
 * UART4: PD0(RX), PD1(TX)   - 远红外检测
 */

#endif /* UART_COMMON_H */
