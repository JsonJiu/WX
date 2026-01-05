#ifndef __GPIO_H__
#define __GPIO_H__
#include "main.h"

void INA219_IIC_INIT(void);
void Others_GPIO_Init(void);
void LED_On(void);
void LED_Off(void);
void zhudian_dianya_CHK_CTRL_ON(void);
void zhudian_dianya_CHK_CTRL_OFF(void);
void erji_dianya_CHK_CTRL_ON(void);
void erji_dianya_CHK_CTRL_OFF(void);
void NB_Current_CHK_CTRL_ON(void);
void NB_Current_CHK_CTRL_OFF(void);
//主电电流检测使能开
void Current_CHK_CTRL_ON(void);
//主电电流检测使能关
void Current_CHK_CTRL_OFF(void);
//霍尔1控制 开
void huuoer_1_ON(void);
//霍尔1控制 关
void huuoer_1_OFF(void);
//霍尔2控制 开
void huuoer_2_ON(void);
//霍尔2控制 关
void huuoer_2_OFF(void);
//按键控制 开(拉低)
void anjian_kongzhi_ON(void);
//按键控制 关(拉高)
void anjian_kongzhi_OFF(void);
//计量IO1置高
void jiliang_IO1_set_Hi(void);
//计量IO1置低
void jiliang_IO1_set_Low(void);
//计量IO2置高
void jiliang_IO2_set_Hi(void);
//计量IO2置低
void jiliang_IO2_set_Low(void);
//脉冲IO11读取状态
uint32_t maichong_IO11_get_state(void);
//脉冲IO12置高
void maichong_IO12_set_Hi(void);
//脉冲IO12置低
void maichong_IO12_set_Low(void);
//脉冲IO13读取状态
uint32_t maichong_IO13_get_state(void);
//开到位置高
void kaidaowei_set_Hi(void);
//开到位置低
void kaidaowei_set_Low(void);
//关到位置高
void guandaowei_set_Hi(void);
//关到位置低
void guandaowei_set_Low(void);
//阀控通信引脚置高
void fakong_tongxin_set_Hi(void);
//阀控通信引脚置低
void fakong_tongxin_set_Low(void);
//脉冲输出1引脚置高
void maichong_shuchu_1_set_Hi(void);
//脉冲输出1引脚置低
void maichong_shuchu_1_set_Low(void);
//脉冲输出2引脚置高
void maichong_shuchu_2_set_Hi(void);
//脉冲输出2引脚置低
void maichong_shuchu_2_set_Low(void);
//断线IO3引脚置低
void duanxian_shuchu_IO3_set_Low(void);
//断线IO3引脚置高
void duanxian_shuchu_IO3_set_Hi(void);
//断线IO4引脚置低
void duanxian_shuchu_IO4_set_Low(void);
//断线IO4引脚置高
void duanxian_shuchu_IO4_set_Hi(void);
//温压供电检测
uint32_t wenya_IO5_get_state(void);
//3.3V输出引脚拉高
void V3_3_OPT_shuchu_set_Hi(void);
//3.3V输出引脚拉低
void V3_3_OPT_shuchu_set_Low(void);
//开盖检测输出引脚拉高IO6
void kaigai_OPT_shuchu_set_Hi(void);
//开盖检测输出引脚拉低IO6
void kaigai_OPT_shuchu_set_Low(void);
//调试口串口通信控制拉低 PB7
void tiaoshi_chuankou_tongxin_set_Low(void);
//调试口串口通信控制拉高 PB7
void tiaoshi_chuankou_tongxin_set_Hi(void);
//NB电压读取控制拉低 PB10
void NB_dianya_duqu_set_Low(void);
//NB电压读取控制拉高 PB10
void NB_dianya_duqu_set_Hi(void);
//VDD电压读取控制 PB10
void VDD_dianya_duqu_set_Low(void);
//VDD电压读取控制 PB10
void VDD_dianya_duqu_set_Hi(void);
//VDD电压读取控制拉低 PB11
void V119_dianya_duqu_set_Low(void);
//VDD电压读取控制拉高 PB11
void V119_dianya_duqu_set_Hi(void);
//通信复位控制拉高 PC13
void tongxin_fuwei_set_Hi(void);
//通信复位控制拉低 PC13
void tongxin_fuwei_set_Low(void);
//通信唤醒控制 PC14
void tongxin_huanxing_set_Hi(void);
//通信唤醒控制拉低 PC14
void tongxin_huanxing_set_Low(void);
//通信电源控制拉低 PC15
void tongxin_dianyuan_set_Hi(void);
//通信电源控制拉高 PC15
void tongxin_dianyuan_set_Low(void);
//屏幕背光控制 PE5
void pingmu_beiguang_set_Hi(void);
//屏幕背光控制拉低 PE5
void pingmu_beiguang_set_Low(void);
//NB电源供电控制 PC0
void NB_dianyuan_gongdian_On(void);
//NB电源供电控制 PC0
void NB_dianyuan_gongdian_Off(void);
//阀门正极电压检测控制高 PC1
void FM_zhengji_dianyuan_gongdian_On(void);
//阀门正极电压检测控制低 PC1
void FM_zhengji_dianyuan_gongdian_Off(void);
//阀控_调试_唤醒开启 PC2
void FK_tiaoshi_chuankou_huanxing_On(void);
//阀控_调试_唤醒关闭 PC2
void FK_tiaoshi_chuankou_huanxing_Off(void);
//阀门断线检测拉高 PC3
void FK_duanxia_CHK_On(void);
//阀门断线检测拉低 PC3
void FK_duanxia_CHK_Off(void);
//3.6V供电控制开启 PC10
void V3_6_gongdian_kaiguan_On(void);
//3.6V供电控制关闭 PC10
void V3_6_gongdian_kaiguan_Off(void);
//阀门负极电压检测控制高 PH15
void FM_fuji_dianyuan_gongdian_On(void);
//阀门负极电压检测控制低 PH15
void FM_fuji_dianyuan_gongdian_Off(void);
void MF_EXTI_Config_Init(void);
void GPIO_IRQHandler(void);
#endif
