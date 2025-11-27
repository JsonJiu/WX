#include    "bsp_rs004.h"

/* Set option bytes */
#pragma location = "OPTBYTE"
__root const uint8_t opbyte0 = 0xEFU;
#pragma location = "OPTBYTE"
__root const uint8_t opbyte1 = 0x5FU;
#pragma location = "OPTBYTE"
__root const uint8_t opbyte2 = 0xEAU;
#pragma location = "OPTBYTE"
__root const uint8_t opbyte3 = 0x04U;

/* Set security ID */
#pragma location = "SECUID"
__root const uint8_t secuid[10] = 
    {0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U, 0x00U};

extern void IrqServerDefaultRS(unsigned char Data);
extern void IrqServerDefault(void);

void IM_m2m_Set_MotorSleep_GPIO(unsigned char fun)
{
    PORT0OutputConfig(BIT3);
    PFSEG5ClearBit(BIT7);
    if (fun == 1)
    {
        PORT0SetBit(BIT3);
    }
    else
    {
        PORT0ClearBit(BIT3);
    }
}
void IM_m2m_Set_MotorOpen_GPIO(unsigned char fun)
{
    PORT12OutputConfig(BIT7);
    if (fun == 1)
    {
        PORT12SetBit(BIT7);
    }
    else
    {
        PORT12ClearBit(BIT7);
    }
}
void IM_m2m_Set_MotorClose_GPIO(unsigned char fun)
{
    PORT12OutputConfig(BIT6);
    if (fun == 1)
    {
        PORT12SetBit(BIT6);
    }
    else
    {
        PORT12ClearBit(BIT6);
    }
}

void BspDoorClose(void)//完成
{
    IO_M_SLEEP_DIR_OUT;
    IO_M_CTRL1_DIR_OUT;
    IO_M_CTRL2_DIR_OUT;
    IO_M_SLEEP_1;
    IO_M_CTRL1_0;
    IO_M_CTRL2_1;
}

void BspDoorOpen(void)//完成
{
    IO_M_SLEEP_DIR_OUT;
    IO_M_CTRL1_DIR_OUT;
    IO_M_CTRL2_DIR_OUT;
    IO_M_SLEEP_1;
    IO_M_CTRL1_1;
    IO_M_CTRL2_0;
}

void BspDoorStop(void)//完成
{
    IO_M_SLEEP_DIR_OUT;
    IO_M_CTRL1_DIR_OUT;
    IO_M_CTRL2_DIR_OUT;
    IO_M_CTRL1_0;
    IO_M_CTRL2_0;
    IO_M_SLEEP_0;
}

void BspBeepOn(void)//完成
{
    IO_BEEP_DIR_OUT;
    IO_BEEP_1;
}
void BspBeepOff(void)//完成
{
    IO_BEEP_DIR_OUT;
    IO_BEEP_O;
}
//------------------------------------------------------------------------
//------------------------------------------------------------------------
//邹航邹航邹航邹航邹航邹航邹航邹航邹航邹航邹航邹航邹航邹航邹航邹航邹航邹航
//------------------------------------------------------------------------
//------------------------------------------------------------------------
//------------------------------------
//液晶液晶液晶液晶液晶液晶液晶液晶液晶
//------------------------------------
//开启液晶显示
void LcdOn(void)//完成
{
    MCU_LcdOpen();
}
//关闭液晶显示
void LcdOff(void)//完成
{
    MCU_LcdClose();
}
//想底层液晶显示寄存器中写入数值
//x：寄存器地址
//reg：寄存器值
void LcdWriteReg(unsigned char x, unsigned char reg)//完成
{
    MCU_LcdWriteReg(x, reg);
}
//读取寄存器地址中的值
unsigned char LcdReadReg(unsigned char x)//完成
{
    return MCU_LcdReadReg(x);
}
//液晶底层初始化
void LcdInit(void)//完成
{
    {
        /* Set segment pins */
        PU3 &= 0xFAU;
        PU5 &= 0x80U;
        PU7 &= 0x00U;
        POM5 &= 0x87U;
        PIM5 &= 0xD7U;
        PFSEG0 |= 0xF0U;
        PFSEG1 |= 0xF7U;
        PFSEG2 |= 0x5FU;
        P3 &= 0xFAU;
        PM3 &= 0xFAU;
        P5 &= 0x80U;
        PM5 &= 0x80U;
        P7 &= 0x00U;
        PM7 &= 0x00U;
    }
    MCU_LcdCfg();
}
//------------------------------------
//霍尔霍尔霍尔霍尔霍尔霍尔霍尔霍尔霍尔
//HALLA-HALL2-INT4-P3.1
//HALLB-HALL1-INT0-P13.7
//------------------------------------
//获取霍尔io口状态

