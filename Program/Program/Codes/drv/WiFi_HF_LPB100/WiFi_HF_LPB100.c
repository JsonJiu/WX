
#include "config.h"
#include "bsp_mc306.h"
#include "WiFi_HF_LPB100.h"

unsigned char WiFi_HWStatus = WIFI_RST;   //WiFi硬件状态
//unsigned char WiFi_HWPreSta = 0xFF;            //WiFi硬件前一状态
//unsigned char WiFi_HWNextSta = 0xFF;           //WiFi硬件下一状态
unsigned short WiFi_HWDlyTime = 0;
unsigned short WiFi_SWDlyTime = 0;
unsigned short WiFi_TimeOutToReset = WIFI_TIME_OUT_TO_RESET;
extern unsigned char TaskNum_WiFi_SW;
extern APP_Frame_t frameBuf;
/*
功能：Wi-Fi初始化函数，包含引脚方向、引脚初始状态和串口初始化
参数：无
*/
void WiFi_Init(void)
{
    UartMode mode;
    WiFi_Pin_Init();
    WiFi_SleepRQ_Set();
    WiFi_Reload_Set();
    WiFi_Reset_Set();

    mode.Baud = UartBaud57600;
    mode.DataBit = DATA_8;
    mode.Parity = PARITY_NONE;
    mode.Sequence = LSB;
    mode.StopBit = STOP_1;
    WiFiUart_Init(&mode, WiFiUartRecv);

    WiFi_HWStatus = WIFI_RST;
}

