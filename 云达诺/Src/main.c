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
#include "tongxin_xieyi_Ctrl.h"
#include "exti.h"
uint8_t Debug_Mode = 0;
uint8_t nihao[2] ={0x68,0xAA} ;
uint8_t i=0;

void test_Init()
{
	Others_GPIO_Init();
	UART3_MF_Config_Init();
	UART4_MF_Config_Init();
	UART1_MF_Config_Init();
	UART0_MF_Config_Init();
	ATIM_Init();
	MF_ADC_Config_Init();
	//工位检测
	gongwei_jiance();
	//主控板配置初始化
	test_start_Init();
	//看门狗
	//WatchDog_Init();
}

uint8_t test[2] = {0x68,0x88};
void test1()
{
			guanfa_xieyi();
			FL_DelayMs(3000);	
			Test_jiejuo_jilu.FM1_dianya = get_famen_2_dianya();	
			if(Test_linshi_cunchushuju_L.L_daowei1 == 0 && Test_linshi_cunchushuju_L.L_daowei2 == 1)
					{
						Test_jiejuo_jilu.FM_daowei_1 = 1;
						guan_daowei();
					}
					FL_DelayMs(2000);	
					kaifa_xieyi();
					FL_DelayMs(3000);	
					Test_jiejuo_jilu.FM2_dianya = get_famen_1_dianya();	
}
int main(void)
{
    /* Initialize FL Driver Library */
    /* SHOULD BE KEPT!!! */
			FL_Init();

    /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
    /* SHOULD BE KEPT!!! */
    MF_Clock_Init();
		MF_Config_Init();
    /* Initialize all configured peripherals */
    /* SHOULD BE KEPT!!! */
    test_Init();
		//FL_DelayMs(5000);
  //find_biaohao_xieyi();
    while(1)
    {
			//Uart0_Tx_Send(test,2);
			//FL_DelayMs(2000);
			jiliang_maichong();
			Uart1_Rx_rec();
			Uart0_Rx_rec();
			LED_FLAG_LOOP();
			test_Loop_Func();


    }
}


