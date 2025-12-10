#include "include.h"

/**
 * 霍尔触发 的定时器启动接口 
 *    实现1s定时
 * @author xxx (2015/5/27)
 */
void HallTimerStartEvent(FunNoInput fun)
{
    
}


/**
 * 计量设备异常(不包含霍尔)
 * 
 * @author xsc (2015-03-27)
 */
void CntPayDeviceErrEvent(void)
{

}

/**
 * 霍尔磁攻击
 * 
 * @author xsc (2014/11/27)
 */
void HallAttackEvent(void)
{

    Set_ValveControlRight(R_EVENT);
    SetState(STATE_MEASURE, STATE_MEASURE_BROKEN);
}

/**
 * 冻结事件
 * 
 * @author xsc (2015-03-27)
 */
void Freeze_Up_Event(void)
{

}

/**
 * 冻结时表具状态
 * 
 * @author xsc (2015-03-27)
 */
void GetMeterState(unsigned char *state)
{
    
}


/**
 * 新周期开始
 * 
 * @author xsc (2015-03-27)
 */
void NewStepCycleEvent(void)
{

}

/**
 * 更新新的阶梯
 * 
 * @author xsc (2015-03-27)
 */
void UpdataNewStepEvent(void)
{

}


/**
 * 金额状态发生变化
 * 
 * @author xsc (2015-03-27)
 * 
 * @param state 
 */
void RemainStateChangeEvent(T_StateVal state)
{

}

/**
 * 过流事件
 * 
 * @author xsc (2015-03-27)
 */
void OverFlowEvent(void)
{

}

/**
 * 闲置事件
 * 
 * @author xsc (2015-03-27)
 */
void UnUsedTimeOverEvent(void)
{

}

/**
 * 读取直读头
 * 
 * @author xsc (2015-04-01)
 */
void ReadZDEvent(void)
{

}

/**
 * 重新上电事件
 * 
 * @author xsc (2015-04-01)
 */
void RePowerUpEvent(void)
{
}
