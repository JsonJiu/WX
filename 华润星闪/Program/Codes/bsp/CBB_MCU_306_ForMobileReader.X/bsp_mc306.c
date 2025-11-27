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
int CONFIG2 __attribute__((space(prog), address(0x157FC))) = 0xF9FF;//0xFA1E;0xF81F
int CONFIG1 __attribute__((space(prog), address(0x157FE))) = 0x399F;
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
extern IRQServerFT IRQServerINT1;
extern IRQServerFT IRQServerINT2;
extern IRQServerFT IRQServerINT3;
extern IRQServerFT IRQServerINT4;

extern void IrqServerDefaultRS(unsigned char Data);
extern void IrqServerDefault(void);

extern IRQServerFT IRQServerCN9;
extern IRQServerFT IRQServerCN10;

#define OUT 0

//WIFI_SleepRQ 输出控制//RF6
void WiFi_SleepRQ_Set(void)
{
    LATFbits.LATF6 = 1;
}
void WiFi_SleepRQ_Clr(void)
{
    LATFbits.LATF6 = 0;
}
//WIFI_SleepRQ 端口方向初始化  OUT-输出，IN-输入//RF6
void WiFi_SleepRQ_Init(unsigned char mode)
{
    if (mode == OUT)
    {
        TRISFbits.TRISF6 = 0;
    }
    else
    {
        TRISFbits.TRISF6 = 1;
    }
    ODCFbits.ODF6 = 0;
}
//WIFI_Reload 输出控制//RB13
void WiFi_Reload_Set(void)
{
    LATBbits.LATB13 = 1;
}
void WiFi_Reload_Clr(void)
{
    LATBbits.LATB13 = 0;
}
//WIFI_Reload 端口方向初始化  OUT-输出，IN-输入
void WiFi_Reload_Init(unsigned char mode)
{
    if (mode == OUT)
    {
        TRISBbits.TRISB13 = 0;
    }
    else
    {
        TRISBbits.TRISB13 = 1;
    }
    ANSBbits.ANSB13 = 0;
    ODCBbits.ODB13 = 0;
}
//WIFI_Reset 输出控制//RB12
void WiFi_Reset_Set(void)
{
    LATBbits.LATB12 = 1;
}
void WiFi_Reset_Clr(void)
{
    LATBbits.LATB12 = 0;
}
//WIFI_Reset 端口方向初始化  OUT-输出，IN-输入//
void WiFi_Reset_Init(unsigned char mode)
{
    if (mode == OUT)
    {
        TRISBbits.TRISB12 = 0;
    }
    else
    {
        TRISBbits.TRISB12 = 1;
    }
    ANSBbits.ANSB12 = 0;
    ODCBbits.ODB12 = 0;
}
// WIFI_Ready 端口方向初始化  OUT-输出，IN-输入//RB14
void WiFi_Ready_Init(unsigned char mode)
{
    if (mode == OUT)
    {
        TRISBbits.TRISB14 = 0;
    }
    else
    {
        TRISBbits.TRISB14 = 1;
    }
    ANSBbits.ANSB14 = 0;
    ODCBbits.ODB14 = 0;
}
//WIFI_Ready 获取输入
unsigned char WiFi_Ready_Get(void)
{
    return(unsigned char)PORTBbits.RB14;
}
//WIFI_SleepON 端口方向初始化  OUT-输出，IN-输入//RF2
void WiFi_SleepON_Init(unsigned char mode)
{
    if (mode == OUT)
    {
        TRISFbits.TRISF2 = 0;
    }
    else
    {
        TRISFbits.TRISF2 = 1;
    }
    ODCFbits.ODF2 = 0;
}
//WIFI_SleepON 获取输入
unsigned char WiFi_SleepON_Get(void)
{
    return(unsigned char)PORTFbits.RF2;
}

void WiFiUart_Init(UartMode* UartMode_ST, void(*fun)(unsigned char))
{
    TRISFbits.TRISF5 = 1;
    TRISFbits.TRISF4 = 0;
    iPPSInput(IN_FN_PPS_U2RX, IN_PIN_PPS_RP17);
    iPPSOutput(OUT_PIN_PPS_RP10, OUT_FN_PPS_U2TX);
    Uart2Config(UartMode_ST,IrqServerDefaultRS,fun);
}
void WiFiUartSend(unsigned char *buf, unsigned int len)
{
    Uart2SendData(buf, len);
}

