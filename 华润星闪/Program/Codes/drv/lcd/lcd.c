/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
//#include "lib_rs004.h" 
#include "lcd.h"

/***********************************************************************************************************************
 Extern
***********************************************************************************************************************/
extern void LcdWriteReg(unsigned char Seg, unsigned char Reg);
extern unsigned char LcdReadReg(unsigned char Seg);
extern void LcdAllSet(void);
extern void LcdInit(void);
extern void LcdOn(void);
extern void LcdOff(void);
/***********************************************************************************************************************
 Pre
***********************************************************************************************************************/
void LcdWriteBuf(unsigned char index, unsigned char buf);

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/**
 * 显示数据缓存
 * 
 * @author Zzzzz (2014-11-09)
 */
unsigned char LcdDataBuf[21];

/**
 * 段码真值表
 *
 * @author Zzzzz (2014-11-09)
 */
const char LcdDigit[] =
{
    //数字
    0x5f,//0xaf,  // '0'
    0x50,//0x06,  // '1'
    0x6b,//0x6d,  // '2'
    0x79,//0x4f,  // '3'
    0x74,//0xc6,  // '4'
    0x3d,//0xcb,  // '5'
    0x3f,//0xeb,  // '6'
    0x58,//0x0e,  // '7'
    0x7f,//0xef,  // '8'
    0x7d,//0xcf,   // '9'
    0x7e,//0xee,   //A 10
    0x37,//0xe3,   //b 11
    0x0f,//0xa9,   //c 12
    0x73,//0x67,   //d 13
    0x2f,//0xe9,   //E 14
    0x2e,//0xe8,   //F 15
    //字符
    0x20,//0x40,   //- 16
    0x76,//0xe6,   //H 17
    0x07,//0xa1,   //L 18
    0x32,//0x62,   //n 19
    0x5f,//0x23,   //O 20
    0x6e,//0xec,   //p 21
    0x22,//0x60,   //r 22
    0x33,//0x63,   //o 23
    0x00,          //  24
    0x27,          //t 25
    0x7d,//0xab,   //g 26
    0x5e,//0xae,   //N 27
    0x57,//0xa7,   //U 28
    0xa8,          //R 29
    0x10,//0x02,   //i 30
    0x23,//0xa9,   //c 31
};

/**
 * 写寄存器低位
 * 
 * @author Zzzzz (2014-11-09)
 * 
 * @param index 
 *          寄存器序号
 * @param buf
 *           
 */
void LcdWriteBuf(unsigned char index, unsigned char buf)
{
    LcdDataBuf[index] &= 0xf0;
    LcdDataBuf[index] |= buf & 0x0f;
    LcdWriteReg(index, LcdDataBuf[index]);//WriteLcdReg(index, LcdDataBuf[index]);
}

/**
 * 写寄存器高位
 * 
 * @author Zzzzz (2014-11-09)
 * 
 * @param index
 *          寄存器序号
 * @param buf 
 */
void LcdWriteBufHi(unsigned char index, unsigned char buf)
{
    LcdDataBuf[index] &= 0x0f;
    LcdDataBuf[index] |= buf << 4;
    LcdDataBuf[index] |= buf & 0x10;
    LcdWriteReg(index, LcdDataBuf[index]);//WriteLcdReg(index, LcdDataBuf[index]);
}

/** 
 * 置位 
 * 
 * @author Zzzzz (2014-11-09)
 * 
 * @param index 
 *          寄存器序号
 * @param bits 
 *          LCD_DISP_BIT_0～LCD_DISP_BIT_7 
 */
void LcdSetBit(unsigned char index,unsigned char bits)
{
    LcdDataBuf[index] |= bits;
    LcdWriteReg(index, LcdDataBuf[index]);//WriteLcdReg(index, LcdDataBuf[index]);
}

/**
 * 复位
 *
 * @author Zzzzz (2014-11-09)
 * 
 * @param index
 *          寄存器序号 
 * @param bits 
 *          LCD_DISP_BIT_0～LCD_DISP_BIT_7
 */
void LcdResetBit(unsigned char index,unsigned char bits)
{
    LcdDataBuf[index] &= ~(bits);
    LcdWriteReg(index, LcdDataBuf[index]);//WriteLcdReg(index, LcdDataBuf[index]);
}

/**
 * 控制显示标识
 * 
 * @author Zzzzz (2014-11-09)
 *
 * @param MarkIndex 
 *          标识序号 
 * @param t
 *          0-不显示 1-显示 
 */
