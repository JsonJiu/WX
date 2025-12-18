#include "config.h"
#include "drv_uart.h"

unsigned char TaskNum_Debug_UartRecv_OverTime = 0;
unsigned char TaskNum_UP_UartRecv_OverTime = 0;
unsigned char TaskNum_Meter_UartRecv_OverTime = 0;
unsigned char TaskNum_IR_UartRecv_OverTime = 0;

PROTOCOL_S MeterBuf, UpBuf, DebugUartBuf, IrdaBuf ;

void Init_DebugUsart(void)
{
   
    if(TaskNum_Debug_UartRecv_OverTime == 0)
    {
        TaskNum_Debug_UartRecv_OverTime = User_AskTimeNumber();
    }
    OM_Open_Usart(Debug_COM);
}

void Init_UpUsart(void)
{
    if(TaskNum_UP_UartRecv_OverTime == 0)
    {
        TaskNum_UP_UartRecv_OverTime = User_AskTimeNumber();
    }
    OM_Open_Usart(UP_COM);
}

void Init_MeterUsart(void)
{
    if(TaskNum_Meter_UartRecv_OverTime == 0)
    {
        TaskNum_Meter_UartRecv_OverTime = User_AskTimeNumber();
    }
    OM_Open_Usart(Meter_COM);
}

void Init_IrdaUsart(void)
{
    if(TaskNum_IR_UartRecv_OverTime == 0)
    {
        TaskNum_IR_UartRecv_OverTime = User_AskTimeNumber();
    }
    OM_Open_Usart(IR_COM);
}

/**
* 打开指定的串口
*
* @author wb (2014-11-06)
*
* @param usart_num :串口号
*/
void OM_Open_Usart(USARTNUM_EU usart_num)
{
    if(usart_num == Meter_COM)
    {
        UsartConfig(usart_num, Baud9600, OM_MeterUart_interrupt_receive);
    }
    else if(usart_num == UP_COM)
    {
        UsartConfig(usart_num, Baud9600, OM_UPUart_interrupt_receive);
    }
    else if(usart_num == Debug_COM)
    {
        UsartConfig(usart_num, Baud9600, OM_DebugUart_interrupt_receive);
    }
    else if(usart_num == IR_COM )
    {
        UsartConfig(usart_num, Baud9600, OM_IrdaUart_interrupt_receive);
    }
    OpenUsart(usart_num);
}

/**
* 关闭指定的串口
*
* @author wb (2014-11-06)
*
* @param usart_num  :串口号
*/
void OM_Close_Usart(USARTNUM_EU usart_num)
{
    CloseUsart(usart_num);
    if(usart_num == COM1)
    {
        COM1_TX_IN;
        COM1_RX_IN;
    }
    else if(usart_num == COM2)
    {
        COM2_TX_IN;
        COM2_RX_IN;
    }
    else if(usart_num == COM3)
    {
        COM3_TX_IN;
        COM3_RX_IN;
    }
    else if(usart_num == COM4 )
    {
        COM4_TX_IN;
        COM4_RX_IN;
    }
}

/**
* 向指定的串口发送数据
*
* @author wb (2014-11-19)
*
* @param usart_num ：串口号
* @param tx_databuf ：数据缓冲区
* @param tx_datalen ：数据长度
*
* @return unsigned char :返回1-成功；0-失败；
*/
unsigned char OM_SendUsartData(USARTNUM_EU usart_num, unsigned char *tx_databuf,unsigned short tx_datalen)
{
    unsigned char dat;
    dat = UsartSendData(usart_num, tx_databuf, tx_datalen);
    return dat;
}

/**
* 调试串口接收中断服务函数接口
*
* @author wb (2014-11-06)
*
* @param rxd ：串口接收的数据
*/
void OM_MeterUart_interrupt_receive (unsigned char rxd)
{
    MeterBuf.RxDataBuff[MeterBuf.p_RxDataWrite] = rxd;
    MeterBuf.p_RxDataWrite++;
    MeterBuf.Recv_OverTime = 2;
    //User_StartTime(MAINTIMER, TaskNum_Meter_UartRecv_OverTime, 10, OM_MeterUsart_RecvData_OverTimeTask);
    if(MeterBuf.p_RxDataWrite >= RXBUFF_SIZE)
        MeterBuf.p_RxDataWrite=0;
}

