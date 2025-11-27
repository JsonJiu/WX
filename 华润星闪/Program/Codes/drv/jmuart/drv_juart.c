#include "include.h"

/********************************************************************************************
直读头硬件初始化 
void ZDInit(void);
启动读取直读头
void ZDStart(void); 
获取直读头读取进度
ZDSTATE ZDState(void); 

*********************************************************************************************/

St_ZDRRUN ZDRun;

void ZD_Recive_Byte(unsigned char data);
void ZDStop(void);



/**
 * 直读接收到一字节
 * 
 * @author xsc (2014-11-09)
 * 
 * @param data 
 */
void ZD_Recive_Byte(unsigned char data)
{
    //缓存区满，则不接收新数据
    if(sizeof(ZDRun.Rx.Buff) - ZDRun.Rx.BuffStart == ZDRun.Rx.BuffEnd)
    {
        return;
    }
    ZDRun.Rx.Buff[ZDRun.Rx.BuffEnd++] = data;
    if(ZDRun.Rx.BuffEnd == sizeof(ZDRun.Rx.Buff))
    {
        ZDStop();
    }
}


/************************ 对外接口 ****************************/
/**
 * 直读头初始化
 * 
 * @author xsc (2014-11-07)
 */
void ZDInit(void)
{
    UartMode uartset;

    uartset.Baud = UartBaud1200;
    uartset.Parity = 2;//偶校验
    uartset.StopBit = 1;//1停止位
    uartset.DataBit = 1;//8位数据位;
    uartset.Sequence = 0;//LSB

    ZD_POW_Ctrl(Dis);
    ZD_POW_Init(IODIR_Output, IOPP_PP);
    ZDRun.state = ZDSTATE_POWDOWN;
    SetState(STATE_MEASURE, STATE_MEASURE_NORMAL);

#if UART_TYPE == UARTTYPE_MUART        //普通IO口模式
    Muart_Init(uartset, ZD_Recive_Byte);
#endif

#if UART_TYPE == UARTTYPE_HUART

    ZDUart_Init((pUartMode)&uartset, ZD_Recive_Byte);
#endif
}

void ZDPowOn(void)
{
    ZDRun.state = ZDSTATE_READING;
    ZDRun.Rx.BuffStart = 0;
    ZDRun.Rx.BuffEnd = 0;
    ZDRun.TimeOut = 2;
    ZD_POW_Ctrl(En);
#if UART_TYPE == UARTTYPE_MUART        //普通IO口模式
    Muart_RxEn();
#endif

#if UART_TYPE == UARTTYPE_HUART
    ZDUart_RX_En();
#endif
    ZDRun.TryTime++;
}

/**
 * 开始读取直读头
 * 
 * @author xsc (2014-11-10)
 */
void ZDStart(void)
{
    ZDPowOn();
    DelayMs(20);
    ZDRun.TryTime = 0;
    ZDRun.Rx.BuffStart = 0;
    ZDRun.Rx.BuffEnd = 0;
}

/**
 * 关闭直读头
 * 
 * @author xsc (2014-11-10)
 */
void ZDStop(void)
{
    //ZDRun.state = ZDSTATE_POWDOWN;
    ZD_POW_Ctrl(Dis);
    ZDRun.Rx.BuffStart = 0;
    ZDRun.Rx.BuffEnd = 0;
    
#if UART_TYPE == UARTTYPE_MUART        //普通IO口模式
    Muart_RxDs();
#endif

#if UART_TYPE == UARTTYPE_HUART
    ZDUart_RX_Ds();
#endif
}


