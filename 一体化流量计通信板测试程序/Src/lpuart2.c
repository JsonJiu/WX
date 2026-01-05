#include "main.h"
#include "uart3.h"
#include "lpuart2.h"
#include "stdarg.h"
#include "time.h"
#include "LED_CTRL.h"
#include "PC_xieyi_Ctrl.h"
#include "tongxin_xieyi_Ctrl.h"
#define lenth_Receive_Send_MAX 200

uint8_t lpuart2_Rec_shuju_flag = 0;
uint8_t lpuart2_Rec_shuju_neirong[lenth_Receive_Send_MAX];
uint16_t lpuart2_Rec_shuju_neirong_count = 0;
uint8_t lpuart2_Uart0_Tx_SendData[lenth_Receive_Send_MAX];
uint8_t send_data_zancun_lpuart2[lenth_Receive_Send_MAX];
uint8_t send_over_flag_lpuart2 = 1;
//这一步需要注册到time时钟里
uint16_t chaoshi_dengdai_lpuart2 = 0;
uint16_t lpuart2_Rec_shuju_time_count = 0;

struct UARTOpStruct LPUART2Op;

//中断服务
void LPUARTx_IRQHandler(void)
{
    //LPUART2接收中断处理
    if((FL_ENABLE == FL_LPUART_IsEnabledIT_RXBuffFull(LPUART2))
            && (FL_SET == FL_LPUART_IsActiveFlag_RXBuffFull(LPUART2)))      //接收数据中断
    {
			//中断接收
        lpuart2_Rec_shuju_neirong[lpuart2_Rec_shuju_neirong_count] = FL_LPUART_ReadRXBuff(LPUART2);    //接收中断标志可通过读取rxreg寄存器清除
			  lpuart2_Rec_shuju_neirong_count++;
			  if(lpuart2_Rec_shuju_neirong_count>=lenth_Receive_Send_MAX)
			  {
				 lpuart2_Rec_shuju_neirong_count = 0;
			  }
			  lpuart2_Rec_shuju_flag = 1;
			  lpuart2_Rec_shuju_time_count = 100;
    }

    //LPUART2发送中断处理
    if((FL_ENABLE == FL_LPUART_IsEnabledIT_TXShiftBuffEmpty(LPUART2))
            && (FL_SET == FL_LPUART_IsActiveFlag_TXShiftBuffEmpty(LPUART2)))
    {
			//这里发送完成之后再进进入，可以解决最后一个字节发不完的问题
				if(LPUART2Op.TxOpc == LPUART2Op.TxLen)
        {
            FL_LPUART_DisableIT_TXShiftBuffEmpty(LPUART2);
					  send_over_flag_lpuart2 = 1;
        }
        //发送中断标志可通过写txreg寄存器清除或txif写1清除
        //发送指定长度的数据
        if(LPUART2Op.TxOpc < LPUART2Op.TxLen)
        {
            FL_LPUART_WriteTXBuff(LPUART2, LPUART2Op.TxBuf[LPUART2Op.TxOpc]); //发送一个数据
            LPUART2Op.TxOpc++;
        }

			
        FL_LPUART_ClearFlag_TXShiftBuffEmpty(LPUART2);                      //清除发送中断标志
        /* 用户代码*/
    }
}

void MF_LPUART2_Init(void)
{
    FL_GPIO_InitTypeDef    GPIO_InitStruct;

    FL_LPUART_InitTypeDef    LPUART2_InitStruct;

    GPIO_InitStruct.pin = FL_GPIO_PIN_4;                                                 /*配置GPIO的引脚号*/
    GPIO_InitStruct.mode = FL_GPIO_MODE_DIGITAL;                                          /*配置GPIO的功能模式*/
    GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;                                 /*配置GPIO的输出模式*/
    GPIO_InitStruct.pull = FL_DISABLE;                                                    /*配置GPIO输上拉模式*/
    GPIO_InitStruct.remapPin = FL_ENABLE;                                                 /*配置GPIO数字重定向功能*/
    GPIO_InitStruct.analogSwitch = FL_DISABLE;                                            /*配置GPIO模拟开关功能*/
    FL_GPIO_Init(GPIOB, &GPIO_InitStruct);                                                /*GPIO初始化*/

    GPIO_InitStruct.pin = FL_GPIO_PIN_5;                                                 /*配置GPIO的引脚号*/
    GPIO_InitStruct.mode = FL_GPIO_MODE_DIGITAL;                                          /*配置GPIO的功能模式*/
    GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;                                 /*配置GPIO的输出模式*/
    GPIO_InitStruct.pull = FL_DISABLE;                                                    /*配置GPIO输上拉模式*/
    GPIO_InitStruct.remapPin = FL_ENABLE;                                                 /*配置GPIO数字重定向功能*/
    GPIO_InitStruct.analogSwitch = FL_DISABLE;                                            /*配置GPIO模拟开关功能*/
    FL_GPIO_Init(GPIOB, &GPIO_InitStruct);                                                /*GPIO初始化*/

    LPUART2_InitStruct.baudRate = FL_LPUART_BAUDRATE_9600;                                /*配置LPUART波特率*/
    LPUART2_InitStruct.dataWidth = FL_LPUART_DATA_WIDTH_8B;                               /*配置LPUART数据位数*/
    LPUART2_InitStruct.stopBits = FL_LPUART_STOP_BIT_WIDTH_1B;                            /*配置LPUART数据停止位*/
    LPUART2_InitStruct.parity = FL_LPUART_PARITY_NONE;                                    /*配置LPUART数据奇偶检验位*/
    LPUART2_InitStruct.transferDirection = FL_LPUART_DIRECTION_NONE;                      /*配置LPUART发送、接收使能*/
    LPUART2_InitStruct.clockSrc = FL_CMU_LPUART_CLK_SOURCE_LSCLK;                         /*配置LPUART工作时钟*/

    FL_LPUART_Init(LPUART2, &LPUART2_InitStruct);                                         /*LPUART初始化*/
}
void LPUART2_Tx_Send_init()
{
	LPUART2Op.TxLen =1;
	LPUART2Op.TxOpc = 1;
}

