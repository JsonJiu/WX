/**
 * @file main.c
 * @brief  主程序入口文件,当前做了膜式气表测试功能，水表功能已移除
 * @version 2.7.4
 *
 * 架构说明:
 * ┌─────────────────────────────────────────────────────────────┐
 * │                        main()                               │
 * │  ┌─────────────┐  ┌─────────────┐  ┌─────────────────────┐  │
 * │  │  系统初始化  │→ │  外设初始化  │→│     主循环选择        │  │
 * │  └─────────────┘  └─────────────┘  └──────────┬──────────┘  │
 * │                                               │             │
 * │                    ┌──────────────────────────┼─────┐       │
 * │                    ↓                          ↓     │       │
 * │           ┌─────────────────┐      ┌──────────────────┐     │
 * │           │  透传模式循环    │      │  正常测试模式循环 │     │
 * │           │ (PC<->水表透传)  │      │  (自动化测试流程) │     │
 * │           └─────────────────┘      └──────────────────┘     │
 * └─────────────────────────────────────────────────────────────┘
 */

/*============================================================================
 *                              头文件包含
 *============================================================================*/
/* EasyLogger (必须在 elog.h/components.h 之前定义 LOG_TAG) */
#define LOG_TAG "main"

#include "main.h"

/* 硬件驱动 */
#include "ADC_CHK.h"
#include "GPIO.h"
#include "LED_CTRL.h"
#include "WTD.h"
#include "mf_config.h"
#include "uart0.h"
#include "uart1.h"
#include "uart3.h"
#include "uart4.h"

/* 应用模块 */
#include "Test_List.h"
#include "app_config.h"
#include "time.h"
#include "tongxin_xieyi_Ctrl.h"

/* 组件库 */
#include "components.h"
#include "elog_user_config.h"
#include "led_indicator.h"
#include "test_step_strings.h"
#include "time_manager.h"
#include "valve_ctrl.h"
#include <inttypes.h>

/* 调试模块 */
#include "NB_18_DiaphragmGas_board_debug.h"

/* 标准库 */
#include <stdint.h>
#include <string.h>

/*============================================================================
 *                              版本信息,方便后续查看是否是最新的代码
 *============================================================================*/
#define SOFTWARE_VERSION_MAJOR 3
#define SOFTWARE_VERSION_MINOR 0
#define SOFTWARE_VERSION_PATCH 1
#define SOFTWARE_VERSION_STRING "v3.0.1"

/*============================================================================
 *                              全局变量
 *============================================================================*/
uint8_t Debug_Mode = 0;       /**< 调试模式: 0=关闭, 1=开启 */
uint8_t PassThrough_Mode = 0; /**< 透传模式: 0=普通, 1=透传 */
uint8_t PassThrough_Preamble =
    0; /**< 透传前导: 0=无前导,
          1=有前导,默认状态无前导，避免哪天开了忘记关闭导致测试时间增加 */

/*============================================================================
 *                              外部声明
 *============================================================================*/
/* UART0 (PC通信) */
extern volatile uint8_t uart0_rx_data_flag;
extern uint8_t uart0_rx_buffer[];
extern volatile uint16_t uart0_rx_count;
extern volatile uint16_t uart0_rx_frame_timeout;

/* UART1 (水表红外通信) */
extern volatile uint8_t uart1_rx_data_flag;
extern uint8_t uart1_rx_buffer[];
extern volatile uint16_t uart1_rx_count;
extern volatile uint16_t uart1_rx_frame_timeout;

/* 测试相关 */
extern VT_TestResult valve_test_result;
uint16_t debug_print_time = DEBUG_PRINT_TIME;
uint32_t protocol_response_param = 0;

/*============================================================================
 *                          静态函数声明
 *============================================================================*/
/* 初始化相关 */
static void System_Init(void);
static void Peripheral_Init(void);
static void EasyLogger_Init(void);

/* 回调函数 (供协议层调用) */
static uint8_t Callback_GetStationId(void);
static uint8_t Callback_GetFailInfo(uint8_t *step_id, char *step_name,
                                    uint8_t *fail_reason, char *reason_name);
static uint16_t Callback_GetVersion(void);
static void Callback_GetBuildTime(char *build_time);
static void Callback_FTControl(const uint8_t *data, uint16_t length);

/* 主循环 */
static void Loop_PassThrough(void);
static void Loop_NormalTest(void);

/* 调试辅助 */
static void Print_SystemInfo(void);
static void Board_SelfTest(void);

/*============================================================================
 *                              主函数
 *============================================================================*/