void OV_LcdDispMark(_EnumMark MarkIndex,char t)
{
    switch(MarkIndex)
    {
    case MARK_AMOUNT:
        t?(LcdSetBit(2,2)):(LcdResetBit(2,2));
        break;
    case MARK_PRICE:
        t?(LcdSetBit(6,2)):(LcdResetBit(6,2));
        break;
    case MARK_REMAIN:
        t?(LcdSetBit(2,4)):(LcdResetBit(2,4));
        break;
    case MARK_STEP:
        t?(LcdSetBit(5,8)):(LcdResetBit(5,8));
        break;
    case MARK_SIGN1:
        t?(LcdSetBit(5,1)):(LcdResetBit(5,1));
        break;
    case MARK_SIGN2:
        t?(LcdSetBit(5,2)):(LcdResetBit(5,2));
        break;
    case MARK_SIGN3:
        t?(LcdSetBit(5,4)):(LcdResetBit(5,4));
        break;
    case MARK_ERR:
        t?(LcdSetBit(6,1)):(LcdResetBit(6,1));
        break;
    case MARK_GAS:
        t?(LcdSetBit(1,1)):(LcdResetBit(1,1));
        break;
    /*case MARK_CURRENT:
        t?(LcdSetBit(14,7)):(LcdResetBit(14,7));
        break;*/
    case MARK_MONEY:
        t?(LcdSetBit(1,2)):(LcdResetBit(1,2));
        break;
    /*case MARK_PAST:
        t?(LcdSetBit(14,4)):(LcdResetBit(14,4));
        break;*/
    case MARK_MONTH:
        t?(LcdSetBit(2,1)):(LcdResetBit(2,1));
        break;
    case MARK_OPEN:
        t?(LcdSetBit(6,8)):(LcdResetBit(6,8));
        break;
    case MARK_CLOSE:
        t?(LcdSetBit(0,1)):(LcdResetBit(0,1));
        break;
    case MARK_BATT1:
        t?(LcdSetBit(0,2)):(LcdResetBit(0,2));
        break;
    case MARK_BATT2:
        t?(LcdSetBit(0,4)):(LcdResetBit(0,4));
        break;
    case MARK_BATT3:
        t?(LcdSetBit(0,8)):(LcdResetBit(0,8));
        break;
    case MARK_BATT_FREAM:
        t?(LcdSetBit(0,2)):(LcdResetBit(0,2));
        break;
    /*case MARK_CHECK:
        t?(LcdSetBit(1,4)):(LcdResetBit(1,4));
        break;*/
    case MARK_CONSUMED:
        t?(LcdSetBit(2,8)):(LcdResetBit(2,8));
        break;
    case MARK_FREEZE:
        t?(LcdSetBit(1,4)):(LcdResetBit(1,4));
        break;
    case MARK_BACKSLASH:
        t?(LcdSetBit(8,8)):(LcdResetBit(8,8));
        break;
   /* case MARK_PRESS:
        t?(LcdSetBit(0,6)):(LcdResetBit(0,6));
        break;*/
    case MARK_CHARGE:
        t?(LcdSetBit(1,8)):(LcdResetBit(1,8));
        break;
    case MARK_YAN:
        t?(LcdSetBit(20,8)):(LcdResetBit(20,8));
        break;
    case MARK_M3:
        t?(LcdSetBit(6,4)):(LcdResetBit(6,4));
        break;
    case MARK_1:
        t?(LcdSetBit(4,8)):(LcdResetBit(4,8));
        break;
    case MARK_2POINT:
        t?(LcdSetBit(10,8)):(LcdResetBit(10,8));
        break;
    default:
        break;
    }
}

/**
 * 显示小数点
 * 
 * @author Zzzzz (2014-11-09)
 * 
 * @param line 
 * @param dots
 */

/*void LcdSetDots(char line,unsigned int dots)
{
    if(line==1)
    {
        switch(dots)
        {
        case 1:
            LcdSetBit(2,4);
            LcdResetBit(4,4);
            LcdResetBit(6,4);
            LcdResetBit(8,4);
            break;
        case 2:
            LcdSetBit(4,4);
            LcdResetBit(2,4);
            LcdResetBit(6,4);
            LcdResetBit(8,4);
            break;
        case 3:
            LcdSetBit(6,4);
            LcdResetBit(2,4);
            LcdResetBit(4,4);
            LcdResetBit(8,4);
            break;
        case 4:
            LcdSetBit(8,4);
            LcdResetBit(2,4);
            LcdResetBit(4,4);
            LcdResetBit(6,4);
            break;
        default:
            LcdResetBit(2,4);
            LcdResetBit(4,4);
            LcdResetBit(6,4);
            LcdResetBit(8,4);
            break;
        }
    }
    else
    {*/
void LcdSetDots(unsigned int dots)
{
        switch(dots)
        {
        case 1:
            LcdResetBit(12,8);
            LcdResetBit(14,8);
            LcdResetBit(16,8);
            LcdSetBit(18,8);
            break;
        case 2:
            LcdResetBit(12,8);
            LcdResetBit(14,8);
            LcdSetBit(16,8);
            LcdResetBit(18,8);
            break;
        case 3:
            LcdResetBit(12,8);
            LcdSetBit(14,8);
            LcdResetBit(16,8);
            LcdResetBit(18,8);
            break;
        case 4:
            LcdSetBit(12,8);
            LcdResetBit(14,8);
            LcdResetBit(16,8);
            LcdResetBit(18,8);
            break;
        default:
            LcdResetBit(12,8);
            LcdResetBit(14,8);
            LcdResetBit(16,8);
            LcdResetBit(18,8);
            break;
        }
    }

void OV_LcdClearValueMark(void)
{
    OV_DispMonth(0);
    OV_LcdDispMark(MARK_AMOUNT,0);
    OV_LcdDispMark(MARK_PRICE,0);
    OV_LcdDispMark(MARK_REMAIN,0);
    OV_LcdDispMark(MARK_STEP,0);
    OV_LcdDispMark(MARK_GAS,0);
    OV_LcdDispMark(MARK_MONEY,0);
    OV_LcdDispMark(MARK_MONTH,0);
    OV_LcdDispMark(MARK_CONSUMED,0);
    OV_LcdDispMark(MARK_FREEZE,0);
    OV_LcdDispMark(MARK_BACKSLASH,0);
    OV_LcdDispMark(MARK_CHARGE,0);
    OV_LcdDispMark(MARK_YAN,0);
    OV_LcdDispMark(MARK_M3,0);
    OV_LcdDispMark(MARK_1,0);
    OV_LcdDispMark(MARK_2POINT,0);
    LcdSetDots(0);
}

