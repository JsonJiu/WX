#ifndef __TEST_LIST_H__
#define __TEST_LIST_H__
#include "main.h"

struct Test_quanju_canshu
{
	uint16_t time_softdelay_ms;
	uint32_t time_aroundtest_ms;
	uint8_t test_over;
};
extern struct Test_quanju_canshu Test_quanju_canshu_L;

struct Test_jieguo
{
	uint8_t gongwei;

	uint32_t zhidian_gongdiandianya;
	uint16_t zhidian_caijidianya;
	uint16_t zhudian_gonghao;
	uint32_t SY_dianya_1_1;
	uint32_t SY_dianya_1_2;
	uint32_t SY_dianya_1_3;
	uint32_t SY_dianya_2_1;
	uint32_t SY_dianya_2_2;
	uint32_t SY_dianya_2_3;
	uint32_t VCC_dianya;
	uint8_t  anjian_state;
	uint8_t  anjian_zhuangtai;
	uint8_t  zhukongban_xingshan_MAC[12];

};
extern struct Test_jieguo Test_jiejuo_jilu;

enum Test_liucheng
{
	w_wait = 0,
	//3.3V VCC检测
	w_start,
	//电压供电检测
	w_zhudian_CHK,
	//功耗测试
	w_gonghao_CHK,
	//第一次电压1，2检测
	w_SY_DY1_DY2_CHK,
	//变更输出，一级升压输出
	w_DY1_HIGH_CHANG,
	//第二次电压1，2检测
	w_SY2_DY1_DY2_CHK,
	//变更输出，二级升压输出
	w_DY2_HIGH_CHANG,
	//第三次电压1，2检测
	w_SY3_DY1_DY2_CHK,
	w_DY3_ANJIAN_CHK,
	w_end
};
extern enum Test_liucheng Test_liucheng_L;

enum test_xieyi_jilu
{
	set_get_1008 = 0,
	No_Receive
};
extern enum test_xieyi_jilu test_xieyi_jilu_Rec;

//工位检测
void gongwei_jiance(void);
//开始测试前恢复标志位
void test_start_Init(void);
//开始测试
void test_start(void);
void test_Loop_Func(void);
#endif
