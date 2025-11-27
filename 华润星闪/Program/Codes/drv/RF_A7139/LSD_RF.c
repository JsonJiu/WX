////////////////////////////////////////////////////////////////////////////////
// 版权:     利尔达科技集团股份有限公司
// 文件名:   LSD_RF.c
// 版本：    v1.0
// 工作环境: IAR  v5.30
// 作者:     于海波
// 生成日期: 2013.11
// 功能:     
// 相关文件: 
// 修改日志：
////////////////////////////////////////////////////////////////////////////////
#include "LSD_RFreg.h"
#include "LSD_RF.h"
#include "LSD_RFregsetting.h"
#include "rfconfig.h"
unsigned short RFTiming;

StuA7139Config A7139RunConfig;

extern RF_CONFIG RFConfig;
extern APP_Frame_t frameBuf;
//**********************************************************************************
// 功能描述 : 发送1字节
// 输入参数 : Uint8 src
// 返回参数 : 无
// 说明     : 
//**********************************************************************************
void LSD_RF_ByteSend(Uint8 src)
{
    Uint8 i;
    for(i = 0; i < 8; i++)
    {
        if(src & 0x80)
            LSD_SDIO_OH;  // P6OUT |= BIT1;
        else
           LSD_SDIO_OL;  // P6OUT &=~ BIT1;
        
        LSD_DELAY;        // _NOP();
        LSD_SCK_H;
        LSD_DELAY;
        LSD_SCK_L;
        src = src << 1;
    }
}
//**********************************************************************************
// 功能描述 : 读1字节
// 输入参数 : 无
// 返回参数 : Uint8
// 说明     : 
//**********************************************************************************
Uint8 LSD_RF_ByteRead(void)
{
    Uint8 i = 0,tmp = 0;
    LSD_SDIO_OH;         // SDIO pull high
    LSD_SDIO_MI;         // change SDIO input 
    for(i = 0; i < 8; i++) // Read one byte data
    {
        if(LSD_SDIO_IN)
            tmp = (tmp << 1) | 0x01;
        else
            tmp = tmp << 1;
        
        LSD_DELAY;
        LSD_SCK_H;
        LSD_DELAY;
        LSD_SCK_L;
    }
    LSD_SDIO_MO;        //这里应该将SDIO重新置位输出.因为很多其他写入没有置位输出.后续修改
    return tmp;           // Return tmp value.
}
//**********************************************************************************
// 功能描述 : 发送控制命令
// 输入参数 : Uint8 cmd
// 返回参数 : 无
// 说明     : 
//**********************************************************************************
void LSD_RF_StrobeCMD(Uint8 cmd)
{
    LSD_SCS_L;
    LSD_SDIO_MO;
    LSD_RF_ByteSend(cmd);
    LSD_SCS_H;
}
//**********************************************************************************
// 功能描述 : 写寄存器
// 输入参数 : Uint8 address地址 Uint16 dataWord数据
// 返回参数 : 无
// 说明     : 
//**********************************************************************************
void LSD_RF_WriteReg(Uint8 address, Uint16 dataWord)
{
    Uint8 dataWord1;
    Uint8 dataWord0;
    dataWord0 = (dataWord)&0x00FF;
    dataWord1 = (dataWord >> 8)&0x00FF;
    LSD_SCS_L;// Set SCS=0 to Enable SPI interface
    LSD_SDIO_MO;                       //// change SDIO outputTRXEM_SDIO_PORT_DIR |= TRXEM_SDIO_PIN
    address |= CMD_CTRLW;                //// Enable write operation of control registers//#define CMD_CTRLW 0x00
    LSD_RF_ByteSend(address); // Assign control register’s address by input variable addr
    LSD_DELAY;
    LSD_RF_ByteSend(dataWord1);// Assign control register’s value by input variable dataByte
    LSD_RF_ByteSend(dataWord0);
    LSD_SCS_H; // Set SCS=1 to disable SPI interface.
}
//**********************************************************************************
// 功能描述 : 读寄存器
// 输入参数 : Uint8 address地址
// 返回参数 : 寄存器值
// 说明     : 
//**********************************************************************************
Uint16 LSD_RF_ReadReg(Uint8 address)
{
    Uint16 tmp;
    LSD_SCS_L;// Set SCS=0 to enable SPI write function.
    LSD_SDIO_MO;// change SDIO output
    address |= CMD_CTRLR;// Enable read operation of control registers.
    LSD_RF_ByteSend(address);
    LSD_DELAY;
    tmp = LSD_RF_ByteRead()<<8;
    tmp = (tmp&0xff00) | (LSD_RF_ByteRead()&0x00ff);   
    LSD_SCS_H; // Set SCS=1 to disable SPI interface.
    LSD_SDIO_MO;  /*TRXEM_SDIO_MO是后加上去的*/
    return tmp;// Return 16 bit value
}


