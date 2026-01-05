#ifndef __TONGXIN_XIEYI_CTRL_H__
#define __TONGXIN_XIEYI_CTRL_H__
#include "main.h"
void TONGXIN_xieyijiexi(uint8_t zufuchua[],uint16_t lenth);
void TONGXIN_xieyifasong(uint16_t shujubiaoshi,uint8_t kongzhima,uint8_t w_biaohao[],uint8_t shujuyu[],uint16_t shujuyu_changdu);
uint8_t bijiao_zifuchuan(uint8_t bijiao1[],uint8_t bijiao2[],uint16_t lenth);
void find_biaohao_xieyi(void);
void send_shanggao_xieyi(void);
void FM_Open_xieyi(void);
void Test_zhuanyong_xieyi(void);
//获取当前版本号
void find_banbenhao_xieyi(void);
#endif
