#include "time.h"
#include "LED_CTRL.h"
#include "Test_List.h"
#include "common_test_configure.h"
#include "main.h"
#include "time_manager.h" /* 统一时间管理模块 */
#include "uart0.h"
#include "uart1.h"
#include "uart3.h"
#include "uart4.h"

volatile uint32_t system_tick_ms = 0; // 系统毫秒计数器定义
uint32_t test_fixture_power_time = 0; // 工装供电时间累加，单位ms

extern uint16_t uart5_Rec_shuju_time_count;
extern volatile uint16_t uart1_rx_frame_timeout;
extern uint8_t LED_thing_time;
extern uint16_t debug_print_time;
extern volatile uint16_t uart0_rx_frame_timeout;
extern volatile uint16_t uart0_tx_timeout_counter;
extern volatile uint16_t uart0_bus_idle_counter;
// test_fixture_power_time 已在上方定义
void MF_ATIM_TimerBase_Init(void) {
  FL_ATIM_InitTypeDef TimerBase_InitStruct;

  TimerBase_InitStruct.clockSource = FL_CMU_ATIM_CLK_SOURCE_APBCLK;
  TimerBase_InitStruct.prescaler = 7;
  TimerBase_InitStruct.counterMode = FL_ATIM_COUNTER_DIR_UP;
  TimerBase_InitStruct.autoReload = 999;
  TimerBase_InitStruct.clockDivision = FL_ATIM_CLK_DIVISION_DIV1;
  TimerBase_InitStruct.repetitionCounter = 0;
  TimerBase_InitStruct.autoReloadState = FL_DISABLE;

  FL_ATIM_Init(ATIM, &TimerBase_InitStruct);
}

void MF_ATIM_Interrupt_Init(void) {
  FL_ATIM_ClearFlag_Update(ATIM);
  FL_ATIM_EnableIT_Update(ATIM);
}

void ATIM_NVIC_Init(void) {
  FL_NVIC_ConfigTypeDef InterruptConfigStruct;

  InterruptConfigStruct.preemptPriority = 0x02;
  FL_NVIC_Init(&InterruptConfigStruct, ATIM_IRQn);
}

void ATIM_Start() {
  // 使能LATIM
  FL_ATIM_Enable(ATIM);
}

void ATIM_Init(void) {
  /* Initial ATIM */
  MF_ATIM_TimerBase_Init();
  MF_ATIM_Interrupt_Init();

  /* Initial NVIC */
  ATIM_NVIC_Init();
  ATIM_Start();
}

void ATIM_IRQHandler() {
  if (FL_ATIM_IsEnabledIT_Update(ATIM) && FL_ATIM_IsActiveFlag_Update(ATIM)) {
    FL_ATIM_ClearFlag_Update(ATIM);
    system_tick_ms++; // 系统毫秒计数器累加
    if (uart3_Rec_shuju_time_count > 0) {
      uart3_Rec_shuju_time_count--;
    }
    if (uart4_Rec_shuju_time_count > 0) {
      uart4_Rec_shuju_time_count--;
    }
    if (uart1_rx_frame_timeout > 0) {
      uart1_rx_frame_timeout--;
    }
    if (uart0_rx_frame_timeout > 0) {
      uart0_rx_frame_timeout--;
    }
    if (Test_quanju_canshu_L.time_softdelay_ms > 0) {
      Test_quanju_canshu_L.time_softdelay_ms--;
    }
    if (Test_quanju_canshu_L.time_aroundtest_ms > 0) {
      Test_quanju_canshu_L.time_aroundtest_ms--;
    }
    if (Test_quanju_canshu_L.danbu_chaoshishijian_ms > 0) {
      Test_quanju_canshu_L.danbu_chaoshishijian_ms--;
    }
    if (LED_thing_time > 0) {
      LED_thing_time--;
    }
    if (uart0_tx_timeout_counter > 0) {
      uart0_tx_timeout_counter--;
    }
    if (uart0_bus_idle_counter > 0) {
      uart0_bus_idle_counter--;
    }
    if (uart1_tx_timeout_counter > 0) {
      uart1_tx_timeout_counter--;
    }
    if (chaoshi_dengdai_3 > 0) {
      chaoshi_dengdai_3--;
    }
    if (chaoshi_dengdai_4 > 0) {
      chaoshi_dengdai_4--;
    }
    if (debug_print_time > 0) {
      debug_print_time--;
    }
    // 通用测试框架超时计数器递减
    common_test_timer_tick();
    // 统一时间管理器滴答处理
    TM_SysTick_Handler();
    // 工装供电时间累加（仅在测试进行中累加，由test_fixture_power_time>0触发）
    if (test_fixture_power_time > 0) {
      test_fixture_power_time++;
    }
  }
}
