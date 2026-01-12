#include "crc.h"

uint16_t CRC16_XMODEM(uint8_t *data,uint16_t datalen) 
{
	uint16_t wCRCin = 0x0000;
	uint16_t wCPoly = 0x1021;
	uint8_t i = 0;
	while (datalen--) 
	{
		wCRCin ^= (*(data++) << 8);
		for(i = 0; i < 8; i++) 
		{
			if(wCRCin & 0x8000)
			wCRCin = (wCRCin << 1) ^ wCPoly;
			else
			wCRCin = wCRCin << 1;
		}
	}
	return wCRCin;
}

