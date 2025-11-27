/** 
 * 原创：李忠园 
 * CBB整理：应英武 
 * 修改记录:-----------------------------------------------------------
 *          日期:           修改:       工作:           版本:
 *          2014-11-09      应英武      整理            v0.0.0
*/
#ifndef     _eeprom_H_
#define     _eeprom_H_

#ifdef      _eeprom_C_
#define     exteeprom
#else
#define     exteeprom   extern
#endif
/*
********************************************************************************
                                                        ************************
DEF_    CFG_                                                    CONFIG
                                                        ************************
*/
/*  CFG_24Cxx定义与器件对应表:
        CFG_24C02:      2   
        CFG_24C08:      8       
        CFG_24C16:      16
        CFG_24C32:      32  
        CFG_24C64:      64  
        CFG_24C128:     128 
        CFG_24C256:     256
*/
#ifndef CFG_24Cxx
#define CFG_24Cxx   16
#endif

#ifndef EE_CHECK_ERR    //错误代码
#define EE_CHECK_ERR    3
#endif


//延时n个ms
extern  void DelayMs(unsigned int n);
//IIC初始化
extern  void IIC_Init(void);
//IIC启动
extern  void IIC_Start(void);
//IIC结束
extern  void IIC_End(void);
//IIC发送数据dat,并返回错误代码
extern  unsigned char IIC_SendByte(unsigned char dat);
//IIC接收数据放入dat,并应答
extern  void IIC_RecvByte(unsigned char *dat);
//IIC接收数据放入dat,不应答
extern  void IIC_RecvByteNOACK(unsigned char *addr);


void EEClear(void);


/*
                                                        ************************
                                                                DEFINE
                                                        ************************
*/
#define EE_ADDR_R   0x01        //读控制字  
#define EE_ADDR_W   0x00        //写控制字

#if     CFG_24Cxx==2
#define DEF_EE_BUF          8           //e2prom缓冲区大小
#define DEF_EE_SIZE         256         //e2prom容量(单位字节)
#elif   CFG_24Cxx==4
#define DEF_EE_BUF          16                  
#define DEF_EE_SIZE         512
#elif   CFG_24Cxx==8
#define DEF_EE_BUF          16                  
#define DEF_EE_SIZE         1024
#elif   CFG_24Cxx==16
#define DEF_EE_BUF          16                  
#define DEF_EE_SIZE         2048
#elif   CFG_24Cxx==32
#define DEF_EE_BUF          32                  
#define DEF_EE_SIZE         4096
#elif   CFG_24Cxx==64
#define DEF_EE_BUF          32                  
#define DEF_EE_SIZE         8192
#elif   CFG_24Cxx==128
#define DEF_EE_BUF          64                  
#define DEF_EE_SIZE         12384
#elif   CFG_24Cxx==256
#define DEF_EE_BUF          64
#define DEF_EE_SIZE         32768
#endif

#define DEF_EE_BUF_MASK     (DEF_EE_BUF-1)

//定义IIC时钟频率,引值越大频率越低
#ifndef IIC_DEL
#define IIC_DEL                10
#endif
//定义错误代码
#ifndef IIC_SEND_FAIL
#define IIC_SEND_FAIL           1
#endif
#define     CFG_IIC_LSB 0
#define     CFG_IIC_MSB 1
#define     SET_IIC_LSB()   (IIC_Mode = CFG_IIC_LSB)
#define     SET_IIC_MSB()   (IIC_Mode = CFG_IIC_MSB)
/**
 * 应用部分
 * @author 115 (2014-11-09)
 */
exteeprom   unsigned int eeprom_Version(void);
exteeprom   void OI_EEInit(void);
exteeprom   void OI_EEIdle(void);
exteeprom   int OI_EERead(unsigned int Adr,unsigned char *pBuf,unsigned int Num,
                       unsigned char DevAdr);
exteeprom   int OI_EEWrite(unsigned int Adr,unsigned char *pBuf,unsigned int Num,
                        unsigned char DevAdr);
exteeprom   int OI_EEFill(unsigned int Adr,unsigned char Data,unsigned int Num,
                       unsigned char DevAdr);
/*
********************************************************************************
*/
#endif




