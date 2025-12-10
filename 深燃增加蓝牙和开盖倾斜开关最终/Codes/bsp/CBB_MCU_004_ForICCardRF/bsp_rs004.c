#include    "bsp_rs004.h"

/* Set option bytes */
#pragma location = "OPTBYTE"
__root const uint8_t opbyte0 = 0xEFU;
#pragma location = "OPTBYTE"
__root const uint8_t opbyte1 = 0x5FU;
#pragma location = "OPTBYTE"
__root const uint8_t opbyte2 = 0xEAU;
#pragma location = "OPTBYTE"
__root const uint8_t opbyte3 = 0x04U;

/* Set security ID */
#pragma location = "SECUID"
__root const uint8_t secuid[10] = 
    {0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U};

extern void IrqServerDefaultRS(unsigned char Data);
extern void IrqServerDefault(void);

void IM_m2m_Set_MotorSleep_GPIO(unsigned char fun)
{
    IO_M_SLEEP_DIR_OUT;
    if (fun == 1)
    {
        IO_M_SLEEP_1;
    }
    else
    {
        IO_M_SLEEP_0;
    }
}
void IM_m2m_Set_MotorOpen_GPIO(unsigned char fun)
{
    IO_M_CTRL2_DIR_OUT;
    if (fun == 1)
    {
        IO_M_CTRL2_1;
    }
    else
    {
        IO_M_CTRL2_0;
    }
}
void IM_m2m_Set_MotorClose_GPIO(unsigned char fun)
{
    IO_M_CTRL1_DIR_OUT;
    if (fun == 1)
    {
        IO_M_CTRL1_1;
    }
    else
    {
        IO_M_CTRL1_0;
    }
}

void BspDoorClose(void)
{
    IO_M_SLEEP_DIR_OUT;
    IO_M_CTRL1_DIR_OUT;
    IO_M_CTRL2_DIR_OUT;
    IO_M_SLEEP_1;
    IO_M_CTRL1_0;
    IO_M_CTRL2_1;
}

void BspDoorOpen(void)
{
    IO_M_SLEEP_DIR_OUT;
    IO_M_CTRL1_DIR_OUT;
    IO_M_CTRL2_DIR_OUT;
    IO_M_SLEEP_1;
    IO_M_CTRL1_1;
    IO_M_CTRL2_0;
}

void BspDoorStop(void)
{
    IO_M_SLEEP_DIR_OUT;
    IO_M_CTRL1_DIR_OUT;
    IO_M_CTRL2_DIR_OUT;
    IO_M_CTRL1_0;
    IO_M_CTRL2_0;
    IO_M_SLEEP_0;
}

void BspBeepOn(void)
{
    IO_BEEP_DIR_OUT;
    IO_BEEP_1;
}
void BspBeepOff(void)
{
    IO_BEEP_DIR_OUT;
    IO_BEEP_O;
}
//------------------------------------------------------------------------
//------------------------------------------------------------------------
//邹航邹航邹航邹航邹航邹航邹航邹航邹航邹航邹航邹航邹航邹航邹航邹航邹航邹航
//------------------------------------------------------------------------
//------------------------------------------------------------------------
//------------------------------------
//液晶液晶液晶液晶液晶液晶液晶液晶液晶
//------------------------------------
//开启液晶显示
void LcdOn(void)
{
    MCU_LcdOpen();
}
//关闭液晶显示
void LcdOff(void)
{
    MCU_LcdClose();
}
//想底层液晶显示寄存器中写入数值
//x：寄存器地址
//reg：寄存器值
void LcdWriteReg(unsigned char x, unsigned char reg)
{
    MCU_LcdWriteReg(x, reg);
}
//读取寄存器地址中的值
unsigned char LcdReadReg(unsigned char x)
{
    return MCU_LcdReadReg(x);
}
//液晶底层初始化
void LcdInit(void)
{
    {
        /* Set segment pins */
        PU0 &= 0x0EU;
        PU1 &= 0x03U;
        PU5 &= 0x0CU;
        POM0 &= 0x0EU;
        POM1 &= 0x3FU;
        POM5 &= 0x8FU;
        PIM0 &= 0x9FU;
        PIM1 &= 0x3FU;
        PIM5 &= 0xDFU;
        PFSEG0 |= 0x30U;
        PFSEG1 |= 0x0FU;
        PFSEG4 |= 0xC0U;
        PFSEG5 |= 0x1FU;
        PFSEG6 |= 0x0FU;
        P0 &= 0x0EU;
        PM0 &= 0x0EU;
        PMC1 &= 0xF3U;
        P1 &= 0x03U;
        PM1 &= 0x03U;
        P5 &= 0x0CU;
        PM5 &= 0x0CU;
    }
    MCU_LcdCfg();
}
//------------------------------------
//霍尔霍尔霍尔霍尔霍尔霍尔霍尔霍尔霍尔
//------------------------------------
//获取霍尔io口状态

