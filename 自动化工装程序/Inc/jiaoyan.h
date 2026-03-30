#ifndef __JIAOYAN__H
#define __JIAOYAN__H
#include "main.h"



uint8_t Sum_jiaoyan(uint8_t *Data,uint16_t lenth);
uint16_t CRC16_XMODEM(uint8_t *data,uint16_t datalen);
uint16_t CRC16_MODBUS(uint8_t *data, uint16_t datalen);
uint32_t Spanish_CRC32(uint8_t *data, int len);
void ASCII_To_Hex(uint8_t *src, uint16_t lenth);
void ASCII8byte_To_ASCII16byte(uint8_t *ascii8byte);
#endif
