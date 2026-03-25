#include "LED_CTRL.h"
#include "GPIO.h"
#include "led_indicator.h"

/*============================================================================
 *                     旧版LED控制 (兼容保留)
 *===========================================================================*/

uint8_t LED_thing_FLAG = 0;
uint8_t LED_thing_time = 0;

void LED_FLAG_Run() {
  LED_On();
  LED_thing_FLAG = 1;
  // 灯会亮一下
  LED_thing_time = 20;
}

void LED_FLAG_LOOP() {
  if (LED_thing_FLAG == 0)
    return;
  if (LED_thing_time != 0)
    return;
  LED_thing_FLAG = 0;
  LED_Off();
}

/*============================================================================
 *                     新版LED指示器组件适配
 *===========================================================================*/

// 系统tick (定义在 time.c)
extern volatile uint32_t system_tick_ms;

/**
 * @brief LED硬件控制回调
 * @param led_index LED索引 (0=LED1, 当前工装只有1个LED)
 * @param state 状态 (0=灭, 1=亮)
 */
static void board_led_control(uint8_t led_index, uint8_t state) {
  if (led_index == 0) {
    if (state) {
      LED_On();
    } else {
      LED_Off();
    }
  }
  // 如果以后加第二个LED，在这里添加 led_index == 1 的处理
}

/**
 * @brief 获取系统tick回调
 * @return 系统毫秒数
 */
static uint32_t board_get_tick(void) { return system_tick_ms; }

/**
 * @brief 初始化LED指示器组件
 * @note 在系统初始化时调用一次
 */
void LedIndicator_BoardInit(void) {
  LedHardwareConfig_t hw_cfg = {
      .led_count = 1, // 当前工装只有1个LED (PB4)
      .control = board_led_control,
      .get_tick = board_get_tick,
  };

  LedIndicator_Init(&hw_cfg);
  LedIndicator_SetScheme(&LED_SCHEME_SINGLE_RED); // 使用单红LED方案

  // 直接进入空闲状态（跳过上电自检动画），避免初始化期间状态不确定
  LedIndicator_SetStatus(LED_STATUS_IDLE);
}
