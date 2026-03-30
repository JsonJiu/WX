#ifndef _BDINA219_H
#define _BDINA219_H
#include "r_cg_macrodriver.h"
#include "MyDefine.h"

///////SCL
#define BDINA219_SCL_PIN_OUT       (P5)
#define BDINA219_SCL_PIN_PORT      (BIT4)
#define BDINA219_SCL_HI            (BDINA219_SCL_PIN_OUT |= BDINA219_SCL_PIN_PORT)
#define BDINA219_SCL_LO            (BDINA219_SCL_PIN_OUT &= ~BDINA219_SCL_PIN_PORT)
#define BDINA219_SCL(a)            (a)?(BDINA219_SCL_HI):(BDINA219_SCL_LO)
///////SDA
#define BDINA219_SDA_PIN_OUT       (P1)
#define BDINA219_SDA_PIN_PORT      (BIT0)
#define BDINA219_SDA_DIR           (PM1)
#define BDINA219_SDA_OUT_Dir       (BDINA219_SDA_DIR &= ~BDINA219_SDA_PIN_PORT)
#define BDINA219_SDA_IN_Dir        (BDINA219_SDA_DIR |= BDINA219_SDA_PIN_PORT)
#define BDINA219_SDA_HI            (BDINA219_SDA_PIN_OUT |= BDINA219_SDA_PIN_PORT)
#define BDINA219_SDA_LO            (BDINA219_SDA_PIN_OUT &= ~BDINA219_SDA_PIN_PORT)
#define BDINA219_SDA(a)            (a)?(BDINA219_SDA_HI):(BDINA219_SDA_LO)
#define BDINA219_SDA_InPut         (BDINA219_SDA_PIN_OUT&BDINA219_SDA_PIN_PORT)

unsigned short ReadBD_Current();////write-0x80 Read-0x81
#endif