#include "ZDINA219.h"
unsigned char ZDINA219Buff[2];
unsigned char ZDINA219CurrentBuff[20];
unsigned short ZDINA219CurrentMidBuff[5];
void ZDINA219_IIC_Delay()
{
  unsigned char ZDINA219_IIC_Delay_i;
  for(ZDINA219_IIC_Delay_i=0;ZDINA219_IIC_Delay_i<50;ZDINA219_IIC_Delay_i++)
  {
    NOP();
  }
}

void ZDINA219_IIC_Init()
{
  ZDINA219_SCL(0);
  ZDINA219_SDA_OUT_Dir;
  ZDINA219_SDA(0);
}
void ZDINA219_IIC_Start()
{
  ZDINA219_SDA_OUT_Dir;
  ZDINA219_SDA(1);
  ZDINA219_SCL(1);
  ZDINA219_IIC_Delay();
  ZDINA219_SDA(0);
  ZDINA219_SCL(0);
}
void ZDINA219_IIC_Stop()
{
  ZDINA219_SDA_OUT_Dir;
  ZDINA219_SDA(0);
  ZDINA219_SCL(1);
  ZDINA219_IIC_Delay();
  ZDINA219_SDA(1);
  ZDINA219_SCL(0);
  ZDINA219_IIC_Delay();
  ZDINA219_SCL(1);
}
unsigned char ZDINA219_IIC_SendByte(unsigned char Data)
{
  unsigned char ZDINA219_IIC_SendByte_i;
  ZDINA219_SDA_OUT_Dir;
  ZDINA219_SCL(0); 
  for(ZDINA219_IIC_SendByte_i=0;ZDINA219_IIC_SendByte_i<8;ZDINA219_IIC_SendByte_i++)
  {
    if(Data&0x80)
    {
      ZDINA219_SDA(1);
    }
    else
    {
      ZDINA219_SDA(0);
    }
    ZDINA219_IIC_Delay();
    ZDINA219_SCL(1);
    ZDINA219_IIC_Delay();
    ZDINA219_SCL(0);
    Data=Data<<1;
  }
  ZDINA219_SDA_IN_Dir;
  ZDINA219_IIC_Delay();
  ZDINA219_SCL(1);
  ZDINA219_IIC_Delay();
  if(ZDINA219_SDA_InPut==0)
  {
    ZDINA219_SCL(0);
  } 
  else
  {
    ZDINA219_SCL(0);
  }
  ZDINA219_IIC_Delay();
  return TRUE;
}
unsigned char ZDINA219_IIC_SendBytes(unsigned char *Datas,unsigned char Len)
{
  unsigned char ZDINA219_IIC_SendBytes_i;
  for(ZDINA219_IIC_SendBytes_i=0;ZDINA219_IIC_SendBytes_i<Len;ZDINA219_IIC_SendBytes_i++)
  {
    ZDINA219_IIC_SendByte(*(Datas+ZDINA219_IIC_SendBytes_i));
  }
  return TRUE;
}
unsigned char ZDINA219_IIC_RevByte()
{
  unsigned char ZDINA219_IIC_RevByte_i,ZDINA219_IIC_RevByte_Data=0;
  ZDINA219_SDA_IN_Dir;
  ZDINA219_SCL(0);
  ZDINA219_IIC_Delay();
  for(ZDINA219_IIC_RevByte_i=0;ZDINA219_IIC_RevByte_i<8;ZDINA219_IIC_RevByte_i++)
  {
    ZDINA219_SCL(1);
    ZDINA219_IIC_Delay();
    if(ZDINA219_SDA_InPut)
    {
      ZDINA219_IIC_RevByte_Data |=0x80>>ZDINA219_IIC_RevByte_i;
    }
    ZDINA219_SCL(0);
    ZDINA219_IIC_Delay();
  }
  return ZDINA219_IIC_RevByte_Data;
}
void ZDINA219_IIC_RevBytes(unsigned char *Datas,unsigned char Len)
{
  unsigned char ZDINA219_IIC_RevBytes_i;
  for(ZDINA219_IIC_RevBytes_i=0;ZDINA219_IIC_RevBytes_i<Len;ZDINA219_IIC_RevBytes_i++)
  {
    *(Datas+ZDINA219_IIC_RevBytes_i)=ZDINA219_IIC_RevByte();
    if(ZDINA219_IIC_RevBytes_i<Len-1)
    {
      ZDINA219_SDA_OUT_Dir;
      ZDINA219_SDA(0);
      ZDINA219_IIC_Delay();
      ZDINA219_SCL(1);
      ZDINA219_IIC_Delay();
      ZDINA219_SCL(0);
    }
    else
    {
      ZDINA219_SDA_OUT_Dir;
      ZDINA219_SDA(1);
      ZDINA219_IIC_Delay();
      ZDINA219_SCL(1);
      ZDINA219_IIC_Delay();
      ZDINA219_SCL(0);
    }
  }
}
unsigned short ReadZD_Current()////write-0x80 Read-0x81
{
	unsigned char Read_Current_i;
	unsigned short Read_Current_midresult=0;
	unsigned short Read_Current_result=0;
  ZDINA219Buff[0]=0x07;
	ZDINA219Buff[1]=0x9f;
	
	ZDINA219_IIC_Start();
  ZDINA219_IIC_SendByte(0x80);		
	ZDINA219_IIC_SendByte(0);
	ZDINA219_IIC_SendBytes(ZDINA219Buff,2);
  ZDINA219_IIC_Stop();

	ZDINA219Buff[0]=0x10;  //10
	ZDINA219Buff[1]=0x00;  //00
  ZDINA219_IIC_Start();
  ZDINA219_IIC_SendByte(0x80);		
	ZDINA219_IIC_SendByte(5);
	ZDINA219_IIC_SendBytes(ZDINA219Buff,2);
  ZDINA219_IIC_Stop();

	for(Read_Current_i=0;Read_Current_i<10;Read_Current_i++)
	{
		Delay_ms(50);
		ZDINA219_IIC_Start();
	  ZDINA219_IIC_SendByte(0x80);	
		ZDINA219_IIC_SendByte(4);
		ZDINA219_IIC_Start();
		ZDINA219_IIC_SendByte(0x81);
		ZDINA219_IIC_RevBytes(ZDINA219CurrentBuff+Read_Current_i*2,2);
	  ZDINA219_IIC_Stop();
	}
	for(Read_Current_i=0;Read_Current_i<5;Read_Current_i++)
	{
		ZDINA219CurrentMidBuff[Read_Current_i] =(unsigned short)ZDINA219CurrentBuff[Read_Current_i*2]*0x100;
		ZDINA219CurrentMidBuff[Read_Current_i] +=ZDINA219CurrentBuff[Read_Current_i*2+1];
	}
	Read_Current_midresult = ZDINA219CurrentMidBuff[0];
	for(Read_Current_i=1;Read_Current_i<5;Read_Current_i++)
	{
		if(Read_Current_midresult > ZDINA219CurrentMidBuff[Read_Current_i])
		{
			Read_Current_midresult = ZDINA219CurrentMidBuff[Read_Current_i];
		}
	}
	Read_Current_result =Read_Current_midresult;
	return Read_Current_result;
}
