#include "dctrl.h"
#include "drv_rfm4463.h"
#include "lib_public.h"
#include "RF_m4463.h"

#define SPI_SPEED          	87
#define IN                 	0
#define OUT                	1
//#define TRUE			1
//#define FALSE			0


unsigned char RFState = FALSE;//RF状态，初始化成功=TRUE,初始化失败=FALSE

UartMode Rfm4463_UartMode_ST = 
{
    UartBaud4800,0x00,0x01,0x01,0x00
};


//RF串口接收循环缓冲区
//unsigned char Rfm4463RevBuf[RFM4463REVBUFMAXLEN];
//unsigned char OverTurn = 0; //串口中断接收存储缓冲区读/写指针过缓冲区末标志位，用于判断读取长度
							//写指针过缓冲区末，OverTurn = 1，如果下次存储写指针超过读指针，读指针需要+1
							//读指针过缓冲区末，OverTurn = 0.

//unsigned short pRfm4463RevBufRead = 0,pRfm4463RevBufWrite = 0; //RF串口接收读写指针

Rfm4463_CtrlStateET Rfm4463_CtrlState_ET = RFM4463_CTRL_SLEEP;

/**
 * @declare 使能Rfm4463无线模块参数配置
 * 
 * @author ZYX (2014/11/5)
 *
 * requirements 
 */
void OR_Rfm4463_SPI_Enable(void)
{
    OR_Rfm4463_SDO_Reset();
    OR_Rfm4463_SCK_Reset();
    OR_Rfm4463_NSS_Reset();
}	

/**
 * @declare 关闭Rfm4464无线模块参数配置
 * 
 * @author ZYX (2014/11/5)
 *
 * requirements 
 */
void OR_Rfm4463_SPI_Disable(void)
{
    OR_Rfm4463_NSS_Set();
    OR_Rfm4463_SDO_Set();
    OR_Rfm4463_SCK_Reset();//输出高或者低，不影响功耗
}

/**
 * @declare Rfm4463无线模块参数配置写一个字节
 * 
 * @author ZYX (2014/11/5)
 *
 * requirements 
 * 
 * @param data 写字节
 */
void OR_Rfm4463_SPI_Write(unsigned char data)
{
	unsigned char i;
	for(i = 0;i < 8;i ++)
	{	  
		DelayUs(SPI_SPEED);
        if(data & 0x80)
			OR_Rfm4463_SDO_Set();
        else
			OR_Rfm4463_SDO_Reset();
         
		OR_Rfm4463_SCK_Set();
        data = data << 1;        
        DelayUs(SPI_SPEED);
		OR_Rfm4463_SCK_Reset();
	}
}

/**
 * @declare Rfm4463无线模块参数配置读一个字节
 * 
 * @author ZYX (2014/11/5)
 *
 * requirements 
 * 
 * @return unsigned char 
 */
unsigned char OR_Rfm4463_SPI_Read(void)
{
    unsigned char i,data_in = 0;
    for(i = 0;i < 8;i ++)
    {
        data_in = data_in << 1;
        OR_Rfm4463_SCK_Set();
        if( OR_Rfm4463_SDI_Read() )
            data_in = data_in | 0x01;
        DelayUs(SPI_SPEED);
        OR_Rfm4463_SCK_Reset();
        DelayUs(SPI_SPEED);
    }
    return data_in;
}

/**
 * @declare 
 *  		向Rfm4463无线模块发送A1指令，配置无线参数，包括无线频率，频偏，功耗
 * 
 * @author ZYX (2014/11/5)
 *
 * requirements 
 * 
 * @param Agc 配置参数缓冲区指针
 * 
 * @return int 配置成功返回TRUE，失败返回FALSE
 */