unsigned char GpioHallARead(void)
{
    return IO_HALL1_READ;
}
unsigned char GpioHallBRead(void)
{
    return IO_HALL2_READ;
}
//改变中断触发边沿
//0：上升沿；1：下降沿
void HallASetInt(unsigned char c,void(*p)(void))
{
    if(c == 0)
	    INT2Config(RISING_EDGE_INT,p);
	else
	    INT2Config(FALLING_EDGE_INT,p);
	EnableINT2;
}
void HallBSetInt(unsigned char c,void(*p)(void))
{
    if(c == 0)
        INT1Config(RISING_EDGE_INT,p);
    else
	    INT1Config(FALLING_EDGE_INT,p);
	EnableINT1;
}
//------------------------------------------------------------------------
//------------------------------------------------------------------------
//胡鑫鑫胡鑫鑫胡鑫鑫胡鑫鑫胡鑫鑫胡鑫鑫胡鑫鑫胡鑫鑫胡鑫鑫胡鑫鑫胡鑫鑫胡鑫鑫
//------------------------------------------------------------------------
//------------------------------------------------------------------------
//------------------------------------
//CPU卡CPU卡CPU卡CPU卡CPU卡CPU卡CPU卡
//------------------------------------
/*
*/
/*void CardSdaInput(void);        //SDA脚，IO口设为输入
void CardSdaOutput(void);       //SDA脚，IO口设为输出
void CardSdaSet(void);          //SDA脚，IO口输出高电平
void CardSdaReset(void);        //SDA脚，IO口输出低电平
unsigned char CardSdaRead(void);//SDA脚，读IO口电平
void CardRstSet(void);          //RST脚，IO口输出高电平
void CardRstReset(void);        //RST脚，IO口输出低电平 */
//CLK脚上电设为输出，要求输出频率在500HZ以上 
//NOP;

//------------------------------------
//1608卡1608卡1608卡1608卡1608卡1608卡
//------------------------------------
/*
*/
//插卡中断 0：上升沿；1：下降沿
void CardIntConfig(unsigned char c,void(*p)(void))
{
    if(c == 0)
	    INT7Config(RISING_EDGE_INT,p);
	else
	    INT7Config(FALLING_EDGE_INT,p);
    EnableINT7;
}
unsigned char CardBslRead(void)
{
    if(IO_CARD_BSL_READ)
	    return 1;
	else
	    return 0;
}

void CardSclOutput(void)
{
    IO_IIC_CARD_SCL_DIR_OUT;
}
void CardSclSet(void)
{
    IO_IIC_CARD_SCL_1;
}
void CardSclClr(void)
{
    IO_IIC_CARD_SCL_0;
}
void CardRstOutput(void)
{
    IO_IIC_CARD_RST_DIR_OUT;
}
void CardRstSet(void)
{
    IO_IIC_CARD_RST_1;
}
void CardRstClr(void)
{
    IO_IIC_CARD_RST_0;
}
void CardSdaSet(void)
{
    IO_IIC_CARD_SDA_1;
}
void CardSdaClr(void)
{
    IO_IIC_CARD_SDA_0;
}
void CardSdaOutput(void)
{
    IO_IIC_CARD_SDA_DIR_OUT;
}
void CardSdaInput(void)
{
    IO_IIC_CARD_SDA_DIR_IN;
}
unsigned char CardSdaRead(void)
{
    if(IO_IIC_CARD_SDA_READ)
        return 1;
    else
        return 0;
}
void CardPowerCheckInput(void)
{
    IO_CARD_POWCHECK_DIR_IN;
}
unsigned char CardPowerCheckRead(void)
{
    if(IO_CARD_POWCHECK_READ)
	    return 1;
	else
	    return 0;
}
void CardPowerContrlOutput(void)
{
    IO_CARD_POW_CON_DIR_OUT;
}
void CardPowerContrlSet(void)
{
    IO_CARD_POW_CON_1;
}
void CardPowerContrlClr(void)
{
    IO_CARD_POW_CON_0;
}

//------------------------------------------------------------------------
//------------------------------------------------------------------------
//林扬宣林扬宣林扬宣林扬宣林扬宣林扬宣林扬宣林扬宣林扬宣林扬宣林扬宣林扬宣
//------------------------------------------------------------------------
//------------------------------------------------------------------------
//------------------------------------
//nand flash nand flash nand flash
//------------------------------------
//输出高/低
#define FLASH_CS(a)   			
//输出高/低
#define FLASH_WP(a)   			
//输出高/低
#define FLASH_MOSI(a)  		 	
//输出高/低
#define FLASH_CLK(a)   			
//输出高/低
#define FLASH_POWER(a)   		
//输入
#define FLASH_MISO()			

