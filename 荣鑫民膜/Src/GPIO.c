#include "GPIO.h"
#include "ina219.h"
uint8_t daowei1_status = 0;
uint8_t daowei2_status = 0;
void Others_GPIO_Init() {
  // LED指示灯 PA3
  FL_GPIO_InitTypeDef GPIO_InitStruct;
  GPIO_InitStruct.pin = FL_GPIO_PIN_3;
  GPIO_InitStruct.mode = FL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.pull = FL_DISABLE;
  GPIO_InitStruct.remapPin = FL_DISABLE;
  GPIO_InitStruct.analogSwitch = FL_DISABLE;
  FL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  // 主电源供电引脚 PB10
  GPIO_InitStruct.pin = FL_GPIO_PIN_10;
  GPIO_InitStruct.mode = FL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.pull = FL_DISABLE;
  GPIO_InitStruct.remapPin = FL_DISABLE;
  GPIO_InitStruct.analogSwitch = FL_DISABLE;
  FL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  // 备电供电引脚PB9
  GPIO_InitStruct.pin = FL_GPIO_PIN_9;
  GPIO_InitStruct.mode = FL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.pull = FL_DISABLE;
  GPIO_InitStruct.remapPin = FL_DISABLE;
  GPIO_InitStruct.analogSwitch = FL_DISABLE;
  FL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  // 主电电压检测控制 PD0
  GPIO_InitStruct.pin = FL_GPIO_PIN_0;
  GPIO_InitStruct.mode = FL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.pull = FL_DISABLE;
  GPIO_InitStruct.remapPin = FL_DISABLE;
  GPIO_InitStruct.analogSwitch = FL_DISABLE;
  FL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  // 备电 3.6V电压检测控制 PD1
  GPIO_InitStruct.pin = FL_GPIO_PIN_1;
  GPIO_InitStruct.mode = FL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.pull = FL_DISABLE;
  GPIO_InitStruct.remapPin = FL_DISABLE;
  GPIO_InitStruct.analogSwitch = FL_DISABLE;
  FL_GPIO_Init(GPIOD, &GPIO_InitStruct);
   // 12V供电控制  PB11
  GPIO_InitStruct.pin = FL_GPIO_PIN_11;
  GPIO_InitStruct.mode = FL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.pull = FL_DISABLE;
  GPIO_InitStruct.remapPin = FL_DISABLE;
  GPIO_InitStruct.analogSwitch = FL_DISABLE;
  FL_GPIO_Init(GPIOB, &GPIO_InitStruct);
  // 12v霍尔1控制 PC14
  GPIO_InitStruct.pin = FL_GPIO_PIN_14;
  GPIO_InitStruct.mode = FL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.pull = FL_DISABLE;
  GPIO_InitStruct.remapPin = FL_DISABLE;
  GPIO_InitStruct.analogSwitch = FL_DISABLE;
  FL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  // 12v霍尔2控制 PC15
  GPIO_InitStruct.pin = FL_GPIO_PIN_15;
  GPIO_InitStruct.mode = FL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.pull = FL_DISABLE;
  GPIO_InitStruct.remapPin = FL_DISABLE;
  GPIO_InitStruct.analogSwitch = FL_DISABLE;
  FL_GPIO_Init(GPIOC, &GPIO_InitStruct);

//   // 119电路 PB12
//   GPIO_InitStruct.pin = FL_GPIO_PIN_12;
//   GPIO_InitStruct.mode = FL_GPIO_MODE_OUTPUT;
//   GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
//   GPIO_InitStruct.pull = FL_DISABLE;
//   GPIO_InitStruct.remapPin = FL_DISABLE;
//   GPIO_InitStruct.analogSwitch = FL_DISABLE;
//   FL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  // 119报警触发 PE1 (低电平触发)
  GPIO_InitStruct.pin = FL_GPIO_PIN_1;
  GPIO_InitStruct.mode = FL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.pull = FL_DISABLE;
  GPIO_InitStruct.remapPin = FL_DISABLE;
  GPIO_InitStruct.analogSwitch = FL_DISABLE;
  FL_GPIO_Init(GPIOE, &GPIO_InitStruct);
  FL_GPIO_SetOutputPin(GPIOE, FL_GPIO_PIN_1); // 默认高电平(未触发状态)

  // 到位信号1 PC10 (开到位信号) - 工装输出给气表
  // 默认输入模式(高阻态)，让气表侧的上拉电阻保持高电平
  GPIO_InitStruct.pin = FL_GPIO_PIN_3;
  GPIO_InitStruct.mode = FL_GPIO_MODE_INPUT;
  GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.pull = FL_DISABLE;
  GPIO_InitStruct.remapPin = FL_DISABLE;
  GPIO_InitStruct.analogSwitch = FL_DISABLE;
  FL_GPIO_Init(GPIOE, &GPIO_InitStruct);

//   // 到位信号2 PA11 (关到位信号) - 工装输出给气表
//   // 默认输入模式(高阻态)，让气表侧的上拉电阻保持高电平
//   GPIO_InitStruct.pin = FL_GPIO_PIN_11;
//   GPIO_InitStruct.mode = FL_GPIO_MODE_INPUT;
//   GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
//   GPIO_InitStruct.pull = FL_DISABLE;
//   GPIO_InitStruct.remapPin = FL_DISABLE;
//   GPIO_InitStruct.analogSwitch = FL_DISABLE;
//   FL_GPIO_Init(GPIOA, &GPIO_InitStruct);

//   // 关闭电解电容控制 PA2
//   GPIO_InitStruct.pin = FL_GPIO_PIN_2;
//   GPIO_InitStruct.mode = FL_GPIO_MODE_OUTPUT;
//   GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
//   GPIO_InitStruct.pull = FL_DISABLE;
//   GPIO_InitStruct.remapPin = FL_DISABLE;
//   GPIO_InitStruct.analogSwitch = FL_DISABLE;
//   FL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  // 工位1检测初始化端口PB8
  GPIO_InitStruct.pin = FL_GPIO_PIN_8;
  GPIO_InitStruct.mode = FL_GPIO_MODE_INPUT;
  GPIO_InitStruct.pull = FL_ENABLE;
  GPIO_InitStruct.remapPin = FL_DISABLE;
  GPIO_InitStruct.analogSwitch = FL_DISABLE;
  FL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  // 工位2检测初始化端口PB7
  GPIO_InitStruct.pin = FL_GPIO_PIN_7;
  GPIO_InitStruct.mode = FL_GPIO_MODE_INPUT;
  GPIO_InitStruct.pull = FL_ENABLE;
  GPIO_InitStruct.remapPin = FL_DISABLE;
  GPIO_InitStruct.analogSwitch = FL_DISABLE;
  FL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  // 工位3检测初始化端口PB6
  GPIO_InitStruct.pin = FL_GPIO_PIN_6;
  GPIO_InitStruct.mode = FL_GPIO_MODE_INPUT;
  GPIO_InitStruct.pull = FL_ENABLE;
  GPIO_InitStruct.remapPin = FL_DISABLE;
  GPIO_InitStruct.analogSwitch = FL_DISABLE;
  FL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  // 工位4检测初始化端口PB5
  GPIO_InitStruct.pin = FL_GPIO_PIN_5;
  GPIO_InitStruct.mode = FL_GPIO_MODE_INPUT;
  GPIO_InitStruct.pull = FL_ENABLE;
  GPIO_InitStruct.remapPin = FL_DISABLE;
  GPIO_InitStruct.analogSwitch = FL_DISABLE;
  FL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  // 上电先关灯
  LED_Off();
  // 主电源供电断开
  zhudian_gongdian_OFF();
  // 备电电源断开
  beidian_gongdian_OFF();
  // 主电电压检测控制关闭
  zhudian_dianya_CHK_CTRL_OFF();
  // 二级电源电压检测控制关闭
  erji_dianya_CHK_CTRL_OFF();
  // 霍尔1控制 关
  huuoer_1_OFF();
  // 霍尔2控制 关
  huuoer_2_OFF();
  // 到位信号高
  daowei_1_OFF();
  // 到位信号低
//   daowei_2_OFF();
  // 功耗检测的IIC和旁路MOS初始化
  ina219_gpio_init();
}
// LED开
void LED_On() {
  // uint8_t buf[2] = {0,1};
  FL_GPIO_ResetOutputPin(GPIOA, FL_GPIO_PIN_3);
}
// LED关
void LED_Off() { FL_GPIO_SetOutputPin(GPIOA, FL_GPIO_PIN_3); }
// 主控板主电供电开
void zhudian_gongdian_On() { FL_GPIO_SetOutputPin(GPIOB, FL_GPIO_PIN_10); }
// 主控板主电供电关
void zhudian_gongdian_OFF() { FL_GPIO_ResetOutputPin(GPIOB, FL_GPIO_PIN_10); }
// 主控板备电电源供电开
void beidian_gongdian_On() { FL_GPIO_SetOutputPin(GPIOB, FL_GPIO_PIN_9); }
// 主控板备电电源供电关
void beidian_gongdian_OFF() { FL_GPIO_ResetOutputPin(GPIOB, FL_GPIO_PIN_9); }

