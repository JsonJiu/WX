#include "GPIO.h"
#include "ZDINA219.h"
void Others_GPIO_Init(void)
{
    FL_GPIO_InitTypeDef    GPIO_InitStruct;

    /* PA2 OUTPUT (RS2103_CTL) */ 
    GPIO_InitStruct.pin = FL_GPIO_PIN_2;
    GPIO_InitStruct.mode = FL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.pull = FL_DISABLE;
    GPIO_InitStruct.remapPin = FL_DISABLE;
    GPIO_InitStruct.analogSwitch = FL_DISABLE;
    FL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* PA3 OUTPUT (RS2103_POWER) */ 
    GPIO_InitStruct.pin = FL_GPIO_PIN_3;
    GPIO_InitStruct.mode = FL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.pull = FL_DISABLE;
    GPIO_InitStruct.remapPin = FL_DISABLE;
    GPIO_InitStruct.analogSwitch = FL_DISABLE;
    FL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* PA11 OUTPUT (OPEN) */ 
    GPIO_InitStruct.pin = FL_GPIO_PIN_11;
    GPIO_InitStruct.mode = FL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.pull = FL_DISABLE;
    GPIO_InitStruct.remapPin = FL_DISABLE;
    GPIO_InitStruct.analogSwitch = FL_DISABLE;
    FL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* PA12 OUTPUT (CLOSE) */ 
    GPIO_InitStruct.pin = FL_GPIO_PIN_12;
    GPIO_InitStruct.mode = FL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.pull = FL_DISABLE;
    GPIO_InitStruct.remapPin = FL_DISABLE;
    GPIO_InitStruct.analogSwitch = FL_DISABLE;
    FL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    /* PB4 OUTPUT (LED) */ 
    GPIO_InitStruct.pin = FL_GPIO_PIN_4;
    GPIO_InitStruct.mode = FL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.pull = FL_DISABLE;
    GPIO_InitStruct.remapPin = FL_DISABLE;
    GPIO_InitStruct.analogSwitch = FL_DISABLE;
    FL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    /* PB15 INPUT (gongwei3) */ 
    GPIO_InitStruct.pin = FL_GPIO_PIN_15;
    GPIO_InitStruct.mode = FL_GPIO_MODE_INPUT;
    GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.pull = FL_ENABLE;
    GPIO_InitStruct.remapPin = FL_DISABLE;
    GPIO_InitStruct.analogSwitch = FL_DISABLE;
    FL_GPIO_Init(GPIOB, &GPIO_InitStruct);

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

    /* PC4 OUTPUT (Pressure_Power_CTL) */ 
    GPIO_InitStruct.pin = FL_GPIO_PIN_4;
    GPIO_InitStruct.mode = FL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.pull = FL_DISABLE;
    GPIO_InitStruct.remapPin = FL_DISABLE;
    GPIO_InitStruct.analogSwitch = FL_DISABLE;
    FL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    /* PC5 OUTPUT (VBAT_CTL) */ 
    GPIO_InitStruct.pin = FL_GPIO_PIN_5;
    GPIO_InitStruct.mode = FL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.pull = FL_DISABLE;
    GPIO_InitStruct.remapPin = FL_DISABLE;
    GPIO_InitStruct.analogSwitch = FL_DISABLE;
    FL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    /* PC14 OUTPUT (SENSOR_P) */ 
    GPIO_InitStruct.pin = FL_GPIO_PIN_14;
    GPIO_InitStruct.mode = FL_GPIO_MODE_INPUT;
    GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.pull = FL_DISABLE;
    GPIO_InitStruct.remapPin = FL_DISABLE;
    GPIO_InitStruct.analogSwitch = FL_DISABLE;
    FL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    /* PC15 OUTPUT (SENSOR_D) */ 
    GPIO_InitStruct.pin = FL_GPIO_PIN_15;
    GPIO_InitStruct.mode = FL_GPIO_MODE_INPUT;
    GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.pull = FL_DISABLE;
    GPIO_InitStruct.remapPin = FL_DISABLE;
    GPIO_InitStruct.analogSwitch = FL_DISABLE;
    FL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    /* PD1 OUTPUT (SENSOR_T) */ 
    GPIO_InitStruct.pin = FL_GPIO_PIN_1;
    GPIO_InitStruct.mode = FL_GPIO_MODE_INPUT;
    GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.pull = FL_DISABLE;
    GPIO_InitStruct.remapPin = FL_DISABLE;
    GPIO_InitStruct.analogSwitch = FL_DISABLE;
    FL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    /* PD12 INPUT (gongwei2) */ 
    GPIO_InitStruct.pin = FL_GPIO_PIN_12;
    GPIO_InitStruct.mode = FL_GPIO_MODE_INPUT;
    GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.pull = FL_ENABLE;
    GPIO_InitStruct.remapPin = FL_DISABLE;
    GPIO_InitStruct.analogSwitch = FL_DISABLE;
    FL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    /* PE0 OUTPUT (Ext_VCC_CHK_Ctl) */ 
    GPIO_InitStruct.pin = FL_GPIO_PIN_0;
    GPIO_InitStruct.mode = FL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.pull = FL_DISABLE;
    GPIO_InitStruct.remapPin = FL_DISABLE;
    GPIO_InitStruct.analogSwitch = FL_DISABLE;
    FL_GPIO_Init(GPIOE, &GPIO_InitStruct);

    /* PE1 OUTPUT (COL_PRE_CHK_CTR) */ 
    GPIO_InitStruct.pin = FL_GPIO_PIN_1;
    GPIO_InitStruct.mode = FL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.pull = FL_DISABLE;
    GPIO_InitStruct.remapPin = FL_DISABLE;
    GPIO_InitStruct.analogSwitch = FL_DISABLE;
    FL_GPIO_Init(GPIOE, &GPIO_InitStruct);

    /* PE2 OUTPUT (3.6V_I_CHK_CTR2) */ 
    GPIO_InitStruct.pin = FL_GPIO_PIN_2;
    GPIO_InitStruct.mode = FL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.pull = FL_DISABLE;
    GPIO_InitStruct.remapPin = FL_DISABLE;
    GPIO_InitStruct.analogSwitch = FL_DISABLE;
    FL_GPIO_Init(GPIOE, &GPIO_InitStruct);
		

    /* PE3 OUTPUT (3.6V_I_CHK_CTR1) */ 
    GPIO_InitStruct.pin = FL_GPIO_PIN_3;
    GPIO_InitStruct.mode = FL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.pull = FL_DISABLE;
    GPIO_InitStruct.remapPin = FL_DISABLE;
    GPIO_InitStruct.analogSwitch = FL_DISABLE;
    FL_GPIO_Init(GPIOE, &GPIO_InitStruct);

    /* PE5 INPUT (gongwei4) */ 
    GPIO_InitStruct.pin = FL_GPIO_PIN_5;
    GPIO_InitStruct.mode = FL_GPIO_MODE_INPUT;
    GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.pull = FL_ENABLE;
    GPIO_InitStruct.remapPin = FL_DISABLE;
    GPIO_InitStruct.analogSwitch = FL_DISABLE;
    FL_GPIO_Init(GPIOE, &GPIO_InitStruct);    

		//上电先关灯
		LED_Off();
		//主电源供电断开
		zhudian_gongdian_OFF();
		//备电电源断开，这里是提供压力检测电压关闭
		beidian_gongdian_OFF();
		//主电电压检测控制关闭
		zhudian_dianya_CHK_CTRL_OFF();
		//二级电源电压检测控制关闭，这里是检测压力电压关闭
		erji_dianya_CHK_CTRL_OFF();
		//主电功耗检测使能关闭
		Current_CHK_CTRL_OFF();
		//备电功耗检测使能关闭
		NB_Current_CHK_CTRL_OFF();
		//SENSOR_P_High();
		//SENSOR_D_High();
		//霍尔1控制 关
		huuoer_1_OFF();
		//霍尔2控制 关
		huuoer_2_OFF();
		//RS2103关闭
		RS2103_POWER_OFF();
		//功耗检测的IIC初始化
		INA219_IIC_GPIO_Init();
}
//LED
void LED_On(void)
{
	FL_GPIO_ResetOutputPin(GPIOB,FL_GPIO_PIN_4);
}
void LED_Off(void)
{
	FL_GPIO_SetOutputPin(GPIOB,FL_GPIO_PIN_4);
}

