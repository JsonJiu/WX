#include "drv_CSB_CBB308.h"
#include "drv_LCD_Lierda308.h"
#include "bsp_mc308.h"
#include "drv_uart.h"
#include "timer_manage.h"
#include "user_manage.h"
#include "task_manage.h"
#include "config.h"

CSB Csb;
CSB_TIMER Csb_Timer;

/*******************************************************
* 普通IO口配置：网络标号为CSB_Irda_EN
* @author LiJi (2014-12-02)
* @打开CSB_Irda_EN
* @网络标号为CSB_Irda_EN
********************************************************/
void CSB_Irda_EN_ON()
{
    unsigned char XXX_BUFF[10]={0x12,0x45,0x98,0x91,0x01,0x33};
    //CSB_LCD_DisVolume(Csb_CURSTEPAMOUNTVOL,123456);
    CSB_LCD_DispStep(0,0,123455);
    CSB_LCD_DispStep(0,0,123456);
    CSB_LCD_DispStep(0,0,123455);
    CSB_LCD_DispStep(0,0,123455);
    CSB_LCD_DispStep(1,3,654741);
    //CSB_LCD_HardVision(0x31,0x42,0x53);
    //CSB_LCD_SoftVision(0x53,0x24,0x13);
    //CSB_LCD_DispKPMeterID(0,XXX_BUFF);
    //CSB_LCD_DispKPMeterID(1,XXX_BUFF+3);
    //CSB_LCD_DispKPPD(0x678901);
    //CSB_LCD_DispKPNetID(XXX_BUFF+4);
    Csb_Irda_EN(1);
    DelayMs(2);
    if(!Csb.CSB_Irda_Start_Flag)
    {
        Csb_Irda_EN(0);
    }
}
/*****************************************************************
            超声波计量异常---无数据超时函数
*****************************************************************/
void CSB_Timer()
{
  if(Csb.CSB_Timer_Time<CSB_OVERTIMER)
  {
    Csb.CSB_Timer_Time++;
    if(Csb.CSB_Timer_Time>=2)
    {
        Csb.CSB_FlowRate=0;
    }
  }
  else
  {
      if(!Csb.CSB_Measure_Status)
      {
          Csb.CSB_Measure_Status=1;
            Set_CSB_Measure_Err(Csb.CSB_Measure_Status);
            sprintf((char*)DebugBuf,"\r\n=====%x年%x月%x日%x时%x分%x秒 ！！出现计量异常！！=====\r\n",RTC.Year, RTC.Mon, RTC.Day, RTC.Hour, RTC.Min, RTC.Sec);
            OM_SendUsartData(Debug_COM, DebugBuf, strlen((char*)DebugBuf));
            //app_UpComing_Task(CSB_MEASURE_UPCOMING);
            OV_CloseValve_Opt(10);
            LCD_Disp();
      }
  }
}
/**********************************************************************
          超声波显示---温度；压力；瞬时量显示
***********************************************************************/
void CSB_Display()
{
  switch(Csb.CSB_Display_Time)
  {
  case 0:
    CSB_LCD_DisFlow(Csb.CSB_Work_Direct,Csb.CSB_Measure_Status,Csb.CSB_FlowRate_Status,Csb.CSB_FlowRate);
    break;
  case 10:
    CSB_LCD_DisTemp(Csb.CSB_Work_Direct,Csb.CSB_Temp_Status,Csb.CSB_Temp);
    break;
  case 20:
    CSB_LCD_DisPre(Csb.CSB_Work_Direct,Csb.CSB_Pre_Status,Csb.CSB_Pre);
    break;
  }
  if(Csb.CSB_FlowRate>0)
  {
      Csb.CSB_Display_Time +=5;
  }
  else
  {
      Csb.CSB_Display_Time++;
  }
  if(Csb.CSB_Display_Time>=30)
  {
    Csb.CSB_Display_Time=0;
  }
}
/**********************************************************************
          超声波显示---温度；压力；瞬时量显示
***********************************************************************/
void CSB_INSP_Display()
{
  CSB_LCD_Insp_DisFlow(Csb.CSB_Work_Direct,Csb.CSB_Measure_Status,Csb.CSB_FlowRate);
}
/*************************************************************************
               超声波数据接收---串口接收数据
*************************************************************************/
void CSB_Uart_RX(unsigned char CSB_Uart_RX_Data)
{
  Csb.CSB_Uart_RX_Buff[Csb.CSB_Uart_RX_BuffLen]=CSB_Uart_RX_Data;
  if(Csb.CSB_Uart_RX_Buff[0]==CSB_Uart_RX_Head)
  {
      Csb.CSB_Uart_RX_BuffLen++;
      if(Csb.CSB_Uart_RX_BuffLen > 47)
      {
          Csb.CSB_Uart_RX_BuffLen =0;
      }
  }
  Csb.CSB_Uart_Stop_Flag=1;
}

