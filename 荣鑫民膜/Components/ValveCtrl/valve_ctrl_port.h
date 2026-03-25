/**
 * @file valve_ctrl_port.h
 * @brief 阀门控制组件 - 适配层接口
 * @version 2.0.0
 * @date 2024-12
 *
 * 适配层负责连接 Core 层和具体硬件实现。
 * 这是唯一"脏"的地方，它负责把 Core 层的需求映射到你的老代码。
 */

#ifndef __VALVE_CTRL_PORT_H__
#define __VALVE_CTRL_PORT_H__

#include "valve_ctrl_def.h"

#ifdef __cplusplus
extern "C" {
#endif

/*============================================================================*/
/*                          全局变量声明 (兼容旧代码) */
/*============================================================================*/

/** 阀门测试使能标志 - 多处读写需要volatile */
extern volatile uint8_t enable_valve_test;

/*============================================================================*/
/*                          对外暴露的高级 API */
/*============================================================================*/

/**
 * @brief 初始化阀门控制模块
 * @note 自动创建 HAL 实例并绑定到内部 Context
 */
void ValveCtrl_Init(void);

/**
 * @brief 启动阀门测试
 */
void ValveCtrl_Start(void);

/**
 * @brief 停止阀门测试
 */
void ValveCtrl_Stop(void);

/**
 * @brief 主循环处理 (放在 main while(1) 中调用)
 * @param tick_ms 调用间隔 (毫秒)
 * @return 当前测试结果
 */
VT_TestResult ValveCtrl_Process(uint32_t tick_ms);

/**
 * @brief 协议层收到响应时调用
 * @param response_code 收到的响应码
 */
void ValveCtrl_OnResponse(uint16_t response_code);

/*============================================================================*/
/*                          状态查询接口 */
/*============================================================================*/

/**
 * @brief 获取当前测试结果
 */
VT_TestResult ValveCtrl_GetResult(void);

/**
 * @brief 获取当前步骤
 */
VT_TestStep ValveCtrl_GetStep(void);

/**
 * @brief 获取失败原因
 */
VT_FailReason ValveCtrl_GetFailReason(void);

/**
 * @brief 获取失败原因字符串
 */
const char *ValveCtrl_GetFailReasonStr(void);

/**
 * @brief 获取步骤名称字符串
 */
const char *ValveCtrl_GetStepName(void);

/**
 * @brief 检查测试是否正在运行
 */
bool ValveCtrl_IsRunning(void);

/*============================================================================*/
/*                          内部 Context 访问 (高级用途) */
/*============================================================================*/

/**
 * @brief 获取内部 Context 指针
 * @note 仅供高级用途，一般不需要直接访问
 */
ValveCtrl_Context_t *ValveCtrl_GetContext(void);

/**
 * @brief 获取 HAL 实例指针
 * @note 仅供高级用途，一般不需要直接访问
 */
const ValveCtrl_HAL_t *ValveCtrl_GetHAL(void);

#ifdef __cplusplus
}
#endif

#endif /* __VALVE_CTRL_PORT_H__ */
