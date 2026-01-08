#include "uart0.h"
#include "LED_CTRL.h"
#include "PC_xieyi_Ctrl.h"
#include "main.h"
#include "stdarg.h"
#include "time.h"
#include "tongxin_xieyi_Ctrl.h"
#include "uart3.h"

#define lenth_Receive_Send_MAX 200

uint8_t uart0_Rec_shuju_flag = 0;
uint8_t uart0_Rec_shuju_neirong[lenth_Receive_Send_MAX];
uint16_t uart0_Rec_shuju_neirong_count = 0;
uint8_t uart0_Uart0_Tx_SendData[lenth_Receive_Send_MAX];
uint8_t send_data_zancun_0[512]; // 扩大到512字节以支持大协议帧格式化输出
uint8_t send_over_flag0 = 1;
// 这一步需要注册到time时钟里
uint16_t chaoshi_dengdai_0 = 0;
uint16_t uart0_Rec_shuju_time_count = 0;

void UART_TX_state_change(uint8_t send_state) {
  FL_GPIO_InitTypeDef GPIO_InitStruct;
  // 空闲态
  if (send_state == 0) {
    GPIO_InitStruct.mode = FL_GPIO_MODE_INPUT;
  } else {
    GPIO_InitStruct.mode = FL_GPIO_MODE_DIGITAL;
  }
  GPIO_InitStruct.pin = FL_GPIO_PIN_14;
  GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.pull = FL_ENABLE;
  GPIO_InitStruct.remapPin = FL_DISABLE;
  GPIO_InitStruct.analogSwitch = FL_DISABLE;
  (void)FL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

struct UARTOpStruct UART0Op;

void UART0_IRQHandler(void) {
  uint32_t UART0RXBuffFullIT = 0;
  uint32_t UART0RXBuffFullFlag = 0;
  uint32_t UART0TXBuffFullIT = 0;
  uint32_t UART0TXBuffFullFlag = 0;

  UART0RXBuffFullIT = FL_UART_IsEnabledIT_RXBuffFull(UART0);
  UART0RXBuffFullFlag = FL_UART_IsActiveFlag_RXBuffFull(UART0);
  UART0TXBuffFullIT = FL_UART_IsEnabledIT_TXShiftBuffEmpty(UART0);
  UART0TXBuffFullFlag = FL_UART_IsActiveFlag_TXShiftBuffEmpty(UART0);

  // 接收中断处理
  if ((UART0RXBuffFullIT == 0x01UL) && (UART0RXBuffFullFlag == 0x01UL)) {
    // 中断接收
    uart0_Rec_shuju_neirong[uart0_Rec_shuju_neirong_count] =
        FL_UART_ReadRXBuff(UART0); // 接收中断标志可通过读取rxreg寄存器清除
    uart0_Rec_shuju_neirong_count++;
    if (uart0_Rec_shuju_neirong_count >= lenth_Receive_Send_MAX) {
      uart0_Rec_shuju_neirong_count = 0;
    }
    uart0_Rec_shuju_flag = 1;
    uart0_Rec_shuju_time_count = 10; // 10ms 超时计数，确保一帧完整接收
  }

  // 发送中断处理
  if ((UART0TXBuffFullIT == 0x01UL) && (UART0TXBuffFullFlag == 0x01UL)) {
    if (UART0Op.TxOpc == UART0Op.TxLen) {
      FL_UART_DisableIT_TXShiftBuffEmpty(UART0);
      send_over_flag0 = 1;
    }
    // 发送中断标志可通过写txreg寄存器清除或txif写1清除
    // 发送指定长度的数据
    if (UART0Op.TxOpc < UART0Op.TxLen) {
      FL_UART_WriteTXBuff(UART0, UART0Op.TxBuf[UART0Op.TxOpc]); // 发送一个数据
      UART0Op.TxOpc++;
    }

    FL_UART_ClearFlag_TXShiftBuffEmpty(UART0); // 清除发送中断标志
  }
}
void Uart0_Tx_Send_init() {
  UART0Op.TxLen = 1;
  UART0Op.TxOpc = 1;
}

void Uart0_Tx_Send(uint8_t zufuchua[], uint16_t lenth) {
  if (lenth == 0) {
    return;
  }

  chaoshi_dengdai_0 = 100;
  while (UART0Op.TxLen != UART0Op.TxOpc && chaoshi_dengdai_0 != 0) {
  }
  FL_DelayMs(5);
  send_over_flag0 = 0;
  UART_TX_state_change(1);
  memcpy(send_data_zancun_0, zufuchua, lenth);
  // 中断发送数组
  UART0Op.TxBuf = send_data_zancun_0;
  UART0Op.TxLen = lenth;
  UART0Op.TxOpc = 0 + 1;
  FL_UART_ClearFlag_TXShiftBuffEmpty(UART0);
  FL_UART_EnableIT_TXShiftBuffEmpty(UART0);
  FL_UART_WriteTXBuff(UART0, UART0Op.TxBuf[0]);
}

void MF_UART0_Init(void) {
  FL_GPIO_InitTypeDef GPIO_InitStruct;

  FL_UART_InitTypeDef UART0_InitStruct;

  GPIO_InitStruct.pin = FL_GPIO_PIN_13;
  GPIO_InitStruct.mode = FL_GPIO_MODE_DIGITAL;
  GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.pull = FL_ENABLE;
  GPIO_InitStruct.remapPin = FL_DISABLE;
  GPIO_InitStruct.analogSwitch = FL_DISABLE;
  (void)FL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  GPIO_InitStruct.pin = FL_GPIO_PIN_14;
  GPIO_InitStruct.mode = FL_GPIO_MODE_DIGITAL;
  GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.pull = FL_DISABLE;
  GPIO_InitStruct.remapPin = FL_DISABLE;
  GPIO_InitStruct.analogSwitch = FL_DISABLE;
  (void)FL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  UART0_InitStruct.clockSrc = FL_CMU_UART0_CLK_SOURCE_APBCLK;
  UART0_InitStruct.baudRate = 9600;
  UART0_InitStruct.transferDirection = FL_UART_DIRECTION_TX_RX;
  UART0_InitStruct.dataWidth = FL_UART_DATA_WIDTH_8B;
  UART0_InitStruct.stopBits = FL_UART_STOP_BIT_WIDTH_1B;
  UART0_InitStruct.parity = FL_UART_PARITY_NONE;

  (void)FL_UART_Init(UART0, &UART0_InitStruct);
}

void MF_UART0_Interrupt_Init(void) {
  FL_UART_ClearFlag_RXBuffFull(UART0);
  FL_UART_EnableIT_RXBuffFull(UART0);

  FL_UART_ClearFlag_TXShiftBuffEmpty(UART0);
  FL_UART_EnableIT_TXShiftBuffEmpty(UART0);
}
void UART0_MF_NVIC_Init(void) {
  FL_NVIC_ConfigTypeDef InterruptConfigStruct;

  InterruptConfigStruct.preemptPriority = 0x02;
  FL_NVIC_Init(&InterruptConfigStruct, UART0_IRQn);
}

void UART0_MF_Config_Init(void) {
  /* Initial UART0 */
  MF_UART0_Init();
  MF_UART0_Interrupt_Init();

  /* Initial NVIC */
  UART0_MF_NVIC_Init();
  Uart0_Tx_Send_init();
}
void Uart0_Rx_rec() {
  if (send_over_flag0 == 1) {
    send_over_flag0 = 0;
    FL_DelayMs(5);
    UART_TX_state_change(0);
  }
  if (uart0_Rec_shuju_flag == 1 && uart0_Rec_shuju_time_count == 0) {
    LED_FLAG_Run();
    uart0_Rec_shuju_flag = 0;
    // 对返回数据进行解析
    memcpy(uart0_Uart0_Tx_SendData, uart0_Rec_shuju_neirong,
           uart0_Rec_shuju_neirong_count);
    // 注释掉回显功能，避免二进制数据乱码
    // 如需调试 PC 发送的原始协议，可使用 protocol_debug_print()
    // PC_Chuankou_tongxin_Debug_send(uart0_Uart0_Tx_SendData,
    //                                uart0_Rec_shuju_neirong_count);
    PC_xieyijiexi(uart0_Uart0_Tx_SendData, uart0_Rec_shuju_neirong_count);
    uart0_Rec_shuju_neirong_count = 0;
  }
}

void DeBug_print(const char fmt[], ...) {
  unsigned char
      DebugBuf[512]; // 扩大到512字节以支持大协议帧的格式化输出(109字节→350字符)
  va_list args;
  int res;
  if (Debug_Mode == 0)
    return;
  va_start(args, fmt);
  res = vsnprintf((char *)DebugBuf, sizeof(DebugBuf), fmt, args);
  va_end(args);
  // vsnprintf()会自动在格式化之后的字符串的末尾增加'\0'，但res不包含这个字符，所以DebugBuf[N]最多能放N-1个字符，多余的字符将被截断
  // res >=
  // sizeof(DebugBuf)的情况下、会有一部分输出信息丢失，以后可以考虑在这里给出buffer不足的提示
  if (res > 0) {
    Uart0_Tx_Send(DebugBuf, res);
  }
}
void PC_Chuankou_tongxin_Debug_send(uint8_t zufuchua[], uint16_t lenth) {
  if (Debug_Mode == 0)
    return;
  Uart0_Tx_Send(zufuchua, lenth);
}
void PC_Chuankou_tongxin_send(uint8_t zufuchua[], uint16_t lenth) {
  Uart0_Tx_Send(zufuchua, lenth);
}

// 将一个字节的hex转换成两个字节的ascii
void hex2ascii(uint8_t hex, char *ascii_result) {
  uint8_t hex_low = hex & 0x0F;
  uint8_t hex_high = (hex >> 4) & 0x0F;

  // 处理高4位
  if (hex_high <= 9) {
    ascii_result[0] = hex_high + '0';
  } else {
    ascii_result[0] = hex_high - 10 + 'A';
  }

  // 处理低4位
  if (hex_low <= 9) {
    ascii_result[1] = hex_low + '0';
  } else {
    ascii_result[1] = hex_low - 10 + 'A';
  }
}

void protocol_debug_print(uint8_t protocol[], uint16_t length) {
  if (Debug_Mode == 0) {
    return;
  }
  uint16_t i;
  char buffer[1024]; // 缓冲区扩大到1024字节：可打印约320字节的协议数据
  char ascii_chars[3]; // 2个字符 + 结束符\0 ,比如"AF\0"
  int offset;

  // 先写入协议头部信息
  offset = snprintf(buffer, sizeof(buffer), "协议数据 (%d 字节): ", length);

  // 逐字节转换并追加到缓冲区
  for (i = 0; i < length && offset < sizeof(buffer) - 4; i++) {
    hex2ascii(protocol[i], ascii_chars);
    ascii_chars[2] = '\0'; // 添加字符串结束符

    // 追加十六进制字符
    offset +=
        snprintf(buffer + offset, sizeof(buffer) - offset, "%s", ascii_chars);

    // 在字节之间添加空格（最后一个字节不加）
    if (i < length - 1 && offset < sizeof(buffer) - 2) {
      buffer[offset++] = ' ';
      buffer[offset] = '\0';
    }
  }

  // 添加换行符
  if (offset < sizeof(buffer) - 2) {
    buffer[offset++] = '\r';
    buffer[offset++] = '\n';
    buffer[offset] = '\0';
  }

  // 对于大数据包添加调试信息
  if (length > 50) {
    char debug[50];
    int debug_len =
        snprintf(debug, sizeof(debug), "[fmt=%d,loop=%d] ", offset, i);
    Uart0_Tx_Send((uint8_t *)debug, debug_len);
    FL_DelayMs(5);
  }

  // 一次性发送整个字符串（显著减少UART调用次数）
  // 对于大数据帧，直接发送避免vsnprintf的处理
  if (offset > 200) {
    // 直接发送，绕过DeBug_print的vsnprintf处理
    Uart0_Tx_Send((uint8_t *)buffer, offset);
  } else {
    DeBug_print("%s", buffer);
  }
}