//------------------------------------------------------------------------
//------------------------------------------------------------------------
//朱毓烜朱毓烜朱毓烜朱毓烜朱毓烜朱毓烜朱毓烜朱毓烜朱毓烜朱毓烜朱毓烜朱毓烜
//------------------------------------------------------------------------
//------------------------------------------------------------------------
//------------------------------------
//ADCADCADCADCADCADCADCADCADCADCADCADC
//------------------------------------
void IO_VBB_CHK_EN_DIR_OUT(void)
{
    PORT2OutputConfig(BIT4);
    PMC2ClearBit(BIT4);
    PFSEG4ClearBit(BIT0);
}
void IO_VBB_CHK_EN_1(void)
{
    PORT2SetBit(BIT4);
}
void IO_VBB_CHK_EN_0(void)
{
    PORT2ClearBit(BIT4);
}
/**
* @declare ADC初始化
*
* @author Administrator (2014-11-03)
*
* @requirements
*
* @param ADC_InitStruct ADC配置信息
*/
void OA_ADC_Init(ADC_InitTypeDef* ADC_InitStruct)
{
    ADCModeST ADCMode;
    ADCMode.ADCModeOpt = ADC_InitStruct->ADCModeConversionMode;
    if (ADC_InitStruct->ADCModeConversionBit == 8)
        ADCMode.ADCModeResolution = 0;
    else
        ADCMode.ADCModeResolution = 1;

    if (ADC_InitStruct->ADCModeChannel == 1)
    {
        MCU_ADCCfg(PortVmNum17, ADCMode);//阀门
    }
    else
    {
        MCU_ADCCfg(PortVmNum16, ADCMode);//系统
    }
}
/**
* @declare 使能或者失能指定的ADC
*
* @author Administrator (2014-11-03)
*
* @requirements
*
* @param NewState 外设ADCx的新状态
*/
void OA_ADC_CMD(FunctionalState NewState)
{
    if (NewState == DISABLE)
        MCU_ADCStop(PortVmNum0);
    else
        MCU_ADCStart(PortVmNum0);
}
/**
* @declare 返回最近一次ADCx的转换结果
*
* @author Administrator (2014-11-03)
*
* @requirements
*
* @return unsigned short 16位模式ADC返回结构
*/
unsigned short OA_ADC_GetConversionValue(void)
{
    return MCU_ADCRead();
}
/**
* @declare 获取ADC转换状态
*
* @author Administrator (2014-11-03)
*
* @requirements
*
*
* @return unsigned char 转换完成返回0;正在转换中返回1
*/
unsigned char OA_ADC_GetConversionState(void)
{
    return (unsigned char)ADCS;
}
/**
* @declare 设置ADC转换通道
*
* @author ZYX (2014/11/6)
*
* requirements
*
* @param channel ADC转换通道:0
*/
typedef enum
{
    ADCCHANNELVOLT = 0,
    ADCCHANNELCUR,
}ADCChannelET;

void OA_ADC_SetConversionChannel(unsigned char channel)
{
    if (channel == 1)
    {
        ADS = 0x11;//(PortVmNum17, ADCMode);//阀门
    }
    else
    {
        ADS = 0x10;//(PortVmNum16, ADCMode);//系统
    }
}
//------------------------------------
//amt4463amt4463amt4463amt4463amt4463
//------------------------------------
/**
* @declare 空操作函数
*
* @author ZYX (2014/11/5)
*
* requirements
*/
//void NOP(void);

/**
* @declare Rfm4463无线模块射频输入输出脚方向控制
*
* @author Administrator (2014-10-31)
*
* @requirements GPIO
*
* @param dir 0：输入 1：输出
*/
void OR_Rfm4463_RFDIO_DIR_Ctrl(unsigned char dir)
{
    if(dir == 1)
	{
	    IO_RF_RFDIO_DIR_OUT;
	}
	else
	{
	    IO_RF_RFDIO_DIR_IN;
	}
}

/**
* @declare Rfm4463无线模块射频输入输出脚输出高电平
*
* @author Administrator (2014-10-30)
*
* @requirements GPIO
*/

void OR_Rfm4463_RFDIO_Set(void)
{
    IO_RF_RFDIO_1;
}

/**
* @declare Rfm4463无线模块射频输入输出脚输出低电平
*
* @requirements GPIO
*
* @author Administrator (2014-10-30)
*/
void OR_Rfm4463_RFDIO_Reset(void)
{
    IO_RF_RFDIO_0;
}

/**
* @declare Rfm4463无线模块应答确认脚方向控制
*
* @author Administrator (2014-10-30)
*
* @requirements GPIO
*
* @param dir 0：输入 1：输出
*/
void OR_Rfm4463_ACK_DIR_Ctrl(unsigned char dir)
{
    if(dir == 1)
	{
	    IO_RF_ACK_DIR_OUT;
	}
	else
	{
	    IO_RF_ACK_DIR_IN;
	}
}

/**
* @declare 读取Rfm4463无线模块应答确认脚返回值
*
* @author Administrator (2014-10-30)
*
* @requirements GPIO
*
* @return unsigned char ：高电平返回1，低电平返回0
*/
unsigned char OR_Rfm4463_ACK_Read(void)
{
	if(IO_RF_ACK_READ)
	    return 1;
	else
	    return 0;
}

/**
* @declare Rfm4463无线模块接收复位脚方向控制
*
* @author Administrator (2014-10-30)
*
* @requirements GPIO
*
* @param dir 0：输入 1：输出
*/
void OR_Rfm4463_RERCV_Dir_Ctrl(unsigned char dir)
{
    if(dir == 1)
	{
	    IO_RF_RERCV_DIR_OUT;
	}
	else
	{
	    IO_RF_RERCV_DIR_IN;
	}
}