////////////////////////////////////////////////////////////////////////////////
// 功能描述 : 写pageA寄存器
// 输入参数 : 
// 返回参数 : 
// 说明     : 
////////////////////////////////////////////////////////////////////////////////
void LSD_RF_WritePageA(Uint8 address, Uint16 dataWord)
{
    Uint16 tmp;

    tmp = address;
    tmp = ((tmp << 12) | LSD_RFregConfig[CRYSTAL_REG]);  /*it's different here*/
    LSD_RF_WriteReg(CRYSTAL_REG, tmp);
    LSD_RF_WriteReg(PAGEA_REG, dataWord);
}

////////////////////////////////////////////////////////////////////////////////
// 功能描述 : 读PAGEA寄存器
// 输入参数 : 无
// 返回参数 : 无
// 说明     : 
////////////////////////////////////////////////////////////////////////////////
Uint16 LSD_RF_ReadPageA(Uint8 address)
{
    Uint16 tmp;

    tmp = address;
    tmp = ((tmp << 12) | LSD_RFregConfig[CRYSTAL_REG]);
    LSD_RF_WriteReg(CRYSTAL_REG, tmp);
    tmp = LSD_RF_ReadReg(PAGEA_REG);
    /*here lost  A7108_WriteReg(CRYSTAL_REG, temp_Crystal_Reg);
    is it because never change CRYSTAL_REG?*/
    return tmp; 
}

/************************************************************************
**  A7139_WritePageB
************************************************************************/
void LSD_RF_WritePageB(Uint8 address, Uint16 dataWord)
{
    Uint16 tmp;

    tmp = address;
    tmp = ((tmp << 7) | LSD_RFregConfig[CRYSTAL_REG]);
    LSD_RF_WriteReg(CRYSTAL_REG, tmp);
    LSD_RF_WriteReg(PAGEB_REG, dataWord);
}

/************************************************************************
**  A7139_ReadPageB
************************************************************************/
Uint16 LSD_RF_ReadPageB(Uint8 address)
{
    Uint16 tmp;

    tmp = address;
    tmp = ((tmp << 7) | LSD_RFregConfig[CRYSTAL_REG]);
    LSD_RF_WriteReg(CRYSTAL_REG, tmp);
    tmp = LSD_RF_ReadReg(PAGEB_REG);
    return tmp;
}
//**********************************************************************************
// 功能描述 : RF初始化
// 输入参数 : 无
// 返回参数 : 无
// 说明     : 
//**********************************************************************************
Uint8 LSD_RF_InitRF(void)
{
    // init io pin
    LSD_SCS_MO;
    LSD_SCK_MO;
    LSD_SDIO_MO;
    LSD_GIO1_MI;
    // AMICCOM_SPISetup();笙科初始化程序
    LSD_SCS_H;
    LSD_SCK_L;                           // CPOL=0,CPAH=0
    LSD_SDIO_OH;

    IRQ_Int_Init;
    
    LSD_RF_StrobeCMD(CMD_RF_RST);	//reset  chip
    DelayMs(1);
    if(LSD_RF_Config())		//config A7139 chip
      return 1;
    DelayMs(1);		//for crystal stabilized
    if(LSD_RF_WriteID())		//write ID code
      return 1;
    if(LSD_RF_Cal())		//IF and VCO calibration
      return 1;
    LSD_DELAY;
    LSD_DELAY;
    LSD_DELAY;
    return 0;
}

/*********************************************************************
** A7139_Config
*********************************************************************/
Uint8 LSD_RF_Config(void)
{
    Uint8 i;
    Uint16 tmp;

    for(i=0; i<8; i++)
        LSD_RF_WriteReg(i, LSD_RFregConfig[i]);

    for(i=10; i<16; i++)
        LSD_RF_WriteReg(i, LSD_RFregConfig[i]);

    for(i=0; i<16; i++)
        LSD_RF_WritePageA(i, LSD_RFregConfig_PageA[i]);

    for(i=0; i<5; i++)
        LSD_RF_WritePageB(i, LSD_RFregConfig_PageB[i]);

    //for check        
    tmp = LSD_RF_ReadReg(SYSTEMCLOCK_REG);
    if(tmp != LSD_RFregConfig[SYSTEMCLOCK_REG])
    {
//        LSD_Err_State();	
        return 1;
    }
    return 0;
}

