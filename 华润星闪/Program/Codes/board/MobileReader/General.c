
#include "General.h"

/*********************************************************************************************************
** Function name:       Calc_CS
** Descriptions:        和校验
** input parameters:    unsigned char  *CS_Buffer : 数据缓冲区首地址
    	    	    	unsigned short Len：        数据长度
** Returned value:      CS
*********************************************************************************************************/
unsigned char Calc_CS(unsigned char *CS_Buffer, unsigned short Len)
{
    unsigned char   CS = 0;
    unsigned short  j = 0;

    for(j = 0; j < Len; j++)
    {
        CS += CS_Buffer[j];
    }
    return CS;
}

/*------------------CRC循环校验函数------------------------
入口参数:	Count CRC校验数据的信息字段的长度
		*pData 校验数据信息字段内容2
算法： CRC-16-IBM 【0xA001】
       x16 + x15 + x2 + 1 【0x8005 or 0xA001 (0x4003)】

返回值：	CRC
-----------------------------------------*/
unsigned short CRC16_0xA001(unsigned char *pData, unsigned short len)
{
    int i, j;
    unsigned short CRC16;
    CRC16 = 0xFFFF;  	        //step 1
    for(i = 0; i < len; i++)    //step 6
    {
        CRC16 ^= *(pData + i);  	//step 2 CRC与校验数据异或	 //得出数据的按位反
        for(j = 0; j < 8; j++)      //step5
        {
            if(CRC16 & 0x1)  	//step 4  最低位是否为1
            {
                CRC16 >>= 1;  	//step 3
                CRC16 ^= 0xA001;
            }
            else
                CRC16 >>= 1;   	//step 3
        }      		            //step 5
    }
    return CRC16;
}
unsigned int CRC16_0x8408(const unsigned char *pData, unsigned int len)
{
    unsigned int uiCRCValue = 0xFFFF;
    unsigned char  ucLoop;
    unsigned char * pu8Buf = (unsigned char *)pData;
    while(len--)
    {
        uiCRCValue ^= *pu8Buf++;
        for(ucLoop=0; ucLoop<8; ucLoop++)
        {
            if(uiCRCValue & 0x0001)
            {
                uiCRCValue >>= 1;
                uiCRCValue ^= 0x8408;
            }
            else
            {
                uiCRCValue >>= 1;
            }
        }
    }
    uiCRCValue ^= 0xFFFF;
    return uiCRCValue;
}

unsigned short CRC16_0x8005(unsigned char *pData, unsigned char len)
{
    unsigned char i;
    unsigned char crcData;
    unsigned short checksum;

    checksum = 0xFFFF;  //Init value for CRC calculation
    for(i = 0; i < len; i++)
    {
        crcData = *pData;
        for (i = 0; i < 8; i++)
        {
            if (((checksum & 0x8000) >> 8) ^ (crcData & 0x80))
                checksum = (checksum << 1) ^ 0x8005;
            else
                checksum = (checksum << 1);
            crcData <<= 1;
        }
        pData++;
    }
    return checksum;
}
//Hex 字符串 转换为 ASCII 字符串
void HexToASCII(char *pdes, unsigned char *psrc, unsigned char len)
{
    unsigned char dat;
    while(len)
    {
        dat = *psrc;
        dat &= 0xF0;
        dat >>= 4;
        if(dat > 9)
            dat = dat + 'A' - 10;
        else
            dat += '0';
        *pdes = dat;
        pdes++;

        dat = *psrc;
        dat &= 0x0F;
        if(dat > 9)
            dat = dat + 'A' - 10;
        else
            dat += '0';
        *pdes = dat;
        pdes++;
        *pdes = ' ';
        pdes++;
        psrc++;
        len--;
    }
    *pdes = '\0';
}

/*********************************************************************************************************
** Function name:       copy_str
** Descriptions:        复制字符串，将buf2的内容复制到buf1
** input parameters:    buf1--字符串1  buf2--字符串2  len--字符串长度
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
unsigned short copy_str(unsigned char *pdes, unsigned char *psrc, unsigned short len)
{
    unsigned short i;
    for(i = 0; i < len; i++)
    {
        *(pdes + i) = *(psrc + i);
    }
    return len;
}

void BCDtoHex(unsigned short *bcd)
{
    unsigned short buf1 = *bcd;
    unsigned short buf2 = 0;
    if(buf1 > 0x9999)
        return ;
    else //(buf1 <= 0x99)
    {
        buf2 = (buf1 & 0x000f);
        buf1 >>= 4;
        buf2 += (buf1 & 0x000f) * 10;
        buf1 >>= 4;
        buf2 += (buf1 & 0x000f) * 100;
        buf1 >>= 4;
        buf2 += (buf1 & 0x000f) * 1000;
    }
    *bcd = buf2;
}

unsigned char HexToBCD(unsigned char hex)
{
    unsigned short hex1 = hex;
    unsigned short bcd = 0;

    bcd = hex1/10;
    hex1 %= 10;
    bcd <<= 4;
    bcd += hex1;

    return bcd;
}
//数组排序
void ArraySeq(unsigned int *pbuf, unsigned char len)
{
    unsigned int tmpInt;
    unsigned char i, j;
    if(len <= 1)
        return;

    for(i = 1; i < len; i++)
    {
        for(j = 0; j < i; j++)
        {
            if(*(pbuf + i) > *(pbuf + j))
            {
                tmpInt = *(pbuf + i);
                *(pbuf + i) = *(pbuf + j);
                *(pbuf + j) = tmpInt;
            }
        }
    }
}



















