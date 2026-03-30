#include "led.h"
#include "gpio.h"
#include "main.h"

uint16_t led_time_count = 0;

void LED_Sign_Open(void)
{
	FL_GPIO_ResetOutputPin(GPIOB, FL_GPIO_PIN_4);
	led_time_count = 400; // 200ms
}

void LED_Sign_Close(void)
{
	if (led_time_count == 0)
		FL_GPIO_SetOutputPin(GPIOB, FL_GPIO_PIN_4);
}
