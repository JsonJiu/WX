#include "gpio.h"




void MF_GPIO_Init(void)
{
    FL_GPIO_InitTypeDef    GPIO_InitStruct;

    /* PA3 OUTPUT (LED) */ 
    GPIO_InitStruct.pin = FL_GPIO_PIN_3;
    GPIO_InitStruct.mode = FL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.pull = FL_DISABLE;
    GPIO_InitStruct.remapPin = FL_DISABLE;
    GPIO_InitStruct.analogSwitch = FL_DISABLE;
    FL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* PA9 OUTPUT (3.6V_I_CHK_CTL) */ 
    GPIO_InitStruct.pin = FL_GPIO_PIN_9;
    GPIO_InitStruct.mode = FL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.pull = FL_DISABLE;
    GPIO_InitStruct.remapPin = FL_DISABLE;
    GPIO_InitStruct.analogSwitch = FL_DISABLE;
    FL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* PA10 OUTPUT (7.2V_I_CHK_CTL) */ 
    GPIO_InitStruct.pin = FL_GPIO_PIN_10;
    GPIO_InitStruct.mode = FL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.pull = FL_DISABLE;
    GPIO_InitStruct.remapPin = FL_DISABLE;
    GPIO_InitStruct.analogSwitch = FL_DISABLE;
    FL_GPIO_Init(GPIOA, &GPIO_InitStruct);

		/* PB4 OUTPUT (PWM) */ 
    GPIO_InitStruct.pin = FL_GPIO_PIN_4;
    GPIO_InitStruct.mode = FL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.pull = FL_DISABLE;
    GPIO_InitStruct.remapPin = FL_DISABLE;
    GPIO_InitStruct.analogSwitch = FL_DISABLE;
    FL_GPIO_Init(GPIOB, &GPIO_InitStruct);
		
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

    /* PB9 OUTPUT (Vout_3V6_CTL) */ 
    GPIO_InitStruct.pin = FL_GPIO_PIN_9;
    GPIO_InitStruct.mode = FL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.pull = FL_DISABLE;
    GPIO_InitStruct.remapPin = FL_DISABLE;
    GPIO_InitStruct.analogSwitch = FL_DISABLE;
    FL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* PB10 OUTPUT (Vout_7V2_CTL) */ 
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
    /* PC2 OUTPUT (AD4_CTL) */ 
    GPIO_InitStruct.pin = FL_GPIO_PIN_2;
    GPIO_InitStruct.mode = FL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.pull = FL_DISABLE;
    GPIO_InitStruct.remapPin = FL_DISABLE;
    GPIO_InitStruct.analogSwitch = FL_DISABLE;
    FL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    /* PC10 OUTPUT (Ext_LED_CTL) */ 
    GPIO_InitStruct.pin = FL_GPIO_PIN_10;
    GPIO_InitStruct.mode = FL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.pull = FL_DISABLE;
    GPIO_InitStruct.remapPin = FL_DISABLE;
    GPIO_InitStruct.analogSwitch = FL_DISABLE;
    FL_GPIO_Init(GPIOC, &GPIO_InitStruct);    
    /* PC14 OUTPUT (12V_CTL1) */ 
    GPIO_InitStruct.pin = FL_GPIO_PIN_14;
    GPIO_InitStruct.mode = FL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.pull = FL_DISABLE;
    GPIO_InitStruct.remapPin = FL_DISABLE;
    GPIO_InitStruct.analogSwitch = FL_DISABLE;
    FL_GPIO_Init(GPIOC, &GPIO_InitStruct);
    /* PC15 OUTPUT (12V_CTL2) */ 
    GPIO_InitStruct.pin = FL_GPIO_PIN_15;
    GPIO_InitStruct.mode = FL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.pull = FL_DISABLE;
    GPIO_InitStruct.remapPin = FL_DISABLE;
    GPIO_InitStruct.analogSwitch = FL_DISABLE;
    FL_GPIO_Init(GPIOC, &GPIO_InitStruct);
    /* PE1 OUTPUT (A119_CTL) */ 
    GPIO_InitStruct.pin = FL_GPIO_PIN_1;
    GPIO_InitStruct.mode = FL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.pull = FL_DISABLE;
    GPIO_InitStruct.remapPin = FL_DISABLE;
    GPIO_InitStruct.analogSwitch = FL_DISABLE;
    FL_GPIO_Init(GPIOE, &GPIO_InitStruct);
     /* PC13 OUTPUT (S+) */ 
    GPIO_InitStruct.pin = FL_GPIO_PIN_13;
    GPIO_InitStruct.mode = FL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.pull = FL_DISABLE;
    GPIO_InitStruct.remapPin = FL_DISABLE;
    GPIO_InitStruct.analogSwitch = FL_DISABLE;
    FL_GPIO_Init(GPIOC, &GPIO_InitStruct);
    /* PE0 OUTPUT (TP4) */ 
    GPIO_InitStruct.pin = FL_GPIO_PIN_0;
    GPIO_InitStruct.mode = FL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.pull = FL_DISABLE;
    GPIO_InitStruct.remapPin = FL_DISABLE;
    GPIO_InitStruct.analogSwitch = FL_DISABLE;
    FL_GPIO_Init(GPIOE, &GPIO_InitStruct);
    /* PE2 OUTPUT (TP5) */ 
    GPIO_InitStruct.pin = FL_GPIO_PIN_2;
    GPIO_InitStruct.mode = FL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.pull = FL_DISABLE;
    GPIO_InitStruct.remapPin = FL_DISABLE;
    GPIO_InitStruct.analogSwitch = FL_DISABLE;
    FL_GPIO_Init(GPIOE, &GPIO_InitStruct);
   /* PE5 OUTPUT (PGND) */ 
    GPIO_InitStruct.pin = FL_GPIO_PIN_5;
    GPIO_InitStruct.mode = FL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.pull = FL_DISABLE;
    GPIO_InitStruct.remapPin = FL_DISABLE;
    GPIO_InitStruct.analogSwitch = FL_DISABLE;
    FL_GPIO_Init(GPIOE, &GPIO_InitStruct);        
}
 
