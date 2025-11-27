
#include    "lib_mc306.h"


/***********需要 BSP提供的函数 ***********/

//WIFI_SleepRQ 输出控制
void WiFi_SleepRQ_Set(void);
void WiFi_SleepRQ_Clr(void);
//WIFI_SleepRQ 端口方向初始化  OUT-输出，IN-输入
void WiFi_SleepRQ_Init(unsigned char mode);

//WIFI_Reload 输出控制
void WiFi_Reload_Set(void);
void WiFi_Reload_Clr(void);
//WIFI_Reload 端口方向初始化  OUT-输出，IN-输入
void WiFi_Reload_Init(unsigned char mode);

//WIFI_Reset 输出控制
void WiFi_Reset_Set(void);
void WiFi_Reset_Clr(void);
//WIFI_Reset 端口方向初始化  OUT-输出，IN-输入
void WiFi_Reset_Init(unsigned char mode);

//WIFI_SleepON 端口方向初始化  OUT-输出，IN-输入
void WiFi_SleepON_Init(unsigned char mode);
//WIFI_SleepON 获取输入
unsigned char WiFi_SleepON_Get(void);

// WIFI_Ready 端口方向初始化  OUT-输出，IN-输入
void WiFi_Ready_Init(unsigned char mode);
//WIFI_Ready 获取输入
unsigned char WiFi_Ready_Get(void);

void WiFiUartConfig(pUartMode Mode,IRQServerRecvDataFT RecvFun);
void WiFiUartSendChar(unsigned char ch);
void WiFiUartSend(unsigned char *buf, unsigned int len);

/***********结束  需要 BSP提供的函数 ***********/


/***********需要 BSP提供的函数 ***********/

//RF_SETA 输出控制
void RF_SETA_Set(void);
void RF_SETA_Clr(void);
//RF_SETA 端口方向初始化  OUT-输出，IN-输入
void RF_SETA_Init(unsigned char mode);
//RF_SETA获取输入
unsigned char RF_SETA_Get(void);

//RF_SETB 输出控制
void RF_SETB_Set(void);
void RF_SETB_Clr(void);
//RF_SETB 端口方向初始化  OUT-输出，IN-输入
void RF_SETB_Init(unsigned char mode);
//RF_SETB 获取输入
unsigned char RF_SETB_Get(void);

//RF_AUX 输出控制
void RF_AUX_Set(void);
void RF_AUX_Clr(void);
//RF_AUX 端口方向初始化  OUT-输出，IN-输入
void RF_AUX_Init(unsigned char mode);
//RF_AUX 获取输入
unsigned char RF_AUX_Get(void);

//RF_POW 输出控制
void RF_POW_Set(void);
void RF_POW_Clr(void);
//RF_POW  端口方向初始化  OUT-输出，IN-输入
void RF_POW_Init(unsigned char mode);


void RFUartConfig(pUartMode Mode, IRQServerSendDataFT SendFun, IRQServerRecvDataFT RecvFun);
void RFUartSend(unsigned char *buf, unsigned int len);

/***********结束  需要 BSP提供的函数 ***********/


/***********需要 BSP提供的函数 ***********/
//端口方向初始化  OUT-输出，IN-输入
void PowerCtrl_Init(unsigned char mode);
void GreenLed_Init(unsigned char mode);
void RedLed_Init(unsigned char mode);
void KEY_Init(unsigned char mode);

// 输出控制
void PowerCtrl_Set(void);
void PowerCtrl_Clr(void);

void GreenLed_Set(void);
void GreenLed_Clr(void);

void RedLed_Set(void);
void RedLed_Clr(void);

//KEY 获取输入
unsigned char KEY_Get(void);

/***********结束  需要 BSP提供的函数 ***********/


