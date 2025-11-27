#ifndef _APC340R_H_
#define _APC340R_H_    


#define RFPowerOn()  RF_POW_Clr()
#define RFPowerOff() RF_POW_Set()
#define RFIsBusy() RF_SETA_Get()

#define DEBUG_RF_ALL    0
#define DEBUG_RF_FRAME  1

#define RECV 0
#define SEND 1

enum
{
    RFS_IDLE = 0,   /*  RF 空闲模式         */
    RFS_RST,        /*  RF 低功耗模式       */
    RFS_HALT,       /*  RF 低功耗模式       */
    RFS_SEND,       /*  RF 发送模式         */
    RFS_RECV,       /*  RF 接收模式         */
    RFS_SET,        /*  RF 配置模式         */
    RFS_WAIT,       /*  RF 等待模式         */
    RFS_INIT,       /*  RF INIT             */
    RFS_GFSK,       /*  RF 切换到 GFSK      */
    RFS_GFSK1,       /*  RF 切换到 GFSK      */
    RFS_LORA,       /*  RF 切换到 LORA      */
    RFS_LORA1,       /*  RF 切换到 LORA      */
    RFS_HOST,       /*  RF配置为 HOST    */
};

//
///***********需要 BSP提供的函数 ***********/
//
////RF_SETA 输出控制
//void RF_SETA_Set(void);
//void RF_SETA_Clr(void);
//
////RF_SETB 输出控制
//void RF_SETB_Set(void);
//void RF_SETB_Clr(void);
//
////RF_AUX 输出控制
//void RF_AUX_Set(void);
//void RF_AUX_Clr(void);
//
////RF_SETA 端口方向初始化  OUT-输出，IN-输入
//void RF_SETA_Init(unsigned char mode);
//
////RF_SETB 端口方向初始化  OUT-输出，IN-输入
//void RF_SETB_Init(unsigned char mode);
//
////RF_AUX 端口方向初始化  OUT-输出，IN-输入
//void RF_AUX_Init(unsigned char mode);
//
//void RFUartConfig(pUartMode Mode, IRQServerSendDataFT SendFun, IRQServerRecvDataFT RecvFun);
//void RFUartSend(unsigned char *buf, unsigned int len);
//
///***********结束  需要 BSP提供的函数 ***********/

/******************************************************************************************************
*
*                                    安美通 RF接口
*
******************************************************************************************************/
#define RF_SETA_PORT GPIOA
#define RF_SETA_PIN   GPIO_Pin_12      /* PA12 */

#define RF_SETB_PORT GPIOA
#define RF_SETB_PIN   GPIO_Pin_8       /* PA8  */

#define RF_AUX_PORT  GPIOA
#define RF_AUX_PIN   GPIO_Pin_11       /* PA11  */

#define SPI_SPEED    5
/******************************************************************************************************/

extern COMM_Buffer_t RF_Buffer;   //RF

void RF_DelayUs(unsigned short us);
void RF_IO_Init(void);       /*RF模块引脚初始化*/
unsigned char RF_Init(void) ;
void RF_ChangeToNormal(void);
void RF_ChangeToWakeUp(void);
void RF_SW_FSM(void);
void Task_RF_HW(void);
void RFUartRecv(unsigned char ch);
#endif
/*********************************************************************************************************
**  End Of File
*********************************************************************************************************/
