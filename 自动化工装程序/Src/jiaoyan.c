#include "jiaoyan.h"
#include "test.h"

extern struct Test Test_result;
uint8_t ASCII_EUI[8] = {0};
uint8_t ASCII_16Byte[16] = {0};

/*！！！！！！！！亅咏才丕刮！！！！！！！！*/
uint8_t Sum_jiaoyan(uint8_t *Data, uint16_t lenth)
{
	uint8_t i = 0;
	uint8_t sum = 0;
	for (i = 0; i < lenth - 2; i++)
	{
		sum = sum + Data[i];
	}
	return sum;
}

/*！！！！！！！！CRC-16/XMODEM！！！！！！！！*/
uint16_t CRC16_XMODEM(uint8_t *data, uint16_t datalen)
{
	uint16_t wCRCin = 0x0000;
	uint16_t wCPoly = 0x1021;
	while (datalen--)
	{
		wCRCin ^= (*(data++) << 8);
		for (int i = 0; i < 8; i++)
		{
			if (wCRCin & 0x8000)
				wCRCin = (wCRCin << 1) ^ wCPoly;
			else
				wCRCin = wCRCin << 1;
		}
	}
	return wCRCin;
}

/*！！！！！！！！CRC-16/MODBUS！！！！！！！！*/
void InvertUint16(uint16_t *dBuf, uint16_t *srcBuf)
{
	uint8_t i;
	uint16_t tmp[4] = {0};

	for (i = 0; i < 16; i++)
	{
		if (srcBuf[0] & (1 << i))
			tmp[0] |= 1 << (15 - i);
	}
	dBuf[0] = tmp[0];
}
uint16_t CRC16_MODBUS(uint8_t *data, uint16_t datalen)
{
	uint16_t wCRCin = 0xFFFF;
	uint16_t wCPoly = 0x8005;
	uint8_t i = 0;
	InvertUint16(&wCPoly, &wCPoly);
	while (datalen--)
	{
		wCRCin ^= *(data++);
		for (i = 0; i < 8; i++)
		{
			if (wCRCin & 0x01)
				wCRCin = (wCRCin >> 1) ^ wCPoly;
			else
				wCRCin = wCRCin >> 1;
		}
	}
	return (wCRCin);
}

uint32_t Spanish_CRC32(uint8_t *data, int len)
{
	uint16_t k, i;
	uint8_t c;
	uint32_t sm = 0, crc32 = 0xFFFFFFFF;

	for (k = 0; k < len; k++)
	{
		c = data[k];
		for (i = 0; i < 8; ++i)
		{
			sm = crc32;
			crc32 <<= 1;
			if (c & 0x80)
				crc32 |= 1;
			if (sm & 0x80000000)
				crc32 ^= 0x4C11DB7; // Polynom
			c <<= 1;
		}
	}
	return crc32;
}

void ASCII_To_Hex(uint8_t *src, uint16_t lenth)
{
	uint8_t desc;
	uint8_t i;
	for (i = 0; i < lenth; i++)
	{
		if (*(src + i) <= 0x09)
			desc = *(src + i) + 0x30;
		else if ((*(src + i) >= 0x0A) && (*(src + i) <= 0x0F))
			desc = *(src + i) + 0x37;
		Test_result.LoraEUI[lenth - i - 1] = desc;
	}
}

void ASCII8byte_To_ASCII16byte(uint8_t *ascii8byte)
{
	uint8_t i = 0;
	uint8_t j = 0;
	for (i = 0; i < 8; i++)
	{
		ASCII_16Byte[i + j] = ascii8byte[i] & 0x0F;
		ASCII_16Byte[i + j + 1] = (ascii8byte[i] & 0xF0) >> 4;
		j++;
	}
}
