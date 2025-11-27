////////////////////////////////////////////////////////////////////////////////
// 版权:     利尔达科技集团股份有限公司
// 文件名:   LSD_RFregsetting.h
// 版本：    v1.0
// 工作环境: IAR  v5.30
// 作者:     于海波
// 生成日期: 2013.11
// 功能:     
// 相关文件: 
// 修改日志：
////////////////////////////////////////////////////////////////////////////////
#ifndef _LSD_RFregsetting_h_
#define _LSD_RFregsetting_h_

#include "config.h"

const Uint16 LSD_RFregConfig[]= //
{
        0x0EC4,         ///SYSTEM CLOCK register
	0x0A24,		//PLL1 register,
	0xC805,		//PLL2 register,	
	0x0000,		//PLL3 register,
	0x0A20,		//PLL4 register,
	0x0024,		//PLL5 register,
	0x0000,		//PLL6 register,
	0x0003,		//CRYSTAL register,
	0x0000,		//PAGEA,
	0x0000,		//PAGEB,	
        0x18D4,         //RX1 register, 		
	0x7009,		//RX2 register, 	by preamble
	0x4000,		//ADC register,	   	
	0x0800,		//PIN CONTROL register,		Use Strobe CMD
	0x4C45,		//CALIBRATION register,
	0x20C0		//MODE CONTROL register, 	Use FIFO mode
};

const Uint16 LSD_RFregConfig_PageA[]=   //470MHz, 10kbps (IFBW = 50KHz, Fdev = 18.75KHz)
{
        0xF333,         /*0x1703--0x1260*///TX1 register, 	
	0x0000,		//WOR1 register,
	0xF800,		//WOR2 register,
	0x1107,		//RFI register, 	Enable Tx Ramp up/down  
	0x8170,		//PM register,		CST=1
	0x0201,		//RTH register,
	0x400F,		//AGC1 register,	
	0x2AC0,		//AGC2 register, 
	0x2041,		//GIO register, 	GIO2=WTR, GIO1=WTR
//        0x4745,		//GIO register, 	GIO2=WTR, GIO1=WTR
	0xD281,		//CKO register
	0x0004,		//VCB register,
	0x0A21,		//CHG1 register, 	480MHz
	0x0022,		//CHG2 register, 	500MHz
	0x003F,		//FIFO register, 	FEP=63+1=64bytes
	0x1507,		//CODE register, Preamble=4bytes, ID=4bytes, 
	0x0000		//WCAL register,
};

const Uint16 LSD_RFregConfig_PageB[]=   //470MHz, 10kbps (IFBW = 100KHz, Fdev = 18.75KHz)
{
	0x0b5f,//0x0B37,		/*0x0B7F-->0x0B37 733 721????*///TX2 register, 	Tx power=20dBm
	0x8312,         //IF1 register, 	Enable Auto-IF, IF=100KHz     0x8400,IF=200KHz 
        0x0000,		//IF2 register,
	0x0000,		//ACK register,
	0x0000		//ART register,
};

int Freq_Cal_Tab[]=
{
        0x0A24,	0xC7F0, //0xC805,	//470.801MHz
        0x0A24,	0xDC05,	//471.801MHz
        0x0A24,	0xF005,	//472.801MHz
        0x0A25,	0x0405,	//473.801MHz
        0x0A25,	0x1805,	//474.801MHz
        0x0A25,	0x2C05,	//475.801MHz
        0x0A25,	0x4005,	//476.801MHz
        0x0A25,	0x5405,	//477.801MHz
        0x0A25,	0x6805,	//478.801MHz
        0x0A25,	0x7C05,	//479.801MHz
        0x0A25,	0x9005,	//480.801MHz
};
int Freq_Cal_Tab_chek[]=
{
        0x0A24,	0xB805,	//470.001MHz
	0x0A26, 0x4805,	//490.001MHz
	0x0A27, 0xD805,	//510.001MHz 
};
//datarate &&DEV    5.2K-DEV==0xF136,20K-DEV=0xF333
const Uint16 DataRates[]=
{
  0x3EC4,0x18D4,0x8312,0xF136,//SYS,RXI,IF1,TX1,2.4K_________12.8M
  0x1EC4,0x18D4,0x8312,0xF136,//SYS,RXI,IF1,TX1,4.8K_________12.8M
  0x0EC4,0x18D4,0x8312,0xF1C3,//0xF333,//SYS,RXI,IF1,TX1,9.6K_________12.8M
  0x06C4,0x18D4,0x8312,0xF333,//SYS,RXI,IF1,TX1,19.2K_________12.8M
  0x02C4,0x18D4,0x8312,0xF333,//SYS,RXI,IF1,TX1,38.4K_________12.8M
};

const Uint8 LSD_ID_Tab[8]={0x48,0x52,0x48,0x52,0xC7,0x33,0x45,0xE7};   //ID code



#endif

