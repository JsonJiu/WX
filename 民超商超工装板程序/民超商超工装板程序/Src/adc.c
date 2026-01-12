#include "adc.h"
#include "gpio.h"
static void MF_ADC_Common_Init(void)
{
    FL_ADC_CommonInitTypeDef    Common_InitStruct;

    Common_InitStruct.clockSource = FL_CMU_ADC_CLK_SOURCE_RCHF;                         /*配置ADC工作时钟源*/
    Common_InitStruct.clockPrescaler = FL_ADC_CLK_PSC_DIV8;                             /*配置ADC工作时钟分配*/
    Common_InitStruct.referenceSource = FL_ADC_REF_SOURCE_VDDA;                         /*配置ADC参考源*/
    Common_InitStruct.bitWidth = FL_ADC_BIT_WIDTH_12B;                                  /*配置ADC输出位宽*/

    (void)FL_ADC_CommonInit(&Common_InitStruct);
}
static void MF_ADC_Init(void)
{
    FL_GPIO_InitTypeDef    GPIO_InitStruct;

    FL_ADC_InitTypeDef    ADC_InitStruct;
	
	  //7V2输出电压工装自检
	  //PD0
    GPIO_InitStruct.pin           = FL_GPIO_PIN_0;
    GPIO_InitStruct.mode = FL_GPIO_MODE_ANALOG;                                         /*配置GPIO的功能模式*/
    GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;                               /*配置GPIO的输出模式*/
    GPIO_InitStruct.pull = FL_DISABLE;                                                  /*配置GPIO输上拉模式*/
    GPIO_InitStruct.remapPin = FL_DISABLE;                                              /*配置GPIO数字重定向功能*/
    GPIO_InitStruct.analogSwitch = FL_DISABLE;                                          /*配置GPIO模拟开关功能*/
    (void)FL_GPIO_Init(GPIOD, &GPIO_InitStruct);                                        /*GPIO初始化*/
	
	  //3V6输出电压工装自检
	  //PD1
    GPIO_InitStruct.pin           = FL_GPIO_PIN_1;
    GPIO_InitStruct.mode = FL_GPIO_MODE_ANALOG;                                         /*配置GPIO的功能模式*/
    GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;                               /*配置GPIO的输出模式*/
    GPIO_InitStruct.pull = FL_DISABLE;                                                  /*配置GPIO输上拉模式*/
    GPIO_InitStruct.remapPin = FL_DISABLE;                                              /*配置GPIO数字重定向功能*/
    GPIO_InitStruct.analogSwitch = FL_DISABLE;                                          /*配置GPIO模拟开关功能*/
    (void)FL_GPIO_Init(GPIOD, &GPIO_InitStruct);                                        /*GPIO初始化*/
		
		//AD1
	  //PD2
    GPIO_InitStruct.pin           = FL_GPIO_PIN_2;
    GPIO_InitStruct.mode = FL_GPIO_MODE_ANALOG;                                         /*配置GPIO的功能模式*/
    GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;                               /*配置GPIO的输出模式*/
    GPIO_InitStruct.pull = FL_DISABLE;                                                  /*配置GPIO输上拉模式*/
    GPIO_InitStruct.remapPin = FL_DISABLE;                                              /*配置GPIO数字重定向功能*/
    GPIO_InitStruct.analogSwitch = FL_DISABLE;                                          /*配置GPIO模拟开关功能*/
    (void)FL_GPIO_Init(GPIOD, &GPIO_InitStruct);                                        /*GPIO初始化*/
		
		//AD2
	  //PD3
    GPIO_InitStruct.pin           = FL_GPIO_PIN_3;
    GPIO_InitStruct.mode = FL_GPIO_MODE_ANALOG;                                         /*配置GPIO的功能模式*/
    GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;                               /*配置GPIO的输出模式*/
    GPIO_InitStruct.pull = FL_DISABLE;                                                  /*配置GPIO输上拉模式*/
    GPIO_InitStruct.remapPin = FL_DISABLE;                                              /*配置GPIO数字重定向功能*/
    GPIO_InitStruct.analogSwitch = FL_DISABLE;                                          /*配置GPIO模拟开关功能*/
    (void)FL_GPIO_Init(GPIOD, &GPIO_InitStruct);                                        /*GPIO初始化*/
		
		//AD3
	  //PD4
    GPIO_InitStruct.pin           = FL_GPIO_PIN_4;
    GPIO_InitStruct.mode = FL_GPIO_MODE_ANALOG;                                         /*配置GPIO的功能模式*/
    GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;                               /*配置GPIO的输出模式*/
    GPIO_InitStruct.pull = FL_DISABLE;                                                  /*配置GPIO输上拉模式*/
    GPIO_InitStruct.remapPin = FL_DISABLE;                                              /*配置GPIO数字重定向功能*/
    GPIO_InitStruct.analogSwitch = FL_DISABLE;                                          /*配置GPIO模拟开关功能*/
    (void)FL_GPIO_Init(GPIOD, &GPIO_InitStruct);                                        /*GPIO初始化*/
		
		//LED_CHK
	  //PD11
    GPIO_InitStruct.pin           = FL_GPIO_PIN_11;
    GPIO_InitStruct.mode = FL_GPIO_MODE_ANALOG;                                         /*配置GPIO的功能模式*/
    GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;                               /*配置GPIO的输出模式*/
    GPIO_InitStruct.pull = FL_DISABLE;                                                  /*配置GPIO输上拉模式*/
    GPIO_InitStruct.remapPin = FL_DISABLE;                                              /*配置GPIO数字重定向功能*/
    GPIO_InitStruct.analogSwitch = FL_DISABLE;                                          /*配置GPIO模拟开关功能*/
    (void)FL_GPIO_Init(GPIOD, &GPIO_InitStruct);                                        /*GPIO初始化*/
		
		//MOTOR_B_CHK
	  //PD5
    GPIO_InitStruct.pin           = FL_GPIO_PIN_5;
    GPIO_InitStruct.mode = FL_GPIO_MODE_ANALOG;                                         /*配置GPIO的功能模式*/
    GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;                               /*配置GPIO的输出模式*/
    GPIO_InitStruct.pull = FL_DISABLE;                                                  /*配置GPIO输上拉模式*/
    GPIO_InitStruct.remapPin = FL_DISABLE;                                              /*配置GPIO数字重定向功能*/
    GPIO_InitStruct.analogSwitch = FL_DISABLE;                                          /*配置GPIO模拟开关功能*/
    (void)FL_GPIO_Init(GPIOD, &GPIO_InitStruct);                                        /*GPIO初始化*/

		//MOTOR_R_CHK
	  //PD6
    GPIO_InitStruct.pin           = FL_GPIO_PIN_6;
    GPIO_InitStruct.mode = FL_GPIO_MODE_ANALOG;                                         /*配置GPIO的功能模式*/
    GPIO_InitStruct.outputType = FL_GPIO_OUTPUT_PUSHPULL;                               /*配置GPIO的输出模式*/
    GPIO_InitStruct.pull = FL_DISABLE;                                                  /*配置GPIO输上拉模式*/
    GPIO_InitStruct.remapPin = FL_DISABLE;                                              /*配置GPIO数字重定向功能*/
    GPIO_InitStruct.analogSwitch = FL_DISABLE;                                          /*配置GPIO模拟开关功能*/
    (void)FL_GPIO_Init(GPIOD, &GPIO_InitStruct);                                        /*GPIO初始化*/
	

    ADC_InitStruct.conversionMode = FL_ADC_CONV_MODE_SINGLE;                            /*配置ADC转换模式*/
    ADC_InitStruct.autoMode = FL_ADC_SINGLE_CONV_MODE_AUTO;                             /*配置ADC转换流程，仅对单次转换有效*/
    ADC_InitStruct.waitMode = FL_ENABLE;                                                /*配置ADC等待模式*/
    ADC_InitStruct.overrunMode = FL_ENABLE;                                             /*配置ADC_Overrun模式*/
    ADC_InitStruct.scanDirection = FL_ADC_SEQ_SCAN_DIR_FORWARD;                         /*配置ADC扫描顺序*/
    ADC_InitStruct.externalTrigConv = FL_ADC_TRIGGER_EDGE_NONE;                         /*配置非软件触发使能及极性*/
    ADC_InitStruct.triggerSource = FL_DISABLE;                                          /*配置ADC非软件触发源*/
    ADC_InitStruct.fastChannelTime = FL_ADC_FAST_CH_SAMPLING_TIME_32_ADCCLK;             /*配置ADC快速通道采样时间*/
    ADC_InitStruct.lowChannelTime = FL_ADC_SLOW_CH_SAMPLING_TIME_512_ADCCLK;            /*配置ADC慢速通道采样时间*/
    ADC_InitStruct.oversamplingMode = FL_DISABLE;                                       /*配置ADC过采样模式*/
    ADC_InitStruct.overSampingMultiplier = FL_ADC_OVERSAMPLING_MUL_16X;                 /*配置ADC过采样率*/
    ADC_InitStruct.oversamplingShift = FL_ADC_OVERSAMPLING_SHIFT_4B;                    /*配置ADC过采样结果移位*/
    (void)FL_ADC_Init(ADC, &ADC_InitStruct);

    FL_ADC_EnableSequencerChannel(ADC, FL_ADC_EXTERNAL_CH6);                            /*通道选择*/
    FL_ADC_EnableSequencerChannel(ADC, FL_ADC_INTERNAL_VREF1P2);
}

