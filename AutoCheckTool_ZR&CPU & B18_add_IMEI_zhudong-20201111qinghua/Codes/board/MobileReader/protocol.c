
#include "include.h"
#include "protocol.h"

APP_Frame_t frameBuf;
extern unsigned char HeadBuf[14];  //RF前导缓冲区
extern unsigned short WiFi_TimeOutToReset;
unsigned char Get_Version(unsigned char *pbuf)//获取版本号
{
    *pbuf++ = 0x06; //设备类型--PDA
    *pbuf++ = (unsigned char)(Software / 256);
    *pbuf++ = (unsigned char)(Software % 256);
    *pbuf++ = (unsigned char)(Hardware / 256);
    *pbuf++ = (unsigned char)(Hardware % 256);
    *pbuf++ = 0x01; //
    return 6;
}

//先白化,再上传数据
//void WhiteDeal(unsigned char *pucStr, unsigned int ulNum)
//{
//    unsigned short len;//数据区总长度
//
//    len = (((unsigned short) * (pucStr + Fream_LenH)) << 8) + *(pucStr + Fream_LenL);
//    vWht_DataWhitening((unsigned char *)pucStr + Fream_Data, len - 10); //将帧序号以后的数据区白化，仅白化帧序号以后的数据
//    *(pucStr + len + 11) =  Calc_CS((unsigned char *)pucStr, len + 11);        //和校验
//    //    COM_SendStr(pucStr, len + 13);    //发送
//}
void DataConvert(unsigned char *buf, COMM_UART uart)
{
    unsigned char i;
    
    frameBuf.Port = uart;

    frameBuf.Head = *buf++;
    frameBuf.FrameType = *buf++;

    frameBuf.FrameLen = *buf++;
    frameBuf.FrameLen += (*buf) << 8;
    buf++;
    
    BCDtoHex(&frameBuf.FrameLen);
    if(frameBuf.FrameLen > COMM_BUF_MAX_LEN)
    {
        frameBuf.FrameLen = 0;
        return;
    }
    frameBuf.FrameLen -= 6;
    for(i = 0; i < frameBuf.FrameLen; i++)
    {
        frameBuf.Dat[i] = *buf++;
    }
}
//协议解析
void Protocol_Resolve(void)
{
    WiFi_TimeOutToReset = 0;    //WiFi超时时间清零
    
    switch(frameBuf.FrameType)
    {
    case CMD_HERTBEAT://心跳
        break;

    case CMD_VER:
        if(frameBuf.Port == UART_DEBUG)
        {
            COM_Buffer.TxLen = MakeFrame(COM_Buffer.TXBuffer, frameBuf.FrameType, 0, 0);
            COM_Buffer.UartFlag.Bit.TxFinished = TRUE;
        }
        else
        {
            WiFi_Buffer.TxLen = MakeFrame(WiFi_Buffer.TXBuffer, frameBuf.FrameType, 0,0);
            WiFi_Buffer.UartFlag.Bit.TxFinished = TRUE;
        }
        break;
    
    case CMD_RF_JZQ:    //·?????????--????·???????????????????
        RF_OptDelayTime = 0;
        RF_Status = RF_Send;
        break;

    case CMD_RF_METER:
        RF_OptDelayTime = 0;
        RF_Status = RF_SendHead;
        break;

    case CMD_RF_MODE:
        RF_OptDelayTime = 0;
        RF_Status = RF_Cfg;
        break;
    default:
        RF_Status = RFS_IDLE;//RF任务信号
        break;
    }
}

