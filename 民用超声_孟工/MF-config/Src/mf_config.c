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
  * @brief  CMU_XTLF Initialization function
  * @param  void
  * @retval None
  */
void MF_CMU_XTLF_Init(void)
{
    FL_VAO_XTLF_InitTypeDef    XTLF_InitStruct;

    XTLF_InitStruct.driveMode = FL_VAO_XTLF_DRIVE_LEVEL_1;
    XTLF_InitStruct.workingCurrentMode = FL_VAO_XTLF_WORK_CURRENT_450NA;

    FL_VAO_XTLF_Init(VAO, &XTLF_InitStruct);    
}
 
 
/**
  * @brief  SWD_IO Initialization function
  * @param  void
  * @retval None
  */
void MF_SWD_IO_Init(void)
{
    FL_GPIO_InitTypeDef    GPIO_InitStruct;

    /* PD7 SWD_SWCLK */ 
    GPIO_InitStruct.pin = FL_GPIO_PIN_7;
    GPIO_InitStruct.mode = FL_GPIO_MODE_DIGITAL;
    GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.pull = FL_DISABLE;
    GPIO_InitStruct.remapPin = FL_DISABLE;
    GPIO_InitStruct.analogSwitch = FL_DISABLE;
    FL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    /* PD8 SWD_SWDIO */ 
    GPIO_InitStruct.pin = FL_GPIO_PIN_8;
    GPIO_InitStruct.mode = FL_GPIO_MODE_DIGITAL;
    GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.pull = FL_DISABLE;
    GPIO_InitStruct.remapPin = FL_DISABLE;
    GPIO_InitStruct.analogSwitch = FL_DISABLE;
    FL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    
}

/**
  * @brief  FDET_IRQHandler
  * @param  void
  * @retval None
  */
void FDET_IRQHandler(void)
{
    //停振检测中断处理
    if((FL_CMU_IsEnabledIT_XTHFFail() == 0x01UL)
        && (FL_CMU_IsActiveFlag_XTHFFail() == 0x01UL))
    {
        FL_CMU_ClearFlag_XTHFFail();
         /*应用处理逻辑*/
    }
}


/**
  * @brief  Clock Initialization.
  * @retval None
  */
void MF_Clock_Init(void)
{
    /* Initial CDIF */
    FL_CDIF_EnableCPUToVAO(CDIF);
    FL_CDIF_EnableVAOToCPU(CDIF);

    /* Initial RTCA Adjust Value */
    FL_CMU_EnableGroup1BusClock(FL_CMU_GROUP1_BUSCLK_RTCA);
    FL_RTCA_WriteAdjustValue(RTCA, 0);
    FL_CMU_DisableGroup1BusClock(FL_CMU_GROUP1_BUSCLK_RTCA);

    /* Initial RTCB Adjust Value */
    FL_RTCB_WriteAdjustValue(RTCB, 0);

    /* Initial XTLF */
    MF_CMU_XTLF_Init();
    FL_VAO_XTLF_Enable(VAO);
    FL_DelayMs(1000);

    /* Initial Low Speed Clock */
    FL_CMU_SetLSCLKClockSource(FL_CMU_LSCLK_CLK_SOURCE_RCLP);

    /* Initial RCHF */
    FL_CMU_RCHF_WriteTrimValue(RCHF32M_TRIM);
    FL_CMU_RCHF_SetFrequency(FL_CMU_RCHF_FREQUENCY_32MHZ);
    FL_CMU_RCHF_Enable();

    /* Initial System Clock */
    FL_FLASH_SetReadWait(FLASH, FL_FLASH_READ_WAIT_1CYCLE);    /* 设置FLASH读等待为 1 个周期 */
    FL_CMU_SetSystemClockSource(FL_CMU_SYSTEM_CLK_SOURCE_RCHF);    /* 设置系统主时钟为 RCHF */
    FL_CMU_SetAHBPrescaler(FL_CMU_AHBCLK_PSC_DIV1);
    FL_CMU_SetAPBPrescaler(FL_CMU_APBCLK_PSC_DIV1);

    /* Initial CDIF Clock Prescaler */
    FL_CDIF_SetPrescaler(CDIF, FL_CDIF_PSC_DIV4);

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
