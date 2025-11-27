#include "include.h"

/*********************************************************************************************************
** Function name:       xor_str
** Descriptions:        异或两个字符串
** input parameters:    buf--异或结果 buf1--字符串1  buf2--字符串2  len--字符串长度
** output parameters:   无
** Returned value:      无
*********************************************************************************************************/
void xor_str(unsigned char *buf, unsigned char *buf1, unsigned char *buf2, unsigned char len)
{
    unsigned char i;
    for(i = 0; i < len; i++)
    {
        *(buf + i) = *(buf1 + i) ^ *(buf2 + i);
    }
}

/**
 * 复制寄存器
 * 
 * @author xsc (2014/11/25)
 * 
 * @param buf1 目标缓冲区
 * @param buf2 源缓冲区
 * @param len 长度
 */
void copy_memery(unsigned char *buf1, unsigned char *buf2, unsigned int len)
{
    unsigned int i;
    for(i = 0; i < len; i++)
    {
        *(buf1 + i) = *(buf2 + i);
    }
}

/**
 * 比较两个字符串
 * 
 * @author xsc (2014/12/2)
 * 
 * @param buf1 目标
 * @param buf2 来源
 * @param len 长度
 * 
 * @return unsigned 结果
 */
unsigned cmp_str(unsigned char *buf1, unsigned char *buf2, unsigned char len)
{
    unsigned char i;
    for(i = 0; i < len; i++)
    {
        if(*(buf1 + i) != *(buf2 + i))
            return FALSE;
    }
    return TRUE;
}


/**
 * 内存填充
 * 
 * @author xsc (2014/12/2)
 * 
 * @param data 缓存指针
 * @param len 大小
 * @param n 填充内容
 */
void mem_set(unsigned char *data, unsigned short len, unsigned short n)
{
    unsigned short i;
    for(i = 0; i < len; i++)
        *(data + i) = n;
}

/**
 * bcd码转换成hex
 * 
 * @author xsc (2014/12/2)
 * 
 * @param bcd 待转换的bcd码
 */
void bcdtohex(unsigned long *bcd)
{
    unsigned long buf1 = *bcd;
    unsigned long buf2 = 0;

    buf2 += (buf1 & 0x0000000f);
    buf1 >>= 4;
    buf2 += (buf1 & 0x0000000f) * 10;
    buf1 >>= 4;
    buf2 += (buf1 & 0x0000000f) * 100;
    buf1 >>= 4;
    buf2 += (buf1 & 0x0000000f) * 1000;
    buf1 >>= 4;
    buf2 += (buf1 & 0x0000000f) * 10000;
    buf1 >>= 4;
    buf2 += (buf1 & 0x0000000f) * 100000;
    buf1 >>= 4;
    buf2 += (buf1 & 0x0000000f) * 1000000;
    buf1 >>= 4;
    buf2 += (buf1 & 0x0000000f) * 10000000;

    *bcd = buf2;
}

unsigned char charTobcd(unsigned char hex)
{
    return ((hex/10)<<4)&0xf0 | ((hex%10)&0x0f);
}


/**
 * 先倒置再将bcd码转换成hex
 * 
 * @author xsc (2014/12/2)
 * 
 * @param bcd 待转换的bcd码
 */
void rollbcdtohex(unsigned long *bcd)
{
    unsigned long buf1 = *bcd;
    unsigned long buf2 = 0;
    buf2 |= (buf1 & 0x000000ff);
    buf1 >>= 8;
    buf2 <<= 8;
    buf2 |= (buf1 & 0x000000ff);
    buf1 >>= 8;
    buf2 <<= 8;
    buf2 |= (buf1 & 0x000000ff);
    buf1 >>= 8;
    buf2 <<= 8;
    buf2 |= (buf1 & 0x000000ff);

    *bcd = buf2;
    bcdtohex(bcd);
}

/**
 * HEX转BCD
 * 
 * @author xsc (2014/12/2)
 * 
 * @param hex 
 */
void hextobcd(unsigned long *hex)
{
    unsigned long buf1 = 0;
    unsigned long buf2 = *hex;
    buf1 += buf2 / 10000000;
    buf2 %= 10000000;
    buf1 <<= 4;
    buf1 += buf2 / 1000000;
    buf2 %= 1000000;
    buf1 <<= 4;
    buf1 += buf2 / 100000;
    buf2 %= 100000;
    buf1 <<= 4;
    buf1 += buf2 / 10000;
    buf2 %= 10000;
    buf1 <<= 4;
    buf1 += buf2 / 1000;
    buf2 %= 1000;
    buf1 <<= 4;
    buf1 += buf2 / 100;
    buf2 %= 100;
    buf1 <<= 4;
    buf1 += buf2 / 10;
    buf2 %= 10;
    buf1 <<= 4;
    buf1 += buf2;

    *hex = buf1;
}