/*----------------------------------------------------------------------------*/

//RF_SETA 输出控制//RE6
void RF_SETA_Set(void)
{
    LATEbits.LATE6 = 1;
}
void RF_SETA_Clr(void)
{
    LATEbits.LATE6 = 0;
}
//RF_SETA 端口方向初始化  OUT-输出，IN-输入
void RF_SETA_Init(unsigned char mode)
{
    if (mode == OUT)
    {
        TRISEbits.TRISE6 = 0;
    }
    else
    {
        TRISEbits.TRISE6 = 1;
    }
    ANSEbits.ANSE6 = 0;
    ODCEbits.ODE6 = 0;
}
unsigned char RF_SETA_Get(void)
{
    return(unsigned char)PORTEbits.RE6;
}
//RF_SETB 输出控制//RE7
void RF_SETB_Set(void)
{
    LATEbits.LATE7 = 1;
}
void RF_SETB_Clr(void)
{
    LATEbits.LATE7 = 0;
}
//RF_SETB 端口方向初始化  OUT-输出，IN-输入
void RF_SETB_Init(unsigned char mode)
{
    if (mode == OUT)
    {
        TRISEbits.TRISE7 = 0;
    }
    else
    {
        TRISEbits.TRISE7 = 1;
    }
    ANSEbits.ANSE7 = 0;
    ODCEbits.ODE7 = 0;
}
unsigned char RF_SETB_Get(void)
{
    return(unsigned char)PORTEbits.RE7;
}
//RF_AUX 输出控制//RG6/RP21
void RF_AUX_Set(void)
{
    LATGbits.LATG6 = 1;
}
void RF_AUX_Clr(void)
{
    LATGbits.LATG6 = 0;
}
//WRF_AUX 端口方向初始化  OUT-输出，IN-输入
void RF_AUX_Init(unsigned char mode)
{
    if (mode == OUT)
    {
        TRISGbits.TRISG6 = 0;
    }
    else
    {
        TRISGbits.TRISG6 = 1;
    }
    ANSGbits.ANSG6 = 0;
    ODCGbits.ODG6 = 0;
}
//RF_AUX 获取输入
unsigned char RF_AUX_Get(void)
{
    return(unsigned char)PORTGbits.RG6;
}
//RF_AUX 中断配置：上升沿中断//RG6/RP21
void RF_AUX_IntEn(void(*p)(void))//中断使能
{
    RPINR0bits.INT1R = 21;//rp21;
    ConfigINT1(FALLING_EDGE_INT | INT_ENABLE | INT_PRI_3);
    IRQServerINT1 = p;
}
void RF_AUX_IntDis(void)//中断失能
{
    ConfigINT1(FALLING_EDGE_INT | INT_DISABLE | INT_PRI_3);
}
//RF_POW 输出控制
void RF_POW_Set(void)
{
    LATGbits.LATG9 = 1;
}
void RF_POW_Clr(void)
{
    LATGbits.LATG9 = 0;
}
//RF_POW  端口方向初始化  OUT-输出，IN-输入
void RF_POW_Init(unsigned char mode)
{
    if (mode == OUT)
    {
        TRISGbits.TRISG9 = 0;
    }
    else
    {
        TRISGbits.TRISG9 = 1;
    }
    ANSGbits.ANSG9 = 0;
    ODCGbits.ODG9 = 0;
    
}
//RF_RXD 输出控制
void RF_RXD_Set(void)
{
    LATGbits.LATG8 = 1;
}
void RF_RXD_Clr(void)
{
    LATGbits.LATG8 = 0;
}
//RF_RXD 获取输入
unsigned char RF_RXD_Get(void)
{
    return(unsigned char)PORTGbits.RG8;
}
//RF_RXD  端口方向初始化  OUT-输出，IN-输入
void RF_RXD_Init(unsigned char mode)
{
    if (mode == OUT)
    {
        TRISGbits.TRISG8 = 0;
    }
    else
    {
        TRISGbits.TRISG8 = 1;
    }
    ANSGbits.ANSG8 = 0;
    ODCGbits.ODG8 = 0;

}
//RF_TXD 输出控制
void RF_TXD_Set(void)
{
    LATGbits.LATG7 = 1;
}
void RF_TXD_Clr(void)
{
    LATGbits.LATG7 = 0;
}
//RF_TXD 获取输入
unsigned char RF_TXD_Get(void)
{
    return(unsigned char)PORTGbits.RG7;
}
//RF_TXD  端口方向初始化  OUT-输出，IN-输入
void RF_TXD_Init(unsigned char mode)
{
    if (mode == OUT)
    {
        TRISGbits.TRISG7 = 0;
    }
    else
    {
        TRISGbits.TRISG7 = 1;
    }
    ANSGbits.ANSG7 = 0;
    ODCGbits.ODG7 = 0;

}

