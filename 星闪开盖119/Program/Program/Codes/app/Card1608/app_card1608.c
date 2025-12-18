#include "app_card1608.h"
#include "drv_lg1608.h"
#include "app_cntpay.h"
#include "app_general.h"
#include "app_datasvr.h"
#include "app_cardhummach.h"
#include "App_Powermanage.h"
#include "app_valve_control.h"
#include "lcd.h"
#include "general.h"
#include "crc16.h"
#include "bsp_rs004.h"
#include "lib_rs004.h"
#include <string.h>

//#if UseSetup==1
//使用设置卡时的密钥
const unsigned char keyseed[4] = {0x6b, 0x34, 0x2d, 0x7e};
const unsigned char skeyA[3] = {0x4f, 0x59, 0xe7};
const unsigned char skeyB[3] = {0x8b, 0x2d, 0x34};
//#else
//不使用设置卡时的密钥
const unsigned char E_keyseed[4] = {0x23, 0x56, 0x98, 0x07};
const unsigned char E_skeyA[4] = {0xE3, 0x26, 0xD9, 0xC8};
const unsigned char E_skeyB[4] = {0xD1, 0xA4, 0xC9, 0xB2};
//#endif

const unsigned char StrAR[8] = {0x23, 0x27, 0x2b, 0x2f, 0x33, 0x37, 0x3b, 0x3f};
const unsigned char CardCheckID[4] = {0x07, 0xff, 0xff, 0xff};

unsigned char cardtype = 0;
//各个区域的卡密码
unsigned char pass2read[4];
unsigned char pass2write[4];
unsigned char pass7read[4];
unsigned char pass7write[4];
unsigned char otherpass[4] = {0xff, 0xff, 0xff, 0xff};

StuUser UserPara;      //卡参数，需要存储
Stu_Card Card;
Stu_EncodeInfo EncodeInfo;

unsigned char CheckCard = 0;

//临时变量
unsigned char bufchar = 0;
unsigned int  bufint = 0;
unsigned long buflong = 0;

/**
 *蜂鸣器响
 */
void BeepMs(unsigned int ms)
{
//    if(GetSysPowState()!=STATUS_POW_CUT)
    {
        BspBeepOn();
        DelayMs(ms);
        BspBeepOff();
    }
}
void CardPowOn(void)
{
    CardPowerContrlClr();
}
void CardPowOff(void)
{
    CardPowerContrlSet();
    CardRstOutput();
    CardRstClr();
    CardSclOutput();
    CardSclClr();
    CardSdaOutput();
    CardSdaClr();
}

CardStateVal GetCardState(void)
{
    CardStateVal re;
   
    re = UserPara.CardState;

    return re;
}

CardStateVal SetCardState(CardStateVal val)
{
    CardStateVal re = val;

    UserPara.CardState = val;

    return re;
}

//获取配置参数
unsigned int GetConfigValue(ConfigValue ValueType)
{
    unsigned int re=0;
    switch(ValueType)
    {
    case CONFIG_OverDraught:     //透支量
        re = UserPara.Config.OverDraught;
        break;
    case CONFIG_LcdSleepTime:   //液晶休眠时间
        re = UserPara.Config.LcdSleepTime;
        break;
    case CONFIG_UseSetupCard:    //使用设置卡
        re = UserPara.Config.UseSetupCard;
        break;
    case CONFIG_HallCheck:        //双霍尔计数
        re = UserPara.Config.HallCheck;
        break;
    case CONFIG_NoHallSigAttact:  //不使用单边磁攻击
        re = UserPara.Config.NoHallSigAttact;
        break;
    case CONFIG_CleanTest:      //使用检测卡后要清除
        re = UserPara.Config.CleanTest;
        break;
    case CONFIG_NoUserUse:       //无用户状态下允许透支
        re = UserPara.Config.NoUserUse;
        break;
    case CONFIG_UseSigPrice:     //使用单一价格
        re = UserPara.Config.UseSigPrice;
        break;
    case CONFIG_NoBandUse:      //设置卡解绑后不关阀
        re = UserPara.Config.NoBandUse;
        break;
    case CONFIG_VerifyCompanyCode:  //验证公司代码
        re = UserPara.Config.VerifyCompanyCode;
        break;
    default:
        re = 0;
        break;
    }
    return re;
}

//获取剩余量
long GetRemainValue(void)
{
    long re = 0;
    unsigned long R_Negtiveuse; 
    unsigned long R_RemainInt; 
    unsigned long R_RemainDec; 
    
    R_Negtiveuse = GetValue(BASEVAL_Negtiveuse);
    R_RemainInt = GetValue(BASEVAL_RemainInt);
    R_RemainDec = GetValue(BASEVAL_RemainDec);
    if(R_Negtiveuse > 0)
    {
        re = (R_Negtiveuse - 1) * 100 + (100 - R_RemainDec / 10000000);
        re = -re;
    }
    else
    {
        re = R_RemainInt * 100 + (R_RemainDec / 10000000);
    }
    
    return re;
}

unsigned char SetRemainValue(long val)
{
    unsigned char re = TRUE;
    unsigned long R_Negtiveuse; 
    unsigned long R_RemainInt; 
    unsigned long R_RemainDec; 
    
    if(val >= 0)
    {
        R_Negtiveuse = 0;
        R_RemainInt = val / 100;
        R_RemainDec = (val % 100) * 10000000;
    }
    else
    {
        val *= -1;
        R_Negtiveuse = val / 100 + 1;
        R_RemainInt = 0;
        R_RemainDec = (100 - val % 100) * 10000000;
    }
    SetValue(BASEVAL_Negtiveuse,R_Negtiveuse);
    SetValue(BASEVAL_RemainInt,R_RemainInt);
    SetValue(BASEVAL_RemainDec,R_RemainDec);
    
    return re;
}

/**
 * 清除表具所有信息
 */
void ClearMeter(void)
{
    
}

