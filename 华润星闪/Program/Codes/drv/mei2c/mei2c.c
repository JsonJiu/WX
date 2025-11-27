/** 
 * 原创：李忠园 
 * CBB整理：应英武 
 * 修改记录:-----------------------------------------------------------
 *          日期:           修改:       工作:           版本:
 *          2014-11-09      应英武      整理            v0.0.0
*/
/*
                                                    ****************************
                                                            eeprom
                                                    ****************************

说明:   　　24c0x系列EEPROM操作,EEPROM初始值为0 
            
                                    控制码                      地址空间        页尺寸
                         ____________/\_______________           __/\___
                        /                             \         /       \
            24c01/2     1   0   1   0   A2  A1  A0  R/W     +   1byte addr      8byte
            24c04       1   0   1   0   A2  A1  a8  R/W     +   1byte addr      16byte
            24c08       1   0   1   0   A2  a9  a8  R/W     +   1byte addr      16byte
            24c16       1   0   1   0   a10 a9  a8  R/W     +   1byte addr      16byte

            24c32       1   0   1   0   A2  A1  A0  R/W     +   2byte addr      32byte
            24c64       1   0   1   0   A2  A1  A0  R/W     +   2byte addr      32byte
            24c128      1   0   1   0   x   x   x   R/W     +   2byte addr      64byte
            24c256      1   0   1   0   0   A1  A0  R/W     +   2byte addr      64byte

需求:   　　一个输入端口,一个输入,输出端口,需要上拉
            iic_io.c    v0.0.7  CFG_IIC_MSB模式
版本:       v0.1.0
            v0.1.0: 1 增加函数EEFill.
                    2 修改函数EEInit,EERead,EEWrite,EEFill
                            更改返回类型void->int.
                    3 修改函数EERead中__ee_read位置.
            v0.0.9: 1 增加24C32,24C64,24C128,24C256的支持
                    2 取消宏:
                            EE_POWER_OUT,
                            EE_POWER_ON,
                            EE_POWER_OFF,
                            EE_ADDR,
                            DEF_EE_PAGE_BIT,
                            的定义
                    3 更改宏名:
                        EE_ADDR ->  EEPROM_DEV_ADDR
            v0.0.8: 1 申明外部DelayMs(ms)
                    2 申明外部IIC驱动函数
                    3 增加函数__EEDelay()
            v0.0.7: 1 更改函数EEWrite,使其可使用任意地址.增加c_addr和count变量
                    2 增加宏定义DEF_EE_BUF_MASK.
            v0.0.6: 1 取消v0.0.5中的更改
            v0.0.5: 1 EEWrite中对if(num==0) break;中增加10ms延时
            v0.0.4: 1 更改变量名error->err
                    2 增加错误代码EE_CHECK_ERR.
                    3 更改宏名DEF_EE_H_ADDR->DEF_EE_PAGE_BIT
            v0.0.3: 1 增加EE_POWER_OFF和EE_POWER_ON操作
            v0.0.2: 1 增加宏定义DEF_EE_SIZE
                    2 更改宏名:
                        DEF_H_ADDR  ->  DEF_EE_H_ADDR
                        EE_WRITE_CYCLE  ->  EE_WRITE_WAIT
            v0.0.1: 1 增加函数EERead()和EEWrite()
                    2 增加宏定义EE_WRITE_CYCLE和DEF_EE_BUF
版权:       版权所有，(c)2000-2050，李忠园
修改记录:   --------------------------------------------------------------------
            日期:           作者:       工作:           版本:
            2013-02-05      李忠园      增加            v0.1.0
            2012-02-06      李忠园      修改            v0.0.9
            2010-11-17      李忠园      修改            v0.0.8
            2010-08-31      李忠园      创建            v0.0.0
********************************************************************************
*/
unsigned int eeprom_Version(void)
{   return  0x0010;     }

/**
 * 移值部分
 * @author 115 (2014-11-09)
 */
