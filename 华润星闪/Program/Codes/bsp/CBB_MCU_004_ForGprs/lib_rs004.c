/*
                                                    ****************************
                                                            lib_rs004
                                                    ****************************

说明:       文件说明

        端口寄存器说明:
            • (PMxx):0 输出;1输入
            • (Pxx):端口电平寄存器
            • (PUxx):0不上拉,1上拉
            • (PIMxx):0 Normal input buffer//1 TTL input buffer
            • (POMxx):0 普通输出//1 N-ch open-drain 输出
            • (PMCxx):0 数字,1模拟
            • (ADPC):ANI0/P21, ANI1/P20 pins to digital I/O of port or analog input of A/D converter
            • (PIOR)
            • (PFSEG0 to PFSEG6):液晶端口
            • (ISCLCD)
            主频配置：
                2.4V-5.5V
                High-speedOCO(fiH)
                8MHz

需求:       对硬件或其它软件的需求
版本:       v0.0.0
版权:       威星智能仪表股份有限公司　版权所有，(c)2000-2050
修改记录:   --------------------------------------------------------------------
            日期:           负责人:     工作:           版本:
            XXXX-XX-XX      XXXX    创建/修改/增加/重写 v0.0.0
********************************************************************************
*/

#define     _lib_rs004_C_
#include 	"r_cg_macrodriver.h"
//#include    "config.h"

#include    "lib_rs004.h"

#include    "r_cg_cgc.h"
#include    "r_cg_lcd.h"
//#include    "r_cg_it.h"
#include    "r_cg_adc.h"
#include    "r_cg_rtc.h"
#include    "r_cg_tau.h"

static unsigned short Random;
void IrqServerDefaultRS(unsigned char Data){ ; }
void IrqServerDefault(void){ ; }

IRQServerFT IRQServerINT0 = IrqServerDefault;
IRQServerFT IRQServerINT1 = IrqServerDefault;
IRQServerFT IRQServerINT2 = IrqServerDefault;
IRQServerFT IRQServerINT3 = IrqServerDefault;
IRQServerFT IRQServerINT4 = IrqServerDefault;
IRQServerFT IRQServerINT5 = IrqServerDefault;
IRQServerFT IRQServerINT6 = IrqServerDefault;
IRQServerFT IRQServerINT7 = IrqServerDefault;
#pragma vector = INTP0_vect
__interrupt static void r_intc0_interrupt(void)
{
    EI();
    IRQServerINT0();
    Random++;
}

#pragma vector = INTP1_vect
__interrupt static void r_intc1_interrupt(void)
{
    EI();
    IRQServerINT1();
    Random++;
}

#pragma vector = INTP2_vect
__interrupt static void r_intc2_interrupt(void)
{
    EI();
    IRQServerINT2();
    Random++;
}

#pragma vector = INTP3_vect
__interrupt static void r_intc3_interrupt(void)
{
    EI();
    IRQServerINT3();
    Random++;
}

#pragma vector = INTP4_vect
__interrupt static void r_intc4_interrupt(void)
{
    EI();
    IRQServerINT4();
    Random++;
}

#pragma vector = INTP5_vect
__interrupt static void r_intc5_interrupt(void)
{
    EI();
    IRQServerINT5();
    Random++;
}

#pragma vector = INTP6_vect
__interrupt static void r_intc6_interrupt(void)
{
    EI();
    IRQServerINT6();
    Random++;
}

#pragma vector = INTP7_vect
__interrupt static void r_intc7_interrupt(void)
{
    EI();
    IRQServerINT7();
    Random++;
}

//Config: RISING_EDGE_INT/FALLING_EDGE_INT
void INT0Config(unsigned char Config, IRQServerFT Fuc)
{
    PMK0 = 1U;    /* disable INTP0 operation */
    PIF0 = 0U;    /* clear INTP0 interrupt flag */
    /* Set INTP0 low priority */
    PPR10 = 1U;
    PPR00 = 1U;
    if(Config == RISING_EDGE_INT)
    {


        EGP0 |= BIT0;	//INTP0上升沿中断
        EGN0 &= ~BIT0;	
    }
    else
    {


        EGP0 &= ~BIT0;	
        EGN0 |= BIT0;	//INTP0下降沿中断
    }
	IRQServerINT0 = Fuc;
}

void INT1Config(unsigned char Config, IRQServerFT Fuc)
{
    PMK1 = 1U;    /* disable INTP1 operation */
    PIF1 = 0U;    /* clear INTP1 interrupt flag */
    /* Set INTP1 low priority */
    PPR11 = 1U;
    PPR01 = 1U;
    if(Config == RISING_EDGE_INT)
    {


        EGP0 |= BIT1;	//INTP1上升沿中断
        EGN0 &= ~BIT1;	
    }
    else
    {


        EGP0 &= ~BIT1;	
        EGN0 |= BIT1;	//INTP1下降沿中断
    }
    /* Set INTP1 pin */
    PFSEG0 &= 0xBFU;
    PM5 |= 0x04U;
	IRQServerINT1 = Fuc;
}

void INT2Config(unsigned char Config, IRQServerFT Fuc)
{
    PMK2 = 1U;    /* disable INTP2 operation */
    PIF2 = 0U;    /* clear INTP2 interrupt flag */
    /* Set INTP2 low priority */
    PPR12 = 1U;
    PPR02 = 1U;
    if(Config == RISING_EDGE_INT)
    {

        EGP0 |= BIT2;	//INTP2上升沿中断
        EGN0 &= ~BIT2;	
    }
    else
    {


        EGP0 &= ~BIT2;	
        EGN0 |= BIT2;	//INTP2下降沿中断
    }
    /* Set INTP2 pin */
    PFSEG0 &= 0x7FU;
    PM5 |= 0x08U;
	IRQServerINT2 = Fuc;
}

void INT3Config(unsigned char Config, IRQServerFT Fuc)
{
    PMK3 = 1U;    /* disable INTP3 operation */
    PIF3 = 0U;    /* clear INTP3 interrupt flag */
    /* Set INTP3 low priority */
    PPR13 = 1U;
    PPR03 = 1U;
    if(Config == RISING_EDGE_INT)
    {


        EGP0 |= BIT3;	//INTP3上升沿中断
        EGN0 &= ~BIT3;	
    }
    else
    {


        EGP0 &= ~BIT3;	
        EGN0 |= BIT3;	//INTP3下降沿中断
    }
    /* Set INTP3 pin */
    PFSEG2 &= 0xDFU;
    PM3 |= 0x02U;
	IRQServerINT3 = Fuc;
}

void INT4Config(unsigned char Config, IRQServerFT Fuc)
{
    PMK4 = 1U;    /* disable INTP4 operation */
    PIF4 = 0U;    /* clear INTP4 interrupt flag */
    /* Set INTP4 low priority */
    PPR14 = 1U;
    PPR04 = 1U;
    if(Config == RISING_EDGE_INT)
    {


        EGP0 |= BIT4;	//INTP4上升沿中断
        EGN0 &= ~BIT4;
    }
    else
    {


        EGP0 &= ~BIT4;
        EGN0 |= BIT4;	//INTP4下降沿中断
    }
    /* Set INTP4 pin */
    PFSEG2 &= 0x7FU;
    PM3 |= 0x08U;
	IRQServerINT4 = Fuc;
}

void INT5Config(unsigned char Config, IRQServerFT Fuc)
{
    PMK5 = 1U;    /* disable INTP5 operation */
    PIF5 = 0U;    /* clear INTP5 interrupt flag */
    /* Set INTP5 low priority */
    PPR15 = 1U;
    PPR05 = 1U;
    if(Config == RISING_EDGE_INT)
    {
        EGP0 |= BIT5;	//INTP5上升沿中断
        EGN0 &= ~BIT5;
    }
    else
    {
        EGP0 &= ~BIT5;	
        EGN0 |= BIT5;	//INTP5下降沿中断
    }
    /* Set INTP5 pin */
    //PFSEG1 &= 0xFDU;
    //PM5 |= 0x20U;
	PFSEG5 &= 0xDFU;    //端口第二功能
	PM0 |= 0x02U;       //端口第二功能
	IRQServerINT5 = Fuc;
}

void INT6Config(unsigned char Config, IRQServerFT Fuc)
{
    PMK6 = 1U;    /* disable INTP6 operation */
    PIF6 = 0U;    /* clear INTP6 interrupt flag */
    /* Set INTP6 low priority */
    PPR16 = 1U;
    PPR06 = 1U;
    if(Config == RISING_EDGE_INT)
    {
        EGP0 |= BIT6;	//INTP6上升沿中断
        EGN0 &= ~BIT6;
    }
    else
    {
        EGP0 &= ~BIT6;
        EGN0 |= BIT6;	//INTP6下降沿中断
    }
    /* Set INTP6 pin */
    PFSEG1 &= 0xF7U;
    PM5 |= 0x80U;
	IRQServerINT6 = Fuc;
}

void INT7Config(unsigned char Config, IRQServerFT Fuc)
{
    PMK7 = 1U;    /* disable INTP7 operation */
    PIF7 = 0U;    /* clear INTP7 interrupt flag */
    /* Set INTP7 low priority */
    PPR17 = 1U;
    PPR07 = 1U;
    if(Config == RISING_EDGE_INT)
    {
        EGP0 |= BIT7;	//INTP7上升沿中断
        EGN0 &= ~BIT7;	
    }
    else
    {
        EGP0 &= ~BIT7;	
        EGN0 |= BIT7;	//INTP7下降沿中断
    }
    /* Set INTP7 pin */
    PFSEG5 &= 0xBFU;
    PM0 |= 0x04U;
	//PM4 |= 0x08U;  //端口第二功能
	IRQServerINT7 = Fuc;
}

/**************************************************************************/

IRQServerRecvDataFT IRQServerUart0Receive = IrqServerDefaultRS;
IRQServerRecvDataFT IRQServerUart1Receive = IrqServerDefaultRS;
IRQServerRecvDataFT IRQServerUart2Receive = IrqServerDefaultRS;
IRQServerRecvDataFT IRQServerUart3Receive = IrqServerDefaultRS;
#pragma vector = INTST0_vect
__interrupt static void r_uart0_interrupt_send(void)
{
    EI();
    Random++;
}

