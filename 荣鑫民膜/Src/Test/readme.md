# 测试框架架构说明

## 目录结构

```
Src/Test/
├── common_test_configure.h       # 通用测试配置头文件（核心配置入口）
├── common_test_configure.c       # 通用测试函数实现
├── readme.md                     # 本说明文档
│
├── NB_18_DiaphragmGas_Test/      # 膜式燃气表测试模块
│   ├── diaphragme_meter_test.h   # 膜式燃气表测试头文件
│   ├── diaphragme_meter_test.c   # 膜式燃气表测试实现
│   ├── NB_18_DiaphragmGas_port.h # 硬件抽象层接口定义
│   └── NB_18_DiaphragmGas_port.c # 硬件抽象层实现
│
└── Domestic_water_meter_Test/    # 民用水表测试模块
    ├── domestic_water_meter_test.h # 民用水表测试头文件
    └── domestic_water_meter_test.c # 民用水表测试实现
```

---

## 核心文件说明

### 1. `common_test_configure.h` - 通用测试配置头文件

**这是整个测试框架的核心配置入口**

#### 功能：
- **宏定义配置测试模块**：通过注释/取消注释来启用/禁用对应的测试模块
  ```c
  #define ENABLE_DIAPHRAGM_METER_TEST          // 膜式燃气表（当前启用）
  // #define ENABLE_DOMESTIC_WATER_METER_TEST  // 民用水表
  // #define ENABLE_ULTRASONIC_WATER_METER_TEST // 超声波水表
  ```
- **统一管理头文件引用**：根据宏定义自动包含对应测试模块的头文件
- **定义通用数据类型**：测试状态枚举、回调函数类型、配置结构体等

### 2. `common_test_configure.c` - 通用测试函数实现

#### 提供的通用函数：
| 函数 | 说明 |
|------|------|
| `common_test_data_clear()` | 测试数据结果清零 |
| `common_get_gongwei_number()` | 获取当前工位编号 |
| `common_test_init()` | 测试初始化（支持回调函数） |
| `common_set_test_status()` | 设置测试状态及描述 |
| `common_test_timer_config()` | 配置计时器（全局/单步超时） |
| `common_check_global_timeout()` | 检查全局超时 |
| `common_check_step_timeout()` | 检查单步超时 |
| `common_reset_step_timeout()` | 重置单步超时计数器 |
| `common_print_step_header()` | 打印测试步骤头部信息 |
| `common_print_step_separator()` | 打印分隔符 |
| `common_print_step_footer()` | 打印测试步骤结果 |
| `common_report_test_result()` | 上报测试结果（通过回调） |
| `common_test_check_timeout()` | 通用超时错误处理 |
| `common_is_test_finished()` | 判断测试是否完成 |
| `common_is_test_success()` | 判断测试是否成功 |
| `common_get_status_string()` | 获取测试状态描述字符串 |
| `test_start()` | 测试入口函数 |
| `auto_test_loop()` | 自动测试循环 |

#### 回调函数机制：
```c
// 初始化回调 - 不同测试有不同的初始化方式
typedef void (*TestInitCallback)(void);

// 结果回调 - 测试完成后的结果处理
typedef void (*TestResultCallback)(CommonTestStatus_t status,
                                   const uint8_t *result_data,
                                   uint16_t data_len);
```

---

## 测试模块文件

### 3. `NB_18_DiaphragmGas_Test/` - 膜式燃气表测试模块

#### `diaphragme_meter_test.h/c` - 测试逻辑
- 测试阈值宏定义（功耗、电压、温度等）
- 测试流程枚举 `DiaphragmTestStep_t`
- 测试结果结构体 `DiaphragmTestResult_t`
- 测试主循环 `diaphragm_test_loop()`
- 各测试步骤处理函数 `diaphragm_handle_xxx()`

#### `NB_18_DiaphragmGas_port.h/c` - 硬件抽象层 (HAL)
- **分层架构设计**：业务逻辑与底层驱动分离
- GPIO引脚配置、ADC通道定义
- 电压/功耗范围宏定义
- 底层驱动回调接口
- 移植时只需修改配置和回调实现

### 4. `Domestic_water_meter_Test/` - 民用水表测试模块

