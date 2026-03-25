/**
 ******************************************************************************
 * @file    app_config.h
 * @brief   应用程序配置文件 -
 *集中管理编译选项，这个仅限用vscode配置使用，keil不支持,keil的烧录地址需要手动去IDE界面修改
 * @author  NB_IOT_Gas_Meter_Board_18
 * @version v3.0.0
 * @date    2025-12-23
 ******************************************************************************
 *
 * 使用说明：
 * =========
 *
 * 【方式一】在此文件中直接修改宏定义（推荐日常使用）
 *   - 取消注释 #define USE_BOOTLOADER 即可启用 Bootloader 模式
 *   - 注释掉该行则使用 Standalone 模式，不需要Bootloader,直接从0x0000地址启动
 *   - 修改后需要：清理 build 目录 → 重新 Configure → 重新编译
 *
 * 【方式二】通过 CMake 命令行参数（推荐自动化构建）
 *   - Standalone 模式: cmake -S . -B build
 *   - Bootloader 模式: cmake -S . -B build -DUSE_BOOTLOADER=ON
 *
 * 注意：CMake 命令行参数优先级高于此文件中的定义
 *
 ******************************************************************************
 */

#ifndef __APP_CONFIG_H
#define __APP_CONFIG_H

#ifdef __cplusplus
extern "C" {
#endif

/* ============================================================================
 * Bootloader 模式配置
 * ============================================================================
 *
 * USE_BOOTLOADER: 启用后 APP 从 0x4000 开始，兼容 16KB Bootloader
 *                 禁用后 APP 从 0x0000 开始，独立运行
 *
 * 内存布局:
 *   Standalone 模式:  FLASH 0x0000-0x3FFFF (256KB 全部给 APP)
 *   Bootloader 模式:  FLASH 0x0000-0x3FFF  (16KB Bootloader)
 *                     FLASH 0x4000-0x3FFFF (240KB APP)
 */

#ifndef USE_BOOTLOADER
#define USE_BOOTLOADER // ✅ 已启用 Bootloader 模式 (APP 从 0x4000 开始)
#endif

/* ============================================================================
 * 派生宏定义 (不要手动修改)
 * ============================================================================
 */

#ifdef USE_BOOTLOADER
#ifndef APP_START_ADDRESS
#define APP_START_ADDRESS 0x00004000UL // APP 起始地址
#endif
#define APP_FLASH_SIZE (240 * 1024) // APP 可用 FLASH 大小 (240KB)
#define BOOTLOADER_SIZE (16 * 1024) // Bootloader 大小 (16KB)
#else
#ifndef APP_START_ADDRESS
#define APP_START_ADDRESS 0x00000000UL // APP 起始地址
#endif
#define APP_FLASH_SIZE (256 * 1024) // APP 可用 FLASH 大小 (256KB)
#define BOOTLOADER_SIZE 0           // 无 Bootloader
#endif

/* RAM 配置 (两种模式相同) */
#define RAM_START_ADDRESS 0x20000000UL
#define RAM_SIZE (32 * 1024) // 32KB

/* ============================================================================
 * 其他应用配置
 * ============================================================================
 */

/* 调试模式配置 */
// #define ENABLE_WATCHDOG          // 取消注释以启用看门狗

/* 日志配置 */
#define DEBUG_PRINT_TIME 2000 // 调试打印时间间隔 (ms)

/* 上电超时配置 */
#define POWER_ON_MAX_TIME_WITHOUT_RESPONSE                                     \
  (5 * 60 * 1000) // 上电无响应最大时间 (5分钟, 单位: ms)

#ifdef __cplusplus
}
#endif

#endif /* __APP_CONFIG_H */
