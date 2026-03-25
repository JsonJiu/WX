#include "main.h"
#include "ZDINA219.h"
#include "GPIO.h"
#define TRUE 1
#define FALSE 0
unsigned char ZDINA219Buff[2];
unsigned char ZDINA219CurrentBuff[20];
unsigned short ZDINA219CurrentMidBuff[5];
uint8_t chuankou_dangqian = 0;

void ZDINA219_IIC_Delay()
{
  unsigned char ZDINA219_IIC_Delay_i;
  for(ZDINA219_IIC_Delay_i=0;ZDINA219_IIC_Delay_i<50;ZDINA219_IIC_Delay_i++)
  {
    __NOP();
  }
}
void INA219_IIC_GPIO_Init()
{
	  //1工位
		FL_GPIO_InitTypeDef    GPIO_InitStruct;
	  //SCL PA13
	  GPIO_InitStruct.pin = FL_GPIO_PIN_13;
    GPIO_InitStruct.mode = FL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_OPENDRAIN;
    GPIO_InitStruct.pull = FL_DISABLE;
    GPIO_InitStruct.remapPin = FL_DISABLE;
    GPIO_InitStruct.analogSwitch = FL_DISABLE;
    FL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	  //SDA PE9
	  GPIO_InitStruct.pin = FL_GPIO_PIN_9;
    GPIO_InitStruct.mode = FL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_OPENDRAIN;
    GPIO_InitStruct.pull = FL_DISABLE;
    GPIO_InitStruct.remapPin = FL_DISABLE;
    GPIO_InitStruct.analogSwitch = FL_DISABLE;
    FL_GPIO_Init(GPIOE, &GPIO_InitStruct);
	  //控制脚 PD10
	  GPIO_InitStruct.pin = FL_GPIO_PIN_10;
    GPIO_InitStruct.mode = FL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.pull = FL_DISABLE;
    GPIO_InitStruct.remapPin = FL_DISABLE;
    GPIO_InitStruct.analogSwitch = FL_DISABLE;
    FL_GPIO_Init(GPIOD, &GPIO_InitStruct);
		//2工位
		//SCL PA0
	  GPIO_InitStruct.pin = FL_GPIO_PIN_0;
    GPIO_InitStruct.mode = FL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_OPENDRAIN;
    GPIO_InitStruct.pull = FL_DISABLE;
    GPIO_InitStruct.remapPin = FL_DISABLE;
    GPIO_InitStruct.analogSwitch = FL_DISABLE;
    FL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	  //SDA PA15
	  GPIO_InitStruct.pin = FL_GPIO_PIN_15;
    GPIO_InitStruct.mode = FL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_OPENDRAIN;
    GPIO_InitStruct.pull = FL_DISABLE;
    GPIO_InitStruct.remapPin = FL_DISABLE;
    GPIO_InitStruct.analogSwitch = FL_DISABLE;
    FL_GPIO_Init(GPIOA, &GPIO_InitStruct);
		//控制脚 PA14
	  GPIO_InitStruct.pin = FL_GPIO_PIN_14;
    GPIO_InitStruct.mode = FL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.pull = FL_DISABLE;
    GPIO_InitStruct.remapPin = FL_DISABLE;
    GPIO_InitStruct.analogSwitch = FL_DISABLE;
    FL_GPIO_Init(GPIOA, &GPIO_InitStruct);
		//3工位
		//SCL PA3
	  GPIO_InitStruct.pin = FL_GPIO_PIN_3;
    GPIO_InitStruct.mode = FL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_OPENDRAIN;
    GPIO_InitStruct.pull = FL_DISABLE;
    GPIO_InitStruct.remapPin = FL_DISABLE;
    GPIO_InitStruct.analogSwitch = FL_DISABLE;
    FL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	  //SDA PA2
	  GPIO_InitStruct.pin = FL_GPIO_PIN_2;
    GPIO_InitStruct.mode = FL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_OPENDRAIN;
    GPIO_InitStruct.pull = FL_DISABLE;
    GPIO_InitStruct.remapPin = FL_DISABLE;
    GPIO_InitStruct.analogSwitch = FL_DISABLE;
    FL_GPIO_Init(GPIOA, &GPIO_InitStruct);
		//控制脚 PA1
	  GPIO_InitStruct.pin = FL_GPIO_PIN_1;
    GPIO_InitStruct.mode = FL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.pull = FL_DISABLE;
    GPIO_InitStruct.remapPin = FL_DISABLE;
    GPIO_InitStruct.analogSwitch = FL_DISABLE;
    FL_GPIO_Init(GPIOA, &GPIO_InitStruct);
		//4工位
		//SCL PA6
	  GPIO_InitStruct.pin = FL_GPIO_PIN_6;
    GPIO_InitStruct.mode = FL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_OPENDRAIN;
    GPIO_InitStruct.pull = FL_DISABLE;
    GPIO_InitStruct.remapPin = FL_DISABLE;
    GPIO_InitStruct.analogSwitch = FL_DISABLE;
    FL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	  //SDA PA5
	  GPIO_InitStruct.pin = FL_GPIO_PIN_5;
    GPIO_InitStruct.mode = FL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_OPENDRAIN;
    GPIO_InitStruct.pull = FL_DISABLE;
    GPIO_InitStruct.remapPin = FL_DISABLE;
    GPIO_InitStruct.analogSwitch = FL_DISABLE;
    FL_GPIO_Init(GPIOA, &GPIO_InitStruct);
		//控制脚 PA4
	  GPIO_InitStruct.pin = FL_GPIO_PIN_4;
    GPIO_InitStruct.mode = FL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.pull = FL_DISABLE;
    GPIO_InitStruct.remapPin = FL_DISABLE;
    GPIO_InitStruct.analogSwitch = FL_DISABLE;
    FL_GPIO_Init(GPIOA, &GPIO_InitStruct);
		//5工位
		//SCL PA9
	  GPIO_InitStruct.pin = FL_GPIO_PIN_9;
    GPIO_InitStruct.mode = FL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_OPENDRAIN;
    GPIO_InitStruct.pull = FL_DISABLE;
    GPIO_InitStruct.remapPin = FL_DISABLE;
    GPIO_InitStruct.analogSwitch = FL_DISABLE;
    FL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	  //SDA PA8
	  GPIO_InitStruct.pin = FL_GPIO_PIN_8;
    GPIO_InitStruct.mode = FL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_OPENDRAIN;
    GPIO_InitStruct.pull = FL_DISABLE;
    GPIO_InitStruct.remapPin = FL_DISABLE;
    GPIO_InitStruct.analogSwitch = FL_DISABLE;
    FL_GPIO_Init(GPIOA, &GPIO_InitStruct);
		//控制脚 PA7
	  GPIO_InitStruct.pin = FL_GPIO_PIN_7;
    GPIO_InitStruct.mode = FL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.pull = FL_DISABLE;
    GPIO_InitStruct.remapPin = FL_DISABLE;
    GPIO_InitStruct.analogSwitch = FL_DISABLE;
    FL_GPIO_Init(GPIOA, &GPIO_InitStruct);
		//6工位
		//SCL PA12
	  GPIO_InitStruct.pin = FL_GPIO_PIN_12;
    GPIO_InitStruct.mode = FL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_OPENDRAIN;
    GPIO_InitStruct.pull = FL_DISABLE;
    GPIO_InitStruct.remapPin = FL_DISABLE;
    GPIO_InitStruct.analogSwitch = FL_DISABLE;
    FL_GPIO_Init(GPIOA, &GPIO_InitStruct);
	  //SDA PA11
	  GPIO_InitStruct.pin = FL_GPIO_PIN_11;
    GPIO_InitStruct.mode = FL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_OPENDRAIN;
    GPIO_InitStruct.pull = FL_DISABLE;
    GPIO_InitStruct.remapPin = FL_DISABLE;
    GPIO_InitStruct.analogSwitch = FL_DISABLE;
    FL_GPIO_Init(GPIOA, &GPIO_InitStruct);
		//控制脚 PA10
	  GPIO_InitStruct.pin = FL_GPIO_PIN_10;
    GPIO_InitStruct.mode = FL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.pull = FL_DISABLE;
    GPIO_InitStruct.remapPin = FL_DISABLE;
    GPIO_InitStruct.analogSwitch = FL_DISABLE;
    FL_GPIO_Init(GPIOA, &GPIO_InitStruct);
		//正式使用前，SDA需要拉高
		FL_GPIO_SetOutputPin(GPIOE,FL_GPIO_PIN_9);
		FL_GPIO_SetOutputPin(GPIOA,FL_GPIO_PIN_15);
		FL_GPIO_SetOutputPin(GPIOA,FL_GPIO_PIN_2);
		FL_GPIO_SetOutputPin(GPIOA,FL_GPIO_PIN_5);
		FL_GPIO_SetOutputPin(GPIOA,FL_GPIO_PIN_8);
		FL_GPIO_SetOutputPin(GPIOA,FL_GPIO_PIN_11);
		//正式使用前，控制脚也需要拉高
		FL_GPIO_SetOutputPin(GPIOD,FL_GPIO_PIN_10);
		FL_GPIO_SetOutputPin(GPIOA,FL_GPIO_PIN_14);
		FL_GPIO_SetOutputPin(GPIOA,FL_GPIO_PIN_1);
		FL_GPIO_SetOutputPin(GPIOA,FL_GPIO_PIN_4);
		FL_GPIO_SetOutputPin(GPIOA,FL_GPIO_PIN_7);
		FL_GPIO_SetOutputPin(GPIOA,FL_GPIO_PIN_10);
}

