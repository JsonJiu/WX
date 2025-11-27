#ifndef __LIB_RS004_H__
#define __LIB_RS004_H__
#include "lib_public.h"


#define CLR_BIT(d,bit)      ((d) &= (~(bit)))
#define SET_BIT(d,bit)      ((d) |= (bit))

typedef	enum PortNumE   //表示串口,定时器,ADC通道....端口号.
{
    PortVmNum0,
    PortVmNum1,
    PortVmNum2,
    PortVmNum3,
    PortVmNum4,
    PortVmNum5,
    PortVmNum6,
    PortVmNum7,
    PortVmNum8,
    PortVmNum9,
    PortVmNum10,
    PortVmNum11,
    PortVmNum12,
    PortVmNum13,
    PortVmNum14,
    PortVmNum15,
    PortVmNum16,
    PortVmNum17,
    PortVmNum18,
    PortVmNum19,
    PortVmNum20,
    PortVmNum21,
    PortVmNum22,
    PortVmNum23,
    PortVmNum24,
    PortVmNum25,
    PortVmNum26,
    PortVmNum27,
    PortVmNum28,
    PortVmNum29,
}PortNumET;

typedef	enum TimeSpE
{
    TimeSpMs1,
    TimeSpMs10,
    TimeSpMs20,
    TimeSpMs50,
    TimeSpMs100,
    TimeSpMs500,
    TimeSpMs1000,
    TimeSpMs2000,
}TimeSpET;


/*Pxx,PUxx,PIMxx,POMxx, Reset to 0*/
/*PMxx,PMCxx,PFSEGxx,   Reset to 1*/
#define PORT0OutputConfig(Output)    { PM0 &= ~Output;}/*Configures Port0 as output*/
#define PORT0InputConfig(Input)      { PM0 |= Input;} /*Configures Port0 as input*/
#define PORT0SetBit(Bit)             { P0 |= Bit; }/*Set Port0 bit*/
#define PORT0ClearBit(Bit)           { P0 &= ~Bit;}/*Clears Port0 bit*/
#define PORT0ReadBit(Bit)             (P0 & Bit)/*Reads given Port0 bit value*/
#define PORT0PullUpBit(Bit)          { PU0 |= Bit; }
#define PIM0SetBit(Bit)              { PIM0 |= Bit; }
#define POM0SetBit(Bit)              { POM0 |= Bit; }

#define PORT1OutputConfig(Output)    { PM1 &= ~Output;}/*Configures Port1 as output*/
#define PORT1InputConfig(Input)      { PM1 |= Input;} /*Configures Port1 as input*/
#define PORT1SetBit(Bit)             { P1 |= Bit; }/*Set Port1 bit*/
#define PORT1ClearBit(Bit)           { P1 &= ~Bit;}/*Clears Port1 bit*/
#define PORT1ReadBit(Bit)             (P1 & Bit)/*Reads given Port1 bit value*/
#define PORT1PullUpBit(Bit)          { PU1 |= Bit; }
#define PIM1SetBit(Bit)              { PIM1 |= Bit; }
#define POM1SetBit(Bit)              { POM1 |= Bit; }
#define PMC1ClearBit(Bit)            { PMC1 &= ~Bit;}

#define PORT2OutputConfig(Output)    { PM2 &= ~Output;}/*Configures Port2 as output*/
#define PORT2InputConfig(Input)      { PM2 |= Input;} /*Configures Port2 as input*/
#define PORT2SetBit(Bit)             { P2 |= Bit; }/*Set Port2 bit*/
#define PORT2ClearBit(Bit)           { P2 &= ~Bit;}/*Clears Port2 bit*/
#define PORT2ReadBit(Bit)             (P2 & Bit)/*Reads given Port2 bit value*/
#define PORT2PullUpBit(Bit)          { PU2 |= Bit; }
#define PMC2ClearBit(Bit)            { PMC2 &= ~Bit;}

#define PORT3OutputConfig(Output)    { PM3 &= ~Output;}/*Configures Port3 as output*/
#define PORT3InputConfig(Input)      { PM3 |= Input;} /*Configures Port3 as input*/
#define PORT3SetBit(Bit)             { P3 |= Bit; }/*Set Port3 bit*/
#define PORT3ClearBit(Bit)           { P3 &= ~Bit;}/*Clears Port3 bit*/
#define PORT3ReadBit(Bit)             (P3 & Bit)/*Reads given Port3 bit value*/
#define PORT3PullUpBit(Bit)          { PU3 |= Bit; }
#define PIM3SetBit(Bit)              { PIM3 |= Bit; }
#define POM3SetBit(Bit)              { POM3 |= Bit; }

