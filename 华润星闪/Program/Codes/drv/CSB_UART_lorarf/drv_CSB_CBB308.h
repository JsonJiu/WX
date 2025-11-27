#ifndef _DRV_CSB_CBB308_H
#define _DRV_CSB_CBB308_H

#define CSB_Uart_MAXRXTIMER  20//50///////////

#define CSB_Irda_MAXRXTIMER  150//////////

#define CSB_OVERTIMER        120//360//120//6////////////////计量超时时间为6分钟

#define CSB_Uart_RX_Head     0x68
#define CSB_Irda_RX_Head     0x68////////////计量板帧头
#define CSB_Irda_RX_Head1    0xAB////////////计量板帧头


typedef struct
{
  unsigned char CSB_Uart_RX_BuffLen;
  unsigned char CSB_Uart_RX_Buff[50];
  unsigned char CSB_Irda_RX_BuffLen;
  unsigned char CSB_Irda_RX_Buff[128];
  unsigned char CSB_Irda_Start_Flag;
  unsigned char CSB_Irda_Stop_Flag;
  unsigned char CSB_Uart_Stop_Flag;
  unsigned char CSB_Measure_Status;
  unsigned char CSB_Work_Status;
  unsigned char CSB_FlowRate_Status;
  unsigned char CSB_Meter_Type;
  unsigned char CSB_Work_Direct;
  unsigned char CSB_Temp_Status;
  unsigned char CSB_Pre_Status;
  unsigned char CSB_Err;
  unsigned char CSB_LCD;
  signed int  CSB_Temp;
  unsigned int  CSB_Pre;
  unsigned long CSB_FlowRate;
  unsigned long CSB_Volume;
  unsigned long CSB_Uart_RX_Time;////////////////接收数据超时
  unsigned long CSB_Irda_RX_Time;////////////////光电接收数据超时
  unsigned long CSB_Timer_Time;/////////////计量异常超时判断
  unsigned long CSB_Display_Time;/////////////轮显计时
}CSB;

extern CSB Csb;


typedef struct
{
  unsigned char Uart_Rx_OverTime_NUM;///////计量数据接收数据NUM
  unsigned char Irda_Rx_OverTime_NUM;///////光电数据接收数据NUM
  unsigned char Irda_Scan_Time_NUM;///////光电数据接收数据NUM
  unsigned char Data_OverTime_NUM;//////////计量超时NUM
  unsigned char Display_ShiftTime_NUM;//////轮显NUM
  unsigned char Insp_Display_NUM;//////轮显NUM
}CSB_TIMER;

extern CSB_TIMER Csb_Timer;

extern void CSB_Uart_Receive_On();
extern void CSB_Uart_Receive_Off();
extern void CSB_Uart_Send(unsigned char *pbuf,unsigned char Len);
extern void CSB_Irda_Config(void(*fun)(unsigned char));
extern void CSB_Irda_Send(unsigned char *pbuf,unsigned char Len);
extern void CSB_Irda_Receive_On();
extern void CSB_Irda_Receive_Off();
extern void CSB_Uart_RX(unsigned char CSB_Uart_RX_Data);
extern void CSB_Irda_RX(unsigned char CSB_Irda_RX_Data);
void CSB_Check_Uart_Data(void);
void CSB_Check_Irda_Data2(void);
void CSB_Handle_Uart_Protocol();
extern void CSB_TASK_Init();
void Csb_Irda_Send(unsigned char *IRDABUFF,unsigned char IRDABUFFLEN);
void Csb_Uart_Send(unsigned char *UARTBUFF,unsigned char UARTBUFFLEN);
void Csb_PowerStatus(unsigned char C_Status);
#endif