unsigned char GpioHallARead(void)//完成
{
    return IO_HALL1_READ;
}
unsigned char GpioHallBRead(void)//完成
{
    return IO_HALL2_READ;
}
//改变中断触发边沿
//0：上升沿；1：下降沿
void HallASetInt(unsigned char c,void(*p)(void))//完成
{
    if(c == 0)
        INT3Config(RISING_EDGE_INT,p);
    else
        INT3Config(FALLING_EDGE_INT,p);
    EnableINT3;
}
void HallBSetInt(unsigned char c,void(*p)(void))//完成
{
    if(c == 0)
        INT0Config(RISING_EDGE_INT,p);
    else
        INT0Config(FALLING_EDGE_INT,p);
	EnableINT0;
}

//------------------------------------------------------------------------
//------------------------------------------------------------------------
//林扬宣林扬宣林扬宣林扬宣林扬宣林扬宣林扬宣林扬宣林扬宣林扬宣林扬宣林扬宣
//------------------------------------------------------------------------
//------------------------------------------------------------------------
//------------------------------------
//nand flash nand flash nand flash
//------------------------------------
//输出高/低
#define FLASH_CS(a)             
//输出高/低
#define FLASH_WP(a)             
//输出高/低
#define FLASH_MOSI(a)           
//输出高/低
#define FLASH_CLK(a)            
//输出高/低
#define FLASH_POWER(a)          
//输入
#define FLASH_MISO()            

//------------------------------------------------------------------------
//------------------------------------------------------------------------
//朱毓烜朱毓烜朱毓烜朱毓烜朱毓烜朱毓烜朱毓烜朱毓烜朱毓烜朱毓烜朱毓烜朱毓烜
//------------------------------------------------------------------------
//------------------------------------------------------------------------
//------------------------------------
//ADCADCADCADCADCADCADCADCADCADCADCADC
//------------------------------------
void IO_VBB_CHK_EN_DIR_OUT(void)
{
    PORT12OutputConfig(BIT5)
}
void IO_VBB_CHK_EN_1(void)
{
    PORT12SetBit(BIT5);
}
void IO_VBB_CHK_EN_0(void)
{
    PORT12ClearBit(BIT5);
}
/**
* @declare ADC初始化
*
* @author Administrator (2014-11-03)
*
* @requirements
*
* @param ADC_InitStruct ADC配置信息
*/
void OA_ADC_Init(ADC_InitTypeDef* ADC_InitStruct)//完成
{
    ADCModeST ADCMode;
    ADCMode.ADCModeOpt = ADC_InitStruct->ADCModeConversionMode;
    if (ADC_InitStruct->ADCModeConversionBit == 8)
        ADCMode.ADCModeResolution = 0;
    else
        ADCMode.ADCModeResolution = 1;

    if (ADC_InitStruct->ADCModeChannel == 1)
    {
        MCU_ADCCfg(PortVmNum17, ADCMode);//阀门
    }
    else
    {
        MCU_ADCCfg(PortVmNum16, ADCMode);//系统
    }
}
/**
* @declare 使能或者失能指定的ADC
*
* @author Administrator (2014-11-03)
*
* @requirements
*
* @param NewState 外设ADCx的新状态
*/
void OA_ADC_CMD(FunctionalState NewState)//完成
{
    if (NewState == DISABLE)
        MCU_ADCStop(PortVmNum0);
    else
        MCU_ADCStart(PortVmNum0);
}
/**
* @declare 返回最近一次ADCx的转换结果
*
* @author Administrator (2014-11-03)
*
* @requirements
*
* @return unsigned short 16位模式ADC返回结构
*/
unsigned short OA_ADC_GetConversionValue(void)//完成
{
    return MCU_ADCRead();
}
/**
* @declare 获取ADC转换状态
*
* @author Administrator (2014-11-03)
*
* @requirements
*
*
* @return unsigned char 转换完成返回0;正在转换中返回1
*/
unsigned char OA_ADC_GetConversionState(void)//完成
{
    return (unsigned char)ADCS;
}
/**
* @declare 设置ADC转换通道
*
* @author ZYX (2014/11/6)
*
* requirements
*
* @param channel ADC转换通道:0
*/
typedef enum
{
    ADCCHANNELVOLT = 0,
    ADCCHANNELCUR,
}ADCChannelET;

