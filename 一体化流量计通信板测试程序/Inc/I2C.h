#ifndef __I2C_H__
#define __I2C_H__

#include "main.h"
void I2C_Init(void);
void send_weituo(void);
void MF_IIC_Config_Init(void);
extern uint8_t IIC_send_weituo;
extern uint8_t read_dizhi[14];
extern uint32_t wendu;
extern uint32_t yali;
#endif