/**
* @declare Rfm4463无线模块接收复位脚输出高电平
*
* @author Administrator (2014-10-30)
*
* @requirements GPIO
*/
void OR_Rfm4463_RERCV_Set(void)
{
    IO_RF_RERCV_1;
}

/**
* @declare Rfm4463无线模块接收复位脚输出低电平
*
* @author Administrator (2014-10-30)
*
* @requirements GPIO
*/
void OR_Rfm4463_RERCV_Reset(void)
{
    IO_RF_RERCV_0;
}

/**
* @declare Rfm4463无线模块SPI输入脚方向控制
*
* @author Administrator (2014-10-30)
*
* @requirements GPIO
*
* @param dir 0：输入 1：输出
*/
void OR_Rfm4463_SDI_DIR_Ctrl(unsigned char dir)
{
    if(dir == 1)
	{
	    IO_RF_SDI_DIR_OUT;
	}
	else
	{
	    IO_RF_SDI_DIR_IN;
	}
}

/**
* @declare 读取Rfm4463无线模块SPI输入脚返回值
*
* @author Administrator (2014-10-30)
*
* @requirements GPIO
*
* @return unsigned char 高电平返回1，低电平返回0
*/
unsigned char OR_Rfm4463_SDI_Read(void)
{
	if(IO_RF_SDI_READ)
	    return 1;
	else
	    return 0;
}

/**
* @declare Rfm4463无线模块RF收发同步时钟脚方向控制
*
* @author Administrator (2014-10-30)
*
* @requirements GPIO
*
* @param dir 0：输入 1：输出
*/
void OR_Rfm4463_RFCLK_DIR_Ctrl(unsigned char dir)
{
    if(dir == 1)
	{
	    IO_RF_RFCLK_DIR_OUT;
	}
	else
	{
	    IO_RF_RFCLK_DIR_IN;
	}
}

/**
* @declare 读取Rfm4463无线模块RF收发同步时钟脚返回值
*
* @author Administrator (2014-10-30)
*
* @requirements GPIO
*
* @return unsigned char 高电平返回1，低电平返回0
*/
unsigned char OR_Rfm4463_RFCLK_Read(void)
{
	if(IO_RF_RFCLK_READ)
	    return 1;
	else
	    return 0;
}

/**
* @declare Rfm4463无线模块SPI输出脚方向控制
*
* @author Administrator (2014-10-30)
*
* @requirements GPIO
*
* @param dir 0：输入 1：输出
*/
void OR_Rfm4463_SDO_DIR_Ctrl(unsigned char dir)
{
    if(dir == 1)
	{
	    IO_RF_SDO_DIR_OUT;
	}
	else
	{
	    IO_RF_SDO_DIR_IN;
	}
}

/**
* @declare Rfm4463无线模块SPI输出脚输出高电平
*
* @author Administrator (2014-10-30)
*
* @requirements GPIO
*/
void OR_Rfm4463_SDO_Set(void)
{
    IO_RF_SDO_1;
}

/**
* @declare Rfm4463无线模块SPI输出脚输出低电平
*
* @author Administrator (2014-10-30)
*
* @requirements GPIO
*/
void OR_Rfm4463_SDO_Reset(void)
{
    IO_RF_SDO_0;
}

/**
* @declare Rfm4463无线模块复位脚方向控制
*
* @author Administrator (2014-10-30)
*
* @requirements GPIO
*
* @param dir 0：输入 1：输出
*/
void OR_Rfm4463_RST_DIR_Ctrl(unsigned char dir)
{
    if(dir == 1)
	{
	    IO_RF_RST_DIR_OUT;
	}
	else
	{
	    IO_RF_RST_DIR_IN;
	}
}

/**
* @declare Rfm4463无线模块复位脚输出高电平
*
* @author Administrator (2014-10-30)
*
* @requirements GPIO
*/
void OR_Rfm4463_RST_Set(void)
{
    IO_RF_RST_1;
}

/**
* @declare Rfm4463无线模块复位脚输出低电平
*
* @author Administrator (2014-10-30)
*
* @requirements GPIO
*/
void OR_Rfm4463_RST_Reset(void)
{
    IO_RF_RST_0;
}

/**
* @declare Rfm4463无线模块SPI时钟脚方向控制
*
* @author Administrator (2014-10-30)
*
* @requirements GPIO
*
* @param dir 0：输入 1：输出
*/
void OR_Rfm4463_SCK_DIR_Ctrl(unsigned char dir)
{
    if(dir == 1)
	{
	    IO_RF_SCK_DIR_OUT;
	}
	else
	{
	    IO_RF_SCK_DIR_IN;
	}
}

/**
* @declare Rfm4463无线模块SPI时钟脚输出高电平
*
* @author Administrator (2014-10-30)
*
* @requirements GPIO
*/
void OR_Rfm4463_SCK_Set(void)
{
    IO_RF_SCK_1;
}

/**
* @declare Rfm4463无线模块SPI时钟脚输出低电平
*
* @author Administrator (2014-10-30)
*
* @requirements GPIO
*/
void OR_Rfm4463_SCK_Reset(void)
{
    IO_RF_SCK_0;
}

