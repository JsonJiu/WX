#include "main.h"
#include "uart1.h"
#include "stdarg.h"
#include "uart5.h"
#include "time.h"
#include "LED_CTRL.h"
#include "PC_xieyi_Ctrl.h"
#define lenth_Receive_Send_MAX 200
uint8_t send_over_flag = 1;

uint8_t uart1_Rec_shuju_flag = 0;
uint8_t uart1_Rec_shuju_neirong[lenth_Receive_Send_MAX];
uint16_t uart1_Rec_shuju_neirong_count = 0;
uint8_t uart1_Uart0_Tx_SendData[lenth_Receive_Send_MAX];
uint8_t send_data_zancun_1[lenth_Receive_Send_MAX];
//这一步需要注册到time时钟里
uint16_t uart1_Rec_shuju_time_count = 0;

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
	GPIO_InitStruct.pin           = FL_GPIO_PIN_3;
  GPIO_InitStruct.outputType    = FL_GPIO_OUTPUT_PUSHPULL;
  GPIO_InitStruct.pull          = FL_ENABLE;
  GPIO_InitStruct.remapPin      = FL_DISABLE;
  GPIO_InitStruct.analogSwitch  = FL_DISABLE;
  (void)FL_GPIO_Init(GPIOC, &GPIO_InitStruct);
}



struct UARTOpStruct UART1Op;

void UART1_IRQHandler(void)
{
    uint32_t UART1RXBuffFullIT = 0;
    uint32_t UART1RXBuffFullFlag = 0;
    uint32_t UART1TXBuffFullIT = 0;
    uint32_t UART1TXBuffFullFlag = 0;

    UART1RXBuffFullIT = FL_UART_IsEnabledIT_RXBuffFull(UART1);
    UART1RXBuffFullFlag = FL_UART_IsActiveFlag_RXBuffFull(UART1);
    UART1TXBuffFullIT = FL_UART_IsEnabledIT_TXShiftBuffEmpty(UART1);
    UART1TXBuffFullFlag = FL_UART_IsActiveFlag_TXShiftBuffEmpty(UART1);

    //接收中断处理
    if((UART1RXBuffFullIT == 0x01UL) && (UART1RXBuffFullFlag == 0x01UL))
    {
        //中断接收
        uart1_Rec_shuju_neirong[uart1_Rec_shuju_neirong_count] = FL_UART_ReadRXBuff(UART1);     //接收中断标志可通过读取rxreg寄存器清除
			  uart1_Rec_shuju_neirong_count++;
			  if(uart1_Rec_shuju_neirong_count>=lenth_Receive_Send_MAX)
			  {
				 uart1_Rec_shuju_neirong_count = 0;
			  }
			  uart1_Rec_shuju_flag = 1;
			  uart1_Rec_shuju_time_count = 100;
    }

    //发送中断处理
    if((UART1TXBuffFullIT == 0x01UL) && (UART1TXBuffFullFlag == 0x01UL))
    {
        //发送中断标志可通过写txreg寄存器清除或txif写1清除
        //发送指定长度的数据
        if(UART1Op.TxOpc < UART1Op.TxLen)
        {
            FL_UART_WriteTXBuff(UART1, UART1Op.TxBuf[UART1Op.TxOpc]); //发送一个数据
            UART1Op.TxOpc++;
        }

        FL_UART_ClearFlag_TXShiftBuffEmpty(UART1);  //清除发送中断标志
				

        if(UART1Op.TxOpc == UART1Op.TxLen)
        {
            FL_UART_DisableIT_TXShiftBuffEmpty(UART1);
					  send_over_flag = 1;
        }
    }
}
void Uart1_Tx_Send_init()
{
	UART1Op.TxLen =1;
	UART1Op.TxOpc = 1;
	UART_TX_state_change(0);
}

void Uart1_Tx_Send(uint8_t zufuchua[],uint16_t lenth)
{
	if(lenth==0)
	{
		return;
	}
	chaoshi_dengdai = 100;
	//多包发送时，会等待上一包发送完再发送下一包
	while(UART1Op.TxLen != UART1Op.TxOpc&&chaoshi_dengdai!=0)
	{
		
	}
	FL_DelayMs(5);
	send_over_flag = 0;
	  memcpy(send_data_zancun_1,zufuchua,lenth);
	  UART_TX_state_change(1);
    //中断发送数组
    UART1Op.TxBuf = send_data_zancun_1;
    UART1Op.TxLen = lenth;
    UART1Op.TxOpc = 0 + 1;
	  FL_UART_ClearFlag_TXShiftBuffEmpty(UART1);
	  FL_UART_EnableIT_TXShiftBuffEmpty(UART1);
    FL_UART_WriteTXBuff(UART1, UART1Op.TxBuf[0]);
}


