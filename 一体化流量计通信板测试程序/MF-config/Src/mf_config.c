/**
******************************************************************************
* @file           : mf_config.c
* @brief          : MCU FUNCTION CONFIG
******************************************************************************
* @attention
*
* Copyright (c) [2021] [Fudan Microelectronics]
* THIS SOFTWARE is licensed under Mulan PSL v2.
* can use this software according to the terms and conditions of the Mulan PSL v2.
* You may obtain a copy of Mulan PSL v2 at:
*          http://license.coscl.org.cn/MulanPSL2
* THIS SOFTWARE IS PROVIDED ON AN "AS IS" BASIS, WITHOUT WARRANTIES OF ANY KIND,
* EITHER EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO NON-INFRINGEMENT,
* MERCHANTABILITY OR FIT FOR A PARTICULAR PURPOSE.
* See the Mulan PSL v2 for more details.
*
******************************************************************************
*/

/* Includes ------------------------------------------------------------------*/
#include "mf_config.h"

/* Private function prototypes -----------------------------------------------*/



/**
  * @brief  Clock Initialization.
  * @retval None
  */
void MF_Clock_Init(void)
{
		/* Initial XTLF */
    FL_CDIF_EnableCPUToVAO(CDIF);                                                         /*使能跨电源域接口*/
    FL_VAO_XTLF_Disable(VAO);                                                             /*禁止XTLF*/
    FL_CDIF_DisableCPUToVAO(CDIF);                                                        /*禁止跨电源域接口*/
	
		/* Initial Low Speed Clock */
    FL_CMU_SetLSCLKClockSource(FL_CMU_LSCLK_CLK_SOURCE_RCLP);                             /*配置LSCLK时钟源*/
	
    /* Initial RCHF */
    FL_CMU_RCHF_SetFrequency(FL_CMU_RCHF_FREQUENCY_32MHZ);
    FL_CMU_RCHF_Enable();

    /* Initial System Clock */
    FL_FLASH_SetReadWait(FLASH, FL_FLASH_READ_WAIT_0CYCLE);    /* 设置FLASH读等待为 0 个周期 */
    FL_CMU_SetSystemClockSource(FL_CMU_SYSTEM_CLK_SOURCE_RCHF);    /* 设置系统主时钟为 RCHF */
    FL_CMU_SetAHBPrescaler(FL_CMU_AHBCLK_PSC_DIV1);
    FL_CMU_SetAPBPrescaler(FL_CMU_APBCLK_PSC_DIV1);

    SystemCoreClockUpdate();
}




/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{

}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{

}
#endif /* USE_FULL_ASSERT */

/*************************(C) COPYRIGHT Fudan Microelectronics **** END OF FILE*************************/
