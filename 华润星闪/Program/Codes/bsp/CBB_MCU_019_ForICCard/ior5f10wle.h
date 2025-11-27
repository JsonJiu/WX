/*-------------------------------------------------------------------------
 *      Declarations of SFR registers, interrupt and callt vector addresses
 *      for RL78 microcontroller R5F10WLE.
 *
 *      This file can be used by both the RL78,
 *      Assembler, ARL78, and the C/C++ compiler, ICCRL78.
 *
 *      This header file is generated from the device file:
 *          DR5F10WLE.DVF
 *          Copyright(C) 2012 Renesas
 *          Format version V3.00, File version V1.00
 *-------------------------------------------------------------------------*/

#ifndef __IOR5F10WLE_H__
#define __IOR5F10WLE_H__

#if !defined(__ARL78__) && !defined(__ICCRL78__)
  #error "IOR5F10WLE.H file for use with RL78 Assembler or Compiler only"
#endif

#if defined(__ARL78__)
  #if __CORE__ != __RL78_1__
    #error "IOR5F10WLE.H file for use with ARL78 option --core rl78_1 only"
  #endif
#endif
#if defined(__ICCRL78__)
  #if __CORE__ != __RL78_1__
    #error "IOR5F10WLE.H file for use with ICCRL78 option --core rl78_1 only"
  #endif
#endif

#ifdef __IAR_SYSTEMS_ICC__

#pragma system_include

#pragma language=save
#pragma language=extended

/*----------------------------------------------
 * SFR bits (default names)
 *--------------------------------------------*/

#ifndef __RL78_BIT_STRUCTURE__
  #define __RL78_BIT_STRUCTURE__
  typedef struct
  {
    unsigned char no0:1;
    unsigned char no1:1;
    unsigned char no2:1;
    unsigned char no3:1;
    unsigned char no4:1;
    unsigned char no5:1;
    unsigned char no6:1;
    unsigned char no7:1;
  } __BITS8;
#endif

#ifndef __RL78_BIT_STRUCTURE2__
  #define __RL78_BIT_STRUCTURE2__
  typedef struct
  {
    unsigned short no0:1;
    unsigned short no1:1;
    unsigned short no2:1;
    unsigned short no3:1;
    unsigned short no4:1;
    unsigned short no5:1;
    unsigned short no6:1;
    unsigned short no7:1;
    unsigned short no8:1;
    unsigned short no9:1;
    unsigned short no10:1;
    unsigned short no11:1;
    unsigned short no12:1;
    unsigned short no13:1;
    unsigned short no14:1;
    unsigned short no15:1;
  } __BITS16;
#endif

/*----------------------------------------------
 *       SFR declarations
 *--------------------------------------------*/

