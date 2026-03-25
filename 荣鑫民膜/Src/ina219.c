/**
 * @file ina219.c
 * @brief INA219 电流检测模块驱动
 *
 * 本板有两路 INA219:
 *   - 主电 INA219: I2C via PC8(SCL)/PC9(SDA), 旁路MOS PE3
 *   - NB电 INA219: I2C via PC6(SCL)/PC7(SDA), 旁路MOS PE2
 *
 * 旁路MOS控制逻辑: 低电平=开启(可测电流), 高电平=关闭(正常供电)
 */

#include "ina219.h"
#include "main.h"

#define TRUE 1
#define FALSE 0

// ===== 内部状态 =====
static uint8_t ina219_channel = INA219_CH_MAIN;
static unsigned char ina219_buf[2];
static unsigned char ina219_current_buf[20];
static unsigned short ina219_current_mid_buf[5];

// I2C 软件延时(微秒), 5us ≈ 100kHz 半周期
// 标准模式需 >4us, 快速模式需 >1.3us
#define I2C_SPEED_DELAY_US 5

// ===== 软件I2C底层 =====

// 软件延时，使用硬件驱动提供的精准微秒延时，不受编译器优化和主频影响
static void ina219_iic_delay(void) { FL_DelayUs(I2C_SPEED_DELAY_US); }

// 设置SCL电平
static void ina219_scl(uint8_t level) {
  if (level) {
    switch (ina219_channel) {
    case INA219_CH_MAIN:
      FL_GPIO_SetOutputPin(INA219_MAIN_PORT, INA219_MAIN_SCL_PIN);
      break;
    case INA219_CH_NB:
      FL_GPIO_SetOutputPin(INA219_NB_PORT, INA219_NB_SCL_PIN);
      break;
    default:
      break;
    }
  } else {
    switch (ina219_channel) {
    case INA219_CH_MAIN:
      FL_GPIO_ResetOutputPin(INA219_MAIN_PORT, INA219_MAIN_SCL_PIN);
      break;
    case INA219_CH_NB:
      FL_GPIO_ResetOutputPin(INA219_NB_PORT, INA219_NB_SCL_PIN);
      break;
    default:
      break;
    }
  }
}

// 设置SDA方向 (1=输出, 0=输入)
static void ina219_sda_dir(uint8_t is_output) {
  FL_GPIO_InitTypeDef gpio;
  gpio.outputType = FL_GPIO_OUTPUT_OPENDRAIN;
  gpio.pull = FL_ENABLE;
  gpio.remapPin = FL_DISABLE;
  gpio.analogSwitch = FL_DISABLE;
  gpio.mode = is_output ? FL_GPIO_MODE_OUTPUT : FL_GPIO_MODE_INPUT;

  switch (ina219_channel) {
  case INA219_CH_MAIN:
    gpio.pin = INA219_MAIN_SDA_PIN;
    (void)FL_GPIO_Init(INA219_MAIN_PORT, &gpio);
    break;
  case INA219_CH_NB:
    gpio.pin = INA219_NB_SDA_PIN;
    (void)FL_GPIO_Init(INA219_NB_PORT, &gpio);
    break;
  default:
    break;
  }
}

#define INA219_SDA_DRIVE() ina219_sda_dir(1) // 主机接管SDA
#define INA219_SDA_RELEASE() ina219_sda_dir(0) // 释放SDA，让从机可以驱动

// 设置SDA电平
static void ina219_sda(uint8_t level) {
  if (level) {
    switch (ina219_channel) {
    case INA219_CH_MAIN:
      FL_GPIO_SetOutputPin(INA219_MAIN_PORT, INA219_MAIN_SDA_PIN);
      break;
    case INA219_CH_NB:
      FL_GPIO_SetOutputPin(INA219_NB_PORT, INA219_NB_SDA_PIN);
      break;
    default:
      break;
    }
  } else {
    switch (ina219_channel) {
    case INA219_CH_MAIN:
      FL_GPIO_ResetOutputPin(INA219_MAIN_PORT, INA219_MAIN_SDA_PIN);
      break;
    case INA219_CH_NB:
      FL_GPIO_ResetOutputPin(INA219_NB_PORT, INA219_NB_SDA_PIN);
      break;
    default:
      break;
    }
  }
}