#define PORT4OutputConfig(Output)    { PM4 &= ~Output;}/*Configures Port4 as output*/
#define PORT4InputConfig(Input)      { PM4 |= Input;} /*Configures Port4 as input*/
#define PORT4SetBit(Bit)             { P4 |= Bit; }/*Set Port4 bit*/
#define PORT4ClearBit(Bit)           { P4 &= ~Bit;}/*Clears Port4 bit*/
#define PORT4ReadBit(Bit)             (P4 & Bit)/*Reads given Port4 bit value*/
#define PORT4PullUpBit(Bit)          { PU4 |= Bit; }
#define PIM4SetBit(Bit)              { PIM4 |= Bit; }
#define POM4SetBit(Bit)              { POM4 |= Bit; }
#define PMC4ClearBit(Bit)            { PMC4 &= ~Bit;}

#define PORT5OutputConfig(Output)    { PM5 &= ~Output;}/*Configures Port5 as output*/
#define PORT5InputConfig(Input)      { PM5 |= Input;} /*Configures Port5 as input*/
#define PORT5SetBit(Bit)             { P5 |= Bit; }/*Set Port5 bit*/
#define PORT5ClearBit(Bit)           { P5 &= ~Bit;}/*Clears Port5 bit*/
#define PORT5ReadBit(Bit)             (P5 & Bit)/*Reads given Port5 bit value*/
#define PORT5PullUpBit(Bit)          { PU5 |= Bit; }
#define PIM5SetBit(Bit)              { PIM5 |= Bit; }
#define POM5SetBit(Bit)              { POM5 |= Bit; }

#define PORT6OutputConfig(Output)    { PM6 &= ~Output;}/*Configures Port6 as output*/
#define PORT6InputConfig(Input)      { PM6 |= Input;} /*Configures Port6 as input*/
#define PORT6SetBit(Bit)             { P6 |= Bit; }/*Set Port6 bit*/
#define PORT6ClearBit(Bit)           { P6 &= ~Bit;}/*Clears Port6 bit*/
#define PORT6ReadBit(Bit)             (P6 & Bit)/*Reads given Port6 bit value*/


#define PORT7OutputConfig(Output)    { PM7 &= ~Output;}/*Configures Port7 as output*/
#define PORT7InputConfig(Input)      { PM7 |= Input;} /*Configures Port7 as input*/
#define PORT7SetBit(Bit)             { P7 |= Bit; }/*Set Port7 bit*/
#define PORT7ClearBit(Bit)           { P7 &= ~Bit;}/*Clears Port7 bit*/
#define PORT7ReadBit(Bit)             (P7 & Bit)/*Reads given Port7 bit value*/
#define PORT7PullUpBit(Bit)          { PU7 |= Bit; }

#define PORT12OutputConfig(Output)    { PM12 &= ~Output;}/*Configures Port12 as output*/
#define PORT12InputConfig(Input)      { PM12 |= Input;} /*Configures Port12 as input*/
#define PORT12SetBit(Bit)             { P12 |= Bit; }/*Set Port12 bit*/
#define PORT12ClearBit(Bit)           { P12 &= ~Bit;}/*Clears Port12 bit*/
#define PORT12ReadBit(Bit)             (P12 & Bit)/*Reads given Port12 bit value*/
#define PORT12PullUpBit(Bit)          { PU12 |= Bit; }

#define PORT13OutputConfig(Output)    { PM13 &= ~Output;}/*Configures Port13 as output*/
#define PORT13InputConfig(Input)      { PM13 |= Input;} /*Configures Port13 as input*/
#define PORT13SetBit(Bit)             { P13 |= Bit; }/*Set Port13 bit*/
#define PORT13ClearBit(Bit)           { P13 &= ~Bit;}/*Clears Port13 bit*/
#define PORT13ReadBit(Bit)             (P13 & Bit)/*Reads given Port13 bit value*/
#define PORT13PullUpBit(Bit)          { PU13 |= Bit; }
#define POM13SetBit(Bit)              { POM13 |= Bit; }
//LCD port function registers 0 to 6 (PFSEG0 to PFSEG6)
#define PFSEG0ClearBit(Bit)           { PFSEG0 &= ~Bit;}/*Clears bit, Used the Pmn pin as port*/
#define PFSEG1ClearBit(Bit)           { PFSEG1 &= ~Bit;}
#define PFSEG2ClearBit(Bit)           { PFSEG2 &= ~Bit;}
#define PFSEG3ClearBit(Bit)           { PFSEG3 &= ~Bit;}
#define PFSEG4ClearBit(Bit)           { PFSEG4 &= ~Bit;}
#define PFSEG5ClearBit(Bit)           { PFSEG5 &= ~Bit;}
#define PFSEG6ClearBit(Bit)           { PFSEG6 &= ~Bit;}

