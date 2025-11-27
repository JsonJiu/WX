/**
 *========================================更新记录====================================
 * -- 更新时间--------更新类型---------------------------更新内容-----------------------
 * 
 *   2015-04-18      增加功能     增加商业表阀门控制功能，采用非阻塞式定时扫描控制阀门，需要
 *                                在工程文件中增加宏定义：#define COMMERCE_METER
 * ------------------------------------------------------------------------------------
 *====================================================================================
 *==========================文件说明：阀门控制逻辑（民用表）============================
 * 外部调用函数：
 * （一）  void OV_Valve_Init(unsigned char flag_PowerOnOpenValve, unsigned char flag_OwerVol_CloseValve)；
 *         =====================阀门初始化；放在初始化处===========================
 * @@@@@@@@@@@@@@@ 示例 ：OV_Valve_Init(POWERON_OPEN, OWER_NOTCLOSEVALVE); 配置为上电自动开阀、欠压不关阀；@@@@@@@@@@
 *     参数一：   flag_PowerOnOpenValve：上电是否开阀的标志：     POWERON_OPEN              -----  开阀；
 *                                                              POWERON_NOTOPEN           ----   不开阀
 *     参数二：   flag_OwerVol_CloseValve：欠压是否关阀的标志：   OWER_CLOSEVALVE           ----- 欠压关阀
 *                                                              OWER_NOTCLOSEVALVE        ----  欠压不关阀
 *     默认阀门为丹东阀门，
 *     不带到位；开阀时间为：800mS;关阀时间：1500mS;
 *     如果君想配置为到到位的4线制三洋阀门：请在工程宏定义栏定义：DEF_OPEN_DAOWEI 和  DEF_CLOSE_DAOWEI;即可；
 *     带到位阀门，开阀时间为：1500mS,关阀时间为：300mS;
 * （二）  void OV_ValveControlLogic(void);
 *        =====================阀门控制逻辑；放在while（1）循环里==================
 *      阀门控制逻辑基于事件触发相应的阀门控制权；
 *  			R_KEY（0x01）	      //按键控制权
 *  			R_VOLTAGE(0x02)		//电压控制权
 *  			R_EVENT(0x04)		//磁攻击控制权
 *  			R_COMMUNICATION(0x08)   //通讯控制权
 *  			R_CONSUME(0x10)         //消费控制权
 * （三）其他函数说明
 *  extern void OV_OpenValve_Opt(unsigned int ms);          //带时间的强制开阀
 *  extern void OV_CloseValve_Opt(unsigned int ms);         //带时间的强制关阀
 *  extern unsigned char SetValveParaValue(T_ValveValue valvevalue_type, unsigned short val);//设置阀门参数
 *  extern unsigned short GetValveParaValue(T_ValveValue valvevalue_type);                   //读取阀门参数
 *  extern void Set_ValveControlRight(unsigned char type);                                   //设置阀门控制权
 *  extern void Reset_ValveControlRight(unsigned char type);                                 //清阀门控制权
 *  extern VALVE_STATUS_EU Get_ValveStatus(void);                                            //获取阀门状态
 *  extern unsigned char Get_FlagValveOverCurrrent(void);                                    //获取阀门过流的标志
 *  extern void Set_CommunicateOpenCloseValveFlag(unsigned char flag);                       //设置通讯开关阀的标志（强制开：OPT_OPEN_VALVE，强制关：OPT_CLOSE_VALVE，退出强制：OPT_EXIT_CONTROL）
 *  extern unsigned char Get_CommunicateOpenCloseValveFlag(void);                            //获取通讯开关阀的标志（强制开：OPT_OPEN_VALVE，强制关：OPT_CLOSE_VALVE，退出强制：OPT_EXIT_CONTROL）
 *  extern void Set_BeepAlarm_Flag(void);                                                    //设置计量每走一个脉冲的标志
 *  extern void ClearBeepAlarm_Flag(void);                                                   //清除走脉冲的标志
 *  extern unsigned char GetBeepAlarm_Flag(void);                                            //获取走脉冲的标志
 */

#include "drv_valve.h"
#include "app_valve_control.h"
#include "drv_ad.h"
#include "app_general.h"
#include "app_powermanage.h"
#include "include.h"
#include "timer_manage.h"
#include "user_manage.h"


