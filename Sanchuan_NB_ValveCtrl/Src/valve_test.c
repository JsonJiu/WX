/**
 * @file valve_test.c
 * @brief 阀门测试模块 - 状态机实现
 *
 * 架构说明:
 * - HAL层: 负责硬件交互（ADC读取、协议发送）
 * - Context: 保存所有状态（流程、时间、数据）
 * - Loop: 驱动状态机流程
 */

#include "valve_test.h"
#include "ADC_CHK.h"
#include "GPIO.h"
#include "Test_List.h"
#include "tongxin_xieyi_Ctrl.h"
#include "uart0.h"
#include <string.h>

/*============ 全局变量 ============*/

uint8_t enable_valve_test = 0;
ValveTestContext g_valve_ctx;
static const ValveTestHAL *g_hal = NULL;

/*============ 内部函数声明 ============*/

static VT_StepResult step_wait_response(ValveTestContext *ctx,
                                        uint16_t expected_code);
static VT_StepResult step_wait_response_with_retry(ValveTestContext *ctx,
                                                   uint16_t expected_code,
                                                   void (*resend_func)(void));
static void enter_step(ValveTestContext *ctx, VT_TestStep step,
                       uint32_t timeout_ms);

/*============ HAL 具体实现函数 ============*/

// 读取阀门电压A（DRV8837输出A）- 交换：famen_1 = A
static uint32_t hal_read_voltage_a(void) { return get_famen_1_dianya(); }

// 读取阀门电压B（DRV8837输出B）- 交换：famen_2 = B
static uint32_t hal_read_voltage_b(void) { return get_famen_2_dianya(); }

// 读取开阀到位（从协议数据获取）
// PA11 = 开阀到位信号 -> 对应水表的 daowei1
static uint8_t hal_read_pos_open(void) {
  return Test_linshi_cunchushuju_L.L_daowei1;
}

// 读取关阀到位（从协议数据获取）
// PA12 = 关阀到位信号 -> 对应水表的 daowei2
static uint8_t hal_read_pos_close(void) {
  return Test_linshi_cunchushuju_L.L_daowei2;
}

// 发送阀门配置命令（根据表类型发送上位机配置的参数）
static void hal_send_config(uint8_t param1, uint8_t param2) {
  (void)param1; // 未使用，参数来自上位机配置
  (void)param2;

  // 根据表类型发送对应配置
  if (Test_jiejuo_jilu.biaoju_leixing_famen == 0) {
    // 机械表：使用结构体配置发送 0x2604
    configure_mechanical_valve(&Test_jiejuo_jilu.mechanical_config);
  } else {
    // 超声波表：使用结构体配置发送 0x2036
    configure_ultrasonic_valve(&Test_jiejuo_jilu.ultrasonic_config);
  }
}

// 发送开阀命令
static void hal_send_open_valve(void) {
  // 发送 0xC022 开阀命令
  FM_Open_xieyi();
}

// 发送关阀命令
static void hal_send_close_valve(void) {
  // 发送 0xC022 关阀命令
  FM_Close_xieyi();
}

// 发送F003状态查询
static void hal_send_read_status(void) {
  // 发送F003协议查询状态
  Test_zhuanyong_xieyi();
}

// 输出到位信号给水表 (模拟开/关阀到位)
// 信号定义: ON=低电平=到位, OFF=高电平=未到位
// valve_open (PA11) = 开阀到位信号
// valve_close (PA12) = 关阀到位信号
static void hal_output_valve_position_signals(uint8_t open_signal,
                                              uint8_t close_signal) {
  // open_signal: 1=开阀到位(输出低电平), 0=未到位(输出高电平)
  // 开阀到位信号输出到 PA11
  if (open_signal) {
    valve_open_on(); // PA11输出低电平，表示开阀到位
  } else {
    valve_open_off(); // PA11输出高电平，表示开阀未到位
  }

  // close_signal: 1=关阀到位(输出低电平), 0=未到位(输出高电平)
  // 关阀到位信号输出到 PA12
  if (close_signal) {
    valve_close_on(); // PA12输出低电平，表示关阀到位
  } else {
    valve_close_off(); // PA12输出高电平，表示关阀未到位
  }

  DeBug_print("  GPIO输出: 开阀到位(PA11)=%s, 关阀到位(PA12)=%s\r\n",
              open_signal ? "低电平(到位)" : "高电平(未到位)",
              close_signal ? "低电平(到位)" : "高电平(未到位)");
}

