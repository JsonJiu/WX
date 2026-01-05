#include "GPIO.h"
#include "ZDINA219.h"
void Others_GPIO_Init()
{
  FL_VAO_IO_InitTypeDef VAO_IO_InitStruct;
  // LED指示灯 PB6
  FL_GPIO_InitTypeDef GPIO_InitStruct;
  GPIO_InitStruct.pin = FL_GPIO_PIN_6;
  GPIO_InitStruct.mode = FL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.pull = FL_DISABLE;
  GPIO_InitStruct.remapPin = FL_DISABLE;
  GPIO_InitStruct.analogSwitch = FL_DISABLE;
  FL_GPIO_Init(GPIOB, &GPIO_InitStruct);

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

  // 阀门断线检测 PC3
  GPIO_InitStruct.pin = FL_GPIO_PIN_3;
  GPIO_InitStruct.mode = FL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.pull = FL_DISABLE;
  GPIO_InitStruct.remapPin = FL_DISABLE;
  GPIO_InitStruct.analogSwitch = FL_DISABLE;
  FL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  // 阀控_调试_唤醒 PC2
  GPIO_InitStruct.pin = FL_GPIO_PIN_2;
  GPIO_InitStruct.mode = FL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.pull = FL_DISABLE;
  GPIO_InitStruct.remapPin = FL_DISABLE;
  GPIO_InitStruct.analogSwitch = FL_DISABLE;
  FL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  // 按键电路控制
  GPIO_InitStruct.pin = FL_GPIO_PIN_1;
  GPIO_InitStruct.mode = FL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_OPENDRAIN;
  GPIO_InitStruct.pull = FL_DISABLE;
  GPIO_InitStruct.remapPin = FL_DISABLE;
  GPIO_InitStruct.analogSwitch = FL_DISABLE;
  FL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  // 计量IO1初始化
  GPIO_InitStruct.pin = FL_GPIO_PIN_3;
  GPIO_InitStruct.mode = FL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.pull = FL_DISABLE;
  GPIO_InitStruct.remapPin = FL_DISABLE;
  GPIO_InitStruct.analogSwitch = FL_DISABLE;
  FL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  // 计量IO2初始化
  GPIO_InitStruct.pin = FL_GPIO_PIN_2;
  GPIO_InitStruct.mode = FL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.pull = FL_DISABLE;
  GPIO_InitStruct.remapPin = FL_DISABLE;
  GPIO_InitStruct.analogSwitch = FL_DISABLE;
  FL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  // 脉冲输出检测IO11初始化 PD10
  GPIO_InitStruct.pin = FL_GPIO_PIN_10;
  GPIO_InitStruct.mode = FL_GPIO_MODE_INPUT;
  GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.pull = FL_ENABLE;
  GPIO_InitStruct.remapPin = FL_DISABLE;
  GPIO_InitStruct.analogSwitch = FL_DISABLE;
  FL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  // 脉冲输出接地 PE9
  GPIO_InitStruct.pin = FL_GPIO_PIN_9;
  GPIO_InitStruct.mode = FL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.pull = FL_DISABLE;
  GPIO_InitStruct.remapPin = FL_DISABLE;
  GPIO_InitStruct.analogSwitch = FL_DISABLE;
  FL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  // 脉冲输出检测IO13初始化 PA15
  GPIO_InitStruct.pin = FL_GPIO_PIN_15;
  GPIO_InitStruct.mode = FL_GPIO_MODE_INPUT;
  GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.pull = FL_ENABLE;
  GPIO_InitStruct.remapPin = FL_DISABLE;
  GPIO_InitStruct.analogSwitch = FL_DISABLE;
  FL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  // 阀控通信引脚 PA4
  GPIO_InitStruct.pin = FL_GPIO_PIN_4;
  GPIO_InitStruct.mode = FL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.pull = FL_ENABLE;
  GPIO_InitStruct.remapPin = FL_DISABLE;
  GPIO_InitStruct.analogSwitch = FL_DISABLE;
  FL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  // NB电源检测 PC0
  GPIO_InitStruct.pin = FL_GPIO_PIN_0;
  GPIO_InitStruct.mode = FL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.pull = FL_ENABLE;
  GPIO_InitStruct.remapPin = FL_DISABLE;
  GPIO_InitStruct.analogSwitch = FL_DISABLE;
  FL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  // 阀门正极电压检测控制 PC1
  GPIO_InitStruct.pin = FL_GPIO_PIN_1;
  GPIO_InitStruct.mode = FL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.pull = FL_ENABLE;
  GPIO_InitStruct.remapPin = FL_DISABLE;
  GPIO_InitStruct.analogSwitch = FL_DISABLE;
  FL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  // 工位选择2 PD12
  GPIO_InitStruct.pin = FL_GPIO_PIN_12;
  GPIO_InitStruct.mode = FL_GPIO_MODE_INPUT;
  GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.pull = FL_ENABLE;
  GPIO_InitStruct.remapPin = FL_DISABLE;
  GPIO_InitStruct.analogSwitch = FL_DISABLE;
  FL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  // 工位选择3 PB15
  GPIO_InitStruct.pin = FL_GPIO_PIN_15;
  GPIO_InitStruct.mode = FL_GPIO_MODE_INPUT;
  GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.pull = FL_ENABLE;
  GPIO_InitStruct.remapPin = FL_DISABLE;
  GPIO_InitStruct.analogSwitch = FL_DISABLE;
  FL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  // 工位选择4 PE5
  GPIO_InitStruct.pin = FL_GPIO_PIN_5;
  GPIO_InitStruct.mode = FL_GPIO_MODE_INPUT;
  GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.pull = FL_ENABLE;
  GPIO_InitStruct.remapPin = FL_DISABLE;
  GPIO_InitStruct.analogSwitch = FL_DISABLE;
  FL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  // 开到位 PA12
  GPIO_InitStruct.pin = FL_GPIO_PIN_12;
  GPIO_InitStruct.mode = FL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.pull = FL_DISABLE;
  GPIO_InitStruct.remapPin = FL_DISABLE;
  GPIO_InitStruct.analogSwitch = FL_DISABLE;
  FL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  // 关到位 PA11
  GPIO_InitStruct.pin = FL_GPIO_PIN_11;
  GPIO_InitStruct.mode = FL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.pull = FL_DISABLE;
  GPIO_InitStruct.remapPin = FL_DISABLE;
  GPIO_InitStruct.analogSwitch = FL_DISABLE;
  FL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  // 脉冲输出引脚1 PA5
  GPIO_InitStruct.pin = FL_GPIO_PIN_5;
  GPIO_InitStruct.mode = FL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.pull = FL_DISABLE;
  GPIO_InitStruct.remapPin = FL_DISABLE;
  GPIO_InitStruct.analogSwitch = FL_DISABLE;
  FL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  // 脉冲输出引脚2 PA6
  GPIO_InitStruct.pin = FL_GPIO_PIN_6;
  GPIO_InitStruct.mode = FL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.pull = FL_DISABLE;
  GPIO_InitStruct.remapPin = FL_DISABLE;
  GPIO_InitStruct.analogSwitch = FL_DISABLE;
  FL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  // 计量与浓度断线检测 PA7
  GPIO_InitStruct.pin = FL_GPIO_PIN_7;
  GPIO_InitStruct.mode = FL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.pull = FL_DISABLE;
  GPIO_InitStruct.remapPin = FL_DISABLE;
  GPIO_InitStruct.analogSwitch = FL_DISABLE;
  FL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  // 计量与浓度断线检测 PA8
  GPIO_InitStruct.pin = FL_GPIO_PIN_8;
  GPIO_InitStruct.mode = FL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.pull = FL_DISABLE;
  GPIO_InitStruct.remapPin = FL_DISABLE;
  GPIO_InitStruct.analogSwitch = FL_DISABLE;
  FL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  // 温压供电检测 PA9
  GPIO_InitStruct.pin = FL_GPIO_PIN_9;
  GPIO_InitStruct.mode = FL_GPIO_MODE_INPUT;
  GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.pull = FL_DISABLE;
  GPIO_InitStruct.remapPin = FL_DISABLE;
  GPIO_InitStruct.analogSwitch = FL_DISABLE;
  FL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  // 3.3V拉高引脚 PA10
  GPIO_InitStruct.pin = FL_GPIO_PIN_10;
  GPIO_InitStruct.mode = FL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.pull = FL_DISABLE;
  GPIO_InitStruct.remapPin = FL_DISABLE;
  GPIO_InitStruct.analogSwitch = FL_DISABLE;
  FL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  // 开盖检测 PD9
  GPIO_InitStruct.pin = FL_GPIO_PIN_9;
  GPIO_InitStruct.mode = FL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_OPENDRAIN;
  GPIO_InitStruct.pull = FL_DISABLE;
  GPIO_InitStruct.remapPin = FL_DISABLE;
  GPIO_InitStruct.analogSwitch = FL_DISABLE;
  FL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  // 按键1控制 PB2
  GPIO_InitStruct.pin = FL_GPIO_PIN_2;
  GPIO_InitStruct.mode = FL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_OPENDRAIN;
  GPIO_InitStruct.pull = FL_DISABLE;
  GPIO_InitStruct.remapPin = FL_DISABLE;
  GPIO_InitStruct.analogSwitch = FL_DISABLE;
  FL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  // 按键2控制 PB3
  GPIO_InitStruct.pin = FL_GPIO_PIN_3;
  GPIO_InitStruct.mode = FL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_OPENDRAIN;
  GPIO_InitStruct.pull = FL_DISABLE;
  GPIO_InitStruct.remapPin = FL_DISABLE;
  GPIO_InitStruct.analogSwitch = FL_DISABLE;
  FL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  // 调试口通信控制 PB7
  GPIO_InitStruct.pin = FL_GPIO_PIN_7;
  GPIO_InitStruct.mode = FL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.pull = FL_DISABLE;
  GPIO_InitStruct.remapPin = FL_DISABLE;
  GPIO_InitStruct.analogSwitch = FL_DISABLE;
  FL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  // NB电压读取控制 PB9
  GPIO_InitStruct.pin = FL_GPIO_PIN_9;
  GPIO_InitStruct.mode = FL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.pull = FL_DISABLE;
  GPIO_InitStruct.remapPin = FL_DISABLE;
  GPIO_InitStruct.analogSwitch = FL_DISABLE;
  FL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  // VDD电压读取控制 PB10
  GPIO_InitStruct.pin = FL_GPIO_PIN_10;
  GPIO_InitStruct.mode = FL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.pull = FL_DISABLE;
  GPIO_InitStruct.remapPin = FL_DISABLE;
  GPIO_InitStruct.analogSwitch = FL_DISABLE;
  FL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  // 119輸出电压读取控制 PB11
  GPIO_InitStruct.pin = FL_GPIO_PIN_11;
  GPIO_InitStruct.mode = FL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.pull = FL_DISABLE;
  GPIO_InitStruct.remapPin = FL_DISABLE;
  GPIO_InitStruct.analogSwitch = FL_DISABLE;
  FL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  // 通信复位控制 PC13
  GPIO_InitStruct.pin = FL_GPIO_PIN_13;
  GPIO_InitStruct.mode = FL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.pull = FL_DISABLE;
  GPIO_InitStruct.remapPin = FL_DISABLE;
  GPIO_InitStruct.analogSwitch = FL_DISABLE;
  FL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  // 通信唤醒控制 PC14
  GPIO_InitStruct.pin = FL_GPIO_PIN_14;
  GPIO_InitStruct.mode = FL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.pull = FL_DISABLE;
  GPIO_InitStruct.remapPin = FL_DISABLE;
  GPIO_InitStruct.analogSwitch = FL_DISABLE;
  FL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  // 通信电源控制 PC15
  GPIO_InitStruct.pin = FL_GPIO_PIN_15;
  GPIO_InitStruct.mode = FL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.pull = FL_DISABLE;
  GPIO_InitStruct.remapPin = FL_DISABLE;
  GPIO_InitStruct.analogSwitch = FL_DISABLE;
  FL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  // 屏幕背光控制 PE5
  GPIO_InitStruct.pin = FL_GPIO_PIN_5;
  GPIO_InitStruct.mode = FL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.pull = FL_DISABLE;
  GPIO_InitStruct.remapPin = FL_DISABLE;
  GPIO_InitStruct.analogSwitch = FL_DISABLE;
  FL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  // 3.6V供电控制 PC10
  GPIO_InitStruct.pin = FL_GPIO_PIN_10;
  GPIO_InitStruct.mode = FL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.pull = FL_DISABLE;
  GPIO_InitStruct.remapPin = FL_DISABLE;
  GPIO_InitStruct.analogSwitch = FL_DISABLE;
  FL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  // 阀门-电压检测电路 PH15
  /*CDIF接口CPU->VAO方向使能*/
  FL_CDIF_EnableCPUToVAO(CDIF);
  /*设置CDIF接口预分频*/
  FL_CDIF_SetPrescaler(CDIF, FL_CDIF_PSC_DIV2);
  /*PH15配置*/
  VAO_IO_InitStruct.input = FL_DISABLE;
  VAO_IO_InitStruct.pullup = FL_DISABLE;
  VAO_IO_InitStruct.opendrainOutput = FL_DISABLE;
  VAO_IO_InitStruct.mode = FL_VAO_PH15_MODE_OUTPUT;
  FL_VAO_IO_Init(VAO, &VAO_IO_InitStruct);
  FL_CDIF_DisableCPUToVAO(CDIF);

  // 上电先关灯
  LED_Off();
  // 按键设置为开漏
  anjian_kongzhi_OFF();
  // 主电源供电断开
  V3_6_gongdian_kaiguan_Off();
  // 备电电源断开
  NB_dianyuan_gongdian_Off();
  // 主电电压检测控制关闭
  zhudian_dianya_CHK_CTRL_OFF();
  // 二级电源电压检测控制关闭
  erji_dianya_CHK_CTRL_OFF();
  // 电流检测使能关闭
  Current_CHK_CTRL_OFF();
  // 霍尔1控制 关
  huuoer_1_OFF();
  // 霍尔2控制 关
  huuoer_2_OFF();
  // 计量IO1置低
  jiliang_IO1_set_Low();
  // 计量IO2置高
  jiliang_IO2_set_Hi();
  // 脉冲IO12接地脚置低
  maichong_IO12_set_Low();
  // 功耗检测的IIC初始化
  INA219_IIC_GPIO_Init();
  // 开到位置高
  kaidaowei_set_Hi();
  // 关到位置低
  guandaowei_set_Low();
  // 阀控通信引脚置低
  fakong_tongxin_set_Low();
  // 脉冲输出1引脚置高
  maichong_shuchu_1_set_Hi();
  // 脉冲输出2引脚置高
  maichong_shuchu_2_set_Hi();
  // 断线IO3引脚置低
  duanxian_shuchu_IO3_set_Low();
  // 断线IO4引脚置低
  duanxian_shuchu_IO4_set_Low();
  // 3.3V输出引脚拉低
  V3_3_OPT_shuchu_set_Low();
  // 开盖检测输出引脚拉高IO6
  kaigai_OPT_shuchu_set_Hi();
  // 调试口串口通信控制拉低 PB7
  tiaoshi_chuankou_tongxin_set_Low();
  // NB电压读取控制拉低 PB9
  NB_dianya_duqu_set_Low();
  // VDD电压读取控制拉低 PB10
  VDD_dianya_duqu_set_Low();
  // VDD电压读取控制拉低 PB11
  V119_dianya_duqu_set_Low();
  // 通信复位控制拉低 PC13
  tongxin_fuwei_set_Low();
  // 通信唤醒控制拉低 PC14
  tongxin_huanxing_set_Low();
  // 通信电源控制拉低 PC15
  tongxin_dianyuan_set_Low();
  // 屏幕背光控制拉低 PE5
  pingmu_beiguang_set_Low();
  // NB电源供电控制关闭 PC0
  NB_dianyuan_gongdian_Off();
  // 阀控_调试_唤醒关闭 PC2
  FK_tiaoshi_chuankou_huanxing_Off();
  // 阀门断线检测拉低 PC3
  FK_duanxia_CHK_Off();
  // 3.6V供电控制关闭 PC10
  V3_6_gongdian_kaiguan_Off();
  // 阀门负极电压检测控制低 PH15
  FM_fuji_dianyuan_gongdian_Off();
}
// LED开
void LED_On()
{
  // uint8_t buf[2] = {0,1};
  FL_GPIO_ResetOutputPin(GPIOB, FL_GPIO_PIN_6);
}
// LED关
void LED_Off()
{
  FL_GPIO_SetOutputPin(GPIOB, FL_GPIO_PIN_6);
}