//主控板主电供电
void zhudian_gongdian_On(void)
{
	FL_GPIO_SetOutputPin(GPIOC,FL_GPIO_PIN_5);
}
void zhudian_gongdian_OFF(void)
{
	FL_GPIO_ResetOutputPin(GPIOC,FL_GPIO_PIN_5);
}

//压力传感器测试供电
void beidian_gongdian_On(void)
{
	FL_GPIO_SetOutputPin(GPIOC,FL_GPIO_PIN_4);
}
void beidian_gongdian_OFF(void)
{
	FL_GPIO_ResetOutputPin(GPIOC,FL_GPIO_PIN_4);
}

//主控板主电电压检测
void zhudian_dianya_CHK_CTRL_ON(void)
{
	FL_GPIO_SetOutputPin(GPIOE,FL_GPIO_PIN_0);
}
void zhudian_dianya_CHK_CTRL_OFF(void)
{
	FL_GPIO_ResetOutputPin(GPIOE,FL_GPIO_PIN_0);
}

//压力传感器电源电压检测
void erji_dianya_CHK_CTRL_ON(void)
{
	FL_GPIO_SetOutputPin(GPIOE,FL_GPIO_PIN_1);
}
void erji_dianya_CHK_CTRL_OFF(void)
{
	FL_GPIO_ResetOutputPin(GPIOE,FL_GPIO_PIN_1);
}