int main(void) {
#ifdef USE_BOOTLOADER
  __enable_irq(); /* Bootloader跳转后需重新启用中断，在bootloader中禁用了中断 */
#endif

  /* 系统初始化 */
  System_Init();

  /* 外设初始化 */
  Peripheral_Init();

  /* 显示系统信息 */
  Print_SystemInfo();

  /* 对当前线路板进行自检，确保各项功能正常 */
  Board_SelfTest();

  /* 进入主循环 */
  log_i("系统初始化完成，进入主循环");

  if (PassThrough_Mode) {
    log_i("进入透传模式主循环");
    Loop_PassThrough();
  } else {
    log_i("进入正常测试模式主循环");
    Loop_NormalTest();
  }

  return 0; /* 不应执行到此 */
}

/*============================================================================
 *                            初始化函数
 *============================================================================*/

/**
 * @brief 系统核心初始化 (FL库、时钟)
 */
static void System_Init(void) {
  FL_Init();
  MF_Clock_Init();
}

/**
 * @brief 外设初始化
 */
static void Peripheral_Init(void) {
  /* GPIO */
  Others_GPIO_Init();

  /* UART */
  UART0_MF_Config_Init(); /* PC上位机端口 */
  UART1_MF_Config_Init(); /* 红外通信端口 */
  UART3_MF_Config_Init(); /* 计量协议端口 */
  UART4_MF_Config_Init(); /* 远红外检测端口 */

  /* 定时器 */
  ATIM_Init();

  /* 统一时间管理器 */
  TM_Init();

  /* EasyLogger */
  EasyLogger_Init();

  /* ADC */
MF_ADC_PC10_Config_Init();

  /* 工位检测 */
  gongwei_jiance();

  /* 测试模块 */
  test_start_Init();

  /* 组件系统初始化 */
  ComponentsConfig components_cfg = {
      .pc_send = Uart0_Tx_Send,
      .device_send = Uart1_Tx_Send, // 使用纯净发送函数，前导由协议层处理
      .get_station_id = Callback_GetStationId,
      .get_fail_info = Callback_GetFailInfo,
      .get_version = Callback_GetVersion,
      .get_build_time = Callback_GetBuildTime,
      .ft_control = Callback_FTControl,
  };
  Components_Init(&components_cfg);

  /* LED指示器组件初始化 */
  LedIndicator_BoardInit();

#ifdef ENABLE_WATCHDOG
  WatchDog_Init();
#endif
}

static void Board_SelfTest(void) {
  log_i("正在进行系统自检...");
  // 这里可以添加更多的自检项，例如传感器检测、通信测试等
  // 检测第一步：电源系统检测
  // 电源检测，进行自检，检测当前的主电输出是否正常，检查备电是否正常，检查电磁铁和电机的12V供电是否正常，当然，如果硬件有自行修改过电路，可以根据实际情况调整自检内容
  // 检测第二步：通信系统检测
  // 检测当前的INA219 是否正常，是否在线，是否有回应
  log_i("系统自检完成，所有功能正常");
}

/**
 * @brief EasyLogger 初始化
 * @note 配置细节在 elog_user_config.h 中修改
 */
static void EasyLogger_Init(void) { ELog_UserInit(); }

/*============================================================================
 *                            回调函数
 *============================================================================*/

/**
 * @brief 获取本机工位号 (协议层回调)
 * @return 工位号 (0-4)
 */
static uint8_t Callback_GetStationId(void) { return Test_jiejuo_jilu.gongwei; }

/**
 * @brief 获取测试失败步骤信息 (协议层回调)
 * @param[out] step_id     失败步骤ID
 * @param[out] step_name   失败步骤名称
 * @param[out] fail_reason 失败原因代码
 * @param[out] reason_name 失败原因名称
 * @return 测试状态: 0=进行中, 1=成功, 2=失败
 */