__saddr __no_init volatile union { unsigned char P0; __BITS8 P0_bit; } @ 0xFFF00;
__saddr __no_init volatile union { unsigned char P1; __BITS8 P1_bit; } @ 0xFFF01;
__saddr __no_init volatile union { unsigned char P2; __BITS8 P2_bit; } @ 0xFFF02;
__saddr __no_init volatile union { unsigned char P3; __BITS8 P3_bit; } @ 0xFFF03;
__saddr __no_init volatile union { unsigned char P4; __BITS8 P4_bit; } @ 0xFFF04;
__saddr __no_init volatile union { unsigned char P5; __BITS8 P5_bit; } @ 0xFFF05;
__saddr __no_init volatile union { unsigned char P6; __BITS8 P6_bit; } @ 0xFFF06;
__saddr __no_init volatile union { unsigned char P7; __BITS8 P7_bit; } @ 0xFFF07;
__saddr __no_init volatile union { unsigned char P12; __BITS8 P12_bit; } @ 0xFFF0C;
__saddr __no_init volatile union { unsigned char P13; __BITS8 P13_bit; } @ 0xFFF0D;
__saddr __no_bit_access __no_init volatile union { unsigned short SDR00; __BITS16 SDR00_bit; struct { union { unsigned char SIO00; __BITS8 SIO00_bit; unsigned char TXD0; __BITS8 TXD0_bit; }; }; } @ 0xFFF10;
__saddr __no_bit_access __no_init volatile union { unsigned short SDR01; __BITS16 SDR01_bit; struct { union { unsigned char RXD0; __BITS8 RXD0_bit; }; }; } @ 0xFFF12;
__saddr __no_bit_access __no_init volatile union { unsigned short TDR00; __BITS16 TDR00_bit; } @ 0xFFF18;
__saddr __no_bit_access __no_init volatile union { unsigned short TDR01; __BITS16 TDR01_bit; struct { union { unsigned char TDR01L; __BITS8 TDR01L_bit; }; union { unsigned char TDR01H; __BITS8 TDR01H_bit; }; }; } @ 0xFFF1A;
__saddr __no_bit_access __no_init volatile const union { unsigned short ADCR; __BITS16 ADCR_bit; struct { union { unsigned char dummy; __BITS8 dummy_bit; }; union { const unsigned char ADCRH; const __BITS8 ADCRH_bit; }; }; } @ 0xFFF1E;
__sfr __no_init volatile union { unsigned char PM0; __BITS8 PM0_bit; } @ 0xFFF20;
__sfr __no_init volatile union { unsigned char PM1; __BITS8 PM1_bit; } @ 0xFFF21;
__sfr __no_init volatile union { unsigned char PM2; __BITS8 PM2_bit; } @ 0xFFF22;
__sfr __no_init volatile union { unsigned char PM3; __BITS8 PM3_bit; } @ 0xFFF23;
__sfr __no_init volatile union { unsigned char PM4; __BITS8 PM4_bit; } @ 0xFFF24;
__sfr __no_init volatile union { unsigned char PM5; __BITS8 PM5_bit; } @ 0xFFF25;
__sfr __no_init volatile union { unsigned char PM6; __BITS8 PM6_bit; } @ 0xFFF26;
__sfr __no_init volatile union { unsigned char PM7; __BITS8 PM7_bit; } @ 0xFFF27;
__sfr __no_init volatile union { unsigned char PM12; __BITS8 PM12_bit; } @ 0xFFF2C;
__sfr __no_init volatile union { unsigned char ADM0; __BITS8 ADM0_bit; } @ 0xFFF30;
__sfr __no_init volatile union { unsigned char ADS; __BITS8 ADS_bit; } @ 0xFFF31;
__sfr __no_init volatile union { unsigned char ADM1; __BITS8 ADM1_bit; } @ 0xFFF32;
__sfr __no_init volatile union { unsigned char KRCTL; __BITS8 KRCTL_bit; } @ 0xFFF34;
__sfr __no_bit_access __no_init volatile union { unsigned char KRF; __BITS8 KRF_bit; } @ 0xFFF35;
__sfr __no_init volatile union { unsigned char KRM0; __BITS8 KRM0_bit; } @ 0xFFF37;
__sfr __no_init volatile union { unsigned char EGP0; __BITS8 EGP0_bit; } @ 0xFFF38;
__sfr __no_init volatile union { unsigned char EGN0; __BITS8 EGN0_bit; } @ 0xFFF39;
__sfr __no_bit_access __no_init volatile union { unsigned char LCDM0; __BITS8 LCDM0_bit; } @ 0xFFF40;
__sfr __no_init volatile union { unsigned char LCDM1; __BITS8 LCDM1_bit; } @ 0xFFF41;
__sfr __no_bit_access __no_init volatile union { unsigned char LCDC0; __BITS8 LCDC0_bit; } @ 0xFFF42;
__sfr __no_bit_access __no_init volatile union { unsigned char VLCD; __BITS8 VLCD_bit; } @ 0xFFF43;
__sfr __no_bit_access __no_init volatile union { unsigned short SDR02; __BITS16 SDR02_bit; struct { union { unsigned char SIO10; __BITS8 SIO10_bit; unsigned char TXD1; __BITS8 TXD1_bit; }; }; } @ 0xFFF44;
__sfr __no_bit_access __no_init volatile union { unsigned short SDR03; __BITS16 SDR03_bit; struct { union { unsigned char RXD1; __BITS8 RXD1_bit; }; }; } @ 0xFFF46;
__sfr __no_bit_access __no_init volatile union { unsigned short SDR10; __BITS16 SDR10_bit; struct { union { unsigned char TXD2; __BITS8 TXD2_bit; }; }; } @ 0xFFF48;
__sfr __no_bit_access __no_init volatile union { unsigned short SDR11; __BITS16 SDR11_bit; struct { union { unsigned char RXD2; __BITS8 RXD2_bit; }; }; } @ 0xFFF4A;
__sfr __no_init volatile union { unsigned char MLCD; __BITS8 MLCD_bit; } @ 0xFFF4C;
__sfr __no_bit_access __no_init volatile union { unsigned char IICA0; __BITS8 IICA0_bit; } @ 0xFFF50;
__sfr __no_init volatile const union { unsigned char IICS0; __BITS8 IICS0_bit; } @ 0xFFF51;
__sfr __no_init volatile union { unsigned char IICF0; __BITS8 IICF0_bit; } @ 0xFFF52;
__sfr __no_bit_access __no_init volatile union { unsigned short TDR02; __BITS16 TDR02_bit; } @ 0xFFF64;
__sfr __no_bit_access __no_init volatile union { unsigned short TDR03; __BITS16 TDR03_bit; struct { union { unsigned char TDR03L; __BITS8 TDR03L_bit; }; union { unsigned char TDR03H; __BITS8 TDR03H_bit; }; }; } @ 0xFFF66;
__sfr __no_bit_access __no_init volatile union { unsigned short TDR04; __BITS16 TDR04_bit; } @ 0xFFF68;
__sfr __no_bit_access __no_init volatile union { unsigned short TDR05; __BITS16 TDR05_bit; } @ 0xFFF6A;
__sfr __no_bit_access __no_init volatile union { unsigned short TDR06; __BITS16 TDR06_bit; } @ 0xFFF6C;
__sfr __no_bit_access __no_init volatile union { unsigned short TDR07; __BITS16 TDR07_bit; } @ 0xFFF6E;
__sfr __no_bit_access __no_init volatile union { unsigned short ITMC; __BITS16 ITMC_bit; } @ 0xFFF90;
__sfr __no_bit_access __no_init volatile union { unsigned char SEC; __BITS8 SEC_bit; } @ 0xFFF92;
__sfr __no_bit_access __no_init volatile union { unsigned char MIN; __BITS8 MIN_bit; } @ 0xFFF93;
__sfr __no_bit_access __no_init volatile union { unsigned char HOUR; __BITS8 HOUR_bit; } @ 0xFFF94;
__sfr __no_bit_access __no_init volatile union { unsigned char WEEK; __BITS8 WEEK_bit; } @ 0xFFF95;
__sfr __no_bit_access __no_init volatile union { unsigned char DAY; __BITS8 DAY_bit; } @ 0xFFF96;
__sfr __no_bit_access __no_init volatile union { unsigned char MONTH; __BITS8 MONTH_bit; } @ 0xFFF97;
__sfr __no_bit_access __no_init volatile union { unsigned char YEAR; __BITS8 YEAR_bit; } @ 0xFFF98;
__sfr __no_bit_access __no_init volatile union { unsigned char ALARMWM; __BITS8 ALARMWM_bit; } @ 0xFFF9A;
__sfr __no_bit_access __no_init volatile union { unsigned char ALARMWH; __BITS8 ALARMWH_bit; } @ 0xFFF9B;
__sfr __no_bit_access __no_init volatile union { unsigned char ALARMWW; __BITS8 ALARMWW_bit; } @ 0xFFF9C;
__sfr __no_init volatile union { unsigned char RTCC0; __BITS8 RTCC0_bit; } @ 0xFFF9D;
__sfr __no_init volatile union { unsigned char RTCC1; __BITS8 RTCC1_bit; } @ 0xFFF9E;
__sfr __no_bit_access __no_init volatile union { unsigned char CMC; __BITS8 CMC_bit; } @ 0xFFFA0;
__sfr __no_init volatile union { unsigned char CSC; __BITS8 CSC_bit; } @ 0xFFFA1;
__sfr __no_init volatile const union { unsigned char OSTC; __BITS8 OSTC_bit; } @ 0xFFFA2;
__sfr __no_bit_access __no_init volatile union { unsigned char OSTS; __BITS8 OSTS_bit; } @ 0xFFFA3;
__sfr __no_init volatile union { unsigned char CKC; __BITS8 CKC_bit; } @ 0xFFFA4;
__sfr __no_init volatile union { unsigned char CKS0; __BITS8 CKS0_bit; } @ 0xFFFA5;
__sfr __no_init volatile union { unsigned char CKS1; __BITS8 CKS1_bit; } @ 0xFFFA6;
__sfr __no_bit_access __no_init volatile const union { unsigned char RESF; __BITS8 RESF_bit; } @ 0xFFFA8;
__sfr __no_init volatile union { unsigned char LVIM; __BITS8 LVIM_bit; } @ 0xFFFA9;
__sfr __no_init volatile union { unsigned char LVIS; __BITS8 LVIS_bit; } @ 0xFFFAA;
__sfr __no_bit_access __no_init volatile union { unsigned char WDTE; __BITS8 WDTE_bit; } @ 0xFFFAB;
__sfr __no_bit_access __no_init volatile union { unsigned char CRCIN; __BITS8 CRCIN_bit; } @ 0xFFFAC;
__sfr __no_bit_access __no_init volatile union { unsigned char DSA0; __BITS8 DSA0_bit; } @ 0xFFFB0;
__sfr __no_bit_access __no_init volatile union { unsigned char DSA1; __BITS8 DSA1_bit; } @ 0xFFFB1;
__sfr __no_bit_access __no_init volatile union { unsigned short DRA0; __BITS16 DRA0_bit; struct { union { unsigned char DRA0L; __BITS8 DRA0L_bit; }; union { unsigned char DRA0H; __BITS8 DRA0H_bit; }; }; } @ 0xFFFB2;
__sfr __no_bit_access __no_init volatile union { unsigned short DRA1; __BITS16 DRA1_bit; struct { union { unsigned char DRA1L; __BITS8 DRA1L_bit; }; union { unsigned char DRA1H; __BITS8 DRA1H_bit; }; }; } @ 0xFFFB4;
__sfr __no_bit_access __no_init volatile union { unsigned short DBC0; __BITS16 DBC0_bit; struct { union { unsigned char DBC0L; __BITS8 DBC0L_bit; }; union { unsigned char DBC0H; __BITS8 DBC0H_bit; }; }; } @ 0xFFFB6;
__sfr __no_bit_access __no_init volatile union { unsigned short DBC1; __BITS16 DBC1_bit; struct { union { unsigned char DBC1L; __BITS8 DBC1L_bit; }; union { unsigned char DBC1H; __BITS8 DBC1H_bit; }; }; } @ 0xFFFB8;
__sfr __no_init volatile union { unsigned char DMC0; __BITS8 DMC0_bit; } @ 0xFFFBA;
__sfr __no_init volatile union { unsigned char DMC1; __BITS8 DMC1_bit; } @ 0xFFFBB;
__sfr __no_init volatile union { unsigned char DRC0; __BITS8 DRC0_bit; } @ 0xFFFBC;
__sfr __no_init volatile union { unsigned char DRC1; __BITS8 DRC1_bit; } @ 0xFFFBD;
__sfr __no_init volatile union { unsigned short IF2; struct { union { unsigned char IF2L; __BITS8 IF2L_bit; }; union { unsigned char IF2H; __BITS8 IF2H_bit; }; }; } @ 0xFFFD0;
__sfr __no_init volatile union { unsigned short IF3; struct { union { unsigned char IF3L; __BITS8 IF3L_bit; }; }; } @ 0xFFFD2;
__sfr __no_init volatile union { unsigned short MK2; struct { union { unsigned char MK2L; __BITS8 MK2L_bit; }; union { unsigned char MK2H; __BITS8 MK2H_bit; }; }; } @ 0xFFFD4;
__sfr __no_init volatile union { unsigned short MK3; struct { union { unsigned char MK3L; __BITS8 MK3L_bit; }; }; } @ 0xFFFD6;
__sfr __no_init volatile union { unsigned short PR02; struct { union { unsigned char PR02L; __BITS8 PR02L_bit; }; union { unsigned char PR02H; __BITS8 PR02H_bit; }; }; } @ 0xFFFD8;
__sfr __no_init volatile union { unsigned short PR03; struct { union { unsigned char PR03L; __BITS8 PR03L_bit; }; }; } @ 0xFFFDA;
__sfr __no_init volatile union { unsigned short PR12; struct { union { unsigned char PR12L; __BITS8 PR12L_bit; }; union { unsigned char PR12H; __BITS8 PR12H_bit; }; }; } @ 0xFFFDC;
__sfr __no_init volatile union { unsigned short PR13; struct { union { unsigned char PR13L; __BITS8 PR13L_bit; }; }; } @ 0xFFFDE;
__sfr __no_init volatile union { unsigned short IF0; struct { union { unsigned char IF0L; __BITS8 IF0L_bit; }; union { unsigned char IF0H; __BITS8 IF0H_bit; }; }; } @ 0xFFFE0;
__sfr __no_init volatile union { unsigned short IF1; struct { union { unsigned char IF1L; __BITS8 IF1L_bit; }; union { unsigned char IF1H; __BITS8 IF1H_bit; }; }; } @ 0xFFFE2;
__sfr __no_init volatile union { unsigned short MK0; struct { union { unsigned char MK0L; __BITS8 MK0L_bit; }; union { unsigned char MK0H; __BITS8 MK0H_bit; }; }; } @ 0xFFFE4;
__sfr __no_init volatile union { unsigned short MK1; struct { union { unsigned char MK1L; __BITS8 MK1L_bit; }; union { unsigned char MK1H; __BITS8 MK1H_bit; }; }; } @ 0xFFFE6;
__sfr __no_init volatile union { unsigned short PR00; struct { union { unsigned char PR00L; __BITS8 PR00L_bit; }; union { unsigned char PR00H; __BITS8 PR00H_bit; }; }; } @ 0xFFFE8;
__sfr __no_init volatile union { unsigned short PR01; struct { union { unsigned char PR01L; __BITS8 PR01L_bit; }; union { unsigned char PR01H; __BITS8 PR01H_bit; }; }; } @ 0xFFFEA;
__sfr __no_init volatile union { unsigned short PR10; struct { union { unsigned char PR10L; __BITS8 PR10L_bit; }; union { unsigned char PR10H; __BITS8 PR10H_bit; }; }; } @ 0xFFFEC;
__sfr __no_init volatile union { unsigned short PR11; struct { union { unsigned char PR11L; __BITS8 PR11L_bit; }; union { unsigned char PR11H; __BITS8 PR11H_bit; }; }; } @ 0xFFFEE;
__sfr __no_bit_access __no_init volatile union { unsigned short MDAL; __BITS16 MDAL_bit; } @ 0xFFFF0;
__sfr __no_bit_access __no_init volatile union { unsigned short MULA; __BITS16 MULA_bit; } @ 0xFFFF0;
__sfr __no_bit_access __no_init volatile union { unsigned short MDAH; __BITS16 MDAH_bit; } @ 0xFFFF2;
__sfr __no_bit_access __no_init volatile union { unsigned short MULB; __BITS16 MULB_bit; } @ 0xFFFF2;
__sfr __no_bit_access __no_init volatile union { unsigned short MDBH; __BITS16 MDBH_bit; } @ 0xFFFF4;
__sfr __no_bit_access __no_init volatile union { unsigned short MULOH; __BITS16 MULOH_bit; } @ 0xFFFF4;
__sfr __no_bit_access __no_init volatile union { unsigned short MDBL; __BITS16 MDBL_bit; } @ 0xFFFF6;
__sfr __no_bit_access __no_init volatile union { unsigned short MULOL; __BITS16 MULOL_bit; } @ 0xFFFF6;
__sfr __no_init volatile union { unsigned char PMC; __BITS8 PMC_bit; } @ 0xFFFFE;

