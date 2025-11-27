
#ifndef _HF_LPB_
#define _HF_LPB_

#define WIFI_TIME_OUT_TO_RESET   3000  //5分钟 = (5min * 60s * 1000ms) / 100ms = 3000
//
///***********需要 BSP提供的函数 ***********/
//
////WIFI_SleepRQ 输出控制
//void WiFi_SleepRQ_Set(void);
//void WiFi_SleepRQ_Clr(void);
//
////WIFI_Reload 输出控制
//void WiFi_Reload_Set(void);
//void WiFi_Reload_Clr(void);
//
////WIFI_Reset 输出控制
//void WiFi_Reset_Set(void);
//void WiFi_Reset_Clr(void);
//
////WIFI_SleepRQ 端口方向初始化  OUT-输出，IN-输入
//void WiFi_SleepRQ_Init(unsigned char mode);
//
////WIFI_Reset 端口方向初始化  OUT-输出，IN-输入
//void WiFi_Reset_Init(unsigned char mode);
//
////WIFI_Reload 端口方向初始化  OUT-输出，IN-输入
//void WiFi_Reload_Init(unsigned char mode);
//
////WIFI_SleepON 端口方向初始化  OUT-输出，IN-输入
//void WiFi_SleepON_Init(unsigned char mode);
//
//// WIFI_Ready 端口方向初始化  OUT-输出，IN-输入
//void WiFi_Ready_Init(unsigned char mode);
//
//
////WIFI_SleepON 获取输入
//unsigned char WiFi_SleepON_Get(void);
//
////WIFI_Ready 获取输入
//unsigned char WiFi_Ready_Get(void);
//
//void WiFiUartConfig(pUartMode Mode, IRQServerSendDataFT SendFun, IRQServerRecvDataFT RecvFun);
//void WiFiUartSend(unsigned char *buf, unsigned int len);
//
///***********结束  需要 BSP提供的函数 ***********/
///////////////////////////////////////////////////////////////////////////////
/*******************************************************************************
 *
 * 状态定义
 *
*******************************************************************************/
enum
{
    WIFI_BUSY = 0,
    WIFI_RST,
    WIFI_RST1,
    WIFI_WAIT_RDY,
    WIFI_ENTERCMD,
    WIFI_ENTERCMD_WAITa,
    WIFI_ENTERCMDa,
    WIFI_ENTERCMD_WAIT_OK,
    WIFI_CONFIG,
    WIFI_READY,

    WIFI_ENTER_SLEEP,
    WIFI_SLEEP1,
    WIFI_SLEEP_CHK,
    WIFI_SLEEP,

    WIFI_RELOAD,
    WIFI_RELOAD1,

    WIFI_WAKEUP,
    WIFI_WAKEUP1,
    WIFI_WAKEUP_CHK,

    WIFI_RUN,
    WIFI_ERROR,
};


extern unsigned char WiFi_HWStatus;   //WiFi硬件状态
extern unsigned char WiFi_HWNextSta;  //WiFi硬件下一状态
extern unsigned short WiFi_HWDlyTime;
extern unsigned short WiFi_SWDlyTime;
extern unsigned char SystemState;//系统状态
extern unsigned long  SYS_IdleTOPowerDownTime;//空闲关机

void WiFi_Init(void);
void WiFi_Pin_Init(void);

void WiFiUartRecv(unsigned char ch);
void Task_WiFi_HW(void);
void Task_WiFi_SW(void);
void WiFi_Status_DLY(void);


#endif /* __WIFI_H */






