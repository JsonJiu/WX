#include "config.h"
unsigned char test1 = 0x77;
unsigned char test2 = 0x66;
/**
 * 调试串口协议解析
 * @param buf
 */
unsigned short MeterUartProtocol_DataAanaly(unsigned char *buf, unsigned char *buf_temp) {
    unsigned char ctlcode;
    unsigned short datamark;
    unsigned char buf_out[250];
    if (buf[0] == 0x68) {
        datamark = buf[Index_DataMark + 1];
        datamark <<= 8;
        datamark += buf[Index_DataMark];
        ctlcode = buf[Index_ControlCode];
        switch (ctlcode & 0x0f) {
            case OPT_READ://读操作
                switch (datamark) {
                    case DEV_read_IMEI_IMSI_ICCID:
                        FindIMEIandIMSIandICCID_where(&buf[Index_VolumeData]);
                        break;
                    case DEV_READ_CHECK_STATUS: //解析星闪MAC地址
                    memset(CheckResult.StarMac, 0x38, 12);
                    if(buf[Index_DataLength]==0X1F)
                    {
                    ParseStarStatusToCheckResult(&buf[Index_VolumeData]);
                    }
                        break;
                    case DEV_TESTCONNECT://解析第二次1001协议，获取开盖状态
                    {
                        Debug(test1);
                        Debug(buf[Index_VolumeData + 23]);
                        if (CheckResult.Status.CoverOpen == 1 && buf[Index_VolumeData + 23] == 0) //第一次1001协议时，电磁铁关闭，返回的电平是1，第二次1001协议时，电磁铁打开，返回状态是0
                        {
                            CheckResult.Status.CoverOpen = 1;
                            Debug(test1);
                        }
                        else
                        {
                            CheckResult.Status.CoverOpen = 0;
                            Debug(test2);
                        }
                    }
                        break;
                    default:break;
                }
                break;
            case OPT_WRITE://写操作
                switch (datamark) {
                    case DEV_TIME:
                        CheckRtcTime_Programe(&buf[Index_VolumeData]);
                        break;
                    case DEV_TESTCONNECT:
                        Debug(test1);
                        Debug(buf[Index_VolumeData + 23]);
                        DebugArray(buf, 49);
                        if (CheckResult.Status.CoverOpen == 1 && buf[Index_VolumeData + 23] == 0) //第一次1001协议时，电磁铁关闭，返回的电平是1，第二次1001协议时，电磁铁打开，返回状态是0
                        {
                            CheckResult.Status.CoverOpen = 1;
                            Debug(test1);
                        }
                        else
                        {
                            CheckResult.Status.CoverOpen = 0;
                            Debug(test2);
                        }
                        break;
                    case DEV_SETOUTIOSTATUS:
                        if (DeviceTool.FlagSetHightLow == 1) {
                            DeviceTool.FlagCheckProcess = MASTER_CHECK_ONE;
                        } else {
                            DeviceTool.FlagCheckProcess = MASTER_CHECK_TWO;
                        }
                        GetMasterOutIO_Status(DeviceTool.FlagSetHightLow);
                        GetMasterInPutIO_Status(&buf[Index_VolumeData], DeviceTool.FlagSetHightLow);
                        CheckResult.Status.IcCard = 1;
                        break;

                    case DEV_CLOSE_IR:
                        DeviceTool.FlagCheckProcess = MASTER_IR_CLOSED;
                        break;

                    default:break;
                }
                break;
            case OPT_INSTALL:
                switch (datamark) {
                    case DEV_AutoCheckFinish:
                        DeviceTool.FlagMasterBoardSelfCheckFinish = MASTER_SELFCHECK_FINISH;
                        break;
                    default:break;
                }
                break;
            default:break;
        }
        return 0; //MeterUartDataFormFrame(ctlcode, datamark);  //modify by mjl
    }
}

