#include "Test_List.h"
#include "ADC_CHK.h"
#include "GPIO.h"
#include "LED_CTRL.h"
#include "ZDINA219.h"
#include "tongxin_xieyi_Ctrl.h"
#include "uart0.h"
#include "uart1.h"

#include "valve_test.h"

// 从 valve_test.c 引入的外部变量
extern uint8_t enable_valve_test;
extern ValveTestContext g_valve_ctx;

struct Test_quanju_canshu Test_quanju_canshu_L;
enum Test_liucheng Test_liucheng_L = w_wait;
struct Test_jieguo Test_jiejuo_jilu;
struct Test_linshi_cunchushuju Test_linshi_cunchushuju_L;
enum test_xieyi_jilu test_xieyi_jilu_Rec = No_Receive;
uint8_t diancitie_shanshuo = 0;
uint8_t diancitie_shanshuo_zhuangtai = 0;
uint16_t ceshi_diancifa_yanshi = 0;
uint8_t shangg_chongfa = 0;
uint8_t famen_test = 0;
uint8_t famen_state_1 = 0;
uint8_t famen_state_2 = 0;
uint32_t famen_voltage1 = 0xFFFFFFFF;
VT_TestResult valve_test_result = VT_IDLE;
uint32_t famen_voltage2 = 0xFFFFFFFF;
uint8_t test_famen_type = 1; // 阀门类型：0=无阀, 1=有阀（默认有阀）
uint8_t low_power_test_fail_count =
    0; // 测试低功耗失败的次数，用于判断是否需要重置阀门状态，如果失败10次以上就发送重置阀门指令
extern uint16_t debug_print_time;
uint16_t famen_debug_print_time = 1000; // 阀门电压打印定时器（独立）
volatile uint32_t system_tick_ms = 0; // 系统毫秒计数器，在定时器中断中递增

// 滑动窗口平均采样相关变量
#define VOLTAGE_SAMPLE_COUNT 10 // 滑动窗口大小：保留最近5次采样
static uint32_t famen1_samples[VOLTAGE_SAMPLE_COUNT] = {
    0}; // 阀门1电压采样缓冲区
static uint32_t famen2_samples[VOLTAGE_SAMPLE_COUNT] = {
    0};                          // 阀门2电压采样缓冲区
static uint8_t sample_index = 0; // 当前采样索引
static uint8_t samples_filled = 0; // 已填充的采样数量（用于初始阶段）

uint8_t dayinbiaohaoyong[12];

// 前向声明
uint32_t get_system_tick_ms(void);
uint32_t process_voltage_reading_ex(uint8_t channel_id, uint16_t total_time_ms,
                                    uint16_t sample_count,
                                    uint32_t (*voltage_read_func)(void),
                                    uint32_t (*get_tick_func)(void),
                                    void (*delay_func)(uint32_t),
                                    uint8_t filter_mode);

/**
 * @brief 发送关阀命令（预关阀步骤使用）
 */
void send_close_valve_command(void) { FM_Close_xieyi(); }

void test_quanju_canshu_Init() { Test_quanju_canshu_L.time_softdelay_ms = 10; }
// 测试参数初始化
void test_start_Init() {
  // 主装具开继电器打开
  // zhudian_gongdian_On();
  // 备用电源打开
  beidian_gongdian_OFF();
  // 主电压检测控制关闭
  zhudian_dianya_CHK_CTRL_OFF();

  change_valve_open_to_output_mode();
  change_valve_close_to_output_mode();
  FL_DelayMs(10);
  // 给open输出低电平，给到位信号，否则阀门会输出高电平，可能功耗测试有问题
  // valve_open_on();
  // valve_close_on();
  FL_DelayMs(10);
  // 开始供电
  zhudian_gongdian_On();
  // 等待10ms让电压稳定
  FL_DelayMs(10);
  // 备用电源电压检测控制关闭
  erji_dianya_CHK_CTRL_OFF();
  diancitie_shanshuo = 0;
  // 电磁铁关闭
  Electromagnet_Off();
  // 电机停止
  Motor_Stop();
  // LED闪烁一下代表初始化完成
  LED_FLAG_Run();
  // 初始化open和close为输入模式（高阻态，非到位状态）
  change_valve_open_to_input_mode();
  change_valve_close_to_input_mode();
}
// 工位检测
void gongwei_jiance() {
#if HardwareStationSet
  Test_jiejuo_jilu.gongwei = board_station_check();
#else
  Test_jiejuo_jilu.gongwei = 0x03;
#endif
  DeBug_print("当前工位是%d\r\n", Test_jiejuo_jilu.gongwei);
}
// 测试结果清零初始化
void test_jieguo_qingling() {
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
  memset(Test_jiejuo_jilu.IMEI_CHK, 0xFF, 15);
  memset(Test_jiejuo_jilu.IMSI_CHK, 0xFF, 15);
  memset(Test_jiejuo_jilu.ICCID_CHK, 0xFF, 20);
  Test_jiejuo_jilu.FM1 = 0;
  Test_jiejuo_jilu.FM2 = 0;
  Test_jiejuo_jilu.FM = 0;
  Test_jiejuo_jilu.FM_daowei_1 = 0;
  Test_jiejuo_jilu.FM_daowei_2 = 0;
  Test_jiejuo_jilu.FM_daowei = 0;
  Test_jiejuo_jilu.EEPROM_jiance = 0;
  Test_jiejuo_jilu.GP30_dianya = 0;
  Test_jiejuo_jilu.CSQ = 0xFF;
  memset(Test_jiejuo_jilu.loraEUI, 0xFF, 16);
  Test_jiejuo_jilu.qiangci_jiance = 0;
  Test_jiejuo_jilu.qiangci_jiance_1 = 0;
  Test_jiejuo_jilu.qiangci_jiance_2 = 0;
  Test_jiejuo_jilu.kaigai_jiance = 0;
  Test_jiejuo_jilu.GPSmozu_jiacne = 0;
  Test_quanju_canshu_L.test_over = 0;
  // 近红外用0xAA表号，远红外最好别用
  memset(Test_jiejuo_jilu.dangqian_biaohao, 0xAA, 6);
  Test_jiejuo_jilu.jiliang_1_1 = 0;
  Test_jiejuo_jilu.jiliang_1_2 = 0;
  Test_jiejuo_jilu.jiliang_2_1 = 0;
  Test_jiejuo_jilu.jiliang_2_2 = 0;
  Test_jiejuo_jilu.jiliang_1_3 = 0;
  Test_jiejuo_jilu.jiliang_2_3 = 0;
  Test_jiejuo_jilu.qiangci_jiance_3 = 0;
  famen_test = 0;
  famen_state_1 = 0;
  famen_state_2 = 0;
  memset(Test_jiejuo_jilu.jiaoyanma, 0xFF, 2);
  memset(Test_jiejuo_jilu.banbenhao, 0xFF, 2);
  // 阀门测试电压重置
  famen_voltage1 = 0xFFFFFFFF;
  famen_voltage2 = 0xFFFFFFFF;
  // 接收数据标志位清零
  test_xieyi_jilu_Rec = No_Receive;
  // 上报重发次数清零
  shangg_chongfa = 0;
  // 低功耗测试失败计数清零
  low_power_test_fail_count = 0;
  // 配置阀门到位信号为输入模式
  change_valve_open_to_input_mode();
  change_valve_close_to_input_mode();
}
// 开始测试
void test_start() {
  // IO口恢复
  test_start_Init();
  // 测试结果清零
  test_jieguo_qingling();
  Test_liucheng_L = w_start;
  // 总测试时间90秒
  Test_quanju_canshu_L.time_aroundtest_ms = 110000;
  // 单步超时时间
  Test_quanju_canshu_L.danbu_chaoshishijian_ms = 3000;
  Test_quanju_canshu_L.test_over = 0;
  Test_quanju_canshu_L.time_softdelay_ms = 0;
  // 使用格式化输出，避免二进制数据乱码
  DeBug_print("模块号: ");
  protocol_debug_print(Test_jiejuo_jilu.mokuaihao, 6);
}
// 终止测试
// 终止测试的触发是：超时或者测试完成板子脱离了工装具
void test_testend() {
  Test_liucheng_L = w_end;
  Test_quanju_canshu_L.test_over = 1;
  Test_quanju_canshu_L.time_softdelay_ms = 0;
}
// 测试过程中的错误和异常事件
void test_err_end_Func() {
  // 测试超时
  if ((Test_quanju_canshu_L.time_aroundtest_ms == 0 ||
       Test_quanju_canshu_L.danbu_chaoshishijian_ms == 0) &&
      Test_quanju_canshu_L.test_over == 0) {
    if (Debug_Mode) {
      if (Test_quanju_canshu_L.time_aroundtest_ms == 0) {
        DeBug_print(">> 测试总时间超时，测试终止\r\n");
      }
      if (Test_quanju_canshu_L.danbu_chaoshishijian_ms == 0) {
        DeBug_print(">> 单步操作超时，测试终止\r\n");
      }
    }
    // 结束测试
    test_testend();
  }
  // 测试过程中电压小于3.3V也结束测试
  else if (Test_quanju_canshu_L.test_over == 0 &&
           get_zhukongban_gongdian_dianya() < 3300) {
    // 结束测试
    test_testend();
    DeBug_print(">> 主控板供电电压低于3.3V，测试终止\r\n");
  }
}