//主控板电流检测使能开
void Current_CHK_CTRL_ON()
{
	switch(chuankou_dangqian)
	{
		case 0:
			FL_GPIO_ResetOutputPin(GPIOD,FL_GPIO_PIN_10);
		break;
		case 1:
			FL_GPIO_ResetOutputPin(GPIOA,FL_GPIO_PIN_14);
		break;
		case 2:
			FL_GPIO_ResetOutputPin(GPIOA,FL_GPIO_PIN_1);
		break;
		case 3:
			FL_GPIO_ResetOutputPin(GPIOA,FL_GPIO_PIN_4);
		break;
		case 4:
			FL_GPIO_ResetOutputPin(GPIOA,FL_GPIO_PIN_7);
		break;
		case 5:
			FL_GPIO_ResetOutputPin(GPIOA,FL_GPIO_PIN_10);
		break;
		default:
			break;
	}
}
//主控板电流检测使能关
void Current_CHK_CTRL_OFF()
{
	switch(chuankou_dangqian)
	{
		case 0:
			FL_GPIO_SetOutputPin(GPIOD,FL_GPIO_PIN_10);
		break;
		case 1:
			FL_GPIO_SetOutputPin(GPIOA,FL_GPIO_PIN_14);
		break;
		case 2:
			FL_GPIO_SetOutputPin(GPIOA,FL_GPIO_PIN_1);
		break;
		case 3:
			FL_GPIO_SetOutputPin(GPIOA,FL_GPIO_PIN_4);
		break;
		case 4:
			FL_GPIO_SetOutputPin(GPIOA,FL_GPIO_PIN_7);
		break;
		case 5:
			FL_GPIO_SetOutputPin(GPIOA,FL_GPIO_PIN_10);
		break;
		default:
			break;
	}
}

