#include "test.h"
#include "gpio.h"
#include "ina219.h"
#include "ir.h"
#include "measurement.h"
#include "adc.h"
#include "led.h"

/*————————结构体枚举定义————————*/
extern struct Test Test_result;
extern enum Test1 Test_process;
extern enum Test2 Test_agreement_receive;

/*————————定时器全局变量计时————————*/
extern uint32_t single_step_timing; // 单步测试时间
extern uint32_t interval_timing;	 // 间隔测试时间

extern uint8_t repeat_report; // 重复上告计数

void Test_Function_CAT1(void)
{
}