void test_Loop_Func() {

  test_err_end_Func();
  // Test_liucheng_L = w_gonghao_CHK;
  if (Test_quanju_canshu_L.time_softdelay_ms > 0) {
    return;
  }
  switch (Test_liucheng_L) {
  case w_wait:
    // 静等待下一次测试

    break;

  case w_start:
    DeBug_print("\r\n========================================\r\n");
    DeBug_print("【步骤1/10】VCC电压检测\r\n");
    DeBug_print("========================================\r\n");
    // 开始测试前校验VDD是否有电，从而判断测试是否开始
    Test_jiejuo_jilu.zhidian_dianya_gongdian = get_zhukongban_gongdian_dianya();
    DeBug_print(">> VCC电压：%d mV (范围: 3200-4000 mV)\r\n",
                Test_jiejuo_jilu.zhidian_dianya_gongdian);
    FL_DelayMs(5);

    if (Test_jiejuo_jilu.zhidian_dianya_gongdian > 3200 &&
        Test_jiejuo_jilu.zhidian_dianya_gongdian < 4000) {
      DeBug_print(">> 结果：✅ VCC电压正常\r\n");
      Test_quanju_canshu_L.time_softdelay_ms = 0;
      Test_quanju_canshu_L.danbu_chaoshishijian_ms = 60000;
      Test_liucheng_L = w_close_valve_command_send;
      // 提供open到位，避免功耗异常,测试完成功耗需要将open恢复
      valve_close_off();
      valve_open_on();
      send_close_valve_command();
      DeBug_print(">> 准备进入下一步：静态功耗测试\r\n");
      FL_DelayMs(5);

    } else {
      // 等待1秒复测一次
      Test_quanju_canshu_L.time_softdelay_ms = 1000;
    }
    break;
  // 发送关阀门指令，之前已经开到位，但是不发关阀门指令，后续在阀门测试的时候，会因为对方业务逻辑，导致发开阀指令失效
  case w_close_valve_command_send: {
    DeBug_print("\r\n========================================\r\n");
    DeBug_print("【步骤2/10】发送关阀门指令\r\n");
    DeBug_print("========================================\r\n");
    if (test_xieyi_jilu_Rec == w_get_famen_dongzuo) {

      DeBug_print(">> 结果：✅ 已收到关阀门指令回复\r\n");
      Test_jiejuo_jilu.hongwai_jiance = 1; // 说明红外没问题
      Test_quanju_canshu_L.time_softdelay_ms = 0;
      Test_quanju_canshu_L.danbu_chaoshishijian_ms = 15000;
      // 提供关到位信号
      valve_open_off();
      valve_close_on();
      FL_DelayMs(100);
      valve_close_off();
      Test_liucheng_L = w_gonghao_CHK;
      Motor_Start(); // 开启电机，验证计量会不会有短路，短路会功耗异常
      Electromagnet_On();
      FL_DelayMs(5);

    } else {
      send_close_valve_command();
      DeBug_print(">> 结果：✅ 关阀门指令发送完成\r\n");
      Test_quanju_canshu_L.time_softdelay_ms = 2000;
      Test_quanju_canshu_L.danbu_chaoshishijian_ms = 60000;
      // Test_liucheng_L = w_gonghao_CHK;
      FL_DelayMs(5);
    }
  } break;
  case w_gonghao_CHK: {
    static uint8_t step_printed = 0;
    if (step_printed == 0) {
      DeBug_print("\r\n========================================\r\n");
      DeBug_print("【步骤2/10】静态功耗测试\r\n");
      DeBug_print("========================================\r\n");
      step_printed = 1;
    }

    uint16_t power_result = static_power_check(
        500, 20, 100, 1); // 采样500ms,20次，只要有超过100uA快速失败
    // 处理快速失败情况（0xFFFF）
    if (power_result == 0xFFFF) {
      // 也读取个异常功耗的值，以便记录
      Test_jiejuo_jilu.zhidian_jingtai_gonghao = Current_CHK_Func(0);
      DeBug_print(">> 结果：❌ 静态功耗测试快速失败（检测到功耗异常）\r\n");
      low_power_test_fail_count++;
      if (low_power_test_fail_count >= 7) {
        DeBug_print("低功耗测试失败次数过多，发送配置阀门状态指令\r\n");
        // reset_water_meter_to_none_valve();
        // configure_valve_parameters(15, 1, 12, 230);
        valve_open_on();

        low_power_test_fail_count = 0;
      }
      Test_quanju_canshu_L.time_softdelay_ms = 1000;
    }
    // 只有当返回非0值且非0xFFFF时，说明测量完成
    else if (power_result > 0) {
      Test_jiejuo_jilu.zhidian_jingtai_gonghao = power_result;
      DeBug_print(">> 测试结果：%d uA (范围: 2-50 uA)\r\n",
                  Test_jiejuo_jilu.zhidian_jingtai_gonghao);

      if (Test_jiejuo_jilu.zhidian_jingtai_gonghao > 2 &&
          Test_jiejuo_jilu.zhidian_jingtai_gonghao < 50) {
        // 关闭电机
        Motor_Stop();
        Electromagnet_Off();
        DeBug_print(">> 结果：✅ 静态功耗正常\r\n");
        valve_open_off();
        // change_valve_open_to_input_mode();
        // change_valve_close_to_input_mode();
        step_printed = 0; // 重置标志
        Test_quanju_canshu_L.time_softdelay_ms = 0;
        Test_quanju_canshu_L.danbu_chaoshishijian_ms =
            60000;                      // 阀门测试需要更长时间
        Test_liucheng_L = w_famenceshi; // 直接进入阀门测试（委托给外部状态机）
        Motor_Stop();
        low_power_test_fail_count = 0;
      } else {
        DeBug_print(">> 结果：❌ 功耗值不在合格范围\r\n");
        low_power_test_fail_count++;
        if (low_power_test_fail_count >= 7) {
          DeBug_print("低功耗测试失败次数过多，发送重置阀门状态指令\r\n");
          // reset_water_meter_to_none_valve();
          //  configure_valve_parameters(15, 0, 12, 230); // 重置阀门状态
          //   配置成5线阀门状态，避免低功耗测试失败
          configure_valve_parameters(15, 1, 12, 230);
          low_power_test_fail_count = 0; // 重置计数器
        } else {
          DeBug_print("继续等待，当前失败次数是%d\r\n",
                      low_power_test_fail_count);
        }
        Test_quanju_canshu_L.time_softdelay_ms = 1000;
      }
    }
    // 如果测量还在进行中，不做任何处理，等待下次循环
  } break;
  case w_famenceshi: {
    // 使用 enable_valve_test 判断是否已启动阀门测试
    // 如果 enable_valve_test 为 0，说明需要启动

    /* 无阀测试直接跳过 */

    if (test_famen_type == 0) {
      DeBug_print("无阀测试，跳过阀门测试步骤\r\n");
      Test_jiejuo_jilu.FM = 0; // 无阀默认不通过
      Test_quanju_canshu_L.time_softdelay_ms = 0;
      Test_liucheng_L = w_zhudong_shangg;
      return;
    }

    /* 有阀测试：检查阀门测试结果 */
    if (valve_test_result == VT_SUCCESS) {
      // 阀门测试成功
      DeBug_print(">> ✅ 阀门测试成功完成\r\n");
      Test_jiejuo_jilu.FM = 1;
      Test_jiejuo_jilu.jiliang_jiance = 1; // 计量检测通过
      Test_quanju_canshu_L.time_softdelay_ms = 0;
      enable_valve_test = 0;
      valve_test_result = VT_IDLE; // 重置结果
      // 测试完成进入下一步
      Test_liucheng_L = w_zhudong_shangg;
    } else if (valve_test_result == VT_FAIL ||
               valve_test_result == VT_TIMEOUT) {
      // 阀门测试失败，打印详细原因
      DeBug_print(">> ❌ 阀门测试失败\r\n");
      DeBug_print("   失败步骤: %s\r\n",
                  valve_test_get_step_name(g_valve_ctx.fail_step));
      DeBug_print("   失败原因: %s\r\n",
                  valve_test_get_fail_reason_str(g_valve_ctx.fail_reason));
      DeBug_print(
          "   当前数据: 开阀到位=%d, 关阀到位=%d, 电压A=%lumV, 电压B=%lumV\r\n",
          g_valve_ctx.pos_open, g_valve_ctx.pos_close, g_valve_ctx.voltage_a,
          g_valve_ctx.voltage_b);
      Test_jiejuo_jilu.FM = 0;
      enable_valve_test = 0;
      valve_test_result = VT_IDLE;
      Test_liucheng_L = w_end;
    } else if (!enable_valve_test) {
      // 首次进入（enable_valve_test为0），启动阀门测试
      enable_valve_test = 1;
      valve_test_start(&g_valve_ctx); // 启动阀门测试状态机
      DeBug_print("当前是有阀项目，启动阀门测试\r\n");
    }
    // VT_RUNNING 或 VT_IDLE（已启动）：继续等待结果
    break;
  }

  case w_zhudong_shangg:
    DeBug_print("\r\n========================================\r\n");
    DeBug_print("【步骤5/8】主动上报\r\n");
    DeBug_print("========================================\r\n");
    if (test_xieyi_jilu_Rec == w_get_shanggao || shangg_chongfa > 1) {
      test_xieyi_jilu_Rec = No_Receive;
      DeBug_print(">> 结果：✅ 上报成功\r\n");
      Test_quanju_canshu_L.time_softdelay_ms = 3000;
      Test_quanju_canshu_L.danbu_chaoshishijian_ms = 20000;
      Test_liucheng_L = w_get_firmware_version;
      find_banbenhao_xieyi();
    } else {
      shangg_chongfa++;
      DeBug_print(">> 正在发送上报协议...(第%d次)\r\n", shangg_chongfa);
      send_shanggao_xieyi();
      Test_quanju_canshu_L.time_softdelay_ms = 1000;
    }
    break;
  case w_get_firmware_version:
    DeBug_print("\r\n========================================\r\n");
    DeBug_print("【步骤6/8】版本号读取\r\n");
    DeBug_print("========================================\r\n");
    if (test_xieyi_jilu_Rec == w_get_banbenhao) {
      test_xieyi_jilu_Rec = No_Receive;
      memcpy(Test_jiejuo_jilu.banbenhao, Test_linshi_cunchushuju_L.L_banbenhao,
             2);
      FL_DelayMs(10);
      DeBug_print(">> 版本号：%02X %02X\r\n", Test_jiejuo_jilu.banbenhao[0],
                  Test_jiejuo_jilu.banbenhao[1]);
      DeBug_print(">> 结果：✅ 版本号读取成功\r\n");

      // 测试合格，进入下一步
      Test_quanju_canshu_L.time_softdelay_ms = 0;
      // 设置单步超时时间 10秒
      Test_quanju_canshu_L.danbu_chaoshishijian_ms = 15000;
      Test_liucheng_L = w_shangg_jieguo_start;
    } else {
      DeBug_print(">> 正在读取版本号...\r\n");
      find_banbenhao_xieyi();
      Test_quanju_canshu_L.time_softdelay_ms = 1000;
    }
    break;
  case w_shangg_jieguo_start: {
    static uint8_t step8_printed = 0;
    if (step8_printed == 0) {
      DeBug_print("\r\n========================================\r\n");
      DeBug_print("【步骤7/"
                  "8】目前系统正在上告，但是时间较长，先做点别的，验证下计量和"
                  "强磁的基本信息\r\n");
      DeBug_print("========================================\r\n");
      step8_printed = 1;
    }
    if (test_xieyi_jilu_Rec == w_get_test_zhuanyong) {
      step8_printed = 0; // 重置标志，准备下次测试
      test_xieyi_jilu_Rec = No_Receive;
      // 测试合格，进入下一步,考虑到上告一下子不会立刻有结果，干脆等待下，这个等待的时间可以自行测试多长合适
      Test_quanju_canshu_L.time_softdelay_ms = 0;
      // 设置单步超时时间 20秒
      Test_quanju_canshu_L.danbu_chaoshishijian_ms = 60000;
      if (Test_linshi_cunchushuju_L.L_jilianghuoer1 == 0) {
        Test_jiejuo_jilu.jiliang_1_1 |= 2;
      } else {
        Test_jiejuo_jilu.jiliang_1_1 |= 1;
      }

      if (Test_linshi_cunchushuju_L.L_jilianghuoer2 == 0) {
        Test_jiejuo_jilu.jiliang_2_1 |= 2;
      } else {
        Test_jiejuo_jilu.jiliang_2_1 |= 1;
      }

      if (Test_linshi_cunchushuju_L.L_huoer1_CHK == 0) {
        Test_jiejuo_jilu.qiangci_jiance_1 |= 2;
      } else {
        Test_jiejuo_jilu.qiangci_jiance_1 |= 1;
      }
      Test_liucheng_L = w_shangg_jieguo;
    } else {
      DeBug_print(">> 正在查询强磁的基本信息信息...\r\n");
      Test_zhuanyong_xieyi();
      Test_quanju_canshu_L.time_softdelay_ms = 500;
    }
    break;
  }
  case w_shangg_jieguo: {
    static uint8_t step9_printed = 0;
    if (step9_printed == 0) {
      DeBug_print("\r\n========================================\r\n");
      DeBug_print("【步骤8/8】模块信息查询（CSQ/IMEI/ICCID等）\r\n");
      DeBug_print("========================================\r\n");
      step9_printed = 1;
    }
    if (test_xieyi_jilu_Rec == w_get_test_zhuanyong) {
      test_xieyi_jilu_Rec = No_Receive;

      // 对测试结果进行映射
      //  flash
      Test_jiejuo_jilu.flash_jiance = Test_linshi_cunchushuju_L.L_flash_chk;
      // 获取主电电压
      Test_jiejuo_jilu.zhidian_dianya_huoqu =
          Test_linshi_cunchushuju_L.L_zhudian_dianya;
      // 获取备电电压
      Test_jiejuo_jilu.beidian_dianya_huoqu =
          Test_linshi_cunchushuju_L.L_beidian_dianya;
      // 压力状态（协议中没有，作为温度）
      Test_jiejuo_jilu.lanya_jiance = Test_linshi_cunchushuju_L.L_yali_CHK;
      // EEPROM
      Test_jiejuo_jilu.EEPROM_jiance = Test_linshi_cunchushuju_L.L_EEPROM_CHK;
      // GPRS模组（当做蓝牙模组，当做蓝牙检测）
      Test_jiejuo_jilu.GPSmozu_jiacne =
          Test_linshi_cunchushuju_L.L_GPRS_mozu_CHK;
      // GP30电压检测
      Test_jiejuo_jilu.GP30_dianya =
          Test_linshi_cunchushuju_L.L_GP30mozu_AD_CHK;
      // IMEI
      memcpy(Test_jiejuo_jilu.IMEI_CHK, Test_linshi_cunchushuju_L.L_IMEI, 15);
      // IMSI
      memcpy(Test_jiejuo_jilu.IMSI_CHK, Test_linshi_cunchushuju_L.L_IMSI, 15);
      // ICCID
      memcpy(Test_jiejuo_jilu.ICCID_CHK, Test_linshi_cunchushuju_L.L_ICCID, 20);
      // CSQ
      Test_jiejuo_jilu.CSQ = Test_linshi_cunchushuju_L.L_CSQ;

      if (Test_linshi_cunchushuju_L.L_jilianghuoer1 == 0) {
        Test_jiejuo_jilu.jiliang_1_1 |= 2;
      } else {
        Test_jiejuo_jilu.jiliang_1_1 |= 1;
      }

      if (Test_linshi_cunchushuju_L.L_jilianghuoer2 == 0) {
        Test_jiejuo_jilu.jiliang_2_1 |= 2;
      } else {
        Test_jiejuo_jilu.jiliang_2_1 |= 1;
      }

      if (Test_linshi_cunchushuju_L.L_huoer1_CHK == 0) {
        Test_jiejuo_jilu.qiangci_jiance_1 |= 2;
      } else {
        Test_jiejuo_jilu.qiangci_jiance_1 |= 1;
      }
      if (Test_linshi_cunchushuju_L.L_CSQ < 33 &&
          Test_linshi_cunchushuju_L.L_CSQ > 10 &&
          Test_linshi_cunchushuju_L.L_ICCID[0] != 0xFF &&
          Test_linshi_cunchushuju_L.L_IMEI[0] != 0xFF &&
          Test_linshi_cunchushuju_L.L_IMSI[0] != 0xFF &&
          Test_linshi_cunchushuju_L.L_ICCID[0] != 0x00 &&
          Test_linshi_cunchushuju_L.L_IMEI[0] != 0x00 &&
          Test_linshi_cunchushuju_L.L_IMSI[0] != 0x00) {
        DeBug_print(">> 结果：✅ 系统信息验证通过 (CSQ=%d)\r\n",
                    Test_linshi_cunchushuju_L.L_CSQ);
        step9_printed = 0; // 重置标志
        Test_quanju_canshu_L.time_softdelay_ms = 0;
        Motor_Start();
        Electromagnet_On();
        Test_liucheng_L = w_jiliang_qiangci_CHK_1;
      } else {
        DeBug_print(">> 结果：❌ CSQ值不合格 (CSQ=%d, 范围: 10-33)\r\n",
                    Test_linshi_cunchushuju_L.L_CSQ);
        // CSQ不合格时继续等待重试，设置延时避免频繁打印
        Test_quanju_canshu_L.time_softdelay_ms = 1000;
        // 不重置step9_printed，避免重复打印步骤标题
      }
    } else {
      DeBug_print(">> 正在查询上告信息...\r\n");
      Test_zhuanyong_xieyi();
      Test_quanju_canshu_L.time_softdelay_ms = 1000;
    }
    break;
  }
  case w_jiliang_qiangci_CHK_1:
    DeBug_print("\r\n========================================\r\n");
    DeBug_print("【步骤10/10】磁驱动和计量测试\r\n");
    DeBug_print("========================================\r\n");
    if (test_xieyi_jilu_Rec == w_get_test_zhuanyong) {
      test_xieyi_jilu_Rec = No_Receive;
      if (Test_linshi_cunchushuju_L.L_jilianghuoer1 == 0) {
        Test_jiejuo_jilu.jiliang_1_1 |= 2;
      } else {
        Test_jiejuo_jilu.jiliang_1_1 |= 1;
      }

      if (Test_linshi_cunchushuju_L.L_jilianghuoer2 == 0) {
        Test_jiejuo_jilu.jiliang_2_1 |= 2;
      } else {
        Test_jiejuo_jilu.jiliang_2_1 |= 1;
      }

      if (Test_linshi_cunchushuju_L.L_huoer1_CHK == 0) {
        Test_jiejuo_jilu.qiangci_jiance_1 |= 2;
      } else {
        Test_jiejuo_jilu.qiangci_jiance_1 |= 1;
      }

      DeBug_print("第一次检测结果 %d %d %d\r\n", Test_jiejuo_jilu.jiliang_1_1,
                  Test_jiejuo_jilu.jiliang_2_1,
                  Test_jiejuo_jilu.qiangci_jiance_1);
      // 设置单步超时时间 10秒
      Test_quanju_canshu_L.danbu_chaoshishijian_ms = 60000;
      // 等待2秒进入下一步
      Test_quanju_canshu_L.time_softdelay_ms = 0;
      // 开启电磁驱动
      //  diancitie_shanshuo = 1;
      Electromagnet_On();
      Motor_Start();
      famen_test = 10;
      Test_liucheng_L = w_jiliang_qiangci_CHK_2;

      // 测试下
      // DeBug_print("发送读当前走水量协议\r\n");
      // read_current_water_usage_protocol();
    } else {
      DeBug_print("开始磁驱动测试\r\n");
      // 发送协议
      Test_zhuanyong_xieyi();
      Test_quanju_canshu_L.time_softdelay_ms = 1000;
    }
    break;
  case w_jiliang_qiangci_CHK_2:
    Electromagnet_Off();
    Motor_Stop();
    if (test_xieyi_jilu_Rec == w_get_test_zhuanyong && famen_test != 0) {
      test_xieyi_jilu_Rec = No_Receive;
      if (Test_linshi_cunchushuju_L.L_jilianghuoer1 == 0) {
        Test_jiejuo_jilu.jiliang_1_1 |= 2;
      } else {
        Test_jiejuo_jilu.jiliang_1_1 |= 1;
      }

      if (Test_linshi_cunchushuju_L.L_jilianghuoer2 == 0) {
        Test_jiejuo_jilu.jiliang_2_1 |= 2;
      } else {
        Test_jiejuo_jilu.jiliang_2_1 |= 1;
      }

      if (Test_linshi_cunchushuju_L.L_huoer1_CHK == 0) {
        Test_jiejuo_jilu.qiangci_jiance_1 |= 2;
      } else {
        Test_jiejuo_jilu.qiangci_jiance_1 |= 1;
      }

      DeBug_print("\r\n第二次检测结果 %d %d %d\r\n",
                  Test_jiejuo_jilu.jiliang_1_1, Test_jiejuo_jilu.jiliang_2_1,
                  Test_jiejuo_jilu.qiangci_jiance_1);
      // 强磁
      if (Test_jiejuo_jilu.qiangci_jiance_1 == 3) {
        Test_jiejuo_jilu.qiangci_jiance = 1;
      }
      // 计量1
      if (Test_jiejuo_jilu.jiliang_1_1 == 3) {
        famen_state_1 = 1;
      }
      // 计量2
      if (Test_jiejuo_jilu.jiliang_2_1 == 3) {
        famen_state_2 = 1;
      }
      if ((famen_state_1 == 1) && (famen_state_2 == 1)) {
        Test_jiejuo_jilu.jiliang_jiance = 1;
      }

      if (Test_jiejuo_jilu.jiliang_jiance == 1 &&
          Test_jiejuo_jilu.qiangci_jiance == 1) {
        // 设置单步超时时间 10秒
        Test_quanju_canshu_L.danbu_chaoshishijian_ms = 40000;
        // 等待2秒进入下一步
        Test_quanju_canshu_L.time_softdelay_ms = 0;
        famen_test = 0;
        Electromagnet_Off();
        Motor_Stop();
        Test_liucheng_L = w_end;
        if (Test_jiejuo_jilu.biaoju_leixing_famen == 0) {
          if (Test_jiejuo_jilu.jixiebiao_canshu[3] == 0x00) {
            Test_jiejuo_jilu.FM = 1;
            Test_liucheng_L = w_end;
          }
        }
        if (Test_jiejuo_jilu.biaoju_leixing_famen == 1) {
          if (Test_jiejuo_jilu.chaoshengbo_canshu[5] == 0x00) {
            Test_jiejuo_jilu.FM = 1;
            Test_liucheng_L = w_end;
          }
        }

        break;
      } else {
        famen_test--;
        if (famen_test == 0) {
          Test_quanju_canshu_L.danbu_chaoshishijian_ms = 30000;
          // Test_liucheng_L = w_famenceshi;
          Test_liucheng_L = w_jiliang_qiangci_CHK_1;
          // 等待2秒
          Test_quanju_canshu_L.time_softdelay_ms = 1000;
        } else {
          // 发送协议
          Test_zhuanyong_xieyi();
          Electromagnet_On();
          Motor_Start();
          // 等待2秒
          Test_quanju_canshu_L.time_softdelay_ms = 1000;
        }
      }
    } else if (famen_test == 0) {
      Test_liucheng_L = w_end;
    } else {
      DeBug_print("第二次检测驱动\r\n");
      // 发送协议
      Test_zhuanyong_xieyi();
      Electromagnet_On();
      Motor_Start();
      Test_quanju_canshu_L.time_softdelay_ms = 1000;
    }
    break;
  case w_end:
    DeBug_print("\r\n========================================\r\n");
    DeBug_print("✅ 测试完成\uff01\r\n");
    DeBug_print("========================================\r\n");
    // 主电源保持打开
    // zhudian_gongdian_On();
    // 主电电源关闭
    zhudian_gongdian_OFF();
    // 备用电源关闭
    beidian_gongdian_OFF();
    // 改变阀门open到位信号为输入模式
    change_valve_open_to_input_mode();
    // 改变阀门close到位信号为输入模式
    change_valve_close_to_input_mode();
    // 没有的项目默认测试通过
    //  Test_jiejuo_jilu.FM = 1;
    //  Test_jiejuo_jilu.qiangci_jiance = 1;
    // 电磁铁和电机停止
    diancitie_shanshuo = 0;
    // 关闭主电
    zhudian_gongdian_OFF();
    Electromagnet_Off();
    Motor_Stop();
    // 一切测试都已经结束，打开测试返回
    Test_quanju_canshu_L.test_over = 1;
    // 回到第一步
    Test_liucheng_L = w_wait;
    break;
  default:
    Test_liucheng_L = w_end;
    break;
  }
}
// 以下件为独立委托事件，与测试无关无关
void Test_loop_func() {
  // 每次循环只采样一次，避免重复采样

  if (Test_liucheng_L == w_valve_test_process_1) {
    // 非阻塞性获取阀门1和2电压
    famen_voltage1 = process_voltage_reading_ex(0, 3000, VOLTAGE_SAMPLE_COUNT,
                                                get_famen_1_dianya,
                                                get_system_tick_ms, NULL, 1);
    famen_voltage2 = process_voltage_reading_ex(1, 3000, VOLTAGE_SAMPLE_COUNT,
                                                get_famen_2_dianya,
                                                get_system_tick_ms, NULL, 1);

    if (famen_debug_print_time == 0) {
      DeBug_print("阀门1电压:%d 阀门2电压:%d\r\n", famen_voltage1,
                  famen_voltage2);
      famen_debug_print_time = 1000;
    }
    // 使用已经采样的值，不要重复采样
    if (famen_voltage1 > 1800) {
      Test_jiejuo_jilu.FM1 |= 0x01;
    } else {
      Test_jiejuo_jilu.FM1 |= 0x02;
    }
    if (famen_voltage2 > 1800) {
      Test_jiejuo_jilu.FM2 |= 0x01;
    } else {
      Test_jiejuo_jilu.FM2 |= 0x02;
    }
    if (Test_jiejuo_jilu.FM1 == 0x03 && Test_jiejuo_jilu.FM2 == 0x03) {
      Test_jiejuo_jilu.FM = 1;
      Test_quanju_canshu_L.time_softdelay_ms = 0;
      // 校验完成后不管协议有没有接收都跳过
      test_xieyi_jilu_Rec = w_get_famen_dongzuo_L;
    }
  }
}

