# Sanchuan_NB_ValveCtrl

三川 NB-IoT 阀控水表测试工装程序

## 版本信息

- **当前版本**: 1.0.0
- **发布日期**: 2024-12-01
- **硬件平台**: FM33LG04x (Cortex-M0+)

## 项目说明

本项目为三川 NB-IoT 阀控水表的测试工装程序，用于生产线水表测试。

### 主要功能

- 水表通信协议测试
- 阀门控制测试
- 电流检测
- LED 状态指示
- 透传模式支持

## 编译环境

- **编译器**: ARM GCC 10 2021.10
- **构建系统**: CMake + Ninja
- **烧录工具**: PyOCD

## 快速开始

### Windows 编译

```bash
# 配置 (首次)
cmake.exe -S . -B build_fast -DCMAKE_BUILD_TYPE=Release -G Ninja

# 编译
cmake.exe --build build_fast --parallel

# 烧录
pyocd flash build_fast/Sanchuan_NB_ValveCtrl.elf --target fm33lg04x
```

### Linux 编译

```bash
# 配置 (首次)
cmake -S . -B build_fast -DCMAKE_BUILD_TYPE=Release -G Ninja

# 编译
cmake --build build_fast --parallel
```

## 目录结构

```
├── CMakeLists.txt      # CMake 构建配置
├── Src/                # 应用源代码
├── Inc/                # 应用头文件
├── Drivers/            # FM33LG0xx 驱动库
├── MF-config/          # 外设配置
├── VscodeGcc/          # VSCode + GCC 配置
├── MDK-ARM/            # Keil MDK 工程 (可选)
└── Document/           # 相关文档
```

## 版本历史

### v1.0.0 (2024-12-01)
- 初始版本
- 从浪花项目移植
- 重命名为 Sanchuan_NB_ValveCtrl

## 作者

Aspiring-Freeman

## 许可证

本项目基于 Mulan PSL v2 许可证开源。
