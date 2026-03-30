#include "BDINA219.h"
unsigned char BDINA219Buff[2];
unsigned char BDINA219CurrentBuff[20];
unsigned short BDINA219CurrentMidBuff[5];
void BDINA219_IIC_Delay()
{
  unsigned char BDINA219_IIC_Delay_i;
  for(BDINA219_IIC_Delay_i=0;BDINA219_IIC_Delay_i<50;BDINA219_IIC_Delay_i++)
  {
    NOP();
  }
}

void BDINA219_IIC_Init()
{
  BDINA219_SCL(0);
  BDINA219_SDA_OUT_Dir;
  BDINA219_SDA(0);
}
void BDINA219_IIC_Start()
{
  BDINA219_SDA_OUT_Dir;
  BDINA219_SDA(1);
  BDINA219_SCL(1);
  BDINA219_IIC_Delay();
  BDINA219_SDA(0);
  BDINA219_SCL(0);
}
void BDINA219_IIC_Stop()
{
  BDINA219_SDA_OUT_Dir;
  BDINA219_SDA(0);
  BDINA219_SCL(1);
  BDINA219_IIC_Delay();
  BDINA219_SDA(1);
  BDINA219_SCL(0);
  BDINA219_IIC_Delay();
  BDINA219_SCL(1);
}
unsigned char BDINA219_IIC_SendByte(unsigned char Data)
{
  unsigned char BDINA219_IIC_SendByte_i;
  BDINA219_SDA_OUT_Dir;
  BDINA219_SCL(0); 
  for(BDINA219_IIC_SendByte_i=0;BDINA219_IIC_SendByte_i<8;BDINA219_IIC_SendByte_i++)
  {
    if(Data&0x80)
    {
      BDINA219_SDA(1);
    }
    else
    {
      BDINA219_SDA(0);
    }
    BDINA219_IIC_Delay();
    BDINA219_SCL(1);
    BDINA219_IIC_Delay();
    BDINA219_SCL(0);
    Data=Data<<1;
  }
  BDINA219_SDA_IN_Dir;
  BDINA219_IIC_Delay();
  BDINA219_SCL(1);
  BDINA219_IIC_Delay();
  if(BDINA219_SDA_InPut==0)
  {
    BDINA219_SCL(0);
  } 
  else
  {
    BDINA219_SCL(0);
  }
  BDINA219_IIC_Delay();
  return TRUE;
}
unsigned char BDINA219_IIC_SendBytes(unsigned char *Datas,unsigned char Len)
{
  unsigned char BDINA219_IIC_SendBytes_i;
  for(BDINA219_IIC_SendBytes_i=0;BDINA219_IIC_SendBytes_i<Len;BDINA219_IIC_SendBytes_i++)
  {
    BDINA219_IIC_SendByte(*(Datas+BDINA219_IIC_SendBytes_i));
  }
  return TRUE;
}
unsigned char BDINA219_IIC_RevByte()
{
  unsigned char BDINA219_IIC_RevByte_i,BDINA219_IIC_RevByte_Data=0;
  BDINA219_SDA_IN_Dir;
  BDINA219_SCL(0);
  BDINA219_IIC_Delay();
  for(BDINA219_IIC_RevByte_i=0;BDINA219_IIC_RevByte_i<8;BDINA219_IIC_RevByte_i++)
  {
    BDINA219_SCL(1);
    BDINA219_IIC_Delay();
    if(BDINA219_SDA_InPut)
    {
      BDINA219_IIC_RevByte_Data |=0x80>>BDINA219_IIC_RevByte_i;
    }
    BDINA219_SCL(0);
    BDINA219_IIC_Delay();
  }
  return BDINA219_IIC_RevByte_Data;
}
void BDINA219_IIC_RevBytes(unsigned char *Datas,unsigned char Len)
{
  unsigned char BDINA219_IIC_RevBytes_i;
  for(BDINA219_IIC_RevBytes_i=0;BDINA219_IIC_RevBytes_i<Len;BDINA219_IIC_RevBytes_i++)
  {
    *(Datas+BDINA219_IIC_RevBytes_i)=BDINA219_IIC_RevByte();
    if(BDINA219_IIC_RevBytes_i<Len-1)
    {
      BDINA219_SDA_OUT_Dir;
      BDINA219_SDA(0);
      BDINA219_IIC_Delay();
      BDINA219_SCL(1);
      BDINA219_IIC_Delay();
      BDINA219_SCL(0);
    }
    else
    {
      BDINA219_SDA_OUT_Dir;
      BDINA219_SDA(1);
      BDINA219_IIC_Delay();
      BDINA219_SCL(1);
      BDINA219_IIC_Delay();
      BDINA219_SCL(0);
    }
  }
}
unsigned short ReadBD_Current()////write-0x80 Read-0x81
{
	unsigned char Read_Current_i;
	unsigned short Read_Current_midresult=0;
	unsigned short Read_Current_result=0;
	
  BDINA219Buff[0]=0x07;
	BDINA219Buff[1]=0x9f;
	
	BDINA219_IIC_Start();
  BDINA219_IIC_SendByte(0x80);		
	BDINA219_IIC_SendByte(0);
	BDINA219_IIC_SendBytes(BDINA219Buff,2);
  BDINA219_IIC_Stop();

	BDINA219Buff[0]=0x10;
	BDINA219Buff[1]=0x00;
  BDINA219_IIC_Start();
  BDINA219_IIC_SendByte(0x80);		
	BDINA219_IIC_SendByte(5);
	BDINA219_IIC_SendBytes(BDINA219Buff,2);
  BDINA219_IIC_Stop();

	for(Read_Current_i=0;Read_Current_i<10;Read_Current_i++)
	{
		Delay_ms(50);
		BDINA219_IIC_Start();
	  BDINA219_IIC_SendByte(0x80);	
		BDINA219_IIC_SendByte(4);
		BDINA219_IIC_Start();
		BDINA219_IIC_SendByte(0x81);
		BDINA219_IIC_RevBytes(BDINA219CurrentBuff+Read_Current_i*2,2);
	  BDINA219_IIC_Stop();
	}
	for(Read_Current_i=0;Read_Current_i<5;Read_Current_i++)
	{
		BDINA219CurrentMidBuff[Read_Current_i] =(unsigned short)BDINA219CurrentBuff[Read_Current_i*2]*0x100;
		BDINA219CurrentMidBuff[Read_Current_i] +=BDINA219CurrentBuff[Read_Current_i*2+1];
	}
	
	Read_Current_midresult = BDINA219CurrentMidBuff[0];
	for(Read_Current_i=1;Read_Current_i<5;Read_Current_i++)
	{
		if(Read_Current_midresult > BDINA219CurrentMidBuff[Read_Current_i])
		{
			Read_Current_midresult = BDINA219CurrentMidBuff[Read_Current_i];
		}
	}
	
	Read_Current_result =Read_Current_midresult;
	return Read_Current_result;
}
