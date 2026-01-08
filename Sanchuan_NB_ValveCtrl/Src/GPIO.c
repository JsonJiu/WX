#include "GPIO.h"
#include "ZDINA219.h"
#include "uart0.h"
uint8_t valve_open_status = FL_GPIO_MODE_OUTPUT; // 阀门到位信号状态变量
uint8_t valve_close_status = FL_GPIO_MODE_OUTPUT; // 阀门到位信号状态变量
void Others_GPIO_Init() {
  FL_GPIO_InitTypeDef GPIO_InitStruct = {0}; // 初始化结构体，避免垃圾值
  // PA11 Open阀门到位信号,输出模式，给水表提供阀门到位信号，低电平有效
  GPIO_InitStruct.pin = FL_GPIO_PIN_11;
  GPIO_InitStruct.mode = FL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.pull = FL_DISABLE;
  GPIO_InitStruct.remapPin = FL_DISABLE;
  GPIO_InitStruct.analogSwitch = FL_DISABLE;
  FL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  valve_open_status = FL_GPIO_MODE_OUTPUT;

  // PA12 Close阀门到位信号,输出模式，给水表提供阀门到位信号,低电平有效
  GPIO_InitStruct.pin = FL_GPIO_PIN_12;
  GPIO_InitStruct.mode = FL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.pull = FL_DISABLE;
  GPIO_InitStruct.remapPin = FL_DISABLE;
  GPIO_InitStruct.analogSwitch = FL_DISABLE;
  FL_GPIO_Init(GPIOA, &GPIO_InitStruct);
  valve_close_status = FL_GPIO_MODE_OUTPUT;

  // LED指示灯 PB4
  GPIO_InitStruct.pin = FL_GPIO_PIN_4;
  GPIO_InitStruct.mode = FL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.pull = FL_DISABLE;
  GPIO_InitStruct.remapPin = FL_DISABLE;
  GPIO_InitStruct.analogSwitch = FL_DISABLE;
  FL_GPIO_Init(GPIOB, &GPIO_InitStruct);
  // 主电源供电引脚 PC5
  GPIO_InitStruct.pin = FL_GPIO_PIN_5;
  GPIO_InitStruct.mode = FL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.pull = FL_DISABLE;
  GPIO_InitStruct.remapPin = FL_DISABLE;
  GPIO_InitStruct.analogSwitch = FL_DISABLE;
  FL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  // NB电源引脚PC4
  GPIO_InitStruct.pin = FL_GPIO_PIN_4;
  GPIO_InitStruct.mode = FL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.pull = FL_DISABLE;
  GPIO_InitStruct.remapPin = FL_DISABLE;
  GPIO_InitStruct.analogSwitch = FL_DISABLE;
  FL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  // 主电电压检测控制 PE0
  GPIO_InitStruct.pin = FL_GPIO_PIN_0;
  GPIO_InitStruct.mode = FL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.pull = FL_DISABLE;
  GPIO_InitStruct.remapPin = FL_DISABLE;
  GPIO_InitStruct.analogSwitch = FL_DISABLE;
  FL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  // NB电压检测控制 PE1
  GPIO_InitStruct.pin = FL_GPIO_PIN_1;
  GPIO_InitStruct.mode = FL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.pull = FL_DISABLE;
  GPIO_InitStruct.remapPin = FL_DISABLE;
  GPIO_InitStruct.analogSwitch = FL_DISABLE;
  FL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  // 主电电流检测使能 PE3
  GPIO_InitStruct.pin = FL_GPIO_PIN_3;
  GPIO_InitStruct.mode = FL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.pull = FL_DISABLE;
  GPIO_InitStruct.remapPin = FL_DISABLE;
  GPIO_InitStruct.analogSwitch = FL_DISABLE;
  FL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  // 备电电流检测使能 PE2
  GPIO_InitStruct.pin = FL_GPIO_PIN_2;
  GPIO_InitStruct.mode = FL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.pull = FL_DISABLE;
  GPIO_InitStruct.remapPin = FL_DISABLE;
  GPIO_InitStruct.analogSwitch = FL_DISABLE;
  FL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  // 电磁铁控制 PC3
  GPIO_InitStruct.pin = FL_GPIO_PIN_3;
  GPIO_InitStruct.mode = FL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.pull = FL_DISABLE;
  GPIO_InitStruct.remapPin = FL_DISABLE;
  GPIO_InitStruct.analogSwitch = FL_DISABLE;
  FL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  // 电机控制 PC2
  GPIO_InitStruct.pin = FL_GPIO_PIN_2;
  GPIO_InitStruct.mode = FL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.pull = FL_DISABLE;
  GPIO_InitStruct.remapPin = FL_DISABLE;
  GPIO_InitStruct.analogSwitch = FL_DISABLE;
  FL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  // 119电路
  GPIO_InitStruct.pin = FL_GPIO_PIN_12;
  GPIO_InitStruct.mode = FL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.pull = FL_DISABLE;
  GPIO_InitStruct.remapPin = FL_DISABLE;
  GPIO_InitStruct.analogSwitch = FL_DISABLE;
  FL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  // 工位硬件IO地址1 ,PC0
  GPIO_InitStruct.pin = FL_GPIO_PIN_0;
  GPIO_InitStruct.mode = FL_GPIO_MODE_INPUT;
  // GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.pull = FL_ENABLE;
  GPIO_InitStruct.remapPin = FL_DISABLE;
  GPIO_InitStruct.analogSwitch = FL_DISABLE;
  FL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  // 工位硬件IO地址2 ,PD12
  GPIO_InitStruct.pin = FL_GPIO_PIN_12;
  GPIO_InitStruct.mode = FL_GPIO_MODE_INPUT;
  // GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.pull = FL_ENABLE;
  GPIO_InitStruct.remapPin = FL_DISABLE;
  GPIO_InitStruct.analogSwitch = FL_DISABLE;
  FL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  // 工位硬件IO地址3 ,PD12
  GPIO_InitStruct.pin = FL_GPIO_PIN_15;
  GPIO_InitStruct.mode = FL_GPIO_MODE_INPUT;
  // GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.pull = FL_ENABLE;
  GPIO_InitStruct.remapPin = FL_DISABLE;
  GPIO_InitStruct.analogSwitch = FL_DISABLE;
  FL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  // 工位硬件IO地址4 ,PE5
  GPIO_InitStruct.pin = FL_GPIO_PIN_5;
  GPIO_InitStruct.mode = FL_GPIO_MODE_INPUT;
  // GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.pull = FL_ENABLE;
  GPIO_InitStruct.remapPin = FL_DISABLE;
  GPIO_InitStruct.analogSwitch = FL_DISABLE;
  FL_GPIO_Init(GPIOE, &GPIO_InitStruct);

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
  // 电流检测使能关闭
  Current_CHK_CTRL_OFF();
  // 电磁铁关闭
  Electromagnet_Off();
  // 电机停止
  Motor_Stop();
  // 功耗检测的IIC初始化
  INA219_IIC_GPIO_Init();
}
// LED开
void LED_On() {
  // uint8_t buf[2] = {0,1};
  FL_GPIO_ResetOutputPin(GPIOB, FL_GPIO_PIN_4);
}
// LED关
void LED_Off() { FL_GPIO_SetOutputPin(GPIOB, FL_GPIO_PIN_4); }

