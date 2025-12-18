//IC卡无线远传燃气表V1.0.7_CBB.PrjPCB
#include    "bsp_mc308.h"
#include    "lib_mc308.h"
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
int CONFIG3 __attribute__((space(prog), address(0x157FA))) = 0xFFFF;//0xFEFF;
#endif
int CONFIG2 __attribute__((space(prog), address(0x157FC))) = 0xF81F;//0xE21E;//0xFA1E
int CONFIG1 __attribute__((space(prog), address(0x157FE))) = 0x399F;
/*
  int CONFIG4 __attribute__((space(prog), address(0x157F8))) = 0xFE3F ;
//_CONFIG4(
//    DSWDTPS_DSWDTPS1F &  // Deep Sleep Watchdog Timer Postscale Select bits (1:68719476736 (25.7 Days))
//    DSWDTOSC_LPRC &      // DSWDT Reference Clock Select (DSWDT uses LPRC as reference clock)
//    DSBOREN_OFF &        // Deep Sleep BOR Enable bit (DSBOR Disabled)
//    DSWDTEN_OFF &        // Deep Sleep Watchdog Timer Enable (DSWDT Disabled)
//    DSSWEN_OFF           // DSEN Bit Enable (Deep Sleep operation is always disabled)
//);
 int CONFIG3 __attribute__((space(prog), address(0x157FA))) = 0xFFFF ;
//_CONFIG3(
//    WPFP_WPFP127 &       // Write Protection Flash Page Segment Boundary (Page 127 (0x1FC00))
//    VBTBOR_ON &          // VBAT BOR enable bit (VBAT BOR enabled)
//    SOSCSEL_ON &         // SOSC Selection bits (SOSC circuit selected)
//    WDTWIN_PS25_0 &      // Watch Dog Timer Window Width (Watch Dog Timer Window Width is 25 percent)
//    BOREN_ON &           // Brown-out Reset Enable (Brown-out Reset Enable)
//    WPDIS_WPDIS &        // Segment Write Protection Disable (Disabled)
//    WPCFG_WPCFGDIS &     // Write Protect Configuration Page Select (Disabled)
//    WPEND_WPENDMEM       // Segment Write Protection End Page Select (Write Protect from WPFP to the last page of memory)
//);
 int CONFIG2 __attribute__((space(prog), address(0x157FC))) = 0xF81F ;
//_CONFIG2(
//    POSCMD_NONE &        // Primary Oscillator Select (Primary Oscillator Disabled)
//    BOREN1_EN &          // BOR Override bit (BOR Enabled [When BOREN=1])
//    IOL1WAY_ON &         // IOLOCK One-Way Set Enable bit (Once set, the IOLOCK bit cannot be cleared)
//    OSCIOFCN_ON &        // OSCO Pin Configuration (OSCO/CLKO/RC15 functions as port I/O (RC15))
//    FCKSM_CSECME &       // Clock Switching and Fail-Safe Clock Monitor Configuration bits (Clock switching is enabled, Fail-Safe Clock Monitor is enabled)
//    FNOSC_FRC &          // Initial Oscillator Select (Fast RC Oscillator (FRC))
//    ALTVREF_DLT_AV_DLT_CV &// Alternate VREF/CVREF Pins Selection bit (Voltage reference input, ADC =RA9/RA10 Comparator =RA9,RA10)
//    IESO_ON              // Internal External Switchover (Enabled)
//);
 int CONFIG1 __attribute__((space(prog), address(0x157FE))) = 0x399F ;
//_CONFIG1(
//    WDTPS_PS32768 &      // Watchdog Timer Postscaler Select (1:32,768)
//    FWPSA_PR128 &        // WDT Prescaler Ratio Select (1:128)
//    FWDTEN_WDT_DIS &     // Watchdog Timer Enable (WDT disabled in hardware; SWDTEN bit disabled)
//    WINDIS_OFF &         // Windowed WDT Disable (Standard Watchdog Timer)
//    ICS_PGx3 &           // Emulator Pin Placement Select bits (Emulator functions are shared with PGEC3/PGED3)
//    LPCFG_ON &           // Low power regulator control (Enabled)
//    GWRP_OFF &           // General Segment Write Protect (Disabled)
//    GCP_OFF &            // General Segment Code Protect (Code protection is disabled)
//    JTAGEN_OFF           // JTAG Port Enable (Disabled)
//);
 */