#pragma vector = INTST1_vect
__interrupt static void r_uart1_interrupt_send(void)
{
    EI();
    Random++;
}

#pragma vector = INTST2_vect
__interrupt static void r_uart2_interrupt_send(void)
{
    EI();
    Random++;
}

#pragma vector = INTST3_vect
__interrupt static void r_uart3_interrupt_send(void)
{
    EI();
    Random++;
}

#pragma vector = INTSR0_vect
__interrupt static void r_uart0_interrupt_receive(void)
{  
    unsigned char rx_data;
    EI();
    rx_data = RXD0;
    IRQServerUart0Receive(rx_data);
    Random += rx_data;
}

#pragma vector = INTSR1_vect
__interrupt static void r_uart1_interrupt_receive(void)
{  
    unsigned char rx_data;
    EI();
    rx_data = RXD1;
    IRQServerUart1Receive(rx_data);
    Random += rx_data;
}

#pragma vector = INTSR2_vect
__interrupt static void r_uart2_interrupt_receive(void)
{  
    unsigned char rx_data;
    EI();
    rx_data = RXD2;
    IRQServerUart2Receive(rx_data);
    Random += rx_data;
}

#pragma vector = INTSR3_vect
__interrupt static void r_uart3_interrupt_receive(void)
{  
    unsigned char rx_data;
    EI();
    rx_data = RXD3;
    IRQServerUart3Receive(rx_data);
    Random += rx_data;
}

void Uart0Config(pUartMode Mode, IRQServerSendDataFT SendFun, IRQServerRecvDataFT RecvFun)
{
    SAU0EN = 1U;    /* supply SAU0 clock */
    NOP();
    NOP();
    NOP();
    NOP();
    switch(Mode->Baud)
    {
    case UartBaud1200:
        SPS0 = SPS0 & 0xF0 | _0005_SAU_CK00_FCLK_5;
        SDR00 = _CE00_UART0_TRANSMIT_DIVISOR;
        SDR01 = _CE00_UART0_RECEIVE_DIVISOR;
        break;
    case UartBaud2400:
        SPS0 = SPS0 & 0xF0 | _0004_SAU_CK00_FCLK_4;
        SDR00 = _CE00_UART0_TRANSMIT_DIVISOR;
        SDR01 = _CE00_UART0_RECEIVE_DIVISOR;
        break;
    case UartBaud4800:
        SPS0 = SPS0 & 0xF0 | _0003_SAU_CK00_FCLK_3;
        SDR00 = _CE00_UART0_TRANSMIT_DIVISOR;
        SDR01 = _CE00_UART0_RECEIVE_DIVISOR;
        break;
    case UartBaud9600:
        SPS0 = SPS0 & 0xF0 | _0002_SAU_CK00_FCLK_2;
        SDR00 = _CE00_UART0_TRANSMIT_DIVISOR;
        SDR01 = _CE00_UART0_RECEIVE_DIVISOR;
        break;
    case UartBaud19200:
        SPS0 = SPS0 & 0xF0 | _0001_SAU_CK00_FCLK_1;
        SDR00 = _CE00_UART0_TRANSMIT_DIVISOR;
        SDR01 = _CE00_UART0_RECEIVE_DIVISOR;
        break;
    case UartBaud38400:
        SPS0 = SPS0 & 0xF0 | _0000_SAU_CK00_FCLK_0;
        SDR00 = _CE00_UART0_TRANSMIT_DIVISOR;
        SDR01 = _CE00_UART0_RECEIVE_DIVISOR;
        break;
    case UartBaud115200:
        SPS0 = SPS0 & 0xF0 | _0000_SAU_CK00_FCLK_0;
        SDR00 = _4400_UART0_TRANSMIT_DIVISOR;
        SDR01 = _4400_UART0_RECEIVE_DIVISOR;
        break;
    default:
        SPS0 = SPS0 & 0xF0 | _0002_SAU_CK00_FCLK_2;
        SDR00 = _CE00_UART0_TRANSMIT_DIVISOR;
        SDR01 = _CE00_UART0_RECEIVE_DIVISOR;
        break;
    }
    ST0 |= _0002_SAU_CH1_STOP_TRG_ON | _0001_SAU_CH0_STOP_TRG_ON;    /* disable UART0 receive and transmit */
    STMK0 = 1U;    /* disable INTST0 interrupt */
    STIF0 = 0U;    /* clear INTST0 interrupt flag */
    SRMK0 = 1U;    /* disable INTSR0 interrupt */
    SRIF0 = 0U;    /* clear INTSR0 interrupt flag */
    SREMK0 = 1U;   /* disable INTSRE0 interrupt */
    SREIF0 = 0U;   /* clear INTSRE0 interrupt flag */
    /* Set INTST0 low priority */
    STPR10 = 1U;
    STPR00 = 1U;
    /* Set INTSR0 low priority */
    SRPR10 = 1U;
    SRPR00 = 1U;
    SMR00 = _0020_SAU_SMRMN_INITIALVALUE | _0000_SAU_CLOCK_SELECT_CK00 | _0000_SAU_TRIGGER_SOFTWARE | 
            _0002_SAU_MODE_UART | _0000_SAU_TRANSFER_END;
    NFEN0 |= _01_SAU_RXD0_FILTER_ON;
    SIR01 = _0004_SAU_SIRMN_FECTMN | _0002_SAU_SIRMN_PECTMN | _0001_SAU_SIRMN_OVCTMN;    /* clear error flag */
    SMR01 = _0020_SAU_SMRMN_INITIALVALUE | _0000_SAU_CLOCK_SELECT_CK00 | _0100_SAU_TRIGGER_RXD | _0000_SAU_EDGE_FALL | 
            _0002_SAU_MODE_UART | _0000_SAU_TRANSFER_END;
    SCR00 = _8000_SAU_TRANSMISSION | _0000_SAU_INTSRE_MASK;
    SCR01 = _4000_SAU_RECEPTION | _0000_SAU_INTSRE_MASK;
    //奇偶检验位
    if(Mode->Parity == PARITY_ODD)
    {
        SCR00 = SCR00 | _0300_SAU_PARITY_ODD;
        SCR01 = SCR01 | _0300_SAU_PARITY_ODD;
    }
    else if(Mode->Parity == PARITY_EVEN)
    {
        SCR00 = SCR00 | _0200_SAU_PARITY_EVEN;
        SCR01 = SCR01 | _0200_SAU_PARITY_EVEN;
    }
    else
    {
        SCR00 = SCR00 | _0000_SAU_PARITY_NONE;
        SCR01 = SCR01 | _0000_SAU_PARITY_NONE;
    }
    //停止位
    if(Mode->StopBit == STOP_NONE)
    {
        SCR00 = SCR00 | _0000_SAU_STOP_NONE;
        SCR01 = SCR01 | _0000_SAU_STOP_NONE;
    }
    else if(Mode->StopBit == STOP_2)
    {
        SCR00 = SCR00 | _0020_SAU_STOP_2;
        SCR01 = SCR01 | _0020_SAU_STOP_2;
    }
    else
    {
        SCR00 = SCR00 | _0010_SAU_STOP_1;
        SCR01 = SCR01 | _0010_SAU_STOP_1;
    }
    //数据位
    if(Mode->DataBit == DATA_7)
    {
        SCR00 = SCR00 | _0006_SAU_LENGTH_7;
        SCR01 = SCR01 | _0006_SAU_LENGTH_7;
    }
    else if(Mode->DataBit == DATA_9)
    {
        SCR00 = SCR00 | _0005_SAU_LENGTH_9;
        SCR01 = SCR01 | _0005_SAU_LENGTH_9;
    }
    else
    {
        SCR00 = SCR00 | _0007_SAU_LENGTH_8;
        SCR01 = SCR01 | _0007_SAU_LENGTH_8;
    }
    if(Mode->Sequence == MSB)
    {
        SCR00 = SCR00 | _0000_SAU_MSB;
        SCR01 = SCR01 | _0000_SAU_MSB;
    }
    else
    {
        SCR00 = SCR00 | _0080_SAU_LSB;
        SCR01 = SCR01 | _0080_SAU_LSB;
    } 
    /* Set RxD0 pin */
    PFSEG5 &= 0xF7U;
    PM1 |= 0x80U;
	//PFSEG3 &= 0xEFU;  //端口第二功能
    //PM4 |= 0x80U;     //端口第二功能
    /* Set TxD0 pin */
    PFSEG5 &= 0xEFU;
    P0 |= 0x01U;
    PM0 &= 0xFEU;
	//PFSEG3 &= 0xDFU;  //端口第二功能
    //P13 |= 0x01U;     //端口第二功能
    //PM13 &= 0xFEU;    //端口第二功能
	IRQServerUart0Receive = RecvFun;
}