/*----------------------------------------------
 *       SFR bit declarations
 *--------------------------------------------*/

#define ADCE              ADM0_bit.no0
#define ADCS              ADM0_bit.no7

#define LCDVLM            LCDM1_bit.no0
#define LCDSEL            LCDM1_bit.no3
#define BLON              LCDM1_bit.no4
#define VLCON             LCDM1_bit.no5
#define SCOC              LCDM1_bit.no6
#define LCDON             LCDM1_bit.no7

#define OPTCKE            MLCD_bit.no4
#define COMEXP            MLCD_bit.no6
#define MLCDEN            MLCD_bit.no7

#define SPD0              IICS0_bit.no0
#define STD0              IICS0_bit.no1
#define ACKD0             IICS0_bit.no2
#define TRC0              IICS0_bit.no3
#define COI0              IICS0_bit.no4
#define EXC0              IICS0_bit.no5
#define ALD0              IICS0_bit.no6
#define MSTS0             IICS0_bit.no7

#define IICRSV0           IICF0_bit.no0
#define STCEN0            IICF0_bit.no1
#define IICBSY0           IICF0_bit.no6
#define STCF0             IICF0_bit.no7

#define RCLOE1            RTCC0_bit.no5
#define RCLOSEL           RTCC0_bit.no6
#define RTCE              RTCC0_bit.no7

