/**
 * @file    uart0.c
 * @brief   UART0 驱动 - PC通信接口（支持多工位并联）
 * @note    硬件: PA13(RX), PA14(TX), 波特率: 9600
 *          特性: 多工位TX并联总线仲裁
 */
#define LOG_TAG "uart0"

/*===========================================================================*/
/*                              头文件                                        */
/*===========================================================================*/
#include "Peripheral/uart/uart0.h"
#include "LED_CTRL.h"
#include "Protocol/protocol.h"
#include "Test_List.h"
#include "main.h"
#include <elog.h>
#include <stdarg.h>
#include <string.h>

/*===========================================================================*/
/*                          外部变量声明                                      */
/*===========================================================================*/
extern uint8_t PassThrough_Mode;

/*===========================================================================*/
/*                            私有宏定义                                      */
/*===========================================================================*/
/* 缓冲区大小和超时配置统一在 uart_common.h 中定义 */

/* TX引脚模式定义 */
#define UART0_TX_MODE_INPUT 0  /* 输入模式（释放总线）*/
#define UART0_TX_MODE_OUTPUT 1 /* 输出模式（控制总线）*/

/* 总线仲裁配置 */
#define BUS_IDLE_BASE_COUNT 20      /* 总线空闲检测基础计数 */
#define BUS_IDLE_STATION_MULT 20    /* 每工位额外等待时间 */
#define BUS_IDLE_TOTAL_TIMEOUT 5000 /* 总线空闲检测总超时(ms) */

/* 发送状态定义（三态）*/
#define UART0_TX_IDLE 0     /* 空闲 */
#define UART0_TX_BUSY 1     /* 发送中 */
#define UART0_TX_COMPLETE 2 /* 完成 */

/*===========================================================================*/
/*                            私有结构体                                      */
/*===========================================================================*/
typedef struct {
  uint8_t *TxBuf; /* 发送缓冲区指针 */
  volatile uint16_t
      TxTotalLen; /* 总发送长度 (ISR中不修改，但与TxSentCount配对比较) */
  volatile uint16_t TxSentCount; /* 已发送计数 (ISR中递增，必须volatile) */
} UART0_TxCtrl_t;

/*===========================================================================*/
/*                            私有变量                                        */
/*===========================================================================*/
/* 接收缓冲区 */
static uint8_t s_rx_buffer[UART0_BUFFER_SIZE];

/* 接收处理缓冲区 */
static uint8_t s_rx_process_buffer[UART0_BUFFER_SIZE];

/* 发送缓冲区 */
static uint8_t s_tx_buffer[UART0_BUFFER_SIZE];

/* 发送控制结构体 */
static UART0_TxCtrl_t s_tx_ctrl = {
    .TxBuf = NULL, .TxTotalLen = 1, .TxSentCount = 1};

/* 接收计数 */
static volatile uint16_t s_rx_count = 0;

/* 接收数据标志 */
static volatile uint8_t s_rx_data_flag = 0;

/* 接收溢出标志 - 缓冲区满时置位，提示数据不完整 */
static volatile uint8_t s_rx_overflow_flag = 0;

/* 发送状态（三态: IDLE/BUSY/COMPLETE）*/
static volatile uint8_t s_tx_state = UART0_TX_IDLE;

/*===========================================================================*/
/*                            公有变量                                        */
/*===========================================================================*/
/* 接收相关（外部模块使用）*/
volatile uint8_t uart0_rx_data_flag = 0;
uint8_t uart0_rx_buffer[UART0_BUFFER_SIZE];
volatile uint16_t uart0_rx_count = 0;

/* 超时计数器 - 在 time.c 中断中递减 */
volatile uint16_t uart0_tx_timeout_counter = 0;
volatile uint16_t uart0_rx_frame_timeout = 0;
volatile uint16_t uart0_bus_idle_counter = 0; /* 总线空闲检测计数器 */

/*===========================================================================*/
/*                          私有函数声明                                      */
/*===========================================================================*/
static void UART0_GPIO_Init(void);
static void UART0_Peripheral_Init(void);
static void UART0_Interrupt_Init(void);
static void UART0_NVIC_Init(void);
static void UART0_TxPinMode(uint8_t mode);
static void UART0_WaitPrevSendDone(void);
static void UART0_WaitBusIdle(void);

/*===========================================================================*/
/*                          初始化函数                                        */
/*===========================================================================*/

/**
 * @brief UART0 完整初始化
 */
void UART0_MF_Config_Init(void) {
  UART0_GPIO_Init();
  UART0_Peripheral_Init();
  UART0_Interrupt_Init();
  UART0_NVIC_Init();

  /* 初始化发送控制 */
  s_tx_ctrl.TxTotalLen = 1;
  s_tx_ctrl.TxSentCount = 1;
  s_tx_state = UART0_TX_IDLE;
}

