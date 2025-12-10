#include "main.h"
#include "GPIO.h"
#include "uart5.h"
#include "uart1.h"
#include "uart0.h"
#include "time.h"
#include "LED_CTRL.h"
#include "Test_List.h"
#include "ADC_CHK.h"
#include "WTD.h"
#include "ZDINA219.h"
#include "tongxin_xieyi_Ctrl.h"
uint8_t Debug_Mode = 0;
uint16_t  time;
uint8_t  i=0;
void test_1008()
{
    if(time == 0 && i < 10)  // 限制最大次数
    {
        if(i < 3)
        {
            TONGXIN_xieyifasong_1008(0,0,0,0);
        }
//        else if(i >= 7)
//        {
//            TONGXIN_xieyifasong_1008(0,0,0,1);
//        }
				FL_DelayMs(2000);
        i++;
    }
}
void test_Init()
{
	Others_GPIO_Init();
	UART5_MF_Config_Init();
	UART1_MF_Config_Init();
	UART0_MF_Config_Init();
	MF_ADC_PC10_Config_Init();
	ATIM_Init();
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
		//time=10000;
    
    while(1)
    {
			Uart5_Rx_rec();
			Uart1_Rx_rec();
			FL_DelayMs(100);
			Uart0_Rx_rec();
			LED_FLAG_LOOP();
			test_Loop_Func();
			FL_IWDT_ReloadCounter(IWDT);
    }
}