///**
// * 读取直读头数据缓存
// *
// * @author xsc (2014-11-07)
// *
// * @param buf 返写缓存地址
// * @param len 预读取长度
// *
// * @return unsigned char 实际读取长度
// */
//unsigned char ZDRead(unsigned char *buf, unsigned char len)
//{
//    unsigned char i,j;
//
//    if(ZDRun.Rx.BuffStart == ZDRun.Rx.BuffEnd)
//    {
//        return 0;
//    }
//    if(ZDRun.Rx.BuffEnd < ZDRun.Rx.BuffStart)
//    {
//        j = sizeof(ZDRun.Rx.Buff) - ZDRun.Rx.BuffStart;
//        if(j >= len)
//        {
//            j = len;
//        }
//        for(i = 0; i < j; i++)
//        {
//            buf[i] = ZDRun.Rx.Buff[ZDRun.Rx.BuffStart++];
//        }
//        if(j == len)
//        {
//            return j;
//        }else{
////          ZDRun.Rx.BuffStart = 0;
//            while(i < len || ZDRun.Rx.BuffStart < ZDRun.Rx.BuffEnd)
//            {
//                buf[i++] = ZDRun.Rx.Buff[ZDRun.Rx.BuffStart++];
//            }
//            return i;
//        }
//    }else{
//        j = ZDRun.Rx.BuffEnd - ZDRun.Rx.BuffStart;
//        if(j >= len)
//        {
//            j = len;
//        }
//        for(i = 0; i < j; i++)
//        {
//            buf[i] = ZDRun.Rx.Buff[ZDRun.Rx.BuffStart++];
//        }
//        return j;
//    }
//}


unsigned long ZDGetData(void)
{
    return 0;
}

/**
 * 直读头解码
 * 
 * @author xsc (2015-01-22)
 * 
 * @param UartBuff 
 * @param UartLen 
 */
unsigned long ZDDecode(unsigned char * UartBuff, unsigned char UartLen)
{
    unsigned long zdnum = 0xFFFFFFFF;
    unsigned char sum, i;
    //unsigned long amnum;

    if(UartBuff[0] == 0xFE)
    {
        if(UartLen > 1)
        {
            if(UartBuff[1] == 0x68)
            {
                if(UartLen > 5)
                {
                    sum = 0;
                    for(i = 2;i < 5; i++)
                    {
                        sum += UartBuff[i];
                    }
                    if(sum != UartBuff[i])
                    {
                        ZDRun.Rx.BuffStart = 0;//接收失败，len清零
                        ZDRun.Rx.BuffEnd = 0;
                    }else{
                        sum = UartBuff[4] & 0xf0;
                        if(sum == 0)
                        {
                            ZDStop();
                            zdnum = BCDtoHEX_B(&UartBuff[2], 3);
							if (zdnum > 0xA0000000)
							{
								ZDRun.state = ZDSTATE_ERR;
							}else{
								ZDRun.amount = zdnum * 100;
								ZDRun.state = ZDSTATE_READED;
							}
                        }else if(sum == 0x30 || sum == 0x80) //直读头坏
                        {
                            ZDRun.state = ZDSTATE_ERR;
                            //MeterState.State_ZD = 1;
                        }
                        return zdnum;//接收成功，len清零
                    }
                }else{
                    return 0;
                }
            }else{
                ZDRun.Rx.BuffStart = 0;
            }
        }
    }else{
        return 0;//ZDRun.Rx.BuffStart = 0;
    }
    return 0;
}



/**
 * 获取直读头状态
 * 
 * @author xsc (2014-11-09)
 * 
 * @return 状态ZDSTATE 
 */
ZDSTATE ZDState(void)
{
    unsigned long temp;
    if(ZDRun.state == ZDSTATE_POWDOWN)
    {
        ZDStart();
        SetState(STATE_MEASURE, STATE_MEASURE_READING);
        return ZDRun.state;
    }
    temp = ZDDecode(ZDRun.Rx.Buff + ZDRun.Rx.BuffStart, ZDRun.Rx.BuffEnd - ZDRun.Rx.BuffStart);


    if(ZDRun.state == ZDSTATE_READED)
    {
        ZDRun.state = ZDSTATE_POWDOWN;
        return ZDSTATE_READED;
    }else if(ZDRun.state == ZDSTATE_READING && temp != 0)
    {
        ZDStop();
    }
    return ZDRun.state;
}

void ZDTimeTick(void)
{
    if(ZDRun.TimeOut)
    {
        ZDRun.TimeOut--;
        if(ZDRun.TimeOut == 0)
        {
            ZDPowOn();
        }
    }
    if(ZDRun.TimeOut == 0)
    {
        if(ZDRun.state == ZDSTATE_READING)
        {
            ZDRun.state = ZDSTATE_ERR;
            ZDStop();
        }
    }
}



