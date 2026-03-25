/**
 * @file valve_ctrl_port.c
 * @brief 阀门控制组件 - 适配层实现 (胶水代码)
 * @version 2.0.0
 * @date 2024-12
 *
 * 本文件是唯一"脏"的地方，负责连接 Core 层和具体硬件实现。
 * 如果要移植到其他项目，只需要修改这个文件。
 */

#define LOG_TAG "valve"
#include "valve_ctrl_port.h"
#include "valve_ctrl_core.h"
#include <elog.h>

/*============================================================================*/
/*                      引用原有驱动 (项目特定依赖) */
/*============================================================================*/

#include "ADC_CHK.h"
#include "GPIO.h"
#include "Test_List.h"
#include "tongxin_xieyi_Ctrl.h"
#include "uart0.h"

#include <stdarg.h>
#include <stdio.h>

/*============================================================================*/
/*                              全局变量 */
/*============================================================================*/

/** 阀门测试使能标志 (兼容旧代码) - 多处读写需要volatile */
volatile uint8_t enable_valve_test = 0;

/*============================================================================*/
/*                              内部变量 */
/*============================================================================*/

/** 全局 Context 实例 */
static ValveCtrl_Context_t g_valve_ctx;

/** HAL 实例 (前向声明，定义在后面) */
static const ValveCtrl_HAL_t g_valve_hal;

/*============================================================================*/
/*                          HAL 接口实现函数 */
/*============================================================================*/

/* ========== 读取接口 ========== */

static uint32_t port_read_voltage_a(void) { return get_famen_1_dianya(); }

static uint32_t port_read_voltage_b(void) { return get_famen_2_dianya(); }

/* 读取开阀到位 PA3 = daowei1 */
static uint8_t port_read_pos_open(void) {
  return Test_linshi_cunchushuju_L.L_daowei1;
}

/* 读取关阀到位 PA11 = daowei2 */
static uint8_t port_read_pos_close(void) {
  return Test_linshi_cunchushuju_L.L_daowei2;
}

/* ========== 协议发送接口 ========== */

static void port_send_config(void) {
  if (Test_jiejuo_jilu.biaoju_leixing_famen == 0) {
    /* 机械表：发送 0x2604 */
    configure_mechanical_valve(&Test_jiejuo_jilu.mechanical_config);
  } else {
    /* 超声波表：发送 0x2036 */
    configure_ultrasonic_valve(&Test_jiejuo_jilu.ultrasonic_config);
  }
}

static void port_send_open_valve(void) { FM_Open_xieyi(); }

static void port_send_close_valve(void) { FM_Close_xieyi(); }

static void port_send_read_status(void) { Test_zhuanyong_xieyi(); }

/* ========== GPIO输出接口 ========== */

static void port_output_valve_position_signals(uint8_t open_signal,
                                               uint8_t close_signal) {
  /* open_signal: 1=开阀到位(输出低电平), 0=未到位(输出高电平)
   * 开阀到位信号输出到 PA3 (daowei_1) */
  if (open_signal) {
    daowei_1_ON(); /* PA3输出低电平，表示开阀到位 */
  } else {
    daowei_1_OFF(); /* PA3输出高电平，表示开阀未到位 */
  }

  /* close_signal: 1=关阀到位(输出低电平), 0=未到位(输出高电平)
   * 关阀到位信号输出到 PA11 (daowei_2) */
  if (close_signal) {
    daowei_2_ON(); /* PA11输出低电平，表示关阀到位 */
  } else {
    daowei_2_OFF(); /* PA11输出高电平，表示关阀未到位 */
  }

  log_d("  GPIO输出: 开阀到位(PA11)=%s, 关阀到位(PA3)=%s",
        open_signal ? "低电平(到位)" : "高电平(未到位)",
        close_signal ? "低电平(到位)" : "高电平(未到位)");
}

/**
 * @brief 恢复 GPIO 为输入模式
 *
 * 防止测试结束后 GPIO 保持输出状态，与外部设备产生电平冲突
 * @note 当前项目硬件无此功能，仅输出高电平(未到位状态)释放控制
 */
static void port_restore_gpio_to_input(void) {
  /* 当前项目没有 GPIO 模式切换函数，通过输出高电平释放到位信号 */
  daowei_2_OFF(); /* PA11输出高电平，释放开阀到位信号 */
  daowei_1_OFF(); /* PA3输出高电平，释放关阀到位信号 */
  log_d("  GPIO释放: PA11/PA3 输出高电平(未到位)");
}

/* ========== 系统接口 ========== */

