# 版本历史

## v3.0.1 (2026-02-26) - 🐛 修复 Keil 构建 HardFault 崩溃

> **关键修复**: 修复 fal_def.h 劫持 EasyLogger 日志宏导致 Keil MicroLib semihosting HardFault

### 🐛 Bug 修复
- **修复 Keil 构建崩溃**: fal_def.h 用 `#undef` 劫持 `log_i`/`log_e`/`log_d` 重定义为 `printf`，
  Keil MicroLib 的 `printf` 走 semihosting，无调试器时 BKPT 指令触发 HardFault
- **修复 GCC 独立模式无日志**: 链接脚本 `fm33lg04x_flash.ld` 缺少 PHDRS 段，
  PyOCD 0.36+ 错误加载 `.data` 段导致全局变量 `Debug_Mode` 被覆盖为 0

### 🔧 改进
- **FAL 日志重定向**: `fal_cfg.h` 预定义 `FAL_PRINTF` → `elog_raw_output()`，
  FAL 内部日志通过 UART 正常输出而非 printf
- **日志宏保护**: `components.h` 在 fal.h 之后恢复 EasyLogger 的 `log_i`/`log_e`/`log_d` 宏
- **fputc 兜底**: `EasyLogger.c` (仅 Keil 编译) 重写 `fputc` 防止残留 printf 触发 semihosting
- **GCC 链接脚本**: 添加 PHDRS 和 PT_LOAD 段分配，确保 ELF 正确加载

### 📝 文档
- 新增 issue 文档: `docs/issues/fal_def_hijack_log_macros_keil_hardfault.md`
- 新增 issue 文档: `docs/issues/ELF_Program_Header_Flash_Address_Issue.md`

---

## v3.0.0 (2026-02-21) - 🔧 INA219驱动重构与系统自检

> **重要更新**: 重构INA219电流检测驱动，支持双通道独立控制，新增系统自检框架

### 🔧 代码重构
- **INA219驱动模块化**: 用全新 `ina219.c/h` 替换旧的 `ZDINA219.c/h`
  - 支持双通道: 主电INA219 (PC8/PC9) + NB电INA219 (PC6/PC7)
  - 旁路MOS独立控制 (PE3主电, PE2 NB电)
  - 硬件引脚定义集中在头文件，便于移植

- **GPIO精简优化**: 移除冗余电流检测使能函数
  - 移除 `Current_CHK_CTRL_ON/OFF`, `NB_Current_CHK_CTRL_ON/OFF`
  - 旁路MOS控制移入ina219模块，职责更清晰
  - 清理中文注释乱码，统一UTF-8编码

- **板级配置INA219集成**: `board_config.c` 使用 `INA219_CH_MAIN/INA219_CH_NB` 常量
  - 电流读取函数使用通道宏替代硬编码数字

### 🆕 新增功能
- **系统自检框架**: `Board_SelfTest()` 启动时自检
  - 电源系统检测 (主电/备电/12V供电)
  - INA219在线状态检测
  - 可扩展的自检步骤架构

### 📝 其他变更
- 原理图文件格式更新: SchDot → SchDoc
- 国内水表测试模块更新
- 膜式燃气表板配置调整

### 📚 文档变更
- 新增 `docs/Referrence/` I2C参考资料目录
  - `i2c_standard.pdf` - I2C标准协议规范
  - `i2c_timing_analyze.html` - I2C时序分析文档
  - `slva704.pdf` - TI INA219应用笔记 (SLVA704)

### 文件变更
- 新增: `Src/ina219.c`, `Inc/ina219.h`
- 新增: `docs/Referrence/i2c_standard.pdf`, `i2c_timing_analyze.html`, `slva704.pdf`
- 删除: `Src/ZDINA219.c`, `Inc/ZDINA219.h`
- 修改: `Src/GPIO.c` - 移除冗余函数，调用新ina219接口
- 修改: `Inc/GPIO.h` - 移除旧函数声明
- 修改: `Src/main.c` - 新增Board_SelfTest，版本号更新至v3.0.0
- 修改: `Src/Test/NB_18_DiaphragmGas_Test/NB_18_DiaphragmGas_board_config.c` - INA219通道宏集成
- 重命名: `docs/项目文件/kicad_pcb_document/*.SchDot` → `*.SchDoc`

---

## v2.3.0 (2026-02-07) - 🏗️ 板级配置重构与测试步骤架构优化

> **重要更新**: 重构板级配置为直接实现架构，移除回调间接层；新增测试步骤配置宏，简化流程定义

