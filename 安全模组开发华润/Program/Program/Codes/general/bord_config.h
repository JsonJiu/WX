/* 
 * File:   bord_config.h
 * Author: Administrator
 *
 * Created on May 11, 2017, 11:13 AM
 */

#ifndef BORD_CONFIG_H
#define	BORD_CONFIG_H

#ifdef	__cplusplus
extern "C" {
#endif

#include    <p24Fxxxx.h>

 //device id
    #define P0_INIT  (TRISBbits.TRISB0 = 1,  ANSBbits.ANSB0 = 0,  ODCBbits.ODB0 = 0, CNPU1bits.CN2PUE = 1)
    #define P0_PORT  PORTBbits.RB0

    #define P1_INIT  (TRISBbits.TRISB6 = 1,  ANSBbits.ANSB6 = 0,  ODCBbits.ODB6 = 0, CNPU2bits.CN24PUE = 1)
    #define P1_PORT  PORTBbits.RB6

    #define P2_INIT  (TRISBbits.TRISB7 = 1,  ANSBbits.ANSB7 = 0,  ODCBbits.ODB7 = 0, CNPU2bits.CN25PUE = 1)
    #define P2_PORT  PORTBbits.RB7

 //串口
// 连主测试板
#define  COM1_TX_IN        (TRISGbits.TRISG6 = 1, ANSGbits.ANSG6 = 0, ODCGbits.ODG6 = 0 )
#define  COM1_TX_OUT       (TRISGbits.TRISG6 = 0, ANSGbits.ANSG6 = 0, ODCGbits.ODG6 = 0 )
#define  COM1_RX_IN        (TRISCbits.TRISC3 = 1, /*ANSCbits.ANSC3 = 0,*/  ODCCbits.ODC3 = 0 )
#define  COM1_OUT_PIN_PPS  OUT_PIN_PPS_RP21
#define  COM1_IN_PIN_PPS   IN_PIN_PPS_RPI40

// 连主控板
#define  COM2_TX_IN        (TRISBbits.TRISB2 = 1, ANSBbits.ANSB2 = 0,  ODCBbits.ODB2 = 0 )
#define  COM2_TX_OUT       (TRISBbits.TRISB2 = 0, ANSBbits.ANSB2 = 0,  ODCBbits.ODB2 = 0 )
#define  COM2_RX_IN        (TRISBbits.TRISB1 = 1, ANSBbits.ANSB1 = 0,  ODCBbits.ODB1 = 0  )
#define  COM2_OUT_PIN_PPS  OUT_PIN_PPS_RP13
#define  COM2_IN_PIN_PPS   IN_PIN_PPS_RP1

// 调试信息
#define  COM3_TX_IN       (TRISDbits.TRISD2 = 1, /* ANSDbits.ANSD2 = 0,*/  ODCDbits.ODD2 = 0 )
#define  COM3_TX_OUT       (TRISDbits.TRISD2 = 0, /* ANSDbits.ANSD2 = 0,*/  ODCDbits.ODD2 = 0 )
#define  COM3_RX_IN        (TRISDbits.TRISD12 = 1, /* ANSDbits.ANSD12 = 0,*/  ODCDbits.ODD12 = 0 )
#define  COM3_OUT_PIN_PPS   OUT_PIN_PPS_RP23
#define  COM3_IN_PIN_PPS    IN_PIN_PPS_RPI42

//IR
#define  COM4_TX_IN       (TRISGbits.TRISG9 = 1, ANSGbits.ANSG9 = 0,  ODCGbits.ODG9 = 0 )
#define  COM4_TX_OUT       (TRISGbits.TRISG9 = 0, ANSGbits.ANSG9 = 0,  ODCGbits.ODG9 = 0 )
#define  COM4_RX_IN        (TRISGbits.TRISG7 = 1, ANSGbits.ANSG7 = 0,  ODCGbits.ODG7 = 0 )
#define  COM4_OUT_PIN_PPS  OUT_PIN_PPS_RP27
#define  COM4_IN_PIN_PPS   IN_PIN_PPS_RP26

/***  主控板输出口 */
#define VALVE_R1_IN          (TRISBbits.TRISB13 = 1, ANSBbits.ANSB13 = 0,  ODCBbits.ODB13 = 0 )
#define VALVE_R1_STATUS      PORTBbits.RB13
#define VALVE_B1_IN          (TRISBbits.TRISB14 = 1, ANSBbits.ANSB14 = 0, ODCBbits.ODB14 = 0)
#define VALVE_B1_STATUS      PORTBbits.RB14

#define VALVE_R2_IN          (TRISEbits.TRISE5 = 1, ANSEbits.ANSE5 = 0,  ODCEbits.ODE5 = 0 )
#define VALVE_R2_STATUS      PORTEbits.RE5
#define VALVE_B2_IN          (TRISEbits.TRISE6 = 1, ANSEbits.ANSE6 = 0,  ODCEbits.ODE6 = 0 )
#define VALVE_B2_STATUS      PORTEbits.RDE6

// IC卡
#define CARD_SDA_IN         (TRISFbits.TRISF8 = 1, /*ANSFbits.ANSF8 = 0,*/  ODCFbits.ODF8 = 0 )
#define CARD_SDA_STATUS     PORTFbits.RF8

#define CARD_SCL_IN         (TRISFbits.TRISF3 = 1, /*ANSFbits.ANSF3 = 0,*/  ODCFbits.ODF3 = 0 )
#define CARD_SCL_STATUS     PORTFbits.RF3

#define CARD_RST_IN         (TRISFbits.TRISF4 = 1, /*ANSFbits.ANSF4 = 0,*/  ODCFbits.ODF4 = 0 )
#define CARD_RST_STATUS     PORTFbits.RF4

//#define CARD_POW_IN         TRISEbits.TRISE4 = 1;
//#define CARD_POW_STATUS     PORTBbits.RB14

// 温压
#define TP_SDA_IN         (TRISFbits.TRISF5 = 1, /*ANSFbits.ANSF5 = 0,*/  ODCFbits.ODF5 = 0 )
#define TP_SDA_STATUS     PORTFbits.RF5

#define TP_SCL_IN         (TRISBbits.TRISB15 = 1, ANSBbits.ANSB15 = 0,  ODCBbits.ODB15 = 0 )
#define TP_SCL_STATUS     PORTBbits.RB15

#define TP_POW_IN         (TRISDbits.TRISD15 = 1, /*ANSDbits.ANSD15 = 0, */ ODCDbits.ODD15 = 0 )
#define TP_POW_STATUS     PORTDbits.RD15

//
#define ZD_POW_IN          (TRISEbits.TRISE4 = 1, ANSEbits.ANSE4 = 0,  ODCEbits.ODE4 = 0 )
#define ZD_POW_STATUS      PORTEbits.RE4

/**  主控板输入口 */

#define ZD_SIGNAL_IN       //(TRISBbits.TRISB13 = 1, ANSBbits.ANSB13 = 0, ODCBbits.ODB13 = 0 )
#define ZD_SIGNAL_STATUS   //PORTBbits.RB13
    
#ifdef DEF_HALL_OUTSIDE // 霍尔在字轮上

#define IO_HALL_A_IN    (TRISDbits.TRISD3 = 1, /*ANSCbits.ANSC1 = 0, */ ODCDbits.ODD3 = 0 )
#define IO_HALL_A_OUT   (TRISDbits.TRISD3 = 0, /*ANSCbits.ANSC1 = 0, */ ODCDbits.ODD3 = 0 )
#define IO_HALL_A_1     LATDbits.LATD3 = 1
#define IO_HALL_A_0     LATDbits.LATD3 = 0

#define IO_HALL_B_IN    (TRISDbits.TRISD5 = 1, /*ANSCbits.ANSC1 = 0, */ ODCDbits.ODD5 = 0 )
#define IO_HALL_B_OUT   (TRISDbits.TRISD5 = 0, /*ANSCbits.ANSC1 = 0, */ ODCDbits.ODD5 = 0 )
#define IO_HALL_B_1     LATDbits.LATD5 = 1
#define IO_HALL_B_0     LATDbits.LATD5 = 0
    
#else // DEF_HALL_ON_BOARD // 霍尔在主控板上

#define IO_HALL_A_IN    (TRISCbits.TRISC1 = 1, /*ANSCbits.ANSC1 = 0, */ ODCCbits.ODC1 = 0 )
#define IO_HALL_A_OUT   (TRISCbits.TRISC1 = 0, /*ANSCbits.ANSC1 = 0, */ ODCCbits.ODC1 = 0 )
#define IO_HALL_A_1     LATCbits.LATC1= 1
#define IO_HALL_A_0     LATCbits.LATC1 = 0

#define IO_HALL_B_IN    (TRISEbits.TRISE7 = 1, ANSEbits.ANSE7 = 0,  ODCEbits.ODE7 = 0 )
#define IO_HALL_B_OUT   (TRISEbits.TRISE7 = 0, ANSEbits.ANSE7 = 0,  ODCEbits.ODE7 = 0 )
#define IO_HALL_B_1     LATEbits.LATE7 = 1
#define IO_HALL_B_0     LATEbits.LATE7 = 0

#define IO_119_OUT      (TRISDbits.TRISD13 = 0, /*ANSDbits.ANSD13 = 0,*/  ODCDbits.ODD13 = 0 )
#define IO_119_0        LATDbits.LATD13 = 0
#define IO_119_1        LATDbits.LATD13 = 1
    
#endif

//#define IO_KEY_1_OUT   (TRISBbits.TRISB7 = 0, ANSBbits.ANSB7 = 0, ODCBbits.ODB7 = 0)
//#define IO_KEY_1_1     LATBbits.LATB7 = 1
//#define IO_KEY_1_0     LATBbits.LATB7 = 0
//
//#define IO_KEY_2_OUT   (TRISBbits.TRISB6 = 0, ANSBbits.ANSB6 = 0, ODCBbits.ODB6 = 0)
//#define IO_KEY_2_1     LATBbits.LATB6 = 1
//#define IO_KEY_2_0     LATBbits.LATB6 = 0

#define IO_CARD_XB_IN   (TRISFbits.TRISF2 = 1, /*ANSFbits.ANSF2 = 0,*/  ODCFbits.ODF2 = 0 )
#define IO_CARD_XB_OUT  (TRISFbits.TRISF2 = 0, /*ANSFbits.ANSF2 = 0,*/  ODCFbits.ODF2 = 0 )
#define IO_CARD_XB_1     (IO_CARD_XB_OUT, LATFbits.LATF2 = 1)
#define IO_CARD_XB_0     (IO_CARD_XB_OUT, LATFbits.LATF2 = 0)

#define IO_CARD_ERR_IN   (TRISDbits.TRISD14 = 1,/*ANSDbits.ANSD14 = 0,*/  ODCDbits.ODD14 = 0 )
#define IO_CARD_ERR_OUT  (TRISDbits.TRISD14 = 0,/*ANSDbits.ANSD14 = 0,*/  ODCDbits.ODD14 = 0 )
#define IO_CARD_ERR_1     (IO_CARD_ERR_OUT, LATDbits.LATD14 = 1)
#define IO_CARD_ERR_0     (IO_CARD_ERR_OUT, LATDbits.LATD14 = 0)

#define IO_VALVE_DAOWEI1_IN    (TRISBbits.TRISB15 = 1, ANSBbits.ANSB15 = 0,  ODCBbits.ODB15 = 0 )
#define IO_VALVE_DAOWEI1_OUT   (TRISBbits.TRISB15 = 0, ANSBbits.ANSB15 = 0,  ODCBbits.ODB15 = 0 )
#define IO_VALVE_DAOWEI1_1     LATBbits.LATB15 = 1
#define IO_VALVE_DAOWEI1_0     LATBbits.LATB15 = 0

//#define IO_VALVE_DAOWEI1_IN    (TRISEbits.TRISE2 = 1, /*ANSEbits.ANSE2 = 0,*/  ODCEbits.ODE2 = 0 )
//#define IO_VALVE_DAOWEI1_OUT   (TRISEbits.TRISE2 = 0, /*ANSEbits.ANSE2 = 0,*/  ODCEbits.ODE2 = 0 )
//#define IO_VALVE_DAOWEI1_1     LATEbits.LATE2 = 1
//#define IO_VALVE_DAOWEI1_0     LATEbits.LATE2 = 0

#define IO_VALVE_DAOWEI2_IN    (TRISEbits.TRISE3 = 1, ANSEbits.ANSE3 = 0,  ODCEbits.ODE3 = 0 )
#define IO_VALVE_DAOWEI2_OUT   (TRISEbits.TRISE3 = 1, ANSEbits.ANSE3 = 0,  ODCEbits.ODE3 = 0 )
#define IO_VALVE_DAOWEI2_1     LATEbits.LATE3 = 1
#define IO_VALVE_DAOWEI2_0     LATEbits.LATE3 = 0

// 低功耗供电
// check consumption power select pin
#define CONSUMPTIONPOWER_OUT (TRISDbits.TRISD10 = 0,/*ANSDbits.ANSD10 = 0,*/  ODCDbits.ODD10 = 0 )
#define CONSUMPTIONPOWER_1  LATDbits.LATD10 = 1
#define CONSUMPTIONPOWER_0  LATDbits.LATD10 = 0

// 正常供电
// master bord power select pin
#define MASTERBORDPOWER_OUT (TRISFbits.TRISF7 = 0, /*ANSFbits.ANSF7 = 0,*/  ODCFbits.ODF7 = 0 )
#define MASTERBORDPOWER_1  LATFbits.LATF7 = 1
#define MASTERBORDPOWER_0  LATFbits.LATF7 = 0

// 串口使能
// SET MASTER LOW CONSUMPTION PIN
#define MASTER_LOWPOW_SET_OUT (TRISBbits.TRISB3 = 0, /*ANSBbits.ANSB3 = 0,*/  ODCBbits.ODB3 = 0 )
#define MASTER_LOWPOW_SET_1 LATBbits.LATB3 = 1
#define MASTER_LOWPOW_SET_0 LATBbits.LATB3 = 0
//led
#define LED_OUT (TRISDbits.TRISD11 = 0, ANSDbits.ANSD11 = 0, ODCDbits.ODD11 = 0 )
#define LED_1  LATDbits.LATD11 = 1
#define LED_0  LATDbits.LATD11 = 0

//ADC
//master bord power adc 5V
#define MASTER_ONLINE_ADC  PortVmNum10
#define MASTER_ONLINE_ADC_TRIS       TRISBbits.TRISB10
#define MASTER_ONLINE_ADC_ANS        ANSBbits.ANSB10
#define MASTER_ONLINE_ADC_CHANNEL   ADC_CH0_POS_SAMPLEA_AN10

//consumption power adc 3.6V
#define MASTER_RTC_ADC  PortVmNum9
#define MASTER_RTC_ADC_TRIS         TRISBbits.TRISB9
#define MASTER_RTC_ADC_ANS          ANSBbits.ANSB9
#define MASTER_RTC_ADC_CHANNEL     ADC_CH0_POS_SAMPLEA_AN9

//consumption power adc 3.3V
#define MASTER_3V3_ADC  PortVmNum8
#define MASTER_3V3_ADC_TRIS         TRISBbits.TRISB8
#define MASTER_3V3_ADC_ANS          ANSBbits.ANSB8
#define MASTER_3V3_ADC_CHANNEL     ADC_CH0_POS_SAMPLEA_AN8

//consumption power adc bat
#define MASTER_BAT_ADC  PortVmNum7
#define MASTER_BAT_ADC_TRIS         TRISBbits.TRISB7
#define MASTER_BAT_ADC_ANS          ANSBbits.ANSB7
#define MASTER_BAT_ADC_CHANNEL     ADC_CH0_POS_SAMPLEA_AN7


#ifdef	__cplusplus
}
#endif

#endif	/* BORD_CONFIG_H */