/**
 * @brief 使用滑动窗口平均采样的阀门电压检测函数（改进版）
 * @note 采用5次采样滑动平均，可有效滤除瞬时干扰和噪声
 *       相比单次采样更稳定可靠，适合工业现场环境
 */
void Test_loop_func_avg() {
  // 使用滑动窗口平均采样，提高可靠性

  if (Test_liucheng_L == w_valve_test_process_1) {
    // 1. 采集新的ADC数据并存入滑动窗口
    famen1_samples[sample_index] = get_famen_1_dianya();
    famen2_samples[sample_index] = get_famen_2_dianya();

    // 2. 更新索引（循环使用数组）
    sample_index++;
    if (sample_index >= VOLTAGE_SAMPLE_COUNT) {
      sample_index = 0;
    }

    // 3. 记录已填充的采样数量（用于初始阶段）
    if (samples_filled < VOLTAGE_SAMPLE_COUNT) {
      samples_filled++;
    }

    // 4. 计算滑动窗口平均值
    uint32_t sum1 = 0, sum2 = 0;
    for (uint8_t i = 0; i < samples_filled; i++) {
      sum1 += famen1_samples[i];
      sum2 += famen2_samples[i];
    }
    famen_voltage1 = sum1 / samples_filled;
    famen_voltage2 = sum2 / samples_filled;

    // 5. 定时打印调试信息
    if (famen_debug_print_time == 0) {
      DeBug_print("阀门电压(平均%d次) - 阀门1:%dmV 阀门2:%dmV\r\n",
                  samples_filled, famen_voltage1, famen_voltage2);
      Test_jiejuo_jilu.zhidian_dianya_gongdian =
          get_zhukongban_gongdian_dianya();
      DeBug_print("VCC电压%d\r\n", Test_jiejuo_jilu.zhidian_dianya_gongdian);
      famen_debug_print_time = 1000;
    }

    // 6. 使用平均后的值进行阈值判断
    // 阈值: 1800mV，判断阀门是否正常工作
    if (famen_voltage1 > 1800) {
      Test_jiejuo_jilu.FM1 |= 0x01; // 阀门1电压合格
    } else {
      Test_jiejuo_jilu.FM1 |= 0x02; // 阀门1电压不合格
    }

    if (famen_voltage2 > 1800) {
      Test_jiejuo_jilu.FM2 |= 0x01; // 阀门2电压合格
    } else {
      Test_jiejuo_jilu.FM2 |= 0x02; // 阀门2电压不合格
    }

    // 7. 当两个阀门都检测到高低电压（0x03 = 0x01 | 0x02）时，认为测试完成
    if (Test_jiejuo_jilu.FM1 == 0x03 && Test_jiejuo_jilu.FM2 == 0x03) {
      Test_jiejuo_jilu.FM = 1; // 阀门测试完成标志
      Test_quanju_canshu_L.time_softdelay_ms = 0;
      test_xieyi_jilu_Rec =
          w_get_famen_dongzuo_L; // 跳过协议等待，直接进入下一步

      // 重置滑动窗口（为下次测试准备）
      sample_index = 0;
      samples_filled = 0;
    }
  } else {
    // 非阀门测试状态时，重置滑动窗口
    if (samples_filled > 0) {
      sample_index = 0;
      samples_filled = 0;
    }
  }
}

