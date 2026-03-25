/**
 * @file diaphragme_meter_test.h
 * @brief 膜式燃气表测试模块头文件
 * @details 定义膜式燃气表特有的测试流程、参数和函数
 *          本模块只包含测试业务逻辑，硬件操作通过 NB_18_DiaphragmGas_port 完成
 *
 * @note 采用回调表驱动的测试框架，支持运行时配置测试流程
 *       协议层通过事件回调通知测试层，实现解耦
 */

#ifndef __DIAPHRAGME_METER_TEST_H__
#define __DIAPHRAGME_METER_TEST_H__

#include "main.h"
#include <stdbool.h>
#include <stdint.h>

// 协议事件定义
#include "device_protocol_diaphragm_gas_meter_events.h"

// 新架构：步骤配置统一定义（包含 DiaphragmTestStep_t, DiaphragmStepDescriptor_t
// 等）
#include "diaphragm_test_step_config.h"

// 注意: 测试总超时时间统一在 time_manager.h 中定义 TM_TIMEOUT_GLOBAL_TEST
// (90秒)

/*============================================================================
 *                          协议事件与失败原因枚举
 *===========================================================================*/

/**
 * @brief 协议事件类型位掩码 - 用于统一管理期望和已接收的事件
 */
typedef enum {
  DGM_EVT_NONE = 0x0000,
  DGM_EVT_SELF_CHECK = 0x0001, /**< 0x1000 自检完成 */
  DGM_EVT_BOARD_INFO = 0x0002, /**< 0x1001 板级信息 */
  DGM_EVT_IMEI = 0x0004,       /**< IMEI/IMSI/ICCID */
  DGM_EVT_IO_STATUS = 0x0008,  /**< IO状态 */
  DGM_EVT_IR_CLOSED = 0x0010,  /**< 红外关闭 */
  DGM_EVT_CONNECT = 0x0020,    /**< 连接完成 */
} DgmEventMask_t;

/**
 * @brief 测试失败原因枚举
 */
typedef enum {
  DGM_FAIL_NONE = 0,          /**< 无错误/测试通过 */
  DGM_FAIL_HARDWARE_INIT,     /**< 硬件初始化失败 */
  DGM_FAIL_MAIN_VOLTAGE,      /**< 主电电压异常 */
  DGM_FAIL_5V_VOLTAGE,        /**< 5V电压异常 */
  DGM_FAIL_3V3_VOLTAGE,       /**< 3.3V电压异常 */
  DGM_FAIL_GPRS_VOLTAGE,      /**< GPRS 3.6V电压异常 */
  DGM_FAIL_MODULE_RESPONSE,   /**< 模块响应超时 */
  DGM_FAIL_BOARD_STATUS,      /**< 板级状态异常 */
  DGM_FAIL_POWER_CONSUMPTION, /**< 静态功耗异常 */
  DGM_FAIL_VALVE,             /**< 阀门测试失败 */
  DGM_FAIL_TIMEOUT,           /**< 步骤超时 */
  DGM_TOTOAL_TIMEOUT,         /**< 测试总超时 */
  DGM_FAIL_RUNNING_POWER,     /**< 运行功耗异常 */
  DGM_FAIL_TEMP_SENSOR,       /**< 温度传感器异常 */
  DGM_FAIL_NB_CONNECT,        /**< NB连接失败 */
  DGM_FAIL_IMEI_READ,         /**< IMEI读取失败 */
  DGM_FAIL_SELF_CHECK,        /**< 自检失败 */
  DGM_FAIL_IR_CLOSE,          /**< 红外关闭失败 */
} DgmFailReason_t;

/*============================================================================
 *                          运行功耗监控参数
 *===========================================================================*/

#define RUNNING_POWER_CHECK_INTERVAL_MS 500 /**< 功耗检测间隔 (ms) */
#define RUNNING_POWER_MAX_CURRENT_UA 50000  /**< 运行功耗上限 50mA */
#define RUNNING_POWER_MIN_CURRENT_UA                                           \
  0 /**< 运行功耗下限 0mA ,0mA至少不会损坏线路板*/