// 读取SDA电平
static uint32_t ina219_sda_read(void) {
  switch (ina219_channel) {
  case INA219_CH_MAIN:
    return FL_GPIO_GetInputPin(INA219_MAIN_PORT, INA219_MAIN_SDA_PIN);
  case INA219_CH_NB:
    return FL_GPIO_GetInputPin(INA219_NB_PORT, INA219_NB_SDA_PIN);
  default:
    return 0;
  }
}

// ===== I2C协议层 =====

// Star:SCL=1 时，SDA 1→0 (下降沿)
static void ina219_iic_start(void) {
  INA219_SDA_DRIVE();
  ina219_sda(1);
  ina219_scl(1);
  ina219_iic_delay();
  ina219_sda(0);
  // start信号已经发出，继续延时确保满足时序要求，需要给一个起始条件的保持时间，也就是tHD;STA，通常至少4.7us，下面的延时大约满足这个要求
  //避免高频时钟信号导致从机无法正确识别起始条件,删除了下面这一行，高频单片机有可能会出问题
  ina219_iic_delay();
  ina219_scl(0);
}

// Stop:SCL=1 时，SDA 0→1 (上升沿)
static void ina219_iic_stop(void) {
  INA219_SDA_DRIVE();
  ina219_sda(0);
  ina219_scl(1);
  ina219_iic_delay();
  ina219_sda(1);
  ina219_scl(0);
  ina219_iic_delay();
  ina219_scl(1);
}

/**
 * @brief  发送一个字节数据，并读取ACK (write addr: 0x80, read addr: 0x81)
 *
 * @param data 要发送的数据字节
 * @return unsigned char
 */
static unsigned char ina219_iic_send_byte(unsigned char data) {
  unsigned char i;
  //使能SDA输出，准备发送数据
  INA219_SDA_DRIVE();
  // 先拉低SCL，准备发送数据
  ina219_scl(0);
  //发送8位数据，每发送一位都要拉高SCL产生时钟信号，然后再拉低SCL准备发送下一位数据
  for (i = 0; i < 8; i++) {
    if (data & 0x80) {
      ina219_sda(1);
    } else {
      ina219_sda(0);
    }
    ina219_iic_delay();
    ina219_scl(1);
    ina219_iic_delay();
    ina219_scl(0);
    data <<= 1;
  }
  // 发送完一个字节后，主机需要释放SDA线，让从机可以驱动SDA线发送ACK信号
  INA219_SDA_RELEASE();
  ina219_iic_delay();
  ina219_scl(1);
  ina219_iic_delay();
  /* 由于INA219 不支持clock
  stretching,所以这部分代码不需要考虑，在手册的第三页，Pin Functions
  部分有说明，SCL线是输入的，INA219不会拉低SCL线来进行clock stretching。
  如果需要用到别的地方，需要增加scl切换成输入模式，并且等待SCL线被拉低的代码，类似下面这样：
   uint32_t waiting_time =1000;
   while (scl_read() == 0 && waiting_time--) {
    //
  如果SCL线被拉低，说明从机正在处理数据，等待一段时间后再继续检查SCL线状态，或者直接等待超时，避免死循环
  }

  */
  // 读取ACK (忽略结果，保持原有行为)
  (void)ina219_sda_read();
  ina219_scl(0);
  ina219_iic_delay();
  return TRUE;
}

static unsigned char ina219_iic_send_bytes(unsigned char *data,
                                           unsigned char len) {
  unsigned char i;
  for (i = 0; i < len; i++) {
    ina219_iic_send_byte(data[i]);
  }
  return TRUE;
}

static unsigned char ina219_iic_recv_byte(void) {
  unsigned char i, data = 0;
  INA219_SDA_RELEASE();
  ina219_scl(0);
  ina219_iic_delay();
  for (i = 0; i < 8; i++) {
    ina219_scl(1);
    ina219_iic_delay();
    if (ina219_sda_read()) {
      data |= 0x80 >> i;
    }
    ina219_scl(0);
    ina219_iic_delay();
  }
  return data;
}

