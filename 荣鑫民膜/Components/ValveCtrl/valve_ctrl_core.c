/**
 * @file valve_ctrl_core.c
 * @brief 阀门控制组件 - 核心层实现
 * @version 2.0.0
 * @date 2024-12
 *
 * 本文件实现阀门测试的核心状态机逻辑。
 * 特点：
 * - 零硬件依赖：不包含任何 #include "main.h" 或硬件头文件
 * - 纯逻辑：所有硬件操作通过 HAL 函数指针完成
 * - 可移植：可以直接复制到其他项目使用
 * - 可测试：可以在 PC 上用 Mock 函数进行单元测试
 */

#include "valve_ctrl_core.h"
#include <stdarg.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>

/*============================================================================*/
/*                              内部宏定义 */
/*============================================================================*/

/** 安全调用 HAL 函数的宏 */
#define HAL_CALL(ctx, func, ...)                                               \
  do {                                                                         \
    if ((ctx)->hal && (ctx)->hal->func) {                                      \
      (ctx)->hal->func(__VA_ARGS__);                                           \
    }                                                                          \
  } while (0)

/** 安全调用 HAL 函数并获取返回值 */
#define HAL_CALL_RET(ctx, func, default_val, ...)                              \
  (((ctx)->hal && (ctx)->hal->func) ? (ctx)->hal->func(__VA_ARGS__)            \
                                    : (default_val))