void MF_UART1_Init(void)
{
    FL_GPIO_InitTypeDef    GPIO_InitStruct;

    FL_UART_InitTypeDef    UART1_InitStruct;

    GPIO_InitStruct.pin           = FL_GPIO_PIN_2;
    GPIO_InitStruct.mode          = FL_GPIO_MODE_DIGITAL;
    GPIO_InitStruct.outputType    = FL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.pull          = FL_DISABLE;
    GPIO_InitStruct.remapPin      = FL_DISABLE;
    GPIO_InitStruct.analogSwitch  = FL_DISABLE;
    (void)FL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    GPIO_InitStruct.pin           = FL_GPIO_PIN_3;
    GPIO_InitStruct.mode          = FL_GPIO_MODE_DIGITAL;
    GPIO_InitStruct.outputType    = FL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.pull          = FL_DISABLE;
    GPIO_InitStruct.remapPin      = FL_DISABLE;
    GPIO_InitStruct.analogSwitch  = FL_DISABLE;
    (void)FL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    UART1_InitStruct.clockSrc          = FL_CMU_UART1_CLK_SOURCE_APBCLK;
    UART1_InitStruct.baudRate          = 9600;
    UART1_InitStruct.transferDirection = FL_UART_DIRECTION_TX_RX;
    UART1_InitStruct.dataWidth         = FL_UART_DATA_WIDTH_8B;
    UART1_InitStruct.stopBits          = FL_UART_STOP_BIT_WIDTH_1B;
    UART1_InitStruct.parity            = FL_UART_PARITY_NONE;

    (void)FL_UART_Init(UART1, &UART1_InitStruct);
}

void MF_UART1_Interrupt_Init(void)
{
    FL_UART_ClearFlag_RXBuffFull(UART1);
    FL_UART_EnableIT_RXBuffFull(UART1);

    FL_UART_ClearFlag_TXShiftBuffEmpty(UART1);
    FL_UART_EnableIT_TXShiftBuffEmpty(UART1);
}
void UART1_MF_NVIC_Init(void)
{
    FL_NVIC_ConfigTypeDef    InterruptConfigStruct;

    InterruptConfigStruct.preemptPriority = 0x00;
    FL_NVIC_Init(&InterruptConfigStruct, UART1_IRQn);
}


void UART1_MF_Config_Init(void)
{
    /* Initial UART0 */
    MF_UART1_Init();
    MF_UART1_Interrupt_Init();

    /* Initial NVIC */
    UART1_MF_NVIC_Init();
	  Uart1_Tx_Send_init();
}
void Uart1_Rx_rec()
{
	if(send_over_flag == 1)
	{
		send_over_flag = 0;
		FL_DelayMs(5);
	  UART_TX_state_change(0);
	}
	if(uart1_Rec_shuju_flag==1&&uart1_Rec_shuju_time_count==0)
	{
		LED_FLAG_Run();
		uart1_Rec_shuju_flag = 0;
		//对返回数据进行解析
		memcpy(uart1_Uart0_Tx_SendData,uart1_Rec_shuju_neirong,uart1_Rec_shuju_neirong_count);
		PC_xieyijiexi(uart1_Uart0_Tx_SendData,uart1_Rec_shuju_neirong_count);
		uart1_Rec_shuju_neirong_count = 0;
	}
}
void DeBug_print(const char fmt[], ...)
{
    unsigned char DebugBuf[lenth_Receive_Send_MAX];  // 串口输出是阻塞式的、不会导致多个print函数同时运行，所以这里不会显著增大占空间的使用
    va_list args;
    int res;
		if(Debug_Mode==0)
		return;
    va_start(args, fmt);
    res = vsnprintf((char *)DebugBuf, sizeof(DebugBuf), fmt, args);
    va_end(args);
    // vsnprintf()会自动在格式化之后的字符串的末尾增加'\0'，但res不包含这个字符，所以DebugBuf[N]最多能放N-1个字符，多余的字符将被截断
    // res >= sizeof(DebugBuf)的情况下、会有一部分输出信息丢失，以后可以考虑在这里给出buffer不足的提示
    if(res > 0)
    {
        Uart1_Tx_Send(DebugBuf,res);
    }
}
void PC_Chuankou_tongxin_Debug_send(uint8_t zufuchua[],uint16_t lenth)
{
	if(Debug_Mode==0)
		return;
	Uart1_Tx_Send(zufuchua,lenth);
}
void PC_Chuankou_tongxin_send(uint8_t zufuchua[],uint16_t lenth)
{
	Uart1_Tx_Send(zufuchua,lenth);
}