/*----------------------------------------------------------------------------*/
void RFUart_Init(UartMode* UartMode_ST, void(*fun)(unsigned char))
{
   ANSGbits.ANSG7 = 0;
   ANSGbits.ANSG8 = 0;
   TRISGbits.TRISG7 = 1;
   TRISGbits.TRISG8 = 0;

    iPPSInput(IN_FN_PPS_U1RX, IN_PIN_PPS_RP26);
    iPPSOutput(OUT_PIN_PPS_RP19, OUT_FN_PPS_U1TX);
    Uart1Config(UartMode_ST,IrqServerDefaultRS,fun);
}

void RFUartSend(unsigned char *buf, unsigned int len)
{
    Uart1SendData(buf, len);
}
/*----------------------------------------------------------------------------*/
//端口方向初始化  OUT-输出，IN-输入//RD7
void PowerCtrl_Init(unsigned char mode)
{
    if (mode == OUT)
    {
        TRISDbits.TRISD7 = 0;
    }
    else
    {
        TRISDbits.TRISD7 = 1;
    }
    ANSDbits.ANSD7 = 0;
    ODCDbits.ODD7 = 0;
}
// 输出控制
void PowerCtrl_Set(void)
{
    LATDbits.LATD7 = 1;
}
void PowerCtrl_Clr(void)
{
    LATDbits.LATD7 = 0;
}
////////////
void GreenLed_Init(unsigned char mode)//
{
    if (mode == OUT)
    {
        TRISDbits.TRISD6 = 0;
    }
    else
    {
        TRISDbits.TRISD6= 1;
    }
    ANSDbits.ANSD6 = 0;
    ODCDbits.ODD6 = 0;
}
void GreenLed_Set(void)
{
    LATDbits.LATD6 = 1;
}
void GreenLed_Clr(void)
{
    LATDbits.LATD6 = 0;
}
unsigned char GreenLed_Get(void)
{
    return(unsigned char)PORTDbits.RD6;
}

void RedLed_Init(unsigned char mode)
{
    if (mode == OUT)
    {
        TRISDbits.TRISD6 = 0;
    }
    else
    {
        TRISDbits.TRISD6= 1;
    }
    ANSDbits.ANSD6 = 0;
    ODCDbits.ODD6 = 0;
}
void RedLed_Set(void)
{
    LATDbits.LATD6 = 1;
}
void RedLed_Clr(void)
{
    LATDbits.LATD6 = 0;
}
unsigned char RedLed_Get(void)
{
    return(unsigned char)PORTDbits.RD6;
}

void KEY_Init(unsigned char mode)
{

}
//KEY 获取输入
unsigned char KEY_Get(void)
{

}

void DebugUARTSend(unsigned char *buf, unsigned int len)
{

}
////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void OA_ADC_Init(ADC_InitTypeDef* ADC_InitStruct)//完成AN3/RB0
{
    ADCModeST ADCMode;
    ADCMode.ADCModeOpt = ADC_InitStruct->ADCModeConversionMode;
    if (ADC_InitStruct->ADCModeConversionBit == 8)
        ADCMode.ADCModeResolution = 0;
    else if (ADC_InitStruct->ADCModeConversionBit == 10)
        ADCMode.ADCModeResolution = 1;
    else
        ADCMode.ADCModeResolution = 2;

    MCU_ADCCfg(PortVmNum3, ADCMode);
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
    ANSBbits.ANSB3 = 1;//配置为模拟
    TRISBbits.TRISB3 = 1;//配置为输入
    SetChanADC10(ADC_CH0_POS_SAMPLEA_AN3);
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
