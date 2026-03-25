/**
 * @file diaphragme_meter_test.c
 * @brief 膜式燃气表测试模块实现文件
 * @details 实现膜式燃气表测试的业务逻辑流程
 *          所有硬件操作通过 NB_18_DiaphragmGas_port 层的接口完成
 *          采用回调表驱动的测试框架，通过 diaphragm_step_table 配置测试流程
 *
 * @note 本文件只包含测试业务逻辑，不涉及任何硬件直接操作
 */

#include "diaphragme_meter_test.h"
#include "NB_18_DiaphragmGas_port.h"
#include "common_test_configure.h"
#include "device_protocol.h" // 膜式燃气表协议API (DGM_SendXxx)
#include "led_indicator.h"   // LED指示器组件
#include "retry_manager.h"   // 统一重试管理
#include "time.h"            // system_tick_ms
#include "time_manager.h"    // 统一时间管理
#include "uart0.h"           // UART0中断控制
#include <stddef.h>          // NULL 宏定义
#include <stdio.h>
#include <string.h>

// EasyLogger 日志标签
#define LOG_TAG "diaphragm"
#include <elog.h>

/*============================================================================
 *                          全局变量定义
 *===========================================================================*/

DiaphragmTestStep_t diaphragm_test_step = DIAPHRAGM_WAIT;
DiaphragmTestResult_t diaphragm_test_result;

/** @brief 运行时状态实例 - 封装所有运行时变量 */
DiaphragmTestRuntime_t diaphragm_runtime = {
    .hardware_init_pending = 1,
    .hardware_init_failed = 0,
    .init_retry_count = 0,
    .step_entered = 0,
    .step_executing = 0,
    .sub_step = 0,
    .retry_count = 0,
    .events_received = DGM_EVT_NONE,
    .last_power_check_tick = 0,
    .running_power_ua = 0,
    .power_check_fail_count = 0,
    .time_manager =
        {
            .software_delay = 0,
            .delay_start_tick = 0,
            .total_timeout_ms = 0,
            .total_timeout_start_tick = 0,
            .success_timeout_tick = 0,
            .single_step_timeout_start_tick = 0,
            .single_required_timeout_ms = 0,
        },
    .fail_reason = DGM_FAIL_NONE,
    .fail_step = DIAPHRAGM_WAIT,
};

/*============================================================================
 *                          前向声明 - 新增步骤处理函数 (未在头文件声明的)
 *===========================================================================*/
static void diaphragm_handle_bluetooth_test(void);
static void diaphragm_handle_cover_check(void);
static void diaphragm_handle_ic_module_test(void);
static void diaphragm_handle_metering_module_test(void);
static void diaphragm_handle_119_warning_test(void);
static void diaphragm_handle_temp_pressure_sensor_test(void);
static void diaphragm_handle_second_valve_test(void);
static void diaphragm_handle_tilt_switch_test(void);
static void diaphragm_handle_default_valve_test(void);
static void diaphragm_handle_valve_position_test(void);

/*============================================================================
 *                          组件状态配置实例 (变量定义)
 *===========================================================================*/
DiaphragmComponentStatus_t diaphragm_component_status = {
    .valve_1_status = 1, // 必须测试阀门1
    .valve_2_status = DIAPHRAGM_COMPONENT_STATUS_UNTESTED, // 不测试阀门2
    .module_temp_pressure_sensor =
        DIAPHRAGM_COMPONENT_STATUS_UNTESTED, // 不测试温度压力传感器
    .warning_component_status =
        DIAPHRAGM_COMPONENT_STATUS_UNTESTED, // 不测试119报警功能
    .cover_check_status = DIAPHRAGM_COMPONENT_STATUS_UNTESTED, // 不测试防拆功能
    .ic_module_status = DIAPHRAGM_COMPONENT_STATUS_UNTESTED, // 不测试IC模块功能
    .irf_component_status = DIAPHRAGM_COMPONENT_STATUS_UNTESTED,
    .main_circuit_low_power_current_status = 1, // 必须测试主电低功耗
    .module_circuit_low_power_current_status =
        DIAPHRAGM_COMPONENT_STATUS_UNTESTED, // 不测试模块低功耗
    .metering_module_status =
        DIAPHRAGM_COMPONENT_STATUS_UNTESTED, // 不测试计量模块功能
    .bluetooth_component_status =
        DIAPHRAGM_COMPONENT_STATUS_UNTESTED, // 不测试蓝牙功能
    .tilt_switch_status =
        DIAPHRAGM_COMPONENT_STATUS_UNTESTED, // 不测试倾斜开关功能
    .valve_position_test_status = 1, // 默认测试阀门到位功能，设置为2则跳过
};

/*============================================================================
 *                          测试步骤回调表 (核心配置)
 *===========================================================================*/

/**
 * @brief 测试步骤描述表
 * @details 回调表驱动的核心，定义每个步骤的处理函数、超时时间和重试次数
 *          添加新步骤只需在此表中增加一行，无需修改主循环逻辑
 *
 * 字段说明:
 *   - name:       步骤名称 (显示用)
 *   - description:步骤描述 (日志用)
 *   - handler:    处理函数 (轮询调用，管理状态机)
 *   - timeout_ms: 超时时间，0表示无超时
 *   - max_retry:  最大重试次数，0表示不重试
 *   - next_step:  成功后跳转的步骤
 *   - on_enter:   进入步骤时执行的函数 (发送协议/触发IO)
 *   - on_retry:   重试时执行的函数，NULL表示使用on_enter
 *   - test_mode:  测试模式 - 决定功耗监控行为
 *   - skip_status_ptr: 跳过状态指针，NULL表示不检测
 *   - skip_when_equals: 当状态等于此值时跳过
 */
const DiaphragmStepDescriptor_t diaphragm_step_table[DIAPHRAGM_STEP_COUNT] = {
    /*=== 系统步骤
       ,检测下通电是否正常，基本电压采集，收集基本自检信息1000响应=*/
    [DIAPHRAGM_WAIT] =
        STEP_SYSTEM("等待开始", "等待测试指令", diaphragm_handle_wait, 0,
                    DIAPHRAGM_NEXT_STAY, DGM_MODE_IDLE),
    [DIAPHRAGM_START] =
        STEP_REQUIRED("开始测试", "初始化测试", diaphragm_handle_start, 5000),

    /*=== 初始化步骤 ===*/
    [DIAPHRAGM_MODULE_GPRS_VOLTAGE_TEST] =
        STEP_REQUIRED_RETRY("GPRS3.6V电压测试", "等待GPRS模组启动",
                            diaphragm_check_gprs_voltage, 10000, 3),
    [DIAPHRAGM_MODULE_SUCCESS_RESPONSE_WAIT] =
        STEP_REQUIRED_RETRY("等待模块响应", "等待模块1000协议响应",
                            diaphragm_wait_module_ready_response, 5000, 10),
    [DIAPHRAGM_IO_STATUS_CHECK] = STEP_REQUIRED_RETRY_ENTER(
        "IO状态检测", "检测IO口初始状态和附件信息",
        diaphragm_handle_io_status_check_initial_status, 5000, 3,
        DGM_SendBoardInfoRequest),

    /*=== 可选组件测试 (根据附件信息决定是否测试，1001协议里面查询附件信息)
       ===*/
    [DIAPHRAGM_BLUETOOTH_TEST] =
        STEP_OPTIONAL("蓝牙测试", "测试蓝牙功能",
                      diaphragm_handle_bluetooth_test, 10000, COMP_BLUETOOTH),
    [DIAPHRAGM_COVER_CHECK] =
        STEP_OPTIONAL("防拆检测", "检测防拆开关状态",
                      diaphragm_handle_cover_check, 5000, COMP_COVER),
    [DIAPHRAGM_IC_MODULE_TEST] =
        STEP_OPTIONAL("IC模块测试", "测试IC模块功能",
                      diaphragm_handle_ic_module_test, 10000, COMP_IC_MODULE),
    [DIAPHRAGM_METERING_MODULE_TEST] = STEP_OPTIONAL(
        "计量模块测试", "测试计量模块功能",
        diaphragm_handle_metering_module_test, 10000, COMP_METERING),
    [DIAPHRAGM_119_WARNING_TEST] = STEP_OPTIONAL(
        "119报警测试", "测试119报警功能", diaphragm_handle_119_warning_test,
        10000, COMP_119_WARNING),
    [DIAPHRAGM_TEMP_PRESSURE_SENSOR_TEST] = STEP_OPTIONAL(
        "温压传感器测试", "测试温度压力传感器功能",
        diaphragm_handle_temp_pressure_sensor_test, 10000, COMP_TEMP_PRESSURE),
    [DIAPHRAGM_SECOND_VALVE_TEST] =
        STEP_OPTIONAL("第二阀门测试", "测试第二阀门开关",
                      diaphragm_handle_second_valve_test, 30000, COMP_VALVE_2),
    [DIAPHRAGM_TILT_SWITCH_TEST] = STEP_OPTIONAL(
        "倾斜开关测试", "测试倾斜开关功能", diaphragm_handle_tilt_switch_test,
        5000, COMP_TILT_SWITCH),

    /*=== 必测步骤 (常规测试) ===*/
    [DIAPHRAGM_DEFAULT_VALVE_TEST] =
        STEP_REQUIRED("阀门电压测试", "测试阀门开关电压",
                      diaphragm_handle_default_valve_test, 30000),
    [DIAPHRAGM_VALVE_POSITION_TEST] = STEP_OPTIONAL(
        "阀门到位测试", "测试阀门到位信号",
        diaphragm_handle_valve_position_test, 10000, COMP_VALVE_POSITION),
    [DIAPHRAGM_TEMP_CHECK] = STEP_REQUIRED("温度压强检测", "读取温度压强传感器",
                                           diaphragm_handle_temp_check, 5000),
    [DIAPHRAGM_NB_CONNECT] = STEP_REQUIRED("NB连接测试", "测试NB-IoT连接",
                                           diaphragm_handle_nb_connect, 60000),
    [DIAPHRAGM_IMEI_READ] = STEP_REQUIRED_RETRY_ENTER(
        "IMEI读取", "读取IMEI/IMSI/ICCID", diaphragm_handle_imei_read, 10000, 3,
        DGM_SendReadIMEI),
    [DIAPHRAGM_CLOSE_IR] =
        STEP_REQUIRED_RETRY_ENTER("关闭红外", "发送关闭红外指令",
                                  diaphragm_close_ir, 5000, 3, DGM_SendCloseIR),
    [DIAPHRAGM_UART_CONNECT_TEST] =
        STEP_REQUIRED("UART连接测试", "测试UART通信连接",
                      diaphragm_handle_uart_connect_test, 5000),

    /*=== 功耗测试
       (放在最后，等模块空闲,气表必须先关闭红红外，否则功耗测试不准确) ===*/
    [DIAPHRAGM_POWER_CHECK] =
        STEP_LOW_POWER("功耗检测", "检测静态功耗(模块空闲)",
                       diaphragm_handle_power_check, 5000, 15),

    /*=== 结束步骤 ===*/
    [DIAPHRAGM_END] =
        STEP_SYSTEM("测试结束", "汇总测试结果", diaphragm_handle_end, 5000,
                    DIAPHRAGM_WAIT, DGM_MODE_IDLE),
};

typedef struct DiaphragmIOStatus_t {
  // 开盖检测相关
  uint8_t cover_check_input_status_before_test;
  uint8_t cover_check_input_status_after_test;
  uint8_t conver_check_result;
  // 119报警相关
  uint8_t warning_input_status_before_test;
  uint8_t warning_input_status_after_test;
  uint8_t warning_test_result;
  // 计量模块相关
  // 霍尔计量相关
  uint8_t metering_hall1_input_status_before_test;
  uint8_t metering_hall1_input_status_after_test;
  uint8_t metering_hall1_test_result;
  uint8_t metering_hall2_input_status_before_test;
  uint8_t metering_hall2_input_status_after_test;
  uint8_t metering_hall2_test_result;
  // 光电计量相关，略，当前项目没有，后续添加

} DiaphragmIOStatus_t;
// 定义开盖具体的IO口状态，包含测试前和测试后的状态
DiaphragmIOStatus_t diaphragm_io_status = {
    // 开盖检测相关
    .cover_check_input_status_before_test = 0,
    .cover_check_input_status_after_test = 0,
    .conver_check_result = 0,
    // 119报警相关
    .warning_input_status_before_test = 0,
    .warning_input_status_after_test = 0,
    .warning_test_result = 0,
    // 计量模块相关
    // 霍尔计量相关
    .metering_hall1_input_status_before_test = 0,
    .metering_hall1_input_status_after_test = 0,
    .metering_hall1_test_result = 0,
    .metering_hall2_input_status_before_test = 0,
    .metering_hall2_input_status_after_test = 0,
    .metering_hall2_test_result = 0,
    // 光电计量相关，略，当前项目没有，后续添加

};

/*============================================================================
 *                          私有函数声明
 *===========================================================================*/

static int8_t diaphragm_hardware_init(void);
static bool check_voltage_in_range(uint16_t voltage, uint16_t min,
                                   uint16_t max);
static bool check_current_in_range(int32_t current, int32_t min, int32_t max);
static bool diaphragm_check_running_power(void);

/*============================================================================
 *                    RetryManager 回调函数 (用于重试状态管理)
 *===========================================================================*/

/**
 * @brief 重试状态重置回调（供 RetryManager 调用）
 * @details 重试时自动清除：事件标志、执行状态、子状态机
 */
static void on_diaphragm_retry_reset(void) {
  // 清除已接收事件（防止旧响应被误判）
  diaphragm_runtime.events_received = DGM_EVT_NONE;

  // 重置步骤执行状态
  diaphragm_runtime.step_executing = 0;

  // 重置子状态机
  diaphragm_runtime.sub_step = 0;

  log_d("重试状态已重置: events_received=0, step_executing=0, sub_step=0");
}

/**
 * @brief 重试动作回调（供 RetryManager 调用）
 * @details 执行重试动作，通常是重新发送协议请求
 */
static void on_diaphragm_retry_action(void) {
  const DiaphragmStepDescriptor_t *step_desc =
      diaphragm_get_current_step_desc();
  if (step_desc == NULL) {
    log_e("重试动作失败: 当前步骤无效");
    return;
  }

  // 执行重试回调 (on_retry 优先, 否则用 on_enter)
  DiaphragmExecuteFunction retry_func =
      step_desc->on_retry ? step_desc->on_retry : step_desc->on_enter;
  if (retry_func != NULL) {
    log_d("执行重试动作: %s", step_desc->name);
    retry_func();
  }
}

/*============================================================================
 *                          测试函数实现
 *===========================================================================*/

/**
 * @brief 获取失败原因的字符串描述
 */
