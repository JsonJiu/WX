#ifndef _INA219_H
#define _INA219_H
#include "main.h"

// ===== INA219 引脚配置 =====
// 主电INA219: SCL=PC8, SDA=PC9 (线路板网络名: INA1_SCL, INA1_SDA)
#define INA219_MAIN_PORT GPIOC
#define INA219_MAIN_SCL_PIN FL_GPIO_PIN_8
#define INA219_MAIN_SDA_PIN FL_GPIO_PIN_9
// NB电INA219(备电): SCL=PC6, SDA=PC7 (线路板网络名: INA2_SCL, INA2_SDA)
#define INA219_NB_PORT GPIOC
#define INA219_NB_SCL_PIN FL_GPIO_PIN_6
#define INA219_NB_SDA_PIN FL_GPIO_PIN_7

// ===== INA219 旁路MOS控制引脚 =====
// 主电旁路MOS: PE3 (低电平=开启旁路, 高电平=关闭旁路)
#define INA219_MAIN_BYPASS_PORT GPIOE
#define INA219_MAIN_BYPASS_PIN FL_GPIO_PIN_3
// NB电旁路MOS: PE2 (低电平=开启旁路, 高电平=关闭旁路)
#define INA219_NB_BYPASS_PORT GPIOE
#define INA219_NB_BYPASS_PIN FL_GPIO_PIN_2

// ===== INA219 通道选择 =====
#define INA219_CH_MAIN 0 // 主电INA219
#define INA219_CH_NB 1   // NB电INA219(备电)

// ===== INA219 I2C地址 =====
#define INA219_I2C_ADDR_WRITE 0x80
#define INA219_I2C_ADDR_READ 0x81

// ===== 对外接口 =====
void ina219_gpio_init(void);
void ina219_bypass_on(uint8_t channel);
void ina219_bypass_off(uint8_t channel);
uint16_t ina219_read_current(uint8_t channel);

// ===== 向后兼容接口 (旧代码迁移用) =====
void INA219_IIC_GPIO_Init(void);
void Current_CHK_CTRL_ON(void);
void Current_CHK_CTRL_OFF(void);
void NB_Current_CHK_CTRL_ON(void);
void NB_Current_CHK_CTRL_OFF(void);
uint16_t Current_CHK_Func(uint8_t channel);

#endif
