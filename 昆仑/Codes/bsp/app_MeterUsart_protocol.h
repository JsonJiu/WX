/* 
 * File:   app_debugusart_protocol.h
 * Author: wb
 *
 * Created on December 24, 2015, 3:05 PM
 */

#ifndef APP_DEBUGUSART_PROTOCOL_H
#define	APP_DEBUGUSART_PROTOCOL_H

#ifdef	__cplusplus
extern "C" {
#endif

/*********************************************************
协议数据索引---宏
*********************************************************/
#define Index_68Frame1          0     //第一个68的起始索引
#define Index_MeterID           1     //表地址启示索引（6字节）
#define Index_68Frame2          7     //第二个68起始索引
#define Index_ControlCode       8     //控制码索引
#define Index_DataLength        9     //数据长度起始索引
#define DataCmdLenth_Front      10    //数据区命令长度的索引
#define Index_Time              11    //时间的起始索引（6字节）
#define Index_DiviceType        17    //设备类型或设备数量的索引
#define Index_DataMark          18    //数据标识索引
#define Index_FrameSequence     20    //帧序号索引
#define Index_VolumeData        21    //流量数据区域的起始索引
#define Index_OpenDataNew	21
#define SIZE_UPCOMING_PACK      57    //上告流量包的大小
/*********************************************************
设备类型---宏
*********************************************************/
#define DEVICETYPE          (RTUTYPE & 0x0f )//设备类型
#define RTUTYPE             0x08
/*********************************************************
控制码---宏
*********************************************************/
#define OPT_READ                0x01
#define OPT_WRITE               0x04
#define OPT_INSTALL             0x05
#define OPT_LOADLINK		0x08

/*********************************************************
数据标识---宏
*********************************************************/
#define DEV_TIME                0xc621  //时间

#define DEV_AutoCheckFinish     0x1000  //自检完成
#define DEV_TESTCONNECT         0x1001  //测试连接
#define DEV_SETOUTIOSTATUS      0x1002  //设置端口输出状态
#define DEV_CLOSE_IR            0x1005  //关闭红外
#define DEV_read_IMEI_IMSI      0xC518  //读取IMEI
#define DEV_read_ESAMID         0xC519  //读取ESAMID
#define DEV_read_ICCID          0xC525  //读取IMEI

#define DEV_START_TEST          0xfc03  //启动测试
#define DEV_GETCHECK_RESULT     0xfc04  //查询测试结果


unsigned short MeterUartProtocol_DataAanaly(unsigned char *buf, unsigned char *buf_temp);
unsigned short MeterUartDataFormFrame(unsigned char ctlcode, unsigned short datamark);
void MeterUartDataDealProcess(void);

#ifdef	__cplusplus
}
#endif

#endif	/* APP_DEBUGUSART_PROTOCOL_H */

