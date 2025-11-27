#include "main.h"
#include "uart1.h"
#include "uart3.h"
#include "uart0.h"
#include "stdarg.h"
#include "time.h"
#include "LED_CTRL.h"
#include "PC_xieyi_Ctrl.h"
#include "tongxin_xieyi_Ctrl.h"
#include "Test_List.h"
#define lenth_Receive_Send_MAX 200

/*uart1在主控板中是红外通讯*/

uint8_t send_over_flag1 = 1;

uint8_t uart1_Rec_shuju_flag = 0;
uint8_t uart1_Rec_shuju_neirong[lenth_Receive_Send_MAX];
uint16_t uart1_Rec_shuju_neirong_count = 0;
uint8_t uart1_Uart0_Tx_SendData[lenth_Receive_Send_MAX];
uint8_t send_data_zancun_1[lenth_Receive_Send_MAX];
//这一步需要注册到time时钟里
uint16_t chaoshi_dengdai_1 = 0;
uint16_t uart1_Rec_shuju_time_count = 0;

uint8_t qiandao[] = {0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,
										 0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,
	                   0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,
										 0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,
	                   0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,0x55,
};
uint8_t qiandao_jiaozhun[] = {0xFE,0xFE,0xFE,0xFE,0xFE,0xFE,0xFE,0xFE,0xFE,0xFE};
//这个带前导
uint8_t qiandao_biaoshi = 1;


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
			  uart1_Rec_shuju_time_count = 100;   //判断串口空闲状态最长时间100ms
    }

    //发送中断处理
    if((UART1TXBuffFullIT == 0x01UL) && (UART1TXBuffFullFlag == 0x01UL))
    {
			  //这里发送完成之后再进进入，可以解决最后一个字节发不完的问题
				if(UART1Op.TxOpc == UART1Op.TxLen)
        {
            FL_UART_DisableIT_TXShiftBuffEmpty(UART1);
					  send_over_flag1 = 1;
        }
        //发送中断标志可通过写txreg寄存器清除或txif写1清除
        //发送指定长度的数据
        if(UART1Op.TxOpc < UART1Op.TxLen)
        {
            FL_UART_WriteTXBuff(UART1, UART1Op.TxBuf[UART1Op.TxOpc]); //发送一个数据
            UART1Op.TxOpc++;
        }

        FL_UART_ClearFlag_TXShiftBuffEmpty(UART1);  //清除发送中断标志
    }
}
void Uart1_Tx_Send_init()
{
	UART1Op.TxLen =1;
	UART1Op.TxOpc = 1;
}
void Uart1_Tx_Send(uint8_t zufuchua[],uint16_t lenth)
{
	if(lenth==0)
	{
		return;
	}
	chaoshi_dengdai_1 = 1000;
	//多包发送时，会等待上一包发送完再发送下一包
	while(UART1Op.TxLen != UART1Op.TxOpc&&chaoshi_dengdai_1!=0)
	{
		
	}
	  memcpy(send_data_zancun_1,zufuchua,lenth);
    //中断发送数组
    UART1Op.TxBuf = send_data_zancun_1;
    UART1Op.TxLen = lenth;
    UART1Op.TxOpc = 0 + 1;
	  FL_UART_ClearFlag_TXShiftBuffEmpty(UART1);
	  FL_UART_EnableIT_TXShiftBuffEmpty(UART1);
    FL_UART_WriteTXBuff(UART1, UART1Op.TxBuf[0]);
}

void Uart1_Tx_Send_ok(uint8_t zufuchua[],uint16_t lenth)
{
	uint16_t diandao_xunhuan = 0; 
	if(qiandao_biaoshi ==1)
	{
		for(diandao_xunhuan=0;diandao_xunhuan<60;diandao_xunhuan++)
		{
			//这里会有长时间阻塞，仍个看门狗
			//FL_IWDT_ReloadCounter(IWDT);
		  Uart1_Tx_Send(qiandao,40);
		}
	}
	Uart1_Tx_Send(qiandao_jiaozhun,10);
	PC_Chuankou_tongxin_Debug_send(zufuchua,lenth);
	Uart1_Tx_Send(zufuchua,lenth);
}


void MF_UART1_Init(void)
{
    FL_GPIO_InitTypeDef    GPIO_InitStruct;

    FL_UART_InitTypeDef    UART1_InitStruct;

    GPIO_InitStruct.pin           = FL_GPIO_PIN_13;
    GPIO_InitStruct.mode          = FL_GPIO_MODE_DIGITAL;
    GPIO_InitStruct.outputType    = FL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.pull          = FL_DISABLE;
    GPIO_InitStruct.remapPin      = FL_DISABLE;
    GPIO_InitStruct.analogSwitch  = FL_DISABLE;
    (void)FL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    GPIO_InitStruct.pin           = FL_GPIO_PIN_14;
    GPIO_InitStruct.mode          = FL_GPIO_MODE_DIGITAL;
    GPIO_InitStruct.outputType    = FL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.pull          = FL_DISABLE;
    GPIO_InitStruct.remapPin      = FL_DISABLE;
    GPIO_InitStruct.analogSwitch  = FL_DISABLE;
    (void)FL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    UART1_InitStruct.clockSrc          = FL_CMU_UART0_CLK_SOURCE_APBCLK;
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

    InterruptConfigStruct.preemptPriority = 0x01;
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
	if(uart1_Rec_shuju_flag==1&&uart1_Rec_shuju_time_count==0)
	{
		LED_FLAG_Run();
		uart1_Rec_shuju_flag = 0;
		//对返回数据进行解析
		memcpy(uart1_Uart0_Tx_SendData,uart1_Rec_shuju_neirong,uart1_Rec_shuju_neirong_count);
		//PC_Chuankou_tongxin_Debug_send(uart1_Uart0_Tx_SendData,uart1_Rec_shuju_neirong_count);
		TONGXIN_xieyijiexi(uart1_Uart0_Tx_SendData,uart1_Rec_shuju_neirong_count);
		uart1_Rec_shuju_neirong_count = 0;
	}
}


