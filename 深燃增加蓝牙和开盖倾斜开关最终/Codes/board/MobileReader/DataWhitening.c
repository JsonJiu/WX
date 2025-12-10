/*--------------------------------------------------------------------
     艾美通RF模块1758接口RF_ACP250_Interface.c
     author:115
     date:  2012.9.28
--------------------------------------------------------------------*/
#include "include.h"
#include "DataWhitening.h"

/*---------------白化与CRC---------------------------*/
#define DATA_WHITENING_KEY  0x1FF	//PN9初值
// generate a new whitening key (carry bit + key)
void vWht_GetNewKey( unsigned char *pbWhtKey_CarryPtr, unsigned char *pbWhtKey_LowBytePtr )
{
    unsigned char bCnt;
    unsigned char bWhtKey_CarryPrev, bWhtKey_Carry, bWhtKey_LowByte;
    bWhtKey_Carry   = *pbWhtKey_CarryPtr;						// use local variable for the carry bit
    bWhtKey_LowByte = *pbWhtKey_LowBytePtr;						// use local variable for the key
    for (bCnt = 0; bCnt < 8; bCnt++)								// generate the new whitening key
    {
        bWhtKey_CarryPrev = bWhtKey_Carry;						// store current carry bit
        bWhtKey_Carry     = (bWhtKey_LowByte & 0x01) ^ ((bWhtKey_LowByte >> 5) & 0x01);			// get new carry bit
        bWhtKey_LowByte   = ((bWhtKey_LowByte >> 1) & 0xFF) | ((bWhtKey_CarryPrev << 7) & 0x80);	// get new key
    }
    *pbWhtKey_CarryPtr   = bWhtKey_Carry;					    // return updated carry bit value
    *pbWhtKey_LowBytePtr = bWhtKey_LowByte;						// return updated key value
    return;
}
// encrypt/decrypt input data by the whitening key
void vWht_DataWhitening( unsigned char *pbData, unsigned short length )
{
    unsigned short bCnt;
    unsigned char bWhtKey_Carry;
    unsigned char bWhtKey_LowByte;
    if(length >= COMM_BUF_MAX_LEN)
        return;
    bWhtKey_Carry = (DATA_WHITENING_KEY >> 8) & 0x01;			// get carry bit of the whitening key
    bWhtKey_LowByte = DATA_WHITENING_KEY & 0xFF;			// get the lower byte (`key` part) of the whitening key
    for (bCnt = 0; bCnt < length; bCnt++)						// do the encryption/decryption
    {
        pbData[bCnt] ^= bWhtKey_LowByte;					// take the XOR of the data and the key
        vWht_GetNewKey(&bWhtKey_Carry, &bWhtKey_LowByte);	// get a new whitening key (carry bit + key)
    }
}