extern SysPowState_ET GetSysPowState(void);
extern int IsSysPowUndering48(void);
extern int IsSysPowNormalToUnder(void);
extern void ClrSysPowNormalToUnder(void);

extern void DelayMs(unsigned int Ms);
extern void OA_ADC_Init(ADC_InitTypeDef* ADC_InitStruct);
extern unsigned short OA_ADC_GetValue(ADCChannel_ET AdcChannel);
extern unsigned char Get_ValveControl_type(void);

unsigned char Flag_BeepAlrm = 0;
unsigned char PowerOnOpenValve_Temp;
unsigned char OwerVol_CloseValve_Temp;

unsigned char VALVE_OPT_TIME_NUM = 0;
unsigned char CHECK_OVERCURRENT_TIME_NUM = 0;

VALVE_STATUS_EU VALVE_STATUS;
VALVE_ST Valve = {
                    NO_STARUS,  //阀门状态
                    0,          //阀门控制权
                    8,          //开阀时间
                    15,         //关阀时间
                    15,          //二次关阀关阀时间
                    0,          //二次开阀开阀时间
                    500,        //过流值(500MA)
                    OPT_EXIT_CONTROL,          //阀门强制标志（退出强制）
                    POWERON_OPEN,
                    OWER_CLOSEVALVE,
                    3000,       //商业表开关阀超时时间（默认值为300S=5分钟）
                    0,          //商业表阀门操作定时
                    0,          //检测过流的定时
                    0           //阀门过流的标志
                };

static ADC_InitTypeDef ADC_InitPara_Cur =
{
    0x00,0x01,0x0A,0x01//单次；阀门电流；10位AD;参考电压VDD
};

const St_RegTxt ValveRegTag[] = {
    {sizeof(Valve.Status                        )+0x30, "阀门状态"},     //
    {sizeof(Valve.Right_Control                 )+0x30, "阀控制权"},     //
    {sizeof(Valve.TimeValue_OpenValve           )+0x30, "开阀时间"},     //
    {sizeof(Valve.TimeValue_CloseValve          )+0x30, "关阀时间"},     //
    {sizeof(Valve.TimeValue_TwoCloseValve       )+0x30, "二关时间"},     //
    {sizeof(Valve.TimeValue_TwoOpenValve        )+0x30, "二开时间"},     //
    {sizeof(Valve.Value_OverCurrent             )+0x30, "阀过流值"},     //
    {sizeof(Valve.ParaFlag.PowerOnOpenValve     )+0x30, "上电关阀"},
    {sizeof(Valve.ParaFlag.OwerVol_CloseValve   )+0x30, "欠压关阀"},
    {sizeof(Valve.OpenCloseOverTime_Time        )+0x30, "超时时间"},
    {sizeof(Valve.Flag_OverCurrent              )+0x30, "过流标志"},
};

/**
 * 阀门参数初始化
 */
void OV_ValveDataSvrDefault(void)
{
#ifdef COMMERCE_METER
        Valve.Status = NO_STARUS;
        Valve.Right_Control= 0;
        Valve.TimeValue_OpenValve = 15;
        Valve.TimeValue_CloseValve = 3;
        Valve.TimeValue_TwoCloseValve = 3;
        Valve.TimeValue_TwoOpenValve = 0;
        Valve.Value_OverCurrent = 500;
        Valve.Flag_Communicate_OpenCloseValve = OPT_EXIT_CONTROL;
        Valve.Flag_OverCurrent = 0;
        Valve.ParaFlag.PowerOnOpenValve = PowerOnOpenValve_Temp;
        Valve.ParaFlag.OwerVol_CloseValve = OwerVol_CloseValve_Temp;
        Valve.OpenCloseOverTime_Time = 3000;
#else
    #ifdef DEF_OPEN_DAOWEI
        Valve.Status = NO_STARUS;
        Valve.Right_Control= 0;
        Valve.TimeValue_OpenValve = 15;
        Valve.TimeValue_CloseValve = 3;
        Valve.TimeValue_TwoCloseValve = 3;
        Valve.TimeValue_TwoOpenValve = 0;
        Valve.Value_OverCurrent = 500;
        Valve.Flag_Communicate_OpenCloseValve = OPT_EXIT_CONTROL;
        Valve.Flag_OverCurrent = 0;
        Valve.ParaFlag.PowerOnOpenValve = PowerOnOpenValve_Temp;
        Valve.ParaFlag.OwerVol_CloseValve = OwerVol_CloseValve_Temp;
        Valve.OpenCloseOverTime_Time = 3000;
    #else
        Valve.Status = NO_STARUS;
        Valve.Right_Control= 0;
        Valve.TimeValue_OpenValve = 8;
        Valve.TimeValue_CloseValve = 15;
        Valve.TimeValue_TwoCloseValve = 15;
        Valve.TimeValue_TwoOpenValve = 0;
        Valve.Value_OverCurrent = 250;
        Valve.Flag_Communicate_OpenCloseValve = OPT_EXIT_CONTROL;
        Valve.Flag_OverCurrent = 0;
        Valve.ParaFlag.PowerOnOpenValve = PowerOnOpenValve_Temp;
        Valve.ParaFlag.OwerVol_CloseValve = OwerVol_CloseValve_Temp;
        Valve.OpenCloseOverTime_Time = 3000;
    #endif
#endif
}

