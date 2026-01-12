#include "gpio.h"

void MF_GPIO_Init(void)
{
  FL_GPIO_InitTypeDef GPIO_InitStruct;

  /* PA3 OUTPUT (LED) */
  GPIO_InitStruct.pin = FL_GPIO_PIN_3;
  GPIO_InitStruct.mode = FL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.pull = FL_DISABLE;
  GPIO_InitStruct.remapPin = FL_DISABLE;
  GPIO_InitStruct.analogSwitch = FL_DISABLE;
  FL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* PA9 OUTPUT (备电电流检测控制) */
  GPIO_InitStruct.pin = FL_GPIO_PIN_9;
  GPIO_InitStruct.mode = FL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.pull = FL_DISABLE;
  GPIO_InitStruct.remapPin = FL_DISABLE;
  GPIO_InitStruct.analogSwitch = FL_DISABLE;
  FL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* PA10 OUTPUT (主电电流检测控制) */
  GPIO_InitStruct.pin = FL_GPIO_PIN_10;
  GPIO_InitStruct.mode = FL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.pull = FL_DISABLE;
  GPIO_InitStruct.remapPin = FL_DISABLE;
  GPIO_InitStruct.analogSwitch = FL_DISABLE;
  FL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* PB5 INPUT (gongwei4) */
  GPIO_InitStruct.pin = FL_GPIO_PIN_5;
  GPIO_InitStruct.mode = FL_GPIO_MODE_INPUT;
  GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.pull = FL_ENABLE;
  GPIO_InitStruct.remapPin = FL_DISABLE;
  GPIO_InitStruct.analogSwitch = FL_DISABLE;
  FL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* PB6 INPUT (gongwei3) */
  GPIO_InitStruct.pin = FL_GPIO_PIN_6;
  GPIO_InitStruct.mode = FL_GPIO_MODE_INPUT;
  GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.pull = FL_ENABLE;
  GPIO_InitStruct.remapPin = FL_DISABLE;
  GPIO_InitStruct.analogSwitch = FL_DISABLE;
  FL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* PB7 INPUT (gongwei2) */
  GPIO_InitStruct.pin = FL_GPIO_PIN_7;
  GPIO_InitStruct.mode = FL_GPIO_MODE_INPUT;
  GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.pull = FL_ENABLE;
  GPIO_InitStruct.remapPin = FL_DISABLE;
  GPIO_InitStruct.analogSwitch = FL_DISABLE;
  FL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* PB8 INPUT (gongwei1) */
  GPIO_InitStruct.pin = FL_GPIO_PIN_8;
  GPIO_InitStruct.mode = FL_GPIO_MODE_INPUT;
  GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.pull = FL_ENABLE;
  GPIO_InitStruct.remapPin = FL_DISABLE;
  GPIO_InitStruct.analogSwitch = FL_DISABLE;
  FL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* PB9 OUTPUT (备电电压输出控制) */
  GPIO_InitStruct.pin = FL_GPIO_PIN_9;
  GPIO_InitStruct.mode = FL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.pull = FL_DISABLE;
  GPIO_InitStruct.remapPin = FL_DISABLE;
  GPIO_InitStruct.analogSwitch = FL_DISABLE;
  FL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* PB10 OUTPUT (主电电压输出控制) */
  GPIO_InitStruct.pin = FL_GPIO_PIN_10;
  GPIO_InitStruct.mode = FL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.pull = FL_DISABLE;
  GPIO_InitStruct.remapPin = FL_DISABLE;
  GPIO_InitStruct.analogSwitch = FL_DISABLE;
  FL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* PB11 OUTPUT (12V_CTL) */
  GPIO_InitStruct.pin = FL_GPIO_PIN_11;
  GPIO_InitStruct.mode = FL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.pull = FL_DISABLE;
  GPIO_InitStruct.remapPin = FL_DISABLE;
  GPIO_InitStruct.analogSwitch = FL_DISABLE;
  FL_GPIO_Init(GPIOB, &GPIO_InitStruct);

  /* PC3 OUTPUT (AD1_CTL) */
  GPIO_InitStruct.pin = FL_GPIO_PIN_3;
  GPIO_InitStruct.mode = FL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.pull = FL_DISABLE;
  GPIO_InitStruct.remapPin = FL_DISABLE;
  GPIO_InitStruct.analogSwitch = FL_DISABLE;
  FL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /* PC4 OUTPUT (AD2_CTL) */
  GPIO_InitStruct.pin = FL_GPIO_PIN_4;
  GPIO_InitStruct.mode = FL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.pull = FL_DISABLE;
  GPIO_InitStruct.remapPin = FL_DISABLE;
  GPIO_InitStruct.analogSwitch = FL_DISABLE;
  FL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /* PC5 OUTPUT (AD3_CTL) */
  GPIO_InitStruct.pin = FL_GPIO_PIN_5;
  GPIO_InitStruct.mode = FL_GPIO_MODE_OUTPUT;
  GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.pull = FL_DISABLE;
  GPIO_InitStruct.remapPin = FL_DISABLE;
  GPIO_InitStruct.analogSwitch = FL_DISABLE;
  FL_GPIO_Init(GPIOC, &GPIO_InitStruct);

  /* PD5 INPUT (MOTOR_B_CHK) */
  GPIO_InitStruct.pin = FL_GPIO_PIN_5;
  GPIO_InitStruct.mode = FL_GPIO_MODE_INPUT;
  GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.pull = FL_DISABLE;
  GPIO_InitStruct.remapPin = FL_DISABLE;
  GPIO_InitStruct.analogSwitch = FL_DISABLE;
  (void)FL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /* PD6 INPUT (MOTOR_R_CHK) */
  GPIO_InitStruct.pin = FL_GPIO_PIN_6;
  GPIO_InitStruct.mode = FL_GPIO_MODE_INPUT;
  GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.pull = FL_DISABLE;
  GPIO_InitStruct.remapPin = FL_DISABLE;
  GPIO_InitStruct.analogSwitch = FL_DISABLE;
  (void)FL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  Output_7V2_CTL(On);
  Output_3V6_CTL(On);
  Current_7V2_I_CHK_CTL(Off);
  Current_3V6_I_CHK_CTL(Off);
  ADC_CHK_CTL(AD1, Off);
  ADC_CHK_CTL(AD2, Off);
  ADC_CHK_CTL(AD3, Off);
  Output_12V_CTL(Off);
}