// LED反转
void LED_Toggle() { FL_GPIO_ToggleOutputPin(GPIOB, FL_GPIO_PIN_4); }

// 主控板主电供电开
void zhudian_gongdian_On() { FL_GPIO_SetOutputPin(GPIOC, FL_GPIO_PIN_5); }
// 主控板主电供电关
void zhudian_gongdian_OFF() { FL_GPIO_ResetOutputPin(GPIOC, FL_GPIO_PIN_5); }
// 主控板NB电源供电开
void beidian_gongdian_On() { FL_GPIO_SetOutputPin(GPIOC, FL_GPIO_PIN_4); }
// 主控板NB电源供电关
void beidian_gongdian_OFF() { FL_GPIO_ResetOutputPin(GPIOC, FL_GPIO_PIN_4); }

// 主控板主电电压检测使能开
void zhudian_dianya_CHK_CTRL_ON() {
  FL_GPIO_SetOutputPin(GPIOE, FL_GPIO_PIN_0);
}
// 主控板主电电压检测使能关
void zhudian_dianya_CHK_CTRL_OFF() {
  FL_GPIO_ResetOutputPin(GPIOE, FL_GPIO_PIN_0);
}
// 主控板NB电压检测使能开
void erji_dianya_CHK_CTRL_ON() { FL_GPIO_SetOutputPin(GPIOE, FL_GPIO_PIN_1); }
// 主控板NB电压检测使能关
void erji_dianya_CHK_CTRL_OFF() {
  FL_GPIO_ResetOutputPin(GPIOE, FL_GPIO_PIN_1);
}

