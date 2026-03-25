/**
 * @file valve_ctrl_core.h
 * @brief 阀门控制组件 - 核心层接口
 * @version 2.0.0
 * @date 2024-12
 *
 * 核心层API，只包含纯逻辑函数，不包含任何硬件依赖。
 * 所有硬件操作通过 HAL 函数指针完成。
 */

#ifndef __VALVE_CTRL_CORE_H__
#define __VALVE_CTRL_CORE_H__

#include "valve_ctrl_def.h"

#ifdef __cplusplus
extern "C" {
#endif

/*============================================================================*/
/*                              核心层 API */
/*============================================================================*/

/**
 * @brief 初始化阀门测试上下文
 * @param ctx 测试上下文指针
 * @param hal HAL接口指针 (由Port层提供)
 * @note 调用此函数后，ctx绑定到指定的HAL实例
 */
void ValveCtrl_Core_Init(ValveCtrl_Context_t *ctx, const ValveCtrl_HAL_t *hal);

/**
 * @brief 启动阀门测试
 * @param ctx 测试上下文指针
 */
void ValveCtrl_Core_Start(ValveCtrl_Context_t *ctx);

/**
 * @brief 停止阀门测试
 * @param ctx 测试上下文指针
 */
void ValveCtrl_Core_Stop(ValveCtrl_Context_t *ctx);

/**
 * @brief 主循环处理函数 (状态机驱动)
 * @param ctx 测试上下文指针
 * @param tick_ms 调用间隔 (毫秒), 通常为10ms
 * @return 当前测试结果
 */
VT_TestResult ValveCtrl_Core_Loop(ValveCtrl_Context_t *ctx, uint32_t tick_ms);

/**
 * @brief 协议层收到响应时调用
 * @param ctx 测试上下文指针
 * @param response_code 收到的响应码
 */
void ValveCtrl_Core_OnResponse(ValveCtrl_Context_t *ctx,
                               uint16_t response_code);

/*============================================================================*/
/*                              状态查询 API */
/*============================================================================*/

/**
 * @brief 获取当前测试结果
 * @param ctx 测试上下文指针
 * @return 测试结果
 */
VT_TestResult ValveCtrl_Core_GetResult(const ValveCtrl_Context_t *ctx);

/**
 * @brief 获取当前步骤
 * @param ctx 测试上下文指针
 * @return 当前步骤
 */
VT_TestStep ValveCtrl_Core_GetStep(const ValveCtrl_Context_t *ctx);

/**
 * @brief 获取失败原因
 * @param ctx 测试上下文指针
 * @return 失败原因
 */
VT_FailReason ValveCtrl_Core_GetFailReason(const ValveCtrl_Context_t *ctx);

/**
 * @brief 获取失败原因描述字符串
 * @param reason 失败原因枚举
 * @return 描述字符串
 */
const char *ValveCtrl_Core_GetFailReasonStr(VT_FailReason reason);

/**
 * @brief 获取步骤名称字符串
 * @param step 步骤枚举
 * @return 步骤名称字符串
 */
const char *ValveCtrl_Core_GetStepName(VT_TestStep step);

/**
 * @brief 检查测试是否正在运行
 * @param ctx 测试上下文指针
 * @return true=运行中, false=空闲
 */
bool ValveCtrl_Core_IsRunning(const ValveCtrl_Context_t *ctx);

#ifdef __cplusplus
}
#endif

#endif /* __VALVE_CTRL_CORE_H__ */