void Uart1Config(pUartMode Mode, IRQServerSendDataFT SendFun, IRQServerRecvDataFT RecvFun)
{
    SAU0EN = 1U;    /* supply SAU0 clock */
    NOP();
    NOP();
    NOP();
    NOP();
    switch(Mode->Baud)
    {
    case UartBaud1200:
        SPS0 = SPS0 & 0x0F | _0050_SAU_CK01_FCLK_5;
        SDR02 = _CE00_UART1_TRANSMIT_DIVISOR;
        SDR03 = _CE00_UART1_RECEIVE_DIVISOR;
        break;
    case UartBaud2400:
        SPS0 = SPS0 & 0x0F | _0040_SAU_CK01_FCLK_4;
        SDR02 = _CE00_UART1_TRANSMIT_DIVISOR;
        SDR03 = _CE00_UART1_RECEIVE_DIVISOR;
        break;
    case UartBaud4800:
        SPS0 = SPS0 & 0x0F | _0030_SAU_CK01_FCLK_3;
        SDR02 = _CE00_UART1_TRANSMIT_DIVISOR;
        SDR03 = _CE00_UART1_RECEIVE_DIVISOR;
        break;
    case UartBaud9600:
        SPS0 = SPS0 & 0x0F | _0020_SAU_CK01_FCLK_2;
        SDR02 = _CE00_UART1_TRANSMIT_DIVISOR;
        SDR03 = _CE00_UART1_RECEIVE_DIVISOR;
        break;
    case UartBaud19200:
        SPS0 = SPS0 & 0x0F | _0010_SAU_CK01_FCLK_1;
        SDR02 = _CE00_UART1_TRANSMIT_DIVISOR;
        SDR03 = _CE00_UART1_RECEIVE_DIVISOR;
        break;
    case UartBaud38400:
        SPS0 = SPS0 & 0x0F | _0000_SAU_CK01_FCLK_0;
        SDR02 = _CE00_UART1_TRANSMIT_DIVISOR;
        SDR03 = _CE00_UART1_RECEIVE_DIVISOR;
        break;
    case UartBaud115200:
        SPS0 = SPS0 & 0x0F | _0000_SAU_CK01_FCLK_0;
        SDR02 = _4400_UART1_TRANSMIT_DIVISOR;
        SDR03 = _4400_UART1_RECEIVE_DIVISOR;
        break;
    default:
        SPS0 = SPS0 & 0x0F | _0020_SAU_CK01_FCLK_2;
        SDR02 = _CE00_UART1_TRANSMIT_DIVISOR;
        SDR03 = _CE00_UART1_RECEIVE_DIVISOR;
        break;
    }
    ST0 |= _0008_SAU_CH3_STOP_TRG_ON | _0004_SAU_CH2_STOP_TRG_ON;    /* disable UART1 receive and transmit */
    STMK1 = 1U;    /* disable INTST1 interrupt */
    STIF1 = 0U;    /* clear INTST1 interrupt flag */
    SRMK1 = 1U;    /* disable INTSR1 interrupt */
    SRIF1 = 0U;    /* clear INTSR1 interrupt flag */
    SREMK1 = 1U;   /* disable INTSRE1 interrupt */
    SREIF1 = 0U;   /* clear INTSRE1 interrupt flag */
    /* Set INTST1 low priority */
    STPR11 = 1U;
    STPR01 = 1U;
    /* Set INTSR1 low priority */
    SRPR11 = 1U;
    SRPR01 = 1U;
    SMR02 = _0020_SAU_SMRMN_INITIALVALUE | _8000_SAU_CLOCK_SELECT_CK01 | _0000_SAU_TRIGGER_SOFTWARE | 
            _0002_SAU_MODE_UART | _0000_SAU_TRANSFER_END;
    NFEN0 |= _04_SAU_RXD1_FILTER_ON;
    SIR03 = _0004_SAU_SIRMN_FECTMN | _0002_SAU_SIRMN_PECTMN | _0001_SAU_SIRMN_OVCTMN;    /* clear error flag */
    SMR03 = _0020_SAU_SMRMN_INITIALVALUE | _8000_SAU_CLOCK_SELECT_CK01 | _0100_SAU_TRIGGER_RXD | _0000_SAU_EDGE_FALL | 
            _0002_SAU_MODE_UART | _0000_SAU_TRANSFER_END;
    SCR02 = _8000_SAU_TRANSMISSION | _0000_SAU_INTSRE_MASK;
    SCR03 = _4000_SAU_RECEPTION | _0000_SAU_INTSRE_MASK;
    //奇偶检验位
    if(Mode->Parity == PARITY_ODD)
    {
        SCR02 = SCR02 | _0300_SAU_PARITY_ODD;
        SCR03 = SCR03 | _0300_SAU_PARITY_ODD;
    }
    else if(Mode->Parity == PARITY_EVEN)
    {
        SCR02 = SCR02 | _0200_SAU_PARITY_EVEN;
        SCR03 = SCR03 | _0200_SAU_PARITY_EVEN;
    }
    else
    {
        SCR02 = SCR02 | _0000_SAU_PARITY_NONE;
        SCR03 = SCR03 | _0000_SAU_PARITY_NONE;
    }
    //停止位
    if(Mode->StopBit == STOP_NONE)
    {
        SCR02 = SCR02 | _0000_SAU_STOP_NONE;
        SCR03 = SCR03 | _0000_SAU_STOP_NONE;
    }
    else if(Mode->StopBit == STOP_2)
    {
        SCR02 = SCR02 | _0020_SAU_STOP_2;
        SCR03 = SCR03 | _0020_SAU_STOP_2;
    }
    else
    {
        SCR02 = SCR02 | _0010_SAU_STOP_1;
        SCR03 = SCR03 | _0010_SAU_STOP_1;
    }
    //数据位
    if(Mode->DataBit == DATA_7)
    {
        SCR02 = SCR02 | _0006_SAU_LENGTH_7;
        SCR03 = SCR03 | _0006_SAU_LENGTH_7;
    }
    else if(Mode->DataBit == DATA_9)
    {
        SCR02 = SCR02 | _0005_SAU_LENGTH_9;
        SCR03 = SCR03 | _0005_SAU_LENGTH_9;
    }
    else
    {
        SCR02 = SCR02 | _0007_SAU_LENGTH_8;
        SCR03 = SCR03 | _0007_SAU_LENGTH_8;
    }
    if(Mode->Sequence == MSB)
    {
        SCR02 = SCR02 | _0000_SAU_MSB;
        SCR03 = SCR03 | _0000_SAU_MSB;
    }
    else
    {
        SCR02 = SCR02 | _0080_SAU_LSB;
        SCR03 = SCR03 | _0080_SAU_LSB;
    } 
    /* Set RxD1 pin */
    PFSEG6 &= 0xFBU;
    PM0 |= 0x40U;
    /* Set TxD1 pin */
    PFSEG6 &= 0xF7U;
    P0 |= 0x80U;
    PM0 &= 0x7FU;
	IRQServerUart1Receive = RecvFun;
}

void Uart2Config(pUartMode Mode, IRQServerSendDataFT SendFun, IRQServerRecvDataFT RecvFun)
{
    SAU1EN = 1U;    /* supply SAU1 clock */
    NOP();
    NOP();
    NOP();
    NOP();
    switch(Mode->Baud)
    {
    case UartBaud1200:
        SPS1 = SPS1 & 0xF0 | _0005_SAU_CK00_FCLK_5;
        SDR10 = _CE00_UART2_TRANSMIT_DIVISOR;
        SDR11 = _CE00_UART2_RECEIVE_DIVISOR;
        break;
    case UartBaud2400:
        SPS1 = SPS1 & 0xF0 | _0004_SAU_CK00_FCLK_4;
        SDR10 = _CE00_UART2_TRANSMIT_DIVISOR;
        SDR11 = _CE00_UART2_RECEIVE_DIVISOR;
        break;
    case UartBaud4800:
        SPS1 = SPS1 & 0xF0 | _0003_SAU_CK00_FCLK_3;
        SDR10 = _CE00_UART2_TRANSMIT_DIVISOR;
        SDR11 = _CE00_UART2_RECEIVE_DIVISOR;
        break;
    case UartBaud9600:
        SPS1 = SPS1 & 0xF0 | _0002_SAU_CK00_FCLK_2;
        SDR10 = _CE00_UART2_TRANSMIT_DIVISOR;
        SDR11 = _CE00_UART2_RECEIVE_DIVISOR;
        break;
    case UartBaud19200:
        SPS1 = SPS1 & 0xF0 | _0001_SAU_CK00_FCLK_1;
        SDR10 = _CE00_UART2_TRANSMIT_DIVISOR;
        SDR11 = _CE00_UART2_RECEIVE_DIVISOR;
        break;
    case UartBaud38400:
        SPS1 = SPS1 & 0xF0 | _0000_SAU_CK00_FCLK_0;
        SDR10 = _CE00_UART2_TRANSMIT_DIVISOR;
        SDR11 = _CE00_UART2_RECEIVE_DIVISOR;
        break;
    case UartBaud115200:
        SPS1 = SPS1 & 0xF0 | _0000_SAU_CK00_FCLK_0;
        SDR10 = _4400_UART2_TRANSMIT_DIVISOR;
        SDR11 = _4400_UART2_RECEIVE_DIVISOR;
        break;
    default:
        SPS1 = SPS1 & 0xF0 | _0002_SAU_CK00_FCLK_2;
        SDR10 = _CE00_UART2_TRANSMIT_DIVISOR;
        SDR11 = _CE00_UART2_RECEIVE_DIVISOR;
        break;
    }
    ST1 |= _0002_SAU_CH1_STOP_TRG_ON | _0001_SAU_CH0_STOP_TRG_ON;    /* disable UART2 receive and transmit */
    STMK2 = 1U;    /* disable INTST2 interrupt */
    STIF2 = 0U;    /* clear INTST2 interrupt flag */
    SRMK2 = 1U;    /* disable INTSR2 interrupt */
    SRIF2 = 0U;    /* clear INTSR2 interrupt flag */
    SREMK2 = 1U;   /* disable INTSRE2 interrupt */
    SREIF2 = 0U;   /* clear INTSRE2 interrupt flag */
    /* Set INTST2 low priority */
    STPR12 = 1U;
    STPR02 = 1U;
    /* Set INTSR2 low priority */
    SRPR12 = 1U;
    SRPR02 = 1U;
    SMR10 = _0020_SAU_SMRMN_INITIALVALUE | _0000_SAU_CLOCK_SELECT_CK00 | _0000_SAU_TRIGGER_SOFTWARE | 
            _0002_SAU_MODE_UART | _0000_SAU_TRANSFER_END;
    NFEN0 |= _10_SAU_RXD2_FILTER_ON;
    SIR11 = _0004_SAU_SIRMN_FECTMN | _0002_SAU_SIRMN_PECTMN | _0001_SAU_SIRMN_OVCTMN;    /* clear error flag */
    SMR11 = _0020_SAU_SMRMN_INITIALVALUE | _0000_SAU_CLOCK_SELECT_CK00 | _0100_SAU_TRIGGER_RXD | _0000_SAU_EDGE_FALL | 
            _0002_SAU_MODE_UART | _0000_SAU_TRANSFER_END;
    SCR10 = _8000_SAU_TRANSMISSION | _0000_SAU_INTSRE_MASK;
    SCR11 = _4000_SAU_RECEPTION | _0000_SAU_INTSRE_MASK;
    //奇偶检验位
    if(Mode->Parity == PARITY_ODD)
    {
        SCR10 = SCR10 | _0300_SAU_PARITY_ODD;
        SCR11 = SCR11 | _0300_SAU_PARITY_ODD;
    }
    else if(Mode->Parity == PARITY_EVEN)
    {
        SCR10 = SCR10 | _0200_SAU_PARITY_EVEN;
        SCR11 = SCR11 | _0200_SAU_PARITY_EVEN;
    }
    else
    {
        SCR10 = SCR10 | _0000_SAU_PARITY_NONE;
        SCR11 = SCR11 | _0000_SAU_PARITY_NONE;
    }
    //停止位
    if(Mode->StopBit == STOP_NONE)
    {
        SCR10 = SCR10 | _0000_SAU_STOP_NONE;
        SCR11 = SCR11 | _0000_SAU_STOP_NONE;
    }
    else if(Mode->StopBit == STOP_2)
    {
        SCR10 = SCR10 | _0020_SAU_STOP_2;
        SCR11 = SCR11 | _0020_SAU_STOP_2;
    }
    else
    {
        SCR10 = SCR10 | _0010_SAU_STOP_1;
        SCR11 = SCR11 | _0010_SAU_STOP_1;
    }
    //数据位
    if(Mode->DataBit == DATA_7)
    {
        SCR10 = SCR10 | _0006_SAU_LENGTH_7;
        SCR11 = SCR11 | _0006_SAU_LENGTH_7;
    }
    else if(Mode->DataBit == DATA_9)
    {
        SCR10 = SCR10 | _0005_SAU_LENGTH_9;
        SCR11 = SCR11 | _0005_SAU_LENGTH_9;
    }
    else
    {
        SCR10 = SCR10 | _0007_SAU_LENGTH_8;
        SCR11 = SCR11 | _0007_SAU_LENGTH_8;
    }
    if(Mode->Sequence == MSB)
    {
        SCR10 = SCR10 | _0000_SAU_MSB;
        SCR11 = SCR11 | _0000_SAU_MSB;
    }
    else
    {
        SCR10 = SCR10 | _0080_SAU_LSB;
        SCR11 = SCR11 | _0080_SAU_LSB;
    } 
    /* Set RxD2 pin */
    PFSEG5 &= 0x7FU;
    PM0 |= 0x08U;
    /* Set TxD2 pin */
    PFSEG6 &= 0xFEU;
    P0 |= 0x10U;
    PM0 &= 0xEFU;
	IRQServerUart2Receive = RecvFun;
}

