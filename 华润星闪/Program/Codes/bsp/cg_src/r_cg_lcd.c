/***********************************************************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only 
* intended for use with Renesas products. No other uses are authorized. This 
* software is owned by Renesas Electronics Corporation and is protected under 
* all applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING 
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT 
* LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE 
* AND NON-INFRINGEMENT.  ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS 
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE 
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR 
* ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE 
* BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software 
* and to discontinue the availability of this software.  By using this software, 
* you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer
*
* Copyright (C) 2013 Renesas Electronics Corporation. All rights reserved.
***********************************************************************************************************************/

/***********************************************************************************************************************
* File Name    : r_cg_lcd.c
* Version      : Applilet4 for RL78/L13 V1.01.00.01 [26 Jul 2013]
* Device(s)    : R5F10WMG
* Tool-Chain   : IAR Systems icc78k0r
* Description  : This file implements device driver for LCD module.
* Creation Date: 2014-11-17
***********************************************************************************************************************/

/***********************************************************************************************************************
Pragma directive
***********************************************************************************************************************/
/* Start user code for pragma. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
Includes
***********************************************************************************************************************/
#include "r_cg_macrodriver.h"
#include "r_cg_lcd.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_LCD_Create
* Description  : This function initializes the LCD module.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_LCD_Create(void)
{
    LCDON = 0U;    /* disable LCD clock operation */
    LCDMK0 = 1U;    /* disable INTLCD0 interrupt */
    LCDIF0 = 0U;    /* clear INTLCD0 interrupt flag */
    LCDM0 = _00_LCD_DISPLAY_WAVEFORM_A | _00_LCD_DISPLAY_STATIC;
    LCDM0 |= _00_LCD_VOLTAGE_MODE_EXTERNAL;
    /* Set segment pins */
    PU0 &= 0x0EU;
    PU1 &= 0x03U;
    PU5 &= 0x0CU;
    POM0 &= 0x0EU;
    POM1 &= 0x3FU;
    POM5 &= 0x8FU;
    PIM0 &= 0x9FU;
    PIM1 &= 0x3FU;
    PIM5 &= 0xDFU;
    PFSEG0 |= 0x30U;
    PFSEG1 |= 0x0FU;
    PFSEG4 |= 0xC0U;
    PFSEG5 |= 0x1FU;
    PFSEG6 |= 0x0FU;
    P0 &= 0x0EU;
    PM0 &= 0x0EU;
    PMC1 &= 0xF3U;
    P1 &= 0x03U;
    PM1 &= 0x03U;
    P5 &= 0x0CU;
    PM5 &= 0x0CU;
    LCDM1 |= _00_LCD_DISPLAY_PATTERN_A;
    LCDC0 = _07_LCD_CLOCK_FSUB_FIL_8;
}
/***********************************************************************************************************************
* Function Name: R_LCD_Start
* Description  : This function enables the LCD display.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_LCD_Start(void)
{
    LCDON = 1U;
}
/***********************************************************************************************************************
* Function Name: R_LCD_Stop
* Description  : This function disables the LCD display.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_LCD_Stop(void)
{
    LCDON = 0U;
}
/***********************************************************************************************************************
* Function Name: R_LCD_Voltage_On
* Description  : This function enables voltage boost circuit or capacitor split circuit.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_LCD_Voltage_On(void)
{
    SCOC = 1U;
}
/***********************************************************************************************************************
* Function Name: R_LCD_Voltage_Off
* Description  : This function disables voltage boost circuit or capacitor split circuit.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_LCD_Voltage_Off(void)
{
    SCOC = 0U;
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