/**
 * @brief 静态功耗检测（在定时器中断中周期调用）
 *
 * @param total_time  总的检测功耗时间 (ms)
 * @param total_count  采样次数
 * @param early_fail_threshold  快速失败阈值（uA），0表示不启用
 * @param early_fail_count  连续多少次超过阈值后快速失败（如3表示连续3次）
 *                          如果前N次采样值都超过阈值，直接返回0xFFFF表示快速失败
 * @return uint16_t  功耗值（未完成返回0，完成返回平均值，快速失败返回0xFFFF）
 */
uint16_t static_power_check(uint16_t total_time, uint16_t total_count,
                            uint16_t early_fail_threshold,
                            uint8_t early_fail_count) {
  static uint16_t count = 0;            // 当前采样次数
  static uint32_t power_sum = 0;        // 功耗累积和
  static uint32_t last_sample_time = 0; // 上次采样时间（毫秒）
  static uint32_t start_time = 0;       // 测量开始时间（毫秒）
  static uint8_t is_measuring = 0;      // 是否正在测量中
  static uint8_t fail_count = 0;        // 连续失败次数计数器

  // 参数检查
  if (total_count == 0 || total_time == 0) {
    return 0;
  }

  // 计算采样间隔（毫秒）
  uint16_t period = total_time / total_count;
  if (period == 0)
    period = 1; // 至少1ms

  // 获取当前系统时间
  uint32_t current_time = system_tick_ms;

  // 如果不在测量中，开始新的测量
  if (is_measuring == 0) {
    is_measuring = 1;
    count = 0;
    power_sum = 0;
    fail_count = 0;
    start_time = current_time;
    last_sample_time = current_time;
    DeBug_print("开始静态功耗测试：总时间=%dms, 采样次数=%d, 间隔=%dms\r\n",
                total_time, total_count, period);
    return 0; // 第一次调用，开始测量
  }

  // 检查是否超时（给予足够的容错时间：至少10秒或total_count*500ms，取大值）
  // 因为主循环调用频率可能较低，每次采样间隔可能远大于理论值
  uint32_t timeout_ms = total_count * 500; // 每次采样最多500ms
  if (timeout_ms < 10000)
    timeout_ms = 10000; // 最少10秒超时

  if (current_time - start_time > timeout_ms) {
    // 超时，重置状态
    DeBug_print("静态功耗测试超时（经过%dms），重置\r\n",
                current_time - start_time);
    is_measuring = 0;
    count = 0;
    power_sum = 0;
    fail_count = 0;
    return 0;
  }

  // 到达采样时间点
  if (current_time - last_sample_time >= period) {
    last_sample_time = current_time; // 更新上次采样时间

    // 执行采样
    uint16_t current_power = Current_CHK_Func(0);
    power_sum += current_power;
    count++;
    DeBug_print("静态功耗采样 %d/%d: 当前功耗=%d uA\r\n", count, total_count,
                current_power);

    // 快速失败检测（如果启用）
    if (early_fail_threshold > 0 && early_fail_count > 0) {
      if (current_power > early_fail_threshold) {
        fail_count++;
        DeBug_print("静态功耗采样第%d次超过阈值%d uA，连续失败%d/%d次\r\n",
                    count, early_fail_threshold, fail_count, early_fail_count);

        // 达到连续失败次数，快速失败
        if (fail_count >= early_fail_count) {
          DeBug_print("连续%d次采样超过阈值，快速失败\r\n", early_fail_count);
          // 重置状态
          is_measuring = 0;
          count = 0;
          power_sum = 0;
          fail_count = 0;
          return 0xFFFF; // 返回特殊值表示快速失败
        }
      } else {
        // 本次采样正常，重置失败计数器
        if (fail_count > 0) {
          DeBug_print("本次采样正常，重置失败计数器\r\n");
          fail_count = 0;
        }
      }
    }

    // 检查是否完成所有采样
    if (count >= total_count) {
      uint16_t avg_power = power_sum / total_count;

      // 重置状态，准备下次测量
      is_measuring = 0;
      DeBug_print("静态功耗测试完成：平均功耗=%d uA\r\n", avg_power);

      return avg_power; // 返回平均功耗
    }
  }

  return 0; // 采样进行中
}

