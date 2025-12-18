#include "pwm.h"
#include "gpio.h"

/*————————舵机使用说明————————*/
/*
舵机控制需要PWM频率为50Hz，即周期为20ms
ATIM每0.5ms触发一次中断，即触发20次为PWM的一个周期
可以用GPIO的输出时间即在20次中断中的占用时间来模拟输出PWM
舵机角度和高电平时间关系：
0.5ms——0度
1.0ms——45度
1.5ms——90度
*/


uint8_t PWM_Low = 0;
uint8_t PWM_Count = 0;

/*————————舵机角度选择————————*/
/*
PWM占空比控制角度
PWM_Low = 39 ———— 0度
PWM_Low = 38 ———— 45度
PWM_Low = 37 ———— 90度
*/
void Motor_Angle_Select(uint8_t angle)
{
	switch(angle)
	{
		case 0: 
			PWM_Low = 39;
			break;
		case 45:
			PWM_Low = 38;
			break;
		case 90:
			PWM_Low = 37;
			break;
		default:
			break;
	}
	PWM_Count = 40;//设置PWM周期为20ms
}

/*————————PWM模拟电平————————*/
void PWM_Output(uint8_t Level)
{
	switch(Level)
	{
		case High:
			FL_GPIO_SetOutputPin(GPIOB,FL_GPIO_PIN_4); 
			break;
		case Low:
			FL_GPIO_ResetOutputPin(GPIOB,FL_GPIO_PIN_4); 
			break;
		default:
			break;
	}
}
