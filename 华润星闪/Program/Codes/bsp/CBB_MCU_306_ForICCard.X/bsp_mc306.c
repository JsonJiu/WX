#include    "bsp_mc306.h"
#include    "lib_mc306.h"
/*
 *         iPPSInput(IN_FN_PPS_INT1|IN_PIN_PPS_RP16);
        ConfigINT1(FALLING_EDGE_INT |INT_ENABLE|INT_PRI_0);//XXXX;
*/
#if 1
//24FJ128GA306配置字
int CONFIG4 __attribute__((space(prog), address(0x157F8))) = 0xFE3F;
#ifdef EXT_TIMER_CLOCK
int CONFIG3 __attribute__((space(prog), address(0x157FA))) = 0xFF7F;
#else
int CONFIG3 __attribute__((space(prog), address(0x157FA))) = 0xFFFF;
#endif
int CONFIG2 __attribute__((space(prog), address(0x157FC))) = 0xFA1E;//0xFA1E
int CONFIG1 __attribute__((space(prog), address(0x157FE))) = 0x399F;//0x399F;
#else
//24FJ64GA306配置字
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
//#define USER_CN_IRQ
//RF4
void IM_m2m_Set_MotorSleep_GPIO(unsigned char fun)//完成
{
    TRISFbits.TRISF4 = 0;
    ODCFbits.ODF4 = 0;
    if (fun == 1)
    {
        LATFbits.LATF4 = 1;
    }
    else
    {
        LATFbits.LATF4 = 0;
    }
}
//RF5
void IM_m2m_Set_MotorOpen_GPIO(unsigned char fun)//完成
{
    TRISFbits.TRISF5 = 0;
    ODCFbits.ODF5 = 0;
    if (fun == 1)
    {
        LATFbits.LATF5 = 1;
    }
    else
    {
        LATFbits.LATF5 = 0;
    }
}
//RF2
void IM_m2m_Set_MotorClose_GPIO(unsigned char fun)//完成
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
#ifdef NEW_CBB_BOARD1
//RE4
#else
//RB12
#endif
void BspBeepOn(void)//完成
{
#ifdef NEW_CBB_BOARD1
    TRISEbits.TRISE4 = 0;
    ANSEbits.ANSE4 = 0;
    ODCEbits.ODE4 = 0;
    LATEbits.LATE4 = 1;
#else
    TRISBbits.TRISB12 = 0;
    ANSBbits.ANSB12 = 0;
    ODCBbits.ODB12 = 0;
    LATBbits.LATB12 = 1;
#endif
}
void BspBeepOff(void)//完成
{
#ifdef NEW_CBB_BOARD1
    TRISEbits.TRISE4 = 0;
    ANSEbits.ANSE4 = 0;
    ODCEbits.ODE4 = 0;
    LATEbits.LATE4 = 0;
#else
    TRISBbits.TRISB12 = 0;
    ANSBbits.ANSB12 = 0;
    ODCBbits.ODB12 = 0;
    LATBbits.LATB12 = 0;
#endif
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
#ifdef USER_CN_IRQ
extern IRQServerFT IRQServerCN9;
extern IRQServerFT IRQServerCN10;
#else
extern IRQServerFT IRQServerINT2;
extern IRQServerFT IRQServerINT3;
#endif
//HALLA:RG8/CN10
//HALLB:RG7/CN9
void HallAInt(void);
void HallBInt(void);
//获取霍尔io口状态
unsigned char GpioHallARead(void)//完成
{
    return (PORTGbits.RG8);//XXXX
}
unsigned char GpioHallBRead(void)//完成
{
    return (PORTGbits.RG7);//XXXX
}
//改变中断触发边沿
//0：上升沿；1：下降沿
void HallASetInt(unsigned char c,void(*p)(void))//完成
{
    TRISGbits.TRISG8 = 1;
    ANSGbits.ANSG8 = 0;
    ODCGbits.ODG8 = 0;
#ifdef USER_CN_IRQ
    InputChange_Clear_Intr_Status_Bit;
    ConfigIntCN(INT_ENABLE | INT_PRI_4);//XXXX;
    CNEN1bits.CN10IE = 1;
    IRQServerCN10 = p;
#else
    RPINR1bits.INT2R = 19;//rp19;
    ConfigINT2(FALLING_EDGE_INT | INT_ENABLE | INT_PRI_3);
    IRQServerINT2 = p;//rp19
#endif
}
void HallBSetInt(unsigned char c,void(*p)(void))//完成
{
    TRISGbits.TRISG7 = 1;
    ANSGbits.ANSG7 = 0;
    ODCGbits.ODG7 = 0;
#ifdef USER_CN_IRQ
    InputChange_Clear_Intr_Status_Bit;
    ConfigIntCN(INT_ENABLE | INT_PRI_4);//XXXX;
    CNEN1bits.CN9IE = 1;
    IRQServerCN9 = p;
#else
    RPINR1bits.INT3R = 26;//rp26;
    ConfigINT3(FALLING_EDGE_INT | INT_ENABLE | INT_PRI_3);
    IRQServerINT3 = p;//rp26
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
 * RST:RB7
 * CHK:RB8;CN26
 * SDA:RB10
 * SCL:RB9
 * ERR:RB6
 * POW:RB11
*/
#ifdef USER_CN_IRQ
extern IRQServerFT IRQServerCN26;
#else
extern IRQServerFT IRQServerINT1;
#endif
//插卡中断 0：上升沿；1：下降沿
void CardIntConfig(unsigned char c,void(*p)(void))//CHK//完成
{
    TRISBbits.TRISB8 = 1;
    ANSBbits.ANSB8 = 0;
    ODCBbits.ODB8 = 0;
#ifdef USER_CN_IRQ
    InputChange_Clear_Intr_Status_Bit;
    ConfigIntCN(INT_ENABLE | INT_PRI_4);//XXXX;
    CNEN2bits.CN26IE = 1;
    IRQServerCN26 = p;
#else
    RPINR0bits.INT1R = 8;//RP8;
    ConfigINT1(FALLING_EDGE_INT | INT_ENABLE | INT_PRI_3);
    IRQServerINT1 = p;
#endif
}
unsigned char CardBslRead(void)//完成
{
    if (PORTBbits.RB8)//XXXX
        return 1;
    else
        return 0;
}
void CardSclOutput(void)//完成
{
    TRISBbits.TRISB9 = 0;
    ANSBbits.ANSB9 = 0;
    ODCBbits.ODB9 = 0;
}
void CardSclSet(void)//完成
{
    LATBbits.LATB9 = 1;
}
void CardSclClr(void)
{
    LATBbits.LATB9 = 0;
}
void CardRstOutput(void)//完成
{
    TRISBbits.TRISB7 = 0;
    ANSBbits.ANSB7 = 0;
    ODCBbits.ODB7 = 0;
}
void CardRstSet(void)//完成
{
    LATBbits.LATB7 = 1;
}
void CardRstClr(void)//完成
{
    LATBbits.LATB7 = 0;
}
void CardSdaSet(void)//完成
{
    LATBbits.LATB10 = 1;
}
void CardSdaClr(void)//完成
{
    LATBbits.LATB10 = 0;
}
void CardSdaOutput(void)//完成
{
    TRISBbits.TRISB10 = 0;
    ANSBbits.ANSB10 = 0;
    ODCBbits.ODB10 = 0;
}
void CardSdaInput(void)//完成
{
    TRISBbits.TRISB10 = 1;
    ANSBbits.ANSB10 = 0;
    ODCBbits.ODB10 = 0;
}
unsigned char CardSdaRead(void)//完成
{
    if (PORTBbits.RB10)//XXXX
        return 1;
    else
        return 0;
}
void CardPowerCheckInput(void)//完成
{
    TRISBbits.TRISB6 = 1;
    ANSBbits.ANSB6 = 0;
    ODCBbits.ODB6 = 0;
}
unsigned char CardPowerCheckRead(void)//完成
{
    if (PORTBbits.RB6)//XXXX
        return 1;
    else
        return 0;
}
void CardPowerContrlOutput(void)//完成
{
    TRISBbits.TRISB11 = 0;
    ANSBbits.ANSB11 = 0;
    ODCBbits.ODB11 = 0;
}
void CardPowerContrlSet(void)//完成
{
    LATBbits.LATB11 = 1;
}
void CardPowerContrlClr(void)//完成
{
    LATBbits.LATB11 = 0;
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
//RB13
void IO_VBB_CHK_EN_DIR_OUT(void)
{
    TRISBbits.TRISB13 = 0;
    ANSBbits.ANSB13 = 0;
    ODCBbits.ODB13 = 0;
}
void IO_VBB_CHK_EN_1(void)
{
    LATBbits.LATB13 = 1;
}
void IO_VBB_CHK_EN_0(void)
{
    LATBbits.LATB13 = 0;
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
//GATE:AN15;RB15
//SYS:AN14;RB14
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
        MCU_ADCCfg(PortVmNum15, ADCMode);
    }
    else//系统
    {
        MCU_ADCCfg(PortVmNum14, ADCMode);
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
        ANSBbits.ANSB15= 1;//配置为模拟
        TRISBbits.TRISB15 = 1;//配置为输入
        SetChanADC10(ADC_CH0_POS_SAMPLEA_AN15);
    }
    else//系统
    {
        ANSBbits.ANSB14 = 1;//配置为模拟
        TRISBbits.TRISB14 = 1;//配置为输入
        SetChanADC10(ADC_CH0_POS_SAMPLEA_AN14);
    }
}

//
//------------------------------------------------------------------------
//------------------------------------------------------------------------
//115 115 115 115 115 115 115 115 115 115 115 115 115 115 115 115 115 115 
//------------------------------------------------------------------------
//------------------------------------------------------------------------
//prt:0表示EEPROM/RTC8025T模拟口;1表示ICCARD模拟口
//EEPROM/8025
//POW:RE5
//SDA:RE6
//SCL:RG6
//I2C电源配置输出   
void IM_IICPWR_OUT(unsigned char prt) //prt,0:1号模拟口,1:2号模拟口//完成
{
    if (prt)
    {

    }
    else
    {
        TRISEbits.TRISE5 = 0;
        ANSEbits.ANSE5 = 0;
        ODCEbits.ODE5 = 0;
    }
}
//I2C电源配置上拉
void IM_IICPWR_UP(unsigned char prt) //prt,0:1号模拟口,1:2号模拟口
{
    if (prt)
    {

    }
    else
    {

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

    }
    else
    {
        LATEbits.LATE5 = 0;
    }
}
//I2C电源输出低
void IM_IICPWR_OFF(unsigned char prt) //prt,0:1号模拟口,1:2号模拟口
{
    if (prt)
    {

    }
    else
    {
        LATEbits.LATE5 = 1;
    }
}    
//prt,0:1号模拟口,1:2号模拟口
void IM_IICSCL_OUT(unsigned char prt)
{
    if (prt)
    {

    }
    else
    {
        TRISGbits.TRISG6 = 0;
        ANSGbits.ANSG6 = 0;
        ODCGbits.ODG6 = 0;
    }
}
//prt,0:1号模拟口,1:2号模拟口
void IM_IICSCL_IN(unsigned char prt)
{
    if (prt)
    {

    }
    else
    {
        TRISGbits.TRISG6 = 1;
        ANSGbits.ANSG6 = 0;
        ODCGbits.ODG6 = 0;
    }
}
//prt,0:1号模拟口,1:2号模拟口
void IM_IICSCL_UP(unsigned char prt)
{
    if (prt)
    {

    }
    else
    {

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

    }
    else
    {
        LATGbits.LATG6 = 1;
    }
}
// prt,0:1号模拟口,1:2号模拟口
void IM_IICSCL_CLR(unsigned char prt)
{
    if (prt)
    {

    }
    else
    {
        LATGbits.LATG6 = 0;
    }
}
// prt,0:1号模拟口,1:2号模拟口
void IM_IICSDA_OUT(unsigned char prt)
{
    if (prt)
    {

    }
    else
    {
        TRISEbits.TRISE6 = 0;
        ANSEbits.ANSE6 = 0;
        ODCEbits.ODE6 = 0;
    }
}
//prt,0:1号模拟口,1:2号模拟口
void IM_IICSDA_IN(unsigned char prt)
{
    if (prt)
    {

    }
    else
    {
        TRISEbits.TRISE6 = 1;
        ANSEbits.ANSE6 = 0;
        ODCEbits.ODE6 = 0;
    }
}
//prt,0:1号模拟口,1:2号模拟口
void IM_IICSDA_UP(unsigned char prt)
{
    if (prt)
    {

    }
    else
    {

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

    }
    else
    {
        LATEbits.LATE6 = 1;
    }
}
// prt,0:1号模拟口,1:2号模拟口
void IM_IICSDA_CLR(unsigned char prt)
{
    if (prt)
    {

    }
    else
    {
        LATEbits.LATE6 = 0;
    }
}
// prt,0:1号模拟口,1:2号模拟口
unsigned char IM_IICSDA_STU(unsigned char prt)
{
    if (prt)
    {
        return 1;//XXXX;
    }
    else
    {
        return (PORTEbits.RE6);//XXXX
    }
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
void UnusedPinIint(void)
{
    //M_POW
    TRISFbits.TRISF6 = 0;
    ODCFbits.ODF6 = 0;
    LATFbits.LATF6 = 0;
    //M_DETECT1
    TRISCbits.TRISC15 = 0;
    ODCCbits.ODC15 = 0;
    LATCbits.LATC15 = 0;
    //M_DETECT2
    TRISCbits.TRISC12 = 0;
    ODCCbits.ODC12 = 0;
    LATCbits.LATC12 = 0;
    //ESAM_POW   高电平断电
    TRISEbits.TRISE7 = 0;
    ANSEbits.ANSE7 = 0;
    ODCEbits.ODE7 = 0;
    LATEbits.LATE7 = 1;
    //ESAM_RST
    TRISGbits.TRISG9 = 0;
    ANSGbits.ANSG9 = 0;
    ODCGbits.ODG9 = 0;
    LATGbits.LATG9 = 0;
}


