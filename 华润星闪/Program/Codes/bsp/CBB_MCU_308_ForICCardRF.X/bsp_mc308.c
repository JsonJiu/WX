//IC卡无线远传燃气表V1.0.7_CBB.PrjPCB
#include    "bsp_mc308.h"
#include    "lib_mc308.h"
#include "p24FJ128GA308.h"
/*
 *         iPPSInput(IN_FN_PPS_INT1|IN_PIN_PPS_RP16);
        ConfigINT1(FALLING_EDGE_INT |INT_ENABLE|INT_PRI_0);//XXXX;
*/
#if 1
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
//RF2
void IM_m2m_Set_MotorSleep_GPIO(unsigned char fun)//完成
{
    TRISFbits.TRISF2 = 0;
    ODCFbits.ODF2 = 0;
    if (fun == 1)
    {
        LATFbits.LATF2 = 1;
    }
    else
    {
        LATFbits.LATF2 = 0;
    }
}
//RF7
void IM_m2m_Set_MotorOpen_GPIO(unsigned char fun)//完成
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
//RF8
void IM_m2m_Set_MotorClose_GPIO(unsigned char fun)//完成
{
    TRISFbits.TRISF8 = 0;
    ODCFbits.ODF8 = 0;
    if (fun == 1)
    {
        LATFbits.LATF8 = 1;
    }
    else
    {
        LATFbits.LATF8 = 0;
    }
}
//RD14
void BspBeepOn(void)//完成
{
    TRISDbits.TRISD14 = 0;
    ODCDbits.ODD14 = 0;
    LATDbits.LATD14 = 1;
}
void BspBeepOff(void)//完成
{
    TRISDbits.TRISD14 = 0;
    ODCDbits.ODD14 = 0;
    LATDbits.LATD14 = 0;
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
void LcdOn(void)//完成
{
    MCU_LcdOpen();
}
//关闭液晶显示
void LcdOff(void)//完成
{
    MCU_LcdClose();
}
//想底层液晶显示寄存器中写入数值
//x：寄存器地址
//reg：寄存器值
void LcdWriteReg(unsigned char x, unsigned char reg)//完成
{
    MCU_LcdWriteReg(x, reg);
}
//读取寄存器地址中的值
unsigned char LcdReadReg(unsigned char x)//完成
{
    return MCU_LcdReadReg(x);
}
//液晶底层初始化
void LcdInit(void)//完成
{
    {   //根据硬件将相应端口配置为液晶口
 
    }
    MCU_LcdCfg();
}
//------------------------------------
//霍尔霍尔霍尔霍尔霍尔霍尔霍尔霍尔霍尔
//------------------------------------
//中断回调函数
//
#define RF_ICCARD_BOARD_TYPE    0// 0表示早期版板,1表示后期版本(霍尔A,B互换)
extern IRQServerFT IRQServerCN9;
extern IRQServerFT IRQServerCN63;
extern IRQServerFT IRQServerCN64;
extern IRQServerFT IRQServerCN67;
#ifdef NEW_CBB_BOARD1
//HALLA:RG7/CN9/HALL1
//HALLB:RE6/CN64/HALL2
#else
//HALLA:RE6/CN64/HALL2
//HALLB:RE5/CN63/HALL1
#endif
void HallAInt(void);
void HallBInt(void);
//光电计数时HallA悬空设置输出低
void HallBReset(void)
{
#ifdef NEW_CBB_BOARD1
    TRISEbits.TRISE6    = 0;
    ANSEbits.ANSE6      = 0;
    ODCEbits.ODE6       = 0;
    LATEbits.LATE6      = 0;
#else
#if (RF_ICCARD_BOARD_TYPE==0)
    TRISEbits.TRISE6    = 0;
    ANSEbits.ANSE6      = 0;
    ODCEbits.ODE6       = 0;
    LATEbits.LATE6      = 0;
#else
    TRISEbits.TRISE5 = 0;
    ANSEbits.ANSE5 = 0;
    ODCEbits.ODE5 = 0;
    LATEbits.LATE5 = 0;
#endif
#endif
}
//获取霍尔io口状态
unsigned char GpioHallARead(void)//完成
{
#ifdef NEW_CBB_BOARD1
    return (PORTGbits.RG7);//XXXX
#else
#if (RF_ICCARD_BOARD_TYPE==0)
    return (PORTEbits.RE6);//XXXX
#else
    return (PORTEbits.RE5);//XXXX
#endif
#endif
}
unsigned char GpioHallBRead(void)//完成
{
#ifdef NEW_CBB_BOARD1
    return (PORTEbits.RE6);//XXXX
#else
#if (RF_ICCARD_BOARD_TYPE==0)
    return (PORTEbits.RE5);//XXXX
#else
    return (PORTEbits.RE6);//XXXX
#endif
#endif
}
//改变中断触发边沿
//0：上升沿；1：下降沿
void HallASetInt(unsigned char c,void(*p)(void))//完成
{
#ifdef NEW_CBB_BOARD1
    TRISGbits.TRISG7 = 1;
    ANSGbits.ANSG7 = 0;
    ODCGbits.ODG7 = 0;
    InputChange_Clear_Intr_Status_Bit;
    ConfigIntCN(INT_ENABLE | INT_PRI_4);//XXXX;
    CNEN1bits.CN9IE = 1;
    IRQServerCN9 = p;
#else
#if (RF_ICCARD_BOARD_TYPE==0)
    TRISEbits.TRISE6 = 1;
    ANSEbits.ANSE6 = 0;
    ODCEbits.ODE6 = 0;
    InputChange_Clear_Intr_Status_Bit;
    ConfigIntCN(INT_ENABLE | INT_PRI_4);//XXXX;
    CNEN5bits.CN64IE = 1;
    IRQServerCN64 = p;
#else
    TRISEbits.TRISE5 = 1;
    ANSEbits.ANSE5 = 0;
    ODCEbits.ODE5 = 0;
    InputChange_Clear_Intr_Status_Bit;
    ConfigIntCN(INT_ENABLE | INT_PRI_4);//XXXX;
    CNEN4bits.CN63IE = 1;
    IRQServerCN63 = p;
#endif
#endif
}
void HallBSetInt(unsigned char c,void(*p)(void))//完成
{
#ifdef NEW_CBB_BOARD1
    TRISEbits.TRISE6 = 1;
    ANSEbits.ANSE6 = 0;
    ODCEbits.ODE6 = 0;
    InputChange_Clear_Intr_Status_Bit;
    ConfigIntCN(INT_ENABLE | INT_PRI_4);//XXXX;
    CNEN5bits.CN64IE = 1;
    IRQServerCN64 = p;
#else
#if (RF_ICCARD_BOARD_TYPE==0)
    TRISEbits.TRISE5 = 1;
    ANSEbits.ANSE5 = 0;
    ODCEbits.ODE5 = 0;
    InputChange_Clear_Intr_Status_Bit;
    ConfigIntCN(INT_ENABLE | INT_PRI_4);//XXXX;
    CNEN4bits.CN63IE = 1;
    IRQServerCN63 = p;
#else
    TRISEbits.TRISE6 = 1;
    ANSEbits.ANSE6 = 0;
    ODCEbits.ODE6 = 0;
    InputChange_Clear_Intr_Status_Bit;
    ConfigIntCN(INT_ENABLE | INT_PRI_4);//XXXX;
    CNEN5bits.CN64IE = 1;
    IRQServerCN64 = p;
#endif
#endif
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
 * RST:RB11
 * CHK:RD15;CN21
 * SDA:RB13
 * SCL:RB12
 * ERR:RB10
 * POW:RF4
*/
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
//CARD_CKH设置输出低
void CardChkReset(void)
{
    TRISDbits.TRISD15   = 0;
    ODCDbits.ODD15      = 0;
    LATDbits.LATD15     = 0;
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
//CARD_ERR设置输出低
void CARDPowerCheckReset(void)
{
    TRISBbits.TRISB10   = 0;
    ANSBbits.ANSB10     = 0;
    ODCBbits.ODB10      = 0;
    LATBbits.LATB10     = 0;
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
//RF3
void IO_VBB_CHK_EN_DIR_OUT(void)
{
    TRISFbits.TRISF3 = 0;
    ODCFbits.ODF3 = 0;
}
void IO_VBB_CHK_EN_1(void)
{
    LATFbits.LATF3 = 1;
}
void IO_VBB_CHK_EN_0(void)
{
    LATFbits.LATF3 = 0;
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
//系统AN15/RB15,阀门AN14/RB14 * SDO:RB1
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
        MCU_ADCCfg(PortVmNum14, ADCMode);
    }
    else//系统
    {
        MCU_ADCCfg(PortVmNum15, ADCMode);
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
        TRISBbits.TRISB14 = 1;//配置为输入
        ANSBbits.ANSB14 = 1;//配置为模拟
        SetChanADC10(ADC_CH0_POS_SAMPLEA_AN14);
    }
    else//系统
    {
        TRISBbits.TRISB15 = 1;//配置为输入
        ANSBbits.ANSB15 = 1;//配置为模拟
        SetChanADC10(ADC_CH0_POS_SAMPLEA_AN15);
    }
}

//阀门电流检测功能取消，引脚设置数字输入
void M_I_AD_Input(void)
{
    TRISBbits.TRISB14 = 1;
    ANSBbits.ANSB14 = 1;
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
 * RFDIO:RB2
 * ACK:RE8
 * RERCV:RE9
 * SDI:RB0
 * RFCLK:RG8
 * SDO:RB1
 * RST:RB3
 * SCK:RA9
 * NSS:RA10
 * RFTX:RB8/RP8
 * RFRX:RB9/RP9
 * RSSI:RG9
 */
void OR_Rfm4463_RFDIO_DIR_Ctrl(unsigned char dir)//
{
    if(dir == 1)
    {
        TRISBbits.TRISB2 = 0;
        ANSBbits.ANSB2 = 0;//配置为模拟
        ODCBbits.ODB2 = 0;
    }
    else
    {
        TRISBbits.TRISB2 = 1;
        ANSBbits.ANSB2 = 0;//配置为模拟
        ODCBbits.ODB2 = 0;
    }
}

/**
* @declare Rfm4463无线模块射频输入输出脚输出高电平
*
* @author Administrator (2014-10-30)
*
* @requirements GPIO
*/

void OR_Rfm4463_RFDIO_Set(void)//
{
    LATBbits.LATB2 = 1;
}

/**
* @declare Rfm4463无线模块射频输入输出脚输出低电平
*
* @requirements GPIO
*
* @author Administrator (2014-10-30)
*/
void OR_Rfm4463_RFDIO_Reset(void)//
{
    LATBbits.LATB2 = 0;
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
void OR_Rfm4463_ACK_DIR_Ctrl(unsigned char dir)//
{
    if (dir == 1)
    {
        TRISEbits.TRISE8 = 0;

        ODCEbits.ODE8 = 0;
    }
    else
    {
        TRISEbits.TRISE8 = 1;

        ODCEbits.ODE8 = 0;
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
unsigned char OR_Rfm4463_ACK_Read(void)//
{
    if (PORTEbits.RE8)//XXXX
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
void OR_Rfm4463_RERCV_Dir_Ctrl(unsigned char dir)//
{
    if (dir == 1)
    {
        TRISEbits.TRISE9 = 0;
        ANSEbits.ANSE9 = 0;//配置为模拟
        ODCEbits.ODE9 = 0;
    }
    else
    {
        TRISEbits.TRISE9 = 1;
        ANSEbits.ANSE9 = 0;//配置为模拟
        ODCEbits.ODE9 = 0;
    }
}

/**
* @declare Rfm4463无线模块接收复位脚输出高电平
*
* @author Administrator (2014-10-30)
*
* @requirements GPIO
*/
void OR_Rfm4463_RERCV_Set(void)//
{
    LATEbits.LATE9 = 1;
}

/**
* @declare Rfm4463无线模块接收复位脚输出低电平
*
* @author Administrator (2014-10-30)
*
* @requirements GPIO
*/
void OR_Rfm4463_RERCV_Reset(void)//
{
    LATEbits.LATE9 = 0;
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
void OR_Rfm4463_SDI_DIR_Ctrl(unsigned char dir)//
{
    if (dir == 1)
    {
        TRISBbits.TRISB0 = 0;
        ANSBbits.ANSB0 = 0;//配置为模拟
        ODCBbits.ODB0 = 0;
    }
    else
    {
        TRISBbits.TRISB0 = 1;
        ANSBbits.ANSB0 = 0;//配置为模拟
        ODCBbits.ODB0 = 0;
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
unsigned char OR_Rfm4463_SDI_Read(void)//
{
    if (PORTBbits.RB0)//XXXX
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
void OR_Rfm4463_RFCLK_DIR_Ctrl(unsigned char dir)//
{
    if (dir == 1)
    {
        TRISGbits.TRISG8 = 0;
        ANSGbits.ANSG8 = 0;//配置为模拟
        ODCGbits.ODG8 = 0;
    }
    else
    {
        TRISGbits.TRISG8 = 1;
        ANSGbits.ANSG8 = 0;//配置为模拟
        ODCGbits.ODG8 = 0;
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
unsigned char OR_Rfm4463_RFCLK_Read(void)//
{
    if (PORTGbits.RG8)//XXXX
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
void OR_Rfm4463_SDO_DIR_Ctrl(unsigned char dir)//
{
    if (dir == 1)
    {
        TRISBbits.TRISB1 = 0;
        ANSBbits.ANSB1 = 0;//配置为模拟
        ODCBbits.ODB1 = 0;
    }
    else
    {
        TRISBbits.TRISB1 = 1;
        ANSBbits.ANSB1 = 0;//配置为模拟
        ODCBbits.ODB1 = 0;
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
    LATBbits.LATB1 = 1;
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
    LATBbits.LATB1 = 0;
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
void OR_Rfm4463_RST_DIR_Ctrl(unsigned char dir)//
{
    if (dir == 1)
    {
        TRISBbits.TRISB3 = 0;
        ANSBbits.ANSB3 = 0;//配置为模拟
        ODCBbits.ODB3 = 0;
    }
    else
    {
        TRISBbits.TRISB3 = 1;
        ANSBbits.ANSB3 = 0;//配置为模拟
        ODCBbits.ODB3 = 0;
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
    LATBbits.LATB3 = 1;
}

/**
* @declare Rfm4463无线模块复位脚输出低电平
*
* @author Administrator (2014-10-30)
*
* @requirements GPIO
*/
void OR_Rfm4463_RST_Reset(void)//
{
    LATBbits.LATB3 = 0;
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
void OR_Rfm4463_SCK_DIR_Ctrl(unsigned char dir)//
{
    if (dir == 1)
    {
        TRISAbits.TRISA9 = 0;
        ODCAbits.ODA9 = 0;
    }
    else
    {
        TRISAbits.TRISA9 = 1;
        ODCAbits.ODA9 = 0;
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
    LATAbits.LATA9 = 1;
}

/**
* @declare Rfm4463无线模块SPI时钟脚输出低电平
*
* @author Administrator (2014-10-30)
*
* @requirements GPIO
*/
void OR_Rfm4463_SCK_Reset(void)//
{
    LATAbits.LATA9 = 0;
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
void OR_Rfm4463_NSS_DIR_Ctrl(unsigned char dir)//
{
    if (dir == 1)
    {
        TRISAbits.TRISA10 = 0;
        ODCAbits.ODA10 = 0;
    }
    else
    {
        TRISAbits.TRISA10 = 1;
        ODCAbits.ODA10 = 0;
    }
}

/**
* @declare Rfm4463无线模块SPI使能脚输出高电平
*
* @author Administrator (2014-10-30)
*
* @requirements GPIO
*/
void OR_Rfm4463_NSS_Set(void)//
{
    LATAbits.LATA10 = 1;
}

/**
* @declare Rfm4463无线模块SPI使能脚输出低电平
*
* @author Administrator (2014-10-30)
*
* @requirements GPIO
*/
void OR_Rfm4463_NSS_Reset(void)//
{
    LATAbits.LATA10 = 0;
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
void OR_Rfm4463_RFTX_DIR_Ctrl(unsigned char dir)//
{
    if (dir == 1)
    {
        TRISBbits.TRISB8 = 0;
        ANSBbits.ANSB8 = 0;//配置为模拟
        ODCBbits.ODB8 = 0;
    }
    else
    {
        TRISBbits.TRISB8 = 1;
        ANSBbits.ANSB8 = 0;//配置为模拟
        ODCBbits.ODB8 = 0;
    }
}

/**
* @declare Rfm4463无线模块射频发射使能脚输出高电平
*
* @author Administrator (2014-10-30)
*
* @requirements GPIO
*/
void OR_Rfm4463_RFTX_Set(void)//
{
    LATBbits.LATB8 = 1;
}

/**
* @declare Rfm4463无线模块射频发射使能脚输出低电平
*
* @author Administrator (2014-10-30)
*
* @requirements GPIO
*/
void OR_Rfm4463_RFTX_Reset(void)//
{
    LATBbits.LATB8 = 0;
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
void OR_Rfm4463_RFRX_DIR_Ctrl(unsigned char dir)//
{
    if (dir == 1)
    {
        TRISBbits.TRISB9 = 0;
        ANSBbits.ANSB9 = 0;//配置为模拟
        ODCBbits.ODB9 = 0;
    }
    else
    {
        TRISBbits.TRISB9 = 1;
        ANSBbits.ANSB9 = 0;//配置为模拟
        ODCBbits.ODB9 = 0;
    }
}

/**
* @declare Rfm4463无线模块射频接收使能脚输出高电平
*
* @author Administrator (2014-10-30)
*
* @requirements GPIO
*/
void OR_Rfm4463_RFRX_Set(void)//
{
    LATBbits.LATB9 = 1;
}

/**
* @declare Rfm4463无线模块射频接收使能脚输出低电平
*
* @author Administrator (2014-10-30)
*
* @requirements GPIO
*/
void OR_Rfm4463_RFRX_Reset(void)//
{
    LATBbits.LATB9 = 0;
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
void OR_Rfm4463_RSSI_DIR_Ctrl(unsigned char dir)//
{
    if (dir == 1)
    {
        TRISGbits.TRISG9 = 0;
        ANSGbits.ANSG9 = 0;//配置为模拟
        ODCGbits.ODG9 = 0;
    }
    else
    {
        TRISGbits.TRISG9 = 1;
        ANSGbits.ANSG9 = 0;//配置为模拟
        ODCGbits.ODG9 = 0;
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
unsigned char OR_Rfm4463_RSSI_Read(void)//
{
    if (PORTGbits.RG9)//XXXX
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
    ANSBbits.ANSB2 = 0;
   TRISBbits.TRISB2 = 1;
    iPPSInput(IN_FN_PPS_U2RX, IN_PIN_PPS_RP13);
    Uart2Config(UartMode_ST,IrqServerDefaultRS,fun);
}
/*----------------------------------------------------------------------------*/
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
    //CloseUART1();
    DisableIntU1RX;
    /*DisableIntU1TX;
    ANSBbits.ANSB0 = 0;
    ANSBbits.ANSB1 = 0;
    TRISBbits.TRISB0 = 1;
    TRISBbits.TRISB1 = 1;*/
}
void OR_APC340F_Uart_Send(unsigned char * tx_buf, unsigned int tx_num)
{
    Uart1SendData(tx_buf, tx_num);
}

/*
 参数设置串口初始化
 * InitPara:串口设置参数
 * RecvInterrupt:接收中断回调函数
 */
void ParaSetUart_Init(UartMode* UartMode_ST, void(*fun)(unsigned char))
{
#if 1//bsp
    ANSBbits.ANSB4 = 0;
    ANSBbits.ANSB5 = 0;
    TRISBbits.TRISB4 = 1;
    TRISBbits.TRISB5 = 0;
    iPPSInput(IN_FN_PPS_U3RX, IN_PIN_PPS_RP28);
    iPPSOutput(OUT_PIN_PPS_RP18, OUT_FN_PPS_U3TX);
    Uart3Config(UartMode_ST,IrqServerDefaultRS,fun);
#else//debug
    //ANSDbits.ANSD15 = 0;
    //ANSFbits.ANSF5 = 0;
    TRISFbits.TRISF5 = 0;
    TRISDbits.TRISD15 = 1;
    iPPSInput(IN_FN_PPS_U3RX, IN_PIN_PPS_RP5);
    iPPSOutput(OUT_PIN_PPS_RP17, OUT_FN_PPS_U3TX);
    Uart3Config(UartMode_ST,IrqServerDefaultRS,fun);
#endif
}
void PartSet_Uart_Stop(void)
{
    CloseUART3();
}
unsigned char IM_m2m_GetPartSet_Status(void)//
{
    if (PORTBbits.RB7)//XXXX
        return 1;
    else
        return 0;
}
/*
 参数设置串口使能
 */
void ParaSetUart_Enable(void)
{
    Uart3SendEnable();
}
/*
 参数设置串口禁止
 */
void ParaSetUart_Disable(void)
{
    Uart3SendDisable();
}
/*
 参数设置串口发送数据
 * SendBuf:发送缓冲区指针
 * Len:发送长度
 * 返回:发送成功或者失败
 */
unsigned int ParaSetUart_SendData(unsigned char *SendBuf,unsigned short Len)
{
    Uart3SendData(SendBuf, Len);
    return TRUE;
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
void OR_Rfm4463_Uart_Cmd(FunctionalState NewState)
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
//EKY:RF5/CN18
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
    if (PORTFbits.RF5)//XXXX
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
    TRISFbits.TRISF5 = 1;
    ODCFbits.ODF5 = 0;
#ifdef USER_CN_IRQ
    InputChange_Clear_Intr_Status_Bit;
    ConfigIntCN(INT_ENABLE | INT_PRI_4);
    CNEN2bits.CN18IE = 1;
#else
    RPINR1bits.INT2R = 17;//rp17;
    ConfigINT2(FALLING_EDGE_INT | INT_ENABLE | INT_PRI_3);
    //IRQServerINT2 = p;
#endif
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
#ifdef USER_CN_IRQ
    IRQServerCN18 = fun;
#else
    IRQServerINT2 = fun;
#endif
}

void IM_OpenParm_Int(unsigned char key_type)//
{
    ANSBbits.ANSB7 = 0;
    TRISBbits.TRISB7 = 1;
    ODCBbits.ODB7 = 0;
#ifdef USER_CN_IRQ
    InputChange_Clear_Intr_Status_Bit;
    ConfigIntCN(INT_ENABLE | INT_PRI_4);
    CNEN2bits.CN25IE = 1;
#else
    RPINR1bits.INT3R = 7;//rp7;
    ConfigINT3(FALLING_EDGE_INT | INT_ENABLE | INT_PRI_3);
    //IRQServerINT3 = fun;
#endif
}
extern IRQServerFT IRQServerCN25;
void IM_m2m_Parm_Intp_Init(void(*fun)(void))//
{
#ifdef USER_CN_IRQ
    IRQServerCN25 = fun;
#else
    IRQServerINT3 = fun;
#endif
}

void IM_OpenAPC340_Int(unsigned char key_type)//
{
    TRISEbits.TRISE9 = 1;
    ANSEbits.ANSE9 = 0;
    ODCEbits.ODE9 = 0;
//#ifdef USER_CN_IRQ
    InputChange_Clear_Intr_Status_Bit;
    ConfigIntCN(INT_ENABLE | INT_PRI_4);
    CNEN5bits.CN67IE = 1;
//#else
//    RPINR2bits.INT4R = 34;//rp67;
//    ConfigINT4(FALLING_EDGE_INT | INT_ENABLE | INT_PRI_3);
//    //IRQServerINT4 = fun;
//#endif
}
extern IRQServerFT IRQServerCN67;
void IM_m2m_APC340_Intp_Init(void(*fun)(void))//
{
//#ifdef USER_CN_IRQ
    IRQServerCN67 = fun;
//#else
//    IRQServerINT4 = fun;
//#endif
}
/*==============================串口配置==============================*/
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
*             :波特率：0-1200；1-2400;2-4800;3-9600；4-19200;5-38400;6-115200
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
        ;//XXXX;//Uart0Config(&Mode,0,fun);
        break;
    case 1:
        ;//XXXX;//Uart1Config(&Mode, 0, fun);
        break;
    case 2:
        ;//XXXX;//Uart3Config(&Mode, 0, fun);
        break;
    case 3:break;
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
        //Uart0SendEnable();
        ;//XXXX;//Uart0ReceiveEnable();
        break;
    case 1:
        //Uart1SendEnable();
        ;//XXXX;//Uart1ReceiveEnable(); 
        break;
    case 2:
        //Uart3SendEnable();
        ;//XXXX;//Uart3ReceiveEnable(); 
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
        //Uart0SendDisable();
        ;//XXXX;//Uart0ReceiveDisable();
        break;
    case 1:
        //Uart1SendDisable();
        ;//XXXX;//Uart1ReceiveDisable(); 
        break;
    case 2:
        //Uart3SendDisable();
        ;//XXXX;//Uart3ReceiveDisable(); 
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

unsigned char IM_m2m_Usart_Send_Data(unsigned char com,unsigned char *buf,unsigned short len)//完成
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
        ;//XXXX;//Uart3SendData(buf, len);
        break;
    case 3:break;
    }
    return 1;
}
//
//------------------------------------------------------------------------
//------------------------------------------------------------------------
//115 115 115 115 115 115 115 115 115 115 115 115 115 115 115 115 115 115 
//------------------------------------------------------------------------
//------------------------------------------------------------------------
//prt:0表示EEPROM/RTC8025T模拟口;1表示ICCARD模拟口
//EEPROM/8025
//POW:RC12
//SDA:RA14
//SCL:RA15
//I2C电源配置输出   
void IM_IICPWR_OUT(unsigned char prt) //prt,0:1号模拟口,1:2号模拟口//
{
    if (prt)
    {

    }
    else
    {
        TRISCbits.TRISC12 = 0;
        ODCCbits.ODC12 = 0;
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
        LATCbits.LATC12 = 0;
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
        LATCbits.LATC12 = 1;
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
        TRISAbits.TRISA15 = 0;
        ODCAbits.ODA15 = 0;
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
        TRISAbits.TRISA15 = 1;
        ODCAbits.ODA15 = 0;
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
        LATAbits.LATA15 = 1;
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
        LATAbits.LATA15 = 0;
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
        TRISAbits.TRISA14 = 0;
        ODCAbits.ODA14 = 0;
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
        TRISAbits.TRISA14 = 1;
        ODCAbits.ODA14 = 0;
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
        LATAbits.LATA14 = 1;
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
        LATAbits.LATA14 = 0;
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
        return PORTAbits.RA14;//XXXX;
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
//GDEN:RE4
//GDRXD:RG7/RP26,CN9
void ZD_POW_Init(IODIR dir, IOPP pp)//完成
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
void ZD_POW_Ctrl(EnDis c)//完成
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
    ANSGbits.ANSG7 = 0;
    TRISGbits.TRISG7 = 1;
    iPPSInput(IN_FN_PPS_U4RX, IN_PIN_PPS_RP26);
    Uart4Config(mode,IrqServerDefaultRS,fun);
}

/**
 * 使能直读头串口
 * 
 * @author xsc (2014/11/21)
 */
void ZDUart_RX_En(void)
{
    Uart4ReceiveEnable();//XXXX;//Uart3ReceiveEnable();
}

/**
 * 关闭直读头串口
 * 
 * @author xsc (2014/11/21)
 */
void ZDUart_RX_Ds(void)
{
    Uart4ReceiveDisable();//XXXX;//Uart3ReceiveDisable();

    TRISGbits.TRISG7 = 0;
    ANSGbits.ANSG7 = 0;
    ODCGbits.ODG7 = 0;
    PORTGbits.RG7 = 0;
}
/*
* 说明：串口插拔中断初始化函数,初始化完成使能中断
* fun:中断回调函数，包括插入中断和拔出中断
*/
//RB7//CN25
extern IRQServerFT IRQServerCN25;
void ParaSet_Interrupt_Init(void(*fun)(void))
{
    TRISBbits.TRISB7 = 1;
    ANSBbits.ANSB7 = 0;
    ODCBbits.ODB7 = 0;
#ifdef USER_CN_IRQ
    InputChange_Clear_Intr_Status_Bit;
    ConfigIntCN(INT_ENABLE | INT_PRI_4);//XXXX;
    CNEN2bits.CN25IE = 1;
    IRQServerCN25 = fun;
#else
    RPINR1bits.INT3R = 7;//rp7;
    ConfigINT3(FALLING_EDGE_INT | INT_ENABLE | INT_PRI_3);
    IRQServerINT3 = fun;
#endif
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
    return (PORTBbits.RB7);//XXXX
}
/*
*说明：阀门到位检测电源脚设置成输出，并输出高
* RG2
*/
void ValveChkPower_Set(void)
{
#ifdef NEW_CBB_BOARD1
    TRISEbits.TRISE5 = 0;
    ANSEbits.ANSE5 = 0;
    ODCEbits.ODE5 = 0;
    //PORTEbits.RE5 = 1;
    LATEbits.LATE5 = 1;
#else
    TRISGbits.TRISG2 = 0;
    //ANSGbits.ANSG2 = 0;
    ODCGbits.ODG2 = 0;
    //PORTGbits.RG2 = 1;
    LATGbits.LATG2  = 1;
#endif
}
/*
*说明：阀门到位检测电源脚设置成输出，并输出低
* RG2
*/
void ValveChkPower_Reset(void)
{
#ifdef NEW_CBB_BOARD1
    TRISEbits.TRISE5 = 0;
    ANSEbits.ANSE5 = 0;
    ODCEbits.ODE5 = 0;
    //PORTEbits.RE5 = 0;
    LATEbits.LATE5 = 0;
#else
    TRISGbits.TRISG2 = 0;
    //ANSGbits.ANSG2 = 0;
    ODCGbits.ODG2 = 0;
    //PORTGbits.RG2 = 0;
    LATGbits.LATG2  = 0;
#endif
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
    //PORTGbits.RG3 = 0;
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
    //PORTGbits.RG3 = 1;
    LATGbits.LATG3  = 1;
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
    LATGbits.LATG3  = 0;
    //PORTGbits.RG3 = 0;
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
    //PORTFbits.RF6 = 0;
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
    //PORTFbits.RF6 = 1;
    LATFbits.LATF6  = 1;
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
    //PORTFbits.RF6 = 0;
    LATFbits.LATF6  = 0;
}

//RD0 晶振边引脚悬空 设置输出低
void NcRD0Reset(void)
{
    TRISDbits.TRISD0    = 0;
    ODCDbits.ODD0       = 0;
    LATDbits.LATD0      = 0;
}

//RC15 SEC RTCC_Test 设置输出低
void RtccTestReset(void)
{
    TRISCbits.TRISC15   = 0;
    ODCCbits.ODC15      = 0;
    LATCbits.LATC15     = 0;
}

//RE7 119 设置输出低
void P119Reset(void)
{
    TRISEbits.TRISE7    = 0;
    ANSEbits.ANSE7      = 0;
    ODCEbits.ODE7       = 0;
    LATEbits.LATE7      = 0;
}

//RB6 T_get 设置输出低
void TgetReset(void)
{
    TRISBbits.TRISB6    = 0;
    ANSBbits.ANSB6      = 0;
    ODCBbits.ODB6       = 0;
    LATBbits.LATB6      = 0;
}
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