/**
 * @brief 电压采样处理器（支持多通道独立采样）
 *
 * 特性：
 * 1. 支持多通道独立采样（最多4路）
 * 2. 支持中值滤波（去除极值干扰）
 * 3. 支持定时间隔采样（非阻塞）和立即采样（阻塞+间隔）
 * 4. 每个通道有独立的状态，互不干扰
 *
 * @param channel_id        通道ID（0-3），用于区分不同的采样通道
 * @param total_time_ms     总采样时间（毫秒），设为0则立即阻塞采样
 * @param sample_count      采样次数（建议奇数，便于中值滤波，最大16）
 * @param voltage_read_func 电压读取回调函数
 * @param get_tick_func     获取系统时间回调函数（返回毫秒），立即模式可传NULL
 * @param delay_func        延时回调函数（参数为毫秒），立即模式使用，可传NULL
 * @param filter_mode       滤波模式：0=平均值, 1=中值滤波, 2=去极值平均
 * @return uint32_t         返回值说明：
 *                          0xFFFFFFFF = 采样进行中（非阻塞模式）
 *                          0xFFFFFFFE = 参数错误（sample_count=0 或
 * 回调为NULL） 0xFFFFFFFD = 非阻塞模式缺少时间源 0xFFFFFFFC = 采样超时 其他值
 * = 最终电压值(mV)
 *
 * 使用示例1（立即阻塞采样，带间隔）：
 *   uint32_t v = process_voltage_reading_ex(0, 0, 5, get_adc, NULL, FL_DelayMs,
 * 1);
 *
 * 使用示例2（非阻塞定时采样，需在主循环中反复调用）：
 *   uint32_t v1 = process_voltage_reading_ex(0, 500, 10, get_adc1,
 * get_sys_tick, NULL, 1); uint32_t v2 = process_voltage_reading_ex(1, 500, 10,
 * get_adc2, get_sys_tick, NULL, 1); if (v1 < 0xFFFFFFFC && v2 < 0xFFFFFFFC) {
 * 两路采样都完成 }
 */
