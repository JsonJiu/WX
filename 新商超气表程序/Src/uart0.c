#include "main.h"
#include "uart0.h"
#include "uart3.h"
#include "led.h"
#include "time.h"
#include "PC.h"
#define lenth_Receive_Send_MAX 200

/*uart0在主控板中是与PC机进行通讯*/


uint8_t uart0_Rec_shuju_flag = 0;
uint8_t uart0_Rec_shuju_neirong[lenth_Receive_Send_MAX];
uint16_t uart0_Rec_shuju_neirong_count = 0;
uint8_t uart0_Uart0_Tx_SendData[lenth_Receive_Send_MAX];
uint8_t send_data_zancun_0[lenth_Receive_Send_MAX];
uint8_t send_over_flag0 = 1;
//这一步需要注册到time时钟里
uint16_t chaoshi_dengdai_0 = 0;
uint16_t uart0_Rec_shuju_time_count = 0;

void UART_TX_state_change(uint8_t send_state)
{
	FL_GPIO_InitTypeDef    GPIO_InitStruct;
	//空闲态
	if(send_state == 0)
	{
		GPIO_InitStruct.mode          = FL_GPIO_MODE_INPUT;
	}
	else
	{
		GPIO_InitStruct.mode          = FL_GPIO_MODE_DIGITAL;
	}
	GPIO_InitStruct.pin           = FL_GPIO_PIN_14;
  GPIO_InitStruct.outputType    = FL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.pull          = FL_ENABLE;
  GPIO_InitStruct.remapPin      = FL_DISABLE;
  GPIO_InitStruct.analogSwitch  = FL_DISABLE;
  (void)FL_GPIO_Init(GPIOA, &GPIO_InitStruct);
}

struct UARTOpStruct UART0Op;

void UART0_IRQHandler(void)
{
    uint32_t UART0RXBuffFullIT = 0;
    uint32_t UART0RXBuffFullFlag = 0;
    uint32_t UART0TXBuffFullIT = 0;
    uint32_t UART0TXBuffFullFlag = 0;

    UART0RXBuffFullIT = FL_UART_IsEnabledIT_RXBuffFull(UART0);
    UART0RXBuffFullFlag = FL_UART_IsActiveFlag_RXBuffFull(UART0);
    UART0TXBuffFullIT = FL_UART_IsEnabledIT_TXShiftBuffEmpty(UART0);
    UART0TXBuffFullFlag = FL_UART_IsActiveFlag_TXShiftBuffEmpty(UART0);

    //接收中断处理
    if((UART0RXBuffFullIT == 0x01UL) && (UART0RXBuffFullFlag == 0x01UL))
    {
        //中断接收
        uart0_Rec_shuju_neirong[uart0_Rec_shuju_neirong_count] = FL_UART_ReadRXBuff(UART0);     //接收中断标志可通过读取rxreg寄存器清除
			  uart0_Rec_shuju_neirong_count++;
			  if(uart0_Rec_shuju_neirong_count>=lenth_Receive_Send_MAX)
			  {
				 uart0_Rec_shuju_neirong_count = 0;
			  }
			  uart0_Rec_shuju_flag = 1;
			  uart0_Rec_shuju_time_count = 200;   //判断串口空闲状态最长时间100ms
    }

    //发送中断处理
    if((UART0TXBuffFullIT == 0x01UL) && (UART0TXBuffFullFlag == 0x01UL))
    {
			  if(UART0Op.TxOpc == UART0Op.TxLen)
        {
            FL_UART_DisableIT_TXShiftBuffEmpty(UART0);
					  send_over_flag0 = 1;
        }
        //发送中断标志可通过写txreg寄存器清除或txif写1清除
        //发送指定长度的数据
        if(UART0Op.TxOpc < UART0Op.TxLen)
        {
            FL_UART_WriteTXBuff(UART0, UART0Op.TxBuf[UART0Op.TxOpc]); //发送一个数据
            UART0Op.TxOpc++;
        }

        FL_UART_ClearFlag_TXShiftBuffEmpty(UART0);  //清除发送中断标志

        
    }
}
void Uart0_Tx_Send_init()
{
	UART0Op.TxLen =1;
	UART0Op.TxOpc = 1;
}

