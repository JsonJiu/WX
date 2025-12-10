#include "drv_lg1608.h"
#include "r_cg_macrodriver.h"

unsigned char CurrentZone;
unsigned char PassBit[2];

/*****************************************************************************/
/***************************************************************************
初始化IIC
***************************************************************************/
unsigned char InitIIC()
{
    unsigned char j, count = 0;
    CardSdaInput();
    CardRstSet();
    NOP();//delay(CardDelayCount);
    CardSclSet();
    NOP();//delay(CardDelayCount);
    CardSclClr();
    CardRstClr();
    NOP();//delay(CardDelayCount);
    for(j = 0; j < 32; j++)
    {
        NOP();//delay(CardDelayCount);
        CardSclSet();//CLK 1
        if(CardSdaRead())
        {
            count += 1;
        }
        NOP();//delay(CardDelayCount);
        CardSclClr();//CLK 0
    }
    CardRstClr();
    if(count == 13)
        return TRUE;
    return FALSE;
}

/***************************************************************************
进行短暂的延时，原因是MSP430的速度比较快。使用者可以根据时钟频率自行调节延时长短
***************************************************************************/
void delay(unsigned char q0)
{
    //unsigned char q0=5;
    while(q0--)
        NOP();
}

/***************************************************************************
启动IIC总线
***************************************************************************/
void Start()
{
    CardSdaOutput();
    CardSdaSet();//SDA 1
    delay(CardDelayCount);
    CardSclSet();//CLK 1
    CardSdaClr();//SDA 0
    delay(CardDelayCount);
    CardSclClr();//CLK 0
}

/***************************************************************************
停止操作，释放IIC总线
***************************************************************************/
void Stop()
{
    CardSdaOutput();
    CardSdaClr();//SDA 0
    delay(CardDelayCount);
    CardSclSet();//CLK 1
    CardSdaSet();//SDA 1
    delay(CardDelayCount);
    CardSclClr();//CLK 0
}

/***************************************************************************
IIC总线应答
***************************************************************************/
unsigned char Ack(void)
{
    unsigned int time;
    CardSdaInput();
    NOP();//delay(CardDelayCount);
    CardSclSet();//CLK 1
    time = 25500;
    while(time--)
    {
        if(CardSdaRead() == 0) //等待为0
        {
            delay(CardDelayCount);
            CardSclClr();
            return TRUE;
        }
    }
    CardSclClr();
    return FALSE;
}

/***************************************************************************
IIC总线无应答
***************************************************************************/
void NoAck()
{
    CardSdaOutput();
    CardSdaSet();
    NOP();//delay(CardDelayCount);
    CardSclSet();
    delay(CardDelayCount);
    CardSclClr();
    //delay();
}

void SendAck(void)
{
    CardSdaOutput();
    CardSdaClr();
    CardSclSet();
    delay(CardDelayCount);
    CardSclClr();

}

/***************************************************************************
IIC总线写8位数据
input：要写的8位数据
***************************************************************************/
void Write8Bit(unsigned char input)
{
    unsigned char temp, q0;
    CardSdaOutput();
    CardSdaClr();
    NOP();
    delay(CardDelayCount);
    for(temp = 8; temp != 0; temp--)
    {
        q0 = input & 0x80;
        if(q0 == 0x80)
            CardSdaSet();
        else
            CardSdaClr();
        delay(CardDelayCount);
        CardSclSet();
        delay(CardDelayCount);
        CardSclClr();
        input = input << 1;
    }
    delay(CardDelayCount*50);
}

/***************************************************************************
IIC总线读8位数据
返回值：读出的8位数据
***************************************************************************/
unsigned char Read8Bit()
{
    unsigned char temp, q0, rbyte = 0;
    CardSdaInput();
    for(temp = 8; temp != 0; temp--)
    {
        NOP();//delay(CardDelayCount);
        CardSclSet();
        rbyte = rbyte << 1;
        q0 = CardSdaRead();
        if(q0)
            rbyte = rbyte | 0x1;
        NOP();//delay(CardDelayCount);
        CardSclClr();
    }
    return(rbyte);
}

/*****************************************************************************/

/**
 * init_card
 * 卡片初始化
 * @author Administrator (2014-11-03)
 * 
 * @return unsigned char    TRUE--初始化成功,FALSE--初始化失败
 */
unsigned char init_card(void)
{
    CurrentZone = 0xff;
    PassBit[0] = 0;
    PassBit[1] = 0;
    if(InitIIC() == TRUE)
        return TRUE;
    return FALSE;
}

/**
 * write_zone 
 * 卡片写入数据
 * @author Administrator (2014-11-03)
 * 
 * @param area 卡片区域
 * @param addr 地址
 * @param data 写入的数据
 * @param len  长度
 * 
 * @return unsigned char TRUE--写入成功,FALSE--写入失败
 */
