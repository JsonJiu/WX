
#include <string.h>
#include "main.h"
#include "config.h"
#include "FECEnc.h"
#include "FECDec.h"

#include "protocol.h"
#include "LSD_RF.h"
#include "LSD_RFreg.h"
#include "RF_Task.h"

#if Device_RF == Use

// unsigned char Debugbuffer[] = {0x0A ,0x00 ,0x0B ,0xFE ,0x10 ,0x00 ,0x00 ,0x05 ,0x82 ,0xAE };
 unsigned char FECTest[] = {0x08 ,0x00 ,0x34 ,0x3c ,0x64 ,0xa0 ,0xa4 ,0x9c ,0x03 ,
                            0x0c ,0x0f ,0x04 ,0x00 ,0x40 ,0xc0 ,0x00 ,0x67 ,0x4c ,
                            0x02 ,0x00 ,0x00 ,0x00 ,0x2a ,0xd5};
///////////////////////// 外部函数 //////////////////////////////

//////////////////////////////////////////////////////////


typedef struct
{
  T_RFSTATE RFState;
}Stu_RFRUN;

Stu_RFRUN RFRun;
void RF_ModeSwitch(T_RFSTATE mode);

/////////////////////////////// BSP START /////////////////////////////////////////
/*RF硬件初始化并设置至休眠模式*/
//void A7139_BSP_Init(void);
//void RFInit(void)
//{
//  A7139_BSP_Init();
//  RF_ModeSwitch(RF_MODE_SLEEP);
//}
void RFBroken(void)
{
//    RF_ModeSwitch(RF_MODE_DEEPSLEEP, 0);
//    DispErr();
//    //BeepMs(100);
//    if (RFRun.BrokenTime >= 5)
//    {
//        DeviceRun.Door = 1;
//    }else{
//        RFRun.BrokenTime++;
//    }
//    DeviceErr.RF = 1;
//    RFRun.RFTimer.waitwakeuptimeout = 0;
//    DeviceRun.RF = 0;
//    RFRun.RFTimer.sleeptimeout = RFRun.RFTimer.sleeptime_ms;
}

/*在电源可控情况下，可关闭RF电源*/
void RF_Pow_Down(void)
{
    //TRXEM_POW_CL;
    //halRfSleepSerial();
    RFRun.RFState = RF_MODE_DEEPSLEEP;
    DelayMs(1);
}

/////////////////////////////// BSP END /////////////////////////////////////////

void RF_ModeSwitch(T_RFSTATE mode)
{        

    if(RFRun.RFState == mode)
		return;
    if(RFRun.RFState == RF_MODE_DEEPSLEEP)
        LSD_RF_InitRF();
	if(mode != RF_MODE_RECIVE)
		A7139RunConfig.RXLen = 0;
    switch (mode)
    {
    case RF_MODE_DEEPSLEEP:
        RF_Pow_Down();
        break;
    case RF_MODE_SLEEP:
        CBB_NOP();
       // halRfSleepSerial();
        break;
    case RF_MODE_IDLE:
        CBB_NOP();
       // halRfIdleSerial();
        break;
    
    case RF_MODE_RECIVE:
        LSD_RF_RXmode();
        A7139RunConfig.RXLen = 1;
        break;
    case RF_MODE_SEND:
        break;
    }
    RFRun.RFState = mode;
}

////////////////////////////////// Function ////////////////////////////////////


void RF_Ds(void)
{
    RF_ModeSwitch(RF_MODE_SLEEP);
}

void RFReciveNextPack(void)
{
    RF_ModeSwitch(RF_MODE_IDLE);
    RF_ModeSwitch(RF_MODE_RECIVE);
}

extern unsigned short fecEncodeTable[];
unsigned char sendcn;
void LSD_RF_Setting(void);
void RFSendBuff(unsigned char *buf, unsigned short len)
{
    RF_ModeSwitch(RF_MODE_SEND);
    A7139RunConfig.TXLen = FECEnc(buf, len);
    //A7139RunConfig.TXLen = len;
//    halRfSendPacketSerial(buf, A7139RunConfig.TXLen);
    
    if(sendcn == 0)
    {
        //DelayMs(10);
        LSD_RF_SendPacket(buf, A7139RunConfig.TXLen);
        sendcn = 1;
        RF_ModeSwitch(RF_MODE_IDLE);
    }else{
        SendAgain();
    }
//    while(frameBuf.FrameType == CMD_WakeUp && RFTiming)
//        SendAgain();
	A7139RunConfig.TXLen = 0;
}

void RFSendTest(void)
{
//    unsigned char size = sizeof(Debugbuffer);
//    copy_str(RFCmd, Debugbuffer, size);
//    RFSendBuff(RFCmd, size);
    
    unsigned char size = sizeof(FECTest);
	
    //halRfSendPacketSerial(FECTest, size);
    RF_ModeSwitch(RF_MODE_SEND);
    RF_ModeSwitch(RF_MODE_RECIVE);
}
extern unsigned char RfBuffer[];
unsigned short RFRecive(unsigned char *buf)
{
    unsigned short crc;
    unsigned short crc2;
    unsigned char retlen = 0;//返回成功接收的长度
    
    if(A7139RunConfig.RXLen == 0)	//其他状态进入接收状态
		RF_ModeSwitch(RF_MODE_RECIVE);
    if(A7139RunConfig.RXOK)
    {
		LSD_RF_RxPacket(buf, 64);
		FEC_Decode(buf, 64);
        {//命令接收完整 进行校验
            RF_ModeSwitch(RF_MODE_SLEEP);
			crc = buf[buf[0] - 2];
			crc = (crc << 8) | buf[buf[0] - 1];
			crc2 = CRC16Default(buf, buf[0] - 2);
			if(crc == crc2) //命令正确
			{
                RF_ModeSwitch(RF_MODE_IDLE);
				retlen = buf[0];
			}else{
                RFReciveNextPack();
            }
		}
		A7139RunConfig.RXOK = 0;
    }
    return retlen;
}
///////////////////////////////////////////////////////////////////////////////
void Task_RF_HW(void)//为了兼容其它移动采集器，添加此空任务
{
    return;
}
#endif
