/**
 * @file    uart1.c
 * @brief   UART1 驱动 - 设备通信接口（膜表/水表）
 * @note    硬件: PB13(RX), PB14(TX), 波特率: 9600
 */
#define LOG_TAG "uart1"

/*===========================================================================*/
/*                              头文件                                        */
/*===========================================================================*/
#include "Peripheral/uart/uart1.h"
#include "LED_CTRL.h"
#include "Protocol/protocol.h"
#include "main.h"
#include <elog.h>
#include <string.h>

/*===========================================================================*/
/*                            私有宏定义                                      */
/*===========================================================================*/
/* 缓冲区大小和超时配置统一在 uart_common.h 中定义 */

/* 看门狗控制宏 - 与main.c保持一致 */
// #define ENABLE_WATCHDOG

/*===========================================================================*/
/*                            私有结构体                                      */
/*===========================================================================*/
typedef struct {
  uint8_t *TxBuf;               /* 发送缓冲区指针 */
  volatile uint16_t TxTotalLen; /* 总发送长度 */
  volatile uint16_t TxSentCount; /* 已发送计数 (ISR中递增，必须volatile) */
} UART1_TxCtrl_t;

/*===========================================================================*/
/*                            私有变量                                        */
/*===========================================================================*/
/* 接收缓冲区 */
static uint8_t s_rx_buffer[UART1_BUFFER_SIZE];

/* 接收处理缓冲区 */
static uint8_t s_rx_process_buffer[UART1_BUFFER_SIZE];

/* 发送缓冲区 */
static uint8_t s_tx_buffer[UART1_BUFFER_SIZE];

/* 发送控制结构体 */
static UART1_TxCtrl_t s_tx_ctrl = {
    .TxBuf = NULL, .TxTotalLen = 1, .TxSentCount = 1};

/* 接收计数 */
static volatile uint16_t s_rx_count = 0;

/* 接收数据标志 */
static volatile uint8_t s_rx_data_flag = 0;

/* 发送完成标志: 1-空闲/完成, 0-发送中 */
static volatile uint8_t s_tx_complete_flag = 1;

/* 接收溢出标志 */
static volatile uint8_t s_rx_overflow_flag = 0;

/* 前导码数据 */
static const uint8_t s_preamble_aa[50] = {
    0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
    0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
    0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
    0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA,
    0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA};

static const uint8_t s_preamble_calibrate[10] = {0xFE, 0xFE, 0xFE, 0xFE, 0xFE,
                                                 0xFE, 0xFE, 0xFE, 0xFE, 0xFE};

/* 前导码使能标志 */
uint8_t uart1_preamble_enabled = 1;

/*===========================================================================*/
/*                            公有变量                                        */
/*===========================================================================*/
/* 接收相关（外部模块使用）*/
volatile uint8_t uart1_rx_data_flag = 0;
uint8_t uart1_rx_buffer[UART1_BUFFER_SIZE];
volatile uint16_t uart1_rx_count = 0;

/* 超时计数器 - 在 time.c 中断中递减 */
volatile uint16_t uart1_tx_timeout_counter = 0;
volatile uint16_t uart1_rx_frame_timeout = 0;

/*===========================================================================*/
/*                          私有函数声明                                      */
/*===========================================================================*/
static void UART1_GPIO_Init(void);
static void UART1_Peripheral_Init(void);
static void UART1_Interrupt_Init(void);
static void UART1_NVIC_Init(void);

/*===========================================================================*/
/*                          初始化函数                                        */
/*===========================================================================*/

/**
 * @brief UART1 完整初始化
 */
void UART1_MF_Config_Init(void) {
  UART1_GPIO_Init();
  UART1_Peripheral_Init();
  UART1_Interrupt_Init();
  UART1_NVIC_Init();

  /* 初始化发送控制 */
  s_tx_ctrl.TxTotalLen = 1;
  s_tx_ctrl.TxSentCount = 1;
  s_tx_complete_flag = 1;
}

/**
 * @brief GPIO 初始化
 * @note  RX: PB13, TX: PB14
 */
