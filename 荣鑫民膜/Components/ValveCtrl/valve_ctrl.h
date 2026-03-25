/**
 * @file valve_ctrl.h
 * @brief 阀门控制组件 - 对外统一接口
 * @version 2.0.0
 * @date 2024-12
 *
 * @section intro 简介
 * 阀门测试组件，用于自动化测试水表阀门的开关功能。
 * 采用状态机架构，支持机械表和超声波表两种类型。
 *
 * @section business 业务流程
 * 阀门测试分为9个步骤，完整流程如下：
 *
 * ```
 * [步骤1] 发送配置命令
 *    │    - 机械表: 发送0x2604，配置管径/阀门类型/超时时间/堵转电流
 *    │    - 超声波表: 发送0x2036
 *    │    - 等待水表响应确认
 *    ▼
 * [步骤2] 检查初始状态
 *    │    - 验证电压A < 100mV（无动作）
 *    │    - 验证电压B < 100mV（无动作）
 *    ▼
 * [步骤3] 发送开阀命令 (0xC022)
 *    │    - 等待水表响应确认
 *    ▼
 * [步骤4] 检测开阀动作
 *    │    - 监测电压A > 2800mV（电机正转）
 *    │    - 监测电压B < 100mV
 *    │    - 超时: 5秒
 *    ▼
 * [步骤5] 输出开阀到位信号
 *    │    - PA11输出低电平（告知水表已开阀到位）
 *    │    - PA12保持高电平
 *    │    - 等待500ms让水表检测
 *    ▼
 * [步骤6] 检查开阀后状态
 *    │    - 验证电机已停止：电压A/B均 < 100mV
 *    ▼
 * [步骤7] 发送关阀命令 (0xC022)
 *    │    - 恢复到位信号为高电平
 *    │    - 等待水表响应确认
 *    ▼
 * [步骤8] 检测关阀动作（电机反转）
 *    │    - 监测电压A < 100mV
 *    │    - 监测电压B > 2800mV（电机反转）
 *    │    - 超时: 15秒
 *    ▼
 * [步骤9] 输出关阀到位信号
 *    │    - PA11保持高电平
 *    │    - PA12输出低电平（告知水表已关阀到位）
 *    │    - 验证电机停止后完成测试
 *    ▼
 * [完成] 测试成功/失败
 * ```
 *
 * @section hardware 硬件连接
 * ```
 * 工装板                          水表
 * ─────────                      ─────
 * PA11 (开阀到位信号) ◄──────────► 开阀到位检测
 * PA12 (关阀到位信号) ◄──────────► 关阀到位检测
 * ADC_A (电压检测)    ◄──────────► DRV8837 输出A
 * ADC_B (电压检测)    ◄──────────► DRV8837 输出B
 * UART1 (红外通信)    ◄──────────► 水表红外接口
 * ```
 *
 * @section signals 信号定义
 * - 到位信号: 低电平=到位, 高电平=未到位
 * - 电压阈值: <100mV=无动作, >2800mV=电机动作中
 *
 * @section usage 使用方法
 *
 * 1. 初始化（程序启动时调用一次）:
 * @code
 * ValveCtrl_Init();
 * @endcode
 *
 * 2. 主循环处理（每10ms调用）:
 * @code
 * while (1) {
 *     if (ValveCtrl_IsRunning()) {
 *         ValveCtrl_Process(10);  // 传入时间增量(ms)
 *     }
 * }
 * @endcode
 *
 * 3. 启动测试:
 * @code
 * if (!ValveCtrl_IsRunning()) {
 *     ValveCtrl_Start();
 * }
 * @endcode
 *
 * 4. 协议响应回调（在协议解析中调用）:
 * @code
 * // 收到水表响应时通知组件
 * ValveCtrl_OnResponse(response_code);  // 如 0x2604, 0xC022
 * @endcode
 *
 * 5. 结果处理:
 * @code
 * VT_TestResult result = ValveCtrl_GetResult();
 * if (result == VT_SUCCESS) {
 *     // 测试成功
 *     ValveCtrl_Stop();  // 重置状态，为下次测试做准备
 * } else if (result == VT_FAIL || result == VT_TIMEOUT) {
 *     // 测试失败
 *     printf("失败原因: %s\n", ValveCtrl_GetFailReasonStr());
 *     ValveCtrl_Stop();
 * }
 * @endcode
 *
 * @section errors 错误码说明
 * | 错误码 | 说明 |
 * |--------|------|
 * | VT_FAIL_CONFIG_TIMEOUT | 配置命令无响应 |
 * | VT_FAIL_INITIAL_VOLTAGE_A | 初始状态电压A异常(>=100mV) |
 * | VT_FAIL_INITIAL_VOLTAGE_B | 初始状态电压B异常(>=100mV) |
 * | VT_FAIL_OPEN_DETECT_TIMEOUT | 开阀动作检测超时(5秒内未检测到) |
 * | VT_FAIL_OPEN_STATE_CHECK | 开阀后电机未停止 |
 * | VT_FAIL_CLOSE_DETECT_TIMEOUT | 关阀动作检测超时(15秒内未检测到) |
 * | VT_FAIL_CLOSE_STATE_CHECK | 关阀后电机未停止 |
 * | VT_FAIL_TOTAL_TIMEOUT | 总测试时间超过60秒 |
 *
 * @section notes 注意事项
 * - 测试完成后必须调用 ValveCtrl_Stop() 重置状态
 * - 配置参数（管径、超时等）通过 Test_jiejuo_jilu 结构体设置
 * - 组件依赖 ADC_CHK.c, GPIO.c, tongxin_xieyi_Ctrl.c 等驱动
 */

#ifndef __VALVE_CTRL_H__
#define __VALVE_CTRL_H__

/* 包含所有需要的头文件 */
#include "valve_ctrl_def.h"  /* 类型定义 */
#include "valve_ctrl_port.h" /* Port层API (用户主要使用这个) */

/*
 * 注意: 用户一般不需要直接包含 valve_ctrl_core.h
 * Port 层已经封装了所有 Core 层的功能。
 *
 * 如果需要更底层的控制（如自定义 HAL），可以：
 * #include "valve_ctrl_core.h"
 */

#endif /* __VALVE_CTRL_H__ */
