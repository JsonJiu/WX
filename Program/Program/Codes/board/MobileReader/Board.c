/* Includes ------------------------------------------------------------------*/
#include "include.h"
#include "Board.h"

unsigned short StateTimer;//用于状态指示灯控制
unsigned char SystemState = 0;//系统状态
unsigned short PowerCheckTime;
/*----------------------------------------------------------------------------*/
/* Configure GPIO                                                             */
/*----------------------------------------------------------------------------*/
void GPIO_Config(void)
{
    PowerCtrl_Init(OUT);
    GreenLed_Init(OUT);
    RedLed_Init(OUT);
    KEY_Init(IN);
}
/*******************************************************************************
*	    函数名：ADC_Config
*******************************************************************************/
void ADC_Config(void)
{
    ADC_InitTypeDef mode;
    mode.ADCModeChannel = PortVmNum3;
    mode.ADCModeConversionBit = 10;
    mode.ADCModeConversionMode = 0;
    mode.ADCRef = 0;
    OA_ADC_Init(&mode);
}

/*******************************************************************************
*	    函数名：HardwareInit
*	            硬件初始化
*******************************************************************************/
void HardwareInit(void)
{
    GPIO_Config();
    PowerHold();

    RF_Init();
    WiFi_Init();
    ADC_Config();
    PowerHold();

    RedLed_On();//红灯亮
}

/**
 * @declare 获取ADC转换结果
 *
 * requirements
 *
 * @param AdcChannel ADC转换源
 *
 * @return unsigned short ADC转换结果
 */
unsigned short OA_ADC_GetValue(ADCChannel_ET AdcChannel)
{
    unsigned char j,index = 0;
    unsigned short temp = 0,TempAdValue = 0;
    
    OA_ADC_SetConversionChannel(ADCCHANNELVOLT);
    OA_ADC_CMD(ENABLE);
    for(j = 0;j < 5;j++)
    {
        temp = OA_ADC_GetConversionValue();
        if(temp != 0)
        {
            TempAdValue += temp ;
            index++;
        }
    }
    OA_ADC_CMD(DISABLE);
    return ( TempAdValue / index );
}

void DebugUARTRecv(unsigned char ch)
{
    COM_Buffer.UART_Buff[COM_Buffer.BufWrite] = ch;
    COM_Buffer.BufWrite++;
    if(COM_Buffer.BufWrite >= UART_BUFF_LEN)
        COM_Buffer.BufWrite = 0;
}

void Printf_DebugMessage(char *str)
{
    DebugUARTSend((unsigned char *)str, strlen((const char *)str));
}

//按键开机后调用 PowerHold() 保持供电电路PMOS导通,不具备开机功能
void PowerHold(void)
{
    PowerCtrl_Set();
}

//设备空闲时间过长时，调用 PowerOff() 掉电关机
void PowerOff(void)
{
    PowerCtrl_Clr();
}

void RedLed_On(void)
{
    RedLed_Set();
}

void RedLed_Off(void)
{
    RedLed_Clr();
}

void GreenLed_On(void)
{
    GreenLed_Set();
}

void GreenLed_Off(void)
{
    GreenLed_Clr();
}

unsigned short LedMaxTime;
//状态指示灯控制
void StateLEDControl(void)
{
//    SysStaLEDCtrl = ON;
    if(StateTimer > 0)
        StateTimer--;
    if(StateTimer > LedMaxTime)
        StateTimer = LedMaxTime;
    if(StateTimer == 0)
        StateTimer = LedMaxTime;
    switch(SystemState)
    {
        case SYS_STATE_IDLE:
            RedLed_On();
            break;

        case SYS_STATE_BUSY:
            LedMaxTime = 25;
            if(StateTimer < 5)
                RedLed_On();
            else
                RedLed_Off();
            break;

        case SYS_STATE_MUL_ERR:    //短闪2次
        case SYS_STATE_WiFi_ERR:
        case SYS_STATE_RF_ERR:
            LedMaxTime = 200;
            if(StateTimer < 5)
                RedLed_On();
            else if((StateTimer >= 5)&&(StateTimer < 25))
                RedLed_Off();
            else if((StateTimer >= 25)&&(StateTimer < 30))
                RedLed_On();
            else
                RedLed_Off();
            break;

        case SYS_STATE_UNDER_POWER:  //短闪3次
            LedMaxTime = 200;
            if(StateTimer < 5)
               RedLed_On();
            else if((StateTimer >= 5)&&(StateTimer < 25))
                RedLed_Off();
            else if((StateTimer >= 25)&&(StateTimer < 30))
                RedLed_On();
            else if((StateTimer >= 30)&&(StateTimer < 50))
                RedLed_Off();
            else if((StateTimer >= 50)&&(StateTimer < 55))
                RedLed_On();
            else
                RedLed_Off();
            break;

        default:
            RedLed_On();
            break;
    }
//    SysStaLEDCtrl = OFF;
}

//电压检测和LED状态控制，100ms一次
void Task_StateManage(void)
{
    unsigned long voltage = 0;

    if(PowerCheckTime)
        PowerCheckTime--;

    if(!SYS_IdleTOPowerDownTime) //空闲超时关机
    {
        PowerOff();
    }

//    SystemState = SYS_STATE_IDLE;

    if(!PowerCheckTime)
    {
        voltage = OA_ADC_GetValue(ADCCHANNELVOLT);
        voltage = (voltage * 330) >> 10;//单位 10mV
        voltage = (voltage * 3) + 60 ;//电池入口串了一个4148二极管，压降约 0.6V = 60个10mV
        if(voltage < LOW_VOLTAGE) //低压关机
        {
             PowerOff();//关机
        }
        else if(voltage < (UNDER_VOLTAGE - 10))    //欠压点亮欠压指示灯
        {
            SystemState = SYS_STATE_UNDER_POWER; //欠压
        }
        else if(voltage  > (UNDER_VOLTAGE  + 10))
        {
            SystemState = SYS_STATE_IDLE;//
            //SystemState = SYS_STATE_HIGHPOW; //满电
        }

        PowerCheckTime = 100; //10S检测一次电压
    }

    if(RF_Status != RF_IDLE )
    {
        LedMaxTime = 25;
        SystemState = SYS_STATE_BUSY;
    }

    if(WiFi_HWStatus == WIFI_ERROR)
    {
        LedMaxTime = 200;
        SystemState = SYS_STATE_WiFi_ERR;
    }

    StateLEDControl();
}


