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
* File Name    : r_cg_rtc.c
* Version      : Code Generator for RL78/L13 V1.01.00.01 [26 Jul 2013]
* Device(s)    : R5F10WMG
* Tool-Chain   : CA78K0R
* Description  : This file implements device driver for RTC module.
* Creation Date: 2018-03-07
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
#include "r_cg_rtc.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_RTC_Create
* Description  : This function initializes the real-time clock module.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_RTC_Create(void)
{
    RTCWEN = 1U;   /* enables input clock supply */
    RTCE = 0U;     /* disable RTC clock operation */
    RTCMK = 1U;    /* disable INTRTC interrupt */
    RTCIF = 0U;    /* clear INTRTC interrupt flag */
    RTITMK = 1U;   /* disable INTRTIT interrupt */
    RTITIF = 0U;   /* clear INTRTIT interrupt flag */
    /* Set INTRTC low priority */
    RTCPR1 = 1U;
    RTCPR0 = 1U;
    RTCC0 = _00_RTC_RTC1HZ_DISABLE | _00_RTC_12HOUR_SYSTEM | _01_RTC_INTRTC_CLOCK_0;
    RTCWEN = 0U;    /* stops input clock supply */
}

/* Start user code for adding. Do not edit comment generated here */
void R_RTC_Start(void)
{
    RTCWEN = 1U;   /* enables input clock supply */
    RTCIF = 0U;    /* clear INTRTC interrupt flag */
    RTCMK = 0U;    /* enable INTRTC interrupt */
    RTCE = 1U;     /* enable RTC clock operation */
    RTCWEN = 0U;   /* stops input clock supply */
}
void R_RTC_Stop(void)
{
    RTCWEN = 1U;  /* enables input clock supply */
    RTCE = 0U;    /* disable RTC clock operation */
    RTCMK = 1U;   /* disable INTRTC interrupt */
    RTCIF = 0U;   /* clear INTRTC interrupt flag */
    RTCWEN = 0U;  /* stops input clock supply */
}
/* End user code. Do not edit comment generated here */
