#include "include.h"

St_SAVESVRRUN   DataSvrRun;


unsigned char datasvrtempi;




///**
// * 加载目前使用的参数到内存
// * 
// * @author xsc (2014/11/14)
// */
//void DataSvrLoadNow(void)
//{
//    OI_EEInit();
//    OI_EERead(SaveDataSvrNowAddr, DataSvrRun.Data, DataSvrRun.DataLen, 0xA0);
//    OI_EEIdle();
//}


///**
// * 加载备份参数到内存
// * 
// * @author xsc (2014/11/14)
// */
//void DataSvrLoadBak(void)
//{
//    OI_EEInit();
//    OI_EERead(SaveDataSvrNowAddr + DataSvrRun.DataLen, DataSvrRun.Data, DataSvrRun.DataLen, 0xA0);
//    OI_EEIdle();
//}

/**
 * 备份数据
 * 
 * @author xsc (2014/11/16)
 */
void DataSvrWriteFlashBak(void)
{
    unsigned char area;
    
    OI_EERead(SaveDataSvrNowAddr - 1, &area, 1, 0xA0);
    area = 1 - area;
    OI_EEWrite(SaveDataSvrNowAddr - 1, &area, 1, 0xA0);
    
//    DataSvrRun.DataNum = 0;
//    for(i = 0; i < DataSvrRun.TaskDataSvrNum ; i++)
//    {
//        OI_EERead(SaveDataSvrNowAddr + DataSvrRun.DataNum, DataSvrRun.Data, DataSvrRun.pTaskDataSvr[i].s.len + 2, 0xA0);
//        OI_EEWrite(SaveDataSvrNowAddr + DataSvrRun.DataLen + DataSvrRun.DataNum, DataSvrRun.Data, DataSvrRun.pTaskDataSvr[i].s.len + 2, 0xA0);
//        DataSvrRun.DataNum += DataSvrRun.pTaskDataSvr[i].s.len + 2;
//    }
    
}

///**
// * 写入当前数据
// *
// * @author xsc (2014/11/16)
// */
//void DataSvrWriteFlashNow(void)
//{
//    OI_EEInit();
//    OI_EEWrite(SaveDataSvrNowAddr, DataSvrRun.Data, DataSvrRun.DataLen, 0xA0);
//    OI_EEIdle();
//}


/**
 * 查找指定任务序号的存储位置
 * 
 * @author xsc (2014/11/14)
 * 
 * @param id 
 * 
 * @return unsigned short 
 */
unsigned short FindId(unsigned char id)
{
    unsigned char i;
    unsigned short n;
    if (DataSvrRun.TaskDataSvrNum == 0)
    {
        return 0xFF;
    }
    n = 0;
    for (i = 1; i < id; i++)
    {
        n += DataSvrRun.pTaskDataSvr[i].s.len + 2;
    }
    return n;

}


/**
 * 获取存储缓冲区读取数据
 * 
 * @author xsc (2014/11/16)
 * 
 * @param s 应用指针
 * @param c 读取当前/备份
 */
unsigned char GetData(St_SAVETASK *s, unsigned char c)
{
    unsigned short crc;
    unsigned long temp;
    unsigned char area;
    
    s->err = 0;
    
    OI_EERead(SaveDataSvrNowAddr - 1, &area, 1, 0xA0);  //最新数据存储地址判断
    
    temp = DataSvrRun.DataNum;
    if(c - area)
        temp += SaveDataSvrNowAddr + DataSvrRun.DataLen;
    else
        temp += SaveDataSvrNowAddr;
    
    OI_EERead(temp, DataSvrRun.Data, s->s.len + 2, 0xA0);
    
    s->Crc = DataSvrRun.Data[s->s.len];
    s->Crc <<= 8;
    s->Crc += DataSvrRun.Data[s->s.len + 1];
    crc = Crc16Get(DataSvrRun.Data, s->s.len);
    DataSvrRun.DataNum += s->s.len + 2;
    if (s->Crc == crc)
    {
//        for (j = 0; j < s->s.len; j++)
//        {
//            s->s.data[j] = DataSvrRun.Data[DataSvrRun.DataNum++];
//        }
        copy_memery(s->s.data, DataSvrRun.Data, s->s.len);
        return TRUE;
    }else{
        s->err = 1;
        DataSvrRun.NeedLoadBak = 1;
        return FALSE;
    }
}