#define VOLTAGE_READ_IN_PROGRESS 0xFFFFFFFF  // 采样进行中
#define VOLTAGE_READ_PARAM_ERROR 0xFFFFFFFE  // 参数错误
#define VOLTAGE_READ_NO_TICK_FUNC 0xFFFFFFFD // 缺少时间源
#define VOLTAGE_READ_TIMEOUT 0xFFFFFFFC      // 采样超时
#define VOLTAGE_MAX_CHANNELS 4               // 最大支持通道数

// 每个通道的采样状态结构体
typedef struct {
  uint8_t is_sampling;       // 是否正在采样
  uint32_t start_time;       // 采样开始时间
  uint32_t last_sample_time; // 上次采样时间
  uint16_t current_count;    // 当前已采样次数
  uint16_t target_count;     // 目标采样次数
  uint16_t sample_interval;  // 采样间隔
  uint32_t samples[16];      // 采样缓冲区（最多16次）
  uint8_t filter_mode;       // 滤波模式
} voltage_sample_state_t;

static voltage_sample_state_t voltage_channels[VOLTAGE_MAX_CHANNELS] = {0};

uint32_t process_voltage_reading_ex(uint8_t channel_id, uint16_t total_time_ms,
                                    uint16_t sample_count,
                                    uint32_t (*voltage_read_func)(void),
                                    uint32_t (*get_tick_func)(void),
                                    void (*delay_func)(uint32_t),
                                    uint8_t filter_mode) {
  // 参数检查
  if (channel_id >= VOLTAGE_MAX_CHANNELS || sample_count == 0 ||
      voltage_read_func == NULL) {
    return VOLTAGE_READ_PARAM_ERROR;
  }
  if (sample_count > 16) {
    sample_count = 16; // 限制最大采样次数
  }

  voltage_sample_state_t *ch = &voltage_channels[channel_id];

  // ========== 立即阻塞采样模式（total_time_ms == 0）==========
  if (total_time_ms == 0) {
    // 计算默认采样间隔（如果没有提供delay函数，则无间隔）
    uint16_t default_interval = 2; // 默认2ms间隔，让ADC稳定

    for (uint16_t i = 0; i < sample_count; i++) {
      ch->samples[i] = voltage_read_func();
      // 非最后一次采样时，加入间隔
      if (i < sample_count - 1 && delay_func != NULL) {
        delay_func(default_interval);
      }
    }
    ch->target_count = sample_count;
    ch->filter_mode = filter_mode;
    goto calculate_result;
  }

  // ========== 非阻塞定时采样模式 ==========
  // 需要时间源
  if (get_tick_func == NULL) {
    return VOLTAGE_READ_NO_TICK_FUNC; // 非阻塞模式必须提供时间源
  }

  uint32_t current_time = get_tick_func();

  // 开始新的采样周期
  if (ch->is_sampling == 0) {
    ch->is_sampling = 1;
    ch->start_time = current_time;
    ch->last_sample_time = current_time;
    ch->current_count = 0;
    ch->target_count = sample_count;
    ch->sample_interval = total_time_ms / sample_count;
    ch->filter_mode = filter_mode;
    if (ch->sample_interval == 0)
      ch->sample_interval = 1;

    // 立即采集第一个样本
    ch->samples[ch->current_count++] = voltage_read_func();
    return VOLTAGE_READ_IN_PROGRESS; // 采样中
  }

  // 超时保护（2倍总时间）
  if (current_time - ch->start_time > (uint32_t)(total_time_ms * 2)) {
    DeBug_print("通道%d电压采样超时，重置\r\n", channel_id);
    ch->is_sampling = 0;
    return VOLTAGE_READ_TIMEOUT;
  }

  // 检查是否到达采样时间点
  if (current_time - ch->last_sample_time >= ch->sample_interval) {
    ch->last_sample_time = current_time;
    ch->samples[ch->current_count++] = voltage_read_func();

    // 检查是否完成所有采样
    if (ch->current_count >= ch->target_count) {
      ch->is_sampling = 0; // 重置状态
      goto calculate_result;
    }
  }

  return VOLTAGE_READ_IN_PROGRESS; // 采样进行中

calculate_result:
  // ========== 滤波计算 ==========
  {
    uint32_t result = 0;
    uint16_t count = ch->target_count;
    uint8_t mode = ch->filter_mode;

    if (mode == 0) {
      // 模式0：简单平均
      uint32_t sum = 0;
      for (uint16_t i = 0; i < count; i++) {
        sum += ch->samples[i];
      }
      result = sum / count;

    } else if (mode == 1) {
      // 模式1：中值滤波（冒泡排序后取中间值）
      uint32_t sorted[16];
      for (uint16_t i = 0; i < count; i++) {
        sorted[i] = ch->samples[i];
      }
      // 冒泡排序
      for (uint16_t i = 0; i < count - 1; i++) {
        for (uint16_t j = 0; j < count - 1 - i; j++) {
          if (sorted[j] > sorted[j + 1]) {
            uint32_t temp = sorted[j];
            sorted[j] = sorted[j + 1];
            sorted[j + 1] = temp;
          }
        }
      }
      // 取中值
      if (count % 2 == 1) {
        result = sorted[count / 2];
      } else {
        result = (sorted[count / 2 - 1] + sorted[count / 2]) / 2;
      }

    } else if (mode == 2) {
      // 模式2：去极值平均（去掉最大最小值后求平均）
      if (count <= 2) {
        uint32_t sum = 0;
        for (uint16_t i = 0; i < count; i++) {
          sum += ch->samples[i];
        }
        result = sum / count;
      } else {
        uint32_t min_val = ch->samples[0];
        uint32_t max_val = ch->samples[0];
        uint32_t sum = ch->samples[0];

        for (uint16_t i = 1; i < count; i++) {
          sum += ch->samples[i];
          if (ch->samples[i] < min_val)
            min_val = ch->samples[i];
          if (ch->samples[i] > max_val)
            max_val = ch->samples[i];
        }
        result = (sum - min_val - max_val) / (count - 2);
      }
    }

    ch->is_sampling = 0; // 确保状态重置
    return result;
  }
}

/**
 * @brief 兼容旧接口的电压采样函数（使用通道0）
 */
uint32_t process_voltage_reading(uint16_t total_time_ms, uint16_t sample_count,
                                 uint32_t (*voltage_read_func)(void),
                                 uint32_t (*get_tick_func)(void),
                                 void (*delay_func)(uint32_t),
                                 uint8_t filter_mode) {
  return process_voltage_reading_ex(0, total_time_ms, sample_count,
                                    voltage_read_func, get_tick_func,
                                    delay_func, filter_mode);
}

/**
 * @brief 获取系统时间（封装函数，供 process_voltage_reading 使用）
 */
uint32_t get_system_tick_ms(void) { return system_tick_ms; }
