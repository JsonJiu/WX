

#include "Debug.h"

unsigned char Test_HeadBuf[] = { 0x55, 0x55, 0x55, 0x55, 0xFF, 0xFF };
unsigned char Test_DataBuf[100] = { 0};
unsigned char Test_EndBuf[] = { 0x68, 0x68, 0x68, 0x68, 0x68, 0x68 };
unsigned short Debug_in = 0 , Debug_out = 0 ;

#ifdef  DEBUG_SELF_SEND
void VirtualCOMRecv(void)
{
    RFTimming = SYS_DLY_1S;//4000;
    while(RFTimming);   //延时

    Test_Buffer[6]++;
    Test_Buffer[21]++;
    memcpy(COM_Buffer.RXBuffer, Test_Buffer, 23);
    COM_Buffer.UartFlag.Bit.RxFinished = SET;
    COM_Buffer.RxLen = 23;

}
#endif


//DEBUG_RF_ALL  DEBUG_RF_FRAME
unsigned char Debug_RF = DEBUG_RF_ALL;
#ifdef  DEBUG_RF_MONITOR

unsigned char debug_temp[1100];
unsigned short debug_buf_in = 0, debug_buf_out = 0;

void SendMonitorData(void)
{
    unsigned char t;
    unsigned int i = 0;
    if(Debug_RF == DEBUG_RF_FRAME)
    {
        if(RF_Buffer.UartFlag.Bit.RxFinished == true)	  // 数据发送完成 清发送缓冲区及相关参数
        {
            for(i = 0; i < RF_Buffer.RxLen; i++)
            {
                //A-65 0-48
                t = ((RF_Buffer.RXBuffer[i] >> 4)  & 0x0F);
                if(t > 0x09)
                    debug_temp[debug_buf_in] = (65 + t - 10);
                else
                    debug_temp[debug_buf_in] = (48 + t);

                debug_buf_in++;
                if(debug_buf_in > 1024)
                    debug_buf_in = 0;

                t = ((RF_Buffer.RXBuffer[i])  & 0x0F);
                if(t > 0x09)
                    debug_temp[debug_buf_in] = (65 + t - 10);
                else
                    debug_temp[debug_buf_in] = (48 + t);

                debug_buf_in++;
                if(debug_buf_in > 1024)
                    debug_buf_in = 0;

                debug_temp[debug_buf_in] = (32);//space

                debug_buf_in++;
                if(debug_buf_in > 1024)
                    debug_buf_in = 0;
            }
            debug_temp[debug_buf_in++] = (0x0D);
            if(debug_buf_in > 1024)
                debug_buf_in = 0;
            debug_temp[debug_buf_in++] = (0x0A);
            if(debug_buf_in > 1024)
                debug_buf_in = 0;
            RF_Buffer.UartFlag.Bit.RxFinished = false;
        }
    }

    if(RF_Buffer.BufWrite != RF_Buffer.BufRead)
    {
//        USART_SendData(UART_DEBUG,  RF_Buffer.UART_Buff[RF_Buffer.BufRead++]);
        if(RF_Buffer.BufRead >= UART_BUFF_LEN)
            RF_Buffer.BufRead = 0;
//        while (USART_GetFlagStatus(UART_DEBUG, USART_FLAG_TXE) == false)
        {
        }
        if(debug_buf_out > 1024)
            debug_buf_out = 0;
    }

}
#else
unsigned char debug_temp[1100];
unsigned short debug_buf_in = 0, debug_buf_out = 0;
#endif



#ifdef DEBUG_SIMULATE_METER
//
//模拟表

unsigned char Test_Buffer[] = {0x68, 0x99, 0x22, 0x00, 0x00, 0x00, 0x00, 0x68, 0x81, 0x1C, 0x00, 0x00, 0x01, 0x01, 0x00, 0x11,
                               0x52, 0x03, 0x18, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
                               0xFF, 0xFF, 0xFF, 0x18, 0x00, 0x00, 0x00, 0x36, 0x16,
                              }; //41
void TestRecvToFinal(void)
{
    unsigned char i, len, headbuf[7];

    RF_Buffer.UartFlag.Bit.RxFinished = RESET;

    Test_Buffer[6]++;
    Test_Buffer[39]++;

    memcpy(RF_Buffer.TXBuffer, Test_Buffer, 41);
    RF_Buffer.TxLen = 41;
    if(RFPara.DataWhitening)
    {
        WhiteDeal(RF_Buffer.TXBuffer, RF_Buffer.TxLen); //去白化
    }

    memcpy(COM_Buffer.TXBuffer, RF_Buffer.RXBuffer, RF_Buffer.RxLen);    //数据转移
    COM_Buffer.TxLen = RF_Buffer.RxLen;
    COM_SendStr(COM_Buffer.TXBuffer, COM_Buffer.TxLen);            //COM应答发送

    RFTimming = 4;
    while(RFTimming);
    // RF_Process(RFS_RST);

    RF_Process(RFS_SEND);
    if(RFS_SEND != RF_Status)
        uart1SendByte(0xCB);
    {
        extern unsigned char MakeHead(unsigned char * pbuf, unsigned char Des);
        len = MakeHead(headbuf, RF_Buffer.TXBuffer[1]);
    }
    for(i = 0; i < 15; i++)	   //发送前导    此处前导码根据表具唤醒时间确定(600个/s)  4秒
    {
        RF_SendStr(headbuf, len);
    }

    RF_SendStr(RF_Buffer.TXBuffer, RF_Buffer.TxLen + 4);
    //  RF_Process(RFS_RST);
    RF_Process(RFS_RECV);          /*直接接收模式 */
}

#endif