/**
 * 保存数据到存储器
 * 
 * @author xsc (2014/11/16)
 * 
 * @param s 
 */
unsigned char SaveData(St_SAVETASK *s, unsigned char c)
{
    unsigned long temp;
    unsigned char area;
    unsigned char tbuf[300];
    unsigned char tcrc;
    
    OI_EERead(SaveDataSvrNowAddr - 1, &area, 1, 0xA0);  //最新数据存储地址判断
    
    temp = DataSvrRun.DataNum;
    
    if(c - area)
        temp += SaveDataSvrNowAddr + DataSvrRun.DataLen;
    else
        temp += SaveDataSvrNowAddr;
    
    copy_memery(DataSvrRun.Data, s->s.data, s->s.len);
    s->Crc = Crc16Get(s->s.data, s->s.len);
    DataSvrRun.Data[s->s.len] = (unsigned char)(s->Crc >> 8);
    DataSvrRun.Data[s->s.len + 1] = (unsigned char)(s->Crc);
    
    OI_EEWrite(temp, DataSvrRun.Data, s->s.len + 2, 0xA0);
    OI_EERead(temp, tbuf, s->s.len + 2, 0xA0);
    tcrc = Crc16Get(tbuf, s->s.len);
    DataSvrRun.DataNum += s->s.len + 2;
    if (tcrc != s->Crc)
    {
        return FALSE;
    }else{
        return TRUE;
    }
}



/**
 * 加载指定任务的存储数据
 * 
 * @author xsc (2015-04-17)
 * 
 * @param id 任务ID 
 * @param s 应用指针 
 * @param c 读取当前/备份 
 * 
 * @return unsigned char 结果 0:失败；1:成功；
 */
unsigned char ReadDataTask(unsigned char id, St_SAVETASK *s, unsigned char c)
{
    DataSvrRun.DataNum = FindId(id);
    return GetData(s, c);
}

/**
 * 保存指定任务数据
 * 
 * @author xsc (2015-04-21)
 * 
 * @param id 任务id
 * 
 * @return unsigned char 保存结果
 */
unsigned char SaveDataTask(unsigned char id)
{
    DataSvrRun.DataNum = FindId(id);
    return SaveData(&DataSvrRun.pTaskDataSvr[id], 0);
}


/**
 * 应用申请存储空间 
 * 根据应用编号，分配存储空间 
 * 
 * @author xsc (2014/11/17)
 * 
 * @param s 任务寄存器指针 
 *          例如:
 * typedef struct 
 * { 
 *  unsigned char freq;
 *  unsigned short rate;
 *  unsigned long dev;
 * }ST_RFCONFIG;
 *  
 * ST_RFCONFIG rfconfig; 
 * St_SAVESTRUCT  rfconfigsave; 
 * rfconfig.fun = RFDefaultSet; 
 * rfconfig.data = &rfconfig.freq; 
 * rfconfig.len = sizeof(rfconfigsave);
 * 
 * @param tag 任务寄存器说明指针 
 *  
 * @param num 寄存器个数 
 *  
 *  
 * 格式为[数据长度][说明字符 串] 数据长度  char:1  short:2 
 * long:4 double:4/8(由编译器决定sizeof所 得 ) 
 *  
 *          例如:
 *          const St_RegTxt rfregtag[] = {
 *          {sizeof(ST_RFCONFIG.freq),{"freq"},
 *          {sizeof(ST_RFCONFIG.rate),{"rate"},
 *          {sizeof(ST_RFCONFIG.dev),{"dev"}
 *          };
 *          
 *  saveid = DataSvrReg(1, rfconfig, rfregtag);
 * 
 * @return unsigned char 返回已注册任务数
 */
