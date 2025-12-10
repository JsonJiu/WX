#ifndef _SYSTEMCOMMAND_H_
#define _SYSTEMCOMMAND_H_



//////////////////////////////////// 结构体 ////////////////////////////////////////

/**
 * 命令指针
 */
typedef struct
{
   unsigned char Device;
   unsigned char Cmd;
   unsigned char AddrLo;
   unsigned char AddrHi;
   unsigned char LenLo;
   unsigned char LenHi;
   unsigned char data[];
}StCmd,*pStCmd;


typedef struct
{
   unsigned short addr;
   unsigned short datalen;
   unsigned short datalennow;
   unsigned char sub;
}StWriteRegCache,*pStWriteRegCache;

unsigned char CheckSysCommand(unsigned char *buf, unsigned short *len);
unsigned short RunSysCommand(unsigned char *CmdBuff);
unsigned short MakeSend(unsigned char *buf, unsigned short len);
#endif