// 重发配置命令（无参数包装，用于重试回调）
static void hal_resend_config(void) {
  if (Test_jiejuo_jilu.biaoju_leixing_famen == 0) {
    configure_mechanical_valve(&Test_jiejuo_jilu.mechanical_config);
  } else {
    configure_ultrasonic_valve(&Test_jiejuo_jilu.ultrasonic_config);
  }
}

// 调试输出
static void hal_debug_print(const char *msg) { DeBug_print((char *)msg); }

// 默认HAL实例
static const ValveTestHAL default_hal = {
    .read_voltage_a = hal_read_voltage_a,
    .read_voltage_b = hal_read_voltage_b,
    .read_pos_open = hal_read_pos_open,
    .read_pos_close = hal_read_pos_close,
    .send_config = hal_send_config,
    .send_open_valve = hal_send_open_valve,
    .send_close_valve = hal_send_close_valve,
    .send_read_status = hal_send_read_status,
    .output_valve_position_signals = hal_output_valve_position_signals,
    .debug_print = hal_debug_print,
};

/*============ 内部辅助函数 ============*/

/**
 * @brief 进入新步骤
 */
static void enter_step(ValveTestContext *ctx, VT_TestStep step,
                       uint32_t timeout_ms) {
  ctx->current_step = step;
  ctx->step_time_ms = 0;
  ctx->step_timeout_ms = timeout_ms;
  ctx->response_received = 0;
  ctx->retry_count = 0;
}

/**
 * @brief 进入新步骤并设置软件延时
 * @param ctx 测试上下文
 * @param step 目标步骤
 * @param timeout_ms 步骤超时时间
 * @param delay_ms 软件延时(给水表处理时间)
 * @note 暂未使用，保留供将来扩展
 */
#if 0 // 暂未使用，避免编译警告
static void enter_step_with_delay(ValveTestContext *ctx, VT_TestStep step,
                                  uint32_t timeout_ms, uint32_t delay_ms) {
  enter_step(ctx, step, timeout_ms);
  ctx->software_delay_ms = delay_ms;
}
#endif

/**
 * @brief 等待协议响应的通用逻辑
 * @note 超时直接失败，不重试
 * @note response_received: 0=未发送, 1=收到响应, 2=已发送等待中
 */
static VT_StepResult step_wait_response(ValveTestContext *ctx,
                                        uint16_t expected_code) {
  // response_received != 1 表示还没收到新响应（可能是0或2）
  if (ctx->response_received != 1) {
    // 未收到响应，检查超时
    if (ctx->step_time_ms >= ctx->step_timeout_ms) {
      DeBug_print(
          "等待响应超时: step_time=%lu, timeout=%lu, expected=0x%04X\r\n",
          ctx->step_time_ms, ctx->step_timeout_ms, expected_code);
      return VT_STEP_FAIL; // 超时直接失败
    }
    return VT_STEP_BUSY; // 继续等待
  }

  // 收到响应(response_received == 1)，检查是否匹配
  DeBug_print("收到响应: code=0x%04X, expected=0x%04X\r\n", ctx->response_code,
              expected_code);
  if (ctx->response_code == expected_code) {
    ctx->response_received = 0; // 成功后重置
    return VT_STEP_SUCCESS;
  }
  // 响应不匹配，重置为2继续等待下一个响应
  ctx->response_received = 2;
  return VT_STEP_MISMATCH;
}

