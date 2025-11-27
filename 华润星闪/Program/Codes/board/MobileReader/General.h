/* 
 * File:   General.h
 * Author: Dano
 *
 * Created on January 8, 2015, 10:26 AM
 */

#ifndef GENERAL_H
#define	GENERAL_H


unsigned char Calc_CS(unsigned char *CS_Buffer, unsigned short Len);
unsigned short CRC16_0xA001(unsigned char *pData, unsigned short len);
unsigned int CRC16_0x8408(const unsigned char *pData, unsigned int len);
unsigned short CRC16_0x8005(unsigned char *pData, unsigned char len);

void HexToASCII(char *pdes, unsigned char *psrc, unsigned char len);
unsigned short copy_str(unsigned char *pdes, unsigned char *psrc, unsigned short len);
void BCDtoHex(unsigned short *bcd);
unsigned char HexToBCD(unsigned char hex);
void ArraySeq(unsigned int *pbuf, unsigned char len);

#endif	/* GENERAL_H */