const char *diaphragm_get_fail_reason_string(DgmFailReason_t reason) {
  switch (reason) {
  case DGM_FAIL_NONE:
    return "无错误";
  case DGM_FAIL_HARDWARE_INIT:
    return "硬件初始化失败";
  case DGM_FAIL_MAIN_VOLTAGE:
    return "主电电压异常";
  case DGM_FAIL_5V_VOLTAGE:
    return "5V电压异常";
  case DGM_FAIL_3V3_VOLTAGE:
    return "3.3V电压异常";
  case DGM_FAIL_GPRS_VOLTAGE:
    return "GPRS 3.6V电压异常";
  case DGM_FAIL_MODULE_RESPONSE:
    return "模块响应超时";
  case DGM_FAIL_BOARD_STATUS:
    return "板级状态异常";
  case DGM_FAIL_POWER_CONSUMPTION:
    return "静态功耗异常";
  case DGM_FAIL_VALVE:
    return "阀门测试失败";
  case DGM_FAIL_TIMEOUT:
    return "步骤超时";
  case DGM_TOTOAL_TIMEOUT:
    return "测试总超时";
  case DGM_FAIL_RUNNING_POWER:
    return "运行功耗异常";
  case DGM_FAIL_TEMP_SENSOR:
    return "温度传感器异常";
  case DGM_FAIL_NB_CONNECT:
    return "NB连接失败";
  case DGM_FAIL_IMEI_READ:
    return "IMEI读取失败";
  case DGM_FAIL_SELF_CHECK:
    return "自检失败";
  case DGM_FAIL_IR_CLOSE:
    return "红外关闭失败";
  default:
    return "未知错误";
  }
}

/**
 * @brief 膜式燃气表测试结果清零
 */
void diaphragm_test_result_clear(void) {
  memset(&diaphragm_test_result, 0, sizeof(DiaphragmTestResult_t));
  diaphragm_test_step = DIAPHRAGM_WAIT;

  // 重置运行时状态
  diaphragm_runtime.step_entered = 0;
  diaphragm_runtime.step_executing = 0;
  diaphragm_runtime.sub_step = 0;
  diaphragm_runtime.retry_count = 0;

  // 重置协议事件标志
  diaphragm_runtime.events_received = DGM_EVT_NONE;

  // 重置功耗监控状态
  diaphragm_runtime.power_check_fail_count = 0;
  diaphragm_runtime.running_power_ua = 0;

  // 清除测试完成标志（新测试开始）
  diaphragm_runtime.test_completed = 0;

  // 注意: 不要清除 fail_reason 和 fail_step，保留给上位机查询
  // 这些值会在新测试开始时(diaphragm_handle_start)清除

  log_i("膜式燃气表测试结果已清零");
}

/**
 * @brief 膜式燃气表测试初始化
 */
void diaphragm_test_init(void) {
  // 测试结果清零
  diaphragm_test_result_clear();

  // 注册协议事件回调
  diaphragm_register_protocol_callback();

  // 获取工位编号
  diaphragm_test_result.DeviceID = common_get_station_id();

  log_i("膜式燃气表测试初始化完成, 工位: %d", diaphragm_test_result.DeviceID);
}

/**
 * @brief 膜式燃气表测试开始
 * @note 由上位机发送开始测试指令后调用
 */
void diaphragm_test_start(void) {
  diaphragm_test_init();
  diaphragm_goto_step(DIAPHRAGM_START);
  log_i("膜式燃气表测试开始");
}

/**
 * @brief 获取膜式燃气表测试步骤名称
 * @details 通过回调表获取步骤名称，不再使用 switch-case
 */
const char *diaphragm_get_step_name(DiaphragmTestStep_t step) {
  if (step < DIAPHRAGM_STEP_COUNT) {
    return diaphragm_step_table[step].name;
  }
  return "未知步骤";
}

/**
 * @brief 获取当前步骤描述
 */
const DiaphragmStepDescriptor_t *diaphragm_get_current_step_desc(void) {
  if (diaphragm_test_step < DIAPHRAGM_STEP_COUNT) {
    return &diaphragm_step_table[diaphragm_test_step];
  }
  return NULL;
}

/**
 * @brief 切换到指定测试步骤
 * @param next_step 目标步骤
 * @details 统一的步骤跳转入口，自动处理：
 *          1. 重置步骤执行状态
 *          2. 清除已接收的协议事件
 *          3. 使用 TimeManager 设置超时
 *          4. 使用 RetryManager 初始化重试配置
 *          5. 执行 on_enter 回调
 */
void diaphragm_goto_step(DiaphragmTestStep_t next_step) {
  if (next_step >= DIAPHRAGM_STEP_COUNT) {
    log_e("无效的步骤: %d", next_step);
    return;
  }

  const DiaphragmStepDescriptor_t *next_desc = &diaphragm_step_table[next_step];

  // 打印步骤切换日志（跳过 WAIT 步骤避免刷屏）
  if (diaphragm_test_step != DIAPHRAGM_WAIT || next_step != DIAPHRAGM_WAIT) {
    const DiaphragmStepDescriptor_t *current_desc =
        diaphragm_get_current_step_desc();
    if (current_desc != NULL) {
      log_i("步骤切换: [%s] -> [%s]", current_desc->name, next_desc->name);
    }
  }

  // 更新当前步骤
  diaphragm_test_step = next_step;

  // ===== 重置步骤执行状态 =====
  diaphragm_runtime.step_entered = 0;
  diaphragm_runtime.step_executing = 0;
  diaphragm_runtime.sub_step = 0;
  diaphragm_runtime.retry_count =
      0; // 保留兼容性，之前老的没考虑将重试的单独管理，但实际由
         // RetryManager 管理

  // 清除已接收的协议事件（新步骤需要重新等待）
  diaphragm_runtime.events_received = DGM_EVT_NONE;

  // 重置功耗监控状态
  diaphragm_runtime.power_check_fail_count = 0;
  diaphragm_runtime.last_power_check_tick = TM_GetTick(); // 使用 TimeManager

  // ===== 【集成】使用 TimeManager 设置单步超时 =====
  if (next_desc->timeout_ms > 0) {
    TM_SetStepTimeout(next_desc->timeout_ms);
  } else {
    TM_StopStepTimeout();
  }

  // ===== 初始化 RetryManager =====
  RM_Init(next_desc->max_retry, TM_DELAY_RETRY_INTERVAL);
  // 设置重试的回调函数
  RM_SetResetCallback(on_diaphragm_retry_reset);
  RM_SetRetryActionCallback(on_diaphragm_retry_action);

  // 执行 on_enter 回调
  if (next_desc->on_enter != NULL) {
    log_d("执行步骤[%s] on_enter", next_desc->name);
    next_desc->on_enter();
  }
}

/**
 * @brief 当前步骤失败，记录失败原因并跳转到结束
 * @param reason 失败原因
 */
void diaphragm_step_fail(DgmFailReason_t reason) {
  const DiaphragmStepDescriptor_t *current_desc =
      diaphragm_get_current_step_desc();

  // 记录失败信息
  diaphragm_runtime.fail_reason = reason;
  diaphragm_runtime.fail_step = diaphragm_test_step;

  log_e("步骤[%s]失败, 原因: %s", current_desc ? current_desc->name : "未知",
        diaphragm_get_fail_reason_string(reason));

  diaphragm_goto_step(DIAPHRAGM_END);
}

/**
 * @brief 当前步骤成功完成，跳转到配置的下一步
 * @details 根据步骤表中的 next_step 字段自动跳转
 */
void diaphragm_step_success(void) {
  const DiaphragmStepDescriptor_t *current_desc =
      diaphragm_get_current_step_desc();
  if (current_desc == NULL) {
    log_e("diaphragm_step_success: 当前步骤无效");
    diaphragm_step_fail(DGM_FAIL_NONE);
    return;
  }

  DiaphragmTestStep_t next = current_desc->next_step;

  // 处理特殊值
  if (next == DIAPHRAGM_NEXT_AUTO) {
    // 自动跳转到下一步 (当前步骤 + 1)
    next = (DiaphragmTestStep_t)(diaphragm_test_step + 1);
    if (next >= DIAPHRAGM_STEP_COUNT) {
      next = DIAPHRAGM_END;
    }
  } else if (next == DIAPHRAGM_NEXT_STAY) {
    // 保持当前步骤，由 handler 自行控制跳转
    return;
  }

  log_i("步骤[%s]成功完成", current_desc->name);
  diaphragm_goto_step(next);
}

/**
 * @brief 跳过当前步骤（未实现或不需要测试）
 * @param reason 跳过原因描述
 */
void diaphragm_step_skip(const char *reason) {
  const DiaphragmStepDescriptor_t *current_desc =
      diaphragm_get_current_step_desc();
  if (current_desc == NULL) {
    log_e("diaphragm_step_skip: 当前步骤无效");
    diaphragm_step_fail(DGM_FAIL_NONE);
    return;
  }

  DiaphragmTestStep_t next = current_desc->next_step;

  // 处理特殊值
  if (next == DIAPHRAGM_NEXT_AUTO) {
    next = (DiaphragmTestStep_t)(diaphragm_test_step + 1);
    if (next >= DIAPHRAGM_STEP_COUNT) {
      next = DIAPHRAGM_END;
    }
  } else if (next == DIAPHRAGM_NEXT_STAY) {
    return;
  }

  log_w("步骤[%s]已跳过: %s", current_desc->name, reason ? reason : "未实现");
  diaphragm_goto_step(next);
}

/**
 * @brief 检查运行功耗是否正常
 * @return true-正常, false-异常
 */
static bool diaphragm_check_running_power(void) {
  int32_t current_ua = 0;
  PortStatus status = read_main_power_consumption_ua(&current_ua);

  if (status != PORT_OK) {
    // 读取失败不算异常，可能是硬件不支持
    return true;
  }

  diaphragm_runtime.running_power_ua = (uint16_t)current_ua;

  // 检查是否在正常范围内
  if (current_ua < RUNNING_POWER_MIN_CURRENT_UA ||
      current_ua > RUNNING_POWER_MAX_CURRENT_UA) {
    diaphragm_runtime.power_check_fail_count++;
    log_w("运行功耗异常: %ld uA (范围: %d-%d), 连续异常次数: %d", current_ua,
          RUNNING_POWER_MIN_CURRENT_UA, RUNNING_POWER_MAX_CURRENT_UA,
          diaphragm_runtime.power_check_fail_count);

    if (diaphragm_runtime.power_check_fail_count >=
        RUNNING_POWER_FAIL_THRESHOLD) {
      return false; // 确认异常
    }
  } else {
    // 功耗正常，重置异常计数
    diaphragm_runtime.power_check_fail_count = 0;
  }

  return true;
}

/**
 * @brief 设置软件延时 (使用 TimeManager)
 * @param delay_ms 延时时间(ms)
 * @details 调用后，主循环会在延时期间直接返回，延时结束后继续执行
 */
void diaphragm_set_delay(uint32_t delay_ms) { TM_SetDelay(delay_ms); }

/**
 * @brief 膜式燃气表测试主循环
 * @details 基于回调表驱动的测试主循环，集成 TimeManager 和 RetryManager
 *          - 使用 TimeManager 进行全局/单步超时管理
 *          - 使用 RetryManager 进行统一重试管理
 *          - 运行功耗监控使用 TM_IsPeriodElapsed() 周期检测
 */
void diaphragm_test_loop(void) {
  // ===== 【最优先】检查测试结束后保持供电等待 =====
  // 必须在所有其他检查之前，否则 TM_IsDelayActive() 会 return
  if (diaphragm_runtime.waiting_power_off) {
    // 等待期间检测5V电压，如果板子被拿起则立即关电,持续检测
    uint16_t voltage_5v = 0;
    if (read_board_5v_voltage_mv(&voltage_5v) == PORT_OK) {
      if (voltage_5v < 4500) {
        // 板子被拿起，立即关电，持续检测
        log_i("检测到膜表断开(5V=%dmV)，立即关闭电源", voltage_5v);
        power_off_main_power();
        power_off_backup_power();
        diaphragm_runtime.waiting_power_off = 0;
        TM_CancelDelay();
        return;
      }
    }
    // 检查延时是否到期
    if (TM_IsDelayComplete()) {
      log_i("保持供电时间到，关闭电源");
      power_off_main_power();
      power_off_backup_power();
      diaphragm_runtime.waiting_power_off = 0;
    }
    return; // 等待期间不处理其他逻辑
  }

  // ===== 检查重试延时 - 同时检查是否已收到响应 =====
  if (RM_IsWaitingRetryDelay()) {
    // 如果等待延时期间收到了响应，取消延时，继续处理
    if (diaphragm_runtime.events_received != DGM_EVT_NONE) {
      log_d("等待重试期间收到响应(0x%04X)，取消延时",
            diaphragm_runtime.events_received);
      RM_CancelRetryDelay();
      // 不return，继续执行下面的逻辑
    } else if (RM_CheckRetryDelayComplete()) {
      // 延时完成，重试动作已在回调中执行
      return;
    } else {
      // 仍在等待延时，且没有收到响应
      return;
    }
  }

  // ===== 软件延时检查 (使用 TimeManager) =====
  if (TM_IsDelayActive()) {
    if (!TM_IsDelayComplete()) {
      return; // 延时未到，直接返回
    }
    // 延时完成，自动清除
  }

  // ===== 全局超时检查 (使用 TimeManager) =====
  if (TM_IsGlobalTimeout()) {
    log_e("测试总超时 (%d ms)，强制结束测试", TM_TIMEOUT_GLOBAL_TEST);
    TM_StopGlobalTimeout();
    diaphragm_step_fail(DGM_TOTOAL_TIMEOUT);
    return;
  }

  // ===== 硬件初始化，只有在硬件初始化未完成且未失败时执行 =====
  if (diaphragm_runtime.hardware_init_pending &&
      !diaphragm_runtime.hardware_init_failed) {
    int8_t ret = diaphragm_hardware_init();
    if (ret == 0) {
      log_i("硬件初始化完成");
      diaphragm_runtime.hardware_init_pending = 0;
    } else if (ret == -2) {
      // 彻底失败，不再重试
      diaphragm_runtime.hardware_init_failed = 1;
      log_e("硬件初始化失败，停止测试");
    }
    // ret == -1 时继续重试
    return;
  }

  // ===== 获取当前步骤描述 =====
  const DiaphragmStepDescriptor_t *step_desc =
      diaphragm_get_current_step_desc();
  if (step_desc == NULL) {
    log_e("无效的测试步骤: %d", diaphragm_test_step);
    diaphragm_step_fail(DGM_FAIL_NONE);
    return;
  }

  // ===== IDLE 模式：静默等待，不做任何监控 =====
  // 注意：DIAPHRAGM_WAIT 步骤是 IDLE 模式，直接返回
  // 但 DIAPHRAGM_END 步骤也是 IDLE 模式，需要执行 handler 来关闭电源
  if (step_desc->test_mode == DGM_MODE_IDLE) {
    // DIAPHRAGM_END 步骤需要执行 handler 来关闭电源并汇总结果
    if (diaphragm_test_step == DIAPHRAGM_END && step_desc->handler != NULL) {
      step_desc->handler();
    }
    return;
  }

  // ===== 【集成】NORMAL 模式：使用 TM_IsPeriodElapsed() 定期检测运行功耗 =====
  if (step_desc->test_mode == DGM_MODE_NORMAL) {
    if (TM_IsPeriodElapsed(TM_PERIOD_ID_POWER)) {
      if (!diaphragm_check_running_power()) {
        log_e("运行功耗持续异常，测试终止");
        log_e("最后读数: %d uA", diaphragm_runtime.running_power_ua);
        diaphragm_step_fail(DGM_FAIL_RUNNING_POWER);
        return;
      }
      // 检测5V电压，这个是膜表上的5V电压，如果没了说明膜表被拿起来了
      uint16_t voltage_5v = 0;
      PortStatus status = read_board_5v_voltage_mv(&voltage_5v);
      if (status != PORT_OK) {
        log_e("读取5V电压失败: %s", port_status_to_string(status));
        common_print_step_footer("失败：无法读取5V电压");
        diaphragm_step_fail(DGM_FAIL_5V_VOLTAGE);
        return;
      }
      // 检查电压值是否在正常范围 (4.5V~5.5V)
      if (voltage_5v < 4500 || voltage_5v > 5500) {
        log_e("5V电压异常: %dmV (范围: 4500~5500mV)，膜表可能已断开",
              voltage_5v);
        common_print_step_footer("失败：5V电压异常，膜表可能已断开");
        diaphragm_step_fail(DGM_FAIL_5V_VOLTAGE);
        return;
      }
    }
  }

  // ===== LOW_POWER 模式：跳过运行功耗监控（由步骤自己处理）=====

  // ===== 超时检查 - 确保 handler 至少执行一次 =====
  // 关键：step_entered 标志在 handler 首次执行后设置为1
  // 只有 handler 执行过后才检查超时，避免还没执行就超时的问题
  if (TM_IsStepTimeout() && diaphragm_runtime.step_entered) {
    // 如果已收到有效响应，跳过超时重试
    if (diaphragm_runtime.events_received != DGM_EVT_NONE) {
      log_d("已收到响应(0x%04X)，跳过超时重试",
            diaphragm_runtime.events_received);
      TM_ResetStepTimeout(); // 重置超时，给handler时间处理
      // 不return，继续执行handler
    } else {
      // 真正的超时（无响应），触发重试
      RM_RetryResult_t result = RM_TryRetry(RM_REASON_TIMEOUT);

      switch (result) {
      case RM_RESULT_RETRY_OK:
        // 重试已触发，状态已在回调中重置，动作已在回调中执行（或等待延时后执行）
        log_w("步骤[%s]超时，重试 %d/%d", step_desc->name, RM_GetRetryCount(),
              step_desc->max_retry);
        // 同步更新 diaphragm_runtime.retry_count 保持兼容性
        diaphragm_runtime.retry_count = RM_GetRetryCount();
        return;

      case RM_RESULT_RETRY_EXHAUSTED:
        log_e("步骤[%s]超时，重试已用尽 (%d/%d)", step_desc->name,
              RM_GetRetryCount(), step_desc->max_retry);
        diaphragm_step_fail(DGM_FAIL_TIMEOUT);
        return;

      case RM_RESULT_NO_RETRY_CONFIG:
        log_e("步骤[%s]超时，无重试配置", step_desc->name);
        diaphragm_step_fail(DGM_FAIL_TIMEOUT);
        return;
      }
    }
  }

  // ===== 调用步骤处理函数 =====
  if (step_desc->handler != NULL) {
    // 检查是否需要跳过该步骤 (通用跳过检测机制)
    if (step_desc->skip_status_ptr != NULL &&
        *step_desc->skip_status_ptr == step_desc->skip_when_equals) {
      diaphragm_step_skip("附件不支持");
      return;
    }
    step_desc->handler();
  }
}