void OA_ADC_SetConversionChannel(unsigned char channel)//完成
{
    if (channel == 1)
    {
        ADS = 0x11;//(PortVmNum17, ADCMode);//阀门
    }
    else
    {
        ADS = 0x10;//(PortVmNum16, ADCMode);//系统
    }
}
//------------------------------------------------------------------------
//------------------------------------------------------------------------
//王兵王兵王兵王兵王兵王兵王兵王兵王兵王兵王兵王兵王兵王兵王兵王兵王兵王兵
//------------------------------------------------------------------------
//------------------------------------------------------------------------

/*===============================LED配置==============================*/
/**R-P1.2,G-P1.3
* LED灯方向配置:红色灯
*    配置成输出
* @author wb (2014-10-31)
*/
void IM_m2m_RedLED_Dir_Config(void)//完成
{
    R_LED_OUT;
}
/**
* LED灯方向配置:绿色灯
*    配置成输出
* @author wb (2014-10-31)
*/
void IM_m2m_GreenLED_Dir_Config(void)//完成
{
    G_LED_OUT;
}
/**
* LED灯IO配置:红色
*
* @author wb (2014-11-06)
*
* @param fun :0-置低；1-置高
*/
void IM_m2m_Set_RedLED_GPIO(unsigned char fun)//完成
{
    if (fun == 0)
    {
        R_LED_0;
    }
    else
    {
        R_LED_1;
    }
}
/**
* LED灯IO配置:绿色
*
* @author wb (2014-11-06)
*
* @param fun :0-置低；1-置高
*/
void IM_m2m_Set_GreenLED_GPIO(unsigned char fun)//完成
{
    if (fun == 0)
    {
        G_LED_0;
    }
    else
    {
        G_LED_1;
    }
}
/*===============================KEY配置==============================*/
/**
* 获取编程按键的状态
* INT4,S1,编程,P3.3
* INT7,S2,用户,P0.2
* @author wb (2014-10-31)
*
* @return unsigned char
*      :返回0：表示按键按下；返回1：表示按键松开
*/
unsigned char IM_m2m_GetProgrameKey_Status(void)//完成
{
    if (IO_KEY_PROM_READ)
        return 1;
    else
        return 0;
}
/**
* 获取用户按键的状态
*
* @author wb (2014-10-31)
*
* @return unsigned char
*      :返回0：表示按键按下；返回1：表示按键松开
*/
unsigned char IM_m2m_GetUsersKey_Status(void)//完成
{
    if (IO_KEY_USER_READ)
        return 1;
    else
        return 0;
}
/**
* 打开外部中断
*
* @author wb (2014-11-10)
*
* @param key_type ：中断类型:0:用户按键  1：编程按键
*/
void IM_OpenExt_Int(unsigned char key_type)//完成
{
    if (key_type == 0)
    {
        EnableINT7;
    }
    else
    {
        EnableINT4;
    }
}
/**
* 关闭外部中断
*
* @author wb (2014-11-10)
*
* @param key_type ：中断类型
*/
void IM_CloseExt_Int(unsigned char key_type)//完成
{
    if (key_type == 0)
    {
        DisableINT7;
    }
    else
    {
        DisableINT4;
    }
}
/**
* 中断初始化:编程按键
*    配置成中断接口，，下降沿触发，不带上拉电阻；
* @author wb (2014-10-31)
*
* @param fun 中断服务函数指针：fun由驱动编写，由BSP调用
*/
void IM_m2m_ProgrameKey_Intp_Config(void(*fun)(void))//完成
{
    INT4Config(FALLING_EDGE_INT, fun);
}
/**
* 中断初始化：用户按键
*    配置成中断接口，，下降沿触发，不带上拉电阻；
* @author wb (2014-10-31)
*
* @param fun 中断服务函数指针：fun由驱动编写，由BSP调用
*/
void IM_m2m_UserKey_Intp_Init(void(*fun)(void))//完成
{
    INT7Config(FALLING_EDGE_INT,fun);
}
/*==============================串口配置==============================*/
/**
* USB->com=0->uart0:    TX:P0.0:PFSEG43//   PFSEG5,BIT4
                        RX:P1.7:PFSEG42//   PFSEG5,BIT3
* GPRS->com=1->uart1:   TX:P0.6:PFSEG49//   PFSEG6,BIT2
                        RX:P0.7:PFSEG50//   PFSEG6,BIT2
* 光电->com=2->uart3:   RX:P3.4:PFSEG24//   PFSEG3,BIT0
*配置USART 发送IO:TX输出高
*
* @author wb (2014-11-06)
*
* @param com :串口端口:0-USB;1-GPRS;2-光电;3-默认
* @param fun :1-置高；0-置低
*/
void IM_m2m_Set_Usart_TX_GPIO(unsigned char com, unsigned char fun)//完成
{
    switch (com)
    {
    default:
    case 0:
        if (fun == 1)
        {
            PFSEG5 &= ~BIT4;
            PORT0OutputConfig(BIT0);
            PORT0SetBit(BIT0);
        }
        else
        {
            PFSEG5 &= ~BIT4;
            PORT0OutputConfig(BIT0);
            PORT0ClearBit(BIT0);
        }
        break;
    case 1:
        if (fun == 1)
        {
            PFSEG6 &= ~BIT2;
            PORT0OutputConfig(BIT6);
            PORT0SetBit(BIT6);
        }
        else
        {
            PFSEG6 &= ~BIT2;
            PORT0OutputConfig(BIT6);
            PORT0ClearBit(BIT6);
        }
        break;
    case 2:
        if (fun == 1)
        {
        }
        else
        {
        }
        break;
    case 3:
        if (fun == 1)
        {
        }
        else
        {
        }
        break;
    }
}
/**
*配置USART 接收IO:配置为输入
*
* @author wb (2014-11-06)
*
* @param com :串口端口:0-USB;1-GPRS;2-光电;3-默认
*/
void IM_m2m_Config_Usart_RX_DIR(unsigned char com)//完成
{
    switch (com)
    {
    default:
    case 0:
        PFSEG5 &= ~BIT3;
        PORT1InputConfig(BIT7);
        break;
    case 1:
        PFSEG6 &= ~BIT3;
        PORT0InputConfig(BIT7);
        break;
    case 2:
        PFSEG3 &= ~BIT0;
        PORT3InputConfig(BIT4);
        break;
    case 3:break;
    }
}
/**
* 串口配置：UART ；1 
* 位起始位，1位停止位，8位数据位，无校验 
* 全部都使能接收中断 
* @author wb (2014-11-06)
*
* @param com :串口端口:0-USB;1-GPRS;2-光电;3-默认
* @param baud 
*             :波特率：0-1200；1-2400;2-4800;3-9600；4-19200;5-38400;6-115200
* @param fun ：串口接收中断服务函数
*/
void IM_m2m_Uart_Config(unsigned char com, unsigned long baud, void(*fun)(unsigned char rxd))//完成
{
    UartMode Mode;
    switch (baud)
    {
    case 0:
        Mode.Baud = UartBaud1200;
        break;
    case 1:
        Mode.Baud = UartBaud2400;
        break;
    case 2:
        Mode.Baud = UartBaud4800;
        break;
    case 3:
        Mode.Baud = UartBaud9600;
        break;
    case 4:
        Mode.Baud = UartBaud19200;
        break;
    case 5:
        Mode.Baud = UartBaud38400;
        break;
    case 6:
        Mode.Baud = UartBaud115200;
        break;
    }
    Mode.Parity = 0;
    Mode.StopBit = 1;
    Mode.DataBit = 1;
    Mode.Sequence = 0;
    switch (com)
    {
    default:
    case 0:
        Uart0Config(&Mode,0,fun);
        break;
    case 1:
        Uart1Config(&Mode, 0, fun);
        break;
    case 2:
        Uart3Config(&Mode, 0, fun);
        break;
    case 3:break;
    }
}
/**
* 串口配置：打开串口
*
* @author wb (2014-11-06)
*
* @param com ：串口端口:0-USB;1-GPRS;2-光电;3-默认
*/
void IM_m2m_Open_Usart(unsigned char com)//完成
{
    switch (com)
    {
    default:
    case 0:
        //Uart0SendEnable();
        Uart0ReceiveEnable();
        break;
    case 1:
        //Uart1SendEnable();
        Uart1ReceiveEnable(); 
        break;
    case 2:
        //Uart3SendEnable();
        Uart3ReceiveEnable(); 
        break;
    case 3:break;
    }
}
/**
* 串口配置：关闭串口
*
* @author wb (2014-11-06)
*
* @param com :串口端口:0-USB;1-GPRS;2-光电;3-默认
*/
void IM_m2m_Close_Usart(unsigned char com)//完成
{
    switch (com)
    {
    default:
    case 0:
        //Uart0SendDisable();
        Uart0ReceiveDisable();
        break;
    case 1:
        //Uart1SendDisable();
        Uart1ReceiveDisable(); 
        break;
    case 2:
        //Uart3SendDisable();
        Uart3ReceiveDisable(); 
        break;
    case 3:break;
    }
}