/**
 *	调试串口数据组帧发送:
 *	unsigned char ctlcode:功能码;
 *	unsigned short datamark：数据标识；
 *   	unsigned char *cmddata:数据缓冲区;
 */
unsigned short MeterUartDataFormFrame(unsigned char ctlcode, unsigned short datamark) {
    unsigned short len = 0;
    unsigned char buf[250];
    unsigned char checksum = 0, i;

    buf[Index_68Frame1] = 0x68; //起始标识
    memcpy(&buf[Index_MeterID], &Meter.Num[0], 6); //表具编号

    buf[Index_68Frame2] = 0x68; //标识符
    buf[Index_ControlCode] = ctlcode; //功能码
    buf[Index_DataLength] = 0; //长度高字节
    buf[Index_DataLength + 1] = 0; //长度低字节
    memcpy(&buf[Index_Time], &RTC.Year, 6);
    buf[Index_DiviceType] = DEVICETYPE; //设备类型
    buf[Index_DataMark] = (unsigned char) (datamark & 0x00ff); //数据标识高
    buf[Index_DataMark + 1] = (unsigned char) ((datamark >> 8) & 0x00ff); //数据标识低
    buf[Index_FrameSequence] = 0; //帧序号
    switch (ctlcode) {
        case OPT_READ: //读操作
            switch (datamark) {
                case DEV_TIME:
                    len += ReadTimeValue(&buf[Index_VolumeData]);
                    break;
                case DEV_read_IMEI_IMSI_ICCID: //读取IMEI、IMSI、ICCID
                    break;
                case DEV_READ_CHECK_STATUS: //读取星闪MAC地址
                    break;
                case DEV_TESTCONNECT://读取开盖状态
                    break;
            }
            break;
        case OPT_WRITE: //写操作
            switch (datamark) {
                case DEV_TESTCONNECT:
                    len += SetConnectMasterBoard(&buf[Index_VolumeData]);
                    break;
                case DEV_SETOUTIOSTATUS:
                    len += SetMasterOut(&buf[Index_VolumeData], DeviceTool.FlagSetHightLow);
                    break;
                case DEV_CLOSE_IR:
                    buf[Index_VolumeData] = 0;
                    len++;
                    break;
                default:break;
            }
            break;
        default:break;
    }
    len += DataCmdLenth_Front + 4;
    buf[Index_DataLength] = (unsigned char) (len % 256);
    buf[Index_DataLength + 1] = (unsigned char) (len >> 8);
    len += 11;
    len = CheckAndFinishCommand(buf, len);

    UsartTxConfig(Meter_COM, 1);
    DelayMs(500);
    OM_SendUsartData(Meter_COM, buf, len);
    DelayMs(100);
    UsartTxConfig(Meter_COM, 0);

    OM_SendUsartData(Debug_COM, DebugBuf, sprintf((char*) DebugBuf, "\r\n测试板-->主控板↓↓↓：\r\n"));
    OM_SendUsartData(Debug_COM, DebugBuf, HexToAscii_Space(buf, DebugBuf, len));

    return len;
}

/**
 * 主控板串口数据处理解析
 */
void MeterUartDataDealProcess(void) {
    unsigned char fun;
    unsigned char buf[100];
    unsigned short len = 0;

    fun = Get_WX68_Frame(&MeterBuf); //侦测串口一帧数据

    if (fun == F_WX_PTR) {
        OM_SendUsartData(Debug_COM, DebugBuf, sprintf((char*) DebugBuf, "\r\n主控板-->测试板↑↑↑：\r\n"));
        //这里发生过因读取IMEI号而产生的字符越界的问题，DebugBuf写爆了，因此注释掉
        //OM_SendUsartData(Debug_COM, DebugBuf, HexToAscii_Space(MeterBuf.RecvCmdBuf, DebugBuf, MeterBuf.p_RecvCmdData));

        len = MeterUartProtocol_DataAanaly(MeterBuf.RecvCmdBuf, buf);
    }
}