// 主控板主电电压检测使能开
void zhudian_dianya_CHK_CTRL_ON()
{
  FL_GPIO_SetOutputPin(GPIOE, FL_GPIO_PIN_0);
}
// 主控板主电电压检测使能关
void zhudian_dianya_CHK_CTRL_OFF()
{
  FL_GPIO_ResetOutputPin(GPIOE, FL_GPIO_PIN_0);
}
// 主控板NB电压检测使能开
void erji_dianya_CHK_CTRL_ON()
{
  FL_GPIO_SetOutputPin(GPIOE, FL_GPIO_PIN_1);
}
// 主控板NB电压检测使能关
void erji_dianya_CHK_CTRL_OFF()
{
  FL_GPIO_ResetOutputPin(GPIOE, FL_GPIO_PIN_1);
}

// 主电电流检测使能开
void Current_CHK_CTRL_ON()
{
  FL_GPIO_ResetOutputPin(GPIOE, FL_GPIO_PIN_3);
}
// 主电电流检测使能关
void Current_CHK_CTRL_OFF()
{
  FL_GPIO_SetOutputPin(GPIOE, FL_GPIO_PIN_3);
}
// NB电流检测使能开
void NB_Current_CHK_CTRL_ON()
{
  FL_GPIO_ResetOutputPin(GPIOE, FL_GPIO_PIN_2);
}
// NB电流检测使能开
void NB_Current_CHK_CTRL_OFF()
{
  FL_GPIO_ResetOutputPin(GPIOE, FL_GPIO_PIN_2);
}