unsigned char DataSvrReg(unsigned short num, St_SAVESTRUCT *s, St_RegTxt *tag)
    if (s->len > SaveDataSvrBuffSize) {
        while (1) {
            ; //任务存储空间 超过 存储任务缓存大小
        }
    }
    unsigned long tasksize = 0,j;
    unsigned short i;
    unsigned char *pchar;
    St_RegTxt *pt;

    for(i = 0; i < num; i++)
    {
        j = sizeof(St_RegTxt) * i;
        pchar = (unsigned char *)tag;
        pt = (St_RegTxt *)(pchar + j);
        tasksize += pt->size;
    }
    if (s->len != tasksize)
    {
        while (1)
        {
            ;//说明尺寸和实际尺寸不一致，请检查说明结构体的数据长度相加是否等于实际变量存储长度；
        }
    }

    copy_memery((unsigned char *)&DataSvrRun.pTaskDataSvr[DataSvrRun.TaskDataSvrNum].s, (unsigned char *)s, sizeof(St_SAVESTRUCT));
    //copy_memery((unsigned char *)&DataSvrRun.pTaskDataSvr[DataSvrRun.TaskDataSvrNum].regtag, (unsigned char *)tag, sizeof(St_RegTxt));
    DataSvrRun.pTaskDataSvr[DataSvrRun.TaskDataSvrNum].regtag = tag;
    
    DataSvrRun.regtagnum[DataSvrRun.TaskDataSvrNum] = num;
    DataSvrRun.TaskDataSvrNum++;
    DataSvrRun.DataLen += s->len + 2;
    return DataSvrRun.TaskDataSvrNum;
}

/**
 * 保存全部 
 * 将旧数据备份并保存新数据 
 * 
 * @author xsc (2014/11/17)
 * 
 * @return unsigned char 
 */
unsigned char DataSvrSaveAll(void)
{
    unsigned char savetimes = 1;
    if (DataSvrRun.TaskDataSvrNum == 0)
        return 0xFF;
    if(GetSysPowState() == STATUS_POW_NORMAL)
    {
        savetimes = 2;
    }
    OI_EEInit();
    while (savetimes--)
    {
        DataSvrWriteFlashBak();
        DataSvrRun.DataNum = 0;
        for (datasvrtempi = 0; datasvrtempi < DataSvrRun.TaskDataSvrNum; datasvrtempi++)
        {
            SaveData(&DataSvrRun.pTaskDataSvr[datasvrtempi], 0);
        }
    }
    OI_EEIdle();
    //DataSvrWriteFlashNow();
    return TRUE;
}


void LoadDefault(void)
{
    unsigned char i;
  
    for (i = 0; i < DataSvrRun.TaskDataSvrNum; i++)
    {
        DataSvrRun.pTaskDataSvr[i].s.fun();
    }
}

/**
 * 数据读取全部 
 * 尝试读取最新数据至应用内存，若某应用数据错误，自动读取备份数据 
 * 
 * @author xsc (2014/11/14)
 * 
 * @return unsigned char 0xFF任务数为零
 */
unsigned char DataSvrReadAll(void)
{
    if (DataSvrRun.TaskDataSvrNum == 0)
        return 0xFF;
    DataSvrRun.FreezeAddr = DataSvrRun.DataLen * 2 + 3;
    DataSvrRun.UserAddr = DataSvrRun.FreezeAddr + 24 * sizeof(StuFreezeData) + 3;
    OI_EEInit();
    DataSvrRun.DataNum = 0;
    DataSvrRun.NeedSave = 0;

    for (datasvrtempi = 0; datasvrtempi < DataSvrRun.TaskDataSvrNum; datasvrtempi++)
    {
        GetData(&DataSvrRun.pTaskDataSvr[datasvrtempi], 0);
    }

    DataSvrRun.DataNum = 0;
    if (DataSvrRun.NeedLoadBak)
    {
        for (datasvrtempi = 0; datasvrtempi < DataSvrRun.TaskDataSvrNum; datasvrtempi++)
        {
            if (DataSvrRun.pTaskDataSvr[datasvrtempi].err)
            {
                GetData(&DataSvrRun.pTaskDataSvr[datasvrtempi], 1);
                if (DataSvrRun.pTaskDataSvr[datasvrtempi].err)
                {
                    DataSvrRun.NeedSave = 1;
                    mem_set(DataSvrRun.pTaskDataSvr[datasvrtempi].s.data,DataSvrRun.pTaskDataSvr[datasvrtempi].s.len, 0);
                    DataSvrRun.pTaskDataSvr[datasvrtempi].s.fun();
                }
            }else{
                DataSvrRun.DataNum += DataSvrRun.pTaskDataSvr[datasvrtempi].s.len + 2;
            }
        }
    }
    DataSvrSaveAll();

    OI_EEIdle();

    DataSvrLoaded(); //任务加载后触发的时间
    return TRUE;
}

/**
 * 所有任务恢复默认参数
 * 
 * @author xsc (2014/11/14)
 * 
 * @return unsigned char TRUE
 */
