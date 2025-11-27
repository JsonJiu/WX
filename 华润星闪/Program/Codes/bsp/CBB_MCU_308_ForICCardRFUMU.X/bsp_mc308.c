//IC卡无线远传燃气表V1.0.7_CBB.PrjPCB
#include    "bsp_mc308.h"
#include    "lib_mc308.h"
/*
 *         iPPSInput(IN_FN_PPS_INT1|IN_PIN_PPS_RP16);
        ConfigINT1(FALLING_EDGE_INT |INT_ENABLE|INT_PRI_0);//XXXX;
*/
#if 0
//24FJ128GA308配置字
int CONFIG4 __attribute__((space(prog), address(0x157F8))) = 0xFE3F;
#ifdef EXT_TIMER_CLOCK
int CONFIG3 __attribute__((space(prog), address(0x157FA))) = 0xFF7F;
#else
int CONFIG3 __attribute__((space(prog), address(0x157FA))) = 0xFFFF;
#endif
int CONFIG2 __attribute__((space(prog), address(0x157FC))) = 0xFA1E;//0xFA1E
int CONFIG1 __attribute__((space(prog), address(0x157FE))) = 0x399F;
#else
//24FJ64GA308配置字
int CONFIG4 __attribute__((space(prog), address(0xABF8))) = 0xFE3F;
#ifdef EXT_TIMER_CLOCK
int CONFIG3 __attribute__((space(prog), address(0xABFA))) = 0xFF7F;
#else
int CONFIG3 __attribute__((space(prog), address(0xABFA))) = 0xFFFF;
#endif
int CONFIG2 __attribute__((space(prog), address(0xABFC))) = 0xFA1E;//0xFA1E
int CONFIG1 __attribute__((space(prog), address(0xABFE))) = 0x399F;
#endif

extern void IrqServerDefaultRS(unsigned char Data);
extern void IrqServerDefault(void);
extern IRQServerFT IRQServerINT1;
extern IRQServerFT IRQServerINT2;
extern IRQServerFT IRQServerINT3;
extern IRQServerFT IRQServerINT4;
//RF7
void IM_m2m_Set_MotorSleep_GPIO(unsigned char fun)//完成
{
    TRISFbits.TRISF7 = 0;
    ODCFbits.ODF7 = 0;
    if (fun == 1)
    {
        LATFbits.LATF7 = 1;
    }
    else
    {
        LATFbits.LATF7 = 0;
    }
}
//RG3
void IM_m2m_Set_MotorOpen_GPIO(unsigned char fun)//完成
{
    TRISFbits.TRISF3 = 0;
    ODCFbits.ODF3 = 0;
    if (fun == 1)
    {
        LATFbits.LATF3 = 1;
    }
    else
    {
        LATFbits.LATF3 = 0;
    }
}
//RG2
void IM_m2m_Set_MotorClose_GPIO(unsigned char fun)//完成
{
    TRISGbits.TRISG2 = 0;
    ODCGbits.ODG2 = 0;
    if (fun == 1)
    {
        LATGbits.LATG2 = 1;
    }
    else
    {
        LATGbits.LATG2 = 0;
    }
}
//RD13
void BspBeepOn(void)//完成
{
    TRISDbits.TRISD13 = 0;
    ODCDbits.ODD13 = 0;
    LATDbits.LATD13 = 1;
}
void BspBeepOff(void)//完成
{
    TRISDbits.TRISD13 = 0;
    ODCDbits.ODD13 = 0;
    LATDbits.LATD13 = 0;
}
//------------------------------------------------------------------------
//------------------------------------------------------------------------
//邹航邹航邹航邹航邹航邹航邹航邹航邹航邹航邹航邹航邹航邹航邹航邹航邹航邹航
//------------------------------------------------------------------------
//------------------------------------------------------------------------
//------------------------------------
//液晶液晶液晶液晶液晶液晶液晶液晶液晶
//------------------------------------
#if 0
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
 
    }
    MCU_LcdCfg();
}
#endif
//------------------------------------
//霍尔霍尔霍尔霍尔霍尔霍尔霍尔霍尔霍尔
//------------------------------------
//中断回调函数
//
#if 0
extern IRQServerFT IRQServerCN63;
extern IRQServerFT IRQServerCN64;
//HALLA:RE6/CN64
//HALLB:RE5/CN63
void HallAInt(void);
void HallBInt(void);
//获取霍尔io口状态
unsigned char GpioHallARead(void)
{
    return (PORTEbits.RE6);//XXXX
}
unsigned char GpioHallBRead(void)
{
    return (PORTEbits.RE5);//XXXX
}
//改变中断触发边沿
//0：上升沿；1：下降沿
void HallASetInt(unsigned char c,void(*p)(void))
{
    TRISEbits.TRISE6 = 1;
    ANSEbits.ANSE6 = 0;
    ODCEbits.ODE6 = 0;
    InputChange_Clear_Intr_Status_Bit;
    ConfigIntCN(INT_ENABLE | INT_PRI_4);//XXXX;
    CNEN5bits.CN64IE = 1;
    IRQServerCN64 = p;
}
void HallBSetInt(unsigned char c,void(*p)(void))
{
    TRISEbits.TRISE5 = 1;
    ANSEbits.ANSE5 = 0;
    ODCEbits.ODE5 = 0;
    InputChange_Clear_Intr_Status_Bit;
    ConfigIntCN(INT_ENABLE | INT_PRI_4);//XXXX;
    CNEN4bits.CN63IE = 1;
    IRQServerCN63 = p;
}
#endif
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
 * RST:RB11
 * CHK:RD15;CN21
 * SDA:RB13
 * SCL:RB12
 * ERR:RB10
 * POW:RF4