int OR_Rfm4463_Send_Cmd_A1(unsigned char *Agc)
{
    unsigned char i;
    int out_time;
    out_time = 20000;     	//10ms
    OR_Rfm4463_SPI_Enable();
    DelayUs(SPI_SPEED);
    OR_Rfm4463_SPI_Write(0xA1);
    DelayUs(SPI_SPEED);
    for(i = 0;i < 4;i ++)
    {
        OR_Rfm4463_SPI_Write(*(Agc + i));
        DelayUs(SPI_SPEED);
    }
    while( (out_time --)&&(OR_Rfm4463_ACK_Read() == 0) );
    OR_Rfm4463_SPI_Disable();
    if(out_time < 0)
        return FALSE;
    else
        return TRUE;
}

/**
 * @declare 向Rfm4463无线模块发送A0读配置参数指令
 * 
 * @author ZYX (2014/11/5)
 *
 * requirements 
 * 
 * @param Agc 读参数保存缓冲区指针
 * 
 * @return int 成功返回TRUE
 */
int OR_Rfm4463_Send_Cmd_A0(unsigned char *Agc)
{
    unsigned char i;
	OR_Rfm4463_SPI_Enable();
	DelayUs(SPI_SPEED);
	OR_Rfm4463_SPI_Write(0xA0);
	DelayUs(SPI_SPEED);
	for(i = 0;i < 4;i ++)
	{
		*(Agc + i) = OR_Rfm4463_SPI_Read();
		DelayUs(SPI_SPEED);
	}
	OR_Rfm4463_SPI_Disable();
	return TRUE;
}

/**
 * @declare 向Rfm4463无线模块发送A2读RSSI指令
 * 
 * @author ZYX (2014/11/5)
 *
 * requirements 
 * 
 * @return int 返回1个字节RSSI值
 */
int OR_Rfm4463_Send_Cmd_A2(void)
{
	unsigned char data_in;
	short out_time;
	out_time = 20000;   //10ms
	OR_Rfm4463_SPI_Enable();
	DelayUs(SPI_SPEED);
	OR_Rfm4463_SPI_Write(0xA2);
	DelayUs(SPI_SPEED);
	while((out_time --) && (OR_Rfm4463_ACK_Read() != 0x01));
	if(out_time < 0)
		return FALSE;
	data_in = OR_Rfm4463_SPI_Read();
	OR_Rfm4463_SPI_Disable();
	return data_in;
}


/**
 * @declare 复位Rfm4463无线模块，并置模块于休眠状态
 * 
 * @author ZYX (2014/11/5)
 *
 * requirements 
 * 
 * @return int 成功返回TRUE，失败返回FALSE
 */
int OR_Rfm4463_Reset(void)
{
    unsigned char cnt = 22;
    OR_Rfm4463_Uart_Cmd(DISABLE);
    OR_Rfm4463_RFDIO_DIR_Ctrl(IN);
    OR_Rfm4463_SDO_Set();
    OR_Rfm4463_RFRX_Reset();
    OR_Rfm4463_RFTX_Reset();
    OR_Rfm4463_RST_Reset();
    DelayUs(10000);   //10ms
    OR_Rfm4463_RST_Set();

    DelayMs(300);
    while(cnt --)
    {
        DelayMs(100);
        if(OR_Rfm4463_ACK_Read())
        {
            OR_Rfm4463_RFRX_Set();
            OR_Rfm4463_RFTX_Set();
            Rfm4463_CtrlState_ET = RFM4463_CTRL_SLEEP;
            return TRUE;
        }
    }
    OR_Rfm4463_RFRX_Set();
    OR_Rfm4463_RFTX_Set();
    return FALSE;
}

/**
 * @declare 设置Rfm4463模块为参数配置状态
 * 
 * @author ZYX (2014/11/5)
 *
 * requirements 
 * 
 * @return int 成功返回TRUE，失败返回FASLE
 */
int OR_Rfm4463_Config_Init(void)
{
    if(Rfm4463_CtrlState_ET != RFM4463_CTRL_SLEEP)
        return FALSE;
    OR_Rfm4463_Uart_Cmd(DISABLE);
    OR_Rfm4463_RFDIO_DIR_Ctrl(IN);
    OR_Rfm4463_SDO_Set();
    OR_Rfm4463_RFRX_Reset();
    OR_Rfm4463_RFTX_Reset();
    DelayUs(SPI_SPEED);
    Rfm4463_CtrlState_ET = RFM4463_CTRL_CONFIG;
    return TRUE;
}