/************************************************************************
**  WriteID
************************************************************************/
Uint8 LSD_RF_WriteID(void)
{
    Uint8 i;
    Uint8 d1, d2, d3, d4;

    LSD_SCS_L;
    LSD_RF_ByteSend(CMD_ID_W);
    for(i=0; i<4; i++)
      LSD_RF_ByteSend(frameBuf.SyncByte[i]);
    
    LSD_SCS_H;
    LSD_SCS_L;
    
    LSD_RF_ByteSend(CMD_ID_R);
    d1=LSD_RF_ByteRead();
    d2=LSD_RF_ByteRead();
    d3=LSD_RF_ByteRead();
    d4=LSD_RF_ByteRead();
    LSD_SCS_H;
	
    if((d1!=frameBuf.SyncByte[0]) || (d2!=frameBuf.SyncByte[1]) || (d3!=frameBuf.SyncByte[2]) || (d4!=frameBuf.SyncByte[3]))
    {
        return 1;
    }
    return 0;
}

////////////////////////////////////////////////////////////////////////////////
// 功能描述 : FreqCalSet
// 输入参数 : ch   建议ch=0~75
// 返回参数 : 无
// 说明     : 
////////////////////////////////////////////////////////////////////////////////
void LSD_RF_FreqSet(Uint8 ch)
{
//    Uint16 unnDataTemp;
//    unnDataTemp = ch*16;// 200k
//    LSD_RF_StrobeCMD(CMD_STBY);          //进入空闲
//    LSD_RF_WritePageB(IF2_PAGEB, unnDataTemp);//setting PLL1     LSD_RF_WritePageB
    LSD_RF_StrobeCMD(CMD_STBY);                // 准备状态切换  
    LSD_RF_WriteReg(PLL1_REG, Freq_Cal_Tab[2*ch]);//setting PLL1
    LSD_RF_WriteReg(PLL2_REG, Freq_Cal_Tab[2*ch+1]);//setting PLL2
}

/*********************************************************************
** A7139_Cal
*********************************************************************/
Uint8 LSD_RF_Cal(void)
{
    unsigned char i = 0;
//    Uint8 i;
    Uint8 fbcf;	//IF Filter
    Uint8 vbcf;		//VCO Current
    Uint8 vccf;	        //VCO Band
    Uint16 tmp;

    //IF calibration procedure @STB state
    LSD_RF_WriteReg(MODE_REG, LSD_RFregConfig[MODE_REG] | 0x0802);	 //IF Filter & VCO Current Calibration
    do{
        tmp = LSD_RF_ReadReg(MODE_REG);
    }while(tmp & 0x0802);
	
    //for check(IF Filter)
    tmp = LSD_RF_ReadReg(CALIBRATION_REG);
    fbcf = (tmp>>4) & 0x01;
    if(fbcf)
    {
//        LSD_Err_State();
        return 1;
    }

	//for check(VCO Current)
    tmp = LSD_RF_ReadPageA(VCB_PAGEA);
    vccf = (tmp>>4) & 0x01;
    if(vccf)
    {
//        LSD_Err_State();
      return 1;
    }
    
    
    //RSSI Calibration procedure @STB state
    LSD_RF_WriteReg(ADC_REG, 0x4C00);					//set ADC average=64
    LSD_RF_WritePageA(WOR2_PAGEA, 0xF800);					//set RSSC_D=40us and RS_DLY=80us
    LSD_RF_WritePageA(TX1_PAGEA, LSD_RFregConfig_PageA[TX1_PAGEA] | 0xE000);	//set RC_DLY=1.5ms
    LSD_RF_WriteReg(MODE_REG, LSD_RFregConfig[MODE_REG] | 0x1000);			//RSSI Calibration
    do{
        tmp = LSD_RF_ReadReg(MODE_REG);
    }while(tmp & 0x1000);
    LSD_RF_WriteReg(ADC_REG, LSD_RFregConfig[ADC_REG]);
    LSD_RF_WritePageA(WOR2_PAGEA, LSD_RFregConfig_PageA[WOR2_PAGEA]);
    LSD_RF_WritePageA(TX1_PAGEA, LSD_RFregConfig_PageA[TX1_PAGEA]);

    //VCO calibration procedure @STB state
    for(i = 0;i < 3;i++)
    {
        LSD_RF_WriteReg(PLL1_REG, Freq_Cal_Tab_chek[i*2]);
        LSD_RF_WriteReg(PLL2_REG, Freq_Cal_Tab_chek[i*2+1]);
        LSD_RF_WriteReg(MODE_REG, LSD_RFregConfig[MODE_REG] | 0x0004);	//VCO Band Calibration
        do{
                tmp = LSD_RF_ReadReg(MODE_REG);
        }while(tmp & 0x0004);

        //for check(VCO Band)
        tmp = LSD_RF_ReadReg(CALIBRATION_REG);
        vbcf = (tmp >>8) & 0x01;
        if(vbcf)
        {
           //LSD_Err_State();
          return 1;
        }
    }
    
        LSD_RF_WriteReg(PLL1_REG, Freq_Cal_Tab[0*2]);
        LSD_RF_WriteReg(PLL2_REG, Freq_Cal_Tab[0*2+1]);
        LSD_RF_WriteReg(MODE_REG, LSD_RFregConfig[MODE_REG] | 0x0004);	//VCO Band Calibration
        do{
                tmp = LSD_RF_ReadReg(MODE_REG);
        }while(tmp & 0x0004);

        //for check(VCO Band)
        tmp = LSD_RF_ReadReg(CALIBRATION_REG);
        vbcf = (tmp >>8) & 0x01;
        if(vbcf)
        {
           //LSD_Err_State();
          return 1;
        }
        
        return 0;
}