// 霍尔1控制 开
void huuoer_1_ON()
{
  FL_GPIO_SetOutputPin(GPIOC, FL_GPIO_PIN_3);
}
// 霍尔1控制 关
void huuoer_1_OFF()
{
  FL_GPIO_ResetOutputPin(GPIOC, FL_GPIO_PIN_3);
}
// 霍尔2控制 开
void huuoer_2_ON()
{
  FL_GPIO_SetOutputPin(GPIOC, FL_GPIO_PIN_2);
}
// 霍尔2控制 关
void huuoer_2_OFF()
{
  FL_GPIO_ResetOutputPin(GPIOC, FL_GPIO_PIN_2);
}
// 按键控制 开(拉低)
void anjian_kongzhi_ON()
{
  FL_GPIO_ResetOutputPin(GPIOD, FL_GPIO_PIN_1);
}
// 按键控制 关(拉高)
void anjian_kongzhi_OFF()
{
  FL_GPIO_SetOutputPin(GPIOD, FL_GPIO_PIN_1);
}
// 计量IO1置高
void jiliang_IO1_set_Hi()
{
  FL_GPIO_SetOutputPin(GPIOA, FL_GPIO_PIN_3);
}
// 计量IO1置低
void jiliang_IO1_set_Low()
{
  FL_GPIO_ResetOutputPin(GPIOA, FL_GPIO_PIN_3);
}
// 计量IO2置高
void jiliang_IO2_set_Hi()
{
  FL_GPIO_SetOutputPin(GPIOA, FL_GPIO_PIN_2);
}
// 计量IO2置低
void jiliang_IO2_set_Low()
{
  FL_GPIO_ResetOutputPin(GPIOA, FL_GPIO_PIN_2);
}
// 脉冲IO11读取状态
uint32_t maichong_IO11_get_state()
{
  return FL_GPIO_GetInputPin(GPIOD, FL_GPIO_PIN_10);
}
// 脉冲IO12置高
void maichong_IO12_set_Hi()
{
  FL_GPIO_SetOutputPin(GPIOE, FL_GPIO_PIN_9);
}
// 脉冲IO12置低
void maichong_IO12_set_Low()
{
  FL_GPIO_ResetOutputPin(GPIOE, FL_GPIO_PIN_9);
}
// 脉冲IO13读取状态
uint32_t maichong_IO13_get_state()
{
  return FL_GPIO_GetInputPin(GPIOA, FL_GPIO_PIN_15);
}

