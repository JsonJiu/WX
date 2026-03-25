# ValveCtrl 组件

阀门控制测试组件，采用分层架构设计，实现硬件解耦。

## 架构设计

```
┌─────────────────────────────────────────────────────────┐
│                     Application                          │
│                      (main.c)                            │
└─────────────────────┬───────────────────────────────────┘
                      │ #include "valve_ctrl.h"
                      ▼
┌─────────────────────────────────────────────────────────┐
│                   valve_ctrl.h                           │
│               (对外统一接口)                              │
└─────────────────────┬───────────────────────────────────┘
                      │
          ┌───────────┴───────────┐
          ▼                       ▼
┌─────────────────┐     ┌─────────────────┐
│ valve_ctrl_port │     │ valve_ctrl_core │
│   (适配层)       │────▶│   (核心层)       │
│                 │     │                 │
│ ✗ 包含硬件依赖   │     │ ✓ 零硬件依赖     │
│ ✗ 项目特定      │     │ ✓ 可移植        │
│ ✓ 实现HAL接口   │     │ ✓ 可单元测试    │
└─────────────────┘     └─────────────────┘
          │                       │
          └───────────┬───────────┘
                      ▼
          ┌─────────────────────┐
          │  valve_ctrl_def.h   │
          │    (类型定义)        │
          └─────────────────────┘
```

## 文件说明

| 文件 | 用途 | 硬件依赖 |
|------|------|----------|
| `valve_ctrl.h` | 对外统一接口，用户只需包含这个 | 无 |
| `valve_ctrl_def.h` | 枚举、结构体、常量、协议码定义 | 无 |
| `valve_ctrl_core.h/c` | 核心状态机逻辑 | **无** |
| `valve_ctrl_port.h/c` | 硬件适配层（胶水代码） | **有** |

## 核心状态机流程

```
┌─────────────┐
│  VT_STEP_   │
│    INIT     │ ← 初始化, 记录初始状态
└──────┬──────┘
       ▼
┌─────────────┐     ┌────────────────────────────────────┐
│  VT_STEP_   │     │ 发送配置命令:                      │
│   CONFIG    │────▶│ - 超声波表: 0x2036                 │
└──────┬──────┘     │ - 机械表:   0x2604                 │
       ▼            └────────────────────────────────────┘
┌─────────────┐
│  VT_STEP_   │ ← 检查初始电压状态
│CHECK_INITIAL│   期望: A>2800mV, B<100mV
└──────┬──────┘
       ▼
┌─────────────┐
│  VT_STEP_   │ ← 发送开阀命令 (0xC022)
│  SEND_OPEN  │
└──────┬──────┘
       ▼
┌─────────────┐
│  VT_STEP_   │ ← 检测开阀动作
│DETECT_OPENING│  期望: A>2800mV, B<100mV
└──────┬──────┘
       ▼
┌─────────────┐
│  VT_STEP_   │ ← 输出开阀到位信号
│OUTPUT_OPEN_ │   PA11=低电平(到位)
│   SIGNAL    │
└──────┬──────┘
       ▼
┌─────────────┐
│  VT_STEP_   │ ← 检查开阀后状态
│CHECK_OPEN_  │   期望: A<100mV, B<100mV
│   STATE     │
└──────┬──────┘
       ▼
┌─────────────┐
│  VT_STEP_   │ ← 发送关阀命令 (0xC022)
│ SEND_CLOSE  │
└──────┬──────┘
       ▼
┌─────────────┐
│  VT_STEP_   │ ← 检测关阀动作(反转)
│DETECT_CLOSING│  期望: A<100mV, B>2800mV
└──────┬──────┘
       ▼
┌─────────────┐
│  VT_STEP_   │ ← 输出关阀到位信号
│OUTPUT_CLOSE_│   PA3=低电平(到位)
│   SIGNAL    │
└──────┬──────┘
       ▼
┌─────────────┐
│  VT_STEP_   │ ← 检查关阀后状态
│CHECK_CLOSE_ │   期望: A<100mV, B<100mV
│   STATE     │
└──────┬──────┘
       ▼
┌─────────────┐
│  VT_STEP_   │ ← 评估测试结果
│  EVALUATE   │
└──────┬──────┘
       ▼
┌─────────────┐
│  VT_STEP_   │ ← 测试完成
│    DONE     │
└─────────────┘
```

## 协议码定义

在 `valve_ctrl_def.h` 中定义了所有协议码，避免魔法数字：