#else
//24FJ64GA308配置字
int CONFIG4 __attribute__((space(prog), address(0xABF8))) = 0xFE3F;
#ifdef EXT_TIMER_CLOCK
int CONFIG3 __attribute__((space(prog), address(0xABFA))) = 0xFF7F;
#else
int CONFIG3 __attribute__((space(prog), address(0xABFA))) = 0xFEFF;
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
//RF6输出
void IM_m2m_Set_MotorSleep_GPIO(unsigned char fun)//完成
{
    TRISFbits.TRISF6 = 0;
    ODCFbits.ODF6 = 0;
    if (fun == 1)
    {
        LATFbits.LATF6 = 1;
    }
    else
    {
        LATFbits.LATF6 = 0;
    }
}
//RG3
void IM_m2m_Set_MotorOpen_GPIO(unsigned char fun)//完成
{
    TRISGbits.TRISG3 = 0;
    ODCGbits.ODG3 = 0;
    if (fun == 1)
    {
        LATGbits.LATG3 = 1;
    }
    else
    {
        LATGbits.LATG3 = 0;
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
//RD4
void BspBeepOn(void)//完成
{
    TRISDbits.TRISD4 = 0;
    ODCDbits.ODD4 = 0;
    LATDbits.LATD4 = 1;
}
void BspBeepOff(void)//完成
{
    TRISDbits.TRISD4 = 0;
    ODCDbits.ODD4 = 0;
    LATDbits.LATD4 = 0;
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
    //MCU_LcdOpen();
}
//关闭液晶显示
void LcdOff(void)//完成
{
    //MCU_LcdClose();
}
//想底层液晶显示寄存器中写入数值
//x：寄存器地址
//reg：寄存器值
void LcdWriteReg(unsigned char x, unsigned char reg)//完成
{
    //MCU_LcdWriteReg(x, reg);
}
//读取寄存器地址中的值
unsigned char LcdReadReg(unsigned char x)//完成
{
    return MCU_LcdReadReg(x);
}
//液晶底层初始化
//RE5:M62_LCD_SDA
//RE6:M62_LCD_SCL
void LcdInit(void)//完成
{
    {   //根据硬件将相应端口配置为液晶口
        TRISEbits.TRISE5 = 0;//M62_LCD_SDA输出低
        ODCEbits.ODE5 = 0;
        LATEbits.LATE5 = 1;
        TRISEbits.TRISE6 = 0;//M62_LCD_SCL输出低
        ODCEbits.ODE6 = 0;
        LATEbits.LATE6 = 1;
    }
    //MCU_LcdCfg();
}
//------------------------------------
//霍尔霍尔霍尔霍尔霍尔霍尔霍尔霍尔霍尔
//------------------------------------
//中断回调函数
//
//extern IRQServerFT IRQServerCN63;
//extern IRQServerFT IRQServerCN64;
//HALLA:RE6/CN64
//HALLB:RE5/CN63
void HallAInt(void);
void HallBInt(void);
//获取霍尔io口状态
unsigned char GpioHallARead(void)//完成
{
    return (PORTEbits.RE6);//XXXX
}
unsigned char GpioHallBRead(void)//完成
{
    return (PORTEbits.RE5);//XXXX
}
//改变中断触发边沿
//0：上升沿；1：下降沿
void HallASetInt(unsigned char c,void(*p)(void))//完成
{

}
void HallBSetInt(unsigned char c,void(*p)(void))//完成
{

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
//extern IRQServerFT IRQServerCN21;
//插卡中断 0：上升沿；1：下降沿
void CardIntConfig(unsigned char c,void(*p)(void))//CHK//
{

}
unsigned char CardBslRead(void)//
{
//    if (PORTDbits.RD15)//XXXX
//        return 1;
//    else
//        return 0;
}
void CardSclOutput(void)//
{
//    TRISBbits.TRISB12 = 0;
//    ANSBbits.ANSB12 = 0;
//    ODCBbits.ODB12 = 0;
}
void CardSclSet(void)//
{
//    LATBbits.LATB12 = 1;
}
void CardSclClr(void)//
{
//    LATBbits.LATB12 = 0;
}
void CardRstOutput(void)//
{
//    TRISBbits.TRISB11 = 0;
//    ANSBbits.ANSB11 = 0;
//    ODCBbits.ODB11= 0;
}
void CardRstSet(void)//
{
//    LATBbits.LATB11 = 1;
}
void CardRstClr(void)//
{
//    LATBbits.LATB11 = 0;
}
void CardSdaSet(void)//
{
//    LATBbits.LATB13 = 1;
}
void CardSdaClr(void)//
{
//    LATBbits.LATB13 = 0;
}
void CardSdaOutput(void)//
{
//    TRISBbits.TRISB13 = 0;
//    ANSBbits.ANSB13 = 0;
//    ODCBbits.ODB13 = 0;
}
void CardSdaInput(void)//
{
//    TRISBbits.TRISB13 = 1;
//    ANSBbits.ANSB13 = 0;
//    ODCBbits.ODB13 = 0;
}
unsigned char CardSdaRead(void)//
{
//    if (PORTBbits.RB13)//XXXX
//        return 1;
//    else
//        return 0;
}
void CardPowerCheckInput(void)//
{
//    TRISBbits.TRISB10 = 1;
//    ANSBbits.ANSB10 = 0;
//    ODCBbits.ODB10 = 0;
}
unsigned char CardPowerCheckRead(void)//
{
//    if (PORTBbits.RB10)//XXXX
//        return 1;
//    else
//        return 0;
}
void CardPowerContrlOutput(void)//
{
 //   TRISFbits.TRISF4 = 0;
//    ODCFbits.ODF4 = 0;
}
void CardPowerContrlSet(void)//
{
//    LATFbits.LATF4 = 1;
}
void CardPowerContrlClr(void)//
{
//    LATFbits.LATF4 = 0;
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
//RA9：CHECK_POWER 系统电源采样使能
void IO_VBB_CHK_EN_DIR_OUT(void)
{
    TRISAbits.TRISA9 = 0;
    ODCAbits.ODA9 = 0;
}
void IO_VBB_CHK_EN_1(void)
{
    LATAbits.LATA9 = 1;
}
void IO_VBB_CHK_EN_0(void)
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
//系统AN2/RB2：LVDIN,    阀门AN3/RB3 * SDO:RB1
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
//------------------------------------------------------------------------
//------------------------------------------------------------------------
//王兵王兵王兵王兵王兵王兵王兵王兵王兵王兵王兵王兵王兵王兵王兵王兵王兵王兵
//------------------------------------------------------------------------
//------------------------------------------------------------------------

/*===============================LED配置==============================*/
/**R-P1.2,G-P1.3
* LED灯方向配置:红色灯
*    配置成输出
* @author wb (2014-10-31)
*/
//RD15
void IM_m2m_RedLED_Dir_Config(void)//完成
{
    TRISDbits.TRISD15 = 0;
}
/**
* LED灯方向配置:绿色灯
*    配置成输出
* @author wb (2014-10-31)
*/
//RD14
void IM_m2m_GreenLED_Dir_Config(void)//完成
{
    TRISDbits.TRISD14 = 0;
}
/**
* LED灯IO配置:红色
*
* @author wb (2014-11-06)
*
* @param fun :0-置低；1-置高
*/
void IM_m2m_Set_RedLED_GPIO(unsigned char fun)//完成
{
    ODCDbits.ODD15 = 0;
    if (fun == 0)
    {
        LATDbits.LATD15 = 1;
    }
    else
    {
        LATDbits.LATD15 = 0;
    }
}
/**
* LED灯IO配置:绿色
*
* @author wb (2014-11-06)
*
* @param fun :0-置低；1-置高
*/
void IM_m2m_Set_GreenLED_GPIO(unsigned char fun)//完成
{
    ODCDbits.ODD14 = 0;
    if (fun == 0)
    {
        LATDbits.LATD14 = 1;
    }
    else
    {
        LATDbits.LATD14 = 0;
    }
}
/*===============================KEY配置==============================*/
//EKY:RB7/CN25
/**
* 获取编程按键的状态
*
* @author wb (2014-10-31)
*
* @return unsigned char
*         :返回0：表示按键按下；返回1：表示按键松开
*/
unsigned char IM_m2m_GetProgrameKey_Status(void)//无此按键//编程按键
{
    if (PORTBbits.RB7)//RB7/CN25
        return 1;
    else
        return 0;
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
    if (PORTFbits.RF4)//RF4/CN17
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
    if(key_type)//编程按键
    {
        TRISBbits.TRISB7 = 1;
        ANSBbits.ANSB7 = 0;//ODCBbits.ODB7 = 0;
#ifdef USER_CN_IRQ
        InputChange_Clear_Intr_Status_Bit;
        ConfigIntCN(INT_ENABLE | INT_PRI_4);
        CNEN2bits.CN25IE = 1;       
#else
        RPINR0bits.INT1R = 7;//rp7;
        ConfigINT1(FALLING_EDGE_INT | INT_ENABLE | INT_PRI_3);
        //IRQServerINT1 = p;
#endif
    }
    else//用户按键
    {
        TRISFbits.TRISF4 = 1;
        //ANSFbits.ANSF4 = 0;//ODCFbits.ODF4 = 0;
#ifdef USER_CN_IRQ
        InputChange_Clear_Intr_Status_Bit;
        ConfigIntCN(INT_ENABLE | INT_PRI_4);
        CNEN2bits.CN17IE = 1;
#else
        RPINR1bits.INT2R = 10;//rp10;
        ConfigINT2(FALLING_EDGE_INT | INT_ENABLE | INT_PRI_3);
        //IRQServerINT2 = p;
#endif
    }
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
    //if(key_type)
    //      CNEN2bits.CN25IE = 0;//编程按键
    //else  CNEN2bits.CN17IE = 0;//用户按键
}
/**
* 中断初始化:编程按键
*    配置成中断接口，，下降沿触发，不带上拉电阻；
* @author wb (2014-10-31)
*
* @param fun 中断服务函数指针：fun由驱动编写，由BSP调用
*/
extern IRQServerFT IRQServerCN25;
void IM_m2m_ProgrameKey_Intp_Config(void(*fun)(void))//无此按键//
{
#ifdef USER_CN_IRQ
    IRQServerCN25 = fun;//少驱动
#else
    IRQServerINT1 = fun;
#endif
}
/**
* 中断初始化：用户按键
*    配置成中断接口，，下降沿触发，不带上拉电阻；
* @author wb (2014-10-31)
*
* @param fun 中断服务函数指针：fun由驱动编写，由BSP调用
*/
extern IRQServerFT IRQServerCN17;
void IM_m2m_UserKey_Intp_Config(void(*fun)(void))//
{
#ifdef USER_CN_IRQ
    IRQServerCN17 = fun;//少驱动
#else
    IRQServerINT2 = fun;
#endif
}
/*==============================电池掉电检测==============================*/
//VIN_INT---RB0
void IM_BatPwrDown_OpenExt_Int(void)//
{
    TRISBbits.TRISB0 = 1;
    ANSBbits.ANSB0 = 0;
    InputChange_Clear_Intr_Status_Bit;
    ConfigIntCN(INT_ENABLE | INT_PRI_4);
    CNEN1bits.CN2IE = 1;
}
//中断入口函数
extern IRQServerFT IRQServerCN2;
void IM_BatPwrDown_Intp_Config(void(*fun)(void))//无此按键//
{
    IRQServerCN2 = fun;//少驱动
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
* @param com :串口端口:2-计量;0-usb;1-gprs;3-光电
* @param fun :1-置高；0-置低
*/
void IM_m2m_Set_Usart_TX_GPIO(unsigned char com, unsigned char fun)
{
    switch (com)
    {
    default:
    case 0://USB-TX
        if (fun == 1)
        {
            TRISBbits.TRISB8 = 0;//RB8
            ODCBbits.ODB8 = 0;
            LATBbits.LATB8 = 1;
        }
        else
        {
            TRISBbits.TRISB8 = 0;//RB8
            ODCBbits.ODB8 = 0;
            LATBbits.LATB8 = 0;
        }
        break;
    case 1://gprs-TX
        if (fun == 1)
        {
            TRISDbits.TRISD2 = 0;//RD2
            ODCDbits.ODD2 = 0;
            LATDbits.LATD2 = 1;
        }
        else
        {
            TRISDbits.TRISD2 = 0;//RD2
            ODCDbits.ODD2 = 0;
            LATDbits.LATD2 = 0;
        }
        break;
    case 2://计量-TX
        if (fun == 1)
        {
            TRISDbits.TRISD8 = 0;//RD8
            ODCDbits.ODD8 = 0;
            LATDbits.LATD8 = 1;
        }
        else
        {
            TRISDbits.TRISD8 = 0;//RD8
            ODCDbits.ODD8 = 0;
            LATDbits.LATD8 = 0;
        }
        break;
    case 3://光电-TX
        if (fun == 1)
        {
            TRISFbits.TRISF8 = 0;//RF8
            ODCFbits.ODF8 = 0;
            LATFbits.LATF8 = 1;
        }
        else
        {
            TRISFbits.TRISF8 = 0;//RF8
            ODCFbits.ODF8 = 0;
            LATFbits.LATF8 = 0;
        }
        break;
    }
}
/**
*配置USART 接收IO:配置为输入
*
* @author wb (2014-11-06)
*
* @param com :串口端口:2-计量;0-usb;1-gprs;3-光电
*/
void IM_m2m_Config_Usart_RX_DIR(unsigned char com)
{
    switch (com)
    {
    default:
    case 0://USB-RX
        TRISBbits.TRISB9 = 1;//RB9
        break;
    case 1://GPRS-RX
        TRISDbits.TRISD1 = 1;//RD1
        break;
    case 2://计量-RX
        TRISDbits.TRISD9 = 1;//RD9
        break;
    case 3://光电-RX
        TRISFbits.TRISF2 = 1;//RF2
        break;
    }
}
/**
* 串口配置：UART ；1 
* 位起始位，1位停止位，8位数据位，无校验 
* 全部都使能接收中断 
* @author wb (2014-11-06)
*
* @param com :串口端口:2-计量;0-usb;1-gprs;3-光电
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
    Mode.Parity = PARITY_NONE;
    Mode.StopBit = STOP_1;
    Mode.DataBit = DATA_8;
    //Mode.Sequence = LSB;
    switch (com)
    {
    default:
    case 0:
       ANSBbits.ANSB8 = 0;
       ANSBbits.ANSB9 = 0;
       TRISBbits.TRISB8 = 0;
       TRISBbits.TRISB9 = 1;
        iPPSInput(IN_FN_PPS_U1RX, IN_PIN_PPS_RP9);
        iPPSOutput(OUT_PIN_PPS_RP8, OUT_FN_PPS_U1TX);
        Uart1Config(&Mode,IrqServerDefaultRS,fun);
        break;
    case 1:
       TRISDbits.TRISD2 = 1;
       TRISDbits.TRISD1 = 0;

        iPPSInput(IN_FN_PPS_U2RX, IN_PIN_PPS_RP23);
        iPPSOutput(OUT_PIN_PPS_RP24, OUT_FN_PPS_U2TX);
        Uart2Config(&Mode,IrqServerDefaultRS,fun);
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
* @param com ：串口端口:2-空;0-usb;1-gprs;3-光电
*/
void IM_m2m_Open_Usart(unsigned char com)
{
    switch (com)
    {
    default:
    case 0:
        Uart1SendEnable();
        Uart1ReceiveEnable();
        break;
    case 1:
        Uart2SendEnable();
        Uart2ReceiveEnable();
        break;
    case 2:
        Uart3SendEnable();
        Uart3ReceiveEnable();
        break;
    case 3:
        Uart4SendEnable();
        Uart4ReceiveEnable();
        break;
    }
}
/**
* 串口配置：关闭串口
*
* @author wb (2014-11-06)
*
* @param com :串口端口:2-空;0-usb;1-gprs;3-光电
*/
void IM_m2m_Close_Usart(unsigned char com)
{
    switch (com)
    {
    default:
    case 0:
        Uart1SendDisable();
        Uart1ReceiveDisable();
        break;
    case 1:
        Uart2SendDisable();
        Uart2ReceiveDisable();
        break;
    case 2:
        Uart3SendDisable();
        Uart3ReceiveDisable();
        break;
    case 3:
        Uart4SendDisable();
        Uart4ReceiveDisable();
        break;
    }
}

/**
* 串口发送数据
* 
* @author wb (2014-11-19)
* 
* @param com ：串口端口:2-空;0-usb;1-gprs;3-光电
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
        Uart1SendData(buf,len);
        break;
    case 1:
        Uart2SendData(buf, len);
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
///*===============================gprs模块配置==============================*/
//
///**
// * GSM模块电源管脚P4.1
// * 
// * @author wb (2014-11-06)
// * 
// * @param fun :0-置低；1-置高
// */
//GSM_POWER---RG9
void IM_m2m_Set_GSM_Modem_Power_GPIO(unsigned char fun)
{
    if (fun == 1)
    {
        TRISGbits.TRISG9 = 0;//RG9
        ODCGbits.ODG9 = 0;
        LATGbits.LATG9 = 1;
    }
    else
    {
        TRISGbits.TRISG9 = 0;//RG9
        ODCGbits.ODG9 = 0;
        LATGbits.LATG9 = 0;
    }
}
// 
///**
// * GSM模块开关机管脚配置P4.4
// * 
// * @author wb (2014-11-06)
// * 
// * @param fun 
// */
//GSM_PWR---RD11
void IM_m2m_Set_GSM_Modem_PowerOnOff_GPIO(unsigned char fun)
{
    if (fun == 1)
    {
        TRISDbits.TRISD11 = 0;//RD11
        ODCDbits.ODD11 = 0;
        LATDbits.LATD11 = 1;
    }
    else
    {
        TRISDbits.TRISD11 = 0;//RD11
        ODCDbits.ODD11 = 0;
        LATDbits.LATD11 = 0;
    }
}
//GSM_DTR---RD0
void IM_m2m_Set_GSM_Modem_DTR_GPIO(unsigned char fun)
{
    if (fun == 1)
    {
        TRISDbits.TRISD0 = 0;//RD0
        ODCDbits.ODD0 = 0;
        LATDbits.LATD0 = 1;
    }
    else
    {
        TRISDbits.TRISD0 = 0;//RD0
        ODCDbits.ODD0 = 0;
        LATDbits.LATD0 = 0;
    }
}
//GSM_EMERG_OFF---RD10
void IM_m2m_Set_GSM_Modem_EMERG_OFF_GPIO(unsigned char fun)
{
    if (fun == 1)
    {
        TRISDbits.TRISD10 = 0;//RD10
        ODCDbits.ODD10 = 0;
        LATDbits.LATD10 = 1;
    }
    else
    {
        TRISDbits.TRISD10 = 0;//RD10
        ODCDbits.ODD10 = 0;
        LATDbits.LATD10 = 0;
    }
}
///**
// * 设置USB串口电源脚方向
// * 为输入
// * @author wb (2014-11-06)
// *
// * @param fun：
// */
//SET
void IM_SetIO_InDir(void)
{
    TRISAbits.TRISA10 = 1;//RA10
    ANSAbits.ANSA10 = 0;
}

///**
// * 获取USB串口电源脚输入值
// * 
// * @author wb (2014-11-06)
// *
// * @param fun：1：为高；0为低
// */
unsigned char Get_SetIO_Status(void)
{
    if (PORTAbits.RA10)//RA10
        return 1;
    else
        return 0;
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
    TRISBbits.TRISB1 = 0;//RB1---EEPROM_POWER
    ANSBbits.ANSB1 = 0;
    ODCBbits.ODB1 = 0;
}
//I2C电源配置上拉
void IM_IICPWR_UP(unsigned char prt) //prt,0:1号模拟口,1:2号模拟口//
{

}
//I2C电源配置下拉
void IM_IICPWR_DOWN(unsigned char prt) //prt,0:1号模拟口,1:2号模拟口//
{

}
//I2C电源输出高
void IM_IICPWR_ON(unsigned char prt) //prt,0:1号模拟口,1:2号模拟口//
{
    LATBbits.LATB1 = 0;//RB1
}
//I2C电源输出低
void IM_IICPWR_OFF(unsigned char prt) //prt,0:1号模拟口,1:2号模拟口//
{
    LATBbits.LATB1 = 1;//RB1
}
//prt,0:1号模拟口,1:2号模拟口   输出
void IM_IICSCL_OUT(unsigned char prt)//
{
    TRISCbits.TRISC1 = 0;//RC1
    ODCCbits.ODC1 = 0;
}
//prt,0:1号模拟口,1:2号模拟口   输入
void IM_IICSCL_IN(unsigned char prt)//
{
    TRISCbits.TRISC1 = 1;//RC1
}
//prt,0:1号模拟口,1:2号模拟口
void IM_IICSCL_UP(unsigned char prt)//
{

}
// prt,0:1号模拟口,1:2号模拟口
void IM_IICSCL_DOWN(unsigned char prt)//
{

}
// prt,0:1号模拟口,1:2号模拟口
void IM_IICSCL_SET(unsigned char prt)//
{
    LATCbits.LATC1 = 1;
}
// prt,0:1号模拟口,1:2号模拟口
void IM_IICSCL_CLR(unsigned char prt)//
{
    LATCbits.LATC1 = 0;
}
// prt,0:1号模拟口,1:2号模拟口
void IM_IICSDA_OUT(unsigned char prt)//
{
    TRISCbits.TRISC3 = 0;//RC3
    ODCCbits.ODC3 = 0;
}
//prt,0:1号模拟口,1:2号模拟口
void IM_IICSDA_IN(unsigned char prt)//
{
    TRISCbits.TRISC3 = 1;//RC3
}
//prt,0:1号模拟口,1:2号模拟口
void IM_IICSDA_UP(unsigned char prt)//
{

}
// prt,0:1号模拟口,1:2号模拟口
void IM_IICSDA_DOWN(unsigned char prt)//
{

}
// prt,0:1号模拟口,1:2号模拟口
void IM_IICSDA_SET(unsigned char prt)//
{
    LATCbits.LATC3 = 1;
}
// prt,0:1号模拟口,1:2号模拟口
void IM_IICSDA_CLR(unsigned char prt)//
{
    LATCbits.LATC3 = 0;
}
// prt,0:1号模拟口,1:2号模拟口
unsigned char IM_IICSDA_STU(unsigned char prt)//
{
    return PORTCbits.RC3;//RC3
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
//        TRISEbits.TRISE4 = 0;
//        ANSEbits.ANSE4 = 0;
//        ODCEbits.ODE4 = 0;
    }
    else
    {
//        TRISEbits.TRISE4 = 1;
//        ANSEbits.ANSE4 = 0;
//        ODCEbits.ODE4 = 0;
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
 //       LATEbits.LATE4 = 1;
    }
    else
    {
  //      LATEbits.LATE4 = 0;
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
//------------------------------------------------------------------------
//计量电源控制
//输出
//fun ： 1为高电平，0为低电平
//------------------------------------------------------------------------
//CSB_LOW_POW
void Set_JLlow_Out(unsigned char fun)
{
    if (fun == 1)
    {
        TRISAbits.TRISA15 = 0;//RA15
        ODCAbits.ODA15 = 0;
        LATAbits.LATA15 = 1;
    }
    else
    {
        TRISAbits.TRISA15 = 0;//RA15
        ODCAbits.ODA15 = 0;
        LATAbits.LATA15 = 0;
    }
}

//------------------------------------------------------------------------
//光电光电电源控制
//输出
//fun ： 1为高电平，0为低电平
//------------------------------------------------------------------------
//CSB_IRDA_EN
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
//LCD_001D,液晶SCL
//输出
//fun ： 1为高电平，0为低电平
//------------------------------------------------------------------------
//CSB_LCD_SCL
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
//------------------------------------------------------------------------
//LCD_001D,液晶SDA
//输出
//fun ： 1为高电平，0为低电平
//------------------------------------------------------------------------
//CSB_LCD_SDA
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
//0：上升沿；1：下降沿
extern IRQServerFT IRQServerCN71;
void Csb_Irda_Int_Config(void(*p)(void))//完成
{
    TRISFbits.TRISF3 = 1;
    //ODCFbits.ODF3 = 0;
#ifdef USER_CN_IRQ
    InputChange_Clear_Intr_Status_Bit;
    ConfigIntCN(INT_ENABLE | INT_PRI_4);//XXXX;//优先级
    //CNPD5bits.CN71PDE = 1;
    CNEN5bits.CN71IE = 1;
    IRQServerCN71 = p;//中断服务函数;//优先级
#else
    RPINR1bits.INT3R = 16;//rp16;
    ConfigINT3(FALLING_EDGE_INT | INT_ENABLE | INT_PRI_3);
    IRQServerINT3 = p;
#endif
}
void CSB_Irda_On()
{
    TRISFbits.TRISF3 = 1;
    //ODCFbits.ODF3 = 0;
#ifdef USER_CN_IRQ
    InputChange_Clear_Intr_Status_Bit;
    ConfigIntCN(INT_ENABLE | INT_PRI_4);//XXXX;//优先级
    //CNPD5bits.CN71PDE = 1;
    CNEN5bits.CN71IE = 1;
#else
    ConfigINT3(FALLING_EDGE_INT | INT_ENABLE | INT_PRI_3);
#endif
}
void CSB_Irda_Off()
{
#ifdef USER_CN_IRQ
    CNEN5bits.CN71IE = 0;
#else
    ConfigINT3(FALLING_EDGE_INT | INT_DISABLE | INT_PRI_3);
#endif
}

//串口中断
//0：上升沿；1：下降沿
extern IRQServerFT IRQServerCN43;
void Csb_Uart_Int_Config(void(*p)(void))//完成
{
    TRISAbits.TRISA14 = 1;
    //ANSAbits.ANSA14 = 0;
    //ODCAbits.ODA14 = 0;
#ifdef USER_CN_IRQ
    InputChange_Clear_Intr_Status_Bit;
    ConfigIntCN(INT_ENABLE | INT_PRI_4);//XXXX;
    //CNPD3bits.CN43PDE = 1;
    CNEN3bits.CN43IE = 1;
    IRQServerCN43 = p;
#else
    RPINR2bits.INT4R = 36;//rpI36;
    ConfigINT4(FALLING_EDGE_INT | INT_ENABLE | INT_PRI_3);
    IRQServerINT4 = p;
#endif
}
void CSB_Uart_On()
{
    TRISAbits.TRISA14 = 1;
#ifdef USER_CN_IRQ
    InputChange_Clear_Intr_Status_Bit;
    ConfigIntCN(INT_ENABLE | INT_PRI_4);//XXXX;
    CNEN3bits.CN43IE = 1;
#else
    ConfigINT4(FALLING_EDGE_INT | INT_ENABLE | INT_PRI_3);
#endif
}
void CSB_Uart_Off()
{
#ifdef USER_CN_IRQ
    CNEN3bits.CN43IE = 0;
#else
    ConfigINT4(FALLING_EDGE_INT | INT_DISABLE | INT_PRI_3);
#endif
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

void aaaaa(void)
{
    TRISAbits.TRISA10 = 0;//RA10
    ODCAbits.ODA10 = 0;
    LATAbits.LATA10 = 1;
    LATAbits.LATA10 = 0;
    LATAbits.LATA10 = 1;
}