// 主电电流检测使能开
void Current_CHK_CTRL_ON() { FL_GPIO_ResetOutputPin(GPIOE, FL_GPIO_PIN_3); }
// 主电电流检测使能关
void Current_CHK_CTRL_OFF() { FL_GPIO_SetOutputPin(GPIOE, FL_GPIO_PIN_3); }
// NB电流检测使能开
void NB_Current_CHK_CTRL_ON() { FL_GPIO_ResetOutputPin(GPIOE, FL_GPIO_PIN_2); }
// NB电流检测使能开
void NB_Current_CHK_CTRL_OFF() { FL_GPIO_ResetOutputPin(GPIOE, FL_GPIO_PIN_2); }

// 电磁铁开启
void Electromagnet_On() { FL_GPIO_SetOutputPin(GPIOC, FL_GPIO_PIN_3); }
// 电磁铁关闭
void Electromagnet_Off() { FL_GPIO_ResetOutputPin(GPIOC, FL_GPIO_PIN_3); }
// 电机启动
void Motor_Start() { FL_GPIO_SetOutputPin(GPIOC, FL_GPIO_PIN_2); }
// 电机停止
void Motor_Stop() { FL_GPIO_ResetOutputPin(GPIOC, FL_GPIO_PIN_2); }
// 工位检测
uint8_t board_station_check(void) {
  uint8_t addr0 = 0, addr1 = 0, addr2 = 0, addr3 = 0, board_station_addr = 0;
  addr0 = FL_GPIO_GetInputPin(GPIOC, FL_GPIO_PIN_0);
  addr1 = FL_GPIO_GetInputPin(GPIOD, FL_GPIO_PIN_12);
  addr2 = FL_GPIO_GetInputPin(GPIOB, FL_GPIO_PIN_15);
  addr3 = FL_GPIO_GetInputPin(GPIOE, FL_GPIO_PIN_5);
  // 简单配置工位，addre0是1就默认当前工位是1,addre1是1就默认当前工位是2,addre2是1就默认当前工位是3,addr3是1就默认当前工位是4
  if (addr0 == 0) {
    board_station_addr = 0;
  } else if (addr1 == 0) {
    board_station_addr = 1;
  } else if (addr2 == 0) {
    board_station_addr = 2;
  } else if (addr3 == 0) {
    board_station_addr = 3;
  } else {
    board_station_addr = 0; // 默认工位0
  }
  return board_station_addr;
}

// 阀门开到位信号输出（到位=低电平）
// ON: 配置为推挽输出，输出低电平
void valve_open_on(void) {
  DeBug_print("当前阀门开到位信号输出低电平\r\n");
  FL_GPIO_SetPinMode(GPIOA, FL_GPIO_PIN_11, FL_GPIO_MODE_OUTPUT);
  FL_GPIO_ResetOutputPin(GPIOA, FL_GPIO_PIN_11);
  valve_open_status = FL_GPIO_MODE_OUTPUT;
}

// 阀门开到位信号关闭（非到位=高电平）
// OFF: 配置为推挽输出，输出高电平（因为没有外部上拉）
void valve_open_off(void) {
  DeBug_print("当前阀门开到位信号输出高电平\r\n");
  FL_GPIO_SetPinMode(GPIOA, FL_GPIO_PIN_11, FL_GPIO_MODE_OUTPUT);
  FL_GPIO_SetOutputPin(GPIOA, FL_GPIO_PIN_11);
  valve_open_status = FL_GPIO_MODE_OUTPUT;
}

// 阀门关到位信号输出（到位=低电平）
// ON: 配置为推挽输出，输出低电平
void valve_close_on(void) {
  DeBug_print("当前阀门关到位信号输出低电平\r\n");
  FL_GPIO_SetPinMode(GPIOA, FL_GPIO_PIN_12, FL_GPIO_MODE_OUTPUT);
  FL_GPIO_ResetOutputPin(GPIOA, FL_GPIO_PIN_12);
  valve_close_status = FL_GPIO_MODE_OUTPUT;
}

