#ifndef __DRV_DISP_H__
#define __DRV_DISP_H__

#define DefaultLine 0
#define ExtLine     1

#define LCD_DISP_BIT_0      0x01
#define LCD_DISP_BIT_1      0x02
#define LCD_DISP_BIT_2      0x04
#define LCD_DISP_BIT_3      0x08
#define LCD_DISP_BIT_4      0x10
#define LCD_DISP_BIT_5      0x20
#define LCD_DISP_BIT_6      0x40
#define LCD_DISP_BIT_7      0x80

#define UNKNOW              24
#define LCD_CHAR_BLANK      24
#define LCD_CHAR_SEPARATE   16
#define LCD_CHAR_A          10
#define LCD_CHAR_b          11
#define LCD_CHAR_C          12
#define LCD_CHAR_c          31
#define LCD_CHAR_d          13
#define LCD_CHAR_E          14
#define LCD_CHAR_F          15
#define LCD_CHAR_G          26
#define LCD_CHAR_H          17
#define LCD_CHAR_h          UNKNOW
#define LCD_CHAR_I          1
#define LCD_CHAR_i          30
#define LCD_CHAR_J          UNKNOW
#define LCD_CHAR_K          UNKNOW
#define LCD_CHAR_L          18
#define LCD_CHAR_M          UNKNOW
#define LCD_CHAR_N          27
#define LCD_CHAR_n          19
#define LCD_CHAR_O          0
#define LCD_CHAR_o          23
#define LCD_CHAR_P          21
#define LCD_CHAR_Q          UNKNOW
#define LCD_CHAR_R          29
#define LCD_CHAR_r          22
#define LCD_CHAR_S          5
#define LCD_CHAR_t          25
#define LCD_CHAR_U          28
#define LCD_CHAR_u          20
#define LCD_CHAR_V          UNKNOW
#define LCD_CHAR_W          UNKNOW
#define LCD_CHAR_X          UNKNOW
#define LCD_CHAR_Y          UNKNOW
#define LCD_CHAR_Z          UNKNOW

typedef enum
{
    MARK_AMOUNT,    //累计
    MARK_PRICE,     //单价
    MARK_REMAIN,    //剩余
    MARK_STEP,      //阶梯
    MARK_SIGN1,     //信号1
    MARK_SIGN2,     //信号2
    MARK_SIGN3,		//信号3
    MARK_ERR,       //告警
    MARK_GAS,       //气量
    //MARK_CURRENT,   //当前
    MARK_MONEY,     //金额
    //MARK_PAST,      //上
    MARK_MONTH,     //月
    MARK_OPEN,      //阀门开
    MARK_CLOSE,     //阀门关
    MARK_BATT1,     //电量1
    MARK_BATT2,     //电量2
    MARK_BATT3,     //电量3
    MARK_BATT_FREAM,//电池框
    //MARK_CHECK,     //检测
    MARK_CONSUMED,   //消费
    MARK_FREEZE,     //冻结
    MARK_BACKSLASH,      //反斜杠
    //MARK_PRESS,     //压力
    MARK_CHARGE,      //充值
    MARK_YAN,       //元
    MARK_M3,        //立方米
    MARK_1,         //月份十位
   MARK_2POINT,    //两点
}_EnumMark;

//bsp接口
extern void OV_LcdOn(void);
extern void OV_LcdOff(void);
extern void OV_LcdInit(void);


//应用接口
void OV_LcdDispAll(void);                                     //全显
void OV_LcdClear(void);                                       //清屏
void OV_LcdRefresh(unsigned char *tLcdDataBuf);               //刷新
void OV_LcdClearNum();                                        //清除数值
void OV_LcdDispChar(unsigned char index, unsigned char ch);   //显示字符
void OV_LcdDispMark(_EnumMark MarkIndex,char t);              //显示标识
void OV_LcdDispNumWithDot(long num,char dots);                //显示带小数点的数值
void OV_DispRemainGas(long num,char dots,unsigned int b);     //显示剩余气量  num-数值  dots-小数点位数  b-月份 
															                  //当b大于12或等于0时不显示月份
void OV_DispRemainMoney(long num,char dots,unsigned int b);   //显示剩余金额
void OV_DispConsumedGas(long num,char dots,unsigned int b);   //显示消费气量
void OV_DispConsumedMoney(long num,char dots,unsigned int b); //显示消费金额
void OV_DispChargeGas(long num,char dots,unsigned int b);     //显示充值气量
void OV_DispChargeMoney(long num,char dots,unsigned int b);   //显示充值金额
void OV_DispAmountGas(long num,char dots,unsigned int b);     //显示累计气量
void OV_DispAmountMoney(long num,char dots,unsigned int b);   //显示累计金额
void OV_DispFreezeGas(long num,char dots,unsigned int b);     //显示冻结气量
void OV_DispFreezeMoney(long num,char dots,unsigned int b);   //显示冻结金额
void OV_DispPrice(long num,char dots);                        //显示单价
void OV_DispErr(void);                                        //显示报警
void OV_DispValve(char c);                                    //显示开/关阀状态
void OV_DispBatt(unsigned int i);                             //显示电量 
void OV_DispSign(unsigned int i);                             //显示信号
void OV_DispMonth(unsigned int b);                             //显示月份
void OV_DispTime(unsigned int a,unsigned int b,unsigned int c);//显示时间
void OV_DispDate(unsigned int n,unsigned int m,unsigned int k);//显示日期
void OV_DispHello(void);                                       //显示"HELLO"
void OV_DispEnd(void);                                         //显示"End"
void OV_DispStepMoney(long num,char dots,unsigned int b);      //显示阶梯金额
void OV_DispStepGas(long num,char dots,unsigned int b);        //显示阶梯气量
void OV_DispStepRemainGas(long num,char dots,unsigned int b);  //显示阶梯剩余气量
void OV_DispStepRemainMoney(long num,char dots,unsigned int b);//显示阶梯剩余金额

void OV_DispSucc(void);
void OV_DispFail(void);
void OV_LcdDispPara1(unsigned char ch1,unsigned long num);
void OV_LcdDispPara2(unsigned char ch1,unsigned char ch2,unsigned int num);
void OV_LcdDispIP(unsigned char c,unsigned char ip0,unsigned char ip1);

//add 2014-12-23
void OV_DispStepNVol(long num,char dots,unsigned char b);
void OV_DispStepNMoney(long num,char dots,unsigned char b);
void OV_DispStepNRemainVol(long num,char dots,unsigned char b);
void OV_DispStepNRemainMoney(long num,char dots,unsigned char b);
void OV_DispStepNPrice(long num,char dots,char b);



void LCD(void);
#endif
