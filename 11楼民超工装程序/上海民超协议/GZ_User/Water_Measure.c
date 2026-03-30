#include "Water_Measure.h"
#include "Struct.h"
#include "GP30.h"
#include "IOIntp.h"
#include "Irda.h"
unsigned char TMPTimeTime = 0;
float TMPTimeValue[10];
float Trab[2];
float Tcab[2];
float Rc;
float Interval_125ms_SumValue;
float Interval_1S_SumValue;
void WaterMeasure()
{
  if(GP30_Data.SRR_FEP_STF_E2&0x01)
  {
    
  }
  else if(GP30_Data.SRR_FEP_STF_E2&0x40)/////TimeValue_Measure
  {
    float tmpval;
    GP30_Data.FDB_US_TOF_SUM_OF_ALL_U = GP30ReadBytes(GP30_RC_RAA_ADR_80);
    GP30_Data.FDB_US_TOF_SUM_OF_ALL_D = GP30ReadBytes(GP30_RC_RAA_ADR_84);
    tmpval= GP30_Data.FDB_US_TOF_SUM_OF_ALL_U - GP30_Data.FDB_US_TOF_SUM_OF_ALL_D;
    Interval_125ms_SumValue +=tmpval;
    Interval_125ms_SumValue -= TMPTimeValue[TMPTimeTime];
    Interval_1S_SumValue += Interval_125ms_SumValue;
    TMPTimeValue[TMPTimeTime++] = tmpval;
    if(TMPTimeTime >= 8)
    {
      TMPTimeTime = 0;
      //Interval_1S_SumValue /=64;////put this divide last step!!
      if(Interval_1S_SumValue < 0)
      {
        TempUserData.WaterFlow = 0;
      }
      else
      {
        if(Interval_1S_SumValue <40265.3184)////(dift*250)/(64*12*65536) < 0.2
        {
          TempUserData.WaterFlow = 0;
        }
        else
        {
          Interval_1S_SumValue =Interval_1S_SumValue*250;
          //Interval_1S_SumValue = Interval_1S_SumValue/50331648;//64*12*65536-put this down;
          TempUserData.WaterFlow = TempUserData.Compensation_C*Interval_1S_SumValue;
          TempUserData.WaterFlow = TempUserData.WaterFlow/33659321*50331648;////t*C2/2L;
          TempUserData.ALLTpVolume +=TempUserData.WaterFlow*Csb_Timer.GP30IntpCounter;
        }
      }
      
      Interval_1S_SumValue = 0;////////////Clear-1s-Sum;
      
      TempUserData.FlowRate[TempUserData.Time++] = (unsigned long)TempUserData.WaterFlow;
      if(TempUserData.Time >= 5)
      {
        TempUserData.Time = 0;
      }
      Csb_Timer.GP30IntpTimer +=Csb_Timer.GP30IntpCounter;
      if(Csb_Timer.GP30IntpTimer > 999999999)
      {
        Csb_Timer.GP30IntpTimer -=999999999;
      }
      Csb_Timer.GP30IntpCounter = 0;
    }
  }
  else if(GP30_Data.SRR_FEP_STF_E2&0x02)/////Temprature_Measure
  {
    GP30ReadTMPData();
    Trab[0] = (float)GP30_Data.TempBuff[1];
    Tcab[0] = (float)GP30_Data.TempBuff[2];
    Trab[1] = (float)GP30_Data.TempBuff[6];
    Tcab[1] = (float)GP30_Data.TempBuff[7];
    Trab[0] = Trab[0]/Tcab[0];
    Trab[1] = Trab[1]/Tcab[1];
    Rc = Trab[0]+Trab[1];
    //Rc = Rc*Rref/2;
    Rc = Rc*5000;
    TempUserData.Compensation_T = T_NTC(Rc);
    TempUserData.Compensation_C = C_Mar_Equ(TempUserData.Compensation_T);
  }
  else if(GP30_Data.SRR_FEP_STF_E2&0x100)
  {
    
  }
  //Check_AM();
}
void Check_AM()
{
  static unsigned char Check_AM_Time = 0;
  if((GP30_Data.UP_AM_Value != 50)&&(GP30_Data.DOWN_AM_Value != 50))
  {
    Check_AM_Time++;
    if(Check_AM_Time > 50)
    {
      Check_AM_Time = 0;
      GP30_Data.UP_AM_Value = GP30ReadBytes(GP30_RC_RAA_ADR_81);
      GP30_Data.DOWN_AM_Value = GP30ReadBytes(GP30_RC_RAA_ADR_85);
      if((GP30_Data.UP_AM_Value > 60)||(GP30_Data.UP_AM_Value < 30))
      {
        GP30_Data.UP_AM_AdjustFlag++;
        if(GP30_Data.UP_AM_AdjustFlag > 10)
        {
          GP30_Data.UP_AM_AdjustFlag = 50;
        }
      }
      
      if((GP30_Data.DOWN_AM_Value > 60)||(GP30_Data.DOWN_AM_Value < 30))
      {
        GP30_Data.DOWN_AM_AdjustFlag++;
        if(GP30_Data.DOWN_AM_AdjustFlag >10)
        {
          GP30_Data.DOWN_AM_AdjustFlag = 50;
        }
      }
    }
  }
  if(GP30_Data.UP_AM_AdjustFlag == 50)
  {
    GP30_Data.UP_AM_Value = GP30ReadBytes(GP30_RC_RAA_ADR_81);
    if(GP30_Data.UP_AM_Value> 55)
    {
      GP30_Data.UP_AM_Para -=2;
    }
    else if(GP30_Data.UP_AM_Value < 35)
    {
      GP30_Data.UP_AM_Para +=2;
    }
    else
    {
      GP30_Data.UP_AM_AdjustFlag = 0;
    }
    GP30WriteCMDBytes(GP30_RC_RAA_ADR_DA,GP30_Data.UP_AM_Para);
  }
  if(GP30_Data.DOWN_AM_AdjustFlag == 50)
  {
    GP30_Data.DOWN_AM_Value = GP30ReadBytes(GP30_RC_RAA_ADR_85);
    if(GP30_Data.DOWN_AM_Value> 55)
    {
      GP30_Data.DOWN_AM_Para -=2;
    }
    else if(GP30_Data.UP_AM_Value < 35)
    {
      GP30_Data.DOWN_AM_Para +=2;
    }
    else
    {
      GP30_Data.DOWN_AM_AdjustFlag = 0;
    }
    GP30WriteCMDBytes(GP30_RC_RAA_ADR_DB,GP30_Data.DOWN_AM_Para);
  }
}
void ScanLXD()
{
  static unsigned char ScanLXDOFFTime = 0;
  static unsigned char ScanLXDONTime = 0;
  static unsigned char ScanLXDGDFlag = 0;
  static unsigned char ScanLXDLXDFlag = 0;
  static unsigned short ScanIntpTime = 0;
  LXD_POWER(1);
  Delay_ms(1);
  if(!LXD_INPUT)
  {
    ScanLXDONTime = 0;
    if(ScanLXDOFFTime < 2)
    {
      ScanLXDOFFTime++;
    }
    else
    {
      if(!ScanLXDGDFlag)
      {
        ScanLXDGDFlag = 1;
        IRDA_POWER(1);
        Start_Intp(IRDA_INTP);
        
        ScanLXDLXDFlag = 0;
        Stop_Intp(KEY_INTP);
      }
    }
  }
  else
  {
    ScanLXDOFFTime = 0;
    if(ScanLXDONTime < 2)
    {
      ScanLXDONTime++;
    }
    else
    {
      if(ScanLXDGDFlag)
      {
        ScanLXDGDFlag = 0;
        IRDA_POWER(0);
        Stop_Intp(IRDA_INTP);
        IrdaRX_EN(0);
        
        ScanLXDLXDFlag = 1;
        ScanIntpTime = 0;
        Start_Intp(KEY_INTP);
      }
    }
  }
  if(ScanLXDLXDFlag)
  {
    ScanIntpTime++;
    if(ScanIntpTime>1800)
    {
      ScanIntpTime = 0;
      ScanLXDLXDFlag = 0;
      Stop_Intp(KEY_INTP);
    }
  }
  else
  {
    LXD_POWER(0);
  }
}