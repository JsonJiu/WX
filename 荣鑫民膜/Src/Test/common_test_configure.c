/**
 * @file common_test_configure.c
 * @brief 通用测试配置实现文件
 * @details 实现测试框架的通用功能函数
 */

#include "common_test_configure.h"
#include "GPIO.h"
#include <stdio.h>

// EasyLogger 日志标签
#define LOG_TAG "test_common"
#include <elog.h>

/*============================================================================
 *                          外部函数声明
 *===========================================================================*/
extern uint8_t read_gongwei_pin_PC0(void);
extern uint8_t read_gongwei_pin_PD12(void);
extern uint8_t read_gongwei_pin_PB15(void);
extern uint8_t read_gongwei_pin_PE5(void);

// 测试函数声明
#ifdef ENABLE_DIAPHRAGM_METER_TEST
extern void diaphragm_test_start(void);
extern void diaphragm_test_loop(void);
#endif

#ifdef ENABLE_DOMESTIC_WATER_METER_TEST
extern void domestic_water_meter_test_start(void);
extern void domestic_water_meter_test_loop(void);
#endif
// 定义首次进入标志,初始化为1，后续每次测试结束后的清0状态里面重置
uint8_t s_first_entry = 1;
// 定义硬件等端口初始化标志，初始化为1，后续永久不重置，只有上电第一次初始化
uint8_t s_hardware_init_done = 1;

/*============================================================================
 *                          全局测试配置实例
 *===========================================================================*/

/** @brief 全局测试配置实例 - 用于所有测试模块 */
CommonTestConfig_t g_test_config;

/**
 * @brief 获取全局测试配置指针
 * @return 全局测试配置指针
 */
CommonTestConfig_t *common_get_test_config(void) { return &g_test_config; }

/**
 * @brief 测试超时计数器递减 - 由定时器中断调用
 * @note 每毫秒调用一次，递减全局超时和单步超时计数器
 */
void common_test_timer_tick(void) {
  if (g_test_config.global_timeout_ms > 0) {
    g_test_config.global_timeout_ms--;
  }
  if (g_test_config.step_timeout_ms > 0) {
    g_test_config.step_timeout_ms--;
  }
  if (g_test_config.soft_delay_ms > 0) {
    g_test_config.soft_delay_ms--;
  }
}

/*============================================================================
 *                          通用测试函数实现
 *===========================================================================*/

/**
 * @brief 通用测试数据结果清零
 */
void common_test_data_clear(CommonTestConfig_t *config) {
  if (config == NULL)
    return;

  config->soft_delay_ms = 0;
  config->global_timeout_ms = 0;
  config->step_timeout_ms = 0;
  config->test_status = COMMON_TEST_RUNNING;
  config->current_step = 0;
  config->failed_step = 0;
  memset(config->status_description, 0, sizeof(config->status_description));
  config->init_callback = NULL;
  config->result_callback = NULL;

  log_i("测试数据已清零");
}

/**
 * @brief 获取当前工位编号
 * @details 通过读取4个跳线帽引脚状态，组合成工位编号
 *          PC0(丝印1), PD12(丝印2), PB15(丝印3), PE5(丝印4)
 *          跳线帽插上为低电平(0)，未插为高电平(1)
 * @return 工位编号 (1-15), 0表示无效
 */
uint8_t common_get_station_id(void) {
  // 读取各个引脚电平状态
  uint8_t pc0 = read_gongwei_pin_PC0();
  uint8_t pd12 = read_gongwei_pin_PD12();
  uint8_t pb15 = read_gongwei_pin_PB15();
  uint8_t pe5 = read_gongwei_pin_PE5();

  // 计算跳线帽状态 (插上跳线帽时引脚为低电平，表示该位为1)
  uint8_t jumper1 = !pc0;  // 丝印1跳线帽状态 - bit0
  uint8_t jumper2 = !pd12; // 丝印2跳线帽状态 - bit1
  uint8_t jumper3 = !pb15; // 丝印3跳线帽状态 - bit2
  uint8_t jumper4 = !pe5;  // 丝印4跳线帽状态 - bit3

  // 组合成工位编号 (4位二进制)
  uint8_t gongwei = (jumper4 << 3) | (jumper3 << 2) | (jumper2 << 1) | jumper1;

  log_d("工位检测: PC0=%d, PD12=%d, PB15=%d, PE5=%d -> 工位号=%d", pc0, pd12,
        pb15, pe5, gongwei);

  return gongwei;
}

/**
 * @brief 通用测试初始化
 */
void common_test_init(CommonTestConfig_t *config, TestInitCallback init_cb,
                      TestResultCallback result_cb) {
  if (config == NULL)
    return;

  // 清零配置
  common_test_data_clear(config);

  // 设置回调函数
  config->init_callback = init_cb;
  config->result_callback = result_cb;

  // 调用初始化回调
  if (config->init_callback != NULL) {
    config->init_callback();
  }

  log_i("测试初始化完成");
}

/**
 * @brief 设置测试状态及描述
 */
void common_set_test_status(CommonTestConfig_t *config,
                            CommonTestStatus_t status, uint8_t step,
                            const char *description) {
  if (config == NULL)
    return;

  config->test_status = status;
  config->failed_step = step;

  if (description != NULL) {
    strncpy(config->status_description, description,
            sizeof(config->status_description) - 1);
    config->status_description[sizeof(config->status_description) - 1] = '\0';
  }

  log_i("测试状态: %s, 步骤: %d, 描述: %s", common_get_status_string(status),
        step, config->status_description);
}

