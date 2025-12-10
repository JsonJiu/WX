/*
                                                    ****************************
                                                            lib_rs004
                                                    ****************************

?Âµ?Ã·:       ??Â¼Ã¾?Âµ?Ã·

                TRISx:0Êä³ö,1ÊäÈë/AD        TRISBbits.TRISB8
                ANSx:0Êý×Ö¶Ë¿Ú,1Ä£Äâ¶Ë¿Ú    ANSBbits.ANSB8
                ODCx:0Êý×ÖÊä³ö,1¿ªÂ©Êä³ö    ODCBbits.ODB8
                PORTx:¶Ë¿ÚµçÆ½              PORTBbits.RB8
                //LATBx:¶ÁÐ´¶Ë¿ÚµçÆ½Ëø´æÆ÷    LATBbits.LATB9

                2.4V-5.5V
                High-speedOCO(fiH)
                8MHz

?Ã¨?Ã³:       Â¶??Â²Â¼Ã¾Â»Ã²?Ã¤?Ã¼?Ã­Â¼Ã¾Âµ??Ã¨?Ã³
Â°Ã¦Â±Â¾:       v0.0.0
Â°Ã¦?Â¨:       ?Ã¾????????Â±Ã­Â¹?Â·?????Â¹Â«?Â¾Â¡Â¡Â°Ã¦?Â¨?Ã¹??Â£Â¬(c)2000-2050
??Â¸?Â¼??Â¼:   --------------------------------------------------------------------
            ????:           Â¸Âº?Ã°??:     Â¹Â¤?Ã·:           Â°Ã¦Â±Â¾:
            XXXX-XX-XX      XXXX    Â´Â´Â½Â¨/??Â¸?/?Ã¶Â¼?/???Â´ v0.0.0
********************************************************************************
*/
#define     _lib_rs004_C_
#include    "lib_mc308.h"

static unsigned short Random;
void IrqServerDefaultRS(unsigned char Data){ ; }
void IrqServerDefault(void){ ; }

IRQServerFT IRQServerCN0 = IrqServerDefault;
IRQServerFT IRQServerCN1 = IrqServerDefault; 
IRQServerFT IRQServerCN2 = IrqServerDefault; 
IRQServerFT IRQServerCN3 = IrqServerDefault; 
IRQServerFT IRQServerCN4 = IrqServerDefault; 
IRQServerFT IRQServerCN5 = IrqServerDefault; 
IRQServerFT IRQServerCN6 = IrqServerDefault; 
IRQServerFT IRQServerCN7 = IrqServerDefault; 
IRQServerFT IRQServerCN8 = IrqServerDefault; 
IRQServerFT IRQServerCN9 = IrqServerDefault; 
IRQServerFT IRQServerCN10 = IrqServerDefault;
IRQServerFT IRQServerCN11 = IrqServerDefault;
IRQServerFT IRQServerCN12 = IrqServerDefault;
IRQServerFT IRQServerCN13 = IrqServerDefault;
IRQServerFT IRQServerCN14 = IrqServerDefault;
IRQServerFT IRQServerCN18 = IrqServerDefault;
IRQServerFT IRQServerCN21 = IrqServerDefault;
IRQServerFT IRQServerCN27 = IrqServerDefault;
IRQServerFT IRQServerCN63 = IrqServerDefault;
IRQServerFT IRQServerCN64 = IrqServerDefault;
IRQServerFT IRQServerCN71 = IrqServerDefault;
IRQServerFT IRQServerCN43 = IrqServerDefault;

IRQServerFT IRQServerCN17 = IrqServerDefault;//20150108
IRQServerFT IRQServerCN25 = IrqServerDefault;//20150108

IRQServerFT IRQServerINT0 = IrqServerDefault;
IRQServerFT IRQServerINT1 = IrqServerDefault;
IRQServerFT IRQServerINT2 = IrqServerDefault;
IRQServerFT IRQServerINT3 = IrqServerDefault;
IRQServerFT IRQServerINT4 = IrqServerDefault;

void __attribute__((interrupt, no_auto_psv)) _CNInterrupt(void)
{
    InputChange_Clear_Intr_Status_Bit;
//    if ((CNEN2bits.CN18IE == 1) && (PORTFbits.RF5 == 0))
//        IRQServerCN21();
//    if ((CNEN2bits.CN21IE == 1) && (PORTDbits.RD15 == 0))
//        IRQServerCN21();
//    if ((CNEN2bits.CN27IE == 1) && (PORTBbits.RB9 == 0))
//        IRQServerCN27();
//    if ((CNEN1bits.CN5IE == 1) && (PORTBbits.RB3 == 0))
//        IRQServerCN5();
//    if ((CNEN4bits.CN63IE == 1) && (PORTEbits.RE5 == 0))
//        IRQServerCN63();
//    if ((CNEN5bits.CN64IE == 1) && (PORTEbits.RE6 == 0))
//        IRQServerCN64();
//    if ((CNEN2bits.CN18IE == 1) && (PORTFbits.RF5 == 0))
//        IRQServerCN64();

    //if ((CNEN2bits.CN17IE == 1) && (PORTFbits.RF4 == 0))
    //    IRQServerCN17();
    //if ((CNEN2bits.CN25IE == 1) && (PORTBbits.RB7 == 0))
    //    IRQServerCN25();
    if ((CNEN1bits.CN2IE == 1) && (PORTBbits.RB0 == 0))
        IRQServerCN2();
    // if ((CNEN3bits.CN43IE == 1) && (PORTAbits.RA14 == 0))
    //    IRQServerCN43();
    //if ((CNEN5bits.CN71IE == 1) && (PORTFbits.RF3 == 0))
    //    IRQServerCN71();
    Random++;
}

void __attribute__((interrupt, no_auto_psv)) _INT0Interrupt(void)
{
    Int0_Clear_Intr_Status_Bit;
    IRQServerINT0();
    Random++;
}

void __attribute__ ((interrupt,no_auto_psv)) _INT1Interrupt(void)
{
    Int1_Clear_Intr_Status_Bit;
    IRQServerINT1();
    Random++;
}

void __attribute__ ((interrupt,no_auto_psv)) _INT2Interrupt(void)
{
    Int2_Clear_Intr_Status_Bit;
    IRQServerINT2();
    Random++;
}

void __attribute__ ((interrupt,no_auto_psv)) _INT3Interrupt(void)
{
    Int3_Clear_Intr_Status_Bit;
    IRQServerINT3();
    Random++;
}

void __attribute__ ((interrupt,no_auto_psv)) _INT4Interrupt(void)
{
    Int4_Clear_Intr_Status_Bit;
    IRQServerINT4();
    Random++;
}


//Config: RISING_EDGE_INT/FALLING_EDGE_INT
void INT0Config(unsigned char Config, IRQServerFT Fuc)
{

}

void INT1Config(unsigned char Config, IRQServerFT Fuc)
{

}

void INT2Config(unsigned char Config, IRQServerFT Fuc)
{

}

void INT3Config(unsigned char Config, IRQServerFT Fuc)
{

}

void INT4Config(unsigned char Config, IRQServerFT Fuc)
{

}

void INT5Config(unsigned char Config, IRQServerFT Fuc)
{

}

