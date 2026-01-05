/*
 * @Description:
 * @Author:
 * @Date: 2025-08-06 10:54:46
 * @LastEditors: Please set LastEditors
 * @LastEditTime: 2025-08-26 10:37:04
 */
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
	uint16_t zhidian_jingtai_gonghao;
	uint8_t chuankou_tongxin;
	uint8_t IMEI_CHK[15];
	uint8_t IMSI_CHK[15];
	uint8_t ICCID_CHK[20];
	uint8_t CSQ;
	// 程序版本号
	uint8_t banbenhao[2];
};
extern struct Test_jieguo Test_jiejuo_jilu;

struct Test_linshi_cunchushuju
{
	uint8_t L_flash_chk;
	uint8_t IMEI_CHK[15];
	uint8_t IMSI_CHK[15];
	uint8_t ICCID_CHK[20];
	uint8_t CSQ;
	// 程序版本号
	uint8_t L_banbenhao[2];
};
extern struct Test_linshi_cunchushuju Test_linshi_cunchushuju_L;

enum Test_liucheng
{
	w_wait = 0,
	// 3.3V VCC检测
	w_start,
	// 获取AT指令
	w_getAT,
	w_getATE0,
	w_getATI,
	w_Get_cpin,
	w_Get_QCCID,
	w_Get_GSN,
	w_Get_CIMI,
	w_Get_CSQ,
	w_Guanji,
	// 功耗测试
	w_gonghao_CHK,
	w_end
};
extern enum Test_liucheng Test_liucheng_L;

enum test_xieyi_jilu
{
	// 获取表号
	w_OK,
	w_get_cpin,
	w_get_QCCID,
	w_get_GSN,
	w_get_CIMI,
	w_get_CSQ,
	w_guanji,
	No_Receive
};
extern enum test_xieyi_jilu test_xieyi_jilu_Rec;

void test_Loop_Func(void);

// 工位检测
void gongwei_jiance(void);
// 开始测试前恢复标志位
void test_start_Init(void);
// 开始测试
void test_start(void);
// 循环测试用
void Test_loop_func(void);
#endif