/**
 * 显示字符
 * 
 * @author Zzzzz (2014-11-09)
 * 
 * @param index
 *          字符序号
 * @param ch 
 *          段码表序号
 */
void LcdDispChar(unsigned char index, unsigned char ch)
{
    if(index < 7)
    {
        LcdWriteBuf(8 + index * 2, LcdDigit[ch] >> 4 | (LcdDataBuf[8 + index * 2] & 0x08));
        LcdWriteBuf(7 + index * 2, LcdDigit[ch] & 0x0f);
    }
    else
    {
        index=2;
        LcdWriteBuf(8 - index * 2, LcdDigit[ch] >> 4 | (LcdDataBuf[8 + index * 2] & 0x00));
        LcdWriteBuf(7 - index * 2, LcdDigit[ch] & 0x0f);
    }
}

/**
 * 跟LcdDispChar功能相同的对外接口
 *  
 * @author Zzzzz (2014-11-09)
 *
 * @param index 
 * @param ch 
 */
void OV_LcdDispChar(unsigned char index, unsigned char ch)
{
    LcdDispChar(index,ch);
}

/**
 * 刷新显示
 * 
 * @author Zzzzz (2014-11-09)
 * 
 * @param tLcdDataBuf 
 *          显示的寄存器缓存
 */
void OV_LcdRefresh(unsigned char *tLcdDataBuf)
{
    unsigned char index;
    for(index=0;index<21;index++)
    {
        LcdWriteReg(index, LcdDataBuf[index]);//WriteLcdReg(index, LcdDataBuf[index]);
    }
}

/**
 * 清除屏幕显示
 * 
 * @author Zzzzz (2014-11-09)
 */
void OV_LcdClear(void)
{
     unsigned char i;
    for(i = 0; i < 21; i++)
    {
        LcdDataBuf[i]=0;
        LcdWriteBuf(i, 0);
    }//MCU_LcdAllClr();
}

/**
 * 全显
 * 
 * @author Zzzzz (2014-11-09)
 */
void OV_LcdDispAll(void)
{
     unsigned char i;
    for(i = 0; i < 21; i++)
    {
        LcdDataBuf[i]=0xff;
        LcdWriteBuf(i, 0xff);
    }//MCU_LcdAllClr();
    //MCU_LcdAllSet();
}

/**
 * 清除屏幕的数字
 * 
 * @author Zzzzz (2014-11-09)
 *
 * @param line 
 */
void OV_LcdClearNum()
{
    unsigned char i;
    
        for(i=0;i<8;i++)
        {
            LcdDispChar(i, LCD_CHAR_BLANK);
        }
            LcdResetBit(12,8);
            LcdResetBit(14,8);
            LcdResetBit(16,8);
            LcdResetBit(18,8);
}

/**
 * 显示第一行的数字
 * 
 * @author Zzzzz (2014-11-09)
 * 
 * @param num
 *          数值 
 * @param forceN 
 *          小数点位数
 */
/*void LcdDispNumLine1(long num,char forceN)
{
    unsigned char k,DispCH;
    num%=100000;
    k=0;
    DispCH=num/10000;
    num-=(long)DispCH*10000;
    if(k==1 || DispCH!=0 || forceN>=5) 
    {
        k=1;
        LcdDispChar(7,DispCH);
    }
    else
    {
        LcdDispChar(7,LCD_CHAR_BLANK);
    }
    DispCH=num/1000;
    num-=(long)DispCH*1000;
    if(k==1 || DispCH!=0 || forceN>=4) 
    {
        k=1;
        LcdDispChar(8,DispCH);
    }
    else
    {
        LcdDispChar(8,LCD_CHAR_BLANK);
    }
    DispCH=num/100;
    num-=(long)DispCH*100;
    if(k==1 || DispCH!=0 || forceN>=3) 
    {
        k=1;
        LcdDispChar(9,DispCH);
    }
    else
    {
        LcdDispChar(9,LCD_CHAR_BLANK);
    }
    DispCH=num/10;
    num-=(long)DispCH*10;
    if(k==1 || DispCH!=0 || forceN>=2)
    {
        k=1;
        LcdDispChar(10,DispCH);
    }
    else
    {
        LcdDispChar(10,LCD_CHAR_BLANK);
    }
    DispCH=num;
    LcdDispChar(11,DispCH);
}*/

/**
 * 显示第二行的数字
 *
 * @author Zzzzz (2014-11-09)
 * 
 * @param num
 * @param forceN 
 */
