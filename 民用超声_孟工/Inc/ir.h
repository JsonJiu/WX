#ifndef __IR__H
#define __IR__H
#include "main.h"

void IR_Agreement_Analysis(uint8_t *Data, uint32_t lenth);
void Send_E20A(void);
void Start_Shanggao(void);
void Read_IMEI_IMSI_ICCID(void);
void Read_Version(void);
void Read_ESAMID(void);
#endif