void INT6Config(unsigned char Config, IRQServerFT Fuc)
{

}

void INT7Config(unsigned char Config, IRQServerFT Fuc)
{

}

/**************************************************************************/

IRQServerRecvDataFT IRQServerUart1Receive = IrqServerDefaultRS;
IRQServerRecvDataFT IRQServerUart2Receive = IrqServerDefaultRS;
IRQServerRecvDataFT IRQServerUart3Receive = IrqServerDefaultRS;
IRQServerRecvDataFT IRQServerUart4Receive = IrqServerDefaultRS;
void __attribute__ ((interrupt,no_auto_psv)) _U1RXInterrupt(void)
{
    unsigned char rx_data;
    U1RX_Clear_Intr_Status_Bit;
    while(!DataRdyUART1());
    rx_data = U1RXREG;
    IRQServerUart1Receive(rx_data);
    //Uart1SendData(&rx_data,1);
    Random += rx_data;
}

void __attribute__ ((interrupt,no_auto_psv)) _U2RXInterrupt(void)
{
    unsigned char rx_data;
    U2RX_Clear_Intr_Status_Bit;
    while(!DataRdyUART2());
    rx_data = U2RXREG;
    IRQServerUart2Receive(rx_data);
    Random += rx_data;
}

void __attribute__ ((interrupt,no_auto_psv)) _U3RXInterrupt(void)
{
    unsigned char rx_data;
    U3RX_Clear_Intr_Status_Bit;
    while(!DataRdyUART3());
    rx_data = U3RXREG;
    IRQServerUart3Receive(rx_data);
    Random += rx_data;
}

void __attribute__ ((interrupt,no_auto_psv)) _U4RXInterrupt(void)
{
    unsigned char rx_data;
    U4RX_Clear_Intr_Status_Bit;
    while(!DataRdyUART4());
    rx_data = U4RXREG;
    IRQServerUart4Receive(rx_data);
    Random += rx_data;
}

#define SUBBAUD
#ifdef SUBBAUD
unsigned int GetBaud(UartBaud Baud)
{
    unsigned long ubrg = 0;

    ubrg =  ((unsigned long) DEF_MAIN_CLK) >> 5;
    switch(Baud)
    {
    case UartBaud1200:
        ubrg = (ubrg / 1200) - 1;
        break;
    case UartBaud2400:
        ubrg = (ubrg / 2400) - 1;
        break;
    case UartBaud4800:
        ubrg = (ubrg / 4800) - 1;
        break;
    case UartBaud9600:
        ubrg = (ubrg / 9600) - 1;
        break;
    case UartBaud19200:
        ubrg = (ubrg / 19200) - 1;
        break;
    case UartBaud38400:
        ubrg = (ubrg / 38400) - 1;
        break;
    case UartBaud115200:
        ubrg = (ubrg / 115200) -1 ;
        break;
    case UartBaud62500:
        ubrg = (ubrg / 62500) - 1;
        break;
    default:
        break;
    }
    return (unsigned int)ubrg;
}
#endif
void Uart1Config(pUartMode Mode, IRQServerSendDataFT SendFun, IRQServerRecvDataFT RecvFun)
{
    unsigned int config1=0 , ss = 0;
    unsigned int config2=0;
    unsigned int ubrg=0;
    config1 |= UART_EN;
    //å¥??æ£??éª??
    if((Mode->Parity == PARITY_NONE)&&(Mode->DataBit == DATA_8))
    {
        config1 |= UART_NO_PAR_8BIT;
    }
    else if((Mode->Parity == PARITY_ODD)&&(Mode->DataBit == DATA_8))
    {
        config1 |= UART_ODD_PAR_8BIT;
    }
    else if((Mode->Parity == PARITY_EVEN)&&(Mode->DataBit == DATA_8))
    {
        config1 |= UART_EVEN_PAR_8BIT;
    }
    else if((Mode->Parity == PARITY_NONE)&&(Mode->DataBit == DATA_9))
    {
        config1 |= UART_NO_PAR_9BIT;
    }

    //???ä½??
    if(Mode->StopBit == STOP_1)
    {
        config1 |= UART_1STOPBIT;
    }
    else if(Mode->StopBit == STOP_2)
    {
        config1 |= UART_2STOPBITS;
    }

    config2 |= UART_TX_ENABLE;

#ifdef SUBBAUD
    ubrg = GetBaud(Mode->Baud);
#else
    switch(Mode->Baud)
    {
    case UartBaud1200:
        ubrg = ((DEF_MAIN_CLK/2)/(16*1200))-1;
        break;
    case UartBaud2400:
        ubrg = ((DEF_MAIN_CLK/2)/(16*2400))-1;
        break;
    case UartBaud4800:
        ubrg = ((DEF_MAIN_CLK/2)/(16*4800))-1;
        break;
    case UartBaud9600:
        ubrg = ((DEF_MAIN_CLK/2)/(16*9600))-1;
        break;
    case UartBaud19200:
        ubrg = ((DEF_MAIN_CLK/2)/(16*19200))-1;
        break;
    case UartBaud38400:
        ubrg = ((DEF_MAIN_CLK/2)/(16*38400))-1;
        break;
    case UartBaud115200:
        ubrg = ((DEF_MAIN_CLK/2)/(16*115200))-1;
        break;
    default:
        break;
    }
#endif
    CloseUART1();
    ConfigIntUART1( UART_RX_INT_EN | UART_RX_INT_PR3);
    OpenUART1(config1 , config2, ubrg);

    IRQServerUart1Receive = RecvFun;
}

void Uart2Config(pUartMode Mode, IRQServerSendDataFT SendFun, IRQServerRecvDataFT RecvFun)
{
   unsigned int config1=0; 
    unsigned int config2=0;
    unsigned int ubrg=0;
    config1 |= UART_EN;
    //å¥??æ£??éª??
    if((Mode->Parity == PARITY_NONE)&&(Mode->DataBit == DATA_8))
    {
        config1 |= UART_NO_PAR_8BIT;
    }
    else if((Mode->Parity == PARITY_ODD)&&(Mode->DataBit == DATA_8))
    {
        config1 |= UART_ODD_PAR_8BIT;
    }
    else if((Mode->Parity == PARITY_EVEN)&&(Mode->DataBit == DATA_8))
    {
        config1 |= UART_EVEN_PAR_8BIT;
    }
    else if((Mode->Parity == PARITY_NONE)&&(Mode->DataBit == DATA_9))
    {
        config1 |= UART_NO_PAR_9BIT;
    }

    //???ä½??
    if(Mode->StopBit == STOP_1)
    {
        config1 |= UART_1STOPBIT;
    }
    else if(Mode->StopBit == STOP_2)
    {
        config1 |= UART_2STOPBITS;
    }
    config2 |= UART_TX_ENABLE;
#ifdef SUBBAUD
    ubrg = GetBaud(Mode->Baud);
#else
    switch(Mode->Baud)
    {
    case UartBaud1200:
        ubrg = ((DEF_MAIN_CLK/2)/(16*1200))-1;
        break;
    case UartBaud2400:
        ubrg = ((DEF_MAIN_CLK/2)/(16*2400))-1;
        break;
    case UartBaud4800:
        ubrg = ((DEF_MAIN_CLK/2)/(16*4800))-1;
        break;
    case UartBaud9600:
        ubrg = ((DEF_MAIN_CLK/2)/(16*9600))-1;
        break;
    case UartBaud19200:
        ubrg = ((DEF_MAIN_CLK/2)/(16*19200))-1;
        break;
    case UartBaud38400:
        ubrg = ((DEF_MAIN_CLK/2)/(16*38400))-1;
        break;
    case UartBaud115200:
        ubrg = ((DEF_MAIN_CLK/2)/(16*115200))-1;
        break;
    default:
        break;
    }
#endif
    CloseUART2();
    ConfigIntUART2( UART_RX_INT_EN | UART_RX_INT_PR3);
    OpenUART2(config1 , config2, ubrg);
    IRQServerUart2Receive = RecvFun;
}