unsigned short MakeFrame(unsigned char *pDesbuf, unsigned char cmd, unsigned char *databuf,unsigned short datalen)
{
    unsigned char len = 0, i;
    unsigned char *ppbuf = pDesbuf;
    unsigned short crc16 = 0;
    
    *pDesbuf++ = 0xFE;

    *pDesbuf++ = 0x80 + cmd; //帧类型
    switch(cmd)
    {
    case CMD_VER:
        len = 12;//加上帧头 和 CRC16校验
        *pDesbuf++ = 0x12;
        *pDesbuf++ = 0x00;
        *pDesbuf++ = HexToBCD((unsigned char)((Hardware >>16) & 0x0000FF));
        *pDesbuf++ = HexToBCD((unsigned char)((Hardware >>8) & 0x0000FF));
        *pDesbuf++ = HexToBCD((unsigned char)((Hardware) & 0x0000FF));
    
        *pDesbuf++ = HexToBCD((unsigned char)((Software >>16) & 0x0000FF));
        *pDesbuf++ = HexToBCD((unsigned char)((Software >>8) & 0x0000FF));
        *pDesbuf++ = HexToBCD((unsigned char)((Software) & 0x0000FF));
        break;
    
    case CMD_RF_JZQ:
    case CMD_RF_METER:
    case CMD_RF_MODE:
        len = datalen + 6;//加上帧头 和 CRC16校验
        *pDesbuf++ = HexToBCD(len % 100);
        *pDesbuf++ = HexToBCD(len / 100);

        for(i = 0; i < len - 6; i++)//数据长度及之前
            *pDesbuf++ = *databuf++;
        break;
    
    default:
        return 0;
        //break;
    }
    
    
    crc16 = CRC16_0xA001(ppbuf,len - 2);
    
    *pDesbuf++ = (crc16 & 0x00FF);
    *pDesbuf++= ((crc16 >> 8) & 0x00FF);
    return len;
}
//接收数据, FE+CMD+Len+DataBag+crc16
unsigned char Frame_Receive(COMM_Buffer_t *pbuf)
{
    unsigned char dat = 0;
    unsigned short crc16 = 0;

    if(pbuf->BufRead == pbuf->BufWrite) //缓冲区无数据
        return 0;
    if(pbuf->UartFlag.Bit.RxFinished == TRUE) //上一帧未处理
        return 0;
    if(pbuf->TimeOver == 0)  //超时
    {
        pbuf->TimeOver = OVERTIME;
        pbuf->State = Recv_Fream_New;
        pbuf->RxLen = 0;
        return 0;
    }
    dat = pbuf->UART_Buff[pbuf->BufRead++];
    if(pbuf->BufRead >= UART_BUFF_LEN)
        pbuf->BufRead = 0;

    pbuf->TimeOver = OVERTIME;
    switch(pbuf->State)
    {
    case Recv_Fream_New: //帧头
        if(dat == 0xFE)
        {
            pbuf->RxLen = 0;
            pbuf->RXBuffer[pbuf->RxLen++] = dat;
            pbuf->State = Recv_Fream_Cmd;
        }
        break;

    case Recv_Fream_Cmd: //CMD
        switch(dat)
        {
        case CMD_VER:
        case CMD_RF_METER:
        case CMD_RF_JZQ:
        case CMD_RF_MODE: // 0x24    // RF模式切换
                pbuf->RXBuffer[pbuf->RxLen++] = dat;
                pbuf->State = Recv_Fream_Len;
                pbuf->AreaLen = 2;
                pbuf->CRC16 = 0;
                break;
        default:
                pbuf->State = Recv_Fream_New;
                break;
        }
        break;

    case Recv_Fream_Len:
        pbuf->RXBuffer[pbuf->RxLen++] = dat;
        pbuf->AreaLen --;
        if(pbuf->AreaLen == 0)
        {

			pbuf->DataLen = (pbuf->RXBuffer[pbuf->RxLen - 1] << 8 ) + pbuf->RXBuffer[pbuf->RxLen - 2];
            BCDtoHex(&pbuf->DataLen);
            pbuf->DataLen = pbuf->DataLen - 6;
            if(pbuf->DataLen == 0)
            {
                pbuf->State = Recv_Fream_CRC;
                pbuf->AreaLen = 2;
            }
            else if(pbuf->DataLen > COMM_BUF_MAX_LEN)//帧长度异常
            {
                pbuf->State = Recv_Fream_New;
                return 0;
            }
            else
                pbuf->State = Recv_Fream_Bag;
        }
        break;

    case Recv_Fream_Bag://数据包
        pbuf->RXBuffer[pbuf->RxLen++] = dat;
        pbuf->DataLen--;
        if(pbuf->DataLen == 0)
        {
            pbuf->State = Recv_Fream_CRC;
            pbuf->AreaLen = 2;
        }
        break;

    case Recv_Fream_CRC://校验--此处校验是"数据包"的校验
        pbuf->RXBuffer[pbuf->RxLen++] = dat;
        pbuf->AreaLen--;
        if(pbuf->AreaLen == 0)
        {
            
            crc16 = pbuf->RXBuffer[pbuf->RxLen - 1];
            crc16 <<= 8;
            crc16 += pbuf->RXBuffer[pbuf->RxLen - 2];
            
            if( crc16 == CRC16_0xA001(pbuf->RXBuffer,pbuf->RxLen - 2))//校验正确
            {
                pbuf->UartFlag.Bit.RxFinished = TRUE;
            }
            
            pbuf->State = Recv_Fream_New;
        }
        break;
        
    default:
            break;
    }
    return 0;
}

#define debugRFRECV
//RF接收数据，符合威星 2.0 协议
unsigned char RF_Frame_Receive(COMM_Buffer_t *pbuf)
{
//    unsigned char dat;

//    if(pbuf->BufRead == pbuf->BufWrite) //缓冲区无数据
//        return 0;
#ifndef debugRFRECV
    if(pbuf->UartFlag.Bit.RxFinished == TRUE) //上一帧未处理
        return 0;
    if(pbuf->RxLen == 0)
    {
        pbuf->TimeOver = OVERTIME;
        pbuf->RxLen = 0;
        if(pbuf->BufRead != pbuf->BufWrite) //缓冲区有数据
        {
            pbuf->RXBuffer[pbuf->RxLen++] = pbuf->UART_Buff[pbuf->BufRead++];
            if(pbuf->BufRead >= UART_BUFF_LEN)
                pbuf->BufRead = 0;
        }
    }
    else if(pbuf->TimeOver == 0)  //超时
    {
        pbuf->TimeOver = OVERTIME;
        pbuf->UartFlag.Bit.RxFinished = TRUE;
        return 1;
    }
    else
    {
       if(pbuf->BufRead != pbuf->BufWrite) //缓冲区有数据
        {
            pbuf->RXBuffer[pbuf->RxLen++] = pbuf->UART_Buff[pbuf->BufRead++];
            if(pbuf->BufRead >= UART_BUFF_LEN)
                pbuf->BufRead = 0;
        }
    }
    return 0;
#else
     if(pbuf->UartFlag.Bit.RxFinished == TRUE) //上一帧未处理
        return 0;

    if(pbuf->BufRead == pbuf->BufWrite) //缓冲区无数据
    {
        if((pbuf->TimeOver == 0)&&(pbuf->RxLen != 0) )//超时
        {
            pbuf->TimeOver = OVERTIME;
            pbuf->UartFlag.Bit.RxFinished = TRUE;
            return 1;
        }
    }
    else
    {
        while(pbuf->BufRead != pbuf->BufWrite)
        {
            pbuf->RXBuffer[pbuf->RxLen++] = pbuf->UART_Buff[pbuf->BufRead++];
            if(pbuf->BufRead >= UART_BUFF_LEN)
                pbuf->BufRead = 0;
        }
        pbuf->TimeOver = OVERTIME;
    }
    return 0;
#endif
}



