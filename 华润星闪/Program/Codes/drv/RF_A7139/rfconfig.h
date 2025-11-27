#ifndef _RFCONFIG_H_
#define _RFCONFIG_H_

/* 波特率 */
//unsigned short RFBautRate0[] = {1200,2400,4800,9600,19200,38400};
//unsigned short RFFreq[] = {800,1800,2800,3800,4800,5800,6800,7800,8800,9800,10800};//Freq0*256 + Freq1 + 470000,800  表示的是 470.8MHz
typedef enum
{
   BautRate_2400 = 0,
   BautRate_4800,
   BautRate_9600,
   //BautRate_14400,
   BautRate_19200,
   BautRate_38400,
   //BautRate_56000,
   //BautRate_57600,
   //BautRate_115200,
}BautRate;

/* 频率 */
typedef enum
{
   Fre_4708 = 0,
   Fre_4718,
   Fre_4728,
   Fre_4738,
   Fre_4748,
   Fre_4758,
   Fre_4768,
   Fre_4778,
   Fre_4788,
   Fre_4798,
   Fre_4808,
}Frequency;


typedef enum
{
  RFS_DEEPSLEEP = 0, /*  RF 深度休眠                 */
  RFS_LOWSLEEP,      /*  RF 浅度休眠                 */
  RFS_HALT,          /*  RF 低功耗模式               */
  RFS_UNLOCK,
  RFS_SEND,          /*  RF 发送模式                 */
  RFS_SENDING,       /*  RF 发送数据中               */
  RFS_RECV,          /*  RF 接收模式                 */
  RFS_RECVED,
  RFS_RECV_END,
  RFS_SET,           /*  RF 空闲模式                 */
  RFS_Err,           /*  RF 出错                     */
  RFS_NoFind,
  RFS_WAIT,
  RFS_IDLE,   
  RFS_ADJFREQ,       /*  RF 频率校准                 */
  RFS_ADJGAIN,       /*  RF 增益校准                 */
  RFS_SAVEPARA,      /*  配置RF默认参数--存储到Flash  */
}RFSTATE;

typedef enum
{
    RFSTATE_SLEEP = 0,    //深度休眠
    RFSTATE_IDLE,             //空闲
    //RFSTATE_RXSTART,          //进入接收状态
    RFSTATE_RXING,            //接收中
    RFSTATE_RXDATA,           //收到数据
    RFSTATE_RXErr,            //接收失败
    RFSTATE_TX,               //发送状态
    RFSTATE_Err,              //错误
}RF_STATE;

typedef struct
{
  unsigned short freqband;
  BautRate rate;//波特率
  Frequency fre;//频率
  unsigned char percent;//占空比
  unsigned char SFDH;
  unsigned char SFDL;
  unsigned char cmddoor;
  unsigned short f;//频率偏差
  unsigned short xo;//晶振偏差
  unsigned int CRC16;
}RF_CONFIG;


#endif