void Uart3Config(pUartMode Mode, IRQServerSendDataFT SendFun, IRQServerRecvDataFT RecvFun)
{
   unsigned int config1=0; 
    unsigned int config2=0;
    unsigned int ubrg=0;
    config1 |= UART_EN;
    //å¥??æ£??éª??
    if((Mode->Parity == PARITY_NONE)&&(Mode->DataBit == DATA_8))
    {
        config1 |= UART_NO_PAR_8BIT;
    }
    else if((Mode->Parity == PARITY_ODD)&&(Mode->DataBit == DATA_8))
    {
        config1 |= UART_ODD_PAR_8BIT;
    }
    else if((Mode->Parity == PARITY_EVEN)&&(Mode->DataBit == DATA_8))
    {
        config1 |= UART_EVEN_PAR_8BIT;
    }
    else if((Mode->Parity == PARITY_NONE)&&(Mode->DataBit == DATA_9))
    {
        config1 |= UART_NO_PAR_9BIT;
    }

    //???ä½??
    if(Mode->StopBit == STOP_1)
    {
        config1 |= UART_1STOPBIT;
    }
    else if(Mode->StopBit == STOP_2)
    {
        config1 |= UART_2STOPBITS;
    }
    config2 |= UART_TX_ENABLE;

#ifdef SUBBAUD
    ubrg = GetBaud(Mode->Baud);
#else
    switch(Mode->Baud)
    {
    case UartBaud1200:
        ubrg = ((DEF_MAIN_CLK/2)/(16*1200))-1;
        break;
    case UartBaud2400:
        ubrg = ((DEF_MAIN_CLK/2)/(16*2400))-1;
        break;
    case UartBaud4800:
        ubrg = ((DEF_MAIN_CLK/2)/(16*4800))-1;
        break;
    case UartBaud9600:
        ubrg = ((DEF_MAIN_CLK/2)/(16*9600))-1;
        break;
    case UartBaud19200:
        ubrg = ((DEF_MAIN_CLK/2)/(16*19200))-1;
        break;
    case UartBaud38400:
        ubrg = ((DEF_MAIN_CLK/2)/(16*38400))-1;
        break;
    case UartBaud115200:
        ubrg = ((DEF_MAIN_CLK/2)/(16*115200))-1;
        break;
    default:
        break;
    }
#endif
    CloseUART3();
    ConfigIntUART3( UART_RX_INT_EN | UART_RX_INT_PR3);
    OpenUART3(config1 , config2, ubrg);
    IRQServerUart3Receive = RecvFun;
}

void Uart4Config(pUartMode Mode, IRQServerSendDataFT SendFun, IRQServerRecvDataFT RecvFun)
{
   unsigned int config1=0; 
    unsigned int config2=0;
    unsigned int ubrg=0;
    config1 |= UART_EN;
    //å¥??æ£??éª??
    if((Mode->Parity == PARITY_NONE)&&(Mode->DataBit == DATA_8))
    {
        config1 |= UART_NO_PAR_8BIT;
    }
    else if((Mode->Parity == PARITY_ODD)&&(Mode->DataBit == DATA_8))
    {
        config1 |= UART_ODD_PAR_8BIT;
    }
    else if((Mode->Parity == PARITY_EVEN)&&(Mode->DataBit == DATA_8))
    {
        config1 |= UART_EVEN_PAR_8BIT;
    }
    else if((Mode->Parity == PARITY_NONE)&&(Mode->DataBit == DATA_9))
    {
        config1 |= UART_NO_PAR_9BIT;
    }

    //???ä½??
    if(Mode->StopBit == STOP_1)
    {
        config1 |= UART_1STOPBIT;
    }
    else if(Mode->StopBit == STOP_2)
    {
        config1 |= UART_2STOPBITS;
    }
    config2 |= UART_TX_ENABLE;
#ifdef SUBBAUD
    ubrg = GetBaud(Mode->Baud);
#else
    switch(Mode->Baud)
    {
    case UartBaud1200:
        ubrg = ((DEF_MAIN_CLK/2)/(16*1200))-1;
        break;
    case UartBaud2400:
        ubrg = ((DEF_MAIN_CLK/2)/(16*2400))-1;
        break;
    case UartBaud4800:
        ubrg = ((DEF_MAIN_CLK/2)/(16*4800))-1;
        break;
    case UartBaud9600:
        ubrg = ((DEF_MAIN_CLK/2)/(16*9600))-1;
        break;
    case UartBaud19200:
        ubrg = ((DEF_MAIN_CLK/2)/(16*19200))-1;
        break;
    case UartBaud38400:
        ubrg = ((DEF_MAIN_CLK/2)/(16*38400))-1;
        break;
    case UartBaud115200:
        ubrg = ((DEF_MAIN_CLK/2)/(16*115200))-1;
        break;
    default:
        break;
    }
#endif
    CloseUART4();
    ConfigIntUART4( UART_RX_INT_EN | UART_RX_INT_PR3);
    OpenUART4(config1 , config2, ubrg);
    IRQServerUart4Receive = RecvFun;
}

void Uart1SendEnable(void)
{
   EnableIntU1TX;
}

void Uart1SendDisable(void)
{
   DisableIntU1TX;
}

void Uart1ReceiveEnable(void)
{
   EnableIntU1RX; 
}

void Uart1ReceiveDisable(void)
{
   DisableIntU1RX;
}

void Uart2SendEnable(void)
{
   EnableIntU2TX;
}

void Uart2SendDisable(void)
{
   DisableIntU2TX;
}

void Uart2ReceiveEnable(void)
{
   EnableIntU2RX;
}

void Uart2ReceiveDisable(void)
{
   DisableIntU2RX;
}

void Uart3SendEnable(void)
{
   EnableIntU3TX;
}

void Uart3SendDisable(void)
{
   DisableIntU3TX;
}

void Uart3ReceiveEnable(void)
{
   EnableIntU3RX;
}

void Uart3ReceiveDisable(void)
{
   DisableIntU3RX;
}
void Uart4SendEnable(void)
{
   EnableIntU4TX;
}

void Uart4SendDisable(void)
{
   DisableIntU4TX;
}

void Uart4ReceiveEnable(void)
{
   EnableIntU4RX;
}

void Uart4ReceiveDisable(void)
{
   DisableIntU4RX;
}
int Uart1SendData(unsigned char*pSendBuf,unsigned int Len)
{ 
   while (Len!=0) {
      while(BusyUART1());
      WriteUART1(*pSendBuf++);
      Len--;
   }
}

