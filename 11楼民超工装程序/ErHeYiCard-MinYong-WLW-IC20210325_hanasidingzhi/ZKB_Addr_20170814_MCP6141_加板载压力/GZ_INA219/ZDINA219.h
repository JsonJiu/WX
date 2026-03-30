#ifndef _ZDINA219_H
#define _ZDINA219_H
#include "r_cg_macrodriver.h"
#include "MyDefine.h"

///////SCL
#define ZDINA219_SCL_PIN_OUT       (P3)
#define ZDINA219_SCL_PIN_PORT      (BIT0)
#define ZDINA219_SCL_HI            (ZDINA219_SCL_PIN_OUT |= ZDINA219_SCL_PIN_PORT)
#define ZDINA219_SCL_LO            (ZDINA219_SCL_PIN_OUT &= ~ZDINA219_SCL_PIN_PORT)
#define ZDINA219_SCL(a)            (a)?(ZDINA219_SCL_HI):(ZDINA219_SCL_LO)
///////SDA
#define ZDINA219_SDA_PIN_OUT       (P5)
#define ZDINA219_SDA_PIN_PORT      (BIT6)
#define ZDINA219_SDA_DIR           (PM5)
#define ZDINA219_SDA_OUT_Dir       (ZDINA219_SDA_DIR &= ~ZDINA219_SDA_PIN_PORT)
#define ZDINA219_SDA_IN_Dir        (ZDINA219_SDA_DIR |= ZDINA219_SDA_PIN_PORT)
#define ZDINA219_SDA_HI            (ZDINA219_SDA_PIN_OUT |= ZDINA219_SDA_PIN_PORT)
#define ZDINA219_SDA_LO            (ZDINA219_SDA_PIN_OUT &= ~ZDINA219_SDA_PIN_PORT)
#define ZDINA219_SDA(a)            (a)?(ZDINA219_SDA_HI):(ZDINA219_SDA_LO)
#define ZDINA219_SDA_InPut         (ZDINA219_SDA_PIN_OUT&ZDINA219_SDA_PIN_PORT)

unsigned short ReadZD_Current();////write-0x80 Read-0x81

#endif