unsigned char write_zone(unsigned char area, unsigned char addr, unsigned char *data, unsigned int len)
{
    unsigned int i;
    unsigned char retry = 2, err = 0;
    unsigned char addrH, addrL;
    //len=256;
    addrH = (addr & 0xf0) >> 4;
    addrL = (addr & 0x0f);
    while(retry--)
    {
        i = 0;
        err = 0;
        if(area != CON_ZONE)
            set_userzone(area);
        Start();
        if(area == CON_ZONE)
            Write8Bit(0xb4);
        else
            Write8Bit(0xb0);
        if(Ack() == FALSE)
            return FALSE;
        Write8Bit((addrH << 4) + addrL);
        while(i < len)
        {
            if(addrL > 0x0f)
            {
                NoAck();
                Stop();
                addrL -= 0x10;
                addrH += 0x01;
                DelayMs(10);
                if(area != CON_ZONE)
                    set_userzone(area);
                Start();
                if(area == CON_ZONE)
                    Write8Bit(0xb4);
                else
                    Write8Bit(0xb0);
                if(Ack() == FALSE)
                    return FALSE;
                Write8Bit((addrH << 4) + addrL);
            }
            if(Ack() == FALSE)
            {
                err = 1;
                break;
            }
            Write8Bit(*(data + i));
            i++;
            addrL += 1;
        }
        if(err == 1)
            return FALSE;
        else
            retry = 0;
        NoAck();
        Stop();
    }
    if(err)
    {
        return FALSE;
    }
    else
    {
        DelayMs(3);
        return TRUE;
    }
}

/**
 * read_zone
 * 读取卡片数据
 * @author Administrator (2014-11-03)
 * 
 * @param area 卡片区域
 * @param add  地址
 * @param data 读取的数据
 * @param len  长度
 * 
 * @return unsigned char TRUE--读取成功,FALSE--读取失败
 */
unsigned char read_zone(unsigned char area, unsigned char add, unsigned char *data, unsigned int len)
{
    unsigned int i;
    unsigned char retry = 2;
    while(retry--)
    {
        if(area != CON_ZONE)
            set_userzone(area);

        Start();
        if(area == CON_ZONE)
            Write8Bit(0xb5);
        else
            Write8Bit(0xb1);

        if(Ack() == FALSE)
            return FALSE;
        Write8Bit(add);
        if(Ack() == FALSE)
            return FALSE;

        for(i = 0; i < len; i++)
        {
            *(data + i) = Read8Bit();
            if(i < (len - 1)) //发送ack
            {
                SendAck();
            }
        }
        NoAck();
        Stop();
        retry = 0;
    }
    //delay_ms(3);

    return TRUE;
}

/**
 * set_userzone
 * 设置卡片读写区域
 * @author Administrator (2014-11-03)
 * 
 * @param zone 卡片区域
 */
void set_userzone(unsigned char zone)
{
    unsigned char retry = 2;
    if(zone == CurrentZone)
    {
        retry = 0;
    }
    else
    {
        CurrentZone = zone;
        PassBit[0] = 0;
        PassBit[1] = 0;
    }

    while(retry--)
    {
        Start();
        Write8Bit(0xb2);
        if(Ack() == FALSE)
            continue;
        Write8Bit(zone);
        if(Ack() == FALSE)
            continue;
        Stop();
        //retry=0;
    }
}

/**
 * verify_pass
 * 核对卡片密码
 * @author Administrator (2014-11-03)
 * 
 * @param zone     卡片区域
 * @param io       读/写
 * @param password 密码
 * 
 * @return unsigned char  TRUE--核对成功,FALSE--核对失败
 */
unsigned char verify_pass(unsigned char zone, unsigned char io, unsigned char *password)
{
//    if(PassBit[io] & (0x01 << zone))
//        return TRUE;
//    else
//        PassBit[io] = (0x01 << zone);

    {
        Start();
        Write8Bit(0xb3);
        if(Ack() == FALSE)
            return FALSE;
        if(io == WRITE)
            Write8Bit(zone);
        else
            Write8Bit((zone | 0x08) & 0x0f);
        if(Ack() == FALSE)
            return FALSE;
        Write8Bit(*(password));
        if(Ack() == FALSE)
            return FALSE;
        Write8Bit(*(password + 1));
        if(Ack() == FALSE)
            return FALSE;
        Write8Bit(*(password + 2));
        if(Ack() == FALSE)
            return FALSE;
        Stop();
    }

    DelayMs(3);
    if(read_pac(zone, io) == 0xff)
        return TRUE;
    return FALSE;
}