/**
* 调试串口接收数据超时任务
*
* @author wb (2014-11-20)
*/
void OM_MeterUsart_RecvData_OverTimeTask(void)
{
    if(MeterBuf.Recv_OverTime)
    {
        MeterBuf.Recv_OverTime--;
    }
    else
    {
        User_CloseTime(TaskNum_Meter_UartRecv_OverTime);
    }
}
/**
 * 侦测调试串口一帧数据内容
 * @param buf:接收一帧数据内容
 * @return：返回一帧数据长度
 */
//unsigned short OM_DetectDebugUsartFrameData(unsigned char *buf)
//{
//    unsigned short len = 0;
//    if(MeterBuf.Recv_OverTime == 0)
//    {
//        while(MeterBuf.p_RxDataWrite != MeterBuf.p_RxDataRead)
//        {
//            buf[len++] = MeterBuf.RxDataBuff[MeterBuf.p_RxDataRead++];
//            if(MeterBuf.p_RxDataRead >= DEBUGRXBUFF_SIZE) MeterBuf.p_RxDataRead = 0;
//        }
//    }
//    return len;
//}

/**
* UP 串口接收中断服务函数接口
*
* @author wb (2014-11-20)
*
* @param rxd :串口接收的数据
*/
void OM_UPUart_interrupt_receive (unsigned char rxd)
{
    UpBuf.RxDataBuff[UpBuf.p_RxDataWrite] = rxd;
    UpBuf.p_RxDataWrite++;
    UpBuf.Recv_OverTime = 2;
    //User_StartTime(MAINTIMER, TaskNum_UP_UartRecv_OverTime, 10, OM_UPUsart_RecvData_OverTimeTask);
    if(UpBuf.p_RxDataWrite >= RXBUFF_SIZE)
        UpBuf.p_RxDataWrite=0;
}
/**
* UP串口接收数据超时任务
*
* @author wb (2014-11-20)
*/

void OM_UPUsart_RecvData_OverTimeTask(void)
{
    if(UpBuf.Recv_OverTime)
    {
        UpBuf.Recv_OverTime--;
    }
    else
    {
        User_CloseTime(TaskNum_UP_UartRecv_OverTime);
    }
}


/**
 * 超声波串口接收中断
 * @param rxd
 */
void OM_DebugUart_interrupt_receive (unsigned char rxd)
{
    DebugUartBuf.RxDataBuff[DebugUartBuf.p_RxDataWrite] = rxd;
    DebugUartBuf.p_RxDataWrite++;
    DebugUartBuf.Recv_OverTime = 2;
    //User_StartTime(MAINTIMER, TaskNum_Debug_UartRecv_OverTime, 10, OM_DebugUsart_RecvData_OverTimeTask);
    if(DebugUartBuf.p_RxDataWrite >= RXBUFF_SIZE)
        DebugUartBuf.p_RxDataWrite=0;
}

/**
* UP串口接收数据超时任务
*
* @author wb (2014-11-20)
*/
void OM_DebugUsart_RecvData_OverTimeTask(void)
{
    if(DebugUartBuf.Recv_OverTime)
    {
        DebugUartBuf.Recv_OverTime--;
    }
    else
    {
        User_CloseTime(TaskNum_Debug_UartRecv_OverTime);
    }
}

/**
 * 调试串口数据处理解析
 */
void DebugUartDataDealProcess(void)
{
    unsigned char fun;
    unsigned char buf[100];
    unsigned short len = 0;

    fun = Get_WX68_Frame(&DebugUartBuf);//侦测串口一帧数据
    if(fun == F_WX_PTR)
    {
        OM_SendUsartData(Debug_COM, DebugBuf, sprintf((char*)DebugBuf,"\r\n调试命令↓↓↓：\r\n"));
        OM_SendUsartData(Debug_COM, DebugBuf, HexToAscii_Space(DebugUartBuf.RecvCmdBuf, DebugBuf, DebugUartBuf.p_RecvCmdData));

        len = PCUartProtocol_DataAanaly(DebugUartBuf.RecvCmdBuf, buf);
    }
}

/**
 * 红外光电通讯
 * @param rxd
 */