/**
 * 先倒置再将hex码转换成bcd
 * 
 * @author xsc (2014/12/2)
 * 
 * @param hex 
 */
void rollhextobcd(unsigned long *hex)
{
    unsigned long buf1;
    unsigned long buf2 = 0;
    hextobcd(hex);
    buf1 = *hex;
    buf2 |= (buf1 & 0x000000ff);
    buf1 >>= 8;
    buf2 <<= 8;
    buf2 |= (buf1 & 0x000000ff);
    buf1 >>= 8;
    buf2 <<= 8;
    buf2 |= (buf1 & 0x000000ff);
    buf1 >>= 8;
    buf2 <<= 8;
    buf2 |= (buf1 & 0x000000ff);

    *hex = buf2;
}

/**
 * BCD转HEX
 * 
 * @author xsc (2014/12/2)
 * 
 * @param T_buf 
 * 
 * @return unsigned char 
 */
unsigned char CharBCDtoHEX(unsigned char T_buf)
{
    unsigned char ReturnBuf;
    ReturnBuf = (T_buf & 0xf0) / 0x10 * 10;
    ReturnBuf += T_buf & 0x0f;
    return ReturnBuf;
}

unsigned char CharBCDtoHEX_S(unsigned char T_buf)
{
    unsigned char ReturnBuf;
    ReturnBuf = (T_buf & 0xf0) / 0x10;
    ReturnBuf += (T_buf & 0x0f) * 10;
    return ReturnBuf;
}

/**
 * HEX转BCD
 * 
 * @author xsc (2014/12/2)
 * 
 * @param T_buf 
 * 
 * @return unsigned char 
 */
unsigned char ByteHEXtoBCD(unsigned char T_buf)
{
    unsigned char ReturnBuf;
    ReturnBuf = T_buf / 10 * 0x10;
    ReturnBuf += T_buf % 10;
    return ReturnBuf;
}

/**
 * BCD转HEX 大端模式，倒转
 * 
 * @author xsc (2014/12/2)
 * 
 * @param T_buf 
 * @param T_len 
 * 
 * @return unsigned long 
 */
unsigned long BCDtoHEX_B(unsigned char *T_buf, unsigned char T_len)
{
    unsigned char i;
    unsigned long ReturnBuf = 0;
    for(i = 0; i < T_len; i++)
    {
        if(((*(T_buf + i)&0xf0)>0x90) || ((*(T_buf + i)&0x0f)>0x09))
            return 0xffffffffl;
        ReturnBuf *= 100;
        ReturnBuf += CharBCDtoHEX(*(T_buf + T_len - i - 1));
    }
    return ReturnBuf;
}

/**
 * BCD转HEX 小端模式，顺转
 * 
 * @author xsc (2014/12/2)
 * 
 * @param T_buf 
 * @param T_len 
 * 
 * @return unsigned long 
 */
unsigned long BCDtoHEX_S(unsigned char *T_buf, unsigned char T_len)
{
    unsigned char i;
    unsigned long ReturnBuf = 0;
    for(i = 0; i < T_len; i++)
    {
        if(((*(T_buf + i)&0xf0)>0x90) || ((*(T_buf + i)&0x0f)>0x09))
            return 0xffffffffl;
        ReturnBuf *= 100;
        ReturnBuf += CharBCDtoHEX(*(T_buf + i));
    }
    return ReturnBuf;
}

unsigned long BCDtoHEX_SS(unsigned char *T_buf, unsigned char T_len)
{
    unsigned char i;
    unsigned long ReturnBuf = 0;
    for(i = 0; i < T_len; i++)
    {
        if(((*(T_buf + i)&0xf0)>0x90) || ((*(T_buf + i)&0x0f)>0x09))
            return 0;
        ReturnBuf *= 100;
        ReturnBuf += CharBCDtoHEX_S(*(T_buf + i));
    }
    return ReturnBuf;
}

/**
 * HEX转BCD 大端模式，倒转
 * 
 * @author xsc (2014/12/2)
 * 
 * @param T_buf 
 * @param T_buflong 
 * @param T_len 
 */
void HEXtoBCD_B(unsigned char *T_buf, unsigned long T_buflong, unsigned char T_len)
{
    unsigned char i;
    unsigned char T_bufchar;
    for(i = 0; i < T_len; i++)
    {
        T_bufchar = T_buflong % 100;
        T_buflong /= 100;
        *(T_buf + i) = T_bufchar / 10 * 0x10 + T_bufchar % 10;
    }
}