### 🔧 板级配置重构
- **架构简化**: 移除回调函数指针间接层，所有平台驱动函数直接实现
  - `NB_18_DiaphragmGas_board_config.c/h` 重写为 v2.0.0
  - `NB_18_DiaphragmGas_port.c/h` 升级为 v3.0.0，只做 feature flag 检查 + 转调 board 函数
  - 移除 `DiaphragmGasDriverOps` / `DiaphragmGasPortConfig` 等复杂结构体
  - 新增 `BoardConfig` 纯数据结构体和 `FeatureFlags` 位域
  - 移除 `PORT_ERR_CALLBACK_NULL` 状态码

- **新增阀门电压读取接口**: `read_valve_b/r_voltage_mv()` 通过 port 层统一访问
  - 阀门电压测试改用 port 层接口，替代直接调用 `get_famen_1/2_dianya()`

### 🏗️ 测试步骤架构优化
- **新增 `diaphragm_test_step_config.h`**: 独立的步骤配置头文件
  - 步骤枚举、描述结构体、组件状态等统一管理
  - 新增 `FailAction_t` 失败处理策略 (STOP/CONTINUE/GOTO/RETRY_THEN_CONTINUE)
  - 新增简化宏: `STEP_REQUIRED` / `STEP_OPTIONAL` / `STEP_SYSTEM` / `STEP_LOW_POWER` 等
  - 配置表可读性大幅提升

- **移除自检步骤**: 删除 `DIAPHRAGM_SELF_CHECK` 和 `diaphragm_handle_self_check()`

### 📝 代码清理
- 移除 `domestic_water_meter_test.c` 中的膜式燃气表条件编译代码
- 阀门电压变量类型从 `uint32_t` 改为 `uint16_t`，修正格式化字符串

### 文件变更
- 新增: `Src/Test/NB_18_DiaphragmGas_Test/diaphragm_test_step_config.h` - 步骤配置独立头文件
- 重写: `Src/Test/NB_18_DiaphragmGas_Test/NB_18_DiaphragmGas_board_config.c/h` - 直接实现架构
- 重构: `Src/Test/NB_18_DiaphragmGas_Test/NB_18_DiaphragmGas_port.c/h` - 简化为转调 board 函数
- 修改: `Src/Test/NB_18_DiaphragmGas_Test/diaphragme_meter_test.c/h` - 步骤配置宏化，移除自检
- 修改: `Src/Test/Domestic_water_meter_Test/domestic_water_meter_test.c` - 移除条件编译
- 修改: `Src/main.c` - 版本号更新为 v2.3.0

---

## v2.2.0 (2026-02-06) - ⚡ 功耗测量优化

> **重要更新**: 优化功耗测量精度，修复 IO 状态导致的功耗偏高问题

### 🔧 功耗测量优化
- **IO 状态清理**: 测量功耗前清理所有可能影响功耗的 IO 口
  - 释放阀门到位信号 (PA12/PA11)
  - 关闭霍尔信号输出
  - 关闭防拆检测电磁铁
  - 恢复 119 报警信号为非触发状态

- **补偿电流调整**: 从 690uA 调整为 17uA
  - 根据实际测量的空载电流重新设定

- **4.7V ADC 优化**: 读取完成后立即禁用采样
  - 避免持续功耗影响低功耗检测

### 📝 代码改进
- 添加更详细的中文注释说明
- 优化函数逻辑注释，便于理解代码意图

### 文件变更
- 修改: `Src/main.c` - 版本号更新为 v2.2.0，注释改进
- 修改: `Src/Test/NB_18_DiaphragmGas_Test/diaphragme_meter_test.c` - 功耗测量优化
- 修改: `Src/Test/NB_18_DiaphragmGas_Test/diaphragme_meter_test.h` - 注释改进
- 修改: `Src/Test/NB_18_DiaphragmGas_Test/NB_18_DiaphragmGas_board_debug.c` - 注释改进
- 修改: `Src/Test/NB_18_DiaphragmGas_Test/NB_18_DiaphragmGas_port.h` - 注释改进

---

## v2.1.0 (2026-02-05) - 🛡️ 安全修复与架构优化

> **重要更新**: 修复多个安全隐患，统一CRC实现，重构UART外设模块

### 🔴 安全修复
- **缓冲区溢出保护**: `TONGXIN_xieyifasong()` 和 `irad_TONGXIN_xieyifasong()` 添加数据域长度校验
  - 防止超过200字节缓冲区导致的内存越界
  - 添加 `FRAME_OVERHEAD` 和 `MAX_DATA_LEN` 宏定义

- **死循环风险修复**: 水表协议解析 `0x2011` 命令处理
  - 将 `while(1)` 改为带边界检查的循环
  - 防止无 `\0` 结束符时的无限循环

