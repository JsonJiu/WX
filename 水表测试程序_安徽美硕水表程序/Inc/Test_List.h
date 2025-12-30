#ifndef __TEST_LIST_H__
#define __TEST_LIST_H__
#include "main.h"

struct Test_quanju_canshu
{
	uint16_t time_softdelay_ms;
	uint32_t time_aroundtest_ms;
	uint16_t danbu_chaoshishijian_ms;
	uint8_t test_over;
};
extern struct Test_quanju_canshu Test_quanju_canshu_L;

struct Test_jieguo
{
	uint8_t gongwei;
	uint32_t zhidian_dianya_gongdian;
	uint32_t zhidian_dianya_huoqu;
	uint16_t zhidian_jingtai_gonghao;
	uint16_t zhidian_manshui_gonghao;
	uint16_t zhidian_zoushui_gonghao;
	uint32_t beidian_dianya_gongdian;
	uint32_t beidian_dianya_huoqu;
	uint16_t beidian_gonghao;
	uint8_t  lanya_jiance;
	uint8_t  flash_jiance;
	uint8_t  jiliang_jiance;
	uint8_t  hongwai_jiance;
	uint8_t  IMEI_CHK[15];
	uint8_t  IMSI_CHK[15];
	uint8_t  ICCID_CHK[20];
	uint8_t  CSQ;
	uint8_t  FM1;
	uint8_t  FM2;
	uint8_t  FM;
	uint8_t  FM_daowei_1;
	uint8_t  FM_daowei_2;
	uint8_t  FM_daowei;
	uint8_t  EEPROM_jiance;
	uint32_t GP30_dianya;
	uint8_t  loraEUI[16];
	uint8_t  qiangci_jiance;
	uint8_t  qiangci_jiance_1;
	uint8_t  qiangci_jiance_2;
	uint8_t  kaigai_jiance;
	uint8_t  GPSmozu_jiacne;
	//需要存储两次计量的值用于比较
	uint8_t jiliang_1;
	uint8_t jiliang_2;
	//用于指示本次测试是否结束
	//考虑到协议通信中没有通用表号，这里需要存一下
	uint8_t  dangqian_biaohao[6];
	//初始化测试获取的数据
	uint8_t  mokuaihao[6];
	//程序校验码
	uint8_t jiaoyanma[2];
	//程序版本号
	uint8_t banbenhao[2];
};
extern struct Test_jieguo Test_jiejuo_jilu;


struct Test_linshi_cunchushuju
{
	uint8_t L_flash_chk;
	uint16_t L_zhudian_dianya;
	uint16_t L_beidian_dianya;
	uint8_t L_yali_CHK;
	uint8_t L_EEPROM_CHK;
	uint8_t L_huoer1_CHK;
	uint8_t L_GPRS_mozu_CHK;
	uint8_t L_huoer2_CHK;
	uint8_t L_chaosheng_shunshiliuliang[4];
	uint8_t L_chaosheng_zousuizhuangtai;
	uint16_t L_GP30mozu_AD_CHK;
	uint8_t L_IMEI[15];
	uint8_t L_IMSI[15];
	uint8_t L_ICCID[20];
	uint8_t L_CSQ;
	uint8_t lorakey[16];
	uint8_t L_daowei1;
	uint8_t L_daowei2;
	uint8_t L_jilianghuoer1;
	uint8_t L_jilianghuoer2;
	uint8_t L_wucijiliang;
	//程序版本号
	uint8_t L_banbenhao[2];
};
extern struct Test_linshi_cunchushuju Test_linshi_cunchushuju_L;

enum Test_liucheng
{
	w_wait = 0,
	//3.3V VCC检测
	w_reset,
	w_reset_OK,
	w_start,
	//功耗测试
	w_gonghao_CHK,
	//电磁铁唤醒红外
	w_diancitie_huanxing_irda,
	//获取表号
	get_biaohao,
	//获取版本号
	get_banbenhao,
	//主动上告
	w_zhudong_shangg,
	//第一次查询上告结果。
	w_shangg_jieguo_start,
	//查询上告结果，等待上告结束。期间会进行走气。
	w_shangg_jieguo,
	//阀门测试
	w_famenceshi,
	//测试结束
	w_end
};
extern enum Test_liucheng Test_liucheng_L;

enum test_xieyi_jilu
{
	//获取表号
	w_get_biaohao,
	//获取版本号,
	w_get_banbenhao,
	// 上告
	w_get_shanggao,
	//阀门动作
	w_get_famen_dongzuo,
	//测试专用
	w_get_test_zhuanyong,
	
	No_Receive
};
extern enum test_xieyi_jilu test_xieyi_jilu_Rec;

void test_Loop_Func(void);

//工位检测
void gongwei_jiance(void);
//开始测试前恢复标志位
void test_start_Init(void);
//开始测试
void test_start(void);
//循环测试用
void Test_loop_func(void);
#endif
