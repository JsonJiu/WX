#include "Test_List.h"
#include "LED_CTRL.h"
#include "GPIO.h"
#include "ZDINA219.h"
#include "uart0.h"
#include "uart1.h"
#include "ADC_CHK.h"
#include "tongxin_xieyi_Ctrl.h"
#include "jiliang_xieyi_Ctrl.h"
#include "tongxin_xieyi_irda_Ctrl.h"
#include "exti.h"

struct Test_quanju_canshu Test_quanju_canshu_L;
enum Test_liucheng Test_liucheng_L = w_wait;
struct Test_jieguo Test_jiejuo_jilu;
struct Test_linshi_cunchushuju Test_linshi_cunchushuju_L;
enum test_xieyi_jilu test_xieyi_jilu_Rec = No_Receive;
enum test_xieyi_jilu_irda test_xieyi_jilu_irda_rec = h_No_Receive;
uint8_t guanhongwai_jishu = 0;
uint32_t delay_4s = 0;
extern uint8_t EXTIFlag;
uint8_t dayinbiaohaoyong[12];

void test_quanju_canshu_Init()
{
	Test_quanju_canshu_L.time_softdelay_ms = 10;
}
//测试参数初始化
void test_start_Init()
{
	//工装放上就会上电
	zhudian_gongdian_On();
	//供压力传感器电压打开
	beidian_gongdian_On();
	//主电电压检测控制关闭
	zhudian_dianya_CHK_CTRL_OFF();
	//二级电源电压检测控制关闭
	erji_dianya_CHK_CTRL_OFF();
	//计量方式选择
	jiliang_fangshi_select();
	//LED会亮一下代表初始化完成
	LED_FLAG_Run();
	//开到位信号高  默认开到位  OPEN=0  CLOSE=1
	kai_daowei();
}
//工位检测
void gongwei_jiance()
{
	if(FL_GPIO_GetInputPin(GPIOC,FL_GPIO_PIN_0)==0 && FL_GPIO_GetInputPin(GPIOD,FL_GPIO_PIN_12)==0)
	{
		Test_jiejuo_jilu.gongwei = 0x00;
	}
	else if(FL_GPIO_GetInputPin(GPIOC,FL_GPIO_PIN_0)==0 && FL_GPIO_GetInputPin(GPIOD,FL_GPIO_PIN_12)==1)
	{
		Test_jiejuo_jilu.gongwei = 0x01;
	}
	else if(FL_GPIO_GetInputPin(GPIOC,FL_GPIO_PIN_0)==1 && FL_GPIO_GetInputPin(GPIOD,FL_GPIO_PIN_12)==0)
	{
		Test_jiejuo_jilu.gongwei = 0x02;
	}
	else if(FL_GPIO_GetInputPin(GPIOC,FL_GPIO_PIN_0)==1 && FL_GPIO_GetInputPin(GPIOD,FL_GPIO_PIN_12)==1)
	{
		Test_jiejuo_jilu.gongwei = 0x03;
	}
}