void MF_ADC_Config_Init(void)
{
    /* Initial ADC */
    MF_ADC_Common_Init();                                                               /*ADC基础配置*/
    MF_ADC_Init();                                                                      /*ADC初始化配置*/
}

static uint8_t GetVREF1P2Sample_POLL(uint32_t *ADCRdresult)
{
    uint32_t counter = 0;
    uint32_t EOC_Flag;
    uint8_t State;
    FL_CMU_SetADCPrescaler(FL_CMU_ADC_PSC_DIV8);                    //配置ADC工作时钟分频
    FL_VREF_EnableVREFBuffer(VREF);                                 //使能VREF BUFFER,
    //为了考虑功耗BUFFER可关闭，从使能到ADC可以正常采样需要100us的建立时间，本例程配置慢速通道较长的采样时间，来保证建立时间满足。
    FL_ADC_DisableSequencerChannel(ADC, FL_ADC_ALL_CHANNEL);        //清空打开的通道
    FL_ADC_EnableSequencerChannel(ADC, FL_ADC_INTERNAL_VREF1P2);    //通道选择

    FL_ADC_ClearFlag_EndOfConversion(ADC);                          //清标志
    FL_ADC_Enable(ADC);                                             //启动ADC
    FL_ADC_EnableSWConversion(ADC);                                 //开始转换
    
    do
    {
        EOC_Flag = FL_ADC_IsActiveFlag_EndOfConversion(ADC);
        counter++;
    } while((counter != 0xFFFFFFFFU) && (EOC_Flag == 0U));           //等待转换完成

    if(EOC_Flag == 0x01U)
    {
        FL_ADC_ClearFlag_EndOfConversion(ADC);                        //清标志
        *ADCRdresult = FL_ADC_ReadConversionData(ADC);                 //获取采样值

        FL_ADC_Disable(ADC);                                          //关闭ADC
        FL_ADC_DisableSequencerChannel(ADC, FL_ADC_INTERNAL_VREF1P2); //通道关闭
        FL_VREF_DisableVREFBuffer(VREF);                              //关闭VREF BUFFER
        State = 0;
    }
    else
    {
        State = 1 ;
    }

    return State;
}



