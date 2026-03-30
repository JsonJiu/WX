#include "OneShot_AD.h"
#include "r_cg_adc.h"
void AD_Init()
{
	R_ADC_Create();
        R_ADC_Set_OperationOff();
        R_ADC_Stop();
        R_ADC_Set_PowerOff();
}
unsigned long CheckPower(unsigned char adtype)
{
  unsigned char VPP_Value_i;
  unsigned short VPP_Time;
  unsigned short VPP_Value_Min,VPP_Value_Max,VPP_Value_Buff[5];
  unsigned long Last_VPP_Value_Result=0;
	if(adtype == 1)
	{
		Change_ADC_Channel(1);
	}
	else
	{
		Change_ADC_Channel(0);
	}
  R_ADC_Set_OperationOn();
  for(VPP_Value_i=0;VPP_Value_i<5;VPP_Value_i++)
  {
    VPP_Time=65530;
    R_ADC_Start();
    while((ADCS)&&(VPP_Time--));
    VPP_Value_Min =ADCRH;
    VPP_Value_Buff[VPP_Value_i] = VPP_Value_Min;
    Last_VPP_Value_Result += VPP_Value_Min;
		Delay_ms(2);
  }
  R_ADC_Stop();
  R_ADC_Set_OperationOff();
  R_ADC_Set_PowerOff();
  VPP_Value_Min=VPP_Value_Buff[1];
  VPP_Value_Max=VPP_Value_Buff[1];
  for(VPP_Value_i=1;VPP_Value_i<5;VPP_Value_i++)
  {
    if(VPP_Value_Min > VPP_Value_Buff[VPP_Value_i])
    {
      VPP_Value_Min=VPP_Value_Buff[VPP_Value_i];
    }
    if(VPP_Value_Max < VPP_Value_Buff[VPP_Value_i])
    {
      VPP_Value_Max=VPP_Value_Buff[VPP_Value_i];
    }
  }
  Last_VPP_Value_Result -= VPP_Value_Min;
  Last_VPP_Value_Result -= VPP_Value_Max;
  Last_VPP_Value_Result -= VPP_Value_Buff[0];
  Last_VPP_Value_Result = Last_VPP_Value_Result*4350;
  Last_VPP_Value_Result /= 512;
  return Last_VPP_Value_Result;
}