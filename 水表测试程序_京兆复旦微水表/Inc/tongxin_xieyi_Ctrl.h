#ifndef __TONGXIN_XIEYI_CTRL_H__
#define __TONGXIN_XIEYI_CTRL_H__
#include "main.h"
void TONGXIN_xieyijiexi(uint8_t zufuchua[],uint16_t lenth);
void TONGXIN_xieyifasong(uint16_t shujubiaoshi,uint8_t kongzhima,uint8_t w_biaohao[],uint8_t shujuyu[],uint16_t shujuyu_changdu);
void find_biaohao_xieyi(void);
void send_shanggao_xieyi(void);
void find_banbenhao_xieyi(void);
void FM_Open_xieyi(void);
void Test_zhuanyong_xieyi(void);
//主控板获取当前无磁采样信号强度
void get_wucicaiyang_xieyi(void);
//获取主控板获取的无磁采样信号强度
void get_wucicaiyang_zhukongban_xieyi(void);
#endif
