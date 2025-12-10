#include "app_irdausart_protocl.h"
#include "config.h"

/**
 * 调试串口协议解析
 * @param buf
 */
unsigned short IrdaUartProtocol_DataAanaly(unsigned char *buf, unsigned char *buf_temp)
{
    unsigned char ctlcode;
    unsigned short datamark;
    unsigned char buf_out[250];
    if(buf[0] == 0x68)
    {
        datamark = buf[Index_DataMark+1];
        datamark <<= 8;
        datamark += buf[Index_DataMark];
        ctlcode = buf[Index_ControlCode];
        switch(ctlcode&0x0f)
        {
            case OPT_READ://读操作
            switch(datamark)
            {
                case DEV_READ_CHECK_STATUS:  //解析星闪MAC地址
                    memset(CheckResult.StarMac, 0x38, 12);
                    if(buf[Index_DataLength]==0X1F)
                    {
                    ParseStarStatusToCheckResult(&buf[Index_VolumeData]);
                    }
                break;
                default:break;
            }
        break;
            case OPT_WRITE://写操作
                switch(datamark)
                {
                    case DEV_TIME:
                        CheckRtcTime_Programe(&buf[Index_VolumeData]);
                    break;
                    case DEV_TESTCONNECT:
                        CheckResult.Status.Irda = 1;
                        RecvConnectResult(&buf[Index_VolumeData]);
                        DeviceTool.FlagCheckProcess = MASTER_CONNCET_CHECK;
                        OM_SendUsartData(Debug_COM, DebugBuf, sprintf((char*)DebugBuf,"\r\n========红外接收数据成功!=========\r\n"));
                    break;
                    case DEV_SETOUTIOSTATUS:
                        if(DeviceTool.FlagSetHightLow == 1)
                        {
                            DeviceTool.FlagCheckProcess = MASTER_CHECK_ONE;
                        }
                        else
                        {
                            DeviceTool.FlagCheckProcess = MASTER_CHECK_TWO;
                        }
                        GetMasterOutIO_Status(DeviceTool.FlagSetHightLow);
                        GetMasterInPutIO_Status(&buf[Index_VolumeData], DeviceTool.FlagSetHightLow);
                        break;

                    case DEV_CLOSE_IR:
                        DeviceTool.FlagCheckProcess = MASTER_IR_CLOSED;
                        break;

                    default:break;
                }
                break;
            case OPT_INSTALL:
                switch(datamark)
                {
                    case DEV_AutoCheckFinish:
                        DeviceTool.FlagMasterBoardSelfCheckFinish = MASTER_SELFCHECK_FINISH;
                        break;
                    default:break;
                }
                break;
            default:break;
        }
        return 0; //IrdaUartDataFormFrame(ctlcode, datamark);
    }
}

/**
*	调试串口数据组帧发送:
*	unsigned char ctlcode:功能码;
*	unsigned short datamark：数据标识；
*   	unsigned char *cmddata:数据缓冲区;
*/
unsigned short IrdaUartDataFormFrame(unsigned char ctlcode, unsigned short datamark)
{
    unsigned short len = 0;
    unsigned char buf[250];
    unsigned char checksum=0 , i;

    buf[Index_68Frame1] = 0x68;                                              //起始标识
//	memcpy(&buf[Index_MeterID], &Metre.Num[0], 6); //表具编号
    buf[Index_68Frame2] = 0x68;                                              //标识符
    buf[Index_ControlCode] = ctlcode;                                    //功能码
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
                    //len += ReadTimeValue(&buf[Index_VolumeData]);
                break;
                case DEV_READ_CHECK_STATUS:  //读取星闪MAC地址
                break;
            }
        break;
        case OPT_WRITE:                                             //写操作
            switch(datamark)
            {
                case DEV_TESTCONNECT:
                    len += SetConnectMasterBoard(&buf[Index_VolumeData]);
                    break;
                case DEV_SETOUTIOSTATUS:
                    len += SetMasterOut(&buf[Index_VolumeData], DeviceTool.FlagSetHightLow);
                    break;
                default:break;
            }
        break;
        default:break;
    }
    len += DataCmdLenth_Front + 4;
    buf[Index_DataLength] = (unsigned char)(len % 256);
    buf[Index_DataLength+1] = (unsigned char)(len >> 8);
    len += 11;
    len = CheckAndFinishCommand(buf, len);
    OM_SendUsartData(IR_COM, buf, len);
    OM_SendUsartData(Debug_COM, DebugBuf, sprintf((char*)DebugBuf,"\r\n红外发送：测试板-->主控板 ↓↓↓：\r\n"));
    OM_SendUsartData(Debug_COM, DebugBuf, HexToAscii_Space(buf, DebugBuf, len));
    return len;
}

/**
 * 调试串口数据处理解析
 */
void IrdaUartDataDealProcess(void)
{
    unsigned char fun;
    unsigned char *buf;
    unsigned short len = 0;
    fun = Get_WX68_Frame(&IrdaBuf );//侦测串口一帧数据

    if(fun == F_WX_PTR)
    {
        OM_SendUsartData(Debug_COM, DebugBuf, sprintf((char*)DebugBuf,"\r\n红外接收：主控板-->测试板 ↑↑↑：\r\n"));
        OM_SendUsartData(Debug_COM, DebugBuf, HexToAscii_Space(IrdaBuf.RecvCmdBuf, DebugBuf, IrdaBuf.p_RecvCmdData));

        len = IrdaUartProtocol_DataAanaly(IrdaBuf.RecvCmdBuf, buf);
    }
}
