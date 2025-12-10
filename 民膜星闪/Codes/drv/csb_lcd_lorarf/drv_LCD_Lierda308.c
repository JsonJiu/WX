#include "drv_Lcd_Lierda308.h"
#include    "bsp_mc308.h"
#include    "lib_mc308.h"
#define LSD_001D_ADD    0x7C        // IIC地址
#define ICSET          0xea       // 软件复位，采用内部晶振
#define ADSET          0x00       // 映射起始地址
#define MODESET_ON     0xc8       // 打开显示
#define MODESET_OFF    0xc0       // 关闭显示
#define MODE_DIS_3     0xc4       // 1/3Bais
#define MODE_DIS_2     0xc0       // 1/2Bais
//*****************************************************************************
// 省电模式

//LINE波形驱动
#define DISCTL_FRPM1_LINE_SRPM1       0xA8  // 101 01 0 00   FR1+SR1+LINE
#define DISCTL_FRPM2_LINE_SRPM2       0xB1  // 101 10 0 01   FR2+SR2+LINE
#define DISCTL_FRPM3_LINE_SRPM1       0xB8  // 101 11 0 00   FR3+SR1+LINE 最省
#define DISCTL_FRNOR_LINE_SRHP        0xA3  // 101 00 0 11   FRNOR+SRHP+LINE 最大

#define DISCTL_FRNOR_LINE_SRNOR       0xA2  // 101 00 0 10   FRNOR+SRNOR+LINE 上电默认

//FRAME波形驱动
#define DISCTL_FRPM1_FRAME_SRPM1      0xAC  // 101 01 1 00   FR1+SR1+FRAME 典型
#define DISCTL_FRPM2_FRAME_SRPM2      0xB5  // 101 10 1 01   FR2+SR2+FRAME
#define DISCTL_FRPM3_FRAME_SRPM1      0xBC  // 101 11 1 00   FR3+SR1+FRAME 最省
#define DISCTL_FRNOR_FRAME_SRHP       0xA7  // 101 00 1 11   FRNOR+SRHP+FRAME 最大


//闪烁控制
#define BLKCTL_OFF                    0xF0  // 关闭闪烁
#define BLKCTL_05HZ                   0xF1  // 闪烁频率为0.5HZ
#define BLKCTL_1HZ                    0xF2  // 闪烁频率为1HZ
#define BLKCTL_2HZ                    0xF3  // 闪烁频率为2HZ

//全屏强行开或关,与显示内存内容无关,两个功能中全屏关为优先执行
#define APCTL_APON_ON                  0xFE // 全屏强行全显示_开
#define APCTL_APON_OFF                 0xFC // 全屏强行全显示_关

#define APCTL_APOFF_ON                 0xFD // 全屏强行关显示_开
#define APCTL_APOFF_OFF                0xFC // 全屏强行关显示_关

#define Up_Start_ADD                   22//21
#define Down_Start_ADD                 0//4//3

unsigned char Up_DisBuff[6];
unsigned char Down_DisBuff[10];
unsigned char Status_DisBuff;

unsigned char Up_Digital_Buff[10] = {Up_A+Up_B+Up_C+Up_D+Up_E+Up_F,
                                    Up_B+Up_C,
                                    Up_A+Up_B+Up_D+Up_E+Up_G,
                                    Up_A+Up_B+Up_C+Up_D+Up_G,
                                    Up_B+Up_C+Up_F+Up_G,
                                    Up_A+Up_F+Up_G+Up_C+Up_D,
                                    Up_A+Up_F+Up_G+Up_E+Up_D+Up_C,
                                    Up_A+Up_B+Up_C,
                                    Up_A+Up_B+Up_C+Up_D+Up_E+Up_F+Up_G,
                                    Up_A+Up_B+Up_F+Up_G+Up_C+Up_D
                                    };

unsigned char Down_Digital_Buff[10] ={Down_A+Down_B+Down_C+Down_D+Down_E+Down_F,
                                    Down_B+Down_C,
                                    Down_A+Down_B+Down_D+Down_E+Down_G,
                                    Down_A+Down_B+Down_C+Down_D+Down_G,
                                    Down_B+Down_C+Down_F+Down_G,
                                    Down_A+Down_F+Down_G+Down_C+Down_D,
                                    Down_A+Down_F+Down_G+Down_E+Down_D+Down_C,
                                    Down_A+Down_B+Down_C,
                                    Down_A+Down_B+Down_C+Down_D+Down_E+Down_F+Down_G,
                                    Down_A+Down_B+Down_F+Down_G+Down_C+Down_D
                                    };

void CSB_LCD_Delay()
{
  /*unsigned char LCD_Delay_i;
  for(LCD_Delay_i=0;LCD_Delay_i<1;LCD_Delay_i++)
  {
    Nop();
  }*/
    Nop();
}
void CSB_LCD_Init()
{
  LCD_SCL_OUT_Dir();
  LCD_SCL(1);
  LCD_SDA_OUT_Dir();
  LCD_SDA(1);
  DelayMs(1);
  CSB_LCD_SoftInit();
  CSB_LCD_ON();
}
void CSB_LCD_Start()
{
  LCD_SDA_OUT_Dir();
  LCD_SDA(1);
  LCD_SCL(1);
  CSB_LCD_Delay();
  LCD_SDA(0);
  LCD_SCL(0);
}
void CSB_LCD_Stop()
{
  LCD_SDA_OUT_Dir();
  LCD_SDA(0);
  LCD_SCL(1);
  CSB_LCD_Delay();
  LCD_SDA(1);
  LCD_SCL(0);
  LCD_SDA(1);
  LCD_SCL(1);
  CSB_LCD_Delay();
}
unsigned char CSB_LCD_SendByte(unsigned char Data)
{
  unsigned char LCD_SendByte_i;
  LCD_SDA_OUT_Dir();
  LCD_SCL(0);
  for(LCD_SendByte_i=0;LCD_SendByte_i<8;LCD_SendByte_i++)
  {
    if(Data&0x80)
    {
      LCD_SDA(1);
    }
    else
    {
      LCD_SDA(0);
    }
    CSB_LCD_Delay();
    LCD_SCL(1);
    CSB_LCD_Delay();
    LCD_SCL(0);
    Data=Data<<1;
  }
  LCD_SDA_IN_Dir();
  CSB_LCD_Delay();
  LCD_SCL(1);
  CSB_LCD_Delay();
  if(LCD_SDA_InPut()==0)
  {
    LCD_SCL(0);
    return TRUE;
  }
  else
  {
    LCD_SCL(0);
    return FALSE;
  }
}
unsigned char CSB_LCD_SendBytes(unsigned char *Datas,unsigned char Len)
{
  unsigned char LCD_SendBytes_i;
  for(LCD_SendBytes_i=0;LCD_SendBytes_i<Len;LCD_SendBytes_i++)
  {
    if(CSB_LCD_SendByte(*(Datas+LCD_SendBytes_i))==FALSE)
     return FALSE;
  }
  return TRUE;
}
unsigned char CSB_LCD_SoftInit()
{
  CSB_LCD_Start();
  if(CSB_LCD_SendByte(LSD_001D_ADD)==FALSE)
  {
    CSB_LCD_Stop();
    return FALSE;
  }
  CSB_LCD_SendByte(ICSET);////ICSET
  CSB_LCD_SendByte(BLKCTL_OFF);////BLKCTL
  CSB_LCD_SendByte(DISCTL_FRPM3_FRAME_SRPM1);////DISCTL
  CSB_LCD_Stop();
  return TRUE;
}
unsigned char CSB_LCD_ON()
{
  CSB_LCD_Start();
  if(CSB_LCD_SendByte(LSD_001D_ADD)==FALSE)
  {
    CSB_LCD_Stop();
    return FALSE;
  }
  CSB_LCD_SendByte(MODESET_ON);////MODESET
  CSB_LCD_Stop();
  return TRUE;
}

