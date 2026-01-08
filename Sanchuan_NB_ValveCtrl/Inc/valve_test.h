#ifndef __VALVE_TEST_H__
#define __VALVE_TEST_H__

#include <stdint.h>

/*============ 1. 状态定义（分离结果和步骤） ============*/

// 单步执行结果
typedef enum {
  VT_STEP_IDLE,
  VT_STEP_BUSY,
  VT_STEP_SUCCESS,
  VT_STEP_TIMEOUT,
  VT_STEP_FAIL,
  VT_STEP_MISMATCH // 响应不匹配，需要重发
} VT_StepResult;

// 整体测试结果
typedef enum {
  VT_IDLE,
  VT_RUNNING,
  VT_SUCCESS,
  VT_TIMEOUT,
  VT_FAIL
} VT_TestResult;

// 失败原因枚举
typedef enum {
  VT_FAIL_NONE = 0,             // 无错误
  VT_FAIL_CONFIG_TIMEOUT,       // 配置命令超时
  VT_FAIL_CONFIG_RETRY,         // 配置命令重试超限
  VT_FAIL_QUERY_TIMEOUT,        // 查询命令超时
  VT_FAIL_INITIAL_POS_OPEN,     // 初始状态：开阀到位异常
  VT_FAIL_INITIAL_POS_CLOSE,    // 初始状态：关阀到位异常
  VT_FAIL_INITIAL_VOLTAGE_A,    // 初始状态：电压A异常
  VT_FAIL_INITIAL_VOLTAGE_B,    // 初始状态：电压B异常
  VT_FAIL_INITIAL_RETRY,        // 初始状态检查重试超限
  VT_FAIL_OPEN_CMD_TIMEOUT,     // 开阀命令超时
  VT_FAIL_OPEN_DETECT_TIMEOUT,  // 开阀动作检测超时
  VT_FAIL_OPEN_STATE_CHECK,     // 开阀状态检查失败
  VT_FAIL_CLOSE_CMD_TIMEOUT,    // 关阀命令超时
  VT_FAIL_CLOSE_DETECT_TIMEOUT, // 关阀动作检测超时
  VT_FAIL_CLOSE_STATE_CHECK,    // 关阀状态检查失败
  VT_FAIL_TOTAL_TIMEOUT,        // 总超时
} VT_FailReason;

// 测试流程步骤
typedef enum {
  VT_STEP_INIT,           // 0: 初始化
  VT_STEP_CONFIG,         // 1: 发送配置，等待响应
  VT_STEP_QUERY_INITIAL,  // 2: 发送F003查询初始状态
  VT_STEP_CHECK_INITIAL,  // 3: 检查初始状态 (open=0,close=0,A<100,B<100)
  VT_STEP_SEND_OPEN,      // 4: 发送开阀命令
  VT_STEP_DETECT_OPENING, // 5: 检测开阀动作 (A>2800,B<100)
  VT_STEP_OUTPUT_OPEN_SIGNAL, // 6: 输出开阀到位信号 (open=1,close=0)
  VT_STEP_QUERY_OPEN_STATE,   // 7: 发送F003查询开阀后状态
  VT_STEP_CHECK_OPEN_STATE, // 8: 检查开阀状态 (open=1,close=0,A<100,B<100)
  VT_STEP_SEND_CLOSE,       // 9: 发送关阀命令
  VT_STEP_DETECT_CLOSING,   // 10: 检测关阀动作 (A<100,B>2800)
  VT_STEP_OUTPUT_CLOSE_SIGNAL, // 11: 输出关阀到位信号 (open=0,close=1)
  VT_STEP_QUERY_CLOSE_STATE,   // 12: 发送F003查询关阀后状态
  VT_STEP_CHECK_CLOSE_STATE, // 13: 检查关阀状态 (open=0,close=1,A<100,B<100)
  VT_STEP_EVALUATE,          // 14: 评估测试结果
  VT_STEP_DONE               // 15: 测试完成
} VT_TestStep;

/*============ 2. 测试上下文（封装所有状态） ============*/

typedef struct {
  // 流程控制
  VT_TestStep current_step;
  VT_TestResult result;
  uint8_t enabled;

  // 时间管理
  uint32_t total_time_ms;
  uint32_t total_timeout_ms;
  uint32_t step_time_ms;
  uint32_t step_timeout_ms;
  uint32_t software_delay_ms; // 软件延时，给水表处理时间

  // 重试管理
  uint8_t retry_count;
  uint8_t retry_max;

  // 测试数据
  uint32_t voltage_a; // 阀门电压A (DRV8837 输出A)
  uint32_t voltage_b; // 阀门电压B (DRV8837 输出B)
  uint8_t pos_open;   // 开阀到位标志
  uint8_t pos_close;  // 关阀到位标志

  // 初始状态（每次步骤变更前获取）
  uint32_t initial_voltage_a; // 初始电压A
  uint32_t initial_voltage_b; // 初始电压B
  uint8_t initial_pos_open;   // 初始开阀到位
  uint8_t initial_pos_close;  // 初始关阀到位

  // 协议交互
  uint8_t response_received;
  uint16_t response_code;

  // 配置参数
  uint8_t config_param1; // 配置参数1 (如: 时间)
  uint8_t config_param2; // 配置参数2 (如: 电压阈值)

  // 失败信息
  VT_FailReason fail_reason; // 失败原因
  VT_TestStep fail_step;     // 失败时的步骤
} ValveTestContext;

/*============ 3. 硬件抽象接口（回调统一注册） ============*/

typedef struct {
  // MCU本地读取
  uint32_t (*read_voltage_a)(void);
  uint32_t (*read_voltage_b)(void);
  uint8_t (*read_pos_open)(void);
  uint8_t (*read_pos_close)(void);

  // 协议发送（不用等响应，响应通过中断/轮询更新context）
  void (*send_config)(uint8_t param1, uint8_t param2);
  void (*send_open_valve)(void);
  void (*send_close_valve)(void);
  void (*send_read_status)(void); // 发送F003查询状态

  // GPIO输出（模拟到位信号给水表）
  void (*output_valve_position_signals)(uint8_t open_signal,
                                        uint8_t close_signal);

  // 调试输出（可选）
  void (*debug_print)(const char *msg);
} ValveTestHAL;

/*============ 4. 对外接口 ============*/

// 初始化阀门测试模块
void valve_test_init(ValveTestContext *ctx, const ValveTestHAL *hal);

// 启动阀门测试
void valve_test_start(ValveTestContext *ctx);

// 停止阀门测试
void valve_test_stop(ValveTestContext *ctx);

// 主循环调用（tick_ms为调用间隔，通常10ms）
VT_TestResult valve_test_loop(ValveTestContext *ctx, uint32_t tick_ms);

// 供协议解析调用，通知收到响应
void valve_test_on_response(ValveTestContext *ctx, uint16_t response_code);

// 获取当前测试结果
VT_TestResult valve_test_get_result(ValveTestContext *ctx);

// 获取当前步骤
VT_TestStep valve_test_get_step(ValveTestContext *ctx);

// 获取失败原因
VT_FailReason valve_test_get_fail_reason(ValveTestContext *ctx);

// 获取失败原因描述字符串
const char *valve_test_get_fail_reason_str(VT_FailReason reason);

// 获取步骤名称字符串
const char *valve_test_get_step_name(VT_TestStep step);

/*============ 5. 全局变量声明 ============*/

extern uint8_t enable_valve_test;
extern ValveTestContext g_valve_ctx;

#endif // __VALVE_TEST_H__