```c
/** 阀门控制命令 (0xC0xx 系列) */
#define PROTOCOL_VALVE_CONTROL      0xC022  // 阀门控制(开/关阀)命令码
#define PROTOCOL_VALVE_RESET_FLOW   0xC021  // 清零累计流量命令码

/** 表配置命令 (0x20xx 系列) */
#define PROTOCOL_CONFIG_ULTRASONIC  0x2036  // 超声波表配置命令码
#define PROTOCOL_CONFIG_MECHANICAL  0x2604  // 机械表配置命令码

/** 查询命令 (0xF0xx 系列) */
#define PROTOCOL_QUERY_STATUS       0xF003  // 查询水表状态命令码
```

## ValveCtrl_Context_t 上下文说明

`ValveCtrl_Context_t` 是组件的核心数据结构，封装了所有运行时状态：

| 成员 | 类型 | 说明 |
|------|------|------|
| `current_step` | `VT_TestStep` | 当前测试步骤 |
| `result` | `VT_TestResult` | 测试结果 (IDLE/RUNNING/SUCCESS/FAIL) |
| `enabled` | `uint8_t` | 测试使能标志 |
| `total_time_ms` | `uint32_t` | 总运行时间 (ms) |
| `step_time_ms` | `uint32_t` | 当前步骤运行时间 (ms) |
| `voltage_a/b` | `uint32_t` | 实时电压读数 (mV) |
| `response_received` | `uint8_t` | 响应接收状态 |
| `fail_reason` | `VT_FailReason` | 失败原因码 |
| `meter_type` | `ValveMeterType` | 表类型 (缓存) |
| `hal` | `ValveCtrl_HAL_t*` | HAL层指针 |

通过 `ValveCtrl_GetContext()` 获取上下文指针，可用于：
- 调试时查看内部状态
- 高级场景下直接访问测试数据

## HAL 接口详细说明

`ValveCtrl_HAL_t` 结构体定义了硬件抽象接口，移植时需实现：

### 读取接口

| 函数指针 | 原型 | 说明 |
|----------|------|------|
| `read_voltage_a` | `uint32_t (*)(void)` | 读取阀门电压A，返回值单位 mV |
| `read_voltage_b` | `uint32_t (*)(void)` | 读取阀门电压B，返回值单位 mV |
| `read_pos_open` | `uint8_t (*)(void)` | 读取开阀到位信号，1=到位, 0=未到位 |
| `read_pos_close` | `uint8_t (*)(void)` | 读取关阀到位信号，1=到位, 0=未到位 |

### 协议发送接口

| 函数指针 | 原型 | 说明 |
|----------|------|------|
| `send_config` | `void (*)(void)` | 发送配置命令 (0x2036/0x2604) |
| `send_open_valve` | `void (*)(void)` | 发送开阀命令 (0xC022 + 开阀参数) |
| `send_close_valve` | `void (*)(void)` | 发送关阀命令 (0xC022 + 关阀参数) |
| `send_read_status` | `void (*)(void)` | 发送状态查询命令 (0xF003) |

### GPIO 输出接口

| 函数指针 | 原型 | 说明 |
|----------|------|------|
| `output_valve_position_signals` | `void (*)(uint8_t open, uint8_t close)` | 输出到位信号，open/close: 1=到位(低电平), 0=未到位(高电平) |
| `restore_gpio_to_input` | `void (*)(void)` | 恢复GPIO为输入模式，防止与水表输出冲突 |

### 系统接口

| 函数指针 | 原型 | 说明 |
|----------|------|------|
| `get_tick_ms` | `uint32_t (*)(void)` | 获取系统时间戳 (ms)，用于超时判断 |
| `set_soft_delay` | `void (*)(uint32_t ms)` | 设置软件延时，用于步骤间等待 |
| `is_soft_delay_done` | `bool (*)(void)` | 查询软件延时是否完成 |

### 配置查询接口

| 函数指针 | 原型 | 说明 |
|----------|------|------|
| `get_meter_type` | `ValveMeterType (*)(void)` | 获取表类型 (机械/超声波) |
| `get_expected_config_code` | `uint16_t (*)(void)` | 获取期望的配置响应码 |

### 调试接口 (可选)

| 函数指针 | 原型 | 说明 |
|----------|------|------|
| `debug_print` | `void (*)(const char *fmt, ...)` | 格式化调试输出，可设为 NULL 禁用 |

## 使用方法

### 1. 包含头文件

```c
#include "valve_ctrl.h"
```

### 2. 初始化

```c
void main(void) {
    // 硬件初始化...

    // 初始化阀门控制组件
    ValveCtrl_Init();

    while (1) {
        // 主循环处理 (每10ms调用一次)
        ValveCtrl_Process(10);

        // 根据条件启动测试
        if (need_start_test) {
            ValveCtrl_Start();
        }
    }
}
```

