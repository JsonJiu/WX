#include "drv_valve.h"

extern void IM_m2m_Set_MotorSleep_GPIO(unsigned char fun);
extern void IM_m2m_Set_MotorOpen_GPIO(unsigned char fun);
extern void IM_m2m_Set_MotorClose_GPIO(unsigned char fun);
extern unsigned char IM_m2m_GetStatus_PlaceDetect(void);

/**
* 开阀操作
* 
* @author wb (2014-11-21)
*/

void Open_Valve(void)
{
    IM_m2m_Set_MotorSleep_GPIO(1);
    IM_m2m_Set_MotorOpen_GPIO(1);
    IM_m2m_Set_MotorClose_GPIO(0);
}

/**
* 关阀操作
* 
* @author wb (2014-11-21)
*/

void Close_Valve(void)
{
    IM_m2m_Set_MotorSleep_GPIO(1);
    IM_m2m_Set_MotorOpen_GPIO(0);
    IM_m2m_Set_MotorClose_GPIO(1);
}

/**
* 停止阀门操作
* 
* @author wb (2014-11-21)
*/

void Stop_Valve(void)
{
    IM_m2m_Set_MotorSleep_GPIO(0);
    IM_m2m_Set_MotorOpen_GPIO(0);
    IM_m2m_Set_MotorClose_GPIO(0);
}

