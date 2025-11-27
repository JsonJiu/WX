#include "config.h"
#include "app_lcddisp.h"

DISP_ST DISP;
DISP_TYPE_E  DISP_TYP;

unsigned char LCDDISP_TIMERNUM;

/*LCD显示超时函数
 */
void LcdDispOverTimer(void)
{
    DISP.Mode = USERS_INTERFACE;
    DISP.Type = DIS_STA_START;
    OV_LcdOff();
    User_CloseTime(LCDDISP_TIMERNUM);
}

/**
 * LCD显示超时任务
 */
void Init_LCD(void)
{
    LCD_Disp();
    if(LCDDISP_TIMERNUM == 0)
    {
        LCDDISP_TIMERNUM = User_AskTimeNumber();
    }
    User_StartTime(SUBTIMER, LCDDISP_TIMERNUM, 10, LcdDispOverTimer);
}

/**
 * LCD显示主函数
 */
void LCD_Disp(void)
{
    long long_temp;
    OV_LcdOn();
    OV_LcdClear();
    switch(DISP.Type)
    {
    case DIS_STA_TIME://显示表具时间
        OV_DispTime(BcdToHex(RTC.Hour), BcdToHex(RTC.Min), BcdToHex(RTC.Sec));
        break;
    case DIS_STA_DATE://显示表具日期
        OV_DispDate(BcdToHex(RTC.Year), BcdToHex(RTC.Mon), BcdToHex(RTC.Day));
        break;
    default:break;
    }
//    Dis_Batter_Status(GetSysPow());
//    Dis_Signal_Status(p_GSMDevice->SignalQuality);
//    Dis_Valve_Status(Get_ValveStatus());
//    Dis_Alarm_String();
}