int Uart2SendData(unsigned char*pSendBuf,unsigned int Len)
{
   while (Len!=0) {
      while(BusyUART2());
      WriteUART2(*pSendBuf++);
      Len--;
   }
}

int Uart3SendData(unsigned char*pSendBuf,unsigned int Len)
{
   while (Len!=0) {
      while(BusyUART3());
      WriteUART3(*pSendBuf++);
      Len--;
   }
}

int Uart4SendData(unsigned char*pSendBuf,unsigned int Len)
{
   while (Len!=0) {
      while(BusyUART4());
      WriteUART4(*pSendBuf++);
      Len--;
   }
}



/*
*******************************************************
*******************************************************
*******************************************************
*******************************************************
?Â¯??
*/
void MCU_SleepEnter(void)
{
    Nop();
    Nop();
    Sleep();    //{__asm__ volatile ("pwrsav #0");}
    Nop();
    Nop();
}
//308??Â´?Â¹Â¦??
void MCU_IntEn(void)
{

}
//308??Â´?Â¹Â¦??
void MCU_IntDis(void)
{

}
/*
*******************************************************

*/
void MCU_Reset(void)
{
    Nop();
    Nop();
    asm volatile("RESET");
    Nop();
    Nop();
    Nop();
}
/*
*******************************************************
 ?Âµ?Ã·:Â¿Â´??Â¹Â·?Ã¨??
*/
void MCU_DogCfg(void)
{
    //_WDTPS3 = 1;
//    EnableWDT(WDT_ENABLE);//WDT_DISABLE
}
void MCU_DogReset(void)
{
    ClrWdt();
}
/*
*******************************************************
?Âµ?Ã·:  
*/
void MCUPllCfg(void)
{
    int Idx;
    //Â¿Ã¬??RC ?Ã±ÂµÂ´?Ã·Â£Â¨FRCÂ£Â©Â±ÃªÂ³?8M
    OSCCONbits.COSC = 0;
    OSCCONbits.NOSC = 0;
    _OSWEN = 0;//?Â¹??
#ifdef EXT_TIMER_CLOCK
    _SOSCEN = 1;//32 kHz ¸¨ÖúÕñµ´Æ÷£¨SOSC£©Ê¹ÄÜÎ»-Ê¹ÄÜ¸¨ÖúÕñµ´Æ÷
#endif
    _POSCEN = 0;//Ö÷Õñµ´Æ÷ÔÚÐÝÃßÄ£Ê½ÆÚ¼ä±»½ûÖ¹

    _NSTDIS = 1;//Â½Ã»?Â¹??Â¶??Â¶??
    _RETEN = 1;//Ê¹ÓÃµÍµçÑ¹Ë¯Ãß.
    for (Idx = 0; Idx < 2000; Idx++)
        ;
}
/*
*******************************************************
?Âµ?Ã·:  ?ÂºÂ¾Â§:    0  1  2  3  4  5  6  7  8  9  10  11  12  13  14  15  16  17  18  19  20  COM0,COM1,COM2,COM3
       MCU:    0  33 32 50 46 48 27 26 25 24 23  45  44  22  21  20  17  16  15  14  13  COM0,COM1,CPM2,COM3
*/
void MCU_LcdCfg(void)   //Ã¦Â¶Â²Ã¦?¢Â¶Ã¥Ë??¥Â§â??¹Ã¥Å??
{
    //LCDSEx
    {//Â¶?Â¿??Ã¤??
        _SE00 = 1;
        _SE33 = 1;
        _SE32 = 1;
        _SE50 = 1;
        _SE46 = 1;
        _SE48 = 1;
        _SE27 = 1;
        _SE26 = 1;
        _SE25 = 1;
        _SE24 = 1;
        _SE23 = 1;
        _SE45 = 1;
        _SE44 = 1;
        _SE22 = 1;
        _SE21 = 1;
        _SE20 = 1;
        _SE17 = 1;
        _SE16 = 1;
        _SE15 = 1;
        _SE14 = 1;
        _SE13 = 1;
    }
    LCDCONbits.LCDSIDL = 0;//LCD driver continues to operate in CPU Idle mode
    LCDCONbits.SLPEN = 0;//LCD driver module is enabled in Sleep mode
    LCDCONbits.CS = 1;//Clock Source Select bits:SOSC-------------------
    LCDCONbits.LMUX = 0x03;//1/4 MUX (COM<3:0>)

    //LCDREGbits.CPEN = 0;
    //LCDREGbits.BIAS = 5;
    //LCDREGbits.MODE13 = 1;
    //LCDREGbits.CKSEL = 0;

    //?Ã¤??Â¼?Â´Ã¦?Ã·
    LCDPSbits.WFT = 0;
    LCDPSbits.BIASMD = 0;
    LCDPSbits.LP = 3;//LCDÔ¤·ÖÆµ±È

    LCDREFbits.LCDIRE = 1;
    LCDREFbits.LCDCST = 0;//Â¶?Â±?Â¶?
    LCDREFbits.LRLAP = 0;//Â¹Â¦Âº?Âµ?Â¼Â¶
    LCDREFbits.LRLBP = 1;//Â¹Â¦Âº?Âµ?Â¼Â¶
    LCDREFbits.LRLAT = 0;
    LCDREFbits.VLCD1PE = 0;
    LCDREFbits.VLCD2PE = 0;
    LCDREFbits.VLCD3PE = 0;
}
void MCU_LcdOpen(void)
{
    LCDCONbits.LCDEN = 1;
}
void MCU_LcdClose(void)
{
    LCDCONbits.LCDEN = 0;
}
void MCU_LcdAllSet(void)
{
    int Idx;
    for (Idx = 0; Idx <= 20; Idx++)
    {
        MCU_LcdWriteReg(Idx, 0xff);
    }
}
void MCU_LcdAllClr(void)
{
    int Idx;
    for (Idx = 0; Idx <= 20; Idx++)
    {
        MCU_LcdWriteReg(Idx, 0);
    }
}
void MCU_LcdWriteReg(unsigned char Seg, unsigned char Reg)
{
    switch (Seg)
    {
    case  0: _S00C0 = (Reg >> 0); _S00C1 = (Reg >> 1); _S00C2 = (Reg >> 2); _S00C3 = (Reg >> 3); break;
    case  1: _S33C0 = (Reg >> 0); _S33C1 = (Reg >> 1); _S33C2 = (Reg >> 2); _S33C3 = (Reg >> 3); break;
    case  2: _S32C0 = (Reg >> 0); _S32C1 = (Reg >> 1); _S32C2 = (Reg >> 2); _S32C3 = (Reg >> 3); break;
    case  3: _S50C0 = (Reg >> 0); _S50C1 = (Reg >> 1); _S50C2 = (Reg >> 2); _S50C3 = (Reg >> 3); break;
    case  4: _S46C0 = (Reg >> 0); _S46C1 = (Reg >> 1); _S46C2 = (Reg >> 2); _S46C3 = (Reg >> 3); break;
    case  5: _S48C0 = (Reg >> 0); _S48C1 = (Reg >> 1); _S48C2 = (Reg >> 2); _S48C3 = (Reg >> 3); break;
    case  6: _S27C0 = (Reg >> 0); _S27C1 = (Reg >> 1); _S27C2 = (Reg >> 2); _S27C3 = (Reg >> 3); break;
    case  7: _S26C0 = (Reg >> 0); _S26C1 = (Reg >> 1); _S26C2 = (Reg >> 2); _S26C3 = (Reg >> 3); break;
    case  8: _S25C0 = (Reg >> 0); _S25C1 = (Reg >> 1); _S25C2 = (Reg >> 2); _S25C3 = (Reg >> 3); break;
    case  9: _S24C0 = (Reg >> 0); _S24C1 = (Reg >> 1); _S24C2 = (Reg >> 2); _S24C3 = (Reg >> 3); break;
    case 10: _S23C0 = (Reg >> 0); _S23C1 = (Reg >> 1); _S23C2 = (Reg >> 2); _S23C3 = (Reg >> 3); break;
    case 11: _S45C0 = (Reg >> 0); _S45C1 = (Reg >> 1); _S45C2 = (Reg >> 2); _S45C3 = (Reg >> 3); break;
    case 12: _S44C0 = (Reg >> 0); _S44C1 = (Reg >> 1); _S44C2 = (Reg >> 2); _S44C3 = (Reg >> 3); break;
    case 13: _S22C0 = (Reg >> 0); _S22C1 = (Reg >> 1); _S22C2 = (Reg >> 2); _S22C3 = (Reg >> 3); break;
    case 14: _S21C0 = (Reg >> 0); _S21C1 = (Reg >> 1); _S21C2 = (Reg >> 2); _S21C3 = (Reg >> 3); break;
    case 15: _S20C0 = (Reg >> 0); _S20C1 = (Reg >> 1); _S20C2 = (Reg >> 2); _S20C3 = (Reg >> 3); break;
    case 16: _S17C0 = (Reg >> 0); _S17C1 = (Reg >> 1); _S17C2 = (Reg >> 2); _S17C3 = (Reg >> 3); break;
    case 17: _S16C0 = (Reg >> 0); _S16C1 = (Reg >> 1); _S16C2 = (Reg >> 2); _S16C3 = (Reg >> 3); break;
    case 18: _S15C0 = (Reg >> 0); _S15C1 = (Reg >> 1); _S15C2 = (Reg >> 2); _S15C3 = (Reg >> 3); break;
    case 19: _S14C0 = (Reg >> 0); _S14C1 = (Reg >> 1); _S14C2 = (Reg >> 2); _S14C3 = (Reg >> 3); break;
    case 20: _S13C0 = (Reg >> 0); _S13C1 = (Reg >> 1); _S13C2 = (Reg >> 2); _S13C3 = (Reg >> 3); break;
    }
}
unsigned char MCU_LcdReadReg(unsigned char Seg)
{
    switch (Seg)
    {
    case 0:  return _S00C0 | (_S00C1 << 1) | (_S00C2 << 2) | (_S00C3 << 3);
    case 1:  return _S33C0 | (_S33C1 << 1) | (_S33C2 << 2) | (_S33C3 << 3);
    case 2:  return _S32C0 | (_S32C1 << 1) | (_S32C2 << 2) | (_S32C3 << 3);
    case 3:  return _S50C0 | (_S50C1 << 1) | (_S50C2 << 2) | (_S50C3 << 3);
    case 4:  return _S46C0 | (_S46C1 << 1) | (_S46C2 << 2) | (_S46C3 << 3);
    case 5:  return _S48C0 | (_S48C1 << 1) | (_S48C2 << 2) | (_S48C3 << 3);
    case 6:  return _S27C0 | (_S27C1 << 1) | (_S27C2 << 2) | (_S27C3 << 3);
    case 7:  return _S26C0 | (_S26C1 << 1) | (_S26C2 << 2) | (_S26C3 << 3);
    case 8:  return _S25C0 | (_S25C1 << 1) | (_S25C2 << 2) | (_S25C3 << 3);
    case 9:  return _S24C0 | (_S24C1 << 1) | (_S24C2 << 2) | (_S24C3 << 3);
    case 10: return _S23C0 | (_S23C1 << 1) | (_S23C2 << 2) | (_S23C3 << 3);
    case 11: return _S45C0 | (_S45C1 << 1) | (_S45C2 << 2) | (_S45C3 << 3);
    case 12: return _S44C0 | (_S44C1 << 1) | (_S44C2 << 2) | (_S44C3 << 3);
    case 13: return _S22C0 | (_S22C1 << 1) | (_S22C2 << 2) | (_S22C3 << 3);
    case 14: return _S21C0 | (_S21C1 << 1) | (_S21C2 << 2) | (_S21C3 << 3);
    case 15: return _S20C0 | (_S20C1 << 1) | (_S20C2 << 2) | (_S20C3 << 3);
    case 16: return _S17C0 | (_S17C1 << 1) | (_S17C2 << 2) | (_S17C3 << 3);
    case 17: return _S16C0 | (_S16C1 << 1) | (_S16C2 << 2) | (_S16C3 << 3);
    case 18: return _S15C0 | (_S15C1 << 1) | (_S15C2 << 2) | (_S15C3 << 3);
    case 19: return _S14C0 | (_S14C1 << 1) | (_S14C2 << 2) | (_S14C3 << 3);
    case 20: return _S13C0 | (_S13C1 << 1) | (_S13C2 << 2) | (_S13C3 << 3);
    default:       return 1;
    }
}
/*
*******************************************************
*/
#define ServerIntervalNum   1
IRQServerFT IRQServerTM1[ServerIntervalNum] = { IrqServerDefault };//, IrqServerDefault, IrqServerDefault };//
IRQServerFT IRQServerTM2[ServerIntervalNum] = { IrqServerDefault };//, IrqServerDefault, IrqServerDefault };//
IRQServerFT IRQServerTM3[ServerIntervalNum] = { IrqServerDefault };//, IrqServerDefault, IrqServerDefault };//
IRQServerFT IRQServerTM4[ServerIntervalNum] = { IrqServerDefault };//, IrqServerDefault, IrqServerDefault };//
IRQServerFT IRQServerTM5[ServerIntervalNum] = { IrqServerDefault };//, IrqServerDefault, IrqServerDefault };//