#define RWAIT             RTCC1_bit.no0
#define RWST              RTCC1_bit.no1
#define RIFG              RTCC1_bit.no3
#define WAFG              RTCC1_bit.no4
#define RITE              RTCC1_bit.no5
#define WALIE             RTCC1_bit.no6
#define WALE              RTCC1_bit.no7

#define HIOSTOP           CSC_bit.no0
#define XTSTOP            CSC_bit.no6
#define MSTOP             CSC_bit.no7

#define SDIV              CKC_bit.no3
#define MCM0              CKC_bit.no4
#define MCS               CKC_bit.no5
#define CSS               CKC_bit.no6
#define CLS               CKC_bit.no7

#define PCLOE0            CKS0_bit.no7

#define PCLOE1            CKS1_bit.no7

#define LVIF              LVIM_bit.no0
#define LVIOMSK           LVIM_bit.no1
#define LVISEN            LVIM_bit.no7

#define LVILV             LVIS_bit.no0
#define LVIMD             LVIS_bit.no7

#define DWAIT0            DMC0_bit.no4
#define DS0               DMC0_bit.no5
#define DRS0              DMC0_bit.no6
#define STG0              DMC0_bit.no7

#define DWAIT1            DMC1_bit.no4
#define DS1               DMC1_bit.no5
#define DRS1              DMC1_bit.no6
#define STG1              DMC1_bit.no7

