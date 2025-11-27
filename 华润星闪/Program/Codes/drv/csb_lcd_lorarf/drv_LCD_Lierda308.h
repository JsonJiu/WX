#ifndef _LCDLIERDA308_H
#define _LCDLIERDA308_H

typedef enum
{
    Csb_AMOUNTVOL,     //累计气量
    Csb_REMAINVOL,     //剩余气量
    Csb_STEPVOL,       //阶梯气量
    Csb_REMAINMONEY,   //剩余金额
    Csb_CURPRICE,      //当前价格
    Csb_CURMONEY,      //当前金额
    Csb_CURSTEPAMOUNTVOL,//当前阶梯累计气量
}CSB_Dis_Mark;


#define Up_A                 0x01
#define Up_B                 0x02
#define Up_C                 0x04
#define Up_D                 0x08
#define Up_E                 0x40
#define Up_F                 0x10
#define Up_G                 0x20

#define Up_Letter_E          (Up_A+Up_D+Up_E+Up_F+Up_G)
#define Up_Letter_r          (Up_E+Up_F+Up_A)

#define FanXiang_Symbol   0x80/////////Up_DisBuff[0]
#define P4_Symbol         0x80/////////Up_DisBuff[1]
#define P3_Symbol         0x80/////////Up_DisBuff[2]
#define P2_Symbol         0x80/////////Up_DisBuff[3]
#define P1_Symbol         0x80/////////Up_DisBuff[4]

#define FanXiang_Position 0/////////Up_DisBuff[0]
#define P4_Position       1/////////Up_DisBuff[1]
#define P3_Position       2/////////Up_DisBuff[2]
#define P2_Position       3/////////Up_DisBuff[3]
#define P1_Position       4/////////Up_DisBuff[4]

#define Link_Symbol       0x20/////////Up_DisBuff[0]---Up_DisBuff[4]
#define Link_Position     0

#define Insp_Symbol       0x10/////////Up_DisBuff[5]
#define Temp_Symbol       0x20/////////Up_DisBuff[5]
#define Pre_Symbol        0x40/////////Up_DisBuff[5]
#define Flow_Symbol       0x80/////////Up_DisBuff[5]

#define Insp_Position     0x10/////////Up_DisBuff[5]
#define Temp_Position     0x20/////////Up_DisBuff[5]
#define Pre_Position      0x40/////////Up_DisBuff[5]
#define Flow_Position     0x80/////////Up_DisBuff[5]

#define Down_A                  0x08
#define Down_B                  0x04
#define Down_C                  0x02
#define Down_D                  0x01
#define Down_E                  0x20
#define Down_F                  0x80
#define Down_G                  0x40

#define QiLiang_DanWei          0x20//////Down_Buff[0]
#define JinE_DanWei             0x40//////Down_Buff[0]

#define LeiJi_Symbol            0x80//////Down_Buff[1]
#define ShengYu_Symbol          0x40//////Down_Buff[1]
#define CuoWu_Symbol            0x20//////Down_Buff[1]
#define DangQian_Symbol         0x10//////Down_Buff[1]
#define DanJia_Symbol           0x08//////Down_Buff[1]
#define JieTi_Symbol            0x04//////Down_Buff[1]
#define QiLiang_Symbol          0x02//////Down_Buff[1]
#define JinE_Symbol             0x01//////Down_Buff[1]

#define P6_Symbol               0x10//////Down_Buff[9]
#define P7_Symbol               0x10//////Down_Buff[8]
#define P8_Symbol               0x10//////Down_Buff[7]
#define P9_Symbol               0x10//////Down_Buff[6]
#define P10_Symbol              0x10//////Down_Buff[5]
#define P11_Symbol              0x10//////Down_Buff[4]

#define QianYa_Symbol           0x10

#define Down_Letter_E          (Down_A+Down_D+Down_E+Down_F+Down_G)
#define Down_Letter_r          (Down_E+Down_F+Down_A)
#define Down_Link_Symbol       (Down_G)

