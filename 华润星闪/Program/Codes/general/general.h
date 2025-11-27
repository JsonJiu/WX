#ifndef _GENERAL_H_
#define _GENERAL_H_

void copy_memery(unsigned char *buf1, unsigned char *buf2, unsigned int len);
unsigned cmp_str(unsigned char *buf1, unsigned char *buf2, unsigned char len);
void mem_set(unsigned char *data, unsigned short len, unsigned short n);
void bcdtohex(unsigned long *bcd);
void rollbcdtohex(unsigned long *bcd);
void hextobcd(unsigned long *hex);
void rollhextobcd(unsigned long *hex);
unsigned char CharBCDtoHEX(unsigned char T_buf);
unsigned char CharBCDtoHEX_S(unsigned char T_buf);
unsigned char ByteHEXtoBCD(unsigned char T_buf);
unsigned long BCDtoHEX_B(unsigned char *T_buf, unsigned char T_len);
unsigned long BCDtoHEX_S(unsigned char *T_buf, unsigned char T_len);
unsigned long BCDtoHEX_SS(unsigned char *T_buf, unsigned char T_len);
void HEXtoBCD_B(unsigned char *T_buf, unsigned long T_buflong, unsigned char T_len);
void HEXtoBCD_S(unsigned char *T_buf, unsigned long T_buflong, unsigned char T_len);
unsigned char CrcAddGet(unsigned char *T_buf, unsigned int T_len);
unsigned char CrcAddCheck(unsigned char T_crc, unsigned char *T_buf, unsigned int T_len);
unsigned char DateCmp(RTC_T T_date1,RTC_T T_date2,unsigned char CmpSegs);
unsigned long CompTime(RTC_T Time1, RTC_T Time2);
unsigned long TimeToSecond(RTC_T * time);
void AddOneDay(RTC_T *t);
unsigned char charTobcd(unsigned char hex);
unsigned long RTCToDay(RTC_T  Time1, RTC_T  Time2);
#endif