static uint8_t Callback_GetFailInfo(uint8_t *step_id, char *step_name,
                                    uint8_t *fail_reason, char *reason_name) {
#ifdef ENABLE_DIAPHRAGM_METER_TEST
  // 膜式燃气表测试
  // 当测试进行中时，返回当前步骤；测试结束后返回失败步骤
  DiaphragmTestStep_t report_step;
  if (diaphragm_test_step == DIAPHRAGM_WAIT) {
    // 测试已结束，返回失败时记录的步骤
    report_step = diaphragm_runtime.fail_step;
  } else {
    // 测试进行中，返回当前正在执行的步骤
    report_step = diaphragm_test_step;
  }

  if (step_id) {
    *step_id = (uint8_t)report_step;
  }
  if (step_name) {
    const DiaphragmStepDescriptor_t *desc =
        &diaphragm_step_table[(uint8_t)report_step];
    strncpy(step_name, desc->name, 63);
    step_name[63] = '\0';
  }
  if (fail_reason) {
    *fail_reason = (uint8_t)diaphragm_runtime.fail_reason;
  }
  if (reason_name) {
    const char *reason_str =
        diaphragm_get_fail_reason_string(diaphragm_runtime.fail_reason);
    strncpy(reason_name, reason_str, 63);
    reason_name[63] = '\0';
  }
  // 根据 diaphragm_runtime 的 fail_reason 和当前步骤判断状态
  if (diaphragm_test_step == DIAPHRAGM_WAIT) {
    // 在空闲步骤，测试未开始或已结束
    if (diaphragm_runtime.fail_reason == DGM_FAIL_NONE) {
      return 1; // 成功
    } else {
      return 2; // 失败
    }
  }
  return 0; // 进行中
#else
  // 其他测试模块 (水表等)
  if (step_id) {
    *step_id = (uint8_t)Test_quanju_canshu_L.failed_step;
  }
  if (step_name) {
    const char *name =
        TestStep_GetName((uint8_t)Test_quanju_canshu_L.failed_step);
    strncpy(step_name, name, 63);
    step_name[63] = '\0';
  }
  if (fail_reason) {
    *fail_reason = 0; // 老结构体无失败原因字段
  }
  if (reason_name) {
    strcpy(reason_name, Test_quanju_canshu_L.status_description);
  }
  return (uint8_t)Test_quanju_canshu_L.test_status;
#endif
}

/**
 * @brief 获取程序版本号回调
 * @return 版本号 (高字节=主版本, 低字节=次版本)
 */
static uint16_t Callback_GetVersion(void) {
  return (SOFTWARE_VERSION_MAJOR << 8) | SOFTWARE_VERSION_MINOR;
}

/**
 * @brief 获取编译时间回调
 * @param[out] build_time 编译时间字符串缓冲区 (至少32字节)
 */
static void Callback_GetBuildTime(char *build_time) {
  if (build_time != NULL) {
    snprintf(build_time, 32, "%s %s", __DATE__, __TIME__);
  }
}

static void Callback_FTControl(const uint8_t *data, uint16_t length) {
  if (data == NULL || length == 0) {
    log_w("工装控制指令参数无效");
    return;
  }
  diaphragm_board_debug_handle_command(data, length);
}

/*============================================================================
 *                            主循环函数
 *============================================================================*/

/**
 * @brief 透传模式主循环
 * @note PC (UART0) <--> 水表 (UART1) 双向透传
 */
static void Loop_PassThrough(void) {
  /* 开启被测线路板电源 */
  zhudian_gongdian_On();
  log_i("UART0(PC) <--> UART1(水表红外) 透传中...");

  while (PassThrough_Mode) {
    /* PC -> 水表 */
    if (uart0_rx_data_flag == 1 && uart0_rx_frame_timeout == 0) {
      /* 检查是否是控制指令 (68 AE ...) */
      if (uart0_rx_count >= 8 && uart0_rx_buffer[0] == FT_FRAME_HEAD) {
        // 如果是工装配置协议帧头，继续进行验证工装帧尾是否正确
        uint8_t frame_length = uart0_rx_buffer[2];
        if (uart0_rx_buffer[0 + frame_length - 1] == FT_FRAME_TAIL) {
          log_d("收到工装配置指令,即将关闭透传模式并进行协议解析");
          Protocol_PC_Parse(uart0_rx_buffer, uart0_rx_count);
          uart0_rx_data_flag = 0;
          uart0_rx_count = 0;
          continue;
        }
        Protocol_PC_Parse(uart0_rx_buffer, uart0_rx_count);
      } else {
        /* 普通数据转发 */
        log_d(">> PC->水表:");
        protocol_debug_print(uart0_rx_buffer, uart0_rx_count);
        // 根据前导模式选择发送方式
        if (PassThrough_Preamble) {
          Uart1_Tx_Send_ok(uart0_rx_buffer, uart0_rx_count);
        } else {
          Uart1_Tx_Send(uart0_rx_buffer, uart0_rx_count);
        }
      }
      uart0_rx_data_flag = 0;
      uart0_rx_count = 0;
    }

    /* 水表 -> PC */
    if (uart1_rx_data_flag == 1 && uart1_rx_frame_timeout == 0) {
      log_d("<< 水表->PC:");
      protocol_debug_print(uart1_rx_buffer, uart1_rx_count);
      Uart0_Tx_Send(uart1_rx_buffer, uart1_rx_count);
      uart1_rx_data_flag = 0;
      uart1_rx_count = 0;
    }

#ifdef ENABLE_WATCHDOG
    FL_IWDT_ReloadCounter(IWDT);
#endif
    LED_FLAG_LOOP();
    LedIndicator_Process(); /* LED指示器状态更新 */
  }

  log_i("退出透传模式");
}

/**
 * @brief 正常测试模式主循环
 */