void Uart3Config(pUartMode Mode, IRQServerSendDataFT SendFun, IRQServerRecvDataFT RecvFun)
{
    SAU1EN = 1U;    /* supply SAU1 clock */
    NOP();
    NOP();
    NOP();
    NOP();
    switch(Mode->Baud)
    {
    case UartBaud1200:
        SPS1 = SPS1 & 0x0F | _0050_SAU_CK01_FCLK_5;
        SDR12 = _CE00_UART3_TRANSMIT_DIVISOR;
        SDR13 = _CE00_UART3_RECEIVE_DIVISOR;
        break;
    case UartBaud2400:
        SPS1 = SPS1 & 0x0F | _0040_SAU_CK01_FCLK_4;
        SDR12 = _CE00_UART3_TRANSMIT_DIVISOR;
        SDR13 = _CE00_UART3_RECEIVE_DIVISOR;
        break;
    case UartBaud4800:
        SPS1 = SPS1 & 0x0F | _0030_SAU_CK01_FCLK_3;
        SDR12 = _CE00_UART3_TRANSMIT_DIVISOR;
        SDR13 = _CE00_UART3_RECEIVE_DIVISOR;
        break;
    case UartBaud9600:
        SPS1 = SPS1 & 0x0F | _0020_SAU_CK01_FCLK_2;
        SDR12 = _CE00_UART3_TRANSMIT_DIVISOR;
        SDR13 = _CE00_UART3_RECEIVE_DIVISOR;
        break;
    case UartBaud19200:
        SPS1 = SPS1 & 0x0F | _0010_SAU_CK01_FCLK_1;
        SDR12 = _CE00_UART3_TRANSMIT_DIVISOR;
        SDR13 = _CE00_UART3_RECEIVE_DIVISOR;
        break;
    case UartBaud38400:
        SPS1 = SPS1 & 0x0F | _0000_SAU_CK01_FCLK_0;
        SDR12 = _CE00_UART3_TRANSMIT_DIVISOR;
        SDR13 = _CE00_UART3_RECEIVE_DIVISOR;
        break;
    case UartBaud115200:
        SPS1 = SPS1 & 0x0F | _0000_SAU_CK01_FCLK_0;
        SDR12 = _4400_UART3_TRANSMIT_DIVISOR;
        SDR13 = _4400_UART3_RECEIVE_DIVISOR;
        break;
    default:
        SPS1 = SPS1 & 0x0F | _0020_SAU_CK01_FCLK_2;
        SDR12 = _CE00_UART3_TRANSMIT_DIVISOR;
        SDR13 = _CE00_UART3_RECEIVE_DIVISOR;
        break;
    }
    ST1 |= _0008_SAU_CH3_STOP_TRG_ON | _0004_SAU_CH2_STOP_TRG_ON;    /* disable UART3 receive and transmit */
    STMK3 = 1U;    /* disable INTST3 interrupt */
    STIF3 = 0U;    /* clear INTST3 interrupt flag */
    SRMK3 = 1U;    /* disable INTSR3 interrupt */
    SRIF3 = 0U;    /* clear INTSR3 interrupt flag */
    SREMK3 = 1U;   /* disable INTSRE3 interrupt */
    SREIF3 = 0U;   /* clear INTSRE3 interrupt flag */
    /* Set INTST3 low priority */
    STPR13 = 1U;
    STPR03 = 1U;
    /* Set INTSR3 low priority */
    SRPR13 = 1U;
    SRPR03 = 1U;
    SMR12 = _0020_SAU_SMRMN_INITIALVALUE | _8000_SAU_CLOCK_SELECT_CK01 | _0000_SAU_TRIGGER_SOFTWARE | 
            _0002_SAU_MODE_UART | _0000_SAU_TRANSFER_END;
    NFEN0 |= _40_SAU_RXD3_FILTER_ON;
    SIR13 = _0004_SAU_SIRMN_FECTMN | _0002_SAU_SIRMN_PECTMN | _0001_SAU_SIRMN_OVCTMN;    /* clear error flag */
    SMR13 = _0020_SAU_SMRMN_INITIALVALUE | _8000_SAU_CLOCK_SELECT_CK01 | _0100_SAU_TRIGGER_RXD | _0000_SAU_EDGE_FALL | 
            _0002_SAU_MODE_UART | _0000_SAU_TRANSFER_END;
    SCR12 = _8000_SAU_TRANSMISSION | _0000_SAU_INTSRE_MASK;
    SCR13 = _4000_SAU_RECEPTION | _0000_SAU_INTSRE_MASK;
    //奇偶检验位
    if(Mode->Parity == PARITY_ODD)
    {
        SCR12 = SCR12 | _0300_SAU_PARITY_ODD;
        SCR13 = SCR13 | _0300_SAU_PARITY_ODD;
    }
    else if(Mode->Parity == PARITY_EVEN)
    {
        SCR12 = SCR12 | _0200_SAU_PARITY_EVEN;
        SCR13 = SCR13 | _0200_SAU_PARITY_EVEN;
    }
    else
    {
        SCR12 = SCR12 | _0000_SAU_PARITY_NONE;
        SCR13 = SCR13 | _0000_SAU_PARITY_NONE;
    }
    //停止位
    if(Mode->StopBit == STOP_NONE)
    {
        SCR12 = SCR12 | _0000_SAU_STOP_NONE;
        SCR13 = SCR13 | _0000_SAU_STOP_NONE;
    }
    else if(Mode->StopBit == STOP_2)
    {
        SCR12 = SCR12 | _0020_SAU_STOP_2;
        SCR13 = SCR13 | _0020_SAU_STOP_2;
    }
    else
    {
        SCR12 = SCR12 | _0010_SAU_STOP_1;
        SCR13 = SCR13 | _0010_SAU_STOP_1;
    }
    //数据位
    if(Mode->DataBit == DATA_7)
    {
        SCR12 = SCR12 | _0006_SAU_LENGTH_7;
        SCR13 = SCR13 | _0006_SAU_LENGTH_7;
    }
    else if(Mode->DataBit == DATA_9)
    {
        SCR12 = SCR12 | _0005_SAU_LENGTH_9;
        SCR13 = SCR13 | _0005_SAU_LENGTH_9;
    }
    else
    {
        SCR12 = SCR12 | _0007_SAU_LENGTH_8;
        SCR13 = SCR13 | _0007_SAU_LENGTH_8;
    }
    if(Mode->Sequence == MSB)
    {
        SCR12 = SCR12 | _0000_SAU_MSB;
        SCR13 = SCR13 | _0000_SAU_MSB;
    }
    else
    {
        SCR12 = SCR12 | _0080_SAU_LSB;
        SCR13 = SCR13 | _0080_SAU_LSB;
    } 
    /* Set RxD3 pin */
    PFSEG3 &= 0xFEU;
    PM3 |= 0x10U;
    /* Set TxD3 pin */
    //PFSEG3 &= 0xFDU;
    //PM3 &= 0xDFU;
    //P3 |= 0x20U;
	IRQServerUart3Receive = RecvFun;
}

void Uart0SendEnable(void)
{
    STIF0 = 0U;    /* clear INTST0 interrupt flag */
    STMK0 = 0U;    /* enable INTST0 interrupt */
    SO0 |= _0001_SAU_CH0_DATA_OUTPUT_1;    /* output level normal */
    SOE0 |= _0001_SAU_CH0_OUTPUT_ENABLE;    /* enable UART0 output */
    SS0 |= _0002_SAU_CH1_START_TRG_ON | _0001_SAU_CH0_START_TRG_ON;    /* enable UART0 receive and transmit */
}

