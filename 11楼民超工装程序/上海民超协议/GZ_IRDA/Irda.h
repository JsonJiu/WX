#ifndef _IRDA_H_
#define _IRDA_H_
#include "r_cg_macrodriver.h"
#include "MyDefine.h"
///////IRDA_Tx
#define IRDA_Tx_PIN_OUT       (P3)
#define IRDA_Tx_PIN_PORT      (BIT5)
#define IRDA_Tx_OUT_DIR       (PM3)
#define IRDA_Tx_OUT_Dir       (IRDA_Tx_OUT_DIR&=~IRDA_Tx_PIN_PORT)
#define IRDA_Tx_HI            (IRDA_Tx_PIN_OUT |= IRDA_Tx_PIN_PORT)
#define IRDA_Tx_LO            (IRDA_Tx_PIN_OUT &= ~IRDA_Tx_PIN_PORT)
#define IRDA_Tx(a)            (a)?(IRDA_Tx_HI):(IRDA_Tx_LO)
///////IRDA_RxPOW
#define IRDA_RxPOW_PIN_OUT       (P2)
#define IRDA_RxPOW_PIN_PORT      (BIT5)
#define IRDA_RxPOW_OUT_DIR       (PM2)
#define IRDA_RxPOW_OUT_Dir       (IRDA_RxPOW_OUT_DIR&=~IRDA_RxPOW_PIN_PORT)
#define IRDA_RxPOW_HI            (IRDA_RxPOW_PIN_OUT |= IRDA_RxPOW_PIN_PORT)
#define IRDA_RxPOW_LO            (IRDA_RxPOW_PIN_OUT &= ~IRDA_RxPOW_PIN_PORT)
#define IRDA_RxPOW(a)            (a)?(IRDA_RxPOW_HI):(IRDA_RxPOW_LO)
///////IRDA_TxPOW
#define IRDA_TxPOW_PIN_OUT       (P2)
#define IRDA_TxPOW_PIN_PORT      (BIT6)
#define IRDA_TxPOW_OUT_DIR       (PM2)
#define IRDA_TxPOW_OUT_Dir       (IRDA_TxPOW_OUT_DIR&=~IRDA_TxPOW_PIN_PORT)
#define IRDA_TxPOW_HI            (IRDA_TxPOW_PIN_OUT |= IRDA_TxPOW_PIN_PORT)
#define IRDA_TxPOW_LO            (IRDA_TxPOW_PIN_OUT &= ~IRDA_TxPOW_PIN_PORT)
#define IRDA_TxPOW(a)            (a)?(IRDA_TxPOW_HI):(IRDA_TxPOW_LO)

unsigned char Irda_Data_Handling(unsigned char *Irda_Buff);
void Check_IRDA_Data();
#endif