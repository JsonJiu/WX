#ifndef _STRUCT_H_
#define _STRUCT_H_

typedef struct
{
  unsigned char ZJFall;
  unsigned char IRDAFall;
	unsigned char CARDFall;
	unsigned char UMUFall;
	unsigned char LedFlag;
	unsigned char ZJData;
	unsigned char CARDData;
	unsigned char UMUData;
	unsigned char IRDAData;
	unsigned char IRDAOKData;
	unsigned char MeterStatus;
        unsigned char Rtc_Come;
	unsigned char Rtc_time;
	unsigned char JudgeVavle;
	unsigned char CAPStatus;
}FLAG;

typedef struct 
{
	unsigned char ZJBuff[50];
	unsigned char CARDBuff[50];
	unsigned char IRDABuff[400];
	unsigned char UMUBuff[50];
	unsigned char ZJBuffLen;
	unsigned char CARDBuffLen;
	unsigned char IRDABuffLen;
	unsigned char UMUBuffLen;
	unsigned char ZJAddr;
	unsigned char ZDADFlag;
	unsigned char BDADFlag;
	unsigned char RtcTimer;
	unsigned char VavleRTimer;
	unsigned char VavleBTimer;
	unsigned short ZDCurrent;
	unsigned short BDCurrent;
	unsigned long ZDVolt;
	unsigned long BDVolt;
	unsigned long ScanVavleTimer;
	unsigned char IMEI_IMSI_ICCID[50];
	unsigned char IRDABuff_REC_len;
	unsigned char versiontime[6];
	unsigned char versionid[2];
	unsigned char esamid[8];
	unsigned char platepressure[5];
	unsigned char BDflag;
}TEMP_USER_DATA;

typedef struct
{
  unsigned char ZJTimer;
  unsigned char UMUTimer;
  unsigned char CARDTimer;
	unsigned char IRDATimer;
}CSB_TIMER;

extern CSB_TIMER Csb_Timer;
extern TEMP_USER_DATA TempUserData;
extern FLAG Flag;
#endif