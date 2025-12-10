#include "stdio.h"
//#include    "bsp_mc308.h"
#include    "lib_mc308.h"
#include "config.h"      // add by mjl

extern void IrqServerDefaultRS(unsigned char Data);
extern void IrqServerDefault(void);
extern IRQServerFT IRQServerINT1;
extern IRQServerFT IRQServerINT2;
extern IRQServerFT IRQServerINT3;
extern IRQServerFT IRQServerINT4;

/**
 *                  24FJ128GA308配置字
 */

_CONFIG4 (0xFE3F);
_CONFIG3 (0xFFFF);
_CONFIG2 (0xF81F);
_CONFIG1 (0x399F);//时间约16S，0x39DC-允许软件使能看门狗；0x39FC-强制使能看门狗； 0x399C-禁止看门狗

/*
int CONFIG4 __attribute__((space(prog), address(0x157F8))) = 0xFE3F;
#ifdef EXT_TIMER_CLOCK
int CONFIG3 __attribute__((space(prog), address(0x157FA))) = 0xFF7F;
#else
int CONFIG3 __attribute__((space(prog), address(0x157FA))) = 0xFFFF;
#endif
int CONFIG2 __attribute__((space(prog), address(0x157FC))) = 0xF81F;
int CONFIG1 __attribute__((space(prog), address(0x157FE))) = 0x399F;//0x39FE;//0x39FE(watch dog)
*/

/**********************************Timer**************************************/
/**
 * 定时器配置
 */
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

/**
 * 开启定时器
 * @param type：0 主时钟；1 低频时钟
 */
void User_TimerStart(unsigned char type)
{
    if (type == 0)
        MCU_TimerStart(PortVmNum2);//MCU_TimerStart(PortVmNum8);
    else
        MCU_TimerStart(PortVmNum1);
}

/**
 * 停止定时器
 * @param type：0 主时钟；1 低频时钟
 */
void User_TimerStop(unsigned char type)
{
    if (type == 0)
        MCU_TimerStop(PortVmNum2);//MCU_TimerStop(PortVmNum8);
    else
        MCU_TimerStop(PortVmNum1);
}


/**********************************电源管理ADC**************************************/
/**
 * 系统电源采样使能：设置方向输出
 */
void IO_VBB_CHK_EN_DIR_OUT(void)
{
//    TRISFbits.TRISF6 = 0;
//    ODCFbits.ODF6 = 0;
}
/**
 * 电源采样脚输出高
 */
void IO_VBB_CHK_EN_1(void)
{
//    LATFbits.LATF6 = 1;
}
/**
 * 电源采样脚输出低
 */
void IO_VBB_CHK_EN_0(void)
{
//    LATFbits.LATF6 = 0;
}

/**
 * ADC初始化
 * @param ADC_InitStruct
 */
void OA_ADC_Init(ADC_InitTypeDef* ADC_InitStruct)
{
    ADCModeST ADCMode;
    ADCMode.ADCModeOpt = ADC_InitStruct->ADCModeConversionMode;
    if (ADC_InitStruct->ADCModeConversionBit == 8)
        ADCMode.ADCModeResolution = 0;
    else if (ADC_InitStruct->ADCModeConversionBit == 10)
        ADCMode.ADCModeResolution = 1;
    else
        ADCMode.ADCModeResolution = 2;

    if (ADC_InitStruct->ADCModeChannel == 1)//RTC电压
    {
       // MCU_ADCCfg(PortVmNum10, ADCMode);
        MCU_ADCCfg(MASTER_RTC_ADC, ADCMode);
        
    }
    else if(ADC_InitStruct->ADCModeChannel == 0)//主控板在线
    {
        //MCU_ADCCfg(PortVmNum11, ADCMode);
        MCU_ADCCfg(MASTER_ONLINE_ADC, ADCMode);
    }
    else//主控板BAT
    {
        //MCU_ADCCfg(PortVmNum11, ADCMode);
        MCU_ADCCfg(MASTER_BAT_ADC, ADCMode);
    }
}

/**
 * 是否开启ADC
 * @param NewState: ENABLE-开启；DISABLE-不开启
 */
