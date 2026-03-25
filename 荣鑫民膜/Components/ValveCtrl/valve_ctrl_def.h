/**
 * @file valve_ctrl_def.h
 * @brief 阀门控制组件 - 类型定义
 * @version 2.0.0
 * @date 2024-12
 *
 * 本文件定义所有公用的数据结构和枚举类型，不包含任何函数实现。
 * Core层和Port层都依赖此文件。
 */

#ifndef __VALVE_CTRL_DEF_H__
#define __VALVE_CTRL_DEF_H__

#include <stdbool.h>
#include <stdint.h>

#ifdef __cplusplus
extern "C" {
#endif

/*============================================================================*/
/*                              枚举定义 */
/*============================================================================*/

/**
 * @brief 单步执行结果
 */
typedef enum {
  VT_STEP_IDLE,    /**< 空闲 */
  VT_STEP_BUSY,    /**< 执行中 */
  VT_STEP_SUCCESS, /**< 成功 */
  VT_STEP_TIMEOUT, /**< 超时 */
  VT_STEP_FAIL,    /**< 失败 */
  VT_STEP_MISMATCH /**< 响应不匹配，需要重发 */
} VT_StepResult;

/**
 * @brief 整体测试结果
 */
typedef enum {
  VT_IDLE,    /**< 空闲 */
  VT_RUNNING, /**< 运行中 */
  VT_SUCCESS, /**< 成功 */
  VT_TIMEOUT, /**< 超时 */
  VT_FAIL     /**< 失败 */
} VT_TestResult;

/**
 * @brief 失败原因枚举
 */
typedef enum {
  VT_FAIL_NONE = 0,             /**< 无错误 */
  VT_FAIL_CONFIG_TIMEOUT,       /**< 配置命令超时 */
  VT_FAIL_CONFIG_RETRY,         /**< 配置命令重试超限 */
  VT_FAIL_QUERY_TIMEOUT,        /**< 查询命令超时 */
  VT_FAIL_INITIAL_POS_OPEN,     /**< 初始状态：开阀到位异常 */
  VT_FAIL_INITIAL_POS_CLOSE,    /**< 初始状态：关阀到位异常 */
  VT_FAIL_INITIAL_VOLTAGE_A,    /**< 初始状态：电压A异常 */
  VT_FAIL_INITIAL_VOLTAGE_B,    /**< 初始状态：电压B异常 */
  VT_FAIL_INITIAL_RETRY,        /**< 初始状态检查重试超限 */
  VT_FAIL_OPEN_CMD_TIMEOUT,     /**< 开阀命令超时 */
  VT_FAIL_OPEN_DETECT_TIMEOUT,  /**< 开阀动作检测超时 */
  VT_FAIL_OPEN_STATE_CHECK,     /**< 开阀状态检查失败 */
  VT_FAIL_CLOSE_CMD_TIMEOUT,    /**< 关阀命令超时 */
  VT_FAIL_CLOSE_DETECT_TIMEOUT, /**< 关阀动作检测超时 */
  VT_FAIL_CLOSE_STATE_CHECK,    /**< 关阀状态检查失败 */
  VT_FAIL_TOTAL_TIMEOUT,        /**< 总超时 */
} VT_FailReason;

/**
 * @brief 测试流程步骤
 */
typedef enum {
  VT_STEP_INIT,                /**< 0: 初始化 */
  VT_STEP_CONFIG,              /**< 1: 发送配置，等待响应 */
  VT_STEP_QUERY_INITIAL,       /**< 2: 发送F003查询初始状态 */
  VT_STEP_CHECK_INITIAL,       /**< 3: 检查初始状态 */
  VT_STEP_SEND_OPEN,           /**< 4: 发送开阀命令 */
  VT_STEP_DETECT_OPENING,      /**< 5: 检测开阀动作 */
  VT_STEP_OUTPUT_OPEN_SIGNAL,  /**< 6: 输出开阀到位信号 */
  VT_STEP_QUERY_OPEN_STATE,    /**< 7: 发送F003查询开阀后状态 */
  VT_STEP_CHECK_OPEN_STATE,    /**< 8: 检查开阀状态 */
  VT_STEP_SEND_CLOSE,          /**< 9: 发送关阀命令 */
  VT_STEP_DETECT_CLOSING,      /**< 10: 检测关阀动作 */
  VT_STEP_OUTPUT_CLOSE_SIGNAL, /**< 11: 输出关阀到位信号 */
  VT_STEP_QUERY_CLOSE_STATE,   /**< 12: 发送F003查询关阀后状态 */
  VT_STEP_CHECK_CLOSE_STATE,   /**< 13: 检查关阀状态 */
  VT_STEP_EVALUATE,            /**< 14: 评估测试结果 */
  VT_STEP_DONE                 /**< 15: 测试完成 */
} VT_TestStep;

/**
 * @brief 表类型枚举
 */
typedef enum {
  VALVE_METER_MECHANICAL = 0, /**< 机械表 */
  VALVE_METER_ULTRASONIC = 1  /**< 超声波表 */
} ValveMeterType;

/*============================================================================*/
/*                              结构体定义 */
/*============================================================================*/

/**
 * @brief 硬件抽象接口 (HAL)
 *
 * Core层通过这些函数指针操作硬件，实现零硬件依赖。
 * Port层负责实现这些函数并注册到Core。
 */
typedef struct {
  /* ========== 读取接口 ========== */
  uint32_t (*read_voltage_a)(void); /**< 读取阀门电压A (mV) */
  uint32_t (*read_voltage_b)(void); /**< 读取阀门电压B (mV) */
  uint8_t (*read_pos_open)(void);   /**< 读取开阀到位信号 */
  uint8_t (*read_pos_close)(void);  /**< 读取关阀到位信号 */

  /* ========== 协议发送接口 ========== */
  void (*send_config)(void);      /**< 发送配置命令 */
  void (*send_open_valve)(void);  /**< 发送开阀命令 */
  void (*send_close_valve)(void); /**< 发送关阀命令 */
  void (*send_read_status)(void); /**< 发送状态查询命令 */

  /* ========== GPIO输出接口 ========== */
  void (*output_valve_position_signals)(
      uint8_t open_signal, uint8_t close_signal); /**< 输出到位信号 */
  void (*restore_gpio_to_input)(void); /**< 恢复GPIO为输入模式(防止电平冲突) */

  /* ========== 系统接口 ========== */
  uint32_t (*get_tick_ms)(void);       /**< 获取系统时间戳 (ms) */
  void (*set_soft_delay)(uint32_t ms); /**< 设置软件延时 */
  bool (*is_soft_delay_done)(void);    /**< 查询软件延时是否完成 */

  /* ========== 配置查询接口 ========== */
  ValveMeterType (*get_meter_type)(void);     /**< 获取表类型 */
  uint16_t (*get_expected_config_code)(void); /**< 获取期望的配置响应码 */

  /* ========== 调试接口 (可选) ========== */
  void (*debug_print)(const char *fmt, ...); /**< 格式化调试输出 */
} ValveCtrl_HAL_t;

/**
 * @brief 阀门测试上下文 (封装所有运行时状态)
 */
typedef struct {
  /* ========== 流程控制 ========== */
  VT_TestStep current_step; /**< 当前步骤 */
  VT_TestResult result;     /**< 测试结果 */
  uint8_t enabled;          /**< 测试使能标志 */

  /* ========== 时间管理 ========== */
  uint32_t total_time_ms;    /**< 总运行时间 */
  uint32_t total_timeout_ms; /**< 总超时时间 */
  uint32_t step_time_ms;     /**< 当前步骤运行时间 */
  uint32_t step_timeout_ms;  /**< 当前步骤超时时间 */

  /* ========== 重试管理 ========== */
  uint8_t retry_count; /**< 当前重试次数 */
  uint8_t retry_max;   /**< 最大重试次数 */

  /* ========== 测试数据 ========== */
  uint32_t voltage_a; /**< 阀门电压A (mV) */
  uint32_t voltage_b; /**< 阀门电压B (mV) */
  uint8_t pos_open;   /**< 开阀到位标志 */
  uint8_t pos_close;  /**< 关阀到位标志 */

  /* ========== 初始状态备份 ========== */
  uint32_t initial_voltage_a; /**< 初始电压A */
  uint32_t initial_voltage_b; /**< 初始电压B */
  uint8_t initial_pos_open;   /**< 初始开阀到位 */
  uint8_t initial_pos_close;  /**< 初始关阀到位 */

  /* ========== 协议交互 ========== */
  uint8_t response_received; /**< 响应接收标志: 0=未发送, 1=收到, 2=等待中 */
  uint16_t response_code; /**< 收到的响应码 */

  /* ========== 配置参数 ========== */
  uint8_t config_param1; /**< 配置参数1 */
  uint8_t config_param2; /**< 配置参数2 */

  /* ========== 失败信息 ========== */
  VT_FailReason fail_reason; /**< 失败原因 */
  VT_TestStep fail_step;     /**< 失败时的步骤 */

  /* ========== 缓存配置 (Start时初始化，避免循环中重复HAL调用) ========== */
  ValveMeterType meter_type;     /**< 表类型 (缓存) */
  uint16_t expected_config_code; /**< 期望的配置响应码 (缓存) */

  /* ========== HAL绑定 ========== */
  const ValveCtrl_HAL_t *hal; /**< 硬件抽象层指针 */
} ValveCtrl_Context_t;

/*============================================================================*/
/*                              阈值常量定义 */
/*============================================================================*/

/** 电压阈值定义 (mV) */
#define VALVE_VOLTAGE_LOW_THRESHOLD 100 /**< 低电压阈值: <100mV表示无动作 */
#define VALVE_VOLTAGE_HIGH_THRESHOLD                                           \
  2800 /**< 高电压阈值: >2800mV表示有动作                            \
        */

/** 超时定义 (ms) */
#define VALVE_TOTAL_TIMEOUT_MS 60000        /**< 总测试超时: 60秒 */
#define VALVE_CONFIG_TIMEOUT_MS 10000       /**< 配置步骤超时: 10秒 */
#define VALVE_INITIAL_CHECK_TIMEOUT_MS 5000 /**< 初始状态检查超时: 5秒 */
#define VALVE_OPEN_CMD_TIMEOUT_MS 5000      /**< 开阀命令超时: 5秒 */
#define VALVE_OPEN_DETECT_TIMEOUT_MS 5000   /**< 开阀检测超时: 5秒 */
#define VALVE_CLOSE_CMD_TIMEOUT_MS 5000     /**< 关阀命令超时: 5秒 */
#define VALVE_CLOSE_DETECT_TIMEOUT_MS 15000 /**< 关阀检测超时: 15秒 */
#define VALVE_STATE_CHECK_TIMEOUT_MS 5000   /**< 状态检查超时: 5秒 */

/** 软件延时定义 (ms) */
#define VALVE_CONFIG_DELAY_MS 500 /**< 配置命令后延时: 500ms */
#define VALVE_CMD_DELAY_MS 500    /**< 开/关阀命令后延时: 500ms */
#define VALVE_SIGNAL_DELAY_MS 500 /**< 到位信号延时: 500ms */

/** 重试次数 */
#define VALVE_MAX_RETRY_COUNT 3 /**< 最大重试次数 */

/*============================================================================*/
/*                              协议码定义 */
/*============================================================================*/

/**
 * @brief 水表通信协议命令码
 *
 * 这些协议码用于与水表通信，发送配置和控制命令。
 * 协议格式: 高字节表示功能分组，低字节表示具体命令。
 */

/** 阀门控制命令 (0xC0xx 系列) */
#define PROTOCOL_VALVE_CONTROL 0xC022 /**< 阀门控制(开/关阀)命令码 */
#define PROTOCOL_VALVE_RESET_FLOW 0xC021 /**< 清零累计流量命令码 */

/** 表配置命令 (0x20xx 系列) */
#define PROTOCOL_CONFIG_ULTRASONIC 0x2036 /**< 超声波表配置命令码 */
#define PROTOCOL_CONFIG_MECHANICAL 0x2604 /**< 机械表配置命令码 */

/** 查询命令 (0xF0xx 系列) */
#define PROTOCOL_QUERY_STATUS 0xF003   /**< 查询水表状态命令码 */
#define PROTOCOL_QUERY_METER_ID 0x2031 /**< 查询表号命令码 */
#define PROTOCOL_QUERY_VERSION 0x2011  /**< 查询版本号命令码 */
#define PROTOCOL_QUERY_FLOW 0x9010     /**< 查询累计流量命令码 */

/** 温度配置命令 (0x2Bxx 系列) */
#define PROTOCOL_CONFIG_TEMPERATURE 0x2B00 /**< 温度配置命令码 */

/** 主动上报命令 (0xF0xx 系列) */
#define PROTOCOL_TRIGGER_REPORT 0xF001 /**< 触发主动上报命令码 */

#ifdef __cplusplus
}
#endif

#endif /* __VALVE_CTRL_DEF_H__ */
