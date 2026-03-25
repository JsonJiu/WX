/**
 * @file common_test_configure.h
 * @brief 通用测试配置头文件
 * @details 测试框架的核心配置文件，管理所有测试模块的启用/禁用，
 *          以及通用测试函数的声明
 *
 * 架构说明：
 * 1. 通过宏定义启用/禁用不同的测试模块（膜表、水表等）
 * 2. 统一管理所有测试相关的头文件引用
 * 3. 定义通用的测试回调函数类型
 */

#ifndef __COMMON_TEST_CONFIGURE_H__
#define __COMMON_TEST_CONFIGURE_H__

#include "main.h"
#include "time.h"
#include <stdint.h>
#include <string.h>

/*============================================================================
 *                          测试模块配置宏定义
 * 说明：通过注释/取消注释来启用/禁用对应的测试模块
 *       可以同时启用多个测试模块
 *===========================================================================*/

/** @brief 启用膜式燃气表测试模块 */
#define ENABLE_DIAPHRAGM_METER_TEST

/** @brief 启用民用水表测试模块 */
// #define ENABLE_DOMESTIC_WATER_METER_TEST

/** @brief 启用超声波水表测试模块 */
// #define ENABLE_ULTRASONIC_WATER_METER_TEST

/** @brief 启用工业表测试模块 */
// #define ENABLE_INDUSTRIAL_METER_TEST

/*============================================================================
 *                          测试模块头文件引用
 * 说明：根据上面的宏定义自动包含对应的测试模块头文件
 *       注意：domestic_water_meter_test.h 包含被多个模块共享的结构体定义，
 *             因此总是需要包含
 *===========================================================================*/

/* 基础测试定义 - 包含共享的结构体和变量定义，总是需要包含 */
#include "domestic_water_meter_test.h"

#ifdef ENABLE_DIAPHRAGM_METER_TEST
#include "diaphragme_meter_test.h"
#endif

/*============================================================================
 *                          通用测试状态定义
 *===========================================================================*/

/** @brief 通用测试完成状态枚举 */
typedef enum {
  COMMON_TEST_RUNNING = 0,        /**< 测试进行中 */
  COMMON_TEST_SUCCESS = 1,        /**< 测试正常完成 */
  COMMON_TEST_TIMEOUT_GLOBAL = 2, /**< 全局超时中断 */
  COMMON_TEST_TIMEOUT_STEP = 3,   /**< 单步超时中断 */
  COMMON_TEST_ERROR_HARDWARE = 4, /**< 硬件错误中断 */
  COMMON_TEST_ERROR_PROTOCOL = 5, /**< 协议错误中断 */
  COMMON_TEST_MANUAL_STOP = 6     /**< 手动停止测试 */
} CommonTestStatus_t;

/*============================================================================
 *                          回调函数类型定义
 *===========================================================================*/

/**
 * @brief 测试初始化回调函数类型
 * @details 不同的测试有不同的初始化方式，通过回调函数实现多态
 */
typedef void (*TestInitCallback)(void);

/**
 * @brief 测试结果回调函数类型
 * @param status 测试状态
 * @param result_data 测试结果数据指针
 * @param data_len 数据长度
 */
typedef void (*TestResultCallback)(CommonTestStatus_t status,
                                   const uint8_t *result_data,
                                   uint16_t data_len);

/**
 * @brief 测试步骤处理回调函数类型
 */
typedef void (*TestStepHandler)(void);

/*============================================================================
 *                          通用测试配置结构体
 *===========================================================================*/

/**
 * @brief 通用测试配置参数结构体
 */
typedef struct {
  volatile uint16_t soft_delay_ms; /**< 软件延时计数器(ms) - 中断修改 */
  volatile uint32_t global_timeout_ms; /**< 全局测试超时计数器(ms) - 中断修改 */
  volatile uint16_t step_timeout_ms; /**< 单步测试超时计数器(ms) - 中断修改 */
  CommonTestStatus_t test_status;     /**< 当前测试状态 */
  uint8_t current_step;               /**< 当前测试步骤 */
  uint8_t failed_step;                /**< 失败时的测试步骤 */
  char status_description[64];        /**< 测试状态详细说明 */
  TestInitCallback init_callback;     /**< 初始化回调 */
  TestResultCallback result_callback; /**< 结果回调 */
} CommonTestConfig_t;

/*============================================================================
 *                          全局测试配置接口
 *===========================================================================*/