unsigned char CSB_LCD_OFF()
{
  CSB_LCD_Start();
  if(CSB_LCD_SendByte(LSD_001D_ADD)==FALSE)
  {
    CSB_LCD_Stop();
    return FALSE;
  }
  CSB_LCD_SendByte(MODESET_OFF);////MODESET
  CSB_LCD_Stop();
  return TRUE;
}
/************************************************
                  清屏
*************************************************/
unsigned char CSB_LCD_Clear()
{
  unsigned char LCD_Clear_i;
  CSB_LCD_Start();
  if(CSB_LCD_SendByte(LSD_001D_ADD)==FALSE)
  {
    CSB_LCD_Stop();
    return FALSE;
  }
  CSB_LCD_SendByte(0);
  for(LCD_Clear_i=0;LCD_Clear_i<32;LCD_Clear_i++)
  {
    CSB_LCD_SendByte(0);
  }
  CSB_LCD_Stop();
  return TRUE;
}
/************************************************
                  全显
*************************************************/
unsigned char CSB_LCD_All()
{
  unsigned char LCD_All_i;
  CSB_LCD_Start();
  if(CSB_LCD_SendByte(LSD_001D_ADD)==FALSE)
  {
    CSB_LCD_Stop();
    return FALSE;
  }
 CSB_LCD_SendByte(0);
  for(LCD_All_i=0;LCD_All_i<32;LCD_All_i++)
  {
    CSB_LCD_SendByte(0xFF);
  }
  CSB_LCD_Stop();
  return TRUE;
}
unsigned char CSB_LCD_DisTest()
{
  unsigned char LCD_DisTest_i;
  //unsigned char LCD_DisTest_j;
  for(LCD_DisTest_i=0;LCD_DisTest_i<21;LCD_DisTest_i++)
  {
    CSB_LCD_Start();
    if(CSB_LCD_SendByte(LSD_001D_ADD)==FALSE)
    {
      CSB_LCD_Stop();
      return FALSE;
    }
    CSB_LCD_SendByte(LCD_DisTest_i);
    CSB_LCD_SendByte(0xff);
    CSB_LCD_Stop();

    CSB_LCD_Clear();

    Nop();
  }
  CSB_LCD_Start();
  if(CSB_LCD_SendByte(LSD_001D_ADD)==FALSE)
  {
    CSB_LCD_Stop();
    return FALSE;
  }
  CSB_LCD_SendByte(15);
  CSB_LCD_SendByte(0xff);
  CSB_LCD_Stop();
  return TRUE;
}
/**********************************************
                 显示流量
***********************************************/
unsigned char CSB_LCD_DisFlow(unsigned char D_Direct,unsigned char D_FlowStatus,unsigned char D_FlowRate_Status,unsigned long D_Flow)
{
  unsigned char LCD_DisFlow_i;
  Up_DisBuff[5] =  0;
  if(D_FlowStatus)
  {
    CSB_LCD_DisUPErr(9);
  }
  else
  {
    if(!D_Direct)
    {
      Up_DisBuff[FanXiang_Position] =FanXiang_Symbol;
    }
    else
    {
      Up_DisBuff[FanXiang_Position] =0;
    }
    if(D_FlowRate_Status&0x01)
    {
        CSB_LCD_DisUPErr(7);
    }
    else if(D_FlowRate_Status&0x02)
    {
        CSB_LCD_DisUPErr(8);
    }
    else if(D_FlowRate_Status&0x04)
    {
        CSB_LCD_DisUPErr(6);
    }
    else
    {
        if(D_Flow<100000)
        {
          Up_DisBuff[0] |=  Up_Digital_Buff[(D_Flow%100000)/10000];
          Up_DisBuff[1] =  Up_Digital_Buff[(D_Flow%10000)/1000];
          Up_DisBuff[1] |= P4_Symbol;
          Up_DisBuff[2] =  Up_Digital_Buff[(D_Flow%1000)/100];
          Up_DisBuff[3] =  Up_Digital_Buff[(D_Flow%100)/10];
          Up_DisBuff[4] =  Up_Digital_Buff[(D_Flow%10)/1];
        }
        else
        {
          Up_DisBuff[0] |=  Up_Digital_Buff[(D_Flow%1000000)/100000];
          Up_DisBuff[1] =  Up_Digital_Buff[(D_Flow%100000)/10000];
          Up_DisBuff[2] =  Up_Digital_Buff[(D_Flow%10000)/1000];
          Up_DisBuff[2] |= P4_Symbol;
          Up_DisBuff[3] =  Up_Digital_Buff[(D_Flow%1000)/100];
          Up_DisBuff[4] =  Up_Digital_Buff[(D_Flow%100)/10];
        }
    }
  }
  Up_DisBuff[5] |=  Flow_Symbol;
  CSB_LCD_Start();
  if(CSB_LCD_SendByte(LSD_001D_ADD)==FALSE)
  {
    CSB_LCD_Stop();
    return FALSE;
  }
  CSB_LCD_SendByte(Up_Start_ADD);
  for(LCD_DisFlow_i=0;LCD_DisFlow_i<6;LCD_DisFlow_i++)
  {
    CSB_LCD_SendByte(Up_DisBuff[LCD_DisFlow_i]);
  }
  CSB_LCD_Stop();
  return TRUE;
}
unsigned char CSB_LCD_Insp_DisFlow(unsigned char D_Direct,unsigned char D_FlowStatus,unsigned long D_Flow)
{
  unsigned char LCD_DisFlow_i;
  Up_DisBuff[5] =  0;
  if(D_FlowStatus)
  {
    CSB_LCD_DisUPErr(9);
  }
  else
  {
    if(!D_Direct)
    {
      Up_DisBuff[FanXiang_Position] =FanXiang_Symbol;
    }
    else
    {
      Up_DisBuff[FanXiang_Position] =0;
    }
    if(D_Flow<100000)
    {
      Up_DisBuff[0] |=  Up_Digital_Buff[(D_Flow%100000)/10000];
      Up_DisBuff[1] =  Up_Digital_Buff[(D_Flow%10000)/1000];
      Up_DisBuff[1] |= P4_Symbol;
      Up_DisBuff[2] =  Up_Digital_Buff[(D_Flow%1000)/100];
      Up_DisBuff[3] =  Up_Digital_Buff[(D_Flow%100)/10];
      Up_DisBuff[4] =  Up_Digital_Buff[(D_Flow%10)/1];
    }
    else
    {
      Up_DisBuff[0] |=  Up_Digital_Buff[(D_Flow%1000000)/100000];
      Up_DisBuff[1] =  Up_Digital_Buff[(D_Flow%100000)/10000];
      Up_DisBuff[2] =  Up_Digital_Buff[(D_Flow%10000)/1000];
      Up_DisBuff[2] |= P4_Symbol;
      Up_DisBuff[3] =  Up_Digital_Buff[(D_Flow%1000)/100];
      Up_DisBuff[4] =  Up_Digital_Buff[(D_Flow%100)/10];
    }

  }
  Up_DisBuff[5] |=  Insp_Position;
  Up_DisBuff[5] |=  Flow_Symbol;
  CSB_LCD_Start();
  if(CSB_LCD_SendByte(LSD_001D_ADD)==FALSE)
  {
    CSB_LCD_Stop();
    return FALSE;
  }
  CSB_LCD_SendByte(Up_Start_ADD);
  for(LCD_DisFlow_i=0;LCD_DisFlow_i<6;LCD_DisFlow_i++)
  {
    CSB_LCD_SendByte(Up_DisBuff[LCD_DisFlow_i]);
  }
  CSB_LCD_Stop();
  return TRUE;
}
/**********************************************
                 显示温度
***********************************************/
unsigned char CSB_LCD_DisTemp(unsigned char D_Direct,unsigned char D_TempStatus,signed int D_Temp)
{
  unsigned char LCD_DisTemp_i;
  Up_DisBuff[5] =  0;
  if(D_TempStatus)
  {
    CSB_LCD_DisUPErr(1);
  }
  else
  {
    if(!D_Direct)
    {
      Up_DisBuff[FanXiang_Position] =FanXiang_Symbol;
    }
    else
    {
      Up_DisBuff[FanXiang_Position] =0;
    }
    if(D_Temp < 0)
    {
      D_Temp *= -1;
      Up_DisBuff[0] |= Link_Symbol;
    }
    else
    {
    }
    if(D_Temp < 1000)
    {
      Up_DisBuff[1] = 0;
    }
    else
    {
      Up_DisBuff[1] =  Up_Digital_Buff[(D_Temp%10000)/1000];
    }

    Up_DisBuff[2] =  Up_Digital_Buff[(D_Temp%1000)/100];
    Up_DisBuff[3] =  Up_Digital_Buff[(D_Temp%100)/10];
    Up_DisBuff[4] =  Up_Digital_Buff[(D_Temp%10)/1];

    Up_DisBuff[3] |= P3_Symbol;
    //Up_DisBuff[5] &=  Insp_Position;

  }
  Up_DisBuff[5] |=  Temp_Symbol;
  CSB_LCD_Start();
  if(CSB_LCD_SendByte(LSD_001D_ADD)==FALSE)
  {
    CSB_LCD_Stop();
    return FALSE;
  }
  CSB_LCD_SendByte(Up_Start_ADD);
  for(LCD_DisTemp_i=0;LCD_DisTemp_i<6;LCD_DisTemp_i++)
  {
    CSB_LCD_SendByte(Up_DisBuff[LCD_DisTemp_i]);
  }
  CSB_LCD_Stop();
  return TRUE;
}
/**********************************************
                 显示压力
***********************************************/
unsigned char CSB_LCD_DisPre(unsigned char D_Direct,unsigned char D_PreStatus,unsigned int D_Pre)
{
  unsigned char LCD_DisPre_i;
  Up_DisBuff[5] =  0;
  if(D_PreStatus)
  {
    CSB_LCD_DisUPErr(0);
  }
  else
  {
    if(!D_Direct)
    {
      Up_DisBuff[FanXiang_Position] =FanXiang_Symbol;
    }
    else
    {
      Up_DisBuff[FanXiang_Position] =0;
    }
    if(D_Pre >= 10000)
    {
      Up_DisBuff[FanXiang_Position] &=FanXiang_Symbol;
      Up_DisBuff[0] |= Up_Digital_Buff[(D_Pre%100000)/10000];
    }

    Up_DisBuff[1] = Up_Digital_Buff[(D_Pre%10000)/1000];
    Up_DisBuff[2] = Up_Digital_Buff[(D_Pre%1000)/100];
    Up_DisBuff[3] = Up_Digital_Buff[(D_Pre%100)/10];
    Up_DisBuff[4] = Up_Digital_Buff[(D_Pre%10)/1];

    Up_DisBuff[3] |= P3_Symbol;
    //Up_DisBuff[5] &=  Insp_Position;
  }
  Up_DisBuff[5] |=  Pre_Symbol;

  CSB_LCD_Start();
  if(CSB_LCD_SendByte(LSD_001D_ADD)==FALSE)
  {
    CSB_LCD_Stop();
    return FALSE;
  }
  CSB_LCD_SendByte(Up_Start_ADD);
  for(LCD_DisPre_i=0;LCD_DisPre_i<6;LCD_DisPre_i++)
  {
    CSB_LCD_SendByte(Up_DisBuff[LCD_DisPre_i]);
  }
  CSB_LCD_Stop();
  return TRUE;
}
/**********************************************
               UP-ERR
***********************************************/
unsigned char CSB_LCD_DisUPErr(unsigned char D_UPErr)
{
  unsigned char LCD_DisUPErr_i;
  Up_DisBuff[5] =  0;

  Up_DisBuff[FanXiang_Position] &=FanXiang_Symbol;

  Up_DisBuff[0] |= Up_Letter_E;

  Up_DisBuff[1] = Up_Letter_r;
  Up_DisBuff[2] = Up_Letter_r;
  Up_DisBuff[3] = Link_Symbol;
  Up_DisBuff[4] = Up_Digital_Buff[(D_UPErr%10)/1];
  Up_DisBuff[5] &=  0x1F;

  CSB_LCD_Start();
  if(CSB_LCD_SendByte(LSD_001D_ADD)==FALSE)
  {
    CSB_LCD_Stop();
    return FALSE;
  }
  CSB_LCD_SendByte(Up_Start_ADD);
  for(LCD_DisUPErr_i=0;LCD_DisUPErr_i<6;LCD_DisUPErr_i++)
  {
    CSB_LCD_SendByte(Up_DisBuff[LCD_DisUPErr_i]);
  }
  CSB_LCD_Stop();
  return TRUE;
}
/**********************************************
           Down---显示累计使用量
***********************************************/
unsigned char CSB_LCD_DisVolume(CSB_Dis_Mark DIS_M,signed long D_Volume)
{
  unsigned char LCD_DisVolume_i;
  for(LCD_DisVolume_i = 0;LCD_DisVolume_i<10;LCD_DisVolume_i++)
  {
    Down_DisBuff[LCD_DisVolume_i] = 0;
  }
  switch(DIS_M)
  {
      case Csb_AMOUNTVOL:
          Down_DisBuff[0]=QiLiang_DanWei;
          Down_DisBuff[2] = LeiJi_Symbol | QiLiang_Symbol;
          break;
      case Csb_REMAINVOL:
          Down_DisBuff[0]=QiLiang_DanWei;
          Down_DisBuff[2] = ShengYu_Symbol | QiLiang_Symbol;
          break;
      case Csb_STEPVOL:
          Down_DisBuff[0]=QiLiang_DanWei;
          Down_DisBuff[2] = JieTi_Symbol | QiLiang_Symbol;
          break;
      case Csb_REMAINMONEY:
          Down_DisBuff[0]=JinE_DanWei;
          Down_DisBuff[2] = ShengYu_Symbol | JinE_Symbol;
          break;
      case Csb_CURPRICE:
          Down_DisBuff[0]=JinE_DanWei;
          Down_DisBuff[2] = DangQian_Symbol | DanJia_Symbol;
          break;
      case Csb_CURMONEY:
          Down_DisBuff[0]=JinE_DanWei;
          Down_DisBuff[2] = DangQian_Symbol | JinE_Symbol;
          break;
      case Csb_CURSTEPAMOUNTVOL:
          Down_DisBuff[0]=QiLiang_DanWei;
          Down_DisBuff[2] = DangQian_Symbol | QiLiang_Symbol|LeiJi_Symbol|JieTi_Symbol;
          break;
      default:
          break;
  }
  if(D_Volume<0)
  {
      D_Volume =-1*D_Volume;
      if(D_Volume < 100000 )
      {
        Down_DisBuff[4]=CSB_LCD_CHAR_SEPARATE;
        Down_DisBuff[5] = Down_Digital_Buff[(D_Volume%100000)/10000];
        Down_DisBuff[6] = Down_Digital_Buff[(D_Volume%10000)/1000];
        Down_DisBuff[7] = Down_Digital_Buff[(D_Volume%1000)/100];
        Down_DisBuff[8] = Down_Digital_Buff[(D_Volume%100)/10];
        Down_DisBuff[9] = Down_Digital_Buff[(D_Volume%10)/1];
        Down_DisBuff[5] |= P10_Symbol;
      }
      else if(D_Volume < 1000000 )
      {
        Down_DisBuff[3]=CSB_LCD_CHAR_SEPARATE;
        Down_DisBuff[4] = Down_Digital_Buff[(D_Volume%1000000)/100000];
        Down_DisBuff[5] = Down_Digital_Buff[(D_Volume%100000)/10000];
        Down_DisBuff[6] = Down_Digital_Buff[(D_Volume%10000)/1000];
        Down_DisBuff[7] = Down_Digital_Buff[(D_Volume%1000)/100];
        Down_DisBuff[8] = Down_Digital_Buff[(D_Volume%100)/10];
        Down_DisBuff[9] = Down_Digital_Buff[(D_Volume%10)/1];
        Down_DisBuff[5] |= P10_Symbol;
      }
      else if(D_Volume < 10000000 )
      {
        Down_DisBuff[3]=CSB_LCD_CHAR_SEPARATE;
        Down_DisBuff[4] = Down_Digital_Buff[(D_Volume%10000000)/1000000];
        Down_DisBuff[5] = Down_Digital_Buff[(D_Volume%1000000)/100000];
        Down_DisBuff[6] = Down_Digital_Buff[(D_Volume%100000)/10000];
        Down_DisBuff[7] = Down_Digital_Buff[(D_Volume%10000)/1000];
        Down_DisBuff[8] = Down_Digital_Buff[(D_Volume%1000)/100];
        Down_DisBuff[9] = Down_Digital_Buff[(D_Volume%100)/10];
        Down_DisBuff[7] |= P8_Symbol;
      }
      else if(D_Volume < 100000000 )
      {
        Down_DisBuff[3]=CSB_LCD_CHAR_SEPARATE;
        Down_DisBuff[4] = Down_Digital_Buff[(D_Volume%100000000)/10000000];
        Down_DisBuff[5] = Down_Digital_Buff[(D_Volume%10000000)/1000000];
        Down_DisBuff[6] = Down_Digital_Buff[(D_Volume%1000000)/100000];
        Down_DisBuff[7] = Down_Digital_Buff[(D_Volume%100000)/10000];
        Down_DisBuff[8] = Down_Digital_Buff[(D_Volume%10000)/1000];
        Down_DisBuff[9] = Down_Digital_Buff[(D_Volume%1000)/100];
        Down_DisBuff[4] |= P11_Symbol;
      }
      else
      {
        Down_DisBuff[3]=CSB_LCD_CHAR_SEPARATE;
        Down_DisBuff[4] = Down_Digital_Buff[(D_Volume%1000000000)/100000000];
        Down_DisBuff[5] = Down_Digital_Buff[(D_Volume%100000000)/10000000];
        Down_DisBuff[6] = Down_Digital_Buff[(D_Volume%10000000)/1000000];
        Down_DisBuff[7] = Down_Digital_Buff[(D_Volume%1000000)/100000];
        Down_DisBuff[8] = Down_Digital_Buff[(D_Volume%100000)/10000];
        Down_DisBuff[9] = Down_Digital_Buff[(D_Volume%10000)/1000];
        Down_DisBuff[9] |= P6_Symbol;
      }
  }
  else
  {
      if(D_Volume < 100000 )
      {
        Down_DisBuff[5] = Down_Digital_Buff[(D_Volume%100000)/10000];
        Down_DisBuff[6] = Down_Digital_Buff[(D_Volume%10000)/1000];
        Down_DisBuff[7] = Down_Digital_Buff[(D_Volume%1000)/100];
        Down_DisBuff[8] = Down_Digital_Buff[(D_Volume%100)/10];
        Down_DisBuff[9] = Down_Digital_Buff[(D_Volume%10)/1];
        Down_DisBuff[5] |= P10_Symbol;
      }
      else if(D_Volume < 1000000 )
      {
        Down_DisBuff[4] = Down_Digital_Buff[(D_Volume%1000000)/100000];
        Down_DisBuff[5] = Down_Digital_Buff[(D_Volume%100000)/10000];
        Down_DisBuff[6] = Down_Digital_Buff[(D_Volume%10000)/1000];
        Down_DisBuff[7] = Down_Digital_Buff[(D_Volume%1000)/100];
        Down_DisBuff[8] = Down_Digital_Buff[(D_Volume%100)/10];
        Down_DisBuff[9] = Down_Digital_Buff[(D_Volume%10)/1];
        Down_DisBuff[5] |= P10_Symbol;
      }
      else if(D_Volume < 10000000 )
      {
        Down_DisBuff[3] = Down_Digital_Buff[(D_Volume%10000000)/1000000];
        Down_DisBuff[4] = Down_Digital_Buff[(D_Volume%1000000)/100000];
        Down_DisBuff[5] = Down_Digital_Buff[(D_Volume%100000)/10000];
        Down_DisBuff[6] = Down_Digital_Buff[(D_Volume%10000)/1000];
        Down_DisBuff[7] = Down_Digital_Buff[(D_Volume%1000)/100];
        Down_DisBuff[8] = Down_Digital_Buff[(D_Volume%100)/10];
        Down_DisBuff[9] = Down_Digital_Buff[(D_Volume%10)/1];
        Down_DisBuff[5] |= P10_Symbol;
      }
      else if(D_Volume < 100000000 )
      {
        Down_DisBuff[3] = Down_Digital_Buff[(D_Volume%100000000)/10000000];
        Down_DisBuff[4] = Down_Digital_Buff[(D_Volume%10000000)/1000000];
        Down_DisBuff[5] = Down_Digital_Buff[(D_Volume%1000000)/100000];
        Down_DisBuff[6] = Down_Digital_Buff[(D_Volume%100000)/10000];
        Down_DisBuff[7] = Down_Digital_Buff[(D_Volume%10000)/1000];
        Down_DisBuff[8] = Down_Digital_Buff[(D_Volume%1000)/100];
        Down_DisBuff[9] = Down_Digital_Buff[(D_Volume%100)/10];
        Down_DisBuff[7] |= P8_Symbol;
      }
      else if(D_Volume < 1000000000 )
      {
        Down_DisBuff[3] = Down_Digital_Buff[(D_Volume%1000000000)/100000000];
        Down_DisBuff[4] = Down_Digital_Buff[(D_Volume%100000000)/10000000];
        Down_DisBuff[5] = Down_Digital_Buff[(D_Volume%10000000)/1000000];
        Down_DisBuff[6] = Down_Digital_Buff[(D_Volume%1000000)/100000];
        Down_DisBuff[7] = Down_Digital_Buff[(D_Volume%100000)/10000];
        Down_DisBuff[8] = Down_Digital_Buff[(D_Volume%10000)/1000];
        Down_DisBuff[9] = Down_Digital_Buff[(D_Volume%1000)/100];
        Down_DisBuff[4] |= P11_Symbol;
      }
      else
      {
        Down_DisBuff[3] = Down_Digital_Buff[D_Volume/1000000000];
        Down_DisBuff[4] = Down_Digital_Buff[(D_Volume%1000000000)/100000000];
        Down_DisBuff[5] = Down_Digital_Buff[(D_Volume%100000000)/10000000];
        Down_DisBuff[6] = Down_Digital_Buff[(D_Volume%10000000)/1000000];
        Down_DisBuff[7] = Down_Digital_Buff[(D_Volume%1000000)/100000];
        Down_DisBuff[8] = Down_Digital_Buff[(D_Volume%100000)/10000];
        Down_DisBuff[9] = Down_Digital_Buff[(D_Volume%10000)/1000];
        Down_DisBuff[9] |= P6_Symbol;
      }
  }
  CSB_LCD_Start();
  if(CSB_LCD_SendByte(LSD_001D_ADD)==FALSE)
  {
    CSB_LCD_Stop();
    return FALSE;
  }
  CSB_LCD_SendByte(Down_Start_ADD);
  for(LCD_DisVolume_i=0;LCD_DisVolume_i<10;LCD_DisVolume_i++)
  {
    CSB_LCD_SendByte(Down_DisBuff[LCD_DisVolume_i]);
  }
  CSB_LCD_Stop();

  return TRUE;
}
unsigned char CSB_LCD_DispStep(unsigned char S_T,unsigned char S_Num,unsigned long S_VOL)
{
  unsigned char LCD_DisVolume_i;
  for(LCD_DisVolume_i = 0;LCD_DisVolume_i<10;LCD_DisVolume_i++)
  {
    Down_DisBuff[LCD_DisVolume_i] = 0;
  }
  if(S_T)
  {
     Down_DisBuff[3] = Down_Digital_Buff[S_Num%10];
     Down_DisBuff[2] = JieTi_Symbol | QiLiang_Symbol;
  }
  else
  {
    Down_DisBuff[3] = Down_Digital_Buff[S_Num%10];
    Down_DisBuff[2] = JieTi_Symbol | DanJia_Symbol;
  }  
  for(LCD_DisVolume_i = 4;LCD_DisVolume_i<10;LCD_DisVolume_i++)
  {
    Down_DisBuff[LCD_DisVolume_i] = Down_Link_Symbol;
  }
  if(S_VOL < 100000 )
  {
    Down_DisBuff[5] = Down_Digital_Buff[(S_VOL%100000)/10000];
    Down_DisBuff[6] = Down_Digital_Buff[(S_VOL%10000)/1000];
    Down_DisBuff[7] = Down_Digital_Buff[(S_VOL%1000)/100];
    Down_DisBuff[8] = Down_Digital_Buff[(S_VOL%100)/10];
    Down_DisBuff[9] = Down_Digital_Buff[(S_VOL%10)/1];
    Down_DisBuff[5] |= P10_Symbol;
  }
  else if(S_VOL < 1000000 )
  {
    Down_DisBuff[5] = Down_Digital_Buff[(S_VOL%1000000)/100000];
    Down_DisBuff[6] = Down_Digital_Buff[(S_VOL%100000)/10000];
    Down_DisBuff[7] = Down_Digital_Buff[(S_VOL%10000)/1000];
    Down_DisBuff[8] = Down_Digital_Buff[(S_VOL%1000)/100];
    Down_DisBuff[9] = Down_Digital_Buff[(S_VOL%100)/10];
    Down_DisBuff[7] |= P8_Symbol;
  }
  else if(S_VOL < 10000000 )
  {
    Down_DisBuff[5] = Down_Digital_Buff[(S_VOL%10000000)/1000000];
    Down_DisBuff[6] = Down_Digital_Buff[(S_VOL%1000000)/100000];
    Down_DisBuff[7] = Down_Digital_Buff[(S_VOL%100000)/10000];
    Down_DisBuff[8] = Down_Digital_Buff[(S_VOL%10000)/1000];
    Down_DisBuff[9] = Down_Digital_Buff[(S_VOL%1000)/100];
    Down_DisBuff[4] |= P11_Symbol;
  }
  else if(S_VOL < 100000000 )
  {
    Down_DisBuff[5] = Down_Digital_Buff[(S_VOL%100000000)/10000000];
    Down_DisBuff[6] = Down_Digital_Buff[(S_VOL%10000000)/1000000];
    Down_DisBuff[7] = Down_Digital_Buff[(S_VOL%1000000)/100000];
    Down_DisBuff[8] = Down_Digital_Buff[(S_VOL%100000)/10000];
    Down_DisBuff[9] = Down_Digital_Buff[(S_VOL%10000)/1000];
    Down_DisBuff[9] |= P6_Symbol;
  }
  else
  {
    Down_DisBuff[5] = Down_Digital_Buff[(S_VOL%1000000000)/100000000];
    Down_DisBuff[6] = Down_Digital_Buff[(S_VOL%100000000)/10000000];
    Down_DisBuff[7] = Down_Digital_Buff[(S_VOL%10000000)/1000000];
    Down_DisBuff[8] = Down_Digital_Buff[(S_VOL%1000000)/100000];
    Down_DisBuff[9] = Down_Digital_Buff[(S_VOL%100000)/10000];
  }
  CSB_LCD_Start();
  if(CSB_LCD_SendByte(LSD_001D_ADD)==FALSE)
  {
    CSB_LCD_Stop();
    return FALSE;
  }
  CSB_LCD_SendByte(Down_Start_ADD);
  for(LCD_DisVolume_i=0;LCD_DisVolume_i<10;LCD_DisVolume_i++)
  {
    CSB_LCD_SendByte(Down_DisBuff[LCD_DisVolume_i]);
  }
  CSB_LCD_Stop();

  return TRUE;
}
/**********************************************
           Down---显示电池容量
***********************************************/
unsigned char CSB_LCD_BatCapacity(unsigned char D_Bat)
{
  Status_DisBuff &=0xF0;
  switch(D_Bat)
  {
  case 0:
    Status_DisBuff |=0x01;
    break;
  case 1:
    Status_DisBuff |=0x01 + 0x08;
    break;
  case 2:
    Status_DisBuff |=0x01 + 0x08 + 0x04;
    break;
  case 3:
    Status_DisBuff |=0x01 + 0x02 + 0x04 + 0x08;
    break;
  default:
    break;
  }
  CSB_LCD_Start();
  if(CSB_LCD_SendByte(LSD_001D_ADD)==FALSE)
  {
    CSB_LCD_Stop();
    return FALSE;
  }
  CSB_LCD_SendByte(20);
  CSB_LCD_SendByte(Status_DisBuff);
  CSB_LCD_Stop();
  return TRUE;
}
/**********************************************
           Down---显示阀门开关
***********************************************/
unsigned char CSB_LCD_Valve(unsigned char D_Valve)
{
  Status_DisBuff &=0x1F;
  if(D_Valve)
  {
    Status_DisBuff |=0x80;
  }
  else
  {
    Status_DisBuff |=0x40;
  }
  CSB_LCD_Start();
  if(CSB_LCD_SendByte(LSD_001D_ADD)==FALSE)
  {
    CSB_LCD_Stop();
    return FALSE;
  }
  CSB_LCD_SendByte(20);
  CSB_LCD_SendByte(Status_DisBuff);
  CSB_LCD_Stop();
  return TRUE;
}
/**********************************************
           Down---显示倒计时
***********************************************/
/*unsigned char CSB_LCD_UnderVoltage(unsigned int D_Hour,unsigned int D_Min,unsigned int D_Sec)
{
  unsigned char LCD_UnderVoltage_i;
  Down_DisBuff[0] = 0;
  Down_DisBuff[1] = 0;
  Down_DisBuff[2] = 0;
  Down_DisBuff[3] = 0;
  Down_DisBuff[4] = Down_Digital_Buff[D_Hour/10];
  Down_DisBuff[5] = Down_Digital_Buff[D_Hour%10];

  Down_DisBuff[6] = Down_Digital_Buff[D_Min/10];
  Down_DisBuff[7] = Down_Digital_Buff[D_Min%10];

  Down_DisBuff[8] = Down_Digital_Buff[D_Sec/10];
  Down_DisBuff[9] = Down_Digital_Buff[D_Sec%10];

  Down_DisBuff[8] |= P7_Symbol;
  Down_DisBuff[6] |= P9_Symbol;
  Down_DisBuff[5] |= P10_Symbol;
  Down_DisBuff[4] |= P11_Symbol;

  CSB_LCD_Start();
  if(CSB_LCD_SendByte(LSD_001D_ADD)==FALSE)
  {
    CSB_LCD_Stop();
    return FALSE;
  }
  CSB_LCD_SendByte(Down_Start_ADD);
  for(LCD_UnderVoltage_i=0;LCD_UnderVoltage_i<11;LCD_UnderVoltage_i++)
  {
    CSB_LCD_SendByte(Down_DisBuff[LCD_UnderVoltage_i]);
  }
  CSB_LCD_Stop();

  CSB_LCD_DisQianYa(1);

  CSB_LCD_DisDanWei(0,0);
  return TRUE;
}*/
/**********************************************
           Down---Err
***********************************************/
unsigned char CSB_LCD_DisDOWNErr(unsigned char D_DOWNErr)
{
  unsigned char LCD_DisDOWNErr_i;
  for(LCD_DisDOWNErr_i = 0;LCD_DisDOWNErr_i<10;LCD_DisDOWNErr_i++)
  {
    Down_DisBuff[LCD_DisDOWNErr_i] = 0;
  }
  Down_DisBuff[3] = Down_Letter_E;
  Down_DisBuff[4] = Down_Letter_r;
  Down_DisBuff[5] = Down_Letter_r;

  Down_DisBuff[6] = Down_Link_Symbol;
  Down_DisBuff[7] = Down_Link_Symbol;

  if(D_DOWNErr>0)
  {
      Down_DisBuff[8] = Down_Digital_Buff[D_DOWNErr/10];
      Down_DisBuff[9] = Down_Digital_Buff[D_DOWNErr%10];
  }
  else
  {
      Down_DisBuff[8] = CSB_LCD_CHAR_n;
      Down_DisBuff[9] = CSB_LCD_CHAR_o;
  }

  CSB_LCD_Start();
  if(CSB_LCD_SendByte(LSD_001D_ADD)==FALSE)
  {
   CSB_LCD_Stop();
    return FALSE;
  }
  CSB_LCD_SendByte(Down_Start_ADD);
  for(LCD_DisDOWNErr_i=0;LCD_DisDOWNErr_i<10;LCD_DisDOWNErr_i++)
  {
    CSB_LCD_SendByte(Down_DisBuff[LCD_DisDOWNErr_i]);
  }
  CSB_LCD_Stop();
  return TRUE;
}
/**********************************************
           Down---显示欠压符号
***********************************************/
/*unsigned char CSB_LCD_DisQianYa(unsigned char D_QianYa)
{
  if(D_QianYa)
  {
    Status_DisBuff |=QianYa_Symbol;
  }
  else
  {
    Status_DisBuff &=~QianYa_Symbol;
  }
  CSB_LCD_Start();
  if(CSB_LCD_SendByte(LSD_001D_ADD)==FALSE)
  {
    CSB_LCD_Stop();
    return FALSE;
  }
  CSB_LCD_SendByte(20);
  CSB_LCD_SendByte(Status_DisBuff);
  CSB_LCD_Stop();
  return TRUE;
}*/
/**********************************************
       Down---显示金额或M3或者取消显示
***********************************************/
/*unsigned char CSB_LCD_DisDanWei(unsigned char D_DanWei,unsigned char D_Switch)
{
  if(D_Switch)
  {
    if(D_DanWei)
    {
      Down_DisBuff[0]=JinE_DanWei;
    }
    else
    {
      Down_DisBuff[0]=QiLiang_DanWei;
    }
  }
  else
  {
    Down_DisBuff[0]=0;
  }
  CSB_LCD_Start();
  if(CSB_LCD_SendByte(LSD_001D_ADD)==FALSE)
  {
    CSB_LCD_Stop();
    return FALSE;
  }
  CSB_LCD_SendByte(0);
  CSB_LCD_SendByte(Down_DisBuff[0]);
  CSB_LCD_Stop();
  return TRUE;
}*/
/**********************************************
           Down---显示某一位字符
***********************************************/
unsigned char CSB_LCD_DispBitNum(unsigned char D_Position,unsigned char D_Num)
{
  CSB_LCD_Start();
  if(CSB_LCD_SendByte(LSD_001D_ADD)==FALSE)
  {
    CSB_LCD_Stop();
    return FALSE;
  }
  CSB_LCD_SendByte(D_Position*2+4);
  CSB_LCD_SendByte(D_Num);
  CSB_LCD_Stop();
  return TRUE;
}
/*unsigned char CSB_LCD_DispMeterID(unsigned char *D_ID)
{
    unsigned char CSB_LCD_DispMeterID_i;
    for(CSB_LCD_DispMeterID_i=0;CSB_LCD_DispMeterID_i<10;CSB_LCD_DispMeterID_i++)
    {
        Down_DisBuff[CSB_LCD_DispMeterID_i]=0;
    }
    CSB_LCD_DispMeterID_i=*D_ID++;
    Down_DisBuff[8] = Down_Digital_Buff[CSB_LCD_DispMeterID_i>>4];
    Down_DisBuff[9] = Down_Digital_Buff[CSB_LCD_DispMeterID_i&0x0F];
    CSB_LCD_DispMeterID_i=*D_ID++;
    Down_DisBuff[6] = Down_Digital_Buff[CSB_LCD_DispMeterID_i>>4];
    Down_DisBuff[7] = Down_Digital_Buff[CSB_LCD_DispMeterID_i&0x0F];
    CSB_LCD_DispMeterID_i=*D_ID++;
    Down_DisBuff[4] = Down_Digital_Buff[CSB_LCD_DispMeterID_i>>4];
    Down_DisBuff[5] = Down_Digital_Buff[CSB_LCD_DispMeterID_i&0x0F];
    CSB_LCD_DispMeterID_i=*D_ID++;
    Down_DisBuff[3] = Down_Digital_Buff[CSB_LCD_DispMeterID_i&0x0F];
   CSB_LCD_Start();
   if(CSB_LCD_SendByte(LSD_001D_ADD)==FALSE)
   {
     CSB_LCD_Stop();
     return FALSE;
   }
   CSB_LCD_SendByte(Down_Start_ADD);
   for(CSB_LCD_DispMeterID_i=0;CSB_LCD_DispMeterID_i<10;CSB_LCD_DispMeterID_i++)
   {
     CSB_LCD_SendByte(Down_DisBuff[CSB_LCD_DispMeterID_i]);
   }
   CSB_LCD_Stop();
   return TRUE;
}*/
unsigned char CSB_LCD_DispKPMeterID(unsigned char M_Num,unsigned char *M_ID)
{
   unsigned char CSB_LCD_DispMeterID_i;
   for(CSB_LCD_DispMeterID_i=0;CSB_LCD_DispMeterID_i<10;CSB_LCD_DispMeterID_i++)
   {
       Down_DisBuff[CSB_LCD_DispMeterID_i]=0;
   }
   Down_DisBuff[3] = CSB_LCD_CHAR_d;
   Down_DisBuff[4] = Down_Digital_Buff[M_Num%10];
   Down_DisBuff[5] = Down_Link_Symbol;
   CSB_LCD_DispMeterID_i=*M_ID++;
   Down_DisBuff[6] = Down_Digital_Buff[CSB_LCD_DispMeterID_i>>4];
   Down_DisBuff[7] = Down_Digital_Buff[CSB_LCD_DispMeterID_i&0x0F];
   CSB_LCD_DispMeterID_i=*M_ID++;
   Down_DisBuff[8] = Down_Digital_Buff[CSB_LCD_DispMeterID_i>>4];
   Down_DisBuff[9] = Down_Digital_Buff[CSB_LCD_DispMeterID_i&0x0F];

   CSB_LCD_Start();
   if(CSB_LCD_SendByte(LSD_001D_ADD)==FALSE)
   {
     CSB_LCD_Stop();
     return FALSE;
   }
   CSB_LCD_SendByte(Down_Start_ADD);
   for(CSB_LCD_DispMeterID_i=0;CSB_LCD_DispMeterID_i<10;CSB_LCD_DispMeterID_i++)
   {
     CSB_LCD_SendByte(Down_DisBuff[CSB_LCD_DispMeterID_i]);
   }
   CSB_LCD_Stop();
   return TRUE;
}
/*unsigned char CSB_LCD_DispH_IP(unsigned char D_H,unsigned char D_L)
{
    unsigned char CSB_LCD_DispHIP_i;
    for(CSB_LCD_DispHIP_i=0;CSB_LCD_DispHIP_i<10;CSB_LCD_DispHIP_i++)
    {
        Down_DisBuff[CSB_LCD_DispHIP_i]=0;
    }
    Down_DisBuff[3] = Down_Digital_Buff[(D_H%1000)/100];
    Down_DisBuff[4] = Down_Digital_Buff[(D_H%100)/10];
    Down_DisBuff[5] = Down_Digital_Buff[(D_H%10)/1];
    Down_DisBuff[5] |= P10_Symbol;

    Down_DisBuff[6] = Down_Digital_Buff[(D_L%1000)/100];
    Down_DisBuff[7] = Down_Digital_Buff[(D_L%100)/10];
    Down_DisBuff[8] = Down_Digital_Buff[(D_L%10)/1];
    Down_DisBuff[9] |=P6_Symbol;
    CSB_LCD_Start();
    if(CSB_LCD_SendByte(LSD_001D_ADD)==FALSE)
    {
      CSB_LCD_Stop();
      return FALSE;
    }
    CSB_LCD_SendByte(Down_Start_ADD);
    for(CSB_LCD_DispHIP_i=0;CSB_LCD_DispHIP_i<10;CSB_LCD_DispHIP_i++)
    {
      CSB_LCD_SendByte(Down_DisBuff[CSB_LCD_DispHIP_i]);
    }
    CSB_LCD_Stop();
    return TRUE;
}*/
unsigned char CSB_LCD_DispKPPD(unsigned char *P_Data)
{
    unsigned char CSB_LCD_DispHIP_i;
    for(CSB_LCD_DispHIP_i=0;CSB_LCD_DispHIP_i<10;CSB_LCD_DispHIP_i++)
    {
        Down_DisBuff[CSB_LCD_DispHIP_i]=0;
    }
    Down_DisBuff[3] = CSB_LCD_CHAR_F;
    CSB_LCD_DispHIP_i=*P_Data++;
    Down_DisBuff[4] = Down_Digital_Buff[(CSB_LCD_DispHIP_i>>4)&0x0F];
    Down_DisBuff[5] = Down_Digital_Buff[CSB_LCD_DispHIP_i&0x0F];
    CSB_LCD_DispHIP_i=*P_Data++;
    Down_DisBuff[6] = Down_Digital_Buff[(CSB_LCD_DispHIP_i>>4)&0x0F];
    Down_DisBuff[7] = Down_Digital_Buff[CSB_LCD_DispHIP_i&0x0F];
    Down_DisBuff[7] |=P8_Symbol;
    CSB_LCD_DispHIP_i=*P_Data;
    Down_DisBuff[8] = Down_Digital_Buff[(CSB_LCD_DispHIP_i>>4)&0x0F];
    Down_DisBuff[9] = Down_Digital_Buff[CSB_LCD_DispHIP_i&0x0F];
    CSB_LCD_Start();
    if(CSB_LCD_SendByte(LSD_001D_ADD)==FALSE)
    {
      CSB_LCD_Stop();
      return FALSE;
    }
    CSB_LCD_SendByte(Down_Start_ADD);
    for(CSB_LCD_DispHIP_i=0;CSB_LCD_DispHIP_i<10;CSB_LCD_DispHIP_i++)
    {
      CSB_LCD_SendByte(Down_DisBuff[CSB_LCD_DispHIP_i]);
    }
    CSB_LCD_Stop();
    return TRUE;
}
unsigned char CSB_LCD_DispKPNetID(unsigned char *N_Data)
{
    unsigned char CSB_LCD_DispHIP_i;
    for(CSB_LCD_DispHIP_i=0;CSB_LCD_DispHIP_i<10;CSB_LCD_DispHIP_i++)
    {
        Down_DisBuff[CSB_LCD_DispHIP_i]=0;
    }
    Down_DisBuff[3] = CSB_LCD_CHAR_n;
    Down_DisBuff[4] = CSB_LCD_CHAR_d;
    Down_DisBuff[5] = Down_Link_Symbol;
    CSB_LCD_DispHIP_i=*N_Data++;
    Down_DisBuff[6] = Down_Digital_Buff[(CSB_LCD_DispHIP_i>>4)&0x0F];
    Down_DisBuff[7] = Down_Digital_Buff[(CSB_LCD_DispHIP_i)&0x0F];
    CSB_LCD_DispHIP_i=*N_Data++;
    Down_DisBuff[8] = Down_Digital_Buff[(CSB_LCD_DispHIP_i>>4)&0x0F];
    Down_DisBuff[9] = Down_Digital_Buff[(CSB_LCD_DispHIP_i)&0x0F];
    CSB_LCD_Start();
    if(CSB_LCD_SendByte(LSD_001D_ADD)==FALSE)
    {
      CSB_LCD_Stop();
      return FALSE;
    }
    CSB_LCD_SendByte(Down_Start_ADD);
    for(CSB_LCD_DispHIP_i=0;CSB_LCD_DispHIP_i<10;CSB_LCD_DispHIP_i++)
    {
      CSB_LCD_SendByte(Down_DisBuff[CSB_LCD_DispHIP_i]);
    }
    CSB_LCD_Stop();
    return TRUE;
}
/*unsigned char CSB_LCD_DispL_IP(unsigned char D_H,unsigned char D_L)
{
    unsigned char CSB_LCD_DispHIP_i;
    for(CSB_LCD_DispHIP_i=0;CSB_LCD_DispHIP_i<10;CSB_LCD_DispHIP_i++)
    {
        Down_DisBuff[CSB_LCD_DispHIP_i]=0;
    }
    Down_DisBuff[3] = Down_Digital_Buff[(D_H%1000)/100];
    Down_DisBuff[4] = Down_Digital_Buff[(D_H%100)/10];
    Down_DisBuff[5] = Down_Digital_Buff[(D_H%10)/1];
    Down_DisBuff[5] |= P10_Symbol;

    Down_DisBuff[6] = Down_Digital_Buff[(D_L%1000)/100];
    Down_DisBuff[7] = Down_Digital_Buff[(D_L%100)/10];
    Down_DisBuff[8] = Down_Digital_Buff[(D_L%10)/1];
    CSB_LCD_Start();
    if(CSB_LCD_SendByte(LSD_001D_ADD)==FALSE)
    {
      CSB_LCD_Stop();
      return FALSE;
    }
    CSB_LCD_SendByte(Down_Start_ADD);
    for(CSB_LCD_DispHIP_i=0;CSB_LCD_DispHIP_i<10;CSB_LCD_DispHIP_i++)
    {
      CSB_LCD_SendByte(Down_DisBuff[CSB_LCD_DispHIP_i]);
    }
    CSB_LCD_Stop();
    return TRUE;
}*/
/*unsigned char CSB_LCD_DispPort(unsigned short D_P)
{
    unsigned char CSB_LCD_DispPort_i;
    for(CSB_LCD_DispPort_i=0;CSB_LCD_DispPort_i<10;CSB_LCD_DispPort_i++)
    {
        Down_DisBuff[CSB_LCD_DispPort_i]=0;
    }
    Down_DisBuff[3] = CSB_LCD_CHAR_P;
    if(D_P<10000)
    {
        Down_DisBuff[4] = CSB_LCD_CHAR_SEPARATE;
        Down_DisBuff[5] = CSB_LCD_CHAR_SEPARATE;
        Down_DisBuff[6] = Down_Digital_Buff[(D_P%10000)/1000];
        Down_DisBuff[7] = Down_Digital_Buff[(D_P%1000)/100];
        Down_DisBuff[8] = Down_Digital_Buff[(D_P%100)/10];
        Down_DisBuff[9] = Down_Digital_Buff[(D_P%10)/1];
    }
    else
    {
        Down_DisBuff[4] = CSB_LCD_CHAR_SEPARATE;
        Down_DisBuff[5] = Down_Digital_Buff[(D_P%100000)/10000];
        Down_DisBuff[6] = Down_Digital_Buff[(D_P%10000)/1000];
        Down_DisBuff[7] = Down_Digital_Buff[(D_P%1000)/100];
        Down_DisBuff[8] = Down_Digital_Buff[(D_P%100)/10];
        Down_DisBuff[9] = Down_Digital_Buff[(D_P%10)/1];
    }

    CSB_LCD_Start();
    if(CSB_LCD_SendByte(LSD_001D_ADD)==FALSE)
    {
      CSB_LCD_Stop();
      return FALSE;
    }
    CSB_LCD_SendByte(Down_Start_ADD);
    for(CSB_LCD_DispPort_i=0;CSB_LCD_DispPort_i<10;CSB_LCD_DispPort_i++)
    {
      CSB_LCD_SendByte(Down_DisBuff[CSB_LCD_DispPort_i]);
    }
    CSB_LCD_Stop();
    return TRUE;
}*/
unsigned char CSB_LCD_DispDate(unsigned char D_Year,unsigned char D_Month,unsigned char D_Day)
{
    unsigned char CSB_LCD_DispDate_i;
    for(CSB_LCD_DispDate_i=0;CSB_LCD_DispDate_i<10;CSB_LCD_DispDate_i++)
    {
        Down_DisBuff[CSB_LCD_DispDate_i]=0;
    }
    Down_DisBuff[4] = Down_Digital_Buff[D_Year>>4];
    Down_DisBuff[5] = Down_Digital_Buff[D_Year&0x0F];

    Down_DisBuff[6] = Down_Digital_Buff[D_Month>>4];
    Down_DisBuff[7] = Down_Digital_Buff[D_Month&0x0F];

    Down_DisBuff[8] = Down_Digital_Buff[D_Day>>4];
    Down_DisBuff[9] = Down_Digital_Buff[D_Day&0x0F];

    CSB_LCD_Start();
    if(CSB_LCD_SendByte(LSD_001D_ADD)==FALSE)
    {
      CSB_LCD_Stop();
      return FALSE;
    }
    CSB_LCD_SendByte(Down_Start_ADD);
    for(CSB_LCD_DispDate_i=0;CSB_LCD_DispDate_i<10;CSB_LCD_DispDate_i++)
    {
      CSB_LCD_SendByte(Down_DisBuff[CSB_LCD_DispDate_i]);
    }
    CSB_LCD_Stop();
    return TRUE;
}
unsigned char CSB_LCD_DispTime(unsigned char D_Hour,unsigned char D_Min,unsigned char D_Sec)
{
    unsigned char CSB_LCD_DispTime_i;
    for(CSB_LCD_DispTime_i=0;CSB_LCD_DispTime_i<10;CSB_LCD_DispTime_i++)
    {
        Down_DisBuff[CSB_LCD_DispTime_i]=0;
    }
    Down_DisBuff[4] = Down_Digital_Buff[D_Hour>>4];
    Down_DisBuff[4] |=P11_Symbol;
    Down_DisBuff[5] = Down_Digital_Buff[D_Hour&0x0F];
    Down_DisBuff[5] |=P10_Symbol;

    Down_DisBuff[6] = Down_Digital_Buff[D_Min>>4];
    Down_DisBuff[6] |=P9_Symbol;
    Down_DisBuff[7] = Down_Digital_Buff[D_Min&0x0F];

    Down_DisBuff[8] = Down_Digital_Buff[D_Sec>>4];
    Down_DisBuff[8] |=P7_Symbol;
    Down_DisBuff[9] = Down_Digital_Buff[D_Sec&0x0F];

    CSB_LCD_Start();
    if(CSB_LCD_SendByte(LSD_001D_ADD)==FALSE)
    {
      CSB_LCD_Stop();
      return FALSE;
    }
    CSB_LCD_SendByte(Down_Start_ADD);
    for(CSB_LCD_DispTime_i=0;CSB_LCD_DispTime_i<10;CSB_LCD_DispTime_i++)
    {
      CSB_LCD_SendByte(Down_DisBuff[CSB_LCD_DispTime_i]);
    }
    CSB_LCD_Stop();
    return TRUE;
}
/*unsigned char CSB_LCD_DispSignal(unsigned char D_Signal)
{
    unsigned char D_Signal_i;
    for(D_Signal_i=0;D_Signal_i<10;D_Signal_i++)
    {
        Down_DisBuff[D_Signal_i]=0;
    }
    Down_DisBuff[4] = CSB_LCD_CHAR_C;
    Down_DisBuff[5] = CSB_LCD_CHAR_S;
    Down_DisBuff[6] = Down_Digital_Buff[9];
    Down_DisBuff[7] = CSB_LCD_CHAR_SEPARATE;
    Down_DisBuff[8] = CSB_LCD_CHAR_SEPARATE;
    if(D_Signal <10)
    {
        Down_DisBuff[9] = Down_Digital_Buff[D_Signal%10];
    }
    else
    {
        Down_DisBuff[8] = Down_Digital_Buff[(D_Signal%100)/10];
        Down_DisBuff[9] = Down_Digital_Buff[D_Signal%10];
    }

    CSB_LCD_Start();
    if(CSB_LCD_SendByte(LSD_001D_ADD)==FALSE)
    {
      CSB_LCD_Stop();
      return FALSE;
    }
    CSB_LCD_SendByte(Down_Start_ADD);
    for(D_Signal_i=0;D_Signal_i<10;D_Signal_i++)
    {
      CSB_LCD_SendByte(Down_DisBuff[D_Signal_i]);
    }
    CSB_LCD_Stop();
    return TRUE;
}*/
/*unsigned char CSB_LCD_DispVersion(unsigned char D_Version,unsigned char D_Version1,unsigned char D_Version2)
{
    unsigned char D_Version_i;
    for(D_Version_i=0;D_Version_i<10;D_Version_i++)
    {
        Down_DisBuff[D_Version_i]=0;
    }
    Down_DisBuff[4] = CSB_LCD_CHAR_U;
    Down_DisBuff[5] = Down_Digital_Buff[(D_Version%10)/1];
    Down_DisBuff[6] = CSB_LCD_CHAR_SEPARATE;
    Down_DisBuff[7] = Down_Digital_Buff[(D_Version1%10)/1];
    Down_DisBuff[8] = CSB_LCD_CHAR_SEPARATE;
    if(D_Version2<10)
    {
        Down_DisBuff[9] = Down_Digital_Buff[(D_Version2%10)/1];
    }
    else
    {
        Down_DisBuff[8] = Down_Digital_Buff[(D_Version2%100)/10];
        Down_DisBuff[9] = Down_Digital_Buff[(D_Version2%10)/1];
    }
    CSB_LCD_Start();
    if(CSB_LCD_SendByte(LSD_001D_ADD)==FALSE)
    {
      CSB_LCD_Stop();
      return FALSE;
    }
    CSB_LCD_SendByte(Down_Start_ADD);
    for(D_Version_i=0;D_Version_i<10;D_Version_i++)
    {
      CSB_LCD_SendByte(Down_DisBuff[D_Version_i]);
    }
    CSB_LCD_Stop();
    return TRUE;
}*/
unsigned char CSB_LCD_HardVision(unsigned char *H_Version)
{
    unsigned char D_Version_i;
    for(D_Version_i=0;D_Version_i<10;D_Version_i++)
    {
        Down_DisBuff[D_Version_i]=0;
    }
    Down_DisBuff[3] = CSB_LCD_CHAR_A;

    D_Version_i=*H_Version++;
    Down_DisBuff[5] = Down_Digital_Buff[D_Version_i&0x0F];
    Down_DisBuff[6] = CSB_LCD_CHAR_SEPARATE;
    D_Version_i=*H_Version;
    Down_DisBuff[7] = Down_Digital_Buff[(D_Version_i>>4)&0x0F];
    Down_DisBuff[8] = CSB_LCD_CHAR_SEPARATE;
    Down_DisBuff[9] = Down_Digital_Buff[D_Version_i&0x0F];
    CSB_LCD_Start();
    if(CSB_LCD_SendByte(LSD_001D_ADD)==FALSE)
    {
      CSB_LCD_Stop();
      return FALSE;
    }
    CSB_LCD_SendByte(Down_Start_ADD);
    for(D_Version_i=0;D_Version_i<10;D_Version_i++)
    {
      CSB_LCD_SendByte(Down_DisBuff[D_Version_i]);
    }
    CSB_LCD_Stop();
    return TRUE;
}
unsigned char CSB_LCD_SoftVision(unsigned char *S_Version)
{
    unsigned char D_Version_i;
    for(D_Version_i=0;D_Version_i<10;D_Version_i++)
    {
        Down_DisBuff[D_Version_i]=0;
    }
    Down_DisBuff[3] = Down_Digital_Buff[8];

    D_Version_i=*S_Version++;
    Down_DisBuff[5] = Down_Digital_Buff[D_Version_i&0x0F];
    Down_DisBuff[6] = CSB_LCD_CHAR_SEPARATE;
    D_Version_i=*S_Version;
    Down_DisBuff[7] = Down_Digital_Buff[(D_Version_i)>>4&0x0F];
    Down_DisBuff[8] = CSB_LCD_CHAR_SEPARATE;
    Down_DisBuff[9] = Down_Digital_Buff[D_Version_i&0x0F];

    CSB_LCD_Start();
    if(CSB_LCD_SendByte(LSD_001D_ADD)==FALSE)
    {
      CSB_LCD_Stop();
      return FALSE;
    }
    CSB_LCD_SendByte(Down_Start_ADD);
    for(D_Version_i=0;D_Version_i<10;D_Version_i++)
    {
      CSB_LCD_SendByte(Down_DisBuff[D_Version_i]);
    }
    CSB_LCD_Stop();
    return TRUE;
}
/*unsigned char CSB_LCD_DispSGDay(unsigned char D_SGDay,unsigned char D_SG1,unsigned char D_SG2)
{
    unsigned char CSB_LCD_DispSGDay_i;
    for(CSB_LCD_DispSGDay_i=0;CSB_LCD_DispSGDay_i<10;CSB_LCD_DispSGDay_i++)
    {
        Down_DisBuff[CSB_LCD_DispSGDay_i]=0;
    }
    Down_DisBuff[3] = Down_Digital_Buff[D_SGDay%10];
    Down_DisBuff[4] = CSB_LCD_CHAR_SEPARATE;
    Down_DisBuff[5] = Down_Digital_Buff[(D_SG1%100)/10];
    Down_DisBuff[6] = Down_Digital_Buff[D_SG1%10];
    Down_DisBuff[7] = CSB_LCD_CHAR_SEPARATE;
    Down_DisBuff[8] = Down_Digital_Buff[(D_SG2%100)/10];
    Down_DisBuff[9] = Down_Digital_Buff[D_SG2%10];

    CSB_LCD_Start();
    if(CSB_LCD_SendByte(LSD_001D_ADD)==FALSE)
    {
      CSB_LCD_Stop();
      return FALSE;
    }
    CSB_LCD_SendByte(Down_Start_ADD);
    for(CSB_LCD_DispSGDay_i=0;CSB_LCD_DispSGDay_i<10;CSB_LCD_DispSGDay_i++)
    {
      CSB_LCD_SendByte(Down_DisBuff[CSB_LCD_DispSGDay_i]);
    }
    CSB_LCD_Stop();
    return TRUE;
}*/
/*unsigned char CSB_LCD_DispKTPage(unsigned char KTPage)
{
    unsigned char CSB_LCD_DispKTPage_i;
    for(CSB_LCD_DispKTPage_i=0;CSB_LCD_DispKTPage_i<10;CSB_LCD_DispKTPage_i++)
    {
        Down_DisBuff[CSB_LCD_DispKTPage_i]=0;
    }
    Down_DisBuff[4] = CSB_LCD_CHAR_O;
    Down_DisBuff[5] = CSB_LCD_CHAR_P;
    Down_DisBuff[6] = CSB_LCD_CHAR_E;
    Down_DisBuff[7] = CSB_LCD_CHAR_N;
    Down_DisBuff[8] = CSB_LCD_CHAR_SEPARATE;
    Down_DisBuff[9] = Down_Digital_Buff[KTPage%10];
    CSB_LCD_Start();
    if(CSB_LCD_SendByte(LSD_001D_ADD)==FALSE)
    {
      CSB_LCD_Stop();
      return FALSE;
    }
    CSB_LCD_SendByte(Down_Start_ADD);
    for(CSB_LCD_DispKTPage_i=0;CSB_LCD_DispKTPage_i<10;CSB_LCD_DispKTPage_i++)
    {
      CSB_LCD_SendByte(Down_DisBuff[CSB_LCD_DispKTPage_i]);
    }
    CSB_LCD_Stop();
    return TRUE;
}*/
unsigned char CSB_LCD_DispMeterType(unsigned char D_Type)
{
    unsigned char CSB_LCD_DispMeterType_i;
    for(CSB_LCD_DispMeterType_i=0;CSB_LCD_DispMeterType_i<10;CSB_LCD_DispMeterType_i++)
    {
        Down_DisBuff[CSB_LCD_DispMeterType_i]=0;
    }
    Down_DisBuff[4] = CSB_LCD_CHAR_t;
    Down_DisBuff[5] = CSB_LCD_CHAR_P;
    Down_DisBuff[6] = CSB_LCD_CHAR_E;
    Down_DisBuff[7] = CSB_LCD_CHAR_SEPARATE;
    if(D_Type ==1)
    {
        Down_DisBuff[8] = Down_Digital_Buff[2];
        Down_DisBuff[9] = Down_Digital_Buff[5];
    }
    else if(D_Type ==2)
    {
        Down_DisBuff[8] = Down_Digital_Buff[4];
        Down_DisBuff[9] = Down_Digital_Buff[0];
    }
    else if(D_Type ==2)
    {
        Down_DisBuff[8] = Down_Digital_Buff[6];
        Down_DisBuff[9] = Down_Digital_Buff[0];
    }
    else if(D_Type ==2)
    {
        Down_DisBuff[8] = Down_Digital_Buff[1];
        Down_DisBuff[9] = Down_Digital_Buff[0];
    }
    else
    {
        Down_DisBuff[8] = Down_Digital_Buff[1];
        Down_DisBuff[9] = Down_Digital_Buff[6];
    }
    CSB_LCD_Start();
    if(CSB_LCD_SendByte(LSD_001D_ADD)==FALSE)
    {
      CSB_LCD_Stop();
      return FALSE;
    }
    CSB_LCD_SendByte(Down_Start_ADD);
    for(CSB_LCD_DispMeterType_i=0;CSB_LCD_DispMeterType_i<10;CSB_LCD_DispMeterType_i++)
    {
      CSB_LCD_SendByte(Down_DisBuff[CSB_LCD_DispMeterType_i]);
    }
    CSB_LCD_Stop();
    return TRUE;
}
/*unsigned char CSB_LCD_DispZDNum(unsigned long D_ZDNum)
{
    unsigned char CSB_LCD_DispZDNum_i;
    for(CSB_LCD_DispZDNum_i=0;CSB_LCD_DispZDNum_i<10;CSB_LCD_DispZDNum_i++)
    {
        Down_DisBuff[CSB_LCD_DispZDNum_i]=0;
    }
    Down_DisBuff[3] = Down_Digital_Buff[(D_ZDNum%10000000)/1000000];
    Down_DisBuff[4] = Down_Digital_Buff[(D_ZDNum%1000000)/100000];
    Down_DisBuff[5] = Down_Digital_Buff[(D_ZDNum%100000)/10000];
    Down_DisBuff[6] = Down_Digital_Buff[(D_ZDNum%10000)/1000];
    Down_DisBuff[7] = Down_Digital_Buff[(D_ZDNum%1000)/100];
    Down_DisBuff[8] = Down_Digital_Buff[(D_ZDNum%100)/10];
    Down_DisBuff[9] = Down_Digital_Buff[(D_ZDNum%10)/1];
    CSB_LCD_Start();
    if(CSB_LCD_SendByte(LSD_001D_ADD)==FALSE)
    {
      CSB_LCD_Stop();
      return FALSE;
    }
    CSB_LCD_SendByte(Down_Start_ADD);
    for(CSB_LCD_DispZDNum_i=0;CSB_LCD_DispZDNum_i<10;CSB_LCD_DispZDNum_i++)
    {
      CSB_LCD_SendByte(Down_DisBuff[CSB_LCD_DispZDNum_i]);
    }
    CSB_LCD_Stop();
    return TRUE;
}*/
