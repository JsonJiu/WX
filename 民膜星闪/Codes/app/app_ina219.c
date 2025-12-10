#include "config.h"
#include "app_ina219.h"

#define INA219DelayCount 10

#define IO_SCL_OUT   {TRISFbits.TRISF6 = 0; ODCFbits.ODF6 = 0;}
#define IO_SCL_1     LATFbits.LATF6 = 1
#define IO_SCL_0     LATFbits.LATF6 = 0

#define IO_SDA_OUT   {TRISGbits.TRISG2 = 0; ODCGbits.ODG2 = 0;}
#define IO_SDA_IN    {TRISGbits.TRISG2 = 1; ODCGbits.ODG2 = 0;}
#define IO_SDA_1     LATGbits.LATG2 = 1
#define IO_SDA_0     LATGbits.LATG2 = 0
#define IO_READ_SDA  PORTGbits.RG2

void INA219Delay(unsigned int num)
{
    while(num--)
        ;
}

void IIC_INA219_init(void)
{
    IO_SDA_OUT;
    IO_SDA_1;//SDA 1
    IO_SCL_OUT;
    IO_SCL_1;//CLK 1
}
/***************************************************************************
启动IIC总线
***************************************************************************/
void IIC_INA219_start(void)
{
    IO_SDA_OUT;
    IO_SDA_1;//SDA 1
    IO_SCL_1;//CLK 1
    INA219Delay(INA219DelayCount);
    IO_SDA_0;//SDA 0
    IO_SCL_0;//CLK 0
    INA219Delay(INA219DelayCount);
}

/***************************************************************************
停止操作，释放IIC总线
***************************************************************************/
void IIC_INA219_stop(void)
{
    IO_SDA_OUT;
    IO_SDA_0;//SDA 0
    IO_SCL_1;//CLK 1
    INA219Delay(INA219DelayCount);
    IO_SDA_1;//SDA 1
    IO_SCL_0;//CLK 0
    INA219Delay(INA219DelayCount);
    IO_SCL_1;
}

/***************************************************************************
IIC总线应答
***************************************************************************/
unsigned char IIC_INA219_ack()
{
    unsigned char time;
    IO_SDA_IN;
    IO_SCL_1;//CLK 1
    time = 255;
    while(time--)
    {
        if(IO_READ_SDA == 0) //等待为0
        {
            IO_SCL_0;
            INA219Delay(INA219DelayCount);
            return 0;
        }
        //delay_ms(1);
    }
    IO_SCL_0;
    INA219Delay(INA219DelayCount);
    return 1;
}
/***************************************************************************
IIC总线无应答
***************************************************************************/
void IIC_INA219_noack()
{
    IO_SDA_OUT;
    IO_SDA_1;
    IO_SCL_1;
    INA219Delay(INA219DelayCount);
    IO_SCL_0;
    INA219Delay(INA219DelayCount);
    //delay();
}
/***************************************************************************
IIC总线写8位数据
input：要写的8位数据
***************************************************************************/
void IIC_INA219_write8bit(unsigned char input)
{
    unsigned char temp, q0;
    IO_SDA_OUT;
    IO_SCL_0;
    INA219Delay(INA219DelayCount);
    for(temp = 8; temp != 0; temp--)
    {
        q0 = input & 0x80;
        if(q0 == 0x80)
            IO_SDA_1;
        else
            IO_SDA_0;
        //delay();
        IO_SCL_1;
        INA219Delay(INA219DelayCount);
        IO_SCL_0;
        INA219Delay(INA219DelayCount);
        input = input << 1;
    }
    INA219Delay(10);
}

/***************************************************************************
IIC总线读8位数据
返回值：读出的8位数据
***************************************************************************/
unsigned char IIC_INA219_read8bit()
{
    unsigned char temp, rbyte = 0;
    IO_SDA_IN;
    for(temp = 8; temp != 0; temp--)
    {
        IO_SCL_1;
        INA219Delay(INA219DelayCount);
        rbyte = rbyte << 1;
        if(IO_READ_SDA)
            rbyte = rbyte | 0x1;
        IO_SCL_0;
        INA219Delay(INA219DelayCount);
    }
    return(rbyte);
}