void ZDINA219_SCL(uint8_t zhuangtai)
{
	if(zhuangtai==1)
	{
		switch(chuankou_dangqian)
		{
			case 0:
				FL_GPIO_SetOutputPin(GPIOA,FL_GPIO_PIN_13);
			break;
			case 1:
				FL_GPIO_SetOutputPin(GPIOA,FL_GPIO_PIN_0);
			break;
			case 2:
				FL_GPIO_SetOutputPin(GPIOA,FL_GPIO_PIN_3);
			break;
			case 3:
				FL_GPIO_SetOutputPin(GPIOA,FL_GPIO_PIN_6);
			break;
			case 4:
				FL_GPIO_SetOutputPin(GPIOA,FL_GPIO_PIN_9);
			break;
			case 5:
				FL_GPIO_SetOutputPin(GPIOA,FL_GPIO_PIN_12);
			break;
			default:
				break;
		}
	}
	else
	{
		switch(chuankou_dangqian)
		{
			case 0:
				FL_GPIO_ResetOutputPin(GPIOA,FL_GPIO_PIN_13);
			break;
			case 1:
				FL_GPIO_ResetOutputPin(GPIOA,FL_GPIO_PIN_0);
			break;
			case 2:
				FL_GPIO_ResetOutputPin(GPIOA,FL_GPIO_PIN_3);
			break;
			case 3:
				FL_GPIO_ResetOutputPin(GPIOA,FL_GPIO_PIN_6);
			break;
			case 4:
				FL_GPIO_ResetOutputPin(GPIOA,FL_GPIO_PIN_9);
			break;
			case 5:
				FL_GPIO_ResetOutputPin(GPIOA,FL_GPIO_PIN_12);
			break;
			default:
				break;
		}
	}
}
void ZDINA219_SDA_OUT_Dir_CTL(uint8_t zhuangtai)
{
	FL_GPIO_InitTypeDef    GPIO_InitStruct;
	//PE9 PA15 PA2 PA5 PA8 PA11
	GPIO_InitStruct.outputType    = FL_GPIO_OUTPUT_OPENDRAIN;
  GPIO_InitStruct.pull          = FL_ENABLE;
  GPIO_InitStruct.remapPin      = FL_DISABLE;
  GPIO_InitStruct.analogSwitch  = FL_DISABLE;
	//1 output 0 Input
	if(zhuangtai==1)
	{
    GPIO_InitStruct.mode          = FL_GPIO_MODE_OUTPUT;
	}
	else
	{
		GPIO_InitStruct.mode          = FL_GPIO_MODE_INPUT;
	}
	switch(chuankou_dangqian)
	{
		case 0:
			GPIO_InitStruct.pin= FL_GPIO_PIN_9;
			(void)FL_GPIO_Init(GPIOE, &GPIO_InitStruct);
			break;
		case 1:
			GPIO_InitStruct.pin= FL_GPIO_PIN_15;
			(void)FL_GPIO_Init(GPIOA, &GPIO_InitStruct);
			break;
		case 2:
			GPIO_InitStruct.pin= FL_GPIO_PIN_2;
			(void)FL_GPIO_Init(GPIOA, &GPIO_InitStruct);
			break;
		case 3:
			GPIO_InitStruct.pin= FL_GPIO_PIN_5;
			(void)FL_GPIO_Init(GPIOA, &GPIO_InitStruct);
			break;
		case 4:
			GPIO_InitStruct.pin= FL_GPIO_PIN_8;
			(void)FL_GPIO_Init(GPIOA, &GPIO_InitStruct);
			break;
		case 5:
			GPIO_InitStruct.pin= FL_GPIO_PIN_11;
			(void)FL_GPIO_Init(GPIOA, &GPIO_InitStruct);
			break;
		default:
			break;
	}
}
#define ZDINA219_SDA_OUT_Dir   ZDINA219_SDA_OUT_Dir_CTL(1)
#define ZDINA219_SDA_IN_Dir    ZDINA219_SDA_OUT_Dir_CTL(0)