void OA_ADC_CMD(FunctionalState NewState)
{
    if (NewState == DISABLE)
        MCU_ADCStop(0);
    else
        MCU_ADCStart(0);
}

/**
 * 获取ADC采样转换的值
 * @return
 */
unsigned short OA_ADC_GetConversionValue(void)//完成
{
    return MCU_ADCRead();
}

/**
 * 设置ADC的转换通道
 * @param channel
 */
void OA_ADC_SetConversionChannel(unsigned char channel)//完成
{
    if (channel == 0)//检测主控板是否在线
    {
       /* TRISBbits.TRISB11 = 1;//配置为输入
        ANSBbits.ANSB11 = 1;//配置为模拟
        SetChanADC10(ADC_CH0_POS_SAMPLEA_AN11);*/

        MASTER_ONLINE_ADC_TRIS = 1;//配置为输入
         MASTER_ONLINE_ADC_ANS = 1;//配置为模拟
        SetChanADC10(MASTER_ONLINE_ADC_CHANNEL);
    }
    else if (channel == 1)//RTC电池检测
    {
       /* TRISBbits.TRISB10 = 1;//配置为输入
        ANSBbits.ANSB10 = 1;//配置为模拟
        SetChanADC10(ADC_CH0_POS_SAMPLEA_AN10);
        */

        MASTER_RTC_ADC_TRIS = 1;
        MASTER_RTC_ADC_ANS = 1;
        SetChanADC10(MASTER_RTC_ADC_CHANNEL);
    }
    else if (channel == 3)//NAT电池检测
    {
       /* TRISBbits.TRISB10 = 1;//配置为输入
        ANSBbits.ANSB10 = 1;//配置为模拟
        SetChanADC10(ADC_CH0_POS_SAMPLEA_AN10);
        */

        MASTER_BAT_ADC_TRIS = 1;
        MASTER_BAT_ADC_ANS = 1;
        SetChanADC10(MASTER_BAT_ADC_CHANNEL);
    }
}

/**
 * 串口配置
 * @param com   ：串口号
 * @param baud  ：波特率
 * @param fun   ：接收中断函数
 */
void UsartConfig(USART_TypeDef com, UartBaud baud, void(*fun)(unsigned char rxd))
{
    UartMode Mode;
    
    Mode.Baud    = baud;
    Mode.Parity  = PARITY_NONE;
    Mode.StopBit = STOP_1;
    Mode.DataBit = DATA_8;
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
    }
    switch(com)
    {
        case COM1:
            COM1_TX_IN;
//            COM1_TX_OUT;
            COM1_RX_IN;
            iPPSInput(IN_FN_PPS_U1RX, COM1_IN_PIN_PPS);
            iPPSOutput(COM1_OUT_PIN_PPS, OUT_FN_PPS_NULL);
            Uart1Config(&Mode,IrqServerDefaultRS,fun);
            break;
        case  COM2:
            COM2_TX_IN;
//            COM2_TX_OUT;
            COM2_RX_IN;
            iPPSInput(IN_FN_PPS_U2RX, COM2_IN_PIN_PPS);
            iPPSOutput(COM2_OUT_PIN_PPS, OUT_FN_PPS_NULL);
            Uart2Config(&Mode,IrqServerDefaultRS,fun);
            break;
        case COM3:
            COM3_TX_OUT;
            COM3_RX_IN;
            iPPSInput(IN_FN_PPS_U3RX, COM3_IN_PIN_PPS);
            iPPSOutput(COM3_OUT_PIN_PPS, OUT_FN_PPS_U3TX);
            
            Uart3Config(&Mode,IrqServerDefaultRS,fun);
            break;
        case COM4:
            COM4_TX_OUT;
            COM4_RX_IN;
            iPPSInput(IN_FN_PPS_U4RX, COM4_IN_PIN_PPS);
            iPPSOutput(COM4_OUT_PIN_PPS, OUT_FN_PPS_U4TX);

            Uart4Config(&Mode,IrqServerDefaultRS,fun);
            break;
        default:break;
    }
}
void UsartConfig2(USART_TypeDef com, UartBaud baud, void(*fun)(unsigned char rxd))
{
    UartMode Mode;

    Mode.Baud    = baud;
    Mode.Parity  = PARITY_EVEN;
    Mode.StopBit = STOP_1;
    Mode.DataBit = DATA_8;
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
    }
    switch(com)
    {
        case COM1:
            COM1_TX_IN;
//            COM1_TX_OUT;
            COM1_RX_IN;
            iPPSInput(IN_FN_PPS_U1RX, COM1_IN_PIN_PPS);
            iPPSOutput(COM1_OUT_PIN_PPS, OUT_FN_PPS_NULL);
            Uart1Config(&Mode,IrqServerDefaultRS,fun);
            break;
        case  COM2:
            COM2_TX_IN;
//            COM2_TX_OUT;
            COM2_RX_IN;
            iPPSInput(IN_FN_PPS_U2RX, COM2_IN_PIN_PPS);
            iPPSOutput(COM2_OUT_PIN_PPS, OUT_FN_PPS_NULL);
            Uart2Config(&Mode,IrqServerDefaultRS,fun);
            break;
        case COM3:
            COM3_TX_OUT;
            COM3_RX_IN;
            iPPSInput(IN_FN_PPS_U3RX, COM3_IN_PIN_PPS);
            iPPSOutput(COM3_OUT_PIN_PPS, OUT_FN_PPS_U3TX);

            Uart3Config(&Mode,IrqServerDefaultRS,fun);
            break;
        case COM4:
            COM4_TX_OUT;
            COM4_RX_IN;
            iPPSInput(IN_FN_PPS_U4RX, COM4_IN_PIN_PPS);
            iPPSOutput(COM4_OUT_PIN_PPS, OUT_FN_PPS_U4TX);

            Uart4Config(&Mode,IrqServerDefaultRS,fun);
            break;
        default:break;
    }
}
/**
 * 打开串口
 * @param com
 */