*/
#if 0
extern IRQServerFT IRQServerCN21;
//插卡中断 0：上升沿；1：下降沿
void CardIntConfig(unsigned char c,void(*p)(void))//CHK//
{
    TRISDbits.TRISD15 = 1;
    ODCDbits.ODD15 = 0;
#ifdef USER_CN_IRQ
    InputChange_Clear_Intr_Status_Bit;
    ConfigIntCN(INT_ENABLE | INT_PRI_4);//XXXX;
    CNEN2bits.CN21IE = 1;
    IRQServerCN21 = p;
#else
    RPINR0bits.INT1R = 5;//rp5;
    ConfigINT1(FALLING_EDGE_INT | INT_ENABLE | INT_PRI_3);
    IRQServerINT1 = p;
#endif
}
unsigned char CardBslRead(void)//
{
    if (PORTDbits.RD15)//XXXX
        return 1;
    else
        return 0;
}
void CardSclOutput(void)//
{
    TRISBbits.TRISB12 = 0;
    ANSBbits.ANSB12 = 0;
    ODCBbits.ODB12 = 0;
}
void CardSclSet(void)//
{
    LATBbits.LATB12 = 1;
}
void CardSclClr(void)//
{
    LATBbits.LATB12 = 0;
}
void CardRstOutput(void)//
{
    TRISBbits.TRISB11 = 0;
    ANSBbits.ANSB11 = 0;
    ODCBbits.ODB11= 0;
}
void CardRstSet(void)//
{
    LATBbits.LATB11 = 1;
}
void CardRstClr(void)//
{
    LATBbits.LATB11 = 0;
}
void CardSdaSet(void)//
{
    LATBbits.LATB13 = 1;
}
void CardSdaClr(void)//
{
    LATBbits.LATB13 = 0;
}
void CardSdaOutput(void)//
{
    TRISBbits.TRISB13 = 0;
    ANSBbits.ANSB13 = 0;
    ODCBbits.ODB13 = 0;
}
void CardSdaInput(void)//
{
    TRISBbits.TRISB13 = 1;
    ANSBbits.ANSB13 = 0;
    ODCBbits.ODB13 = 0;
}
unsigned char CardSdaRead(void)//
{
    if (PORTBbits.RB13)//XXXX
        return 1;
    else
        return 0;
}
void CardPowerCheckInput(void)//
{
    TRISBbits.TRISB10 = 1;
    ANSBbits.ANSB10 = 0;
    ODCBbits.ODB10 = 0;
}
unsigned char CardPowerCheckRead(void)//
{
    if (PORTBbits.RB10)//XXXX
        return 1;
    else
        return 0;
}
void CardPowerContrlOutput(void)//
{
    TRISFbits.TRISF4 = 0;
    ODCFbits.ODF4 = 0;
}
void CardPowerContrlSet(void)//
{
    LATFbits.LATF4 = 1;
}
void CardPowerContrlClr(void)//
{
    LATFbits.LATF4 = 0;
}
#endif
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
//朱毓?朱毓?朱毓?朱毓?朱毓?朱毓?朱毓?朱毓?朱毓?朱毓?朱毓?朱毓?
//------------------------------------------------------------------------
//------------------------------------------------------------------------
//------------------------------------
//ADCADCADCADCADCADCADCADCADCADCADCADC
//------------------------------------
//RA9
void IO_VBB_CHK_EN_DIR_OUT(void)//完成
{
    TRISAbits.TRISA9 = 0;
    ODCAbits.ODA9 = 0;
}
void IO_VBB_CHK_EN_1(void)//完成
{
    LATAbits.LATA9 = 1;
}
void IO_VBB_CHK_EN_0(void)//完成
{
    LATAbits.LATA9 = 0;
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
//系统AN2/RB2,阀门AN3/RB3
void OA_ADC_Init(ADC_InitTypeDef* ADC_InitStruct)//完成
{
    ADCModeST ADCMode;
    ADCMode.ADCModeOpt = ADC_InitStruct->ADCModeConversionMode;
    if (ADC_InitStruct->ADCModeConversionBit == 8)
        ADCMode.ADCModeResolution = 0;
    else if (ADC_InitStruct->ADCModeConversionBit == 10)
        ADCMode.ADCModeResolution = 1;
    else
        ADCMode.ADCModeResolution = 2;

    if (ADC_InitStruct->ADCModeChannel == 1)//阀门
    {
        MCU_ADCCfg(PortVmNum3, ADCMode);
    }
    else//系统
    {
        MCU_ADCCfg(PortVmNum2, ADCMode);
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
void OA_ADC_CMD(FunctionalState NewState)//完成
{
    if (NewState == DISABLE)
        MCU_ADCStop(0);
    else
        MCU_ADCStart(0);
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
unsigned short OA_ADC_GetConversionValue(void)//完成
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
unsigned char OA_ADC_GetConversionState(void)//完成
{
    if (BusySampADC1 == FALSE)
        return 0;
    else
        return 1;
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

void OA_ADC_SetConversionChannel(unsigned char channel)//完成
{
    if (channel == 1)//阀门
    {
        TRISBbits.TRISB3 = 1;//配置为输入
        ANSBbits.ANSB3 = 1;//配置为模拟
        SetChanADC10(ADC_CH0_POS_SAMPLEA_AN3);
    }
    else//系统
    {
        TRISBbits.TRISB2 = 1;//配置为输入
        ANSBbits.ANSB2 = 1;//配置为模拟
        SetChanADC10(ADC_CH0_POS_SAMPLEA_AN2);
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
/*
 * RFDIO:RD4
 * ACK:RE0
 * RERCV:RG0
 * SDI:RD3
 * RFCLK:RD1
 * SDO:RG1
 * RST:RF1
 * SCK:RF0
 * NSS:RD2
 * RFTX:RD5/RF10
 * RFRX:RD11
 * RSSI:RE2
 */
void OR_Rfm4463_RFDIO_DIR_Ctrl(unsigned char dir)//完成
{
    if(dir == 1)
    {
        TRISDbits.TRISD4 = 0;
        //ANSDbits.ANSD4 = 0;//配置为模拟
        ODCDbits.ODD4 = 0;
    }
    else
    {
        TRISDbits.TRISD4 = 1;
        //ANSDbits.ANSD4 = 0;//配置为模拟
        ODCDbits.ODD4 = 0;
    }
}

/**
* @declare Rfm4463无线模块射频输入输出脚输出高电平
*
* @author Administrator (2014-10-30)
*
* @requirements GPIO
*/

void OR_Rfm4463_RFDIO_Set(void)//完成
{
    LATDbits.LATD4 = 1;
}

/**
* @declare Rfm4463无线模块射频输入输出脚输出低电平
*
* @requirements GPIO
*
* @author Administrator (2014-10-30)
*/
void OR_Rfm4463_RFDIO_Reset(void)//完成
{
    LATDbits.LATD4 = 0;
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
void OR_Rfm4463_ACK_DIR_Ctrl(unsigned char dir)//完成
{
    if (dir == 1)
    {
        TRISEbits.TRISE0 = 0;

        ODCEbits.ODE0 = 0;
    }
    else
    {
        TRISEbits.TRISE0 = 1;

        ODCEbits.ODE0 = 0;
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
unsigned char OR_Rfm4463_ACK_Read(void)//完成
{
    if (PORTEbits.RE0)//XXXX
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
void OR_Rfm4463_RERCV_Dir_Ctrl(unsigned char dir)//完成
{
    if (dir == 1)
    {
        TRISGbits.TRISG0 = 0;
        //ANSGbits.ANSG0 = 0;//配置为模拟
        ODCGbits.ODG0 = 0;
    }
    else
    {
        TRISGbits.TRISG0 = 1;
        //ANSGbits.ANSG0 = 0;//配置为模拟
        ODCGbits.ODG0 = 0;
    }
}

/**
* @declare Rfm4463无线模块接收复位脚输出高电平
*
* @author Administrator (2014-10-30)
*
* @requirements GPIO
*/
void OR_Rfm4463_RERCV_Set(void)//完成
{
    LATGbits.LATG0 = 1;
}

/**
* @declare Rfm4463无线模块接收复位脚输出低电平
*
* @author Administrator (2014-10-30)
*
* @requirements GPIO
*/
void OR_Rfm4463_RERCV_Reset(void)//完成
{
    LATGbits.LATG0 = 0;
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
void OR_Rfm4463_SDI_DIR_Ctrl(unsigned char dir)//完成
{
    if (dir == 1)
    {
        TRISDbits.TRISD3 = 0;
        //ANSDbits.ANSD3 = 0;//配置为模拟
        ODCDbits.ODD3 = 0;
    }
    else
    {
        TRISDbits.TRISD3 = 1;
        //ANSDbits.ANSD3 = 0;//配置为模拟
        ODCDbits.ODD3 = 0;
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
unsigned char OR_Rfm4463_SDI_Read(void)//完成
{
    if (PORTDbits.RD3)//XXXX
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
void OR_Rfm4463_RFCLK_DIR_Ctrl(unsigned char dir)//完成
{
    if (dir == 1)
    {
        TRISDbits.TRISD1 = 0;
        //ANSDbits.ANSD1 = 0;//配置为模拟
        ODCDbits.ODD1 = 0;
    }
    else
    {
        TRISDbits.TRISD1 = 1;
        //ANSDbits.ANSD1 = 0;//配置为模拟
        ODCDbits.ODD1 = 0;
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
unsigned char OR_Rfm4463_RFCLK_Read(void)//完成
{
    if (PORTDbits.RD1)//XXXX
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
void OR_Rfm4463_SDO_DIR_Ctrl(unsigned char dir)//完成
{
    if (dir == 1)
    {
        TRISGbits.TRISG1 = 0;
        //ANSGbits.ANSG1 = 0;//配置为模拟
        ODCGbits.ODG1 = 0;
    }
    else
    {
        TRISGbits.TRISG1 = 1;
        //ANSGbits.ANSG1 = 0;//配置为模拟
        ODCGbits.ODG1 = 0;
    }
}

/**
* @declare Rfm4463无线模块SPI输出脚输出高电平
*
* @author Administrator (2014-10-30)
*
* @requirements GPIO
*/
void OR_Rfm4463_SDO_Set(void)//完成
{
    LATGbits.LATG1 = 1;
}

/**
* @declare Rfm4463无线模块SPI输出脚输出低电平
*
* @author Administrator (2014-10-30)
*
* @requirements GPIO
*/
void OR_Rfm4463_SDO_Reset(void)//完成
{
    LATGbits.LATG1 = 0;
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
void OR_Rfm4463_RST_DIR_Ctrl(unsigned char dir)//完成
{
    if (dir == 1)
    {
        TRISFbits.TRISF1 = 0;
        //ANSFbits.ANSF1 = 0;//配置为模拟
        ODCFbits.ODF1 = 0;
    }
    else
    {
        TRISFbits.TRISF1 = 1;
        //ANSFbits.ANSF1 = 0;//配置为模拟
        ODCFbits.ODF1 = 0;
    }
}

/**
* @declare Rfm4463无线模块复位脚输出高电平
*
* @author Administrator (2014-10-30)
*
* @requirements GPIO
*/
void OR_Rfm4463_RST_Set(void)//完成
{
    LATFbits.LATF1 = 1;
}

/**
* @declare Rfm4463无线模块复位脚输出低电平
*
* @author Administrator (2014-10-30)
*
* @requirements GPIO
*/
void OR_Rfm4463_RST_Reset(void)//完成
{
    LATFbits.LATF1 = 0;
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
void OR_Rfm4463_SCK_DIR_Ctrl(unsigned char dir)//完成
{
    if (dir == 1)
    {
        TRISFbits.TRISF0 = 0;
        ODCFbits.ODF0 = 0;
    }
    else
    {
        TRISFbits.TRISF0 = 1;
        ODCFbits.ODF0 = 0;
    }
}

/**
* @declare Rfm4463无线模块SPI时钟脚输出高电平
*
* @author Administrator (2014-10-30)
*
* @requirements GPIO
*/
void OR_Rfm4463_SCK_Set(void)//完成
{
    LATFbits.LATF0 = 1;
}

/**
* @declare Rfm4463无线模块SPI时钟脚输出低电平
*
* @author Administrator (2014-10-30)
*
* @requirements GPIO
*/
void OR_Rfm4463_SCK_Reset(void)//完成
{
    LATFbits.LATF0 = 0;
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
void OR_Rfm4463_NSS_DIR_Ctrl(unsigned char dir)//完成
{
    if (dir == 1)
    {
        TRISDbits.TRISD2 = 0;
        ODCDbits.ODD2 = 0;
    }
    else
    {
        TRISDbits.TRISD2 = 1;
        ODCDbits.ODD2 = 0;
    }
}

/**
* @declare Rfm4463无线模块SPI使能脚输出高电平
*
* @author Administrator (2014-10-30)
*
* @requirements GPIO
*/
void OR_Rfm4463_NSS_Set(void)//完成
{
    LATDbits.LATD2 = 1;
}

/**
* @declare Rfm4463无线模块SPI使能脚输出低电平
*
* @author Administrator (2014-10-30)
*
* @requirements GPIO
*/
void OR_Rfm4463_NSS_Reset(void)//完成
{
    LATDbits.LATD2 = 0;
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
void OR_Rfm4463_RFTX_DIR_Ctrl(unsigned char dir)//完成
{
    if (dir == 1)
    {
        TRISDbits.TRISD5 = 0;
        //ANSDbits.ANSD5 = 0;//配置为模拟
        ODCDbits.ODD5 = 0;
    }
    else
    {
        TRISDbits.TRISD5 = 1;
        //ANSDbits.ANSD5 = 0;//配置为模拟
        ODCDbits.ODD5 = 0;
    }
}

/**
* @declare Rfm4463无线模块射频发射使能脚输出高电平
*
* @author Administrator (2014-10-30)
*
* @requirements GPIO
*/
void OR_Rfm4463_RFTX_Set(void)//完成
{
    LATDbits.LATD5 = 1;
}

/**
* @declare Rfm4463无线模块射频发射使能脚输出低电平
*
* @author Administrator (2014-10-30)
*
* @requirements GPIO
*/
void OR_Rfm4463_RFTX_Reset(void)//完成
{
    LATDbits.LATD5 = 0;
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
void OR_Rfm4463_RFRX_DIR_Ctrl(unsigned char dir)//完成
{
    if (dir == 1)
    {
        TRISDbits.TRISD11 = 0;
        ANSDbits.ANSD11 = 0;//配置为模拟
        ODCDbits.ODD11 = 0;
    }
    else
    {
        TRISDbits.TRISD11 = 1;
        ANSDbits.ANSD11 = 0;//配置为模拟
        ODCDbits.ODD11 = 0;
    }
}

/**
* @declare Rfm4463无线模块射频接收使能脚输出高电平
*
* @author Administrator (2014-10-30)
*
* @requirements GPIO
*/
void OR_Rfm4463_RFRX_Set(void)//完成
{
    LATDbits.LATD11 = 1;
}

/**
* @declare Rfm4463无线模块射频接收使能脚输出低电平
*
* @author Administrator (2014-10-30)
*
* @requirements GPIO
*/
void OR_Rfm4463_RFRX_Reset(void)//完成
{
    LATDbits.LATD11 = 0;
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
void OR_Rfm4463_RSSI_DIR_Ctrl(unsigned char dir)//完成
{
    if (dir == 1)
    {
        TRISEbits.TRISE2 = 0;
        //ANSEbits.ANSE2 = 0;//配置为模拟
        ODCEbits.ODE2 = 0;
    }
    else
    {
        TRISEbits.TRISE2 = 1;
        //ANSEbits.ANSE2 = 0;//配置为模拟
        ODCEbits.ODE2 = 0;
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
unsigned char OR_Rfm4463_RSSI_Read(void)//完成
{
    if (PORTEbits.RE2)//XXXX
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
//RP25/RD4
void OR_Rfm4463_Uart_Init(UartMode* UartMode_ST, void(*fun)(unsigned char))
{
    ANSBbits.ANSB2 = 0;
   TRISBbits.TRISB2 = 1;
    iPPSInput(IN_FN_PPS_U2RX, IN_PIN_PPS_RP13);
    Uart2Config(UartMode_ST,IrqServerDefaultRS,fun);
}
/*----------------------------------------------------------------------------*/
#if 0
void RFUart_Init(UartMode* UartMode_ST, void(*fun)(unsigned char))
{
   ANSBbits.ANSB8 = 0;
   ANSBbits.ANSB9 = 0;
   TRISBbits.TRISB8 = 1;
   TRISBbits.TRISB9 = 0;

    iPPSInput(IN_FN_PPS_U2RX, IN_PIN_PPS_RP8);
    iPPSOutput(OUT_PIN_PPS_RP9, OUT_FN_PPS_U2TX);
    Uart2Config(UartMode_ST,IrqServerDefaultRS,fun);
}

void RFUartSend(unsigned char *buf, unsigned int len)
{
    Uart2SendData(buf, len);
}
/**
 *
 *
 * @author linyx (2015-01-06)
 *
 * @param UartMode_ST
 * @param fun
 */
void OR_APC340F_Uart_Start(UartMode* UartMode_ST, void(*fun)(unsigned char))
{
    ANSBbits.ANSB0 = 0;
    ANSBbits.ANSB1 = 0;
    TRISBbits.TRISB0 = 1;
    TRISBbits.TRISB1 = 0;
    iPPSInput(IN_FN_PPS_U1RX, IN_PIN_PPS_RP0);
    iPPSOutput(OUT_PIN_PPS_RP1, OUT_FN_PPS_U1TX);
    Uart1Config(UartMode_ST,IrqServerDefaultRS,fun);
}
void OR_APC340F_Uart_Stop(void)
{
    CloseUART1();
}
void OR_APC340F_Uart_Send(unsigned char * tx_buf, unsigned int tx_num)
{
    Uart1SendData(tx_buf, tx_num);
}
#endif
/*
 参数设置串口初始化
 * InitPara:串口设置参数
 * RecvInterrupt:接收中断回调函数
 */
//TXD:RP8;RB8;RXD:RP9;RB9
void ParaSetUart_Init(UartMode* UartMode_ST, void(*fun)(unsigned char))//完成
{
    ANSBbits.ANSB9 = 0;
    ANSBbits.ANSB8 = 0;
    TRISBbits.TRISB9 = 1;
    TRISBbits.TRISB8 = 0;
    iPPSInput(IN_FN_PPS_U3RX, IN_PIN_PPS_RP9);
    iPPSOutput(OUT_PIN_PPS_RP8, OUT_FN_PPS_U3TX);
    Uart3Config(UartMode_ST,IrqServerDefaultRS,fun);
}
/*
 参数设置串口使能
 */
void ParaSetUart_Enable(void)//完成
{
    Uart3SendEnable();
}
/*
 参数设置串口禁止
 */
void ParaSetUart_Disable(void)//完成
{
    Uart3SendDisable();
}
/*
 参数设置串口发送数据
 * SendBuf:发送缓冲区指针
 * Len:发送长度
 * 返回:发送成功或者失败
 */
unsigned int ParaSetUart_SendData(unsigned char *SendBuf,unsigned short Len)//完成
{
    Uart3SendData(SendBuf, Len);
}

/**
* @declare 使能或者失能Rfm4463射频接收UART模块
*
* @author ZYX (2014/11/5)
*
* requirements 单片机UART接收模块
*
* @param NewState
*                 外设UART的新状态，这个参数可以取：ENABLE或者DISABLE
*/
void OR_Rfm4463_Uart_Cmd(FunctionalState NewState)//完成
{
    if(NewState == DISABLE)
    {
        Uart2ReceiveDisable();
    }
    else
    {
        Uart2ReceiveEnable();
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

/*===============================KEY配置==============================*/
//EKY:RP10/RF4
/**
* 获取编程按键的状态
*
* @author wb (2014-10-31)
*
* @return unsigned char
*         :返回0：表示按键按下；返回1：表示按键松开
*/
unsigned char IM_m2m_GetProgrameKey_Status(void)//无此按键//
{
    return 1;
}
/**
* 获取用户按键的状态
*
* @author wb (2014-10-31)
*
* @return unsigned char
*         :返回0：表示按键按下；返回1：表示按键松开
*/
unsigned char IM_m2m_GetUsersKey_Status(void)//
{
    if (PORTFbits.RF4)//XXXX
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
void IM_OpenExt_Int(unsigned char key_type)//
{
    TRISFbits.TRISF4 = 1;
    ODCFbits.ODF4 = 0;
    RPINR1bits.INT2R = 10;//rp10;
    ConfigINT2(FALLING_EDGE_INT | INT_ENABLE | INT_PRI_3);
    //IRQServerINT2 = p;
}
/**
* 关闭外部中断
*
* @author wb (2014-11-10)
*
* @param key_type ：中断类型
*/
void IM_CloseExt_Int(unsigned char key_type)//不可关闭//
{

}
/**
* 中断初始化:编程按键
*    配置成中断接口，，下降沿触发，不带上拉电阻；
* @author wb (2014-10-31)
*
* @param fun 中断服务函数指针：fun由驱动编写，由BSP调用
*/
void IM_m2m_ProgrameKey_Intp_Config(void(*fun)(void))//无此按键//
{
}
/**
* 中断初始化：用户按键
*    配置成中断接口，，下降沿触发，不带上拉电阻；
* @author wb (2014-10-31)
*
* @param fun 中断服务函数指针：fun由驱动编写，由BSP调用
*/
extern IRQServerFT IRQServerCN18;
void IM_m2m_UserKey_Intp_Init(void(*fun)(void))//
{
    IRQServerINT2 = fun;
}
/*==============================串口配置==============================*/
#if 1
/**
* USB->com=0->uart0:    
                        
* GPRS->com=1->uart1:   
                        
* 光电->com=2->uart3:   
*配置USART 发送IO:TX输出高
*
* @author wb (2014-11-06)
*
* @param com :串口端口:0-空;1-usb;2-gprs;3-光电
* @param fun :1-置高；0-置低
*/
void IM_m2m_Set_Usart_TX_GPIO(unsigned char com, unsigned char fun)
{
    switch (com)
    {
    default:
    case 0:
        if (fun == 1)
        {

        }
        else
        {

        }
        break;
    case 1:
        if (fun == 1)
        {

        }
        else
        {

        }
        break;
    case 2:
        if (fun == 1)
        {
        }
        else
        {
        }
        break;
    case 3:
        if (fun == 1)
        {
        }
        else
        {
        }
        break;
    }
}
/**
*配置USART 接收IO:配置为输入
*
* @author wb (2014-11-06)
*
* @param com :串口端口:0-USB;1-GPRS;2-光电;3-默认
*/
void IM_m2m_Config_Usart_RX_DIR(unsigned char com)
{
    switch (com)
    {
    default:
    case 0:

        break;
    case 1:

        break;
    case 2:

        break;
    case 3:break;
    }
}
/**
* 串口配置：UART ；1 
* 位起始位，1位停止位，8位数据位，无校验 
* 全部都使能接收中断 
* @author wb (2014-11-06)
*
* @param com :串口端口:0-USB;1-GPRS;2-光电;3-默认
* @param baud 
*             :波特率：0-1200；1-2400;2-4800;3-9600；4-19200;5-38400;6-115200;7-62500
* @param fun ：串口接收中断服务函数
*/
void IM_m2m_Uart_Config(unsigned char com, unsigned long baud, void(*fun)(unsigned char rxd))
{
    UartMode Mode;
    switch (baud)
    {
    case 0:
        Mode.Baud = UartBaud1200;
        break;
    case 1:
        Mode.Baud = UartBaud2400;
        break;
    case 2:
        Mode.Baud = UartBaud4800;
        break;
    case 3:
        Mode.Baud = UartBaud9600;
        break;
    case 4:
        Mode.Baud = UartBaud19200;
        break;
    case 5:
        Mode.Baud = UartBaud38400;
        break;
    case 6:
        Mode.Baud = UartBaud115200;
        break;
    case 7:
        Mode.Baud = UartBaud62500;
        break;
    }
    Mode.Parity = 0;
    Mode.StopBit = 1;
    Mode.DataBit = 1;
    Mode.Sequence = 0;
    switch (com)
    {
    default:
    case 0:
        Uart1Config(&Mode,0,fun);
        break;
    case 1:
        Uart2Config(&Mode, 0, fun);
        break;
    case 2:
        TRISDbits.TRISD8 = 1;
        TRISDbits.TRISD9 = 0;

        iPPSInput(IN_FN_PPS_U3RX, IN_PIN_PPS_RP2);
        iPPSOutput(OUT_PIN_PPS_RP4, OUT_FN_PPS_U3TX);
        Uart3Config(&Mode,IrqServerDefaultRS,fun);

        break;
    case 3:
        TRISFbits.TRISF2 = 1;
        TRISFbits.TRISF8 = 0;

        iPPSInput(IN_FN_PPS_U4RX, IN_PIN_PPS_RP30);
        iPPSOutput(OUT_PIN_PPS_RP15, OUT_FN_PPS_U4TX);
        Uart4Config(&Mode,IrqServerDefaultRS,fun);

        break;
    }
}
/**
* 串口配置：打开串口
*
* @author wb (2014-11-06)
*
* @param com ：串口端口:0-USB;1-GPRS;2-光电;3-默认
*/
void IM_m2m_Open_Usart(unsigned char com)
{
    switch (com)
    {
    default:
    case 0:
        Uart0SendEnable();
        Uart0ReceiveEnable();
        break;
    case 1:
        Uart1SendEnable();
        Uart1ReceiveEnable(); 
        break;
    case 2:
        Uart3SendEnable();
        Uart3ReceiveEnable(); 
        break;
    case 3:break;
    }
}
/**
* 串口配置：关闭串口
*
* @author wb (2014-11-06)
*
* @param com :串口端口:0-USB;1-GPRS;2-光电;3-默认
*/
void IM_m2m_Close_Usart(unsigned char com)
{
    switch (com)
    {
    default:
    case 0:
        Uart0SendDisable();
        Uart0ReceiveDisable();
        break;
    case 1:
        Uart1SendDisable();
        Uart1ReceiveDisable(); 
        break;
    case 2:
        Uart3SendDisable();
        Uart3ReceiveDisable(); 
        break;
    case 3:break;
    }
}

/**
* 串口发送数据
* 
* @author wb (2014-11-19)
* 
* @param com ：串口端口:0-USB;1-GPRS;2-光电;3-默认
* @param buf :发送数据缓冲区
* @param len ：发送数据长度
* 
* @return unsigned char ：返回1-发送成功；返回0-发送失败
*/

unsigned char IM_m2m_Usart_Send_Data(unsigned char com,unsigned char *buf,unsigned short len)
{
    switch (com)
    {
    default:
    case 0:
        ;//XXXX;//Uart0SendData(buf,len);
        break;
    case 1:
        ;//XXXX;//Uart1SendData(buf, len);
        break;
    case 2:
        Uart3SendData(buf, len);
        break;
    case 3:
        Uart4SendData(buf, len);
        break;
    }
    return 1;
}
#endif
//
//------------------------------------------------------------------------
//------------------------------------------------------------------------
//115 115 115 115 115 115 115 115 115 115 115 115 115 115
//------------------------------------------------------------------------
//------------------------------------------------------------------------
//prt:0表示EEPROM/RTC8025T模拟口;1表示ICCARD模拟口
//EEPROM/8025
//POW:RB1
//SDA:RC3
//SCL:RC1
//I2C电源配置输出   
void IM_IICPWR_OUT(unsigned char prt) //prt,0:1号模拟口,1:2号模拟口//
{
    if (prt)
    {

    }
    else
    {
        TRISBbits.TRISB1 = 0;
        ODCBbits.ODB1 = 0;
    }
}
//I2C电源配置上拉
void IM_IICPWR_UP(unsigned char prt) //prt,0:1号模拟口,1:2号模拟口//
{
    if (prt)
    {

    }
    else
    {

    }
}
//I2C电源配置下拉
void IM_IICPWR_DOWN(unsigned char prt) //prt,0:1号模拟口,1:2号模拟口//
{
    if (prt)
    {
    }
    else
    {
    }
}
//I2C电源输出高
void IM_IICPWR_ON(unsigned char prt) //prt,0:1号模拟口,1:2号模拟口//
{
    if (prt)
    {

    }
    else
    {
        LATBbits.LATB1 = 0;
    }
}
//I2C电源输出低
void IM_IICPWR_OFF(unsigned char prt) //prt,0:1号模拟口,1:2号模拟口//
{
    if (prt)
    {

    }
    else
    {
        LATBbits.LATB1 = 1;
    }
}    
//prt,0:1号模拟口,1:2号模拟口
void IM_IICSCL_OUT(unsigned char prt)//
{
    if (prt)
    {

    }
    else
    {
        TRISCbits.TRISC1 = 0;
        ODCCbits.ODC1 = 0;
    }
}
//prt,0:1号模拟口,1:2号模拟口
void IM_IICSCL_IN(unsigned char prt)//
{
    if (prt)
    {

    }
    else
    {
        TRISCbits.TRISC1 = 1;
        ODCCbits.ODC1 = 0;
    }
}
//prt,0:1号模拟口,1:2号模拟口
void IM_IICSCL_UP(unsigned char prt)//
{
    if (prt)
    {

    }
    else
    {

    }
}
// prt,0:1号模拟口,1:2号模拟口
void IM_IICSCL_DOWN(unsigned char prt)//
{
    if (prt)
    {
    }
    else
    {
    }
}
// prt,0:1号模拟口,1:2号模拟口
void IM_IICSCL_SET(unsigned char prt)//
{
    if (prt)
    {

    }
    else
    {
        LATCbits.LATC1 = 1;
    }
}
// prt,0:1号模拟口,1:2号模拟口
void IM_IICSCL_CLR(unsigned char prt)//
{
    if (prt)
    {

    }
    else
    {
        LATCbits.LATC1 = 0;
    }
}
// prt,0:1号模拟口,1:2号模拟口
void IM_IICSDA_OUT(unsigned char prt)//
{
    if (prt)
    {

    }
    else
    {
        TRISCbits.TRISC3 = 0;
        ODCCbits.ODC3 = 0;
    }
}
//prt,0:1号模拟口,1:2号模拟口
void IM_IICSDA_IN(unsigned char prt)//
{
    if (prt)
    {

    }
    else
    {
        TRISCbits.TRISC3 = 1;
        ODCCbits.ODC3 = 0;
    }
}
//prt,0:1号模拟口,1:2号模拟口
void IM_IICSDA_UP(unsigned char prt)//
{
    if (prt)
    {

    }
    else
    {

    }
}
// prt,0:1号模拟口,1:2号模拟口
void IM_IICSDA_DOWN(unsigned char prt)//
{
    if (prt)
    {
    }
    else
    {
    }
}
// prt,0:1号模拟口,1:2号模拟口
void IM_IICSDA_SET(unsigned char prt)//
{
    if (prt)
    {

    }
    else
    {
        LATCbits.LATC3 = 1;
    }
}
// prt,0:1号模拟口,1:2号模拟口
void IM_IICSDA_CLR(unsigned char prt)//
{
    if (prt)
    {

    }
    else
    {
        LATCbits.LATC3 = 0;
    }
}
// prt,0:1号模拟口,1:2号模拟口
unsigned char IM_IICSDA_STU(unsigned char prt)//
{
    if (prt)
    {
        return 1;//XXXX;
    }
    else
    {
        return PORTCbits.RC3;//XXXX;
    }
}
// prt,0:1号模拟口,1:2号模拟口
//返回值，0:低，非0:高

//------------------------------------------------------------------------
//------------------------------------------------------------------------
//谢尚川谢尚川谢尚川谢尚川谢尚川谢尚川谢尚川谢尚川谢尚川谢尚川谢尚川谢尚川
//------------------------------------------------------------------------
//------------------------------------------------------------------------
#if 0
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
//GDEN:RE4
//GDRXD:RG7/RP26,CN9
void ZD_POW_Init(IODIR dir, IOPP pp)
{
    if(dir == IODIR_Output)
    {
        TRISEbits.TRISE4 = 0;
        ANSEbits.ANSE4 = 0;
        ODCEbits.ODE4 = 0;
    }
    else
    {
        TRISEbits.TRISE4 = 1;
        ANSEbits.ANSE4 = 0;
        ODCEbits.ODE4 = 0;
    }
    if(pp == IOPP_PP)
    {
        //IO_GD_EN_PullUp;
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
        LATEbits.LATE4 = 1;
    }
    else
    {
        LATEbits.LATE4 = 0;
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
void ZDUart_Init(pUartMode mode, void(*fun)(unsigned char))
{
    //Uart2Config(mode,IrqServerDefaultRS,fun);
}

/**
 * 使能直读头串口
 * 
 * @author xsc (2014/11/21)
 */
void ZDUart_RX_En(void)
{
    ;//XXXX;//Uart3ReceiveEnable();
}

/**
 * 关闭直读头串口
 * 
 * @author xsc (2014/11/21)
 */
void ZDUart_RX_Ds(void)
{
    ;//XXXX;//Uart3ReceiveDisable();
}
#endif
/*
* 说明：串口插拔中断初始化函数,初始化完成使能中断
* fun:中断回调函数，包括插入中断和拔出中断
*/
//RA10//CN42
extern IRQServerFT IRQServerCN42;
void ParaSet_Interrupt_Init(void(*fun)(void))
{
    TRISAbits.TRISA10 = 1;
    //ANSAbits.ANSA10 = 0;
    ODCAbits.ODA10 = 0;
    InputChange_Clear_Intr_Status_Bit;
    ConfigIntCN(INT_ENABLE | INT_PRI_4);//XXXX;
    CNEN3bits.CN42IE = 1;
    IRQServerCN42 = fun;
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
    return (PORTAbits.RA10);//XXXX
}

//------------------------------------------------------------------------
//超声波光电光电初始化及电源控制
//------------------------------------------------------------------------
void Csb_Irda_Init()
{
    TRISFbits.TRISF7 = 0;//RF7
    ODCFbits.ODF7 = 0;
}
void Csb_Irda_EN(unsigned char fun)
{
    if (fun == 1)
    {
        LATFbits.LATF7 = 1;
    }
    else
    {
        LATFbits.LATF7 = 0;
    }
}
//------------------------------------------------------------------------
//超声波液晶初始化及控制
//------------------------------------------------------------------------
void LCD_SCL_OUT_Dir()
{
    TRISEbits.TRISE6 = 0;//RE6
    ODCEbits.ODE6 = 0;
}
void LCD_SCL(unsigned char fun)
{
    if (fun == 1)
    {
        LATEbits.LATE6 = 1;
    }
    else
    {
        LATEbits.LATE6 = 0;
    }
}
void LCD_SDA_OUT_Dir()
{
    TRISEbits.TRISE5 = 0;//RE5
    ODCEbits.ODE5 = 0;
}
void LCD_SDA_IN_Dir()
{
    TRISEbits.TRISE5 = 1;//RE5
}
void LCD_SDA(unsigned char fun)
{
    if (fun == 1)
    {
        LATEbits.LATE5 = 1;
    }
    else
    {
        LATEbits.LATE5 = 0;
    }
}
unsigned char LCD_SDA_InPut()
{
    return PORTEbits.RE5;
}
//光电中断
extern IRQServerFT IRQServerCN71;
void Csb_Irda_Int_Config(void(*p)(void))//完成
{
    TRISFbits.TRISF3 = 1;
    //ODCFbits.ODF3 = 0;
    InputChange_Clear_Intr_Status_Bit;
    ConfigIntCN(INT_ENABLE | INT_PRI_4);//XXXX;//优先级
    //CNPD5bits.CN71PDE = 1;
    CNEN5bits.CN71IE = 1;
    IRQServerCN71 = p;//中断服务函数;//优先级
}
void CSB_Irda_On()
{
    TRISFbits.TRISF3 = 1;
    //ODCFbits.ODF3 = 0;
    InputChange_Clear_Intr_Status_Bit;
    ConfigIntCN(INT_ENABLE | INT_PRI_4);//XXXX;//优先级
    //CNPD5bits.CN71PDE = 1;
    CNEN5bits.CN71IE = 1;
}
void CSB_Irda_Off()
{
    CNEN5bits.CN71IE = 0;
}

//串口中断
extern IRQServerFT IRQServerCN43;
void Csb_Uart_Int_Config(void(*p)(void))//完成
{
    TRISAbits.TRISA14 = 1;
    //ANSAbits.ANSA14 = 0;
    //ODCAbits.ODA14 = 0;
    InputChange_Clear_Intr_Status_Bit;
    ConfigIntCN(INT_ENABLE | INT_PRI_4);//XXXX;
    //CNPD3bits.CN43PDE = 1;
    CNEN3bits.CN43IE = 1;
    IRQServerCN43 = p;
}
void CSB_Uart_On()
{
    TRISAbits.TRISA14 = 1;
    InputChange_Clear_Intr_Status_Bit;
    ConfigIntCN(INT_ENABLE | INT_PRI_4);//XXXX;
    CNEN3bits.CN43IE = 1;
}
void CSB_Uart_Off()
{
    CNEN3bits.CN43IE = 0;
}


#if 0
/*
*说明：阀门到位检测电源脚设置成输出，并输出高
* RG2
*/
void ValveChkPower_Set(void)
{
    TRISGbits.TRISG2 = 0;
    //ANSGbits.ANSG2 = 0;
    ODCGbits.ODG2 = 0;
    PORTGbits.RG2 = 1;
}
/*
*说明：阀门到位检测电源脚设置成输出，并输出低
* RG2
*/
void ValveChkPower_Reset(void)
{
    TRISGbits.TRISG2 = 0;
    //ANSGbits.ANSG2 = 0;
    ODCGbits.ODG2 = 0;
    PORTGbits.RG2 = 0;
}
/*
*说明：阀门到位检测1设置成输入
* RG3
*/
void ValveChk1_Dir_In(void)
{
    TRISGbits.TRISG3 = 1;
    //ANSGbits.ANSG3 = 0;
    ODCGbits.ODG3 = 0;
    PORTGbits.RG3 = 0;
}
/*
*说明：阀门到位检测1脚设置成输出，并输出高
* RG3
*/
void ValveChk1_Set(void)
{
    TRISGbits.TRISG3 = 0;
    //ANSGbits.ANSG3 = 0;
    ODCGbits.ODG3 = 0;
    PORTGbits.RG3 = 1;
}
/*
*说明：阀门到位检测1脚设置成输出，并输出低
* RG3
*/
void ValveChk1_Reset(void)
{
    TRISGbits.TRISG3 = 0;
    //ANSGbits.ANSG3 = 0;
    ODCGbits.ODG3 = 0;
    PORTGbits.RG3 = 0;
}
/*
*说明：阀门到位检测2设置成输入
* RF6
*/
void ValveChk2_Dir_In(void)
{
    TRISFbits.TRISF6 = 1;
    //ANSFbits.ANSF6 = 0;
    ODCFbits.ODF6 = 0;
    PORTFbits.RF6 = 0;
}
/*
*说明：阀门到位检测2脚设置成输出，并输出高
* RF6
*/
void ValveChk2_Set(void)
{
    TRISFbits.TRISF6 = 0;
    //ANSFbits.ANSF6 = 0;
    ODCFbits.ODF6 = 0;
    PORTFbits.RF6 = 1;
}
/*
*说明：阀门到位检测2脚设置成输出，并输出低
* RF6
*/
void ValveChk2_Reset(void)
{
    TRISFbits.TRISF6 = 0;
    //ANSFbits.ANSF6 = 0;
    ODCFbits.ODF6 = 0;
    PORTFbits.RF6 = 0;
}
#endif
//------------------------------------------------------------------------
//------------------------------------------------------------------------
//林扬宣林扬宣林扬宣林扬宣林扬宣林扬宣林扬宣林扬宣林扬宣林扬宣林扬宣林扬宣
//------------------------------------------------------------------------
//------------------------------------------------------------------------
//typedef void (*ISR_FUNC_PTR)(void);
//type: 0 主时钟       1ms
//type: 1 低频时钟     100ms
//len: 0 1ms
//len: 1 100ms
//len: 2 1000ms
static char IntTimeLen = 0;
void User_TimerCfg(unsigned char type, unsigned char len, IRQServerFT CallBack)
{
    if (type == 0)
        MCU_TimerCfg(PortVmNum2, TimeSpMs1, CallBack);//MCU_TimerCfg(PortVmNum8, TimeSpMs100, CallBack);
    else
    {
        IntTimeLen = len;
        if (len == 1)
            MCU_TimerCfg(PortVmNum1, TimeSpMs100, CallBack);
        else
            MCU_TimerCfg(PortVmNum1, TimeSpMs1000, CallBack);
    }
}
//type: 0 主时钟
//type: 1 低频时钟
void User_TimerStart(unsigned char type)
{
    if (type == 0)
        MCU_TimerStart(PortVmNum2);//MCU_TimerStart(PortVmNum8);
    else
        MCU_TimerStart(PortVmNum1);
}
//type: 0 主时钟
//type: 1 低频时钟
void User_TimerStop(unsigned char type)
{
    if (type == 0)
        MCU_TimerStop(PortVmNum2);//MCU_TimerStop(PortVmNum8);
    else
        MCU_TimerStop(PortVmNum1);
}
//extern IRQServerFT IRQServerTM1[3];
//void RegOneSecIRQFun(IRQServerFT TimeServer)
//{
//    int i;
//    for (i = 0; i < 3; i++)
//    {
//        if (IRQServerTM1[i] == IrqServerDefault)
//        {
//            IRQServerTM1[i] = TimeServer;
//            break;
//        }
//    }
//}