/**
  * @brief  LPUART0 Interrupt Initialization function
  * @param  void
  * @retval None
  */
void MF_LPUART2_Interrupt_Init(void)
{
    FL_LPUART_ClearFlag_RXBuffFull(LPUART2);                                              /*配置LPUART中断*/
    FL_LPUART_EnableIT_RXBuffFull(LPUART2);

    FL_LPUART_ClearFlag_TXShiftBuffEmpty(LPUART2);
    FL_LPUART_EnableIT_TXShiftBuffEmpty(LPUART2);
}

/**
  * @brief  NVIC Initialization function
  * @param  void
  * @retval None
  */
void MF_NVIC_LPUART2_Init(void)
{
    FL_NVIC_ConfigTypeDef    InterruptConfigStruct;

    InterruptConfigStruct.preemptPriority = 0x02;                                         /*配置LPUART_NVIC*/
    FL_NVIC_Init(&InterruptConfigStruct, LPUARTx_IRQn);
}



void LPUART2_MF_Config_Init(void)
{
    /* Initial LPUART0 */
    MF_LPUART2_Init();                                                                    /*LPUART初始化*/
    MF_LPUART2_Interrupt_Init();                                                          /*配置LPUART中断*/

    /* Initial NVIC */
    MF_NVIC_LPUART2_Init();                                                                       /*配置LPUART_NVIC*/
	  //延时喂狗防止重启
		FL_IWDT_ReloadCounter(IWDT);
		FL_DelayMs(1000);//32768 起振时间典型时间1S，最大3S
		FL_IWDT_ReloadCounter(IWDT);
		FL_DelayMs(1000);
		FL_IWDT_ReloadCounter(IWDT);
		FL_DelayMs(1000);
		FL_IWDT_ReloadCounter(IWDT);
    FL_CDIF_EnableVAOToCPU(CDIF);//32768信号从VAO传输到CPU
		FL_LPUART_EnableRX(LPUART2);
    FL_LPUART_EnableTX(LPUART2);
		LPUART2_Tx_Send_init();
}

void LPUART2_Tx_Send(uint8_t zufuchua[],uint16_t lenth)
{
	if(lenth==0)
	{
		return;
	}
	chaoshi_dengdai_lpuart2 = 100;
	while(LPUART2Op.TxLen != LPUART2Op.TxOpc&&chaoshi_dengdai_lpuart2!=0)
	{
		
	}
	FL_DelayMs(1);
	  memcpy(send_data_zancun_lpuart2,zufuchua,lenth);
    //中断发送数组
    LPUART2Op.TxBuf = send_data_zancun_lpuart2;
    LPUART2Op.TxLen = lenth;
    LPUART2Op.TxOpc = 0 + 1;
	  FL_LPUART_ClearFlag_TXShiftBuffEmpty(LPUART2);
	  FL_LPUART_EnableIT_TXShiftBuffEmpty(LPUART2);
    FL_LPUART_WriteTXBuff(LPUART2, LPUART2Op.TxBuf[0]);
    
}
void LPUART2_Rx_rec()
{
	if(lpuart2_Rec_shuju_flag==1&&lpuart2_Rec_shuju_time_count==0)
	{
		LED_FLAG_Run();
		lpuart2_Rec_shuju_flag = 0;
		//对返回数据进行解析
		memcpy(lpuart2_Uart0_Tx_SendData,lpuart2_Rec_shuju_neirong,lpuart2_Rec_shuju_neirong_count);
		LPUART2_Tx_Send(lpuart2_Uart0_Tx_SendData,lpuart2_Rec_shuju_neirong_count);
		lpuart2_Rec_shuju_neirong_count = 0;
	}
}


/*************************(C) COPYRIGHT Fudan Microelectronics **** END OF FILE*************************/