#define DST0              DRC0_bit.no0
#define DEN0              DRC0_bit.no7

#define DST1              DRC1_bit.no0
#define DEN1              DRC1_bit.no7

#define TKBIF20           IF2L_bit.no0
#define TMIF04            IF2L_bit.no1
#define TMIF05            IF2L_bit.no2
#define PIF6              IF2L_bit.no3
#define PIF7              IF2L_bit.no4
#define LCDIF0            IF2L_bit.no5
#define CMPIF0            IF2L_bit.no6
#define CMPIF1            IF2L_bit.no7

#define TMIF06            IF2H_bit.no0
#define TMIF07            IF2H_bit.no1
#define MDIF              IF2H_bit.no5
#define FLIF              IF2H_bit.no7

#define DMAIF2            IF3L_bit.no0
#define DMAIF3            IF3L_bit.no1

#define TKBMK20           MK2L_bit.no0
#define TMMK04            MK2L_bit.no1
#define TMMK05            MK2L_bit.no2
#define PMK6              MK2L_bit.no3
#define PMK7              MK2L_bit.no4
#define LCDMK0            MK2L_bit.no5
#define CMPMK0            MK2L_bit.no6
#define CMPMK1            MK2L_bit.no7

#define TMMK06            MK2H_bit.no0
#define TMMK07            MK2H_bit.no1
#define MDMK              MK2H_bit.no5
#define FLMK              MK2H_bit.no7

#define DMAMK2            MK3L_bit.no0
#define DMAMK3            MK3L_bit.no1

#define TKBPR020          PR02L_bit.no0
#define TMPR004           PR02L_bit.no1
#define TMPR005           PR02L_bit.no2
#define PPR06             PR02L_bit.no3
#define PPR07             PR02L_bit.no4
#define LCDPR00           PR02L_bit.no5
#define CMPPR00           PR02L_bit.no6
#define CMPPR01           PR02L_bit.no7

#define TMPR006           PR02H_bit.no0
#define TMPR007           PR02H_bit.no1
#define MDPR0             PR02H_bit.no5
#define FLPR0             PR02H_bit.no7

#define DMAPR02           PR03L_bit.no0
#define DMAPR03           PR03L_bit.no1

#define TKBPR120          PR12L_bit.no0
#define TMPR104           PR12L_bit.no1
#define TMPR105           PR12L_bit.no2
#define PPR16             PR12L_bit.no3
#define PPR17             PR12L_bit.no4
#define LCDPR10           PR12L_bit.no5
#define CMPPR10           PR12L_bit.no6
#define CMPPR11           PR12L_bit.no7

#define TMPR106           PR12H_bit.no0
#define TMPR107           PR12H_bit.no1
#define MDPR1             PR12H_bit.no5
#define FLPR1             PR12H_bit.no7

#define DMAPR12           PR13L_bit.no0
#define DMAPR13           PR13L_bit.no1

#define WDTIIF            IF0L_bit.no0
#define LVIIF             IF0L_bit.no1
#define PIF0              IF0L_bit.no2
#define PIF1              IF0L_bit.no3
#define PIF2              IF0L_bit.no4
#define PIF3              IF0L_bit.no5
#define PIF4              IF0L_bit.no6
#define PIF5              IF0L_bit.no7

#define STIF2             IF0H_bit.no0
#define SRIF2             IF0H_bit.no1
#define SREIF2            IF0H_bit.no2
#define DMAIF0            IF0H_bit.no3
#define DMAIF1            IF0H_bit.no4
#define CSIIF00           IF0H_bit.no5
#define IICIF00           IF0H_bit.no5
#define STIF0             IF0H_bit.no5
#define TMIF00            IF0H_bit.no6
#define SRIF0             IF0H_bit.no7

#define SREIF0            IF1L_bit.no0
#define TMIF01H           IF1L_bit.no0
#define CSIIF10           IF1L_bit.no1
#define IICIF10           IF1L_bit.no1
#define STIF1             IF1L_bit.no1
#define SRIF1             IF1L_bit.no2
#define SREIF1            IF1L_bit.no3
#define TMIF03H           IF1L_bit.no3
#define IICAIF0           IF1L_bit.no4
#define RTITIF            IF1L_bit.no5
#define TMIF01            IF1L_bit.no7

#define TMIF02            IF1H_bit.no0
#define TMIF03            IF1H_bit.no1
#define ADIF              IF1H_bit.no2
#define RTCIF             IF1H_bit.no3
#define TMKAIF            IF1H_bit.no4
#define KRIF              IF1H_bit.no5

