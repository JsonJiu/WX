#include "main.h"
#include "GPIO.h"
#include "uart5.h"
#include "uart1.h"
#include "uart0.h"
#include "time.h"
#include "ADC_CHK.h"
#include "LED_CTRL.h"
#include "Test_List.h"
#include "PC_xieyi_Ctrl.h"
#include "WTD.h"

uint8_t Debug_Mode = 0;
void test_Init()
{
	Others_GPIO_Init();
	//UART5_MF_Config_Init();
	UART1_MF_Config_Init();
	//UART0_MF_Config_Init();
	ATIM_Init();
	MF_ADC_PC10_Config_Init();
	//主控板配置初始化
	test_start_Init();
	//看门狗
	WatchDog_Init();
	//工位及回复信息初始化
	xuceshi_gongwei_qingchu();
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
    //SPI_RW_Demo();

    while(1)
    {
			//Uart5_Rx_rec();
			Uart1_Rx_rec();
			//Uart0_Rx_rec();
			LED_FLAG_LOOP();
//			//不走主流程
//			test_Loop_Func();
			FL_IWDT_ReloadCounter(IWDT);
    }
}