static void ina219_iic_recv_bytes(unsigned char *data, unsigned char len) {
  unsigned char i;
  for (i = 0; i < len; i++) {
    data[i] = ina219_iic_recv_byte();
    INA219_SDA_DRIVE();
    if (i < len - 1) {
      ina219_sda(0); // ACK
    } else {
      ina219_sda(1); // NACK
    }
    ina219_iic_delay();
    ina219_scl(1);
    ina219_iic_delay();
    ina219_scl(0);
  }
}

// ===== INA219寄存器操作 =====

// 读取一次电流原始值 (write addr: 0x80, read addr: 0x81)
static unsigned short ina219_read_raw(void) {
  unsigned short result;

  // 写配置寄存器 (reg 0x00)
  ina219_buf[0] = 0x07;
  ina219_buf[1] = 0x9f;
  ina219_iic_start();
  ina219_iic_send_byte(INA219_I2C_ADDR_WRITE);
  ina219_iic_send_byte(0);
  ina219_iic_send_bytes(ina219_buf, 2);
  ina219_iic_stop();

  // 写校准寄存器 (reg 0x05)
  ina219_buf[0] = 0x10;
  ina219_buf[1] = 0x00;
  ina219_iic_start();
  ina219_iic_send_byte(INA219_I2C_ADDR_WRITE);
  ina219_iic_send_byte(5);
  ina219_iic_send_bytes(ina219_buf, 2);
  ina219_iic_stop();

  FL_DelayMs(20);

  // 读电流寄存器 (reg 0x04)
  ina219_iic_start();
  ina219_iic_send_byte(INA219_I2C_ADDR_WRITE);
  ina219_iic_send_byte(4);
  ina219_iic_start();
  ina219_iic_send_byte(INA219_I2C_ADDR_READ);
  ina219_iic_recv_bytes(ina219_current_buf, 2);
  ina219_iic_stop();

  result = (unsigned short)ina219_current_buf[0] * 0x100;
  result += ina219_current_buf[1];
  return result;
}

// 多次采样取平均值
static uint16_t ina219_read_avg(void) {
  uint8_t i;
  uint16_t min_current;
  uint16_t samples[3];

  // 第一次读取丢弃 (可能不准)
  ina219_read_raw();
  FL_DelayMs(10);

  for (i = 0; i < 3; i++) {
    FL_DelayMs(10);
    samples[i] = ina219_read_raw();
  }

  min_current = 0;
  for (i = 0; i < 3; i++) {
    if (samples[i] >> 15 == 0x01) {
      samples[i] = 0xFFFF - samples[i] + 1;
      min_current -= samples[i];
    } else {
      min_current += samples[i];
    }
  }

  if (min_current >> 15 == 0x01) {
    min_current = 0xFFFF - min_current + 1;
    min_current = min_current / 3;
    min_current = 0xFFFF - min_current + 1;
  } else {
    min_current = min_current / 3;
  }

  return min_current;
}

// ===== 对外接口 =====

/**
 * @brief 初始化两路INA219的I2C引脚和旁路MOS控制引脚
 */