extern void IM_IICPWR_OUT(unsigned char prt);//prt,0:1号模拟口,1:2号模拟口
extern void IM_IICPWR_UP(unsigned char prt);//prt,0:1号模拟口,1:2号模拟口
extern void IM_IICPWR_DOWN(unsigned char prt);//prt,0:1号模拟口,1:2号模拟口
extern void IM_IICPWR_ON(unsigned char prt);//prt,0:1号模拟口,1:2号模拟口
extern void IM_IICPWR_OFF(unsigned char prt);//prt,0:1号模拟口,1:2号模拟口
                                             //
extern void IM_IICSCL_OUT(unsigned char prt);//prt,0:1号模拟口,1:2号模拟口
extern void IM_IICSCL_IN(unsigned char prt);//prt,0:1号模拟口,1:2号模拟口
extern void IM_IICSCL_UP(unsigned char prt);// prt,0:1号模拟口,1:2号模拟口
extern void IM_IICSCL_DOWN(unsigned char prt);// prt,0:1号模拟口,1:2号模拟口
extern void IM_IICSCL_SET(unsigned char prt);// prt,0:1号模拟口,1:2号模拟口
extern void IM_IICSCL_CLR(unsigned char prt);// prt,0:1号模拟口,1:2号模拟口
                                             //
extern void IM_IICSDA_OUT(unsigned char prt);//prt,0:1号模拟口,1:2号模拟口
extern void IM_IICSDA_IN(unsigned char prt);//prt,0:1号模拟口,1:2号模拟口
extern void IM_IICSDA_UP(unsigned char prt);// prt,0:1号模拟口,1:2号模拟口
extern void IM_IICSDA_DOWN(unsigned char prt);// prt,0:1号模拟口,1:2号模拟口
extern void IM_IICSDA_SET(unsigned char prt);// prt,0:1号模拟口,1:2号模拟口
extern void IM_IICSDA_CLR(unsigned char prt);// prt,0:1号模拟口,1:2号模拟口

unsigned char IM_IICSDA_STU(unsigned char prt);// prt,0:1号模拟口,1:2号模拟口//返回值，0:低，非0:高