#define RISING_EDGE_INT   0
#define FALLING_EDGE_INT  1
#define EnableINT0     {PIF0 = 0U; PMK0 = 0U;}
#define DisableINT0    {PMK0 = 1U; PIF0 = 0U;}
#define EnableINT1     {PIF1 = 0U; PMK1 = 0U;}
#define DisableINT1    {PMK1 = 1U; PIF1 = 0U;}
#define EnableINT2     {PIF2 = 0U; PMK2 = 0U;}
#define DisableINT2    {PMK2 = 1U; PIF2 = 0U;}
#define EnableINT3     {PIF3 = 0U; PMK3 = 0U;}
#define DisableINT3    {PMK3 = 1U; PIF3 = 0U;}
#define EnableINT4     {PIF4 = 0U; PMK4 = 0U;}
#define DisableINT4    {PMK4 = 1U; PIF4 = 0U;}
#define EnableINT5     {PIF5 = 0U; PMK5 = 0U;}
#define DisableINT5    {PMK5 = 1U; PIF5 = 0U;}
#define EnableINT6     {PIF6 = 0U; PMK6 = 0U;}
#define DisableINT6    {PMK6 = 1U; PIF6 = 0U;}
#define EnableINT7     {PIF7 = 0U; PMK7 = 0U;}
#define DisableINT7    {PMK7 = 1U; PIF7 = 0U;}
void INT0Config(unsigned char Config, IRQServerFT Fuc);
void INT1Config(unsigned char Config, IRQServerFT Fuc);
void INT2Config(unsigned char Config, IRQServerFT Fuc);
void INT3Config(unsigned char Config, IRQServerFT Fuc);
void INT4Config(unsigned char Config, IRQServerFT Fuc);
void INT5Config(unsigned char Config, IRQServerFT Fuc);
void INT6Config(unsigned char Config, IRQServerFT Fuc);
void INT7Config(unsigned char Config, IRQServerFT Fuc);



/*****************************************************************************/
/*
    Serial Clock Select Register m (SPSm)
*/
/* Operating mode and clear mode selection (PRSm03 - PRSm00) */
#define _0000_SAU_CK00_FCLK_0            (0x0000U) /* ck00 - fCLK */
#define _0001_SAU_CK00_FCLK_1            (0x0001U) /* ck00 - fCLK/2^1 */
#define _0002_SAU_CK00_FCLK_2            (0x0002U) /* ck00 - fCLK/2^2 */
#define _0003_SAU_CK00_FCLK_3            (0x0003U) /* ck00 - fCLK/2^3 */
#define _0004_SAU_CK00_FCLK_4            (0x0004U) /* ck00 - fCLK/2^4 */
#define _0005_SAU_CK00_FCLK_5            (0x0005U) /* ck00 - fCLK/2^5 */
#define _0006_SAU_CK00_FCLK_6            (0x0006U) /* ck00 - fCLK/2^6 */
#define _0007_SAU_CK00_FCLK_7            (0x0007U) /* ck00 - fCLK/2^7 */
#define _0008_SAU_CK00_FCLK_8            (0x0008U) /* ck00 - fCLK/2^8 */
#define _0009_SAU_CK00_FCLK_9            (0x0009U) /* ck00 - fCLK/2^9 */
#define _000A_SAU_CK00_FCLK_10           (0x000AU) /* ck00 - fCLK/2^10 */
#define _000B_SAU_CK00_FCLK_11           (0x000BU) /* ck00 - fCLK/2^11 */
#define _000C_SAU_CK00_FCLK_12           (0x000CU) /* ck00 - fCLK/2^12 */
#define _000D_SAU_CK00_FCLK_13           (0x000DU) /* ck00 - fCLK/2^13 */
#define _000E_SAU_CK00_FCLK_14           (0x000EU) /* ck00 - fCLK/2^14 */
#define _000F_SAU_CK00_FCLK_15           (0x000FU) /* ck00 - fCLK/2^15 */
/* Operating mode and clear mode selection (PRSm13 - PRSm10) */
#define _0000_SAU_CK01_FCLK_0            (0x0000U) /* ck01 - fCLK */
#define _0010_SAU_CK01_FCLK_1            (0x0010U) /* ck01 - fCLK/2^1 */
#define _0020_SAU_CK01_FCLK_2            (0x0020U) /* ck01 - fCLK/2^2 */
#define _0030_SAU_CK01_FCLK_3            (0x0030U) /* ck01 - fCLK/2^3 */
#define _0040_SAU_CK01_FCLK_4            (0x0040U) /* ck01 - fCLK/2^4 */
#define _0050_SAU_CK01_FCLK_5            (0x0050U) /* ck01 - fCLK/2^5 */
#define _0060_SAU_CK01_FCLK_6            (0x0060U) /* ck01 - fCLK/2^6 */
#define _0070_SAU_CK01_FCLK_7            (0x0070U) /* ck01 - fCLK/2^7 */
#define _0080_SAU_CK01_FCLK_8            (0x0080U) /* ck01 - fCLK/2^8 */
#define _0090_SAU_CK01_FCLK_9            (0x0090U) /* ck01 - fCLK/2^9 */
#define _00A0_SAU_CK01_FCLK_10           (0x00A0U) /* ck01 - fCLK/2^10 */
#define _00B0_SAU_CK01_FCLK_11           (0x00B0U) /* ck01 - fCLK/2^11 */
#define _00C0_SAU_CK01_FCLK_12           (0x00C0U) /* ck01 - fCLK/2^12 */
#define _00D0_SAU_CK01_FCLK_13           (0x00D0U) /* ck01 - fCLK/2^13 */
#define _00E0_SAU_CK01_FCLK_14           (0x00E0U) /* ck01 - fCLK/2^14 */
#define _00F0_SAU_CK01_FCLK_15           (0x00F0U) /* ck01 - fCLK/2^15 */