/**
 * @declare 设置Rfm4463无线模块于休眠状态
 * 
 * @author ZYX (2014/11/6)
 *
 * requirements 
 * 
 * @return int 成功返回TRUE
 */
int OR_Rfm4463_Sleep(void)
{
    OR_Rfm4463_Uart_Cmd(DISABLE);
    OR_Rfm4463_RFDIO_DIR_Ctrl(IN);
    OR_Rfm4463_RFTX_Set();
    OR_Rfm4463_RFRX_Set();
    DelayUs(SPI_SPEED);
    Rfm4463_CtrlState_ET = RFM4463_CTRL_SLEEP;
    return TRUE;
}

//RF相关IO口初始化
void OR_Rfm4463_IOInit(void)
{
    OR_Rfm4463_ACK_DIR_Ctrl(IN);

    OR_Rfm4463_RERCV_Set();
    OR_Rfm4463_RERCV_Dir_Ctrl(OUT);

    OR_Rfm4463_SDO_Set();
    OR_Rfm4463_SDO_DIR_Ctrl(OUT);

    OR_Rfm4463_RFCLK_DIR_Ctrl(IN);

    OR_Rfm4463_SDI_DIR_Ctrl(IN);

    OR_Rfm4463_RST_Set();
    OR_Rfm4463_RST_DIR_Ctrl(OUT);

    OR_Rfm4463_SCK_Set();
    OR_Rfm4463_SCK_DIR_Ctrl(OUT);

    OR_Rfm4463_NSS_Set();
    OR_Rfm4463_NSS_DIR_Ctrl(OUT);

    OR_Rfm4463_RFTX_Set();
    OR_Rfm4463_RFTX_DIR_Ctrl(OUT);

    OR_Rfm4463_RFRX_Set();
    OR_Rfm4463_RFRX_DIR_Ctrl(OUT);

    OR_Rfm4463_RFDIO_DIR_Ctrl(IN);

    OR_Rfm4463_RSSI_DIR_Ctrl(IN);
}
//static unsigned char RfInitIsErr = TRUE;//TRUE:succeed
/**
 * @declare 初始化Rfm4463无线模块
 * 
 * @author ZYX (2014/11/5)
 *
 * requirements 
 * 
 * @param pParm Rfm4463初始化参数缓冲区指针
 * 
 * @return int 成功返回TRUE，失败返回FALSE
 */
int OR_Rfm4463_Init(void)
{
    unsigned char read_parm[4],i,j;
    unsigned char baud_set;
    RFState = FALSE;
    OR_Rfm4463_IOInit();
    //RfInitIsErr = TRUE;
    if(Rfm4463_CtrlState_ET != RFM4463_CTRL_SLEEP)
        return FALSE;
	//baud_set = (RFParm[2] & 0x18) >> 3;
    for(j = 0;j < 4;j ++)
    {
        if(OR_Rfm4463_Reset() == TRUE)
            break;
    }
    if(j == 4)
    {   
        return FALSE;
    }
    OR_Rfm4463_Config_Init();
    for(j = 0; j < 4; j++)
    {
        DelayMs(50);
        if(OR_Rfm4463_Send_Cmd_A1(RFParaST.RFPara.para) == FALSE)
            continue;
        DelayMs(50);
    	OR_Rfm4463_Send_Cmd_A0(read_parm);
    	for(i = 0;i < 4;i ++)
    	{
            if( RFParaST.RFPara.para[i] != read_parm[i] )
                break;  //zyx_debug 2014.2.27
    	}
        if(i == 4)
            break;
    }
    OR_Rfm4463_Sleep();
    if( j==4 )
        return FALSE;
    baud_set = (RFParaST.RFPara.para[2] & 0x18) >> 3;
    switch(baud_set)
    {
    case 0x00:
        Rfm4463_UartMode_ST.Baud = UartBaud4800;
        break;
    case 0x01:
        Rfm4463_UartMode_ST.Baud = UartBaud9600;
        break;
    case 0x10:
        Rfm4463_UartMode_ST.Baud = UartBaud19200;
        break;
    case 0x11:
        Rfm4463_UartMode_ST.Baud = UartBaud38400;
        break;
    default:
        Rfm4463_UartMode_ST.Baud = UartBaud9600;
        break;
    }
    RFState = TRUE;
    return TRUE;
}

