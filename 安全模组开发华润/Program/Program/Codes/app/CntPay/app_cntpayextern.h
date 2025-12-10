#ifndef _APP_CNTPAYEXTERN_H_
#define _APP_CNTPAYEXTERN_H_






void Freeze_Up_Event(void);
void NewStepCycleEvent(void);
void UpdataNewStepEvent(void);
void OverFlowEvent(void);
void UnUsedTimeOverEvent(void);
void CntPayDeviceErrEvent(void);
void HallAttackEvent(void);
void GetMeterState(unsigned char *state);
void ReadZDEvent(void);
void RePowerUpEvent(void);
#endif