/**
 * encrypt
 * 计算卡内存储器密码(需要设置卡时使用)
 * @author Administrator (2014-11-03)
 * 
 * @param passbuf 
 * @param keyseed 
 * @param skey1 
 * @param skey2 
 */
void encrypt(unsigned char *passbuf, unsigned char *keyseed, unsigned char *skey1, unsigned char *skey2)
{
    unsigned char buf1[4], buf2[4], buf4, buf5;
    unsigned char buf3[3] = {0xb6, 0xa7, 0x2d};
    unsigned long a, b;

    buf1[0] = (*(keyseed + 0) & 0xf0) | (*(keyseed + 2) & 0x0f);
    buf1[1] = (*(keyseed + 2) & 0xf0) | (*(keyseed + 0) & 0x0f);
    buf1[2] = ((*(keyseed + 3) & 0x0f) << 4) | ((*(keyseed + 1) & 0xf0) >> 4);
    buf1[3] = ((*(keyseed + 1) & 0x0f) << 4) | ((*(keyseed + 3) & 0xf0) >> 4);

    buf4 = buf1[1] & 0x0f;

    if(buf4 % 2 != 0)
        buf4 = (buf4 - 1) / 2 + 8;
    else
        buf4 = buf4 / 2;

    buf5 = buf1[2];

    if(buf5 % 2 != 0)
        buf5 = (buf5 - 1) / 2 + 128;
    else
        buf5 = buf5 / 2;

    buf1[1] = (buf1[1] & 0xf0) | (buf4 & 0x0f);
    buf1[2] = buf5;

    a = buf1[0];
    a <<= 8;
    a += buf1[1];
    a <<= 8;
    a += buf1[2];
    a <<= 8;
    a += buf1[3];

    b = (*(skey1 + 0));
    b = b << 8;
    b += (*(skey1 + 1));
    b = b << 8;
    b += (*(skey1 + 2));

    a -= b;

    buf1[3] = a & 0xff;
    a >>= 8;
    buf1[2] = a & 0xff;
    a >>= 8;
    buf1[1] = a & 0xff;
    a >>= 8;
    buf1[0] = a & 0xff;

    buf2[0] = buf1[0];
    buf2[1] = buf1[3];
    buf2[2] = buf1[1];
    buf2[3] = buf1[2];

    a = buf2[0];
    a <<= 8;
    a += buf2[1];
    a <<= 8;
    a += buf2[2];
    a <<= 8;
    a += buf2[3];
    b = (*(skey2 + 0));
    b = b << 8;
    b += (*(skey2 + 1));
    b = b << 8;
    b += (*(skey2 + 2));
    a -= b;

    buf1[3] = a & 0xff;
    a >>= 8;
    buf1[2] = a & 0xff;
    a >>= 8;
    buf1[1] = a & 0xff;
    a >>= 8;
    buf1[0] = a & 0xff;

    buf2[0] = buf1[0];
    buf2[1] = ((buf1[3] & 0xf0)) | ((buf1[1] & 0xf0) >> 4);
    buf2[2] = ((buf1[2] & 0x0f) << 4) | (buf1[1] & 0x0f);
    buf2[3] = (buf1[2] & 0xf0) | (buf1[3] & 0x0f);

    a = buf2[0];
    a <<= 8;
    a += buf2[1];
    a <<= 8;
    a += buf2[2];
    a <<= 8;
    a += buf2[3];

    b = buf3[0];
    b = b << 8;
    b += buf3[1];
    b = b << 8;
    b += buf3[2];
    a -= b;

    *(passbuf + 2) = a & 0xff;
    a >>= 8;
    *(passbuf + 1) = a & 0xff;
    a >>= 8;
    *(passbuf + 0) = a & 0xff;
    a >>= 8;
    //最高位舍去
}

/**
 * E_encrypt
 * 计算卡内存储器密码(不需设置卡时使用)
 * @author Administrator (2014-11-03)
 * 
 * @param passbuf 
 * @param keyseed 
 * @param skey1 
 * @param skey2 
 */