static void UART1_GPIO_Init(void) {
  FL_GPIO_InitTypeDef GPIO_InitStruct;

  /* RX: PB13 - 数字输入，上拉 */
  GPIO_InitStruct.pin = FL_GPIO_PIN_13;
  GPIO_InitStruct.mode = FL_GPIO_MODE_DIGITAL;
  GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.pull = FL_ENABLE; /* 内部上拉，空闲时保持高电平 */
  GPIO_InitStruct.remapPin = FL_DISABLE;
  GPIO_InitStruct.analogSwitch = FL_DISABLE;
  FL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* TX: PB14 - 推挽输出 */
  GPIO_InitStruct.pin = FL_GPIO_PIN_14;
  GPIO_InitStruct.mode = FL_GPIO_MODE_DIGITAL;
  GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.pull = FL_DISABLE;
  FL_GPIO_Init(GPIOB, &GPIO_InitStruct);
}

/**
 * @brief UART 外设初始化
 * @note  时钟源使用 FL_CMU_UART1_CLK_SOURCE_APBCLK (UART0和UART1共用同一个宏名)
 */
static void UART1_Peripheral_Init(void) {
  FL_UART_InitTypeDef UART_InitStruct;

  UART_InitStruct.clockSrc = FL_CMU_UART0_CLK_SOURCE_APBCLK;
  UART_InitStruct.baudRate = 9600;
  UART_InitStruct.transferDirection = FL_UART_DIRECTION_TX_RX;
  UART_InitStruct.dataWidth = FL_UART_DATA_WIDTH_8B;
  UART_InitStruct.stopBits = FL_UART_STOP_BIT_WIDTH_1B;
  UART_InitStruct.parity = FL_UART_PARITY_NONE;
  FL_UART_Init(UART1, &UART_InitStruct);
}

/**
 * @brief 中断使能初始化
 */
static void UART1_Interrupt_Init(void) {
  /* 清除历史残留 */
  FL_UART_ClearFlag_RXBuffFull(UART1);
  FL_UART_EnableIT_RXBuffFull(UART1);

  /* 清除发送中断标志（发送时才使能）*/
  FL_UART_ClearFlag_TXShiftBuffEmpty(UART1);
}

/**
 * @brief NVIC 中断配置
 * @note  优先级 0x00，最高优先级
 *        设备通信优先级最高，确保响应及时
 */
static void UART1_NVIC_Init(void) {
  FL_NVIC_ConfigTypeDef NVIC_InitStruct;
  NVIC_InitStruct.preemptPriority = 0x00;
  FL_NVIC_Init(&NVIC_InitStruct, UART1_IRQn);
}

/*===========================================================================*/
/*                            发送函数                                        */
/*===========================================================================*/

/**
 * @brief 初始化发送控制（兼容接口）
 */
void Uart1_Tx_Send_init(void) {
  s_tx_ctrl.TxTotalLen = 1;
  s_tx_ctrl.TxSentCount = 1;
  s_tx_complete_flag = 1;
}

/**
 * @brief UART1 发送数据
 * @param data 数据指针
 * @param len  数据长度
 */
void Uart1_Tx_Send(uint8_t data[], uint16_t len) {
  if (len == 0) {
    return;
  }

  /* 1. 等待上一次发送完成 */
  uart1_tx_timeout_counter = UART1_TX_TIMEOUT_MS;
  while (s_tx_complete_flag == 0 && uart1_tx_timeout_counter != 0) {
    /* 等待定时器递减 */
  }

  /* 2. 检查是否超时 */
  if (uart1_tx_timeout_counter == 0) {
    /* 超时了，上一帧可能没发完，强制复位 */
    FL_UART_DisableIT_TXShiftBuffEmpty(UART1);
    s_tx_complete_flag = 1;
    log_w("UART1 TX timeout, force reset");
  }

  /* 3. 设置发送中状态 */
  s_tx_complete_flag = 0;

  /* 4. 复制数据，配置发送参数 */
  memcpy(s_tx_buffer, data, len);
  s_tx_ctrl.TxBuf = s_tx_buffer;
  s_tx_ctrl.TxTotalLen = len;
  s_tx_ctrl.TxSentCount = 1; /* 从1开始，第0字节手动发送 */

  /* 5. 启动中断链 */
  FL_UART_ClearFlag_TXShiftBuffEmpty(UART1);
  FL_UART_EnableIT_TXShiftBuffEmpty(UART1);
  FL_UART_WriteTXBuff(UART1, s_tx_ctrl.TxBuf[0]);
}

/**
 * @brief 带前导码的发送函数（用于唤醒水表）
 * @param data 数据指针
 * @param len  数据长度
 */