static uint8_t GetSingleChannelSample_POLL(uint32_t channel, uint32_t *ADCRdresult)
{
    uint32_t counter = 0;
    uint32_t EOC_Flag;
    uint8_t State;

    FL_CMU_SetADCPrescaler(FL_CMU_ADC_PSC_DIV8);                  //配置ADC工作时钟分频
    FL_ADC_DisableSequencerChannel(ADC, FL_ADC_ALL_CHANNEL);      //清空打开的通道
    FL_ADC_EnableSequencerChannel(ADC, channel);                  //通道选择

    FL_ADC_ClearFlag_EndOfConversion(ADC);                        //清标志
    FL_ADC_Enable(ADC);                                           //启动ADC
    FL_ADC_EnableSWConversion(ADC);                               //开始转换


    do
    {
        EOC_Flag = FL_ADC_IsActiveFlag_EndOfConversion(ADC);
        counter++;
    } while((counter != 0xFFFFFFFFU) && (EOC_Flag == 0U));           //等待转换完成

    if(EOC_Flag == 0x01U)
    {
        FL_ADC_ClearFlag_EndOfConversion(ADC);                        //清标志
        *ADCRdresult = FL_ADC_ReadConversionData(ADC);                 //获取采样值

        FL_ADC_Disable(ADC);                                          //关闭ADC
        FL_ADC_DisableSequencerChannel(ADC, channel);                 //通道关闭
        State = 0;
    }
    else
    {
        State = 1 ;
    }
    return State;
}