void Uart0SendDisable(void)
{
    ST0 |= _0001_SAU_CH0_STOP_TRG_ON;    /* disable UART0 transmit */
    SOE0 &= ~_0001_SAU_CH0_OUTPUT_ENABLE;    /* disable UART0 output */
    STMK0 = 1U;    /* disable INTST0 interrupt */
    STIF0 = 0U;    /* clear INTST0 interrupt flag */
}

void Uart0ReceiveEnable(void)
{
    SRIF0 = 0U;    /* clear INTSR0 interrupt flag */
    SRMK0 = 0U;    /* enable INTSR0 interrupt */
    SO0 |= _0001_SAU_CH0_DATA_OUTPUT_1;    /* output level normal */
    SOE0 |= _0001_SAU_CH0_OUTPUT_ENABLE;    /* enable UART0 output */
    SS0 |= _0002_SAU_CH1_START_TRG_ON | _0001_SAU_CH0_START_TRG_ON;    /* enable UART0 receive and transmit */
}

void Uart0ReceiveDisable(void)
{
    ST0 |= _0002_SAU_CH1_STOP_TRG_ON;    /* disable UART0 receive */
    SRMK0 = 1U;    /* disable INTSR0 interrupt */
    SRIF0 = 0U;    /* clear INTSR0 interrupt flag */
}

void Uart1SendEnable(void)
{
    STIF1 = 0U;    /* clear INTST1 interrupt flag */
    STMK1 = 0U;    /* enable INTST1 interrupt */
    SO0 |= _0004_SAU_CH2_DATA_OUTPUT_1;    /* output level normal */
    SOE0 |= _0004_SAU_CH2_OUTPUT_ENABLE;    /* enable UART1 output */
    SS0 |= _0008_SAU_CH3_START_TRG_ON | _0004_SAU_CH2_START_TRG_ON;    /* enable UART1 receive and transmit */
}

void Uart1SendDisable(void)
{
    ST0 |= _0004_SAU_CH2_STOP_TRG_ON;    /* disable UART1 transmit */
    SOE0 &= ~_0004_SAU_CH2_OUTPUT_ENABLE;    /* disable UART1 output */
    STMK1 = 1U;    /* disable INTST1 interrupt */
    STIF1 = 0U;    /* clear INTST1 interrupt flag */
}

void Uart1ReceiveEnable(void)
{
    SRIF1 = 0U;    /* clear INTSR1 interrupt flag */
    SRMK1 = 0U;    /* enable INTSR1 interrupt */
    SO0 |= _0004_SAU_CH2_DATA_OUTPUT_1;    /* output level normal */
    SOE0 |= _0004_SAU_CH2_OUTPUT_ENABLE;    /* enable UART1 output */
    SS0 |= _0008_SAU_CH3_START_TRG_ON | _0004_SAU_CH2_START_TRG_ON;    /* enable UART1 receive and transmit */
}

void Uart1ReceiveDisable(void)
{
    ST0 |= _0008_SAU_CH3_STOP_TRG_ON;    /* disable UART1 receive */
    SRMK1 = 1U;    /* disable INTSR1 interrupt */
    SRIF1 = 0U;    /* clear INTSR1 interrupt flag */
}

void Uart2SendEnable(void)
{
    STIF2 = 0U;    /* clear INTST2 interrupt flag */
    STMK2 = 0U;    /* enable INTST2 interrupt */
    SO1 |= _0001_SAU_CH0_DATA_OUTPUT_1;    /* output level normal */
    SOE1 |= _0001_SAU_CH0_OUTPUT_ENABLE;    /* enable UART2 output */
    SS1 |= _0002_SAU_CH1_START_TRG_ON | _0001_SAU_CH0_START_TRG_ON;    /* enable UART2 receive and transmit */
}

void Uart2SendDisable(void)
{
    ST1 |= _0001_SAU_CH0_STOP_TRG_ON;    /* disable UART2 transmit */
    SOE1 &= ~_0001_SAU_CH0_OUTPUT_ENABLE;    /* disable UART2 output */
    STMK2 = 1U;    /* disable INTST2 interrupt */
    STIF2 = 0U;    /* clear INTST2 interrupt flag */
}

void Uart2ReceiveEnable(void)
{
    SRIF2 = 0U;    /* clear INTSR2 interrupt flag */
    SRMK2 = 0U;    /* enable INTSR2 interrupt */
    SO1 |= _0001_SAU_CH0_DATA_OUTPUT_1;    /* output level normal */
    SOE1 |= _0001_SAU_CH0_OUTPUT_ENABLE;    /* enable UART2 output */
    SS1 |= _0002_SAU_CH1_START_TRG_ON | _0001_SAU_CH0_START_TRG_ON;    /* enable UART2 receive and transmit */
}

void Uart2ReceiveDisable(void)
{
    ST1 |= _0002_SAU_CH1_STOP_TRG_ON;    /* disable UART2 receive */
    SRMK2 = 1U;    /* disable INTSR2 interrupt */
    SRIF2 = 0U;    /* clear INTSR2 interrupt flag */
}

void Uart3SendEnable(void)
{
    STIF3 = 0U;    /* clear INTST3 interrupt flag */
    STMK3 = 0U;    /* enable INTST3 interrupt */
    SO1 |= _0004_SAU_CH2_DATA_OUTPUT_1;    /* output level normal */
    SOE1 |= _0004_SAU_CH2_OUTPUT_ENABLE;    /* enable UART3 output */
    SS1 |= _0008_SAU_CH3_START_TRG_ON | _0004_SAU_CH2_START_TRG_ON;    /* enable UART3 receive and transmit */
}

void Uart3SendDisable(void)
{
    ST1 |= _0004_SAU_CH2_STOP_TRG_ON;    /* disable UART3 transmit */
    SOE1 &= ~_0004_SAU_CH2_OUTPUT_ENABLE;    /* disable UART3 output */
    STMK3 = 1U;    /* disable INTST3 interrupt */
    STIF3 = 0U;    /* clear INTST3 interrupt flag */
}

void Uart3ReceiveEnable(void)
{
    SRIF3 = 0U;    /* clear INTSR3 interrupt flag */
    SRMK3 = 0U;    /* enable INTSR3 interrupt */
    SO1 |= _0004_SAU_CH2_DATA_OUTPUT_1;    /* output level normal */
    SOE1 |= _0004_SAU_CH2_OUTPUT_ENABLE;    /* enable UART3 output */
    SS1 |= _0008_SAU_CH3_START_TRG_ON | _0004_SAU_CH2_START_TRG_ON;    /* enable UART3 receive and transmit */
}

void Uart3ReceiveDisable(void)
{
    ST1 |= _0008_SAU_CH3_STOP_TRG_ON;    /* disable UART3 receive */
    SRMK3 = 1U;    /* disable INTSR3 interrupt */
    SRIF3 = 0U;    /* clear INTSR3 interrupt flag */
}

int Uart0SendData(unsigned char*pSendBuf,unsigned int Len)
{
    unsigned short i;
    while(Len)
    {
        i = 0x1fff;


        while(SSR00L & 0x20)
        {
            i --;
            if(i == 0)
                return 0;
        }
        TXD0 = *pSendBuf ++;
        Len --;
    }
    return 1;
}

int Uart1SendData(unsigned char*pSendBuf,unsigned int Len)
{
    unsigned short i;
    while(Len)
    {
        i = 0x1fff;


        while(SSR02L & 0x20)
        {
            i --;
            if(i == 0)
                return 0;
        }
        TXD1 = *pSendBuf ++;
        Len --;
    }
    return 1;
}

int Uart2SendData(unsigned char*pSendBuf,unsigned int Len)
{
    unsigned short i;
    while(Len)
    {
        i = 0x1fff;


        while(SSR10L & 0x20)
        {
            i --;
            if(i == 0)
                return 0;
        }
        TXD2 = *pSendBuf ++;
        Len --;
    }
    return 1;
}

