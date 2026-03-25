# 小口径超声波水表测试系统

基于 FM33LG0xx 微控制器的水表自动化测试平台。

## 功能特性

| 功能模块 | 描述 |
|---------|------|
| 静态功耗测试 | 测量水表待机功耗（阈值可配置） |
| 动态功耗测试 | 测量水表工作功耗 |
| 阀门控制测试 | 自动化阀门开/关/半开测试，带状态机驱动和超时检测 |
| 显示屏测试 | LCD 显示功能验证 |
| 通信协议测试 | 超声波/机械计量模块配置与查询 |
| 温度监测 | 实时温度采集与显示 |
| 主电源电压监测 | 电源电压测量与报警 |

## 系统架构

```
┌─────────────────────────────────────────────────────────┐
│                      PC 上位机                           │
│                    (UART0 协议)                          │
└─────────────────────────────────────────────────────────┘
                           │
                           ▼
┌─────────────────────────────────────────────────────────┐
│                    FM33LG0xx MCU                         │
│  ┌─────────────┐  ┌─────────────┐  ┌─────────────────┐  │
│  │   测试流程   │  │   阀门控制   │  │    协议处理     │  │
│  │  Test_List  │  │  ValveCtrl  │  │  protocol_uart  │  │
│  └─────────────┘  └─────────────┘  └─────────────────┘  │
│         │                │                 │            │
│         ▼                ▼                 ▼            │
│  ┌─────────────────────────────────────────────────────┐│
│  │              HAL 层 / BSP 驱动                       ││
│  │   UART0  │  UART1  │  UART3  │  GPIO  │  ADC       ││
│  └─────────────────────────────────────────────────────┘│
└─────────────────────────────────────────────────────────┘
         │           │           │
         ▼           ▼           ▼
    ┌────────┐  ┌────────┐  ┌──────────┐
    │ PC通信  │  │ 水表   │  │ 计量模块  │
    │ UART0  │  │ UART1  │  │  UART3   │
    └────────┘  └────────┘  └──────────┘
```

## 目录结构

```
small_caliber_uwm_tester/
├── Src/                    # 应用层源码
│   ├── Test_List.*         # 测试流程控制与状态机
│   ├── ValveCtrl/          # 阀门控制组件（Core/Port 分层）
│   │   ├── valve_ctrl_core.*   # 硬件无关的状态机逻辑
│   │   ├── valve_ctrl_port.*   # 硬件相关的 HAL 实现
│   │   └── valve_ctrl_def.h    # 类型定义与协议码
│   ├── protocol_uart0.*    # PC 上位机通信协议
│   ├── protocol_debug.*    # 调试输出接口
│   └── uart*.c             # UART 外设驱动
├── Drivers/                # FM33LG0xx HAL 库
├── cmake/                  # CMake 构建配置
├── CMakeLists.txt          # 主构建文件
└── README.md               # 本文件
```

## 开发环境

### 依赖项

| 工具 | 版本要求 | 用途 |
|------|---------|------|
| ARM GCC | 10.0+ | 交叉编译工具链 |
| CMake | 3.20+ | 构建系统 |
| Ninja | 1.10+ | 构建后端 |
| PyOCD | 0.34+ | 调试与烧录 |

### 构建

```bash
# 配置（首次）
cmake -B build -G Ninja

# 编译
cmake --build build

# 或使用 VS Code 任务
# Ctrl+Shift+B -> "Build OpenOCD"
```

### 烧录

```bash
# 使用 PyOCD 烧录
pyocd flash -t fm33lg0xx build/small_caliber_uwm_tester.hex

# 或通过 OpenOCD（如已配置）
# 运行任务 "Run OpenOCD (STM32F1)"
```

## 测试流程

测试按以下顺序执行：

```
1. 静态功耗测试 (GONGHAO)
   ├─ 通过 → 继续
   └─ 失败 → 停止测试
         ↓
2. 阀门测试 (VALVE)
   ├─ 关阀测试 → 开阀测试 → 半开测试
   └─ 任一失败 → 停止测试
         ↓
3. 显示测试 (DISPLAY)
         ↓
4. 动态功耗测试 (DONGTAI_GONGHAO)
         ↓
5. 测试完成，等待下一轮
```

### 测试阈值配置

所有测试阈值集中定义在 `Src/Test_List.h`：

| 宏定义 | 默认值 | 说明 |
|--------|-------|------|
| `GONGHAO_CLOSE_MIN` | 10 | 关阀静态功耗最小值 (μA) |
| `GONGHAO_CLOSE_MAX` | 40 | 关阀静态功耗最大值 (μA) |
| `GONGHAO_OPEN_MIN` | 10 | 开阀静态功耗最小值 (μA) |
| `GONGHAO_OPEN_MAX` | 40 | 开阀静态功耗最大值 (μA) |
| `GONGHAO_HALFOPEN_MIN` | 10 | 半开静态功耗最小值 (μA) |
| `GONGHAO_HALFOPEN_MAX` | 40 | 半开静态功耗最大值 (μA) |
| `ZHUDIAN_DIANYA_MIN` | 3000 | 主电源电压最小值 (mV) |
| `ZHUDIAN_DIANYA_MAX` | 3600 | 主电源电压最大值 (mV) |
| `WENDU_MIN` | -40 | 温度最小值 (°C) |
| `WENDU_MAX` | 60 | 温度最大值 (°C) |

## 通信协议

### 协议码定义

定义在 `Src/ValveCtrl/valve_ctrl_def.h`：

| 协议码 | 宏定义 | 功能 |
|--------|-------|------|
| `0xC022` | `PROTOCOL_VALVE_CONTROL` | 阀门控制命令 |
| `0xC021` | `PROTOCOL_RESET_FLOW` | 清零流量 |
| `0x2036` | `PROTOCOL_CONFIG_ULTRASONIC` | 超声波计量模块配置 |
| `0x2604` | `PROTOCOL_CONFIG_MECHANICAL` | 机械计量模块配置 |
| `0xF003` | `PROTOCOL_QUERY_STATUS` | 查询水表状态 |

### UART 端口分配

| 端口 | 波特率 | 用途 |
|------|-------|------|
| UART0 | 115200 | PC 上位机通信 |
| UART1 | 2400 | 水表通信（阀门控制） |
| UART3 | 9600 | 计量模块通信 |

## 模块文档

- [ValveCtrl 阀门控制组件](Components/ValveCtrl/README.md)
- [Protocol 协议框架](Components/Protocol/README.md)
- [版本历史](VERSION.md)

## 最新版本

**v2.8.0** (2026-01-07)
- volatile 修复: 中断共享变量安全
- main.c 架构重构
- EasyLogger 配置简化
- 测试步骤字符串库

## 许可证

内部项目，版权归前锋电子所有。
