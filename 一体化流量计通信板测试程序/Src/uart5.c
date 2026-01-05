#include "main.h"
#include "uart3.h"
#include "uart5.h"
#include "time.h"
#include "LED_CTRL.h"

uint8_t uart5_Rec_shuju_flag = 0;
uint8_t uart5_Rec_shuju_neirong[200];
uint16_t uart5_Rec_shuju_neirong_count = 0;
uint8_t uart5_Uart0_Tx_SendData[200];
uint8_t send_data_zancun_5[200];
//这一步需要注册到time时钟里
uint16_t chaoshi_dengdai_5 = 100;
uint16_t uart5_Rec_shuju_time_count = 0;



struct UARTOpStruct UART5Op;

void UART5_IRQHandler(void)
{
    uint32_t UART5RXBuffFullIT = 0;
    uint32_t UART5RXBuffFullFlag = 0;
    uint32_t UART5TXBuffFullIT = 0;
    uint32_t UART5TXBuffFullFlag = 0;

    UART5RXBuffFullIT = FL_UART_IsEnabledIT_RXBuffFull(UART5);
    UART5RXBuffFullFlag = FL_UART_IsActiveFlag_RXBuffFull(UART5);
    UART5TXBuffFullIT = FL_UART_IsEnabledIT_TXShiftBuffEmpty(UART5);
    UART5TXBuffFullFlag = FL_UART_IsActiveFlag_TXShiftBuffEmpty(UART5);

    //接收中断处理
    if((UART5RXBuffFullIT == 0x01UL) && (UART5RXBuffFullFlag == 0x01UL))
    {
        //中断接收
        uart5_Rec_shuju_neirong[uart5_Rec_shuju_neirong_count] = FL_UART_ReadRXBuff(UART5);     //接收中断标志可通过读取rxreg寄存器清除
			  uart5_Rec_shuju_neirong_count++;
			  if(uart5_Rec_shuju_neirong_count>=200)
			  {
				 uart5_Rec_shuju_neirong_count = 0;
			  }
			  uart5_Rec_shuju_flag = 1;
			  uart5_Rec_shuju_time_count = 100;
    }

    //发送中断处理
    if((UART5TXBuffFullIT == 0x01UL) && (UART5TXBuffFullFlag == 0x01UL))
    {
        //发送中断标志可通过写txreg寄存器清除或txif写1清除
        //发送指定长度的数据
        if(UART5Op.TxOpc < UART5Op.TxLen)
        {
            FL_UART_WriteTXBuff(UART5, UART5Op.TxBuf[UART5Op.TxOpc]); //发送一个数据
            UART5Op.TxOpc++;
        }

        FL_UART_ClearFlag_TXShiftBuffEmpty(UART5);  //清除发送中断标志

        if(UART5Op.TxOpc == UART5Op.TxLen)
        {
            FL_UART_DisableIT_TXShiftBuffEmpty(UART5);
        }
    }
}
void Uart5_Tx_Send_init()
{
	UART5Op.TxLen =1;
	UART5Op.TxOpc = 1;
}

void Uart5_Tx_Send(uint8_t zufuchua[],uint16_t lenth)
{
	if(lenth==0)
	{
		return;
	}
	chaoshi_dengdai_5 = 100;
	while(UART5Op.TxLen != UART5Op.TxOpc&&chaoshi_dengdai_5!=0)
	{
		
	}
	FL_DelayMs(1);
	  memcpy(send_data_zancun_5,zufuchua,lenth);
    //中断发送数组
    UART5Op.TxBuf = send_data_zancun_5;
    UART5Op.TxLen = lenth;
    UART5Op.TxOpc = 0 + 1;
	  FL_UART_ClearFlag_TXShiftBuffEmpty(UART5);
	  FL_UART_EnableIT_TXShiftBuffEmpty(UART5);
    FL_UART_WriteTXBuff(UART5, UART5Op.TxBuf[0]);
    
}
void Uart5_Rx_rec()
{
	if(uart5_Rec_shuju_flag==1&&uart5_Rec_shuju_time_count==0)
	{
		LED_FLAG_Run();
		uart5_Rec_shuju_flag = 0;
		//对返回数据进行解析
		memcpy(uart5_Uart0_Tx_SendData,uart5_Rec_shuju_neirong,uart5_Rec_shuju_neirong_count);
		Uart5_Tx_Send(uart5_Uart0_Tx_SendData,uart5_Rec_shuju_neirong_count);
		uart5_Rec_shuju_neirong_count = 0;
	}
}

void MF_UART5_Init(void)
{
    FL_GPIO_InitTypeDef    GPIO_InitStruct;

    FL_UART_InitTypeDef    UART5_InitStruct;

    GPIO_InitStruct.pin           = FL_GPIO_PIN_4;
    GPIO_InitStruct.mode          = FL_GPIO_MODE_DIGITAL;
    GPIO_InitStruct.outputType    = FL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.pull          = FL_ENABLE;
    GPIO_InitStruct.remapPin      = FL_DISABLE;
    GPIO_InitStruct.analogSwitch  = FL_DISABLE;
    (void)FL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    GPIO_InitStruct.pin           = FL_GPIO_PIN_5;
    GPIO_InitStruct.mode          = FL_GPIO_MODE_DIGITAL;
    GPIO_InitStruct.outputType    = FL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.pull          = FL_DISABLE;
    GPIO_InitStruct.remapPin      = FL_DISABLE;
    GPIO_InitStruct.analogSwitch  = FL_DISABLE;
    (void)FL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    UART5_InitStruct.clockSrc          = FL_CMU_UART0_CLK_SOURCE_APBCLK;
    UART5_InitStruct.baudRate          = 9600;
    UART5_InitStruct.transferDirection = FL_UART_DIRECTION_TX_RX;
    UART5_InitStruct.dataWidth         = FL_UART_DATA_WIDTH_8B;
    UART5_InitStruct.stopBits          = FL_UART_STOP_BIT_WIDTH_1B;
    UART5_InitStruct.parity            = FL_UART_PARITY_NONE;

    (void)FL_UART_Init(UART5, &UART5_InitStruct);
}

void MF_UART5_Interrupt_Init(void)
{
    FL_UART_ClearFlag_RXBuffFull(UART5);
    FL_UART_EnableIT_RXBuffFull(UART5);

    FL_UART_ClearFlag_TXShiftBuffEmpty(UART5);
    FL_UART_EnableIT_TXShiftBuffEmpty(UART5);
}
void UART5_MF_NVIC_Init(void)
{
    FL_NVIC_ConfigTypeDef    InterruptConfigStruct;

    InterruptConfigStruct.preemptPriority = 0x02;
    FL_NVIC_Init(&InterruptConfigStruct, UART5_IRQn);
}


void UART5_MF_Config_Init(void)
{
    /* Initial UART0 */
    MF_UART5_Init();
    MF_UART5_Interrupt_Init();

    /* Initial NVIC */
    UART5_MF_NVIC_Init();
	  Uart5_Tx_Send_init();
}