/*
    Serial Mode Register mn (SMRmn)
*/
#define _0020_SAU_SMRMN_INITIALVALUE     (0x0020U)
/* Selection of macro clock (MCK) of channel n (CKSmn) */
#define _0000_SAU_CLOCK_SELECT_CK00      (0x0000U) /* operation clock CK0 set by PRS register */ 
#define _8000_SAU_CLOCK_SELECT_CK01      (0x8000U) /* operation clock CK1 set by PRS register */
/* Selection of transfer clock (TCLK) of channel n (CCSmn) */
#define _0000_SAU_CLOCK_MODE_CKS         (0x0000U) /* divided operation clock MCK specified by CKSmn bit */  
#define _4000_SAU_CLOCK_MODE_TI0N        (0x4000U) /* clock input from SCK pin (slave transfer in CSI mode) */
/* Selection of start trigger source (STSmn) */
#define _0000_SAU_TRIGGER_SOFTWARE       (0x0000U) /* only software trigger is valid */
#define _0100_SAU_TRIGGER_RXD            (0x0100U) /* valid edge of RXD pin */
/* Controls inversion of level of receive data of channel n in UART mode (SISmn0) */
#define _0000_SAU_EDGE_FALL              (0x0000U) /* falling edge is detected as the start bit */
#define _0040_SAU_EDGE_RISING            (0x0040U) /* rising edge is detected as the start bit */
/* Setting of operation mode of channel n (MDmn2, MDmn1) */
#define _0000_SAU_MODE_CSI               (0x0000U) /* CSI mode */
#define _0002_SAU_MODE_UART              (0x0002U) /* UART mode */
#define _0004_SAU_MODE_IIC               (0x0004U) /* simplified IIC mode */
/* Selection of interrupt source of channel n (MDmn0) */
#define _0000_SAU_TRANSFER_END           (0x0000U) /* transfer end interrupt */
#define _0001_SAU_BUFFER_EMPTY           (0x0001U) /* buffer empty interrupt */