#define WDTIMK            MK0L_bit.no0
#define LVIMK             MK0L_bit.no1
#define PMK0              MK0L_bit.no2
#define PMK1              MK0L_bit.no3
#define PMK2              MK0L_bit.no4
#define PMK3              MK0L_bit.no5
#define PMK4              MK0L_bit.no6
#define PMK5              MK0L_bit.no7

#define STMK2             MK0H_bit.no0
#define SRMK2             MK0H_bit.no1
#define SREMK2            MK0H_bit.no2
#define DMAMK0            MK0H_bit.no3
#define DMAMK1            MK0H_bit.no4
#define CSIMK00           MK0H_bit.no5
#define IICMK00           MK0H_bit.no5
#define STMK0             MK0H_bit.no5
#define TMMK00            MK0H_bit.no6
#define SRMK0             MK0H_bit.no7

#define SREMK0            MK1L_bit.no0
#define TMMK01H           MK1L_bit.no0
#define CSIMK10           MK1L_bit.no1
#define IICMK10           MK1L_bit.no1
#define STMK1             MK1L_bit.no1
#define SRMK1             MK1L_bit.no2
#define SREMK1            MK1L_bit.no3
#define TMMK03H           MK1L_bit.no3
#define IICAMK0           MK1L_bit.no4
#define RTITMK            MK1L_bit.no5
#define TMMK01            MK1L_bit.no7

#define TMMK02            MK1H_bit.no0
#define TMMK03            MK1H_bit.no1
#define ADMK              MK1H_bit.no2
#define RTCMK             MK1H_bit.no3
#define TMKAMK            MK1H_bit.no4
#define KRMK              MK1H_bit.no5

#define WDTIPR0           PR00L_bit.no0
#define LVIPR0            PR00L_bit.no1
#define PPR00             PR00L_bit.no2
#define PPR01             PR00L_bit.no3
#define PPR02             PR00L_bit.no4
#define PPR03             PR00L_bit.no5
#define PPR04             PR00L_bit.no6
#define PPR05             PR00L_bit.no7

#define STPR02            PR00H_bit.no0
#define SRPR02            PR00H_bit.no1
#define SREPR02           PR00H_bit.no2
#define DMAPR00           PR00H_bit.no3
#define DMAPR01           PR00H_bit.no4
#define CSIPR000          PR00H_bit.no5
#define IICPR000          PR00H_bit.no5
#define STPR00            PR00H_bit.no5
#define TMPR000           PR00H_bit.no6
#define SRPR00            PR00H_bit.no7

#define SREPR00           PR01L_bit.no0
#define TMPR001H          PR01L_bit.no0
#define CSIPR010          PR01L_bit.no1
#define IICPR010          PR01L_bit.no1
#define STPR01            PR01L_bit.no1
#define SRPR01            PR01L_bit.no2
#define SREPR01           PR01L_bit.no3
#define TMPR003H          PR01L_bit.no3
#define IICAPR00          PR01L_bit.no4
#define RTITPR0           PR01L_bit.no5
#define TMPR001           PR01L_bit.no7

#define TMPR002           PR01H_bit.no0
#define TMPR003           PR01H_bit.no1
#define ADPR0             PR01H_bit.no2
#define RTCPR0            PR01H_bit.no3
#define TMKAPR0           PR01H_bit.no4
#define KRPR0             PR01H_bit.no5

#define WDTIPR1           PR10L_bit.no0
#define LVIPR1            PR10L_bit.no1
#define PPR10             PR10L_bit.no2
#define PPR11             PR10L_bit.no3
#define PPR12             PR10L_bit.no4
#define PPR13             PR10L_bit.no5
#define PPR14             PR10L_bit.no6
#define PPR15             PR10L_bit.no7

#define STPR12            PR10H_bit.no0
#define SRPR12            PR10H_bit.no1
#define SREPR12           PR10H_bit.no2
#define DMAPR10           PR10H_bit.no3
#define DMAPR11           PR10H_bit.no4
#define CSIPR100          PR10H_bit.no5
#define IICPR100          PR10H_bit.no5
#define STPR10            PR10H_bit.no5
#define TMPR100           PR10H_bit.no6
#define SRPR10            PR10H_bit.no7

#define SREPR10           PR11L_bit.no0
#define TMPR101H          PR11L_bit.no0
#define CSIPR110          PR11L_bit.no1
#define IICPR110          PR11L_bit.no1
#define STPR11            PR11L_bit.no1
#define SRPR11            PR11L_bit.no2
#define SREPR11           PR11L_bit.no3
#define TMPR103H          PR11L_bit.no3
#define IICAPR10          PR11L_bit.no4
#define RTITPR1           PR11L_bit.no5
#define TMPR101           PR11L_bit.no7

#define TMPR102           PR11H_bit.no0
#define TMPR103           PR11H_bit.no1
#define ADPR1             PR11H_bit.no2
#define RTCPR1            PR11H_bit.no3
#define TMKAPR1           PR11H_bit.no4
#define KRPR1             PR11H_bit.no5

#define MAA               PMC_bit.no0

#pragma language=restore

#endif /* __IAR_SYSTEMS_ICC__ */

#ifdef __IAR_SYSTEMS_ASM__

/*----------------------------------------------
 *       SFR declarations
 *--------------------------------------------*/