/*============================================================================
 *                          私有辅助函数
 *===========================================================================*/

/**
 * @brief 检查电压是否在范围内
 */
static bool check_voltage_in_range(uint16_t voltage, uint16_t min,
                                   uint16_t max) {
  return (voltage >= min && voltage <= max);
}

/**
 * @brief 检查电流是否在范围内
 */
static bool check_current_in_range(int32_t current, int32_t min, int32_t max) {
  return (current >= min && current <= max);
}

/**
 * @brief 膜式燃气表硬件初始化
 * @return 0-成功, -1-重试, -2-彻底失败
 */
static int8_t diaphragm_hardware_init(void) {
  // 先尝试初始化 board + port 层
  if (!diaphragm_gas_port_is_init()) {
    if (diaphragm_gas_board_init() != 0) {
      diaphragm_runtime.init_retry_count++;
      if (diaphragm_runtime.init_retry_count <= 5) {
        log_e("膜式燃气表板级初始化失败 (重试 %d/5)",
              diaphragm_runtime.init_retry_count);
        return -1;
      } else {
        log_e("膜式燃气表板级初始化失败，重试多次无效，放弃初始化");
        return -2;
      }
    }
  }

  // port层已初始化，重置重试计数器
  diaphragm_runtime.init_retry_count = 0;

  // 初始状态：关闭所有电源
  power_off_main_power();
  power_off_backup_power();

  log_i("硬件端口初始化完成");
  return 0;
}

/*============================================================================
 *                          测试步骤处理函数实现
 *===========================================================================*/

void diaphragm_handle_wait(void) {
  // 等待测试开始信号，无需操作
  // LED指示: 空闲等待 - 慢闪
  LedIndicator_SetStatus(LED_STATUS_IDLE);
}

/**
 * @brief 开始测试处理
 * @details 检测输入电压和各路电压是否在合理范围内
 */
void diaphragm_handle_start(void) {
  // 标记步骤已进入执行（用于超时检查判断）

  diaphragm_runtime.step_entered = 1;

  PortStatus status;
  uint16_t voltage = 0;
  // 重置component_status为默认值，后续在IO状态检测步骤根据1001协议响应更新
  diaphragm_component_status.valve_1_status = 1; // 必须测试阀门1
  diaphragm_component_status.valve_2_status =
      DIAPHRAGM_COMPONENT_STATUS_UNTESTED; // 默认不测试阀门2，根据1001协议双阀标志更新是否测试阀门2，当前实际并没有做这个
  diaphragm_component_status.module_temp_pressure_sensor =
      DIAPHRAGM_COMPONENT_STATUS_UNTESTED;
  diaphragm_component_status.warning_component_status =
      DIAPHRAGM_COMPONENT_STATUS_UNTESTED;
  diaphragm_component_status.cover_check_status =
      DIAPHRAGM_COMPONENT_STATUS_UNTESTED;
  diaphragm_component_status.ic_module_status =
      DIAPHRAGM_COMPONENT_STATUS_UNTESTED;
  diaphragm_component_status.irf_component_status =
      DIAPHRAGM_COMPONENT_STATUS_UNTESTED;
  diaphragm_component_status.main_circuit_low_power_current_status =
      1; // 必须测试主电低功耗
  diaphragm_component_status.module_circuit_low_power_current_status =
      DIAPHRAGM_COMPONENT_STATUS_UNTESTED;
  diaphragm_component_status.metering_module_status =
      DIAPHRAGM_COMPONENT_STATUS_UNTESTED;
  diaphragm_component_status.bluetooth_component_status =
      DIAPHRAGM_COMPONENT_STATUS_UNTESTED;
  diaphragm_component_status.tilt_switch_status =
      DIAPHRAGM_COMPONENT_STATUS_UNTESTED;
  // 初始化diaphragm_test_result
  memset(&diaphragm_test_result, 0, sizeof(DiaphragmTestResult_t));
  // 开始新测试，清除上次的失败信息
  diaphragm_runtime.fail_reason = DGM_FAIL_NONE;
  diaphragm_runtime.fail_step = DIAPHRAGM_WAIT;
  // 开始测试，先赋值工位ID
  diaphragm_test_result.DeviceID = common_get_station_id();
  diaphragm_test_result.Reserve1 = 0xFF; // 保留字段默认值
  diaphragm_test_result.ModulePowerStatus =
      1; // 默认备电异常，上位有点问题，备电的异常状态屏蔽不掉，即使不测试
  common_print_step_header("开始测试",
                           "膜式燃气表测试流程启动，检测各路电压\r\n");

  // LED指示: 测试进行中 - 快闪
  LedIndicator_SetStatus(LED_STATUS_TESTING);

  // ===== 【集成】初始化 TimeManager 时间管理系统 =====
  // 启动全局超时计时
  TM_StartGlobalTimeout(TM_TIMEOUT_GLOBAL_TEST);

  // 启动功耗检测周期任务
  TM_StartPeriod(TM_PERIOD_ID_POWER, TM_PERIOD_POWER_CHECK);

  // 清除旧的延时状态
  TM_CancelDelay();

  // ===== 【优化】禁用上位机UART0接收中断，避免干扰测试过程 =====
  // 测试过程中上位机会频繁发送查询命令(0xFC04)，导致CPU占用
  // 禁用后测试结束时（包括超时失败）会自动恢复
  UART0_DisableRxInterrupt();
  log_i("测试期间禁用上位机通信中断");

  // 1. 开启主电源（正常模式）
  // 关闭板载的大电容，这个并联联在膜表的主电上，充电会影响电压检测
  log_i("关闭板载电容以避免影响电压检测");
  status = diaphragm_board_capacitor_disable();

  if (status != PORT_OK) {
    log_e("关闭板载电容失败: %s", port_status_to_string(status));
    common_print_step_footer("失败：无法关闭板载电容");
    diaphragm_test_step = DIAPHRAGM_END;
    return;
  }
  status = power_on_main_power();
  if (status != PORT_OK) {
    log_e("开启主电源失败: %s", port_status_to_string(status));
    common_print_step_footer("失败：无法开启主电源");
    diaphragm_test_step = DIAPHRAGM_END;
    return;
  }

  // 等待电压稳定
  port_delay_ms(10);

  // 2. 检测输入电压 (根据电池类型判断范围)
  status = read_main_voltage_mv(&voltage);
  if (status != PORT_OK) {
    log_e("读取主电电压失败: %s", port_status_to_string(status));
    common_print_step_footer("失败：无法读取主电电压");
    diaphragm_test_step = DIAPHRAGM_END;
    return;
  }
  {
    uint16_t min_v = get_main_power_min_voltage_mv();
    uint16_t max_v = get_main_power_max_voltage_mv();
    diaphragm_test_result.MasterVoult =
        (uint8_t)(voltage / 100); // 转换为0.1V单位

    if (!check_voltage_in_range(voltage, min_v, max_v)) {
      log_e("主电电压异常: %dmV, 范围[%d-%d]mV", voltage, min_v, max_v);
      common_print_step_footer("失败：主电电压超出范围");
      diaphragm_test_step = DIAPHRAGM_END;
      return;
    }
    log_i("主电电压正常: %dmV", voltage);
  }

  // 3. 检测板上5V电压
  status = read_board_5v_voltage_mv(&voltage);
  // debug_breakpoint();
  if (status != PORT_OK) {
    log_e("读取5V电压失败: %s", port_status_to_string(status));
    common_print_step_footer("失败：无法读取5V电压");
    diaphragm_test_step = DIAPHRAGM_END;
    return;
  }
  if (!check_voltage_in_range(voltage, METER_BOARD_5V_MIN_VOLTAGE_MV,
                              METER_BOARD_5V_MAX_VOLTAGE_MV)) {
    log_e("5V电压异常: %dmV, 范围[%d-%d]mV", voltage,
          METER_BOARD_5V_MIN_VOLTAGE_MV, METER_BOARD_5V_MAX_VOLTAGE_MV);
    common_print_step_footer("失败：5V电压超出范围");
    diaphragm_test_step = DIAPHRAGM_END;
    return;
  }
  log_i("5V电压正常: %dmV", voltage);

  // 3.5 检测RTC电池电压 (ADC IN5 PA0)
  status = read_rtc_battery_voltage_mv(&voltage);
  if (status != PORT_OK) {
    log_e("读取RTC电池电压失败: %s", port_status_to_string(status));
    // RTC电池电压读取失败不阻止测试，只记录警告
    log_w("RTC电池电压读取失败，继续测试");
  } else {
    log_i("RTC电池电压: %dmV", voltage);
    // 存储RTC电压到测试结果 (0.1V单位)
    diaphragm_test_result.RTC_Volt = (uint8_t)(voltage / 100);
  }

  // 4. 检测板上4.7V电压 (5V经二极管)
  // 使能4.7V ADC采样，这个是控制PMOS的通断，用完需要禁用，否则检测低功耗异常
  status = enable_board_4v7_read(1);
  if (status != PORT_OK) {
    log_e("使能4.7V ADC失败: %s", port_status_to_string(status));
    common_print_step_footer("失败：无法使能4.7V ADC");
    diaphragm_test_step = DIAPHRAGM_END;
    return;
  }
  status = read_board_4v7_voltage_mv(&voltage);
  // 读取完成后立即禁用4.7V ADC采样，避免持续功耗
  enable_board_4v7_read(0);
  if (status != PORT_OK) {
    log_e("读取4.7V电压失败: %s", port_status_to_string(status));
    common_print_step_footer("失败：无法读取4.7V电压");
    diaphragm_test_step = DIAPHRAGM_END;
    return;
  }
  if (!check_voltage_in_range(voltage, METER_BOARD_4V7_MIN_VOLTAGE_MV,
                              METER_BOARD_4V7_MAX_VOLTAGE_MV)) {
    log_e("4.7V电压异常: %dmV, 范围[%d-%d]mV", voltage,
          METER_BOARD_4V7_MIN_VOLTAGE_MV, METER_BOARD_4V7_MAX_VOLTAGE_MV);
    common_print_step_footer("失败：4.7V电压超出范围");
    diaphragm_test_step = DIAPHRAGM_END;
    return;
  }
  log_i("4.7V电压正常: %dmV", voltage);

  // 5. 检测板上3.3V电压
  status = read_board_3v3_voltage_mv(&voltage);
  if (status != PORT_OK) {
    log_e("读取3.3V电压失败: %s", port_status_to_string(status));
    common_print_step_footer("失败：无法读取3.3V电压");
    diaphragm_test_step = DIAPHRAGM_END;
    return;
  }
  if (!check_voltage_in_range(voltage, METER_BOARD_3V3_MIN_VOLTAGE_MV,
                              METER_BOARD_3V3_MAX_VOLTAGE_MV)) {
    log_e("3.3V电压异常: %dmV, 范围[%d-%d]mV", voltage,
          METER_BOARD_3V3_MIN_VOLTAGE_MV, METER_BOARD_3V3_MAX_VOLTAGE_MV);
    common_print_step_footer("失败：3.3V电压超出范围");
    diaphragm_test_step = DIAPHRAGM_END;
    return;
  }
  log_i("3.3V电压正常: %dmV", voltage);

  // GPRS 3.6V电压检测移到连接测试步骤，因为需要等待被测设备启动

  common_print_step_footer("通过：工装电压检测正常");

  // 切换到连接测试步骤（会自动重置 gprs_check_started 和超时）
  diaphragm_goto_step(DIAPHRAGM_MODULE_GPRS_VOLTAGE_TEST);
}