/*void LcdDispNumLine2(long num,char forceN)
{
    unsigned char k,DispCH;
    num%=10000000;
    k=0;
    DispCH=num/1000000;
    num-=(long)DispCH*1000000;
    if(k==1 || DispCH!=0 || forceN>=7) 
    {
        k=1;
        LcdDispChar(0,DispCH);
    }
    else
    {
        LcdDispChar(0,LCD_CHAR_BLANK);
    }
    
    DispCH=num/100000;
    num-=(long)DispCH*100000;
    if(k==1 || DispCH!=0 || forceN>=6) 
    {
        k=1;
        LcdDispChar(1,DispCH);
    }
    else
    {
        LcdDispChar(1,LCD_CHAR_BLANK);
    }
    
    DispCH=num/10000;
    num-=(long)DispCH*10000;
    if(k==1 || DispCH!=0 || forceN>=5) 
    {
        k=1;
        LcdDispChar(2,DispCH);
    }
    else
    {
        LcdDispChar(2,LCD_CHAR_BLANK);
    }
    
    DispCH=num/1000;
    num-=(long)DispCH*1000;
    if(k==1 || DispCH!=0 || forceN>=4) 
    {
        k=1;
        LcdDispChar(3,DispCH);
    }
    else
    {
        LcdDispChar(3,LCD_CHAR_BLANK);
    }
    
    DispCH=num/100;
    num-=(long)DispCH*100;
    if(k==1 || DispCH!=0 || forceN>=3) 
    {
        k=1;
        LcdDispChar(4,DispCH);
    }
    else
    {
        LcdDispChar(4,LCD_CHAR_BLANK);
    }
    
    DispCH=num/10;
    num-=(long)DispCH*10;
    if(k==1 || DispCH!=0 || forceN>=2) 
    {
        k=1;
        LcdDispChar(5,DispCH);
    }
    else
    {
        LcdDispChar(5,LCD_CHAR_BLANK);
    }
    
    DispCH=num;
    LcdDispChar(6,DispCH);
}*/
void LcdDispNumLine2(long num,char forceN)
{
    unsigned char k,DispCH,Mark;
    Mark=0;
    if(num<0)
    {
        num=-num;
        Mark=1;
    }
    num%=10000000;
    k=0;
    DispCH=num/1000000;
    num-=(long)DispCH*1000000;
    if(k>0 || DispCH!=0 || forceN>=7) 
    {
        k=1;
        LcdDispChar(0,DispCH);
    }
    else
    {
        LcdDispChar(0,LCD_CHAR_BLANK);
    }
    
    DispCH=num/100000;
    num-=(long)DispCH*100000;
    if(k>0 || DispCH!=0 || forceN>=6) 
    {
        if(k==0)
            k=2;
        LcdDispChar(1,DispCH);
    }
    else
    {
        LcdDispChar(1,LCD_CHAR_BLANK);
    }
    
    DispCH=num/10000;
    num-=(long)DispCH*10000;
    if(k>0 || DispCH!=0 || forceN>=5) 
    {
        if(k==0)
            k=3;
        LcdDispChar(2,DispCH);
    }
    else
    {
        LcdDispChar(2,LCD_CHAR_BLANK);
    }
    
    DispCH=num/1000;
    num-=(long)DispCH*1000;
    if(k>0 || DispCH!=0 || forceN>=4) 
    {
        if(k==0)
            k=4;
        LcdDispChar(3,DispCH);
    }
    else
    {
        LcdDispChar(3,LCD_CHAR_BLANK);
    }
    
    DispCH=num/100;
    num-=(long)DispCH*100;
    if(k>0 || DispCH!=0 || forceN>=3) 
    {
        if(k==0)
            k=5;
        LcdDispChar(4,DispCH);
    }
    else
    {
        LcdDispChar(4,LCD_CHAR_BLANK);
    }
    
    DispCH=num/10;
    num-=(long)DispCH*10;
    if(k>0 || DispCH!=0 || forceN>=2) 
    {
        if(k==0)
            k=6;
        LcdDispChar(5,DispCH);
    }
    else
    {
        LcdDispChar(5,LCD_CHAR_BLANK);
    }
    
    DispCH=num;
    LcdDispChar(6,DispCH);
    
    if(Mark)
    {
        if(k>=2)
            LcdDispChar(k-2,LCD_CHAR_SEPARATE);
        else
            LcdDispChar(0,LCD_CHAR_SEPARATE);
    }
}

/**
 * 显示带小数点的数值
 *  
 * @author Zzzzz (2014-11-09)
 * @param num 
 *          数值
 * @param dots 
 *          小数点位数 
 */
void OV_LcdDispNumWithDot(long num,char dots)
{
    unsigned char numlen,t=1;
    numlen = dots>>4;
    dots  &= 0x0f;
    while((dots>0) && t)
    {
        if(num%10==0)
        {
            dots-=1;
            num/=10;
        }
        else
        {
            t=0;
        }
    }
    while(num>9999999)
    {
        if(dots>0)
        {
            num/=10;
            dots-=1;
        }
        else
        {
            num=num%10000000;
        }
    }
    LcdSetDots(dots);
    /*if(line==ExtLine)
    {
        LcdDispNumLine1(num,dots+1);
    }
    else 
    */ 
    {
        if(numlen>0)
        {
            LcdDispNumLine2(num,numlen);
        }
        else
        {
            LcdDispNumLine2(num,dots+1);
        }
    }
}
/**
 *显示剩余气量
 *  
 * num-数值  dots-小数点位数  b-月份 
*/
void OV_DispRemainGas(long num,char dots,unsigned int b)
{   
    OV_LcdClearValueMark();
    OV_LcdDispMark((MARK_REMAIN),1);
    OV_LcdDispMark((MARK_GAS),1);
    OV_LcdDispMark((MARK_M3),1);
    OV_LcdDispNumWithDot(num,dots);
    OV_DispMonth(b);
   
}
/**
 * 显示剩余金额
 * 
 * @author Administrator (2014-11-21) 
 * num-数值  dots-小数点位数  b-月份 
 */