unsigned char Onesec = 0;
unsigned char sec = 0;
unsigned char min = 0;
void __attribute__((interrupt,auto_psv)) _T1Interrupt(void)
{
    char i;
    T1_Clear_Intr_Status_Bit;
    for (i = 0; i < ServerIntervalNum; i++)
        IRQServerTM1[i]();
    Random++;
}

void __attribute__((interrupt,auto_psv)) _T2Interrupt(void)
{
    char i;
    T2_Clear_Intr_Status_Bit;
    for (i = 0; i < ServerIntervalNum; i++)
        IRQServerTM2[i]();
    Random++;
}

void __attribute__((interrupt,auto_psv)) _T3Interrupt(void)
{
    char i;
    T3_Clear_Intr_Status_Bit;
    for (i = 0; i < ServerIntervalNum; i++)
        IRQServerTM3[i]();
    Random++;
}

void __attribute__((interrupt,auto_psv)) _T4Interrupt(void)
{
    char i;
    T4_Clear_Intr_Status_Bit;
    for (i = 0; i < ServerIntervalNum; i++)
        IRQServerTM4[i]();
    Random++;
}

void __attribute__((interrupt,auto_psv)) _T5Interrupt(void)
{
    char i;
    T5_Clear_Intr_Status_Bit;
    for (i = 0; i < ServerIntervalNum; i++)
        IRQServerTM5[i]();
    Random++;
}

