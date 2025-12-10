#include "include.h"



#define CMD_HEAD_LEN    17 + 6
#define CMD_NODATA_LEN  CMD_HEAD_LEN + 2




/**
 * 读取寄存器数据
 * 
 * @author xsc (2014/11/20)
 * 
 * @param buf 返写缓存
 * @param start 开始地址
 * @param len 长度
 * 
 * @return unsigned short 返写有效长度
 */
unsigned short ReadReg(unsigned char *buf, unsigned short start, unsigned short len)
{
    unsigned short temp = start + len;
    unsigned short i, j;
    unsigned short num;
    unsigned short tempaddr;
    unsigned char sub = 0;
    pStCmd p = (pStCmd)buf;
    
    
    if(temp > (DataSvrRun.DataLen - DataSvrRun.TaskDataSvrNum * 2))
    {
        len = DataSvrRun.DataLen - DataSvrRun.TaskDataSvrNum * 2 - start;
    }
    
    
    p->Cmd |= 0x10;
    p->LenLo = (unsigned char)(len & 0xFF);
    p->LenHi = (unsigned char)((len >> 8) & 0xFF);
    
    Uart0SendData(buf,6);
    sub += CrcAddGet(buf + 1, 5);

    
    temp = 0;
    tempaddr = 0;
    num = 0;
    for (i = 0; i < DataSvrRun.TaskDataSvrNum ; i++) {
        if ((temp + DataSvrRun.pTaskDataSvr[i].s.len) >= start && tempaddr == 0) {
            tempaddr = start - temp + 1;
            num = 0;
        }else{
            if(tempaddr == 0) {
                temp += DataSvrRun.pTaskDataSvr[i].s.len;
            }else{
                tempaddr = 1;
            }
        }
        for (j = tempaddr - 1; j < DataSvrRun.pTaskDataSvr[i].s.len; j++) {
            p->data[num] = DataSvrRun.pTaskDataSvr[i].s.data[j];
            Uart0SendData(&p->data[num], 1);
            sub += p->data[num];
            temp++;
            num++;
            if (num == len) {  //tempaddr - 1 + temp - start
                p->data[num] = sub;
                Uart0SendData(&sub, 1);
                sub = 0x16;
                Uart0SendData(&sub, 1);
                return len;
            }
        }
        //Uart0SendData(&p->data[num - j], j);

    }
    Uart0SendData(&sub, 1);
    sub = 0x16;
    Uart0SendData(&sub, 1);
    return len;
}

/**
 * 保存寄存器
 * 
 * @author xsc (2014/11/20)
 * 
 * @param buf 数据缓存
 * @param start 开始地址
 * @param len 长度
 * 
 * @return unsigned short 返回有效长度
 */
unsigned short WriteReg(unsigned char *buf, unsigned short start, unsigned short len)
{
    unsigned short temp = 0;
    unsigned short i, j;
  
    for (i = 0; i < DataSvrRun.TaskDataSvrNum; i++) {
        if ((temp + DataSvrRun.pTaskDataSvr[i].s.len) > start) {

            temp = start - temp;
            for (j = 0; j < len; j++) {
                DataSvrRun.pTaskDataSvr[i].s.data[temp] = buf[j];
                temp++;
                if (temp > DataSvrRun.pTaskDataSvr[i].s.len) {
                    i++;
                    temp = 0;
                }
            }
            break;
        }
        temp += DataSvrRun.pTaskDataSvr[i].s.len;
    }
    buf[0] = 1;
    return 1;
}


void WriteRegOne(unsigned char *buf, unsigned short start, unsigned short len)
{
    
}



extern Stu_Set SetCache;

/**
 * 
 * 
 * @author xsc (2014/11/20) 
 * 
 * @return unsigned short 
 */
unsigned short GetTag(pStCmd p)
{
    unsigned char i, j;
    unsigned short templen = 0;
    unsigned char *ptemp;
    unsigned char sub = 0;
    unsigned short addr = 0;
    extern const unsigned char MeterName[];
    pSt_RegTxt preg;

    p->Cmd |= 0x10;     //修改方向标志
    //计算数据长度
    for (i = 0; i < DataSvrRun.TaskDataSvrNum; i++) {
        templen += DataSvrRun.regtagnum[i] * (sizeof(St_RegTxt) + 2);//地址+长度+说明
    }
    templen += 30;
    p->LenHi = (templen >> 8) & 0xFF;
    p->LenLo = templen & 0xFF;

    Uart0SendData((unsigned char *)p,6);
    sub += CrcAddGet(SetCache.SetCmd + 1, 5);
    
    
    Uart0SendData((unsigned char *)MeterName,30);
    sub += CrcAddGet((unsigned char *)MeterName, 30);
    
    
    //发送内容
    for (i = 0; i < DataSvrRun.TaskDataSvrNum; i++) {
        ptemp = (unsigned char *)DataSvrRun.pTaskDataSvr[i].regtag;
        for (j = 0; j < DataSvrRun.regtagnum[i]; j++) {
            preg = (pSt_RegTxt)ptemp;
            Uart0SendData((unsigned char *)&addr, sizeof(addr));
            sub += CrcAddGet((unsigned char *)&addr, sizeof(addr));
            sub += CrcAddGet((unsigned char *)ptemp, sizeof(St_RegTxt));
            Uart0SendData(ptemp, sizeof(St_RegTxt));
            
            addr += preg->size;
            ptemp += sizeof(St_RegTxt);
        }
    }
    Uart0SendData(&sub, 1);
    sub = 0x16;
    Uart0SendData(&sub, 1);
    return 0;
}