- **NULL指针检查**: `build_cmd_frame()` 添加缓冲区空指针检查

### 🔧 代码重构
- **CRC函数统一**: 删除重复的CRC16-CCITT实现
  - 移除 `user_cmd_get_crc()` 和 `irad_user_cmd_get_crc()`
  - 统一使用 `utility.h` 中的 `util_crc16_ccitt()`

- **回调函数架构优化**: PC协议工装控制
  - 新增 `PC_Protocol_GetFTControlFunc()` getter函数
  - `pc_protocol_config.c` 改用回调调用，解耦硬件实现

- **UART外设重构**: 移动到 `Src/Peripheral/uart/` 目录
  - uart0.c/h, uart1.c/h, uart3.c/h, uart4.c/h
  - 更清晰的代码组织结构

### 🆕 新增功能
- **BoardDebug调试模块**: 工装板调试控制功能
  - 支持主/辅电源控制、功耗测试、电压采样
  - 控制模式状态管理 (进入/退出控制循环)
  - `BoardDebug_Task_Process()` 周期任务处理

### 文件变更
- 新增: `Src/Test/NB_18_DiaphragmGas_Test/NB_18_DiaphragmGas_board_debug.c/h`
- 移动: `Src/uart*.c` → `Src/Peripheral/uart/`
- 移动: `Inc/uart*.h` → `Inc/Peripheral/`
- 修改: `Components/Protocol/PC/pc_protocol_common.c` - 添加getter
- 修改: `Components/Protocol/PC/pc_protocol_config.c` - 回调解耦
- 修改: `Src/tongxin_xieyi_Ctrl.c` - 安全修复 + CRC统一
- 修改: `Src/tongxin_xieyi_irda_Ctrl.c` - 安全修复 + CRC统一

---

## v2.0.1 (2026-01-31) - 开发版本

> 过渡版本，已合并到v2.1.0

---

## v2.0.0 (2026-01-31) - 🚀 重大版本发布

> **重要里程碑**: 修复关键测试流程 Bug，完善超时机制

### Bug 修复
- **修复测试步骤信息回调**: `Callback_GetFailInfo()` 现在正确返回当前执行步骤
  - 测试进行中时返回 `diaphragm_test_step` (当前步骤)
  - 测试结束后返回 `fail_step` (失败时记录的步骤)

- **修复步骤超时误判问题**: 优化 `diaphragm_test_loop()` 超时检查逻辑
  - 增加 `step_entered` 标志位检查，确保 handler 至少执行一次后才检查超时
  - 防止 "开始测试" 等步骤还未执行就触发超时的问题
  - 在 `diaphragm_handle_start()` 入口设置 `step_entered = 1`

### 文件变更
- 修改: `Src/main.c` - 版本号更新为 v2.0.0, Callback_GetFailInfo 逻辑优化
- 修改: `Src/Test/NB_18_DiaphragmGas_Test/diaphragme_meter_test.c` - 超时检查逻辑完善

---

## v1.9.0 (2026-01-31) - 🎉 稳定测试版本

> **重要里程碑**: 本版本经过实际测试验证，可正常完成膜式燃气表全流程测试！

### 新增功能
- **LED指示器组件 (LedIndicator)**: 全新的LED状态指示系统
  - 支持单LED/双LED多种硬件配置
  - 预定义多种闪烁模式: 常亮/常灭/慢闪/快闪/双闪/心跳等
  - 状态驱动设计: 空闲/测试中/通过/失败等状态自动映射到LED效果
  - 可扩展的方案注册机制，支持不同工装定制

- **阀门到位测试功能**: 完整实现开到位信号检测
  - PA11(signal_2) = 开到位信号触发（已验证工作正常）
  - PA12(signal_1) = 关到位信号（硬件未实现，已注释）
  - 完整的状态机: 初始状态 → 触发反转 → 恢复验证

### 改进优化
- **测试总超时时间**: 从60秒增加到90秒，避免正常流程超时
- **变量命名规范化**:
  - `kai_daowei` → `open_pos` (开到位)
  - `guan_daowei` → `close_pos` (关到位)
  - 协议层和测试层统一使用英文命名

- **日志输出优化**: 阀门到位测试日志改为中文，更易理解
- **数组边界修复**: IMEI/IMSI/ICCID 移除多余的null终止符，符合协议固定长度

- **静态功耗日志增强**: 显示补偿前后的功耗值，便于调试