//?Â¹??FRC?Ã±ÂµÂ´?Ã·,Â¶Â¨?Â±?Ã·?Â¹??64Â·??Âµ
#define TIME_SP_CNT_MS1   61
#define TIME_SP_CNT_MS10  610
#define TIME_SP_CNT_MS20 (610*2)
#define TIME_SP_CNT_MS50 (610*5)
#define TIME_SP_CNT_MS100 (610*10)
#define TIME_SP_CNT_MS500 (610*50)
#define TIME_SP_CNT_MS1000 (61000)
void MCU_TimerCfg(PortNumET TimeNo, TimeSpET Sp, IRQServerFT TimeServer)
{
    unsigned int i;
    unsigned int Space;
    switch(Sp)
    {
        case TimeSpMs1: Space=TIME_SP_CNT_MS1; break;
        case TimeSpMs10: Space=TIME_SP_CNT_MS10;break;
        case TimeSpMs20: Space=TIME_SP_CNT_MS20;break;
        case TimeSpMs50: Space=TIME_SP_CNT_MS50;break;
        case TimeSpMs100: Space=TIME_SP_CNT_MS100;break;
        case TimeSpMs500: Space = TIME_SP_CNT_MS500; break;
        case TimeSpMs1000: Space = TIME_SP_CNT_MS1000; break;
    }
    switch (TimeNo)
    {
    case PortVmNum1:
        for (i = 0; i < ServerIntervalNum; i++)
        {
            if (IRQServerTM1[i] == IrqServerDefault)
            {
                IRQServerTM1[i] = TimeServer;
                break;
            }
        }
        switch (Sp)
        {
        case TimeSpMs1: Space = 4; break;
        case TimeSpMs10: Space = 41; break;
        case TimeSpMs20: Space = 82; break;
        case TimeSpMs50: Space = 205; break;
        case TimeSpMs100: Space = 410; break;
        case TimeSpMs500: Space = 2048; break;
        case TimeSpMs1000: Space = 4096; break;
        }
        ConfigIntTimer1(T1_INT_PRIOR_1 + T1_INT_OFF);
        OpenTimer1(T1_OFF | T1_IDLE_STOP | T1_CLK_SOSC | T1_SOURCE_EXT | T1_PS_1_8, Space);
        break;
    case PortVmNum2:
        for (i = 0; i < ServerIntervalNum; i++)
        {
            if (IRQServerTM2[i] == IrqServerDefault)
            {
                IRQServerTM2[i] = TimeServer;
                break;
            }
        }
        ConfigIntTimer2(T2_INT_PRIOR_1 + T2_INT_OFF);
        OpenTimer2(T2_OFF | T2_IDLE_STOP | T2_PS_1_64 | T2_SOURCE_INT, Space);//TSYNC=0;TCS=1;
        break;
    case PortVmNum3:
        for (i = 0; i < ServerIntervalNum; i++)
        {
            if (IRQServerTM3[i] == IrqServerDefault)
            {
                IRQServerTM3[i] = TimeServer;
                break;
            }
        }
        ConfigIntTimer3(T3_INT_PRIOR_1 + T3_INT_OFF);
        OpenTimer3(T3_OFF | T3_IDLE_STOP | T3_GATE_OFF | T3_PS_1_64 | T3_SOURCE_INT, Space);
        break;
    case PortVmNum4:
        for (i = 0; i < ServerIntervalNum; i++)
        {
            if (IRQServerTM4[i] == IrqServerDefault)
            {
                IRQServerTM4[i] = TimeServer;
                break;
            }
        }
        ConfigIntTimer4(T4_INT_PRIOR_1 + T4_INT_OFF);
        OpenTimer4(T4_OFF | T4_IDLE_STOP | T4_GATE_OFF | T4_PS_1_64 | T4_32BIT_MODE_OFF | T4_SOURCE_INT, Space);
        break;
    case PortVmNum5:
        for (i = 0; i < ServerIntervalNum; i++)
        {
            if (IRQServerTM5[i] == IrqServerDefault)
            {
                IRQServerTM5[i] = TimeServer;
                break;
            }
        }
        ConfigIntTimer5(T5_INT_PRIOR_1 + T5_INT_OFF);
        OpenTimer5(T5_OFF | T5_IDLE_STOP | T5_GATE_OFF | T5_PS_1_64 | T5_SOURCE_INT, Space);
        break;
    //case PortVmNum6:
    //    ConfigIntTimer23(T23_INT_PRIOR_1 + T23_INT_OFF);
    //    break;
    //case PortVmNum7:
    //    ConfigIntTimer45(T45_INT_PRIOR_1 + T45_INT_OFF);
    //    break;
    }
}
void MCU_TimerStart(PortNumET TimeNo)
{
    switch (TimeNo)
    {
    case PortVmNum1:
        T1CONbits.TON = 1;
        EnableIntT1;
        break;
    case PortVmNum2:
        T2CONbits.TON = 1;
        EnableIntT2;
        break;
    case PortVmNum3:
        T3CONbits.TON = 1;
        EnableIntT3;
        break;
    case PortVmNum4:
        T4CONbits.TON = 1;
        EnableIntT4;
        break;
    case PortVmNum5:
        T5CONbits.TON = 1;
        EnableIntT5;
        break;
    //case PortVmNum6:
    //    OpenTimer23();
    //    EnableIntT23;
    //    break;
    //case PortVmNum7:
    //    OpenTimer45();
    //    EnableIntT45;
    //    break;
    }
}
void MCU_TimerStop(PortNumET TimeNo)
{
    switch (TimeNo)
    {
    case PortVmNum1:
        CloseTimer1();
        DisableIntT1;
        break;
    case PortVmNum2:
        CloseTimer2();
        DisableIntT2;
        break;
    case PortVmNum3:
        CloseTimer3();
        DisableIntT3;
        break;
    case PortVmNum4:
        CloseTimer4();
        DisableIntT4;
        break;
    case PortVmNum5:
        CloseTimer5();
        DisableIntT5;
        break;
//    case PortVmNum6:
//        CloseTimer23();
//        DisableIntT6;
//        break;
//    case PortVmNum7:
//        CloseTimer45();
//        DisableIntT7;
//        break;
    }
}
/*
*******************************************************å®??
è¯??    ???     AD???

*/
unsigned short AdcRefVolt = 3300;//ADC²É¼¯»ù×¼,µ¥Î»mV
unsigned char ADChangeBit = 8;
unsigned char ADCStaIsBusy = FALSE;
unsigned int ADCResult; //ä¿?????è¿??æ¬?DCç»??
void __MCU_ADCCfg(PortNumET ADCNum, ADCModeST Mode)   //
{
    UINT channel = 0, config1 = 0, config2 = 0, config3 = 0, config4 = 0, config5 = 0;
    UINT configctmu_low = 0, configctmu_high = 0, configscan_low = 0, config_high = 0;

    CloseADC10();

    /*set adc channel*/
    ANCFGbits.VBGEN = 0; //??VBG
    switch (ADCNum)
    {
    case PortVmNum0:
        channel = ADC_CH0_POS_SAMPLEA_AN0;
        configscan_low = ADC_SCAN_AN0;
        ANSBbits.ANSB0 = 1;//ÅäÖÃÎªÄ£Äâ
        TRISBbits.TRISB0 = 1;//ÅäÖÃÎªÊäÈë
        break;
    case PortVmNum1:
        channel = ADC_CH0_POS_SAMPLEA_AN1;
        configscan_low = ADC_SCAN_AN1;
        ANSBbits.ANSB1 = 1;//ÅäÖÃÎªÄ£Äâ
        TRISBbits.TRISB1 = 1;//ÅäÖÃÎªÊäÈë
        break;
    case PortVmNum2:
        channel = ADC_CH0_POS_SAMPLEA_AN2;
        configscan_low = ADC_SCAN_AN2;
        ANSBbits.ANSB2 = 1;//ÅäÖÃÎªÄ£Äâ
        TRISBbits.TRISB2 = 1;//ÅäÖÃÎªÊäÈë
        break;
    case PortVmNum3:
        channel = ADC_CH0_POS_SAMPLEA_AN3;
        configscan_low = ADC_SCAN_AN3;
        ANSBbits.ANSB3 = 1;//ÅäÖÃÎªÄ£Äâ
        TRISBbits.TRISB3 = 1;//ÅäÖÃÎªÊäÈë
        break;
    case PortVmNum4:
        channel = ADC_CH0_POS_SAMPLEA_AN4;
        configscan_low = ADC_SCAN_AN4;
        ANSBbits.ANSB4 = 1;//ÅäÖÃÎªÄ£Äâ
        TRISBbits.TRISB4 = 1;//ÅäÖÃÎªÊäÈë
        break;
    case PortVmNum5:
        channel = ADC_CH0_POS_SAMPLEA_AN5;
        configscan_low = ADC_SCAN_AN5;
        ANSBbits.ANSB5 = 1;//ÅäÖÃÎªÄ£Äâ
        TRISBbits.TRISB5 = 1;//ÅäÖÃÎªÊäÈë
        break;
    case PortVmNum6:
        channel = ADC_CH0_POS_SAMPLEA_AN6;
        configscan_low = ADC_SCAN_AN6;
        ANSBbits.ANSB6 = 1;//ÅäÖÃÎªÄ£Äâ
        TRISBbits.TRISB6 = 1;//ÅäÖÃÎªÊäÈë
        break;
    case PortVmNum7:
        channel = ADC_CH0_POS_SAMPLEA_AN7;
        configscan_low = ADC_SCAN_AN7;
        ANSBbits.ANSB7 = 1;//ÅäÖÃÎªÄ£Äâ
        TRISBbits.TRISB7 = 1;//ÅäÖÃÎªÊäÈë
        break;
    case PortVmNum8:
        channel = ADC_CH0_POS_SAMPLEA_AN8;
        configscan_low = ADC_SCAN_AN8;
        ANSBbits.ANSB8 = 1;//ÅäÖÃÎªÄ£Äâ
        TRISBbits.TRISB8 = 1;//ÅäÖÃÎªÊäÈë
        break;
    case PortVmNum9:
        channel = ADC_CH0_POS_SAMPLEA_AN9;
        configscan_low = ADC_SCAN_AN9;
        ANSBbits.ANSB9 = 1;//ÅäÖÃÎªÄ£Äâ
        TRISBbits.TRISB9 = 1;//ÅäÖÃÎªÊäÈë
        break;
    case PortVmNum10:
        channel = ADC_CH0_POS_SAMPLEA_AN10;
        configscan_low = ADC_SCAN_AN10;
        ANSBbits.ANSB10 = 1;//ÅäÖÃÎªÄ£Äâ
        TRISBbits.TRISB10 = 1;//ÅäÖÃÎªÊäÈë
        break;
    case PortVmNum11:
        channel = ADC_CH0_POS_SAMPLEA_AN11;
        configscan_low = ADC_SCAN_AN11;
        ANSBbits.ANSB11 = 1;//ÅäÖÃÎªÄ£Äâ
        TRISBbits.TRISB11 = 1;//ÅäÖÃÎªÊäÈë
        break;
    case PortVmNum12:
        channel = ADC_CH0_POS_SAMPLEA_AN12;
        configscan_low = ADC_SCAN_AN12;
        ANSBbits.ANSB12 = 1;//ÅäÖÃÎªÄ£Äâ
        TRISBbits.TRISB12 = 1;//ÅäÖÃÎªÊäÈë
        break;
    case PortVmNum13:
        channel = ADC_CH0_POS_SAMPLEA_AN13;
        configscan_low = ADC_SCAN_AN13;
        ANSBbits.ANSB13 = 1;//ÅäÖÃÎªÄ£Äâ
        TRISBbits.TRISB13 = 1;//ÅäÖÃÎªÊäÈë
        break;
    case PortVmNum14:
        channel = ADC_CH0_POS_SAMPLEA_AN14;
        configscan_low = ADC_SCAN_AN14;
        ANSBbits.ANSB14 = 1;//ÅäÖÃÎªÄ£Äâ
        TRISBbits.TRISB14 = 1;//ÅäÖÃÎªÊäÈë
        break;
    case PortVmNum15:
        channel = ADC_CH0_POS_SAMPLEA_AN15;
        configscan_low = ADC_SCAN_AN15;
        ANSBbits.ANSB15= 1;//ÅäÖÃÎªÄ£Äâ
        TRISBbits.TRISB15 = 1;//ÅäÖÃÎªÊäÈë
        break;
    case PortVmNum25:
        channel = ADC_CH0_POS_SAMPLEA_VBG;
        //configscan_low = ADC_SCAN_VBG;
        ANCFGbits.VBGEN = 1; //??VBG
        break;
    default:
        return;
        break;
    }
    SetChanADC10(channel);

    /*Configure adc*/
   config1 = ADC_MODULE_OFF | ADC_CLK_AUTO |ADC_AUTO_SAMPLING_ON ;
   if (Mode.ADCModeResolution == 1)
   {
       config1 |= ADC_10BIT_MODE;
       ADChangeBit = 10;
   }
   else if (Mode.ADCModeResolution == 2)
   {
       config1 |= ADC_12BIT_MODE;
       ADChangeBit = 12;
   }
   else if (Mode.ADCModeResolution == 0)
   {
       config1 |= ADC_10BIT_MODE;
       ADChangeBit = 8;
   }

   config2 = ADC_INTR_EACH_CONV;
   config3 = ADC_EXT_SAMP_OFF | ADC_SAMPLE_TIME_31 | ADC_CONV_CLK_31Tcy;
   config5 = ADC_LOW_POWER_DISABLE | ADC_CTMU_DISABLE;

   if (ADCNum >= PortVmNum25)
       config5 |= ADC_BG_ENABLE;

   OpenADC10(config1, config2, config3, config4, config5,
       configctmu_low, configctmu_high, configscan_low, config_high);

   if (ADCNum >= PortVmNum25)
       AD1CON5bits.BGREQ = 1;
}
unsigned int __MCU_ADCRead(void);
void MCU_ADCCfg(PortNumET ADCNum, ADCModeST Mode)   //
{
    static char ADCCntMax = 4;
    static char ADCCnt = 4;
    static char InitAdcChk = 0;
    char Idx = 0;
    ADCCnt++;
    if (InitAdcChk == 0)
    {
        char Jdx = 0;
        InitAdcChk = 1;
        for (Jdx = 0; Jdx < 30; Jdx++)
        {
            unsigned int Ret = 0, Temp;
            unsigned short RefTemp;
            ADCModeST ADCMode;
            ADCCnt = 0;
            ADCMode.ADCModeResolution = 1;
            __MCU_ADCCfg(PortVmNum25, ADCMode);
            MCU_ADCStart(0);
            __MCU_ADCRead();
            for (Idx = 0; Idx < 2; Idx++)
            {
                Temp = __MCU_ADCRead();
                if (Temp == 0)
                    break;
                Ret += Temp;
            }
            if (Idx == 2)
                Ret >>= 1;
            RefTemp = (unsigned short)(((unsigned long long)1024)*((unsigned long long)1200) / ((unsigned long long)Ret));
            if ((RefTemp <= 3400) && (RefTemp >= 1700))
            {
                AdcRefVolt += RefTemp;
                AdcRefVolt >>= 1;
            }
            if (AdcRefVolt >= 3250)
                ADCCntMax = 100;
            else
                ADCCntMax = 1;
            MCU_ADCStop(0);
        }
    }
    else
    {
        if (ADCCnt >= ADCCntMax)
        {
            unsigned int Ret = 0, Temp;
            unsigned short RefTemp;
            ADCModeST ADCMode;
            ADCCnt = 0;
            ADCMode.ADCModeResolution = 1;
            __MCU_ADCCfg(PortVmNum25, ADCMode);
            MCU_ADCStart(0);
            __MCU_ADCRead();
            for (Idx = 0; Idx < 2; Idx++)
            {
                Temp = __MCU_ADCRead();
                if (Temp == 0)
                    break;
                Ret += Temp;
            }
            if (Idx == 2)
                Ret >>= 1;
            RefTemp = (unsigned short)(((unsigned long long)1024)*((unsigned long long)1200) / ((unsigned long long)Ret));
            if ((RefTemp <= 3400) && (RefTemp >= 1700))
            {
                AdcRefVolt += RefTemp;
                AdcRefVolt >>= 1;
            }
            if (AdcRefVolt >= 3250)
                ADCCntMax = 100;
            else
                ADCCntMax = 1;
            MCU_ADCStop(0);
        }
    }
    __MCU_ADCCfg(ADCNum, Mode);
}
void MCU_ADCStart(PortNumET ADCNum)
{
    ADCStaIsBusy = TRUE;
    EnableADC1;
}