////////////////////////////////////////////////////////////////////////////////
// 功能描述 : 发送数据
// 输入参数 : Uint8 *txBuffer:发送数据存储数组首地址,
//            Uint8 size：发送数据长度
// 返回参数 : 无
// 说明     : 
////////////////////////////////////////////////////////////////////////////////
void LSD_RF_SendPacket(Uint8 *txBuffer, Uint8 size)
{
    Uint8 i;
    A7139_GIO1_IntDis;							//关闭中断

    //LSD_GPIO_INT_PORT_IFG &=~ LSD_GPIO_INT_PIN; 
    //LSD_GPIO_INT_PORT_IE &=~ LSD_GPIO_INT_PIN;   //发送数据包前关闭中断
    LSD_RF_StrobeCMD(CMD_STBY);          //进入空闲
    DelayUs(20);
    LSD_RF_WritePageA(FIFO_PAGEA, size - 1);
    LSD_RF_StrobeCMD(CMD_TFR);		//TX FIFO address pointer reset
    LSD_SCS_L;
    LSD_RF_ByteSend(CMD_FIFO_W);	//TX FIFO write command
    for(i=0; i <size; i++)
    LSD_RF_ByteSend(txBuffer[i]);
    LSD_SCS_H;
    LSD_RF_StrobeCMD(CMD_TX);
    DelayUs(10);
    while(GIO1_IN); 		    //wait transmit completed

}

void SendAgain(void)
{
    if(GIO1_IN)
        return; 		    //wait transmit completed
    LSD_RF_StrobeCMD(CMD_TFR);		//TX FIFO address pointer reset
    LSD_RF_StrobeCMD(CMD_TX);
}
////////////////////////////////////////////////////////////////////////////////
// 功能描述 : 接收数据包
// 输入参数 : Uint8 *cRxBuf,返回数据存储首地址。 Uint8 cLength 读的数据长度
// 返回参数 : 无
// 说明     : 
////////////////////////////////////////////////////////////////////////////////
void LSD_RF_RxPacket(Uint8 *cRxBuf, Uint8 cLength)
{
    Uint8 i;
    LSD_RF_StrobeCMD(CMD_RFR);		//RX FIFO address pointer reset
    LSD_SCS_L;
    LSD_RF_ByteSend(CMD_FIFO_R);	//RX FIFO read command
    for(i=0; i <cLength; i++)
    {
        cRxBuf[i] = LSD_RF_ByteRead();
    }
    LSD_SCS_H;

//    for(i=0; i<64; i++)
//    {
//        recv = tmpbuf[i];
//        tmp = recv ^ PN9_Tab[i];
//        if(tmp!=0)
//        {
//            Err_ByteCnt++;
//            Err_BitCnt += (BitCount_Tab[tmp>>4] + BitCount_Tab[tmp & 0x0F]);
//        }
//    }
}

/*********************************************************************
** Err_State
*********************************************************************/
//void LSD_Err_State(void)
//{
//    //ERR display
//    //Error Proc...
//    //...
//    while(1);
//}

