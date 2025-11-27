#include "main.h"
#include "uart0.h"
#include "uart3.h"
#include "time.h"
#include "LED_CTRL.h"
#include "jiliang_xieyi_Ctrl.h"

/*uart3在主控板中是与计量板进行通讯*/


uint8_t uart3_Rec_shuju_flag = 0;
uint8_t uart3_Rec_shuju_neirong[200];
uint16_t uart3_Rec_shuju_neirong_count = 0;
uint8_t uart3_Uart0_Tx_SendData[200];
uint8_t send_data_zancun[200];
//这一步需要注册到time时钟里
uint16_t chaoshi_dengdai_3 = 100;
uint16_t uart3_Rec_shuju_time_count = 0;



struct UARTOpStruct UART3Op;

void UART3_IRQHandler(void)
{
    uint32_t UART3RXBuffFullIT = 0;
    uint32_t UART3RXBuffFullFlag = 0;
    uint32_t UART3TXBuffFullIT = 0;
    uint32_t UART3TXBuffFullFlag = 0;

    UART3RXBuffFullIT = FL_UART_IsEnabledIT_RXBuffFull(UART3);
    UART3RXBuffFullFlag = FL_UART_IsActiveFlag_RXBuffFull(UART3);
    UART3TXBuffFullIT = FL_UART_IsEnabledIT_TXShiftBuffEmpty(UART3);
    UART3TXBuffFullFlag = FL_UART_IsActiveFlag_TXShiftBuffEmpty(UART3);

    //接收中断处理
    if((UART3RXBuffFullIT == 0x01UL) && (UART3RXBuffFullFlag == 0x01UL))
    {
        //中断接收
        uart3_Rec_shuju_neirong[uart3_Rec_shuju_neirong_count] = FL_UART_ReadRXBuff(UART3);     //接收中断标志可通过读取rxreg寄存器清除
			  uart3_Rec_shuju_neirong_count++;
			  if(uart3_Rec_shuju_neirong_count>=200)
			  {
				 uart3_Rec_shuju_neirong_count = 0;
			  }
			  uart3_Rec_shuju_flag = 1;
			  uart3_Rec_shuju_time_count = 100;   //判断串口空闲状态最长时间100ms
    }

    //发送中断处理
    if((UART3TXBuffFullIT == 0x01UL) && (UART3TXBuffFullFlag == 0x01UL))
    {
        //发送中断标志可通过写txreg寄存器清除或txif写1清除
        //发送指定长度的数据
        if(UART3Op.TxOpc < UART3Op.TxLen)
        {
            FL_UART_WriteTXBuff(UART3, UART3Op.TxBuf[UART3Op.TxOpc]); //发送一个数据
            UART3Op.TxOpc++;
        }

        FL_UART_ClearFlag_TXShiftBuffEmpty(UART3);  //清除发送中断标志

        if(UART3Op.TxOpc == UART3Op.TxLen)
        {
            FL_UART_DisableIT_TXShiftBuffEmpty(UART3);
        }
    }
}
void Uart3_Tx_Send_init()
{
	UART3Op.TxLen =1;
	UART3Op.TxOpc = 1;
}

void Uart3_Tx_Send(uint8_t zufuchua[],uint16_t lenth)
{
	if(lenth==0)
	{
		return;
	}
	chaoshi_dengdai_3 = 100;
	while(UART3Op.TxLen != UART3Op.TxOpc&&chaoshi_dengdai_3!=0) 
	{
		
	}
	FL_DelayMs(1);
	  memcpy(send_data_zancun,zufuchua,lenth);
    //中断发送数组
    UART3Op.TxBuf = send_data_zancun;
    UART3Op.TxLen = lenth;
    UART3Op.TxOpc = 0 + 1;
	  FL_UART_ClearFlag_TXShiftBuffEmpty(UART3);
	  FL_UART_EnableIT_TXShiftBuffEmpty(UART3);
    FL_UART_WriteTXBuff(UART3, UART3Op.TxBuf[0]);
    
}
void Uart3_Rx_rec()
{
	if(uart3_Rec_shuju_flag==1&&uart3_Rec_shuju_time_count==0)
	{
		LED_FLAG_Run();
		uart3_Rec_shuju_flag = 0;
		//对返回数据进行解析
		memcpy(uart3_Uart0_Tx_SendData,uart3_Rec_shuju_neirong,uart3_Rec_shuju_neirong_count);
		PC_Chuankou_tongxin_Debug_send(uart3_Uart0_Tx_SendData,uart3_Rec_shuju_neirong_count);
		JILIAN_TONGXIN_xieyijiexi(uart3_Uart0_Tx_SendData,uart3_Rec_shuju_neirong_count);
		//Uart3_Tx_Send(uart3_Uart0_Tx_SendData,uart3_Rec_shuju_neirong_count);
		uart3_Rec_shuju_neirong_count = 0;
	}
}

void MF_UART3_Init(void)
{
    FL_GPIO_InitTypeDef    GPIO_InitStruct;

    FL_UART_InitTypeDef    UART3_InitStruct;

    GPIO_InitStruct.pin           = FL_GPIO_PIN_0;
    GPIO_InitStruct.mode          = FL_GPIO_MODE_DIGITAL;
    GPIO_InitStruct.outputType    = FL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.pull          = FL_ENABLE;
    GPIO_InitStruct.remapPin      = FL_ENABLE;
    GPIO_InitStruct.analogSwitch  = FL_DISABLE;
    (void)FL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    GPIO_InitStruct.pin           = FL_GPIO_PIN_1;
    GPIO_InitStruct.mode          = FL_GPIO_MODE_DIGITAL;
    GPIO_InitStruct.outputType    = FL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.pull          = FL_DISABLE;
    GPIO_InitStruct.remapPin      = FL_ENABLE;
    GPIO_InitStruct.analogSwitch  = FL_DISABLE;
    (void)FL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    UART3_InitStruct.clockSrc          = FL_CMU_UART0_CLK_SOURCE_APBCLK;
    UART3_InitStruct.baudRate          = 9600;
    UART3_InitStruct.transferDirection = FL_UART_DIRECTION_TX_RX;
    UART3_InitStruct.dataWidth         = FL_UART_DATA_WIDTH_8B;
    UART3_InitStruct.stopBits          = FL_UART_STOP_BIT_WIDTH_1B;
    UART3_InitStruct.parity            = FL_UART_PARITY_NONE;

    (void)FL_UART_Init(UART3, &UART3_InitStruct);
}

void MF_UART3_Interrupt_Init(void)
{
    FL_UART_ClearFlag_RXBuffFull(UART3);
    FL_UART_EnableIT_RXBuffFull(UART3);

    FL_UART_ClearFlag_TXShiftBuffEmpty(UART3);
    FL_UART_EnableIT_TXShiftBuffEmpty(UART3);
}
void UART3_MF_NVIC_Init(void)
{
    FL_NVIC_ConfigTypeDef    InterruptConfigStruct;

    InterruptConfigStruct.preemptPriority = 0x02;
    FL_NVIC_Init(&InterruptConfigStruct, UART3_IRQn);
}


void UART3_MF_Config_Init(void)
{
    /* Initial UART0 */
    MF_UART3_Init();
    MF_UART3_Interrupt_Init();

    /* Initial NVIC */
    UART3_MF_NVIC_Init();
	  Uart3_Tx_Send_init();
}