/**
 *  注册阀门存储空间
 */
void RegValveData(void)
{
   St_SAVESTRUCT temp;
   temp.fun = OV_ValveDataSvrDefault;
   temp.data = (unsigned char *)&Valve.Right_Control;
   temp.len = sizeof(Valve)-4;
   DataSvrReg(sizeof(ValveRegTag) / 9, &temp, (St_RegTxt *)&ValveRegTag);
}

/**
 *  阀门参数初始化
 * @param flag_PowerOnOpenValve：上电是否直接开阀的标志
 * @param flag_OwerVol_CloseValve：欠压是否关阀的标志
 */
void OV_Valve_Init(unsigned char flag_PowerOnOpenValve, unsigned char flag_OwerVol_CloseValve)
{
    Valve.ParaFlag.PowerOnOpenValve = flag_PowerOnOpenValve;
    Valve.ParaFlag.OwerVol_CloseValve = flag_OwerVol_CloseValve;
    PowerOnOpenValve_Temp = flag_PowerOnOpenValve;
    OwerVol_CloseValve_Temp = flag_OwerVol_CloseValve;
    RegValveData();
#ifdef DEF_OPEN_DAOWEI
    GATE_CH1_IN();
#endif
    
#ifdef COMMERCE_METER
  if(VALVE_OPT_TIME_NUM == 0)
  {
     VALVE_OPT_TIME_NUM = User_AskTimeNumber();
  }
  if(CHECK_OVERCURRENT_TIME_NUM == 0)
  {
     CHECK_OVERCURRENT_TIME_NUM = User_AskTimeNumber();
  }
  GATE_CH1_IN();
  GATE_CH2_IN();
#endif
}

#ifdef COMMERCE_METER
    /**
     * 阀门到位、超时、或过流，停止阀门处理
     * @return
     */
    void OV_Valve_Stop_Opt(void)
    {
        Stop_Valve();
        GATE_CTL_GPIO(0);
        User_CloseTime(VALVE_OPT_TIME_NUM);
        User_CloseTime(CHECK_OVERCURRENT_TIME_NUM);
    }

    /**
     * 检测阀门过流
     */
    void OV_Check_ValveOverCurrent(void)
    {
        long ad_value;
        Valve.Flag_OverCurrent = 0;
        OA_ADC_Init(&ADC_InitPara_Cur);
        ad_value = (unsigned long)OA_ADC_GetValue(ADCCHANNELCUR);
        ad_value = (ad_value * AdcRefVolt) /1024 * 2;
        if(ad_value >= Valve.Value_OverCurrent)
        {
            Valve.Status = ERR_STATUS;
            Valve.Flag_OverCurrent = 1;
            OV_Valve_Stop_Opt();
        }
    }

#endif


#ifdef COMMERCE_METER
    /**
     * 检测商业表阀门开到位
     */
    void OV_Check_OpenDaoWei(void)
    {
        Valve.CommerceMeter_OptTime--;
        if(Valve.CommerceMeter_OptTime)
        {
            Valve.Status = OPENING_STATUS;
            if(Get_GATE_CH1_Status() == 0)
            {
                Valve.Status = OPEN_STATUS;
                OV_Valve_Stop_Opt();
            }
        }
        else
        {
            Valve.Status = ERR_STATUS;
            OV_Valve_Stop_Opt();
        }
    }