/**
 * @declare 设置Rfm4463模块为射频发送状态
 * 
 * @author ZYX (2014/11/5)
 *
 * requirements 
 * 
 * @return int 成功返回TRUE，失败返回FALSE
 */
int OR_Rfm4463_Send_Init(void)
{
	if(Rfm4463_CtrlState_ET != RFM4463_CTRL_SLEEP)
		return FALSE;
	OR_Rfm4463_Uart_Cmd(DISABLE);
	OR_Rfm4463_RFDIO_DIR_Ctrl(OUT);
	OR_Rfm4463_RFDIO_Set();
	OR_Rfm4463_RFTX_Reset();
	OR_Rfm4463_RFRX_Set();
	DelayUs(SPI_SPEED);
	Rfm4463_CtrlState_ET = RFM4463_CTRL_SEND;
	return TRUE;
}

/**
 * @declare Rfm4463模块位模式发送1字节射频数据
 * 
 * @author ZYX (2014/11/7)
 *
 * requirements 
 * 
 * @param byte 发送位指针
 * 
 * @return int 成功返回TRUE，失败返回FALSE
 */
int OR_Rfm4463_SendByte(unsigned char *byte)
{
    unsigned char i;
	unsigned short out_time;
	out_time = 1000;  //1ms
	for(i = 0;i < 10;i ++)
	{
		while(OR_Rfm4463_RFCLK_Read())	//wait RFCLK to H
		{
			out_time--;
			if(out_time == 0)
				return FALSE;
		}
		while(!OR_Rfm4463_RFCLK_Read())	//wait RFCLK to low
		{
			out_time--;
		    if(out_time == 0)
			    return FALSE;
		}
		if(i == 0)
		{
		    OR_Rfm4463_RFDIO_Reset();
		}
		else if(i == 9)
		{
			OR_Rfm4463_RFDIO_Set();
		}
		else
		{
			if(*byte & (0x01 << (i - 1)))	
				OR_Rfm4463_RFDIO_Set();
			else
			   OR_Rfm4463_RFDIO_Reset();
		}
	}
	return TRUE;
}

/**
 * @declare Rfm4463模块发送射频数据
 * 
 * @author ZYX (2014/11/7)
 *
 * requirements 
 * 
 * @param Buf 发送缓冲区指针
 * @param SendLen 发送长度
 * 
 * @return int 成功返回TRUE,失败返回FALSE
 */
int OR_Rfm4463_SendBit(unsigned char *Bit,unsigned short SendLen)
{
	//unsigned short i;
	unsigned short out_time;
	out_time = 1000;  //1ms
	//if(RF_State != RF_STATE_SEND)
			//return FALSE;
	//for(i = 0;i < SendLen;i ++)
	{
		while(OR_Rfm4463_RFCLK_Read())	//wait RFCLK to H
		{
			out_time--;
			if(out_time == 0)
				return FALSE;
		}
		while(!OR_Rfm4463_RFCLK_Read())	//wait RFCLK to low
		{
			out_time--;
			if(out_time == 0)
				return FALSE;
		}
		if(*Bit)	
			OR_Rfm4463_RFDIO_Set();
		else
			OR_Rfm4463_RFDIO_Reset();
		//Bit ++;
	}
	return TRUE;
}

/**
 * @declare 设置Rfm4463为射频接收状态
 * 
 * @author ZYX (2014/11/5)
 *
 * requirements 
 * 
 * @return int 成功返回TRUE，失败返回FALSE
 */