/**
 * @brief 等待协议响应，响应不匹配时自动重发
 * @param ctx 测试上下文
 * @param expected_code 期望的响应码
 * @param resend_func 响应不匹配时调用的重发函数
 * @return VT_StepResult 步骤结果
 * @note 超时直接失败，只有响应不匹配才重发
 */
static VT_StepResult step_wait_response_with_retry(ValveTestContext *ctx,
                                                   uint16_t expected_code,
                                                   void (*resend_func)(void)) {
  VT_StepResult r = step_wait_response(ctx, expected_code);

  if (r == VT_STEP_MISMATCH && resend_func != NULL) {
    // 响应不匹配，重发
    if (++ctx->retry_count > ctx->retry_max) {
      // 重置重试计数器
      ctx->retry_count = 0;
      DeBug_print("重试次数超限，失败\r\n");
      return VT_STEP_FAIL;
    }
    resend_func();
    DeBug_print("响应不匹配，重发...\r\n");
    return VT_STEP_BUSY; // 继续等待
  }

  return r;
}

/**
 * @brief 当前初始
 *
 *
 * @param ctx
 * @param hal
 */
void valve_test_init(ValveTestContext *ctx, const ValveTestHAL *hal) {
  if (hal != NULL) {
    g_hal = hal;
  } else {
    g_hal = &default_hal;
  }

  memset(ctx, 0, sizeof(ValveTestContext));
  ctx->current_step = VT_STEP_INIT;
  ctx->result = VT_IDLE;
  ctx->enabled = 0;
  ctx->retry_max = 3;
  ctx->total_timeout_ms = 60000; // 60秒总超时
  ctx->config_param1 = 15;       // 默认时间15秒
  ctx->config_param2 = 230;      // 默认电压阈值230
  ctx->fail_reason = VT_FAIL_NONE;
  ctx->fail_step = VT_STEP_INIT;
}

/**
 * @brief 启动阀门测试
 */
void valve_test_start(ValveTestContext *ctx) {
  // 首先确保两个到位信号都是高电平（未到位），让水表知道阀门需要动作
  g_hal->output_valve_position_signals(0, 0); // open=0(高电平), close=0(高电平)

  ctx->enabled = 1;
  ctx->current_step = VT_STEP_INIT;
  ctx->result = VT_RUNNING;
  ctx->total_time_ms = 0;
  ctx->step_time_ms = 0;
  ctx->software_delay_ms = 0; // 清零软件延时
  ctx->retry_count = 0;
  ctx->response_received = 0;
  ctx->voltage_a = 0;
  ctx->voltage_b = 0;
  ctx->pos_open = 0;
  ctx->pos_close = 0;
  // 初始状态清零
  ctx->initial_voltage_a = 0;
  ctx->initial_voltage_b = 0;
  ctx->initial_pos_open = 0;
  ctx->initial_pos_close = 0;
  // 失败信息清零
  ctx->fail_reason = VT_FAIL_NONE;
  ctx->fail_step = VT_STEP_INIT;

  DeBug_print("\r\n");
  DeBug_print("========================================\r\n");
  DeBug_print("           阀 门 测 试 启 动           \r\n");
  DeBug_print("========================================\r\n");
  DeBug_print("表类型: %s\r\n", Test_jiejuo_jilu.biaoju_leixing_famen == 0
                                    ? "机械表"
                                    : "超声波表");
  DeBug_print("超时设置: %lu秒\r\n", ctx->total_timeout_ms / 1000);
  DeBug_print("----------------------------------------\r\n");
}

/**
 * @brief 停止阀门测试
 */
void valve_test_stop(ValveTestContext *ctx) {
  ctx->enabled = 0;
  ctx->result = VT_IDLE;
  ctx->current_step = VT_STEP_INIT;

  DeBug_print("阀门测试停止\r\n");
}

/**
 * @brief 主循环调用
 * @param ctx 测试上下文
 * @param tick_ms 调用间隔(毫秒)
 * @return 当前测试结果
 */