/**
 * BCD转HEX 小端模式，顺转
 * 
 * @author xsc (2014/12/2)
 * 
 * @param T_buf 
 * @param T_buflong 
 * @param T_len 
 */
void HEXtoBCD_S(unsigned char *T_buf, unsigned long T_buflong, unsigned char T_len)
{
    unsigned char i;
    unsigned char T_bufchar;
    for(i = 0; i < T_len; i++)
    {
        T_bufchar = T_buflong % 100;
        T_buflong /= 100;
        *(T_buf + T_len - i - 1) = T_bufchar / 10 * 0x10 + T_bufchar % 10;
    }
}


unsigned char CrcAddGet(unsigned char *T_buf, unsigned int T_len)
{
    unsigned int i;
    unsigned char R_crc = 0;
    for(i = 0; i < T_len; i++)
    {
        R_crc += *(T_buf + i);
    }
    return R_crc;
}

unsigned char CrcAddCheck(unsigned char T_crc, unsigned char *T_buf, unsigned int T_len)
{
    unsigned char R_crc;
    R_crc = CrcAddGet(T_buf, T_len);
    if(R_crc == T_crc)
        return TRUE;
    else
        return FALSE;
}

/**
 * 比较日期
 * 
 * @author xsc (2014-04-08)
 * 
 * @param T_date1 
 * @param T_date2 
 * @param CmpSegs 比较的位数  年 月 日 小时 分钟  秒 1  2  3   4    5    6 
 * 
 * @return unsigned char 1>2返回1;2>1返回2;相同返回0
 */
unsigned char DateCmp(RTC_T T_date1,RTC_T T_date2,unsigned char CmpSegs)
{
    unsigned long TmpDate1,TmpDate2;

    TmpDate1=T_date1.Year;
    TmpDate1<<=8;
    TmpDate2=T_date2.Year;
    TmpDate2<<=8;

    if(CmpSegs>1)
    {
    TmpDate1+=T_date1.Mon;
    TmpDate1<<=8;
    TmpDate2+=T_date2.Mon;
    TmpDate2<<=8;
    }

    if(CmpSegs>2)
    {
    TmpDate1+=T_date1.Day;
    TmpDate1<<=8;
    TmpDate2+=T_date2.Day;
    TmpDate2<<=8;
    }

    if(CmpSegs>3)
    {
    TmpDate1+=T_date1.Hour;
    TmpDate2+=T_date2.Hour;
    }
    if(TmpDate1>TmpDate2)
    {
        return 1;
    }
    else if(TmpDate1<TmpDate2)
    {
        return 2;
    }
    else
    {
        if(CmpSegs>4)
        {
            TmpDate1=T_date1.Min;
            TmpDate1<<=8;
            TmpDate2=T_date2.Min;
            TmpDate2<<=8;
        }
        if(CmpSegs>5)
        {
            TmpDate1+=T_date1.Sec;
            TmpDate2+=T_date2.Sec;
        }

        if(TmpDate1>TmpDate2)
            return 1;
        else if(TmpDate1<TmpDate2)
            return 2;
        else
            return 0;
    }
}

/**
 * 时间转秒
 * 
 * @author xsc (2015-03-12)
 * 
 * @param time 
 * 
 * @return unsigned long 
 */
unsigned long TimeToSecond(RTC_T * time)
{
    unsigned long sec;
    sec = CharBCDtoHEX(time->Hour) * 60;
    sec += CharBCDtoHEX(time->Min);
    sec *= 60;
    sec += CharBCDtoHEX(time->Sec);
    return sec;
}



/*******************************************************************************
名   称: LeapYear
功   能: 闰年判断
入口参数: 无
返   回: 闰年的状态
*******************************************************************************/
unsigned char Leap_Year(unsigned int t_year)
{
    if((t_year%4==0&&t_year%100!=0)||t_year%400==0)//闰年的条件,能同时被和整除的或者能被整除的年份就是闰年
        return 1;
    else
        return 0;
}

/**
 * 将月份转换成天数
 * 
 * @author xsc (2014-04-09)
 * 
 * @param now 
 * 
 * @return int 
 */
unsigned long Month_To_Day(RTC_T * now)
{
    int i, day = 0;

    for (i = 0; i < CharBCDtoHEX(now->Mon) - 1; ++i)
    {
        day = day + month[i];
    }

    return (CharBCDtoHEX(now->Mon) >= 3) ? day + CharBCDtoHEX(now->Day) + Leap_Year(CharBCDtoHEX(now->Year)) :  day + CharBCDtoHEX(now->Day);
 
}