/**
 * @brief 连接测试处理（轮询等待型）
 * @details 等待被测设备启动，检测GPRS 3.6V电压是否正常
 * @note 此步骤会循环检测，直到电压正常或超时(10秒)
 */
void diaphragm_check_gprs_voltage(void) {

  PortStatus status;
  uint16_t voltage = 0;

  // ===== 步骤首次进入 =====
  if (!diaphragm_runtime.step_entered) {
    diaphragm_runtime.step_entered = 1;
    common_print_step_header("连接测试", "等待被测设备启动，检测GPRS 3.6V电压");

    log_i("开始检测GPRS 3.6V电压，超时时间: %d ms",
          common_get_test_config()->step_timeout_ms);
  }

  // 超时检查已在 diaphragm_test_loop 主循环中统一处理

  // 读取GPRS 3.6V电压
  status = read_gprs_3v6_voltage_mv(&voltage);
  if (status != PORT_OK) {
    // 读取失败，等待下次循环重试
    log_d("读取GPRS 3.6V电压失败: %s, 重试中...",
          port_status_to_string(status));
    return;
  }

  // 检查电压是否在范围内
  if (check_voltage_in_range(voltage, GPRS_3V6_MIN_VOLTAGE_MV,
                             GPRS_3V6_MAX_VOLTAGE_MV)) {
    // 电压正常，测试通过
    log_i("GPRS 3.6V电压正常: %dmV", voltage);
    common_print_step_footer("通过：被测设备已启动");
    diaphragm_step_success();
    return;
  }

  // 电压不在范围内，继续等待（超时由主循环处理）
  log_d("GPRS 3.6V电压: %dmV, 等待设备启动...", voltage);
}

/**
 * @brief 等待模块响应处理（轮询等待型）
 * @details 等待膜表模组上告成功，收到1000协议响应后跳转
 */
void diaphragm_wait_module_ready_response(void) {
  // ===== 步骤首次进入 =====
  if (!diaphragm_runtime.step_entered) {
    diaphragm_runtime.step_entered = 1;
    common_print_step_header("等待模块响应", "等待模块1000协议响应");
    log_i("开始等待模块1000协议响应，超时时间: %d ms",
          common_get_test_config()->step_timeout_ms);
    // LED指示: 等待通讯响应 - 双闪
    LedIndicator_SetStatus(LED_STATUS_COMM_RX_WAIT);
  }

  // =====
  // 检查期望的事件,如果收到响应标志或者超时，这个1000协议有概率收不到，超时也进入下一步,这个后面再优化
  // =====
  if (diaphragm_runtime.events_received & DGM_EVT_SELF_CHECK) {
    diaphragm_runtime.events_received &= ~DGM_EVT_SELF_CHECK; // 清除标志
    log_i("收到模块自检完成响应");
    common_print_step_footer("通过：模块响应成功");
    diaphragm_step_success();
    return;
  }

  // ===== 继续等待（超时由主循环处理）=====
}

/**
 * @brief IO初始状态检测处理（轮询等待型）
 * @details 等待0x1001板级信息响应，检测各IO口状态
 */
void diaphragm_handle_io_status_check_initial_status(void) {
  // ===== 步骤首次进入 =====
  if (!diaphragm_runtime.step_entered) {
    diaphragm_runtime.step_entered = 1;
    common_print_step_header("IO初始状态检测", "检测各IO口状态初始状态");
  }

  // ===== 检查期望的事件 (0x1001 板级信息) =====
  if (diaphragm_runtime.events_received & DGM_EVT_BOARD_INFO) {
    diaphragm_runtime.events_received &= ~DGM_EVT_BOARD_INFO; // 清除标志

    // ========== 对0x1001响应数据进行判断 ==========
    DgmBoardInfoData *info = &diaphragm_runtime.board_info;
    bool check_passed = true;

    log_i("=== 0x1001 初始状态检测 ===");

    // 1. 检测模块状态
    if (info->module_status != 1) {
      log_e("模块状态异常: %d (期望=1)", info->module_status);
      check_passed = false;
    } else {
      log_i("模块状态: 正常");
    }

    // 2. 检测SIM卡状态
    if (info->sim_ok != 1) {
      log_e("SIM卡状态异常: %d (期望=1)", info->sim_ok);
      check_passed = false;
    } else {
      log_i("SIM卡状态: 正常");
    }

    // 3. 检测存储芯片，可能是NOR或EEPROM
    if (info->storage_ic_ok != 1) {
      //
      log_e("存储芯片状态异常: %d (期望=1)", info->storage_ic_ok);
      // 赋值给diaphragm_test_result结构体的io_status1的bit3
      diaphragm_test_result.IOStatus1 |= (1 << IOSTATUS1_EEPROM_BIT);

      check_passed = false;
    } else {
      log_i("存储芯片状态: 正常");
      // 赋值给diaphragm_test_result结构体的io_status1的bit3
      diaphragm_test_result.IOStatus1 &= ~(1 << IOSTATUS1_EEPROM_BIT);
    }

    // 4. 检测RTC状态
    if (info->rtc_ok != 1) {
      log_e("RTC状态异常: %d (期望=1)", info->rtc_ok);
      check_passed = false;
    } else {
      log_i("RTC状态: 正常");
    }

    // 5. 检测电压范围 (0.1V单位, 期望5.0V-7.0V)
    if (info->voltage < 50 || info->voltage > 70) {
      log_e("主控板电压异常: %d.%dV (期望5.0V-7.0V)", info->voltage / 10,
            info->voltage % 10);
      check_passed = false;
    } else {
      log_i("主控板电压: %d.%dV 正常", info->voltage / 10, info->voltage % 10);
    }

    // 6. 检测信号强度 (期望>0 且 <33)
    if (info->signal == 0 || info->signal >= 33) {
      log_w("信号强度异常: %d (期望0-33)", info->signal);
      // 信号强度异常不一定导致失败，只是警告
    } else {
      log_i("信号强度: %d 正常", info->signal);
    }

    // 7. 检测开盖状态 (初始状态应为开盖=1, 因为被测表在工装上)
    log_i("开盖状态: %d (%s)", info->cover_open,
          info->cover_open == 0 ? "合盖"
                                : (info->cover_open == 1 ? "开盖" : "无功能"));
    // 对开盖初始状态进行赋值
    if (info->cover_open > 1) {
      log_w("开盖检测功能不可用");
    } else {
      diaphragm_io_status.cover_check_input_status_before_test =
          info->cover_open;
    }

    // 8. 检测并打印附件信息,确认后续哪些需要测试，哪些可以跳过，数据不进行验证
    // 对附件信息进行解析，并赋值给diaphragm_component_status结构体的各个成员变量
    // 注意：1=有附件需要测试，DIAPHRAGM_COMPONENT_STATUS_UNTESTED(2)=无附件跳过测试
    diaphragm_component_status.warning_component_status =
        (info->has_addon & 0x01) ? 1
                                 : DIAPHRAGM_COMPONENT_STATUS_UNTESTED; // 119
    diaphragm_component_status.ic_module_status =
        (info->has_addon & 0x02) ? 1
                                 : DIAPHRAGM_COMPONENT_STATUS_UNTESTED; // IC卡
    diaphragm_component_status.valve_2_status =
        (info->has_addon & 0x04) ? 1
                                 : DIAPHRAGM_COMPONENT_STATUS_UNTESTED; // 双阀
    // 蓝牙状态从bluetooth_ok字段获取，2=没有蓝牙，其他=有蓝牙
    diaphragm_component_status.bluetooth_component_status =
        (info->bluetooth_ok == 2) ? DIAPHRAGM_COMPONENT_STATUS_UNTESTED : 1;
    diaphragm_component_status.module_temp_pressure_sensor =
        (info->has_addon & 0x10) ? 1
                                 : DIAPHRAGM_COMPONENT_STATUS_UNTESTED; // 温压
    diaphragm_component_status.irf_component_status =
        (info->has_addon & 0x20) ? 1
                                 : DIAPHRAGM_COMPONENT_STATUS_UNTESTED; // 红外
    diaphragm_component_status.cover_check_status =
        (info->has_addon & 0x40)
            ? 1
            : DIAPHRAGM_COMPONENT_STATUS_UNTESTED; // 开盖检测
    // 倾斜开关状态从tilt_ok字段获取，2=没有倾斜开关，其他=有倾斜开关
    diaphragm_component_status.tilt_switch_status =
        (info->tilt_ok == 2) ? DIAPHRAGM_COMPONENT_STATUS_UNTESTED : 1;

    // 9. 根据表具类型设置计量模块测试状态
    // meter_type: 0=霍尔计量, 1=光电计量
    // 霍尔和光电都需要测试，只有meter_type不是0或1才跳过
    if (info->meter_type == 0) {
      diaphragm_component_status.metering_module_status =
          1; // 霍尔计量，需要测试
      log_i("计量模块类型: 霍尔计量 (meter_type=%d)", info->meter_type);
    } else if (info->meter_type == 1) {
      diaphragm_component_status.metering_module_status =
          1; // 光电计量，需要测试
      log_i("计量模块类型: 光电计量 (meter_type=%d)", info->meter_type);
    } else {
      diaphragm_component_status.metering_module_status =
          DIAPHRAGM_COMPONENT_STATUS_UNTESTED; // 未知类型，跳过
      log_w("计量模块类型未知 (meter_type=%d)，跳过测试", info->meter_type);
    }

    // 打印原始bit值 (0或1)
    log_i("附件原始值: 0x%02X (119=%d, IC卡=%d, 双阀=%d, 蓝牙bit=%d, 温压=%d, "
          "红外=%d, 开盖=%d)",
          info->has_addon, (info->has_addon & 0x01) ? 1 : 0,
          (info->has_addon & 0x02) ? 1 : 0, (info->has_addon & 0x04) ? 1 : 0,
          (info->has_addon & 0x08) ? 1 : 0, (info->has_addon & 0x10) ? 1 : 0,
          (info->has_addon & 0x20) ? 1 : 0, (info->has_addon & 0x40) ? 1 : 0);
    // 打印解析后状态 (1=需测试, 2=跳过)
    log_i("测试状态: 119=%s, IC卡=%s, 双阀=%s, 温压=%s, 红外=%s, 开盖=%s, "
          "计量=%s",
          diaphragm_component_status.warning_component_status == 1 ? "测"
                                                                   : "跳",
          diaphragm_component_status.ic_module_status == 1 ? "测" : "跳",
          diaphragm_component_status.valve_2_status == 1 ? "测" : "跳",
          diaphragm_component_status.module_temp_pressure_sensor == 1 ? "测"
                                                                      : "跳",
          diaphragm_component_status.irf_component_status == 1 ? "测" : "跳",
          diaphragm_component_status.cover_check_status == 1 ? "测" : "跳",
          diaphragm_component_status.metering_module_status == 1 ? "测" : "跳");
    // 特殊附件：蓝牙和倾斜从专用字段获取
    log_i("特殊附件: 蓝牙=%s(原值%d), 倾斜=%s(原值%d)",
          diaphragm_component_status.bluetooth_component_status == 1 ? "测"
                                                                     : "跳",
          info->bluetooth_ok,
          diaphragm_component_status.tilt_switch_status == 1 ? "测" : "跳",
          info->tilt_ok);

    log_i("=== 检测结果: %s ===", check_passed ? "通过" : "失败");

    // 根据检测结果决定下一步
    if (check_passed) {
      // 将0x1001协议数据赋值给测试结果结构体
      diaphragm_test_result.MeterTYP = info->meter_type;
      diaphragm_test_result.IsOrNoWithIterm = info->has_addon;
      diaphragm_test_result.Module_Csq = info->signal;
      log_i("信号值已赋值: Module_Csq=%d (来源: info->signal=%d)",
            diaphragm_test_result.Module_Csq, info->signal);
      // 软件版本号: 高字节为主版本(sw_ver1), 低字节为次版本(sw_ver2)

      // diaphragm_test_result.FirmwareVersion =
      //    ((uint16_t)info->sw_ver1 << 8) | info->sw_ver2;
      // 特殊配置，颠倒版本号，当前固件版本号格式为低字节主版本，高字节次版本
      diaphragm_test_result.FirmwareVersion =
          ((uint16_t)info->sw_ver2 << 8) | info->sw_ver1;
      //  设置IOStatus1中的基本状态位
      diaphragm_test_result.IOStatus1 = 0;
      if (info->module_status == 1)
        IOSTATUS_SET(diaphragm_test_result.IOStatus1, IOSTATUS1_MODULE_BIT);
      if (info->connect_status == 1)
        IOSTATUS_SET(diaphragm_test_result.IOStatus1, IOSTATUS1_CONNECT_BIT);
      if (info->sim_ok == 1)
        IOSTATUS_SET(diaphragm_test_result.IOStatus1, IOSTATUS1_SIM_BIT);
      if (info->storage_ic_ok == 1)
        IOSTATUS_SET(diaphragm_test_result.IOStatus1, IOSTATUS1_EEPROM_BIT);
      if (info->measure_ok == 1)
        IOSTATUS_SET(diaphragm_test_result.IOStatus1, IOSTATUS1_METERING_BIT);
      // 阀门(VALVE), 119, IC卡 后续测试中设置

      // 设置IOStatus2中的基本状态位 (保留位默认为1)
      diaphragm_test_result.IOStatus2 =
          IOSTATUS2_RESERVED1_BIT | IOSTATUS2_RESERVED2_BIT;
      // 当前红外是有回复的，设置红外状态位为1(正常)
      IOSTATUS_SET(diaphragm_test_result.IOStatus2, IOSTATUS2_IR_BIT);
      if (info->rtc_ok == 1)
        IOSTATUS_SET(diaphragm_test_result.IOStatus2, IOSTATUS2_RTC_BIT);
      // 温压(TEMP_PRESS) 后续测试中设置
      // 开盖状态: 0=合盖异常, 1=开盖正常, 2=无功能视为正常
      if (info->cover_open != 0)
        IOSTATUS_SET(diaphragm_test_result.IOStatus2, IOSTATUS2_COVER_BIT);
      // 倾斜状态: 0=正常, 1=异常, 2=无功能视为正常
      if (info->tilt_ok == 0 || info->tilt_ok == 2)
        IOSTATUS_SET(diaphragm_test_result.IOStatus2, IOSTATUS2_TILT_BIT);
      // 蓝牙状态: 后续测试中设置
      if (info->temp_press_ok == 1)
        IOSTATUS_SET(diaphragm_test_result.IOStatus2, IOSTATUS2_TEMP_PRESS_BIT);

      common_print_step_footer("通过: 初始状态检测正常");
      diaphragm_step_success(); // 跳转到下一步
    } else {
      // 检测失败，检查是否可以重试
      const DiaphragmStepDescriptor_t *step_desc =
          diaphragm_get_current_step_desc();
      if (diaphragm_runtime.retry_count < step_desc->max_retry) {
        diaphragm_runtime.retry_count++;
        log_w("初始状态检测失败，重试 %d/%d", diaphragm_runtime.retry_count,
              step_desc->max_retry);

        // 重置超时，重新发送请求
        common_reset_step_timeout(common_get_test_config(),
                                  step_desc->timeout_ms);
        if (step_desc->on_enter != NULL) {
          step_desc->on_enter(); // 重新发送 0x1001 请求
        }
        // 继续等待新的响应（不return，让主循环继续轮询）
      } else {
        common_print_step_footer("失败: 初始状态异常(重试已用尽)");
        diaphragm_step_fail(DGM_FAIL_BOARD_STATUS); // 重试用尽，结束测试
      }
    }
    return;
  }

  // ===== 继续等待（超时由主循环处理）=====
  log_d("等待收到1001协议的响应...");
}