// 开到位置高
void kaidaowei_set_Hi()
{
  FL_GPIO_SetOutputPin(GPIOA, FL_GPIO_PIN_11);
}
// 开到位置低
void kaidaowei_set_Low()
{
  FL_GPIO_ResetOutputPin(GPIOA, FL_GPIO_PIN_11);
}
// 关到位置高
void guandaowei_set_Hi()
{
  FL_GPIO_SetOutputPin(GPIOA, FL_GPIO_PIN_12);
}
// 关到位置低
void guandaowei_set_Low()
{
  FL_GPIO_ResetOutputPin(GPIOA, FL_GPIO_PIN_12);
}

// 阀控通信引脚置高
void fakong_tongxin_set_Hi()
{
  FL_GPIO_SetOutputPin(GPIOA, FL_GPIO_PIN_4);
}
// 阀控通信引脚置低
void fakong_tongxin_set_Low()
{
  FL_GPIO_ResetOutputPin(GPIOA, FL_GPIO_PIN_4);
}

// 脉冲输出1引脚置高
void maichong_shuchu_1_set_Hi()
{
  FL_GPIO_SetOutputPin(GPIOA, FL_GPIO_PIN_5);
}
// 脉冲输出1引脚置低
void maichong_shuchu_1_set_Low()
{
  FL_GPIO_ResetOutputPin(GPIOA, FL_GPIO_PIN_5);
}
// 脉冲输出2引脚置高
void maichong_shuchu_2_set_Hi()
{
  FL_GPIO_SetOutputPin(GPIOA, FL_GPIO_PIN_6);
}
// 脉冲输出2引脚置低
void maichong_shuchu_2_set_Low()
{
  FL_GPIO_ResetOutputPin(GPIOA, FL_GPIO_PIN_6);
}
// 断线IO3引脚置低
void duanxian_shuchu_IO3_set_Low()
{
  FL_GPIO_ResetOutputPin(GPIOA, FL_GPIO_PIN_7);
}
// 断线IO3引脚置高
void duanxian_shuchu_IO3_set_Hi()
{
  FL_GPIO_SetOutputPin(GPIOA, FL_GPIO_PIN_7);
}
// 断线IO4引脚置低
void duanxian_shuchu_IO4_set_Low()
{
  FL_GPIO_ResetOutputPin(GPIOA, FL_GPIO_PIN_8);
}
// 断线IO4引脚置高
void duanxian_shuchu_IO4_set_Hi()
{
  FL_GPIO_SetOutputPin(GPIOA, FL_GPIO_PIN_8);
}
// 温压供电检测
uint32_t wenya_IO5_get_state()
{
  return FL_GPIO_GetInputPin(GPIOA, FL_GPIO_PIN_9);
}
// 3.3V输出引脚拉高
void V3_3_OPT_shuchu_set_Hi()
{
  FL_GPIO_SetOutputPin(GPIOA, FL_GPIO_PIN_10);
}
// 3.3V输出引脚拉低
void V3_3_OPT_shuchu_set_Low()
{
  FL_GPIO_ResetOutputPin(GPIOA, FL_GPIO_PIN_10);
}

