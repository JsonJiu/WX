#include "gpio.h"
#include "ina219.h"
void User_GPIO_Init(void)
{
  FL_GPIO_InitTypeDef GPIO_InitStruct;

  /* PB4 OUTPUT (LED) */
  GPIO_InitStruct.pin = FL_GPIO_PIN_4;
  GPIO_InitStruct.mode = FL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.pull = FL_DISABLE;
  GPIO_InitStruct.remapPin = FL_DISABLE;
  GPIO_InitStruct.analogSwitch = FL_DISABLE;
  FL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* PE5 INPUT (gongwei4) */
  GPIO_InitStruct.pin = FL_GPIO_PIN_5;
  GPIO_InitStruct.mode = FL_GPIO_MODE_INPUT;
  GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.pull = FL_ENABLE;
  GPIO_InitStruct.remapPin = FL_DISABLE;
  GPIO_InitStruct.analogSwitch = FL_DISABLE;
  FL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /* PB15 INPUT (gongwei3) */
  GPIO_InitStruct.pin = FL_GPIO_PIN_15;
  GPIO_InitStruct.mode = FL_GPIO_MODE_INPUT;
  GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.pull = FL_ENABLE;
  GPIO_InitStruct.remapPin = FL_DISABLE;
  GPIO_InitStruct.analogSwitch = FL_DISABLE;
  FL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* PD12 INPUT (gongwei2) */
  GPIO_InitStruct.pin = FL_GPIO_PIN_12;
  GPIO_InitStruct.mode = FL_GPIO_MODE_INPUT;
  GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.pull = FL_ENABLE;
  GPIO_InitStruct.remapPin = FL_DISABLE;
  GPIO_InitStruct.analogSwitch = FL_DISABLE;
  FL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /* PC0 INPUT (gongwei1) */
  GPIO_InitStruct.pin = FL_GPIO_PIN_0;
  GPIO_InitStruct.mode = FL_GPIO_MODE_INPUT;
  GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.pull = FL_ENABLE;
  GPIO_InitStruct.remapPin = FL_DISABLE;
  GPIO_InitStruct.analogSwitch = FL_DISABLE;
  FL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /* PC2 OUTPUT (12V_CTL2) */
  GPIO_InitStruct.pin = FL_GPIO_PIN_2;
  GPIO_InitStruct.mode = FL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.pull = FL_DISABLE;
  GPIO_InitStruct.remapPin = FL_DISABLE;
  GPIO_InitStruct.analogSwitch = FL_DISABLE;
  FL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /* PC3 OUTPUT (12V_CTL1) */
  GPIO_InitStruct.pin = FL_GPIO_PIN_3;
  GPIO_InitStruct.mode = FL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.pull = FL_DISABLE;
  GPIO_InitStruct.remapPin = FL_DISABLE;
  GPIO_InitStruct.analogSwitch = FL_DISABLE;
  FL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /* PC4 OUTPUT (Standby_Voltage_Output_Ctl) */
  GPIO_InitStruct.pin = FL_GPIO_PIN_4;
  GPIO_InitStruct.mode = FL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.pull = FL_DISABLE;
  GPIO_InitStruct.remapPin = FL_DISABLE;
  GPIO_InitStruct.analogSwitch = FL_DISABLE;
  FL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /* PC5 OUTPUT (Main_Voltage_Output_Ctl) */
  GPIO_InitStruct.pin = FL_GPIO_PIN_5;
  GPIO_InitStruct.mode = FL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.pull = FL_DISABLE;
  GPIO_InitStruct.remapPin = FL_DISABLE;
  GPIO_InitStruct.analogSwitch = FL_DISABLE;
  FL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /* PE2 OUTPUT (Current_Standby_I_CHK_CTL) */
  GPIO_InitStruct.pin = FL_GPIO_PIN_2;
  GPIO_InitStruct.mode = FL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.pull = FL_DISABLE;
  GPIO_InitStruct.remapPin = FL_DISABLE;
  GPIO_InitStruct.analogSwitch = FL_DISABLE;
  FL_GPIO_Init(GPIOE, &GPIO_InitStruct);

  /* PE3 OUTPUT (Current_Main_I_CHK_CTL) */
  GPIO_InitStruct.pin = FL_GPIO_PIN_3;
  GPIO_InitStruct.mode = FL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.pull = FL_DISABLE;
  GPIO_InitStruct.remapPin = FL_DISABLE;
  GPIO_InitStruct.analogSwitch = FL_DISABLE;
  FL_GPIO_Init(GPIOE, &GPIO_InitStruct);
}

/*————————主电电压输出控制————————*/
void Main_Voltage_Output_Ctl(uint8_t State)
{
  switch (State)
  {
  case On:
    FL_GPIO_SetOutputPin(GPIOC, FL_GPIO_PIN_5);
    break;
  case Off:
    FL_GPIO_ResetOutputPin(GPIOC, FL_GPIO_PIN_5);
    break;
  default:
    break;
  }
}

/*————————备电电压输出控制————————*/
void Standby_Voltage_Output_Ctl(uint8_t State)
{
  switch (State)
  {
  case On:
    FL_GPIO_SetOutputPin(GPIOC, FL_GPIO_PIN_4);
    break;
  case Off:
    FL_GPIO_ResetOutputPin(GPIOC, FL_GPIO_PIN_4);
    break;
  default:
    break;
  }
}

/*————————主电功耗检测控制————————*/
void Main_Current_CHK_CTL(uint8_t State)
{
  switch (State)
  {
  case On:
    FL_GPIO_ResetOutputPin(GPIOE, FL_GPIO_PIN_3);
    break;
  case Off:
    FL_GPIO_SetOutputPin(GPIOE, FL_GPIO_PIN_3);
    break;
  default:
    break;
  }
}

/*————————备电功耗检测控制————————*/
void Standby_Current_CHK_CTL(uint8_t State)
{
  switch (State)
  {
  case On:
    FL_GPIO_ResetOutputPin(GPIOE, FL_GPIO_PIN_2);
    break;
  case Off:
    FL_GPIO_SetOutputPin(GPIOE, FL_GPIO_PIN_2);
    break;
  default:
    break;
  }
}

/*————————12V电磁铁电源输出控制1————————*/
void Output_12V_CTL1(uint8_t State)
{
  switch (State)
  {
  case On:
    FL_GPIO_SetOutputPin(GPIOC, FL_GPIO_PIN_3);
    break;
  case Off:
    FL_GPIO_ResetOutputPin(GPIOC, FL_GPIO_PIN_3);
    break;
  default:
    break;
  }
}

/*————————12V电磁铁电源输出控制2————————*/
void Output_12V_CTL2(uint8_t State)
{
  switch (State)
  {
  case On:
    FL_GPIO_SetOutputPin(GPIOC, FL_GPIO_PIN_2);
    break;
  case Off:
    FL_GPIO_ResetOutputPin(GPIOC, FL_GPIO_PIN_2);
    break;
  default:
    break;
  }
}