void MCU_ADCStop(PortNumET ADCNum)
{
    ADCStaIsBusy = FALSE;
    CloseADC10();
}
unsigned int __MCU_ADCRead(void)
{
    unsigned short OverCnt = 0;

    ConvertADC10();
    while (ADCStaIsBusy == TRUE)/*wait till conversion complete*/
    {
        if (BusySampADC1 == FALSE)
        {
            ADCResult = ReadADC10(0);//ADC1BUF0;
            break;
        }
        OverCnt++;
        if (OverCnt >= 20000)
            return 0;
    }
    if (ADChangeBit == 8)
        ADCResult = ADCResult >> 2;
    Random += ADCResult;
    return ADCResult;
}
unsigned int MCU_ADCRead(void)
{
    char Idx;
    unsigned int Ret = 0, Temp;
    __MCU_ADCRead();
    for (Idx = 0; Idx < 2; Idx++)
    {
        Temp = __MCU_ADCRead();
        if (Temp == 0)
            return 0;
        Ret += Temp;
    }
    return Ret >>= 1;
}
rtccTimeDate RtccTimeDate, RtccTimeDateVal;
int RTCCfg(void)                //Ã¨Â¿??¥â??ºÅ?RUE/FALSE
{
    RtccInitClock(); //turn on clock source
    //mRtccSetIntPriority(4);//set interrupt priority to 4
    //mRtccSetInt(1);//set interrupt
    RtccWrOn(); //enable RTCC peripheral
    mRtccOn();
    return 0;
}
int RTCReadTime(RTC_T* Rtc)     //Ã¨Â¿??¥â??ºÅ?RUE/FALSE
{
    RtccReadTimeDate(&RtccTimeDateVal);
    Rtc->Year = RtccTimeDateVal.f.year;
    Rtc->Mon = RtccTimeDateVal.f.mon;
    Rtc->Day = RtccTimeDateVal.f.mday;
    Rtc->Hour = RtccTimeDateVal.f.hour;
    Rtc->Min = RtccTimeDateVal.f.min;
    Rtc->Sec = RtccTimeDateVal.f.sec;
    Rtc->Week = RtccTimeDateVal.f.wday;
    if (RCON2bits.VBPOR == 1)
        return FALSE;
    return TRUE;
}
int RTCWriteTime(RTC_T* Rtc)    //Ã¨Â¿??¥â??ºÅ?RUE/FALSE
{
    int Time_Input_out_of_range ;
//    RtccReadTimeDate(&RtccTimeDateVal);
    RtccTimeDateVal.f.year = Rtc->Year;
    RtccTimeDateVal.f.mon = Rtc->Mon;
    RtccTimeDateVal.f.mday = Rtc->Day;
    RtccTimeDateVal.f.hour = Rtc->Hour;
    RtccTimeDateVal.f.min = Rtc->Min;
    RtccTimeDateVal.f.sec = Rtc->Sec;
    RtccTimeDateVal.f.wday = Rtc->Week;
    Time_Input_out_of_range = RtccWriteTimeDate(&RtccTimeDateVal, TRUE);
    RCON2bits.VBPOR = 0;
    return Time_Input_out_of_range;
}
void DelayUs(unsigned int Us)
{
    while (Us--)
    {
        Nop();  //4M:for( char i = 0; i < 2; i++ );
        //8M:NOP();
    }
}
void DelayMs(unsigned int Ms)
{
    int i;
    while (Ms)
    {
        for (i = 0; i < 800; i++)  //4M:400.8M:800
            ;
        Ms--;
    }
}
void CBB_NOP(void)
{
    Nop();
}
unsigned short GetRandom(void)
{
    return Random;
}

void Set_AllIOtoLow(void)
{
//    ANSA = 0;
//    TRISA = 0;
//    LATA = 0;
//    ODCA = 0;
    
    ANSB = 0;
    TRISB = 0;
    LATB = 0;
    ODCB = 0;

    TRISC = 0;
    LATC = 0;
    ODCC = 0;

    ANSD = 0;
    TRISD = 0;
    LATD = 0;
    ODCD = 0;

    ANSE = 0;
    TRISE = 0;
    LATE = 0;
    ODCE = 0;

    TRISF = 0;
    LATF = 0;
    ODCF = 0;

    ANSG = 0;
    TRISG = 0;
    LATG = 0;
    ODCG = 0;
}

