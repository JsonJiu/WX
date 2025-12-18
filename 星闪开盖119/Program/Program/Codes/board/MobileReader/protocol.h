
#ifndef _protocol_H_
#define _protocol_H_

#include "config.h"

#define OVERTIME  10

/*通信相关***********************************************************************************************/
enum
{
    CMD_HERTBEAT = 0x00,    // 心跳
    CMD_VER     = 0x21,     // 读固件版本号
    CMD_RF_JZQ,  // 0x22    // 与集中器通信
    CMD_RF_METER,// 0x23    // 与表具通信
    CMD_RF_MODE, // 0x24    // RF模式切换
};

//帧格式
#define Fream_Start    0   //帧起始符
#define Fream_Addr_0   1      //地址
#define Fream_Addr_1   2      //地址
#define Fream_Addr_2   3      //地址
#define Fream_Addr_3   4      //地址

#define Fream_Midu     6     //网络密度
#define Fream_Con      8   //控制码
#define Fream_LenL     9     //数据域长度
#define Fream_LenH     10     //数据域长度 
#define Fream_Stime    11     //系统时钟
#define Fream_MType    17     //设备类型
#define Fream_ID0      18     //数据标识低
#define Fream_ID1      19     //数据标识高
#define Fream_Num      20     //帧序号
#define Fream_Data     21     //数据内容


//数据接收状态标志
#define Recv_Fream_New   0x00
#define Recv_Fream_Cmd   0x01
#define Recv_Fream_Len   0x02
#define Recv_Fream_Bag   0x03
#define Recv_Fream_CRC   0x04

#define Recv_Fream_Start 0x06
#define Recv_Fream_Addr  0x07
#define Recv_Fream_Data  0x08
#define Recv_Head_1      0x09
#define Recv_Head_2      0x0A
#define Recv_Head_3      0x0B
#define Recv_Head_4      0x0C
#define Recv_Head_5      0x0D
#define Recv_Head_6      0x0E


typedef struct ProtWXFram_S
{
    unsigned char Head1;	//0X68
    unsigned char Id[4];
    unsigned char Ssser[2];
    unsigned char Head2;	//0X68
    unsigned char Ctrl;
    unsigned char Len[2];	//低位在前,包含长度(不含)之后,校验(不含)之前.
    unsigned char YY;
    unsigned char MMon;
    unsigned char DD;
    unsigned char HH;
    unsigned char mm;
    unsigned char SS;
    unsigned char TypeM;	//表类型.0x0f表示所有类型
    unsigned char Cmd[2];	//低在前.
    unsigned char Ser;		//命令序号
    unsigned char Data[1];
    //	unsigned char Chk;		//和校验,Head1(包含)开始,Chk(不包含)之前
    //	unsigned char Tail;		//0x16
} ProtWXFram_ST;

extern COMM_Buffer_t    COM_Buffer;  //串口
extern COMM_Buffer_t    RF_Buffer;   //RF
extern COMM_Buffer_t    WiFi_Buffer; //WiFi

extern unsigned char  RF_Status;
extern unsigned int   RF_HeadTimeOver_70S;

extern unsigned short RF_OptDelayTime;
extern unsigned char RF_TryTimes;//发送尝试次数
extern unsigned char  Signal_RF_Run;
extern unsigned char  RF_Status;

void WhiteDeal(unsigned char *pucStr, unsigned int ulNum);

//void WiFi_Recv_Protocol_Resolve(unsigned char* buf);
void DataConvert(unsigned char *buf, COMM_UART uart) ;
unsigned char Frame_Receive(COMM_Buffer_t *pbuf);
unsigned char RF_Frame_Receive(COMM_Buffer_t *pbuf);
void Frame_Receive_WX2_0(pCOMM_Buffer_t T_CMD, unsigned char CmdChar, unsigned char Src);
unsigned short get_fre(unsigned char ch);
void Protocol_Resolve(void) ;

unsigned int MakeFrameWX200( unsigned char *pOutBuf, const unsigned char *pAdr,
                             unsigned char *pParm, unsigned char Ctrl,
                             unsigned int Fun, unsigned char Area,
                             unsigned char MeterNum, unsigned char DeviceType);
unsigned short MakeFrame(unsigned char *pDesbuf, unsigned char cmd, unsigned char *databuf,unsigned short datalen);
void COM_WhiteAndSend(unsigned char *pucStr, unsigned int ulNum);
unsigned int CRC16_0x8408(const unsigned char *pucBuf, unsigned int uwLength) ;
#endif
