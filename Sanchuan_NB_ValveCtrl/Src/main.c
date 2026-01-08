/**
 * @file    main.c
 * @version v1.2.2
 * @date    2024-12-11
 * @brief   Sanchuan NB Valve Controller Firmware
 */

#include "main.h"
#include "ADC_CHK.h"
#include "GPIO.h"
#include "LED_CTRL.h"
#include "PC_xieyi_Ctrl.h"
#include "Test_List.h"
#include "WTD.h"
#include "ZDINA219.h"
#include "time.h"
#include "tongxin_xieyi_Ctrl.h"
#include "uart0.h"
#include "uart1.h"
#include "uart3.h"
#include "uart4.h"
#include "valve_test.h"

// 加 volatile 防止被优化，确保调试时可见
volatile uint8_t Debug_Mode = 0;
// 透传模式设置
volatile uint8_t PassThrough_Mode = 0;
#define DEBUG_IDLE_TIME 1000
#define DEBUG_TEST_TIME 5000
#define WATCH_DOG_ENABLE 0
uint16_t debug_print_time = DEBUG_IDLE_TIME;
extern VT_TestResult valve_test_result;
extern volatile uint32_t system_tick_ms; // 系统毫秒计数器（time.c中定义）
// uint8_t hello[2] = {0x01,0x02};

void test_Init() {
  UART3_MF_Config_Init();
  UART4_MF_Config_Init();
  UART1_MF_Config_Init();
  UART0_MF_Config_Init();
  Others_GPIO_Init();
  ATIM_Init();
  MF_ADC_PC10_Config_Init();
  gongwei_jiance();
  test_start_Init();
#if WATCH_DOG_ENABLE
  WatchDog_Init();
#endif
}

int main(void) {
  /* Initialize FL Driver Library */
  /* SHOULD BE KEPT!!! */
  FL_Init();

  /* Reset of all peripherals, Initializes the Flash interface and the Systick.
   */
  /* SHOULD BE KEPT!!! */
  MF_Clock_Init();

  /* Initialize all configured peripherals */
  /* SHOULD BE KEPT!!! */
  test_Init();
  // SPI_RW_Demo();

  while (1) {
    if (PassThrough_Mode) {
      Run_PassThrough_Mode();
    } else {
      Run_Protocol_Mode();
    }
  }
}

void debug_break(void) {

  while (1) {
    if (debug_print_time == 0) {
      DeBug_print("当前已经关闭看门狗，是日志断点\r\n");
      debug_print_time = DEBUG_IDLE_TIME;
    }
  }
}

/**
 * @brief 透传模式主循环函数
 * @note 实现 UART0(PC) <--> UART1(水表红外) 双向透传，带前导
 */
void Run_PassThrough_Mode(void) {
  DeBug_print("当前工作在透传模式\r\n");
  DeBug_print("开启备电电源...\r\n");
  zhudian_gongdian_On();
  beidian_gongdian_On(); // 开启备电电源，水表需要备电才能通信
  DeBug_print("UART0(PC) <--> UART1(水表红外) 透传中（带前导）...\r\n");

  // 透传模式主循环：实现 UART0 和 UART1 双向透传（带前导）
  while (1) {
    // 如果当前当前是透传模式，就进行透传处理,当前死循环直接跳出
    if (!PassThrough_Mode) {
      break;
    }

    // Test_loop_func();
    //  UART0 接收到数据，转发到 UART1 (PC -> 水表)，使用带前导的发送
    // 如果数据帧大20，就不解析，如果数据帧小于10，就解析，看看是否是设置日志和透传的协议
    if (uart0_Rec_shuju_flag == 1 && uart0_Rec_shuju_time_count == 0) {
      DeBug_print(">> PC->水表: ");
      protocol_debug_print(uart0_Rec_shuju_neirong,
                           uart0_Rec_shuju_neirong_count);
      // 转发到 UART1 (水表)，带前导
      if (uart0_Rec_shuju_neirong_count > 20) {
        // 数据帧过大，不解析，直接透传
        Uart1_Tx_Send_ok(uart0_Rec_shuju_neirong,
                         uart0_Rec_shuju_neirong_count);
      } else {
        // 解析协议，看看是否是设置日志和透传模式的协议,如果不是，就进行透传
        PC_xieyijiexi(uart0_Rec_shuju_neirong, uart0_Rec_shuju_neirong_count);
        if (PassThrough_Mode) {
          // 仍然是透传模式，进行透传
          Uart1_Tx_Send_ok(uart0_Rec_shuju_neirong,
                           uart0_Rec_shuju_neirong_count);
        }
      }
      // 清除标志
      uart0_Rec_shuju_flag = 0;
      uart0_Rec_shuju_neirong_count = 0;
    }

    // UART1 接收到数据，转发到 UART0 (水表 -> PC)
    if (uart1_Rec_shuju_flag == 1 && uart1_Rec_shuju_time_count == 0) {
      // 如果不是Debug模式，就直接发送数据给PC
      if (!Debug_Mode) {
        Uart0_Tx_Send(uart1_Rec_shuju_neirong, uart1_Rec_shuju_neirong_count);
      }
      DeBug_print("<< 水表->PC: ");
      protocol_debug_print(uart1_Rec_shuju_neirong,
                           uart1_Rec_shuju_neirong_count);
      // 转发到 UART0 (PC) - 注释掉以避免二进制数据乱码
      // 如果需要真实透传（如给上位机软件），请取消注释下面这行
      // Uart0_Tx_Send(uart1_Rec_shuju_neirong, uart1_Rec_shuju_neirong_count);
      // 清除标志
      uart1_Rec_shuju_flag = 0;
      uart1_Rec_shuju_neirong_count = 0;
    }

    // 喂狗
#if WATCH_DOG_ENABLE
    FL_IWDT_ReloadCounter(IWDT);
#endif

    // LED 指示
    // LED_FLAG_LOOP();
  }
}