### 3. 协议响应回调

在协议解析代码中调用：

```c
void on_protocol_response(uint16_t code) {
    // 通知阀门控制组件收到响应
    ValveCtrl_OnResponse(code);
}
```

### 4. 状态查询

```c
// 检查是否正在运行
if (ValveCtrl_IsRunning()) {
    // ...
}

// 获取测试结果
VT_TestResult result = ValveCtrl_GetResult();

// 获取失败原因
if (result == VT_FAIL) {
    const char *reason = ValveCtrl_GetFailReasonStr();
    printf("失败原因: %s\n", reason);
}

// 获取上下文查看详细状态
ValveCtrl_Context_t *ctx = ValveCtrl_GetContext();
printf("当前步骤: %d, 耗时: %lums\n", ctx->current_step, ctx->total_time_ms);
```

## API 迁移指南

| 旧 API (valve_test.h) | 新 API (valve_ctrl.h) |
|-----------------------|----------------------|
| `valve_test_init()` | `ValveCtrl_Init()` |
| `valve_test_start()` | `ValveCtrl_Start()` |
| `valve_test_stop()` | `ValveCtrl_Stop()` |
| `valve_test_loop()` | `ValveCtrl_Process()` |
| `valve_test_on_response()` | `ValveCtrl_OnResponse()` |
| `valve_test_get_result()` | `ValveCtrl_GetResult()` |
| `valve_test_get_step()` | `ValveCtrl_GetStep()` |
| `valve_test_get_fail_reason()` | `ValveCtrl_GetFailReason()` |
| `g_valve_ctx` | `ValveCtrl_GetContext()` |

## 移植到其他项目

只需修改 `valve_ctrl_port.c`，实现 HAL 接口：

```c
static const ValveCtrl_HAL_t g_valve_hal = {
    /* 读取接口 */
    .read_voltage_a   = your_read_voltage_a,    // 返回mV
    .read_voltage_b   = your_read_voltage_b,    // 返回mV
    .read_pos_open    = your_read_pos_open,     // 返回0/1
    .read_pos_close   = your_read_pos_close,    // 返回0/1

    /* 协议发送接口 */
    .send_config      = your_send_config,       // 发送0x2036或0x2604
    .send_open_valve  = your_send_open_valve,   // 发送0xC022+开阀参数
    .send_close_valve = your_send_close_valve,  // 发送0xC022+关阀参数
    .send_read_status = your_send_read_status,  // 发送0xF003

    /* GPIO输出接口 */
    .output_valve_position_signals = your_output_signals,
    .restore_gpio_to_input = your_restore_gpio,

    /* 系统接口 */
    .get_tick_ms      = your_get_tick_ms,
    .set_soft_delay   = your_set_delay,
    .is_soft_delay_done = your_check_delay,

    /* 配置查询接口 */
    .get_meter_type   = your_get_meter_type,
    .get_expected_config_code = your_get_config_code,

    /* 调试接口 (可选) */
    .debug_print      = your_debug_print,       // 可设为NULL
};
```

## 设计优势

1. **零依赖核心** - `valve_ctrl_core.c` 不包含任何 `main.h` 或硬件头文件
2. **可移植** - 复制 `Components/ValveCtrl/` 到其他项目，只需修改 `port.c`
3. **可测试** - Core 层可在 PC 上用 Mock 函数进行单元测试
4. **低耦合** - 核心逻辑与硬件实现完全分离
5. **易维护** - 修改硬件驱动不影响核心状态机
6. **语义清晰** - 协议码使用宏定义，避免魔法数字

## 常量配置

在 `valve_ctrl_def.h` 中可调整以下参数：

```c
/* 电压阈值 (mV) */
#define VALVE_VOLTAGE_LOW_THRESHOLD  100   // <100mV 表示无动作
#define VALVE_VOLTAGE_HIGH_THRESHOLD 2800  // >2800mV 表示有动作

/* 超时设置 (ms) */
#define VALVE_TOTAL_TIMEOUT_MS       60000 // 总测试超时: 60秒
#define VALVE_CONFIG_TIMEOUT_MS      10000 // 配置步骤超时: 10秒
#define VALVE_OPEN_DETECT_TIMEOUT_MS 5000  // 开阀检测超时: 5秒
#define VALVE_CLOSE_DETECT_TIMEOUT_MS 15000 // 关阀检测超时: 15秒

/* 重试次数 */
#define VALVE_MAX_RETRY_COUNT        3     // 最大重试次数
```