unsigned char INA219Read(unsigned char RegAddr,unsigned char *tbuf)
{
    //read 0x65
    unsigned char buf[10],i,Retry;
    unsigned char size=2;
    unsigned char DeviceAddr=0x80;

    INA219Delay(10);
    for(Retry=0;Retry<3;Retry++)
    {
        IIC_INA219_start();
        IIC_INA219_write8bit(DeviceAddr);//写
        if(IIC_INA219_ack())
            continue;
        IIC_INA219_write8bit(RegAddr);
        if(IIC_INA219_ack())
            continue;

        IIC_INA219_start();
        IIC_INA219_write8bit(DeviceAddr|0x01);//读
        if(IIC_INA219_ack())
            continue;

        for(i = 0; i < size; i++)
        {
            *(buf + i) = IIC_INA219_read8bit();
            if(i < (size - 1))
            {
                IO_SDA_OUT;  //发送ack
                IO_SDA_0;//SDA 0
                IO_SCL_1;//CLK 1
                INA219Delay(INA219DelayCount);
                IO_SCL_0;//CLK 0
                INA219Delay(INA219DelayCount);
            }
        }
        IIC_INA219_noack();
        IIC_INA219_stop();
        break;
    }

    IO_SDA_0;//SDA 0
    IO_SCL_0;//CLK 0
    if(Retry<3)
    {
        memcpy(tbuf,buf,size);
        return 0;
    }
    else
    {
        return 1;
    }
}
unsigned char INA219Write(unsigned char RegAddr,unsigned char *buf)
{
    //read 0x65
    unsigned char i,Retry;
    unsigned char size=2;
    unsigned char DeviceAddr=0x80;

    INA219Delay(10);
    for(Retry=0;Retry<3;Retry++)
    {
        IIC_INA219_start();
        IIC_INA219_write8bit(DeviceAddr);//写
        if(IIC_INA219_ack())
                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                                   continue;
        IIC_INA219_write8bit(RegAddr);
        if(IIC_INA219_ack())
            continue;

        for(i = 0; i < size; i++)
        {
            IIC_INA219_write8bit(*(buf + i));
            IIC_INA219_ack();
        }
        IIC_INA219_noack();
        IIC_INA219_stop();
        break;
    }

    IO_SDA_0;//SDA 0
    IO_SCL_0;//CLK 0
    if(Retry<3)
    {
        return 0;
    }
    else
    {
        return 1;
    }
}

unsigned int Current[10];
unsigned char CurrentCount;

unsigned int CurrentEve()
{
    unsigned long total;
    unsigned int sortCurrent[10];
    unsigned char i,k;
    ArraySort(sortCurrent,Current,10);
    total=0;
    k=0;
    for(i=1;i<9;i++)
    {
         if(sortCurrent[i]==0)
            k++;
        total+=sortCurrent[i];
    }
    return total/(8-k);
}

void CurrentInsert(unsigned int i)
{
    CurrentCount++;
//    Current[CurrentCount]=0;
    if(CurrentCount>=10)
    {
        CurrentCount=0;
    }
    Current[CurrentCount]=i;
}

unsigned char readbuf[10];
void tstIna219(void)
{
    unsigned char i,j, f;
    readbuf[0]=0x07;
    readbuf[1]=0x9f;
    INA219Write(0,readbuf);

    readbuf[0]=0x10;
    readbuf[1]=0x00;
    INA219Write(5,readbuf);
    DelayMs(50);

    for(i=0;i<6;i++)
    {
        INA219Read(4,readbuf);
//            if(INA219Read(i,readbuf));
//            {
////                printf("读取失败\r\n");
//            }
            if(i>=4)
            {
                for(j=0;j<10;j++)
                {
                    f = INA219Read(4,readbuf);
                    if(f == 0)
                        CurrentInsert((unsigned int)readbuf[0]*0x100+readbuf[1]);
                    DelayMs(10);
                }
            }
      }
}

void ArraySort(unsigned int *Array1,unsigned int *Array2,unsigned char len)
{
    unsigned int tmpInt;
    unsigned char i,j;
    if(len<=1)
        return;

    for(i=0;i<len;i++)
    {
        *(Array1+i)=*(Array2+i);
    }

    for(i=1;i<len;i++)
    {
        for(j=0;j<i;j++)
        {
            if(*(Array1+i)>*(Array1+j))
            {
                tmpInt=*(Array1+i);
                *(Array1+i)=*(Array1+j);
                *(Array1+j)=tmpInt;
            }
        }
    }
}

//IIC取到的值减去37uA为实际的值
unsigned char GetCurrent()//电流值
{
    unsigned int re;

    IIC_INA219_init();
    tstIna219();

    re = CurrentEve();

    re /= 10;
    if(re > 55)
        re -= 50;
    else
        re -= 40;

    return re;
}