#else
    /**
     * 检测民用表阀门开到位；
     * @return  :1：开到位；0：开位到位
     */
    unsigned char OV_Check_OpenDaoWei(void)
    {
    #ifdef DEF_OPEN_DAOWEI
        GATE_CTL_GPIO(1);
        DelayMs(10);
        if(Get_GATE_CH1_Status() == 0)
        {
            DelayMs(50);
            GATE_CTL_GPIO(0);
            return 1;
        }
        GATE_CTL_GPIO(0);
    #endif
    return 0;
    }
#endif

#ifdef COMMERCE_METER
    /**
     * 检测商业表阀门关到位
     */
    void OV_Check_CloseDaoWei(void)
    {
        Valve.CommerceMeter_OptTime--;
        if(Valve.CommerceMeter_OptTime)
        {
            Valve.Status = CLOSING_STATUS;
            if(Get_GATE_CH2_Status() == 0)
            {
                Valve.Status = CLOSE_STATUS;
                OV_Valve_Stop_Opt();
            }
        }
        else
        {
            Valve.Status = ERR_STATUS;
            OV_Valve_Stop_Opt();
        }
    }
#else
    /**
     * 检测民用表阀门关到位；
     * @return  ：1：关到位；0：关未到位
     */
    unsigned char OV_Check_CloseDaoWei(void)
    {
    #ifdef DEF_CLOSE_DAOWEI
        GATE_CTL_GPIO(1);
        DelayMs(10);
        if(Get_GATE_CH1_Status() == 1)
        {
            DelayMs(50);
            GATE_CTL_GPIO(0);
            return 1;
        }
        GATE_CTL_GPIO(0);
    #endif
        return 0;
    }
#endif

#ifdef COMMERCE_METER
    /**
     *
     * @param type:0-OPT_OPEN_VALVE;开阀操作；
     *              1-OPT_CLOSE_VALVE;关阀操作；
     */
    void OV_OptValve_Control(unsigned char type)
    {
        Valve.CommerceMeter_OptTime = Valve.OpenCloseOverTime_Time;
        User_StartTime(MAINTIMER, CHECK_OVERCURRENT_TIME_NUM, 100, OV_Check_ValveOverCurrent);
        GATE_CTL_GPIO(1);
        DelayMs(10);
        if(type == OPT_OPEN_VALVE)
        {
            if(OV_Check_OpenDaoWei() == 1)
            Open_Valve();
            User_StartTime(MAINTIMER, VALVE_OPT_TIME_NUM, 100, OV_Check_OpenDaoWei);
        }
        else
        {
            if(OV_Check_CloseDaoWei() == 1)
            Close_Valve();
            User_StartTime(MAINTIMER, VALVE_OPT_TIME_NUM, 100, OV_Check_CloseDaoWei);
        }
    }
#endif

/**
* 开阀操作-强制开阀，时间可调，时基100MS
* 包括过流保护功能
* @author wb (2014-11-21)
*/
void OV_OpenValve_Opt(unsigned int ms)
{
#ifdef COMMERCE_METER
    Valve.Status = NO_STARUS;
    OV_OpenValve();
#else
    unsigned char i;
    Valve.Status = OPENING_STATUS;

    Open_Valve();
    i = ms;
    while(i--)
    {
        DelayMs(100);
        if(OV_Check_OpenDaoWei() == 1) break;
    }
    Stop_Valve();
    Valve.Status = OPEN_STATUS;
#endif
}

/**
 * 强制开阀
 * @param ms：开阀时间；100ms单位
 * 包括过流保护功能
 */
void OV_CloseValve_Opt(unsigned int ms)
{
#ifdef COMMERCE_METER
    Valve.Status = NO_STARUS;
    OV_CloseValve();
#else
    unsigned char i;
    Valve.Status = CLOSING_STATUS;
    Close_Valve();
    i = ms;
    while(i--)
    {
        DelayMs(100);
        if(OV_Check_CloseDaoWei() == 1) break;
    }
    Stop_Valve();
    Valve.Status = CLOSE_STATUS;
#endif
}