int Uart3SendData(unsigned char*pSendBuf,unsigned int Len)
{
    unsigned short i;
    while(Len)
    {
        i = 0x1fff;


        while(SSR12L & 0x20)
        {
            i --;
            if(i == 0)
                return 0;
        }
        TXD3 = *pSendBuf ++;
        Len --;
    }
    return 1;
}
/*
*******************************************************
*******************************************************
*******************************************************
*******************************************************完成
说    明:     进入睡眠
*/
void MCU_SleepEnter(void)
{
    NOP();  /* Add one instruction delay */
    STOP(); /* Enter STOP mode */
    NOP();  /* Add one instruction delay */
}
void MCU_IntEn(void)
{
    EI();//IE = 1;// 
}
void MCU_IntDis(void)
{
    DI(); //IE = 0;//
}
/*
*******************************************************完成
说    明:     复位函数
*/
void MCU_Reset(void)
{
    WDTE = 0x00;
}
/*
*******************************************************完成
说    明:     看门狗
*/
void MCU_DogCfg(void)               //初始化
{
    //void R_WDT_Create(void)
    {
        WDTIMK = 1U;    /* disable INTWDTI interrupt */
        WDTIIF = 0U;    /* clear INTWDTI interrupt flag */
    }
}
void MCU_DogReset(void)             //喂狗
{
    //void R_WDT_Restart(void)
    {
        WDTE = 0xACU;     /* restart watchdog timer */
    }
}
/*
*******************************************************完成
说    明:     系统速度配置,仅支持8MHz模式
*/
void MCUPllCfg(void)
{
    //void R_CGC_Create(void)
    {
        volatile uint16_t w_count;

        /* Set fMX */
        CMC = _00_CGC_HISYS_PORT | _10_CGC_SUB_OSC | _00_CGC_SUBMODE_LOW;
        MSTOP = 1U;
        /* Set fMAIN */
        MCM0 = 0U;
        /* Set fSUB */
        XTSTOP = 0U;

        /* Change the waiting time according to the system */
        for (w_count = 0U; w_count <= CGC_SUBWAITTIME; w_count++)
        {
            NOP();
        }

        OSMC = _00_CGC_SUBINHALT_ON | _00_CGC_RTC_IT_LCD_CLK_FSUB;
        /* Set fCLK */
        CSS = 0U;
        /* Set fIH */
        HIOSTOP = 0U;
    }
    //void R_Systeminit(void)
    {
        //PIOR = 0x12U;
        PIOR = 0x01U;   //第二功能端口
        CRC0CTL = 0x00U;
        IAWCTL = 0x00U;
        PMS = 0x00U;
    }
}
/*
*******************************************************完成
说    明:     LCD驱动
MCU,SETG:	0	1	2	3	4	5	6	7	8	9	10	12	13	14	15	16	17	18	19	20	22	COM0	COM1	COM2	COM3
液晶SEG:    0	1	2	3	4	5	6	7	8	9	10	11	12	13	14	15	16	17	18	19	20	COM0	COM1	COM2	COM3
*/
void MCU_LcdCfg(void)   //液晶初始化
{
    //void R_LCD_Create(void)
    {
        LCDON = 0U;    /* disable LCD clock operation */
        LCDMK0 = 1U;    /* disable INTLCD0 interrupt */
        LCDIF0 = 0U;    /* clear INTLCD0 interrupt flag */
        LCDM0 = _00_LCD_DISPLAY_WAVEFORM_A | _0D_LCD_DISPLAY_MODE1;
        LCDM0 |= _00_LCD_VOLTAGE_MODE_EXTERNAL;
        //{   //根据硬件将相应端口配置为液晶口//在bsp中实现
        //    PU0 &= 0x0EU;
        //    PU1 &= 0x03U;
        //    PU5 &= 0x0CU;
        //    POM0 &= 0x0EU;
        //    POM1 &= 0x3FU;
        //    POM5 &= 0x8FU;
        //    PIM0 &= 0x9FU;
        //    PIM1 &= 0x3FU;
        //    PIM5 &= 0xDFU;
        //    PFSEG0 |= 0x30U;
        //    PFSEG1 |= 0x0FU;
        //    PFSEG4 |= 0xC0U;
        //    PFSEG5 |= 0x1FU;
        //    PFSEG6 |= 0x0FU;
        //    P0 &= 0x0EU;
        //    PM0 &= 0x0EU;
        //    PMC1 &= 0xF3U;
        //    P1 &= 0x03U;
        //    PM1 &= 0x03U;
        //    P5 &= 0x0CU;
        //    PM5 &= 0x0CU;
        //}
        LCDM1 |= _00_LCD_DISPLAY_PATTERN_A;
        LCDC0 = _07_LCD_CLOCK_FSUB_FIL_8;
    }
}
void MCU_LcdOpen(void)  //打开液晶
{
    //void R_LCD_Start(void)
    {
        LCDON = 1U;
    }
    //void R_LCD_Voltage_On(void)
    {
        SCOC = 1U;
    }
}
void MCU_LcdClose(void) //关闭液晶
{
    //void R_LCD_Voltage_Off(void)
    {
        SCOC = 0U;
    }
    //void R_LCD_Stop(void)
    {
        LCDON = 0U;
    }
}
void MCU_LcdMemSet(unsigned char Reg)
{
    SEG0 = Reg;
    SEG1 = Reg;
    SEG2 = Reg;
    SEG3 = Reg;
    SEG4 = Reg;
    SEG5 = Reg;
    SEG6 = Reg;
    SEG7 = Reg;
    SEG8 = Reg;
    SEG9 = Reg;
    SEG10 = Reg;
    SEG12 = Reg;
    SEG13 = Reg;
    SEG14 = Reg;
    SEG15 = Reg;
    SEG16 = Reg;
    SEG17 = Reg;
    SEG18 = Reg;
    SEG19 = Reg;
    SEG20 = Reg;
    SEG22 = Reg;
}
void MCU_LcdAllSet(void) //关闭液晶
{
    MCU_LcdMemSet(0x0f);
}
void MCU_LcdAllClr(void) //关闭液晶
{
    MCU_LcdMemSet(0x00);
}

void MCU_LcdWriteReg(unsigned char Seg, unsigned char Reg)
{
    switch (Seg)
    {
    case 0:        SEG0 = Reg;    break;
    case 1:        SEG1 = Reg;    break;
    case 2:        SEG2 = Reg;    break;
    case 3:        SEG3 = Reg;    break;
    case 4:        SEG4 = Reg;    break;
    case 5:        SEG5 = Reg;    break;
    case 6:        SEG6 = Reg;    break;
    case 7:        SEG7 = Reg;    break;
    case 8:        SEG8 = Reg;    break;
    case 9:        SEG9 = Reg;    break;
    case 10:       SEG10 = Reg;    break;
    case 11:       SEG12 = Reg;    break;
    case 12:       SEG13 = Reg;    break;
    case 13:       SEG14 = Reg;    break;
    case 14:       SEG15 = Reg;    break;
    case 15:       SEG16 = Reg;    break;
    case 16:       SEG17 = Reg;    break;
    case 17:       SEG18 = Reg;    break;
    case 18:       SEG19 = Reg;    break;
    case 19:       SEG20 = Reg;    break;
    case 20:       SEG22 = Reg;    break;
    }
}
unsigned char MCU_LcdReadReg(unsigned char Seg)
{
    switch (Seg)
    {
    case 0:        return SEG0 ;
    case 1:        return SEG1 ;
    case 2:        return SEG2 ;
    case 3:        return SEG3 ;
    case 4:        return SEG4 ;
    case 5:        return SEG5 ;
    case 6:        return SEG6 ;
    case 7:        return SEG7 ;
    case 8:        return SEG8 ;
    case 9:        return SEG9 ;
    case 10:       return SEG10;
    case 11:       return SEG12;
    case 12:       return SEG13;
    case 13:       return SEG14;
    case 14:       return SEG15;
    case 15:       return SEG16;
    case 16:       return SEG17;
    case 17:       return SEG18;
    case 18:       return SEG19;
    case 19:       return SEG20;
    case 20:       return SEG22;
    default:       return SEG22;
    }
}
/*
*******************************************************完成
说    明:     定时器驱动
PortVmNum0:使用IntervalTimer时钟,支持1ms,100ms
PortVmNum8:使用IntervalTimer时钟,支持1ms,10ms,20ms,100ms
PortVmNum9:使用Real-time定时器,支持0.5秒,1秒.
*/
#define ServerIntervalNum   1
IRQServerFT IRQServerTM0[ServerIntervalNum] = { IrqServerDefault };
IRQServerFT IRQServerIntervalTimer[ServerIntervalNum] = { IrqServerDefault };
IRQServerFT IRQServerRealTime[ServerIntervalNum] = { IrqServerDefault };
#pragma vector = INTTM00_vect
__interrupt static void r_tau0_channel0_interrupt(void)
{
    char i;
    EI();
    for (i = 0; i < ServerIntervalNum; i++)
        IRQServerTM0[i]();
    Random++;
}
#pragma vector = INTIT_vect
__interrupt static void r_it_interrupt(void)
{
    char i;
    EI();
    for (i = 0; i < ServerIntervalNum; i++)
        IRQServerIntervalTimer[i]();
    Random++;
}
#pragma vector = INTRTC_vect
__interrupt static void r_rtc_interrupt(void)
{
    char i;
    EI();
    if (1U == WAFG)
    {
        RTCWEN = 1U;
        RTCC1 &= (uint8_t)~_10_RTC_ALARM_MATCH;        /* clear WAFG */
        RTCWEN = 0U;
    }

    if (1U == RIFG)
    {
        RTCWEN = 1U;
        RTCC1 &= (uint8_t)~_08_RTC_INTC_GENERATE_FLAG;    /* clear RIFG */
        RTCWEN = 0U;
    }
    for (i = 0; i < ServerIntervalNum; i++)
        IRQServerRealTime[i]();
    Random++;
}

