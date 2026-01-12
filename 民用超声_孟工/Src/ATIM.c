#include "main.h"
#include "ATIM.h"
#include "pwm.h"
#include "uart0.h"
#include "uart1.h"
#include "uart3.h"
#include "uart4.h"
#include "gpio.h"

extern uint16_t led_time_count;
extern uint16_t	uart0_Rec_shuju_time_count;
extern uint16_t	uart1_Rec_shuju_time_count;
extern uint16_t	uart4_Rec_shuju_time_count;
extern uint32_t single_step_timing; //单步测试时间
extern uint32_t interval_timing; 	 //间隔测试时间
extern uint32_t umu_timing;		 // UMU定时发送时间


void MF_ATIM_TimerBase_Init(void)
{
    FL_ATIM_InitTypeDef    TimerBase_InitStruct;

    TimerBase_InitStruct.clockSource = FL_CMU_ATIM_CLK_SOURCE_APBCLK;
    TimerBase_InitStruct.prescaler = 31;
    TimerBase_InitStruct.counterMode = FL_ATIM_COUNTER_DIR_UP;
    TimerBase_InitStruct.autoReload = 999;
    TimerBase_InitStruct.clockDivision = FL_ATIM_CLK_DIVISION_DIV1;
    TimerBase_InitStruct.repetitionCounter = 0;
    TimerBase_InitStruct.autoReloadState = FL_DISABLE;

    FL_ATIM_Init(ATIM, &TimerBase_InitStruct);
}


void MF_ATIM_Interrupt_Init(void)
{
    FL_ATIM_ClearFlag_Update(ATIM);
    FL_ATIM_EnableIT_Update(ATIM);
}


void ATIM_NVIC_Init(void)
{
    FL_NVIC_ConfigTypeDef    InterruptConfigStruct;

    InterruptConfigStruct.preemptPriority = 0x02;
    FL_NVIC_Init(&InterruptConfigStruct, ATIM_IRQn);
}

void ATIM_Start()
{
    //使能LATIM
    FL_ATIM_Enable(ATIM);
}

void ATIM_Init(void)
{
    /* Initial ATIM */
    MF_ATIM_TimerBase_Init();
    MF_ATIM_Interrupt_Init();

    /* Initial NVIC */
    ATIM_NVIC_Init();
	  ATIM_Start();
}

void ATIM_IRQHandler()
{
    if(FL_ATIM_IsEnabledIT_Update(ATIM) && FL_ATIM_IsActiveFlag_Update(ATIM))
    {
			FL_ATIM_ClearFlag_Update(ATIM);
			if(led_time_count>0)
			{
				led_time_count--;
			}
			if(uart0_Rec_shuju_time_count>0)
			{
				uart0_Rec_shuju_time_count--;
			}
			if(uart1_Rec_shuju_time_count>0)
			{
				uart1_Rec_shuju_time_count--;
			}
			if(uart4_Rec_shuju_time_count>0)
			{
				uart4_Rec_shuju_time_count--;
			}
			if(single_step_timing>0)
			{
				single_step_timing--;
			}
			if(interval_timing>0)
			{
				interval_timing--;
			}
			if(umu_timing>0)
			{
				umu_timing--;
			}
    }
}