#define RUNNING_POWER_FAIL_THRESHOLD 3 /**< 连续异常次数阈值 */

/*============================================================================
 *                          时间管理结构体 (放在 Runtime 结构体外部)
 *===========================================================================*/

/**
 * @brief 膜式燃气表时间管理结构体
 */
typedef struct {
  /*=== 软件延时控制 ===*/
  uint32_t software_delay;   /**< 软件延时时间(ms)，0表示无延时 */
  uint32_t delay_start_tick; /**< 延时开始时间戳 */

  /*=== 总的超时时间配置 ===*/
  uint32_t total_timeout_ms; /**< 总超时时间(ms)，0表示无超时 */
  uint32_t total_timeout_start_tick; /**< 总超时开始时间戳 */
  uint32_t success_timeout_tick;     /**< 成功后的延时关机时间戳 */

  /*=== 单步超时时间配置 ===*/
  uint32_t single_step_timeout_start_tick; /**< 单步超时开始时间戳 */
  uint32_t single_required_timeout_ms;     /**< 单步所需超时时间(ms) */
} DgmTimeManager_t;

/*============================================================================
 *                          膜式燃气表运行时状态结构体
 *===========================================================================*/

/**
 * @brief 膜式燃气表测试运行时状态
 * @details 封装所有运行时状态变量，避免散落的全局变量
 *          重新设计：使用位掩码管理事件，统一步骤执行状态
 */
typedef struct {
  /*=== 硬件初始化状态 (仅上电时使用) ===*/
  uint8_t hardware_init_pending : 1; /**< 硬件初始化待执行 */
  uint8_t hardware_init_failed : 1; /**< 硬件初始化已失败（不再重试）*/
  uint8_t init_retry_count;         /**< 初始化重试计数 */

  /*=== 步骤执行状态 (每次 goto_step 自动重置) ===*/
  uint8_t step_entered : 1; /**< 当前步骤已进入（已打印 header）*/
  uint8_t step_executing : 1; /**< 当前步骤正在执行中（用于一次性步骤防重入）*/
  uint8_t sub_step;    /**< 子状态机 */
  uint8_t retry_count; /**< 当前步骤重试计数 */

  /*=== 协议事件管理 (使用位掩码) ===*/
  volatile uint16_t events_received; /**< 已接收的事件位掩码 */

  /*=== 协议数据缓存 ===*/
  DgmBoardInfoData board_info; /**< 0x1001 响应数据缓存 */
  DgmIoStatusData io_status;   /**< 0x1002 IO状态响应数据缓存 */

  /*=== 运行功耗监控 ===*/
  uint32_t last_power_check_tick; /**< 上次功耗检测时间戳 */
  uint16_t running_power_ua;      /**< 最近一次运行功耗读数 (uA) */
  uint8_t power_check_fail_count; /**< 连续功耗异常计数（防误判）*/

  /*=== 时间管理 ===*/
  DgmTimeManager_t time_manager;

  /*=== 测试结果状态 ===*/
  DgmFailReason_t fail_reason;   /**< 失败原因（0=通过）*/
  DiaphragmTestStep_t fail_step; /**< 失败发生的步骤 */
  uint8_t test_completed : 1; /**< 测试已完成标志（用于PC查询结果判断）*/
  uint8_t waiting_power_off : 1; /**< 等待关电标志（成功/失败后延时关电）*/

} DiaphragmTestRuntime_t;

/*============================================================================
 *                          IOStatus 位定义宏
 *===========================================================================*/

/**
 * @brief IOStatus1 位定义 (低4位=EF/SIM卡/连接/模块, 高4位=IC卡/119/阀门/计量)
 */
