#ifndef DRV_UART_H
#define	DRV_UART_H



typedef enum BAUD_E
{
    Baud1200 = 0,
    Baud2400,
    Baud4800,
    Baud9600,
    Baud19200,
    Baud38400,
    Baud115200,
}BAUD_EU;

typedef enum S_CMDDATA_E
{
    FALSE_CMD,	  	//错误帧
    ERR_LEN,            //长度错误
    ERR_CS,             //校验错误
    ERR_ENDCODE,        //结束符错误
    F_WX_PTR,           //威星协议标志
    F_AT_PTR,           //AT命令协议标志
    F_WAITSENDDATA,	//等待发送的标志
            test_finish
}S_CMDDATA_EU;
extern S_CMDDATA_EU S_CMDDATA;

typedef enum T_PRTL_E
{
    TYPE_AT_PRTL,
    TYPE_WX68_PRTLS
}T_PRTL_EU;
extern T_PRTL_EU T_PRTL;

/*********************************   串口结构体********************************/
#define RXBUFF_SIZE  512
typedef struct PROTOCOL_ST
{
    unsigned char   RxDataBuff[RXBUFF_SIZE];
    unsigned short  p_RxDataWrite;
    unsigned short  p_RxDataRead;
    unsigned char f_PrtlReceiving;
    unsigned char f_RecvCmdHead;
    unsigned short p_RecvCmdData;
    unsigned char RecvCmdCheckSum;
    unsigned char RecvCmdBuf[RXBUFF_SIZE];
    unsigned short RecvCmdLen;
    unsigned short Recv_OverTime;
}PROTOCOL_S;
extern PROTOCOL_S MeterBuf, UpBuf, DebugUartBuf, IrdaBuf;

void Init_DebugUsart(void);
void Init_UpUsart(void);
void Init_MeterUsart(void);
void Init_IrdaUsart(void);

void OM_Open_Usart(USARTNUM_EU usart_num);
void OM_Close_Usart(USARTNUM_EU usart_num);

unsigned char OM_SendUsartData(USARTNUM_EU usart_num, unsigned char *tx_databuf,unsigned short tx_datalen);

extern void OM_MeterUsart_RecvData_OverTimeTask(void);
extern void OM_UPUsart_RecvData_OverTimeTask(void);
extern void OM_DebugUsart_RecvData_OverTimeTask(void);
extern void OM_IrdaUsart_RecvData_OverTimeTask(void);
extern void OM_MeterUart_interrupt_receive (unsigned char rxd);
extern void OM_UPUart_interrupt_receive (unsigned char rxd);
extern void OM_DebugUart_interrupt_receive (unsigned char rxd);
extern void OM_IrdaUart_interrupt_receive (unsigned char rxd);

void DebugUartDataDealProcess(void);
unsigned short Get_WX68_Frame(PROTOCOL_S* RecvBuf);
#endif