P0         DEFINE  0xFFF00
P1         DEFINE  0xFFF01
P2         DEFINE  0xFFF02
P3         DEFINE  0xFFF03
P4         DEFINE  0xFFF04
P5         DEFINE  0xFFF05
P6         DEFINE  0xFFF06
P7         DEFINE  0xFFF07
P12        DEFINE  0xFFF0C
P13        DEFINE  0xFFF0D
SDR00      DEFINE  0xFFF10
SIO00      DEFINE  0xFFF10
TXD0       DEFINE  0xFFF10
SDR01      DEFINE  0xFFF12
RXD0       DEFINE  0xFFF12
TDR00      DEFINE  0xFFF18
TDR01      DEFINE  0xFFF1A
TDR01L     DEFINE  0xFFF1A
TDR01H     DEFINE  0xFFF1B
ADCR       DEFINE  0xFFF1E
ADCRH      DEFINE  0xFFF1F
PM0        DEFINE  0xFFF20
PM1        DEFINE  0xFFF21
PM2        DEFINE  0xFFF22
PM3        DEFINE  0xFFF23
PM4        DEFINE  0xFFF24
PM5        DEFINE  0xFFF25
PM6        DEFINE  0xFFF26
PM7        DEFINE  0xFFF27
PM12       DEFINE  0xFFF2C
ADM0       DEFINE  0xFFF30
ADS        DEFINE  0xFFF31
ADM1       DEFINE  0xFFF32
KRCTL      DEFINE  0xFFF34
KRF        DEFINE  0xFFF35
KRM0       DEFINE  0xFFF37
EGP0       DEFINE  0xFFF38
EGN0       DEFINE  0xFFF39
LCDM0      DEFINE  0xFFF40
LCDM1      DEFINE  0xFFF41
LCDC0      DEFINE  0xFFF42
VLCD       DEFINE  0xFFF43
SDR02      DEFINE  0xFFF44
SIO10      DEFINE  0xFFF44
TXD1       DEFINE  0xFFF44
SDR03      DEFINE  0xFFF46
RXD1       DEFINE  0xFFF46
SDR10      DEFINE  0xFFF48
TXD2       DEFINE  0xFFF48
SDR11      DEFINE  0xFFF4A
RXD2       DEFINE  0xFFF4A
MLCD       DEFINE  0xFFF4C
IICA0      DEFINE  0xFFF50
IICS0      DEFINE  0xFFF51
IICF0      DEFINE  0xFFF52
TDR02      DEFINE  0xFFF64
TDR03      DEFINE  0xFFF66
TDR03L     DEFINE  0xFFF66
TDR03H     DEFINE  0xFFF67
TDR04      DEFINE  0xFFF68
TDR05      DEFINE  0xFFF6A
TDR06      DEFINE  0xFFF6C
TDR07      DEFINE  0xFFF6E
ITMC       DEFINE  0xFFF90
SEC        DEFINE  0xFFF92
MIN        DEFINE  0xFFF93
HOUR       DEFINE  0xFFF94
WEEK       DEFINE  0xFFF95
DAY        DEFINE  0xFFF96
MONTH      DEFINE  0xFFF97
YEAR       DEFINE  0xFFF98
ALARMWM    DEFINE  0xFFF9A
ALARMWH    DEFINE  0xFFF9B
ALARMWW    DEFINE  0xFFF9C
RTCC0      DEFINE  0xFFF9D
RTCC1      DEFINE  0xFFF9E
CMC        DEFINE  0xFFFA0
CSC        DEFINE  0xFFFA1
OSTC       DEFINE  0xFFFA2
OSTS       DEFINE  0xFFFA3
CKC        DEFINE  0xFFFA4
CKS0       DEFINE  0xFFFA5
CKS1       DEFINE  0xFFFA6
RESF       DEFINE  0xFFFA8
LVIM       DEFINE  0xFFFA9
LVIS       DEFINE  0xFFFAA
WDTE       DEFINE  0xFFFAB
CRCIN      DEFINE  0xFFFAC
DSA0       DEFINE  0xFFFB0
DSA1       DEFINE  0xFFFB1
DRA0       DEFINE  0xFFFB2
DRA0L      DEFINE  0xFFFB2
DRA0H      DEFINE  0xFFFB3
DRA1       DEFINE  0xFFFB4
DRA1L      DEFINE  0xFFFB4
DRA1H      DEFINE  0xFFFB5
DBC0       DEFINE  0xFFFB6
DBC0L      DEFINE  0xFFFB6
DBC0H      DEFINE  0xFFFB7
DBC1       DEFINE  0xFFFB8
DBC1L      DEFINE  0xFFFB8
DBC1H      DEFINE  0xFFFB9
DMC0       DEFINE  0xFFFBA
DMC1       DEFINE  0xFFFBB
DRC0       DEFINE  0xFFFBC
DRC1       DEFINE  0xFFFBD
IF2        DEFINE  0xFFFD0
IF2L       DEFINE  0xFFFD0
IF2H       DEFINE  0xFFFD1
IF3        DEFINE  0xFFFD2
IF3L       DEFINE  0xFFFD2
MK2        DEFINE  0xFFFD4
MK2L       DEFINE  0xFFFD4
MK2H       DEFINE  0xFFFD5
MK3        DEFINE  0xFFFD6
MK3L       DEFINE  0xFFFD6
PR02       DEFINE  0xFFFD8
PR02L      DEFINE  0xFFFD8
PR02H      DEFINE  0xFFFD9
PR03       DEFINE  0xFFFDA
PR03L      DEFINE  0xFFFDA
PR12       DEFINE  0xFFFDC
PR12L      DEFINE  0xFFFDC
PR12H      DEFINE  0xFFFDD
PR13       DEFINE  0xFFFDE
PR13L      DEFINE  0xFFFDE
IF0        DEFINE  0xFFFE0
IF0L       DEFINE  0xFFFE0
IF0H       DEFINE  0xFFFE1
IF1        DEFINE  0xFFFE2
IF1L       DEFINE  0xFFFE2
IF1H       DEFINE  0xFFFE3
MK0        DEFINE  0xFFFE4
MK0L       DEFINE  0xFFFE4
MK0H       DEFINE  0xFFFE5
MK1        DEFINE  0xFFFE6
MK1L       DEFINE  0xFFFE6
MK1H       DEFINE  0xFFFE7
PR00       DEFINE  0xFFFE8
PR00L      DEFINE  0xFFFE8
PR00H      DEFINE  0xFFFE9
PR01       DEFINE  0xFFFEA
PR01L      DEFINE  0xFFFEA
PR01H      DEFINE  0xFFFEB
PR10       DEFINE  0xFFFEC
PR10L      DEFINE  0xFFFEC
PR10H      DEFINE  0xFFFED
PR11       DEFINE  0xFFFEE
PR11L      DEFINE  0xFFFEE
PR11H      DEFINE  0xFFFEF
MDAL       DEFINE  0xFFFF0
MULA       DEFINE  0xFFFF0
MDAH       DEFINE  0xFFFF2
MULB       DEFINE  0xFFFF2
MDBH       DEFINE  0xFFFF4
MULOH      DEFINE  0xFFFF4
MDBL       DEFINE  0xFFFF6
MULOL      DEFINE  0xFFFF6
PMC        DEFINE  0xFFFFE

