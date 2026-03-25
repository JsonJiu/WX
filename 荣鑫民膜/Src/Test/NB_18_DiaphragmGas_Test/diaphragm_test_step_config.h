/**
 * @file diaphragm_test_step_config.h
 * @brief 膜式燃气表测试步骤配置 - 结构体定义与简化宏
 * @version 2.0.0
 * @date 2026-02-07
 *
 * @note 优化点:
 *       1. 增加 fail_action + fail_goto_step 支持失败跳转
 *       2. 提供简化宏，减少重复代码
 *       3. 保留所有字段，兼容后续扩展
 */

#ifndef __DIAPHRAGM_TEST_STEP_CONFIG_H__
#define __DIAPHRAGM_TEST_STEP_CONFIG_H__

#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

/*============================================================================
 *                          测试模式枚举
 *===========================================================================*/

/**
 * @brief 测试模式 - 决定功耗监控行为
 */
typedef enum {
  DGM_MODE_IDLE = 0,  /**< 空闲模式，不监控功耗 */
  DGM_MODE_NORMAL,    /**<
                         正常模式，监控运行功耗,这个步骤主要是包含验证运行时候功耗，如果功耗异常，立刻退出，实际需求需要根据具体场合，不排除有的场合确实也有大电流
                       */
  DGM_MODE_LOW_POWER, /**< 低功耗模式，监控静态功耗 */
} DgmTestMode_t;

/*============================================================================
 *                          失败处理策略枚举 (新增)
 *===========================================================================*/

/**
 * @brief 失败处理策略
 */
typedef enum {
  FAIL_ACTION_STOP = 0, /**< 失败后停止测试，跳转到END (默认，最安全) */
  FAIL_ACTION_CONTINUE, /**< 失败后记录结果，继续下一步 */
  FAIL_ACTION_GOTO,     /**< 失败后跳转到指定步骤 */
  FAIL_ACTION_RETRY_THEN_CONTINUE, /**< 重试耗尽后继续 (而非停止) */
} FailAction_t;

/*============================================================================
 *                          测试步骤枚举
 *===========================================================================*/

/**
 * @brief 膜式燃气表测试流程枚举
 */
typedef enum {
  /*-----------------------------------------------------------------------
   * 系统步骤
   *---------------------------------------------------------------------*/
  DIAPHRAGM_WAIT = 0, /**< 等待开始 */
  DIAPHRAGM_START,    /**< 开始测试 */

  /*-----------------------------------------------------------------------
   * 初始化步骤
   *---------------------------------------------------------------------*/
  DIAPHRAGM_MODULE_GPRS_VOLTAGE_TEST,     /**< GPRS 3.6V电压测试 */
  DIAPHRAGM_MODULE_SUCCESS_RESPONSE_WAIT, /**< 等待模块1000协议响应 */
  DIAPHRAGM_IO_STATUS_CHECK, /**< IO初始状态和附件信息检测 */

  /*-----------------------------------------------------------------------
   * 可选组件测试 (根据附件信息决定是否测试)
   *---------------------------------------------------------------------*/
  DIAPHRAGM_BLUETOOTH_TEST,            /**< 蓝牙测试 */
  DIAPHRAGM_COVER_CHECK,               /**< 防拆检测 */
  DIAPHRAGM_IC_MODULE_TEST,            /**< IC模块测试 */
  DIAPHRAGM_METERING_MODULE_TEST,      /**< 计量模块测试 */
  DIAPHRAGM_119_WARNING_TEST,          /**< 119报警测试 */
  DIAPHRAGM_TEMP_PRESSURE_SENSOR_TEST, /**< 温压传感器测试 */
  DIAPHRAGM_SECOND_VALVE_TEST,         /**< 第二阀门测试 */
  DIAPHRAGM_TILT_SWITCH_TEST,          /**< 倾斜开关测试 */

  /*-----------------------------------------------------------------------
   * 必测步骤 (常规测试)
   *---------------------------------------------------------------------*/
  DIAPHRAGM_DEFAULT_VALVE_TEST,  /**< 阀门电压测试 */
  DIAPHRAGM_VALVE_POSITION_TEST, /**< 阀门到位测试 */
  DIAPHRAGM_TEMP_CHECK,          /**< 温度压强检测 */
  DIAPHRAGM_NB_CONNECT,          /**< NB连接测试 */
  DIAPHRAGM_IMEI_READ,           /**< IMEI读取 */
  DIAPHRAGM_CLOSE_IR,            /**< 关闭红外 */
  DIAPHRAGM_UART_CONNECT_TEST,   /**< UART连接测试 */
  DIAPHRAGM_POWER_CHECK,         /**< 功耗检测 (放最后) */

  /*-----------------------------------------------------------------------
   * 结束步骤
   *---------------------------------------------------------------------*/
  DIAPHRAGM_END,       /**< 测试结束 */
  DIAPHRAGM_STEP_COUNT /**< 步骤总数 (用于数组大小) */
} DiaphragmTestStep_t;

