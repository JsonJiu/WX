/**
 * @file led_indicator.c
 * @brief LED指示器组件实现
 */

#include "led_indicator.h"
#include <string.h>

/*============================================================================
 *                          闪烁模式时序表
 *===========================================================================*/

/**
 * @brief 闪烁模式时序定义
 * @note 用数组定义一个周期内的亮灭序列，-1表示结束
 *       时间单位：10ms
 */
typedef struct {
  uint16_t period_ms;   // 总周期时间
  uint8_t sequence[16]; // 亮灭序列 (1=亮, 0=灭)，按时间片划分
  uint8_t seq_count;    // 序列长度
} BlinkPattern_t;

static const BlinkPattern_t s_blink_patterns[LED_BLINK_MODE_COUNT] = {
    // LED_BLINK_OFF: 常灭
    [LED_BLINK_OFF] = {.period_ms = 1000, .sequence = {0}, .seq_count = 1},

    // LED_BLINK_ON: 常亮
    [LED_BLINK_ON] = {.period_ms = 1000, .sequence = {1}, .seq_count = 1},

    // LED_BLINK_SLOW: 慢闪 1Hz (500ms亮/500ms灭)
    [LED_BLINK_SLOW] = {.period_ms = 1000, .sequence = {1, 0}, .seq_count = 2},

    // LED_BLINK_NORMAL: 正常闪 2Hz (250ms亮/250ms灭)
    [LED_BLINK_NORMAL] = {.period_ms = 500, .sequence = {1, 0}, .seq_count = 2},

    // LED_BLINK_FAST: 快闪 5Hz (100ms亮/100ms灭)
    [LED_BLINK_FAST] = {.period_ms = 200, .sequence = {1, 0}, .seq_count = 2},

    // LED_BLINK_RAPID: 急促闪 10Hz (50ms亮/50ms灭)
    [LED_BLINK_RAPID] = {.period_ms = 100, .sequence = {1, 0}, .seq_count = 2},

    // LED_BLINK_DOUBLE: 双闪 (闪2下-停)
    // 100亮-100灭-100亮-700灭 = 1000ms周期
    [LED_BLINK_DOUBLE] = {.period_ms = 1000,
                          .sequence = {1, 0, 1, 0, 0, 0, 0, 0, 0, 0},
                          .seq_count = 10},

    // LED_BLINK_TRIPLE: 三闪 (闪3下-停)
    // 100亮-100灭-100亮-100灭-100亮-500灭 = 1000ms周期
    [LED_BLINK_TRIPLE] = {.period_ms = 1000,
                          .sequence = {1, 0, 1, 0, 1, 0, 0, 0, 0, 0},
                          .seq_count = 10},

    // LED_BLINK_HEARTBEAT: 心跳 (快闪2下-长停)
    // 50亮-50灭-50亮-850灭 = 1000ms周期
    [LED_BLINK_HEARTBEAT] = {.period_ms = 1000,
                             .sequence = {1, 0, 1, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
                                          0, 0, 0},
                             .seq_count = 16},

    // LED_BLINK_BREATH: 呼吸灯（简化为慢闪）
    [LED_BLINK_BREATH] = {.period_ms = 2000,
                          .sequence = {1, 0},
                          .seq_count = 2},
};

/*============================================================================
 *                          预定义方案
 *===========================================================================*/

// 单红色LED方案
const LedScheme_t LED_SCHEME_SINGLE_RED = {
    .name = "单LED(红)",
    .led_count = 1,
    .mapping = {
        //                        LED1模式              LED2模式(忽略) 持续时间
        //                        下一状态
        [LED_STATUS_POWER_ON] = {LED_BLINK_TRIPLE, LED_BLINK_OFF, 1000,
                                 LED_STATUS_IDLE},
        [LED_STATUS_IDLE] = {LED_BLINK_SLOW, LED_BLINK_OFF, 0, 0xFF},
        [LED_STATUS_TESTING] = {LED_BLINK_FAST, LED_BLINK_OFF, 0, 0xFF},
        [LED_STATUS_COMM_TX] = {LED_BLINK_ON, LED_BLINK_OFF, 100,
                                LED_STATUS_COMM_RX_WAIT},
        [LED_STATUS_COMM_RX_WAIT] = {LED_BLINK_DOUBLE, LED_BLINK_OFF, 0, 0xFF},
        [LED_STATUS_PASS] = {LED_BLINK_ON, LED_BLINK_OFF, 3000,
                             LED_STATUS_IDLE},
        [LED_STATUS_FAIL] = {LED_BLINK_RAPID, LED_BLINK_OFF, 5000,
                             LED_STATUS_IDLE},
        [LED_STATUS_COMM_TIMEOUT] = {LED_BLINK_TRIPLE, LED_BLINK_OFF, 0, 0xFF},
        [LED_STATUS_HARDWARE_ERROR] = {LED_BLINK_OFF, LED_BLINK_OFF, 0, 0xFF},
    }};