int OR_Rfm4463_Rec_Init(void)
{
    if(Rfm4463_CtrlState_ET != RFM4463_CTRL_SLEEP)
        return FALSE;
    //RfRecvIsErr = TRUE;
    OR_Rfm4463_Uart_Init(&Rfm4463_UartMode_ST,OR_Rfm4463_Uart_Interrupt);
    OR_Rfm4463_Uart_Cmd(ENABLE);
    //OR_Rfm4463_RFDIO_DIR_Ctrl(IN);
    OR_Rfm4463_RFTX_Set();
    OR_Rfm4463_RFRX_Reset();
    DelayUs(SPI_SPEED);
    Rfm4463_CtrlState_ET = RFM4463_CTRL_RECEIVE;
    return TRUE;
}

/**
 * @declare 返回Rfm4463最近接收数据
 * 
 * @author ZYX (2014/11/6)
 *
 * requirements 
 * 
 * @param RxData 接收缓冲区指针
 * 
 * @return unsigned short 接收缓冲区长度
 */
unsigned short OR_Rfm4463_RecData(unsigned char *Buf)
{
#if 0
   unsigned short i = 0,Len = 0,Len1 = 0,Len2 = 0;
   if( pRfm4463RevBufWrite > pRfm4463RevBufRead )
   {
        if(OverTurn == 0)
        {    
            Len = pRfm4463RevBufWrite - pRfm4463RevBufRead;
            for(i = 0;i < Len;i ++)
            {
             *(Buf ++) = Rfm4463RevBuf[pRfm4463RevBufRead ++];
            }
        }
        else//出错，复位
        {
            pRfm4463RevBufWrite = 0;
            pRfm4463RevBufRead  = 0;
            OverTurn            = 0;
        }

   }
   else if( pRfm4463RevBufWrite < pRfm4463RevBufRead )   	//取数据过程中串口中断若还在接收，会更改pRfm4463RevBufWrite指针
   {													//所以用临时变量确定长度
        if(OverTurn == 1)
        {
            Len1 = RFM4463REVBUFMAXLEN - pRfm4463RevBufRead;
            Len2 = pRfm4463RevBufWrite;
            Len = Len1 + Len2;
            for(i = 0;i < Len1;i ++)
            {
                *(Buf ++) = Rfm4463RevBuf[pRfm4463RevBufRead ++];
            }
            pRfm4463RevBufRead = 0;
            for(i = 0;i < Len2;i ++)
            {
                *(Buf ++) = Rfm4463RevBuf[pRfm4463RevBufRead ++];
            }
            OverTurn = 0;
        }
        else 
        {   //出错，复位
            pRfm4463RevBufWrite = 0;
            pRfm4463RevBufRead  = 0;
            OverTurn            = 0;
        }
   }
   else
   {
        if(OverTurn == 1)
        {
            Len = RFM4463REVBUFMAXLEN;
            for(i = 0;i  < Len;i ++)
            {
                *(Buf ++) = Rfm4463RevBuf[pRfm4463RevBufRead ++];
                if(pRfm4463RevBufRead >= RFM4463REVBUFMAXLEN)
                    pRfm4463RevBufRead = 0;
            }
        }
        else
            Len = 0;
   }
   return Len;
#endif
   return 0;
}

/**
 * @declare 配置Rfm4463无线模块状态
 * 
 * @author ZYX (2014/11/6)
 *
 * requirements 
 * 
 * @param Rfm4463_CtrlState_ET Rfm4463无线模块状态
 * 
 * @return int 成功返回TRUE，失败返回FALSE
 */
