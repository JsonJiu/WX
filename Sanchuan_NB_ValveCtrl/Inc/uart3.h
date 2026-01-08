#ifndef __UART3_H__
#define __UART3_H__
#include "main.h"
struct UARTOpStruct {
  uint8_t *TxBuf; //发送缓存指针
  uint16_t TxLen; //发送数据长度 - 扩展到16位支持大数据包
  uint16_t TxOpc; //已发送数据长度 - 同步扩展
  uint8_t *RxBuf; //接收缓存指针
  uint8_t RxLen;  //接收数据长度
  uint8_t RxOpc;  //已接收数据长度
};
void UART3_MF_Config_Init(void);
void Uart3_Rx_rec(void);
void UART3_IRQHandler(void);
void Uart3_Tx_Send(uint8_t zufuchua[], uint16_t lenth);
void Uart3_Tx_Send_init(void);
extern uint16_t chaoshi_dengdai_3;
extern uint16_t uart3_Rec_shuju_time_count;
#endif