/**
 * @brief GPIO 初始化,默认状态我们将 TX 设置为输入模式
 * @note  RX: PA13, TX: PA14
 */
static void UART0_GPIO_Init(void) {
  FL_GPIO_InitTypeDef GPIO_InitStruct;

  /* RX: PA13 - 数字输入，使能上拉 */
  GPIO_InitStruct.pin = FL_GPIO_PIN_13;
  GPIO_InitStruct.mode = FL_GPIO_MODE_DIGITAL;
  GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.pull = FL_ENABLE;
  GPIO_InitStruct.remapPin = FL_DISABLE;
  GPIO_InitStruct.analogSwitch = FL_DISABLE;
  FL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* TX: PA14 - 初始为输入模式（IDLE状态，释放总线）*/
  GPIO_InitStruct.pin = FL_GPIO_PIN_14;
  GPIO_InitStruct.mode = FL_GPIO_MODE_INPUT;
  GPIO_InitStruct.pull = FL_DISABLE; /* 上拉保持高电平 */
  FL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

/**
 * @brief UART 外设初始化
 */
static void UART0_Peripheral_Init(void) {
  FL_UART_InitTypeDef UART_InitStruct;

  UART_InitStruct.clockSrc = FL_CMU_UART0_CLK_SOURCE_APBCLK;
  UART_InitStruct.baudRate = 9600;
  UART_InitStruct.transferDirection = FL_UART_DIRECTION_TX_RX;
  UART_InitStruct.dataWidth = FL_UART_DATA_WIDTH_8B;
  UART_InitStruct.stopBits = FL_UART_STOP_BIT_WIDTH_1B;
  UART_InitStruct.parity = FL_UART_PARITY_NONE;
  FL_UART_Init(UART0, &UART_InitStruct);
}

/**
 * @brief 中断使能初始化
 */
static void UART0_Interrupt_Init(void) {
  /* 清除历史残留 */
  FL_UART_ClearFlag_RXBuffFull(UART0);
  FL_UART_EnableIT_RXBuffFull(UART0);

  /* 清除发送中断标志（发送时才使能）*/
  FL_UART_ClearFlag_TXShiftBuffEmpty(UART0);
}

/**
 * @brief NVIC 中断配置
 * @note  优先级 0x02，低于 UART1(0x00)
 *        PC通信优先级较低，确保设备通信不被打断
 */
static void UART0_NVIC_Init(void) {
  FL_NVIC_ConfigTypeDef NVIC_InitStruct;
  NVIC_InitStruct.preemptPriority = 0x02;
  FL_NVIC_Init(&NVIC_InitStruct, UART0_IRQn);
}

/*===========================================================================*/
/*                          TX引脚控制                                        */
/*===========================================================================*/

/**
 * @brief 切换TX引脚模式
 * @param mode UART0_TX_MODE_INPUT: 输入模式（释放总线）
 *             UART0_TX_MODE_OUTPUT: 输出模式（控制总线）
 */
static void UART0_TxPinMode(uint8_t mode) {
  FL_GPIO_InitTypeDef GPIO_InitStruct;

  GPIO_InitStruct.pin = FL_GPIO_PIN_14;
  GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.remapPin = FL_DISABLE;
  GPIO_InitStruct.analogSwitch = FL_DISABLE;

  if (mode == UART0_TX_MODE_INPUT) {
    GPIO_InitStruct.mode = FL_GPIO_MODE_INPUT;
    GPIO_InitStruct.pull = FL_DISABLE; /* 输入时禁止上拉 */
  } else {
    GPIO_InitStruct.mode = FL_GPIO_MODE_DIGITAL;
    GPIO_InitStruct.pull = FL_DISABLE; /* 输出时禁止上拉 */
  }

  FL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

/*===========================================================================*/
/*                            发送函数                                        */
/*===========================================================================*/

/**
 * @brief 初始化发送控制（兼容接口）
 */
void Uart0_Tx_Send_init(void) {
  s_tx_ctrl.TxTotalLen = 1;
  s_tx_ctrl.TxSentCount = 1;
  s_tx_state = UART0_TX_IDLE;
}

/**
 * @brief 等待上一次发送完成并恢复TX为输入模式
 */
static void UART0_WaitPrevSendDone(void) {
  /* 等待中断发送完成 */
  uart0_tx_timeout_counter = UART0_TX_TIMEOUT_MS;
  while (s_tx_ctrl.TxTotalLen != s_tx_ctrl.TxSentCount &&
         uart0_tx_timeout_counter) {
    /* 等待定时器递减 */
  }

  /* 恢复TX引脚为输入模式 */
  if (s_tx_state != UART0_TX_IDLE) {
    if (s_tx_state == UART0_TX_BUSY) {
      uart0_tx_timeout_counter = 50; /* 清理超时 */
      while (s_tx_state == UART0_TX_BUSY && uart0_tx_timeout_counter) {
        /* 等待中断完成 */
      }
      if (s_tx_state == UART0_TX_BUSY) {
        FL_UART_DisableIT_TXShiftBuffEmpty(UART0);
        /* 注意: 此处不能调用 log_w()！
         * 因为调用链是 log_i → Uart0_Tx_Send → WaitPrevSendDone,
         * elog 锁已被持有，再调用 log_w 会导致永久死锁。
         * 仅静默处理超时。 */
      }
    }

    FL_DelayMs(1); /* 等待最后字节发出 */
    UART0_TxPinMode(UART0_TX_MODE_INPUT);
    FL_DelayMs(1); /* 等待总线稳定 */
    s_tx_state = UART0_TX_IDLE;
  }
}

/**
 * @brief 等待总线空闲（多工位防冲突）
 *
 * @note 检测原理：
 *       - 总线空闲时，上拉使PA14读到高电平
 *       - 其他工位发送时，会拉低总线
 *       - 工位号越大等待越久，形成优先级避让
 */
static void UART0_WaitBusIdle(void) {
  if (Debug_Mode != 0) {
    return; /* 调试模式跳过 */
  }

  uart0_bus_idle_counter =
      BUS_IDLE_BASE_COUNT + BUS_IDLE_STATION_MULT * Test_jiejuo_jilu.gongwei;
  uint16_t total_timeout = BUS_IDLE_TOTAL_TIMEOUT;

  while (uart0_bus_idle_counter && total_timeout--) {
    if (FL_GPIO_GetInputPin(GPIOA, FL_GPIO_PIN_14) == 0) {
      /* 总线被占用，重置计数器 */
      uart0_bus_idle_counter = BUS_IDLE_BASE_COUNT +
                               BUS_IDLE_STATION_MULT * Test_jiejuo_jilu.gongwei;
    }
    /* uart0_bus_idle_counter 在定时器中断中递减 */
  }

  if (total_timeout == 0) {
    log_w("UART0 bus idle timeout");
  }
}

/**
 * @brief UART0 发送数据
 * @param data 数据指针
 * @param len  数据长度
 *
 * @note 发送流程：
 *       1. 等待上一次发送完成
 *       2. 等待总线空闲
 *       3. 切换TX为输出，启动中断发送
 */
void Uart0_Tx_Send(uint8_t data[], uint16_t len) {
  if (len == 0 || data == NULL) {
    return;
  }

  /* 长度校验，防止缓冲区溢出 */
  if (len > UART0_BUFFER_SIZE) {
    /* 注意: 此处不能调用 log_e()！
     * 因为 Uart0_Tx_Send 可能被 elog 的输出函数调用，
     * elog 锁已被持有，再调用 log_e 会导致永久死锁。 */
    return;
  }

  /* 1. 等待上一次发送完成 */
  UART0_WaitPrevSendDone();

  /* 2. 等待总线空闲 */
  UART0_WaitBusIdle();

  /* 3. 切换TX为输出，启动发送 */
  UART0_TxPinMode(UART0_TX_MODE_OUTPUT);
  s_tx_state = UART0_TX_BUSY;

  /* 4. 复制数据，配置发送参数 */
  memcpy(s_tx_buffer, data, len);
  s_tx_ctrl.TxBuf = s_tx_buffer;
  s_tx_ctrl.TxTotalLen = len;
  s_tx_ctrl.TxSentCount = 1; /* 从1开始，第0字节手动发送 */

  /* 5. 启动中断链 */
  FL_UART_ClearFlag_TXShiftBuffEmpty(UART0);
  FL_UART_EnableIT_TXShiftBuffEmpty(UART0);
  FL_UART_WriteTXBuff(UART0, s_tx_ctrl.TxBuf[0]);
}

/**
 * @brief 调试模式发送（Debug_Mode=0时不发送）
 */
void PC_Debug_Send(uint8_t data[], uint16_t len) {
  if (Debug_Mode == 0) {
    return;
  }
  Uart0_Tx_Send(data, len);
}

/**
 * @brief PC串口发送
 */
void PC_Comm_Send(uint8_t data[], uint16_t len) { Uart0_Tx_Send(data, len); }

/*===========================================================================*/
/*                            接收处理                                        */
/*===========================================================================*/

/**
 * @brief 接收处理（主循环调用）
 *
 * @note 功能：
 *       1. 发送完成后恢复TX为输入模式
 *       2. 帧超时后处理接收到的完整数据帧
 */
void Uart0_Rx_rec(void) {
  /* 发送完成后恢复TX为输入模式 */
  if (s_tx_state == UART0_TX_COMPLETE) {
    FL_DelayMs(5);
    UART0_TxPinMode(UART0_TX_MODE_INPUT);
    s_tx_state = UART0_TX_IDLE;
  }

  /* 透传模式下不在这里处理数据 */
  if (PassThrough_Mode) {
    return;
  }

  /* 帧接收完成判断 */
  if (s_rx_data_flag && uart0_rx_frame_timeout == 0) {
    LED_FLAG_Run();

    /* 保护临界区：同时处理溢出标志和数据获取，防止竞态 */
    __disable_irq();
    uint8_t overflow = s_rx_overflow_flag;
    uint16_t rx_len = s_rx_count;
    s_rx_overflow_flag = 0;
    s_rx_count = 0;
    s_rx_data_flag = 0;
    __enable_irq();

    /* 检查溢出标志 */
    if (overflow) {
      log_w("UART0 RX overflow, frame discarded");
      return;
    }

    /* 复制到处理缓冲区 */
    memcpy(s_rx_process_buffer, s_rx_buffer, rx_len);

    /* 同步到公有变量 */
    memcpy(uart0_rx_buffer, s_rx_process_buffer, rx_len);
    uart0_rx_count = rx_len;

    /* 调试打印 */
    log_d("PC->设备 收到 %d 字节:", rx_len);
    elog_hexdump("uart0", ELOG_LVL_DEBUG, s_rx_process_buffer, rx_len);

    /* 协议解析 */
    log_d("即将进行PC协议解析");
    Protocol_PC_Parse(s_rx_process_buffer, rx_len);

    /* 清空公有变量 */
    uart0_rx_data_flag = 0;
    uart0_rx_count = 0;
  }
}

/*===========================================================================*/
/*                            中断服务                                        */
/*===========================================================================*/

/**
 * @brief UART0 中断服务函数
 */
void UART0_IRQHandler(void) {
  /* 接收中断 */
  if (FL_UART_IsEnabledIT_RXBuffFull(UART0) &&
      FL_UART_IsActiveFlag_RXBuffFull(UART0)) {
    uint8_t data = FL_UART_ReadRXBuff(UART0);

    /* 先检查溢出，防止越界写入 */
    if (s_rx_count >= UART0_BUFFER_SIZE) {
      s_rx_overflow_flag = 1; /* 标记溢出，让上层丢弃此帧 */
      /* 不写入数据，但仍需更新标志和超时 */
    } else {
      s_rx_buffer[s_rx_count++] = data;
    }

    s_rx_data_flag = 1;
    uart0_rx_data_flag = 1;
    uart0_rx_frame_timeout = UART0_RX_FRAME_TIMEOUT_MS;
  }

  /* 发送中断 */
  if (FL_UART_IsEnabledIT_TXShiftBuffEmpty(UART0) &&
      FL_UART_IsActiveFlag_TXShiftBuffEmpty(UART0)) {
    if (s_tx_ctrl.TxSentCount >= s_tx_ctrl.TxTotalLen) {
      /* 发送完成，禁止中断 */
      FL_UART_DisableIT_TXShiftBuffEmpty(UART0);
      s_tx_state = UART0_TX_COMPLETE;
    } else {
      /* 继续发送下一个字节 */
      FL_UART_WriteTXBuff(UART0, s_tx_ctrl.TxBuf[s_tx_ctrl.TxSentCount++]);
    }
    /* 不需要手动清除 TXShiftBuffEmpty：
     * - 发送完成时：中断已禁止，标志状态无影响
     * - 继续发送时：WriteTXBuff 写入数据后硬件自动清除 */
  }
}

/*===========================================================================*/
/*                            调试打印                                        */
/*===========================================================================*/

/**
 * @brief 协议调试打印 - 使用 EasyLogger 的 hexdump 功能
 * @param protocol 协议数据
 * @param length 数据长度
 */
void protocol_debug_print(uint8_t protocol[], uint16_t length) {
  if (Debug_Mode == 0) {
    return;
  }
  elog_hexdump("protocol", ELOG_LVL_DEBUG, protocol, length);
}

/*===========================================================================*/
/*                          中断控制函数                                      */
/*===========================================================================*/

/**
 * @brief 禁用UART0接收中断
 * @details 用于测试过程中暂时屏蔽上位机通信干扰
 */
void UART0_DisableRxInterrupt(void) { FL_UART_DisableIT_RXBuffFull(UART0); }

/**
 * @brief 启用UART0接收中断
 * @details 用于测试结束后恢复上位机通信
 */
void UART0_EnableRxInterrupt(void) {
  FL_UART_ClearFlag_RXBuffFull(UART0);
  FL_UART_EnableIT_RXBuffFull(UART0);

  /* 清空接收缓冲区 */
  s_rx_data_flag = 0;
  s_rx_count = 0;
  s_rx_overflow_flag = 0;
  uart0_rx_data_flag = 0;
  uart0_rx_count = 0;
}