#define IOSTATUS1_MODULE_BIT (1 << 0)   /**< bit0: 模块状态 (1=正常) */
#define IOSTATUS1_CONNECT_BIT (1 << 1)  /**< bit1: 连接状态 (1=正常) */
#define IOSTATUS1_SIM_BIT (1 << 2)      /**< bit2: SIM卡状态 (1=正常) */
#define IOSTATUS1_EEPROM_BIT (1 << 3)   /**< bit3: EEPROM状态 (1=正常) */
#define IOSTATUS1_METERING_BIT (1 << 4) /**< bit4: 计量状态 (1=正常) */
#define IOSTATUS1_VALVE_BIT (1 << 5)    /**< bit5: 阀门状态 (1=正常) */
#define IOSTATUS1_119_BIT (1 << 6)      /**< bit6: 119报警状态 (1=正常) */
#define IOSTATUS1_IC_CARD_BIT (1 << 7)  /**< bit7: IC卡状态 (1=正常) */

/**
 * @brief IOStatus2 位定义 (低4位=开盖/温压/红外/RTC, 高4位=保留/蓝牙/倾斜)
 */
#define IOSTATUS2_RTC_BIT (1 << 0)        /**< bit0: RTC状态 (1=正常) */
#define IOSTATUS2_IR_BIT (1 << 1)         /**< bit1: 红外状态 (1=正常) */
#define IOSTATUS2_TEMP_PRESS_BIT (1 << 2) /**< bit2: 温压状态 (1=正常) */
#define IOSTATUS2_COVER_BIT                                                    \
  (1 << 3) /**< bit3: 开盖状态 (0=低电平, 1=正常高电平) */
#define IOSTATUS2_TILT_BIT (1 << 4) /**< bit4: 倾斜状态 (0=正常, 1=异常) */
#define IOSTATUS2_BLUETOOTH_BIT (1 << 5) /**< bit5: 蓝牙状态 (1=正常) */
#define IOSTATUS2_RESERVED1_BIT (1 << 6) /**< bit6: 保留 */
#define IOSTATUS2_RESERVED2_BIT (1 << 7) /**< bit7: 保留 */

/**
 * @brief IOStatus 操作宏
 */
#define IOSTATUS_SET(reg, bit) ((reg) |= (bit))    /**< 设置状态位 */
#define IOSTATUS_CLEAR(reg, bit) ((reg) &= ~(bit)) /**< 清除状态位 */
#define IOSTATUS_TEST(reg, bit) ((reg) & (bit))    /**< 测试状态位 */

/*============================================================================
 *                          膜式燃气表测试结果结构体 (返回上位机)
 *===========================================================================*/

/**
 * @brief 返回上位机的信息结构体定义
 * @details 与上位机协议对应，包含所有需要上报的测试数据
 */
#pragma pack(1)
typedef struct {
  uint8_t DeviceID;              /**< 工位ID */
  uint8_t MeterTYP;              /**< 表具类型 */
  uint8_t IsOrNoWithIterm;       /**< 是否带附件 */
  uint8_t MasterVoult;           /**< 主电电压 (0.1V单位) */
  uint8_t MasterLowPowerCurrent; /**< 主电静态功耗 (uA) */
  uint8_t Module_Csq;            /**< 模块信号强度 CSQ */
  uint8_t RTC_Volt; /**< RTC电压 (0.1V单位), 实际为备电电压 */
  uint16_t FirmwareVersion; /**< 软件版本号 */
  uint8_t Reserve1;         /**< 保留1, 设置为0xF */
  uint8_t IOStatus1;        /**< IO状态1: 高4位=IC卡/119/阀门/计量,
                               低4位=EF/SIM卡/连接/模块 */
  uint8_t
      IOStatus2; /**< IO状态2: 高4位=保留/蓝牙/倾斜, 低4位=开盖/温压/红外/RTC */
  uint8_t ModuleIMEI[15]; /**< 模块IMEI, ASCII格式, 固定15字节(协议) */
  uint8_t ModuleIMSI[15]; /**< 模块IMSI, ASCII格式, 固定15字节(协议) */
  uint8_t ModuleICCID[20]; /**< 模块ICCID, ASCII格式, 固定20字节(协议) */
  uint8_t ModulePowerStatus;    /**< 模块备电状态, 0=正常, 1=异常 */
  uint8_t FirmwareBuildTime[6]; /**< 软件版本编译时间 */
  uint8_t StarMac[12];          /**< 星闪MAC地址 */
  uint8_t ESAMID[8];            /**< ESAM ID */
  uint8_t PressureOnBoard[4];   /**< 板载压力值 */
} DiaphragmTestResult_t;
#pragma pack()