/*********************************************************************************************************
** Function name:       get_card_type
** Descriptions:        获取卡片类型
** input parameters:    无
** output parameters:   无
** Returned value:      卡片类型或卡片错误信息
*********************************************************************************************************/
unsigned char get_card_type(void)
{
    unsigned char cardtypebuf, buf[8], buf2[4];

    read_ar(buf);										//验证AR
    if(buf[0] == 0xff && buf[1] == 0xff) //配置卡使用不加密的1608卡
    {
        return PEIZHIKA;
    }
    if(cmp_str(buf, (unsigned char *)StrAR, 8) == FALSE)
    {
        return C_ERR_AR;
    }
    if(UserPara.Config.UseSetupCard == 1) //使用设置卡时的7区读明码
    {
        //检测卡片类型 读cmc
        read_cmc(buf); //读取卡设置区cmc读到buf2
        //------------------------------------------------------------------------//
        if(cmp_str(buf, UserPara.CardCMC, 4) == TRUE )
        {
            if(GetCardState() == STATUS_METER_INIT) //未设置过则返回错误
                return C_ERR_NOSET;
            encrypt(pass7read, UserPara.KeySeed, UserPara.sKeyA, UserPara.sKeyB);
        }
        else    //设置卡 读卡类型
        {
            //copy_str(keyseed, buf, 4);
            encrypt(pass7read, buf, (unsigned char *)skeyA, (unsigned char *)skeyB);
        }

        if(verify_pass(USER_ZONE7, READ, pass7read) == FALSE) //核实用户区7读密码
        {
            if(GetCardState() == STATUS_METER_INIT)
                memcpy(UserPara.KeySeed, keyseed, 4);
            return C_ERR_PASS;
        }
        read_zone(USER_ZONE7, 0xee, &cardtypebuf, 1);

        //计算其它区域的密码
        read_zone(USER_ZONE7, 0x40, buf2, 4); // 2区读明码
        encrypt(pass2read, buf2, (unsigned char *)skeyA, (unsigned char *)skeyB);

        read_zone(USER_ZONE7, 0x44, buf2, 4); // 2区写明码
        encrypt(pass2write, buf2, (unsigned char *)skeyA, (unsigned char *)skeyB);

        read_zone(USER_ZONE7, 0xe4, buf2, 4); //7区写明码
        encrypt(pass7write, buf2, (unsigned char *)skeyA, (unsigned char *)skeyB);

    }
    else//不使用设置卡时的操作
    {
        E_encrypt(pass7read, E_keyseed, E_skeyA, E_skeyB);
        if(verify_pass(USER_ZONE7, READ, pass7read) == FALSE) //核实用户区7读密码
            return C_ERR_PASS;
        read_zone(USER_ZONE7, 0xee, &cardtypebuf, 1);

        if(GetCardState() == STATUS_METER_INIT)
        {
            read_zone(USER_ZONE7, 0xe9, buf, 5);
            memcpy(UserPara.CompanyCode, buf, 5);
            if(cardtypebuf == YONGHUKA)
                SetCardState(STATUS_METER_INUSE);
        }

        //计算其它区域的密码
        read_zone(USER_ZONE7, 0x40, buf2, 4); // 2区读明码
        E_encrypt(pass2read, (unsigned char *)buf2, E_skeyA, E_skeyB);

        read_zone(USER_ZONE7, 0x44, buf2, 4); // 2区写明码
        E_encrypt(pass2write, (unsigned char *)buf2, E_skeyA, E_skeyB);

        read_zone(USER_ZONE7, 0xe4, buf2, 4); //7区写明码
        E_encrypt(pass7write, (unsigned char *)buf2, E_skeyA, E_skeyB);
    }

    return cardtypebuf;
}

unsigned char  read_card(void)
{
    if(init_card() == FALSE) //反插卡
    {
        return INVERSIONCARD;
    }
    else
    {
        //LcdDispLoading();
        //LCD_MARK_READ_ON;
    }
    cardtype = get_card_type();
    
    if(CardBslRead())
        return C_ERR_NOCARD;

    if(cardtype < INVERSIONCARD)
    {
        return cardtype;
    }
    
    switch (cardtype)
    {
    case PEIZHIKA:
        cardtype = card_peizhika_process();
        break;
    case SHEZHIKA:
        cardtype = card_shezhika_process();
        break;
    case YONGHUKA: //用户卡
        cardtype = card_yonghuka_process();
        if(cardtype == JIANCEKA)
            cardtype = card_jianceka_process();        
        DataSvrSaveAll();       
        break;
    case ZHUANCUNKA:
        MCU_IntDis();
        cardtype = card_zhuancunka_process();
        if(cardtype == OLD_TRAN_CARD)
            cardtype = old_tran_process();
        MCU_IntEn();
        break;
    case QINGCHUKA: //清除卡
        card_qingchuka_process();
        break;
    case QIEHUANKA:
        cardtype=card_qiehuanka_process();
        break;
    default:
        break;
    }
   
    return cardtype;
}

unsigned char UserReadCard(void)
{
    unsigned char cardtype;

    cardtype = read_card();

    return cardtype;
}

void InversionCard(void)
{
//    if(Fault.Magnet)
//    {
//        if(CheckHall() == TRUE)
//            Fault.Magnet = 0;
//    }
//    if(Fault.Warning119)
//    {
//        if(Check119() == TRUE)
//            Fault.Warning119 = 0;
//    }
//    
//    if(Fault.UnusedTimeup)
//    {
//        Fault.UnusedTimeup=0;
//        Globe.IdleTime=0;//Meter.UnusedCloseTime;
//    }
//    
//    //重新检查阀门状态
//    Fault.DoorBroken=0;
//    Globe.ForceCloseTimes=0;

    if(GetSysPowState()==STATUS_POW_NORMAL)
    {
        if(Get_ValveStatus()==OPEN_STATUS)
        {
            OV_OpenValve_Opt(2);//强制开200ms
        }
    }

    //正常用户状态下启用透支
    switch(GetCardState())
    {
    case STATUS_METER_INIT:         //出厂状态，已经初始化，未设置，显示hello
        break;
    case STATUS_METER_SETUP:        //设置状态
    case STATUS_METER_CHECK:         //检表状态
        SetCardState(STATUS_METER_NOUSER_USE);
        break;
    case STATUS_METER_INUSE:        //正常用户状态
        if(GetRemainValue() <= 0)
            SetCardState(STATUS_METER_OVERUSE);
        break;
    case STATUS_METER_NOUSER_USE:    //无用户的状态的检表
    case STATUS_METER_OUTBAND:       //解除绑定状态，解绑状态下不允许透支。
    case STATUS_METER_OVERUSE:       //透支状态
    //case STATUS_METER_UNUSED:        //出厂状态，未初始化，需要写入一些参数，显示程序版本号SP X-XX
    default:
        break;
    }
    //标记开始轮显
    SetLcdState(STATE_LCD_DISP_CHANGE);   //需要切换显示
}

void TaskCheckCard(void)
{
    VALVE_STATUS_EU t_State;
    unsigned cardtype;
    
    if(CheckCard)
    {
        //SetFault(FAULT_WaitInv,0);
        
        OV_LcdOn();
        t_State=Get_ValveStatus();
        if((t_State==CLOSING_STATUS)||(t_State==OPENING_STATUS))
        {
            CheckCard = 0;
            return;
        }
        switch(GetSysPowState())
        {
            //case STATE_POW_HIGH:
        case STATUS_POW_NORMAL:
            CardPowOn();
            DelayMs(100);
            if(CardPowerCheckRead() == 0)
            {
                cardtype = C_ERR_METAL;
            }
            else
            {
                LcdDispLoading();
                //LCD_MARK_READ_ON;
                //SysEnterHighSpeed();
                //DelayMs(100);
                cardtype = UserReadCard();
            }
            CardPowOff();
            //LCD_MARK_READ_OFF;
            BeepMs(100);
            //显示错误代码
            if(cardtype == 0xff)
            {
                BeepMs(100);
                DelayMs(1000);
            }
            else
            {
                if(cardtype < INVERSIONCARD)
                {
                    if(cardtype != C_ERR_ADD_0)
                    {
                        LcdDispErr(cardtype);
                        DelayMs(1000);
                    }
                }
                else if(cardtype != INVERSIONCARD)
                {
                    BeepMs(100);
                    OV_DispEnd();
                    DelayMs(1000);
                }
            }
            break;
        default:
            cardtype=INVERSIONCARD;
            break;
        }
        //处理反插卡
        if(cardtype == INVERSIONCARD)
        {
            InversionCard();
        }
        CheckCard = 0;
        OV_LcdOn();
    }
}