/**
 * @brief 正常协议模式主循环函数
 * @note 实现标准的测试流程，PC 通过协议控制测试过程
 */
void Run_Protocol_Mode(void) {
  DeBug_print("当前工作在协议模式\r\n");

  // 初始化阀门测试模块（只初始化一次）
  valve_test_init(&g_valve_ctx, NULL); // 使用默认HAL

  while (1) {
    if ((PassThrough_Mode)) {
      break;
    }

    // 接收各个串口的数据
    Uart3_Rx_rec();
    Uart4_Rx_rec();
    Uart1_Rx_rec();
    Uart0_Rx_rec();
    // LED 指示
    // LED_FLAG_LOOP();
    // 如果当前不在测试阶段，就每隔一段时间打印一次心跳指示，使用Debug_IDLE_TIME时间间隔
    LED_Control();
    // Motor_Start();
    //   Electromagnet_On();
    //      测试流程控制
    if (!enable_valve_test | (valve_test_result == VT_SUCCESS) |
        (valve_test_result == VT_FAIL) | (valve_test_result == VT_TIMEOUT)) {
      test_Loop_Func();
    }
    // test_Loop_Func();
    // 测试用
    // 开启主电源
    // zhudian_gongdian_On();
    // if (debug_print_time == 0) {
    //   // 发送阀门开阀指令,需要前导
    //   // 调用tongyongxieyi_ctrl.c中的函数
    //   void FM_Open_xieyi();
    // }
    // // 检测A和B电压
    // uint32_t A_voltage = get_famen_1_dianya();
    // uint32_t B_voltage = get_famen_2_dianya();
    // DeBug_print("阀门A电压:%lu mV, 阀门B电压:%lu mV\r\n", A_voltage,
    // B_voltage);

    //  阀门测试循环调用
    // 阀门测试独立循环（使用新架构）
    // 使用系统时间控制调用频率，确保每10ms调用一次
    {
      static uint32_t last_valve_tick = 0;
      static uint8_t valve_test_was_enabled = 0;

      if (enable_valve_test) {
        uint32_t current_tick = system_tick_ms;

        // 阀门测试刚启动时，重置last_valve_tick，避免首次elapsed过大
        if (!valve_test_was_enabled) {
          last_valve_tick = current_tick;
          valve_test_was_enabled = 1;
        }

        if (current_tick - last_valve_tick >= 10) {
          uint32_t elapsed = current_tick - last_valve_tick;
          last_valve_tick = current_tick;
          valve_test_result = valve_test_loop(&g_valve_ctx, elapsed);
        }
      } else {
        // 阀门测试未启用时，重置启动标志，为下次测试做准备
        valve_test_was_enabled = 0;
      }
    }
    // Test_loop_func();

    // Test_loop_func_avg();
    //  喂狗
#if WATCH_DOG_ENABLE
    FL_IWDT_ReloadCounter(IWDT);
#endif
  }
}

// LED的闪烁控制
void LED_Control() {
  if (Debug_Mode == 1) {
    if (debug_print_time == 0 && Test_liucheng_L == w_wait) {
      DeBug_print("心跳指示：当前程序运行在协议模式，正在空闲状态\r\n");
      LED_Toggle();
      debug_print_time = DEBUG_IDLE_TIME;
    } else if (debug_print_time == 0 && Test_liucheng_L != w_wait) {
      // 测试过程中打印更少一些，避免无关日志干扰测试日志
      // DeBug_print("测试过程中：当前程序运行在协议模式，正在进行测试\r\n");
      LED_Toggle();
      debug_print_time = DEBUG_TEST_TIME;
    }
  }
  // 非调试模式下LED常亮
  else {
    LED_On();
  }
}