void OpenUsart(USART_TypeDef com)
{
    switch(com)
    {
        case COM1:
            Uart1SendEnable();
            Uart1ReceiveEnable();
            break;
        case COM2:
            Uart2SendEnable();
            Uart2ReceiveEnable();
            break;
        case COM3:
            Uart3SendEnable();
            Uart3ReceiveEnable();
            break;
        case COM4:
            Uart4SendEnable();
            Uart4ReceiveEnable();
            break;
    }
}

/**
 * 关闭串口
 * @param com：
 */
void CloseUsart(USART_TypeDef com)
{
    switch(com)
    {
        case COM1:
            Uart1SendDisable();
            Uart1ReceiveDisable();
            break;
        case COM2:
            Uart2SendDisable();
            Uart2ReceiveDisable();
            break;
        case COM3:
            Uart3SendDisable();
            Uart3ReceiveDisable();
            break;
        case COM4:
            Uart4SendDisable();
            Uart4ReceiveDisable();
            break;
    }
}

/**
 * 设置串口IO为输出低
 * @param com
 */
void SetUsartIoOut(USART_TypeDef com, unsigned char fun)
{
    switch(com)
    {
        case COM1:
//           ANSBbits.ANSB0 = 0;
           TRISFbits.TRISF2 = 0;
           ODCFbits.ODF2 = 0;
            if(fun)
            {
                LATFbits.LATF2 = 1;
            }
            else
            {
                LATFbits.LATF2 = 0;
            }
            break;
        case COM2:
            TRISDbits.TRISD15 = 0;
            ODCDbits.ODD15 = 0;
            if (fun == 1)
            {
                LATDbits.LATD15 = 1;
            }
            else
            {
                LATDbits.LATD15 = 0;
            }
            break;
        case COM3:
            TRISBbits.TRISB8 = 0;
            ODCBbits.ODB8 = 0;
            if (fun == 1)
                LATBbits.LATB8 = 1;
            else
                LATBbits.LATB8 = 0;
            break;
        case COM4:
            TRISGbits.TRISG7 = 0;
            ODCGbits.ODG7 = 0;
            if (fun == 1)
                LATGbits.LATG7 = 1;
            else
                LATGbits.LATG7 = 0;
            break;
        default:break;
    }
}

void SetUartTxDirIn(void)
{
        TRISDbits.TRISD1 = 1;
}