/*============================================================================
 *                          特殊步骤跳转值
 *===========================================================================*/

/** @brief 自动跳转到下一步 (当前步骤+1) */
#define DIAPHRAGM_NEXT_AUTO ((DiaphragmTestStep_t)(-1))

/** @brief 保持当前步骤 (由handler决定跳转) */
#define DIAPHRAGM_NEXT_STAY ((DiaphragmTestStep_t)(-2))

/*============================================================================
 *                          组件状态值定义
 *===========================================================================*/

/** @brief 组件未测试 (跳过该步骤) */
#define DIAPHRAGM_COMPONENT_STATUS_UNTESTED 2

/** @brief 组件测试通过 */
#define DIAPHRAGM_COMPONENT_STATUS_PASS 1

/** @brief 组件测试失败 */
#define DIAPHRAGM_COMPONENT_STATUS_FAIL 0

/*============================================================================
 *                          回调函数类型定义
 *===========================================================================*/

/** @brief 测试步骤处理函数类型 (轮询调用) */
typedef void (*DiaphragmStepHandler)(void);

/** @brief 步骤进入/重试执行函数类型 */
typedef void (*DiaphragmExecuteFunction)(void);

/*============================================================================
 *                          测试步骤描述结构体 (扩展版)
 *===========================================================================*/

/**
 * @brief 测试步骤描述结构体
 * @details 用于构建回调表，实现测试流程的数据驱动
 */
typedef struct {
  /*--- 基本信息 ---*/
  const char *name;        /**< 步骤名称 (用于日志/显示) */
  const char *description; /**< 步骤描述 (详细说明) */
  DiaphragmStepHandler handler; /**< 处理函数 (轮询调用，处理状态机逻辑) */

  /*--- 超时与重试 ---*/
  uint16_t timeout_ms; /**< 超时时间(ms)，0表示无超时 */
  uint8_t max_retry;   /**< 最大重试次数，0表示不重试 */

  /*--- 成功跳转 ---*/
  DiaphragmTestStep_t
      next_step; /**< 成功后跳转步骤，DIAPHRAGM_NEXT_AUTO表示自动下一步 */

  /*--- 执行回调 ---*/
  DiaphragmExecuteFunction on_enter; /**< 进入步骤时执行，NULL表示无操作 */
  DiaphragmExecuteFunction on_retry; /**< 重试时执行，NULL表示使用on_enter */

  /*--- 测试模式 ---*/
  DgmTestMode_t test_mode; /**< 测试模式 - 决定功耗监控行为 */

  /*--- 跳过逻辑 (可选组件) ---*/
  uint8_t *skip_status_ptr; /**< 跳过检测状态指针，NULL表示不检测 */
  uint8_t skip_when_equals; /**< 当 *skip_status_ptr == 此值时跳过该步骤 */

  /*--- 失败处理 (新增) ---*/
  FailAction_t fail_action;           /**< 失败处理策略 */
  DiaphragmTestStep_t fail_goto_step; /**< FAIL_ACTION_GOTO 时的目标步骤 */
} DiaphragmStepDescriptor_t;

/*============================================================================
 *                          组件状态结构体
 *===========================================================================*/

/**
 * @brief 组件状态结构体 (用于可选步骤跳过判断)
 * @note 状态值: 0=测试失败, 1=需要测试, 2=不测试(跳过)
 */
typedef struct {
  uint8_t valve_1_status; /**< 阀门1测试状态，默认1需要测试 */
  uint8_t valve_2_status; /**< 第二阀门状态 */
  uint8_t module_temp_pressure_sensor;           /**< 温压传感器状态 */
  uint8_t warning_component_status;              /**< 119报警状态 */
  uint8_t cover_check_status;                    /**< 防拆检测状态 */
  uint8_t ic_module_status;                      /**< IC模块状态 */
  uint8_t irf_component_status;                  /**< 红外功能状态 */
  uint8_t main_circuit_low_power_current_status; /**< 主电低功耗测试状态 */
  uint8_t module_circuit_low_power_current_status; /**< 模块低功耗测试状态 */
  uint8_t metering_module_status;                  /**< 计量模块状态 */
  uint8_t bluetooth_component_status;              /**< 蓝牙组件状态 */
  uint8_t tilt_switch_status;                      /**< 倾斜开关状态 */
  uint8_t valve_position_test_status; /**< 阀门到位测试状态 */
} DiaphragmComponentStatus_t;

