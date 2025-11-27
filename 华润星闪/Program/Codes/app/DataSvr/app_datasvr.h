#ifndef _APP_DATASVR_H_
#define _APP_DATASVR_H_

/**************************** 配置参数 ************************************/

#define SaveDataSvrNowAddr       1

//#define SaveDataSvrBakAddr       0

#define SaveDataSvrBuffSize         300

/**************************** 配置参数 ************************************/



/****************************** Define ************************************/



typedef void(*FunNoInput)(void);



typedef struct
{
  unsigned short size;            //参数占用空间大小
  unsigned char txt[8];          //参数功能说明,用于上位机识别
}St_RegTxt,*pSt_RegTxt;


/**
 * 参数存储结构体
 */
typedef struct
{
    FunNoInput fun;                 //恢复默认值函数
    unsigned short len;             //存储数据长度，不包含Crc
    unsigned char *data;           //数据指针
}St_SAVESTRUCT;



typedef struct
{
    unsigned char err;
    unsigned char id;
    unsigned short Crc;         //数据内容校验
    St_SAVESTRUCT s;
    pSt_RegTxt regtag;
}St_SAVETASK;

typedef enum
{
    EEP_STATE_IDLE = 0,
    EEP_STATE_INIT,
}t_EEP_STATE;


typedef struct
{
    St_SAVETASK pTaskDataSvr[10];                       //应用存储信息指针
    unsigned short regtagnum[10];                         //应用寄存器说明个数
    unsigned char TaskDataSvrNum;                       //已注册应用个数
    unsigned char Data[SaveDataSvrBuffSize];            //存储缓存区
    unsigned short DataLen;                              //存储有效长度
    unsigned short DataNum;                              //操作位置
    unsigned char NeedLoadBak;
    t_EEP_STATE EEP_State;
    unsigned char NeedSave;
    unsigned long FreezeAddr;       //冻结数据存储地址偏移量
    unsigned long UserAddr;         //用户区地址
}St_SAVESVRRUN;


/****************************** extern ************************************/
extern St_SAVESVRRUN    DataSvrRun;
extern const St_RegTxt  MeterRegTag[];
/***************************** function ***********************************/
void DataSvrDefault(void);


unsigned char DataSvrReg(unsigned short num, St_SAVESTRUCT *s, St_RegTxt *tag);
unsigned char DataSvrSaveAll(void);
unsigned char DataSvrReadAll(void);
void ReadFreezeData(unsigned short add, unsigned char *buf, unsigned short len);
void WriteFreezeData(unsigned short add, unsigned char *buf, unsigned short len);
unsigned char DataSvrResetAll(void);


/***************************** extern function ****************************/
unsigned char GpioHallARead(void);
unsigned char GpioHallBRead(void);
void HallASetInt(unsigned char c,void(*p)(void));
void HallBSetInt(unsigned char c,void(*p)(void));
#endif