// 阀门关到位信号关闭（非到位=高电平）
// OFF: 配置为推挽输出，输出高电平（因为没有外部上拉）
void valve_close_off(void) {
  DeBug_print("当前阀门关到位信号输出高电平\r\n");
  FL_GPIO_SetPinMode(GPIOA, FL_GPIO_PIN_12, FL_GPIO_MODE_OUTPUT);
  FL_GPIO_SetOutputPin(GPIOA, FL_GPIO_PIN_12);
  valve_close_status = FL_GPIO_MODE_OUTPUT;
}
/*;
//GPIO中断
void MF_EXTI_Init(void)
{
    FL_EXTI_CommonInitTypeDef    EXTI_InitStruct;

    EXTI_InitStruct.clockSource = FL_CMU_EXTI_CLK_SOURCE_HCLK;

    (void)FL_EXTI_CommonInit(&EXTI_InitStruct);
}
void MF_EXTI_Line2_Init(void)
{
    FL_GPIO_InitTypeDef    GPIO_InitStruct;

    FL_EXTI_InitTypeDef    EXTI_InitStruct;

    GPIO_InitStruct.pin          = FL_GPIO_PIN_10;
    GPIO_InitStruct.mode         = FL_GPIO_MODE_INPUT;
    GPIO_InitStruct.outputType   = FL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.pull         = FL_ENABLE;
    GPIO_InitStruct.remapPin     = FL_DISABLE;
    GPIO_InitStruct.analogSwitch = FL_DISABLE;
    (void)FL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    EXTI_InitStruct.filter      = FL_ENABLE;
    EXTI_InitStruct.input       = FL_GPIO_EXTI_INPUT_GROUP2;
    EXTI_InitStruct.triggerEdge = FL_GPIO_EXTI_TRIGGER_EDGE_FALLING;

    (void)FL_EXTI_Init(FL_GPIO_EXTI_LINE_2, &EXTI_InitStruct);
}
void MF_NVIC_Init(void)
{
    FL_NVIC_ConfigTypeDef    InterruptConfigStruct;

    InterruptConfigStruct.preemptPriority = 0x02;
    FL_NVIC_Init(&InterruptConfigStruct, GPIO_IRQn);
}

void MF_EXTI_Config_Init(void)
{
    MF_EXTI_Init();
    MF_EXTI_Line2_Init();
    MF_NVIC_Init();
}
*/

void GPIO_IRQHandler(void) {
  if (FL_GPIO_IsActiveFlag_EXTI(GPIO, FL_GPIO_EXTI_LINE_2) == 0x01UL) {
    FL_GPIO_ClearFlag_EXTI(GPIO, FL_GPIO_EXTI_LINE_2);
  }
}

void configure_gpio(GPIO_Type *GPIOx, uint32_t pin, uint8_t mode) {
  FL_GPIO_InitTypeDef GPIO_InitStruct = {0}; // 初始化结构体，避免垃圾值
  GPIO_InitStruct.pin = pin;
  GPIO_InitStruct.mode = mode;
  if (mode == FL_GPIO_MODE_OUTPUT) {
    GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
  } else if (mode == FL_GPIO_MODE_INPUT) {
    GPIO_InitStruct.pull = FL_DISABLE; // 输入模式，无上下拉
  }
  GPIO_InitStruct.remapPin = FL_DISABLE;
  GPIO_InitStruct.analogSwitch = FL_DISABLE;
  FL_GPIO_Init(GPIOx, &GPIO_InitStruct);
}

void change_valve_open_to_output_mode(void) {
  configure_gpio(GPIOA, FL_GPIO_PIN_11, FL_GPIO_MODE_OUTPUT);
  valve_open_status = FL_GPIO_MODE_OUTPUT;
}

void change_valve_close_to_output_mode(void) {
  configure_gpio(GPIOA, FL_GPIO_PIN_12, FL_GPIO_MODE_OUTPUT);
  valve_close_status = FL_GPIO_MODE_OUTPUT;
}

void change_valve_open_to_input_mode(void) {
  configure_gpio(GPIOA, FL_GPIO_PIN_11, FL_GPIO_MODE_INPUT);
  valve_open_status = FL_GPIO_MODE_INPUT;
}

void change_valve_close_to_input_mode(void) {
  configure_gpio(GPIOA, FL_GPIO_PIN_12, FL_GPIO_MODE_INPUT);
  valve_close_status = FL_GPIO_MODE_INPUT;
}

uint8_t read_valve_open_status(void) {
  if (valve_open_status == FL_GPIO_MODE_INPUT) {
    return FL_GPIO_GetInputPin(GPIOA, FL_GPIO_PIN_11);
  } else {
    return 0xFF; // 非输入模式下返回无效值
  }
}

uint8_t read_valve_close_status(void) {
  if (valve_close_status == FL_GPIO_MODE_INPUT) {
    return FL_GPIO_GetInputPin(GPIOA, FL_GPIO_PIN_12);
  } else {
    return 0xFF; // 非输入模式下返回无效值
  }
}