// 主控板主电电压检测使能开
void zhudian_dianya_CHK_CTRL_ON() {
  FL_GPIO_SetOutputPin(GPIOD, FL_GPIO_PIN_0);
}
// 主控板主电电压检测使能关
void zhudian_dianya_CHK_CTRL_OFF() {
  FL_GPIO_ResetOutputPin(GPIOD, FL_GPIO_PIN_0);
}
// 主控板备电电压检测使能开
void erji_dianya_CHK_CTRL_ON() { FL_GPIO_SetOutputPin(GPIOD, FL_GPIO_PIN_1); }
// 主控板备电电压检测使能关
void erji_dianya_CHK_CTRL_OFF() {
  FL_GPIO_ResetOutputPin(GPIOD, FL_GPIO_PIN_1);
}
// 12V供电控制开
void out12V_ON() { FL_GPIO_SetOutputPin(GPIOB, FL_GPIO_PIN_11); }
// 12V供电控制关
void out12V_OFF() { FL_GPIO_ResetOutputPin(GPIOB, FL_GPIO_PIN_11); }
// 霍尔1控制 开
void huuoer_1_ON() { FL_GPIO_SetOutputPin(GPIOC, FL_GPIO_PIN_14); }
// 霍尔1控制 关
void huuoer_1_OFF() { FL_GPIO_ResetOutputPin(GPIOC, FL_GPIO_PIN_14); }
// 霍尔2控制 开
void huuoer_2_ON() { FL_GPIO_SetOutputPin(GPIOC, FL_GPIO_PIN_15); }
// 霍尔2控制 关
void huuoer_2_OFF() { FL_GPIO_ResetOutputPin(GPIOC, FL_GPIO_PIN_15); }
// 到位1控制,输出低电平 (PA12 - 开到位信号) - 模拟到位触发
void daowei_1_ON() {
  // 配置成推挽输出
  FL_GPIO_SetPinMode(GPIOE, FL_GPIO_PIN_3, FL_GPIO_MODE_OUTPUT);
  // 输出低电平 - 模拟到位信号触发
  FL_GPIO_ResetOutputPin(GPIOE, FL_GPIO_PIN_3);
  daowei1_status = 1;
}
// 到位1控制,释放 (PA12 - 开到位信号) - 正常状态(高阻态)
void daowei_1_OFF() {
  // 配置成输入模式(高阻态)，让对方的上拉电阻拉高
  FL_GPIO_SetPinMode(GPIOE, FL_GPIO_PIN_3, FL_GPIO_MODE_INPUT);
  daowei1_status = 0;
}
// 到位2控制,输出低电平 (PA11 - 关到位信号) - 模拟到位触发
void daowei_2_ON() {
  // 配置成推挽输出
  FL_GPIO_SetPinMode(GPIOA, FL_GPIO_PIN_11, FL_GPIO_MODE_OUTPUT);
  // 输出低电平 - 模拟到位信号触发
  FL_GPIO_ResetOutputPin(GPIOA, FL_GPIO_PIN_11);
  daowei2_status = 1;
}
// 到位2控制,释放 (PA11 - 关到位信号) - 正常状态(高阻态)
void daowei_2_OFF() {
  // 配置成输入模式(高阻态)，让对方的上拉电阻拉高
  FL_GPIO_SetPinMode(GPIOA, FL_GPIO_PIN_11, FL_GPIO_MODE_INPUT);
  daowei2_status = 0;
}
void GPIO_IRQHandler(void) {
  if (FL_GPIO_IsActiveFlag_EXTI(GPIO, FL_GPIO_EXTI_LINE_2) == 0x01UL) {
    FL_GPIO_ClearFlag_EXTI(GPIO, FL_GPIO_EXTI_LINE_2);
  }
}

