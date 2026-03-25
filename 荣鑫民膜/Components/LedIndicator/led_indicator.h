/**
 * @file led_indicator.h
 * @brief LED指示器组件 - 支持单/双LED，可注册不同方案
 * @version 1.0.0
 * @date 2026-01-31
 */

#ifndef __LED_INDICATOR_H__
#define __LED_INDICATOR_H__

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/*============================================================================
 *                          闪烁模式定义
 *===========================================================================*/

/**
 * @brief 闪烁模式枚举
 * @note 这些是基础闪烁模式，可组合使用
 */
typedef enum {
  LED_BLINK_OFF = 0,   // 常灭
  LED_BLINK_ON,        // 常亮
  LED_BLINK_SLOW,      // 慢闪 (1Hz, 500ms亮/500ms灭)
  LED_BLINK_NORMAL,    // 正常闪 (2Hz, 250ms亮/250ms灭)
  LED_BLINK_FAST,      // 快闪 (5Hz, 100ms亮/100ms灭)
  LED_BLINK_RAPID,     // 急促闪 (10Hz, 50ms亮/50ms灭)
  LED_BLINK_DOUBLE,    // 双闪 (闪2下-停-闪2下)
  LED_BLINK_TRIPLE,    // 三闪 (闪3下-停)
  LED_BLINK_HEARTBEAT, // 心跳 (快闪2下-长停)
  LED_BLINK_BREATH, // 呼吸灯效果（需PWM支持，否则降级为慢闪）
  LED_BLINK_MODE_COUNT
} LedBlinkMode_t;

/**
 * @brief 工装状态枚举 - 业务层使用
 */
typedef enum {
  LED_STATUS_POWER_ON = 0,   // 上电自检
  LED_STATUS_IDLE,           // 空闲等待
  LED_STATUS_TESTING,        // 测试进行中
  LED_STATUS_COMM_TX,        // 正在发送
  LED_STATUS_COMM_RX_WAIT,   // 等待响应
  LED_STATUS_PASS,           // 测试通过
  LED_STATUS_FAIL,           // 测试失败
  LED_STATUS_COMM_TIMEOUT,   // 通讯超时
  LED_STATUS_HARDWARE_ERROR, // 硬件故障
  LED_STATUS_COUNT
} LedStatus_t;

/*============================================================================
 *                          硬件抽象层
 *===========================================================================*/

/**
 * @brief LED控制回调函数类型
 * @param led_index LED索引 (0=LED1, 1=LED2, ...)
 * @param state 状态 (0=灭, 1=亮, 2-255=PWM亮度)
 */
typedef void (*LedControlFunc)(uint8_t led_index, uint8_t state);

/**
 * @brief 获取系统tick回调 (返回毫秒)
 */
typedef uint32_t (*GetTickFunc)(void);

/**
 * @brief LED硬件配置
 */
typedef struct {
  uint8_t led_count;      // LED数量 (1或2)
  LedControlFunc control; // LED控制函数
  GetTickFunc get_tick;   // 获取系统tick函数
} LedHardwareConfig_t;

/*============================================================================
 *                          方案定义
 *===========================================================================*/

/**
 * @brief 单个LED的状态映射
 */
typedef struct {
  LedBlinkMode_t led1_mode; // LED1闪烁模式
  LedBlinkMode_t led2_mode; // LED2闪烁模式 (单LED时忽略)
  uint16_t duration_ms;     // 持续时间，0=永久
  LedStatus_t next_status;  // 持续时间后跳转的状态，0xFF=保持
} LedStatusMapping_t;

/**
 * @brief LED指示方案
 */
typedef struct {
  const char *name;                             // 方案名称
  uint8_t led_count;                            // 适用的LED数量
  LedStatusMapping_t mapping[LED_STATUS_COUNT]; // 状态映射表
} LedScheme_t;

/*============================================================================
 *                          API
 *===========================================================================*/

/**
 * @brief 初始化LED指示器
 * @param hw_config 硬件配置
 * @return true=成功
 */
bool LedIndicator_Init(const LedHardwareConfig_t *hw_config);

/**
 * @brief 注册/切换指示方案
 * @param scheme 方案指针
 * @return true=成功
 */
bool LedIndicator_SetScheme(const LedScheme_t *scheme);

/**
 * @brief 设置当前状态
 * @param status 状态枚举
 */
void LedIndicator_SetStatus(LedStatus_t status);

/**
 * @brief 获取当前状态
 * @return 当前状态
 */
LedStatus_t LedIndicator_GetStatus(void);

/**
 * @brief 处理函数 - 在主循环中调用
 * @note 调用频率不影响闪烁效果，建议10-50ms调用一次
 */
void LedIndicator_Process(void);

/**
 * @brief 强制LED状态（调试用）
 * @param led_index LED索引
 * @param state 状态
 */
void LedIndicator_ForceState(uint8_t led_index, uint8_t state);

/**
 * @brief 获取方案名称
 */
const char *LedIndicator_GetSchemeName(void);

/*============================================================================
 *                          预定义方案
 *===========================================================================*/

/** @brief 单LED方案 - 红色LED */
extern const LedScheme_t LED_SCHEME_SINGLE_RED;

/** @brief 单LED方案 - 绿色LED */
extern const LedScheme_t LED_SCHEME_SINGLE_GREEN;

/** @brief 双LED方案 - 红+绿 */
extern const LedScheme_t LED_SCHEME_DUAL_RG;

/** @brief 双LED方案 - 红+蓝 */
extern const LedScheme_t LED_SCHEME_DUAL_RB;

#ifdef __cplusplus
}
#endif

#endif /* __LED_INDICATOR_H__ */