// 开盖检测输出引脚拉高IO6
void kaigai_OPT_shuchu_set_Hi()
{
  FL_GPIO_SetOutputPin(GPIOD, FL_GPIO_PIN_9);
}
// 开盖检测输出引脚拉低IO6
void kaigai_OPT_shuchu_set_Low()
{
  FL_GPIO_ResetOutputPin(GPIOD, FL_GPIO_PIN_9);
}
// 调试口串口通信控制拉低 PB7
void tiaoshi_chuankou_tongxin_set_Low()
{
  FL_GPIO_ResetOutputPin(GPIOB, FL_GPIO_PIN_7);
}

// 调试口串口通信控制拉高 PB7
void tiaoshi_chuankou_tongxin_set_Hi()
{
  FL_GPIO_SetOutputPin(GPIOB, FL_GPIO_PIN_7);
}
// NB电压读取控制拉低 PB9
void NB_dianya_duqu_set_Low()
{
  FL_GPIO_ResetOutputPin(GPIOB, FL_GPIO_PIN_9);
}

// NB电压读取控制拉高 PB9
void NB_dianya_duqu_set_Hi()
{
  FL_GPIO_SetOutputPin(GPIOB, FL_GPIO_PIN_9);
}
// VDD电压读取控制 PB10
void VDD_dianya_duqu_set_Low()
{
  FL_GPIO_ResetOutputPin(GPIOB, FL_GPIO_PIN_10);
}