void MCU_TimerCfg(PortNumET TimeNo, TimeSpET Sp, IRQServerFT TimeServer)
{
    char i;
    switch (TimeNo)
    {
    case PortVmNum0:
        for (i = 0; i < ServerIntervalNum; i++)
        {
            if (IRQServerTM0[i] == IrqServerDefault)
                IRQServerTM0[i] = TimeServer;
        }
        //void R_TAU0_Create(void)
        {
            TAU0EN = 1U;    /* supplies input clock */
            switch (Sp)
            {
            case TimeSpMs1:
                TPS0 = _0000_TAU_CKM0_fCLK_0 | _0000_TAU_CKM1_fCLK_0 | _0000_TAU_CKM2_fCLK_1 | _0000_TAU_CKM3_fCLK_8;
                break;
            case TimeSpMs100:
                TPS0 = _0004_TAU_CKM0_fCLK_4 | _0000_TAU_CKM1_fCLK_0 | _0000_TAU_CKM2_fCLK_1 | _0000_TAU_CKM3_fCLK_8;
                break;
            }
            /* Stop all channels */
            TT0 = _0001_TAU_CH0_STOP_TRG_ON | _0002_TAU_CH1_STOP_TRG_ON | _0004_TAU_CH2_STOP_TRG_ON |
                _0008_TAU_CH3_STOP_TRG_ON | _0010_TAU_CH4_STOP_TRG_ON | _0020_TAU_CH5_STOP_TRG_ON |
                _0040_TAU_CH6_STOP_TRG_ON | _0080_TAU_CH7_STOP_TRG_ON | _0200_TAU_CH1_H8_STOP_TRG_ON |
                _0800_TAU_CH3_H8_STOP_TRG_ON;
            /* Mask channel 0 interrupt */
            TMMK00 = 1U;    /* disable INTTM00 interrupt */
            TMIF00 = 0U;    /* clear INTTM00 interrupt flag */
            /* Mask channel 1 interrupt */
            TMMK01 = 1U;    /* disable INTTM01 interrupt */
            TMIF01 = 0U;    /* clear INTTM01 interrupt flag */
            /* Mask channel 1 higher 8 bits interrupt */
            TMMK01H = 1U;    /* disable INTTM01H interrupt */
            TMIF01H = 0U;    /* clear INTTM01H interrupt flag */
            /* Mask channel 2 interrupt */
            TMMK02 = 1U;    /* disable INTTM02 interrupt */
            TMIF02 = 0U;    /* clear INTTM02 interrupt flag */
            /* Mask channel 3 interrupt */
            TMMK03 = 1U;    /* disable INTTM03 interrupt */
            TMIF03 = 0U;    /* clear INTTM03 interrupt flag */
            /* Mask channel 3 higher 8 bits interrupt */
            TMMK03H = 1U;    /* disable INTTM03H interrupt */
            TMIF03H = 0U;    /* clear INTTM03H interrupt flag */
            /* Mask channel 4 interrupt */
            TMMK04 = 1U;    /* disable INTTM04 interrupt */
            TMIF04 = 0U;    /* clear INTTM04 interrupt flag */
            /* Mask channel 5 interrupt */
            TMMK05 = 1U;    /* disable INTTM05 interrupt */
            TMIF05 = 0U;    /* clear INTTM05 interrupt flag */
            /* Mask channel 6 interrupt */
            TMMK06 = 1U;    /* disable INTTM06 interrupt */
            TMIF06 = 0U;    /* clear INTTM06 interrupt flag */
            /* Mask channel 7 interrupt */
            TMMK07 = 1U;    /* disable INTTM07 interrupt */
            TMIF07 = 0U;    /* clear INTTM07 interrupt flag */
            /* Set INTTM00 low priority */
            TMPR100 = 1U;
            TMPR000 = 1U;
            /* Channel 0 used as interval timer */
            TMR00 = _0000_TAU_CLOCK_SELECT_CKM0 | _0000_TAU_CLOCK_MODE_CKS | _0000_TAU_COMBINATION_SLAVE |
                _0000_TAU_TRIGGER_SOFTWARE | _0000_TAU_MODE_INTERVAL_TIMER | _0000_TAU_START_INT_UNUSED;
            switch (Sp)
            {
            case TimeSpMs1:
                TDR00 = (0x1F3FU);
                break;
            case TimeSpMs100:
                TDR00 = (0xC34FU);
                break;
            }
            TO0 &= ~_0001_TAU_CH0_OUTPUT_VALUE_1;
            TOE0 &= ~_0001_TAU_CH0_OUTPUT_ENABLE;
        }
        break;
    case PortVmNum1:break;
    case PortVmNum8://使用IntervalTimer定时器
        for (i = 0; i < ServerIntervalNum; i++)
        {
            if (IRQServerIntervalTimer[i] == IrqServerDefault)
                IRQServerIntervalTimer[i] = TimeServer;
        }
    //void R_IT_Create(void)
    {
                        TMKAEN = 1U;    /* supply IT clock */
                        ITMC = _0000_IT_OPERATION_DISABLE;    /* disable IT operation */
                        TMKAMK = 1U;    /* disable INTIT interrupt */
                        TMKAIF = 0U;    /* clear INTIT interrupt flag */
                        /* Set INTIT low priority */
                        TMKAPR1 = 1U;
                        TMKAPR0 = 1U;

                        switch (Sp)
                        {
                        case TimeSpMs1:
                            ITMC = _0020_ITMCMP_VALUE;
                            break;
                        case TimeSpMs10:
                            ITMC = _0147_ITMCMP_VALUE;
                            break;
                        case TimeSpMs20:
                            ITMC = _028E_ITMCMP_VALUE;
                            break;
                        case TimeSpMs100:
                            ITMC = (0x0CCCU);
                            break;
                        }
    }
        break;
    case PortVmNum9://使用Real-time定时器
        for (i = 0; i < ServerIntervalNum; i++)
        {
            if (IRQServerRealTime[i] == IrqServerDefault)
                IRQServerRealTime[i] = TimeServer;
        }
        //void R_RTC_Create(void)
    {
                        RTCWEN = 1U;   /* enables input clock supply */
                        RTCE = 0U;     /* disable RTC clock operation */
                        RTCMK = 1U;    /* disable INTRTC interrupt */
                        RTCIF = 0U;    /* clear INTRTC interrupt flag */
                        RTITMK = 1U;   /* disable INTRTIT interrupt */
                        RTITIF = 0U;   /* clear INTRTIT interrupt flag */
                        /* Set INTRTC low priority */
                        RTCPR1 = 1U;
                        RTCPR0 = 1U;
                        switch (Sp)
                        {
                        case TimeSpMs500:
                            RTCC0 = _00_RTC_RTC1HZ_DISABLE | _08_RTC_24HOUR_SYSTEM | _01_RTC_INTRTC_CLOCK_0;
                            break;
                        case TimeSpMs1000:
                            RTCC0 = _00_RTC_RTC1HZ_DISABLE | _08_RTC_24HOUR_SYSTEM | _02_RTC_INTRTC_CLOCK_1;
                            break;
                        case TimeSpMs2000:
                            break;
                        }
                        /* Set real-time clock */
//                        SEC = _00_RTC_COUNTER_SEC;
//                        MIN = _00_RTC_COUNTER_MIN;
//                        HOUR = _00_RTC_COUNTER_HOUR;
//                        WEEK = _02_RTC_COUNTER_WEEK;
//                        DAY = _11_RTC_COUNTER_DAY;
//                        MONTH = _11_RTC_COUNTER_MONTH;
//                        YEAR = _14_RTC_COUNTER_YEAR;
                        /* Set alarm function */
                        WALE = 0U;     /* invalidate alarm match operation */
                        WALIE = 1U;    /* generate INTRTC on matching of alarm */
                        RTCWEN = 0U;    /* stops input clock supply */
    }
        break;
    }
}
void MCU_TimerStart(PortNumET TimeNo)
{
    switch (TimeNo)
    {
    case PortVmNum0:
        //void R_TAU0_Channel0_Start(void)
        {
            TMIF00 = 0U;    /* clear INTTM00 interrupt flag */
            TMMK00 = 0U;    /* enable INTTM00 interrupt */
            TS0 |= _0001_TAU_CH0_START_TRG_ON;
        }
        break;
    case PortVmNum1:break;
    case PortVmNum8:
        //void R_IT_Start(void)
        {
            TMKAIF = 0U;    /* clear INTIT interrupt flag */
            TMKAMK = 0U;    /* enable INTIT interrupt */
            ITMC |= _8000_IT_OPERATION_ENABLE;    /* enable IT operation */
        }
        break;
    case PortVmNum9:
        //void R_RTC_Operation_Start(void)
        {
            unsigned int WaitCnt = 0;
            //void R_RTC_Start(void)
            {
                RTCWEN = 1U;   /* enables input clock supply */
                RTCIF = 0U;    /* clear INTRTC interrupt flag */
                RTCMK = 0U;    /* enable INTRTC interrupt */
                RTCE = 1U;     /* enable RTC clock operation */
                RTCWEN = 0U;   /* stops input clock supply */
            }

            RTCWEN = 1U;    /* enables input clock supply */

            /* ---- Wait after a RTCE bit setup ---- */
            RWAIT = 1;

            while (RWST != 1)
            {
                WaitCnt++;
                if (WaitCnt > 5000)
                {
                    WaitCnt++;
                    break;
                }
                /* Do Nothing */
            }

            RWAIT = 0;
            WaitCnt = 0;
            while (RWST != 0)
            {
                WaitCnt++;
                if (WaitCnt > 5000)
                {
                    WaitCnt++;
                    break;
                }
                /* Do Nothing */
            }

            RTCWEN = 0U;    /* stops input clock supply */
        }
        break;
    }
}
void MCU_TimerStop(PortNumET TimeNo)
{
    switch (TimeNo)
    {
    case PortVmNum0:
        //void R_TAU0_Channel0_Stop(void)
        {
            TT0 |= _0001_TAU_CH0_STOP_TRG_ON;
            /* Mask channel 0 interrupt */
            TMMK00 = 1U;    /* disable INTTM00 interrupt */
            TMIF00 = 0U;    /* clear INTTM00 interrupt flag */
        }
        break;
    case PortVmNum1:break;
    case PortVmNum8:
        //void R_IT_Stop(void)
        {
            TMKAMK = 1U;    /* disable INTIT interrupt */
            TMKAIF = 0U;    /* clear INTIT interrupt flag */
            ITMC &= (uint16_t)~_8000_IT_OPERATION_ENABLE;    /* disable IT operation */
        }
        break;
    case PortVmNum9:
        //void R_RTC_Stop(void)
        {
            RTCWEN = 1U;  /* enables input clock supply */
            RTCE = 0U;    /* disable RTC clock operation */
            RTCMK = 1U;   /* disable INTRTC interrupt */
            RTCIF = 0U;   /* clear INTRTC interrupt flag */
            RTCWEN = 0U;  /* stops input clock supply */
        }
        break;
    }
}
/*
*******************************************************完成
说    明:     AD采样
如果采用8bit精度,使用内部基准
如果采用10bit精度,使用VDD基准
*/
unsigned short AdcRefVolt = 1450;//ADC采集基准,单位mV
unsigned char ADCStaIsBusy = FALSE;
unsigned int ADCResult; //保存最近一次ADC结果
void MCU_ADCCfg(PortNumET ADCNum, ADCModeST Mode)   //
{
    //void R_ADC_Create(void)
    {
        ADCEN = 1U;  /* supply AD clock */
        ADM0 = _00_AD_ADM0_INITIALVALUE;  /* disable AD conversion and clear ADM0 register */
        ADMK = 1U;  /* disable INTAD interrupt */
        ADIF = 0U;  /* clear INTAD interrupt flag */
        /* Set INTAD low priority */
        ADPR1 = 1U;
        ADPR0 = 1U;
        switch (ADCNum)
        {
        case PortVmNum0:
        {
                           /* Set ANI0 pin */
                           PM2 |= 0x02U;
        }
            break;
        case PortVmNum1:
        {
                           /* Set ANI0 - ANI1 pin */
                           PM2 |= 0x03U;
        }
            break;
        case PortVmNum16:
        {
                            /* Set ANI16 pin */
                            PFSEG3 |= 0x40U;
                            PMC2 |= 0x04U;
                            PM2 |= 0x04U;
        }
            break;
        case PortVmNum17:
        {
                            /* Set ANI17 pin */
                            PFSEG3 |= 0x80U;
                            PMC2 |= 0x08U;
                            PM2 |= 0x08U;
        }
            break;
        }
        if (Mode.ADCModeOpt == 0)
        {   //单次模式
            ADM1 = _00_AD_TRIGGER_SOFTWARE | _20_AD_CONVMODE_ONESELECT;
        }
        else
        {
            //多次模式
            ADM1 = _00_AD_TRIGGER_SOFTWARE | _00_AD_CONVMODE_CONSELECT;
        }
        if (Mode.ADCModeResolution == 0)
        {   //8bit,内部基准
            ADM0 = _10_AD_CONVERSION_CLOCK_16 | _04_AD_TIME_MODE_LOWVOLTAGE_1;
            ADM2 = _80_AD_POSITIVE_INTERVOLT | _00_AD_NEGATIVE_VSS | _00_AD_AREA_MODE_1 | _01_AD_RESOLUTION_8BIT;
            AdcRefVolt = 1450;
        }
        else
        {//10bit,VDD基准
            ADM0 = _00_AD_CONVERSION_CLOCK_64 | _04_AD_TIME_MODE_LOWVOLTAGE_1;
            ADM2 = _00_AD_POSITIVE_VDD | _00_AD_NEGATIVE_VSS | _00_AD_AREA_MODE_1 | _00_AD_RESOLUTION_10BIT;
            AdcRefVolt = 3300;
        }
        ADUL = _FF_AD_ADUL_VALUE;
        ADLL = _00_AD_ADLL_VALUE;
        switch (ADCNum)
        {
        case PortVmNum0:
            //ANI0
            ADS = _00_AD_INPUT_CHANNEL_0; break;
        case PortVmNum1:
            //ANI1
            ADS = _01_AD_INPUT_CHANNEL_1; break;
        case PortVmNum16:
            //ANI16
            ADS = _10_AD_INPUT_CHANNEL_16; break;
        case PortVmNum17:
            //ANI17
            ADS = _11_AD_INPUT_CHANNEL_17; break;
        }
    }
}
void MCU_ADCStart(PortNumET ADCNum)
{
    ADCStaIsBusy = TRUE;
    //void R_ADC_Set_OperationOn(void)
    {
        ADCE = 1U;  /* enable AD comparator */
    }
    //void R_ADC_Start(void)
    {
        ADIF = 0U;  /* clear INTAD interrupt flag */
        ADMK = 0U;  /* enable INTAD interrupt */
        ADCS = 1U;  /* enable AD conversion */
    }
}
void MCU_ADCStop(PortNumET ADCNum)
{
    ADCStaIsBusy = FALSE;
    //void R_ADC_Stop(void)
    {
        ADCS = 0U;  /* disable AD conversion */
        ADMK = 1U;  /* disable INTAD interrupt */
        ADIF = 0U;  /* clear INTAD interrupt flag */
    }
    //void R_ADC_Set_OperationOff(void)
    {
        ADCE = 0U;  /* disable AD comparator */
    }
    //void R_ADC_Set_PowerOff(void)
    {
        ADCEN = 0U;  /* stop AD clock */
    }
}
unsigned short OverCnt = 0;
unsigned int MCU_ADCRead(void)
{
    //unsigned short OverCnt = 0;
    while (ADCStaIsBusy == TRUE)
    {
        OverCnt++;
        if (OverCnt >= 15000)
        {
            OverCnt = 0;
            return 0;
        }
    }
    OverCnt = 0;
    return ADCResult;
}
#pragma vector = INTAD_vect
__interrupt static void r_adc_interrupt(void)
{
    EI();
    /* Start user code. Do not edit comment generated here */
    //ADCResult = (unsigned int)ADCRH;
    ADCResult = (unsigned int)ADCR >> 6;
    /* End user code. Do not edit comment generated here */
    ADCStaIsBusy = FALSE;
    Random += ADCResult;
}
/*
*******************************************************完成
说    明:     实时时钟
*/
MD_STATUS R_RTC_Get_CounterValuemy(rtc_counter_value_t * const counter_read_val)
{
    MD_STATUS status = MD_OK;
    volatile uint32_t  w_count;
    
    RTCWEN = 1U;    /* enables input clock supply */
    RTCC1 |= _01_RTC_COUNTER_PAUSE;

    /* Change the waiting time according to the system */
    for (w_count = 0U; w_count < RTC_WAITTIME; w_count++)
    {
        NOP();
    }

    if (0U == RWST)
    {
        status = MD_BUSY1;
    }
    else
    {
        counter_read_val->sec = SEC;
        counter_read_val->min = MIN;
        counter_read_val->hour = HOUR;
        counter_read_val->week = WEEK;
        counter_read_val->day = DAY;
        counter_read_val->month = MONTH;
        counter_read_val->year = YEAR;

        RTCC1 &= (uint8_t)~_01_RTC_COUNTER_PAUSE;

        /* Change the waiting time according to the system */
        for (w_count = 0U; w_count < RTC_WAITTIME; w_count++)
        {
            NOP();
        }

        if (1U == RWST)
        {
            status = MD_BUSY2;
        }
    }

    RTCWEN = 0U;    /* stops input clock supply */

    return (status);
}