### 修复问题
- 修复阀门到位测试使用错误引脚的问题 (PA12→PA11)
- 修复测试总超时配置分散的问题，统一在time_manager.h管理
- 清理无用的宏定义 `DGM_TOTOAL_TIMEOUT_S`

### 文件变更
- 新增: `Components/LedIndicator/led_indicator.c/h` - LED指示器组件
- 修改: `Src/main.c` - 版本号v1.9.0、LED组件初始化
- 修改: `Src/LED_CTRL.c/h` - 集成LED指示器组件
- 修改: `Src/Test/NB_18_DiaphragmGas_Test/diaphragme_meter_test.c/h` - 阀门到位测试、LED状态指示
- 修改: `Components/TimeManager/time_manager.h` - 总超时90秒
- 修改: `Components/Protocol/Device/.../device_protocol_diaphragm_gas_meter.c` - 变量重命名
- 修改: `Components/Protocol/Device/.../device_protocol_diaphragm_gas_meter_events.h` - 结构体字段重命名
- 修改: `CMakeLists.txt` - 添加LedIndicator组件编译配置

---

## v1.8.0 (2026-01-30)

### 新增功能
- **统一时间管理模块 (TimeManager)**: 集中管理所有时间配置
  - TM_Init() / TM_SysTick_Handler() 核心时基
  - TM_SetStepTimeout() / TM_IsStepTimeout() 单步超时
  - TM_SetDelay() / TM_IsDelayComplete() 非阻塞延时
  - TM_StartGlobalTimeout() 全局测试超时(60秒)
  - TM_IsPeriodElapsed() 周期任务管理

- **统一重试管理模块 (RetryManager)**: 标准化重试逻辑
  - RM_TryRetry() 统一重试入口
  - 支持超时/失败/通信错误等多种重试原因
  - 自动重置状态机状态

- **阀门电压测试**: 新增阀门B-R端电压检测
  - 开阀/关阀/停止状态电压验证
  - 极性反转检测

- **阀门到位测试**: 新增阀门到位信号检测步骤
  - 可独立跳过配置
  - PA11/PA12引脚信号控制

- **UART连接测试步骤**: 新增UART通信状态检测

- **RTC电池电压检测**: ADC IN5 (PA0) 读取RTC电池电压

- **测试结束供电策略**:
  - 成功后保持5分钟供电（等待工人按按钮）
  - 失败后立即断电（可配置）
  - 检测5V电压判断膜表是否拿起

### 改进优化
- **UART0中断控制**: 测试期间禁用上位机通信干扰
  - UART0_DisableRxInterrupt() / UART0_EnableRxInterrupt()

- **到位信号引脚优化**: PA12(开到位)/PA11(关到位)
  - 默认输入模式(高阻态)，触发时推挽输出低电平

- **IOStatus状态位标准化**: 新增宏定义
  - IOSTATUS1_MODULE_BIT ~ IOSTATUS1_IC_CARD_BIT
  - IOSTATUS2_RTC_BIT ~ IOSTATUS2_BLUETOOTH_BIT
  - IOSTATUS_SET() / IOSTATUS_CLEAR() / IOSTATUS_TEST()

- **测试结果结构体优化**:
  - MasterLowPowerCurrent 从 uint16_t 改为 uint8_t
  - IMEI/IMSI/ICCID 移除结束符，固定15/15/20字节
  - Reserve1 默认值改为0xFF

- **协议响应处理优化**:
  - 等待重试期间收到响应则取消延时继续处理
  - 超时前检查是否已收到响应，避免误重试

- **电流补偿值调整**: 静态功耗补偿从8uA改为460uA

- **CMakeLists.txt**: 添加TimeManager组件编译配置

### 修复问题
- 修复测试未结束时查询结果返回0xFF的问题
- 修复diaphragm_runtime.test_completed标志未正确设置的问题
- 修复到位信号引脚配置错误（PA3→PA12）
- 修复测试结束后未恢复UART0中断的问题

### 文件变更
- 新增: `Components/TimeManager/time_manager.c/h` - 统一时间管理
- 新增: `Components/TimeManager/retry_manager.c/h` - 统一重试管理
- 修改: `Src/main.c` - 版本号v1.8.0、TM_Init()初始化
- 修改: `Src/time.c` - TM_SysTick_Handler()集成
- 修改: `Src/uart0.c/h` - UART0中断控制函数
- 修改: `Src/GPIO.c` - PA11/PA12到位信号引脚配置
- 修改: `Src/ADC_CHK.c/h` - ADC IN5 RTC电压读取
- 修改: `Src/Test/NB_18_DiaphragmGas_Test/diaphragme_meter_test.c/h` - 大量测试流程优化
- 修改: `Src/Test/NB_18_DiaphragmGas_Test/NB_18_DiaphragmGas_port.c/h` - RTC电压读取接口
- 修改: `Src/Test/NB_18_DiaphragmGas_Test/NB_18_DiaphragmGas_board_config.c` - ADC通道配置
- 修改: `Components/Protocol/Device/device_protocol.c/h` - 1002协议扩展
- 修改: `Components/Protocol/PC/Domestic/DiaphragmGasMeters/pc_protocol_diaphragm_gas_meter.c` - 测试结果结构体优化
- 修改: `CMakeLists.txt` - TimeManager编译配置