uint32_t GetSingleChannelVoltage_POLL(uint32_t channel)
{
    uint32_t Get122VSample = 0, GetChannelVoltage = 0, GetVSample = 0;
    uint8_t  Get122VSample_State, GetVSample_State;
    Get122VSample_State = GetVREF1P2Sample_POLL(&Get122VSample);
    GetVSample_State = GetSingleChannelSample_POLL(channel, &GetVSample);

    if((Get122VSample != 0) && (Get122VSample_State == 0)  && (GetVSample_State == 0))
    {
        GetChannelVoltage = (uint32_t)(((uint64_t)GetVSample * 3000 * (ADC_VREF)) / ((uint64_t)Get122VSample * 4095));  //计算通道电压
    }

    return GetChannelVoltage;
}

/*————————7V2工装自检电压————————*/
uint32_t Get_7V2_Voltage(void)
{
	uint32_t test_shuju = 0;
	test_shuju = GetSingleChannelVoltage_POLL(FL_ADC_EXTERNAL_CH7);
	test_shuju = test_shuju*11;
	return test_shuju;
}

/*————————3V6工装自检电压————————*/
uint32_t Get_3V6_Voltage(void)
{
	uint32_t test_shuju = 0;
	test_shuju = GetSingleChannelVoltage_POLL(FL_ADC_EXTERNAL_CH1);
	test_shuju = test_shuju*11;
	return test_shuju;
}

/*————————AD1电压检测————————*/
uint32_t Get_AD1_Voltage(void)
{
	uint32_t test_shuju = 0;
	ADC_CHK_CTL(AD1,On);
	test_shuju = GetSingleChannelVoltage_POLL(FL_ADC_EXTERNAL_CH8);
	test_shuju = test_shuju*11;
	ADC_CHK_CTL(AD1,Off);
	return test_shuju;
}

/*————————AD2电压检测————————*/
uint32_t Get_AD2_Voltage(void)
{
	uint32_t test_shuju = 0;
	ADC_CHK_CTL(AD2,On);
	test_shuju = GetSingleChannelVoltage_POLL(FL_ADC_EXTERNAL_CH2);
	test_shuju = test_shuju*11;
	ADC_CHK_CTL(AD2,Off);
	return test_shuju;
}

/*————————AD3电压检测————————*/
uint32_t Get_AD3_Voltage(void)
{
	uint32_t test_shuju = 0;
	ADC_CHK_CTL(AD3,On);
	test_shuju = GetSingleChannelVoltage_POLL(FL_ADC_EXTERNAL_CH9);
	test_shuju = test_shuju*11;
	ADC_CHK_CTL(AD3,Off);
	return test_shuju;
}


/*————————MOTOR_B电压检测————————*/
uint32_t Get_Motor_B_Voltage(void)
{
	uint32_t test_shuju = 0;
	test_shuju = GetSingleChannelVoltage_POLL(FL_ADC_EXTERNAL_CH3);
	test_shuju = test_shuju*11;
	return test_shuju;
}


/*————————MOTOR_R电压检测————————*/
uint32_t Get_Motor_R_Voltage(void)
{
	uint32_t test_shuju = 0;
	test_shuju = GetSingleChannelVoltage_POLL(FL_ADC_EXTERNAL_CH10);
	test_shuju = test_shuju*11;
	return test_shuju;
}

/*————————LED电压检测————————*/
uint32_t Get_LED_Voltage(void)
{
	uint32_t test_shuju = 0;
	test_shuju = GetSingleChannelVoltage_POLL(FL_ADC_EXTERNAL_CH0);
	test_shuju = test_shuju*11;
	return test_shuju;
}
