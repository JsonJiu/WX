#ifndef __JIAOYAN__H
#define __JIAOYAN__H
#include "main.h"



uint8_t Sum_jiaoyan(uint8_t *Data,uint16_t lenth);
uint16_t CRC16_XMODEM(uint8_t *data,uint16_t datalen);
uint16_t CRC16_MODBUS(uint8_t *data, uint16_t datalen);
uint16_t get_crc16(void const * const p, const uint16_t len);

#endif
