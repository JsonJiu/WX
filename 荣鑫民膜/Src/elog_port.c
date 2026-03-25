/*
 * This file is part of the EasyLogger Library.
 *
 * Copyright (c) 2015, Armink, <armink.ztl@gmail.com>
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * 'Software'), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
 * IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
 * CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
 * TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
 * SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 *
 * Function: Portable interface for FM33LG0xx (non-os).
 * Created on: 2025-12-22
 */

#include "main.h"
#include "time.h"
#include "uart0.h"
#include <elog.h>
#include <stdio.h>

/*===========================================================================*/
/*                           运行环境配置 */
/*===========================================================================*/
/**
 * 运行环境选择 (只能选择一个):
 *
 * ELOG_ENV_BARE_METAL     - 裸机单线程环境 (使用简单标志位锁)
 * ELOG_ENV_BARE_METAL_IRQ - 裸机 + 中断中打印 (需要轮询发送，禁用中断)
 * ELOG_ENV_RTOS           - RTOS 多线程环境 (使用互斥锁)
 */
#define ELOG_ENV_BARE_METAL
// #define ELOG_ENV_BARE_METAL_IRQ
// #define ELOG_ENV_RTOS

/* 环境配置检查 */
#if defined(ELOG_ENV_BARE_METAL) + defined(ELOG_ENV_BARE_METAL_IRQ) +          \
        defined(ELOG_ENV_RTOS) !=                                              \
    1
#error                                                                         \
    "Please select exactly ONE environment: ELOG_ENV_BARE_METAL, ELOG_ENV_BARE_METAL_IRQ, or ELOG_ENV_RTOS"
#endif

/*===========================================================================*/
/*                           RTOS 适配层 (如使用RTOS请修改此处) */
/*===========================================================================*/
#ifdef ELOG_ENV_RTOS
/* FreeRTOS 示例 */
// #include "FreeRTOS.h"
// #include "semphr.h"
// static SemaphoreHandle_t elog_mutex = NULL;

/* RT-Thread 示例 */
// #include <rtthread.h>
// static rt_mutex_t elog_mutex = RT_NULL;
#endif

/**
 * EasyLogger port initialize
 *
 * @return result
 */
ElogErrCode elog_port_init(void) {
  ElogErrCode result = ELOG_NO_ERR;
  /* UART0 已在 main.c 中初始化 */
  return result;
}

/**
 * EasyLogger port deinitialize
 */
void elog_port_deinit(void) { /* nothing to do */ }

/**
 * output log port interface
 *
 * @param log output of log
 * @param size log size
 *
 * 注意: Debug_Mode = 0 时彻底关闭所有日志输出
 */
void elog_port_output(const char *log, size_t size) {
  /* Debug_Mode = 0 时不输出任何日志 */
  if (Debug_Mode == 0) {
    return;
  }
  /* 输出到串口 */
  Uart0_Tx_Send((uint8_t *)log, (uint16_t)size);
}

/**
 * output lock
 * 注意：不能使用 __disable_irq() 因为 Uart0_Tx_Send 依赖中断发送
 * 对于裸机单线程环境，可以使用简单的标志位锁或者直接空实现
 */
#ifdef ELOG_ENV_BARE_METAL
/* 裸机单线程：简单标志位锁 */
static volatile uint8_t elog_output_locked = 0;

void elog_port_output_lock(void) {
  while (elog_output_locked) {
    /* 等待解锁（单线程裸机环境下通常不会发生） */
  }
  elog_output_locked = 1;
}

void elog_port_output_unlock(void) { elog_output_locked = 0; }

#elif defined(ELOG_ENV_BARE_METAL_IRQ)
/* 裸机 + 中断中打印：禁用全局中断 */
/* 警告：此模式需要将 Uart0_Tx_Send 改为轮询发送，否则会死锁！ */
void elog_port_output_lock(void) { __disable_irq(); }

void elog_port_output_unlock(void) { __enable_irq(); }

#elif defined(ELOG_ENV_RTOS)
/* RTOS 多线程：互斥锁 */
void elog_port_output_lock(void) {
  /* FreeRTOS: xSemaphoreTake(elog_mutex, portMAX_DELAY); */
  /* RT-Thread: rt_mutex_take(elog_mutex, RT_WAITING_FOREVER); */
}

void elog_port_output_unlock(void) {
  /* FreeRTOS: xSemaphoreGive(elog_mutex); */
  /* RT-Thread: rt_mutex_release(elog_mutex); */
}
#endif

/* 时间字符串缓冲区 */
static char time_buf[16];

/**
 * get current time interface
 *
 * @return current time
 */
const char *elog_port_get_time(void) {
  extern volatile uint32_t system_tick_ms;
  uint32_t tick = system_tick_ms;
  uint32_t sec = tick / 1000;
  uint32_t ms = tick % 1000;
  uint32_t min = sec / 60;
  sec = sec % 60;
  snprintf(time_buf, sizeof(time_buf), "%02lu:%02lu.%03lu",
           (unsigned long)(min % 100), (unsigned long)sec, (unsigned long)ms);
  return time_buf;
}

/**
 * get current process name interface
 *
 * @return current process name
 */
const char *elog_port_get_p_info(void) { return ""; }

/**
 * get current thread name interface
 *
 * @return current thread name
 */
const char *elog_port_get_t_info(void) { return ""; }