void ZDINA219_SDA(uint8_t zhuangtai)
{
	//PE9 PA15 PA2 PA5 PA8 PA11
	if(zhuangtai==1)
	{
		switch(chuankou_dangqian)
	{
		case 0:
			FL_GPIO_SetOutputPin(GPIOE,FL_GPIO_PIN_9);
			break;
		case 1:
			FL_GPIO_SetOutputPin(GPIOA,FL_GPIO_PIN_15);
			break;
		case 2:
			FL_GPIO_SetOutputPin(GPIOA,FL_GPIO_PIN_2);
			break;
		case 3:
			FL_GPIO_SetOutputPin(GPIOA,FL_GPIO_PIN_5);
			break;
		case 4:
			FL_GPIO_SetOutputPin(GPIOA,FL_GPIO_PIN_8);
			break;
		case 5:
			FL_GPIO_SetOutputPin(GPIOA,FL_GPIO_PIN_11);
			break;
		default:
			break;
		}
	}
	else
	{
		switch(chuankou_dangqian)
	{
		case 0:
			FL_GPIO_ResetOutputPin(GPIOE,FL_GPIO_PIN_9);
			break;
		case 1:
			FL_GPIO_ResetOutputPin(GPIOA,FL_GPIO_PIN_15);
			break;
		case 2:
			FL_GPIO_ResetOutputPin(GPIOA,FL_GPIO_PIN_2);
			break;
		case 3:
			FL_GPIO_ResetOutputPin(GPIOA,FL_GPIO_PIN_5);
			break;
		case 4:
			FL_GPIO_ResetOutputPin(GPIOA,FL_GPIO_PIN_8);
			break;
		case 5:
			FL_GPIO_ResetOutputPin(GPIOA,FL_GPIO_PIN_11);
			break;
		default:
			break;
		}
	}
}
uint32_t ZDINA219_SDA_InPut()
{
	switch(chuankou_dangqian)
	{
		case 0:
			return FL_GPIO_GetInputPin(GPIOE,FL_GPIO_PIN_9);
		case 1:
			return FL_GPIO_GetInputPin(GPIOA,FL_GPIO_PIN_15);
		case 2:
			return FL_GPIO_GetInputPin(GPIOA,FL_GPIO_PIN_2);
		case 3:
			return FL_GPIO_GetInputPin(GPIOA,FL_GPIO_PIN_5);
		case 4:
			return FL_GPIO_GetInputPin(GPIOA,FL_GPIO_PIN_8);
		case 5:
			return FL_GPIO_GetInputPin(GPIOA,FL_GPIO_PIN_11);
		default:
			return 0;
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
  if(ZDINA219_SDA_InPut()==0)
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
    if(ZDINA219_SDA_InPut())
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
	unsigned short Read_Current_midresult=0;
	unsigned short Read_Current_result=0;
  ZDINA219Buff[0]=0x07;
	ZDINA219Buff[1]=0x9f;
	
	ZDINA219_IIC_Start();
  ZDINA219_IIC_SendByte(0x80);		
	ZDINA219_IIC_SendByte(0);
	ZDINA219_IIC_SendBytes(ZDINA219Buff,2);
  ZDINA219_IIC_Stop();

	ZDINA219Buff[0]=0xA0;
	ZDINA219Buff[1]=0x00;
  ZDINA219_IIC_Start();
  ZDINA219_IIC_SendByte(0x80);		
	ZDINA219_IIC_SendByte(5);
	ZDINA219_IIC_SendBytes(ZDINA219Buff,2);
  ZDINA219_IIC_Stop();
	FL_DelayMs(20);
	ZDINA219_IIC_Start();
	ZDINA219_IIC_SendByte(0x80);	
	ZDINA219_IIC_SendByte(4);
	ZDINA219_IIC_Start();
	ZDINA219_IIC_SendByte(0x81);
	ZDINA219_IIC_RevBytes(ZDINA219CurrentBuff,2);
	ZDINA219_IIC_Stop();
	ZDINA219CurrentMidBuff[0] =(unsigned short)ZDINA219CurrentBuff[0]*0x100;
	ZDINA219CurrentMidBuff[0] +=ZDINA219CurrentBuff[1];
	Read_Current_midresult = ZDINA219CurrentMidBuff[0];
	Read_Current_result =Read_Current_midresult;
	return Read_Current_result;
}

uint16_t CheckZDCurrent()//检测主电功耗
{
	uint8_t CheckZDCurrent_i;
	uint16_t minZDCurrent;
	uint16_t tmpZDCurrent[3];
	//MCP4561_Write_Data(0,0);///big;
	ReadZD_Current();//先读一次，第一次读取有可能读错
	FL_DelayMs(10);
	for(CheckZDCurrent_i=0;CheckZDCurrent_i<3;CheckZDCurrent_i++)
	{
		FL_DelayMs(10);
		tmpZDCurrent[CheckZDCurrent_i]=ReadZD_Current();
	}
	minZDCurrent = 0;
	for(CheckZDCurrent_i=0;CheckZDCurrent_i<3;CheckZDCurrent_i++)
	{
          if(tmpZDCurrent[CheckZDCurrent_i]>>15==0x01)
          {
            tmpZDCurrent[CheckZDCurrent_i] = 0xFFFF - tmpZDCurrent[CheckZDCurrent_i]+1;
            minZDCurrent-=tmpZDCurrent[CheckZDCurrent_i];
          }
          else
          {
            minZDCurrent += tmpZDCurrent[CheckZDCurrent_i];
          }	
	}
        if(minZDCurrent>>15==0x01)
        {
          minZDCurrent = 0xFFFF-minZDCurrent+1;
          minZDCurrent = minZDCurrent/3;
          minZDCurrent = 0xFFFF-minZDCurrent+1;
        }
        else
        {
          minZDCurrent = minZDCurrent/3;
        }
        return minZDCurrent;
}

uint16_t Current_CHK_Func(uint8_t gongwei)
{
	uint16_t dianliu;
	if(gongwei>5)
	{
		return 0xFFFF;
	}
	chuankou_dangqian = gongwei;
	Current_CHK_CTRL_ON();
	FL_DelayMs(100);
	dianliu = CheckZDCurrent()/10;//检测主电功耗
	if(dianliu>=280)
	{
		dianliu-=280;
	}
//	else if(dianliu>65532)
//	{
//		dianliu+=5;
//	}
//	if(gongwei==5)
//		dianliu-=26;
//	return dianliu;
	Current_CHK_CTRL_OFF();
	return dianliu;
}