/**
 * 建立帧头
 * 
 * @author 谢尚川 (2014-03-31)
 * 
 * @param buf 原帧缓存
 * @param buf 除校验和和结束符后的长度
 * 
 * @return unsigned char 帧总长度
 */
unsigned short MakeSend(unsigned char *buf, unsigned short len)
{
    unsigned short crc;
    pStCmd CmdPoint;
    CmdPoint = (pStCmd)buf;

    CmdPoint->Cmd |= 0x10;
    CmdPoint->LenHi = (len >> 8) & 0xFF;
    CmdPoint->LenLo = len & 0xFF;
    crc = CrcAddGet(buf + 1, len + 5);
    buf[len + 6] = crc;
    buf[len + 7] = 0x16;

    return len + 8;
}


unsigned char tempsum;
StWriteRegCache WriteRegCache;

unsigned char CheckSysCommand(unsigned char *buf, unsigned short *len)
{
    unsigned short l;
    unsigned char i, sum = 0;
    pStCmd CmdPoint;
    CmdPoint = (pStCmd)buf;


    if (buf[0] == 0x68) {           //起始标志
        if (*len > 2 && (buf[1] != 0x03 && buf[1] != 0x06 && buf[1] != 0x08 && buf[1] != 0x01 )) {
            return 1;
        }
        if (*len >= 6) {
            if (buf[1] == 0x06) {   //若为写，特殊处理
                if(*len == 6)
                {
                    l = CmdPoint->LenHi;
                    l = (l << 8) + CmdPoint->LenLo;
                    WriteRegCache.datalen = l;
                    l = CmdPoint->AddrHi;
                    l = (l << 8) + CmdPoint->AddrLo;
                    WriteRegCache.addr = l;
                    WriteRegCache.datalennow = 0;
                    WriteRegCache.sub = CrcAddGet(buf + 1, 5);
                }else{
                    if(WriteRegCache.datalennow >= WriteRegCache.datalen)       //写入数据接收完成
                    {
                        if(WriteRegCache.sub == CmdPoint->data[0])
                        {
                            //DataSvrSaveAll();
                            return 0xFE;
                        }else{
                            DataSvrReadAll();
                            return *len;
                        }
                    }
                    if(WriteRegCache.datalennow < WriteRegCache.datalen)
                    {
                        *len = 6;
                        WriteRegCache.sub += CmdPoint->data[0];
                        WriteReg(CmdPoint->data, WriteRegCache.addr, 1);
                    }
                    WriteRegCache.datalennow++;
                    WriteRegCache.addr++;
                }
                
            }else{
                l = 0;
            }
            if (*len >= (l + 8)) {
                for(i = 1; i < l + 6; i++)
                {
                    sum += buf[i];
                }
                if(sum == buf[l + 6]){
                    return 0xFE;    //数据正确
                }else{
                    return *len;    //校验失败
                }
            }else{
                return 0xFF;        //数量不足
            }
        }else{
            return 0xFF;            //数量不足
        }
    }else{
        return 1;                   //起始标志错误
    }
    
}




/**
 * 解析指令
 * 
 * @author 谢尚川 (2014/3/25)
 * 
 * @param buf 数据缓存
 *  
 * @param len 数据长度
 * 
 * @return unsigned char  需返回长度
 */
unsigned short RunSysCommand(unsigned char *CmdBuff)
{
    pStCmd CmdPoint = (pStCmd)CmdBuff;
    unsigned short len = (CmdPoint->LenHi << 8) + CmdPoint->LenLo;
    unsigned short addr = (CmdPoint->AddrHi << 8) + CmdPoint->AddrLo;
    unsigned short result = 0;

    switch (CmdPoint->Cmd) {
    case 0x01:  //保存
        DataSvrSaveAll();
        CmdPoint->data[0] = 1;
        result = 1;
        break;
    case 0x03:  //读寄存器值
        ReadReg(CmdBuff, addr, len);
        break;

    case 0x06:  //写寄存器值
        CmdPoint->data[0] = 1;
        result = 1;//WriteReg(CmdPoint->data, addr, len);
        break;

    case 0x08:  //获取寄存器说明
        GetTag(CmdPoint);
        break;
    }
    return result;
}



