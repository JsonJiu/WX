#ifndef NB_18_DIAPHRAGMGAS_BOARD_DEBUG_H
#define NB_18_DIAPHRAGMGAS_BOARD_DEBUG_H

#include "NB_18_DiaphragmGas_port.h"
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief 处理工装调试命令入口
 * @param data 完整帧数据
 * @param length 数据长度
 */
void diaphragm_board_debug_handle_command(const uint8_t *data, uint16_t length);

/**
 * @brief 调试任务周期处理 (需在主循环中周期调用)
 */
void BoardDebug_Task_Process(void);

/**
 * @brief 获取功耗测试使能状态
 * @return 1=使能, 0=禁用
 */
uint8_t BoardDebug_IsPwrTestEnabled(void);

/**
 * @brief 停止所有调试任务
 */
void BoardDebug_StopAllTasks(void);

/**
 * @brief 查询是否处于控制模式
 * @return 1=处于控制模式, 0=未处于控制模式
 */
uint8_t BoardDebug_IsControlModeActive(void);

/**
 * @brief 退出控制模式
 */
void BoardDebug_ExitControlMode(void);

/**
 * @brief 控制模式循环
 * @param uart_process_callback 串口数据处理回调函数，用于在循环中处理新的命令
 * @note 调用此函数后会进入控制模式循环，直到control_mode_active被设为0
 *       在循环中会持续执行功耗测试、电压采集等任务
 *       同时通过回调函数处理串口数据以接收退出命令
 */
void BoardDebug_ControlMode_Loop(void (*uart_process_callback)(void));

/**
 * @brief 控制模式循环（带超时）
 * @param uart_process_callback 串口数据处理回调函数
 * @param timeout_ms 超时时间(毫秒), 0=无超时
 * @return 0=正常退出(收到退出命令), 1=超时退出
 */
uint8_t BoardDebug_ControlMode_Loop_Timeout(void (*uart_process_callback)(void),
                                            uint32_t timeout_ms);

#ifdef __cplusplus
}
#endif

#endif /* NB_18_DIAPHRAGMGAS_BOARD_DEBUG_H */
