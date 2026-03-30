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
* File Name    : r_cg_port.c
* Version      : Code Generator for RL78/L13 V1.01.00.01 [26 Jul 2013]
* Device(s)    : R5F10WMG
* Tool-Chain   : CA78K0R
* Description  : This file implements device driver for Port module.
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
#include "r_cg_port.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#include "r_cg_userdefine.h"

/***********************************************************************************************************************
Global variables and functions
***********************************************************************************************************************/
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/***********************************************************************************************************************
* Function Name: R_PORT_Create
* Description  : This function initializes the Port I/O.
* Arguments    : None
* Return Value : None
***********************************************************************************************************************/
    void R_PORT_Create(void)
{
    PFSEG0 = _00_PFSEG07_PORT | _00_PFSEG06_PORT | _00_PFSEG05_PORT | _00_PFSEG04_PORT;
    PFSEG1 = _00_PFSEG15_PORT | _00_PFSEG14_PORT | _00_PFSEG13_PORT | _00_PFSEG12_PORT | _08_PFSEG11_SEG | 
             _00_PFSEG10_PORT | _00_PFSEG09_PORT | _00_PFSEG08_PORT;
    PFSEG2 = _80_PFSEG23_SEG | _00_PFSEG22_PORT | _20_PFSEG21_SEG | _00_PFSEG20_PORT | _00_PFSEG19_PORT | 
             _00_PFSEG18_PORT | _00_PFSEG17_PORT | _00_PFSEG16_PORT;
    PFSEG3 = _80_PFSEG30_SEG | _00_PFSEG29_PORT | _20_PFSEG28_SEG | _10_PFSEG27_SEG | _00_PFSEG26_PORT | 
             _02_PFSEG25_SEG | _01_PFSEG24_SEG | _04_PFDEG_DEFAULT;
    PFSEG4 = _00_PFSEG38_PORT | _00_PFSEG37_PORT | _00_PFSEG36_PORT | _00_PFSEG35_PORT | _00_PFSEG34_PORT | 
             _00_PFSEG33_PORT | _00_PFSEG32_PORT | _00_PFSEG31_PORT;
    PFSEG5 = _00_PFSEG46_PORT | _00_PFSEG45_PORT | _20_PFSEG44_SEG | _00_PFSEG43_PORT | _00_PFSEG42_PORT | 
             _00_PFSEG41_PORT | _00_PFSEG40_PORT | _00_PFSEG39_PORT;
    ISCLCD = _02_ISCVL3_VALID | _01_ISCCAP_VALID;
    P0 = _00_Pn0_OUTPUT_0 | _00_Pn2_OUTPUT_0 | _00_Pn5_OUTPUT_0;
    P1 = _01_Pn0_OUTPUT_1 | _00_Pn1_OUTPUT_0 | _00_Pn2_OUTPUT_0 | _00_Pn3_OUTPUT_0 | _00_Pn4_OUTPUT_0 | 
         _00_Pn5_OUTPUT_0 | _40_Pn6_OUTPUT_1 | _00_Pn7_OUTPUT_0;
    P2 = _00_Pn4_OUTPUT_0 | _00_Pn5_OUTPUT_0 | _00_Pn6_OUTPUT_0 | _00_Pn7_OUTPUT_0;
    P3 = _01_Pn0_OUTPUT_1 | _00_Pn2_OUTPUT_0;
    P4 = _00_Pn0_OUTPUT_0 | _00_Pn1_OUTPUT_0 | _00_Pn2_OUTPUT_0 | _00_Pn3_OUTPUT_0 | _00_Pn4_OUTPUT_0 | 
         _00_Pn5_OUTPUT_0 | _00_Pn6_OUTPUT_0;
    P5 = _00_Pn0_OUTPUT_0 | _02_Pn1_OUTPUT_1 | _00_Pn2_OUTPUT_0 | _08_Pn3_OUTPUT_1 | _10_Pn4_OUTPUT_1 | 
         _00_Pn5_OUTPUT_0 | _40_Pn6_OUTPUT_1;
    P6 = _00_Pn0_OUTPUT_0 | _00_Pn1_OUTPUT_0;
    P7 = _00_Pn3_OUTPUT_0 | _00_Pn4_OUTPUT_0 | _00_Pn5_OUTPUT_0 | _00_Pn6_OUTPUT_0 | _00_Pn7_OUTPUT_0;
    P12 = _00_Pn5_OUTPUT_0 | _00_Pn6_OUTPUT_0 | _80_Pn7_OUTPUT_1;
    PU0 = _08_PUn3_PULLUP_ON;
    PMC1 = _00_PMCn0_DI_ON | _00_PMCn1_DI_ON | _00_PMCn2_DI_ON | _00_PMCn3_DI_ON | _F0_PMC1_DEFAULT;
    PMC2 = _00_PMCn2_DI_ON | _08_PMCn3_NOT_USE | _00_PMCn4_DI_ON | _00_PMCn5_DI_ON | _03_PMC2_DEFAULT;
    ADPC = _01_ADPC_DI_ON;
    PM0 = _00_PMn0_MODE_OUTPUT | _02_PMn1_NOT_USE | _00_PMn2_MODE_OUTPUT | _08_PMn3_NOT_USE | _10_PMn4_NOT_USE | 
          _00_PMn5_MODE_OUTPUT | _40_PMn6_NOT_USE | _80_PMn7_NOT_USE;
    PM1 = _00_PMn0_MODE_OUTPUT | _00_PMn1_MODE_OUTPUT | _00_PMn2_MODE_OUTPUT | _00_PMn3_MODE_OUTPUT | 
          _00_PMn4_MODE_OUTPUT | _00_PMn5_MODE_OUTPUT | _00_PMn6_MODE_OUTPUT | _00_PMn7_MODE_OUTPUT;
    PM2 = _01_PMn0_MODE_INPUT | _02_PMn1_MODE_INPUT | _04_PMn2_MODE_INPUT | _08_PMn3_NOT_USE | _00_PMn4_MODE_OUTPUT | 
          _00_PMn5_MODE_OUTPUT | _00_PMn6_MODE_OUTPUT | _00_PMn7_MODE_OUTPUT;
    PM3 = _00_PMn0_MODE_OUTPUT | _02_PMn1_NOT_USE | _00_PMn2_MODE_OUTPUT | _08_PMn3_NOT_USE | _10_PMn4_NOT_USE | 
          _20_PMn5_NOT_USE | _C0_PM3_DEFAULT;
    PM4 = _00_PMn0_MODE_OUTPUT | _00_PMn1_MODE_OUTPUT | _00_PMn2_MODE_OUTPUT | _00_PMn3_MODE_OUTPUT | 
          _00_PMn4_MODE_OUTPUT | _00_PMn5_MODE_OUTPUT | _00_PMn6_MODE_OUTPUT | _80_PMn7_NOT_USE;
    PM5 = _00_PMn0_MODE_OUTPUT | _00_PMn1_MODE_OUTPUT | _00_PMn2_MODE_OUTPUT | _00_PMn3_MODE_OUTPUT | 
          _00_PMn4_MODE_OUTPUT | _00_PMn5_MODE_OUTPUT | _00_PMn6_MODE_OUTPUT | _80_PMn7_NOT_USE;
    PM6 = _00_PMn0_MODE_OUTPUT | _00_PMn1_MODE_OUTPUT | _FC_PM6_DEFAULT;
    PM7 = _01_PMn0_MODE_INPUT | _02_PMn1_MODE_INPUT | _04_PMn2_MODE_INPUT | _00_PMn3_MODE_OUTPUT | 
          _00_PMn4_MODE_OUTPUT | _00_PMn5_MODE_OUTPUT | _00_PMn6_MODE_OUTPUT | _00_PMn7_MODE_OUTPUT;
    PM12 = _00_PMn5_MODE_OUTPUT | _00_PMn6_MODE_OUTPUT | _00_PMn7_MODE_OUTPUT | _1F_PM12_DEFAULT;

}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