/**
* 开阀操作
* 包括过流保护功能
* @author wb (2014-11-21)
*/
void OV_OpenValve(void)
{
#ifdef COMMERCE_METER
    if((Valve.Status == CLOSE_STATUS) || (Valve.Status == NO_STARUS))
    {
        Valve.Status = OPENING_STATUS;
        OV_OptValve_Control(OPT_OPEN_VALVE);
        ClearLeakTime();
    }
#else
    if(Valve.Status == OPEN_STATUS) return;
    if(Valve.Status == ERR_STATUS)  return;
    OV_OpenValve_Opt(Valve.TimeValue_OpenValve);
    ClearLeakTime();
#endif
}

/**
* 关阀操作:一次关阀和二次关阀；
* 包括过流保护功能
* @author wb (2014-11-21)
*/
void OV_CloseValve(void)
{
#ifdef COMMERCE_METER
    if((Valve.Status == OPEN_STATUS) || (Valve.Status == NO_STARUS))
    {
        Valve.Status = CLOSING_STATUS;
        OV_OptValve_Control(OPT_CLOSE_VALVE);
    }
#else
    unsigned char i;
    long ad_value;
    if(Valve.Status == CLOSE_STATUS) return;
    if(Valve.Status == ERR_STATUS)   return;
    if(Valve.TimeValue_TwoCloseValve)//表示有阀门,二次关阀的第一次关阀
    {
        Valve.Status = CLOSING_STATUS;
        Close_Valve();
        i = Valve.TimeValue_TwoCloseValve;
        while(i--)
        {
            DelayMs(100);
        }
        Stop_Valve();
        DelayMs(100);
        
        if(Valve.TimeValue_TwoOpenValve)//表示二次关阀的第二次开阀
        {
            Open_Valve();
            i = Valve.TimeValue_TwoOpenValve;
            while(i--)
            {
                DelayMs(100);
            }
            Stop_Valve();
            DelayMs(100);
            if(Valve.TimeValue_CloseValve)//二次关阀的关
            {
                Close_Valve();
                i = Valve.TimeValue_CloseValve;
                while(i--)
                {
                    DelayMs(100);
                    if(OV_Check_CloseDaoWei() == 1) break;
                }
                Stop_Valve();
            }
        }
        Valve.Status = CLOSE_STATUS;
    }
#endif
}

