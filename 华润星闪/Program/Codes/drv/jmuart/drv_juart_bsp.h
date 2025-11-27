#ifndef DRV_JUART_BSP_H
#define DRV_JUART_BSP_H

#include "include.h"
#include "define.h"




void ZD_POW_Init(IODIR dir, IOPP pp);
void ZD_POW_Ctrl(EnDis c);
void ZD_DATA_Init(IODIR dir, IOPP pp);
void ZD_DATA_Ctrl(IOVAL c);
void ZDUart_Init(pUartMode mode, void(*fun)(unsigned char));

#endif

