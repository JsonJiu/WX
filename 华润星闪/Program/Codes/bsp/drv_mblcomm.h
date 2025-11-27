/* 
 * File:   drv_mblcomm.h
 * Author: wb
 *
 * Created on December 28, 2015, 8:59 AM
 */

#ifndef DRV_MBLCOMM_H
#define	DRV_MBLCOMM_H

#ifdef	__cplusplus
extern "C" {
#endif

#define false		    0
#define true		    1
#define eror		    2

    //----------------GSM模块运行操作------------------
#define O_IDLE							0x00
#define O_POWERON						0x01
#define O_POWEROFF						0x02
#define O_RESET							0x03

    //----------------GSMTCP连接操作类型---------------
#define CT_INIT				0x00//初始化
#define CT_CONNECT			0x01//连接TCP服务器
#define CT_DISCONNECT                   0x02//断开连接TCP服务器

#define ST_NONE				0x00//无
#define ST_INITED			0x01//初始化状态
#define ST_CONNECTED                    0x02//连接成功状态
#define ST_CONNECTLOST                  0x03//连接丢失状态
#define ST_CONNECTFAIL                  0x04//连接失败
#define ST_CONNECTERR                   0x05//连接错误

//模块上电的状态
enum STATUS_POWER
{
    S_IDLE,	//空闲
    S_WAITPOWERSUPPLY,//准备提供电源
    S_WAITPOWERON,S_WAITCHECKPOWERON,S_DETECTPOWERON,S_POWERON,S_POWERRESET,//准备开机->开机的过程状态
    S_WAITPOWEROFF,S_WAITPOWERCUT,//关机状态
};

    //AT命令的回应
    enum ACK_AT
    {
	A_NONE, A_OK
    };

//GSM模块设备结构体
typedef struct GSMDevice_ST
{
    unsigned char S_Power;                  //模块上电状态
    unsigned char opt_IdlePowerOn;          //开机操作
    unsigned char opt_PowerOff;             //关机操作
    unsigned char PowerOnTimes;             //开机操作次数

    unsigned char 	S_Device;
    unsigned long 	F_Command;			//设置AT命令的标志
    unsigned char 	SS_Command;			//设置AT命令的状态
    unsigned char 	S_CHECKNET;			//查询网络的状态
    unsigned char 	Count_SendATCmd;        //发送AT命令的次数

}GSMDevice;
extern GSMDevice GsmDevice;

typedef struct _GSMTCP_ //GSM模块TCP通信
{
    unsigned char IpPORT_Type;
    unsigned char Ip[4];				//服务器IP地址
    unsigned short NetPort;			//服务器端口号
    unsigned char UsersName_Len;                    //用户名长度
    char UsersName[30];                             //用户名
    unsigned char MiMa_Len;                         //密码长度
    char MiMa[30];       //密码

    unsigned char opt_Cnnect;			//连接操作
    unsigned char ConnectType;
    unsigned char CntStatus;			//状态
    unsigned char QisrvcVal;

}GSMTCP;
extern GSMTCP GSMTcp;

//设置AT命令的类型(枚举顺序不能随便更改)
enum COMMAND_AT
{
    C_NONE, C_LOWWARM, C_CSQ,  C_YULIU, C_CPIN, C_BAUD, C_QCCID, C_CREG, C_CGREG, C_CGATT,
    C_QIFGCNT, C_QICSGP, C_QIDEACT, C_QIREGAPP, C_QIACT, C_QIOPEN,
    C_QISEND, C_QISACK, C_QICLOSE, C_QISENDDATA, C_AT,
};

#ifdef	__cplusplus
}
#endif

#endif	/* DRV_MBLCOMM_H */

