#ifndef __TONGXIN_XIEYI_CTRL_H__
#define __TONGXIN_XIEYI_CTRL_H__
#include "main.h"
void TONGXIN_xieyijiexi(uint8_t zufuchua[],uint16_t lenth);
void TONGXIN_xieyifasong(uint16_t shujubiaoshi,uint8_t kongzhima,uint8_t w_biaohao[],uint8_t shujuyu[],uint16_t shujuyu_changdu);
void find_biaohao_xieyi(void);
void send_shanggao_xieyi(void);
void FM_Test_xieyi(void);
void send_chaxun_xieyi(void);
void guan_hongwai_xieyi(void);
void kaifa_xieyi(void);
void guanfa_xieyi(void);
void yali_enable_xieyi(void);
void Get_Famen_Status(void);
void Get_yali_Status(void);
void find_banbenhao_xieyi(void);

void Read_biaohao_xieyi(uint8_t zufuchua[],uint16_t lenth);
void get_wucicaiyang_xieyi(void);
//获取主控板获取的无磁采样信号强度
void get_wucicaiyang_zhukongban_xieyi(void);
#endif
