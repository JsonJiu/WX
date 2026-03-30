#ifndef _STRUCT_H_
#define _STRUCT_H_

typedef struct
{
        unsigned char ZJFall;
        unsigned char FJFall;
	unsigned char ZJData;
	unsigned char FJData;
	unsigned char Rtc_Come;
}FLAG;

typedef struct 
{
	unsigned char ZJBuff[400];
	unsigned char FJBuff[400];
	unsigned char ZJBuffLen;
	unsigned char FJBuffLen;
}TEMP_USER_DATA;

typedef struct
{
  unsigned char ZJTimer;
  unsigned char FJTimer;
}CSB_TIMER;

extern CSB_TIMER Csb_Timer;
extern TEMP_USER_DATA TempUserData;
extern FLAG Flag;
#endif