// VDD电压读取控制 PB10
void VDD_dianya_duqu_set_Hi()
{
  FL_GPIO_SetOutputPin(GPIOB, FL_GPIO_PIN_10);
}
// 119輸出电压读取控制拉低 PB11
void V119_dianya_duqu_set_Low()
{
  FL_GPIO_ResetOutputPin(GPIOB, FL_GPIO_PIN_11);
}

// VDD电压读取控制拉高 PB11
void V119_dianya_duqu_set_Hi()
{
  FL_GPIO_SetOutputPin(GPIOB, FL_GPIO_PIN_11);
}

// 通信复位控制拉高 PC13
void tongxin_fuwei_set_Hi()
{
  FL_GPIO_SetOutputPin(GPIOC, FL_GPIO_PIN_13);
}
// 通信复位控制拉低 PC13
void tongxin_fuwei_set_Low()
{
  FL_GPIO_ResetOutputPin(GPIOC, FL_GPIO_PIN_13);
}
// 通信唤醒控制 PC14
void tongxin_huanxing_set_Hi()
{
  FL_GPIO_SetOutputPin(GPIOC, FL_GPIO_PIN_14);
}
// 通信唤醒控制拉低 PC14
void tongxin_huanxing_set_Low()
{
  FL_GPIO_ResetOutputPin(GPIOC, FL_GPIO_PIN_14);
}

