#ifndef _CRC16_H_
#define _CRC16_H_


unsigned int Crc16Get(unsigned char *puchMsg, unsigned int usDataLen);
unsigned char CrcCheck(unsigned int T_CRC, unsigned char *T_buf, unsigned int T_len);

#endif