/**
 * 读取卡内数据
 *
 */
unsigned char CardReadAllData(void)
{
    //读取卡内2区所有数据
    if(verify_pass(USER_ZONE2, READ, pass2read) == FALSE)
        return C_ERR_PASS;
    read_zone(USER_ZONE2, 0x00, Card.buf, 0xff);
    return 0;
}

/**
 * 检测卡号
 * 未插入用户卡时直接写入卡号并返回成功
 * 写入卡号后需验证卡号
 */
unsigned char CheckCardID(unsigned char *pCardId)
{
    unsigned re=TRUE;
    switch(GetCardState())
    {
    case STATUS_METER_INIT:         //出厂状态，已经初始化，未设置，显示hello
    case STATUS_METER_SETUP:        //设置状态
    case STATUS_METER_NOUSER_USE:   //无用户的状态的检表
    case STATUS_METER_CHECK:        //检表状态
    case STATUS_METER_OUTBAND:      //解除绑定状态，解绑状态下不允许透支。
        memcpy(UserPara.CardId,pCardId,10);
        SetCardState(STATUS_METER_INUSE);
        break;                  
    case STATUS_METER_OVERUSE:      //透支状态
    case STATUS_METER_INUSE:        //正常用户状态
        if(memcmp(UserPara.CardId,pCardId,10)==0)
            re = TRUE;
        else
            re = FALSE;
        break;    
    //case STATUS_METER_UNUSED:       //出厂状态，未初始化，需要写入一些参数，显示程序版本号SP X-XX
    default:
        re=FALSE;
        break;
    }
    return re;
}

/*
 * 验证表具ID
 * 1、判断是否为检测卡
 * 2、判断检测卡是否要清除
 * 3、判断卡号规则
 * 4、验证卡号
 */
unsigned char CardVerifyUserID(unsigned char *pCardID)
{
    unsigned char tbuf[10];
    if(memcmp(pCardID, (unsigned char *)CardCheckID, 4) == 0)
        return JIANCEKA;
    if((GetCardState() == STATUS_METER_CHECK) && (UserPara.Config.CleanTest == 1)) //判断检测卡是否需要清除
    {
        return C_ERR_NOCLEAN;
    }
    if(pCardID[0] & 0xf0)
        return C_ERR_CARDID_RULE;
    memset(tbuf,0xff,10);
    memcpy(tbuf,pCardID,4);
    if (CheckCardID(tbuf)==FALSE)
        return C_ERR_CARDID;
    return 0;
}

/**
 * 表具验证换卡次数
 */
unsigned char CardVerifyCardIndex(void)
{
    memcpy((unsigned char *)&bufint,Card.UserCard.blank5+2,2);
    if((CrcCheck(bufint,Card.UserCard.blank5,2)==FALSE) ||
       (UserPara.UserCardIndex[0]==0x00 && UserPara.UserCardIndex[1]==0x00) ||
       (UserPara.UserCardIndex[0]==0xFF && UserPara.UserCardIndex[1]==0xFF))   //新卡
    //if(CrcCheck(bufint,Card.UserCard.blank5,2)==FALSE)
    {
        memcpy((unsigned char *)&bufint,UserPara.UserCardIndex,2);
        bufint = bufint + 1;
        memcpy(UserPara.UserCardIndex,(unsigned char *)&bufint,2);
        bufint=Crc16Get(UserPara.UserCardIndex,2);
        memcpy(UserPara.UserCardIndex+2,(unsigned char *)&bufint,2);
        memcpy(Card.UserCard.blank5,UserPara.UserCardIndex,4);
        verify_pass(USER_ZONE2, WRITE, pass2write);
        write_zone(USER_ZONE2, (unsigned char)(Card.UserCard.blank5-Card.buf), 
               UserPara.UserCardIndex, 4);
        return 0;
    }
    else//老卡
    {
        if(memcmp(UserPara.UserCardIndex,Card.UserCard.blank5,4)==0)
            return 0;
        else
            return C_ERR_CARDTIMES;
    }
}

void UpdataCardPara(void)
{
    //读取卡内各个参数
    //表存上限
    buflong=BCDtoHEX_B(Card.UserCard.MaxRemain,3);
    if(buflong==0)
        SetValue(USERVAL_Maxremain, 99999999l);
    else    
        SetValue(USERVAL_Maxremain, buflong*10);
    
    //透支量
    buflong=BCDtoHEX_SS(Card.UserCard.Overuse,3);
    buflong*=10;
    SetValue(USERVAL_Overdraught, buflong);
   
    //报警量
    buflong=BCDtoHEX_S(&Card.UserCard.Warning,1);
    SetValue(USERVAL_Warning, buflong*100);
    
    //闲置天数
    buflong=BCDtoHEX_S(&Card.UserCard.IdleTime,1);
    SetValue(USERVAL_UnusedCloseTime, buflong*24);
}

//////////////////////////////////////////////
//返写数据
//////////////////////////////////////////////
unsigned char write_data_to_card(void)
{
    unsigned char *prt;
    unsigned long TempLong;
    prt = (unsigned char *)&TempLong;
    /////////////////////////////返写数据////////////////////////////////////
    if(verify_pass(USER_ZONE2, WRITE, pass2write) == FALSE) //核实用户区2写密码
        return C_ERR_PASS;
    //写入剩余气量
    if(GetRemainValue()>0)
    {
        buflong = ((GetRemainValue() / 1000) * 1000) / (GetValue(BASEVAL_Price)/10) * 1000;
        buflong+= ((GetRemainValue() % 1000) * 1000) / (GetValue(BASEVAL_Price)/10);
        buflong/=10;
        HEXtoBCD_B((unsigned char *)&TempLong, buflong, 4);
    }
    else
    {
        TempLong=0;
    }
    write_zone(USER_ZONE2, 0x88, prt, 4);

    //写入累计量
    buflong = GetValue(BASEVAL_Amount) / 10;
    HEXtoBCD_B((unsigned char *)&TempLong, buflong, 4);
    write_zone(USER_ZONE2, 0x8c, prt, 4);

    //写入充值总量
    buflong =  GetValue(BASEVAL_Totaladd);
    HEXtoBCD_B((unsigned char *)&TempLong, buflong, 4);
    write_zone(USER_ZONE2, 0x4b, prt, 4);

    if(GetSetupVal(SETUP_MeterType) == SETUP_MeterType_MONEY)
    {
        //写入剩余金额
        if(GetRemainValue() > 0)
        {
            buflong = GetRemainValue() / 100;
            HEXtoBCD_S((unsigned char *)&TempLong, buflong, 4);
            write_zone(USER_ZONE2, 0x40, prt, 4);
            buflong = GetRemainValue() % 100;
            HEXtoBCD_B((unsigned char *)&TempLong, buflong, 2);
            write_zone(USER_ZONE2, 0x44, prt, 2);
        }
        else
        {
            TempLong = 0;
            write_zone(USER_ZONE2, 0x40, prt, 4);
            write_zone(USER_ZONE2, 0x44, prt, 2);
        }
        //写入单价更新序号
        buflong = GetValue(USERVAL_PriceIndex);
        HEXtoBCD_B((unsigned char *)&TempLong, buflong, 2);
        //bufint = buflong & 0x0ffff;
        write_zone(USER_ZONE2, 0x3c, prt, 2); //单价更新次数

        //写入单价整数部分
        buflong = GetValue(BASEVAL_Price) / 1000;
        HEXtoBCD_B((unsigned char *)&TempLong, buflong, 3);
        write_zone(USER_ZONE2, 0x37, prt, 3);
        //写入单价小数部分
        buflong = GetValue(BASEVAL_Price) % 1000;
        buflong *= 10;
        HEXtoBCD_B((unsigned char *)&TempLong, buflong, 2);
        write_zone(USER_ZONE2, 0x3a, prt, 2);

        //写入累计消费金额=累计充值量-余量
        buflong = GetValue(BASEVAL_Totaladd) - GetRemainValue();
        if(buflong > 0)
        {
            HEXtoBCD_B((unsigned char *)&TempLong, buflong / 100, 3);
            write_zone(USER_ZONE2, 0x46, prt, 3);

            HEXtoBCD_B((unsigned char *)&TempLong, buflong % 100, 2);
            write_zone(USER_ZONE2, 0x49, prt, 2);
        }
        else
        {
            TempLong = 0;
            write_zone(USER_ZONE2, 0x46, prt, 3);
            write_zone(USER_ZONE2, 0x49, prt, 2);
        }
    }
    return TRUE;
}