static uint32_t port_get_tick_ms(void) {
  /* 如果有系统 tick，可以返回它。这里假设 Core_Loop 的 tick_ms
   * 参数已经提供时间增量 */
  return 0; /* 此项目中 Core 层使用传入的 tick_ms 累加，不需要系统时钟 */
}

static void port_set_soft_delay(uint32_t ms) {
  Test_quanju_canshu_L.time_softdelay_ms = ms;
}

static bool port_is_soft_delay_done(void) {
  return (Test_quanju_canshu_L.time_softdelay_ms == 0);
}

/* ========== 配置查询接口 ========== */

static ValveMeterType port_get_meter_type(void) {
  return (Test_jiejuo_jilu.biaoju_leixing_famen == 0) ? VALVE_METER_MECHANICAL
                                                      : VALVE_METER_ULTRASONIC;
}

static uint16_t port_get_expected_config_code(void) {
  return (Test_jiejuo_jilu.biaoju_leixing_famen == 0) ? 0x2604 : 0x2036;
}

/* ========== 调试接口 ========== */

static void port_debug_print(const char *fmt, ...) {
  va_list args;
  va_start(args, fmt);

  /* 使用 vsnprintf 格式化，然后调用 log_d */
  char buffer[256];
  vsnprintf(buffer, sizeof(buffer), fmt, args);
  log_d("%s", buffer);

  va_end(args);
}

/*============================================================================*/
/*                              HAL 实例定义 */
/*============================================================================*/

static const ValveCtrl_HAL_t g_valve_hal = {
    /* 读取接口 */
    .read_voltage_a = port_read_voltage_a,
    .read_voltage_b = port_read_voltage_b,
    .read_pos_open = port_read_pos_open,
    .read_pos_close = port_read_pos_close,

    /* 协议发送接口 */
    .send_config = port_send_config,
    .send_open_valve = port_send_open_valve,
    .send_close_valve = port_send_close_valve,
    .send_read_status = port_send_read_status,

    /* GPIO输出接口 */
    .output_valve_position_signals = port_output_valve_position_signals,
    .restore_gpio_to_input = port_restore_gpio_to_input,

    /* 系统接口 */
    .get_tick_ms = port_get_tick_ms,
    .set_soft_delay = port_set_soft_delay,
    .is_soft_delay_done = port_is_soft_delay_done,

    /* 配置查询接口 */
    .get_meter_type = port_get_meter_type,
    .get_expected_config_code = port_get_expected_config_code,

    /* 调试接口 */
    .debug_print = port_debug_print,
};

/*============================================================================*/
/*                          对外暴露的高级 API 实现 */
/*============================================================================*/

void ValveCtrl_Init(void) { ValveCtrl_Core_Init(&g_valve_ctx, &g_valve_hal); }

void ValveCtrl_Start(void) { ValveCtrl_Core_Start(&g_valve_ctx); }

void ValveCtrl_Stop(void) { ValveCtrl_Core_Stop(&g_valve_ctx); }

VT_TestResult ValveCtrl_Process(uint32_t tick_ms) {
  return ValveCtrl_Core_Loop(&g_valve_ctx, tick_ms);
}

void ValveCtrl_OnResponse(uint16_t response_code) {
  ValveCtrl_Core_OnResponse(&g_valve_ctx, response_code);
}

/*============================================================================*/
/*                          状态查询接口实现 */
/*============================================================================*/

VT_TestResult ValveCtrl_GetResult(void) {
  return ValveCtrl_Core_GetResult(&g_valve_ctx);
}

VT_TestStep ValveCtrl_GetStep(void) {
  return ValveCtrl_Core_GetStep(&g_valve_ctx);
}

VT_FailReason ValveCtrl_GetFailReason(void) {
  return ValveCtrl_Core_GetFailReason(&g_valve_ctx);
}

const char *ValveCtrl_GetFailReasonStr(void) {
  return ValveCtrl_Core_GetFailReasonStr(g_valve_ctx.fail_reason);
}

const char *ValveCtrl_GetStepName(void) {
  return ValveCtrl_Core_GetStepName(g_valve_ctx.current_step);
}

bool ValveCtrl_IsRunning(void) {
  return ValveCtrl_Core_IsRunning(&g_valve_ctx);
}

/*============================================================================*/
/*                          内部 Context 访问 */
/*============================================================================*/

ValveCtrl_Context_t *ValveCtrl_GetContext(void) { return &g_valve_ctx; }

const ValveCtrl_HAL_t *ValveCtrl_GetHAL(void) { return &g_valve_hal; }