#define      _eeprom_C_
//#include    "config.h"
#include    "mei2c.h"
unsigned char IIC_Mode = 1;
/*
********************************************************************************************
*/
void IIC_Init(void)
{
    IM_IICSCL_OUT(0);
    IM_IICSCL_UP(0);
    IM_IICSCL_SET(0);
    IM_IICSDA_OUT(0);
    IM_IICSDA_UP(0);
    IM_IICSDA_SET(0);

//  IIC_SCL_OUT();
//  IIC_SCL_PUP();
//  IIC_SCL_SET();
//
//  IIC_SDA_OUT();
//  IIC_SDA_PUP();
//  IIC_SDA_SET();
}
/*
********************************************************************************************
*/
void IIC_Delay(unsigned char del)
{   
    unsigned char j;
    for( ; del>0; del--)
    {   
        for(j=0;j<IIC_DEL;j++)
            ;
    }
}
/*
********************************************************************************************
说明:       IIC总线开始
*/
void IIC_Start(void)    
{  
    IM_IICSDA_OUT(0);
    IM_IICSDA_SET(0); 
    IIC_Delay(1);
    IM_IICSCL_SET(0);
    IIC_Delay(1);
    IM_IICSDA_CLR(0);
    IIC_Delay(1);
    IM_IICSCL_CLR(0);
    IIC_Delay(1);
//  IIC_SDA_OUT();
//  IIC_SDA_SET();
//  IIC_Delay(1);
//  IIC_SCL_SET();
//  IIC_Delay(1);
//  IIC_SDA_CLR();
//  IIC_Delay(1);
//  IIC_SCL_CLR();
//  IIC_Delay(1);
}
/*
********************************************************************************************
说明:       IIC总线结束
*/
void IIC_End(void)  
{  
    IM_IICSDA_OUT(0);
    IM_IICSCL_CLR(0);
    IIC_Delay(1);
    IM_IICSDA_CLR(0);
    IIC_Delay(1);
    IM_IICSCL_SET(0);
    IIC_Delay(1);
    IM_IICSDA_SET(0);
    IIC_Delay(1);
//  IIC_SDA_OUT();
//  IIC_SCL_CLR();
//  IIC_Delay(1);
//  IIC_SDA_CLR();
//  IIC_Delay(1);
//  IIC_SCL_SET();
//  IIC_Delay(1);
//  IIC_SDA_SET();
//  IIC_Delay(1);
}
/*
********************************************************************************************
说明:       接收一字节数据，并发送应答
参数:       dat:接收数据指针
*/
void IIC_RecvByte(unsigned char *dat)
{   
    unsigned char i;
    *dat = 0;
    IM_IICSDA_IN(0);
    IM_IICSCL_CLR(0);
    IIC_Delay(1);
    for(i=0;i<8;i++)
    {   
        IIC_Delay(1);
        IM_IICSCL_SET(0);
        IIC_Delay(1);
        if( IM_IICSDA_STU(0) )
        if(IIC_Mode == CFG_IIC_MSB)
            (*dat) |= (unsigned char)(0x80>>i);
        else
            (*dat) |= (unsigned char)(0x01<<i);
        IIC_Delay(1);
        IM_IICSCL_CLR(0);
        IIC_Delay(1);
    }
    //send ack
    IM_IICSDA_OUT(0);
    IM_IICSDA_CLR(0);
    IIC_Delay(1);
    IM_IICSCL_SET(0);
    IIC_Delay(2);
    IM_IICSCL_CLR(0);

/*
    *dat = 0;                                  
    IIC_SDA_IN();                              
    IIC_SCL_CLR();                             
    IIC_Delay(1);                              
    for(i=0;i<8;i++)                           
    {                                          
        IIC_Delay(1);                          
        IIC_SCL_SET();                         
        IIC_Delay(1);                          
        if( IIC_SDA() )                        
        if(IIC_Mode == CFG_IIC_MSB)            
            (*dat) |= (unsigned char)(0x80>>i);
        else                                   
            (*dat) |= (unsigned char)(0x01<<i);
        IIC_Delay(1);                          
        IIC_SCL_CLR();                         
        IIC_Delay(1);                          
    }                                          
    //send ack                                 
    IIC_SDA_OUT();                             
    IIC_SDA_CLR();                             
    IIC_Delay(1);                              
    IIC_SCL_SET();                             
    IIC_Delay(2);                              
    IIC_SCL_CLR();                             
*/
}
/*
********************************************************************************************
说明:       接收一字节数据，不发送应答
参数:       dat:接收数据指针
*/
void IIC_RecvByteNOACK(unsigned char *dat)
{   
    unsigned char i;
    IM_IICSDA_IN(0);
    IM_IICSCL_CLR(0);
    IIC_Delay(1);
    for(i=0;i<8;i++)
    {
        if(IIC_Mode == CFG_IIC_MSB)
            (*dat) <<= 1;
        else
            (*dat) >>= 1;
        IIC_Delay(1);
        IM_IICSCL_SET(0);
        IIC_Delay(1);
        if(IM_IICSDA_STU(0))
        if(IIC_Mode == CFG_IIC_MSB)
            (*dat) |= 0x01;
        else
            (*dat) |= 0x80;
        IIC_Delay(1);
        IM_IICSCL_CLR(0);
        IIC_Delay(1);
    }
/*
    IIC_SDA_IN();                  
    IIC_SCL_CLR();                 
    IIC_Delay(1);                  
    for(i=0;i<8;i++)               
    {                              
        if(IIC_Mode == CFG_IIC_MSB)
            (*dat) <<= 1;          
        else                       
            (*dat) >>= 1;          
        IIC_Delay(1);              
        IIC_SCL_SET();             
        IIC_Delay(1);              
        if(IIC_SDA())              
        if(IIC_Mode == CFG_IIC_MSB)
            (*dat) |= 0x01;        
        else                       
            (*dat) |= 0x80;        
        IIC_Delay(1);              
        IIC_SCL_CLR();             
        IIC_Delay(1);              
    }                              
*/
}
/*
********************************************************************************************
说明:       发送一字节数据，并接收应答
参数:       dat:发送的数据
返回:       0:表示成功,
            其它:表示错误代码
*/
unsigned char IIC_SendByte(unsigned char dat)
{   
    unsigned char i;
    IM_IICSCL_CLR(0);
    IIC_Delay(1);
    IM_IICSDA_OUT(0);
    for(i=0;i<8;i++)
    {
        
        if(IIC_Mode == CFG_IIC_MSB)
        {
            if( dat & 0x80 )
            {   IM_IICSDA_SET(0);  }
            else
            {   IM_IICSDA_CLR(0);  }
        }
        else
        {
            if( dat & 0x01 )
            {   IM_IICSDA_SET(0);  }
            else
            {   IM_IICSDA_CLR(0);  }
        }
        if(IIC_Mode == CFG_IIC_MSB)
            dat <<= 1;
        else
            dat >>= 1;
        IIC_Delay(1);
        IM_IICSCL_SET(0);
        IIC_Delay(2);
        IM_IICSCL_CLR(0);
        IIC_Delay(1);
    }
    //receive ack
    IM_IICSDA_IN(0);
    IIC_Delay(1);
    IM_IICSCL_SET(0);
    IIC_Delay(1);
    if(IM_IICSDA_STU(0))
    {
        IM_IICSCL_CLR(0);
        return(IIC_SEND_FAIL);
    }
    else
    {   
        IM_IICSCL_CLR(0);
        return(0);
    }
/*
    IIC_SCL_CLR();                 
    IIC_Delay(1);                  
    IIC_SDA_OUT();                 
    for(i=0;i<8;i++)               
    {                              
                                   
        if(IIC_Mode == CFG_IIC_MSB)
        {                          
            if( dat & 0x80 )       
            {   IIC_SDA_SET();  }  
            else                   
            {   IIC_SDA_CLR();  }  
        }                          
        else                       
        {                          
            if( dat & 0x01 )       
            {   IIC_SDA_SET();  }  
            else                   
            {   IIC_SDA_CLR();  }  
        }                          
        if(IIC_Mode == CFG_IIC_MSB)
            dat <<= 1;             
        else                       
            dat >>= 1;             
        IIC_Delay(1);              
        IIC_SCL_SET();             
        IIC_Delay(2);              
        IIC_SCL_CLR();             
        IIC_Delay(1);              
    }                              
    //receive ack                  
    IIC_SDA_IN();                  
    IIC_Delay(1);                  
    IIC_SCL_SET();                 
    IIC_Delay(1);                  
    if(IIC_SDA())                  
    {                              
        IIC_SCL_CLR();             
        return(IIC_SEND_FAIL);     
    }                              
    else                           
    {                              
        IIC_SCL_CLR();             
        return(0);                 
    }                              
*/
}