void WiFi_Pin_Init(void)
{
    /** Configure pins as GPIO
        WiFi_Reset     ------> GPIO_Output
        WiFi_RDY       ------> GPIO_Input
        WiFi_Sleep_ON  ------> GPIO_Input
        WiFi_Sleep_RQ  ------> GPIO_Output
        WiFi_Reload    ------> GPIO_Output
    */

    WiFi_SleepRQ_Init(OUT);
    WiFi_Reset_Init(OUT);
    WiFi_Reload_Init(OUT);
    WiFi_SleepON_Init(IN);
    WiFi_Ready_Init(IN);
}
unsigned char wifiInitTryTimes = 3;
/*
功能：Wi-Fi硬件控制，100mS运行一次，
参数：该任务有一个全局的Wi-Fi硬件状态标志，
unsigned char WiFi_HWStatus = WIFI_RST;   //WiFi硬件状态

该状态可以被置为以下状态
	WIFI_RST,            //复位
	WIFI_READY,         //就绪，可以收发数据
	WIFI_ENTER_SLEEP,   //进入睡眠模式，
	WIFI_SLEEP,         //模块处于睡眠状态
	WIFI_RELOAD,       //恢复出厂设置
	WIFI_WAKEUP,      //从睡眠模式唤醒
	WIFI_ERROR,       //模块复位失败、进入睡眠模式失败或唤醒失败
//模块进入这个状态后会立即复位。
初始化完成后WiFi_HWStatus = WIFI_RST。
*/
void Task_WiFi_HW(void)
{
    if(WiFi_HWDlyTime)
        WiFi_HWDlyTime--;
    if(WiFi_TimeOutToReset)
        WiFi_TimeOutToReset--;
    switch(WiFi_HWStatus)
    {
        /********************************-- 复位（启动）时序 --***********************************/
    case WIFI_RST:
#ifdef DEBUG
        Printf_DebugMessage("WiFi Reset...\r\n");
#endif
        WiFi_Reset_Clr();
        WiFi_HWDlyTime = 1;             //20ms
        WiFi_HWStatus = WIFI_RST1;
        break;

    case WIFI_RST1:
        if(WiFi_HWDlyTime)
            return;
        WiFi_Reset_Set();
        WiFi_HWDlyTime = 150;            //15s  //超时则认为启动不成功
        WiFi_HWStatus = WIFI_WAIT_RDY;
        break;

    case WIFI_WAIT_RDY:                                       //检测 Ready 信号
        if(!(WiFi_Ready_Get()))   //如果检测不到Ready 信号，15S后进入错误处理；
        {
#ifdef DEBUG
            Printf_DebugMessage("WIFI Ready...\r\n");
#endif
            WiFi_HWStatus = WIFI_READY;//WIFI_ENTERCMD;//
            WiFi_HWDlyTime = 3000;//置5分钟心跳时间
        }
        else
        {
            if(WiFi_HWDlyTime)
                return;
            if(wifiInitTryTimes)
            {
                wifiInitTryTimes--;
                WiFi_HWStatus = WIFI_RST;
            }
            else
                WiFi_HWStatus = WIFI_ERROR;
        }
        break;
#if 0
        case WIFI_ENTERCMD:
            sprintf((char*)WiFi_Buffer.TXBuffer,"+++");
            WiFi_Buffer.TxLen = 3;
            WiFiUartSend(WiFi_Buffer.TXBuffer, WiFi_Buffer.TxLen );
            WiFi_HWDlyTime = 10;
            WiFi_TimeOutToReset = 0;
            WiFi_HWStatus =WIFI_ENTERCMD_WAITa;
            break;
        case WIFI_ENTERCMD_WAITa:
            if(WiFi_HWDlyTime)
                return;
            if(WiFi_Buffer.BufRead == WiFi_Buffer.BufWrite) //缓冲区无数据
            {
                WiFi_TimeOutToReset++;
                if(WiFi_TimeOutToReset >= 20 )
                    WiFi_HWStatus = WIFI_ENTERCMD;
            }
            else
            {
                if(WiFi_Buffer.UART_Buff[WiFi_Buffer.BufRead++] == 'a')
                {
                    WiFi_HWDlyTime = 10;
                    WiFi_HWStatus = WIFI_ENTERCMDa;
                }
                if(WiFi_Buffer.BufRead >= UART_BUFF_LEN)
                WiFi_Buffer.BufRead = 0;
            }
            break;
        case WIFI_ENTERCMDa:
            if(WiFi_HWDlyTime)
                return;
            WiFi_Buffer.TXBuffer[0] = 'a';
            WiFi_Buffer.TxLen = 1;
            WiFiUartSend(WiFi_Buffer.TXBuffer, WiFi_Buffer.TxLen );

            WiFi_HWDlyTime = 10;
            WiFi_HWStatus = WIFI_ENTERCMD_WAIT_OK;
            break;
        case WIFI_ENTERCMD_WAIT_OK:
            if(WiFi_HWDlyTime)
                return;

            if(WiFi_Buffer.BufRead == WiFi_Buffer.BufWrite) //缓冲区无数据
                return ;
            if(WiFi_Buffer.UART_Buff[WiFi_Buffer.BufRead++] == '+')
            {
                if(WiFi_Buffer.UART_Buff[WiFi_Buffer.BufRead++] == 'o')
                {
                    if(WiFi_Buffer.UART_Buff[WiFi_Buffer.BufRead++] == 'k')
                    {
                         WiFi_HWDlyTime = 10;
                         WiFi_HWStatus = WIFI_CONFIG;
                    }
                }
            }
            if(WiFi_Buffer.BufRead >= UART_BUFF_LEN)
            WiFi_Buffer.BufRead = 0;

            WiFi_HWDlyTime = 10;
            WiFi_HWStatus = WIFI_CONFIG;
            break;
        
    case WIFI_CONFIG:
        if(WiFi_HWDlyTime)
            return;
        sprintf((char*)WiFi_Buffer.TXBuffer,"AT+UART=19200,8,1,NONE,NFC%d",0x0d);
        WiFi_Buffer.TxLen = sizeof(WiFi_Buffer.TXBuffer);
        WiFiUartSend(WiFi_Buffer.TXBuffer, WiFi_Buffer.TxLen );

        TaskNum_WiFi_SW = User_AskTimeNumber();    //定时器编号在使用前需申请，如果返回值为0，表明已超出最大定时器数
        if(TaskNum_WiFi_SW != 0)
        {
            User_StartTime(0, TaskNum_WiFi_SW, 10, Task_WiFi_SW);//MAINTIMER
        }
        WiFi_HWDlyTime = 20;
        WiFi_HWStatus = WIFI_READY;
        break;
#endif
    case WIFI_READY:
        if(WiFi_TimeOutToReset == 0)
        {
            WiFi_TimeOutToReset = WIFI_TIME_OUT_TO_RESET;//置5分钟心跳时间
            WiFi_HWStatus = WIFI_RST;
        }
        break;

        /********************************-- 休眠时序 --***********************************/
    case WIFI_ENTER_SLEEP:
        if(!(WiFi_SleepON_Get()))        //Sleep_ON 为低 表示模块已经进入休眠；
        {
#ifdef DEBUG
            Printf_DebugMessage("WIFI is Closed...\r\n");
#endif
            WiFi_HWStatus = WIFI_SLEEP;
        }
        else
        {
            WiFi_SleepRQ_Clr();
            WiFi_HWDlyTime = 30;             //3s  拉低3S进入休眠
            WiFi_HWStatus = WIFI_SLEEP1;
        }
        break;

    case WIFI_SLEEP1:
        if(WiFi_HWDlyTime)
            return;
        WiFi_SleepRQ_Clr();
        WiFi_HWDlyTime = 1;             //100ms
        WiFi_HWStatus = WIFI_SLEEP_CHK;
        break;

    case WIFI_SLEEP_CHK:
        if(!(WiFi_SleepON_Get()))//Sleep_ON  为低 表示模块已经进入休眠；
        {
#ifdef DEBUG
            Printf_DebugMessage("WIFI Close Success...\r\n");
#endif
            WiFi_HWStatus = WIFI_SLEEP;
        }
        else
        {
            if(WiFi_HWDlyTime)
                return;
#ifdef DEBUG
            Printf_DebugMessage("WIFI Close Fail...\r\n");
#endif
            WiFi_HWStatus =  WIFI_ERROR;
        }
        break;
    case WIFI_SLEEP:

        break;
        /********************************-- 休眠唤醒时序 --*******************************/
    case WIFI_WAKEUP:
        if(WiFi_SleepON_Get())        //Sleep_ON 为高表示已唤醒
        {
#ifdef DEBUG
            Printf_DebugMessage("WIFI is Running...\r\n");
#endif
            WiFi_HWStatus = WIFI_READY;
        }
        else
        {
            WiFi_SleepRQ_Clr();
            WiFi_HWDlyTime = 10;             //1s  拉低1S唤醒
            WiFi_HWStatus = WIFI_WAKEUP1;
        }
        break;

    case WIFI_WAKEUP1:
        if(WiFi_HWDlyTime)
            return;
        WiFi_SleepRQ_Set();
        WiFi_HWDlyTime = 100;             //100ms
        WiFi_HWStatus = WIFI_WAKEUP_CHK;
        break;
    case WIFI_WAKEUP_CHK:
        if(WiFi_SleepON_Get())        //Sleep_ON  为高表示已唤醒
        {
#ifdef DEBUG
            Printf_DebugMessage("WIFI Wakeup Success...\r\n");
#endif
            WiFi_HWDlyTime = 50;      //5S
            WiFi_HWStatus = WIFI_WAIT_RDY; //转等待就绪
        }
        else
        {
            if(WiFi_HWDlyTime)
                return;
#ifdef DEBUG
            Printf_DebugMessage("WIFI Wakeup Fail...\r\n");
#endif
            WiFi_HWStatus = WIFI_ERROR;
        }
        break;

        /********************************-- 出厂设置时序 --*******************************/
    case WIFI_RELOAD:
        WiFi_Reload_Clr();
        WiFi_HWDlyTime = 50;             //5s  拉低5S,恢复出厂设置
        WiFi_HWStatus = WIFI_RELOAD1;
        break;
    case WIFI_RELOAD1:
#ifdef DEBUG
        Printf_DebugMessage("WIFI is Reloaded...\r\n");
#endif
        WiFi_Reload_Set();
        WiFi_HWDlyTime = 2;             //200ms 后开始复位;
        WiFi_HWStatus = WIFI_RST;
        break;
    case WIFI_ERROR:
        WiFi_HWStatus = WIFI_RST;
        break;
    default:
        break;
    }
}

