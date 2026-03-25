#ifndef __TIME_H__
#define __TIME_H__
#include "main.h"

extern volatile uint32_t system_tick_ms; // 系统毫秒计数器

void ATIM_Init(void);
#endif
