#ifndef __TEST_LIST_H__
#define __TEST_LIST_H__
#include "main.h"

struct Test_quanju_canshu
{
	uint16_t time_softdelay_ms;
	uint32_t time_aroundtest_ms;
};
extern struct Test_quanju_canshu Test_quanju_canshu_L;

struct Test_jieguo
{
	uint8_t gongwei;
	uint16_t zhudian_gonghao;
};
extern struct Test_jieguo Test_jiejuo_jilu;

enum Test_liucheng
{
	w_wait = 0,
	//功耗测试
	w_gonghao_CHK,
	w_end
};
extern enum Test_liucheng Test_liucheng_L;

enum test_xieyi_jilu
{
	connect_xingshan = 0,
	shanggao_zhengchang,
	No_Receive
};
extern enum test_xieyi_jilu test_xieyi_jilu_Rec;

void test_Loop_Func(void);

//开始测试前恢复标志位
void test_start_Init(void);
//开始测试
void test_start(void);
#endif