unsigned char DataSvrResetAll(void)
{
    if (DataSvrRun.TaskDataSvrNum == 0)
        return 0xFF;
    DataSvrRun.FreezeAddr = DataSvrRun.DataLen * 2 + 3;
    DataSvrRun.UserAddr = DataSvrRun.FreezeAddr + 24 * sizeof(StuFreezeData) + 3;
    OI_EEInit();
    DataSvrRun.DataNum = 0;
    DataSvrRun.NeedSave = 0;

    for (datasvrtempi = 0; datasvrtempi < DataSvrRun.TaskDataSvrNum; datasvrtempi++)
    {
        DataSvrRun.pTaskDataSvr[datasvrtempi].s.fun();
    }

    DataSvrSaveAll();

    OI_EEIdle();

    return TRUE;
}



/**
 * 自定义存储区域读
 * 
 * @author xsc (2015-02-06)
 */
void DataSvrUserRead(unsigned long add, unsigned char *buf, unsigned short len)
{
    OI_EEInit();
    OI_EERead(DataSvrRun.UserAddr + add, buf, len, 0xA0);
    OI_EEIdle();
}

/**
 * 自定义存储区域写
 * 
 * @author xsc (2015-02-06)
 */
void DataSvrUserWrite(unsigned long add, unsigned char *buf, unsigned short len)
{
    OI_EEInit();
    OI_EEWrite(DataSvrRun.UserAddr + add, buf, len, 0xA0);
    OI_EEIdle();
}


//
///**
// * 读取指定任务的最新存储数据
// *
// * @author xsc (2014/11/16)
// *
// * @param id 任务ID号
// * @param s 存储的寄存器
// *
// * @return unsigned char 0xFF:数据校验失败;0x00~0xFE:有效长度
// */
//unsigned char DataSvrReadNow(unsigned char id,  St_SAVESTRUCT *s)
//{
//    unsigned char i;
//    unsigned short crc1,crc2;
//
//    DataSvrRun.DataNum = FindId(id);
//
//    //DataSvrLoadNow();
//    GetData(s);
//
////    for (i = 0; i < s->len; i++)
////    {
////        s->data[i] = DataSvrRun.Data[DataSvrRun.DataNum++];
////    }
////    crc2 = DataSvrRun.Data[DataSvrRun.DataNum++];
////    crc2 <<= 8;
////    crc2 += DataSvrRun.Data[DataSvrRun.DataNum++];
////    crc1 = Crc16Get(s->data, s->len);
////    if (crc1 == crc2)
////    {
////        return s->len;
////    }else{
////        return 0xFF;
////    }
//}


///**
// * 读取指定任务的备份数据
// *
// * @author xsc (2014/11/16)
// *
// * @param id 任务ID号
// * @param s 存储的寄存器
// *
// * @return unsigned char 0xFF:数据校验失败;0x00~0xFE:有效长度
// */
//unsigned char DataSvrReadBak(unsigned char id,  St_SAVESTRUCT *s)
//{
//    unsigned char i;
//    unsigned short crc1,crc2;
//
//    DataSvrRun.DataNum = FindId(id);
//
//    DataSvrLoadBak();
//
//    for (i = 0; i < s->len; i++)
//    {
//        s->data[i] = DataSvrRun.Data[DataSvrRun.DataNum++];
//    }
//    crc2 = DataSvrRun.Data[DataSvrRun.DataNum++];
//    crc2 <<= 8;
//    crc2 += DataSvrRun.Data[DataSvrRun.DataNum++];
//    crc1 = Crc16Get(s->data, s->len);
//    if (crc1 == crc2)
//    {
//        return s->len;
//    }else{
//        return 0xFF;
//    }
//}

/**
 * 读取冻结数据内容
 * 
 * @author xsc (2014/12/9)
 * 
 * @param add 偏移地址
 * @param buf 缓冲区
 * @param len 长度
 */
void ReadFreezeData(unsigned short add, unsigned char *buf, unsigned short len)
{
    OI_EEInit();
    OI_EERead(DataSvrRun.FreezeAddr + add, buf, len, 0xA0);
    OI_EEIdle();
}

/**
 * 保存冻结数据内容
 * 
 * @author xsc (2014/12/9)
 * 
 * @param add 偏移地址
 * @param buf 缓冲区
 * @param len 长度
 */
void WriteFreezeData(unsigned short add, unsigned char *buf, unsigned short len)
{
    OI_EEInit();
    OI_EEWrite(DataSvrRun.FreezeAddr + add, buf, len, 0xA0);
    OI_EEIdle();
}


