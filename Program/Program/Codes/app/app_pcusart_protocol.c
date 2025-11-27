#include "config.h"
#include "app_autocheck.h"
//增加检测完成的判断条件
/**
 * PC串口协议解析
 * @param buf
 */
unsigned char test=0x77;
unsigned short PCUartProtocol_DataAanaly(unsigned char *buf, unsigned char *buf_temp)
{
    unsigned char ctlcode;
    unsigned short datamark;
    if(buf[0] == 0x68)
    {
        datamark = buf[Index_DataMark+1];
        datamark <<= 8;
        datamark += buf[Index_DataMark];
        ctlcode = buf[Index_ControlCode];
        switch(ctlcode)
        {
            case OPT_READ://读操作
                switch(datamark)
                {
                    case DEV_GETCHECK_RESULT://查询上告结果
                        printf("\r\n 查询结果2...\r\n");
                        DebugPrintf("\r\n 查询结果1...\r\n");
                        
			if(buf[Index_VolumeData] == CheckResult.DeviceID)
                        {
                            
                        }
                        else
                            return;
                        break;
                }
                break;
            case OPT_WRITE://写操作
                switch(datamark)
                {
                    case DEV_TIME:
                        CheckRtcTime_Programe(&buf[Index_VolumeData]);
                    break;
                    default:return;
                }
                break;
            case OPT_INSTALL:
                switch(datamark)
                {
                    case DEV_START_TEST:
		    if(buf[Index_VolumeData] == CheckResult.DeviceID)
                    {
                         DebugPrintf("\r\n 启动测试1...\r\n");
                         DeviceTool.EnableDeviceToolCheck = 1;
                         CheckRtcTime_Programe(&buf[Index_Time]);
                         memcpy(Meter.Num, &buf[Index_VolumeData+1], 7);
                         memset( ((unsigned char *)&CheckResult.MeterTYP), 0xFF, (sizeof(CHECKPRESULT_S) ) );
                         //if (DeviceTool.StatusCheck == chk_Halt)
                         DeviceTool.StatusCheck = chk_Start;
		    }
                    else
                       return;
                        break;
                    default:
                        return;
                }
                break;
            default:break;
        }
        return PCUartDataFormFrame(ctlcode, datamark);
    }
}

/**
*	PC串口数据组帧发送:
*	unsigned char ctlcode:功能码;
*	unsigned short datamark：数据标识；
*   	unsigned char *cmddata:数据缓冲区;
*/
unsigned short PCUartDataFormFrame(unsigned char ctlcode, unsigned short datamark)
{
    unsigned short len = 0;
    unsigned char buf[250];
    unsigned char checksum=0 , i;

    buf[Index_68Frame1] = 0x68;                                              //起始标识
//	memcpy(&buf[Index_MeterID], &Metre.Num[0], 6); //表具编号

    buf[Index_MeterID] = 0xaa;
    buf[Index_MeterID+1] = 0xaa;
    buf[Index_MeterID+2] = 0xaa;
    buf[Index_MeterID+3] = 0xaa;
    buf[Index_MeterID+4] = 0xaa;
    buf[Index_MeterID+5] = 0xaa;

    buf[Index_68Frame2] = 0x68;                                              //标识符
    buf[Index_ControlCode] = ctlcode | 0x80;                                    //功能码
    buf[Index_DataLength] = 0;                                                 //长度高字节
    buf[Index_DataLength+1] = 0;                                                 //长度低字节
    memcpy(&buf[Index_Time], &RTC.Year, 6);
    buf[Index_DiviceType] = DEVICETYPE;                                        //设备类型
    buf[Index_DataMark] = (unsigned char)(datamark & 0x00ff);                //数据标识高
    buf[Index_DataMark+1] = (unsigned char)((datamark >> 8) & 0x00ff);         //数据标识低
    buf[Index_FrameSequence] = 0;                                                 //帧序号
    switch(ctlcode)
    {
        case OPT_READ:                                              //读操作
            switch(datamark)
            {
                case DEV_TIME:
                    len += ReadTimeValue(&buf[Index_VolumeData]);
                break;
                case DEV_GETCHECK_RESULT://查询上告结果
                    if(DeviceTool.StatusCheck == chk_end)
                    {
                        len += ReadCheckResult(&buf[Index_VolumeData]);
                       // LedOrBeepTick((LED_TypeDef)(GREEN_LED) , TICK_COUNT(10), TICK_ON_1MS(10), TICK_OFF_1MS(80)); // add by mjl
                    }
                    else
                    {
                        buf[Index_VolumeData] = CheckResult.DeviceID;
                        len++;
                        memset(&buf[Index_VolumeData+len], 0xff, sizeof(CHECKPRESULT_S));
                        len += sizeof(CHECKPRESULT_S);
                    }
                    len--;
                    break;
                    default:return;
            }
        break;
        case OPT_INSTALL:
             switch(datamark)
            {
                case DEV_START_TEST:
                    buf[Index_VolumeData + len++] = CheckResult.DeviceID;
                    memcpy(&buf[Index_VolumeData+len], Meter.Num, 7);
                    len += 7;
                    break;
                break;
            }
        break;
        default:break;
    }
    len += DataCmdLenth_Front;
    buf[Index_DataLength] = (unsigned char)(len % 256);
    buf[Index_DataLength+1] = (unsigned char)(len >> 8);
    len += 11;
    len = CheckAndFinishCommand(buf, len);

    UsartTxConfig(UP_COM, 1);
    DelayMs(200);
    OM_SendUsartData(UP_COM, buf, len);
    DelayMs(100);
    UsartTxConfig(UP_COM, 0);
    OM_SendUsartData(Debug_COM, DebugBuf, sprintf((char*)DebugBuf,"\r\n测试板-->上位↑↑↑：\r\n"));
    OM_SendUsartData(Debug_COM, DebugBuf, HexToAscii_Space(buf, DebugBuf, len));

    return len;
}


/**
 * 调试串口数据处理解析
 */
void PCUartDataDealProcess(void)
{
    unsigned char fun;
    unsigned char buf[100];
    unsigned short len = 0;

    fun = Get_WX68_Frame(&UpBuf);//侦测串口一帧数据

    if(fun == F_WX_PTR)
    {
        if(UpBuf.RecvCmdBuf[Index_VolumeData] == CheckResult.DeviceID)
        {
            OM_SendUsartData(Debug_COM, DebugBuf, sprintf((char*)DebugBuf,"\r\n上位-->测试板↓↓↓：\r\n"));
            OM_SendUsartData(Debug_COM, DebugBuf, HexToAscii_Space(UpBuf.RecvCmdBuf, DebugBuf, UpBuf.p_RecvCmdData));
            len = PCUartProtocol_DataAanaly(UpBuf.RecvCmdBuf,buf);
        }
    }

}