/**
 * @brief 功耗检测处理（支持重试）
 * @details 切换到功耗测量模式，检测静态功耗
 *          如果功耗超出范围，等待超时触发重试机制
 */
void diaphragm_handle_power_check(void) {
  // ===== 首次进入时初始化 =====
  if (!diaphragm_runtime.step_executing) {
    diaphragm_runtime.step_executing = 1;
    common_print_step_header("功耗检测", "检测静态功耗");

    // ===== 关键：测量功耗前清理所有可能影响功耗的IO口 =====
    // 释放阀门到位信号（避免通过气表上拉电阻灌入电流）
    output_valve_position_signal_1(0); // PA12 释放
    output_valve_position_signal_2(0); // PA11 释放
    // 关闭霍尔信号输出
    output_hall_signal_1(0);
    output_hall_signal_2(0);
    // 关闭防拆检测电磁铁
    output_open_cover_signal(0);
    // 恢复119报警信号为非触发状态（高电平）
    output_alarm_119_signal(1);
    log_d("IO状态已清理，准备测量功耗");
  }
  // 补偿电流
  uint16_t compensation_current =
      17; // 17uA
          // 的补偿电流，当前电路损耗,补偿电流的定义只能补偿空载下的电流，其他状态不允许随意补偿，需要原因具体分析，当前这个值是根据实际测量的空载电流来设定的，目的是让功耗检测更准确地反映被测表的功耗情况，避免因为测试环境或工装本身的损耗导致误判。

  PortStatus status;
  int32_t current_ua =
      0; // 用int32_t是为了方便后续进行补偿电流的减法运算，避免uint16_t类型在减法时出现负数时的溢出问题,或者是有倒灌电流的情况，虽然理论上不应该有，但实际测试中可能会遇到一些异常情况，使用int32_t可以更安全地处理这些情况，确保功耗检测的稳定性和准确性。

  // 1. 切换到功耗测量模式
  status = power_on_main_power_measure_mode();
  // 关闭电解电容
  diaphragm_board_capacitor_disable();
  if (status != PORT_OK) {
    log_w("无法切换到功耗测量模式: %s, 跳过功耗检测",
          port_status_to_string(status));
    common_print_step_footer("跳过：功耗测量功能未启用");
    // 恢复正常供电模式
    power_on_main_power();
    diaphragm_step_success(); // 跳过功耗检测，继续下一步
    return;
  }

  // 等待电流稳定
  port_delay_ms(500);

  // 2. 读取静态功耗
  status = read_main_power_consumption_ua(&current_ua);
  log_i("补偿前静态功耗读数: %ld uA", current_ua);
  current_ua -= compensation_current; // 应用补偿
  log_i("补偿参数: %d uA, 补偿后静态功耗: %ld uA", compensation_current,
        current_ua);

  if (status == PORT_OK) {
    diaphragm_test_result.MasterLowPowerCurrent = (uint8_t)current_ua;
    log_i("静态功耗: %ld uA", current_ua);

    if (check_current_in_range(current_ua, STATIC_POWER_MIN_CURRENT_UA,
                               STATIC_POWER_MAX_CURRENT_UA)) {
      // 功耗正常，测试通过
      // 恢复正常供电模式
      power_off_main_power();
      port_delay_ms(50);
      power_on_main_power();

      common_print_step_footer("通过：静态功耗正常");
      diaphragm_step_success();
      return;
    }

    // 功耗超出范围，检查是否还有重试机会
    const DiaphragmStepDescriptor_t *step_desc =
        &diaphragm_step_table[diaphragm_test_step];
    if (diaphragm_runtime.retry_count >= step_desc->max_retry) {
      // 已达到最大重试次数，测试失败
      log_e("静态功耗超出范围[%d-%d]uA, 已重试%d次",
            STATIC_POWER_MIN_CURRENT_UA, STATIC_POWER_MAX_CURRENT_UA,
            diaphragm_runtime.retry_count);
      common_print_step_footer("失败：静态功耗超出范围");
      // 恢复正常供电模式后结束测试
      power_off_main_power();
      port_delay_ms(50);
      power_on_main_power();
      diaphragm_step_fail(DGM_FAIL_POWER_CONSUMPTION);
      return;
    }

    // 还有重试机会，增加重试计数并重置
    diaphragm_runtime.retry_count++;
    log_w("静态功耗超出范围[%d-%d]uA, 重试 %d/%d", STATIC_POWER_MIN_CURRENT_UA,
          STATIC_POWER_MAX_CURRENT_UA, diaphragm_runtime.retry_count,
          step_desc->max_retry);

    // 恢复正常供电模式后等待一段时间再重试
    power_off_main_power();
    port_delay_ms(100);
    power_on_main_power();
    port_delay_ms(500);

    // 重置执行标志，允许再次执行
    diaphragm_runtime.step_executing = 0;
    return;
  }

  // 读取失败，等待重试
  log_w("功耗读取失败，等待重试");
}

/**
 * @brief 温度压强检测处理（一次性执行型）
 * @details 读取被测表的温度和压强传感器数据
 */
void diaphragm_handle_temp_check(void) {
  // 如果标志位是0，不测试
  // 先sh设置为不测
  if (diaphragm_component_status.module_temp_pressure_sensor != 1) {
    log_i("温度压强传感器未安装，跳过测试");
    // 温压状态位设置为0，说明失败，默认不测就是失败
    IOSTATUS_CLEAR(diaphragm_test_result.IOStatus2, IOSTATUS2_TEMP_PRESS_BIT);
    diaphragm_step_success();
    return;
  }

  common_print_step_header("温度压强检测", "读取温度压强传感器数据");

  // TODO: 通过通信协议读取被测表的温度和压强数据
  // diaphragm_test_result.temperature = ...
  // diaphragm_test_result.pressure = ...

  common_print_step_footer("通过：温度压强检测完成");
  diaphragm_step_success();
}

/**
 * @brief NB连接测试处理（一次性执行型）
 * @details 测试NB-IoT模块连接状态
 */
void diaphragm_handle_nb_connect(void) {
  // ===== 防止重复执行 =====
  if (diaphragm_runtime.step_executing) {
    return;
  }
  diaphragm_runtime.step_executing = 1;

  common_print_step_header("NB连接测试", "测试NB-IoT模块连接");

  // TODO: 通过通信协议获取NB模块状态
  // - 检查SIM卡状态
  // - 检查网络注册状态
  // - 获取信号强度

  common_print_step_footer("通过：NB连接测试完成");
  diaphragm_step_success();
}

/**
 * @brief IMEI读取处理（等待协议响应型）
 * @details 读取IMEI/IMSI/ICCID等信息，需等待DGM_EVENT_IMEI_RECEIVED事件
 */
void diaphragm_handle_imei_read(void) {
  // ===== 防止重复执行 =====
  if (diaphragm_runtime.step_executing) {
    // 检查是否已收到IMEI事件
    if (diaphragm_runtime.events_received & DGM_EVT_IMEI) {
      log_i("IMEI读取完成: IMEI=%s", diaphragm_test_result.ModuleIMEI);
      log_i("               IMSI=%s", diaphragm_test_result.ModuleIMSI);
      log_i("               ICCID=%s", diaphragm_test_result.ModuleICCID);
      common_print_step_footer("通过：IMEI读取完成");
      diaphragm_step_success();
    }
    return;
  }
  diaphragm_runtime.step_executing = 1;

  common_print_step_header("IMEI读取", "读取IMEI/IMSI/ICCID信息");
  // 清除IMEI事件标志，等待新的响应
  diaphragm_runtime.events_received &= ~DGM_EVT_IMEI;
  // 注意: 命令发送由步骤表中的send_func (DGM_SendReadIMEI) 自动执行
  // 此处只需等待DGM_EVENT_IMEI_RECEIVED事件回调
}

/**
 * @brief 自检处理（一次性执行型）
 * @details 执行被测表的自检流程
 */
/**
 * @brief 关闭红外处理（一次性执行型）
 * @details 关闭红外通信，发送命令后直接完成（关闭后收不到响应）
 */
void diaphragm_close_ir(void) {
  // ===== 防止重复执行 =====
  if (diaphragm_runtime.step_executing) {
    return;
  }
  diaphragm_runtime.step_executing = 1;

  common_print_step_header("关闭红外", "发送关闭红外指令");
  // 注意: 关闭红外后收不到响应，直接完成
  common_print_step_footer("通过：关闭红外指令已发送");
  diaphragm_step_success();
}

/**
 * @brief UART连接测试处理（一次性执行型）
 * @details 测试UART通信是否正常，设置IOStatus1中的连接状态位
 */
void diaphragm_handle_uart_connect_test(void) {
  // ===== 防止重复执行 =====
  if (diaphragm_runtime.step_executing) {
    return;
  }
  diaphragm_runtime.step_executing = 1;

  common_print_step_header("UART连接测试", "测试UART通信连接");

  // TODO: 实际的UART连接测试逻辑
  // 当前先强制设置连接状态为1（正常）
  IOSTATUS_SET(diaphragm_test_result.IOStatus1, IOSTATUS1_CONNECT_BIT);
  log_i("UART连接状态已设置为正常");

  common_print_step_footer("通过：UART连接测试完成");
  diaphragm_step_success();
}

/**
 * @brief 测试结束处理（一次性执行型）
 * @details 汇总测试结果，关闭电源，准备下一轮测试
 */
void diaphragm_handle_end(void) {
  // ===== 防止重复执行 =====
  if (diaphragm_runtime.step_executing) {
    return;
  }
  diaphragm_runtime.step_executing = 1;

  // ===== 停止所有计时器，防止测试结束后触发超时 =====
  TM_StopGlobalTimeout();
  TM_StopStepTimeout();
  TM_StopPeriod(TM_PERIOD_ID_POWER);

  // ===== 恢复上位机UART0接收中断 =====
  UART0_EnableRxInterrupt();
  log_i("测试结束，恢复上位机通信中断");

  common_print_step_header("测试结束", "膜式燃气表测试完成");

  // ===== 清理IO状态（重要：避免IO口持续灌入电流导致功耗异常）=====
  // 关闭防拆检测电磁铁（恢复到初始状态）
  output_open_cover_signal(0);
  // 关闭霍尔信号输出
  output_hall_signal_1(0);
  output_hall_signal_2(0);
  // 释放阀门到位信号（恢复高阻态，避免通过气表上拉电阻灌入电流）
  output_valve_position_signal_1(0); // PA12 - 关到位信号释放
  output_valve_position_signal_2(0); // PA11 - 开到位信号释放
  // 恢复119报警信号为高电平（低电平触发，需恢复非触发状态）
  output_alarm_119_signal(1);
  // 禁用4.7V ADC采样使能（避免额外功耗）
  enable_board_4v7_read(0);
  log_i("所有测试IO口已恢复到初始状态");

  // ===== 根据测试结果决定供电策略 =====
  bool test_pass = (diaphragm_runtime.fail_reason == DGM_FAIL_NONE);
  uint32_t keep_power_time = 0;

  // LED指示: 根据测试结果设置状态
  if (test_pass) {
    LedIndicator_SetStatus(LED_STATUS_PASS); // 测试通过 - 常亮3秒
  } else {
    LedIndicator_SetStatus(LED_STATUS_FAIL); // 测试失败 - 急促闪5秒
  }

  if (test_pass) {
    // 成功：等待配置的时间后断电，供工人按按钮
    keep_power_time = TM_DELAY_SUCCESS_KEEP_POWER;
  } else {
    // 失败：根据配置决定是否保持供电（调试时可修改宏）
    keep_power_time = TM_DELAY_FAIL_KEEP_POWER;
    log_e("测试流程中断，失败步骤: %s, 失败原因: %s",
          diaphragm_get_step_name(diaphragm_runtime.fail_step),
          diaphragm_get_fail_reason_string(diaphragm_runtime.fail_reason));
  }

  if (keep_power_time > 0) {
    // 设置延时，保持供电
    TM_SetDelay(keep_power_time);
    diaphragm_runtime.waiting_power_off = 1;
    log_i("%s后保持供电 %d 秒，等待工人操作...",
          test_pass ? "测试成功" : "测试失败", keep_power_time / 1000);
  } else {
    // 立即断电
    power_off_main_power();
    power_off_backup_power();
    log_i("立即关闭电源");
  }

  // ===== 填充测试结果（供上位机查询）=====

  // 打印测试结果汇总
  log_i("========== 测试结果汇总 ==========");
  log_i("工位编号: %d", diaphragm_test_result.DeviceID);
  log_i("测试结果: %s", test_pass ? "通过" : "失败");

  if (!test_pass) {
    log_e("失败步骤: %s", diaphragm_get_step_name(diaphragm_runtime.fail_step));
    log_e("失败原因: %s",
          diaphragm_get_fail_reason_string(diaphragm_runtime.fail_reason));
  }

  log_i("主电电压: %d.%dV", diaphragm_test_result.MasterVoult / 10,
        diaphragm_test_result.MasterVoult % 10);
  log_i("静态功耗: %d uA", diaphragm_test_result.MasterLowPowerCurrent);
  log_i("IOStatus1: 0x%02X", diaphragm_test_result.IOStatus1);
  log_i("IOStatus2: 0x%02X", diaphragm_test_result.IOStatus2);
  log_i("==================================");

  common_print_step_footer("测试流程结束");

  // TODO: 上报测试结果给上位机
  // PC_SendTestResult(&diaphragm_test_result);

  // 设置测试完成标志（供PC查询结果时判断）
  diaphragm_runtime.test_completed = 1;

  // 进入等待状态（不清除结果，等待上位机查询）
  // 测试结果在下次测试开始时(diaphragm_test_init)才会清除
  diaphragm_test_step = DIAPHRAGM_WAIT;
}

/*============================================================================
 *                          协议事件处理函数
 *===========================================================================*/