---

## v1.7.0 (2025-01-28)

### 新增功能
- **霍尔+119联合检测**: 在计量模块测试中同时检测霍尔传感器和119报警
  - 使用1002协议(IO状态检测)获取hall1, hall2, io_119状态
  - 测试流程: 初始状态→触发(磁铁+PC14低电平)→验证变化→释放→验证恢复
  - 霍尔和119一起测试，减少整体测试时间

- **倾斜开关检测**: 使用1001协议的tilt_ok字段检测
  - tilt_ok: 0=正常(未倾斜), 1=异常(已倾斜), 2=无此功能
  - 状态为0则通过，为1则失败，为2则跳过

- **119报警触发信号输出**: 新增PC14引脚控制
  - 低电平触发119报警状态(io_119从1变为0)
  - 高电平恢复正常状态
  - GPIO初始化默认输出高电平(未触发)

- **PC14 GPIO初始化**: 在GPIO.c中添加PC14推挽输出配置

- **4.7V电压检测**: 新增ADC IN9 (PD4)电压读取
  - 用于检测5V经二极管后的电压(约4.7V)
  - 分压系数×11

- **软件延时控制**: diaphragm_set_delay()函数
  - 用于测试流程中的非阻塞延时等待

### 改进优化
- **功耗检测支持重试**: 功耗超出范围时自动重试，最多10次
- **步骤超时时间调整**:
  - GPRS电压测试: 20s→10s
  - 模块响应等待: 50s→5s(重试10次)
  - 功耗检测: 5s→100ms(重试10次)

- **IO状态事件处理优化**: 合并DGM_EVENT_IO_STATUS_HIGH/LOW为DGM_EVENT_IO_STATUS
- **计量模块测试状态**: 根据meter_type(0=霍尔, 1=光电)动态设置

- **工装协议帧头帧尾**: 新增FT_FRAME_HEAD(0x55)和FT_FRAME_TAIL(0xAA)
  - 避免与其他协议(如0x68/0x16)冲突

- **版本查询协议**: 新增0xC0查询配置命令和0xC1响应
  - 返回程序版本和编译时间

- **夹具控制协议框架**: 新增0xC2控制命令(待实现)

### 修复问题
- 修复PC14未初始化导致119检测无法工作的问题
- 修复关闭红外函数名称diaphragm_handle_close_ir→diaphragm_close_ir
- 修复测试结束时未清理霍尔和开盖信号输出的问题
- 修复diaphragm_test_result_clear()误清除fail_reason和fail_step的问题

### 文件变更
- 修改: `main.c` - 版本号更新、版本查询回调
- 修改: `GPIO.c` - PC14初始化
- 修改: `ADC_CHK.c` - ADC IN9电压读取
- 修改: `diaphragme_meter_test.c/h` - 霍尔+119联合检测、倾斜开关检测
- 修改: `NB_18_DiaphragmGas_port.c/h` - 119信号输出接口、4.7V读取
- 修改: `NB_18_DiaphragmGas_board_config.c` - alarm_119引脚配置
- 修改: `protocol_def.h` - FT_FRAME_HEAD/TAIL定义
- 修改: `pc_protocol.h/c` - 版本查询协议
- 修改: `pc_protocol_config.c` - 0xC0/0xC1/0xC2命令处理
- 修改: `device_protocol_diaphragm_gas_meter_events.h` - 事件类型优化

---

## v1.6.0 (2025-01-27)

### 新增功能
- **防拆检测 (开盖检测) 实现**: `diaphragm_handle_cover_check()`
  - 使用 sub_step 状态机管理 2 阶段检测流程
  - 阶段 0: 触发开盖信号并发送 0x1001 请求
  - 阶段 1: 等待响应并比对前后 cover_open 状态
  - 状态变化则检测通过，未变化则检测失败

- **测试步骤跳过机制**: `diaphragm_step_skip()`
  - 新增步骤跳过函数，用于未实现或不需要测试的步骤
  - 步骤描述器新增 `skip_status_ptr` 和 `skip_when_equals` 字段
  - 支持根据附件信息动态跳过可选测试步骤

