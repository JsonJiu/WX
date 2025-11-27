#include    "bsp_rs019.h"

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
}
void IM_m2m_Set_MotorOpen_GPIO(unsigned char fun)
{
}
void IM_m2m_Set_MotorClose_GPIO(unsigned char fun)
{
}

void BspDoorClose(void)
{
    IO_M_SLEEP_1;
    IO_M_CTRL1_0;
    IO_M_CTRL2_1;
}

void BspDoorOpen(void)
{
    IO_M_SLEEP_1;
    IO_M_CTRL1_1;
    IO_M_CTRL2_0;
}

void BspDoorStop(void)
{
    IO_M_CTRL1_0;
    IO_M_CTRL2_0;
    IO_M_SLEEP_0;
}

void BspBeepOn(void)
{
    IO_BEEP_1;
}
void BspBeepOff(void)
{
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
    {   //根据硬件将相应端口配置为液晶口
        PU1 &= 0xFEU;
        PU2 &= 0x3BU;
        PU3 &= 0xF0U;
        PU5 &= 0x63U;
        PU7 &= 0x0EU;
        POM5 &= 0xE7U;
        PIM5 &= 0xF7U;
        PFSEG0 |= 0xC0U;
        PFSEG1 |= 0x19U;
        PFSEG2 |= 0xFFU;
        PFSEG3 |= 0x40U;
        PFSEG4 |= 0x1CU;
        PMC1 &= 0xFEU;
        P1 &= 0xFEU;
        PM1 &= 0xFEU;
        PMC2 &= 0x3BU;
        P2 &= 0x3BU;
        PM2 &= 0x3BU;
        P3 &= 0xF0U;
        PM3 &= 0xF0U;
        P5 &= 0x63U;
        PM5 &= 0x63U;
        P7 &= 0x0EU;
        PM7 &= 0x0EU;
    }
    MCU_LcdCfg();
}
//------------------------------------
//霍尔霍尔霍尔霍尔霍尔霍尔霍尔霍尔霍尔
//------------------------------------
//中断回调函数
void HallAInt(void);
void HallBInt(void);
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
	    INT7Config(RISING_EDGE_INT,p);
	else
	    INT7Config(FALLING_EDGE_INT,p);
}
void HallBSetInt(unsigned char c,void(*p)(void))
{
    if(c == 0)
        INT5Config(RISING_EDGE_INT,p);
    else
	    INT5Config(FALLING_EDGE_INT,p);
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
	    INT0Config(RISING_EDGE_INT,p);
	else
	    INT0Config(FALLING_EDGE_INT,p);
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
//朱毓烜朱毓烜朱毓烜朱毓烜朱毓烜朱毓烜朱毓烜朱毓烜朱毓烜朱毓烜朱毓烜朱毓烜
//------------------------------------------------------------------------
//------------------------------------------------------------------------
//------------------------------------
//ADCADCADCADCADCADCADCADCADCADCADCADC
//------------------------------------
void IO_VBB_CHK_EN_DIR_OUT(void)
{
    PORT4OutputConfig(BIT5); 
    PMC4ClearBit(BIT5);
}
void IO_VBB_CHK_EN_1(void)
{
    PORT4SetBit(BIT5);
}
void IO_VBB_CHK_EN_0(void)
{
    PORT4ClearBit(BIT5);
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
        MCU_ADCCfg(PortVmNum0, ADCMode);//阀门ANI0
    }
    else
    {
        MCU_ADCCfg(PortVmNum1, ADCMode);//系统ANI1
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
        ADS = 0x00;//(PortVmNum0, ADCMode);//阀门
    }
    else
    {
        ADS = 0x01;//(PortVmNum1, ADCMode);//系统
    }
}

//------------------------------------------------------------------------
//------------------------------------------------------------------------
//王兵王兵王兵王兵王兵王兵王兵王兵王兵王兵王兵王兵王兵王兵王兵王兵王兵王兵
//------------------------------------------------------------------------
//------------------------------------------------------------------------


