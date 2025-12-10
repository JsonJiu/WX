#ifndef BSP_MC308_H
#define	BSP_MC308_H

#define DebugPrintf putsUART3
typedef enum USARTNUM_E
{
    UP_COM = 1,       //串口1 、、上位1
    Meter_COM = 2,    //串口0、、红外
    Debug_COM  = 3,   //串口2、、debug
    IR_COM = 4        //串口3、、主控板2
}USARTNUM_EU;

/***********************************串口配置*********************************/
typedef enum
{
    COM1 = 1,
    COM2 = 2,
    COM3 = 3,
    COM4 = 4
} USART_TypeDef;



typedef enum
{
    Null_0      = 0,
    GREEN_LED   = 1,
    RED_LED     = 2,
    Null_3      = 3,
    BEEP_LED    = 4,
} LED_TypeDef;

#define BEEP_LED_COUNT   	3

#define CLOSE_LED   		0
#define OPEN_LED    		1

typedef enum
{
    BUTTON_PROGRAM = 0, //编程按键
    BUTTON_USERS = 1,   //用户按键
} Button_TypeDef;

#define  PROGRAM_KEY        BUTTON_PROGRAM
#define  USERS_KEY          BUTTON_USERS

void User_TimerCfg(unsigned char type, unsigned char len, IRQServerFT CallBack);
void User_TimerStart(unsigned char type);
void User_TimerStop(unsigned char type);
void IO_VBB_CHK_EN_DIR_OUT(void);
void IO_VBB_CHK_EN_1(void);
void IO_VBB_CHK_EN_0(void);

void OA_ADC_Init(ADC_InitTypeDef* ADC_InitStruct);
void OA_ADC_CMD(FunctionalState NewState);
unsigned short OA_ADC_GetConversionValue(void);
void OA_ADC_SetConversionChannel(unsigned char channel);
void UsartConfig(USART_TypeDef com, UartBaud baud, void(*fun)(unsigned char rxd));
void OpenUsart(USART_TypeDef com);
void CloseUsart(USART_TypeDef com);
void SetUsartIoOut(USART_TypeDef com, unsigned char fun);
void SetUartTxDirIn(void);
unsigned char UsartSendData(USART_TypeDef com, unsigned char *buf, unsigned short len);
void UsartTxConfig(USART_TypeDef com, unsigned char fun);
void SetUsbUartTXRXIN(void);
void Init_DeviceID(void);
unsigned char GetP0Status(void);
unsigned char GetP1Status(void);
unsigned char GetP2Status(void);
void SetMasterBoardPower(unsigned char fun);
void SetCheckConsumptionPower(unsigned char fun);
void Set_MasterLowConsManage(unsigned char fun);
void SetMasterOnLineIO_Dir(void);
unsigned char GetMasterOnLineIOStatus(void);
void ZDSetIntIo(void(*p)(void));
void CloseZdPowerInt(void);
void SetIrdaPower(unsigned char fun);
void LEDOn(LED_TypeDef type);
void LEDOff(LED_TypeDef type);
void LedOrBeepTick(LED_TypeDef type, unsigned short count, unsigned short time_on, unsigned short time_off);




#endif