void ina219_gpio_init(void) {
  FL_GPIO_InitTypeDef gpio;

  // --- I2C引脚初始化 (开漏输出) ---
  gpio.mode = FL_GPIO_MODE_OUTPUT;
  gpio.outputType = FL_GPIO_OUTPUT_OPENDRAIN;
  gpio.pull = FL_DISABLE;
  gpio.remapPin = FL_DISABLE;
  gpio.analogSwitch = FL_DISABLE;

  // 主电 SCL PC8
  gpio.pin = INA219_MAIN_SCL_PIN;
  FL_GPIO_Init(INA219_MAIN_PORT, &gpio);
  // 主电 SDA PC9
  gpio.pin = INA219_MAIN_SDA_PIN;
  FL_GPIO_Init(INA219_MAIN_PORT, &gpio);
  // NB电 SCL PC6
  gpio.pin = INA219_NB_SCL_PIN;
  FL_GPIO_Init(INA219_NB_PORT, &gpio);
  // NB电 SDA PC7
  gpio.pin = INA219_NB_SDA_PIN;
  FL_GPIO_Init(INA219_NB_PORT, &gpio);

  // I2C总线空闲状态: SDA拉高
  FL_GPIO_SetOutputPin(INA219_MAIN_PORT, INA219_MAIN_SDA_PIN);
  FL_GPIO_SetOutputPin(INA219_NB_PORT, INA219_NB_SDA_PIN);

  // --- 旁路MOS控制引脚初始化 (推挽输出, 默认高电平=关闭旁路) ---
  gpio.mode = FL_GPIO_MODE_OUTPUT;
  gpio.outputType = FL_GPIO_OUTPUT_PUSHPULL;
  gpio.pull = FL_DISABLE;
  gpio.remapPin = FL_DISABLE;
  gpio.analogSwitch = FL_DISABLE;

  // 主电旁路MOS PE3
  gpio.pin = INA219_MAIN_BYPASS_PIN;
  FL_GPIO_Init(INA219_MAIN_BYPASS_PORT, &gpio);
  FL_GPIO_SetOutputPin(INA219_MAIN_BYPASS_PORT, INA219_MAIN_BYPASS_PIN);

  // NB电旁路MOS PE2
  gpio.pin = INA219_NB_BYPASS_PIN;
  FL_GPIO_Init(INA219_NB_BYPASS_PORT, &gpio);
  FL_GPIO_SetOutputPin(INA219_NB_BYPASS_PORT, INA219_NB_BYPASS_PIN);
}

/**
 * @brief 开启旁路MOS (低电平=开启, 可以测量电流)
 * @param channel INA219_CH_MAIN 或 INA219_CH_NB
 */
void ina219_bypass_on(uint8_t channel) {
  switch (channel) {
  case INA219_CH_MAIN:
    FL_GPIO_ResetOutputPin(INA219_MAIN_BYPASS_PORT, INA219_MAIN_BYPASS_PIN);
    break;
  case INA219_CH_NB:
    FL_GPIO_ResetOutputPin(INA219_NB_BYPASS_PORT, INA219_NB_BYPASS_PIN);
    break;
  default:
    break;
  }
}

/**
 * @brief 关闭旁路MOS (高电平=关闭, 正常供电)
 * @param channel INA219_CH_MAIN 或 INA219_CH_NB
 *
 * 注: 原代码 NB_Current_CHK_CTRL_OFF 存在 bug，使用了 ResetOutputPin (低电平),
 *     导致旁路MOS无法关闭。此处已修复为 SetOutputPin (高电平)。
 */
void ina219_bypass_off(uint8_t channel) {
  switch (channel) {
  case INA219_CH_MAIN:
    FL_GPIO_SetOutputPin(INA219_MAIN_BYPASS_PORT, INA219_MAIN_BYPASS_PIN);
    break;
  case INA219_CH_NB:
    FL_GPIO_SetOutputPin(INA219_NB_BYPASS_PORT, INA219_NB_BYPASS_PIN);
    break;
  default:
    break;
  }
}

/**
 * @brief 读取指定通道的电流值
 * @param channel INA219_CH_MAIN 或 INA219_CH_NB
 * @return 电流值 (原始单位)
 */
uint16_t ina219_read_current(uint8_t channel) {
  uint16_t current_raw;

  ina219_channel = channel;
  ina219_bypass_on(channel);
  current_raw = ina219_read_avg();
  ina219_bypass_off(channel);

  if (current_raw > 35)
    current_raw -= 35;

  return current_raw;
}

// ===== 向后兼容 =====
// 以下函数保持旧接口，供未迁移的代码调用

void INA219_IIC_GPIO_Init(void) { ina219_gpio_init(); }

void Current_CHK_CTRL_ON(void) { ina219_bypass_on(INA219_CH_MAIN); }

void Current_CHK_CTRL_OFF(void) { ina219_bypass_off(INA219_CH_MAIN); }

void NB_Current_CHK_CTRL_ON(void) { ina219_bypass_on(INA219_CH_NB); }

void NB_Current_CHK_CTRL_OFF(void) { ina219_bypass_off(INA219_CH_NB); }

uint16_t Current_CHK_Func(uint8_t channel) {
  return ina219_read_current(channel);
}