int RTCCfg(void)                //返回TRUE/FALSE
{
    //void R_RTC_Create(void)
    {
        RTCWEN = 1U;   /* enables input clock supply */
        RTCE = 0U;     /* disable RTC clock operation */
        RTCMK = 1U;    /* disable INTRTC interrupt */
        RTCIF = 0U;    /* clear INTRTC interrupt flag */
        RTITMK = 1U;   /* disable INTRTIT interrupt */
        RTITIF = 0U;   /* clear INTRTIT interrupt flag */
        /* Set INTRTC low priority */
        RTCPR1 = 1U;
        RTCPR0 = 1U;
        //switch (Sp)
        //{
        //case TimeSpMs500:
        //    RTCC0 = _00_RTC_RTC1HZ_DISABLE | _08_RTC_24HOUR_SYSTEM | _01_RTC_INTRTC_CLOCK_0;
        //    break;
        //case TimeSpMs1000:
            RTCC0 = _00_RTC_RTC1HZ_DISABLE | _08_RTC_24HOUR_SYSTEM | _02_RTC_INTRTC_CLOCK_1;
        //    break;
        //case TimeSpMs2000:
        //}
        /* Set real-time clock */
        //SEC = _00_RTC_COUNTER_SEC;
        //MIN = _00_RTC_COUNTER_MIN;
        //HOUR = _00_RTC_COUNTER_HOUR;
        //WEEK = _02_RTC_COUNTER_WEEK;
        //DAY = _11_RTC_COUNTER_DAY;
        //MONTH = _11_RTC_COUNTER_MONTH;
        //YEAR = _14_RTC_COUNTER_YEAR;
        /* Set alarm function */
        WALE = 0U;     /* invalidate alarm match operation */
        WALIE = 1U;    /* generate INTRTC on matching of alarm */
        RTCWEN = 0U;    /* stops input clock supply */
    }
    MCU_TimerStart(PortVmNum9);
    return 0;
}
MD_STATUS R_RTC_Set_CounterValuemy(rtc_counter_value_t counter_write_val)
{
    MD_STATUS status = MD_OK;
    volatile uint32_t  w_count;
    
    RTCWEN = 1U;    /* enables input clock supply */
    RTCC1 |= _01_RTC_COUNTER_PAUSE;

    /* Change the waiting time according to the system */
    for (w_count = 0U; w_count < RTC_WAITTIME; w_count++)
    {
        NOP();
    }

    if (0U == RWST)
    {
        status = MD_BUSY1;
    }
    else
    {
        SEC = counter_write_val.sec;
        MIN = counter_write_val.min;
        HOUR = counter_write_val.hour;
        WEEK = counter_write_val.week;
        DAY = counter_write_val.day;
        MONTH = counter_write_val.month;
        YEAR = counter_write_val.year;
        RTCC1 &= (uint8_t)~_01_RTC_COUNTER_PAUSE;

        /* Change the waiting time according to the system */
        for (w_count = 0U; w_count < RTC_WAITTIME; w_count++)
        {
            NOP();
        }

        if (1U == RWST)
        {
            status = MD_BUSY2;
        }
    }

    RTCWEN = 0U;    /* stops input clock supply */

    return (status);
}

int RTCReadTime(RTC_T* Rtc)     //返回TRUE/FALSE
{
    rtc_counter_value_t InterRtc;
    if (R_RTC_Get_CounterValuemy(&InterRtc) == MD_OK)//需要多尝试
    {
        Rtc->Year = InterRtc.year;
        Rtc->Mon = InterRtc.month;
        Rtc->Day = InterRtc.day;
        Rtc->Week = InterRtc.week;
        Rtc->Hour = InterRtc.hour;// & 0x3f;
        Rtc->Min = InterRtc.min;
        Rtc->Sec = InterRtc.sec;
        return TRUE;
    }
    return FALSE;
}
int RTCWriteTime(RTC_T* Rtc)    //返回TRUE/FALSE
{
    rtc_counter_value_t InterRtc;
    InterRtc.year = Rtc->Year;
    InterRtc.month = Rtc->Mon;
    InterRtc.day = Rtc->Day;
    InterRtc.week = Rtc->Week;
    InterRtc.hour = Rtc->Hour;
    InterRtc.min = Rtc->Min;
    InterRtc.sec = Rtc->Sec;
    if (R_RTC_Set_CounterValuemy(InterRtc) == MD_OK)
        return TRUE;
    return FALSE;
}

void DelayUs(unsigned int Us)
{
    while (Us--)
    {
        NOP();  //4M:for( char i = 0; i < 2; i++ );
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
    NOP();
}
unsigned short GetRandom(void)
{
    return Random;
}