/**
* 串口发送数据
* 
* @author wb (2014-11-19)
* 
* @param com ：串口端口:0-USB;1-GPRS;2-光电;3-默认
* @param buf :发送数据缓冲区
* @param len ：发送数据长度
* 
* @return unsigned char ：返回1-发送成功；返回0-发送失败
*/

unsigned char IM_m2m_Usart_Send_Data(unsigned char com,unsigned char *buf,unsigned short len)//完成
{
    switch (com)
    {
    default:
    case 0:
        Uart0SendData(buf,len);
        break;
    case 1:
        Uart1SendData(buf, len);
        break;
    case 2:
        Uart3SendData(buf, len);
        break;
    case 3:break;
    }
    return 1;
}

///*===============================gprs模块配置==============================*/
//
///**
// * GSM模块电源管脚P4.1
// * 
// * @author wb (2014-11-06)
// * 
// * @param fun :0-置低；1-置高
// */
//
void IM_m2m_Set_GSM_Modem_Power_GPIO(unsigned char fun)
{
    PORT4OutputConfig(BIT1);
    if (fun == 1)
    {
        PORT4SetBit(BIT1);
    }
    else
    {
        PORT4ClearBit(BIT1);
    }
}
// 
///**
// * GSM模块开关机管脚配置P4.4
// * 
// * @author wb (2014-11-06)
// * 
// * @param fun 
// */
//
void IM_m2m_Set_GSM_Modem_PowerOnOff_GPIO(unsigned char fun)
{
    PORT4OutputConfig(BIT4);
    POM4ClearBit(BIT4);
    PMC4ClearBit(BIT4);
    if (fun == 1)
    {
        PORT4SetBit(BIT4);
    }
    else
    {
        PORT4ClearBit(BIT4);
    }
}