- **组件测试状态配置**: `DiaphragmComponentStatus_t`
  - 根据 0x1001 协议返回的附件信息动态配置测试项
  - 支持 119、IC卡、双阀、蓝牙、温压、红外、开盖、倾斜开关等组件
  - 状态值: 1=需要测试, 2=跳过测试

- **新增可选测试步骤枚举**:
  - `DIAPHRAGM_BLUETOOTH_TEST` - 蓝牙测试
  - `DIAPHRAGM_COVER_CHECK` - 防拆检测
  - `DIAPHRAGM_IC_MODULE_TEST` - IC模块测试
  - `DIAPHRAGM_METERING_MODULE_TEST` - 计量模块测试
  - `DIAPHRAGM_119_WARNING_TEST` - 119报警测试
  - `DIAPHRAGM_TEMP_PRESSURE_SENSOR_TEST` - 温压传感器测试
  - `DIAPHRAGM_SECOND_VALVE_TEST` - 第二阀门测试
  - `DIAPHRAGM_TILT_SWITCH_TEST` - 倾斜开关测试
  - `DIAPHRAGM_DEFAULT_VALVE_TEST` - 默认阀门测试

### 改进优化
- **附件信息日志优化**:
  - 打印原始 bit 值和解析后的测试状态 (测/跳)
  - 区分来自 has_addon 字段和专用字段 (bluetooth_ok, tilt_ok) 的状态

- **测试结果结构体重构**: `DiaphragmTestResult_t`
  - 与上位机协议对齐，使用 IOStatus1/IOStatus2 位字段
  - 字段重命名: eeprom_ok → storage_ic_ok

- **函数重命名规范化**:
  - `common_get_gongwei_number()` → `common_get_station_id()`

- **电解电容控制**: 新增板载电解电容禁用功能
  - `diaphragm_board_capacitor_disable()` 接口
  - 启动测试前关闭电解电容，避免影响电压检测

- **UART1 缓冲区扩容**: 256 → 1024 字节
  - 膜表数据量较大，防止日志丢包

### 修复问题
- 修复 `DGM_EVENT_BOARD_INFO` 标识符错误为 `DGM_EVENT_POWER_ON_INFO_RECEIVED`
- 修复 DIAPHRAGM_END 步骤在 IDLE 模式下不执行 handler 的问题
- 修复 main.c 中 `Callback_GetFailInfo` 逻辑错误

### 文件变更
- 修改: `diaphragme_meter_test.c/h` - 防拆检测实现、跳过机制、步骤扩展
- 修改: `device_protocol_diaphragm_gas_meter_events.h` - storage_ic_ok 重命名
- 修改: `device_protocol_diaphragm_gas_meter.c` - 字段重命名、0x1000自检响应处理
- 修改: `pc_protocol_diaphragm_gas_meter.c` - 结果结构体重构
- 修改: `NB_18_DiaphragmGas_port.c/h` - 电解电容控制接口
- 修改: `NB_18_DiaphragmGas_board_config.c` - 电解电容引脚配置
- 修改: `common_test_configure.c/h` - 函数重命名
- 修改: `GPIO.c` - PA2 电解电容控制引脚初始化
- 修改: `uart1.c` - 缓冲区扩容
- 修改: `main.c` - Callback_GetFailInfo 逻辑修复

---

## v1.5.0 (2026-01-25)

### 新增功能
- **扩展 0xBF 协议响应格式**: 查询测试结果命令返回完整失败信息
  - 新增测试状态字段 (0=进行中, 1=成功, 2=失败)
  - 新增失败原因代码 (DgmFailReason_t 枚举值)
  - 新增失败原因名称字符串
  - 响应格式: `68 BF [长度] [工位号] [测试状态] [失败原因] [步骤ID] [名称长度] [步骤名称...] [原因长度] [原因名称...] [校验和] 16`

- **运行功耗监控**: 测试过程中实时监控功耗
  - 在 NORMAL 模式步骤中每500ms检测一次功耗
  - 超过阈值 (200μA) 连续3次触发失败
  - 新增 `diaphragm_check_running_power()` 函数

- **测试失败原因追踪**: DiaphragmTestRuntime_t 增强
  - 新增 `fail_reason` 字段记录失败原因
  - 新增 `fail_step` 字段记录失败步骤
  - `diaphragm_step_fail()` 自动记录并跳转到 END