void OV_DispRemainMoney(long num,char dots,unsigned int b)
{
    OV_LcdClearValueMark();
    OV_LcdDispMark((MARK_REMAIN),1);
    OV_LcdDispMark((MARK_MONEY),1);
    OV_LcdDispMark((MARK_YAN),1);
    OV_LcdDispNumWithDot(num,dots);
     OV_DispMonth(b);

}
/**
 * 显示消费气量
 * 
 * @author Administrator (2014-11-21) 
 * num-数值  dots-小数点位数  b-月份 
 */
void OV_DispConsumedGas(long num,char dots,unsigned int b)
{
    OV_LcdClearValueMark();
    OV_LcdDispMark((MARK_CONSUMED),1);
    OV_LcdDispMark((MARK_GAS),1);
    OV_LcdDispMark((MARK_M3),1);
    OV_LcdDispNumWithDot(num,dots);
     OV_DispMonth(b);

}
/**
 * 显示消费金额
 * 
 * @author Administrator (2014-11-21) 
 * num-数值  dots-小数点位数  b-月份 
 */
void OV_DispConsumedMoney(long num,char dots,unsigned int b)
{
    OV_LcdClearValueMark();
    OV_LcdDispMark((MARK_CONSUMED),1);
    OV_LcdDispMark((MARK_MONEY),1);
    OV_LcdDispMark((MARK_YAN),1);
    OV_LcdDispNumWithDot(num,dots);
     OV_DispMonth(b);
}
/**
 * 显示充值气量
 * 
 * @author Administrator (2014-11-21) 
 * num-数值  dots-小数点位数  b-月份 
 */
void OV_DispChargeGas(long num,char dots,unsigned int b)
{
    OV_LcdClearValueMark();
    OV_LcdDispMark((MARK_CHARGE),1);
    OV_LcdDispMark((MARK_GAS),1);
    OV_LcdDispMark((MARK_M3),1);
    OV_LcdDispNumWithDot(num,dots);
     OV_DispMonth(b);
}
/**
 * 显示充值金额
 * 
 * @author Administrator (2014-11-21) 
 * num-数值  dots-小数点位数  b-月份 
 */
void OV_DispChargeMoney(long num,char dots,unsigned int b)
{
    OV_LcdClearValueMark();
    OV_LcdDispMark((MARK_CHARGE),1);
    OV_LcdDispMark((MARK_MONEY),1);
    OV_LcdDispMark((MARK_YAN),1);
    OV_LcdDispNumWithDot(num,dots);
     OV_DispMonth(b);
}
/**
 * 显示累积气量
 * 
 * @author Administrator (2014-11-21) 
 * num-数值  dots-小数点位数  b-月份 
 */
void OV_DispAmountGas(long num,char dots,unsigned int b)
{
    OV_LcdClearValueMark();
    OV_LcdDispMark((MARK_AMOUNT),1);
    OV_LcdDispMark((MARK_GAS),1);
    OV_LcdDispMark((MARK_M3),1);
    OV_LcdDispNumWithDot(num,dots);
     OV_DispMonth(b);
}
/**
 * 显示累积金额
 * 
 * @author Administrator (2014-11-21) 
 * num-数值  dots-小数点位数  b-月份 
 */
void OV_DispAmountMoney(long num,char dots,unsigned int b)
{
    OV_LcdClearValueMark();
    OV_LcdDispMark((MARK_AMOUNT),1);
    OV_LcdDispMark((MARK_MONEY),1);
    OV_LcdDispMark((MARK_YAN),1);
    OV_LcdDispNumWithDot(num,dots);
     OV_DispMonth(b);
}
/**
 * 显示冻结气量
 * 
 * @author Administrator (2014-11-21) 
 * num-数值  dots-小数点位数  b-月份 
 */
void OV_DispFreezeGas(long num,char dots,unsigned int b)
{
    OV_LcdClearValueMark();
    OV_LcdDispMark((MARK_FREEZE),1);
    OV_LcdDispMark((MARK_GAS),1);
    OV_LcdDispMark((MARK_M3),1);
    OV_LcdDispNumWithDot(num,dots);
     OV_DispMonth(b);
}
/**
 * 显示冻结金额
 * 
 * @author Administrator (2014-11-21) 
 * num-数值  dots-小数点位数  b-月份 
 */
void OV_DispFreezeMoney(long num,char dots,unsigned int b)
{
    OV_LcdClearValueMark();
    OV_LcdDispMark((MARK_FREEZE),1);
    OV_LcdDispMark((MARK_MONEY),1);
    OV_LcdDispMark((MARK_YAN),1);
    OV_LcdDispNumWithDot(num,dots);
     OV_DispMonth(b);
}
/**
 * 显示单价
 * 
 * @author Administrator (2014-11-21) 
 * num-数值  dots-小数点位数   
 */
void OV_DispPrice(long num,char dots)
{
    OV_LcdClearValueMark();
    OV_LcdDispMark((MARK_PRICE),1);
    OV_LcdDispMark((MARK_BACKSLASH),1);
    OV_LcdDispMark((MARK_YAN),1);
    OV_LcdDispMark((MARK_M3),1);
    OV_LcdDispNumWithDot(num,dots);
}
/**
 * 显示报警
 * 
 * @author Administrator (2014-11-21)
 */
void OV_DispErr(void)
{
    OV_LcdDispMark((MARK_ERR),1);
}
/**
 * 显时阀门状态
 * 
 * @author Administrator (2014-11-21) 
 * 0-显示阀门开，1-显示阀门关 
 */