/** @brief 全局组件状态实例 */
extern DiaphragmComponentStatus_t diaphragm_component_status;

/*============================================================================
 *                          简化宏定义
 *===========================================================================*/

/**
 * @brief 组件状态指针快捷引用
 * @note 避免在配置表中写长路径
 */
#define COMP_BLUETOOTH (&diaphragm_component_status.bluetooth_component_status)
#define COMP_COVER (&diaphragm_component_status.cover_check_status)
#define COMP_IC_MODULE (&diaphragm_component_status.ic_module_status)
#define COMP_METERING (&diaphragm_component_status.metering_module_status)
#define COMP_119_WARNING (&diaphragm_component_status.warning_component_status)
#define COMP_TEMP_PRESSURE                                                     \
  (&diaphragm_component_status.module_temp_pressure_sensor)
#define COMP_VALVE_2 (&diaphragm_component_status.valve_2_status)
#define COMP_TILT_SWITCH (&diaphragm_component_status.tilt_switch_status)
#define COMP_VALVE_POSITION                                                    \
  (&diaphragm_component_status.valve_position_test_status)

/*---------------------------------------------------------------------------
 * 步骤定义宏 - 简化配置表书写
 *
 * 命名规则:
 *   STEP_REQUIRED     - 必测步骤，失败停止
 *   STEP_REQUIRED_SOFT- 必测步骤，失败继续
 *   STEP_OPTIONAL     - 可选步骤，可跳过，失败继续
 *   STEP_SYSTEM       - 系统步骤 (WAIT/END)
 *   STEP_CUSTOM       - 完全自定义
 *--------------------------------------------------------------------------*/

/**
 * @brief 必测步骤 - 失败停止 (最常用)
 * @param _name     步骤名称
 * @param _desc     步骤描述
 * @param _handler  处理函数
 * @param _timeout  超时时间(ms)
 */
#define STEP_REQUIRED(_name, _desc, _handler, _timeout)                        \
  {                                                                            \
    .name = _name, .description = _desc, .handler = _handler,                  \
    .timeout_ms = _timeout, .max_retry = 0, .next_step = DIAPHRAGM_NEXT_AUTO,  \
    .on_enter = NULL, .on_retry = NULL, .test_mode = DGM_MODE_NORMAL,          \
    .skip_status_ptr = NULL, .skip_when_equals = 0,                            \
    .fail_action = FAIL_ACTION_STOP, .fail_goto_step = DIAPHRAGM_END,          \
  }

/**
 * @brief 必测步骤 (带重试) - 失败停止
 */
#define STEP_REQUIRED_RETRY(_name, _desc, _handler, _timeout, _retry)          \
  {                                                                            \
    .name = _name, .description = _desc, .handler = _handler,                  \
    .timeout_ms = _timeout, .max_retry = _retry,                               \
    .next_step = DIAPHRAGM_NEXT_AUTO, .on_enter = NULL, .on_retry = NULL,      \
    .test_mode = DGM_MODE_NORMAL, .skip_status_ptr = NULL,                     \
    .skip_when_equals = 0, .fail_action = FAIL_ACTION_STOP,                    \
    .fail_goto_step = DIAPHRAGM_END,                                           \
  }

/**
 * @brief 必测步骤 (带重试和on_enter) - 失败停止
 */
#define STEP_REQUIRED_RETRY_ENTER(_name, _desc, _handler, _timeout, _retry,    \
                                  _on_enter)                                   \
  {                                                                            \
    .name = _name, .description = _desc, .handler = _handler,                  \
    .timeout_ms = _timeout, .max_retry = _retry,                               \
    .next_step = DIAPHRAGM_NEXT_AUTO, .on_enter = _on_enter, .on_retry = NULL, \
    .test_mode = DGM_MODE_NORMAL, .skip_status_ptr = NULL,                     \
    .skip_when_equals = 0, .fail_action = FAIL_ACTION_STOP,                    \
    .fail_goto_step = DIAPHRAGM_END,                                           \
  }

/**
 * @brief 必测步骤 - 失败继续 (软性必测)
 * @note 用于：失败不影响整体测试，但需要记录结果
 */
#define STEP_REQUIRED_SOFT(_name, _desc, _handler, _timeout)                   \
  {                                                                            \
    .name = _name, .description = _desc, .handler = _handler,                  \
    .timeout_ms = _timeout, .max_retry = 0, .next_step = DIAPHRAGM_NEXT_AUTO,  \
    .on_enter = NULL, .on_retry = NULL, .test_mode = DGM_MODE_NORMAL,          \
    .skip_status_ptr = NULL, .skip_when_equals = 0,                            \
    .fail_action = FAIL_ACTION_CONTINUE, .fail_goto_step = DIAPHRAGM_END,      \
  }

