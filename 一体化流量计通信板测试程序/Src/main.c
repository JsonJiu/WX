/*
 * @Description:
 * @Author:
 * @Date: 2025-08-06 10:54:47
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2025-08-12 09:29:55
 */
#include "main.h"
#include "GPIO.h"
#include "uart1.h"
#include "uart3.h"
#include "uart4.h"
#include "uart0.h"
#include "uart5.h"
#include "lpuart2.h"
#include "time.h"
#include "ADC_CHK.h"
#include "LED_CTRL.h"
#include "Test_List.h"
#include "WTD.h"
#include "DAC1.h"
#include "I2C.h"

uint8_t Debug_Mode = 0;
// uint8_t nihao[] = "你好\r\n";

void test_Init()
{
	// 电压优先控制
	MF_DAC1_Init();
	// IIC初始化
	MF_IIC_Config_Init();
	// 先设置为3.6V电压
	DAC_SWTrigger(3800);
	Others_GPIO_Init();
	UART3_MF_Config_Init();
	UART4_MF_Config_Init();
	UART1_MF_Config_Init();
	UART0_MF_Config_Init();
	UART5_MF_Config_Init();
	LPUART2_MF_Config_Init();
	ATIM_Init();
	MF_ADC_PC10_Config_Init();
	// 工位检测
	gongwei_jiance();
	// 主控板配置初始化
	test_start_Init();
	// 看门狗
	WatchDog_Init();
}

int main(void)
{
	/* Initialize FL Driver Library */
	/* SHOULD BE KEPT!!! */
	FL_Init();

	/* Reset of all peripherals, Initializes the Flash interface and the Systick. */
	/* SHOULD BE KEPT!!! */
	MF_Clock_Init();

	/* Initialize all configured peripherals */
	/* SHOULD BE KEPT!!! */
	test_Init();
	// SPI_RW_Demo();

	while (1)
	{
		//			PC_Chuankou_tongxin_Debug_send(nihao,sizeof(nihao)-1);
		Uart3_Rx_rec();
		Uart4_Rx_rec();
		Uart1_Rx_rec();
		Uart0_Rx_rec();
		Uart5_Rx_rec();
		LPUART2_Rx_rec();
		send_weituo();
		LED_FLAG_LOOP();
		test_Loop_Func();
		Test_loop_func();

		FL_IWDT_ReloadCounter(IWDT);
	}
}