/**
 * 清除卡内验证区数据
 * 清除是否成功是没有关系的
 */
void CardClearEncode(void)
{
    memset(&EncodeInfo,0xff,sizeof(EncodeInfo));
    verify_pass(USER_ZONE2, WRITE, pass2write);
    write_zone(USER_ZONE2, (unsigned char)(Card.UserCard.blank4-Card.buf), 
               (unsigned char *)&EncodeInfo, sizeof(EncodeInfo));
}

/**
 * 写卡余量
 *
 */
unsigned char CardWriteRemain(unsigned long tRemain)
{
    unsigned long sRemain=0xffffffffl;
    unsigned char tbuf[4];
    
    HEXtoBCD_S(tbuf,tRemain,4);    
    if(verify_pass(USER_ZONE2, WRITE, pass2write)==TRUE)
    {
        write_zone(USER_ZONE2, (unsigned char)(Card.UserCard.CardRemain-Card.buf), 
                  tbuf, 4);
        tbuf[0]=0xff;
        //清除充值记录内的数据
        if((tRemain==0) && (Card.UserCard.ChargeRecode1[0x0d]!=0xff))
        {
            write_zone(USER_ZONE2, 0xbd, tbuf, 1);
            write_zone(USER_ZONE2, 0xcd, tbuf, 1);
            write_zone(USER_ZONE2, 0xdd, tbuf, 1);
            write_zone(USER_ZONE2, 0xed, tbuf, 1);
            write_zone(USER_ZONE2, 0xfd, tbuf, 1);
        }
    }
    
    memset(tbuf,0xff,4);
    if(verify_pass(USER_ZONE2, READ, pass2read)==TRUE){
        read_zone(USER_ZONE2, (unsigned char)(Card.UserCard.CardRemain-Card.buf), 
                  tbuf, 4);
        sRemain=BCDtoHEX_S(tbuf,4);
    }
    
    if(sRemain==tRemain)
        return TRUE;
    else
        return FALSE;
}

//新的充值过程
unsigned char ChargeNew(unsigned long tCharge,unsigned int tIndex)
{
    Stu_EncodeInfo tEncodeInfo;
    if(tCharge==0)
        return 0;

    //写备份验证区
    if((tIndex!=0xffff) && (tIndex!=0))
    {
        if(tIndex<=GetValue(USERVAL_ChargeIndex))
            return C_ERR_CHARGETIMES;
    }
    
    if(((long)tCharge+GetRemainValue())>GetValue(USERVAL_Maxremain))
        return C_ERR_MAX;
    EncodeInfo.ChargeAmount=tCharge;
    EncodeInfo.ChargeIndex=UserPara.EncodeChargeIndex+1;
    EncodeInfo.CRC=Crc16Get((unsigned char *)&EncodeInfo,sizeof(EncodeInfo)-2);
    if(verify_pass(USER_ZONE2, WRITE, pass2write) == FALSE)
        return C_ERR_PASS;
    write_zone(USER_ZONE2, (unsigned char)(Card.UserCard.blank4-Card.buf), 
               (unsigned char *)&EncodeInfo, sizeof(EncodeInfo));
    
    //读备份区再验证
    memset((unsigned char *)&tEncodeInfo,0xff,sizeof(EncodeInfo));    
    if(verify_pass(USER_ZONE2, READ, pass2read) == FALSE)
        return C_ERR_PASS;
    read_zone(USER_ZONE2, (unsigned char)(Card.UserCard.blank4-Card.buf), 
              (unsigned char *)&tEncodeInfo, sizeof(EncodeInfo));
    
    if(memcmp((unsigned char *)&EncodeInfo,(unsigned char *)&tEncodeInfo,sizeof(EncodeInfo))==0)
    {
        //清除卡内存量
        if(CardWriteRemain(0)==TRUE)
        {
            ReCharge(tCharge,tIndex);
            SetValue(USERVAL_ChargeAmount, tCharge);
            UserPara.EncodeChargeIndex=EncodeInfo.ChargeIndex;
            CardClearEncode();
        }
        else
        {
            return C_ERR_NOCARD;
        }
    }
    else
    {
        return C_ERR_NOCARD;
    }

    return 0;
}