/**
* @declare Rfm4463无线模块SPI使能脚方向控制
*
* @author Administrator (2014-10-30)
*
* @requirements GPIO
*
* @param dir 0：输入 1：输出
*/
void OR_Rfm4463_NSS_DIR_Ctrl(unsigned char dir)
{
    if(dir == 1)
	{
	    IO_RF_NSS_DIR_OUT;
	}
	else
	{
	    IO_RF_NSS_DIR_IN;
	}
}

/**
* @declare Rfm4463无线模块SPI使能脚输出高电平
*
* @author Administrator (2014-10-30)
*
* @requirements GPIO
*/
void OR_Rfm4463_NSS_Set(void)
{
    IO_RF_NSS_1;
}

/**
* @declare Rfm4463无线模块SPI使能脚输出低电平
*
* @author Administrator (2014-10-30)
*
* @requirements GPIO
*/
void OR_Rfm4463_NSS_Reset(void)
{
    IO_RF_NSS_0;
}

/**
* @declare Rfm4463无线模块射频发射使能脚方向控制
*
* @author Administrator (2014-10-30)
*
* @requirements GPIO
*
* @param dir 0：输入 1：输出
*/
void OR_Rfm4463_RFTX_DIR_Ctrl(unsigned char dir)
{
    if(dir == 1)
	{
	    IO_RF_RFTX_DIR_OUT;
	}
	else
	{
	    IO_RF_RFTX_DIR_IN;
	}
}

/**
* @declare Rfm4463无线模块射频发射使能脚输出高电平
*
* @author Administrator (2014-10-30)
*
* @requirements GPIO
*/
void OR_Rfm4463_RFTX_Set(void)
{
    IO_RF_RFTX_1;
}

/**
* @declare Rfm4463无线模块射频发射使能脚输出低电平
*
* @author Administrator (2014-10-30)
*
* @requirements GPIO
*/
void OR_Rfm4463_RFTX_Reset(void)
{
    IO_RF_RFTX_0;
}

/**
* @declare Rfm4463无线模块射频接收使能脚方向控制
*
* @author Administrator (2014-10-30)
*
* @requirements GPIO
*
* @param dir 0：输入 1：输出
*/
void OR_Rfm4463_RFRX_DIR_Ctrl(unsigned char dir)
{
    if(dir == 1)
	{
	    IO_RF_RFRX_DIR_OUT;
	}
	else
	{
	    IO_RF_RFRX_DIR_IN;
	}
}

/**
* @declare Rfm4463无线模块射频接收使能脚输出高电平
*
* @author Administrator (2014-10-30)
*
* @requirements GPIO
*/
void OR_Rfm4463_RFRX_Set(void)
{
    IO_RF_RFRX_1;
}

/**
* @declare Rfm4463无线模块射频接收使能脚输出低电平
*
* @author Administrator (2014-10-30)
*
* @requirements GPIO
*/
void OR_Rfm4463_RFRX_Reset(void)
{
    IO_RF_RFRX_0;
}

/**
* @declare Rfm4463无线模块门限输出脚方向控制
*
* @author Administrator (2014-10-30)
*
* @requirements GPIO
*
* @param dir 0：输入 1：输出
*/
void OR_Rfm4463_RSSI_DIR_Ctrl(unsigned char dir)
{
    if(dir == 1)
	{
	    IO_RF_RSSI_DIR_OUT;
	}
	else
	{
	    IO_RF_RSSI_DIR_IN;
	}
}

/**
* @declare 读取Rfm4463无线模块门限输出脚返回值
*
* @author Administrator (2014-10-30)
*
* @requirements GPIO
*
* @return unsigned char 高电平返回1，低电平返回0
*/
unsigned char OR_Rfm4463_RSSI_Read(void)
{
	if(IO_RF_RSSI_READ)
	    return 1;
	else
	    return 0;
}

/**
* @declare Rfm4463射频接收UART模块初始化
*
* @author ZYX (2014/11/6)
*
* requirements
*
* @param UartMode_ST UART模块配置参数缓冲区指针
* @param fun 串口接收中断回调函数
*/

void OR_Rfm4463_Uart_Init(UartMode* UartMode_ST, void(*fun)(unsigned char))
{
    Uart3Config(UartMode_ST,IrqServerDefaultRS,fun);
}


/**
* @declare 使能或者失能Rfm4463射频接收UART模块
*
* @author ZYX (2014/11/5)
*
* requirements 单片机UART接收模块
*
* @param NewState
*  			   外设UART的新状态，这个参数可以取：ENABLE或者DISABLE
*/
void OR_Rfm4463_Uart_Cmd(FunctionalState NewState)
{
    if(NewState == DISABLE)
	{
	    Uart3ReceiveDisable();
	}
	else
	{
	    Uart3ReceiveEnable();
	}
}

/**
* @declare 读取Rfm4463射频接收UART模块最近接收到的数据
*
* @author Administrator (2014-10-30)
*
* @requirements 单片机UART接收模块
*
* @return unsigned char
*         返回Rfm4463射频接收UART模块最近接收到的数据
*/
unsigned char OR_Rfm4463_Uart_ReceiveData(void);

