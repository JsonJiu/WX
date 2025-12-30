#include "main.h"
#include "GPIO.h"
#include "uart1.h"
#include "uart3.h"
#include "uart4.h"
#include "uart0.h"
#include "time.h"
#include "ADC_CHK.h"
#include "LED_CTRL.h"
#include "Test_List.h"
#include "WTD.h"
uint8_t Debug_Mode = 0;
//uint8_t nihao[] = "你好\r\n";

void test_Init()
{
	Others_GPIO_Init();
	UART3_MF_Config_Init();
	UART4_MF_Config_Init();
	UART1_MF_Config_Init();
	UART0_MF_Config_Init();
	ATIM_Init();
	MF_ADC_PC10_Config_Init();
	//工位检测
	gongwei_jiance();
	//主控板配置初始化
	test_start_Init();
	//看门狗
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
    //SPI_RW_Demo();

    while(1)
    {
//			PC_Chuankou_tongxin_Debug_send(nihao,sizeof(nihao)-1);
			Uart3_Rx_rec();
			Uart4_Rx_rec();
			Uart1_Rx_rec();
			Uart0_Rx_rec();
			LED_FLAG_LOOP();
			test_Loop_Func();
			Test_loop_func();
			FL_IWDT_ReloadCounter(IWDT);
    }
}


