#include "jiaoyan.h"

/*！！！！！！！！亅咏才丕刮！！！！！！！！*/
uint8_t Sum_jiaoyan(uint8_t *Data,uint16_t lenth)
{
	uint8_t i = 0;
	uint8_t sum = 0;
	for(i=0;i<lenth-2;i++)
	{
		sum = sum + Data[i];
	}
	return sum;
}

/*！！！！！！！！CRC-16/XMODEM！！！！！！！！*/
uint16_t CRC16_XMODEM(uint8_t *data,uint16_t datalen) 
{
	uint16_t wCRCin = 0x0000;
	uint16_t wCPoly = 0x1021;
	while (datalen--) 
	{
		wCRCin ^= (*(data++) << 8);
		for(int i = 0; i < 8; i++) 
		{
			if(wCRCin & 0x8000)
			wCRCin = (wCRCin << 1) ^ wCPoly;
			else
			wCRCin = wCRCin << 1;
		}
	}
	return wCRCin;
}

/*！！！！！！！！CRC-16/MODBUS！！！！！！！！*/
void InvertUint16(uint16_t *dBuf,uint16_t *srcBuf)
{
	uint8_t i;
	uint16_t tmp[4]={0};
 
	for(i=0;i< 16;i++)
	{
		if(srcBuf[0]& (1 << i))
		tmp[0]|=1<<(15 - i);
	}
	dBuf[0] = tmp[0];
}
uint16_t CRC16_MODBUS(uint8_t *data, uint16_t datalen)
{
	uint16_t wCRCin = 0xFFFF;
	uint16_t wCPoly = 0x8005;
	uint8_t i = 0;
	InvertUint16(&wCPoly,&wCPoly);
	while (datalen--) 	
	{
		wCRCin ^= *(data++);
		for(i = 0;i < 8;i++)
		{
			if(wCRCin & 0x01)
				wCRCin = (wCRCin >> 1) ^ wCPoly;
			else
				wCRCin = wCRCin >> 1;
		}
	}
	return (wCRCin);
}