/*********************************************************************
** RC Oscillator Calibration
*********************************************************************/
void LSD_RF_RCOSC_Cal(void)
{
      Uint16 tmp;

      LSD_RF_WritePageA(WOR2_PAGEA, LSD_RFregConfig_PageA[WOR2_PAGEA] | 0x0010);		//enable RC OSC

      while(1)
      {
            LSD_RF_WritePageA(WCAL_PAGEA, LSD_RFregConfig_PageA[WCAL_PAGEA] | 0x0001);	//set ENCAL=1 to start RC OSC CAL
            do{
                    tmp = LSD_RF_ReadPageA(WCAL_PAGEA);
            }while(tmp & 0x0001);
                    
            tmp = (LSD_RF_ReadPageA(WCAL_PAGEA) & 0x03FF);		//read NUMLH[8:0]
            tmp>>=1;	
            if((tmp>186)&&(tmp<198))	//NUMLH[8:0]~192
            {
                    break;
            }
      }
}

////////////////////////////////////////////////////////////////////////////////
// 功能描述 : 进入接收
// 输入参数 : 无
// 返回参数 : 无
// 说明     : 
////////////////////////////////////////////////////////////////////////////////
void LSD_RF_RXmode(void)
{
    LSD_RF_StrobeCMD(CMD_STBY);            //进入RX,准备接受数据
    LSD_RF_WritePageA(FIFO_PAGEA, 63);
    LSD_RF_StrobeCMD(CMD_RX);            //进入RX,准备接受数据
    A7139_GIO1_IntEn;						//允许中断
    //ir_int5ic = 0;				//清除中断标志
    A7139RunConfig.RXOK = 0;
    //LSD_GPIO_INT_PORT_IFG &=~ LSD_GPIO_INT_PIN; 
    //LSD_GPIO_INT_PORT_IE |= LSD_GPIO_INT_PIN;      //进入接收状态要开启中断
    
}
///*********************************************************************
//** WOR_enable_by_preamble
//*********************************************************************/
//void WOR_enable_by_preamble(void)
//{
//	StrobeCMD(CMD_STBY);
//	
//	A7139_WritePageA(GIO_PAGEA, 0x004D);	//GIO1=PMDO, GIO2=WTR
//
//	//Real WOR Active Period = (WOR_AC[5:0]+1) x 244us  X'TAL and Regulator Settling Time
//	//Note : Be aware that Xˇtal settling time requirement includes initial tolerance, 
//	//       temperature drift, aging and crystal loading.
//	A7139_WritePageA(WOR1_PAGEA, 0x8005);	//setup WOR Sleep time and Rx time
//	
//	RCOSC_Cal();     	//RC Oscillator Calibration
//	
//	A7139_WritePageA(WOR2_PAGEA, A7139Config_PageA[WOR2_PAGEA] | 0x0030);	//enable RC OSC & WOR by preamble
//	
//	A7139_WriteReg(MODE_REG, A7139Config[MODE_REG] | 0x0200);		//WORE=1 to enable WOR function
//	
//	while(GIO1==0);		//Stay in WOR mode until receiving preamble(preamble ok)
//	
//}
//
///*********************************************************************
//** WOR_enable_by_sync
//*********************************************************************/
//void WOR_enable_by_sync(void)
//{
//	StrobeCMD(CMD_STBY);
//	
//	A7139_WritePageA(GIO_PAGEA, 0x0045);	//GIO1=FSYNC, GIO2=WTR
//
//	//Real WOR Active Period = (WOR_AC[5:0]+1) x 244us  X'TAL and Regulator Settling Time
//	//Note : Be aware that Xˇtal settling time requirement includes initial tolerance, 
//	//       temperature drift, aging and crystal loading.
//	A7139_WritePageA(WOR1_PAGEA, 0x8005);	//setup WOR Sleep time and Rx time
//	
//	RCOSC_Cal();     	//RC Oscillator Calibration
//	
//	A7139_WritePageA(WOR2_PAGEA, A7139Config_PageA[WOR2_PAGEA] | 0x0010);	//enable RC OSC & WOR by sync
//	
//	A7139_WriteReg(MODE_REG, A7139Config[MODE_REG] | 0x0200);		//WORE=1 to enable WOR function
//	
//	while(GIO1==0);		//Stay in WOR mode until receiving ID code(sync ok)
//	
//}