VT_TestResult valve_test_loop(ValveTestContext *ctx, uint32_t tick_ms) {
  // 如果测试未启用，直接返回空闲状态
  if (!ctx->enabled) {
    return VT_IDLE;
  }

  // 软件延时处理，使用全局延时变量（由定时器中断自动递减）
  // 延时期间不累加时间，直接返回
  if (Test_quanju_canshu_L.time_softdelay_ms > 0) {
    return VT_RUNNING; // 延时期间继续等待
  }

  // 更新时间（只在非延时期间累加）
  ctx->total_time_ms += tick_ms;
  ctx->step_time_ms += tick_ms;

  // 总超时检查
  if (ctx->total_time_ms > ctx->total_timeout_ms) {
    ctx->result = VT_TIMEOUT;
    ctx->fail_reason = VT_FAIL_TOTAL_TIMEOUT;
    ctx->fail_step = ctx->current_step;
    ctx->enabled = 0;
    DeBug_print("阀门测试总超时，当前步骤=%d\r\n", ctx->current_step);
    return VT_TIMEOUT;
  }

  switch (ctx->current_step) {

    /*==================== 初始化阶段 ====================*/

  case VT_STEP_INIT:
    // 初始化，进入配置步骤
    DeBug_print("\r\n[步骤1/9] 📤 发送配置命令\r\n");
    DeBug_print("  命令: %s\r\n", Test_jiejuo_jilu.biaoju_leixing_famen == 0
                                      ? "0x2604(机械表)"
                                      : "0x2036(超声波表)");
    DeBug_print("  等待响应中...\r\n");
    enter_step(ctx, VT_STEP_CONFIG, 10000);
    g_hal->send_config(ctx->config_param1, ctx->config_param2);
    break;

  case VT_STEP_CONFIG: {
    // 等待配置响应 (机械0x2604/超声0x2036)
    uint16_t expected_code =
        (Test_jiejuo_jilu.biaoju_leixing_famen == 0) ? 0x2604 : 0x2036;
    VT_StepResult r =
        step_wait_response_with_retry(ctx, expected_code, hal_resend_config);
    if (r == VT_STEP_SUCCESS) {
      DeBug_print("  ✓ 配置响应收到\r\n");
      // 发送Open到位指令低，close到位指令低，表示阀门未到位
      // g_hal->output_valve_position_signals(1, 0);
      enter_step(ctx, VT_STEP_CHECK_INITIAL,
                 5000); // 增加到8秒超时
    } else if (r == VT_STEP_FAIL) {
      ctx->result = VT_FAIL;
      ctx->fail_reason = VT_FAIL_CONFIG_RETRY;
      ctx->fail_step = VT_STEP_CONFIG;
      ctx->enabled = 0;
      DeBug_print("  ✗ 配置失败，重试超限\r\n");
      return VT_FAIL;
    }
    break;
  }

  case VT_STEP_CHECK_INITIAL: {
    // 检查初始状态: open=1(高电平未到位), close=1(高电平未到位), A<100, B<100
    // 到位信号: 0=低电平到位, 1=高电平未到位
    ctx->voltage_a = g_hal->read_voltage_a();
    ctx->voltage_b = g_hal->read_voltage_b();

    // 打印当前状态
    DeBug_print("\r\n[步骤2/9] 🔍 检查初始状态\r\n");
    DeBug_print("  ┌────────────────────────────────────┐\r\n");
    DeBug_print("  │ 电压A: %4lumV (期望<100mV)         │\r\n", ctx->voltage_a);
    DeBug_print("  │ 电压B: %4lumV (期望<100mV)         │\r\n", ctx->voltage_b);
    DeBug_print("  └────────────────────────────────────┘\r\n");

    // 初始状态: 两个到位信号都应该是1(高电平未到位)，电压都应该<100mV
    if (ctx->voltage_a < 100 && ctx->voltage_b < 100) {
      DeBug_print("  ✓ 初始状态正常\r\n");
      DeBug_print("\r\n[步骤3/9] 📤 发送开阀命令 (0xC022)\r\n");
      DeBug_print("  等待响应中...\r\n");
      enter_step(ctx, VT_STEP_SEND_OPEN, 5000);
      g_hal->send_open_valve();
    } else {
      // 详细打印异常原因
      DeBug_print(">> ⚠️ 初始状态异常:\r\n");
      if (ctx->voltage_a >= 100) {
        DeBug_print("   - 电压A异常: 期望<100mV, 实际=%lumV\r\n",
                    ctx->voltage_a);
      }
      if (ctx->voltage_b >= 100) {
        DeBug_print("   - 电压B异常: 期望<100mV, 实际=%lumV\r\n",
                    ctx->voltage_b);
      }
      // 重试查询
      if (++ctx->retry_count > ctx->retry_max) {
        DeBug_print(">> ❌ 初始状态检查失败，重试次数已用尽\r\n");
        ctx->result = VT_FAIL;
        ctx->fail_step = VT_STEP_CHECK_INITIAL;
        // 记录第一个发现的异常作为失败原因
        if (ctx->voltage_a >= 100) {
          ctx->fail_reason = VT_FAIL_INITIAL_VOLTAGE_A;
        } else if (ctx->voltage_b >= 100) {
          ctx->fail_reason = VT_FAIL_INITIAL_VOLTAGE_B;
        } else {
          ctx->fail_reason = VT_FAIL_INITIAL_RETRY;
        }
        ctx->enabled = 0;
        return VT_FAIL;
      }
      DeBug_print("重试 %d/%d...\r\n", ctx->retry_count, ctx->retry_max);
      enter_step(ctx, VT_STEP_CONFIG, 5000);
    }
    break;
  }

    /*==================== 开阀测试阶段 ====================*/

  case VT_STEP_SEND_OPEN: {
    // 等待开阀响应 (0xC022)
    VT_StepResult r =
        step_wait_response_with_retry(ctx, 0xC022, hal_send_open_valve);
    if (r == VT_STEP_SUCCESS) {
      DeBug_print("  ✓ 开阀命令响应收到\r\n");
      DeBug_print("\r\n[步骤4/9] ⚡ 检测开阀动作\r\n");
      DeBug_print("  期望: 电压A>2800mV, 电压B<100mV\r\n");
      DeBug_print("  超时: 5秒\r\n");
      enter_step(ctx, VT_STEP_DETECT_OPENING, 5000); // 5秒检测开阀
    } else if (r == VT_STEP_FAIL) {
      if (ctx->retry_count >= ctx->retry_max) {
        DeBug_print("  ✗ 开阀命令重试超限\r\n");
        ctx->result = VT_FAIL;
        ctx->fail_reason = VT_FAIL_OPEN_CMD_TIMEOUT;
        ctx->fail_step = VT_STEP_SEND_OPEN;
        ctx->enabled = 0;
        DeBug_print("  ✗ 开阀命令发送失败\r\n");
        return VT_FAIL;
      }
      DeBug_print("  ✗ 开阀命令响应超时，重发...\r\n");
      enter_step(ctx, VT_STEP_SEND_OPEN, 5000);
    }
    break;
  }

  case VT_STEP_DETECT_OPENING: {
    // 检测开阀动作: A>2800, B<100
    ctx->voltage_a = g_hal->read_voltage_a();
    ctx->voltage_b = g_hal->read_voltage_b();

    // 每秒打印一次当前状态
    if (ctx->step_time_ms % 1000 < tick_ms) {
      DeBug_print("  [%2lus] A=%4lumV, B=%4lumV\r\n", ctx->step_time_ms / 1000,
                  ctx->voltage_a, ctx->voltage_b);
    }

    if (ctx->voltage_a > 2800 && ctx->voltage_b < 100) {
      DeBug_print("  ✓ 检测到开阀动作! A=%lumV, B=%lumV\r\n", ctx->voltage_a,
                  ctx->voltage_b);
      DeBug_print("\r\n[步骤5/9] 📍 输出开阀到位信号\r\n");
      enter_step(ctx, VT_STEP_OUTPUT_OPEN_SIGNAL, 1000);
    } else if (ctx->step_time_ms >= ctx->step_timeout_ms) {
      DeBug_print("\r\n  ✗ 开阀动作检测超时!\r\n");
      DeBug_print("    最终状态: A=%lumV, B=%lumV\r\n", ctx->voltage_a,
                  ctx->voltage_b);
      DeBug_print("    期望: A>2800mV, B<100mV\r\n");
      if (ctx->voltage_a < 100 && ctx->voltage_b > 2800) {
        DeBug_print("    提示: 电压AB可能定义反了!\r\n");
      }
      ctx->result = VT_TIMEOUT;
      ctx->fail_reason = VT_FAIL_OPEN_DETECT_TIMEOUT;
      ctx->fail_step = VT_STEP_DETECT_OPENING;
      ctx->enabled = 1;
      // return VT_TIMEOUT;
      // 重新回到开阀检测步骤，继续等待
      DeBug_print("    重新发送开阀命令...\r\n");
      enter_step(ctx, VT_STEP_SEND_OPEN, 15000);
      g_hal->send_open_valve(); // 重新发送开阀命令
    }
    break;
  }

  case VT_STEP_OUTPUT_OPEN_SIGNAL: {
    // 输出开阀到位信号: open=1, close=0
    g_hal->output_valve_position_signals(1, 0);
    DeBug_print("  输出: 开阀到位=低电平, 关阀到位=高电平\r\n");
    DeBug_print("  等待500ms让水表检测信号...\r\n");
    // 设置软件延时（由定时器中断自动递减）
    Test_quanju_canshu_L.time_softdelay_ms = 500;
    // 不再发送F003查询，直接进入电压检查步骤
    enter_step(ctx, VT_STEP_CHECK_OPEN_STATE, 5000);
    break;
  }

  case VT_STEP_CHECK_OPEN_STATE: {
    // 检查开阀后状态: 只验证电压 A<100, B<100
    ctx->voltage_a = g_hal->read_voltage_a();
    ctx->voltage_b = g_hal->read_voltage_b();

    DeBug_print("\r\n[步骤6/9] 🔍 检查开阀后状态\r\n");
    DeBug_print("  ┌────────────────────────────────────┐\r\n");
    DeBug_print("  │ 电压A: %4lumV (期望<100mV)         │\r\n", ctx->voltage_a);
    DeBug_print("  │ 电压B: %4lumV (期望<100mV)         │\r\n", ctx->voltage_b);
    DeBug_print("  └────────────────────────────────────┘\r\n");

    // 开阀后: 电压都应该<100mV(电机停止)
    if (ctx->voltage_a < 100 && ctx->voltage_b < 100) {
      DeBug_print("  ✓ 开阀状态正常\r\n");
      DeBug_print("\r\n[步骤7/9] 📤 发送关阀命令 (0xC022)\r\n");

      // 开阀到位之后,恢复开阀到位信号为高电平(未到位)，然后发指令关阀
      g_hal->output_valve_position_signals(
          0, 0); // open=0(高电平), close=0(高电平)
      DeBug_print("  恢复: 开阀到位=高电平(未到位)\r\n");
      DeBug_print("  等待响应中...\r\n");

      enter_step(ctx, VT_STEP_SEND_CLOSE, 5000);
      g_hal->send_close_valve();
    } else {
      DeBug_print("  ✗ 开阀状态异常!\r\n");

      if (ctx->voltage_a >= 100)
        DeBug_print("    - 电压A异常\r\n");
      if (ctx->voltage_b >= 100)
        DeBug_print("    - 电压B异常\r\n");
      if (++ctx->retry_count > ctx->retry_max) {
        ctx->result = VT_FAIL;
        ctx->fail_reason = VT_FAIL_OPEN_STATE_CHECK;
        ctx->fail_step = VT_STEP_CHECK_OPEN_STATE;
        ctx->enabled = 0;
        DeBug_print("  ✗ 重试次数已用尽\r\n");
        return VT_FAIL;
      }
      DeBug_print("  重试 %d/%d...\r\n", ctx->retry_count, ctx->retry_max);
      enter_step(ctx, VT_STEP_OUTPUT_OPEN_SIGNAL, 10000);
    }
    break;
  }

    /*==================== 关阀测试阶段 ====================*/

  case VT_STEP_SEND_CLOSE: {
    // 等待关阀响应 (0xC022)
    VT_StepResult r =
        step_wait_response_with_retry(ctx, 0xC022, hal_send_close_valve);
    if (r == VT_STEP_SUCCESS) {
      DeBug_print("  ✓ 关阀命令响应收到\r\n");
      DeBug_print("\r\n[步骤8/9] ⚡ 检测关阀动作(反转)\r\n");
      DeBug_print("  期望: 电压A<100mV, 电压B>2800mV\r\n");
      DeBug_print("  超时: 15秒\r\n");
      enter_step(ctx, VT_STEP_DETECT_CLOSING, 15000); // 15秒检测关阀
    } else if (r == VT_STEP_FAIL) {
      ctx->result = VT_FAIL;
      ctx->fail_reason = VT_FAIL_CLOSE_CMD_TIMEOUT;
      ctx->fail_step = VT_STEP_SEND_CLOSE;
      ctx->enabled = 0;
      DeBug_print("  ✗ 关阀命令发送失败\r\n");
      return VT_FAIL;
    }
    break;
  }

  case VT_STEP_DETECT_CLOSING: {
    // 检测关阀动作 (反转): A<100, B>2800
    ctx->voltage_a = g_hal->read_voltage_a();
    ctx->voltage_b = g_hal->read_voltage_b();

    // 每秒打印一次当前状态
    if (ctx->step_time_ms % 1000 < tick_ms) {
      DeBug_print("  [%2lus] A=%4lumV, B=%4lumV\r\n", ctx->step_time_ms / 1000,
                  ctx->voltage_a, ctx->voltage_b);
    }

    if (ctx->voltage_a < 100 && ctx->voltage_b > 2800) {
      DeBug_print("  ✓ 检测到关阀反转! A=%lumV, B=%lumV\r\n", ctx->voltage_a,
                  ctx->voltage_b);
      DeBug_print("\r\n[步骤9/9] 📍 输出关阀到位信号\r\n");
      enter_step(ctx, VT_STEP_OUTPUT_CLOSE_SIGNAL, 1000);
    } else if (ctx->step_time_ms >= ctx->step_timeout_ms) {
      DeBug_print("\r\n  ✗ 关阀动作检测超时!\r\n");
      DeBug_print("    最终状态: A=%lumV, B=%lumV\r\n", ctx->voltage_a,
                  ctx->voltage_b);
      DeBug_print("    期望: A<100mV, B>2800mV\r\n");
      if (ctx->voltage_a > 2800 && ctx->voltage_b < 100) {
        DeBug_print("    提示: 电压AB可能定义反了!\r\n");
      }
      ctx->result = VT_TIMEOUT;
      ctx->fail_reason = VT_FAIL_CLOSE_DETECT_TIMEOUT;
      ctx->fail_step = VT_STEP_DETECT_CLOSING;
      ctx->enabled = 0;
      return VT_TIMEOUT;
    }
    break;
  }

  case VT_STEP_OUTPUT_CLOSE_SIGNAL: {
    // 输出关阀到位信号: open=0, close=1
    g_hal->output_valve_position_signals(0, 1);
    DeBug_print("  输出: 开阀到位=高电平, 关阀到位=低电平\r\n");
    DeBug_print("  等待500ms让水表检测信号...\r\n");
    // 设置软件延时（由定时器中断自动递减）
    Test_quanju_canshu_L.time_softdelay_ms = 500;
    // 不再发送F003查询，直接进入电压检查步骤
    enter_step(ctx, VT_STEP_CHECK_CLOSE_STATE, 5000);
    break;
  }

  case VT_STEP_CHECK_CLOSE_STATE: {
    // 检查关阀后状态: 只验证电压 A<100, B<100
    ctx->voltage_a = g_hal->read_voltage_a();
    ctx->voltage_b = g_hal->read_voltage_b();

    DeBug_print("\r\n[验证] 🔍 检查关阀后状态\r\n");
    DeBug_print("  ┌────────────────────────────────────┐\r\n");
    DeBug_print("  │ 电压A: %4lumV (期望<100mV)         │\r\n", ctx->voltage_a);
    DeBug_print("  │ 电压B: %4lumV (期望<100mV)         │\r\n", ctx->voltage_b);
    DeBug_print("  └────────────────────────────────────┘\r\n");

    // 关阀后: 电压都应该<100mV(电机停止)
    if (ctx->voltage_a < 100 && ctx->voltage_b < 100) {
      DeBug_print("  ✓ 关阀状态正常\r\n");
      // 全部测试通过，关闭close到位
      g_hal->output_valve_position_signals(
          0, 0); // open=0(高电平), close=0(高电平)
      enter_step(ctx, VT_STEP_EVALUATE, 1000);
    } else {
      DeBug_print("  ✗ 关阀状态异常!\r\n");

      if (ctx->voltage_a >= 100)
        DeBug_print("    - 电压A异常\r\n");
      if (ctx->voltage_b >= 100)
        DeBug_print("    - 电压B异常\r\n");
      if (++ctx->retry_count > ctx->retry_max) {
        ctx->result = VT_FAIL;
        ctx->fail_reason = VT_FAIL_CLOSE_STATE_CHECK;
        ctx->fail_step = VT_STEP_CHECK_CLOSE_STATE;
        ctx->enabled = 0;
        DeBug_print("  ✗ 重试次数已用尽\r\n");
        return VT_FAIL;
      }
      DeBug_print("  重试 %d/%d...\r\n", ctx->retry_count, ctx->retry_max);
      // 重试时重新输出到位信号并等待
      enter_step(ctx, VT_STEP_OUTPUT_CLOSE_SIGNAL, 5000);
    }
    break;
  }

    /*==================== 评估阶段 ====================*/

  case VT_STEP_EVALUATE:
    // 所有测试通过
    ctx->result = VT_SUCCESS;
    DeBug_print("\r\n");
    DeBug_print("========================================\r\n");
    DeBug_print("       ✓ ✓ ✓  阀门测试成功  ✓ ✓ ✓       \r\n");
    DeBug_print("========================================\r\n");
    DeBug_print("  总耗时: %lu.%lus\r\n", ctx->total_time_ms / 1000,
                (ctx->total_time_ms % 1000) / 100);
    DeBug_print("========================================\r\n");
    ctx->current_step = VT_STEP_DONE;
    break;

  case VT_STEP_DONE:
    // 测试完成
    ctx->enabled = 0;
    return ctx->result;

  default:
    break;
  }

  return VT_RUNNING;
}

/**
 * @brief 协议层收到响应时调用
 */
void valve_test_on_response(ValveTestContext *ctx, uint16_t response_code) {
  DeBug_print("valve_test_on_response: global_en=%d, ctx_en=%d, step=%d, "
              "code=0x%04X\r\n",
              enable_valve_test, ctx->enabled, ctx->current_step,
              response_code);
  ctx->response_received = 1;
  ctx->response_code = response_code;
}

/**
 * @brief 获取当前测试结果
 */
VT_TestResult valve_test_get_result(ValveTestContext *ctx) {
  return ctx->result;
}

/**
 * @brief 获取当前步骤
 */
VT_TestStep valve_test_get_step(ValveTestContext *ctx) {
  return ctx->current_step;
}

/**
 * @brief 获取失败原因
 */
VT_FailReason valve_test_get_fail_reason(ValveTestContext *ctx) {
  return ctx->fail_reason;
}

/**
 * @brief 获取失败原因描述字符串
 */
const char *valve_test_get_fail_reason_str(VT_FailReason reason) {
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

/**
 * @brief 获取步骤名称字符串
 */
const char *valve_test_get_step_name(VT_TestStep step) {
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
