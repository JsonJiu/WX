#ifndef __APP_CARDHUMMACH_H__
#define __APP_CARDHUMMACH_H__

typedef enum {
    STATE_LCD_DISP_RELEASE = 0,     //释放显示，显示表具不同状态下的默认值
    STATE_LCD_DISP_PRELOCK,     //
    STATE_LCD_DISP_LOCK,        //锁定当前显示，等待下次中断后解除锁定
    STATE_LCD_DISP_CHANGE,      //需要切换显示，下一次中断都切换
}
LcdStateVal;

void TaskCardMeterDisplay(void);
LcdStateVal SetLcdState(LcdStateVal val);

void LcdDispVer(unsigned char Head1, unsigned int T_num);
void LcdDispLoading(void);
void LcdDispErr(unsigned char ErrNum);

extern void OV_LcdClearValueMark(void);
extern unsigned char CardBslRead(void);

#endif
