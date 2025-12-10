#include "include.h"
#include "APC340R.h"

unsigned short RFTiming, RFRecv;
unsigned char  RF_Hz_Set, RF_Status;       // 定时器

void RF_DelayUs(unsigned short us)
{
    unsigned int t;
    while(us--)
    {
        for( t = 0; t < 100; t++)
            ;
    }
}

/*******************************************************************************
** Descriptions:        RF 模块引脚初始化
**     2013.11.14 匹配安美通
*******************************************************************************/
void RF_IO_Init(void)
{
    RF_SETA_Init(IN);
    RF_SETB_Init(OUT);
    RF_AUX_Init(IN);
    RF_POW_Init(OUT);
}

/*******************************************************************************
** Function name:       RF_INT
** Descriptions:        RF相关参数初始化
** input parameters:
** output parameters:   无
** Returned value:      无
*******************************************************************************/
unsigned char RF_Init(void)
{
    UartMode mode;
   
    memset(&RF_Buffer, 0, sizeof(RF_Buffer));
    RF_Buffer.RxLen = 0;
    RF_Buffer.TxLen = 0;
    RF_Buffer.UartFlag.Bit.RxFinished = TRUE;
    RF_Buffer.UartFlag.Bit.TxFinished = FALSE;
    RF_Buffer.State = Recv_Fream_New;

    RF_IO_Init();  /* RF模块初始化 */
    RFPowerOn();//上电

    mode.Baud = UartBaud57600;
    mode.DataBit = DATA_8;
    mode.Parity = PARITY_NONE;
    mode.Sequence = LSB;
    mode.StopBit = STOP_1;
    RFUart_Init(&mode, RFUartRecv);

    return TRUE;
}

/******************************************************************************
** Function name:       RF_ChangeToNormal
** Descriptions:        RF 切换到正常模式
*******************************************************************************/
void RF_ChangeToNormal(void)
{
    RF_SETA_Clr();
    RF_SETB_Clr();
    RF_DelayUs(50);
}
/******************************************************************************
** Function name:       RF_ChangeToWakeUp
** Descriptions:        RF 切换到唤醒模式
*******************************************************************************/
void RF_ChangeToWakeUp(void)
{
    RF_SETA_Clr();
    RF_SETB_Set();
    RF_DelayUs(50);
}
/*******************************************************************************
    RF软件控制
*******************************************************************************/
void RF_SW_FSM(void)
{
    RF_Frame_Receive(&RF_Buffer);
    if(RF_Buffer.UartFlag.Bit.TxFinished == SET)
    {
#ifdef DEBUG
        Printf_DebugMessage("\r\nRF Recv Data: ");
        HexToASCII(DebugMsgBuff, RF_Buffer.TXBuffer, RF_Buffer.TxLen );
        Printf_DebugMessage(DebugMsgBuff);
#endif
        WiFiUartSend(RF_Buffer.TXBuffer, RF_Buffer.TxLen );
        RF_Buffer.UartFlag.Bit.TxFinished = false;
    }
}


unsigned char RF_HWStatus = RFS_INIT;
/*******************************************************************************
    RF硬件控制
*******************************************************************************/
void Task_RF_HW(void)
{
    if(RFTiming != 0)
        return;
    switch(RF_HWStatus)
    {
        case RFS_INIT:
            //RF_Init();
            RFTiming = 100;//100ms
            RF_HWStatus = RFS_GFSK;
            break;
/*-----------------------------切换到 GFSK 模式---------------------------*/
        case RFS_GFSK:
            RFPowerOff();//掉电
            RF_SETB_Set();
            RFTiming = 100;//100ms
            RF_HWStatus = RFS_GFSK1;
            break;

        case RFS_GFSK1:
            RFPowerOn();//上电
            RFTiming = 2000;//100ms
            RF_HWStatus = RFS_HOST;
            break;
/*----------------------------切换到 LORA 模式---------------------------*/
        case RFS_LORA:
            RFPowerOff();//掉电
            RF_SETB_Clr();
            RFTiming = 100;//100ms
            RF_HWStatus = RFS_LORA1;
            break;

        case RFS_LORA1:
            RFPowerOn();//上电
            RFTiming = 2000;//100ms
            RF_HWStatus = RFS_HOST;
            break;
/*-------------------------------------------------------------------------*/
        case RFS_HOST:
            if(RFIsBusy() == TRUE)
                return;
            RF_HWStatus = RFS_IDLE;
            break;
        case RFS_IDLE:
        default:
            break;
    }
}
/*************************************************************************
 * Function Name: Usart3IntrHandler
 * Parameters: none
 *
 * Return: none
 *
 * Description: Usart 3 interrupt handler
 * RF口
 *************************************************************************/
void RFUartRecv(unsigned char ch)
{
        RF_Buffer.UART_Buff[RF_Buffer.BufWrite] = ch;
        RF_Buffer.BufWrite++;
        if(RF_Buffer.BufWrite >= UART_BUFF_LEN)
            RF_Buffer.BufWrite = 0;
}


/*******************************************************************************
**  End Of File
*******************************************************************************/