//主电电流检测使能开
void Current_CHK_CTRL_ON(void)
{
	FL_GPIO_ResetOutputPin(GPIOE,FL_GPIO_PIN_3);
}
void Current_CHK_CTRL_OFF(void)
{
	FL_GPIO_SetOutputPin(GPIOE,FL_GPIO_PIN_3);
}

//备电电流检测
void NB_Current_CHK_CTRL_ON(void)
{
	FL_GPIO_ResetOutputPin(GPIOE,FL_GPIO_PIN_2);
}
void NB_Current_CHK_CTRL_OFF(void)
{
	FL_GPIO_ResetOutputPin(GPIOE,FL_GPIO_PIN_2);
}

//霍尔1控制
void huuoer_1_ON(void)
{
	FL_GPIO_SetOutputPin(GPIOC,FL_GPIO_PIN_3);
}
void huuoer_1_OFF(void)
{
	FL_GPIO_ResetOutputPin(GPIOC,FL_GPIO_PIN_3);
}

//霍尔2控制
void huuoer_2_ON(void)
{
	FL_GPIO_SetOutputPin(GPIOC,FL_GPIO_PIN_2);
}
void huuoer_2_OFF(void)
{
	FL_GPIO_ResetOutputPin(GPIOC,FL_GPIO_PIN_2);
}

//开到位控制
void daowei_OPEN_High(void)
{
	FL_GPIO_SetOutputPin(GPIOA,FL_GPIO_PIN_11);
}
void daowei_OPEN_Low(void)
{
	FL_GPIO_ResetOutputPin(GPIOA,FL_GPIO_PIN_11);
}

//关到位控制
void daowei_CLOSE_High(void)
{
	FL_GPIO_SetOutputPin(GPIOA,FL_GPIO_PIN_12);
}
void daowei_CLOSE_Low(void)
{
	FL_GPIO_ResetOutputPin(GPIOA,FL_GPIO_PIN_12);
}

//开到位
void kai_daowei(void)
{
	daowei_OPEN_Low();
	daowei_CLOSE_High();
}
//关到位
void guan_daowei(void)
{
	daowei_CLOSE_Low();
	daowei_OPEN_High();
}

//RS2103电源
void RS2103_POWER_ON(void)
{
	FL_GPIO_SetOutputPin(GPIOA,FL_GPIO_PIN_3);
}
void RS2103_POWER_OFF(void)
{
	FL_GPIO_ResetOutputPin(GPIOA,FL_GPIO_PIN_3);
}

//RS2103选择
void RS2103_Sel_wuci(void)
{
	FL_GPIO_SetOutputPin(GPIOA,FL_GPIO_PIN_2);
}
void RS2103_Sel_youci(void)
{
	FL_GPIO_ResetOutputPin(GPIOA,FL_GPIO_PIN_2);
}

//霍尔1控制
void SENSOR_P_High(void)
{
	FL_GPIO_SetOutputPin(GPIOC,FL_GPIO_PIN_14);
}
void SENSOR_P_Low(void)
{
	FL_GPIO_ResetOutputPin(GPIOC,FL_GPIO_PIN_14);
}
//霍尔2控制
void SENSOR_D_High(void)
{
	FL_GPIO_SetOutputPin(GPIOC,FL_GPIO_PIN_15);
}
void SENSOR_D_Low(void)
{
	FL_GPIO_ResetOutputPin(GPIOC,FL_GPIO_PIN_15);
}
//磁干扰控制
void SENSOR_T_High(void)
{
	FL_GPIO_SetOutputPin(GPIOD,FL_GPIO_PIN_1);
}
void SENSOR_T_Low(void)
{
	FL_GPIO_ResetOutputPin(GPIOD,FL_GPIO_PIN_1);
}