unsigned char card_peizhika_process(void)
{
    unsigned char buf[8];
    //unsigned char i, k = 0;
    LcdDispLoading();

    read_zone(CON_ZONE, 0x38, buf, 8);
    /*for(i = 0; i < 8; i++) //超级清除卡~~~~~~~~
    {
        if(buf[i] == 0xaa)
            k += 1;
    }
    if(k == 8)
        clear_meter();*/

    if(GetCardState()!=STATUS_METER_INIT)
    {
        return C_ERR_LOCK;
    }

    if(buf[7] != 0xcc)
        return C_ERR_UNKNOW;

    switch (buf[0])
    {
    case 0xaa:
        SetSetupVal(SETUP_MeterType, SETUP_MeterType_MONEY);
        break;
    case 0x55:
        SetSetupVal(SETUP_MeterType, SETUP_MeterType_GAS);
        break;
    default:
        return C_ERR_UNKNOW;
    }

    UserPara.Config.UseSetupCard = 0;	        //1--使用设置卡		0--不使用
    UserPara.Config.HallCheck = 0;		//1--双霍尔计数		0--单霍尔计数
    UserPara.Config.NoHallSigAttact = 0;        //1--不使用单边磁攻击   0--使用
    UserPara.Config.CleanTest = 0;		//1--使用检测卡后要清除     0--不清除
    UserPara.Config.NoUserUse = 0;		//1--无用户状态下允许透支   0--不允许
    UserPara.Config.UseSigPrice = 0;            //1--使用单一价格           0--使用梯级价
    UserPara.Config.NoBandUse = 0;   	        //1--设置卡解绑后不关阀     0--关阀
    UserPara.Config.VerifyCompanyCode = 0;      //1--验证公司代码           0--不验证公司代码
    SetSetupVal(SETUP_CloseWarning,SETUP_OFF);  //告警不关阀
    //UserPara.Config.TwiceClose = 0;
    
    if(buf[1] & 0x01)
        UserPara.Config.UseSetupCard = 1;	//1--使用设置卡		0--不使用
    if(buf[1] & 0x02)
        UserPara.Config.HallCheck = 1;	        //1--双霍尔计数		0--单霍尔计数
    if(buf[1] & 0x04)
        UserPara.Config.NoHallSigAttact = 1;    //1--不使用单边磁攻击   0--使用
    if(buf[1] & 0x08)
        UserPara.Config.CleanTest = 1;	        //1--使用检测卡后要清除     0--不清除
    if(buf[1] & 0x10)
        UserPara.Config.NoUserUse = 1;	        //1--无用户状态下允许透支   0--不允许
    if(buf[1] & 0x20)
        UserPara.Config.UseSigPrice = 1;        //1--使用单一价格           0--使用梯级价
    if(buf[1] & 0x40)
        UserPara.Config.NoBandUse = 1;          //1--设置卡解绑后不关阀      0--关阀
    if(buf[1] & 0x80)
        UserPara.Config.VerifyCompanyCode = 1;  //1--验证公司代码           0--不验证公司代码
    if(buf[2] & 0x01)
        SetSetupVal(SETUP_CloseWarning,SETUP_ON);//告警关阀   
    //if(buf[2] & 0x02)
    //    UserPara.Config.TwiceClose = 1;     
       
    UserPara.Config.OverDraught = buf[4] * 10;
    UserPara.Config.LcdSleepTime = buf[5] * 10;
    //Setup.DoorActTime = buf[6] * 100;  

    SetSetupVal(SETUP_Precision, SETUP_Precision_001);
    DataSvrSaveAll();

    return PEIZHIKA;
}

/***************************************************************************
设置卡处理函数
返回值：卡类型
***************************************************************************/
unsigned char card_shezhika_process(void)
{
    unsigned char re;
    unsigned char R_cardcmc[4];
    unsigned char R_companycode[5];
    ////////////////////////step1 read para from card///////////////////////////////
    if((GetCardState() != STATUS_METER_INIT) && (UserPara.Config.VerifyCompanyCode == 1)) //已设置过的表需验证公司代码
    {
        if(verify_pass(USER_ZONE7, READ, pass7read) == FALSE)
            return C_ERR_PASS;
        read_zone(USER_ZONE7, 0xe9, R_companycode, 5);   //验证公司代码
        if(cmp_str(R_companycode, UserPara.CompanyCode, 5) == FALSE)
            return C_ERR_COMPANYCODE;
    }
    //读取所有数据
    re=CardReadAllData();
    if(re!=0)
        return re;
    //验证是否是小表设置卡
    if(Card.SetupCard.MeterType!=0x0f)
        return C_ERR_UNKNOW;
	
    //核实是否为金额设置卡
    if((Card.SetupCard.SetupCardCode[0]+Card.SetupCard.SetupCardCodeInv[0]!=0xff) ||
       (Card.SetupCard.SetupCardCode[1]+Card.SetupCard.SetupCardCodeInv[1]!=0xff) ||
       (Card.SetupCard.SetupCardCode[2]+Card.SetupCard.SetupCardCodeInv[2]!=0xff) ||
       (Card.SetupCard.SetupCardCode[3]+Card.SetupCard.SetupCardCodeInv[3]!=0xff))
    { 
        //气量设置卡
        if(GetSetupVal(SETUP_MeterType)==SETUP_MeterType_GAS)
        {
            SetSetupVal(SETUP_MeterType, SETUP_MeterType_GAS);
            memcpy(R_cardcmc,Card.SetupCard.IssueGasCMCode,4);
        }
        else
        {
            return C_ERR_UNKNOW;
        }
    }
    else
    {
        //金额设置卡
        if(GetSetupVal(SETUP_MeterType)==SETUP_MeterType_MONEY)
        {
            SetSetupVal(SETUP_MeterType, SETUP_MeterType_MONEY);
            memcpy(R_cardcmc,Card.SetupCard.IssueMoneyCMCode,4);
        }
        else
        {
            return C_ERR_UNKNOW;
        }
    }
    
    if(verify_pass(USER_ZONE7, READ, pass7read) == FALSE)
        return C_ERR_PASS;
    read_zone(USER_ZONE7, 0xe9, R_companycode, 5);
    
    memcpy(UserPara.CardCMC, R_cardcmc, 4);
    memcpy(UserPara.KeySeed, Card.SetupCard.UserKeySeed, 4);
    memcpy(UserPara.sKeyA, Card.SetupCard.UserSkeyA+1, 3);
    memcpy(UserPara.sKeyB, Card.SetupCard.UserSkeyB+1, 3);
    memcpy(UserPara.CompanyCode, R_companycode, 5);
    switch(GetCardState())
    {
    case STATUS_METER_INIT:
        SetValue(USERVAL_Maxremain, 99999999l);
        SetCardState(STATUS_METER_SETUP);
        break;
    case STATUS_METER_INUSE:
    case STATUS_METER_OVERUSE:
        SetCardState(STATUS_METER_OUTBAND);
        break;
    case STATUS_METER_CHECK:
    default:
        break;
    }
    
//    //解除异常状态
//    SetValue(GLOBEVAL_ForceCloseTimes, 0);
//    SetValue(GLOBEVAL_UnuseCloseTimes, 0);

    return SHEZHIKA;
}

/**
 * 用户卡处理函数
 * 返回值：卡类型
 * 
 */