### 架构重构
- **测试模式分类**: 新增 `DgmTestMode_t` 枚举
  - `DGM_MODE_IDLE`: 空闲模式 (WAIT/END 步骤)
  - `DGM_MODE_NORMAL`: 正常测试模式 (监控运行功耗)
  - `DGM_MODE_LOW_POWER`: 低功耗等待模式 (不监控功耗)

- **事件掩码系统**: 新增 `DgmEventMask_t` 位掩码
  - 替代多个独立的事件标志变量
  - `diaphragm_runtime.events_received` 统一管理
  - `diaphragm_goto_step()` 自动清除事件掩码

- **步骤状态语义优化**:
  - `first_entry` 拆分为 `step_entered` 和 `step_executing`
  - 避免 "第一次进入" 的语义混淆

- **PCGetFailInfoFunc 回调扩展**:
  - 参数从 `(step_id, step_name, description)` 改为 `(step_id, step_name, fail_reason, reason_name)`
  - 返回值语义: 0=进行中, 1=成功, 2=失败

### 协议解析修复
- **0x1001 (上告开机信息)**: 正确解析为 26 字节 `DgmBoardInfoData`
- **0x1002 (IO状态)**: 正确解析为 7 字节 `DgmIoStatusData`
- **0x1008 (星型模块MAC)**: 正确解析为 17 字节 `DgmStarMacData`
- **0xC525 (网络参数)**: 正确解析为 107 字节 `DgmImeiData`

### 文件变更
- 修改: `diaphragme_meter_test.h` - 新增枚举和结构体字段
- 修改: `diaphragme_meter_test.c` - 运行功耗监控、失败追踪
- 修改: `pc_protocol_config.c` - 扩展 0xBF 响应格式
- 修改: `pc_protocol.h` - PCGetFailInfoFunc 回调签名
- 修改: `pc_protocol_common.c` - GetFailInfo 实现
- 修改: `components.c/h` - 回调接口更新
- 修改: `main.c` - Callback_GetFailInfo 使用膜表变量
- 修改: `device_protocol_diaphragm_gas_meter.c` - 协议解析修复

---

## v1.4.0 (2026-01-24)

### 新增功能
- **膜式燃气表协议事件系统**: 新增 `device_protocol_diaphragm_gas_meter_events.h`
  - 定义协议层与测试层之间的事件接口
  - 支持自检完成、上告开机信息、IO状态、IMEI读取等事件类型
  - 事件回调机制实现协议层与测试层解耦

- **步骤描述器扩展**: `DiaphragmStepDescriptor_t` 增加 `on_enter`、`on_retry`、`next_step` 字段
  - 支持进入步骤时自动执行协议发送
  - 支持重试时自动重发协议
  - 支持配置跳转到指定步骤或自动下一步

- **协议层前导码支持**: 新增 `ProtocolPreambleConfig` 结构体
  - 水表协议配置前导唤醒 (32次 x 50字节 0xAA + 10字节 0xFE 同步)
  - 膜式燃气表协议不需要前导
  - 协议管理器自动处理前导发送

- **透传模式前导控制**: 配置协议 (0xAE) 扩展支持透传前导模式
  - 帧格式从8字节扩展到9字节
  - 新增 `preamble_mode` 字段: 0=无前导(膜表), 1=有前导(水表)

### 改进
- **Components_Init 重构**: 使用 `ComponentsConfig` 结构体传递所有回调函数
  - 简化初始化调用，避免多次单独设置回调
  - 确保回调设置在协议注册之后执行

- **协议码修正**:
  - `DEV_BoardInfo` (0x1001) 改用读命令 (0x01) 代替写命令 (0x04)
  - 添加 `DGM_SendBoardInfoRequest()` API，保留 `DGM_SendTestConnect()` 兼容

- **测试步骤优化**:
  - 拆分 `DIAPHRAGM_CONNECT_TEST` 为 `DIAPHRAGM_MODULE_GPRS_VOLTAGE_TEST` 和 `DIAPHRAGM_MODULE_SUCCESS_RESPONSE_WAIT`
  - 新增 `diaphragm_step_success()` 自动跳转函数

### 文件变更
- 新增: `Components/Protocol/Device/Diomestic/DiaphragmGasMeters/device_protocol_diaphragm_gas_meter_events.h`
- 修改: `components.c/h` - ComponentsConfig 结构体初始化
- 修改: `protocol_def.h` - 添加 ProtocolPreambleConfig
- 修改: `protocol_manager.c` - 前导码包装函数
- 修改: `pc_protocol_config.c` - 9字节帧格式支持
- 修改: `device_protocol_diaphragm_gas_meter.c` - 事件回调系统
- 修改: `diaphragme_meter_test.c/h` - 事件处理和步骤描述器扩展

---