void Task_WiFi_SW(void)
{
    Frame_Receive(&WiFi_Buffer);
    //if((WiFi_Buffer.UartFlag.Bit.RxFinished == TRUE)&&(frameBuf.StatusIsBusy == FALSE))
    if(WiFi_Buffer.UartFlag.Bit.RxFinished == TRUE)
    {
#ifdef DEBUG
        Printf_DebugMessage("WiFi Recv Data: \r\n");
        HexToASCII(DebugMsgBuff, WiFi_Buffer.RXBuffer, WiFi_Buffer.RxLen );
        Printf_DebugMessage(DebugMsgBuff);//打印调试信息
#endif
        SYS_IdleTOPowerDownTime =  AUTOPOWERDOWNTIME;

        if(DataConvert(WiFi_Buffer.RXBuffer,UART_WIFI) == FALSE)
            return;
       // DataConvert(WiFi_Buffer.RXBuffer,UART_WIFI);
        Protocol_Resolve();
        WiFi_Buffer.UartFlag.Bit.RxFinished = FALSE;
    }
    if(WiFi_Buffer.UartFlag.Bit.TxFinished == TRUE)
    {
#ifdef DEBUG
        Printf_DebugMessage("WiFi Send Data: \r\n");
        HexToASCII(DebugMsgBuff, WiFi_Buffer.TXBuffer, WiFi_Buffer.TxLen );
        Printf_DebugMessage(DebugMsgBuff);//打印调试信息
#endif
        WiFiUartSend(WiFi_Buffer.TXBuffer, WiFi_Buffer.TxLen );
        WiFi_Buffer.UartFlag.Bit.TxFinished = FALSE;
    }
}

/*************************************************************************
 * Function Name: WiFi串口接收
 * Parameters: none
 *
 * Return: none
 *
 * Description: Usart 2 interrupt handler
 * WiFi口
 *************************************************************************/
void WiFiUartRecv(unsigned char ch)
{
    WiFi_TimeOutToReset = WIFI_TIME_OUT_TO_RESET;//WiFi串口有数据，重置5分钟心跳时间
    WiFi_Buffer.UART_Buff[WiFi_Buffer.BufWrite] = ch;
    WiFi_Buffer.BufWrite++;
    if(WiFi_Buffer.BufWrite >= UART_BUFF_LEN)
        WiFi_Buffer.BufWrite = 0;
}