///*********************************************************************
//** WOR_enable_by_carrier
//*********************************************************************/
//void WOR_enable_by_carrier(void)
//{
//	StrobeCMD(CMD_STBY);
//	
//	A7139_WritePageA(GIO_PAGEA, 0x0049);	//GIO1=CD, GIO2=WTR
//
//	//Real WOR Active Period = (WOR_AC[5:0]+1) x 244us  X'TAL and Regulator Settling Time
//	//Note : Be aware that Xˇtal settling time requirement includes initial tolerance, 
//	//       temperature drift, aging and crystal loading.
//	A7139_WritePageA(WOR1_PAGEA, 0x8005);	//setup WOR Sleep time and Rx time
//	
//	RCOSC_Cal();     	//RC Oscillator Calibration
//	
//	A7139_WritePageA(WOR2_PAGEA, A7139Config_PageA[WOR2_PAGEA] | 0x0410);	//enable RC OSC & WOR by carrier
//	A7139_WriteReg(ADC_REG, A7139Config[ADC_REG] | 0x8096);			//ARSSI=1, RTH=150
//	A7139_WritePageA(RFI_PAGEA, A7139Config_PageA[RFI_PAGEA] | 0x6000);	//RSSI Carrier Detect plus In-band Carrier Detect
//	A7139_WritePageB(ACK_PAGEB, A7139Config_PageB[ACK_PAGEB] | 0x0200);	//CDRS=[01]
//	A7139_WritePageA(VCB_PAGEA, A7139Config_PageA[VCB_PAGEA] | 0x4000);	//CDTM=[01]
//	
//	A7139_WriteReg(MODE_REG, A7139Config[MODE_REG] | 0x0200);		//WORE=1 to enable WOR function
//	
//	while(GIO1==0);		//Stay in WOR mode until carrier signal strength is greater than the value set by RTH[7:0](carrier ok)
//	
//}
//
///*********************************************************************
//** WOT_enable
//*********************************************************************/
//void WOT_enable(void)
//{
//	StrobeCMD(CMD_STBY);
//	
//	A7139_WritePageA(GIO_PAGEA, 0x0045);	//GIO1=FSYNC, GIO2=WTR
//
//	//Real WOR Active Period = (WOR_AC[5:0]+1) x 244us  X'TAL and Regulator Settling Time
//	//Note : Be aware that Xˇtal settling time requirement includes initial tolerance, 
//	//       temperature drift, aging and crystal loading.
//	A7139_WritePageA(WOR1_PAGEA, 0x8005);	//setup WOR Sleep time and Rx time
//	
//	RCOSC_Cal();     		//RC Oscillator Calibration
//	
//	A7139_WriteFIFO();		//write data to TX FIFO
//
//	A7139_WriteReg(PIN_REG, A7139Config[PIN_REG] | 0x0400);		//WMODE=1 select WOT function
//	
//	A7139_WriteReg(MODE_REG, A7139Config[MODE_REG] | 0x0200);	//WORE=1 to enable WOR function
//	
//	while(1);	
//	
//}
//
///*********************************************************************
//** TWOR_enable
//*********************************************************************/
//void TWOR_enable(void)
//{
//	StrobeCMD(CMD_STBY);
//	
//	A7139_WritePageA(GIO_PAGEA, 0x0051);	//GIO1=TWOR, GIO2=WTR
//
//	//Real WOR Active Period = (WOR_AC[5:0]+1) x 244us  X'TAL and Regulator Settling Time
//	//Note : Be aware that Xˇtal settling time requirement includes initial tolerance, 
//	//       temperature drift, aging and crystal loading.
//	A7139_WritePageA(WOR1_PAGEA, 0x8005);	//setup WOR Sleep time and Rx time
//	
//	RCOSC_Cal();     		//RC Oscillator Calibration
//	
//	A7139_WritePageA(WOR2_PAGEA, A7139Config_PageA[WOR2_PAGEA] | 0x0014);	//start TWOR by WOR_AC
//	//A7139_WritePageA(WOR2_PAGEA, A7139Config_PageA[WOR2_PAGEA] | 0x001C);	//start TWOR by WOR_SL
//	
//	StrobeCMD(CMD_SLEEP);	//entry sleep mode
//	
//	while(1);
//
//}
//
///*********************************************************************
//** RSSI_measurement
//*********************************************************************/
//void RSSI_measurement(void)
//{
//	Uint16 tmp;
//
//	StrobeCMD(CMD_STBY);
//	
//	A7139_WritePageA(GIO_PAGEA, 0x0045);			//GIO1=FSYNC, GIO2=WTR
//	
//	A7139_WriteReg(ADC_REG, A7139Config[ADC_REG] | 0x8000);	//ARSSI=1
//	
//	StrobeCMD(CMD_RX);	//entry RX mode
//	Delay10us(1);
//	
//	while(GIO1==0)		//Stay in WOR mode until receiving ID code(sync ok)
//	{
//		tmp = (A7139_ReadReg(ADC_REG) & 0x00FF);	//read RSSI value(environment RSSI)
//	}
//	tmp = (A7139_ReadReg(ADC_REG) & 0x00FF);		//read RSSI value(wanted signal RSSI)
//	
//}
//
///*********************************************************************
//** FIFO_extension_Infinite_TX
//*********************************************************************/
//void FIFO_extension_Infinite_TX(void)
//{
//	Uint8 i, n;
//	Uint16 j;	
//
//	//for Infinite : FEP[13:0]=remainder,  5 < FEP[13:0] < 63
//	A7139_WritePageA(VCB_PAGEA, 0x0004);
//	A7139_WritePageA(FIFO_PAGEA, 0x0006);
//
//	A7139_WritePageA(GIO_PAGEA, 0x0075);	//GIO1=FPF, GIO2=WTR
//	A7139_WritePageA(CKO_PAGEA, 0xD283);	//CKO=DCK
//	
//	A7139_WriteReg(PIN_REG, 0x0A00);	//set INFS=1, infinite length
//
//	n=0;
//
//	SCS=0;
//	ByteSend(CMD_FIFO_W);		        //write 1st TX FIFO 64bytes
//	for(i=0; i<64; i++)
//	{
//		ByteSend(n);
//		if(n==255)
//			n=0;
//		else
//			n++;
//	}
//	SCS=1;
//
//	StrobeCMD(CMD_TX);			//entry TX mode
//
//	for(j=0; j<500; j++)
//	{
//		while(GIO1==0);			//wait FPF go high
//		while(GIO1==1);			//wait FPF go low
//
//		SCS=0;
//		ByteSend(CMD_FIFO_W);	//write TX FIFO 64bytes
//		for(i=0; i <64; i++)
//		{
//			ByteSend(n);
//			if(n==255)
//				n=0;
//			else
//				n++;
//		}
//		SCS=1;
//	}
//
//	//remainder : FEP[13:0]=6, FIFO Length=6+1=7
//	while(GIO1==0);				//wait last FPF go high
//	while(GIO1==1);				//wait last FPF go low
//
//	SCS=0;
//	ByteSend(CMD_FIFO_W);		//write TX FIFO 7bytes
//	for(i=0; i<7; i++)
//	{
//		ByteSend(n);
//		if(n==255)
//			n=0;
//		else
//			n++;
//	}
//	SCS=1;
//
//	A7139_WriteReg(PIN_REG, 0x0800);	//disable INFS
//	
//	while(GIO2);	//wait transmit completed
//}
//
///*********************************************************************
//** FIFO_extension_Infinite_RX
//*********************************************************************/
//void FIFO_extension_Infinite_RX(void)
//{
//    Uint8 i, n;
//    Uint8 recv;
//    Uint8 tmp;
//	Uint16 j;
//
//	//for Infinite : FEP[13:0]=remainder,  5 < FEP[13:0] < 63
//	A7139_WritePageA(VCB_PAGEA, 0x0004);
//	A7139_WritePageA(FIFO_PAGEA, 0x0006);
//
//	A7139_WritePageA(GIO_PAGEA, 0x0075);	//GIO1=FPF, GIO2=WTR
//	A7139_WritePageA(CKO_PAGEA, 0xD28B);	//CKO=RCK
//
//	A7139_WriteReg(PIN_REG, 0x0A00);		//set INFS=1, infinite length
//
//	Err_ByteCnt=0;
//	Err_BitCnt=0;
//	n=0;
//	 
//	StrobeCMD(CMD_RX);			//entry RX mode
//
//	for(j=0; j<501 ; j++)
//	{
//		while(GIO1==0);			//wait FPF go high
//		while(GIO1==1);			//wait FPF go low
//
//		SCS=0;
//		ByteSend(CMD_FIFO_R);	//read RX FIFO 64bytes
//		for(i=0; i<64; i++)		
//			tmpbuf[i]=ByteRead();
//		SCS=1;
//
//		//for check
//		for(i=0; i<64; i++)
//		{
//			recv = tmpbuf[i];
//			tmp = recv ^ n;
//			if(tmp!=0)
//        	{
//				Err_ByteCnt++;
//            	Err_BitCnt += (BitCount_Tab[tmp>>4] + BitCount_Tab[tmp & 0x0F]);
//        	}
//
//			if(n==255)
//				n=0;
//			else
//				n++;
//		}
//	}
//
//	A7139_WriteReg(PIN_REG, 0x0800);	//disable INFS
//
//	while(GIO2);	//wait receive completed
//
//	//remainder : FEP[13:0]=6, FIFO Length=6+1=7
//	SCS=0;
//	ByteSend(CMD_FIFO_R);		//read RX FIFO 7bytes
//	for(i=0; i<7; i++)		
//		tmpbuf[i]=ByteRead();
//	SCS=1;
//
//	//for check
//	for(i=0; i<7; i++)
//	{
//		recv = tmpbuf[i];
//		tmp = recv ^ n;
//		if(tmp!=0)
//        {
//			Err_ByteCnt++;
//           	Err_BitCnt += (BitCount_Tab[tmp>>4] + BitCount_Tab[tmp & 0x0F]);
//        }
//
//		if(n==255)
//			n=0;
//		else
//			n++;
//	}
//}
//
///*********************************************************************
//** Auto_Resend
//*********************************************************************/
//void Auto_Resend(void)
//{
//	StrobeCMD(CMD_STBY);
//
//	A7139_WritePageA(GIO_PAGEA, 0x0071);	//GIO1=VPOAK, GIO2=WTR
//	A7139_WritePageA(CKO_PAGEA, 0xD283);	//CKO=DCK
//
//	A7139_WritePageB(ACK_PAGEB, 0x003F);	//enable Auto Resend, by event, fixed, ARC=15
//	A7139_WritePageB(ART_PAGEB, 0x001E);	//RND=0, ARD=30
//	//ACK format = Preamble + ID = 8bytes
//	//data rate=10kbps, Transmission Time=(1/10k)*8*8=6.4ms
//	//so set ARD=30, RX time=30*250us=7.75ms
//
//	while(1)
//	{
//		A7139_WriteFIFO();
//		StrobeCMD(CMD_TX);		//entry TX mode
//		Delay10us(1);
//		while(GIO2);			//wait transmit completed
//
//		//check VPOAK
//		if(GIO1)
//		{
//			//ACK ok
//		}
//		else
//		{
//			//NO ACK
//		}
//
//		StrobeCMD(CMD_STBY);	//clear VPOAK(GIO1 signal)
//
//		Delay10ms(1);
//	}
//}
//
///*********************************************************************
//** Auto_ACK
//*********************************************************************/
//void Auto_ACK(void)
//{
//	A7139_WritePageA(GIO_PAGEA, 0x0045);	//GIO1=FSYNC, GIO2=WTR
//	A7139_WritePageA(CKO_PAGEA, 0xD28B);	//CKO=RCK
//
//	A7139_WritePageB(ACK_PAGEB, 0x0001);	//enable Auto ACK
//
//	while(1)
//	{
//		StrobeCMD(CMD_RX);		//entry RX mode
//		Delay10us(1);			
//		while(GIO2);			//wait receive completed
//
//		RxPacket();
//	}
//}
//
///*********************************************************************
//** entry__sleep_mode
//*********************************************************************/
void LSD_RF_Sleepmode(void)
{
	LSD_RF_StrobeCMD(CMD_STBY);			//
	DelayUs(600);
        LSD_RF_StrobeCMD(CMD_SLEEP);			//entry sleep mode
}
//
///*********************************************************************
//** wake_up_from_deep_sleep_mode
//*********************************************************************/
//void wake_up_from_deep_sleep_mode(void)
//{
//	StrobeCMD(CMD_STBY);	//wake up (any Strobe CMD)
//	Delay100us(3);		//for VDD_D stabilized
//	InitRF();
//}

void LSD_RF_RateSet(unsigned char rate)
{
  unsigned char t = rate * 4;
  //波特率
  LSD_RF_WriteReg(0, DataRates[t]);
  LSD_RF_WriteReg(10, DataRates[t + 1]);
  LSD_RF_WritePageB(1, DataRates[t + 2]);
  //频偏
  LSD_RF_WritePageA(0, DataRates[t + 3]);
}
unsigned char RFBrokenTime = 0;

void LSDCheckRF(void)
{
    unsigned int tt;
    extern const Uint16 DataRates[];
    
    tt = LSD_RF_ReadReg(SYSTEMCLOCK_REG);
    if(tt != DataRates[RFConfig.rate * 4])
    {
        if (RFBrokenTime < 5) {
            LSD_RF_InitRF();
            LSD_RF_Config();
        }
    }
}

void LSD_RF_Setting(void)
{
  LSD_RF_StrobeCMD(CMD_STBY);
  DelayMs(1);
  LSD_RF_WriteID();
  LSD_RF_FreqSet(RFConfig.fre);
  LSD_RF_RateSet(RFConfig.rate);
  LSDCheckRF();
}

void A7139_GIO1_IRQServer(void)
{
    A7139RunConfig.RXOK = 1;
}
