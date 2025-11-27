#ifndef _DEFINE_H_
#define _DEFINE_H_


#ifndef TRUE
#define TRUE    1
#define FALSE   0
#endif


typedef enum
{
    Dis = 0,
    En,
}EnDis;

typedef enum
{
    IODIR_Input = 0,    //输入
    IODIR_Output,       //输出
}IODIR;

typedef enum
{
    IOPP_PD = 0,        //下拉
    IOPP_PP,            //上拉
    IOPP_FL,            //悬空
}IOPP;

typedef enum
{
    IOVAL_L = 0,        //低电平
    IOVAL_H,            //高电平
    IOVAL_S,            //切换电平
}IOVAL;

#define Nop  asm("nop")


extern void IrqServerDefaultRS(unsigned char Data);

#endif