/*
    Serial Communication Operation Setting Register mn (SCRmn)
*/
/* Setting of operation mode of channel n (TXEmn, RXEmn) */
#define _0000_SAU_NOT_COMMUNICATION      (0x0000U) /* does not start communication */
#define _4000_SAU_RECEPTION              (0x4000U) /* reception only */
#define _8000_SAU_TRANSMISSION           (0x8000U) /* transmission only */
#define _C000_SAU_RECEPTION_TRANSMISSION (0xC000U) /* reception and transmission */
/* Selection of data and clock phase in CSI mode (DAPmn, CKPmn) */
#define _0000_SAU_TIMING_1               (0x0000U) /* type 1 */
#define _1000_SAU_TIMING_2               (0x1000U) /* type 2 */
#define _2000_SAU_TIMING_3               (0x2000U) /* type 3 */
#define _3000_SAU_TIMING_4               (0x3000U) /* type 4 */
/* Selection of masking of error interrupt signal (EOCmn) */
#define _0000_SAU_INTSRE_MASK            (0x0000U) /* masks error interrupt INTSREx */
#define _0400_SAU_INTSRE_ENABLE          (0x0400U) /* enables generation of error interrupt INTSREx */
/* Setting of parity bit in UART mode (PTCmn1 - PTCmn0) */
#define _0000_SAU_PARITY_NONE            (0x0000U) /* none parity */
#define _0100_SAU_PARITY_ZERO            (0x0100U) /* zero parity */
#define _0200_SAU_PARITY_EVEN            (0x0200U) /* even parity */
#define _0300_SAU_PARITY_ODD             (0x0300U) /* odd parity */
/* Selection of data transfer sequence in CSI and UART modes (DIRmn) */
#define _0000_SAU_MSB                    (0x0000U) /* MSB */
#define _0080_SAU_LSB                    (0x0080U) /* LSB */
/* Setting of stop bit in UART mode (SLCmn1, SLCmn0) */
#define _0000_SAU_STOP_NONE              (0x0000U) /* none stop bit */
#define _0010_SAU_STOP_1                 (0x0010U) /* 1 stop bit */
#define _0020_SAU_STOP_2                 (0x0020U) /* 2 stop bits */
/* Setting of data length in CSI and UART modes (DLSmn2 - DLSmn0) */
#define _0005_SAU_LENGTH_9               (0x0005U) /* 9-bit data length */
#define _0006_SAU_LENGTH_7               (0x0006U) /* 7-bit data length */
#define _0007_SAU_LENGTH_8               (0x0007U) /* 8-bit data length */

/*
    Serial Output Level Register m (SOLm)
*/
/* Selects inversion of the level of the transmit data of channel n in UART mode (SOLm0) */
#define _0000_SAU_CHANNEL0_NORMAL        (0x0000U) /* normal bit level */
#define _0001_SAU_CHANNEL0_INVERTED      (0x0001U) /* inverted bit level */
/* Selects inversion of the level of the transmit data of channel n in UART mode (SOLm2) */
#define _0000_SAU_CHANNEL2_NORMAL        (0x0000U) /* normal bit level */
#define _0004_SAU_CHANNEL2_INVERTED      (0x0004U) /* inverted bit level */

/*
    Noise Filter Enable Register 0 (NFEN0)
*/
/* Use of noise filter of RXD0 pin (SNFEN00) */
#define _00_SAU_RXD0_FILTER_OFF          (0x00U) /* noise filter off */
#define _01_SAU_RXD0_FILTER_ON           (0x01U) /* noise filter on */
/* Use of noise filter of RXD1 pin (SNFEN10) */
#define _00_SAU_RXD1_FILTER_OFF          (0x00U) /* noise filter off */
#define _04_SAU_RXD1_FILTER_ON           (0x04U) /* noise filter on */
/* Use of noise filter of RXD2 pin (SNFEN20) */
#define _00_SAU_RXD2_FILTER_OFF          (0x00U) /* noise filter off */
#define _10_SAU_RXD2_FILTER_ON           (0x10U) /* noise filter on */
/* Use of noise filter of RXD3 pin (SNFEN30) */
#define _00_SAU_RXD3_FILTER_OFF          (0x00U) /* noise filter off */
#define _40_SAU_RXD3_FILTER_ON           (0x40U) /* noise filter on */

/*
    Format of Serial Status Register mn (SSRmn)
*/
/* Communication status indication flag of channel n (TSFmn) */
#define _0040_SAU_UNDER_EXECUTE          (0x0040U) /* communication is under execution */
/* Buffer register status indication flag of channel n (BFFmn) */
#define _0020_SAU_VALID_STORED           (0x0020U) /* valid data is stored in the SDRmn register */
/* Framing error detection flag of channel n (FEFmn) */
#define _0004_SAU_FRAM_ERROR             (0x0004U) /* a framing error occurs during UART reception */
/* Parity error detection flag of channel n (PEFmn) */
#define _0002_SAU_PARITY_ERROR           (0x0002U) /* a parity error occurs or ACK is not detected */
/* Overrun error detection flag of channel n (OVFmn) */
#define _0001_SAU_OVERRUN_ERROR          (0x0001U) /* an overrun error occurs */