/*************************************************************************
               光电数据接收---串口接收数据
*************************************************************************/
void CSB_Irda_RX(unsigned char CSB_Irda_RX_Data)
{
  Csb.CSB_Irda_RX_Buff[Csb.CSB_Irda_RX_BuffLen]=CSB_Irda_RX_Data;
  if(Csb.CSB_Irda_RX_Buff[0]==CSB_Irda_RX_Head)
  {
      Csb.CSB_Irda_RX_BuffLen++;
      if(Csb.CSB_Irda_RX_BuffLen > 125)
      {
          Csb.CSB_Irda_RX_BuffLen=0;
      }
  }
  Csb.CSB_Irda_Stop_Flag=1;
}

/*************************************************************************
                      超声波计量板数据解析
*************************************************************************/
void CSB_Check_Uart_Data(void)
{
    if(Csb.CSB_Uart_RX_Time <CSB_Uart_MAXRXTIMER)
    {
    if((Csb.CSB_Uart_RX_Buff[0] ==CSB_Uart_RX_Head)&&(Csb.CSB_Uart_RX_BuffLen>0))
    {
      if(!Csb.CSB_Uart_Stop_Flag)
      {
        unsigned char CSB_Uart_Check_Data_i;
        unsigned char CSB_Uart_Check_Data_cs = 0;
        unsigned CSB_Check_Uart_Data_Len;
        if(Csb.CSB_Uart_RX_BuffLen < 22)////////
        {
          CSB_Uart_Receive_Off();
        }
        else
        {
          if(Csb.CSB_Uart_RX_Buff[1]==0xAB)///////////如果为校检表协议直接发出
          {
              Csb_Irda_Send(Csb.CSB_Uart_RX_Buff,Csb.CSB_Uart_RX_BuffLen);
              CSB_Uart_Receive_Off();
          }
          else///////////如果为计量数据解析
          {
            if(Csb.CSB_Uart_RX_BuffLen == 3+Csb.CSB_Uart_RX_Buff[1])
            {
              CSB_Check_Uart_Data_Len = 3+Csb.CSB_Uart_RX_Buff[1];
              for(CSB_Uart_Check_Data_i=1;CSB_Uart_Check_Data_i< CSB_Check_Uart_Data_Len-2;CSB_Uart_Check_Data_i++)
              {
                CSB_Uart_Check_Data_cs += Csb.CSB_Uart_RX_Buff[CSB_Uart_Check_Data_i];
              }
              if(CSB_Uart_Check_Data_cs ==Csb.CSB_Uart_RX_Buff[CSB_Check_Uart_Data_Len-2] )
              {
                Csb.CSB_Timer_Time=0;///////////////////////////计量数据解析成功
                if(Csb.CSB_Measure_Status)
                {
                    Csb.CSB_Measure_Status=0;
                }
                CSB_Handle_Uart_Protocol();
                CSB_Uart_Receive_Off();
              }
              else
              {
                CSB_Uart_Receive_Off();
              }
            }
            else
            {
              CSB_Uart_Receive_Off();
            }
          }
        }
      }
      else
      {
        Csb.CSB_Uart_Stop_Flag=0;
      }
    }
    else
    {
      Csb.CSB_Uart_RX_BuffLen=0;
    }
    Csb.CSB_Uart_RX_Time++;
    }
  else
  {
    CSB_Uart_Receive_Off();
  }
}
/***************************************************************************
          超声波数据接收---有数据关闭中断启动数据接收
***************************************************************************/
void CSB_Uart_Receive_On()
{
  CSB_Uart_Off();
  Csb.CSB_Uart_RX_BuffLen=0;
  Csb.CSB_Uart_RX_Time=0;
  OM_Open_Usart(Metre_COM);
  User_StartTime(MAINTIMER,Csb_Timer.Uart_Rx_OverTime_NUM,10,CSB_Check_Uart_Data);//////10ms
}
/**************************************************************************
        超声波数据接收---数据解析完成开启中断关闭数据接收
**************************************************************************/
void CSB_Uart_Receive_Off()
{
  User_CloseTime(Csb_Timer.Uart_Rx_OverTime_NUM);
  Csb.CSB_Uart_RX_BuffLen=0;
  Csb.CSB_Uart_RX_Time=0;
  CSB_Uart_On();
}
/*******************************************************
* 普通IO口配置：网络标号为CSB_Irda_EN
* @author LiJi (2014-12-02)
* @关闭CSB_Irda_EN
* @网络标号为CSB_Irda_EN
********************************************************/
void CSB_Irda_EN_OFF()
{
    Csb_Irda_EN(0);
}
void CSB_Irda_Receive_On()
{
  Csb.CSB_Irda_Start_Flag=1;
  User_CloseTime(Csb_Timer.Irda_Scan_Time_NUM);
  CSB_Irda_Off();
  Csb.CSB_Irda_RX_BuffLen=0;
  Csb.CSB_Irda_RX_Time=0;
  OM_Open_Usart(DEFAULT);
  User_StartTime(MAINTIMER,Csb_Timer.Irda_Rx_OverTime_NUM,20,CSB_Check_Irda_Data2);//////10ms
}
void CSB_Irda_Receive_Off()
{
    User_CloseTime(Csb_Timer.Irda_Rx_OverTime_NUM);
    Csb.CSB_Irda_RX_BuffLen=0;
    Csb.CSB_Irda_RX_Time=0;
    Csb.CSB_Irda_Start_Flag=0;
    CSB_Irda_On();
    User_StartTime(SUBTIMER,Csb_Timer.Irda_Scan_Time_NUM,2,CSB_Irda_EN_ON);//////2s光电扫描
}
/*************************************************************************
                      光电数据解析
*************************************************************************/
void CSB_Check_Irda_Data2(void)
{
    if(Csb.CSB_Irda_RX_Time <CSB_Irda_MAXRXTIMER)
    {
    if((Csb.CSB_Irda_RX_BuffLen >0)&&(Csb.CSB_Irda_RX_Buff[0] == CSB_Irda_RX_Head))
    {
      if(!Csb.CSB_Irda_Stop_Flag)
      {
        unsigned char CSB_Check_Irda_Data_i,CSB_Check_Irda_Data_cs=0;
        unsigned CSB_Check_Irda_Data_Len;
        if(Csb.CSB_Irda_RX_Buff[1] == CSB_Irda_RX_Head1)
        {
          if(Csb.CSB_Irda_RX_BuffLen < 18)
          {
            CSB_Irda_Receive_Off();
          }
          else if(Csb.CSB_Irda_RX_BuffLen == 19+Csb.CSB_Irda_RX_Buff[15]+Csb.CSB_Irda_RX_Buff[16])
          {
            CSB_Check_Irda_Data_Len =Csb.CSB_Irda_RX_BuffLen-2;
            for(CSB_Check_Irda_Data_i=0;CSB_Check_Irda_Data_i<CSB_Check_Irda_Data_Len;CSB_Check_Irda_Data_i++)
            {
              CSB_Check_Irda_Data_cs +=Csb.CSB_Irda_RX_Buff[CSB_Check_Irda_Data_i];
            }
            if(CSB_Check_Irda_Data_cs == Csb.CSB_Irda_RX_Buff[CSB_Check_Irda_Data_Len])
            {
              //////////////////////光电计量板协议通过直接发给计量板
                BspBeepOn();
                DelayMs(50);
                BspBeepOff();
                Csb_Uart_Send(Csb.CSB_Irda_RX_Buff,Csb.CSB_Irda_RX_BuffLen);
                CSB_Irda_Receive_Off();
            }
            else
            {
              CSB_Irda_Receive_Off();
            }
          }
          else
          {
            CSB_Irda_Receive_Off();
          }
        }
        else
        {
          if(Csb.CSB_Irda_RX_BuffLen < 13)
          {
            CSB_Irda_Receive_Off();
          }
          else
          {
            CSB_Check_Irda_Data_Len  =Csb.CSB_Irda_RX_Buff[10];
            CSB_Check_Irda_Data_Len <<=8;
            CSB_Check_Irda_Data_Len  |=Csb.CSB_Irda_RX_Buff[9];
            CSB_Check_Irda_Data_Len +=13;
            if(Csb.CSB_Irda_RX_BuffLen ==CSB_Check_Irda_Data_Len)
            {
              if((Csb.CSB_Irda_RX_Buff[7] !=0x68)||(Csb.CSB_Irda_RX_Buff[Csb.CSB_Irda_RX_BuffLen-1] !=0x16))
              {
                CSB_Irda_Receive_Off();
              }
              else
              {
                CSB_Check_Irda_Data_Len = 13+Csb.CSB_Irda_RX_Buff[9]+Csb.CSB_Irda_RX_Buff[10]-2;
                for(CSB_Check_Irda_Data_i=0;CSB_Check_Irda_Data_i<CSB_Check_Irda_Data_Len;CSB_Check_Irda_Data_i++)
                {
                  CSB_Check_Irda_Data_cs +=Csb.CSB_Irda_RX_Buff[CSB_Check_Irda_Data_i];
                }
                if(CSB_Check_Irda_Data_cs == Csb.CSB_Irda_RX_Buff[CSB_Check_Irda_Data_Len])
                {
                  //////////////////////智能板协议解析成功
                    BspBeepOn();
                    DelayMs(50);
                    BspBeepOff();
                    /*if(DealDebugUsartCommand(Csb.CSB_Irda_RX_Buff) != 0)
                    {
                        DebugUart_AckData(DebufUsartACK);
                    }*/
                    CSB_Irda_Receive_Off();
                }
                else
                {
                    CSB_Irda_Receive_Off();
                }
              }
            }
            else
            {
              CSB_Irda_Receive_Off();
            }
          }
        }
      }
      else
      {
        Csb.CSB_Irda_Stop_Flag=0;
      }
    }
    else
    {
      Csb.CSB_Irda_RX_BuffLen=0;
    }
    Csb.CSB_Irda_RX_Time++;
    }
  else
  {
    CSB_Irda_Receive_Off();
  }
}
/**********************************************************************
                  超声波数据解析
**********************************************************************/
void CSB_Handle_Uart_Protocol()
{
  Csb.CSB_Temp_Status=Csb.CSB_Uart_RX_Buff[13];
  if(!Csb.CSB_Temp_Status)
  {
    Csb.CSB_Temp=Csb.CSB_Uart_RX_Buff[11];
    Csb.CSB_Temp <<=8;
    Csb.CSB_Temp |=Csb.CSB_Uart_RX_Buff[12];
  }
  Csb.CSB_Pre_Status=Csb.CSB_Uart_RX_Buff[16];
  if(!Csb.CSB_Pre_Status)
  {
    Csb.CSB_Pre=Csb.CSB_Uart_RX_Buff[14];
    Csb.CSB_Pre <<=8;
    Csb.CSB_Pre |=Csb.CSB_Uart_RX_Buff[15];
  }
  Csb.CSB_FlowRate=Csb.CSB_Uart_RX_Buff[2];
  Csb.CSB_FlowRate <<=8;
  Csb.CSB_FlowRate |=Csb.CSB_Uart_RX_Buff[3];
  Csb.CSB_FlowRate <<=8;
  Csb.CSB_FlowRate |=Csb.CSB_Uart_RX_Buff[4];
  Csb.CSB_FlowRate <<=8;
  Csb.CSB_FlowRate |=Csb.CSB_Uart_RX_Buff[5];

  Csb.CSB_Volume =Csb.CSB_Uart_RX_Buff[6];
  Csb.CSB_Volume <<=8;
  Csb.CSB_Volume |=Csb.CSB_Uart_RX_Buff[7];
  Csb.CSB_Volume <<=8;
  Csb.CSB_Volume |=Csb.CSB_Uart_RX_Buff[8];
  Csb.CSB_Volume <<=8;
  Csb.CSB_Volume |=Csb.CSB_Uart_RX_Buff[9];

  Csb.CSB_Work_Direct=Csb.CSB_Uart_RX_Buff[18];
  Csb.CSB_Meter_Type =Csb.CSB_Uart_RX_Buff[17];
  if((!Csb.CSB_Work_Status)&&(Csb.CSB_Uart_RX_Buff[10] !=0 ))
  {
    Csb.CSB_Work_Status=1;
    User_CloseTime(Csb_Timer.Display_ShiftTime_NUM);
    User_StartTime(SUBTIMER,Csb_Timer.Insp_Display_NUM,1,CSB_INSP_Display);
  }
  if((Csb.CSB_Work_Status)&&(Csb.CSB_Uart_RX_Buff[10] ==0))
  {
    Csb.CSB_Work_Status=0;
    User_CloseTime(Csb_Timer.Insp_Display_NUM);
    User_StartTime(SUBTIMER,Csb_Timer.Display_ShiftTime_NUM,1,CSB_Display);
  }
  Csb.CSB_Err |=Csb.CSB_Uart_RX_Buff[19];
  Csb.CSB_FlowRate_Status =Csb.CSB_Uart_RX_Buff[19];
  if(!Csb.CSB_Work_Direct)
  {
      Csb.CSB_Err |=0x08;
  }
  if(Csb.CSB_Temp_Status)
  {
      Csb.CSB_Err |=0x10;
  }
  if(Csb.CSB_Pre_Status)
  {
      Csb.CSB_Err |=0x20;
  }
  Csb.CSB_Measure_Status = 0;
  Csb.CSB_LCD = 1;
  //CSB_Set_Status_(Csb.CSB_Err);
  //SetValue(BASEVAL_Amount, Csb.CSB_Volume/100);
  //SetValueTemp(Csb.CSB_Temp);
  //SetValuePre(Csb.CSB_Pre);
  //Set_CSB_Measure_Err(Csb.CSB_Measure_Status);
  /*if(UpComingProgress.Status == opt_Halt)
  {
      DISP.Type = DIS_STA_GAS;
      LCD_Disp();
  }
  else
  {
      App_LCD_DispOverTime_Task();
  */
}
void CSB_TASK_Init()
{
    if(Csb_Timer.Uart_Rx_OverTime_NUM == 0)
    {
        Csb_Timer.Uart_Rx_OverTime_NUM=User_AskTimeNumber();//////计量串口接收数据时间片
    }
    if(Csb_Timer.Irda_Rx_OverTime_NUM == 0)
    {
        Csb_Timer.Irda_Rx_OverTime_NUM=User_AskTimeNumber();//////光电串口接收数据时间片
    }
    if(Csb_Timer.Data_OverTime_NUM == 0)
    {
        Csb_Timer.Data_OverTime_NUM=User_AskTimeNumber();//////计量数据超时异常时间片
    }
    if(Csb_Timer.Display_ShiftTime_NUM == 0)
    {
        Csb_Timer.Display_ShiftTime_NUM=User_AskTimeNumber();//////轮询显示时间片
    }
    if(Csb_Timer.Insp_Display_NUM ==0)
    {
        Csb_Timer.Insp_Display_NUM=User_AskTimeNumber();///////////检测态显示
    }
    if(Csb_Timer.Irda_Scan_Time_NUM == 0)
    {
        Csb_Timer.Irda_Scan_Time_NUM=User_AskTimeNumber();//////光电扫描时间片
    }
    User_CloseTime(Csb_Timer.Uart_Rx_OverTime_NUM);
    User_CloseTime(Csb_Timer.Irda_Rx_OverTime_NUM);
    User_CloseTime(Csb_Timer.Insp_Display_NUM);
    User_CloseTime(Csb_Timer.Irda_Scan_Time_NUM);
    User_CloseTime(Csb_Timer.Display_ShiftTime_NUM);
    User_CloseTime(Csb_Timer.Data_OverTime_NUM);
    User_StartTime(SUBTIMER,Csb_Timer.Irda_Scan_Time_NUM,2,CSB_Irda_EN_ON);//////2s光电扫描
    User_StartTime(SUBTIMER,Csb_Timer.Display_ShiftTime_NUM,1,CSB_Display);//////10s轮显
    User_StartTime(SUBTIMER,Csb_Timer.Data_OverTime_NUM,3,CSB_Timer);//////1min计量板数据
    Csb.CSB_Work_Direct=1;
    Csb.CSB_Measure_Status = 1;
    Csb.CSB_Display_Time=10;
    Csb_Irda_Init();
    OM_Open_Usart(Metre_COM);
    OM_Open_Usart(DEFAULT);
    Csb_Uart_Int_Config(CSB_Uart_Receive_On);
    Csb_Irda_Int_Config(CSB_Irda_Receive_On);
    DelayMs(1000);
    Csb_PowerStatus(1);
}
void Csb_Irda_Send(unsigned char *IRDABUFF,unsigned char IRDABUFFLEN)
{
    unsigned char Irda_HuanXing[10] ={0xFE,0xFE,0xFE,0xFE,0xFE,0xFE,0xFE,0xFE,0xFE,0xFE};
    Csb_Irda_EN(1);
    DelayMs(5);
    IM_m2m_Usart_Send_Data(DEFAULT,Irda_HuanXing,10);
    IM_m2m_Usart_Send_Data(DEFAULT,IRDABUFF,IRDABUFFLEN);
    DelayMs(5);
    Csb_Irda_EN(0);
}
void Csb_Uart_Send(unsigned char *UARTBUFF,unsigned char UARTBUFFLEN)
{
    unsigned char Irda_HuanXing[10] ={0xFE,0xFE,0xFE,0xFE,0xFE,0xFE,0xFE,0xFE,0xFE,0xFE};
    DelayMs(2);
    IM_m2m_Usart_Send_Data(Metre_COM,Irda_HuanXing,10);
    IM_m2m_Usart_Send_Data(Metre_COM,UARTBUFF,UARTBUFFLEN);
    DelayMs(2);
}
void Csb_PowerStatus(unsigned char C_Status)
{
    unsigned char Irda_HuanXing[7] ={0xFE,0xFE,0xFE,0xFE,0xFE,0x68,0xFE};
    DelayMs(2);
    if(C_Status)
    {
       Irda_HuanXing[6] =0xFA;
       IM_m2m_Usart_Send_Data(Metre_COM,Irda_HuanXing,7);
       DelayMs(500);
    }
    else
    {
        Irda_HuanXing[6] =0xFC;
        IM_m2m_Usart_Send_Data(Metre_COM,Irda_HuanXing,7);
        DelayMs(500);
    }
    IM_m2m_Usart_Send_Data(Metre_COM,Irda_HuanXing,7);
    DelayMs(2);
}
