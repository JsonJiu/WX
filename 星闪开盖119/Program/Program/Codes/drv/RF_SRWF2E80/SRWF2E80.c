#include "include.h"
#include "SRWF2E80.h"

unsigned short RFTiming, RFRecv;
unsigned char  RF_Hz_Set, RF_Status;       // 定时器

unsigned int RFBusyTimes = 0;//RF BUSY状态检测
unsigned char RF_HWStatus = RFS_RST;

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
    RFPowerOff();//掉电

    mode.Baud = UartBaud9600;
    mode.DataBit = DATA_8;
    mode.Parity = PARITY_NONE;
    mode.Sequence = LSB;
    mode.StopBit = STOP_1;
    RFUart_Init(&mode, RFUartRecv);

    return TRUE;
}
//检测RF状态，长时间未忙则复位RF
unsigned char CheckRFBusy(void)
{
    if(RFIsBusy() == TRUE)
    {
        RFBusyTimes++;
        if(RFBusyTimes > 2000)//任务 5ms运行一次，5*2000 = 5S,5S模块仍超时则重启RF
        {
            RFBusyTimes = 0;
            RF_HWStatus = RFS_RST;
        }
        return TRUE;
    }
    else
    {
        RFBusyTimes = 0;
        return FALSE;
    }
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
unsigned char Hostdata[] = {0x55, 0xAA, 0x0A, 0x17, 0x00, 0x01, 0x01, 0xA9, 0x77, 0x16};
/*******************************************************************************
    RF硬件控制
*******************************************************************************/
void Task_RF_HW(void)
{
    //设置模块为主机模式的数据
    
    if(RFTiming != 0)
        return;
    switch(RF_HWStatus)
    {
        case RFS_RST:
            //RF_Init();
            RFTiming = 100;//100ms
            RF_HWStatus = RFS_GFSK;
            break;
/*-----------------------------切换到 GFSK 模式---------------------------*/
        case RFS_GFSK:
            RFPowerOff();//掉电
            RF_SETB_Set();
            RFTiming = 2000;//100ms
            RF_HWStatus = RFS_GFSK1;
            break;

        case RFS_GFSK1:
            RFPowerOn();//上电
            RFTiming = 200;//100ms
            RF_HWStatus = RFS_HOST;
            break;
/*----------------------------切换到 LORA 模式---------------------------*/
        case RFS_LORA:
            RFPowerOff();//掉电
            RF_SETB_Clr();
            RFTiming = 2000;//100ms
            RF_HWStatus = RFS_LORA1;
            break;

        case RFS_LORA1:
            RFPowerOn();//上电
            RFTiming = 200;//100ms
            RF_HWStatus = RFS_HOST;
            break;
/*-------------------------------------------------------------------------*/
        case RFS_HOST:
            if(RFIsBusy() == TRUE)
                return;
            RFUartSend(Hostdata,10);//设为主机模式
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