/**
 * @brief 配置测试计时器
 */
void common_test_timer_config(CommonTestConfig_t *config,
                              uint32_t global_timeout_ms,
                              uint16_t step_timeout_ms) {
  if (config == NULL)
    return;

  config->global_timeout_ms = global_timeout_ms;
  config->step_timeout_ms = step_timeout_ms;

  log_d("计时器配置: 全局超时=%ldms, 单步超时=%dms", global_timeout_ms,
        step_timeout_ms);
}

/**
 * @brief 检查全局超时
 */
uint8_t common_check_global_timeout(CommonTestConfig_t *config) {
  if (config == NULL)
    return 0;

  if (config->global_timeout_ms == 0) {
    return 1; // 超时
  }
  return 0;
}

/**
 * @brief 检查单步超时
 */
uint8_t common_check_step_timeout(CommonTestConfig_t *config) {
  if (config == NULL)
    return 0;

  if (config->step_timeout_ms == 0) {
    return 1; // 超时
  }
  return 0;
}

/**
 * @brief 重置单步超时计数器
 */
void common_reset_step_timeout(CommonTestConfig_t *config,
                               uint16_t timeout_ms) {
  if (config == NULL)
    return;

  config->step_timeout_ms = timeout_ms;
}

/**
 * @brief 打印测试步骤分隔符
 */
void common_print_step_separator(void) {
  // UART发送函数已修复为等待上一帧发送完毕，无需额外延时
  log_i("----------------------------------------");
}

/**
 * @brief 打印测试步骤头部信息
 */
void common_print_step_header(const char *step_name, const char *description) {
  common_print_step_separator();
  log_i("【%s】: %s", step_name, description);
  common_print_step_separator();
}

/**
 * @brief 打印测试步骤结果
 */
void common_print_step_footer(const char *result) {
  log_i("【结果】: %s", result);
  log_i("");
}

/**
 * @brief 获取测试状态描述字符串
 */
const char *common_get_status_string(CommonTestStatus_t status) {
  switch (status) {
  case COMMON_TEST_RUNNING:
    return "测试进行中";
  case COMMON_TEST_SUCCESS:
    return "测试成功";
  case COMMON_TEST_TIMEOUT_GLOBAL:
    return "全局超时";
  case COMMON_TEST_TIMEOUT_STEP:
    return "单步超时";
  case COMMON_TEST_ERROR_HARDWARE:
    return "硬件错误";
  case COMMON_TEST_ERROR_PROTOCOL:
    return "协议错误";
  case COMMON_TEST_MANUAL_STOP:
    return "手动停止";
  default:
    return "未知状态";
  }
}

/**
 * @brief 判断测试是否完成
 */
uint8_t common_is_test_finished(CommonTestConfig_t *config) {
  if (config == NULL)
    return 0;

  return (config->test_status != COMMON_TEST_RUNNING);
}

/**
 * @brief 判断测试是否成功
 */
uint8_t common_is_test_success(CommonTestConfig_t *config) {
  if (config == NULL)
    return 0;

  return (config->test_status == COMMON_TEST_SUCCESS);
}

/**
 * @brief 上报测试结果（通过回调）
 */
void common_report_test_result(CommonTestConfig_t *config,
                               const uint8_t *result_data, uint16_t data_len) {
  if (config == NULL)
    return;

  if (config->result_callback != NULL) {
    config->result_callback(config->test_status, result_data, data_len);
  }

  log_i("测试结果已上报, 状态: %s",
        common_get_status_string(config->test_status));
}

/**
 * @brief 通用超时错误处理
 * @details 检查全局超时和单步超时，自动设置测试状态
 * @param config 测试配置指针
 * @param current_step 当前测试步骤
 * @param step_name 步骤名称（用于日志）
 * @return 1-发生超时需要结束测试, 0-未超时
 */
uint8_t common_test_check_timeout(CommonTestConfig_t *config,
                                  uint8_t current_step, const char *step_name) {
  if (config == NULL)
    return 0;

  // 只在测试进行中时检查超时
  if (config->test_status != COMMON_TEST_RUNNING)
    return 0;

  // 检查全局超时
  if (config->global_timeout_ms == 0) {
    log_i("全局测试超时，强制结束测试");
    if (step_name != NULL) {
      log_i("当前步骤：%s", step_name);
    }
    common_set_test_status(config, COMMON_TEST_TIMEOUT_GLOBAL, current_step,
                           "全局测试超时");
    return 1;
  }

  // 检查单步超时
  if (config->step_timeout_ms == 0) {
    log_i("单步测试超时，强制结束测试");
    if (step_name != NULL) {
      log_i("失败步骤：%s", step_name);
    }
    common_set_test_status(config, COMMON_TEST_TIMEOUT_STEP, current_step,
                           "单步测试超时");
    return 1;
  }

  return 0;
}

// 开始测试
void test_start(void) {
  log_i("测试开始");
  // 根据宏定义启用对应的测试
#ifdef ENABLE_DIAPHRAGM_METER_TEST
  diaphragm_test_start();
#endif
#ifdef ENABLE_DOMESTIC_WATER_METER_TEST
  domestic_water_meter_test_start();
#endif
}

void auto_test_loop(void) {
#ifdef ENABLE_DIAPHRAGM_METER_TEST
  diaphragm_test_loop();
#endif
#ifdef ENABLE_DOMESTIC_WATER_METER_TEST
  domestic_water_meter_test_loop();
#endif
}
