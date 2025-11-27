#include "main.h"
#include "uart3.h"
#include "uart1.h"
#include "uart0.h"
#include "uart4.h"
#include "time.h"
#include "LED_CTRL.h"
#include "Test_List.h"
#include "tongxin_xieyi_irda_Ctrl.h"

/*uart4在主控板中是与表具进行通讯*/

uint8_t send_over_flag4 = 1;

uint8_t uart4_Rec_shuju_flag = 0;
uint8_t uart4_Rec_shuju_neirong[200];
uint16_t uart4_Rec_shuju_neirong_count = 0;
uint8_t uart4_Uart0_Tx_SendData[200];
uint8_t send_data_zancun_4[200];
//这一步需要注册到time时钟里
uint16_t chaoshi_dengdai_4 = 100;
uint16_t uart4_Rec_shuju_time_count = 0;

extern uint8_t qiandao[];
extern uint8_t qiandao_jiaozhun[];
//这个带前导
uint8_t qiandao_biaoshi_4 = 1;

struct UARTOpStruct UART4Op;

void UART4_IRQHandler(void)
{
    uint32_t UART4RXBuffFullIT = 0;
    uint32_t UART4RXBuffFullFlag = 0;
    uint32_t UART4TXBuffFullIT = 0;
    uint32_t UART4TXBuffFullFlag = 0;

    UART4RXBuffFullIT = FL_UART_IsEnabledIT_RXBuffFull(UART4);
    UART4RXBuffFullFlag = FL_UART_IsActiveFlag_RXBuffFull(UART4);
    UART4TXBuffFullIT = FL_UART_IsEnabledIT_TXShiftBuffEmpty(UART4);
    UART4TXBuffFullFlag = FL_UART_IsActiveFlag_TXShiftBuffEmpty(UART4);

    //接收中断处理
    if((UART4RXBuffFullIT == 0x01UL) && (UART4RXBuffFullFlag == 0x01UL))
    {
        //中断接收
        uart4_Rec_shuju_neirong[uart4_Rec_shuju_neirong_count] = FL_UART_ReadRXBuff(UART4);     //接收中断标志可通过读取rxreg寄存器清除
			  uart4_Rec_shuju_neirong_count++;
			  if(uart4_Rec_shuju_neirong_count>=200)
			  {
				 uart4_Rec_shuju_neirong_count = 0;
			  }
			  uart4_Rec_shuju_flag = 1;
			  uart4_Rec_shuju_time_count = 100;   //判断串口空闲状态最长时间100ms
    }

    //发送中断处理
    if((UART4TXBuffFullIT == 0x01UL) && (UART4TXBuffFullFlag == 0x01UL))
    {
			  //这里发送完成之后再进进入，可以解决最后一个字节发不完的问题
			  if(UART4Op.TxOpc == UART4Op.TxLen)
        {
          FL_UART_DisableIT_TXShiftBuffEmpty(UART4);
					send_over_flag4 = 1;
        }
        //发送中断标志可通过写txreg寄存器清除或txif写1清除
        //发送指定长度的数据
        if(UART4Op.TxOpc < UART4Op.TxLen)
        {
            FL_UART_WriteTXBuff(UART5, UART4Op.TxBuf[UART4Op.TxOpc]); //发送一个数据
            UART4Op.TxOpc++;
        }

        FL_UART_ClearFlag_TXShiftBuffEmpty(UART4);  //清除发送中断标志

        
    }
}
void Uart4_Tx_Send_init()
{
	UART4Op.TxLen =1;
	UART4Op.TxOpc = 1;
}

void Uart4_Tx_Send(uint8_t zufuchua[],uint16_t lenth)
{
	if(lenth==0)
	{
		return;
	}
	chaoshi_dengdai_4 = 1000;
	send_over_flag4 = 0;
	while(send_over_flag4 == 1&&chaoshi_dengdai_4!=0)
	{
		
	}
	  memcpy(send_data_zancun_4,zufuchua,lenth);
    //中断发送数组
    UART4Op.TxBuf = send_data_zancun_4;
    UART4Op.TxLen = lenth;
    UART4Op.TxOpc = 0 + 1;
	  FL_UART_ClearFlag_TXShiftBuffEmpty(UART4);
	  FL_UART_EnableIT_TXShiftBuffEmpty(UART4);
    FL_UART_WriteTXBuff(UART4, UART4Op.TxBuf[0]);
    
}