/*————————备电功耗检测控制————————*/
void Current_3V6_I_CHK_CTL(uint8_t State)
{
  switch (State)
  {
  case On:
    FL_GPIO_ResetOutputPin(GPIOA, FL_GPIO_PIN_9);
    break;
  case Off:
    FL_GPIO_SetOutputPin(GPIOA, FL_GPIO_PIN_9);
    break;
  default:
    break;
  }
}

/*————————主电功耗检测控制————————*/
void Current_7V2_I_CHK_CTL(uint8_t State)
{
  switch (State)
  {
  case On:
    FL_GPIO_ResetOutputPin(GPIOA, FL_GPIO_PIN_10);
    break;
  case Off:
    FL_GPIO_SetOutputPin(GPIOA, FL_GPIO_PIN_10);
    break;
  default:
    break;
  }
}

/*————————备电电源输出控制————————*/
void Output_3V6_CTL(uint8_t State)
{
  switch (State)
  {
  case On:
    FL_GPIO_ResetOutputPin(GPIOB, FL_GPIO_PIN_9);
    break;
  case Off:
    FL_GPIO_SetOutputPin(GPIOB, FL_GPIO_PIN_9);
    break;
  default:
    break;
  }
}

/*————————主电电源输出控制————————*/
void Output_7V2_CTL(uint8_t State)
{
  switch (State)
  {
  case On:
    FL_GPIO_ResetOutputPin(GPIOB, FL_GPIO_PIN_10);
    break;
  case Off:
    FL_GPIO_SetOutputPin(GPIOB, FL_GPIO_PIN_10);
    break;
  default:
    break;
  }
}

/*————————12V电磁铁电源输出控制————————*/
void Output_12V_CTL(uint8_t State)
{
  switch (State)
  {
  case On:
    FL_GPIO_ResetOutputPin(GPIOB, FL_GPIO_PIN_11);
    break;
  case Off:
    FL_GPIO_SetOutputPin(GPIOB, FL_GPIO_PIN_11);
    break;
  default:
    break;
  }
}

/*————————ADC检测控制————————*/
void ADC_CHK_CTL(uint8_t Num, uint8_t State)
{
  switch (Num)
  {
  case AD1:
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
    break;
  case AD2:
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
    break;
  case AD3:
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
    break;
  default:
    break;
  }
}

/*————————阀门高低电平测试————————*/
uint8_t Motor_CHK(uint8_t Motor_Pin)
{
  uint8_t Motor_State = 0;

  switch (Motor_Pin)
  {
  case MOTOR_B:
    Motor_State = FL_GPIO_GetInputPin(GPIOD, FL_GPIO_PIN_5);
    break;
  case MOTOR_R:
    Motor_State = FL_GPIO_GetInputPin(GPIOD, FL_GPIO_PIN_6);
    break;
  default:
    break;
  }

  return Motor_State;
}