unsigned char card_yonghuka_process(void)
{unsigned char buf[80];
    StuStepPrice T_StepPrice;
    unsigned long R_addgas = 0;
    unsigned int  R_chargeindex = 0;
    unsigned char R_companycode[5];
    unsigned long R_price;
    unsigned int  R_priceindex;
    unsigned char re;
    //恢复3区数据
    if(verify_pass(USER_ZONE3, READ, pass2read) == TRUE) //核实用户区3读密码
    {
        //写入新表的验证信息
        if(verify_pass(USER_ZONE3, WRITE, pass2write) == FALSE)
            return C_ERR_PASS;
        write_zone(USER_ZONE3,0x00,"IC卡民用表",10);
        write_zone(USER_ZONE3,0xA0,"Product by hxx",14);        
    }
    else
    {
        verify_pass(USER_ZONE3, READ, otherpass);
    }
    
    if(UserPara.Config.VerifyCompanyCode == 1)
    {
        if(verify_pass(USER_ZONE7, READ, pass7read) == FALSE)
            return C_ERR_PASS;
        read_zone(USER_ZONE7, 0xe9, R_companycode, 5);   //验证公司代码
        if(cmp_str(R_companycode, UserPara.CompanyCode, 5) == FALSE)
            return C_ERR_COMPANYCODE;
    }
    ////////////////////////step1 read para from card///////////////////////////////       
    //读取所有数据
    re=CardReadAllData();
    if(re!=0)
        return re;
    
    if(Card.UserCard.Mark3aa3[0] != 0x3a || Card.UserCard.Mark3aa3[1] != 0xa3)
        return C_ERR_DATA;
    
    //验证卡片ID
    re=CardVerifyUserID(Card.UserCard.Cardid);
    if(re!=0)
        return re;
    //验证卡序号
    re=CardVerifyCardIndex();
    if(re!=0)
        return re;
    
    //读取阶梯价/单价
    if(GetSetupVal(SETUP_MeterType)==SETUP_MeterType_MONEY)
    {
        if(UserPara.Config.UseSigPrice == 1)   //单一价
        {
            R_price = BCDtoHEX_S(Card.UserCard.PriceV3,5);
            R_priceindex = BCDtoHEX_S(Card.UserCard.PriceIndexV3,2);
        }
        else                                   //阶梯价
        {
            if(verify_pass(USER_ZONE4, READ, pass2read) == FALSE)
                return C_ERR_PASS;
            read_zone(USER_ZONE4,0x78,(unsigned char *)&T_StepPrice,sizeof(T_StepPrice));
            read_zone(USER_ZONE4,0x70,(unsigned char *)&buf,sizeof(buf));
            //if(!CrcAddCheck(StepPriceInfo.CRC,(unsigned char *)&StepPriceInfo,sizeof(StepPriceInfo)-1))
            //{
            //    memset(StepPriceInfo.PriceSerialNum,0x00,4);
            //    return C_ERR_DATA;
            //}
        }
    }
    else
    {
        SetValue(BASEVAL_Price, 10000);
    }
    
    UpdataCardPara();
    write_data_to_card();//充值前反写数据

    //验证价格/调价
    if(GetSetupVal(SETUP_MeterType)==SETUP_MeterType_MONEY)
    {
        if(UserPara.Config.UseSigPrice == 1)   //单一价
        {
            if(GetValue(BASEVAL_Price) == 0)
                SetValue(BASEVAL_Price,R_price);
            if((GetValue(BASEVAL_Price) != R_price) && (R_priceindex != 0xffff))//验证单价
            {
                if(GetValue(USERVAL_PriceIndex) >= R_priceindex)
                    return C_ERR_PRICETIMES;
            }
        }
        else                                  //阶梯价
        {
            R_priceindex = BCDtoHEX_S((unsigned char *)&T_StepPrice.PriceIndex,4);
            if(R_priceindex > GetValue(USERVAL_PriceIndex))
            {
                re=UpdataStep(&T_StepPrice);
                if(re==FALSE)
                    return C_ERR_PRICETIMES;
            }
            else
            {
                return C_ERR_PRICETIMES;
            }
        }
    }
    
    //获取充值量及充值序号
    R_addgas=BCDtoHEX_S(Card.UserCard.CardRemain,4);   
    buflong = BCDtoHEX_B(Card.UserCard.ChargeInfo+6,2);
    if(buflong==0xffffffffl)
        R_chargeindex = 0xffff;
    else
        R_chargeindex = (unsigned int)buflong;
    
    //获取备份区数据
    memcpy(&EncodeInfo,Card.UserCard.blank4,sizeof(EncodeInfo));
    //验证充值备份区数据，有效则直接充值
    if(Crc16Get((unsigned char *)&EncodeInfo,sizeof(EncodeInfo)-2)==EncodeInfo.CRC)
    {
        if(EncodeInfo.ChargeIndex==UserPara.EncodeChargeIndex+1)
        {
            //验证区数据有效
            if(R_addgas==0)
            {
                ReCharge(EncodeInfo.ChargeAmount,R_chargeindex);
                SetValue(USERVAL_ChargeAmount, EncodeInfo.ChargeAmount);
                UserPara.EncodeChargeIndex=EncodeInfo.ChargeIndex;
                CardClearEncode();
            }
            else
            {
                //清除存量
                if(CardWriteRemain(0)==TRUE)
                {
                    ReCharge(EncodeInfo.ChargeAmount,R_chargeindex);
                    SetValue(USERVAL_ChargeAmount, EncodeInfo.ChargeAmount);
                    UserPara.EncodeChargeIndex=EncodeInfo.ChargeIndex;
                    CardClearEncode();
                }
                else
                {
                    return C_ERR_NOCARD;
                }
            }
        }
        else
        {
            CardClearEncode();
            re=ChargeNew(R_addgas,R_chargeindex);
        }
    }
    else
    {
        //充值备份区数据无效，重新充值
        re=ChargeNew(R_addgas,R_chargeindex);
    }
    
    if(re!=0)
    {
        return re;
    }
    else
    {
        if(UserPara.Config.UseSigPrice == 1)
        {
            SetValue(BASEVAL_Price,R_price);
            SetValue(USERVAL_PriceIndex,R_priceindex);
        }
        SetValue(USERVAL_ChargeIndex,R_chargeindex);
        write_data_to_card();//充值成功后反写数据
    }
    
    return YONGHUKA;
}

/***************************************************************************
检测卡处理函数
返回值：卡类型
***************************************************************************/
unsigned char card_jianceka_process(void)
{
    unsigned char re;
    unsigned long R_addgas;
    
    switch(GetCardState())
    {
    case STATUS_METER_SETUP:        //设置状态
    case STATUS_METER_NOUSER_USE:   //无用户的状态的检表
    case STATUS_METER_CHECK:        //检表状态
        //透支量
        buflong=BCDtoHEX_SS(Card.UserCard.Overuse,3);
        buflong*=10;
        SetValue(USERVAL_Overdraught, buflong);
        //充值量
        buflong=BCDtoHEX_B(Card.UserCard.MaxRemain,3)*10;
        R_addgas=BCDtoHEX_S(Card.UserCard.CardRemain,4);
        
        if(R_addgas>buflong)
        {
            ReCharge(buflong,0xffff);
            R_addgas-=buflong;
        }
        else
        {
            ReCharge(R_addgas,0xffff);
            R_addgas=0;
        }
        CardWriteRemain(R_addgas);
        SetCardState(STATUS_METER_CHECK);
        re = JIANCEKA;
        break;
    case STATUS_METER_INIT:         //出厂状态，已经初始化，未设置，显示hello
    case STATUS_METER_INUSE:        //正常用户状态
    case STATUS_METER_OUTBAND:      //解除绑定状态，解绑状态下不允许透支。
    case STATUS_METER_OVERUSE:      //透支状态
    default:
        re=C_ERR_UNKNOW;
        break;
    }
    
    return re;
}

/** 
 * 将卡内数据转存到表内
 * 返回：错误代码
 */