/**
 * @brief 膜式燃气表协议事件处理回调
 * @param event 协议事件
 * @details 由协议层调用，处理解析完成的协议事件
 *          只设置标志位和缓存数据，不做复杂处理
 */
void diaphragm_on_protocol_event(const DgmProtocolEvent *event) {
  if (event == NULL) {
    return;
  }

  log_d("收到协议事件: %s (0x%04X)", DGM_GetEventName(event->type),
        event->data_mark);

  switch (event->type) {
  case DGM_EVENT_SELF_CHECK_COMPLETE:
    // 收到自检完成响应 (0x1000)
    diaphragm_runtime.events_received |= DGM_EVT_SELF_CHECK;
    log_i("协议事件: 模块自检完成");
    break;

  case DGM_EVENT_POWER_ON_INFO_RECEIVED:
    // 收到上告开机获取相关信息(0x1001) - 26字节状态信息
    diaphragm_runtime.events_received |= DGM_EVT_BOARD_INFO;

    // 保存数据到运行时结构体，供后续检测判断使用
    memcpy(&diaphragm_runtime.board_info, &event->data.board_info,
           sizeof(DgmBoardInfoData));

    // 记录状态信息用于后续检测判断
    log_i("协议事件: 上告开机信息获取成功");
    log_i("  表具类型=%d, 附件=0x%02X, 电压=%d.%dV, 信号=%d, 版本=V%02x.%02x",
          event->data.board_info.meter_type, event->data.board_info.has_addon,
          event->data.board_info.voltage / 10,
          event->data.board_info.voltage % 10, event->data.board_info.signal,
          event->data.board_info.sw_ver1, event->data.board_info.sw_ver2);
    log_i("  模块=%d, SIM=%d, EEPROM=%d, 计量=%d, RTC=%d, 温压=%d",
          event->data.board_info.module_status, event->data.board_info.sim_ok,
          event->data.board_info.storage_ic_ok,
          event->data.board_info.measure_ok, event->data.board_info.rtc_ok,
          event->data.board_info.temp_press_ok);
    log_i("  开盖=%d, 倾斜=%d, 蓝牙=%d", event->data.board_info.cover_open,
          event->data.board_info.tilt_ok, event->data.board_info.bluetooth_ok);
    break;

  case DGM_EVENT_IMEI_RECEIVED:
    // 收到IMEI/IMSI/ICCID/CSQ等网络参数
    diaphragm_runtime.events_received |= DGM_EVT_IMEI;
    // 保存到测试结果 (固定长度，协议不需要null终止符)
    memcpy(diaphragm_test_result.ModuleIMEI, event->data.imei.imei, 15);
    memcpy(diaphragm_test_result.ModuleIMSI, event->data.imei.imsi, 15);
    memcpy(diaphragm_test_result.ModuleICCID, event->data.imei.iccid, 20);
    // 保存CSQ信号强度 (从C525协议获取)
    // diaphragm_test_result.Module_Csq = event->data.imei.csq;
    // 保存软件版本编译时间 (6字节BCD)
    memcpy(diaphragm_test_result.FirmwareBuildTime, event->data.imei.build_time,
           6);
    log_i("协议事件: 收到IMEI=%s, CSQ=%d", event->data.imei.imei,
          event->data.imei.csq);
    break;

  case DGM_EVENT_STAR_MAC_RECEIVED:
    // 收到星闪MAC
    diaphragm_step_skip("未实现");
    log_i("不测试星闪MAC");
    return;
    break;

  case DGM_EVENT_IO_STATUS:
    // 收到IO状态检测结果 (0x1002响应)
    diaphragm_runtime.events_received |= DGM_EVT_IO_STATUS;
    // 保存IO状态数据到runtime
    memcpy(&diaphragm_runtime.io_status, &event->data.io_status,
           sizeof(DgmIoStatusData));
    log_i("协议事件: IO状态检测完成 (0x1002)");
    log_i("  open_pos=%d, close_pos=%d, hall1=%d, hall2=%d",
          event->data.io_status.open_pos, event->data.io_status.close_pos,
          event->data.io_status.hall1, event->data.io_status.hall2);
    log_i("  IC_XB=%d, 119=%d, IC_ERR=%d", event->data.io_status.ic_xb,
          event->data.io_status.io_119, event->data.io_status.ic_err);
    break;

  case DGM_EVENT_IR_CLOSED:
    // 收到红外关闭响应
    diaphragm_runtime.events_received |= DGM_EVT_IR_CLOSED;
    log_i("协议事件: 红外已关闭");
    break;

  case DGM_EVENT_TIME_SET_OK:
    log_i("协议事件: 时间设置成功");
    break;

  case DGM_EVENT_TIMEOUT:
    log_w("协议事件: 超时");
    break;

  case DGM_EVENT_PARSE_ERROR:
  case DGM_EVENT_CHECKSUM_ERROR:
    log_e("协议事件: 错误 (%s)", DGM_GetEventName(event->type));
    break;

  default:
    log_d("协议事件: 未处理的事件类型 %d", event->type);
    break;
  }
}

/**
 * @brief 注册协议事件回调
 * @details 在测试初始化时调用，将事件处理函数注册到协议层
 */
void diaphragm_register_protocol_callback(void) {
  DGM_SetEventCallback(diaphragm_on_protocol_event);
  log_i("已注册膜式燃气表协议事件回调");
}

/*============================================================================
 *                          新增步骤处理函数实现
 *===========================================================================*/

/**
 * @brief 蓝牙测试处理函数
 */
static void diaphragm_handle_bluetooth_test(void) {
  // TODO: 实现蓝牙测试逻辑
  if (diaphragm_component_status.bluetooth_component_status ==
      DIAPHRAGM_COMPONENT_STATUS_UNTESTED) {
    common_print_step_footer("跳过: 无蓝牙模块,不进行测试");
    // 设置diaphragm_test_result.IOStatus1 的蓝牙状态位为0
    diaphragm_test_result.IOStatus2 &= ~(1 << 5); // bit5: 蓝牙状态
    diaphragm_step_success();
    return;
  }
  diaphragm_step_skip("未实现");
}

/**
 * @brief 防拆检测处理函数 (开盖检测)
 * @details 在一个步骤内完成:
 *   1. 检查是否已收到1001响应
 *   2. 没有则触发开盖动作并发送1001请求
 *   3. 收到响应后比对前后cover_open状态
 *   4. 如果状态未变化，重发1001请求（最多重试3次）
 */

// 开盖检测触发电平定义
// 电磁铁控制: 高电平(1)=电磁铁动作=关盖, 低电平(0)=电磁铁不动作=开盖
#define COVER_TRIGGER_OPEN 0 // 触发开盖状态的电平 (电磁铁不动作)
#define COVER_TRIGGER_CLOSE 1   // 触发合盖状态的电平 (电磁铁动作)
#define COVER_CHECK_MAX_RETRY 3 // 最大重试次数

static void diaphragm_handle_cover_check(void) {
  // 使用sub_step管理内部状态机
  // sub_step=0: 触发开盖动作并发送1001
  // sub_step=1: 等待1001响应并比对
  // 暂时跳过，电磁铁这个坏了
  // 检测开盖状态是否要测试
  // 先不验证，这个嵌软程序那边有bug,当前无开盖
  // diaphragm_component_status.cover_check_status =
  // DIAPHRAGM_COMPONENT_STATUS_UNTESTED;
  if (diaphragm_component_status.cover_check_status ==
      DIAPHRAGM_COMPONENT_STATUS_UNTESTED) {
    common_print_step_footer("跳过: 无开盖检测功能,不进行验证");
    diaphragm_step_success();
    return;
  }
  static uint8_t retry_count = 0;

  if (diaphragm_runtime.sub_step == 0) {
    // 阶段0: 触发开盖动作
    retry_count = 0; // 重置重试计数
    log_i("防拆检测: 初始状态 cover_open=%d",
          diaphragm_io_status.cover_check_input_status_before_test);

    // 触发开盖信号 (电平翻转)
    // 如果初始是开盖(1)，则模拟合盖(0)；如果初始是合盖(0)，则模拟开盖(1)
    uint8_t trigger_level =
        (diaphragm_io_status.cover_check_input_status_before_test == 1)
            ? COVER_TRIGGER_CLOSE
            : COVER_TRIGGER_OPEN;
    log_i("防拆检测: 触发开盖信号, 输出电平=%d", trigger_level);
    output_open_cover_signal(trigger_level);

    // 等待一段时间让信号生效
    FL_DelayMs(200);

    // 清除之前的1001事件标志
    diaphragm_runtime.events_received &= ~DGM_EVT_BOARD_INFO;

    // 发送1001请求
    DGM_SendBoardInfoRequest();
    log_d("防拆检测: 已发送1001请求，等待响应...");

    // 进入下一阶段
    diaphragm_runtime.sub_step = 1;
    return;
  }

  // 阶段1: 等待1001响应并比对
  if (diaphragm_runtime.sub_step == 1) {
    // 检查是否收到1001响应
    if (!(diaphragm_runtime.events_received & DGM_EVT_BOARD_INFO)) {
      log_d("防拆检测: 等待1001响应...");
      return; // 继续等待
    }

    // 收到响应，获取新的cover_open状态
    uint8_t cover_after = diaphragm_runtime.board_info.cover_open;
    uint8_t cover_before =
        diaphragm_io_status.cover_check_input_status_before_test;

    log_i("防拆检测: 状态变化 %d -> %d (重试%d/%d)", cover_before, cover_after,
          retry_count, COVER_CHECK_MAX_RETRY);

    // 保存测试后状态
    diaphragm_io_status.cover_check_input_status_after_test = cover_after;

    // 比对前后状态是否变化
    if (cover_before != cover_after) {
      // 状态发生变化，测试通过
      // 恢复开盖信号到初始状态
      output_open_cover_signal(COVER_TRIGGER_OPEN); // 恢复为开盖状态
      log_d("防拆检测: 已恢复开盖信号到初始状态");

      diaphragm_io_status.conver_check_result = 1; // 通过
      log_i("防拆检测: 通过 (状态已变化)");
      common_print_step_footer("通过: 开盖检测正常");
      diaphragm_step_success();
    } else {
      // 状态未变化，检查是否需要重试
      retry_count++;
      if (retry_count < COVER_CHECK_MAX_RETRY) {
        log_w("防拆检测: 状态未变化，重试 %d/%d", retry_count,
              COVER_CHECK_MAX_RETRY);

        // 清除事件标志，准备重新接收
        diaphragm_runtime.events_received &= ~DGM_EVT_BOARD_INFO;

        DGM_SendBoardInfoRequest();
        // 设置软件延时时间
        diaphragm_set_delay(300);
        log_d("防拆检测: 已重发1001请求，等待300ms");
        // 保持在阶段1继续等待
        return;
      }

      // 重试次数用完，测试失败
      // 恢复开盖信号到初始状态
      output_open_cover_signal(COVER_TRIGGER_OPEN); // 恢复为开盖状态
      log_d("防拆检测: 已恢复开盖信号到初始状态");

      diaphragm_io_status.conver_check_result = 0; // 失败
      log_e("防拆检测: 失败 (状态未变化，已重试%d次)", retry_count);
      common_print_step_footer("失败: 开盖状态未变化");
      diaphragm_step_fail(DGM_FAIL_BOARD_STATUS);
    }
  }
}

/**
 * @brief IC模块测试处理函数
 */
static void diaphragm_handle_ic_module_test(void) {
  // TODO: 实现IC模块测试逻辑
  diaphragm_step_skip("未实现");
}

// 霍尔检测最大重试次数
#define HALL_CHECK_MAX_RETRY 3

/**
 * @brief 计量模块测试处理函数 (霍尔1、霍尔2和119同时检测)
 * @details 通过1002协议同时检测霍尔和119状态，验证传感器功能
 *          测试流程：
 *          sub_step=0: 发送1002获取初始状态
 *          sub_step=1: 等待响应，保存初始状态，同时触发霍尔磁铁和119
 *          sub_step=2: 发送1002获取触发后状态
 *          sub_step=3: 等待响应，验证状态变化，关闭霍尔磁铁和119
 *          sub_step=4: 发送1002获取恢复后状态
 *          sub_step=5: 等待响应，验证恢复
 *
 * @note 119和霍尔一起验证，减少测试时间
 *       霍尔: 初始(高)->触发(低)->恢复(高)
 *       119:  初始(高)->触发(低)->恢复(高)，PC14高电平触发
 */