/**
 * 串口发送数据函数
 * @param com
 * @param buf ：发送缓冲区
 * @param len ：发送长度
 * @return
 */
unsigned char UsartSendData(USART_TypeDef com, unsigned char *buf, unsigned short len)
{
    switch(com)
    {
        case COM1:
            Uart1SendData(buf, len);
            break;
        case COM2:
            Uart2SendData(buf, len);
            break;
        case COM3:
            Uart3SendData(buf, len);
            break;
        case COM4:
            Uart4SendData(buf, len);
            break;
    }

    return 1;
}

/***********************************串口配置*********************************/
void UsartTxConfig(USART_TypeDef com, unsigned char fun)
{
    switch(com)
    {
        case COM1:
            if(fun == 0)
            {
                iPPSOutput(COM1_OUT_PIN_PPS, OUT_FN_PPS_NULL);
                COM1_TX_IN;
            }
            else
            {
                COM1_TX_OUT;
                iPPSOutput(COM1_OUT_PIN_PPS, OUT_FN_PPS_U1TX);
            }
            break;
        case  COM2:
            if(fun == 0)
            {
                iPPSOutput(COM2_OUT_PIN_PPS, OUT_FN_PPS_NULL);
                COM2_TX_IN;
            }
            else
            {
                COM2_TX_OUT;
                iPPSOutput(COM2_OUT_PIN_PPS, OUT_FN_PPS_U2TX);
            }
            break;
        case COM3:
            /*
            TRISDbits.TRISD8 = 1;
            TRISDbits.TRISD9 = 0;
            iPPSInput(IN_FN_PPS_U3RX, IN_PIN_PPS_RP2);
            iPPSOutput(OUT_PIN_PPS_RP4, OUT_FN_PPS_U3TX);
            Uart3Config(&Mode,IrqServerDefaultRS,fun);
            */
            break;
        case COM4:
            break;
        default:break;
    }
}

void SetUsbUartTXRXIN(void)
{
    TRISFbits.TRISF2 = 1;
    TRISFbits.TRISF8 = 1;
}

/**
 * 初始化设备ID
 */
void Init_DeviceID(void)
{
    P0_INIT;
    P1_INIT;
    P2_INIT;
   
}

/**
 * 获取1号ID
 * @return
 */
unsigned char GetP0Status(void)
{
    /*
    if (PORTDbits.RD8)
        return 1;
    else
        return 0;
     */
    if (P0_PORT)
        return 1;
    else
        return 0;
}
/**
 * 获取2号ID
 * @return
 */
unsigned char GetP1Status(void)
{
    /*
    if (PORTDbits.RD9)
        return 1;
    else
        return 0;
    */
    if (P1_PORT)
        return 1;
    else
        return 0;
}
/**
 * 获取3号ID
 * @return
 */
unsigned char GetP2Status(void)
{
   /*
    if (PORTDbits.RD10)
        return 1;
    else
        return 0;
    */
    if (P2_PORT)
        return 1;
    else
        return 0;
}

/**
 * 设置主控板电源电流
 * @param fun
 */
void SetMasterBoardPower(unsigned char fun)
{
   /* TRISDbits.TRISD2 = 0;
    ODCDbits.ODD2 = 0;
    if (fun == 1)
        LATDbits.LATD2 = 1;
    else
        LATDbits.LATD2 = 0;
   */
   MASTERBORDPOWER_OUT;

    if (fun == 1)
        MASTERBORDPOWER_1;
    else
        MASTERBORDPOWER_0; 
}

/**
 *  设置测试电流功耗电源
 * @param fun
 */
void SetCheckConsumptionPower(unsigned char fun)
{
    CONSUMPTIONPOWER_OUT;
    if (fun == 1)
        CONSUMPTIONPOWER_1;
    else
        CONSUMPTIONPOWER_0;
}

/**
 * 设置主控板进入低功耗
 * @param fun
 */
void Set_MasterLowConsManage(unsigned char fun)
{
    /*
    TRISFbits.TRISF3 = 0;
    ODCFbits.ODF3 = 0;
//    ANSBbits.ANSB2 = 0;
    if (fun == 1)
        LATFbits.LATF3 = 1;
    else
        LATFbits.LATF3 = 0;
     * */

    MASTER_LOWPOW_SET_OUT;
    if (fun == ENABLE)
        MASTER_LOWPOW_SET_1;
    else
        MASTER_LOWPOW_SET_0;
}