/*————————3.6V功耗检测控制————————*/
void Current_3V6_I_CHK_CTL(uint8_t State)
{
	switch(State)
	{
		case On:
			FL_GPIO_ResetOutputPin(GPIOA,FL_GPIO_PIN_9); 
			break;
		case Off:
			FL_GPIO_SetOutputPin(GPIOA,FL_GPIO_PIN_9); 
			break;
		default:
			break;
	}
}

/*————————7.2V功耗检测控制————————*/
void Current_7V2_I_CHK_CTL(uint8_t State)
{
	switch(State)
	{
		case On:
			FL_GPIO_ResetOutputPin(GPIOA,FL_GPIO_PIN_10); 
			break;
		case Off:
			FL_GPIO_SetOutputPin(GPIOA,FL_GPIO_PIN_10); 
			break;
		default:
			break;
	}
}

/*————————3.6V电源输出控制————————*/
void Output_3V6_CTL(uint8_t State)
{
	switch(State)
	{
		case On:
			FL_GPIO_ResetOutputPin(GPIOB,FL_GPIO_PIN_9); 
			break;
		case Off:
			FL_GPIO_SetOutputPin(GPIOB,FL_GPIO_PIN_9); 
			break;
		default:
			break;
	}
}

/*————————7.2V电源输出控制————————*/
void Output_7V2_CTL(uint8_t State)
{
	switch(State)
	{
		case On:
			FL_GPIO_ResetOutputPin(GPIOB,FL_GPIO_PIN_10); 
			break;
		case Off:
			FL_GPIO_SetOutputPin(GPIOB,FL_GPIO_PIN_10); 
			break;
		default:
			break;
	}
}

/*————————12V电磁铁电源输出控制————————*/
void Output_12V_CTL(uint8_t State)
{
	switch(State)
	{
		case On:
			FL_GPIO_ResetOutputPin(GPIOB,FL_GPIO_PIN_11); 
			break;
		case Off:
			FL_GPIO_SetOutputPin(GPIOB,FL_GPIO_PIN_11); 
			break;
		default:
			break;
	}
}