void OV_DispValve(char c)
{   
    if(c)
    {
        OV_LcdDispMark((MARK_CLOSE),1);
        OV_LcdDispMark((MARK_OPEN),0);
    }
    else
    {
        OV_LcdDispMark((MARK_CLOSE),0);
        OV_LcdDispMark((MARK_OPEN),1);
    }
}
/**
 * 显示电量
 * 
 * @author Administrator (2014-11-21)
 */
void OV_DispBatt(unsigned int i)
{   
    switch(i)
    {   
    case 1://OV_LcdDispMark((MARK_BATT_FREAM),1);
           OV_LcdDispMark((MARK_BATT1),1);
           OV_LcdDispMark((MARK_BATT2),0);
           OV_LcdDispMark((MARK_BATT3),0);
           break;
    case 2://OV_LcdDispMark((MARK_BATT_FREAM),1);
           OV_LcdDispMark((MARK_BATT1),1);
           OV_LcdDispMark((MARK_BATT2),1);
           OV_LcdDispMark((MARK_BATT3),0);
           break;
    case 3://OV_LcdDispMark((MARK_BATT_FREAM),1);
           OV_LcdDispMark((MARK_BATT1),1);
           OV_LcdDispMark((MARK_BATT2),1);
           OV_LcdDispMark((MARK_BATT3),1);
           break;
    default:
        //OV_LcdDispMark((MARK_BATT_FREAM),1);
        OV_LcdDispMark((MARK_BATT1),0);
        OV_LcdDispMark((MARK_BATT2),0);
        OV_LcdDispMark((MARK_BATT3),0);
        break;
    }
}
/**
 * 显示信号
 * 
 * @author Administrator (2014-11-21)
 */
void OV_DispSign(unsigned int i)
{
    switch(i)
    {   
    
    case 1:OV_LcdDispMark((MARK_SIGN1),1);
           break;
    case 2:
           OV_LcdDispMark((MARK_SIGN1),1);
           OV_LcdDispMark((MARK_SIGN2),1);
           break;
    case 3:
           OV_LcdDispMark((MARK_SIGN1),1);
           OV_LcdDispMark((MARK_SIGN2),1);
           OV_LcdDispMark((MARK_SIGN3),1);
           break;
    default:
           OV_LcdDispMark((MARK_SIGN1),0);
           OV_LcdDispMark((MARK_SIGN2),0);
           OV_LcdDispMark((MARK_SIGN3),0);
           break;
    }
}

/**
 * 显示月份
 * 
 * @author Administrator (2014-11-24)
 */
void OV_DispMonth(unsigned int b)
{
    if(b==0||b>12)
    {   
        LcdWriteBuf(4,0x00);
        LcdWriteBuf(3,0x00);
    }
    else if(b<=9)
    {
        OV_LcdDispMark((MARK_MONTH),1);
        LcdDispChar(9,b);
    }
    else 
    {
        OV_LcdDispMark((MARK_MONTH),1);
        LcdDispChar(9,b-10);
        LcdSetBit(4,8);
    }
}

/**
 * 显示时间 00:00:00
 * 
 * @author Administrator (2014-11-24) 
 * a-时 b-分 c-秒  时间 格式：XX时XX分XX秒
 */
void OV_DispTime(unsigned int a,unsigned int b,unsigned int c)
{
    OV_LcdClearValueMark();
    LcdSetBit(10,8);
    LcdSetBit(12,8);
    LcdSetBit(16,8);

    LcdDispChar(0,LCD_CHAR_BLANK);
    LcdDispChar(1,a/10);
    LcdDispChar(2,a%10);

    LcdDispChar(3,b/10);
    LcdDispChar(4,b%10);

    LcdDispChar(5,c/10);
    LcdDispChar(6,c%10);
}

/**
 * 显示日期  XXXXXX(如141124)
 * 
 * @author Administrator (2014-11-24) 
 * n-年 m-月 k-日  格式：XX年XX月XX日 
 */
void OV_DispDate(unsigned int n,unsigned int m,unsigned int k)
{
    OV_LcdClearValueMark();
        
    LcdDispChar(0,LCD_CHAR_BLANK);
    LcdDispChar(1,n/10);
    LcdDispChar(2,n%10);

    LcdDispChar(3,m/10);
    LcdDispChar(4,m%10);

    LcdDispChar(5,k/10);
    LcdDispChar(6,k%10);

}

/**
 * 显示 HELLO
 * 
 * @author Administrator (2014-11-24)
 */
void OV_DispHello(void)
{
    OV_LcdClearValueMark();

    OV_LcdDispChar(0,LCD_CHAR_BLANK);        
    OV_LcdDispChar(1,LCD_CHAR_H);
    OV_LcdDispChar(2,LCD_CHAR_E);
    OV_LcdDispChar(3,LCD_CHAR_L);
    OV_LcdDispChar(4,LCD_CHAR_L);
    OV_LcdDispChar(5,LCD_CHAR_O);
    OV_LcdDispChar(6,LCD_CHAR_BLANK);        
}

/**
 * 显示End
 * 
 * @author Administrator (2014-11-24)
 */
void OV_DispEnd(void)
{
    OV_LcdClearValueMark();
    OV_LcdDispChar(0,LCD_CHAR_BLANK);    
    OV_LcdDispChar(1,LCD_CHAR_BLANK);    
    OV_LcdDispChar(2,LCD_CHAR_E);
    OV_LcdDispChar(3,LCD_CHAR_n);
    OV_LcdDispChar(4,LCD_CHAR_d);
    OV_LcdDispChar(5,LCD_CHAR_BLANK);    
    OV_LcdDispChar(6,LCD_CHAR_BLANK);    
}