//------------------------------------------------------------------------
//------------------------------------------------------------------------
//王兵王兵王兵王兵王兵王兵王兵王兵王兵王兵王兵王兵王兵王兵王兵王兵王兵王兵
//------------------------------------------------------------------------
//------------------------------------------------------------------------

/*===============================LED配置==============================*/
/**
* LED灯方向配置:红色灯
*    配置成输出
* @author wb (2014-10-31)
*/
void IM_m2m_RedLED_Dir_Config(void)
{

}
/**
* LED灯方向配置:绿色灯
*    配置成输出
* @author wb (2014-10-31)
*/
void IM_m2m_GreenLED_Dir_Config(void)
{

}
/**
* LED灯IO配置:红色
*
* @author wb (2014-11-06)
*
* @param fun :0-置低；1-置高
*/
void IM_m2m_Set_RedLED_GPIO(unsigned char fun)
{

}
/**
* LED灯IO配置:绿色
*
* @author wb (2014-11-06)
*
* @param fun :0-置低；1-置高
*/
void IM_m2m_Set_GreenLED_GPIO(unsigned char fun)
{

}
/*===============================KEY配置==============================*/
/**
* 获取编程按键的状态
*
* @author wb (2014-10-31)
*
* @return unsigned char
*  	   :返回0：表示按键按下；返回1：表示按键松开
*/
unsigned char IM_m2m_GetProgrameKey_Status(void)
{
    return 0;
}
/**
* 获取用户按键的状态
*
* @author wb (2014-10-31)
*
* @return unsigned char
*  	   :返回0：表示按键按下；返回1：表示按键松开
*/
unsigned char IM_m2m_GetUsersKey_Status(void)
{
    if(IO_KEY_READ)
	    return 1;
	else
	    return 0;
}
/**
* 打开外部中断
*
* @author wb (2014-11-10)
*
* @param key_type ：中断类型:0:用户按键  1：编程按键
*/
void IM_OpenExt_Int(unsigned char key_type)
{
    if(key_type == 0)
	    EnableINT5;
}
/**
* 关闭外部中断
*
* @author wb (2014-11-10)
*
* @param key_type ：中断类型
*/
void IM_CloseExt_Int(unsigned char key_type)
{
    if(key_type == 0)
	    DisableINT5;
}
/**
* 中断初始化:编程按键
*    配置成中断接口，，下降沿触发，不带上拉电阻；
* @author wb (2014-10-31)
*
* @param fun 中断服务函数指针：fun由驱动编写，由BSP调用
*/
void IM_m2m_ProgrameKey_Intp_Config(void(*fun))
{
}
/**
* 中断初始化：用户按键
*    配置成中断接口，，下降沿触发，不带上拉电阻；
* @author wb (2014-10-31)
*
* @param fun 中断服务函数指针：fun由驱动编写，由BSP调用
*/
void IM_m2m_UserKey_Intp_Init(void(*fun)(void))
{
    INT5Config(FALLING_EDGE_INT,fun);
}
/*==============================串口配置==============================*/
/**
*配置USART 发送IO:TX输出高
*
* @author wb (2014-11-06)
*
* @param com :串口端口:0-空;1-usb;2-gprs;3-光电
* @param fun :1-置高；0-置低
*/
void IM_m2m_Set_Usart_TX_GPIO(unsigned char com, unsigned char fun)
{

}
/**
*配置USART 接收IO:配置为输入
*
* @author wb (2014-11-06)
*
* @param com :串口端口:0-空;1-usb;2-gprs;3-光电
*/
void IM_m2m_Config_Usart_RX_DIR(unsigned char com)
{

}
/**
* 串口配置：UART
* 波特率1200,9600,115200；1位起始位，1位停止位，8位数据位，无校验
* 全部都使能发送中断和接收中断
* @author wb (2014-11-06)
*
* @param com :串口端口:0-空;1-usb;2-gprs;3-光电
* @param baud :波特率：1200；9600；115200
* @param fun1 ：串口发送中断服务函数
* @param fun2 ：串口接收中断服务函数
*/
void IM_m2m_Uart_Config(unsigned char com, unsigned long baud, void(*fun1), void(*fun2))
{

}
/**
* 串口配置：打开串口
*
* @author wb (2014-11-06)
*
* @param com ：串口端口:0-空;1-usb;2-gprs;3-光电
*/
void IM_m2m_Open_Usart(unsigned char com)
{

}
/**
* 串口配置：关闭串口
*
* @author wb (2014-11-06)
*
* @param com :串口端口:0-空;1-usb;2-gprs;3-光电
*/
void IM_m2m_Close_Usart(unsigned char com)
{

}
///*===============================gprs模块配置==============================*/
//
///**
// * GSM模块电源管脚
// * 
// * @author wb (2014-11-06)
// * 
// * @param fun :0-置低；1-置高
// */
//
void IM_m2m_Set_GSM_Modem_Power_GPIO(unsigned char fun)
{

}
// 
///**
// * GSM模块开关机管脚配置
// * 
// * @author wb (2014-11-06)
// * 
// * @param fun 
// */
//
void IM_m2m_Set_GSM_Modem_PowerOnOff_GPIO(unsigned char fun)
{

}
//
//------------------------------------------------------------------------
//------------------------------------------------------------------------
//115 115 115 115 115 115 115 115 115 115 115 115 115 115 115 115 115 115 
//------------------------------------------------------------------------
//------------------------------------------------------------------------
//prt:0表示EEPROM/RTC8025T模拟口;1表示ICCARD模拟口
//ICCARD
//PWR: P4.5; PFDEG = 0,
//SCL: P4.2
//SDA: P4.1
//EEPROM/RTC8025T:
//PWR: P1.1; SEG36; PFSEG4,BIT5//CLR表示上电
//SCL: P2.7; SEG34; PFSEG4,BIT3
//SDA: P1.0; SEG35; PFSEG4,BIT4
//I2C电源配置输出   
void IM_IICPWR_OUT(unsigned char prt) //prt,0:1号模拟口,1:2号模拟口
{
    if (prt)
    {
        PORT4OutputConfig(BIT5);
        CLR_BIT(PMC4, BIT5);
        CLR_BIT(PFSEG3, BIT2);
    }
    else
    {
        PORT1OutputConfig(BIT1);
        CLR_BIT(PMC1, BIT1);
        CLR_BIT(PFSEG4, BIT5);
    }
}	
//I2C电源配置上拉	
void IM_IICPWR_UP(unsigned char prt) //prt,0:1号模拟口,1:2号模拟口	
{
    if (prt)
    {
        //PORT4PullUpBit(BIT5);
    }
    else
    {
        //PORT1PullUpBit(BIT1);
    }
}
//I2C电源配置下拉	
void IM_IICPWR_DOWN(unsigned char prt) //prt,0:1号模拟口,1:2号模拟口	
{
    if (prt)
    {
    }
    else
    {
    }
}
//I2C电源输出高	
void IM_IICPWR_ON(unsigned char prt) //prt,0:1号模拟口,1:2号模拟口
{
    if (prt)
    {
        PORT4ClearBit(BIT5);
    }
    else
    {
        PORT1ClearBit(BIT1);
    }
}	
//I2C电源输出低	
void IM_IICPWR_OFF(unsigned char prt) //prt,0:1号模拟口,1:2号模拟口
{
    if (prt)
    {
        PORT4SetBit(BIT5);
    }
    else
    {
        PORT1SetBit(BIT1);
    }
}	
//prt,0:1号模拟口,1:2号模拟口
void IM_IICSCL_OUT(unsigned char prt)
{
    if (prt)
    {
        PORT4OutputConfig(BIT2);
        CLR_BIT(POM4, BIT2);
        CLR_BIT(PMC4, BIT2);
    }
    else
    {
        PORT2OutputConfig(BIT7);
        CLR_BIT(PMC2, BIT7);
        CLR_BIT(PFSEG4, BIT3);
    }
}
//prt,0:1号模拟口,1:2号模拟口
void IM_IICSCL_IN(unsigned char prt)
{
    if (prt)
    {
        PORT4InputConfig(BIT2);
        CLR_BIT(PMC4, BIT2);
    }
    else
    {
        PORT2InputConfig(BIT7);
        CLR_BIT(PMC2, BIT7);
        CLR_BIT(PFSEG4, BIT3);
    }
}
//prt,0:1号模拟口,1:2号模拟口
void IM_IICSCL_UP(unsigned char prt)
{
    if (prt)
    {
        //PORT4PullUpBit(BIT2);
    }
    else
    {
        //PORT2PullUpBit(BIT7);
    }
}
// prt,0:1号模拟口,1:2号模拟口
void IM_IICSCL_DOWN(unsigned char prt)
{
    if (prt)
    {
    }
    else
    {
    }
}
// prt,0:1号模拟口,1:2号模拟口
void IM_IICSCL_SET(unsigned char prt)
{
    if (prt)
    {
        PORT4SetBit(BIT2);
    }
    else
    {
        PORT2SetBit(BIT7);
    }
}
// prt,0:1号模拟口,1:2号模拟口
void IM_IICSCL_CLR(unsigned char prt)
{
    if (prt)
    {
        PORT4ClearBit(BIT2);
    }
    else
    {
        PORT2ClearBit(BIT7);
    }
}
// prt,0:1号模拟口,1:2号模拟口
void IM_IICSDA_OUT(unsigned char prt)
{
    if (prt)
    {
        PORT4OutputConfig(BIT1);
    }
    else
    {
        PORT1OutputConfig(BIT0);
        CLR_BIT(PMC1, BIT0);
        CLR_BIT(PFSEG4, BIT4);
    }
}
//prt,0:1号模拟口,1:2号模拟口
void IM_IICSDA_IN(unsigned char prt)
{
    if (prt)
    {
        PORT4InputConfig(BIT1);
    }
    else
    {
        PORT1InputConfig(BIT0);
        CLR_BIT(PMC1, BIT0);
        CLR_BIT(PFSEG4, BIT4);
    }
}
//prt,0:1号模拟口,1:2号模拟口
void IM_IICSDA_UP(unsigned char prt)
{
    if (prt)
    {
        //PORT4PullUpBit(BIT1);
    }
    else
    {
        //PORT1PullUpBit(BIT0);
    }
}
// prt,0:1号模拟口,1:2号模拟口
void IM_IICSDA_DOWN(unsigned char prt)
{
    if (prt)
    {
    }
    else
    {
    }
}
// prt,0:1号模拟口,1:2号模拟口
void IM_IICSDA_SET(unsigned char prt)
{
    if (prt)
    {
        PORT4SetBit(BIT1);
    }
    else
    {
        PORT1SetBit(BIT0);
    }
}
// prt,0:1号模拟口,1:2号模拟口
void IM_IICSDA_CLR(unsigned char prt)
{
    if (prt)
    {
        PORT4ClearBit(BIT1);
    }
    else
    {
        PORT1ClearBit(BIT0);
    }
}
// prt,0:1号模拟口,1:2号模拟口
unsigned char IM_IICSDA_STU(unsigned char prt)
{
    if (prt)
    {
        return P4&BIT1;
    }
    else
    {
        return P1&BIT0;
    }
}
// prt,0:1号模拟口,1:2号模拟口
//返回值，0:低，非0:高

