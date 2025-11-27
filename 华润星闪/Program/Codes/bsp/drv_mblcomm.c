#include "drv_mblcomm.h"
#include "drv_uart.h"
#include "timer_manage.h"

#include <string.h>
#include <stdio.h>
#include <stdlib.h>

unsigned char ATACK_TIME_NUM;
unsigned char SecondsTiming;
GSMDevice GsmDevice;
GSMTCP GSMTcp;

/**
 * 设置GPRS模块电源脚
 * @param fun
 */
void OM_SetGprsPower(unsigned char fun)
{
    SetGprsPower(fun);
}

/**
 * 设置GPRS开关机脚
 * @param fun
 */
void OM_SetGprsPowerOnOff(unsigned char fun)
{
    SetGprsPowerOnOff(fun);
}

/**
* GPRS发送AT命令超时回应
*
* @author wb (2014-11-25)
*/

void GPRS_RecvData_OverTimeTask(void)
{
    if(SecondsTiming) SecondsTiming--;
}

/**
 * GSM模块初始化
 * @return
 */
unsigned char OM_GSM_Modem_Init(void)
{
    if(ATACK_TIME_NUM == 0)
    {
        ATACK_TIME_NUM = User_AskTimeNumber();
    }
    User_StartTime(MAINTIMER, ATACK_TIME_NUM, 100, GPRS_RecvData_OverTimeTask);
}

/**
* GSM发送AT指令组帧
*
* @author wb (2014-11-06)
*
* @param fun - AT指令类型
* @param pBuf - AT指令缓冲区
*
* @return unsigned short - 返回AT指令的长度
*/

unsigned short FormATCommandPackage(unsigned char fun,unsigned char *pBuf)
{
    switch(fun)
    {
        case C_AT: 	        sprintf((char*)pBuf,"AT%c",0x0D); break;
        default:break;
    }
    return strlen((char*)pBuf);
}

/**
 * GPRS发送数据
 * @param buf
 * @param len
 */
void GprsSendData(unsigned char *buf, unsigned short len)
{
    OM_SendUsartData(GPRS_COM, buf, len);
}

unsigned char Get_GSMCmdAck_Type(void)
{
    
}

unsigned char GprsRecvData(void)
{
    
}

/**
* 模块供电开关机控制设置
*
* @author wb (2014-11-06)
*
* @param fun
*  O_POWERON - 开机
*  O_POWEROFF - 关机
*  O_RESET - 复位
*/

void SetGsmDevicePowerControl(unsigned char fun)
{
    if(fun == O_POWERON)
    {
        GsmDevice.S_Device = S_IDLE;
        GsmDevice.S_Power = S_IDLE;
        GsmDevice.opt_IdlePowerOn = true;
    }
    else if(fun == O_POWEROFF)
    {
        GsmDevice.S_Device = S_IDLE;
        GsmDevice.S_Power = S_POWERON;
        GsmDevice.opt_PowerOff = true;
    }
    else if(fun == O_RESET)
    {
        GsmDevice.S_Device = S_IDLE;
        //GsmDevice.S_Power = S_POWERON;
        GsmDevice.opt_PowerOff = true;
        GsmDevice.opt_IdlePowerOn = true;
        GSMTcp.CntStatus = ST_NONE;
    }
    else
        return;
}

/**
* 模块上电,开机,关机的过程
* 通过AT命令同步成功，表示开机成功
* @author wb (2014-11-06)
*/

void Process_PowerOn_Modem(void)
{
    unsigned char tx_atcmd_buf[256];
    unsigned short len = 0;
    unsigned char r_Function;
    switch(GsmDevice.S_Power)
    {
    case S_IDLE:
        if(GsmDevice.opt_IdlePowerOn)//空闲开机
        {
            GsmDevice.opt_IdlePowerOn = false;
            OM_SetGprsPower(false);
            DelayMs(100);
            OM_SetGprsPowerOnOff(true);	//拉高开关机电平
            SecondsTiming = 10;   	        //1秒延时
            GsmDevice.PowerOnTimes = 0;
            GsmDevice.S_Power = S_WAITPOWERSUPPLY;
        }
        break;
    case S_WAITPOWERSUPPLY://等待上电成功
        if(SecondsTiming == 0)
        {
            SecondsTiming = 30;		//2秒及以上
            GsmDevice.S_Power = S_WAITPOWERON;
            OM_SetGprsPowerOnOff (false);	//拉低开关机电平
        }
        break;
    case S_WAITPOWERON://等待开机
        if(SecondsTiming == 0)
        {
            SecondsTiming = 80;		//4秒以上
            GsmDevice.S_Power = S_WAITCHECKPOWERON;
            OM_SetGprsPowerOnOff(true);	//拉高开关机电平
        }
        break;
    case S_WAITCHECKPOWERON://等待检测开机成功
        if(SecondsTiming == 0)
        {
#ifdef DEF_TOUCHUAN
            GsmDevice.S_Power = S_POWERON;
            GsmDevice.S_Device = S_POWERON;
            sprintf((char*)DebugBuf,"\r\n==========================开机成功！=====================\r\n");
            WriteUsartSendBuf(Debug_COM_2, DebugBuf, strlen((char*)DebugBuf));
#else
            GsmDevice.S_Power = S_DETECTPOWERON;
            len = FormATCommandPackage(C_AT, tx_atcmd_buf);
            GprsSendData(tx_atcmd_buf, len);
            SecondsTiming = 30;
            GsmDevice.PowerOnTimes++;
#endif
        }
        break;
    case S_DETECTPOWERON:
        if(SecondsTiming)
        {
            r_Function = GprsRecvData();
            if(r_Function == true)//是否收到有效字符串
            {
                r_Function = Get_GSMCmdAck_Type();
                if(r_Function == A_OK)//收到OK回应，开机成功
                {
                    GsmDevice.S_Power = S_POWERON;
                    GsmDevice.S_Device = S_POWERON;
                    SecondsTiming = 30;
                    GsmDevice.PowerOnTimes = 0;
//                    sprintf((char*)DebugBuf,"\r\n==========================开机成功！=====================\r\n");
//                    OM_SendUsartData(Debug_COM, DebugBuf, strlen((char*)DebugBuf));
                }
            }
        }
        else
        {
            if(GsmDevice.PowerOnTimes < 15)
            {
                GsmDevice.S_Power = S_WAITCHECKPOWERON;
            }
            else
            {
                GsmDevice.PowerOnTimes = 0;
                GsmDevice.S_Power = S_IDLE;
                GsmDevice.opt_IdlePowerOn = true;
            }
        }
        break;
    case S_POWERON://已开机
        if(GsmDevice.opt_PowerOff)//关机
        {
            GsmDevice.opt_PowerOff = 0;
            OM_SetGprsPowerOnOff(true);
            SecondsTiming = 8;//(M72D模块)低电平应该<1S,>0.6S
            GsmDevice.S_Power = S_WAITPOWEROFF;
        }
        break;
    case S_WAITPOWEROFF:
        if(!SecondsTiming)
        {
            OM_SetGprsPowerOnOff(false);
            GsmDevice.S_Power = S_WAITPOWERCUT;
            SecondsTiming = 80;//(M72D模块)高电平保持时间应该大于2S,小于12S
        }
        break;
    case S_WAITPOWERCUT:
        if(!SecondsTiming)
        {
            GsmDevice.S_Power = S_IDLE;
        }
        break;
    default:
        GsmDevice.S_Power = S_IDLE;
        break;
    }
}