// 工位检测函数 - 读取各个引脚状态
uint8_t read_gongwei_1(void) {
  return FL_GPIO_GetInputPin(GPIOB, FL_GPIO_PIN_8) ? 1 : 0;
}

uint8_t read_gongwei_2(void) {
  return FL_GPIO_GetInputPin(GPIOB, FL_GPIO_PIN_7) ? 1 : 0;
}

uint8_t read_gongwei_3(void) {
  return FL_GPIO_GetInputPin(GPIOB, FL_GPIO_PIN_6) ? 1 : 0;
}

uint8_t read_gongwei_4(void) {
  return FL_GPIO_GetInputPin(GPIOB, FL_GPIO_PIN_5) ? 1 : 0;
}

// 获取工位编号 - 根据4个引脚组合计算工位
uint8_t get_gongwei_number(void) {
  uint8_t gongwei1 = read_gongwei_1();
  uint8_t gongwei2 = read_gongwei_2();
  uint8_t gongwei3 = read_gongwei_3();
  uint8_t gongwei4 = read_gongwei_4();

  // 跳线帽插上时引脚为低电平，所以需要反转逻辑
  // PC0对应丝印1，PD12对应丝印2，PB15对应丝印3，PE5对应丝印4
  uint8_t jumper1 = !gongwei1;  // 丝印1跳线帽状态 (1=插上, 0=未插)
  uint8_t jumper2 = !gongwei2; // 丝印2跳线帽状态 (1=插上, 0=未插)
  uint8_t jumper3 = !gongwei3; // 丝印3跳线帽状态 (1=插上, 0=未插)
  uint8_t jumper4 = !gongwei4;  // 丝印4跳线帽状态 (1=插上, 0=未插)

  // 将4个跳线帽状态组合成4位二进制数 (jumper4:jumper3:jumper2:jumper1)
  // 返回0-15的工位编号
  return (jumper4 << 3) | (jumper3 << 2) | (jumper2 << 1) | jumper1;
}
