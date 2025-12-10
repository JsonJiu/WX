#ifndef __UART5_H__
#define __UART5_H__
#include "main.h"
struct UARTOpStruct
{
    uint8_t *TxBuf; //发送数据指针
    uint8_t TxLen;  //待发送数据长度
    uint8_t TxOpc;  //已发送数据长度
    uint8_t *RxBuf; //接收数据指针
    uint8_t RxLen;  //待接收数据长度
    uint8_t RxOpc;  //已接收数据长度
};
extern uint16_t chaoshi_dengdai;
void UART5_MF_Config_Init(void);
void Uart5_Rx_rec(void);
void UART5_IRQHandler(void);
void Uart5_Tx_Send(uint8_t zufuchua[],uint16_t lenth);
void Uart5_Tx_Send_init(void);
#endif