/*
    Serial Channel Start Register m (SSm)
*/
/* Operation start trigger of channel 0 (SSm0) */
#define _0000_SAU_CH0_START_TRG_OFF      (0x0000U) /* no trigger operation */
#define _0001_SAU_CH0_START_TRG_ON       (0x0001U) /* sets SEm0 to 1 and enters the communication wait status */
/* Operation start trigger of channel 1 (SSm1) */
#define _0000_SAU_CH1_START_TRG_OFF      (0x0000U) /* no trigger operation */
#define _0002_SAU_CH1_START_TRG_ON       (0x0002U) /* sets SEm1 to 1 and enters the communication wait status */
/* Operation start trigger of channel 2 (SSm2) */
#define _0000_SAU_CH2_START_TRG_OFF      (0x0000U) /* no trigger operation */
#define _0004_SAU_CH2_START_TRG_ON       (0x0004U) /* sets SEm2 to 2 and enters the communication wait status */
/* Operation start trigger of channel 3 (SSm3) */
#define _0000_SAU_CH3_START_TRG_OFF      (0x0000U) /* no trigger operation */
#define _0008_SAU_CH3_START_TRG_ON       (0x0008U) /* sets SEm3 to 3 and enters the communication wait status */

/*
    Serial Channel Stop Register m (STm)
*/
/* Operation stop trigger of channel 0 (STm0) */
#define _0000_SAU_CH0_STOP_TRG_OFF       (0x0000U) /* no trigger operation */
#define _0001_SAU_CH0_STOP_TRG_ON        (0x0001U) /* operation is stopped (stop trigger is generated) */
/* Operation stop trigger of channel 1 (STm1) */
#define _0000_SAU_CH1_STOP_TRG_OFF       (0x0000U) /* no trigger operation */
#define _0002_SAU_CH1_STOP_TRG_ON        (0x0002U) /* operation is stopped (stop trigger is generated) */
/* Operation stop trigger of channel 2 (STm2) */
#define _0000_SAU_CH2_STOP_TRG_OFF       (0x0000U) /* no trigger operation */
#define _0004_SAU_CH2_STOP_TRG_ON        (0x0004U) /* operation is stopped (stop trigger is generated) */
/* Operation stop trigger of channel 3 (STm3) */
#define _0000_SAU_CH3_STOP_TRG_OFF       (0x0000U) /* no trigger operation */
#define _0008_SAU_CH3_STOP_TRG_ON        (0x0008U) /* operation is stopped (stop trigger is generated) */

/*
    Serial Channel Enable Status Register m (SEm)
*/
/* Indication of operation enable/stop status of channel 0 (SEm0) */
#define _0000_SAU_CH0_STATUS_DISABLE     (0x0000U) /* operation stops */
#define _0001_SAU_CH0_STATUS_ENABLE      (0x0001U) /* operation is enabled */
/* Indication of operation enable/stop status of channel 1 (SEm1) */
#define _0000_SAU_CH1_STATUS_DISABLE     (0x0000U) /* operation stops */
#define _0002_SAU_CH1_STATUS_ENABLE      (0x0002U) /* operation is enabled */
/* Indication of operation enable/stop status of channel 2 (SEm2) */
#define _0000_SAU_CH2_STATUS_DISABLE     (0x0000U) /* operation stops */
#define _0004_SAU_CH2_STATUS_ENABLE      (0x0004U) /* operation is enabled */
/* Indication of operation enable/stop status of channel 3 (SEm3) */
#define _0000_SAU_CH3_STATUS_DISABLE     (0x0000U) /* operation stops */
#define _0008_SAU_CH3_STATUS_ENABLE      (0x0008U) /* operation is enabled */

/*
    Format of Serial Flag Clear Trigger Register mn (SIRmn)
*/
/* Clear trigger of overrun error flag of channel n (OVCTmn) */
#define _0001_SAU_SIRMN_OVCTMN           (0x0001U)
/* Clear trigger of parity error flag of channel n (PECTmn) */
#define _0002_SAU_SIRMN_PECTMN           (0x0002U)
/* Clear trigger of framing error of channel n (FECTmn) */
#define _0004_SAU_SIRMN_FECTMN           (0x0004U)