unsigned char TranIn(void)
{
    unsigned char buf[10];
    long tLong;
    if((Card.TranCard.CardId[0]==0xff) && (Card.TranCard.CardId[1]==0xff))
        return C_ERR_TRAN_EMP;
    memset(buf,0xff,4);
    //清除数据
    if(verify_pass(USER_ZONE2, WRITE, pass2write) == FALSE) //核实用户区2写密码
        return C_ERR_PASS;
    write_zone(USER_ZONE2, 0x80, buf, 4);//清卡号
    write_zone(USER_ZONE2, 0x88, buf, 4);//清存量
    write_zone(USER_ZONE2, 0xd0, buf, 4);//清存量
    write_zone(USER_ZONE2, 0x0e, buf, 2);//清标志

    memset(buf,0x00,4);
    if(verify_pass(USER_ZONE2, READ, pass2read) == FALSE) //核实用户区2写密码
        return C_ERR_PASS;
    read_zone(USER_ZONE2, 0x80, buf, 4);//读卡号
    if((buf[0]!=0xff) || (buf[1]!=0xff))
        return C_ERR_TRAN_EMP;
    //将卡内数据存入表内
    //写入卡号
    memset(buf,0xff,10);
    memcpy(buf,Card.TranCard.CardId,4);
    if(CheckCardID(buf)==FALSE)
        return C_ERR_UNKNOW;
    //存量
    tLong=BCDtoHEX_B(Card.TranCard.Remain,4);
    //tLong*=10;
    if(Card.TranCard.RemainMark==0xfe)//负数
    {
        tLong*=-1;
    }
    SetRemainValue(tLong);
    //透支量
    tLong=BCDtoHEX_S(Card.TranCard.Overdraught,3);
    if(tLong>9999999 || tLong < 0)
        SetValue(USERVAL_Overdraught,0);
    else    
        SetValue(USERVAL_Overdraught,tLong);
    //累积量
    memcpy((unsigned char *)&tLong,Card.TranCard.AmountGas,4);
    if(tLong>9999999 || tLong < 0)
        SetValue(BASEVAL_Amount,0);
    else
        SetValue(BASEVAL_Amount,tLong*10);
    //报警量
    tLong=BCDtoHEX_S(Card.TranCard.Warning,1);
    SetValue(USERVAL_Warning,tLong*100);
    //闲置天数
    tLong=BCDtoHEX_S(Card.TranCard.IdleTime,1);
    SetValue(USERVAL_UnusedCloseTime,tLong*24);
    
    //充值序号
    tLong=0x100*Card.TranCard.ChargeIndex[1]+Card.TranCard.ChargeIndex[0];
    SetValue(USERVAL_ChargeIndex,tLong);
    
    //单价,充值序号
    if(GetSetupVal(SETUP_MeterType) == SETUP_MeterType_GAS)
    {
        SetValue(BASEVAL_Price,10000);
        //钱版的表不转存累积量 充值序号
        if((Card.TranCard.TranMarkV3!=0xac) && (Card.TranCard.TranMark!=0x00))
        {
            tLong=BCDtoHEX_B(Card.TranCard.AmountOld,4);
            tLong*=100;
            SetValue(BASEVAL_Amount,tLong);
            SetValue(USERVAL_ChargeIndex,0);//充值序号
        }
    }
    else//金额式
    {
        //清除单价序号
        SetValue(USERVAL_PriceIndex,0);    
        //单价,充值序号 分为：V1 V2 V3
        if(Card.TranCard.TranMarkV3==0xac)
        {
            //单价
            tLong=BCDtoHEX_S(Card.TranCard.PriceV3,4);
            SetValue(BASEVAL_Price,tLong);
        }
        else
        {
            if(Card.TranCard.TranMark==0x00)//V1 V2
            {
                //单价
                memcpy((unsigned char *)&tLong,Card.TranCard.Price,4);
                tLong*=100;
                SetValue(BASEVAL_Price,tLong);
            }
        }
    }
    //价格阶梯表
//    memcpy((unsigned char *)&Meter.StepPrice,Card.TranCard.StepPrice,sizeof(Meter.StepPrice));
    //清除换卡验证序号
    memset(UserPara.UserCardIndex,0xff,4);
    
    //清除卡内数据
    memset(Card.buf+0x20,0xff,0xff-0x20);
    if(verify_pass(USER_ZONE2, WRITE, pass2write) == FALSE) //核实用户区2写密码
        return C_ERR_PASS;
    if(write_zone(USER_ZONE2, 0x20, Card.buf+0x20, 0xff-0x20)==FALSE)
        return C_ERR_DATA;
        
    RefreshRemainVal();
    return ZHUANCUNKA;
}

/**
 * 将表内数据转存到卡内
 * 返回：错误代码
 */
unsigned char TranOut(void)
{
    unsigned char buf[4];
    long tLong;
    if((Card.TranCard.CardId[0]!=0xff) || (Card.TranCard.CardId[1]!=0xff))
        return C_ERR_FULLDATA;
    
    /*
     * 准备写入卡中的数据
     */
    memset(Card.buf,0xff,0xff);
    //卡号
    memcpy(Card.TranCard.CardId,UserPara.CardId,4);
    //存量
    tLong=GetRemainValue();
    //tLong/=10;
    if(tLong<0) //负数
    {
        Card.TranCard.RemainMark=0xfe;
        tLong*=-1;
    }
    else        //正数
    {
        Card.TranCard.RemainMark=0xff;
    }
    HEXtoBCD_B(Card.TranCard.Remain,tLong,4);
    HEXtoBCD_B(Card.TranCard.Remain2,tLong,4);
    //透支量        
    tLong=GetValue(USERVAL_Overdraught);
    HEXtoBCD_S(Card.TranCard.Overdraught,tLong,3);
    //累积量
    tLong=GetValue(BASEVAL_Amount);
    tLong/=10;
    memcpy(Card.TranCard.AmountGas,(unsigned char *)&tLong,4);
    tLong/=10;
    HEXtoBCD_B(Card.TranCard.AmountOld,tLong,4);
    //报警量
    tLong=GetValue(USERVAL_Warning)/100;
    HEXtoBCD_S(Card.TranCard.Warning,tLong,1);
    //闲置天数
    tLong=GetValue(USERVAL_UnusedCloseTime)/24;
    HEXtoBCD_S(Card.TranCard.IdleTime,tLong,1);
    //充值序号
    tLong=GetValue(USERVAL_ChargeIndex);
    HEXtoBCD_S(Card.TranCard.ChargeIndex,tLong,2);
    Card.TranCard.ChargeIndex[0]=tLong%0x100;
    Card.TranCard.ChargeIndex[1]=tLong/0x100;
    
    Card.TranCard.TranMarkV3=0xac;  //V3
    Card.TranCard.TranMark=0x00;    //V1 V2
    //单价
    tLong=GetValue(BASEVAL_Price);
    HEXtoBCD_S(Card.TranCard.PriceV3,tLong,4);      //V3
    tLong/=100;
    memcpy(Card.TranCard.Price,(unsigned char *)&tLong,4);//V1 V2       
    //阶梯价格表
//    memcpy(Card.TranCard.StepPrice,(unsigned char *)&Meter.StepPrice,sizeof(Meter.StepPrice));
    
    memset(buf,0x00,4);
    memcpy(Card.TranCard.Blank5,buf,4);
    memcpy(Card.TranCard.Blank6,buf,4);
    memcpy(Card.TranCard.Blank9,buf,4);
    
    /*
     * 向卡中写入数据
     */    
    if(verify_pass(USER_ZONE2, WRITE, pass2write) == FALSE) //核实用户区2写密码
        return C_ERR_PASS;
    if(write_zone(USER_ZONE2, 0x20, Card.buf+0x20, 0xff-0x20) == FALSE)
        return C_ERR_DATA; 
    if(write_zone(USER_ZONE2, 0x0e, Card.buf+0x0e, 2) == FALSE)
        return C_ERR_DATA; 
        
    DelayMs(10);
    memset(buf,0x00,4);
    if(verify_pass(USER_ZONE2, READ, pass2read) == FALSE) //核实用户区2写密码
        return C_ERR_PASS;
    read_zone(USER_ZONE2, 0x80, buf, 4);//读卡号

    if(cmp_str(buf,UserPara.CardId,4)==TRUE)
    {
        ClearMeter();
    }
    else
    {
        return C_ERR_NOCARD;
    }   

    return ZHUANCUNKA;
}

