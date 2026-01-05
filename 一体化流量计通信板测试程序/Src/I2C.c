#include "I2C.h"
#include "UART0.h"
#include "GPIO.h"
#include "LED_CTRL.h"

#define I2CREAD     1   //I2C主机读操作
#define I2CWRITE    0   //I2C主机写操作
uint32_t wendu = 20;
uint32_t yali  = 110000;
uint32_t KT = 524288;
uint32_t KP = 253952;

uint8_t read_dizhi[14]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
uint8_t rec_0x10_count[18]={0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00};
uint8_t IIC_send_weituo = 0x00;
// 设备地址 0X77

uint8_t readdata[3];                       //接收的数据
uint8_t writedata[3] = {0x10, 0x55, 0x16}; //要发送的数据

void I2C_IRQHandler(void)
{
	  uint8_t i = 0;
    uint32_t temp = 0;
	  uint32_t set_wendu = 0;
	  uint32_t set_yali = 0;
	  uint32_t wendu_xiaoshu = 0;
	  uint32_t yali_shezhi = 0;
	  LED_FLAG_Run();
		read_dizhi[0] = FL_I2C_Slave_ReadRXBuff(I2C);                            //读地址
				
    if((FL_ENABLE == FL_I2C_Slave_IsEnabledIT_AddressMatch(I2C))
            && (FL_SET == FL_I2C_Slave_IsActiveFlag_AddressMatch(I2C)))
    {
        temp = FL_I2C_Slave_ReadRXBuff(I2C);                            //读地址
        FL_I2C_Slave_ClearFlag_AddressMatch(I2C);
        if((temp & 0x01) == I2CWRITE)                     //主机写
        {
					 FL_I2C_Slave_ClearFlag_RXComplete(I2C);                          //读功能码
           while(!FL_I2C_Slave_IsActiveFlag_RXComplete(I2C));
           read_dizhi[1] = FL_I2C_Slave_ReadRXBuff(I2C);
           FL_I2C_Slave_ClearFlag_RXComplete(I2C);
					 if(read_dizhi[1]==0x06)
					 {
						 while(!FL_I2C_Slave_IsActiveFlag_RXComplete(I2C))
							 ;
             read_dizhi[2] = FL_I2C_Slave_ReadRXBuff(I2C);
             FL_I2C_Slave_ClearFlag_RXComplete(I2C);
					 }
					 else if(read_dizhi[1]==0x07)
					 {
						 while(!FL_I2C_Slave_IsActiveFlag_RXComplete(I2C))
							 ;
             read_dizhi[3] = FL_I2C_Slave_ReadRXBuff(I2C);
             FL_I2C_Slave_ClearFlag_RXComplete(I2C);
					 }
					 else if(read_dizhi[1]==0x09)
					 {
						 while(!FL_I2C_Slave_IsActiveFlag_RXComplete(I2C))
							 ;
             read_dizhi[4] = FL_I2C_Slave_ReadRXBuff(I2C);
             FL_I2C_Slave_ClearFlag_RXComplete(I2C);
					 }
					 else if(read_dizhi[1]==0x08)
					 {
						 while(!FL_I2C_Slave_IsActiveFlag_RXComplete(I2C))
							 ;
             read_dizhi[5] = FL_I2C_Slave_ReadRXBuff(I2C);
             FL_I2C_Slave_ClearFlag_RXComplete(I2C);
					 }
        }
        else
            if((temp & 0x01) == I2CREAD)              //主机读
            {
							if(read_dizhi[1]==0x0D)//读地址 返回 0x10
							{
									FL_I2C_Slave_WriteTXBuff(I2C, writedata[0]);

									while(!FL_I2C_Slave_IsActiveFlag_TXComplete(I2C));

									FL_I2C_Slave_ClearFlag_TXComplete(I2C);
							}
							else if(read_dizhi[1]==0x10)
							{
								//温度精度和这里有关
								wendu_xiaoshu = wendu*2;
								rec_0x10_count[0] = wendu_xiaoshu>>4;
								rec_0x10_count[1] = wendu_xiaoshu<<4&0xF0;
								//这里可以直接赋值压力
								yali_shezhi = yali;
								rec_0x10_count[3] = yali_shezhi>>12;
								rec_0x10_count[4] = yali_shezhi>>4;
								rec_0x10_count[5] = yali_shezhi<<4&0xf0;
								for(i=0;i<18;i++)
								{
									FL_I2C_Slave_WriteTXBuff(I2C, rec_0x10_count[i]);

									while(!FL_I2C_Slave_IsActiveFlag_TXComplete(I2C));

									FL_I2C_Slave_ClearFlag_TXComplete(I2C);
								}
							}
							else if(read_dizhi[1]==0x06)
							{
								FL_I2C_Slave_WriteTXBuff(I2C, 0xB0);

									while(!FL_I2C_Slave_IsActiveFlag_TXComplete(I2C));

									FL_I2C_Slave_ClearFlag_TXComplete(I2C);
							}
							else if(read_dizhi[1]==0x07)
							{
								FL_I2C_Slave_WriteTXBuff(I2C, 0xB0);

									while(!FL_I2C_Slave_IsActiveFlag_TXComplete(I2C));

									FL_I2C_Slave_ClearFlag_TXComplete(I2C);
							}
							else if(read_dizhi[1]==0x09)
							{
								FL_I2C_Slave_WriteTXBuff(I2C, 0x00);

									while(!FL_I2C_Slave_IsActiveFlag_TXComplete(I2C));

									FL_I2C_Slave_ClearFlag_TXComplete(I2C);
							}
							else if(read_dizhi[1]==0x08)
							{
								FL_I2C_Slave_WriteTXBuff(I2C, 0x02);

									while(!FL_I2C_Slave_IsActiveFlag_TXComplete(I2C));

									FL_I2C_Slave_ClearFlag_TXComplete(I2C);
							}
							else if(read_dizhi[1]==0x03)
							{
								  set_wendu = KT;
								  FL_I2C_Slave_WriteTXBuff(I2C, set_wendu>>16&0xFF);

									while(!FL_I2C_Slave_IsActiveFlag_TXComplete(I2C));

									FL_I2C_Slave_ClearFlag_TXComplete(I2C);
								
								
								FL_I2C_Slave_WriteTXBuff(I2C, set_wendu>>8&0xFF);

									while(!FL_I2C_Slave_IsActiveFlag_TXComplete(I2C));

									FL_I2C_Slave_ClearFlag_TXComplete(I2C);
								
								
								FL_I2C_Slave_WriteTXBuff(I2C, set_wendu&0xFF);

									while(!FL_I2C_Slave_IsActiveFlag_TXComplete(I2C));

									FL_I2C_Slave_ClearFlag_TXComplete(I2C);
							}
							else if(read_dizhi[1]==0x00)
							{
								  set_yali = KP;
								  FL_I2C_Slave_WriteTXBuff(I2C, set_yali>>16&0xFF);

									while(!FL_I2C_Slave_IsActiveFlag_TXComplete(I2C));

									FL_I2C_Slave_ClearFlag_TXComplete(I2C);
								
								
								FL_I2C_Slave_WriteTXBuff(I2C, set_yali>>8&0xFF);

									while(!FL_I2C_Slave_IsActiveFlag_TXComplete(I2C));

									FL_I2C_Slave_ClearFlag_TXComplete(I2C);
								
								
								FL_I2C_Slave_WriteTXBuff(I2C, set_yali&0xFF);

									while(!FL_I2C_Slave_IsActiveFlag_TXComplete(I2C));

									FL_I2C_Slave_ClearFlag_TXComplete(I2C);
							}
            }
    }
		LED_FLAG_Run();
		IIC_send_weituo = 0x01;
}
void MF_I2C_Init(void)
{
    FL_GPIO_InitTypeDef    GPIO_InitStruct;
    FL_I2C_SlaveMode_InitTypeDef   I2CInitStructer;

    GPIO_InitStruct.pin = FL_GPIO_PIN_15;
    GPIO_InitStruct.mode = FL_GPIO_MODE_DIGITAL;
    GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.pull = FL_DISABLE;
    GPIO_InitStruct.remapPin = FL_DISABLE;
    GPIO_InitStruct.analogSwitch = FL_DISABLE;
    FL_GPIO_Init(GPIOB, &GPIO_InitStruct);

    GPIO_InitStruct.pin = FL_GPIO_PIN_12;
    GPIO_InitStruct.mode = FL_GPIO_MODE_DIGITAL;
    GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.pull = FL_DISABLE;
    GPIO_InitStruct.remapPin = FL_DISABLE;
    GPIO_InitStruct.analogSwitch = FL_DISABLE;
    FL_GPIO_Init(GPIOD, &GPIO_InitStruct);

    I2CInitStructer.ACK = FL_ENABLE;
    //I2CInitStructer.ownAddr = 0X49;//TMP102
		I2CInitStructer.ownAddr = 0X77;
    I2CInitStructer.ownAddrSize10bit = FL_DISABLE;
    I2CInitStructer.SCLSEN = FL_ENABLE;
    FL_I2C_SlaveMode_Init(I2C, &I2CInitStructer);
}
static void MF_I2C_Interrupt_Init(void)
{
    FL_I2C_Slave_ClearFlag_AddressMatch(I2C);
	  //FL_I2C_Slave_ClearFlag_AddressError(I2C);
    FL_I2C_Slave_EnableIT_AddressMatch(I2C);
	  //FL_I2C_Slave_EnableIT_AddressError(I2C);
	  //FL_I2C_Slave_EnableIT_Start(I2C);
	  //FL_I2C_Slave_EnableIT_BuffOverflow(I2C);
	  //FL_I2C_Slave_EnableIT_Stop(I2C);
}
static void MF_NVIC_IIC_Init(void)
{
    FL_NVIC_ConfigTypeDef    InterruptConfigStruct;

    InterruptConfigStruct.preemptPriority = 0x0;                                        /*配置中断优先级*/
    FL_NVIC_Init(&InterruptConfigStruct, I2C_IRQn);
}

void MF_IIC_Config_Init(void)
{
    /* Initial I2C */
    MF_I2C_Init();

    MF_I2C_Interrupt_Init();

    MF_NVIC_IIC_Init();
}
void send_weituo()
{
	if(IIC_send_weituo == 0x01)
	{
		IIC_send_weituo = 0x00;
	//新增数据打印委托，在中断中执行打印容易出问题
	PC_Chuankou_tongxin_Debug_send(read_dizhi,14);
	}
}