/*
    Serial Output Enable Register m (SOEm)
*/
/* Serial output enable/disable of channel 0 (SOEm0) */
#define _0000_SAU_CH0_OUTPUT_DISABLE     (0x0000U) /* stops output by serial communication operation */
#define _0001_SAU_CH0_OUTPUT_ENABLE      (0x0001U) /* enables output by serial communication operation */
/* Serial output enable/disable of channel 2 (SOEm2) */
#define _0000_SAU_CH2_OUTPUT_DISABLE     (0x0000U) /* stops output by serial communication operation */
#define _0004_SAU_CH2_OUTPUT_ENABLE      (0x0004U) /* enables output by serial communication operation */

/*
    Serial Output Register m (SOm)
*/
/* Serial data output of channel 0 (SOm0) */
#define _0000_SAU_CH0_DATA_OUTPUT_0      (0x0000U) /* serial data output value is "0" */
#define _0001_SAU_CH0_DATA_OUTPUT_1      (0x0001U) /* serial data output value is "1" */
/* Serial data output of channel 2 (SOm2) */
#define _0000_SAU_CH2_DATA_OUTPUT_0      (0x0000U) /* serial data output value is "0" */
#define _0004_SAU_CH2_DATA_OUTPUT_1      (0x0004U) /* serial data output value is "1" */
/* Serial clock output of channel 0 (CKOm0) */
#define _0000_SAU_CH0_CLOCK_OUTPUT_0     (0x0000U) /* serial clock output value is "0" */
#define _0100_SAU_CH0_CLOCK_OUTPUT_1     (0x0100U) /* serial clock output value is "1" */
/* Serial clock output of channel 2 (CKOm2) */
#define _0000_SAU_CH2_CLOCK_OUTPUT_0     (0x0000U) /* serial clock output value is "0" */
#define _0400_SAU_CH2_CLOCK_OUTPUT_1     (0x0400U) /* serial clock output value is "1" */

/*
    SAU Standby Control Register m (SSCm)
*/
/* Selection of whether to enable or stop the startup of CSI00 or UART0 reception while in the STOP mode (SWCm) */
#define _0000_SAU_CH0_SNOOZE_OFF         (0x0000U) /* stop the SNOOZE mode function */
#define _0001_SAU_CH0_SNOOZE_ON          (0x0001U) /* use the SNOOZE mode function */
/* Selection of whether to enable or stop the generation of transfer end interrupts (SSECm) */
#define _0000_SAU_INTSRE0_ENABLE         (0x0000U) /* enable the generation of error interrupts (INTSRE0/INTSRE2) */
#define _0002_SAU_INTSRE0_DISABLE        (0x0002U) /* stop the generation of error interrupts (INTSRE0/INTSRE2) */

/* SAU used flag */
#define _00_SAU_IIC_MASTER_FLAG_CLEAR    (0x00U)
#define _01_SAU_IIC_SEND_FLAG            (0x01U)
#define _02_SAU_IIC_RECEIVE_FLAG         (0x02U)
#define _04_SAU_IIC_SENDED_ADDRESS_FLAG  (0x04U)

/***********************************************************************************************************************
Macro definitions
***********************************************************************************************************************/
//1200~38400
#define _CE00_UART0_RECEIVE_DIVISOR      (0xCE00U)
#define _CE00_UART0_TRANSMIT_DIVISOR     (0xCE00U)
#define _CE00_UART1_RECEIVE_DIVISOR      (0xCE00U)
#define _CE00_UART1_TRANSMIT_DIVISOR     (0xCE00U)
#define _CE00_UART2_RECEIVE_DIVISOR      (0xCE00U)
#define _CE00_UART2_TRANSMIT_DIVISOR     (0xCE00U)
#define _CE00_UART3_RECEIVE_DIVISOR      (0xCE00U)
#define _CE00_UART3_TRANSMIT_DIVISOR     (0xCE00U)
//115200
#define _4400_UART0_RECEIVE_DIVISOR      (0x4400U)
#define _4400_UART0_TRANSMIT_DIVISOR     (0x4400U)
#define _4400_UART1_RECEIVE_DIVISOR      (0x4400U)
#define _4400_UART1_TRANSMIT_DIVISOR     (0x4400U)
#define _4400_UART2_RECEIVE_DIVISOR      (0x4400U)
#define _4400_UART2_TRANSMIT_DIVISOR     (0x4400U)
#define _4400_UART3_RECEIVE_DIVISOR      (0x4400U)
#define _4400_UART3_TRANSMIT_DIVISOR     (0x4400U)