/**
 * 设置主控板在线方向
 */
void SetMasterOnLineIO_Dir(void)
{
    TRISBbits.TRISB11 = 1;
    ODCBbits.ODB11 = 0;
    ANSBbits.ANSB11 = 0;
}

/**
 * 获取表具在线IO状态
 * @return
 */
unsigned char GetMasterOnLineIOStatus(void)
{
    if (PORTBbits.RB11)
        return 1;
    else
        return 0;
}

/**
 *直读电源IO中断
 * @param p
 */
void ZDSetIntIo(void(*p)(void))
{
    TRISDbits.TRISD12 = 1;
    ODCDbits.ODD12 = 0;
    RPINR2bits.INT4R = 42;//rp26;
    ConfigINT4(FALLING_EDGE_INT | INT_ENABLE | INT_PRI_3);
    IRQServerINT4 = p;
}


void CloseZdPowerInt(void)
{
    TRISDbits.TRISD12 = 1;
    ODCDbits.ODD12 = 0;
    ConfigINT4(FALLING_EDGE_INT | INT_ENABLE | INT_PRI_3);
}

void SetIrdaPower(unsigned char fun)
{
//    TRISFbits.TRISF8 = 0;
//    ODCFbits.ODF8 = 0;
//    if (fun == 1)
//        LATFbits.LATF8 = 1;
//    else
//        LATFbits.LATF8 = 0;
}

/**********************************LED**************************************/


/**
 * 点亮LED灯
 * @param type :0-绿灯；1-红灯；2-蜂鸣器
 */
void LEDOn(LED_TypeDef type)
{
    switch(type)
    {
        case GREEN_LED:
          //  TRISEbits.TRISE5 = 0;    ODCEbits.ODE5 = 0;  LATEbits.LATE5 = 0;
             LED_OUT;
             LED_0;
            break;
        case RED_LED:
            break;
        case BEEP_LED:
            break;
        default:break;
    }
}
/**
 * 关闭LED
 * @param type：0-绿灯；1-红灯；2-蜂鸣器
 */
void LEDOff(LED_TypeDef type)
{
    switch(type)
    {
        case GREEN_LED:
           // TRISEbits.TRISE5 = 0;    ODCEbits.ODE5 = 0;  LATEbits.LATE5 = 1;
            LED_OUT;
            LED_1;
            break;
        case RED_LED:
            
            break;
        case BEEP_LED:
            break;
        default:break;
    }
}

/**
 * LED 或者 蜂鸣器滴答
 * @param type:类型，可以多种类型同时打开或关闭；用法：type1 | type2 | type3
 * @param count：滴答的次数；
 * @param time_on：滴答开的时间；
 * @param time_off：滴答关的时间；
 */
void LedOrBeepTick(LED_TypeDef type, unsigned short count, unsigned short time_on, unsigned short time_off)
{
    unsigned short i;
    for(i=0; i<count; i++)
    {
        LEDOn((LED_TypeDef)(type & GREEN_LED));
        LEDOn((LED_TypeDef)(type & RED_LED));
        LEDOn((LED_TypeDef)(type & BEEP_LED));
        DelayMs(time_on);
        LEDOff((LED_TypeDef)(type & GREEN_LED));
        LEDOff((LED_TypeDef)(type & RED_LED));
        LEDOff((LED_TypeDef)(type & BEEP_LED));
        DelayMs(time_off);
    }
}


int fputc(int ch,FILE *f)
{
    unsigned int timeout = 20000;

    switch(Debug_COM )
    {
        case COM1:
            while((BusyUART1()) && (timeout--))
                ;
            WriteUART1(ch);
            break;

        case COM2:
            while((BusyUART2()) && (timeout--))
                ;
            WriteUART2(ch);
            break;

        case COM3:
            while((BusyUART3()) && (timeout--))
                ;
            WriteUART3(ch);
            break;

        case COM4:
            while((BusyUART4()) && (timeout--))
                ;
            WriteUART4(ch);
            break;
    }
    return 0;
}
