/**
  ****************************************************************************************************
  * @file    main.c
  * @author  FMSH Application Team
  * @brief   Header file of FL Module
  ****************************************************************************************************
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
  ****************************************************************************************************
  */

#include "main.h"
#include "fm33_assert.h"
#include "ATIM.h"
#include "uart0.h"
#include "uart1.h"
#include "uart3.h"
#include "uart4.h"
#include "ZDINA219.h"
#include "led.h"
#include "test.h"
#include "adc.h"
#include "umu.h"

/*————————串口初始化————————*/
void UART_Init(void)
{
	UART0_MF_Config_Init();
	UART1_MF_Config_Init();
	UART3_MF_Config_Init();
	UART4_MF_Config_Init();
}
  
int main(void)
{
    FL_Init();
    MF_Clock_Init();
		MF_SWD_IO_Init();
    MF_GPIO_Init();
		ATIM_Init();
		UART_Init();
		INA219_IIC_GPIO_Init();
		MF_ADC_Config_Init();
		Test_Init();
		
    while(1)
    {     
			LED_Sign_Close();
			Uart0_Rx_rec();
			Uart1_Rx_rec();
			Uart3_Rx_rec();
			Uart4_Rx_rec();
			Test_Function();
			Moter_Test();
			UMU_Simulate();
    }
}


