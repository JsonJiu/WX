
#ifndef _DEBUG_H_
#define _DEBUG_H_

#include "config.h"

#define DEBUG_RF_ALL    0
#define DEBUG_RF_FRAME  1

#define DEBUG_RF_MONITOR        //RF¼à¿Ø

extern COMM_Buffer_t    RF_Buffer;   //RF

extern unsigned char debug_temp[1100], Test_Buffer[];
extern unsigned short debug_buf_in, debug_buf_out;

void VirtualCOMRecv(void);

void SendMonitorData(void);

#endif