/**
 * @brief 获取全局测试配置指针
 * @return 全局测试配置指针
 */
CommonTestConfig_t *common_get_test_config(void);

/**
 * @brief 测试超时计数器递减 - 由定时器中断调用
 * @note 每毫秒调用一次，递减全局超时和单步超时计数器
 */
void common_test_timer_tick(void);

/*============================================================================
 *                          通用测试函数声明
 *===========================================================================*/

/**
 * @brief 通用测试数据结果清零
 * @param config 测试配置指针
 */
void common_test_data_clear(CommonTestConfig_t *config);

/**
 * @brief 获取当前工位编号
 * @return 工位编号 (1-15)
 */
uint8_t common_get_station_id(void);

/**
 * @brief 通用测试初始化
 * @param config 测试配置指针
 * @param init_cb 初始化回调函数
 * @param result_cb 结果回调函数
 */
void common_test_init(CommonTestConfig_t *config, TestInitCallback init_cb,
                      TestResultCallback result_cb);

/**
 * @brief 设置测试状态及描述
 * @param config 测试配置指针
 * @param status 测试状态
 * @param step 失败步骤
 * @param description 状态描述
 */
void common_set_test_status(CommonTestConfig_t *config,
                            CommonTestStatus_t status, uint8_t step,
                            const char *description);

/**
 * @brief 配置测试计时器
 * @param config 测试配置指针
 * @param global_timeout_ms 全局超时时间(ms)
 * @param step_timeout_ms 单步超时时间(ms)
 */
void common_test_timer_config(CommonTestConfig_t *config,
                              uint32_t global_timeout_ms,
                              uint16_t step_timeout_ms);

/**
 * @brief 检查全局超时
 * @param config 测试配置指针
 * @return 1-超时, 0-未超时
 */
uint8_t common_check_global_timeout(CommonTestConfig_t *config);

/**
 * @brief 检查单步超时
 * @param config 测试配置指针
 * @return 1-超时, 0-未超时
 */
uint8_t common_check_step_timeout(CommonTestConfig_t *config);

/**
 * @brief 重置单步超时计数器
 * @param config 测试配置指针
 * @param timeout_ms 新的超时时间(ms)
 */
void common_reset_step_timeout(CommonTestConfig_t *config, uint16_t timeout_ms);

/**
 * @brief 打印测试步骤分隔符
 */
void common_print_step_separator(void);

/**
 * @brief 打印测试步骤头部信息
 * @param step_name 步骤名称
 * @param description 步骤描述
 */
void common_print_step_header(const char *step_name, const char *description);

/**
 * @brief 打印测试步骤结果
 * @param result 结果描述
 */
void common_print_step_footer(const char *result);

/**
 * @brief 获取测试状态描述字符串
 * @param status 测试状态
 * @return 状态描述字符串
 */
const char *common_get_status_string(CommonTestStatus_t status);

/**
 * @brief 判断测试是否完成
 * @param config 测试配置指针
 * @return 1-完成, 0-进行中
 */
uint8_t common_is_test_finished(CommonTestConfig_t *config);

/**
 * @brief 判断测试是否成功
 * @param config 测试配置指针
 * @return 1-成功, 0-失败或进行中
 */
uint8_t common_is_test_success(CommonTestConfig_t *config);

/*============================================================================
 *                          测试结果上报函数
 *===========================================================================*/

/**
 * @brief 上报测试结果（通过回调）
 * @param config 测试配置指针
 * @param result_data 结果数据
 * @param data_len 数据长度
 */
void common_report_test_result(CommonTestConfig_t *config,
                               const uint8_t *result_data, uint16_t data_len);

/**
 * @brief 通用超时错误处理
 * @param config 测试配置指针
 * @param current_step 当前测试步骤
 * @param step_name 步骤名称（用于日志）
 * @return 1-发生超时需要结束测试, 0-未超时
 */
uint8_t common_test_check_timeout(CommonTestConfig_t *config,
                                  uint8_t current_step, const char *step_name);

/*============================================================================
 *                          测试入口函数
 *===========================================================================*/

/**
 * @brief 开始测试
 */
void test_start(void);

/**
 * @brief 自动测试循环
 */
void auto_test_loop(void);
extern uint16_t debug_print_time;
extern uint8_t s_first_entry;
extern uint8_t s_hardware_init_done;
#endif /* __COMMON_TEST_CONFIGURE_H__ */