// 单绿色LED方案（与红色逻辑相同，只是语义不同）
const LedScheme_t LED_SCHEME_SINGLE_GREEN = {
    .name = "单LED(绿)",
    .led_count = 1,
    .mapping = {
        [LED_STATUS_POWER_ON] = {LED_BLINK_TRIPLE, LED_BLINK_OFF, 1000,
                                 LED_STATUS_IDLE},
        [LED_STATUS_IDLE] = {LED_BLINK_HEARTBEAT, LED_BLINK_OFF, 0, 0xFF},
        [LED_STATUS_TESTING] = {LED_BLINK_FAST, LED_BLINK_OFF, 0, 0xFF},
        [LED_STATUS_COMM_TX] = {LED_BLINK_ON, LED_BLINK_OFF, 100,
                                LED_STATUS_COMM_RX_WAIT},
        [LED_STATUS_COMM_RX_WAIT] = {LED_BLINK_NORMAL, LED_BLINK_OFF, 0, 0xFF},
        [LED_STATUS_PASS] = {LED_BLINK_ON, LED_BLINK_OFF, 3000,
                             LED_STATUS_IDLE},
        [LED_STATUS_FAIL] = {LED_BLINK_OFF, LED_BLINK_OFF, 5000,
                             LED_STATUS_IDLE}, // 绿灯灭=失败
        [LED_STATUS_COMM_TIMEOUT] = {LED_BLINK_SLOW, LED_BLINK_OFF, 0, 0xFF},
        [LED_STATUS_HARDWARE_ERROR] = {LED_BLINK_OFF, LED_BLINK_OFF, 0, 0xFF},
    }};

// 双LED方案 (LED1=绿, LED2=红)
const LedScheme_t LED_SCHEME_DUAL_RG = {
    .name = "双LED(绿+红)",
    .led_count = 2,
    .mapping = {
        //                        LED1(绿)              LED2(红) 持续时间
        //                        下一状态
        [LED_STATUS_POWER_ON] = {LED_BLINK_TRIPLE, LED_BLINK_TRIPLE, 1000,
                                 LED_STATUS_IDLE},
        [LED_STATUS_IDLE] = {LED_BLINK_SLOW, LED_BLINK_OFF, 0, 0xFF},
        [LED_STATUS_TESTING] = {LED_BLINK_FAST, LED_BLINK_OFF, 0, 0xFF},
        [LED_STATUS_COMM_TX] = {LED_BLINK_ON, LED_BLINK_OFF, 100,
                                LED_STATUS_COMM_RX_WAIT},
        [LED_STATUS_COMM_RX_WAIT] = {LED_BLINK_FAST, LED_BLINK_SLOW, 0, 0xFF},
        [LED_STATUS_PASS] = {LED_BLINK_ON, LED_BLINK_OFF, 3000,
                             LED_STATUS_IDLE},
        [LED_STATUS_FAIL] = {LED_BLINK_OFF, LED_BLINK_ON, 3000,
                             LED_STATUS_IDLE},
        [LED_STATUS_COMM_TIMEOUT] = {LED_BLINK_OFF, LED_BLINK_FAST, 0, 0xFF},
        [LED_STATUS_HARDWARE_ERROR] = {LED_BLINK_OFF, LED_BLINK_OFF, 0, 0xFF},
    }};

// 双LED方案 (LED1=红, LED2=蓝)
const LedScheme_t LED_SCHEME_DUAL_RB = {
    .name = "双LED(红+蓝)",
    .led_count = 2,
    .mapping = {
        [LED_STATUS_POWER_ON] = {LED_BLINK_TRIPLE, LED_BLINK_TRIPLE, 1000,
                                 LED_STATUS_IDLE},
        [LED_STATUS_IDLE] = {LED_BLINK_SLOW, LED_BLINK_OFF, 0, 0xFF},
        [LED_STATUS_TESTING] = {LED_BLINK_OFF, LED_BLINK_FAST, 0, 0xFF},
        [LED_STATUS_COMM_TX] = {LED_BLINK_OFF, LED_BLINK_ON, 100,
                                LED_STATUS_COMM_RX_WAIT},
        [LED_STATUS_COMM_RX_WAIT] = {LED_BLINK_SLOW, LED_BLINK_FAST, 0, 0xFF},
        [LED_STATUS_PASS] = {LED_BLINK_OFF, LED_BLINK_ON, 3000,
                             LED_STATUS_IDLE},
        [LED_STATUS_FAIL] = {LED_BLINK_ON, LED_BLINK_OFF, 3000,
                             LED_STATUS_IDLE},
        [LED_STATUS_COMM_TIMEOUT] = {LED_BLINK_FAST, LED_BLINK_OFF, 0, 0xFF},
        [LED_STATUS_HARDWARE_ERROR] = {LED_BLINK_OFF, LED_BLINK_OFF, 0, 0xFF},
    }};