/**
* 控制逻辑
* （掉电，低压，欠压）>（磁攻击事件）>(远程控制)>（扣费透支）
* 开阀动作:由磁攻击、远程控制、扣费引起的关阀，均需要通过按键开阀；
*    	由掉电，欠压，低压引起的关阀，电压正常时直接开阀；
*    	上电时，条件符合，直接开阀。然后再调用阀门控制逻辑
* @author wb (2014-11-22)
*/
void OV_ValveControlLogic(void)
{
    if(GetErrCode() == GATEERR_Leak)
    {
        SetErrCode(GATEERR_Null);
        OV_CloseValve_Opt(8);
    }
#if CntPay_Device == CntPay_HALL
#ifdef COMMERCE_METER
    if((GpioHallARead() == 0))
    {
        SetState(STATE_MEASURE, STATE_MEASURE_BROKEN);
    }
#else
    if((GpioHallARead() == 0)  && (GpioHallBRead() == 0))
    {
        Set_ValveControlRight(R_EVENT);
        SetState(STATE_MEASURE, STATE_MEASURE_BROKEN);
    }
#endif
#endif
    
    if((Valve.Right_Control & R_KEY) !=  R_KEY)//无按键控制权
    {
        if((Valve.Right_Control & R_VOLTAGE) != R_VOLTAGE)//欠压、低压、掉电关阀事件解除
        {
            if((Valve.Right_Control & R_EVENT) != R_EVENT)//磁攻击关阀事件解除
            {
                if((Valve.Right_Control & R_COMMUNICATION) != R_COMMUNICATION)//远程强制关阀解除(退出强制)
                {
                    if(GetSetupVal(SETUP_PostPaid) == SETUP_PostPaid_ON)//预付费
                    {
                        if((Valve.Right_Control & R_CONSUME) != R_CONSUME)//消费（少额、欠费、透支）关阀解除
                        {
                            if(Valve.ParaFlag.PowerOnOpenValve == POWERON_OPEN)//上电直接开阀
                            {
                                if(GetState(STATE_REMAIN) < STATE_REMAIN_NEGTIVEOVER)   OV_OpenValve();
                            }
                        }
                        else
                           OV_CloseValve();
                    }
                    else //后付费
                    {
                        if(Valve.ParaFlag.PowerOnOpenValve == POWERON_OPEN) OV_OpenValve();
                    }
                }
                else
                {
                    if(Get_CommunicateOpenCloseValveFlag() == OPT_CLOSE_VALVE) OV_CloseValve();
                }
            }
            else
                OV_CloseValve();
        }
        else
            OV_CloseValve();
    }
    else
    {
        Reset_ValveControlRight(R_KEY);
        if(Valve.Status == ERR_STATUS)  Valve.Status = NO_STARUS;
        if(GetSysPowState() >= STATUS_POW_LOW) return;
        if(IsSysPowUndering48() <  2)//电压正常或欠压48小时内；
        {
            if(IsSysPowUndering48() == 1) Reset_ValveControlRight(R_VOLTAGE);
            if((Valve.Right_Control & R_EVENT) != R_EVENT)//磁攻击关阀事件解除
            {
                if((Valve.Right_Control & R_COMMUNICATION) != R_COMMUNICATION)//远程强制关阀解除(退出强制)
                {
                    if(GetSetupVal(SETUP_PostPaid) == SETUP_PostPaid_ON)//预付费
                    {
                        if((Valve.Right_Control & R_CONSUME) != R_CONSUME)//消费（少额、欠费、透支）关阀解除
                            OV_OpenValve();
                        else
                        {
                            if(GetState(STATE_REMAIN) < STATE_REMAIN_NEGTIVEOVER)
                            {
                                OV_OpenValve();
                                Reset_ValveControlRight(R_CONSUME);
                            }
                        }
                    }
                    else
                        OV_OpenValve();
                }
                else 
                {
                    if(Get_CommunicateOpenCloseValveFlag() == OPT_OPEN_VALVE)   OV_OpenValve();
                }
            }
            else
            {
#if CntPay_Device == CntPay_HALL
                if(GpioHallARead() || GpioHallBRead())
                {
                    SetState(STATE_MEASURE, STATE_MEASURE_NORMAL);
                    Reset_ValveControlRight(R_EVENT);
                }
#endif
                if(GetErrCode() == GATEERR_OverFlow)
                {
                    SetErrCode(GATEERR_Null);
                    Reset_ValveControlRight(R_EVENT);
                }
                if(GetErrCode() == GATEERR_UnUsedTimeOver)
                {
                    SetErrCode(GATEERR_Null);
                    Reset_ValveControlRight(R_EVENT);
                }
            }
        }
    }
}

/**
* 置阀门控制权
*
* @author wb (2014-11-22)
*
* @param type ：
*  			R_KEY（0x01）		//按键控制权
*  			R_VOLTAGE(0x02)		//电压控制权
*  			R_EVENT(0x04)		//磁攻击控制权
*  			R_COMMUNICATION(0x08)   //通讯控制权
*  			R_CONSUME(0x10)         //消费控制权
*/

void Set_ValveControlRight(unsigned char type)
{
    Valve.Right_Control |= type;
}

/**
* 获取阀门控制权R_KEY（0x01）		   /按键控制权
*  		R_VOLTAGE(0x02)		   //电压控制权
*  		R_EVENT(0x04)		   //磁攻击控制权
*  		R_COMMUNICATION(0x08)      //通讯控制权
*    		R_CONSUME(0x10)            //消费控制权
* @author wb (2014-11-24)
*
* @return unsigned char
*/

unsigned char Get_ValveControl_type(void)
{
    return Valve.Right_Control;
}

/**
* 清除阀门控制权
*
* @author wb (2014-11-22)
*
* @param type ：
*  			R_KEY（0x01）		//按键控制权
*  			R_VOLTAGE(0x02)		//电压控制权
*  			R_EVENT(0x04)		//磁攻击控制权
*  			R_COMMUNICATION(0x08)   //通讯控制权
*  			R_CONSUME(0x10)         //消费控制权
*/

void Reset_ValveControlRight(unsigned char type)
{
    Valve.Right_Control &= ~type;
}

/**
* 获取阀门状态
*
* @author wb (2014-11-24)
*
* @return VALVE_STATUS_EU
*    	  ：阀门状态，见VALVE_STATUS_EU枚举变量值
*/

VALVE_STATUS_EU Get_ValveStatus(void)
{
    return Valve.Status;
}

/**
* 获取阀门过流的标志
*
* @author wb (2014-11-24)
*
* @return 1：阀门过流；0：阀门电流正常
*/