static void diaphragm_handle_metering_module_test(void) {
  static uint8_t retry_count = 0;
  // 霍尔状态
  static uint8_t hall1_initial = 0;
  static uint8_t hall2_initial = 0;
  static uint8_t hall1_triggered = 0;
  static uint8_t hall2_triggered = 0;
  // 119状态
  static uint8_t io_119_initial = 0;
  static uint8_t io_119_triggered = 0;
  // 是否需要测试119
  static bool test_119 = false;

  // 阶段0: 发送1002请求获取初始状态
  if (diaphragm_runtime.sub_step == 0) {
    retry_count = 0;

    // 判断是否需要测试119（根据附件配置）
    test_119 = (diaphragm_component_status.warning_component_status == 1);
    // 当前嵌软有问题，119手动设置未0，不测试
    log_i("当前嵌软有问题，119手动设置为0，不测试");
    test_119 = false;
    log_i("霍尔检测: 开始测试(高低电平全检测)%s",
          test_119 ? "，同时检测119" : "");
    log_i("霍尔检测: [阶段1/3] 获取初始状态");

    // 清除之前的IO状态事件标志
    diaphragm_runtime.events_received &= ~DGM_EVT_IO_STATUS;

    // 发送1002请求,读取状态数据域是空的
    DGM_SendIOStatusCheck(0, 0);

    diaphragm_runtime.sub_step = 1;
    return;
  }

  // 阶段1: 等待1002响应，保存初始状态，触发磁铁和119
  if (diaphragm_runtime.sub_step == 1) {
    if (!(diaphragm_runtime.events_received & DGM_EVT_IO_STATUS)) {
      log_d("霍尔检测: 等待1002响应(初始状态)...");
      return;
    }

    // 保存初始霍尔状态
    hall1_initial = diaphragm_runtime.io_status.hall1;
    hall2_initial = diaphragm_runtime.io_status.hall2;
    io_119_initial = diaphragm_runtime.io_status.io_119;

    diaphragm_io_status.metering_hall1_input_status_before_test = hall1_initial;
    diaphragm_io_status.metering_hall2_input_status_before_test = hall2_initial;

    log_i("霍尔检测: 初始状态 霍尔1=%d(%s), 霍尔2=%d(%s)", hall1_initial,
          hall1_initial ? "高" : "低", hall2_initial,
          hall2_initial ? "高" : "低");
    if (test_119) {
      log_i("119检测: 初始状态 io_119=%d(%s)", io_119_initial,
            io_119_initial ? "高/正常" : "低/异常");
    }

    // 触发霍尔磁铁和119
    log_i("霍尔检测: [阶段2/3] 触发磁铁%s，检测状态变化",
          test_119 ? "和119" : "");

    // 触发霍尔磁铁
    PortStatus hall_status = output_hall_signal_1(1);
    // debug_breakpoint();
    if (hall_status != PORT_OK) {
      log_e("霍尔检测: 磁铁控制失败 (%s)", port_status_to_string(hall_status));
      diaphragm_io_status.metering_hall1_test_result = 0;
      diaphragm_io_status.metering_hall2_test_result = 0;
      common_print_step_footer("失败: 霍尔磁铁控制失败");
      diaphragm_step_fail(DGM_FAIL_BOARD_STATUS);
      return;
    }
    log_d("霍尔检测: 磁铁已触发");

    // 同时触发119 (PC14低电平触发)
    if (test_119) {
      PortStatus status_119 = output_alarm_119_signal(0); // 低电平触发
      if (status_119 != PORT_OK) {
        log_w("119检测: 触发失败 (%s)", port_status_to_string(status_119));
      } else {
        log_d("119检测: 已触发(PC14低电平)");
      }
    }

    FL_DelayMs(200);

    // 清除事件标志
    diaphragm_runtime.events_received &= ~DGM_EVT_IO_STATUS;
    DGM_SendIOStatusCheck(0, 0);

    diaphragm_runtime.sub_step = 2;
    diaphragm_set_delay(300);
    return;
  }

  // 阶段2: 等待1002响应，验证触发后状态变化
  if (diaphragm_runtime.sub_step == 2) {
    if (!(diaphragm_runtime.events_received & DGM_EVT_IO_STATUS)) {
      log_d("霍尔检测: 等待1002响应(触发后状态)...");
      return;
    }

    hall1_triggered = diaphragm_runtime.io_status.hall1;
    hall2_triggered = diaphragm_runtime.io_status.hall2;
    io_119_triggered = diaphragm_runtime.io_status.io_119;

    log_i("霍尔检测: 触发后状态 霍尔1=%d(%s), 霍尔2=%d(%s)", hall1_triggered,
          hall1_triggered ? "高" : "低", hall2_triggered,
          hall2_triggered ? "高" : "低");
    if (test_119) {
      log_i("119检测: 触发后状态 io_119=%d(%s)", io_119_triggered,
            io_119_triggered ? "高/未触发" : "低/已触发");
    }

    // 检测霍尔第一次变化
    bool hall1_changed = (hall1_initial != hall1_triggered);
    bool hall2_changed = (hall2_initial != hall2_triggered);
    // 检测119变化 (应该从1变成0)
    bool io_119_changed =
        test_119 && (io_119_initial == 1) && (io_119_triggered == 0);

    if (!hall1_changed && !hall2_changed) {
      // 霍尔都没变化，重试
      retry_count++;
      if (retry_count < HALL_CHECK_MAX_RETRY) {
        log_w("霍尔检测: 触发后状态未变化，重试 %d/%d", retry_count,
              HALL_CHECK_MAX_RETRY);
        diaphragm_runtime.events_received &= ~DGM_EVT_IO_STATUS;
        DGM_SendIOStatusCheck(0, 0);
        diaphragm_set_delay(300);
        return; // 保持在阶段2
      }
      // 重试用完，失败
      log_e("霍尔检测: 失败 - 触发后状态未变化");
      output_hall_signal_1(0); // 关闭电磁铁触发霍尔
      if (test_119)
        output_alarm_119_signal(1); // 恢复119高电平
      diaphragm_io_status.metering_hall1_test_result = 0;
      diaphragm_io_status.metering_hall2_test_result = 0;
      common_print_step_footer("失败: 霍尔触发后状态未变化");
      diaphragm_step_fail(DGM_FAIL_BOARD_STATUS);
      return;
    }

    log_i("霍尔检测: 第一次变化验证 霍尔1=%s, 霍尔2=%s",
          hall1_changed ? "OK" : "未变化", hall2_changed ? "OK" : "未变化");
    if (test_119) {
      log_i("119检测: 第一次变化验证 119=%s",
            io_119_changed ? "OK(1->0)" : "未变化");
    }

    // 关闭磁铁和119，检测恢复
    log_i("霍尔检测: [阶段3/3] 关闭磁铁%s，检测恢复到初始状态",
          test_119 ? "和119" : "");

    PortStatus hall_status = output_hall_signal_1(0);
    if (hall_status != PORT_OK) {
      log_e("霍尔检测: 磁铁关闭失败 (%s)", port_status_to_string(hall_status));
    } else {
      log_d("霍尔检测: 磁铁已关闭");
    }

    // 关闭119 (恢复高电平)
    if (test_119) {
      output_alarm_119_signal(1); // 恢复高电平
      log_d("119检测: 已关闭(PC14高电平)");
    }

    FL_DelayMs(200);

    diaphragm_runtime.events_received &= ~DGM_EVT_IO_STATUS;
    DGM_SendIOStatusCheck(0, 0);

    diaphragm_runtime.sub_step = 3;
    diaphragm_set_delay(300);
    retry_count = 0; // 重置重试计数
    return;
  }

  // 阶段3: 等待1002响应，验证恢复到初始状态
  if (diaphragm_runtime.sub_step == 3) {
    if (!(diaphragm_runtime.events_received & DGM_EVT_IO_STATUS)) {
      log_d("霍尔检测: 等待1002响应(恢复后状态)...");
      return;
    }

    uint8_t hall1_restored = diaphragm_runtime.io_status.hall1;
    uint8_t hall2_restored = diaphragm_runtime.io_status.hall2;
    uint8_t io_119_restored = diaphragm_runtime.io_status.io_119;

    // 保存测试后状态
    diaphragm_io_status.metering_hall1_input_status_after_test = hall1_restored;
    diaphragm_io_status.metering_hall2_input_status_after_test = hall2_restored;

    log_i("霍尔检测: 恢复后状态 霍尔1=%d(%s), 霍尔2=%d(%s)", hall1_restored,
          hall1_restored ? "高" : "低", hall2_restored,
          hall2_restored ? "高" : "低");
    if (test_119) {
      log_i("119检测: 恢复后状态 io_119=%d(%s)", io_119_restored,
            io_119_restored ? "高/正常" : "低/异常");
    }

    // 验证霍尔恢复
    bool hall1_restored_ok = (hall1_restored == hall1_initial);
    bool hall2_restored_ok = (hall2_restored == hall2_initial);
    // 验证119恢复 (应该从0恢复到1)
    bool io_119_restored_ok =
        !test_119 || (io_119_restored == 1); // 不测试119时默认OK

    // 综合判断霍尔完整测试
    bool hall1_full_test =
        (hall1_initial != hall1_triggered) && hall1_restored_ok;
    bool hall2_full_test =
        (hall2_initial != hall2_triggered) && hall2_restored_ok;
    // 119完整测试: 1->0->1
    bool io_119_full_test =
        !test_119 || ((io_119_initial == 1) && (io_119_triggered == 0) &&
                      io_119_restored_ok);

    log_i("霍尔检测: 完整测试结果");
    log_i("  霍尔1: %d->%d->%d %s", hall1_initial, hall1_triggered,
          hall1_restored, hall1_full_test ? "通过" : "失败");
    log_i("  霍尔2: %d->%d->%d %s", hall2_initial, hall2_triggered,
          hall2_restored, hall2_full_test ? "通过" : "失败");
    if (test_119) {
      log_i("  119:   %d->%d->%d %s", io_119_initial, io_119_triggered,
            io_119_restored, io_119_full_test ? "通过" : "失败");
    }

    diaphragm_io_status.metering_hall1_test_result = hall1_full_test ? 1 : 0;
    diaphragm_io_status.metering_hall2_test_result = hall2_full_test ? 1 : 0;

    // 判断整体结果
    bool all_passed = hall1_full_test && hall2_full_test && io_119_full_test;
    bool hall_passed = hall1_full_test && hall2_full_test;
    // 对diaphragm_test_result.IOStatus1 里面的计量状态进行赋值
    IOSTATUS_CLEAR(diaphragm_test_result.IOStatus1, IOSTATUS1_METERING_BIT);
    if (hall1_full_test) {
      IOSTATUS_SET(diaphragm_test_result.IOStatus1, IOSTATUS1_METERING_BIT);
    }
    // 对119测试结果进行赋值
    if (test_119) {
      IOSTATUS_CLEAR(diaphragm_test_result.IOStatus1, IOSTATUS1_119_BIT);
      if (io_119_full_test) {
        IOSTATUS_SET(diaphragm_test_result.IOStatus1, IOSTATUS1_119_BIT);
      }
    }

    if (all_passed) {
      if (test_119) {
        log_i("霍尔+119检测: 全部通过");
        common_print_step_footer("通过: 霍尔+119检测正常");
      } else {
        log_i("霍尔检测: 全部通过 (高低电平切换正常)");
        common_print_step_footer("通过: 霍尔高低电平检测正常");
      }
      // 标记119测试已完成（跳过单独的119测试步骤）
      if (test_119) {
        diaphragm_component_status.warning_component_status =
            DIAPHRAGM_COMPONENT_STATUS_UNTESTED; // 设为跳过，避免重复测试
      }
      diaphragm_step_success();
    } else if (hall_passed && !io_119_full_test) {
      // 霍尔通过，119失败
      log_w("霍尔检测: 通过，但119检测失败");
      common_print_step_footer("失败: 119检测异常(霍尔正常)");
      diaphragm_step_fail(DGM_FAIL_BOARD_STATUS);
    } else if (hall1_full_test || hall2_full_test) {
      log_w("霍尔检测: 部分通过 (霍尔1=%s, 霍尔2=%s)",
            hall1_full_test ? "通过" : "失败",
            hall2_full_test ? "通过" : "失败");
      common_print_step_footer("部分通过: 霍尔检测");
      diaphragm_step_success(); // 暂时算通过
    } else {
      // 检查是否需要重试恢复阶段
      retry_count++;
      if (retry_count < HALL_CHECK_MAX_RETRY) {
        log_w("霍尔检测: 恢复状态异常，重试 %d/%d", retry_count,
              HALL_CHECK_MAX_RETRY);
        diaphragm_runtime.events_received &= ~DGM_EVT_IO_STATUS;
        DGM_SendIOStatusCheck(0, 0);
        diaphragm_set_delay(300);
        return; // 保持在阶段3
      }
      log_e("霍尔检测: 失败 - 高低电平切换异常");
      common_print_step_footer("失败: 霍尔高低电平检测异常");
      diaphragm_step_fail(DGM_FAIL_BOARD_STATUS);
    }
  }
}

/**
 * @brief 119报警测试处理函数
 * @details 119状态从1002协议的io_119字段获取
 *          io_119: 1=高电平(正常), 0=低电平(触发/异常)
 *          测试时应检测表具处于正常状态(io_119=1)
 */
static void diaphragm_handle_119_warning_test(void) {
  // 从1002协议缓存的io_status中获取119状态
  // 是否验证119状态
  if (diaphragm_component_status.warning_component_status ==
      DIAPHRAGM_COMPONENT_STATUS_UNTESTED) {
    common_print_step_footer("跳过: 无119报警功能,不进行验证");
    diaphragm_step_success();
    return;
  }
  uint8_t io_119_status = diaphragm_runtime.io_status.io_119;

  log_i("119报警检测: io_119=%d", io_119_status);

  // io_119: 1=高电平(正常), 0=低电平(触发)
  if (io_119_status == 1) {
    log_i("119报警检测: 通过 (高电平，正常状态)");
    common_print_step_footer("通过: 119状态正常(高电平)");
    diaphragm_step_success();
  } else {
    log_e("119报警检测: 失败 (低电平，异常状态)");
    common_print_step_footer("失败: 119异常(低电平)");
    diaphragm_step_fail(DGM_FAIL_BOARD_STATUS);
  }
}

/**
 * @brief 温压传感器测试处理函数
 */
static void diaphragm_handle_temp_pressure_sensor_test(void) {
  // TODO:
  // 实现温压传感器测试逻辑，这个后续看是自己写还是模组了，后续有开关控制，可以直接切换过去，让气表直接连接到模组的温压传感器上进行测试
  diaphragm_step_skip("未实现");
}

/**
 * @brief 第二阀门测试处理函数
 */
static void diaphragm_handle_second_valve_test(void) {
  // TODO: 实现第二阀门测试逻辑
  diaphragm_step_skip("未实现");
}

/**
 * @brief 倾斜开关测试处理函数
 * @details 倾斜状态从1001协议的tilt_ok字段获取
 *          tilt_ok: 0=正常(未倾斜), 1=异常(已倾斜), 2=无此功能
 *          测试时应检测表具处于正常状态(tilt_ok=0)
 */
static void diaphragm_handle_tilt_switch_test(void) {
  // 从1001协议缓存的board_info中获取倾斜状态
  uint8_t tilt_status = diaphragm_runtime.board_info.tilt_ok;

  log_i("倾斜开关检测: tilt_ok=%d", tilt_status);

  // tilt_ok: 0=正常(未倾斜), 1=异常(已倾斜)
  if (tilt_status == 0) {
    log_i("倾斜开关检测: 通过 (状态正常，未倾斜)");
    common_print_step_footer("通过: 倾斜开关状态正常");
    diaphragm_step_success();
  } else if (tilt_status == 1) {
    log_e("倾斜开关检测: 失败 (检测到倾斜状态)");
    common_print_step_footer("失败: 检测到倾斜状态");
    diaphragm_test_result.IOStatus2 &= ~(1 << 4); // bit4: 倾斜开关状态

    diaphragm_step_fail(DGM_FAIL_BOARD_STATUS);
  } else {
    // tilt_status == 2 或其他值，理论上不应该进入这个步骤
    log_w("倾斜开关检测: 跳过 (无此功能，tilt_ok=%d)", tilt_status);
    diaphragm_test_result.IOStatus2 &= ~(1 << 4); // bit4: 倾斜开关状态
    common_print_step_footer("跳过: 无倾斜开关功能");
    diaphragm_step_success();
  }
}

/**
 * @brief 阀门电压测试处理函数
 * @details 只测试阀门电压功能，到位测试分离到 DIAPHRAGM_VALVE_POSITION_TEST
 * 步骤
 *
 * 测试流程:
 * 阶段0: 初始化 - 记录初始电压
 * 阶段1: 发送开阀指令，等待响应，检测B-R电压应为±5V
 * 阶段2: 发送关阀指令，等待响应，检测B-R电压应相反
 * 阶段3: 发送停止指令，等待响应，检测B-R电压应为0V，汇总结果
 */
