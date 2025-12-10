/* 
 * File:   app_lcddisp.h
 * Author: wb
 *
 * Created on December 22, 2015, 4:37 PM
 */

#ifndef APP_LCDDISP_H
#define	APP_LCDDISP_H

#ifdef	__cplusplus
extern "C" {
#endif

/**显示界面的枚举类型
 */
enum MODE
{
    USERS_INTERFACE,            //用户界面
    READY_INTERFACE,            //准备界面
    WORK_INTERFACE,             //调试界面
};

/**显示变量的结构体
 */
typedef struct DISP_S
{
    unsigned char Type;         //显示类型
    unsigned char State;        //显示状态
    unsigned char Mode;         //显示模式（用户界面模式、准备界面、调试界面模式）；
    unsigned char Mem[8];       //显示数据缓存
}DISP_ST;
extern DISP_ST DISP;

/*  显示类型 */
#define DIS_STA_START               0  //显示开始

typedef enum DISP_TYPE_EU
{
    DIS_STA_DJS,                //显示倒计时
    DIS_STA_ERR,                //显示报警
    DIS_STA_GAS,                //累计气量
    DIS_STA_PERIODGAS,          //周期内的气量
    DIS_STA_YUE,                //剩余金额
    DIS_STA_PRICE,              //当前价格
    DIS_STA_VOLUME,             //剩余气量
    DIS_STA_ALREDYYUE,          //消费金额
    DIS_STA_AMOUNTLAST,         //末次充值金额SS
    DIS_STA_METERTYPE,          //表具类型
    DIS_STA_NUM_H,              //表具编号高6位
    DIS_STA_NUM_L,              //表具编号低6位
    DIS_STA_TIME,               //表具时间
    DIS_STA_DATE,               //表具日期

    DIS_STA_PROGRAME,           //设备准备界面
    DIS_STA_IPH,                //显示IP地址高位
    DIS_STA_IPL,                //显示IP地址低位
    DIS_STA_PORT,               //显示端口号
    DIS_STA_TERMINAL_NUM,       //显示终端号
    DIS_STA_VERSION_NUM,        //显示程序版本号
    DIS_STA_UPCOMING_RESULT,    //上告结果（上一次成功，失败)
    DIS_STA_UPCOMING_DAY,       //显示上告日
    DIS_STA_OPENSTATUS,         //显示开通状态
    DIS_STA_METERTYPE1,         //表具类型
    DIS_STA_SIGNAL,             //显示信号
    DIS_STA_NED,                //显示结束
}DISP_TYPE_E;
extern DISP_TYPE_E  DISP_TYP;

extern void LCD_Disp(void);
extern void LcdDispOverTimer(void);

#ifdef	__cplusplus
}
#endif

#endif	/* APP_LCDDISP_H */

