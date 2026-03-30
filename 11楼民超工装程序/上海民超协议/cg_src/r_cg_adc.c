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
* File Name    : r_cg_adc.c
* Version      : Code Generator for RL78/L13 V1.01.00.01 [26 Jul 2013]
* Device(s)    : R5F10WMG
* Tool-Chain   : CA78K0R
* Description  : This file implements device driver for ADC module.
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
#include "r_cg_adc.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_ADC_Create
* Description  : This function initializes the AD converter.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_ADC_Create(void)
{
    ADCEN = 1U;  /* supply AD clock */
    ADM0 = _00_AD_ADM0_INITIALVALUE;  /* disable AD conversion and clear ADM0 register */
    ADMK = 1U;  /* disable INTAD interrupt */
    ADIF = 0U;  /* clear INTAD interrupt flag */
    /* Set ANI0 - ANI1 pin */
    PM2 |= 0x03U;
		//PM2 |= 0x02U;
    /* Set ANI17 pin */
    PFSEG3 |= 0x80U;
    PMC2 |= 0x08U;
    PM2 |= 0x08U;
    ADM0 = _10_AD_CONVERSION_CLOCK_16 | _04_AD_TIME_MODE_LOWVOLTAGE_1;
    ADM1 = _00_AD_TRIGGER_SOFTWARE | _20_AD_CONVMODE_ONESELECT;
    ADM2 = _80_AD_POSITIVE_INTERVOLT | _00_AD_NEGATIVE_VSS | _00_AD_AREA_MODE_1 | _01_AD_RESOLUTION_8BIT;
    ADUL = _FF_AD_ADUL_VALUE;
    ADLL = _00_AD_ADLL_VALUE;
    ADS = _11_AD_INPUT_CHANNEL_17;//caiyang VOUT
}

/* Start user code for adding. Do not edit comment generated here */
void R_ADC_Start(void)
{
    ADCS = 1U;  /* enable AD conversion */
}
/***********************************************************************************************************************
* Function Name: R_ADC_Stop
* Description  : This function stops the AD converter.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
void R_ADC_Stop(void)
{
    ADCS = 0U;  /* disable AD conversion */
}
void R_ADC_Set_OperationOn(void)
{
    ADCE = 1U;  /* enable AD comparator */
}
void R_ADC_Set_OperationOff(void)
{
    ADCE = 0U;  /* disable AD comparator */
}
void R_ADC_Set_PowerOff(void)
{
    ADCEN = 0U;  /* stop AD clock */
}
void Change_ADC_Channel(unsigned char adchannel)
{
  	ADCEN = 1U;  /* supply AD clock */
    ADM0 = _00_AD_ADM0_INITIALVALUE;  /* disable AD conversion and clear ADM0 register */
    ADMK = 1U;  /* disable INTAD interrupt */
    ADIF = 0U;  /* clear INTAD interrupt flag */
    /* Set ANI0 - ANI1 pin */
    PM2 |= 0x03U;
    /* Set ANI17 pin */
    PFSEG3 |= 0x80U;
    PMC2 |= 0x08U;
    PM2 |= 0x08U;
    ADM0 = _10_AD_CONVERSION_CLOCK_16 | _04_AD_TIME_MODE_LOWVOLTAGE_1;
    ADM1 = _00_AD_TRIGGER_SOFTWARE | _20_AD_CONVMODE_ONESELECT;
    ADM2 = _00_AD_POSITIVE_VDD | _00_AD_NEGATIVE_VSS | _00_AD_AREA_MODE_1 | _01_AD_RESOLUTION_8BIT;
    ADUL = _FF_AD_ADUL_VALUE;
    ADLL = _00_AD_ADLL_VALUE;
    //ADS = _11_AD_INPUT_CHANNEL_17;
		if(adchannel)
		{
			ADS = _11_AD_INPUT_CHANNEL_17;
		}
		else
		{
			ADS = _01_AD_INPUT_CHANNEL_1;
		}
}
/* End user code. Do not edit comment generated here */
