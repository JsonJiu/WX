#ifndef _IOINTP_H
#define _IOINTP_H
#include "r_cg_macrodriver.h"
#include "r_cg_userdefine.h"
#include "Mydefine.h"
typedef enum 
{
    ZJ_INTP,  
    FJ_INTP,
}_Intp_Def;

void INTP_Init();
void Start_Intp(_Intp_Def Intp_io);
void Stop_Intp(_Intp_Def Intp_io);
#endif