//
//------------------------------------------------------------------------
//------------------------------------------------------------------------
//115 115 115 115 115 115 115 115 115 115 115 115 115 115 115 115 115 115 
//------------------------------------------------------------------------
//------------------------------------------------------------------------
//PWR: P0.5; SEG48; PFSEG6,BIT1 //CLR表示上电
//SCL: P0.4; SEG47; PFSEG6,BIT0
//SDA: P0.3; SEG46; PFSEG5,BIT7
//I2C电源配置输出	
void IM_IICPWR_OUT(unsigned char prt) //prt,0:1号模拟口,1:2号模拟口
{
    CLR_BIT(PM0, BIT5);
    CLR_BIT(POM0, BIT5);
    CLR_BIT(PFSEG6, BIT1);
}	
//I2C电源配置上拉	
void IM_IICPWR_UP(unsigned char prt) //prt,0:1号模拟口,1:2号模拟口	
{
}
//I2C电源配置下拉	
void IM_IICPWR_DOWN(unsigned char prt) //prt,0:1号模拟口,1:2号模拟口	
{

}
//I2C电源输出高	
void IM_IICPWR_ON(unsigned char prt) //prt,0:1号模拟口,1:2号模拟口
{
    CLR_BIT(P0, BIT5);
}	
//I2C电源输出低	
void IM_IICPWR_OFF(unsigned char prt) //prt,0:1号模拟口,1:2号模拟口
{
    SET_BIT(P0, BIT5);
}
//prt,0:1号模拟口,1:2号模拟口
void IM_IICSCL_OUT(unsigned char prt)
{
    CLR_BIT(PM0, BIT4);
    CLR_BIT(POM0, BIT4);
    CLR_BIT(PFSEG6, BIT0);
}
//prt,0:1号模拟口,1:2号模拟口
void IM_IICSCL_IN(unsigned char prt)
{
    SET_BIT(PM0, BIT4);
    CLR_BIT(PFSEG6, BIT0);
}
//prt,0:1号模拟口,1:2号模拟口
void IM_IICSCL_UP(unsigned char prt)
{
}
// prt,0:1号模拟口,1:2号模拟口
void IM_IICSCL_DOWN(unsigned char prt)
{
}
// prt,0:1号模拟口,1:2号模拟口
void IM_IICSCL_SET(unsigned char prt)
{
    SET_BIT(P0, BIT4);
}
// prt,0:1号模拟口,1:2号模拟口
void IM_IICSCL_CLR(unsigned char prt)
{
    CLR_BIT(P0, BIT4);
}
// prt,0:1号模拟口,1:2号模拟口
void IM_IICSDA_OUT(unsigned char prt)
{
    CLR_BIT(PM0, BIT3);
    CLR_BIT(PFSEG5, BIT7);
}
//prt,0:1号模拟口,1:2号模拟口
void IM_IICSDA_IN(unsigned char prt)
{
    SET_BIT(PM0, BIT3);
    CLR_BIT(PIM0, BIT3);
    CLR_BIT(PFSEG5, BIT7);
}
//prt,0:1号模拟口,1:2号模拟口
void IM_IICSDA_UP(unsigned char prt)
{
}
// prt,0:1号模拟口,1:2号模拟口
void IM_IICSDA_DOWN(unsigned char prt)
{
}
// prt,0:1号模拟口,1:2号模拟口
void IM_IICSDA_SET(unsigned char prt)
{
    SET_BIT(P0, BIT3);
}
// prt,0:1号模拟口,1:2号模拟口
void IM_IICSDA_CLR(unsigned char prt)
{
    CLR_BIT(P0, BIT3);
}
// prt,0:1号模拟口,1:2号模拟口
unsigned char IM_IICSDA_STU(unsigned char prt)
{
    return P0&BIT3;
}
// prt,0:1号模拟口,1:2号模拟口
//返回值，0:低，非0:高
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