/**
 * 显示End
 * 
 * @author Administrator (2014-11-24)
 */
void OV_DispSucc(void)
{
    OV_LcdClearValueMark();
    
    OV_LcdDispChar(0,LCD_CHAR_BLANK);    
    OV_LcdDispChar(1,LCD_CHAR_S);
    OV_LcdDispChar(2,LCD_CHAR_U);
    OV_LcdDispChar(3,LCD_CHAR_C);
    OV_LcdDispChar(4,LCD_CHAR_C);
    OV_LcdDispChar(5,LCD_CHAR_BLANK);    
    OV_LcdDispChar(6,LCD_CHAR_BLANK);    
}
/**
 * 显示End
 * 
 * @author Administrator (2014-11-24)
 */
void OV_DispFail(void)
{
    OV_LcdClearValueMark();
    
    OV_LcdDispChar(0,LCD_CHAR_BLANK);    
    OV_LcdDispChar(1,LCD_CHAR_F);
    OV_LcdDispChar(2,LCD_CHAR_A);
    OV_LcdDispChar(3,LCD_CHAR_I);
    OV_LcdDispChar(4,LCD_CHAR_L);
    OV_LcdDispChar(5,LCD_CHAR_BLANK);    
    OV_LcdDispChar(6,LCD_CHAR_BLANK);    
}

/**
 * 显示阶梯金额
 * 
 * @author Administrator (2014-11-24) 
 *  num-数值  dots-小数点位数  b-月份
 */
void OV_DispStepMoney(long num,char dots,unsigned int b)
{
    OV_LcdClearValueMark();
    OV_LcdDispMark((MARK_MONEY),1);
    OV_LcdDispMark((MARK_YAN),1);
    OV_LcdDispMark((MARK_STEP),1);
    OV_LcdDispNumWithDot(num,dots);
    OV_DispMonth(b);
}
/**
 * 显示阶梯气量
 * 
 * @author Administrator (2014-11-24) 
 * num-数值  dots-小数点位数  b-月份 
 */
void OV_DispStepGas(long num,char dots,unsigned int b)
{
    OV_LcdClearValueMark();
    OV_LcdDispMark((MARK_GAS),1);
    OV_LcdDispMark((MARK_M3),1);
    OV_LcdDispMark((MARK_STEP),1);
    OV_LcdDispNumWithDot(num,dots);
    OV_DispMonth(b);
}
/**
 * 显示阶梯剩余气量
 * 
 * @author Administrator (2014-11-24) 
 * num-数值  dots-小数点位数  b-月份 
 */
void OV_DispStepRemainGas(long num,char dots,unsigned int b)
{
    OV_LcdClearValueMark();
    OV_LcdDispMark((MARK_REMAIN),1);
    OV_LcdDispMark((MARK_GAS),1);
    OV_LcdDispMark((MARK_M3),1);
    OV_LcdDispMark((MARK_STEP),1);
    OV_LcdDispNumWithDot(num,dots);
    OV_DispMonth(b);
}
/**
 * 显示阶梯剩余金额
 * 
 * @author Administrator (2014-11-24) 
 *  num-数值  dots-小数点位数  b-月份
 */
void OV_DispStepRemainMoney(long num,char dots,unsigned int b)
{
    OV_LcdClearValueMark();
    OV_LcdDispMark((MARK_REMAIN),1);
    OV_LcdDispMark((MARK_MONEY),1);
    OV_LcdDispMark((MARK_YAN),1);
    OV_LcdDispMark((MARK_STEP),1);
    OV_LcdDispNumWithDot(num,dots);
    OV_DispMonth(b);
}


/**
 * 显示阶梯金额
 * 
 * @author Administrator (2014-11-24) 
 *  num-数值  dots-小数点位数  b-阶梯级数
 */
void OV_DispStepNMoney(long num,char dots,unsigned char b)
{
    OV_LcdClearValueMark();
    OV_LcdDispMark((MARK_MONEY),1);
    OV_LcdDispMark((MARK_YAN),1);
    OV_LcdDispMark((MARK_STEP),1);
    OV_LcdDispNumWithDot(num,dots);
    OV_DispMonth(b);
    OV_LcdDispMark((MARK_MONTH),0);
}

/**
 * 显示阶梯气量
 * 
 * @author Administrator (2014-11-24) 
 * num-数值  dots-小数点位数  b-阶梯级数 
 */
void OV_DispStepNVol(long num,char dots,unsigned char b)
{
    OV_LcdClearValueMark();
    OV_LcdDispMark((MARK_GAS),1);
    OV_LcdDispMark((MARK_M3),1);
    OV_LcdDispMark((MARK_STEP),1);
    OV_LcdDispNumWithDot(num,dots);
    OV_DispMonth(b);
    OV_LcdDispMark((MARK_MONTH),0);
}
/**
 * 显示某阶梯剩余气量
 * 
 * @author Administrator (2014-11-24) 
 * num-数值  dots-小数点位数  b-阶梯级数 
 */
void OV_DispStepNRemainVol(long num,char dots,unsigned char b)
{
    OV_LcdClearValueMark();
    OV_LcdDispMark((MARK_REMAIN),1);
    OV_LcdDispMark((MARK_GAS),1);
    OV_LcdDispMark((MARK_M3),1);
    OV_LcdDispMark((MARK_STEP),1);
    OV_LcdDispNumWithDot(num,dots);
    OV_DispMonth(b);
    OV_LcdDispMark((MARK_MONTH),0);
    
    
}
/**
 * 显示某阶梯剩余金额
 * 
 * @author Administrator (2014-11-24) 
 *  num-数值  dots-小数点位数  b-阶梯级数
 */