void OM_IrdaUart_interrupt_receive (unsigned char rxd)
{
    IrdaBuf.RxDataBuff[IrdaBuf.p_RxDataWrite] = rxd;
    IrdaBuf.p_RxDataWrite++;
    IrdaBuf.Recv_OverTime = 2;
    User_StartTime(MAINTIMER, TaskNum_IR_UartRecv_OverTime, 10, OM_IrdaUsart_RecvData_OverTimeTask);
    if(IrdaBuf.p_RxDataWrite >= RXBUFF_SIZE) IrdaBuf.p_RxDataWrite=0;
}

/**
* UP串口接收数据超时任务
*
* @author wb (2014-11-20)
*/
void OM_IrdaUsart_RecvData_OverTimeTask(void)
{
    if(IrdaBuf.Recv_OverTime)
    {
        IrdaBuf.Recv_OverTime--;
    }
    else
    {
        User_CloseTime(TaskNum_IR_UartRecv_OverTime);
    }
}

unsigned short Get_WX68_Frame(PROTOCOL_S* RecvBuf)
{
    unsigned char  dat;
    while(RecvBuf->p_RxDataWrite != RecvBuf->p_RxDataRead)
    {
    	dat = RecvBuf->RxDataBuff[RecvBuf->p_RxDataRead++];
    	if(RecvBuf->p_RxDataRead >= RXBUFF_SIZE)
            RecvBuf->p_RxDataRead = 0;

    	if(!RecvBuf->f_RecvCmdHead)
    	{
            if(dat == 0x68)//威星协议
            {
                RecvBuf->f_RecvCmdHead = 1;
                RecvBuf->RecvCmdBuf[RecvBuf->p_RecvCmdData] = dat;
                RecvBuf->p_RecvCmdData = 1;
                RecvBuf->RecvCmdCheckSum = dat;
                RecvBuf->RecvCmdLen = 11;
            }
            else
            {
                RecvBuf->f_RecvCmdHead = 0;
            }
    	}
    	else
    	{
            if(RecvBuf->p_RecvCmdData < 11)
            {
                RecvBuf->RecvCmdBuf[RecvBuf->p_RecvCmdData++] = dat;
                RecvBuf->RecvCmdCheckSum += dat;
                if(RecvBuf->p_RecvCmdData >= 11)
                {
                    RecvBuf->RecvCmdLen = RecvBuf->RecvCmdBuf[10];
                    RecvBuf->RecvCmdLen <<= 8;
                    RecvBuf->RecvCmdLen += RecvBuf->RecvCmdBuf[9];
                    if(RecvBuf->RecvCmdLen > RXBUFF_SIZE)
                    {
                        RecvBuf->f_RecvCmdHead = 0;
                        return ERR_LEN;
                    }
                    RecvBuf->RecvCmdLen += 11;
                }
            }
            else if(RecvBuf->p_RecvCmdData < RecvBuf->RecvCmdLen)
            {
                RecvBuf->RecvCmdBuf[RecvBuf->p_RecvCmdData++] = dat;
                RecvBuf->RecvCmdCheckSum += dat;
            }
            else if(RecvBuf->p_RecvCmdData == RecvBuf->RecvCmdLen)
            {
                if(RecvBuf->RecvCmdCheckSum == dat)
                {
                    RecvBuf->RecvCmdBuf[RecvBuf->p_RecvCmdData++] = dat;
                }
                else
                {
                    RecvBuf->f_RecvCmdHead = 0;
                    return ERR_CS; //和校验不对
                }
            }
            else if(RecvBuf->p_RecvCmdData == RecvBuf->RecvCmdLen+1)
            {
                if(dat == 0x16)
                {
                    RecvBuf->RecvCmdBuf[RecvBuf->p_RecvCmdData++] = dat;
                    RecvBuf->f_RecvCmdHead = 0;
//                        OM_SendUsartData(Debug_COM, RecvBuf->RecvCmdBuf, RecvBuf->p_RecvCmdData);
                    return F_WX_PTR;
                }
                else
                {
                    RecvBuf->RecvCmdBuf[RecvBuf->p_RecvCmdData++] = dat;
                    RecvBuf->f_RecvCmdHead = 0;
                    return ERR_ENDCODE;
                }
            }
            else
            {
                RecvBuf->f_RecvCmdHead = 0;
                return ERR_LEN;
            }
    	}
    }
    return FALSE_CMD;
}


