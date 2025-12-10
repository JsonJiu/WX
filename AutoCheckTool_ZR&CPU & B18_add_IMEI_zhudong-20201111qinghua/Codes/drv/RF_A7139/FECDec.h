
#ifndef _DECDEC_H_
#define _DECDEC_H_



unsigned short FEC_Decode(unsigned char *data, unsigned char datalen);
unsigned short fecDecode(unsigned char *pDecData, unsigned char* pInData, unsigned short RemBytes);
static unsigned char hammWeight(unsigned char a);
static unsigned char min(unsigned char a, unsigned char b);
#ifndef NULL
#define NULL 0
#endif

#endif