// 通信电源控制拉低 PC15
void tongxin_dianyuan_set_Hi()
{
  FL_GPIO_ResetOutputPin(GPIOC, FL_GPIO_PIN_15);
}
// 通信电源控制拉高 PC15
void tongxin_dianyuan_set_Low()
{
  FL_GPIO_SetOutputPin(GPIOC, FL_GPIO_PIN_15);
}
// 屏幕背光控制拉高 PE5
void pingmu_beiguang_set_Hi()
{
  FL_GPIO_SetOutputPin(GPIOE, FL_GPIO_PIN_5);
}
// 屏幕背光控制拉低 PE5
void pingmu_beiguang_set_Low()
{
  FL_GPIO_ResetOutputPin(GPIOE, FL_GPIO_PIN_5);
}
// NB电源供电控制 PC0
void NB_dianyuan_gongdian_On()
{
  FL_GPIO_SetOutputPin(GPIOC, FL_GPIO_PIN_0);
}
// NB电源供电控制 PC0
void NB_dianyuan_gongdian_Off()
{
  FL_GPIO_ResetOutputPin(GPIOC, FL_GPIO_PIN_0);
}
// 阀门正极电压检测控制高 PC1
void FM_zhengji_dianyuan_gongdian_On()
{
  FL_GPIO_SetOutputPin(GPIOC, FL_GPIO_PIN_1);
}
// 阀门正极电压检测控制低 PC1
void FM_zhengji_dianyuan_gongdian_Off()
{
  FL_GPIO_ResetOutputPin(GPIOC, FL_GPIO_PIN_1);
}
// 阀控_调试_唤醒开启 PC2
void FK_tiaoshi_chuankou_huanxing_On()
{
  FL_GPIO_SetOutputPin(GPIOC, FL_GPIO_PIN_2);
}
// 阀控_调试_唤醒关闭 PC2
void FK_tiaoshi_chuankou_huanxing_Off()
{
  FL_GPIO_ResetOutputPin(GPIOC, FL_GPIO_PIN_2);
}
// 阀门断线检测拉高 PC3
void FK_duanxia_CHK_On()
{
  FL_GPIO_SetOutputPin(GPIOC, FL_GPIO_PIN_3);
}
// 阀门断线检测拉低 PC3
void FK_duanxia_CHK_Off()
{
  FL_GPIO_ResetOutputPin(GPIOC, FL_GPIO_PIN_3);
}
// 3.6V供电控制开启 PC10
void V3_6_gongdian_kaiguan_On()
{
  FL_GPIO_SetOutputPin(GPIOC, FL_GPIO_PIN_10);
}
// 3.6V供电控制关闭 PC10
void V3_6_gongdian_kaiguan_Off()
{
  FL_GPIO_ResetOutputPin(GPIOC, FL_GPIO_PIN_10);
}
// 阀门负极电压检测控制高 PH15
void FM_fuji_dianyuan_gongdian_On()
{
  FL_CDIF_EnableCPUToVAO(CDIF);

  FL_VAO_GPIO_WritePH15Output(VAO, 1);

  FL_CDIF_DisableCPUToVAO(CDIF);
}
// 阀门负极电压检测控制低 PH15
void FM_fuji_dianyuan_gongdian_Off()
{
  FL_CDIF_EnableCPUToVAO(CDIF);

  FL_VAO_GPIO_WritePH15Output(VAO, 0);

  FL_CDIF_DisableCPUToVAO(CDIF);
}

/*
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

void GPIO_IRQHandler(void)
{
  if (FL_GPIO_IsActiveFlag_EXTI(GPIO, FL_GPIO_EXTI_LINE_2) == 0x01UL)
  {
    FL_GPIO_ClearFlag_EXTI(GPIO, FL_GPIO_EXTI_LINE_2);
  }
}