//
//------------------------------------------------------------------------
//------------------------------------------------------------------------
//115 115 115 115 115 115 115 115 115 115 115 115 115 115 115 115 115 115 
//------------------------------------------------------------------------
//------------------------------------------------------------------------
//prt:0表示EEPROM/RTC8025T模拟口;
//EEPROM/RTC8025T:
//PWR: P1.4; SEG39; PFSEG5,BIT0//CLR表示上电
//SCL: P1.6; SEG41; PFSEG5,BIT2
//SDA: P1.5; SEG40; PFSEG5,BIT1
void IM_IICPWR_OUT(unsigned char prt) //prt,0:1号模拟口,1:2号模拟口
{
    PORT1OutputConfig(BIT4);
    CLR_BIT(PFSEG5, BIT0);
}
//I2C电源配置上拉   
void IM_IICPWR_UP(unsigned char prt) //prt,0:1号模拟口,1:2号模拟口  
{
    //PORT1PullUpBit(BIT4);
}
//I2C电源配置下拉   
void IM_IICPWR_DOWN(unsigned char prt) //prt,0:1号模拟口,1:2号模拟口    
{

}
//I2C电源输出高 
void IM_IICPWR_ON(unsigned char prt) //prt,0:1号模拟口,1:2号模拟口
{
    PORT1ClearBit(BIT4);
}
//I2C电源输出低 
void IM_IICPWR_OFF(unsigned char prt) //prt,0:1号模拟口,1:2号模拟口
{
    PORT1SetBit(BIT4);
}   
//prt,0:1号模拟口,1:2号模拟口
void IM_IICSCL_OUT(unsigned char prt)
{
    PORT1OutputConfig(BIT6);
    CLR_BIT(POM1, BIT6);
    CLR_BIT(PFSEG5, BIT2);
}
//prt,0:1号模拟口,1:2号模拟口
void IM_IICSCL_IN(unsigned char prt)
{
    PORT1InputConfig(BIT6);
    CLR_BIT(PIM1, BIT6);
    CLR_BIT(PFSEG5, BIT2);
}
//prt,0:1号模拟口,1:2号模拟口
void IM_IICSCL_UP(unsigned char prt)
{
    //PORT1PullUpBit(BIT6);
}
// prt,0:1号模拟口,1:2号模拟口
void IM_IICSCL_DOWN(unsigned char prt)
{
}
// prt,0:1号模拟口,1:2号模拟口
void IM_IICSCL_SET(unsigned char prt)
{
    PORT1SetBit(BIT6);
}
// prt,0:1号模拟口,1:2号模拟口
void IM_IICSCL_CLR(unsigned char prt)
{
    PORT1ClearBit(BIT6);
}
// prt,0:1号模拟口,1:2号模拟口
void IM_IICSDA_OUT(unsigned char prt)
{
    PORT1OutputConfig(BIT5);
    CLR_BIT(PFSEG5, BIT1);
}
//prt,0:1号模拟口,1:2号模拟口
void IM_IICSDA_IN(unsigned char prt)
{
    PORT1InputConfig(BIT5);
    CLR_BIT(PFSEG5, BIT1);
}
//prt,0:1号模拟口,1:2号模拟口
void IM_IICSDA_UP(unsigned char prt)
{
    //PORT1PullUpBit(BIT5);
}
// prt,0:1号模拟口,1:2号模拟口
void IM_IICSDA_DOWN(unsigned char prt)
{
}
// prt,0:1号模拟口,1:2号模拟口
void IM_IICSDA_SET(unsigned char prt)
{
    PORT1SetBit(BIT5);
}
// prt,0:1号模拟口,1:2号模拟口
void IM_IICSDA_CLR(unsigned char prt)
{
    PORT1ClearBit(BIT5);
}
// prt,0:1号模拟口,1:2号模拟口
unsigned char IM_IICSDA_STU(unsigned char prt)
{
    return (P1&BIT5);
}
// prt,0:1号模拟口,1:2号模拟口
//返回值，0:低，非0:高