int OR_Rfm4463_Control(Rfm4463_CtrlStateET Rfm4463_CtrlState_ET)
{
   int state = TRUE;
   switch(Rfm4463_CtrlState_ET)
   {
   case(RFM4463_CTRL_SLEEP):
        if(OR_Rfm4463_Sleep() == FALSE)
            state = FALSE;
        break;
   case(RFM4463_CTRL_CONFIG):
       if(OR_Rfm4463_Config_Init() == FALSE)
          state = FALSE;
      break;
   case(RFM4463_CTRL_SEND):
      if(OR_Rfm4463_Send_Init() == FALSE)
         state = FALSE;
      break;
   case(RFM4463_CTRL_RECEIVE):
      if(OR_Rfm4463_Rec_Init() == FALSE)
         state = FALSE;
      break;
   default:
      state = FALSE;
   }
   return state;
}

/**
 * @declare 配置Rfm4463无线模块参数
 * 
 * @author ZYX (2014/11/6)
 *
 * requirements 
 * 
 * @param Agc 配置参数缓冲区指针
 * 
 * @return int 成功返回TRUE，失败返回FALSE
 */
int OR_Rfm4463_SetConfigValue(unsigned char *Agc)
{
    unsigned char i,j,read_parm[4];
    if(OR_Rfm4463_Control(RFM4463_CTRL_CONFIG) == TRUE)
    {  
      for(j = 0; j < 4; j++)
        {
            DelayMs(50);
            if(OR_Rfm4463_Send_Cmd_A1(Agc) == FALSE)
                continue;
            DelayMs(50);
            OR_Rfm4463_Send_Cmd_A0(read_parm);
            for(i = 0;i < 4;i ++)
            {
                if( Agc[i] != read_parm[i] )          
                    break;
            }
            if(i == 4)
                break;
        }
        OR_Rfm4463_Control(RFM4463_CTRL_SLEEP);
        if( j==4 )
            return FALSE;
        return TRUE;
    }
    return FALSE;
}

/**
 * @declare 读取Rfm4463无线模块参数
 * 
 * @author ZYX (2014/11/6)
 *
 * requirements 
 * 
 * @param Agc 读取配置参数缓冲区指针
 * 
 * @return int 成功返回TRUE，失败返回FALSe
 */
int OR_Rfm4463_GetConfigValue(unsigned char *Agc)
{
   if( OR_Rfm4463_Send_Cmd_A0(Agc) == TRUE )
      return TRUE;
   else
      return FALSE;
}

/**
 * @declare 
 *  		Rfm4463无线模块串口接收中断回调函数,新接收超过读指针时覆盖
 * 
 * @author ZYX (2014/11/6)
 *
 * requirements 覆盖存储！！！
 * 
 * @param RxData 接收到的字节
 */
void OR_Rfm4463_Uart_Interrupt(unsigned char RxData)
{
    PhyRecvData(&RxData,1);
}

void OR_Rfm4463_RfRecvChek(void)
{
    OR_Rfm4463_RERCV_Reset();
    DelayUs(10);
    OR_Rfm4463_RERCV_Set();
}

int OR_Rfm4463_DevIsErr(void)
{
	return 0;
}
int DevNullFun(void)
{
	return 0;
}

const PhyDirverOptST PhyDivRfOptObj = 
{
    /*
    返回:		无特别说明,均返回:TRUE/FALSE
    */
    OR_Rfm4463_Init,
    OR_Rfm4463_Sleep,
    OR_Rfm4463_Rec_Init,
    /*
    此函数调用PhyApplyOptST.RecvData,将数据传递给应用.
    */
    OR_Rfm4463_RecData,
    OR_Rfm4463_Sleep,
    OR_Rfm4463_Send_Init,
    OR_Rfm4463_SendBit,
    OR_Rfm4463_Sleep,
    /*
    说明:		获取最近一次信号质量
    返回:		0-100类型信号状态,0表示无信号,100表示满信号
    */
    DevNullFun,
    OR_Rfm4463_DevIsErr,
    /*
    说明：     配置设备
    返回：     成功或者失败
    */
    OR_Rfm4463_SetConfigValue,
};
//查询RF状态
unsigned char GetRFState(void)
{
    if( TRUE == RFState )
        return 0;//正常
    else
        return 1;//异常
}