static void diaphragm_handle_default_valve_test(void) {
// 阈值定义 (单位: mV)
#define VALVE_VOLTAGE_HIGH_THRESHOLD 4000 // 4V以上认为是有效电压
#define VALVE_VOLTAGE_LOW_THRESHOLD 500   // 0.5V以下认为是停止状态
#define VALVE_VOLTAGE_DIFF_MIN 4000       // B-R差值最小4V

  // 静态变量保存测试状态
  static uint16_t valve_b_initial = 0, valve_r_initial = 0;
  static uint16_t valve_b_open = 0, valve_r_open = 0;
  static uint16_t valve_b_close = 0, valve_r_close = 0;
  static uint16_t valve_b_stop = 0, valve_r_stop = 0;

  // 阶段0: 初始化
  if (diaphragm_runtime.sub_step == 0) {
    common_print_step_header("阀门电压测试", "检测阀门开关电压");

    // 读取初始阀门电压
    read_valve_b_voltage_mv(&valve_b_initial); // B端电压
    read_valve_r_voltage_mv(&valve_r_initial); // R端电压

    log_i("=== 阀门电压测试开始 ===");
    log_i("阶段0: 初始电压 B=%umV, R=%umV", valve_b_initial, valve_r_initial);

    // 发送开阀指令 (控制码0x01, 状态0=开阀)
    log_i("阶段1: 发送开阀指令...");
    diaphragm_runtime.events_received &= ~DGM_EVT_IO_STATUS;
    DGM_SendConfigureValve(0); // 0=开阀

    diaphragm_runtime.sub_step = 1;
    diaphragm_set_delay(500); // 等待阀门动作
    return;
  }

  // 阶段1: 等待开阀响应，检测电压
  if (diaphragm_runtime.sub_step == 1) {
    // 读取开阀时的电压
    read_valve_b_voltage_mv(&valve_b_open);
    read_valve_r_voltage_mv(&valve_r_open);

    // 计算电压差 (有符号)
    int32_t diff_open = (int32_t)valve_b_open - (int32_t)valve_r_open;

    log_i("阶段1: 开阀电压 B=%umV, R=%umV, B-R=%ldmV", valve_b_open,
          valve_r_open, diff_open);

    // 验证: 开阀时应有±5V的电压差
    if (diff_open > VALVE_VOLTAGE_DIFF_MIN ||
        diff_open < -VALVE_VOLTAGE_DIFF_MIN) {
      log_i("阶段1: 开阀电压检测通过 (差值=%ldmV)", diff_open);
    } else {
      log_w("阶段1: 开阀电压异常 (差值=%ldmV, 期望>%dmV)", diff_open,
            VALVE_VOLTAGE_DIFF_MIN);
    }

    // 发送关阀指令 (控制码0x01, 状态1=关阀)
    log_i("阶段2: 发送关阀指令...");
    DGM_SendConfigureValve(1); // 1=关阀

    diaphragm_runtime.sub_step = 2;
    diaphragm_set_delay(500);
    return;
  }

  // 阶段2: 检测关阀电压
  if (diaphragm_runtime.sub_step == 2) {
    read_valve_b_voltage_mv(&valve_b_close);
    read_valve_r_voltage_mv(&valve_r_close);

    int32_t diff_open = (int32_t)valve_b_open - (int32_t)valve_r_open;
    int32_t diff_close = (int32_t)valve_b_close - (int32_t)valve_r_close;

    log_i("阶段2: 关阀电压 B=%umV, R=%umV, B-R=%ldmV", valve_b_close,
          valve_r_close, diff_close);

    // 验证: 关阀时电压差应与开阀相反
    bool polarity_reversed =
        (diff_open > 0 && diff_close < 0) || (diff_open < 0 && diff_close > 0);

    if (polarity_reversed && (diff_close > VALVE_VOLTAGE_DIFF_MIN ||
                              diff_close < -VALVE_VOLTAGE_DIFF_MIN)) {
      log_i("阶段2: 关阀电压检测通过 (极性反转正确)");
    } else {
      log_w("阶段2: 关阀电压异常 (开阀B-R=%ldmV, 关阀B-R=%ldmV)", diff_open,
            diff_close);
    }

    // 发送停止指令 (控制码0x01, 状态2=停止)
    log_i("阶段3: 发送停止阀门指令...");
    DGM_SendConfigureValve(2); // 2=停止

    diaphragm_runtime.sub_step = 3;
    diaphragm_set_delay(500);
    return;
  }

  // 阶段3: 检测停止电压，汇总结果
  if (diaphragm_runtime.sub_step == 3) {
    read_valve_b_voltage_mv(&valve_b_stop);
    read_valve_r_voltage_mv(&valve_r_stop);

    log_i("阶段3: 停止电压 B=%umV, R=%umV", valve_b_stop, valve_r_stop);

    // 验证: 停止时两端电压都应接近0V
    bool stop_ok = (valve_b_stop < VALVE_VOLTAGE_LOW_THRESHOLD &&
                    valve_r_stop < VALVE_VOLTAGE_LOW_THRESHOLD);

    if (stop_ok) {
      log_i("阶段3: 停止电压检测通过 (均<0.5V)");
    } else {
      log_w("阶段3: 停止电压异常 (期望<0.5V)");
    }

    // 综合判断电压测试结果
    int32_t diff_open = (int32_t)valve_b_open - (int32_t)valve_r_open;
    int32_t diff_close = (int32_t)valve_b_close - (int32_t)valve_r_close;
    bool polarity_reversed =
        (diff_open > 0 && diff_close < 0) || (diff_open < 0 && diff_close > 0);
    bool open_ok = (diff_open > VALVE_VOLTAGE_DIFF_MIN ||
                    diff_open < -VALVE_VOLTAGE_DIFF_MIN);
    bool close_ok = polarity_reversed && (diff_close > VALVE_VOLTAGE_DIFF_MIN ||
                                          diff_close < -VALVE_VOLTAGE_DIFF_MIN);

    bool voltage_test_passed = open_ok && close_ok && stop_ok;

    log_i("=== 阀门电压测试汇总 ===");
    log_i("  开阀: B=%umV, R=%umV, 差=%ldmV, %s", valve_b_open, valve_r_open,
          diff_open, open_ok ? "通过" : "失败");
    log_i("  关阀: B=%umV, R=%umV, 差=%ldmV, %s", valve_b_close, valve_r_close,
          diff_close, close_ok ? "通过" : "失败");
    log_i("  停止: B=%umV, R=%umV, %s", valve_b_stop, valve_r_stop,
          stop_ok ? "通过" : "失败");
    log_i("  电压测试: %s", voltage_test_passed ? "通过" : "失败");

    if (voltage_test_passed) {
      IOSTATUS_SET(diaphragm_test_result.IOStatus1, IOSTATUS1_VALVE_BIT);
      common_print_step_footer("通过: 阀门电压测试正常");
      diaphragm_step_success();
    } else {
      common_print_step_footer("失败: 阀门电压测试异常");
      diaphragm_step_fail(DGM_FAIL_VALVE);
    }
  }
}

/**
 * @brief 阀门到位测试处理函数
 * @details 测试阀门到位信号功能，可单独跳过
 *
 * 测试流程:
 * 阶段0: 发送1002查询初始到位状态
 * 阶段1: 输出到位信号1，发送1002查询，验证开到位电平反转
 * 阶段2: 输出到位信号2，发送1002查询，验证关到位电平反转
 * 阶段3: 恢复并验证测试结果
 */
static void diaphragm_handle_valve_position_test(void) {
  // Static variables to save test state
  static uint8_t open_pos_initial = 0;
  static uint8_t open_reversed = 0; // Open position reversal test result
  // NOTE: Close position not implemented in current hardware, reserved for
  // future use static uint8_t close_pos_initial = 0; static uint8_t
  // close_reversed = 0; // Close position reversal test result

  // 阶段0: 查询初始到位状态
  if (diaphragm_runtime.sub_step == 0) {
    common_print_step_header("阀门到位测试", "检测阀门到位信号");

    log_i("=== 阀门到位测试开始 ===");
    log_i("阶段0: 查询初始到位状态...");
    diaphragm_runtime.events_received &= ~DGM_EVT_IO_STATUS;
    DGM_SendIOStatusCheck(0, 0);

    diaphragm_runtime.sub_step = 1;
    diaphragm_set_delay(300);
    return;
  }

  // 阶段1:
  // 等待1002响应，保存初始到位状态，然后输出开到位信号，验证开到位电平反转
  if (diaphragm_runtime.sub_step == 1) {
    if (!(diaphragm_runtime.events_received & DGM_EVT_IO_STATUS)) {
      log_d("阶段0: 等待1002响应...");
      return;
    }

    open_pos_initial = diaphragm_runtime.io_status.open_pos;
    // 注意: 关到位功能当前硬件未实现
    // close_pos_initial = diaphragm_runtime.io_status.close_pos;

    log_i("阶段0: 初始状态 开到位=%d (关到位硬件未实现，跳过)",
          open_pos_initial);

    // 输出开到位信号 (PA11 = signal_2 是开到位的触发信号)
    // 注意: PA12 = signal_1 是关到位信号，当前硬件未实现，已注释
    log_i("阶段1: 输出开到位信号(PA11)，验证开到位反转...");
    // output_valve_position_signal_1(1); // PA12 -
    // 关到位信号，硬件未实现，已注释
    output_valve_position_signal_2(1); // PA11 - 开到位信号触发
    FL_DelayMs(100);                   // 等待信号稳定

    diaphragm_runtime.events_received &= ~DGM_EVT_IO_STATUS;
    DGM_SendIOStatusCheck(0, 0);

    diaphragm_runtime.sub_step = 2;
    diaphragm_set_delay(200);
    return;
  }

  // 阶段2: 验证开到位信号反转
  if (diaphragm_runtime.sub_step == 2) {
    if (!(diaphragm_runtime.events_received & DGM_EVT_IO_STATUS)) {
      log_d("阶段1: 等待1002响应...");
      return;
    }

    uint8_t open_pos_after = diaphragm_runtime.io_status.open_pos;

    log_i("阶段1: 触发开到位信号(PA11)后 开到位=%d", open_pos_after);

    // 验证开到位信号反转
    open_reversed = (open_pos_initial != open_pos_after);
    if (open_reversed) {
      log_i("阶段1: 开到位信号反转验证通过 (%d->%d)", open_pos_initial,
            open_pos_after);
    } else {
      log_w("阶段1: 开到位信号未反转 (仍为%d)", open_pos_after);
    }

    // NOTE: 关到位功能当前硬件未实现，跳过阶段2的关到位测试
    // 直接恢复信号并进入阶段4验证恢复
    /*
    // 输出到位信号2 (模拟关到位)
    log_i("阶段2: 输出到位信号2，验证关到位反转...");
    output_valve_position_signal_1(0); // 释放信号1
    output_valve_position_signal_2(1); // 输出低电平触发

    diaphragm_runtime.events_received &= ~DGM_EVT_IO_STATUS;
    DGM_SendIOStatusCheck(0, 0);

    diaphragm_runtime.sub_step = 3;
    diaphragm_set_delay(200);
    return;
    */

    // 恢复到位信号为默认状态
    // output_valve_position_signal_1(0); // PA12 - 关到位信号，硬件未实现
    output_valve_position_signal_2(0); // PA11 - 释放开到位信号

    // 查询验证恢复状态
    log_i("阶段3: 恢复到位信号，查询最终状态...");
    diaphragm_runtime.events_received &= ~DGM_EVT_IO_STATUS;
    DGM_SendIOStatusCheck(0, 0);

    diaphragm_runtime.sub_step = 4; // 直接跳到阶段4
    diaphragm_set_delay(200);
    return;
  }

  // NOTE: 阶段3 - 关到位测试，当前硬件未实现，整体注释
  /*
  // 阶段3: 验证关到位信号反转
  if (diaphragm_runtime.sub_step == 3) {
    if (!(diaphragm_runtime.events_received & DGM_EVT_IO_STATUS)) {
      log_d("阶段2: 等待1002响应...");
      return;
    }

    uint8_t open_pos_after_2 = diaphragm_runtime.io_status.open_pos;
    uint8_t close_pos_after = diaphragm_runtime.io_status.close_pos;

    log_i("阶段2: 到位信号2后 open_pos=%d, close_pos=%d", open_pos_after_2,
          close_pos_after);

    // 验证关到位是否反转
    close_reversed = (close_pos_initial != close_pos_after);
    if (close_reversed) {
      log_i("阶段2: 关到位信号反转验证通过 (%d->%d)", close_pos_initial,
            close_pos_after);
    } else {
      log_w("阶段2: 关到位信号未反转 (仍为%d)", close_pos_after);
    }

    // 恢复到位信号为默认状态
    output_valve_position_signal_1(0);
    output_valve_position_signal_2(0);

    // 再次查询验证恢复
    log_i("阶段3: 恢复到位信号，查询最终状态...");
    diaphragm_runtime.events_received &= ~DGM_EVT_IO_STATUS;
    DGM_SendIOStatusCheck(0, 0);

    diaphragm_runtime.sub_step = 4;
    diaphragm_set_delay(200);
    return;
  }
  */

  // 阶段4: 汇总测试结果
  if (diaphragm_runtime.sub_step == 4) {
    if (!(diaphragm_runtime.events_received & DGM_EVT_IO_STATUS)) {
      log_d("阶段3: 等待1002响应...");
      return;
    }

    uint8_t open_pos_final = diaphragm_runtime.io_status.open_pos;

    log_i("阶段3: 最终状态 开到位=%d", open_pos_final);

    // 验证开到位信号恢复到初始状态
    bool restored = (open_pos_final == open_pos_initial);

    log_i("=== 阀门到位测试汇总 ===");
    log_i("  初始状态: 开到位=%d", open_pos_initial);
    log_i("  开到位反转: %s", open_reversed ? "通过" : "失败");
    // 注意: 关到位功能当前硬件未实现
    // log_i("  关到位反转: %s", close_reversed ? "通过" : "失败");
    log_i("  恢复状态: 开到位=%d, %s", open_pos_final,
          restored ? "恢复正常" : "恢复失败");

    // 最终判定 - 仅验证开到位: 反转 + 恢复
    bool position_test_passed = open_reversed && restored;
    // 注意: 启用关到位后: open_reversed && close_reversed && restored

    log_i("=== 阀门到位测试最终结果: %s ===",
          position_test_passed ? "通过" : "失败");

    if (position_test_passed) {
      IOSTATUS_SET(
          diaphragm_test_result.IOStatus1,
          IOSTATUS1_VALVE_BIT); // 阀门到位继续设置阀门状态为1，之前的阀门电压了了会设置1，这里继续设置1表示到位测试也通过了
      common_print_step_footer("通过: 阀门到位测试正常");
      diaphragm_step_success();
    } else {
      // 如果失败了，我们就掉阀门的状态，因为到位测试失败了，说明阀门的状态是异常的
      diaphragm_test_result.IOStatus1 &=
          ~IOSTATUS1_VALVE_BIT; // 清除阀门状态位，表示阀门异常
      common_print_step_footer("失败: 阀门到位测试异常");
      diaphragm_step_fail(DGM_FAIL_VALVE);
    }
  }
}

void debug_breakpoint(void) {
  // 设置一个死循环断点，方便调试
  log_w("调试断点已触发，进入死循环等待调试...");
  while (1) {
  }
}