void E_encrypt(unsigned char *passbuf, const unsigned char *keyseed, const unsigned char *skey1, const unsigned char *skey2)
{
    unsigned char buf1[4], buf2[4], buf4, buf5;
    unsigned char skey3[4] = {0x5A, 0x43, 0x7E, 0x08};

    buf1[0] = (*(keyseed + 0) & 0xf0) | (*(keyseed + 2) & 0x0f);
    buf1[1] = (*(keyseed + 2) & 0xf0) | (*(keyseed + 0) & 0x0f);
    buf1[2] = ((*(keyseed + 3) & 0x0f) << 4) | ((*(keyseed + 1) & 0xf0) >> 4);
    buf1[3] = ((*(keyseed + 1) & 0x0f) << 4) | ((*(keyseed + 3) & 0xf0) >> 4);

    buf4 = buf1[1] & 0x0f;

    if(buf4 % 2 != 0)
        buf4 = (buf4 - 1) / 2 + 8;
    else
        buf4 = buf4 / 2;

    buf5 = buf1[2];

    if(buf5 % 2 != 0)
        buf5 = (buf5 - 1) / 2 + 128;
    else
        buf5 = buf5 / 2;

    buf1[1] = (buf1[1] & 0xf0) | (buf4 & 0x0f);
    buf1[2] = buf5;
    /*
    a=buf1[0];a<<=8;
    a+=buf1[1];a<<=8;
    a+=buf1[2];a<<=8;
    a+=buf1[3];

    b=(*(skey1+0));
    b=b<<8;
    b+=(*(skey1+1));
    b=b<<8;
    b+=(*(skey1+2));

    a-=b;

    buf1[3]=a&0xff; a>>=8;
    buf1[2]=a&0xff; a>>=8;
    buf1[1]=a&0xff; a>>=8;
    buf1[0]=a&0xff;
     */
    buf1[0] ^= *(skey1 + 0);
    buf1[1] ^= *(skey1 + 1);
    buf1[2] ^= *(skey1 + 2);
    buf1[3] ^= *(skey1 + 3);



    buf2[0] = buf1[0];
    buf2[1] = buf1[3];
    buf2[2] = buf1[1];
    buf2[3] = buf1[2];

    /*
      a=buf2[0];a<<=8;
      a+=buf2[1];a<<=8;
      a+=buf2[2];a<<=8;
      a+=buf2[3];
      b=(*(skey2+0));
      b=b<<8;
      b+=(*(skey2+1));
      b=b<<8;
      b+=(*(skey2+2));
      a-=b;

      buf1[3]=a&0xff; a>>=8;
      buf1[2]=a&0xff; a>>=8;
      buf1[1]=a&0xff; a>>=8;
      buf1[0]=a&0xff;
     */

    buf1[0] = buf2[0] ^ (*(skey2 + 0));
    buf1[1] = buf2[1] ^ (*(skey2 + 1));
    buf1[2] = buf2[2] ^ (*(skey2 + 2));
    buf1[3] = buf2[3] ^ (*(skey2 + 3));


    buf2[0] = buf1[0];
    buf2[1] = ((buf1[3] & 0xf0)) | ((buf1[1] & 0xf0) >> 4);
    buf2[2] = ((buf1[2] & 0x0f) << 4) | (buf1[1] & 0x0f);
    buf2[3] = (buf1[2] & 0xf0) | (buf1[3] & 0x0f);

    /*
      a=buf2[0];a<<=8;
      a+=buf2[1];a<<=8;
      a+=buf2[2];a<<=8;
      a+=buf2[3];

      b=buf3[0];
      b=b<<8;
      b+=buf3[1];
      b=b<<8;
      b+=buf3[2];

      a-=b;


      *(passbuf+2)=a&0xff; a>>=8;
      *(passbuf+1)=a&0xff; a>>=8;
      *(passbuf+0)=a&0xff; a>>=8;
      //最高位舍去
    */
    buf1[0] = buf2[0] ^ skey3[0];
    buf1[1] = buf2[1] ^ skey3[1];
    buf1[2] = buf2[2] ^ skey3[2];
    buf1[3] = buf2[3] ^ skey3[3];

    *(passbuf + 2) = buf1[3];
    *(passbuf + 1) = buf1[2];
    *(passbuf + 0) = buf1[1];
}

/**
 * read_cmc
 * 读取卡片cmc
 * @author Administrator (2014-11-03)
 * 
 * @param cmcbuf 读出cmc存放地址
 */
void read_cmc(unsigned char *cmcbuf)
{
    read_zone(CON_ZONE, 0x0c, cmcbuf, 4);
}

/**
 * read_ar
 * 读取卡片ar
 * @author Administrator (2014-11-03)
 * 
 * @param arbuf 读出的ar存放地址
 */
void read_ar(unsigned char *arbuf)
{
    read_zone(CON_ZONE, 0x10, arbuf, 8);
}

/**
 * read_pac
 * 读取卡片pac，用于验证核对密码是否成功
 * @author Administrator (2014-11-03)
 * 
 * @param area 区域
 * @param io   读或写
 * 
 * @return unsigned char  0xff--核对成功 , other--核对失败
 */
unsigned char read_pac(unsigned char area, unsigned char io)
{
    unsigned char pacadd, pacbuf;

    pacadd = 0x40;
    pacadd += (area * 8);
    if(io == READ)
        pacadd += 4;

    read_zone(CON_ZONE, pacadd, &pacbuf, 1);
    return pacbuf;
}
