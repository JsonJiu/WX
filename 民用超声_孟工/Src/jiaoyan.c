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

/*！！！！！！！！掲肖碕翌CRC丕刮！！！！！！！！*/
uint16_t get_crc16(void const * const p, const uint16_t len)//CRC丕刮 海業頁CRC丕刮峙念中議方象
{
	uint8_t *ptr = (uint8_t *)p;
	uint8_t CRC16L = 0xFF;
	uint8_t CRC16H = 0xFF;
	uint8_t CL = 0x01;
	uint8_t CH = 0xA0;
	uint8_t SaveH = 0;
	uint8_t SaveL = 0;
	uint8_t j = 0;
	uint16_t i = 0;

	if(!p || len < 1)
	{
		return 0;
	}

	for (i = 0; i < len; i++)
	{
		CRC16L ^= *(ptr + i);
		for (j = 0; j < 8; j++)
		{
			SaveH = CRC16H;
			SaveL = CRC16L;
			CRC16H >>= 1;
			CRC16L >>= 1;
			if ((SaveH & 0x01) == 0x01)
			{
				CRC16L |= 0x80;
			}
			if ((SaveL & 0x01) == 0x01)
			{
				CRC16H ^= CH;
				CRC16L ^= CL;
			}
		}
	}
	i = CRC16H;
	i = (i << 8) + CRC16L;
	return i;
}