/**
 * @brief 可选步骤 - 可跳过，失败继续
 * @param _name     步骤名称
 * @param _desc     步骤描述
 * @param _handler  处理函数
 * @param _timeout  超时时间(ms)
 * @param _skip_ptr 跳过状态指针 (使用 COMP_xxx 宏)
 */
#define STEP_OPTIONAL(_name, _desc, _handler, _timeout, _skip_ptr)             \
  {                                                                            \
    .name = _name, .description = _desc, .handler = _handler,                  \
    .timeout_ms = _timeout, .max_retry = 0, .next_step = DIAPHRAGM_NEXT_AUTO,  \
    .on_enter = NULL, .on_retry = NULL, .test_mode = DGM_MODE_NORMAL,          \
    .skip_status_ptr = _skip_ptr,                                              \
    .skip_when_equals = DIAPHRAGM_COMPONENT_STATUS_UNTESTED,                   \
    .fail_action = FAIL_ACTION_CONTINUE, .fail_goto_step = DIAPHRAGM_END,      \
  }

/**
 * @brief 系统步骤 - WAIT/END 等特殊步骤
 */
#define STEP_SYSTEM(_name, _desc, _handler, _timeout, _next, _mode)            \
  {                                                                            \
    .name = _name, .description = _desc, .handler = _handler,                  \
    .timeout_ms = _timeout, .max_retry = 0, .next_step = _next,                \
    .on_enter = NULL, .on_retry = NULL, .test_mode = _mode,                    \
    .skip_status_ptr = NULL, .skip_when_equals = 0,                            \
    .fail_action = FAIL_ACTION_STOP, .fail_goto_step = DIAPHRAGM_END,          \
  }

/**
 * @brief 低功耗测试步骤
 */
#define STEP_LOW_POWER(_name, _desc, _handler, _timeout, _retry)               \
  {                                                                            \
    .name = _name, .description = _desc, .handler = _handler,                  \
    .timeout_ms = _timeout, .max_retry = _retry,                               \
    .next_step = DIAPHRAGM_NEXT_AUTO, .on_enter = NULL, .on_retry = NULL,      \
    .test_mode = DGM_MODE_LOW_POWER, .skip_status_ptr = NULL,                  \
    .skip_when_equals = 0, .fail_action = FAIL_ACTION_STOP,                    \
    .fail_goto_step = DIAPHRAGM_END,                                           \
  }

/**
 * @brief 失败跳转步骤
 * @param _fail_goto 失败时跳转的目标步骤
 */
#define STEP_REQUIRED_FAIL_GOTO(_name, _desc, _handler, _timeout, _fail_goto)  \
  {                                                                            \
    .name = _name, .description = _desc, .handler = _handler,                  \
    .timeout_ms = _timeout, .max_retry = 0, .next_step = DIAPHRAGM_NEXT_AUTO,  \
    .on_enter = NULL, .on_retry = NULL, .test_mode = DGM_MODE_NORMAL,          \
    .skip_status_ptr = NULL, .skip_when_equals = 0,                            \
    .fail_action = FAIL_ACTION_GOTO, .fail_goto_step = _fail_goto,             \
  }

/*============================================================================
 *                          配置表声明
 *===========================================================================*/

/** @brief 测试步骤配置表 */
extern const DiaphragmStepDescriptor_t
    diaphragm_step_table[DIAPHRAGM_STEP_COUNT];

/*============================================================================
 *                          辅助函数声明
 *===========================================================================*/

/**
 * @brief 获取步骤名称
 * @param step 步骤枚举值
 * @return 步骤名称字符串
 */
const char *diaphragm_get_step_name(DiaphragmTestStep_t step);

/**
 * @brief 获取步骤描述
 * @param step 步骤枚举值
 * @return 步骤描述字符串
 */
const char *diaphragm_get_step_description(DiaphragmTestStep_t step);

/**
 * @brief 检查步骤是否应该跳过
 * @param step 步骤枚举值
 * @return true=跳过, false=执行
 */
bool diaphragm_should_skip_step(DiaphragmTestStep_t step);

/**
 * @brief 获取失败后的目标步骤
 * @param step 当前步骤
 * @return 失败后应跳转的步骤
 */
DiaphragmTestStep_t diaphragm_get_fail_target(DiaphragmTestStep_t step);

/**
 * @brief 获取失败策略名称
 * @param action 失败策略枚举
 * @return 策略名称字符串
 */
const char *diaphragm_get_fail_action_name(FailAction_t action);

/**
 * @brief 打印配置表信息 (调试用)
 */
void diaphragm_print_step_table(void);

#endif /* __DIAPHRAGM_TEST_STEP_CONFIG_H__ */