/*============================================================================
 *                          内部状态
 *===========================================================================*/

static struct {
  bool initialized;
  LedHardwareConfig_t hw;
  const LedScheme_t *scheme;
  LedStatus_t current_status;
  uint32_t status_enter_tick; // 进入当前状态的时刻
  uint8_t led_states[2];      // 当前LED状态缓存
} s_ctx = {0};

/*============================================================================
 *                          内部函数
 *===========================================================================*/

/**
 * @brief 根据闪烁模式和当前时间计算LED应该亮还是灭
 * @param mode 闪烁模式
 * @param tick 当前系统tick (ms)
 * @return 0=灭, 1=亮
 */
static uint8_t calc_led_state(LedBlinkMode_t mode, uint32_t tick) {
  if (mode >= LED_BLINK_MODE_COUNT) {
    return 0;
  }

  const BlinkPattern_t *pattern = &s_blink_patterns[mode];

  // 计算在周期内的位置
  uint32_t pos_in_period = tick % pattern->period_ms;

  // 计算当前在哪个时间片
  uint32_t slice_duration = pattern->period_ms / pattern->seq_count;
  uint8_t slice_index = pos_in_period / slice_duration;

  if (slice_index >= pattern->seq_count) {
    slice_index = pattern->seq_count - 1;
  }

  return pattern->sequence[slice_index];
}

/**
 * @brief 更新LED输出
 */
static void update_led_output(uint8_t led_index, uint8_t new_state) {
  if (led_index >= 2 || s_ctx.hw.control == NULL) {
    return;
  }

  // 只在状态变化时调用硬件
  if (s_ctx.led_states[led_index] != new_state) {
    s_ctx.led_states[led_index] = new_state;
    s_ctx.hw.control(led_index, new_state);
  }
}

/*============================================================================
 *                          API实现
 *===========================================================================*/

bool LedIndicator_Init(const LedHardwareConfig_t *hw_config) {
  if (hw_config == NULL || hw_config->control == NULL ||
      hw_config->get_tick == NULL || hw_config->led_count == 0) {
    return false;
  }

  memset(&s_ctx, 0, sizeof(s_ctx));
  s_ctx.hw = *hw_config;
  s_ctx.initialized = true;
  s_ctx.current_status = LED_STATUS_POWER_ON;
  s_ctx.status_enter_tick = s_ctx.hw.get_tick();

  // 默认使用单LED红色方案
  if (hw_config->led_count == 1) {
    s_ctx.scheme = &LED_SCHEME_SINGLE_RED;
  } else {
    s_ctx.scheme = &LED_SCHEME_DUAL_RG;
  }

  return true;
}

bool LedIndicator_SetScheme(const LedScheme_t *scheme) {
  if (scheme == NULL) {
    return false;
  }

  s_ctx.scheme = scheme;
  return true;
}

void LedIndicator_SetStatus(LedStatus_t status) {
  if (!s_ctx.initialized || status >= LED_STATUS_COUNT) {
    return;
  }

  if (s_ctx.current_status != status) {
    s_ctx.current_status = status;
    s_ctx.status_enter_tick = s_ctx.hw.get_tick();
  }
}

LedStatus_t LedIndicator_GetStatus(void) { return s_ctx.current_status; }

void LedIndicator_Process(void) {
  if (!s_ctx.initialized || s_ctx.scheme == NULL) {
    return;
  }

  uint32_t current_tick = s_ctx.hw.get_tick();
  const LedStatusMapping_t *mapping =
      &s_ctx.scheme->mapping[s_ctx.current_status];

  // 检查是否需要自动跳转状态
  if (mapping->duration_ms > 0) {
    uint32_t elapsed = current_tick - s_ctx.status_enter_tick;
    if (elapsed >= mapping->duration_ms &&
        (uint8_t)mapping->next_status != 0xFF) {
      LedIndicator_SetStatus(mapping->next_status);
      mapping = &s_ctx.scheme->mapping[s_ctx.current_status];
    }
  }

  // 计算LED1状态
  uint8_t led1_state = calc_led_state(mapping->led1_mode, current_tick);
  update_led_output(0, led1_state);

  // 计算LED2状态（如果有）
  if (s_ctx.hw.led_count >= 2) {
    uint8_t led2_state = calc_led_state(mapping->led2_mode, current_tick);
    update_led_output(1, led2_state);
  }
}

void LedIndicator_ForceState(uint8_t led_index, uint8_t state) {
  if (s_ctx.initialized && s_ctx.hw.control != NULL && led_index < 2) {
    s_ctx.hw.control(led_index, state);
    s_ctx.led_states[led_index] = state;
  }
}

const char *LedIndicator_GetSchemeName(void) {
  return s_ctx.scheme ? s_ctx.scheme->name : "未设置";
}