void Uart1_Tx_Send_ok(uint8_t data[], uint16_t len) {
  if (uart1_preamble_enabled == 1) {
#ifdef ENABLE_WATCHDOG
    FL_IWDT_ReloadCounter(IWDT);
#endif

    /* 发送32次前导码，每次50字节 */
    for (int i = 0; i < 32; i++) {
      Uart1_Tx_Send((uint8_t *)s_preamble_aa, 50);
      /* 等待发送完成 */
      while (s_tx_complete_flag == 0) {
#ifdef ENABLE_WATCHDOG
        FL_IWDT_ReloadCounter(IWDT);
#endif
      }
      FL_DelayMs(3);
#ifdef ENABLE_WATCHDOG
      FL_IWDT_ReloadCounter(IWDT);
#endif
    }

    /* 发送校准前导码 */
    Uart1_Tx_Send((uint8_t *)s_preamble_calibrate, 10);
    while (s_tx_complete_flag == 0) {
#ifdef ENABLE_WATCHDOG
      FL_IWDT_ReloadCounter(IWDT);
#endif
    }
  }

  /* 发送实际数据 */
  Uart1_Tx_Send(data, len);

  /* 前导码发送完成后，给水表足够的时间唤醒和稳定 */
  FL_DelayMs(100);
#ifdef ENABLE_WATCHDOG
  FL_IWDT_ReloadCounter(IWDT);
#endif
}

/*===========================================================================*/
/*                            接收处理                                        */
/*===========================================================================*/

/**
 * @brief 接收处理（主循环调用）
 */
void Uart1_Rx_rec(void) {
  /* 帧接收完成判断 */
  if (s_rx_data_flag && uart1_rx_frame_timeout == 0) {
    LED_FLAG_Run();

    /* 检查溢出标志 */
    if (s_rx_overflow_flag) {
      log_w("UART1 RX overflow, frame discarded");
      s_rx_overflow_flag = 0;
      s_rx_count = 0;
      s_rx_data_flag = 0;
      return;
    }

    /* 保护临界区：先保存长度再清零，防止竞态 */
    __disable_irq();
    uint16_t rx_len = s_rx_count;
    s_rx_count = 0;
    s_rx_data_flag = 0;
    __enable_irq();

    /* 复制到处理缓冲区 */
    memcpy(s_rx_process_buffer, s_rx_buffer, rx_len);

    /* 同步到公有变量 */
    memcpy(uart1_rx_buffer, s_rx_process_buffer, rx_len);
    uart1_rx_count = rx_len;

    /* 协议解析 */
    ProtocolResult result = Protocol_Device_Parse(s_rx_process_buffer, rx_len);
    (void)result; /* 消除未使用变量警告 */

    /* 清空公有变量 */
    uart1_rx_data_flag = 0;
    uart1_rx_count = 0;
  }
}

/*===========================================================================*/
/*                            中断服务                                        */
/*===========================================================================*/

/**
 * @brief UART1 中断服务函数
 */
void UART1_IRQHandler(void) {
  /* 接收中断 */
  if (FL_UART_IsEnabledIT_RXBuffFull(UART1) &&
      FL_UART_IsActiveFlag_RXBuffFull(UART1)) {
    uint8_t data = FL_UART_ReadRXBuff(UART1);
    s_rx_buffer[s_rx_count++] = data;

    /* 检查溢出 */
    if (s_rx_count >= UART1_BUFFER_SIZE) {
      s_rx_count = 0;
      s_rx_overflow_flag = 1; /* 标记溢出，让上层丢弃此帧 */
    }

    s_rx_data_flag = 1;
    uart1_rx_data_flag = 1;
    uart1_rx_frame_timeout = UART1_RX_FRAME_TIMEOUT_MS;
  }

  /* 发送中断 */
  if (FL_UART_IsEnabledIT_TXShiftBuffEmpty(UART1) &&
      FL_UART_IsActiveFlag_TXShiftBuffEmpty(UART1)) {
    if (s_tx_ctrl.TxSentCount >= s_tx_ctrl.TxTotalLen) {
      /* 发送完成，禁止中断 */
      FL_UART_DisableIT_TXShiftBuffEmpty(UART1);
      s_tx_complete_flag = 1;
    } else {
      /* 继续发送下一个字节 */
      FL_UART_WriteTXBuff(UART1, s_tx_ctrl.TxBuf[s_tx_ctrl.TxSentCount++]);
    }

    FL_UART_ClearFlag_TXShiftBuffEmpty(UART1);
  }
}