#define CSB_UNKNOW              24
#define CSB_LCD_CHAR_BLANK      0//24
#define CSB_LCD_CHAR_SEPARATE   Down_G//16
#define CSB_LCD_CHAR_A          (Down_A + Down_B + Down_C + Down_E + Down_F + Down_G)//10
#define CSB_LCD_CHAR_b          (Down_C + Down_D + Down_E + Down_F + Down_G)//11
#define CSB_LCD_CHAR_C          (Down_A + Down_D + Down_E + Down_F)//12
#define CSB_LCD_CHAR_c          (Down_D + Down_E + Down_G)//31
#define CSB_LCD_CHAR_d          (Down_B + Down_C + Down_D + Down_E + Down_G)//13
#define CSB_LCD_CHAR_E          (Down_A + Down_D + Down_E + Down_F + Down_G)//14
#define CSB_LCD_CHAR_F          (Down_A + Down_E + Down_F + Down_G)//15
#define CSB_LCD_CHAR_G          (Down_A + Down_C + Down_D + Down_E + Down_F)//26
#define CSB_LCD_CHAR_H          (Down_B + Down_C + Down_E + Down_F + Down_G)//17
#define CSB_LCD_CHAR_h          (Down_C + Down_E + Down_F + Down_G)//UNKNOW
#define CSB_LCD_CHAR_I          (Down_B + Down_C)//1
#define CSB_LCD_CHAR_i          (Down_C)//30
#define CSB_LCD_CHAR_J          UNKNOW
#define CSB_LCD_CHAR_K          UNKNOW
#define CSB_LCD_CHAR_L          (Down_D + Down_E + Down_F)//18
#define CSB_LCD_CHAR_M          UNKNOW
#define CSB_LCD_CHAR_N          (Down_A + Down_B + Down_C + Down_E + Down_F)//27
#define CSB_LCD_CHAR_n          (Down_C + Down_E + Down_G)//19
#define CSB_LCD_CHAR_O          (Down_A + Down_B + Down_C + Down_D + Down_E + Down_F)//0
#define CSB_LCD_CHAR_o          (Down_C + Down_D + Down_E + Down_G)//23
#define CSB_LCD_CHAR_P          (Down_A + Down_B + Down_E + Down_F + Down_G)//21
#define CSB_LCD_CHAR_Q          UNKNOW
#define CSB_LCD_CHAR_R          (Down_A + Down_E + Down_F)//29
#define CSB_LCD_CHAR_r          (Down_E + Down_F + Down_G)//22
#define CSB_LCD_CHAR_S          (Down_A + Down_C + Down_D + Down_F + Down_G)//5
#define CSB_LCD_CHAR_t          (Down_D + Down_E + Down_F + Down_G)//25
#define CSB_LCD_CHAR_U          (Down_B + Down_C + Down_D + Down_E + Down_F)//28
#define CSB_LCD_CHAR_u          (Down_C + Down_D + Down_E)//20
#define CSB_LCD_CHAR_V          UNKNOW
#define CSB_LCD_CHAR_W          UNKNOW
#define CSB_LCD_CHAR_X          UNKNOW
#define CSB_LCD_CHAR_Y          UNKNOW
#define CSB_LCD_CHAR_Z          UNKNOW


#define OtheDown_ADD            6

void CSB_LCD_Init();
unsigned char CSB_LCD_SoftInit();
unsigned char CSB_LCD_ON();
unsigned char CSB_LCD_OFF();
unsigned char CSB_LCD_Clear();
unsigned char CSB_LCD_All();
unsigned char CSB_LCD_DisFlow(unsigned char D_Direct,unsigned char D_FlowStatus,unsigned char D_FlowRate_Status,unsigned long D_Flow);
unsigned char CSB_LCD_Insp_DisFlow(unsigned char D_Direct,unsigned char D_FlowStatus,unsigned long D_Flow);
unsigned char CSB_LCD_DisTemp(unsigned char D_Direct,unsigned char D_Temptatus,signed int D_Temp);
unsigned char CSB_LCD_DisPre(unsigned char D_Direct,unsigned char D_PreStatus,unsigned int D_Pre);
unsigned char CSB_LCD_DisUPErr(unsigned char D_UPErr);
unsigned char CSB_LCD_DisVolume(CSB_Dis_Mark DIS_M,signed long D_Volume);
unsigned char CSB_LCD_BatCapacity(unsigned char D_Bat);
unsigned char CSB_LCD_Valve(unsigned char D_Valve);
//unsigned char CSB_LCD_UnderVoltage(unsigned int D_Hour,unsigned int D_Min,unsigned int D_Sec);
unsigned char CSB_LCD_DisDOWNErr(unsigned char D_DOWNErr);
//unsigned char CSB_LCD_DisQianYa(unsigned char D_QianYa);
//unsigned char CSB_LCD_DisDanWei(unsigned char D_DanWei,unsigned char D_Switch);
unsigned char CSB_LCD_DispBitNum(unsigned char D_Position,unsigned char D_Num);
//unsigned char CSB_LCD_DispMeterID(unsigned char *D_ID);
//unsigned char CSB_LCD_DispH_IP(unsigned char D_H,unsigned char D_L);
//unsigned char CSB_LCD_DispL_IP(unsigned char D_H,unsigned char D_L);
//unsigned char CSB_LCD_DispPort(unsigned short D_P);
unsigned char CSB_LCD_DispDate(unsigned char D_Year,unsigned char D_Month,unsigned char D_Day);
unsigned char CSB_LCD_DispTime(unsigned char D_Hour,unsigned char D_Min,unsigned char D_Sec);
//unsigned char CSB_LCD_DispSignal(unsigned char D_Signal);
//unsigned char CSB_LCD_DispVersion(unsigned char D_Version,unsigned char D_Version1,unsigned char D_Version2);
//unsigned char CSB_LCD_DispSGDay(unsigned char D_SGDay,unsigned char D_SG1,unsigned char D_SG2);
//unsigned char CSB_LCD_DispKTPage(unsigned char KTPage);
//unsigned char CSB_LCD_DispMeterType(unsigned char D_Type);
//unsigned char CSB_LCD_DispZDNum(unsigned long D_ZDNum);
unsigned char CSB_LCD_DispStep(unsigned char S_T,unsigned char S_Num,unsigned long S_VOL);
unsigned char CSB_LCD_HardVision(unsigned char *H_Version);
unsigned char CSB_LCD_SoftVision(unsigned char *S_Version);
unsigned char CSB_LCD_DispKPMeterID(unsigned char M_Num,unsigned char *M_ID);
unsigned char CSB_LCD_DispKPPD(unsigned char *P_Data);
unsigned char CSB_LCD_DispKPNetID(unsigned char *N_Data);
#endif