## v1.3.0 (2026-01-23)

### 新增功能
- **膜式燃气表测试模块**: 新增 `Src/Test/NB_18_DiaphragmGas_Test/` 目录
  - `NB_18_DiaphragmGas_port.c/h` - 膜表硬件抽象层接口
  - `NB_18_DiaphragmGas_board_config.c/h` - 膜表板级配置
  - `diaphragme_meter_test.c/h` - 膜表测试业务逻辑
  - 采用回调表驱动的测试框架设计

- **国内水表测试模块**: 新增 `Src/Test/Domestic_water_meter_Test/` 目录
  - `domestic_water_meter_test.c/h` - 国内水表测试完整实现
  - 支持机械表和超声波表的阀门配置
  - 阀门配置结构体 `MechanicalValveConfig` 和 `UltrasonicValveConfig`

### 改进
- **串口发送优化**: 改进 `uart0.c` 发送等待逻辑
  - 等待上一帧发送完毕后再发送新数据
  - 增加超时时间适应较长数据发送

### 硬件问题记录
- 记录膜表工装硬件问题 (3.3V 和 GPRS 3.6V 需分压)

---

## v1.2.0 (2026-01-21)

### 新增功能
- **通用测试框架**: 新增 `Src/Test/` 目录
  - `common_test_configure.c/h` - 通用测试配置和工具函数
  - `diaphragme_meter_test.c/h` - 膜式气表测试模块
  - `domestic_water_meter_test.c/h` - 民用水表测试模块
  - 支持多产品测试模块的宏定义启用/禁用

- **通用超时处理**: 新增 `common_test_check_timeout()` 函数
  - 支持全局超时和单步超时检测
  - 自动设置测试状态和描述
  - 可被所有测试模块复用

- **工位号检测**: 新增 `common_get_gongwei_number()` 函数
  - 通过4个跳线帽引脚读取工位编号 (1-15)

### 修复
- **协议认领bug**: 修复 `upgrade_parse()` 错误认领非升级命令的问题
  - 现在只在处理 0xBA 命令时返回 OK，否则返回 UNKNOWN_CMD
  - 修复 0xAE 配置命令被 upgrade 协议错误拦截的问题

### 改进
- 统一测试函数命名风格
  - `diaphragm_test_start()` / `diaphragm_test_loop()`
  - `domestic_water_meter_test_start()` / `domestic_water_meter_test_loop()`
- 测试模块通过宏定义控制启用
  - `ENABLE_DIAPHRAGM_METER_TEST`
  - `ENABLE_DOMESTIC_WATER_METER_TEST`

---

## v1.1.0 (2026-01-20)

### 新增功能
- **协议重构**: PC协议和设备协议按表计类型分类到子目录
  - `PC/Domestic/WaterMeters/` - 国内水表PC协议
  - `PC/Domestic/DiaphragmGasMeters/` - 国内膜式气表PC协议
  - `Device/Diomestic/WaterMeters/` - 国内水表设备协议
  - `Device/Diomestic/DiaphragmGasMeters/` - 国内膜式气表设备协议

- **公共配置协议**: 新增 `pc_protocol_config.c`
  - 调试模式开关 (0xAE)
  - 透传模式开关 (0xAE)
  - 查询当前测试步骤 (0xBE)
  - 与具体表计类型解耦，所有表计通用

- **膜式气表协议**: 从 PIC 项目迁移并重构
  - 支持双68帧格式
  - 完整的控制码和数据标识定义
  - 开阀、关阀、读表、写表等功能

### 改进
- 组件宏重命名，更清晰的命名规范:
  - `COMPONENT_PC_PROTOCOL_WATER_METER`
  - `COMPONENT_PC_PROTOCOL_DIAPHRAGM_GAS_METER`
  - `COMPONENT_DEVICE_PROTOCOL_WATER_METER`
  - `COMPONENT_DEVICE_PROTOCOL_DIAPHRAGM_GAS_METER`
  - `COMPONENT_PC_PROTOCOL_CONFIG`

### 文件变更
- 删除: `pc_protocol_mes.c`, `pc_protocol_mes_Gas_Meter.c`
- 新增: `pc_protocol_water_meter.c`, `pc_protocol_diaphragm_gas_meter.c`, `pc_protocol_config.c`
- 新增: `device_protocol_water_meter.c`, `device_protocol_diaphragm_gas_meter.c`

---

## v1.0.0 (2026-01-19)

### 初始版本
- NB-IoT 燃气表测试工装固件
- 协议管理框架
- EasyLogger 日志系统
- FlashDB 存储
- 阀门控制模块
- APP 升级功能
