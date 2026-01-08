#include "LED_CTRL.h"
#include "GPIO.h"

uint8_t LED_thing_FLAG = 0;
uint8_t LED_thing_time = 0;

void LED_FLAG_Run() {
  LED_On();
  LED_thing_FLAG = 1;
  //灯会亮一下
  LED_thing_time = 20;
}
void LED_FLAG_LOOP() {
  if (LED_thing_FLAG == 0)
    return;
  if (LED_thing_time != 0)
    return;
  LED_thing_FLAG = 0;
  LED_Off();
}