void jiliang_fangshi_select(void)
{
	if(FL_GPIO_GetInputPin(GPIOE,FL_GPIO_PIN_5)==0)
	{
		Test_jiejuo_jilu.jiliang_fangshi = youci;   //有磁
		RS2103_POWER_ON();
		RS2103_Sel_youci();
		//有磁计量初始化，两个霍尔电平不能相同
		SENSOR_P_Low();
		SENSOR_D_High();
		SENSOR_T_High();
	}
	else if(FL_GPIO_GetInputPin(GPIOE,FL_GPIO_PIN_5)==1)
	{
		Test_jiejuo_jilu.jiliang_fangshi = wuci;   //无磁
		//RS2103_POWER_ON();
		//RS2103_Sel_wuci();
	}

}
//测试结果初始化
void test_jieguo_qingling()
{
	Test_jiejuo_jilu.zhidian_dianya_gongdian = 0;
	Test_jiejuo_jilu.zhidian_dianya_huoqu = 0;
	Test_jiejuo_jilu.zhidian_jingtai_gonghao = 0;
	Test_jiejuo_jilu.zhidian_manshui_gonghao = 0;
	Test_jiejuo_jilu.zhidian_zoushui_gonghao = 0;
	Test_jiejuo_jilu.beidian_dianya_gongdian = 0;
	Test_jiejuo_jilu.beidian_dianya_huoqu = 0;
	Test_jiejuo_jilu.beidian_gonghao = 0;
	Test_jiejuo_jilu.lanya_jiance = 0;
	Test_jiejuo_jilu.flash_jiance = 0;
	Test_jiejuo_jilu.jiliang_jiance = 0;
	Test_jiejuo_jilu.hongwai_jiance = 0;
	memset(Test_jiejuo_jilu.IMEI_CHK,0x00,15);
	memset(Test_jiejuo_jilu.IMSI_CHK,0x00,15);
	memset(Test_jiejuo_jilu.ICCID_CHK,0x00,20);
	Test_jiejuo_jilu.FM1 = 0;
	Test_jiejuo_jilu.FM2 = 0;
	Test_jiejuo_jilu.FM = 0;
	Test_jiejuo_jilu.FM_daowei_1 = 0;
	Test_jiejuo_jilu.FM_daowei_2 = 0;
	Test_jiejuo_jilu.FM_daowei = 0;
	Test_jiejuo_jilu.EEPROM_jiance = 0;
	Test_jiejuo_jilu.GP30_dianya = 0;
	memset(Test_jiejuo_jilu.loraEUI,0x00,16);
	Test_jiejuo_jilu.qiangci_jiance = 0;
	Test_jiejuo_jilu.qiangci_jiance_1 = 0;
	Test_jiejuo_jilu.qiangci_jiance_2 = 0;
	Test_jiejuo_jilu.kaigai_jiance = 0;
	Test_jiejuo_jilu.GPSmozu_jiacne = 0;
	Test_quanju_canshu_L.test_over = 0;
	memset(Test_jiejuo_jilu.dangqian_biaohao,0x00,6);
	Test_jiejuo_jilu.jiliang_1_1 = 0;
	Test_jiejuo_jilu.jiliang_1_2 = 0;
	Test_jiejuo_jilu.jiliang_2_1 = 0;
	Test_jiejuo_jilu.jiliang_2_2 = 0;
	Test_jiejuo_jilu.FM1_dianya = 0;
	Test_jiejuo_jilu.FM2_dianya = 0;
	Test_jiejuo_jilu.FM_Status = 0;
	Test_jiejuo_jilu.YaLi_Status = 0;
	memset(Test_jiejuo_jilu.jiaoyanma,0x00,2);
	memset(Test_jiejuo_jilu.banbenhao,0x00,2);
	
	EXTIFlag = 0;
	//接收数据标志位清除
	test_xieyi_jilu_Rec = No_Receive;
	test_xieyi_jilu_irda_rec = h_No_Receive;
}
//开始测试
void test_start()
{
	//IO口恢复
	test_start_Init();
	//测试结果清零
	test_jieguo_qingling();
	Test_liucheng_L = w_start;
	//最大测试时间100秒
	Test_quanju_canshu_L.time_aroundtest_ms = 100000;
	//单步超时时间
	Test_quanju_canshu_L.danbu_chaoshishijian_ms = 3000;  //工装板电压检测时间3s
	Test_quanju_canshu_L.test_over = 0;
	Test_quanju_canshu_L.time_softdelay_ms = 0;
	PC_Chuankou_tongxin_Debug_send(Test_jiejuo_jilu.mokuaihao,6);
}
//终止测试
//终止测试的条件是，测试时间过长，或是主控板离开了工装。
void test_testend()
{
	Test_liucheng_L = w_end;
	Test_quanju_canshu_L.test_over = 1;
	Test_quanju_canshu_L.time_softdelay_ms = 0;
}
//测试过程中的额外异常事件
void test_err_end_Func()
{
	//测试超时
	if((Test_quanju_canshu_L.time_aroundtest_ms==0||Test_quanju_canshu_L.danbu_chaoshishijian_ms==0)&&Test_quanju_canshu_L.test_over==0)
	{
		//结束测试
		test_testend();
	}
}