void Uart4_Tx_Send_ok(uint8_t zufuchua[],uint16_t lenth)
{
	uint16_t diandao_xunhuan = 0; 
	if(qiandao_biaoshi_4 ==1)
	{
		for(diandao_xunhuan=0;diandao_xunhuan<11;diandao_xunhuan++)
		{
			//这里会有长时间阻塞，仍个看门狗
			FL_IWDT_ReloadCounter(IWDT);
		  Uart4_Tx_Send(qiandao,50);
		}
		Uart4_Tx_Send(qiandao_jiaozhun,5);
	}
	PC_Chuankou_tongxin_Debug_send(zufuchua,lenth);
	Uart4_Tx_Send(zufuchua,lenth);
}


void MF_UART4_Init(void)
{
    FL_GPIO_InitTypeDef    GPIO_InitStruct;

    FL_UART_InitTypeDef    UART4_InitStruct;

    GPIO_InitStruct.pin           = FL_GPIO_PIN_0;
    GPIO_InitStruct.mode          = FL_GPIO_MODE_DIGITAL;
    GPIO_InitStruct.outputType    = FL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.pull          = FL_ENABLE;
    GPIO_InitStruct.remapPin      = FL_DISABLE;
    GPIO_InitStruct.analogSwitch  = FL_DISABLE;
    (void)FL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.pin           = FL_GPIO_PIN_1;
    GPIO_InitStruct.mode          = FL_GPIO_MODE_DIGITAL;
    GPIO_InitStruct.outputType    = FL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.pull          = FL_DISABLE;
    GPIO_InitStruct.remapPin      = FL_DISABLE;
    GPIO_InitStruct.analogSwitch  = FL_DISABLE;
    (void)FL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    UART4_InitStruct.clockSrc          = FL_CMU_UART0_CLK_SOURCE_APBCLK;
    UART4_InitStruct.baudRate          = 115200;
    UART4_InitStruct.transferDirection = FL_UART_DIRECTION_TX_RX;
    UART4_InitStruct.dataWidth         = FL_UART_DATA_WIDTH_8B;
    UART4_InitStruct.stopBits          = FL_UART_STOP_BIT_WIDTH_1B;
    UART4_InitStruct.parity            = FL_UART_PARITY_NONE;

    (void)FL_UART_Init(UART4, &UART4_InitStruct);
}

void MF_UART4_Interrupt_Init(void)
{
    FL_UART_ClearFlag_RXBuffFull(UART4);
    FL_UART_EnableIT_RXBuffFull(UART4);

    FL_UART_ClearFlag_TXShiftBuffEmpty(UART4);
    FL_UART_EnableIT_TXShiftBuffEmpty(UART4);
}
void UART4_MF_NVIC_Init(void)
{
    FL_NVIC_ConfigTypeDef    InterruptConfigStruct;

    InterruptConfigStruct.preemptPriority = 0x02;
    FL_NVIC_Init(&InterruptConfigStruct, UART4_IRQn);
}


void UART4_MF_Config_Init(void)
{
    /* Initial UART0 */
    MF_UART4_Init();
    MF_UART4_Interrupt_Init();

    /* Initial NVIC */
    UART4_MF_NVIC_Init();
	  Uart4_Tx_Send_init();
}

void Uart4_Rx_rec()
{
	if(uart4_Rec_shuju_flag==1&&uart4_Rec_shuju_time_count==0)
	{
		LED_FLAG_Run();
		uart4_Rec_shuju_flag = 0;
		//对返回数据进行解析
		memcpy(uart4_Uart0_Tx_SendData,uart4_Rec_shuju_neirong,uart4_Rec_shuju_neirong_count);
		//由于串口仅用于检测是否正常这里的协议仅引用一条
		Test_jiejuo_jilu.hongwai_jiance = 1;
		//irad_TONGXIN_xieyijiexi(uart4_Uart0_Tx_SendData,uart4_Rec_shuju_neirong_count);
		//Uart4_Tx_Send(uart4_Uart0_Tx_SendData,uart4_Rec_shuju_neirong_count);
		uart4_Rec_shuju_neirong_count = 0;
	}
}