//------------------------------------------------------------------------
//------------------------------------------------------------------------
//谢尚川谢尚川谢尚川谢尚川谢尚川谢尚川谢尚川谢尚川谢尚川谢尚川谢尚川谢尚川
//------------------------------------------------------------------------
//------------------------------------------------------------------------
typedef enum
{
    Dis = 0,
    En,
}EnDis;

typedef enum
{
    IODIR_Input = 0,    //输入
    IODIR_Output,       //输出
}IODIR;

typedef enum
{
    IOPP_PD = 0,        //下拉
    IOPP_PP,            //上拉
    IOPP_FL,            //悬空
}IOPP;

typedef enum
{
    IOVAL_L = 0,        //低电平
    IOVAL_H,            //高电平
    IOVAL_S,            //切换电平
}IOVAL;
/**
* 电源引脚初始化
*
* @author xsc (2014-11-09)
*
* @param dir 方向
* @param pp 上拉
*/
void ZD_POW_Init(IODIR dir, IOPP pp)
{
    if(dir == IODIR_Output)
	{
	    IO_GD_EN_DIR_OUT;
	}
	else
	{
	    IO_GD_EN_DIR_IN;
	}
	if(pp == IOPP_PP)
	{
	    IO_GD_EN_PullUp;
	}
}