/***********************************************************************************************************************
Macro definitions (Register bit)
***********************************************************************************************************************/
/*
    Peripheral Enable Register 1 (PER1)
*/
/* Interval timer input clock supply (TMKAEN) */
#define _00_IT_CLOCK_STOP             (0x00U) /* stops supply of input clock */
#define _80_IT_CLOCK_SUPPLY           (0x80U) /* supplies input clock */

/* 
    Interval timer control register (ITMC)
*/
/* Interval timer operation enable/disable specification (RINTE) */
#define _0000_IT_OPERATION_DISABLE    (0x0000U) /* disable interval timer operation */
#define _8000_IT_OPERATION_ENABLE     (0x8000U) /* enable interval timer operation */

#define _0020_ITMCMP_VALUE            (0x0020U)
#define _0147_ITMCMP_VALUE            (0x0147U)
#define _028E_ITMCMP_VALUE            (0x028EU)



/***********************************************************************************************************************
***********************************************************************************************************************/

//端口中断配置
void INT0Config(unsigned char Config, IRQServerFT Fuc);
void INT1Config(unsigned char Config, IRQServerFT Fuc);
void INT2Config(unsigned char Config, IRQServerFT Fuc);
void INT3Config(unsigned char Config, IRQServerFT Fuc);
void INT4Config(unsigned char Config, IRQServerFT Fuc);
void INT5Config(unsigned char Config, IRQServerFT Fuc);
void INT6Config(unsigned char Config, IRQServerFT Fuc);
void INT7Config(unsigned char Config, IRQServerFT Fuc);
//串口配置
void Uart0Config(pUartMode Mode, IRQServerSendDataFT SendFun, IRQServerRecvDataFT RecvFun);
void Uart1Config(pUartMode Mode, IRQServerSendDataFT SendFun, IRQServerRecvDataFT RecvFun);
void Uart2Config(pUartMode Mode, IRQServerSendDataFT SendFun, IRQServerRecvDataFT RecvFun);
void Uart3Config(pUartMode Mode, IRQServerSendDataFT SendFun, IRQServerRecvDataFT RecvFun);
//串口使能控制
void Uart0SendEnable(void);
void Uart0SendDisable(void);
void Uart0ReceiveEnable(void);
void Uart0ReceiveDisable(void);
void Uart1SendEnable(void);
void Uart1SendDisable(void);
void Uart1ReceiveEnable(void);
void Uart1ReceiveDisable(void);
void Uart2SendEnable(void);
void Uart2SendDisable(void);
void Uart2ReceiveEnable(void);
void Uart2ReceiveDisable(void);
void Uart3SendEnable(void);
void Uart3SendDisable(void);
void Uart3ReceiveEnable(void);
void Uart3ReceiveDisable(void);
int Uart0SendData(unsigned char*pSendBuf, unsigned int Len);
int Uart1SendData(unsigned char*pSendBuf, unsigned int Len);
int Uart2SendData(unsigned char*pSendBuf, unsigned int Len);
int Uart3SendData(unsigned char*pSendBuf, unsigned int Len);
//MCU睡眠
void MCU_SleepEnter(void);
//MCU中断控制
void MCU_IntEn(void);
void MCU_IntDis(void);
void MCU_DogCfg(void);              //初始化
void MCU_DogReset(void);            //喂狗
void MCUPllCfg(void);
void MCU_LcdCfg(void);   //液晶初始化
void MCU_LcdOpen(void);  //打开液晶
void MCU_LcdClose(void); //关闭液晶
void MCU_LcdMemSet(unsigned char Reg);
void MCU_LcdAllSet(void); //关闭液晶
void MCU_LcdAllClr(void); //关闭液晶
void MCU_LcdWriteReg(unsigned char Seg, unsigned char Reg);
unsigned char MCU_LcdReadReg(unsigned char Seg);
//定时器控制
void MCU_TimerCfg(PortNumET TimeNo, TimeSpET Sp, IRQServerFT TimeServer);
void MCU_TimerStart(PortNumET TimeNo);
void MCU_TimerStop(PortNumET TimeNo);
//ADC控制
void MCU_ADCCfg(PortNumET ADCNum, ADCModeST Mode);   //
void MCU_ADCStart(PortNumET ADCNum);
void MCU_ADCStop(PortNumET ADCNum);
unsigned int MCU_ADCRead(void);
//实时时钟控制
int RTCCfg(void);                //返回TRUE/FALSE
int RTCReadTime(RTC_T* Rtc);     //返回TRUE/FALSE
int RTCWriteTime(RTC_T* Rtc);    //返回TRUE/FALSE

#endif