/*
********************************************************************************
说明:       初始化函数
*/
void OI_EEInit(void)
{
    IM_IICPWR_OUT(0);
    IM_IICPWR_UP(0);
    IM_IICPWR_ON(0);//上电
    IIC_Init();
}
/**
 * 结束进入低功耗
 * 115 (2014-11-09)
 */
void OI_EEIdle(void)
{
    IM_IICPWR_OUT(0);
    IM_IICPWR_UP(0);
    IM_IICPWR_OFF(0);//关电
    IM_IICSCL_OUT(0);
    IM_IICSCL_UP(0);
    IM_IICSCL_SET(0);//SCL上拉、输出高
    IM_IICSDA_OUT(0);
    IM_IICSDA_UP(0);
    IM_IICSDA_SET(0);//SDA上拉、输出高
}
/*
********************************************************************************
说明:       读数据,返回非0表示错误代码
        DevAdr:设备地址,格式如下(以24C01/2为例):
            高位                        低位
            1   0   1   0   A2  A1  A0  0(R/W)  
            注:R/W位应为0
返回:       0表示成功,非0表示失败代码
*/
int OI_EERead(unsigned int Adr,unsigned char *pBuf,unsigned int Num,
           unsigned char DevAdr)
{
    unsigned char Err,AdrH,AdrL,Ctrl;

//  if( (Adr+Num)>=DEF_EE_SIZE )
//      return 1;
//  if( Num==0 )
//      return 1;

    AdrH = (unsigned char)(Adr>>8);
    AdrL = (unsigned char)(Adr);
#if CFG_24Cxx >= 32
    Ctrl = DevAdr;
#else
    Ctrl = DevAdr | ((unsigned char)((AdrH<<1)&0x0e));
#endif

    IIC_Start();
    Err=IIC_SendByte(Ctrl);
    if(Err)
        goto __ee_read;

#if CFG_24Cxx >= 32
    Err=IIC_SendByte(AdrH);
    if(Err)
        goto __ee_read;
#endif

    Err=IIC_SendByte(AdrL);
    if(Err)
        goto __ee_read;

    Ctrl |= EE_ADDR_R;
    IIC_Start();
    Err=IIC_SendByte(Ctrl);
    if(Err)
        goto __ee_read;

    while( Num>1 )
    {
        IIC_RecvByte(pBuf);
        pBuf++;
        Num--;
    }

    IIC_RecvByteNOACK(pBuf);
__ee_read:
    IIC_End();
    return (int)(Err);
}
/*
********************************************************************************
说明:       写入并校验,返回非0表示错误代码,
返回:       0表示成功,非0表示失败代码
*/
int OI_EEWrite(unsigned int Adr,unsigned char *pBuf,unsigned int Num,
            unsigned char DevAdr)
{
    unsigned char Err,AdrH,AdrL,Ctrl;
    unsigned int i,AdrCur;

//  if( (Adr+Num)>=DEF_EE_SIZE )
//      return 1;
//  if( Num==0 )
//      return 1;

    AdrCur = Adr;
    i = 0;
    while( i!=Num )
    {
        AdrH = (unsigned char)(AdrCur>>8);
        AdrL = (unsigned char)(AdrCur);
#if CFG_24Cxx >= 32
        Ctrl = DevAdr;
#else
        Ctrl = DevAdr | ((unsigned char)((AdrH<<1)&0x0e));
#endif

        IIC_Start();
        Err=IIC_SendByte(Ctrl);
        if(Err)
            goto __ee_write;

#if CFG_24Cxx >= 32
        Err=IIC_SendByte(AdrH);
        if(Err)
            goto __ee_write;
#endif

        Err=IIC_SendByte(AdrL);
        if(Err)
            goto __ee_write;

        while(1)
        {
            Err = IIC_SendByte(pBuf[i]);
            if(Err)
                goto __ee_write;
            AdrCur++;
            i++;
            if( ((AdrCur&DEF_EE_BUF_MASK) ==0) || (i==Num) )    //两种地址翻方式
            //if( ((i&DEF_EE_BUF_MASK) ==0) || (i==Num) )
            {
                IIC_End();
                //Waiting for at least 10 ms
                DelayMs(10);
                break;
            }
        }
    }

    AdrCur = Adr;
    AdrH = (unsigned char)(AdrCur>>8);
    AdrL = (unsigned char)(AdrCur);
#if CFG_24Cxx >= 32
    Ctrl = DevAdr;
#else
    Ctrl = DevAdr | ((unsigned char)((AdrH<<1)&0x0e));
#endif

    IIC_Start();
    Err=IIC_SendByte(Ctrl);
    if(Err)
        goto __ee_write;

#if CFG_24Cxx >= 32
    Err=IIC_SendByte(AdrH);
    if(Err)
        goto __ee_write;
#endif

    Err=IIC_SendByte(AdrL);
    if(Err)
        goto __ee_write;

    Ctrl |= EE_ADDR_R;
    IIC_Start();
    Err=IIC_SendByte(Ctrl);
    if(Err)
        goto __ee_write;

    for( i=0; i<Num-1; i++ )
    {
        IIC_RecvByte(&AdrH);
        if( pBuf[i] != AdrH )
        {
            Err = EE_CHECK_ERR;
            break;
        }
    }
    IIC_RecvByteNOACK(&AdrH);
    if( pBuf[i]!=AdrH )
        Err = EE_CHECK_ERR;

__ee_write:
    IIC_End();
    return (int)Err;
}
int EEFill(unsigned int Adr,unsigned char Data,unsigned int Num,
           unsigned char DevAdr)
{
    unsigned char Err,AdrH,AdrL,Ctrl;
    unsigned int i,AdrCur;

    //  if( (Adr+Num)>=DEF_EE_SIZE )
    //      return 1;
    //  if( Num==0 )
    //      return 1;

    AdrCur = Adr;
    i = 0;
    while( i!=Num )
    {
        AdrH = (unsigned char)(AdrCur>>8);
        AdrL = (unsigned char)(AdrCur);
#if CFG_24Cxx >= 32
        Ctrl = DevAdr;
#else
        Ctrl = DevAdr | ((unsigned char)((AdrH<<1)&0x0e));
#endif

        IIC_Start();
        Err=IIC_SendByte(Ctrl);
        if(Err)
            goto __ee_write;

#if CFG_24Cxx >= 32
        Err=IIC_SendByte(AdrH);
        if(Err)
            goto __ee_write;
#endif

        Err=IIC_SendByte(AdrL);
        if(Err)
            goto __ee_write;

        while(1)
        {
            Err = IIC_SendByte(Data);
            if(Err)
                goto __ee_write;
            AdrCur++;
            i++;
            if( ((AdrCur&DEF_EE_BUF_MASK) ==0) || (i==Num) )    //两种地址翻方式
                //if( ((i&DEF_EE_BUF_MASK) ==0) || (i==Num) )
            {
                IIC_End();
                //Waiting for at least 10 ms
                DelayMs((30));
                break;
            }
        }
    }

    AdrCur = Adr;
    AdrH = (unsigned char)(AdrCur>>8);
    AdrL = (unsigned char)(AdrCur);
#if CFG_24Cxx >= 32
    Ctrl = DevAdr;
#else
    Ctrl = DevAdr | ((unsigned char)((AdrH<<1)&0x0e));
#endif

    IIC_Start();
    Err=IIC_SendByte(Ctrl);
    if(Err)
        goto __ee_write;

#if CFG_24Cxx >= 32
    Err=IIC_SendByte(AdrH);
    if(Err)
        goto __ee_write;
#endif

    Err=IIC_SendByte(AdrL);
    if(Err)
        goto __ee_write;

    Ctrl |= EE_ADDR_R;
    IIC_Start();
    Err=IIC_SendByte(Ctrl);
    if(Err)
        goto __ee_write;

    for( i=0; i<Num-1; i++ )
    {
        IIC_RecvByte(&AdrH);
        if( Data != AdrH )
        {
            Err = EE_CHECK_ERR;
            break;
        }
    }
    IIC_RecvByteNOACK(&AdrH);
    if( Data!=AdrH )
        Err = EE_CHECK_ERR;

__ee_write:
    IIC_End();
    return (int)(Err);
}
/*
********************************************************************************
说明:       测试代码,使用两片CFG_24Cxx=256进行地址连续控制
            地址空间为:     0 ~~ (2*DEF_EE_SIZE-1)
            片1:    0 ~~ DEF_EE_SIZE-1
            片2:    DEF_EE_SIZE ~~ 2*DEF_EE_SIZE-1
*/
#if 0
static int FileAccess38957(u32 Adr,unsigned char* pBuf,unsigned int Num,unsigned char Sign)
{
    int Err = SUCCESS;
    unsigned int Adr1,Adr2,Size1 = 0,Size2 = 0;

    if( (Adr+Num) >= (2*DEF_EE_SIZE) )
        return FALSE;

    if( Adr < DEF_EE_SIZE )
    {
        Adr1 = (unsigned int)Adr;
        if( (Adr+Num) < DEF_EE_SIZE )
            Size1 = Num;
        else
        {
            Size1 = DEF_EE_SIZE - Adr1;
            Adr2 = 0;
            Size2 = Num - Size1;
        }
    }
    else
    {
        Adr2 = ( ((unsigned int)Adr)-DEF_EE_SIZE );
        Size2 = Num;
    }
#define     DELAOISF        20
    EEPROM_POWER_ON();      //电源控制
    if( Size1 )
    {
        if( Sign == 1)
        {
            EEPROM_WP1_EN_W();  //保护控制
            DelayMs((DELAOISF));
            if( EEWrite( Adr1,pBuf,Size1,0xa2 ) )
                Err = FALSE;
            EEPROM_WP1_DIS_W();
        }
        else
        {
            DelayMs((DELAOISF));
            if( EERead( Adr1,pBuf,Size1,0xa2 ) )
                Err = FALSE;
        }
        pBuf += Size1;
    }
    if( Size2 )
    {
        if( Sign == 1)
        {
            EEPROM_WP2_EN_W();
            DelayMs((DELAOISF));
            if( EEWrite( Adr2,pBuf,Size2,0xa0 ) )
                Err = FALSE;
            EEPROM_WP2_DIS_W();
        }
        else
        {
            DelayMs((DELAOISF));
            if( EERead( Adr2,pBuf,Size2,0xa0 ) )
                Err = FALSE;
        }
        pBuf += Size2;
    }
    EEPROM_POWER_OFF();
    return Err;
}   
static int FileRead (u32 Adr,unsigned char* pBuf,unsigned int Num)
{
    return FileAccess38957(Adr,pBuf,Num,0);
}
static int FileWrite(u32 Adr,unsigned char* pBuf,unsigned int Num)
{
    return FileAccess38957(Adr,pBuf,Num,1);
}
void main(void)
{   //eeprom测试代码
    unsigned char TempqRead34532[54];
    unsigned char TempqWrite45654[54];
    int Err438758934;
    u32 Adr = 0;
    unsigned int i;
    for( i=0; i<sizeof(TempqWrite45654); i++ )
    {
        TempqWrite45654[i] = i;
    }
    while( 1 )
    {
        if( (Adr+sizeof(TempqRead34532)) >= 2*DEF_EE_SIZE )
            Adr = 0;
        Err438758934 = FileWrite(Adr,TempqWrite45654,sizeof(TempqWrite45654));
        if( Err438758934==FALSE )
            Err438758934 = FALSE;
        memset(TempqRead34532,0x00,sizeof(TempqRead34532));
        Err438758934 = FileRead(Adr,TempqRead34532,sizeof(TempqRead34532));
        if( memcmp(TempqWrite45654,TempqRead34532,sizeof(TempqRead34532)) )
            Err438758934 = FALSE;
        if( Err438758934==FALSE )
            Err438758934 = FALSE;
        Adr += sizeof(TempqRead34532);
    }
}
#endif

void mem_set(unsigned char *data, unsigned short len, unsigned short n);
void EEClear(void)
{
#if 0
    unsigned char ttt[10];
    unsigned char i;
    mem_set(ttt, sizeof(ttt), 0);
    OI_EEInit();
    for(i = 0; i < 100; i++)
    {
        OI_EEWrite(i * 10, ttt, sizeof(ttt), 0xA0);
    }
    OI_EEIdle();
#else
    OI_EEInit();
    EEFill(0,0,3000,0xA0);
    OI_EEIdle();
#endif
}