/** 调试打印宏 */
#define CORE_DEBUG(ctx, fmt, ...)                                              \
  do {                                                                         \
    if ((ctx)->hal && (ctx)->hal->debug_print) {                               \
      (ctx)->hal->debug_print(fmt, ##__VA_ARGS__);                             \
    }                                                                          \
  } while (0)

/*============================================================================*/
/*                              内部函数声明 */
/*============================================================================*/

static void enter_step(ValveCtrl_Context_t *ctx, VT_TestStep step,
                       uint32_t timeout_ms);
static VT_StepResult step_wait_response(ValveCtrl_Context_t *ctx,
                                        uint16_t expected_code);
static VT_StepResult
step_wait_response_with_retry(ValveCtrl_Context_t *ctx, uint16_t expected_code,
                              void (*resend_func)(ValveCtrl_Context_t *),
                              uint32_t success_delay_ms,
                              uint32_t fail_delay_ms);

/* 步骤处理函数 - 重发回调 */
static void resend_config(ValveCtrl_Context_t *ctx);
static void resend_open_valve(ValveCtrl_Context_t *ctx);
static void resend_close_valve(ValveCtrl_Context_t *ctx);

/*============================================================================*/
/*                              内部辅助函数 */
/*============================================================================*/

/**
 * @brief 进入新步骤
 */
static void enter_step(ValveCtrl_Context_t *ctx, VT_TestStep step,
                       uint32_t timeout_ms) {
  ctx->current_step = step;
  ctx->step_time_ms = 0;
  ctx->step_timeout_ms = timeout_ms;
  ctx->response_received = 0;
  ctx->retry_count = 0;
}

/**
 * @brief 等待协议响应的通用逻辑
 * @param ctx 阀门测试上下文,包含相应的测试步骤，超时等等所有的信息
 * @param expected_code 期望的响应码
 */
static VT_StepResult step_wait_response(ValveCtrl_Context_t *ctx,
                                        uint16_t expected_code) {
  // 如果还未收到响应，检查超时，超时时间内，返回忙碌状态;超时则返回失败
  if (ctx->response_received != 1) {
    if (ctx->step_time_ms >= ctx->step_timeout_ms) {
      CORE_DEBUG(
          ctx, "等待响应超时: step_time=%lu, timeout=%lu, expected=0x%04X\r\n",
          ctx->step_time_ms, ctx->step_timeout_ms, expected_code);
      return VT_STEP_FAIL;
    }
    return VT_STEP_BUSY;
  }

  CORE_DEBUG(ctx, "收到响应: code=0x%04X, expected=0x%04X\r\n",
             ctx->response_code, expected_code);
  // 如果响应匹配，那么清除响应标志并返回成功
  if (ctx->response_code == expected_code) {
    ctx->response_received = 0;
    return VT_STEP_SUCCESS;
  }

  // 设置响应2避免重复处理，说明响应不匹配
  ctx->response_received = 2;
  return VT_STEP_MISMATCH;
}

/**
 * @brief 等待协议响应，响应不匹配时自动重发
 * @param ctx 阀门测试上下文,包含相应的测试步骤，超时等等所有的信息
 * @param expected_code 期望的响应码
 * @param resend_func 重发函数指针
 * @param success_delay_ms 成功后的软件延时(ms)，0表示不延时
 * @param fail_delay_ms 失败后的软件延时(ms)，0表示不延时
 */
static VT_StepResult
step_wait_response_with_retry(ValveCtrl_Context_t *ctx, uint16_t expected_code,
                              void (*resend_func)(ValveCtrl_Context_t *),
                              uint32_t success_delay_ms,
                              uint32_t fail_delay_ms) {
  VT_StepResult r = step_wait_response(ctx, expected_code);

  // 成功时设置延时(如果需要)
  if (r == VT_STEP_SUCCESS && success_delay_ms > 0) {
    HAL_CALL(ctx, set_soft_delay, success_delay_ms);
  }

  // 如果响应匹配不成功且提供了重发函数，则进行重发处理,否则后续返回当前的结果
  if (r == VT_STEP_MISMATCH && resend_func != NULL) {
    if (++ctx->retry_count > ctx->retry_max) {
      ctx->retry_count = 0;
      CORE_DEBUG(ctx, "重试次数超限，失败\r\n");
      // 失败时设置延时(如果需要)
      if (fail_delay_ms > 0) {
        HAL_CALL(ctx, set_soft_delay, fail_delay_ms);
      }
      return VT_STEP_FAIL;
    }
    resend_func(ctx);
    CORE_DEBUG(ctx, "响应不匹配，重发...\r\n");
    return VT_STEP_BUSY;
  }

  // 超时失败时也设置延时
  if (r == VT_STEP_FAIL && fail_delay_ms > 0) {
    HAL_CALL(ctx, set_soft_delay, fail_delay_ms);
  }

  return r;
}

/* 重发回调函数 */
static void resend_config(ValveCtrl_Context_t *ctx) {
  HAL_CALL(ctx, send_config);
}

static void resend_open_valve(ValveCtrl_Context_t *ctx) {
  HAL_CALL(ctx, send_open_valve);
}

static void resend_close_valve(ValveCtrl_Context_t *ctx) {
  HAL_CALL(ctx, send_close_valve);
}

/*============================================================================*/
/*                              公开 API 实现 */
/*============================================================================*/

void ValveCtrl_Core_Init(ValveCtrl_Context_t *ctx, const ValveCtrl_HAL_t *hal) {
  if (ctx == NULL)
    return;

  memset(ctx, 0, sizeof(ValveCtrl_Context_t));
  ctx->hal = hal;
  ctx->current_step = VT_STEP_INIT;
  ctx->result = VT_IDLE;
  ctx->enabled = 0;
  ctx->retry_max = VALVE_MAX_RETRY_COUNT;
  ctx->total_timeout_ms = VALVE_TOTAL_TIMEOUT_MS;
  ctx->config_param1 = 15;
  ctx->config_param2 = 230;
  ctx->fail_reason = VT_FAIL_NONE;
  ctx->fail_step = VT_STEP_INIT;
}

void ValveCtrl_Core_Start(ValveCtrl_Context_t *ctx) {
  if (ctx == NULL || ctx->hal == NULL)
    return;

  /* 确保两个到位信号都是高电平（未到位） */
  HAL_CALL(ctx, output_valve_position_signals, 0, 0);

  ctx->enabled = 1;
  ctx->current_step = VT_STEP_INIT;
  ctx->result = VT_RUNNING;
  ctx->total_time_ms = 0;
  ctx->step_time_ms = 0;
  ctx->retry_count = 0;
  ctx->response_received = 0;
  ctx->voltage_a = 0;
  ctx->voltage_b = 0;
  ctx->pos_open = 0;
  ctx->pos_close = 0;
  ctx->initial_voltage_a = 0;
  ctx->initial_voltage_b = 0;
  ctx->initial_pos_open = 0;
  ctx->initial_pos_close = 0;
  ctx->fail_reason = VT_FAIL_NONE;
  ctx->fail_step = VT_STEP_INIT;

  /* 缓存表类型和期望响应码 (测试期间不变，避免循环中重复HAL调用) */
  ctx->meter_type = HAL_CALL_RET(ctx, get_meter_type, VALVE_METER_MECHANICAL);
  ctx->expected_config_code =
      HAL_CALL_RET(ctx, get_expected_config_code, 0x2604);

  CORE_DEBUG(ctx, "\r\n");
  CORE_DEBUG(ctx, "========================================\r\n");
  CORE_DEBUG(ctx, "           阀 门 测 试 启 动           \r\n");
  CORE_DEBUG(ctx, "========================================\r\n");
  CORE_DEBUG(ctx, "表类型: %s\r\n",
             ctx->meter_type == VALVE_METER_MECHANICAL ? "机械表" : "超声波表");
  CORE_DEBUG(ctx, "超时设置: %lus\r\n", ctx->total_timeout_ms / 1000);
  CORE_DEBUG(ctx, "----------------------------------------\r\n");
}

void ValveCtrl_Core_Stop(ValveCtrl_Context_t *ctx) {
  if (ctx == NULL)
    return;

  /* 恢复 GPIO 为输入模式，防止与外部设备电平冲突 */
  HAL_CALL(ctx, restore_gpio_to_input);

  ctx->enabled = 0;
  ctx->result = VT_IDLE;
  ctx->current_step = VT_STEP_INIT;

  CORE_DEBUG(ctx, "阀门测试停止\r\n");
}

/**
 * @brief  阀门测试主循环，主要测试逻辑
 *
 * @param ctx 阀门的测试上下文，包含所有运行时状态，配置参数等
 * @param tick_ms 本次调用的时间增量 (毫秒)
 * @note 该函数应周期性调用，返回当前测试结果
 * @return VT_TestResult
 */
VT_TestResult ValveCtrl_Core_Loop(ValveCtrl_Context_t *ctx, uint32_t tick_ms) {
  if (ctx == NULL || ctx->hal == NULL)
    return VT_IDLE;
  if (!ctx->enabled)
    return VT_IDLE;

  /* 更新时间 - 无论是否在延时中都要累加 */
  ctx->total_time_ms += tick_ms;
  ctx->step_time_ms += tick_ms;

  /* 总超时检查 */
  if (ctx->total_time_ms > ctx->total_timeout_ms) {
    ctx->result = VT_TIMEOUT;
    ctx->fail_reason = VT_FAIL_TOTAL_TIMEOUT;
    ctx->fail_step = ctx->current_step;
    ctx->enabled = 0;
    CORE_DEBUG(ctx, "阀门测试总超时，当前步骤=%d\r\n", ctx->current_step);
    return VT_TIMEOUT;
  }

  /* 软件延时处理 */
  if (!HAL_CALL_RET(ctx, is_soft_delay_done, true)) {
    return VT_RUNNING;
  }

  /* 使用缓存的表类型和期望响应码 (Start时已初始化，避免重复HAL调用) */
  ValveMeterType meter_type = ctx->meter_type;
  uint16_t expected_config_code = ctx->expected_config_code;

  switch (ctx->current_step) {

    /*==================== 初始化阶段 ====================*/

  case VT_STEP_INIT:
    CORE_DEBUG(ctx, "\r\n[步骤1/9] 📤 发送配置命令\r\n");
    CORE_DEBUG(ctx, "  命令: %s\r\n",
               meter_type == VALVE_METER_MECHANICAL ? "0x2604(机械表)"
                                                    : "0x2036(超声波表)");
    CORE_DEBUG(ctx, "  等待响应中...\r\n");
    enter_step(ctx, VT_STEP_CONFIG, VALVE_CONFIG_TIMEOUT_MS);
    HAL_CALL(ctx, send_config);
    // 设置软件延时等待
    HAL_CALL(ctx, set_soft_delay, VALVE_CONFIG_DELAY_MS);
    break;

  case VT_STEP_CONFIG: {
    VT_StepResult r = step_wait_response_with_retry(ctx, expected_config_code,
                                                    resend_config, 100, 0);
    if (r == VT_STEP_SUCCESS) {
      CORE_DEBUG(ctx, "  ✓ 配置响应收到\r\n");
      enter_step(ctx, VT_STEP_CHECK_INITIAL, VALVE_INITIAL_CHECK_TIMEOUT_MS);
    } else if (r == VT_STEP_FAIL) {
      ctx->result = VT_FAIL;
      ctx->fail_reason = VT_FAIL_CONFIG_RETRY;
      ctx->fail_step = VT_STEP_CONFIG;
      ctx->enabled = 0;
      CORE_DEBUG(ctx, "  ✗ 配置失败，重试超限\r\n");
      return VT_FAIL;
    }
    break;
  }

  case VT_STEP_CHECK_INITIAL: {
    ctx->voltage_a = HAL_CALL_RET(ctx, read_voltage_a, 0);
    ctx->voltage_b = HAL_CALL_RET(ctx, read_voltage_b, 0);

    CORE_DEBUG(ctx, "\r\n[步骤2/9] 🔍 检查初始状态\r\n");
    CORE_DEBUG(ctx, "  ┌────────────────────────────────────┐\r\n");
    CORE_DEBUG(ctx, "  │ 电压A: %4lumV (期望>%dmV)         │\r\n",
               ctx->voltage_a, VALVE_VOLTAGE_LOW_THRESHOLD);
    CORE_DEBUG(ctx, "  │ 电压B: %4lumV (期望<%dmV)         │\r\n",
               ctx->voltage_b, VALVE_VOLTAGE_LOW_THRESHOLD);
    CORE_DEBUG(ctx, "  └────────────────────────────────────┘\r\n");

    if (ctx->voltage_a > VALVE_VOLTAGE_LOW_THRESHOLD &&
        ctx->voltage_b < VALVE_VOLTAGE_LOW_THRESHOLD) {
      CORE_DEBUG(ctx, "  ✓ 初始状态正常\r\n");
      CORE_DEBUG(ctx, "\r\n[步骤3/9] 📤 发送开阀命令 (0xC022)\r\n");
      CORE_DEBUG(ctx, "  等待响应中...\r\n");
      enter_step(ctx, VT_STEP_SEND_OPEN, VALVE_OPEN_CMD_TIMEOUT_MS);
      HAL_CALL(ctx, send_open_valve);
      HAL_CALL(ctx, set_soft_delay, VALVE_CMD_DELAY_MS);
    } else {
      CORE_DEBUG(ctx, ">> ⚠️ 初始状态异常:\r\n");
      if (ctx->voltage_a >= VALVE_VOLTAGE_LOW_THRESHOLD) {
        CORE_DEBUG(ctx, "   - 电压A异常: 期望>%dmV, 实际=%lumV\r\n",
                   VALVE_VOLTAGE_LOW_THRESHOLD, ctx->voltage_a);
      }
      if (ctx->voltage_b >= VALVE_VOLTAGE_LOW_THRESHOLD) {
        CORE_DEBUG(ctx, "   - 电压B异常: 期望<%dmV, 实际=%lumV\r\n",
                   VALVE_VOLTAGE_LOW_THRESHOLD, ctx->voltage_b);
      }

      if (++ctx->retry_count > ctx->retry_max) {
        CORE_DEBUG(ctx, ">> ❌ 初始状态检查失败，重试次数已用尽\r\n");
        ctx->result = VT_FAIL;
        ctx->fail_step = VT_STEP_CHECK_INITIAL;
        if (ctx->voltage_a >= VALVE_VOLTAGE_LOW_THRESHOLD) {
          ctx->fail_reason = VT_FAIL_INITIAL_VOLTAGE_A;
        } else if (ctx->voltage_b >= VALVE_VOLTAGE_LOW_THRESHOLD) {
          ctx->fail_reason = VT_FAIL_INITIAL_VOLTAGE_B;
        } else {
          ctx->fail_reason = VT_FAIL_INITIAL_RETRY;
        }
        ctx->enabled = 0;
        return VT_FAIL;
      }
      CORE_DEBUG(ctx, "重试 %d/%d...\r\n", ctx->retry_count, ctx->retry_max);
      enter_step(ctx, VT_STEP_CONFIG, VALVE_CONFIG_TIMEOUT_MS);
    }
    break;
  }

    /*==================== 开阀测试阶段 ====================*/

  case VT_STEP_SEND_OPEN: {
    VT_StepResult r =
        step_wait_response_with_retry(ctx, 0xC022, resend_open_valve, 0, 0);
    if (r == VT_STEP_SUCCESS) {
      CORE_DEBUG(ctx, "  ✓ 开阀命令响应收到\r\n");
      CORE_DEBUG(ctx, "\r\n[步骤4/9] ⚡ 检测开阀动作\r\n");
      CORE_DEBUG(ctx, "  期望: 电压A>%dmV, 电压B<%dmV\r\n",
                 VALVE_VOLTAGE_HIGH_THRESHOLD, VALVE_VOLTAGE_LOW_THRESHOLD);
      enter_step(ctx, VT_STEP_DETECT_OPENING, VALVE_OPEN_DETECT_TIMEOUT_MS);
    } else if (r == VT_STEP_FAIL) {
      if (ctx->retry_count >= ctx->retry_max) {
        CORE_DEBUG(ctx, "  ✗ 开阀命令重试超限\r\n");
        ctx->result = VT_FAIL;
        ctx->fail_reason = VT_FAIL_OPEN_CMD_TIMEOUT;
        ctx->fail_step = VT_STEP_SEND_OPEN;
        ctx->enabled = 0;
        CORE_DEBUG(ctx, "  ✗ 开阀命令发送失败\r\n");
        return VT_FAIL;
      }
      CORE_DEBUG(ctx, "  ✗ 开阀命令响应超时，重发...\r\n");
      enter_step(ctx, VT_STEP_SEND_OPEN, VALVE_OPEN_CMD_TIMEOUT_MS);
    }
    break;
  }

  case VT_STEP_DETECT_OPENING: {
    ctx->voltage_a = HAL_CALL_RET(ctx, read_voltage_a, 0);
    ctx->voltage_b = HAL_CALL_RET(ctx, read_voltage_b, 0);

    if (ctx->step_time_ms % 1000 < tick_ms) {
      CORE_DEBUG(ctx, "  [%2lus] A=%4lumV, B=%4lumV\r\n",
                 ctx->step_time_ms / 1000, ctx->voltage_a, ctx->voltage_b);
    }

    if (ctx->voltage_a > VALVE_VOLTAGE_HIGH_THRESHOLD &&
        ctx->voltage_b < VALVE_VOLTAGE_LOW_THRESHOLD) {
      CORE_DEBUG(ctx, "  ✓ 检测到开阀动作! A=%lumV, B=%lumV\r\n",
                 ctx->voltage_a, ctx->voltage_b);
      CORE_DEBUG(ctx, "\r\n[步骤5/9] 📍 输出开阀到位信号\r\n");
      enter_step(ctx, VT_STEP_OUTPUT_OPEN_SIGNAL, 1000);
    } else if (ctx->step_time_ms >= ctx->step_timeout_ms) {
      CORE_DEBUG(ctx, "\r\n  ✗ 开阀动作检测超时!\r\n");
      CORE_DEBUG(ctx, "    最终状态: A=%lumV, B=%lumV\r\n", ctx->voltage_a,
                 ctx->voltage_b);
      CORE_DEBUG(ctx, "    期望: A>%dmV, B<%dmV\r\n",
                 VALVE_VOLTAGE_HIGH_THRESHOLD, VALVE_VOLTAGE_LOW_THRESHOLD);
      if (ctx->voltage_a < VALVE_VOLTAGE_LOW_THRESHOLD &&
          ctx->voltage_b > VALVE_VOLTAGE_HIGH_THRESHOLD) {
        CORE_DEBUG(ctx, "    提示: 电压AB可能定义反了!\r\n");
      }

      /* 有限重试策略：最多重试 retry_max 次 */
      if (++ctx->retry_count > ctx->retry_max) {
        CORE_DEBUG(ctx, "  ✗ 开阀动作检测重试超限 (%d/%d)\r\n",
                   ctx->retry_count - 1, ctx->retry_max);
        ctx->result = VT_TIMEOUT;
        ctx->fail_reason = VT_FAIL_OPEN_DETECT_TIMEOUT;
        ctx->fail_step = VT_STEP_DETECT_OPENING;
        ctx->enabled = 0;
        return VT_TIMEOUT;
      }

      CORE_DEBUG(ctx, "    重试 %d/%d: 重新发送开阀命令...\r\n",
                 ctx->retry_count, ctx->retry_max);
      enter_step(ctx, VT_STEP_SEND_OPEN, VALVE_OPEN_CMD_TIMEOUT_MS);
      HAL_CALL(ctx, send_open_valve);
    }
    break;
  }

  case VT_STEP_OUTPUT_OPEN_SIGNAL:
    HAL_CALL(ctx, output_valve_position_signals, 1, 0);
    CORE_DEBUG(ctx, "  输出: 开阀到位=低电平, 关阀到位=高电平\r\n");
    CORE_DEBUG(ctx, "  等待%dms让水表检测信号...\r\n", VALVE_SIGNAL_DELAY_MS);
    HAL_CALL(ctx, set_soft_delay, VALVE_SIGNAL_DELAY_MS);
    enter_step(ctx, VT_STEP_CHECK_OPEN_STATE, VALVE_STATE_CHECK_TIMEOUT_MS);
    break;

  case VT_STEP_CHECK_OPEN_STATE: {
    ctx->voltage_a = HAL_CALL_RET(ctx, read_voltage_a, 0);
    ctx->voltage_b = HAL_CALL_RET(ctx, read_voltage_b, 0);

    CORE_DEBUG(ctx, "\r\n[步骤6/9] 🔍 检查开阀后状态\r\n");
    CORE_DEBUG(ctx, "  ┌────────────────────────────────────┐\r\n");
    CORE_DEBUG(ctx, "  │ 电压A: %4lumV (期望<%dmV)         │\r\n",
               ctx->voltage_a, VALVE_VOLTAGE_LOW_THRESHOLD);
    CORE_DEBUG(ctx, "  │ 电压B: %4lumV (期望<%dmV)         │\r\n",
               ctx->voltage_b, VALVE_VOLTAGE_LOW_THRESHOLD);
    CORE_DEBUG(ctx, "  └────────────────────────────────────┘\r\n");

    if (ctx->voltage_a < VALVE_VOLTAGE_LOW_THRESHOLD &&
        ctx->voltage_b < VALVE_VOLTAGE_LOW_THRESHOLD) {
      CORE_DEBUG(ctx, "  ✓ 开阀状态正常\r\n");
      CORE_DEBUG(ctx, "\r\n[步骤7/9] 📤 发送关阀命令 (0xC022)\r\n");
      HAL_CALL(ctx, output_valve_position_signals, 0, 0);
      CORE_DEBUG(ctx, "  恢复: 开阀到位=高电平(未到位)\r\n");
      CORE_DEBUG(ctx, "  等待响应中...\r\n");
      enter_step(ctx, VT_STEP_SEND_CLOSE, VALVE_CLOSE_CMD_TIMEOUT_MS);
      HAL_CALL(ctx, send_close_valve);
    } else {
      CORE_DEBUG(ctx, "  ✗ 开阀状态异常!\r\n");
      if (ctx->voltage_a >= VALVE_VOLTAGE_LOW_THRESHOLD)
        CORE_DEBUG(ctx, "    - 电压A异常\r\n");
      if (ctx->voltage_b >= VALVE_VOLTAGE_LOW_THRESHOLD)
        CORE_DEBUG(ctx, "    - 电压B异常\r\n");
      if (++ctx->retry_count > ctx->retry_max) {
        ctx->result = VT_FAIL;
        ctx->fail_reason = VT_FAIL_OPEN_STATE_CHECK;
        ctx->fail_step = VT_STEP_CHECK_OPEN_STATE;
        ctx->enabled = 0;
        CORE_DEBUG(ctx, "  ✗ 重试次数已用尽\r\n");
        return VT_FAIL;
      }
      CORE_DEBUG(ctx, "  重试 %d/%d...\r\n", ctx->retry_count, ctx->retry_max);
      enter_step(ctx, VT_STEP_OUTPUT_OPEN_SIGNAL, 10000);
    }
    break;
  }

    /*==================== 关阀测试阶段 ====================*/

  case VT_STEP_SEND_CLOSE: {
    VT_StepResult r =
        step_wait_response_with_retry(ctx, 0xC022, resend_close_valve, 0, 0);
    if (r == VT_STEP_SUCCESS) {
      CORE_DEBUG(ctx, "  ✓ 关阀命令响应收到\r\n");
      CORE_DEBUG(ctx, "\r\n[步骤8/9] ⚡ 检测关阀动作(反转)\r\n");
      CORE_DEBUG(ctx, "  期望: 电压A<%dmV, 电压B>%dmV\r\n",
                 VALVE_VOLTAGE_LOW_THRESHOLD, VALVE_VOLTAGE_HIGH_THRESHOLD);
      enter_step(ctx, VT_STEP_DETECT_CLOSING, VALVE_CLOSE_DETECT_TIMEOUT_MS);
    } else if (r == VT_STEP_FAIL) {
      ctx->result = VT_FAIL;
      ctx->fail_reason = VT_FAIL_CLOSE_CMD_TIMEOUT;
      ctx->fail_step = VT_STEP_SEND_CLOSE;
      ctx->enabled = 0;
      CORE_DEBUG(ctx, "  ✗ 关阀命令发送失败\r\n");
      return VT_FAIL;
    }
    break;
  }

  case VT_STEP_DETECT_CLOSING: {
    ctx->voltage_a = HAL_CALL_RET(ctx, read_voltage_a, 0);
    ctx->voltage_b = HAL_CALL_RET(ctx, read_voltage_b, 0);

    if (ctx->step_time_ms % 1000 < tick_ms) {
      CORE_DEBUG(ctx, "  [%2lus] A=%4lumV, B=%4lumV\r\n",
                 ctx->step_time_ms / 1000, ctx->voltage_a, ctx->voltage_b);
    }

    if (ctx->voltage_a < VALVE_VOLTAGE_LOW_THRESHOLD &&
        ctx->voltage_b > VALVE_VOLTAGE_HIGH_THRESHOLD) {
      CORE_DEBUG(ctx, "  ✓ 检测到关阀反转! A=%lumV, B=%lumV\r\n",
                 ctx->voltage_a, ctx->voltage_b);
      CORE_DEBUG(ctx, "\r\n[步骤9/9] 📍 输出关阀到位信号\r\n");
      enter_step(ctx, VT_STEP_OUTPUT_CLOSE_SIGNAL, 1000);
    } else if (ctx->step_time_ms >= ctx->step_timeout_ms) {
      CORE_DEBUG(ctx, "\r\n  ✗ 关阀动作检测超时!\r\n");
      CORE_DEBUG(ctx, "    最终状态: A=%lumV, B=%lumV\r\n", ctx->voltage_a,
                 ctx->voltage_b);
      CORE_DEBUG(ctx, "    期望: A<%dmV, B>%dmV\r\n",
                 VALVE_VOLTAGE_LOW_THRESHOLD, VALVE_VOLTAGE_HIGH_THRESHOLD);
      if (ctx->voltage_a > VALVE_VOLTAGE_HIGH_THRESHOLD &&
          ctx->voltage_b < VALVE_VOLTAGE_LOW_THRESHOLD) {
        CORE_DEBUG(ctx, "    提示: 电压AB可能定义反了!\r\n");
      }
      ctx->result = VT_TIMEOUT;
      ctx->fail_reason = VT_FAIL_CLOSE_DETECT_TIMEOUT;
      ctx->fail_step = VT_STEP_DETECT_CLOSING;
      ctx->enabled = 0;
      return VT_TIMEOUT;
    }
    break;
  }

  case VT_STEP_OUTPUT_CLOSE_SIGNAL:
    HAL_CALL(ctx, output_valve_position_signals, 0, 1);
    CORE_DEBUG(ctx, "  输出: 开阀到位=高电平, 关阀到位=低电平\r\n");
    CORE_DEBUG(ctx, "  等待%dms让水表检测信号...\r\n", VALVE_SIGNAL_DELAY_MS);
    HAL_CALL(ctx, set_soft_delay, VALVE_SIGNAL_DELAY_MS);
    enter_step(ctx, VT_STEP_CHECK_CLOSE_STATE, VALVE_STATE_CHECK_TIMEOUT_MS);
    break;

  case VT_STEP_CHECK_CLOSE_STATE: {
    ctx->voltage_a = HAL_CALL_RET(ctx, read_voltage_a, 0);
    ctx->voltage_b = HAL_CALL_RET(ctx, read_voltage_b, 0);

    CORE_DEBUG(ctx, "\r\n[验证] 🔍 检查关阀后状态\r\n");
    CORE_DEBUG(ctx, "  ┌────────────────────────────────────┐\r\n");
    CORE_DEBUG(ctx, "  │ 电压A: %4lumV (期望<%dmV)         │\r\n",
               ctx->voltage_a, VALVE_VOLTAGE_LOW_THRESHOLD);
    CORE_DEBUG(ctx, "  │ 电压B: %4lumV (期望<%dmV)         │\r\n",
               ctx->voltage_b, VALVE_VOLTAGE_LOW_THRESHOLD);
    CORE_DEBUG(ctx, "  └────────────────────────────────────┘\r\n");

    if (ctx->voltage_a < VALVE_VOLTAGE_LOW_THRESHOLD &&
        ctx->voltage_b < VALVE_VOLTAGE_LOW_THRESHOLD) {
      CORE_DEBUG(ctx, "  ✓ 关阀状态正常\r\n");
      HAL_CALL(ctx, output_valve_position_signals, 0, 0);
      enter_step(ctx, VT_STEP_EVALUATE, 1000);
    } else {
      CORE_DEBUG(ctx, "  ✗ 关阀状态异常!\r\n");
      if (ctx->voltage_a >= VALVE_VOLTAGE_LOW_THRESHOLD)
        CORE_DEBUG(ctx, "    - 电压A异常\r\n");
      if (ctx->voltage_b >= VALVE_VOLTAGE_LOW_THRESHOLD)
        CORE_DEBUG(ctx, "    - 电压B异常\r\n");
      if (++ctx->retry_count > ctx->retry_max) {
        ctx->result = VT_FAIL;
        ctx->fail_reason = VT_FAIL_CLOSE_STATE_CHECK;
        ctx->fail_step = VT_STEP_CHECK_CLOSE_STATE;
        ctx->enabled = 0;
        CORE_DEBUG(ctx, "  ✗ 重试次数已用尽\r\n");
        return VT_FAIL;
      }
      CORE_DEBUG(ctx, "  重试 %d/%d...\r\n", ctx->retry_count, ctx->retry_max);
      enter_step(ctx, VT_STEP_OUTPUT_CLOSE_SIGNAL, 5000);
    }
    break;
  }

    /*==================== 评估阶段 ====================*/

  case VT_STEP_EVALUATE:
    ctx->result = VT_SUCCESS;
    CORE_DEBUG(ctx, "\r\n");
    CORE_DEBUG(ctx, "========================================\r\n");
    CORE_DEBUG(ctx, "       ✓ ✓ ✓  阀门测试成功  ✓ ✓ ✓       \r\n");
    CORE_DEBUG(ctx, "========================================\r\n");
    CORE_DEBUG(ctx, "  总耗时: %lu.%lus\r\n", ctx->total_time_ms / 1000,
               (ctx->total_time_ms % 1000) / 100);
    CORE_DEBUG(ctx, "========================================\r\n");
    ctx->current_step = VT_STEP_DONE;
    break;

  case VT_STEP_DONE:
    ctx->enabled = 0;
    return ctx->result;

  default:
    break;
  }

  return VT_RUNNING;
}

void ValveCtrl_Core_OnResponse(ValveCtrl_Context_t *ctx,
                               uint16_t response_code) {
  if (ctx == NULL)
    return;

  CORE_DEBUG(ctx,
             "ValveCtrl_Core_OnResponse: ctx_en=%d, step=%d, code=0x%04X\r\n",
             ctx->enabled, ctx->current_step, response_code);
  ctx->response_received = 1;
  ctx->response_code = response_code;
}

/*============================================================================*/
/*                              状态查询函数 */
/*============================================================================*/

VT_TestResult ValveCtrl_Core_GetResult(const ValveCtrl_Context_t *ctx) {
  return ctx ? ctx->result : VT_IDLE;
}

VT_TestStep ValveCtrl_Core_GetStep(const ValveCtrl_Context_t *ctx) {
  return ctx ? ctx->current_step : VT_STEP_INIT;
}

VT_FailReason ValveCtrl_Core_GetFailReason(const ValveCtrl_Context_t *ctx) {
  return ctx ? ctx->fail_reason : VT_FAIL_NONE;
}

bool ValveCtrl_Core_IsRunning(const ValveCtrl_Context_t *ctx) {
  return ctx ? (ctx->enabled != 0) : false;
}

const char *ValveCtrl_Core_GetFailReasonStr(VT_FailReason reason) {
  switch (reason) {
  case VT_FAIL_NONE:
    return "无错误";
  case VT_FAIL_CONFIG_TIMEOUT:
    return "配置命令超时";
  case VT_FAIL_CONFIG_RETRY:
    return "配置命令重试超限";
  case VT_FAIL_QUERY_TIMEOUT:
    return "查询命令超时";
  case VT_FAIL_INITIAL_POS_OPEN:
    return "初始状态：开阀到位信号异常";
  case VT_FAIL_INITIAL_POS_CLOSE:
    return "初始状态：关阀到位信号异常";
  case VT_FAIL_INITIAL_VOLTAGE_A:
    return "初始状态：电压A异常(>=100mV)";
  case VT_FAIL_INITIAL_VOLTAGE_B:
    return "初始状态：电压B异常(>=100mV)";
  case VT_FAIL_INITIAL_RETRY:
    return "初始状态检查重试超限";
  case VT_FAIL_OPEN_CMD_TIMEOUT:
    return "开阀命令超时";
  case VT_FAIL_OPEN_DETECT_TIMEOUT:
    return "开阀动作检测超时";
  case VT_FAIL_OPEN_STATE_CHECK:
    return "开阀状态检查失败";
  case VT_FAIL_CLOSE_CMD_TIMEOUT:
    return "关阀命令超时";
  case VT_FAIL_CLOSE_DETECT_TIMEOUT:
    return "关阀动作检测超时";
  case VT_FAIL_CLOSE_STATE_CHECK:
    return "关阀状态检查失败";
  case VT_FAIL_TOTAL_TIMEOUT:
    return "总超时";
  default:
    return "未知错误";
  }
}

const char *ValveCtrl_Core_GetStepName(VT_TestStep step) {
  switch (step) {
  case VT_STEP_INIT:
    return "初始化";
  case VT_STEP_CONFIG:
    return "发送配置";
  case VT_STEP_QUERY_INITIAL:
    return "查询初始状态";
  case VT_STEP_CHECK_INITIAL:
    return "检查初始状态";
  case VT_STEP_SEND_OPEN:
    return "发送开阀命令";
  case VT_STEP_DETECT_OPENING:
    return "检测开阀动作";
  case VT_STEP_OUTPUT_OPEN_SIGNAL:
    return "输出开阀到位信号";
  case VT_STEP_QUERY_OPEN_STATE:
    return "查询开阀状态";
  case VT_STEP_CHECK_OPEN_STATE:
    return "检查开阀状态";
  case VT_STEP_SEND_CLOSE:
    return "发送关阀命令";
  case VT_STEP_DETECT_CLOSING:
    return "检测关阀动作";
  case VT_STEP_OUTPUT_CLOSE_SIGNAL:
    return "输出关阀到位信号";
  case VT_STEP_QUERY_CLOSE_STATE:
    return "查询关阀状态";
  case VT_STEP_CHECK_CLOSE_STATE:
    return "检查关阀状态";
  case VT_STEP_EVALUATE:
    return "评估结果";
  case VT_STEP_DONE:
    return "测试完成";
  default:
    return "未知步骤";
  }
}
