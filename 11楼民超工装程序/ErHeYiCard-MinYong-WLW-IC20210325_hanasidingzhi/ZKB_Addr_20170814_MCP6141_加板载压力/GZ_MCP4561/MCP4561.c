#include "MCP4561.h"
void MCP4561_IIC_Delay()
{
  unsigned char i;
  for(i=0;i<50;i++)
  {
    NOP();
  }
} 
void MCP4561_IIC_Init()
{
  MCP4561_SCL(1);
  MCP4561_SDA_OUT_Dir;
  MCP4561_SDA(1);
}
void MCP4561_IIC_Start()
{
  MCP4561_SDA(1);
  MCP4561_SCL(1);
  MCP4561_IIC_Delay();
  MCP4561_SDA(0);
  MCP4561_SCL(0);
}
void MCP4561_IIC_Stop()
{
  MCP4561_SDA(0);
  MCP4561_SCL(1);
	MCP4561_IIC_Delay();
  MCP4561_SDA(1);
  MCP4561_SCL(0);
	MCP4561_IIC_Delay();
  MCP4561_SCL(1);
}
unsigned char MCP4561_IIC_Data(unsigned char MCP_Data)
{
	unsigned int i;
  MCP4561_SDA_OUT_Dir;
  MCP4561_SCL(0);
  for(i=0;i<8;i++)
  {
    if(MCP_Data&0x80)
    {
      MCP4561_SDA(1);
    }
    else
    {
      MCP4561_SDA(0);
    }
    MCP4561_IIC_Delay();
    MCP4561_SCL(1);
    MCP4561_IIC_Delay();
    MCP_Data<<=1;
    MCP4561_SCL(0);
  }
  MCP4561_SDA_IN_Dir;
  MCP4561_IIC_Delay();
  MCP4561_SCL(1);
  MCP4561_IIC_Delay();
  if(!MCP4561_SDA_InPut)
  {
    MCP4561_SCL(0);
    MCP4561_SDA_OUT_Dir;
    return TRUE;
  }
  else
  {
    MCP4561_SCL(0);
    MCP4561_SDA_OUT_Dir;
    return FALSE;
  }
}
unsigned char MCP4561_Write_Data(unsigned char Choice,unsigned int MCP_Data)
{
  unsigned char d;
  if(MCP_Data>0x100)
  {
    MCP_Data=0x100;
  }
  MCP4561_IIC_Start();
	if(Choice)
	{
		d=0x5e;
	}
	else
	{
		d=0x5c;
	}
  //d |=Choice<<1;
  MCP4561_IIC_Data(d);
  d=(MCP_Data>>7)&0x2;
  MCP4561_IIC_Data(d);
  d=MCP_Data;
  if(MCP4561_IIC_Data(d)==FALSE)
  {
    MCP4561_IIC_Stop();
    return FALSE;
  }
  MCP4561_IIC_Stop();
  return TRUE;
}