void OV_DispStepNRemainMoney(long num,char dots,unsigned char b)
{
    OV_LcdClearValueMark();
    OV_LcdDispMark((MARK_REMAIN),1);
    OV_LcdDispMark((MARK_MONEY),1);
    OV_LcdDispMark((MARK_YAN),1);
    OV_LcdDispMark((MARK_STEP),1);
    OV_LcdDispNumWithDot(num,dots);
    OV_DispMonth(b);
    OV_LcdDispMark((MARK_MONTH),0);
}
/**
 * 显示阶梯单价
 * 
 * @author Administrator (2014-11-21) 
 * num-数值  dots-小数点位数  b-阶梯级数 
 */
void OV_DispStepNPrice(long num,char dots,char b)
{
    OV_LcdClearValueMark();
    OV_LcdDispMark((MARK_PRICE),1);
    OV_LcdDispMark((MARK_BACKSLASH),1);
    OV_LcdDispMark((MARK_YAN),1);
    OV_LcdDispMark((MARK_M3),1);
    OV_LcdDispMark((MARK_STEP),1);
    OV_LcdDispNumWithDot(num,dots);
    OV_DispMonth(b);
    OV_LcdDispMark((MARK_MONTH),0);
}

/**
 * LCD开
 * 
 * @author Administrator (2014-11-24)
 */
void OV_LcdOn(void)
{
    LcdOn();
}
/**
 * LCD关
 * 
 * @author Administrator (2014-11-24)
 */
void OV_LcdOff(void)
{
    LcdOff();
}
/**
 * LCD初始化
 * 
 * @author Administrator (2014-11-24)
 */
void OV_LcdInit(void)
{
    LcdInit();
    //OV_DispHello();
}


/**
 * 显示带有一个字母的参数
 * 
 * @author Zzzzz (2014-12-11)
 * 
 * @param ch1 
 *          参数字母1 
 * @param num 
 *          参数值 
 *           
 */
void OV_LcdDispPara1(unsigned char ch1,unsigned long num)
{
    OV_LcdClearValueMark();
    OV_LcdDispNumWithDot(num,0);        
    OV_LcdDispChar(0,ch1);    
    OV_LcdDispChar(1,LCD_CHAR_SEPARATE);
}

/**
 * 显示带有两个字母的参数
 * 
 * @author Zzzzz (2014-12-11)
 * 
 * @param ch1 
 *          参数字母1 
 * @param ch2 
 *          参数字母2
 * @param num 
 *          参数值 
 *           
 */
void OV_LcdDispPara2(unsigned char ch1,unsigned char ch2,unsigned int num)
{
    OV_LcdClearValueMark();
    OV_LcdDispNumWithDot(num,0);        
    
    OV_LcdDispChar(0,ch1);    
    OV_LcdDispChar(1,ch2);
    OV_LcdDispChar(2,LCD_CHAR_SEPARATE);
}

/**
 * 分段显示IP地址
 * 
 * @author Zzzzz (2014-12-11)
 * 
 * @param c 
 *          c=0 显示第一行 如 192.168.
 *          c=1 显示第二行 如 000.001
 * @param ip0 
 *          ip值 
 * @param ip1 
 *          ip值 
 */
void OV_LcdDispIP(unsigned char c,unsigned char ip0,unsigned char ip1)
{
    OV_LcdClearValueMark();  
    OV_LcdDispChar(0,ip0/100);
    ip0%=100;
    OV_LcdDispChar(1,ip0/10);
    ip0%=10;
    OV_LcdDispChar(2,ip0);
    LcdSetDots(4);
    
    OV_LcdDispChar(3,ip1/100);
    ip1%=100;
    OV_LcdDispChar(4,ip1/10);
    ip1%=10;
    OV_LcdDispChar(5,ip1);
    OV_LcdDispChar(6,LCD_CHAR_BLANK);    
    
    
    if(c==0)
    {
        LcdSetBit(18,8);
    }
}

void LCD(void)
{
   OV_LcdInit();
   LcdOn();
   OV_LcdDispAll();
   OV_DispHello();
   OV_LcdDispPara2(LCD_CHAR_A,LCD_CHAR_A,123);
   OV_LcdDispPara1(LCD_CHAR_P,65535);
   OV_LcdDispIP(0,192,168);
   OV_LcdDispIP(1,32,11);
   
    OV_DispEnd();
    OV_DispSucc();
    OV_DispFail();
    OV_DispBatt(0);
    OV_DispBatt(1);
    OV_DispBatt(2);
    OV_DispBatt(3);
    OV_DispBatt(2);
    OV_DispBatt(1);
    OV_DispBatt(0);
   //MCU_LcdAllSet();
   OV_LcdDispNumWithDot(345,3);
   
   OV_LcdDispNumWithDot(3450,3);
   
   OV_LcdDispNumWithDot(123456789l,3);
   OV_DispRemainGas(112,3,12);
   OV_DispTime(16,45,00);
   OV_DispStepRemainMoney(-5635,4,13);
   OV_DispFreezeGas(2345678,4,11);
   
//   OV_LcdClearNum();
//   OV_DispDate(14,11,25);
//   OV_DispPrice(75,3);
//   MCU_LcdClose();
}