/*————————12V电磁铁1输出控制————————*/
void Output_12V_CTL1(uint8_t State)
{
	switch(State)
	{
		case On:
			FL_GPIO_SetOutputPin(GPIOC,FL_GPIO_PIN_14); 
			break;
		case Off:
			FL_GPIO_ResetOutputPin(GPIOC,FL_GPIO_PIN_14); 
			break;
		default:
			break;
	}
}

/*————————12V电磁铁2输出控制————————*/
void Output_12V_CTL2(uint8_t State)
{
	switch(State)
	{
		case On:
			FL_GPIO_SetOutputPin(GPIOC,FL_GPIO_PIN_15); 
			break;
		case Off:
			FL_GPIO_ResetOutputPin(GPIOC,FL_GPIO_PIN_15); 
			break;
		default:
			break;
	}
}

/*————————ADC检测控制————————*/
void ADC_CHK_CTL(uint8_t Num,uint8_t State)
{
	switch(Num)
	{
		case AD1:
			switch(State)
			{
				case On:
					FL_GPIO_SetOutputPin(GPIOC,FL_GPIO_PIN_3); 
					break;
				case Off:
					FL_GPIO_ResetOutputPin(GPIOC,FL_GPIO_PIN_3);
					break;
				default:
					break;
			}
			break;
		case AD2:
			switch(State)
			{
				case On:
					FL_GPIO_SetOutputPin(GPIOC,FL_GPIO_PIN_4);
					break;
				case Off:
					FL_GPIO_ResetOutputPin(GPIOC,FL_GPIO_PIN_4);
					break;
				default:
					break;
			}
			break;
		case AD3:
			switch(State)
			{
				case On:
					FL_GPIO_SetOutputPin(GPIOC,FL_GPIO_PIN_5);
					break;
				case Off:
					FL_GPIO_ResetOutputPin(GPIOC,FL_GPIO_PIN_5);
					break;
				default:
					break;
			}
      case AD4:
			switch(State)
			{
				case On:
					FL_GPIO_SetOutputPin(GPIOC,FL_GPIO_PIN_2);
					break;
				case Off:
					FL_GPIO_ResetOutputPin(GPIOC,FL_GPIO_PIN_2);
					break;
				default:
					break;
			}
			break;
		default:
			break;	
	}
}

/*————————主控板LED控制————————*/
void Ext_LED_CTL(uint8_t State)
{
	switch(State)
	{
		case On:
			FL_GPIO_SetOutputPin(GPIOC,FL_GPIO_PIN_10); 
			break;
		case Off:
			FL_GPIO_ResetOutputPin(GPIOC,FL_GPIO_PIN_10); 
			break;
		default:
			break;
	}
}
/*————————119控制————————*/
void A119_CTL(uint8_t State)
{
	switch(State)
	{
		case On:
			FL_GPIO_SetOutputPin(GPIOE,FL_GPIO_PIN_1); 
			break;
		case Off:
			FL_GPIO_ResetOutputPin(GPIOE,FL_GPIO_PIN_1); 
			break;
		default:
			break;
	}
}
/*————————掉电检测控制————————*/
void Brownout_detect(uint8_t State)
{
	switch(State)
	{
		case On:
			FL_GPIO_SetOutputPin(GPIOC,FL_GPIO_PIN_13); 
			FL_GPIO_SetOutputPin(GPIOE,FL_GPIO_PIN_0); 
			FL_GPIO_SetOutputPin(GPIOE,FL_GPIO_PIN_2); 
			FL_GPIO_SetOutputPin(GPIOE,FL_GPIO_PIN_5); 

			break;
		case Off:
			FL_GPIO_ResetOutputPin(GPIOC,FL_GPIO_PIN_13);
      FL_GPIO_ResetOutputPin(GPIOE,FL_GPIO_PIN_0);
      FL_GPIO_ResetOutputPin(GPIOE,FL_GPIO_PIN_2);
      FL_GPIO_ResetOutputPin(GPIOE,FL_GPIO_PIN_5);
			break;
		default:
			break;
	}
}