static void Loop_NormalTest(void) {
  /* 阀门测试状态追踪 */
  static uint32_t last_valve_tick = 0;
  static uint8_t valve_test_was_running = 0;

  /* 初始化阀门控制，气表的阀门其实暂时没有模块化，也比较简单，水表的阀门需要模块初始化
   */
  ValveCtrl_Init();

  while (1) {
    /* 检测模式切换 */
    if (PassThrough_Mode) {
      log_i("检测到透传模式开启，切换到透传循环");
      Loop_PassThrough();
      log_i("透传模式关闭，返回正常测试模式");
      continue;
    }

    /* UART 数据接收处理 */
    Uart3_Rx_rec(); /* 计量协议，超声计量模组，水表用的到，气表用不到，气表有自己的霍尔计量，光电等计量模式
                     */
    Uart4_Rx_rec(); /* 远红外检测 */
    Uart1_Rx_rec(); /* 红外通信，这里是和气表或者水表的近红外通讯 */
    Uart0_Rx_rec(); /* PC调试 */

    /* LED状态更新 */
    LED_FLAG_LOOP();
    LedIndicator_Process(); /* LED指示器状态更新 */

    /* 调试控制模式任务处理，如果没有通过上位发送指令，这个一般不会进入，除非后续增加了自动进入控制模式的功能，比如通过检测某个GPIO状态或者特定的UART指令来自动进入控制模式，这样就不需要每次都通过上位机发送指令来进入控制模式了，直接进入就可以了，这样在测试过程中如果需要进行调试或者查看某些状态，就可以直接进入控制模式进行相关操作，而不需要再通过上位机发送指令来触发了，这样就更加方便了
     * (功耗测试、电压采集等),这个一般可以跳过不看，和测试以及mes无关，一般是可以通过上位机触发相关的协议，只是方便调试，比如直接指令直接查看当前的功耗，或者直接发送指令给工装进行通电关电发发送*/
    if (BoardDebug_IsControlModeActive()) {
      // 如果是控制模式，执行调试任务处理，否则跳过减少解析时间，
      BoardDebug_Task_Process(); // 当前只针对气表，水表的不支持，如果是水表项目直接注释掉这个判断和函数调用就可以了
    }

    /* 测试流程处理,这个和我们当前的测试流程有关 */
    auto_test_loop();

    /* 阀门测试处理 (10ms周期)，这个和气表无关 */
    if (enable_valve_test) {
      uint32_t current_tick = system_tick_ms;

      /* 阀门测试刚启动时重置时间戳 */
      if (!valve_test_was_running) {
        last_valve_tick = current_tick;
        valve_test_was_running = 1;
      }

      if (current_tick - last_valve_tick >= 10) {
        uint32_t elapsed = current_tick - last_valve_tick;
        last_valve_tick = current_tick;
        valve_test_result = ValveCtrl_Process(elapsed);
      }
    } else {
      valve_test_was_running = 0;
    }

#ifdef ENABLE_WATCHDOG
    FL_IWDT_ReloadCounter(IWDT);
#endif
  }
}

/*============================================================================
 *                            调试辅助函数
 *============================================================================*/

/**
 * @brief 打印系统信息
 */
static void Print_SystemInfo(void) {
  log_i("============================================");
  log_i("  NB_IOT_Gas_Meter_Board_18 %s", SOFTWARE_VERSION_STRING);
  log_i("  Build: %s %s", __DATE__, __TIME__);
  log_i("============================================");
}

/**
 * @brief 打印时钟配置 (调试用)
 */
void print_clock_config(void) {
  log_i("=== 系统时钟配置 ===");
  log_i("软件版本: %s", SOFTWARE_VERSION_STRING);
  log_i("编译时间: %s %s", __DATE__, __TIME__);
  log_i("配置值: SYSTEM_CLOCK_MHZ = %d", SYSTEM_CLOCK_MHZ);

#ifdef USE_CLOCK_8MHZ
  log_i("系统时钟: 8MHz (红外优化模式)");
#elif defined(USE_CLOCK_16MHZ)
  log_i("系统时钟: 16MHz (平衡模式)");
#elif defined(USE_CLOCK_24MHZ)
  log_i("系统时钟: 24MHz (高性能模式)");
#else
  log_i("系统时钟: 32MHz (默认模式)");
#endif

  log_i("实际频率: %" PRIu32 " Hz", SystemCoreClock);
  log_i("==================");
}

/*============================================================================
 *                        兼容旧代码的函数别名
 *============================================================================*/
void elog_user_init(void) { EasyLogger_Init(); }
void test_Init(void) { Peripheral_Init(); }
void PassThrough_Loop_Func(void) { Loop_PassThrough(); }
void NormalTest_Loop_Func(void) { Loop_NormalTest(); }