//------------------------------------------------------------------------
//------------------------------------------------------------------------
//谢尚川谢尚川谢尚川谢尚川谢尚川谢尚川谢尚川谢尚川谢尚川谢尚川谢尚川谢尚川
//------------------------------------------------------------------------
//------------------------------------------------------------------------

/**
* 电源引脚初始化
*
* @author xsc (2014-11-09)
*
* @param dir 方向
* @param pp 上拉
*/
#if 0
void ZD_POW_Init(IODIR dir, IOPP pp)
{
    if(dir == IODIR_Output)
    {
        IO_GD_EN_DIR_OUT;
    }
    else
    {
        IO_GD_EN_DIR_IN;
    }
    if(pp == IOPP_PP)
    {
        IO_GD_EN_PullUp;
    }
}

/**
* 电源开关控制
*
* @author xsc (2014-11-09)
*
* @param c 控制参数En/Dis
*/
void ZD_POW_Ctrl(EnDis c)
{
    if (c == En)
    {
        IO_GD_EN_1;
    }
    else
    {
        IO_GD_EN_0;
    }
}

/**
* 硬件串口初始化
*
* @author xsc (2014-11-09)
*
* @param mode
* @param fun
*/
void ZDUart_Init(pUartMode mode, void(*fun)(unsigned char))
{
    Uart3Config(mode,IrqServerDefaultRS,fun);
}

