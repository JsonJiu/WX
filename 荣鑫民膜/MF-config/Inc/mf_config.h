/**
******************************************************************************
* @file           : mf_config.c
* @brief          : MCU FUNCTION CONFIG
******************************************************************************
* @attention
*
* Copyright (c) [2021] [Fudan Microelectronics]
* THIS SOFTWARE is licensed under Mulan PSL v2.
* can use this software according to the terms and conditions of the Mulan PSL
*v2. You may obtain a copy of Mulan PSL v2 at:
*          http://license.coscl.org.cn/MulanPSL2
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
* EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
* MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
* See the Mulan PSL v2 for more details.
*
******************************************************************************
*/

/* USER CODE BEGIN Header */

/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MF_CONFIG_H
#define __MF_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif
/* Includes ------------------------------------------------------------------*/
#if defined(FM33LG0XX)
#include "fm33lg0xx_fl.h"
#elif defined(FM33LC0XX)
#include "fm33lc0xx_fl.h"
#endif

/* Exported functions prototypes ---------------------------------------------*/
void MF_Clock_Init(void);
void MF_Config_Init(void);
void Error_Handler(void);

void assert_failed(uint8_t *file, uint32_t linetmp);

/* Private defines -----------------------------------------------------------*/

/* =============================================================================
 * 系统时钟配置 - 一键切换
 * =============================================================================
 * 修改下面的数字即可切换时钟频率：
 * 8  = 8MHz  (红外通信优化模式)
 * 16 = 16MHz (平衡模式)
 * 24 = 24MHz (高性能模式)
 * 32 = 32MHz (默认高性能模式)
 */
#define SYSTEM_CLOCK_MHZ 8 // ← 在这里修改数字：8, 16, 24, 32

/* 内部宏定义处理 - 请勿修改 */
#if SYSTEM_CLOCK_MHZ == 8
#define USE_CLOCK_8MHZ
#elif SYSTEM_CLOCK_MHZ == 16
#define USE_CLOCK_16MHZ
#elif SYSTEM_CLOCK_MHZ == 24
#define USE_CLOCK_24MHZ
#elif SYSTEM_CLOCK_MHZ == 32
#define USE_CLOCK_32MHZ
#else
#error "不支持的时钟频率！请设置为 8, 16, 24, 或 32"
#endif

#ifndef NVIC_PRIORITYGROUP_0

#define NVIC_PRIORITYGROUP_0                                                   \
  ((uint32_t)0x00000007) /*!< 0 bit  for pre-emption priority,                 \
                              4 bits for subpriority */
#define NVIC_PRIORITYGROUP_1                                                   \
  ((uint32_t)0x00000006) /*!< 1 bit  for pre-emption priority,                 \
                              3 bits for subpriority */
#define NVIC_PRIORITYGROUP_2                                                   \
  ((uint32_t)0x00000005) /*!< 2 bits for pre-emption priority,                 \
                              2 bits for subpriority */
#define NVIC_PRIORITYGROUP_3                                                   \
  ((uint32_t)0x00000004) /*!< 3 bits for pre-emption priority,                 \
                              1 bit  for subpriority */
#define NVIC_PRIORITYGROUP_4                                                   \
  ((uint32_t)0x00000003) /*!< 4 bits for pre-emption priority,                 \
                              0 bit  for subpriority */
#endif

#ifdef __cplusplus
}
#endif

#endif /* __MF_CONFIG_H */

/*************************(C) COPYRIGHT Fudan Microelectronics **** END OF
 * FILE*************************/