/**
 * 计算年相对起始年份相差天数  year_begin <= year_end
 * 
 * @author xsc (2014-04-09)
 * 
 * @param year_begin 起始年份
 * @param year_end 结束年份
 * 
 * @return int 相差天数
 */
int Year_To_Day(int year_begin, int year_end)
{
    int sum = 0;
    int number = year_begin;

    while(number != year_end)
    {
        sum = sum + 365 + Leap_Year(year_begin);
        number++;
    }

    return sum;
}

/**
 * 两个时间时分秒的差值
 * 
 * @author xsc (2014-04-09)
 * 
 * @param t1 
 * @param t2 
 * @param t 返写的时间差值
 * 
 * @return unsigned char 0:t1>t2  1:t2>t1
 */
unsigned long CompSecond(RTC_T * t1, RTC_T * t2,unsigned char *t)
{
    unsigned long s1, s2;
    s1 = TimeToSecond(t1);
    s2 = TimeToSecond(t2);
    if (s1 > s2) {
        s1= s1 - s2;
        *t = 0;
    }else{
        s1 = s2 - s1;
        *t = 1;
    }
    return s1;
}


/**
 * 比较两者时间差
 * 
 * @author xsc (2014-04-09)
 * 
 * @param Time1 时间1
 * @param Time2 时间2
 * 
 * @return unsigned long 两时间相差秒数
 */
unsigned long CompTime(RTC_T  Time1, RTC_T  Time2)
{
    unsigned long long t,temp;
    unsigned char result;
    RTC_T * t1, * t2;
    //判断时间哪个大 调整时间，大的给t1,小的给t2
    result = DateCmp(Time1, Time2, 6);
    if (result == 1) {  //
        t1 = &Time1;
        t2 = &Time2;
    }else if(result == 2){
        t1 = &Time2;
        t2 = &Time1;
    }else if(result == 0){
        return 0;
    }

    temp = Year_To_Day(CharBCDtoHEX(t2->Year), CharBCDtoHEX(t1->Year)) - Month_To_Day(t2) + Month_To_Day(t1);
    t = temp * 86400;
    temp = CompSecond(t1, t2, &result);
    if (result == 0) {
        t += temp;
    }else{
        t -= temp;
    }

    return t;
}

/**
 * 比较两者时间差
 * 
 * @author xsc (2014-04-09)
 * 
 * @param Time1 时间1
 * @param Time2 时间2
 * 
 * @return unsigned long 两时间相差秒数
 */
unsigned long RTCToDay(RTC_T  Time1, RTC_T  Time2)
{
    unsigned long long t,tt1,tt2;
    unsigned char result;
    RTC_T * t1, * t2;
    //判断时间哪个大 调整时间，大的给t1,小的给t2
    result = DateCmp(Time1, Time2, 3);
    if (result == 1) {  //
        t1 = &Time1;
        t2 = &Time2;
    }else if(result == 2){
        t1 = &Time2;
        t2 = &Time1;
    }else if(result == 0){
        return 0;
    }

    t = Year_To_Day(CharBCDtoHEX(t2->Year), CharBCDtoHEX(t1->Year));
    tt1 = Month_To_Day(t1);
    tt2 = Month_To_Day(t2);

    t = t + tt1 - tt2;

    return t;
}



void AddOneDay(RTC_T *t)
{
    unsigned char temp;

    temp = CharBCDtoHEX(t->Day);
    if (temp < 28) { //日期未到28，直接+1
        temp ++;
        t->Day = ByteHEXtoBCD(temp);
        return;
    }else{
        if (t->Mon == 2) {  //二月
            if(CheckFeb(CharBCDtoHEX(t->Year)) == 1)    //闰月
            {
                 if(temp < 29) {
                     temp ++;
                     t->Day = ByteHEXtoBCD(temp);
                 }else{
                     t->Day = 1;
                     t->Mon = 3;
                 }
            }else{
                t->Day = 1;
                t->Mon = 3;
            }
        }else{
            temp = CharBCDtoHEX(t->Mon);
            if(CharBCDtoHEX(t->Day) >= month[temp - 1]){
                t->Day = 1;
                if (temp == 12) {
                    temp = 1;
                }else{
                    temp ++;
                }
                t->Mon = ByteHEXtoBCD(temp);
                if (t->Mon == 1) {
                    temp = CharBCDtoHEX(t->Year);
                    temp ++;
                    t->Year = ByteHEXtoBCD(temp);
                }
            }else{
                temp = CharBCDtoHEX(t->Day) + 1;
                t->Day = ByteHEXtoBCD(temp);
            }
        }
    }
}