/**
 * 使能直读头串口
 * 
 * @author xsc (2014/11/21)
 */
void ZDUart_RX_En(void)
{
    Uart3ReceiveEnable();
}

/**
 * 关闭直读头串口
 * 
 * @author xsc (2014/11/21)
 */
void ZDUart_RX_Ds(void)
{
    Uart3ReceiveDisable();
}
#endif
//------------------------------------------------------------------------
//------------------------------------------------------------------------
//林扬宣林扬宣林扬宣林扬宣林扬宣林扬宣林扬宣林扬宣林扬宣林扬宣林扬宣林扬宣
//------------------------------------------------------------------------
//------------------------------------------------------------------------
//typedef void (*ISR_FUNC_PTR)(void);
//type: 0 主时钟
//type: 1 低频时钟
//len: 0 1ms
//len: 1 100ms
//len: 2 1000ms
static char IntTimeLen = 0;
void User_TimerCfg(unsigned char type, unsigned char len, IRQServerFT CallBack)
{
    if (type == 0)
        MCU_TimerCfg(PortVmNum0, TimeSpMs1, CallBack);
    else
    {
        IntTimeLen = len;
        if (len == 1)
            MCU_TimerCfg(PortVmNum8, TimeSpMs100, CallBack);
        else
            MCU_TimerCfg(PortVmNum9, TimeSpMs1000, CallBack);//MCU_TimerCfg(PortVmNum8, TimeSpMs100, CallBack);
    }
}
//type: 0 主时钟
//type: 1 低频时钟
void User_TimerStart(unsigned char type)
{
    if (type == 0)
        MCU_TimerStart(PortVmNum0);
    else
    {
        if (IntTimeLen == 1)
            MCU_TimerStart(PortVmNum8);
        else
            MCU_TimerStart(PortVmNum9);
    }
}
//type: 0 主时钟
//type: 1 低频时钟
void User_TimerStop(unsigned char type)
{
    if (type == 0)
        MCU_TimerStop(PortVmNum0);
    else
    {
        if (IntTimeLen == 1)
            MCU_TimerStop(PortVmNum8);
        else
            MCU_TimerStop(PortVmNum9);
    }
}