/*  检测过程步骤
1.检测主控板供电是否正常
3.检测水表提供压力传感器电压
4.查询表号
5.启动上告
6.第一次查询  得到有磁/无磁计量默认参数和阀门到位参数以及其他参数  有磁计量获得三个字节表示磁干扰和两个霍尔，默认参数是1   无磁计量只有一个字节，计量值
7.第二次查询  得到得到有磁置0参数和阀门到位参数
8.启动阀门测试 如果是有磁计量，则将参数都置0
2.检测静态功耗(因为手动开红外会导致功耗变大，所以把功耗检测放到最后一步)
*/

void test_Loop_Func()
{
	test_err_end_Func();
	//Test_liucheng_L = w_gonghao_CHK;
	if(Test_quanju_canshu_L.time_softdelay_ms>0)
		return;
	switch(Test_liucheng_L)
	{
		case w_wait:
			//仅等待下一次测试
			break;
		case w_start:
			//开始测试前校验VDD是否有电，从而判断测试是否开始。
		  Test_jiejuo_jilu.zhidian_dianya_gongdian = get_zhukongban_gongdian_dianya();    //工装板主电电压检测
		  //测试合格条件判断
		  if(Test_jiejuo_jilu.zhidian_dianya_gongdian>3200&&Test_jiejuo_jilu.zhidian_dianya_gongdian<4000)    //如果工装板3.6V内部点满足3.2V-4V则符合条件
			{
				//测试合格，进入下一步
				Test_quanju_canshu_L.time_softdelay_ms = 0;
				//重置单步超时时间
	      Test_quanju_canshu_L.danbu_chaoshishijian_ms = 20000;
				Test_liucheng_L = w_gonghao_CHK;
			}
			else
			{
				//否则1秒复测一次
				Test_quanju_canshu_L.time_softdelay_ms = 1000;
			}
			break;
			case w_gonghao_CHK:
			Test_jiejuo_jilu.zhidian_jingtai_gonghao = Current_CHK_Func(0);      //测试静态功耗
			if(Test_jiejuo_jilu.zhidian_jingtai_gonghao>4&&Test_jiejuo_jilu.zhidian_jingtai_gonghao<30)  //功耗在2uA-30uA之内测试合格 
			{
				//测试合格，进入下一步
				Test_quanju_canshu_L.time_softdelay_ms = 0;
				//重置单步超时时间 10秒
	      Test_quanju_canshu_L.danbu_chaoshishijian_ms = 10000;
				Test_liucheng_L = get_biaohao; 
			}
			else
			{
				//否则1秒复测一次
				Test_quanju_canshu_L.time_softdelay_ms = 1000;
			}
			break;
		case get_biaohao:
		  if(test_xieyi_jilu_Rec == w_get_biaohao)	
			{
				test_xieyi_jilu_Rec = No_Receive;
				//测试合格，进入下一步
				Test_quanju_canshu_L.time_softdelay_ms = 0;
				//重置单步超时时间 10秒
	      Test_quanju_canshu_L.danbu_chaoshishijian_ms = 10000;
				Test_liucheng_L = get_banbenhao;
			}
			else
			{
				//发送协议
				find_biaohao_xieyi();
				//每2秒重发一次（这是远红外，所以可以）
				Test_quanju_canshu_L.time_softdelay_ms = 3000;
			}
			break;
		case get_banbenhao:
		  if(test_xieyi_jilu_Rec == w_get_banbenhao)
			{
				test_xieyi_jilu_Rec = No_Receive;
				memcpy(Test_jiejuo_jilu.banbenhao,Test_linshi_cunchushuju_L.L_banbenhao,2);
				
				//测试合格，进入下一步
				Test_quanju_canshu_L.time_softdelay_ms = 1000;
				//重置单步超时时间 10秒
	      Test_quanju_canshu_L.danbu_chaoshishijian_ms = 15000;
				Test_liucheng_L = w_famen_chaxun;
			}
			else
			{
				DeBug_print("发送读版本号协议\r\n");
				//发送协议
				find_banbenhao_xieyi();
				//每2秒重发一次（这是远红外，所以可以）
				Test_quanju_canshu_L.time_softdelay_ms = 2000;
			}
			break;
		case w_famen_chaxun:
			if(test_xieyi_jilu_Rec == w_get_famen)	
			{
				test_xieyi_jilu_Rec = No_Receive;
				//测试合格，进入下一步
				Test_quanju_canshu_L.time_softdelay_ms = 0;
				//重置单步超时时间 10秒
	      Test_quanju_canshu_L.danbu_chaoshishijian_ms = 20000;
				Test_liucheng_L = w_yali_chaxun;
			}
			else
			{
				//发送协议
				Get_Famen_Status();
				//每2秒重发一次（这是远红外，所以可以）
				Test_quanju_canshu_L.time_softdelay_ms = 3000;
			}
			break;
		case w_yali_chaxun:
			if(test_xieyi_jilu_Rec == w_get_yali_status)	
			{
				test_xieyi_jilu_Rec = No_Receive;
				//测试合格，进入下一步
				Test_quanju_canshu_L.time_softdelay_ms = 0;
				//重置单步超时时间 10秒
	      Test_quanju_canshu_L.danbu_chaoshishijian_ms = 10000;//主动上告时间20s
				Test_liucheng_L = w_zhudong_shangg;
			}
			else
			{
				//发送协议
				Get_yali_Status();
				//每2秒重发一次（这是远红外，所以可以）
				Test_quanju_canshu_L.time_softdelay_ms = 3000;
			}
			break;
		case w_zhudong_shangg:
			if(test_xieyi_jilu_Rec == w_get_shanggao)
			{
				delay_4s = 15000;//上告10秒后才能读数据
				test_xieyi_jilu_Rec = No_Receive;
				//测试合格，进入下一步，这里的延时是考虑到下一步通信一致导致的通信异常
				Test_quanju_canshu_L.time_softdelay_ms = 2000;
				//重置单步超时时间 20秒
	      Test_quanju_canshu_L.danbu_chaoshishijian_ms = 20000;
				if(Test_jiejuo_jilu.YaLi_Status == youyali)
				{
					Test_liucheng_L = w_yali_jiance;	
				}
				if(Test_jiejuo_jilu.YaLi_Status == wuyali)
				{
					if(Test_jiejuo_jilu.FM_Status == youfa)
					{
						Test_liucheng_L = w_famen_guan;
					}
					else if(Test_jiejuo_jilu.FM_Status == wufa)
					{
						Test_liucheng_L = w_chaxun_jieguo_first;
					}
				}
			}
			else
			{
				//发送协议
				send_shanggao_xieyi();
				//每2秒重发一次（这是远红外，所以可以）
				Test_quanju_canshu_L.time_softdelay_ms = 2000;			
			}
			break;
		case w_yali_jiance:
			if(test_xieyi_jilu_Rec == w_get_yali)    
			{
				//测试合格，进入下一步
				Test_quanju_canshu_L.time_softdelay_ms = 0;
				//重置单步超时时间
	      Test_quanju_canshu_L.danbu_chaoshishijian_ms = 20000;
				if(Test_jiejuo_jilu.FM_Status == youfa) //如果是有阀
				{
					Test_liucheng_L = w_famen_guan;
				}
				else if(Test_jiejuo_jilu.FM_Status == wufa)  //如果是无阀
				{
					Test_liucheng_L = w_chaxun_jieguo_first;
				}
			}
			else
			{
				yali_enable_xieyi();
				//否则1秒复测一次
				Test_quanju_canshu_L.time_softdelay_ms = 2000;
			}
			break;
		case w_famen_guan:
			Test_jiejuo_jilu.FM1_dianya = get_famen_2_dianya();
			if(test_xieyi_jilu_Rec == w_get_famen_dongzuo && Test_jiejuo_jilu.FM1_dianya>1800)
			{
				test_xieyi_jilu_Rec = No_Receive;
				//测试合格，进入下一步，这里的延时是考虑到下一步通信一致导致的通信异常
				Test_quanju_canshu_L.time_softdelay_ms = 2000;
				//重置单步超时时间 10秒
	      Test_quanju_canshu_L.danbu_chaoshishijian_ms = 20000;
				Test_liucheng_L = w_chaxun_jieguo_first;	
			}
			else
			{
				//发送协议
				guanfa_xieyi();
				//每2秒重发一次（这是远红外，所以可以）
				Test_quanju_canshu_L.time_softdelay_ms = 2000;			
			}
			break;
		
		case w_chaxun_jieguo_first:
			if(test_xieyi_jilu_Rec == w_get_test_zhuanyong) 
			{
				test_xieyi_jilu_Rec = No_Receive;
					//压力状态（协议里没加，改为温度）  映射到蓝牙
						if(Test_linshi_cunchushuju_L.L_yali_CHK == 1 )
					{
						Test_jiejuo_jilu.yali_jiance = 1;
					}
					//如果是无磁计量，则直接判断无磁计量值是否大于0，大于0则表示计量正常
					if(Test_jiejuo_jilu.jiliang_fangshi == wuci && Test_linshi_cunchushuju_L.L_wucijiliang>0)
					{
						Test_jiejuo_jilu.jiliang_1_2=Test_linshi_cunchushuju_L.L_wucijiliang;
					}
					//测试合格，进入下一步
					Test_quanju_canshu_L.time_softdelay_ms = 0;
					//重置单步超时时间 10秒
					Test_quanju_canshu_L.danbu_chaoshishijian_ms = 20000;//第二次查询结果时间10s
					if(Test_jiejuo_jilu.FM_Status == youfa) //如果有阀
					{
						Test_liucheng_L = w_famen_kai;  
					}
					else if(Test_jiejuo_jilu.FM_Status == wufa) //如果无阀
					{
						Test_liucheng_L = w_chaxun_jieguo_second;  
					}
			}
			else if(delay_4s == 0)
			{
				//发送协议
				send_chaxun_xieyi();
				//每2秒重发一次（这是远红外，所以可以）
				Test_quanju_canshu_L.time_softdelay_ms = 2000;
			}
			break;
		case w_famen_kai:
			Test_jiejuo_jilu.FM2_dianya = get_famen_1_dianya();
			if(test_xieyi_jilu_Rec == w_get_famen_dongzuo && Test_jiejuo_jilu.FM2_dianya>1800)
			{
				test_xieyi_jilu_Rec = No_Receive;
				Test_jiejuo_jilu.FM = 1;
				//测试合格，进入下一步，这里的延时是考虑到下一步通信一致导致的通信异常
				Test_quanju_canshu_L.time_softdelay_ms = 2000;
				//重置单步超时时间 10秒
	      Test_quanju_canshu_L.danbu_chaoshishijian_ms = 10000;
				Test_liucheng_L = w_chaxun_jieguo_second;	
			}
			else
			{
				//发送协议
				kaifa_xieyi();
				//每2秒重发一次（这是远红外，所以可以）
				Test_quanju_canshu_L.time_softdelay_ms = 2000;			
			}
			break;
		case w_chaxun_jieguo_second: //第二次查询只负责校验有磁计量和阀门到位
			if(test_xieyi_jilu_Rec == w_get_test_zhuanyong) 
			{
				test_xieyi_jilu_Rec = No_Receive;
				
					if(Test_jiejuo_jilu.jiliang_fangshi == wuci && Test_linshi_cunchushuju_L.L_wucijiliang!=Test_jiejuo_jilu.jiliang_1_2)
					{
						Test_jiejuo_jilu.jiliang_jiance = 1;
					}
					//对测试结果进行映射
					//flash
					Test_jiejuo_jilu.flash_jiance = Test_linshi_cunchushuju_L.L_flash_chk;
					//获取的主电电压
					Test_jiejuo_jilu.zhidian_dianya_huoqu = Test_linshi_cunchushuju_L.L_zhudian_dianya;
					//获取的备电电压
					Test_jiejuo_jilu.beidian_dianya_huoqu = Test_linshi_cunchushuju_L.L_beidian_dianya;
					//eeprom获取
					Test_jiejuo_jilu.EEPROM_jiance = Test_linshi_cunchushuju_L.L_EEPROM_CHK;
					//模组检测
					Test_jiejuo_jilu.GPSmozu_jiacne = Test_linshi_cunchushuju_L.L_GPRS_mozu_CHK;
					//开盖获取
					Test_jiejuo_jilu.kaigai_jiance = Test_linshi_cunchushuju_L.L_kaigai_CHK;
					//GP30电压获取
					Test_jiejuo_jilu.GP30_dianya = Test_linshi_cunchushuju_L.L_GP30mozu_AD_CHK;
					//IMEI
					memcpy(Test_jiejuo_jilu.IMEI_CHK,Test_linshi_cunchushuju_L.L_IMEI,15);
					//IMSI
					memcpy(Test_jiejuo_jilu.IMSI_CHK,Test_linshi_cunchushuju_L.L_IMSI,15);
					//ICCID
					memcpy(Test_jiejuo_jilu.ICCID_CHK,Test_linshi_cunchushuju_L.L_ICCID,20);
					//CSQ
					Test_jiejuo_jilu.CSQ = Test_linshi_cunchushuju_L.L_CSQ;
					//lora
					memcpy(Test_jiejuo_jilu.loraEUI,Test_linshi_cunchushuju_L.lorakey,16);				
					//测试合格，进入下一步
					Test_quanju_canshu_L.time_softdelay_ms = 0;
					//重置单步超时时间 20秒
					Test_quanju_canshu_L.danbu_chaoshishijian_ms = 20000;
					Test_liucheng_L = w_get_wuci_xinhao1;  //如果有阀门，则Test_liucheng_L = w_famenceshi
			}
			else
			{
				//让电机转动十秒
				huuoer_2_ON();	     
			  FL_DelayMs(10000);
	     huuoer_2_OFF();
				//发送协议
				send_chaxun_xieyi();
				//每2秒重发一次（这是远红外，所以可以）
				Test_quanju_canshu_L.time_softdelay_ms = 2000;
			}
			break;
			case w_get_wuci_xinhao1:
			if(test_xieyi_jilu_Rec == w_set_wuci_xinhao_huoqu)
			{
				test_xieyi_jilu_Rec = No_Receive;
				//测试合格，进入下一步
				Test_quanju_canshu_L.time_softdelay_ms = 2000;
				//重置单步超时时间 10秒
	      Test_quanju_canshu_L.danbu_chaoshishijian_ms = 10000;
				Test_liucheng_L = w_get_wuci_xinhao2;
			}
			else
			{
				//电机开始工作
				huuoer_2_ON();
				
				DeBug_print("获取无磁模组信号强度至主控板\r\n");
				//发送协议
				get_wucicaiyang_xieyi();
				//每2秒重发一次（这是远红外，所以可以）
				Test_quanju_canshu_L.time_softdelay_ms = 3000;
			}
			break;
			case w_get_wuci_xinhao2:
			if(test_xieyi_jilu_Rec == w_get_zhukongban_wuci_xinhao)
			{
				test_xieyi_jilu_Rec = No_Receive;
				Test_jiejuo_jilu.wucixinhaoqiangdu = Test_linshi_cunchushuju_L.L_wucijiliang_xinhaoqiangdu;
				//测试合格，进入下一步
				Test_quanju_canshu_L.time_softdelay_ms = 2000;
				//重置单步超时时间 10秒
	      Test_quanju_canshu_L.danbu_chaoshishijian_ms = 10000;
				
				if(Test_jiejuo_jilu.wucixinhaoqiangdu>14)
				{
					huuoer_2_OFF();
					Test_liucheng_L = w_end;
				}
				else
				{
					Test_liucheng_L = w_get_wuci_xinhao1;
				}
				
			}
			else
			{
				
				DeBug_print("获取无磁模组信号强度\r\n");
				//发送协议
				get_wucicaiyang_zhukongban_xieyi();
				//每2秒重发一次（这是远红外，所以可以）
				Test_quanju_canshu_L.time_softdelay_ms = 3000;
			}
			break;
		case w_end:
			//一切测试都已结束，打开测试返回
		  Test_quanju_canshu_L.test_over = 1;
		  //回到第一步
		  Test_liucheng_L = w_wait;
			break;
		default:
			Test_liucheng_L = w_end;
			break;
	}
}