void Uart0_Tx_Send(uint8_t zufuchua[],uint16_t lenth)
{
	if(lenth==0)
	{
		return;
	}
	
	chaoshi_dengdai_0 = 100;
	while(UART0Op.TxLen != UART0Op.TxOpc&&chaoshi_dengdai_0!=0)
	{
		
	}
	FL_DelayMs(1);
	UART_TX_state_change(1);
	  memcpy(send_data_zancun_0,zufuchua,lenth);
    //中断发送数组
    UART0Op.TxBuf = send_data_zancun_0;
    UART0Op.TxLen = lenth;
    UART0Op.TxOpc = 0 + 1;
	  FL_UART_ClearFlag_TXShiftBuffEmpty(UART0);
	  FL_UART_EnableIT_TXShiftBuffEmpty(UART0);
    FL_UART_WriteTXBuff(UART0, UART0Op.TxBuf[0]);
}


void MF_UART0_Init(void)
{
    FL_GPIO_InitTypeDef    GPIO_InitStruct;

    FL_UART_InitTypeDef    UART0_InitStruct;

    GPIO_InitStruct.pin           = FL_GPIO_PIN_13;
    GPIO_InitStruct.mode          = FL_GPIO_MODE_DIGITAL;
    GPIO_InitStruct.outputType    = FL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.pull          = FL_ENABLE;
    GPIO_InitStruct.remapPin      = FL_DISABLE;
    GPIO_InitStruct.analogSwitch  = FL_DISABLE;
    (void)FL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.pin           = FL_GPIO_PIN_14;
    GPIO_InitStruct.mode          = FL_GPIO_MODE_DIGITAL;
    GPIO_InitStruct.outputType    = FL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.pull          = FL_DISABLE;
    GPIO_InitStruct.remapPin      = FL_DISABLE;
    GPIO_InitStruct.analogSwitch  = FL_DISABLE;
    (void)FL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    UART0_InitStruct.clockSrc          = FL_CMU_UART0_CLK_SOURCE_APBCLK;
    UART0_InitStruct.baudRate          = 9600;
    UART0_InitStruct.transferDirection = FL_UART_DIRECTION_TX_RX;
    UART0_InitStruct.dataWidth         = FL_UART_DATA_WIDTH_8B;
    UART0_InitStruct.stopBits          = FL_UART_STOP_BIT_WIDTH_1B;
    UART0_InitStruct.parity            = FL_UART_PARITY_NONE;

    (void)FL_UART_Init(UART0, &UART0_InitStruct);
}

void MF_UART0_Interrupt_Init(void)
{
    FL_UART_ClearFlag_RXBuffFull(UART0);
    FL_UART_EnableIT_RXBuffFull(UART0);

    FL_UART_ClearFlag_TXShiftBuffEmpty(UART0);
    FL_UART_EnableIT_TXShiftBuffEmpty(UART0);
}
void UART0_MF_NVIC_Init(void)
{
    FL_NVIC_ConfigTypeDef    InterruptConfigStruct;

    InterruptConfigStruct.preemptPriority = 0x02;
    FL_NVIC_Init(&InterruptConfigStruct, UART0_IRQn);
}


void UART0_MF_Config_Init(void)
{
    /* Initial UART0 */
    MF_UART0_Init();
    MF_UART0_Interrupt_Init();

    /* Initial NVIC */
    UART0_MF_NVIC_Init();
	  Uart0_Tx_Send_init();
}
void Uart0_Rx_rec(void)
{
	if(send_over_flag0 == 1)
	{
		send_over_flag0 = 0;
		FL_DelayMs(5);
	  UART_TX_state_change(0);
	}
	if(uart0_Rec_shuju_flag==1&&uart0_Rec_shuju_time_count==0)
	{
		LED_Sign_Open();
		uart0_Rec_shuju_flag = 0;
		//对返回数据进行解析
		memcpy(uart0_Uart0_Tx_SendData,uart0_Rec_shuju_neirong,uart0_Rec_shuju_neirong_count);
		PC_Agreement_Analysis(uart0_Uart0_Tx_SendData,uart0_Rec_shuju_neirong_count);
		uart0_Rec_shuju_neirong_count = 0;
	}
}