/*============================================================================
 *                          膜式燃气表全局变量声明
 *===========================================================================*/

extern DiaphragmTestStep_t diaphragm_test_step;
extern DiaphragmTestResult_t diaphragm_test_result;
extern DiaphragmTestRuntime_t diaphragm_runtime;

/** @brief 测试步骤描述表 - 回调表驱动的核心 */
extern const DiaphragmStepDescriptor_t diaphragm_step_table[];

/*============================================================================
 *                          膜式燃气表测试函数声明
 *===========================================================================*/

/**
 * @brief 膜式燃气表测试初始化
 */
void diaphragm_test_init(void);

/**
 * @brief 膜式燃气表测试主循环
 */
void diaphragm_test_loop(void);

/**
 * @brief 膜式燃气表测试开始
 */
void diaphragm_test_start(void);

/**
 * @brief 膜式燃气表测试结果清零
 */
void diaphragm_test_result_clear(void);

/**
 * @brief 获取膜式燃气表测试步骤名称
 * @param step 测试步骤
 * @return 步骤名称字符串
 */
const char *diaphragm_get_step_name(DiaphragmTestStep_t step);

/**
 * @brief 切换到指定测试步骤
 * @param next_step 目标步骤
 * @details 自动重置重试计数、子状态，并设置新步骤的超时时间
 *          如果步骤配置了 on_enter 回调，则执行它
 */
void diaphragm_goto_step(DiaphragmTestStep_t next_step);

/**
 * @brief 当前步骤成功完成，跳转到配置的下一步
 * @details 根据步骤表中的 next_step 字段自动跳转：
 *          - DIAPHRAGM_NEXT_AUTO: 跳转到当前步骤+1
 *          - DIAPHRAGM_NEXT_STAY: 保持当前步骤 (由handler控制)
 *          - 具体步骤枚举: 跳转到指定步骤
 */
void diaphragm_step_success(void);

/**
 * @brief 跳过当前步骤（未实现或不需要测试）
 * @param reason 跳过原因描述
 * @details 会打印跳过日志，便于区分真正执行的步骤和跳过的步骤
 */
void diaphragm_step_skip(const char *reason);

/**
 * @brief 获取当前步骤描述
 * @return 当前步骤的描述结构体指针
 */
const DiaphragmStepDescriptor_t *diaphragm_get_current_step_desc(void);

/**
 * @brief 当前步骤失败，记录失败原因并跳转到结束
 * @param reason 失败原因
 */
void diaphragm_step_fail(DgmFailReason_t reason);

/**
 * @brief 获取失败原因的字符串描述
 * @param reason 失败原因枚举
 * @return 失败原因字符串
 */
const char *diaphragm_get_fail_reason_string(DgmFailReason_t reason);

/*============================================================================
 *                          膜式燃气表测试步骤处理函数
 *===========================================================================*/

void diaphragm_handle_wait(void);
void diaphragm_handle_start(void);
void diaphragm_check_gprs_voltage(void);
void diaphragm_wait_module_ready_response(void);
void diaphragm_handle_io_status_check_initial_status(void);
void diaphragm_handle_power_check(void);
void diaphragm_handle_temp_check(void);
void diaphragm_handle_nb_connect(void);
void diaphragm_handle_imei_read(void);
void diaphragm_close_ir(void);
void diaphragm_handle_uart_connect_test(void);
void diaphragm_handle_end(void);
void debug_breakpoint(void);
/*============================================================================
 *                          协议事件处理函数
 *===========================================================================*/

/**
 * @brief 膜式燃气表协议事件处理回调
 * @param event 协议事件
 * @details 由协议层调用，处理解析完成的协议事件
 */
void diaphragm_on_protocol_event(const DgmProtocolEvent *event);

/**
 * @brief 注册协议事件回调
 * @details 在测试初始化时调用，将事件处理函数注册到协议层
 */
void diaphragm_register_protocol_callback(void);

#endif /* __DIAPHRAGME_METER_TEST_H__ */