unsigned char Get_FlagValveOverCurrrent(void)
{
    return Valve.Flag_OverCurrent;
}

/**
* 设置通讯开关阀的标志
*
* @author wb (2014-11-24)
*
* @param flag ： flag :OPT_OPEN_VALVE  开阀；OPT_CLOSE_VALVE：关阀
*/

void Set_CommunicateOpenCloseValveFlag(unsigned char flag)
{
    Valve.Flag_Communicate_OpenCloseValve = flag;
    switch(flag)
    {
        case OPT_OPEN_VALVE:
            Set_ValveControlRight(R_COMMUNICATION);
            break;
        case OPT_CLOSE_VALVE:
            Set_ValveControlRight(R_COMMUNICATION);
            break;
        case OPT_EXIT_CONTROL:
            Reset_ValveControlRight(R_COMMUNICATION);
            break;
        default: break;
    }
}

/**
* 获取通讯开关阀的标志
*
* @author wb (2014-11-24)
*
* @return 1  开阀；2：关阀
*/

unsigned char Get_CommunicateOpenCloseValveFlag(void)
{
    return Valve.Flag_Communicate_OpenCloseValve;
}

/**
* 设置阀门各参数值
*
* @author wb (2014-11-24)
*
* @return 1  参数正确；0-参数错误
*/
unsigned char SetValveParaValue(T_ValveValue valvevalue_type, unsigned short val)
{
    unsigned char re = 1;
    switch(valvevalue_type)
    {
    case BASEVAL_TIMEOPENVALVE:
        Valve.TimeValue_OpenValve = val;
        break;
    case BASEVAL_TIMECLOSEVALVE:
        Valve.TimeValue_CloseValve = val;
        break;
    case BASEVAL_TIMETWOCLOSEVALVE:
        Valve.TimeValue_TwoCloseValve = val;
        break;
    case BASEVAL_TIMETWOOPENVALVE:
        Valve.TimeValue_TwoOpenValve = val;
        break;
    case BASEVAL_VALUEOVERCRRUNT:
        Valve.Value_OverCurrent = val;
        break;
    case BASEAL_POWERONOPENVALVE:
        Valve.ParaFlag.PowerOnOpenValve = val;
        break;
    case BASEAL_OWERCLOSEVALVE:
        Valve.ParaFlag.OwerVol_CloseValve = val;
        break;
    case BASEL_OVERTIMEVALUE:
        Valve.OpenCloseOverTime_Time = val;
        break;
    default:
        re = 0;
        break;
    }
    return re;
}

/**
* 设置阀门各参数值
*
* @author wb (2014-11-24)
*
* @return 1  参数正确；0-参数错误
*/
unsigned short GetValveParaValue(T_ValveValue valvevalue_type)
{
    unsigned short val = 0;
    switch(valvevalue_type)
    {
    case BASEVAL_TIMEOPENVALVE:
        val = Valve.TimeValue_OpenValve;
        break;
    case BASEVAL_TIMECLOSEVALVE:
        val = Valve.TimeValue_CloseValve;
        break;
    case BASEVAL_TIMETWOCLOSEVALVE:
        val = Valve.TimeValue_TwoCloseValve;
        break;
    case BASEVAL_TIMETWOOPENVALVE:
        val = Valve.TimeValue_TwoOpenValve;
        break;
    case BASEVAL_VALUEOVERCRRUNT:
        val = Valve.Value_OverCurrent;
        break;
    case BASEAL_POWERONOPENVALVE:
        val = Valve.ParaFlag.PowerOnOpenValve;
        break;
    case BASEAL_OWERCLOSEVALVE:
        val = Valve.ParaFlag.OwerVol_CloseValve;
        break;
    case BASEL_OVERTIMEVALUE:
        val = Valve.OpenCloseOverTime_Time;
        break;
    default:
        val = 0;
        break;
    }
    return val;
}
/**
 * 置响铃标志
 */
void Set_BeepAlarm_Flag(void)
{
    Flag_BeepAlrm = 1;
}
/**
 * 清除响铃标志
 */
void ClearBeepAlarm_Flag(void)
{
    Flag_BeepAlrm = 0;
}
/**
 * 获取响铃标志；
 * @return：1：有响铃；0-无响铃
 */
unsigned char GetBeepAlarm_Flag(void)
{
    return Flag_BeepAlrm;
}