#### `domestic_water_meter_test.h/c`
- 测试阈值宏定义
- 阀门配置结构体（机械表/超声波表）
- 测试流程枚举 `Test_liucheng`
- 测试结果结构体 `Test_jieguo`
- 测试主循环 `Test_loop_func()`
- 各测试步骤处理函数 `handle_xxx()`

---

## 使用方法

### 启用/禁用测试模块

在 `common_test_configure.h` 中修改宏定义：

```c
// 启用膜式燃气表测试
#define ENABLE_DIAPHRAGM_METER_TEST

// 禁用民用水表测试（注释掉）
// #define ENABLE_DOMESTIC_WATER_METER_TEST

// 可同时启用多个模块
```

### 添加新的测试模块

1. 创建头文件 `xxx_meter_test.h`：
   - 定义测试阈值宏
   - 定义测试流程枚举
   - 定义测试结果结构体
   - 声明测试函数

2. 创建实现文件 `xxx_meter_test.c`：
   - 实现测试初始化
   - 实现测试主循环
   - 实现各步骤处理函数

3. 在 `common_test_configure.h` 中添加：
   ```c
   // 添加宏定义
   // #define ENABLE_XXX_METER_TEST

   // 添加条件包含
   #ifdef ENABLE_XXX_METER_TEST
   #include "xxx_meter_test.h"
   #endif
   ```

---

## 架构图

```
                    ┌─────────────────────────────────┐
                    │   common_test_configure.h       │
                    │   (核心配置 & 宏定义开关)        │
                    └─────────────┬───────────────────┘
                                  │
                                  │ 根据宏定义包含
                    ┌─────────────┴───────────────────┐
                    │                                 │
                    ▼                                 ▼
    ┌───────────────────────────┐     ┌───────────────────────────┐
    │ NB_18_DiaphragmGas_Test/  │     │ Domestic_water_meter_Test/│
    ├───────────────────────────┤     ├───────────────────────────┤
    │ diaphragme_meter_test.h/c │     │ domestic_water_meter_test │
    │ (测试逻辑)                │     │ .h/.c (测试逻辑)          │
    ├───────────────────────────┤     └───────────────────────────┘
    │ NB_18_DiaphragmGas_port   │
    │ .h/.c (硬件抽象层)        │
    └───────────────────────────┘
                    │                                 │
                    └─────────────┬───────────────────┘
                                  │
                                  ▼
                    ┌─────────────────────────────────┐
                    │   common_test_configure.c       │
                    │   (通用函数: 工位、计时、日志等)  │
                    └─────────────────────────────────┘
```

---

## 硬件抽象层 (HAL) 设计

膜式燃气表测试模块采用了分层设计，将硬件操作封装在 `NB_18_DiaphragmGas_port` 中：

| 层次           | 内容                     | 移植时需修改 |
|----------------|--------------------------|--------------|
| 硬件资源配置   | 引脚、通道、地址         | ✓ 仅改配置值 |
| 底层驱动回调   | gpio_write, adc_read等   | ✓ 适配新平台 |
| 业务接口       | power_on, read_voltage等 | ✗ 无需修改   |

---

## 测试状态说明

```c
typedef enum {
  COMMON_TEST_RUNNING = 0,        // 测试进行中
  COMMON_TEST_SUCCESS = 1,        // 测试正常完成
  COMMON_TEST_TIMEOUT_GLOBAL = 2, // 全局超时中断
  COMMON_TEST_TIMEOUT_STEP = 3,   // 单步超时中断
  COMMON_TEST_ERROR_HARDWARE = 4, // 硬件错误中断
  COMMON_TEST_ERROR_PROTOCOL = 5, // 协议错误中断
  COMMON_TEST_MANUAL_STOP = 6     // 手动停止测试
} CommonTestStatus_t;
```

---

## 工位检测说明

通过4个跳线帽引脚组合成工位编号 (1-15)：

| 丝印 | 引脚 | 位置 |
|------|------|------|
| 1    | PC0  | bit0 |
| 2    | PD12 | bit1 |
| 3    | PB15 | bit2 |
| 4    | PE5  | bit3 |

跳线帽插上为低电平(0)，表示该位为1。

---

## 文件命名规范

- 测试模块文件夹：`<产品类型>_Test/`
- 测试逻辑文件：`<产品名>_test.h/c`
- 硬件抽象层文件：`<产品名>_port.h/c`