/**
* 电源开关控制
*
* @author xsc (2014-11-09)
*
* @param c 控制参数En/Dis
*/
void ZD_POW_Ctrl(EnDis c)
{
    if (c == En)
    {
        IO_GD_EN_1;
    }
    else
    {
        IO_GD_EN_0;
    }
}

/**
* 硬件串口初始化
*
* @author xsc (2014-11-09)
*
* @param mode
* @param fun
*/
void Uart_Init(pUartMode mode, void(*fun)(unsigned char))
{
    Uart2Config(mode,IrqServerDefaultRS,fun);
}
/*
* 说明：串口插拔中断初始化函数,初始化完成使能中断
* fun:中断回调函数，包括插入中断和拔出中断
*/
//
void ParaSet_Interrupt_Init(void(*fun)(void))
{

}
/*
* 说明：关闭串口插拔中断功能，并恢复引脚到低功耗状态
*/
void ParaSet_Interrupt_Disable(void)
{

}
/*
* 说明：获取插入检测脚电平，低电平返回0，高电平返回1
*/
unsigned char GetParaSetInputRead(void)
{

}
//------------------------------------------------------------------------
//------------------------------------------------------------------------
//林扬宣林扬宣林扬宣林扬宣林扬宣林扬宣林扬宣林扬宣林扬宣林扬宣林扬宣林扬宣
//------------------------------------------------------------------------
//------------------------------------------------------------------------
//typedef void (*ISR_FUNC_PTR)(void);
//type: 0 主时钟
//type: 1 低频时钟
//len: 0 1ms
//len: 1 100ms
//len: 2 1000ms
static char IntTimeLen = 0;
void User_TimerCfg(unsigned char type, unsigned char len, IRQServerFT CallBack)
{
    if (type == 0)
        MCU_TimerCfg(PortVmNum0, TimeSpMs1, CallBack);
    else
    {
        IntTimeLen = len;
        if (len == 1)
            MCU_TimerCfg(PortVmNum8, TimeSpMs100, CallBack);
        else
            MCU_TimerCfg(PortVmNum9, TimeSpMs1000, CallBack);//MCU_TimerCfg(PortVmNum8, TimeSpMs100, CallBack);
    }
}
//type: 0 主时钟
//type: 1 低频时钟
void User_TimerStart(unsigned char type)
{
    if (type == 0)
        MCU_TimerStart(PortVmNum0);
    else
    {
        if (IntTimeLen == 1)
            MCU_TimerStart(PortVmNum8);
        else
            MCU_TimerStart(PortVmNum9);
    }
}
//type: 0 主时钟
//type: 1 低频时钟
void User_TimerStop(unsigned char type)
{
    if (type == 0)
        MCU_TimerStop(PortVmNum0);
    else
    {
        if (IntTimeLen == 1)
            MCU_TimerStop(PortVmNum8);
        else
            MCU_TimerStop(PortVmNum9);
    }
}