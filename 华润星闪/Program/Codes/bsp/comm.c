#include "comm.h"

/**
 * BCD数据转化成HEX
 * @param bcd
 * @return ：返回HEX数据
 */
unsigned char BcdToHex(unsigned char bcd)
{
    return ((bcd>>4)&0x0f)*10 + (bcd&0x0f);
}

/**
 * 校验和函数
 * @param pCmd：校验和数据
 * @param len：校验和前长度
 * @return：校验后的长度
 */
unsigned short CheckAndFinishCommand(unsigned char *pCmd,unsigned short len)
{
    unsigned short i;
    unsigned char check;
    check=0;
    for(i=0; i<len; i++)
    {
        check += pCmd[i];
    }
    pCmd[i] = check;
    i++;
    pCmd[i] = 0x16;
    i++;
    return i;
}

/**
 * hex转ascii带空格
 * @param data_hex：
 * @param data_ascii
 * @param len_hex
 * @return
 */
unsigned short HexToAscii_Space(unsigned char* data_hex,unsigned char* data_ascii, unsigned short len_hex)
{
    unsigned char a, b;
    unsigned short i;
    unsigned short j = 0;
    for(i = 0;i < len_hex; i++)
    {
        a = ((*data_hex) & 0xf0) >> 4;
        b = (*data_hex) & 0x0f;
        if(a <= 0x09)
            a = a + '0';
        else if((a >= 0x0a) && (a <= 0x0f))
            a = (a - 0x0a) + 65;
        if( b<= 0x09 )
            b = b + '0';
        else if((b >= 0x0a) && (b <= 0x0f))
            b = (b - 0x0a) + 65;
        *data_ascii++ = a;
        *data_ascii++ = b;
        *data_ascii++ = 0x20;

        data_hex++;
        j += 3;
    }
    *data_ascii++ = '\r';
    *data_ascii++ = '\n';
    *data_ascii++ = '\0';
    j += 3;
    return j;
}
