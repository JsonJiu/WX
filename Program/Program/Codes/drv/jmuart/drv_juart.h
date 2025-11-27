#ifndef DRV_JUART_H
#define DRV_JUART_H


#include "drv_juart_bsp.h"
/******************************* Setting && include *********************************/

//串口设置
#define UARTTYPE_MUART          1 //模拟串口
#define UARTTYPE_HUART          2 //硬件串口


#define UART_TYPE     UARTTYPE_HUART

#if UART_TYPE == UARTTYPE_MUART
#include "drv_muart.h"
#endif
#if UART_TYPE == UARTTYPE_HUART
#endif

#if UART_TYPE != UARTTYPE_MUART && UART_TYPE != UARTTYPE_HUART
#error "请指定直读头使用的串口类型"
#endif

//直读头数据长度
#define Size  6 + 1


/******************************* Setting && include *********************************/




typedef struct
{
    unsigned char Buff[Size];
    unsigned char BuffStart;
    unsigned char BuffEnd;
}St_Buff;

typedef enum
{
    ZDSTATE_POWDOWN = 0,
    ZDSTATE_READING,
    ZDSTATE_READED,
    ZDSTATE_ERR,
}ZDSTATE;

typedef struct
{
    ZDSTATE state;
    unsigned long amount;
    unsigned short TimeOut;
    unsigned char TryTime;
    St_Buff Rx;
}St_ZDRRUN;


extern St_ZDRRUN ZDRun;

void ZD_POW_Ctrl(EnDis c);

void ZDStart(void);
void ZDUart_RX_Ds(void);
void ZDUart_RX_En(void);
ZDSTATE ZDState(void);
#endif