unsigned char	old_tran_process(void)
{
    return 0;
}

/***************************************************************************
转存卡处理函数
返回值：卡类型
***************************************************************************/
unsigned char card_zhuancunka_process(void)
{
    unsigned char re;
    unsigned char R_companycode[5];
    
    if(UserPara.Config.VerifyCompanyCode == 1)
    {
        if(verify_pass(USER_ZONE7, READ, pass7read) == FALSE)
            return C_ERR_PASS;
        read_zone(USER_ZONE7, 0xe9, R_companycode, 5);   //验证公司代码
        if(cmp_str(R_companycode, UserPara.CompanyCode, 5) == FALSE)
            return C_ERR_COMPANYCODE;
    }
    //读取所有数据
    re=CardReadAllData();
    if(re!=0)
        return re;
    
    switch(GetCardState())
    {
    //case STATUS_METER_UNUSED:       //出厂状态，未初始化，需要写入一些参数，显示程序版本号SP X-XX
    case STATUS_METER_INIT:         //出厂状态，已经初始化，未设置，显示hello
    case STATUS_METER_SETUP:        //设置状态
    case STATUS_METER_NOUSER_USE:   //无用户的状态的检表
    case STATUS_METER_CHECK:        //检表状态
        //表内无数据，转入
        re=TranIn();
        break;
    case STATUS_METER_INUSE:        //正常用户状态
    case STATUS_METER_OUTBAND:      //解除绑定状态，解绑状态下不允许透支。
    case STATUS_METER_OVERUSE:      //透支状态
        //表内有数据，转出
        re=TranOut();
        break;
    default:
        re=C_ERR_UNKNOW;
        break;
    }
    
    return re;
}

/***************************************************************************
清除卡处理函数
返回值：卡类型
***************************************************************************/
unsigned char card_qingchuka_process(void)
{
    unsigned char R_companycode[5];
    if(UserPara.Config.VerifyCompanyCode == 1)
    {
        if(verify_pass(USER_ZONE7, READ, pass7read) == FALSE)
            return C_ERR_PASS;
        read_zone(USER_ZONE7, 0xe9, R_companycode, 5);   //验证公司代码
        if(cmp_str(R_companycode, UserPara.CompanyCode, 5) == FALSE)
            return C_ERR_COMPANYCODE;
    }
    ClearMeter();
    return QINGCHUKA;
}

unsigned char card_qiehuanka_process(void)
{
    if(GetCardState()!=STATUS_METER_INIT)
        return C_ERR_UNKNOW;
    switch(GetSetupVal(SETUP_Precision))
    {
    case SETUP_Precision_001:
        SetSetupVal(SETUP_Precision,SETUP_Precision_010);
        break;
    case SETUP_Precision_010:
    default:
        SetSetupVal(SETUP_Precision,SETUP_Precision_001);
        break;
    }
    return QIEHUANKA;
}


//卡插入中断
void IntCard(void)
{
    if(CardBslRead() == 0)
    {
        CheckCard = 1;
    }
}

void Card_Init(void)
{
    CardPowerCheckInput();
    CardPowerContrlOutput();
    CardPowerContrlSet();
    CardRstOutput();
    CardRstClr();
    CardSclOutput();
    CardSclClr();
    CardSdaOutput();
    CardSdaClr();
    CardIntConfig(1,IntCard);  //下降沿中断
}

const St_RegTxt CardRegTag[] = {
    {sizeof(UserPara.EncodeChargeIndex)    ,"充值序号"},
    {sizeof(UserPara.CardCMC)              ,"CMC"},
    {sizeof(UserPara.KeySeed)              ,"Seed"},
    {sizeof(UserPara.sKeyA)                ,"KeyA"},
    {sizeof(UserPara.sKeyB)                ,"KeyB"},
    {sizeof(UserPara.CompanyCode)          ,"公司代码"},
    {sizeof(UserPara.UserCardIndex)        ,"换卡序号"},
    {sizeof(UserPara.CardState)            ,"卡状态"},
    {sizeof(UserPara.Config)               ,"配置参数"},
    {sizeof(UserPara.CardId)               ,"卡号"}
};

void LoadDefaultConfig(void)
{
    UserPara.Config.OverDraught = 50;		    //透支量	                默认50（0.5方）
    UserPara.Config.LcdSleepTime = 200;             //液晶休眠时间（100ms)	默认20s
    UserPara.Config.UseSetupCard = 1;		    //1--使用设置卡		0--不使用
    UserPara.Config.HallCheck = 1;		    //1--双霍尔计数	        0--单霍尔计数
    UserPara.Config.NoHallSigAttact = 1; 	    //1--不使用单边磁攻击       0--使用  
    UserPara.Config.CleanTest = 0;		    //1--使用检测卡后要清除   	0--不清除
    UserPara.Config.NoUserUse = 1;		    //1--无用户状态下允许透支	0--不允许
    UserPara.Config.UseSigPrice = 0;		    //1--使用单一价格		0--使用梯级价
    UserPara.Config.NoBandUse = 1;   		    //1--设置卡解绑后不关阀   	0--关阀  
    UserPara.Config.VerifyCompanyCode = 0;	    //1--验证公司代码           0--不验证公司代码
    SetCardState(STATUS_METER_INIT);   //卡状态
    memset(UserPara.UserCardIndex,0xff,4);
}

/**
 * 卡参数申请空间
 * 
 * @author HXX (2014-12-17)
 */
void RegCardData(void)
{
    St_SAVESTRUCT temp;

    temp.fun = LoadDefaultConfig;
    temp.data = (unsigned char *)&UserPara;
    temp.len = sizeof(UserPara);
    DataSvrReg(sizeof(CardRegTag) / 9, &temp, (St_RegTxt *)&CardRegTag);
}