#endif /* __IAR_SYSTEMS_ASM__ */

/*----------------------------------------------
 *       Interrupt vector addresses
 *--------------------------------------------*/

#define RST_vect                 (0x00)
#define INTDBG_vect              (0x02)
#define INTWDTI_vect             (0x04)
#define INTLVI_vect              (0x06)
#define INTP0_vect               (0x08)
#define INTP1_vect               (0x0A)
#define INTP2_vect               (0x0C)
#define INTP3_vect               (0x0E)
#define INTP4_vect               (0x10)
#define INTP5_vect               (0x12)
#define INTST2_vect              (0x14)
#define INTSR2_vect              (0x16)
#define INTSRE2_vect             (0x18)
#define INTDMA0_vect             (0x1A)
#define INTDMA1_vect             (0x1C)
#define INTCSI00_vect            (0x1E)
#define INTIIC00_vect            (0x1E)
#define INTST0_vect              (0x1E)
#define INTTM00_vect             (0x20)
#define INTSR0_vect              (0x22)
#define INTSRE0_vect             (0x24)
#define INTTM01H_vect            (0x24)
#define INTCSI10_vect            (0x26)
#define INTIIC10_vect            (0x26)
#define INTST1_vect              (0x26)
#define INTSR1_vect              (0x28)
#define INTSRE1_vect             (0x2A)
#define INTTM03H_vect            (0x2A)
#define INTIICA0_vect            (0x2C)
#define INTRTIT_vect             (0x2E)
#define INTTM01_vect             (0x32)
#define INTTM02_vect             (0x34)
#define INTTM03_vect             (0x36)
#define INTAD_vect               (0x38)
#define INTRTC_vect              (0x3A)
#define INTIT_vect               (0x3C)
#define INTKR_vect               (0x3E)
#define INTTKB20_vect            (0x44)
#define INTTM04_vect             (0x46)
#define INTTM05_vect             (0x48)
#define INTP6_vect               (0x4A)
#define INTP7_vect               (0x4C)
#define INTLCD0_vect             (0x4E)
#define INTCMP0_vect             (0x50)
#define INTCMP1_vect             (0x52)
#define INTTM06_vect             (0x54)
#define INTTM07_vect             (0x56)
#define INTSRE3_vect             (0x5C)
#define INTMD_vect               (0x5E)
#define INTFL_vect               (0x62)
#define INTDMA2_vect             (0x64)
#define INTDMA3_vect             (0x66)
#define BRK_I_vect               (0x7E)

/*----------------------------------------------
 *       Callt vector addresses
 *--------------------------------------------*/

#define CALLT_80_vect            (0x80)
#define CALLT_82_vect            (0x82)
#define CALLT_84_vect            (0x84)
#define CALLT_86_vect            (0x86)
#define CALLT_88_vect            (0x88)
#define CALLT_8A_vect            (0x8A)
#define CALLT_8C_vect            (0x8C)
#define CALLT_8E_vect            (0x8E)
#define CALLT_90_vect            (0x90)
#define CALLT_92_vect            (0x92)
#define CALLT_94_vect            (0x94)
#define CALLT_96_vect            (0x96)
#define CALLT_98_vect            (0x98)
#define CALLT_9A_vect            (0x9A)
#define CALLT_9C_vect            (0x9C)
#define CALLT_9E_vect            (0x9E)
#define CALLT_A0_vect            (0xA0)
#define CALLT_A2_vect            (0xA2)
#define CALLT_A4_vect            (0xA4)
#define CALLT_A6_vect            (0xA6)
#define CALLT_A8_vect            (0xA8)
#define CALLT_AA_vect            (0xAA)
#define CALLT_AC_vect            (0xAC)
#define CALLT_AE_vect            (0xAE)
#define CALLT_B0_vect            (0xB0)
#define CALLT_B2_vect            (0xB2)
#define CALLT_B4_vect            (0xB4)
#define CALLT_B6_vect            (0xB6)
#define CALLT_B8_vect            (0xB8)
#define CALLT_BA_vect            (0xBA)
#define CALLT_BC_vect            (0xBC)
#define CALLT_BE_vect            (0xBE)

#endif /* __IOR5F10WLE_H__ */
