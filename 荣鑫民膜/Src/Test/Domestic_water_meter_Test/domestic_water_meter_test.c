#include "ADC_CHK.h"
#include "GPIO.h"
#include "LED_CTRL.h"
#include "Protocol/protocol.h"
#include "Test_List.h"
#include "Utility/utility.h"
#include "ina219.h"
#include "app_config.h"
#include "jiliang_xieyi_Ctrl.h"
#include "time.h"
#include "tongxin_xieyi_Ctrl.h"
#include "tongxin_xieyi_irda_Ctrl.h"
#include "uart0.h"
#include "uart1.h"
#include "valve_ctrl.h"
#include <stdio.h>
#include <string.h>

// EasyLogger 日志标签
#define LOG_TAG "test"
#include <elog.h>

// 前向声明
void handle_read_accumulated_flux(void);
extern void reset_accumulated_flux_protocol(void);
extern void read_accumulated_flux_protocol(void);

// 声明外部函数
extern uint8_t get_gongwei_number(void);
extern uint8_t read_gongwei_pin_PC0(void);
extern uint8_t read_gongwei_pin_PD12(void);
extern uint8_t read_gongwei_pin_PB15(void);
extern uint8_t read_gongwei_pin_PE5(void);
// 声明外部变量
extern uint32_t protocol_response_param;
extern uint8_t water_temperture[2];
uint16_t water_temperature_value =
    0; // 默认0度,如果读取到数据，如果没设置电阻，读取到的就是20度，上位设置的默认值是200,20度，小端数据，发回C8
       // 00
struct Test_quanju_canshu Test_quanju_canshu_L;
enum Test_liucheng Test_liucheng_L = w_wait;
struct Test_jieguo Test_jiejuo_jilu;
struct Test_linshi_cunchushuju Test_linshi_cunchushuju_L;
enum test_xieyi_jilu test_xieyi_jilu_Rec = No_Receive;
enum test_xieyi_jilu_irda test_xieyi_jilu_irda_rec = h_No_Receive;
uint8_t shangg_chongfa = 0;
uint8_t biaohao_retry_count = 0; // 表号查询重试计数器
uint8_t w_end_executed = 0;      // 标记w_end步骤是否已执行
VT_TestResult valve_test_result = VT_IDLE;
uint8_t dayinbiaohaoyong[12];
extern uint32_t test_fixture_power_time; // 工装当前的供电时间，单位ms
// 全局变量控制重置所有步骤的首次调用标志
static uint8_t reset_first_call_flags = 0;
uint8_t test_famen_type =
    0; // 0无阀 1有阀, 是否有无阀门，根据上位pc发送的 开始测试指令来
uint8_t water_meter_type =
    0; // 0是机械表
       // 1是超声波水表，通过上位指令设置，上位开始指令从0数，第10个字节
       // bit0位 1是超声波水表 0是机械表
// ==================== 测试步骤日志和分割符函数 ====================
void print_test_step_separator(void) {
  log_i("----------------------------------------");
}

void print_test_step_header(const char *step_name, const char *description) {
  print_test_step_separator();
  log_i("【%s】: %s", step_name, description);
  print_test_step_separator();
}

void print_test_step_footer(const char *result) {
  log_i("【结果】: %s", result);
  log_i("");
}

void test_quanju_canshu_Init() { Test_quanju_canshu_L.time_softdelay_ms = 10; }
// 测试开始初始化
void test_start_Init() {
  // 主电供电关闭
  zhudian_gongdian_OFF();
  // 备用电源断开
  beidian_gongdian_OFF();
  // 主电压检测控制关闭
  zhudian_dianya_CHK_CTRL_OFF();
  // 霍尔停止检测
  huuoer_1_OFF();
  // 二级电源电压检测控制关闭
  erji_dianya_CHK_CTRL_OFF();
  // 霍尔1检测 关
  huuoer_1_OFF();
  // 霍尔2检测 关
  huuoer_2_OFF();
  // LED点亮一下代表开始检测
  LED_FLAG_Run();
  daowei_1_OFF(); // 这里实际是输入状态，水表那边有上拉
  daowei_2_OFF();
}
// 工位检测
void gongwei_jiance() {

  // 读取各个引脚电平状态
  uint8_t pc0 = read_gongwei_pin_PC0();
  uint8_t pd12 = read_gongwei_pin_PD12();
  uint8_t pb15 = read_gongwei_pin_PB15();
  uint8_t pe5 = read_gongwei_pin_PE5();

  // 计算跳线帽状态 (插上跳线帽时引脚为低电平)
  uint8_t jumper1 = !pc0;  // 丝印1跳线帽状态
  uint8_t jumper2 = !pd12; // 丝印2跳线帽状态
  uint8_t jumper3 = !pb15; // 丝印3跳线帽状态
  uint8_t jumper4 = !pe5;  // 丝印4跳线帽状态

  // 获取工位编号
  Test_jiejuo_jilu.gongwei = get_gongwei_number();

  log_i("工位检测详情:");
  log_i("  引脚电平: PC0(丝印1)=%d, PD12(丝印2)=%d, PB15(丝印3)=%d, "
        "PE5(丝印4)=%d",
        pc0, pd12, pb15, pe5);
  log_i("  跳线帽状态: 1=%s, 2=%s, 3=%s, 4=%s", jumper1 ? "插上" : "未插",
        jumper2 ? "插上" : "未插", jumper3 ? "插上" : "未插",
        jumper4 ? "插上" : "未插");
  log_i("当前工位编号: %d", Test_jiejuo_jilu.gongwei);
  // log_i("");
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
  memset(Test_jiejuo_jilu.IMEI_CHK, 0x00, 15);
  memset(Test_jiejuo_jilu.IMSI_CHK, 0x00, 15);
  memset(Test_jiejuo_jilu.ICCID_CHK, 0x00, 20);
  Test_jiejuo_jilu.CSQ = 0;
  Test_jiejuo_jilu.FM1 = 0;
  Test_jiejuo_jilu.FM2 = 0;
  Test_jiejuo_jilu.FM = 0;
  Test_jiejuo_jilu.FM_daowei_1 = 0;
  Test_jiejuo_jilu.FM_daowei_2 = 0;
  Test_jiejuo_jilu.FM_daowei = 0;
  Test_jiejuo_jilu.EEPROM_jiance = 0;
  Test_jiejuo_jilu.GP30_dianya = 0;
  memset(Test_jiejuo_jilu.loraEUI, 0x00, 16);
  Test_jiejuo_jilu.qiangci_jiance = 0;
  Test_jiejuo_jilu.qiangci_jiance_1 = 0;
  Test_jiejuo_jilu.qiangci_jiance_2 = 0;
  Test_jiejuo_jilu.kaigai_jiance = 0;
  Test_jiejuo_jilu.GPSmozu_jiacne = 0;
  Test_quanju_canshu_L.test_status = TEST_RUNNING; // 初始化为测试进行中

  // 清理失败步骤记录（每次测试前清0）
  Test_quanju_canshu_L.failed_step = w_wait;
  memset(Test_quanju_canshu_L.status_description, 0,
         sizeof(Test_quanju_canshu_L.status_description));

  memset(Test_jiejuo_jilu.dangqian_biaohao, 0x00, 6);
  Test_jiejuo_jilu.jiliang_1_1 = 0;
  Test_jiejuo_jilu.jiliang_1_2 = 0;
  Test_jiejuo_jilu.jiliang_2_1 = 0;
  Test_jiejuo_jilu.jiliang_2_2 = 0;
  memset(Test_jiejuo_jilu.jiaoyanma, 0x00, 2);
  memset(Test_jiejuo_jilu.banbenhao, 0x00, 2);
  Test_jiejuo_jilu.water_temp = 0;
  Test_jiejuo_jilu.pressure_value = 0;

  // Bug1修复: 暴力清除整个临时结构体，防止残留上一次测试的数据
  // 包括IMEI, IMSI, ICCID, 信号强度等关键数据
  memset(&Test_linshi_cunchushuju_L, 0, sizeof(Test_linshi_cunchushuju_L));

  water_temperature_value = 0;
  // 清理全局温度数组
  water_temperture[0] = 0;
  water_temperture[1] = 0;

  //  复位全局重试计数器，防止上一次测试的计数值影响新测试
  shangg_chongfa = 0;
  biaohao_retry_count = 0;
  test_xieyi_jilu_Rec = No_Receive;
  test_xieyi_jilu_irda_rec = h_No_Receive;
}

// 开始测试
void domestic_water_meter_test_start(void) {
  // IO口恢复
  test_start_Init();
  // 清除结果缓存
  test_jieguo_qingling();

  // 重置所有步骤的首次调用标志，允许新测试显示日志
  reset_first_call_flags = 1;

  // 重置w_end执行标志，允许新测试正常执行
  w_end_executed = 0;

  Test_liucheng_L = w_start;
  // 设置超时时间90秒
  Test_quanju_canshu_L.time_aroundtest_ms = 120000; // 全局超时120秒
  // 单步测试时间
  Test_quanju_canshu_L.danbu_chaoshishijian_ms = 3000;
  Test_quanju_canshu_L.test_status = TEST_RUNNING; // 设置为测试进行中
  Test_quanju_canshu_L.time_softdelay_ms = 0;
  log_i("开始测试模块:");
  log_d("收到PC发送的模块编号");
  protocol_debug_print(Test_jiejuo_jilu.mokuaihao, 6);
}
// 手动停止测试
// 用于强制结束测试流程，将状态设置为手动停止
void test_testend() {
  Test_liucheng_L = w_end;
  Test_quanju_canshu_L.test_status = TEST_MANUAL_STOP; // 设置为手动停止
  Test_quanju_canshu_L.time_softdelay_ms = 0;
}

// 测试超时错误处理
void domestic_water_meter_test_err_end(void) {
  static uint8_t timeout_reported = 0;

  // 检查是否需要重置超时报告标志
  if (reset_first_call_flags) {
    timeout_reported = 0;
  }

  // 检查是否发生超时且测试仍在进行中
  if (Test_quanju_canshu_L.test_status == TEST_RUNNING) {
    if (Test_quanju_canshu_L.time_aroundtest_ms == 0) {
      // 全局测试超时 - 只报告一次
      if (!timeout_reported) {
        log_i("全局测试超时，强制结束测试");
        log_i("当前步骤：%s", get_test_step_name(Test_liucheng_L));
        timeout_reported = 1;
      }
      set_test_status_with_description(TEST_TIMEOUT_GLOBAL, Test_liucheng_L,
                                       "全局测试超时");
      Test_liucheng_L = w_end;
    } else if (Test_quanju_canshu_L.danbu_chaoshishijian_ms == 0) {
      // 单步测试超时 - 只报告一次
      if (!timeout_reported) {
        log_i("单步测试超时，强制结束测试");
        log_i("失败步骤：%s", get_test_step_name(Test_liucheng_L));
        timeout_reported = 1;
      }
      set_test_status_with_description(TEST_TIMEOUT_STEP, Test_liucheng_L,
                                       "单步测试超时");
      Test_liucheng_L = w_end;
    }
  }
}

void domestic_water_meter_test_loop(void) {
  domestic_water_meter_test_err_end();

  // 安全保护：检测工装长时间供电无响应（工人可能离开工位）
  if (test_fixture_power_time > POWER_ON_MAX_TIME_WITHOUT_RESPONSE) {
    log_w("工装供电超时（%lu ms），自动关闭电源保护", test_fixture_power_time);
    // 关闭主电
    zhudian_gongdian_OFF();
    // 关闭备电
    beidian_gongdian_OFF();
    // 清零工装供电时间计数器
    test_fixture_power_time = 0;
    // 重置测试流程到等待状态
    Test_liucheng_L = w_wait;
    return;
  }

  // 如果当前步骤是w_end并且已经执行过，则直接返回，避免重复执行
  if (Test_liucheng_L == w_end && w_end_executed) {
    return;
  }
  // 检查软延时
  if (Test_quanju_canshu_L.time_softdelay_ms > 0) {
    return;
  }

  // 实时更新当前步骤（用于查询当前进度，失败时保留最后停留步骤）
  if (Test_liucheng_L != w_wait && Test_liucheng_L != w_end) {
    Test_quanju_canshu_L.failed_step = Test_liucheng_L;
  }

  switch (Test_liucheng_L) {
  case w_wait:
    handle_test_wait();
    break;
  case w_start:
    handle_test_start();
    break;
  case w_close_valve_wait_response:
    handle_close_valve_wait_response();
    break;
  case w_reset_accumulated_flux:
    handle_reset_accumulated_flux();
    break;
  case w_qiehuan_kongshui:
    handle_test_qiehuan_kongshui();
    break;
  case w_gonghao_CHK:
    handle_test_gonghao_chk();
    break;
  case w_qiehuan_manshui:
    handle_test_qiehuan_manshui();
    break;
  case w_gonghao_manshui_CHK:
    handle_test_gonghao_manshui_chk();
    break;
  case w_qiehuan_zoushui:
    handle_test_qiehuan_zoushui();
    break;
  case w_gonghao_zoushui_CHK:
    handle_test_gonghao_zoushui_chk();
    break;

  case w_diancitie_huanxing_irda:
    handle_test_diancitie_huanxing_irda();
    break;
  case get_biaohao:
    handle_test_get_biaohao();
    break;

  /*========================================================================
   * 阀门测试流程 - 只检查 Test_loop_func() 返回的结果，不执行具体检测逻辑
   * 具体的阀门电压检测和到位信号控制由 Test_loop_func() 独立完成
   *========================================================================*/
  case w_famenceshi: {
    // 使用 enable_valve_test 判断是否已启动阀门测试
    // 如果 enable_valve_test 为 0，说明需要启动

    /* 无阀测试直接跳过，进入温度检测 */
    if (test_famen_type == 0) {
      log_i("无阀测试，跳过阀门测试步骤，进入温度检测");
      Test_jiejuo_jilu.FM = 0;
      Test_liucheng_L = w_zhudong_shangg;
      Test_quanju_canshu_L.time_softdelay_ms = 0;
      return;
    }

    /* 有阀测试：检查阀门测试结果 */
    if (valve_test_result == VT_SUCCESS) {
      // 阀门测试成功
      log_i(">> ✅ 阀门测试成功完成");
      Test_quanju_canshu_L.time_softdelay_ms = 0;
      // 停止阀门测试
      ValveCtrl_Stop();
      enable_valve_test = 0;
      valve_test_result = VT_IDLE; // 重置结果
      // 在 ValveCtrl_Stop() 之后设置结果，避免被库函数覆盖
      Test_jiejuo_jilu.FM = 1;
      Test_jiejuo_jilu.FM_daowei = 1;
      // 测试完成进入温度检测步骤
      Test_liucheng_L = w_zhudong_shangg;
    } else if (valve_test_result == VT_FAIL ||
               valve_test_result == VT_TIMEOUT) {
      // 阀门测试失败，打印详细原因
      ValveCtrl_Context_t *ctx = ValveCtrl_GetContext();
      log_i(">> ❌ 阀门测试失败");
      log_i("   失败步骤: %s", ValveCtrl_GetStepName());
      log_i("   失败原因: %s", ValveCtrl_GetFailReasonStr());
      log_d("   当前数据: 开阀到位=%d, 关阀到位=%d, 电压A=%lumV, 电压B=%lumV",
            ctx->pos_open, ctx->pos_close, ctx->voltage_a, ctx->voltage_b);
      Test_jiejuo_jilu.FM = 0;
      Test_jiejuo_jilu.FM_daowei = 0;
      Test_quanju_canshu_L.test_status = TEST_ERROR_HARDWARE;
      enable_valve_test = 0;
      valve_test_result = VT_IDLE;
      Test_liucheng_L = w_end;
    } else if (!enable_valve_test) {
      // 首次进入（enable_valve_test为0），启动阀门测试
      enable_valve_test = 1;
      ValveCtrl_Start(); // 启动阀门测试状态机
      log_i("当前是有阀项目，启动阀门测试");
    }
    // VT_RUNNING 或 VT_IDLE（已启动）：继续等待结果
    break;
  }

  case w_zhudong_shangg:
    handle_test_zhudong_shangg();
    break;
  case w_water_temp_CHK:
    handle_test_water_temp_chk();
    break;
  case get_banbenhao:
    handle_test_get_banbenhao();
    break;
    // 读取累计流量，判断是否有真实走水行为
  case w_read_accumulated_flux:
    handle_read_accumulated_flux();
    break;
  case w_shangg_jieguo_start:
    handle_test_shangg_jieguo_start();
    break;
  case w_shangg_jieguo:
    handle_test_shangg_jieguo();
    break;

  case w_end:
    // handle_test_end(); // TODO: 用户需要提供结束处理逻辑
    log_i("=== 进入w_end步骤 ===");
    // 注意：不清零 test_fixture_power_time，让超时保护继续生效
    // 只有当电源被关闭时（超时保护触发或手动关闭）才清零
    log_i("测试流程结束");

    // 根据测试状态输出结果
    switch (Test_quanju_canshu_L.test_status) {
    case TEST_SUCCESS:
      log_i("测试正常完成 ✓");
      break;
    case TEST_TIMEOUT_GLOBAL:
      log_i("测试因全局超时而结束 ✗");
      break;
    case TEST_TIMEOUT_STEP:
      log_i("测试因单步超时而结束 ✗");
      break;
    case TEST_MANUAL_STOP:
      log_i("测试被手动停止 ⏹");
      break;
    case TEST_ERROR_HARDWARE:
      log_i("测试因硬件错误而结束 ✗");
      break;
    case TEST_ERROR_PROTOCOL:
      log_i("测试因协议错误而结束 ✗");
      break;
    default:
      log_i("测试结束，状态未知");
      break;
    }

    // 确保从原始数据中提取温度值（防止超时跳过解析步骤时温度为0）
    if (water_temperature_value == 0) {
      water_temperature_value =
          Test_linshi_cunchushuju_L.L_water_temperature[0] +
          Test_linshi_cunchushuju_L.L_water_temperature[1] * 256;
    }

    // 发送详细的测试结果分析报告给PC
    log_i("=== 准备调用分析函数 ===");
    PC_test_result_analysis();
    log_i("=== 分析函数调用完成 ===");
    w_end_executed = 1; // 标记w_end步骤已执行
    break;
  default:
    print_test_step_header("ERROR", "未知测试步骤");
    log_i("未知测试步骤: %d", Test_liucheng_L);
    Test_liucheng_L = w_end;
    print_test_step_footer("强制结束测试");
    break;
  }
}

// 获取测试状态的便捷函数
// 返回当前测试状态，用于外部模块查询
enum Test_Status get_test_status(void) {
  return Test_quanju_canshu_L.test_status;
}

// 判断测试是否完成的便捷函数
// 返回1表示测试已完成（无论成功还是失败），0表示测试进行中
uint8_t is_test_finished(void) {
  return (Test_quanju_canshu_L.test_status != TEST_RUNNING) ? 1 : 0;
}

// 判断测试是否成功完成的便捷函数
// 返回1表示测试成功完成，0表示测试失败或未完成
uint8_t is_test_success(void) {
  return (Test_quanju_canshu_L.test_status == TEST_SUCCESS) ? 1 : 0;
}

// 设置测试状态和详细说明
void set_test_status_with_description(enum Test_Status status,
                                      enum Test_liucheng step,
                                      const char *description) {
  Test_quanju_canshu_L.test_status = status;
  Test_quanju_canshu_L.failed_step = step;

  // 清空状态说明缓冲区
  memset(Test_quanju_canshu_L.status_description, 0,
         sizeof(Test_quanju_canshu_L.status_description));

  // 如果提供了描述，则复制到缓冲区
  if (description != NULL) {
    strncpy(Test_quanju_canshu_L.status_description, description,
            sizeof(Test_quanju_canshu_L.status_description) - 1);
  }
}

// 获取测试状态说明
const char *get_test_status_description(void) {
  return Test_quanju_canshu_L.status_description;
}

// 获取测试步骤名称
const char *get_test_step_name(enum Test_liucheng step) {
  switch (step) {
  case w_wait:
    return "等待开始";
  case w_start:
    return "3.3V VCC测试";
  case w_close_valve_wait_response:
    return "5线阀关阀等待";
  case w_qiehuan_kongshui:
    return "切换为空水状态";
  case w_gonghao_CHK:
    return "功耗测试";
  case w_qiehuan_manshui:
    return "切换为满水状态";
  case w_gonghao_manshui_CHK:
    return "满水功耗测试";
  case w_qiehuan_zoushui:
    return "切换为走水状态";
  case w_gonghao_zoushui_CHK:
    return "走水功耗测试";
  case w_diancitie_huanxing_irda:
    return "电磁铁唤醒红外";
  case get_biaohao:
    return "获取表号";
  case get_banbenhao:
    return "获取版本号";
  case set_famen_peizhi:
    return "设置阀门配置";
  case w_zhudong_shangg:
    return "主动上报";
  case w_shangg_jieguo_start:
    return "第一次查询上报结果";
  case w_shangg_jieguo:
    return "查询上报结果等待上报完成";
  case w_famenceshi:
    return "阀门测试";
  case w_water_temp_CHK:
    return "水温检测";
  case w_end:
    return "测试结束";
  default:
    return "未知步骤";
  }
}

// ==================== 重构后的测试步骤处理函数 ====================

void handle_test_wait(void) {
  // 等待上位机发送测试指令，无需特殊处理
}

void handle_test_start(void) {
  // 启动工装供电时间计数器（设为1触发time.c中的累加）
  test_fixture_power_time = 1;

  print_test_step_header("电压检测", "检测产品是否装入");
  // 开启主电
  zhudian_gongdian_On();
  // 延迟50ms,等待主电稳定
  FL_DelayMs(50);
  // 重置w_end执行标志
  w_end_executed = 0;

  // 开始检测前校验VDD是否有电，从而判断产品是否装进
  Test_jiejuo_jilu.zhidian_dianya_gongdian = get_zhukongban_gongdian_dianya();

  if (Test_jiejuo_jilu.zhidian_dianya_gongdian > 3200 &&
      Test_jiejuo_jilu.zhidian_dianya_gongdian < 4000) {
    Test_quanju_canshu_L.time_softdelay_ms = 0;
    Test_quanju_canshu_L.danbu_chaoshishijian_ms = 30000;

    if (test_famen_type == 1) {
      // 5线阀上电后会开阀，需要先给开阀到位信号，然后发关阀命令
      // PA3(daowei_1)=开阀到位, PA11(daowei_2)=关阀到位
      daowei_2_OFF(); // 关阀到位信号=高电平(未到位)
      daowei_1_ON();  // 开阀到位信号=低电平(到位)
      log_i("5线阀：输出开阀到位信号(PA3=低)，准备发送关阀命令");
      FL_DelayMs(500);
      FM_Close_xieyi();                             // 发送关阀命令
      Test_quanju_canshu_L.time_softdelay_ms = 500; // 等待500ms
      Test_quanju_canshu_L.danbu_chaoshishijian_ms = 20000; // 关阀等待超时60秒
      // 进入下一个步骤，等待关阀动作完成
      Test_liucheng_L = w_close_valve_wait_response;
    } else {
      Test_liucheng_L = w_qiehuan_kongshui;
    }
    print_test_step_footer("电压正常 ✓");
  } else {

    Test_quanju_canshu_L.time_softdelay_ms = 1000;
    // 只在异常时显示详细信息
    log_i("电压异常: %lu mV (正常范围: 3200-4000 mV)",
          (unsigned long)Test_jiejuo_jilu.zhidian_dianya_gongdian);
  }
}

void handle_close_valve_wait_response(void) {
  static uint8_t handle_close_valve_wait_count = 0;
  if (test_xieyi_jilu_Rec == w_get_famen_dongzuo) {
    test_xieyi_jilu_Rec = No_Receive;
    log_i(">> ✅ 已收到关阀指令回复");
    // 提供关阀到位信号: PA3(daowei_1)=开阀到位, PA11(daowei_2)=关阀到位
    daowei_1_OFF(); // 开阀到位信号=高电平(未到位)
    daowei_2_ON();  // 关阀到位信号=低电平(到位)
    FL_DelayMs(300);
    daowei_2_OFF(); // 关阀到位信号恢复高电平(未到位)

    Test_quanju_canshu_L.time_softdelay_ms = 0;
    Test_quanju_canshu_L.danbu_chaoshishijian_ms = 20000;

    // 发送无阀门配置后，进入空水切换步骤
    handle_close_valve_wait_count = 0;
    Test_liucheng_L = w_reset_accumulated_flux;
    print_test_step_footer("5线阀关阀完成 ✓");
  } else {
    if (handle_close_valve_wait_count++ > 3) {
      log_i(">> ❌ 关阀指令无回复，发送配置阀门指令");
      handle_close_valve_wait_count = 0;
      // 发送配置5线阀阀门配置
      if (water_meter_type == 1) {
        configure_ultrasonic_valve(&Test_jiejuo_jilu.ultrasonic_config);
      } else if (water_meter_type == 0) {
        configure_mechanical_valve(&Test_jiejuo_jilu.mechanical_config);
      }
    }
    FM_Close_xieyi();
    log_i(">> 关阀指令已发送，等待回复...");
    Test_quanju_canshu_L.time_softdelay_ms = 2000;
  }
}
void handle_test_qiehuan_kongshui(void) {
  static uint8_t first_call = 1;
  static uint8_t retry_count = 0; // 快速失败重试计数器
  const uint8_t max_retry = 10;   // 最大重试次数
  if (first_call) {
    print_test_step_header("空水切换", "切换到空水状态");
    first_call = 0;
  }

  if (test_xieyi_jilu_Rec == w_get_kongshui) {
    test_xieyi_jilu_Rec = No_Receive;
    Test_quanju_canshu_L.time_softdelay_ms = 0;
    Test_quanju_canshu_L.danbu_chaoshishijian_ms = 20000;
    Test_liucheng_L = w_gonghao_CHK;
    first_call = 1; // 重置标志
    print_test_step_footer("空水状态切换成功 ✓");
    retry_count = 0; // 重置重试计数器

  } else {
    if (retry_count++ >= max_retry) {
      log_w("空水切换指令无响应，重试次数已达上限(%d次)，强制结束测试",
            max_retry);
      log_i("计量模块无响应，请检查连接是否正确或更换计量模块");
      set_test_status_with_description(TEST_ERROR_PROTOCOL, Test_liucheng_L,
                                       "空水切换指令无响应");
      Test_liucheng_L = w_end;
      first_call = 1;  // 重置标志
      retry_count = 0; // 重置重试计数器
      return;
    }
    JILIAN_wushui_xieyi();
    Test_quanju_canshu_L.time_softdelay_ms = 3000;
  }
}

// 清理水表之前的累计流量
void handle_reset_accumulated_flux(void) {
  static uint8_t first_call = 1;
  if (first_call) {
    print_test_step_header("清零累计流量", "发送清零累计流量指令");
    first_call = 0;
  }
  if (test_xieyi_jilu_Rec == w_set_accumulated_flux) {
    test_xieyi_jilu_Rec = No_Receive;
    Test_quanju_canshu_L.time_softdelay_ms = 0;
    Test_quanju_canshu_L.danbu_chaoshishijian_ms = 20000;
    Test_liucheng_L = w_qiehuan_kongshui;
    first_call = 1; // 重置标志
    print_test_step_footer("清零累计流量成功 ✓");
  } else {
    reset_accumulated_flux_protocol();
    Test_quanju_canshu_L.time_softdelay_ms = 2000;
  }
}
void handle_test_gonghao_chk(void) {
  static uint8_t first_call = 1;
  static uint8_t retry_count = 0; // 快速失败重试计数器
  const uint8_t max_retry = 3;    // 最大重试次数

  if (first_call) {
    print_test_step_header("空水功耗", "检测空水状态功耗");
    first_call = 0;
    retry_count = 0; // 重置重试计数器
  }

  Test_jiejuo_jilu.zhidian_jingtai_gonghao = static_power_check(
      200, 10, 200, 1); // 采样200ms,10次，只要有超过200uA快速失败
  if (Test_jiejuo_jilu.zhidian_jingtai_gonghao == 0xFFFE) {
    // 采样进行中，等待下次调用继续采样
    Test_jiejuo_jilu.zhidian_jingtai_gonghao = Current_CHK_Func(INA219_CH_MAIN);
    return;
  } else if (Test_jiejuo_jilu.zhidian_jingtai_gonghao == 0xFFFF) {
    // 快速失败
    retry_count++;
    Test_jiejuo_jilu.zhidian_jingtai_gonghao = Current_CHK_Func(INA219_CH_MAIN);
    if (retry_count < max_retry) {
      // 还有重试机会，等待后重试
      Test_quanju_canshu_L.time_softdelay_ms = 1000;
      return;
    }
    // 重试次数用完，继续往下走到异常处理
    log_i("快速失败重试%d次后仍失败", max_retry);
  }

  // 采样完成或重试次数用完，检查结果
  if (Test_jiejuo_jilu.zhidian_jingtai_gonghao > 0) {

    log_i("空水功耗测量值: %u μA",
          (unsigned int)Test_jiejuo_jilu.zhidian_jingtai_gonghao);
    if (Test_jiejuo_jilu.zhidian_jingtai_gonghao >= GONGHAO_KONGSHUI_MIN &&
        Test_jiejuo_jilu.zhidian_jingtai_gonghao <= GONGHAO_KONGSHUI_MAX) {
      Test_quanju_canshu_L.time_softdelay_ms = 0;
      Test_quanju_canshu_L.danbu_chaoshishijian_ms = 20000;
      // 使用配置无阀门函数，将当前表配置成无阀门
      // send_none_valve_configuration();

      Test_liucheng_L = w_qiehuan_manshui;
      first_call = 1; // 重置标志
      daowei_1_OFF(); // 关闭开阀到位信号输出，恢复高电平
      print_test_step_footer("空水功耗正常 ✓");

    } else {
      Test_quanju_canshu_L.time_softdelay_ms = 5000;
      // 只在异常时显示详细信息
      log_i("功耗异常: %u μA (正常范围: %u-%u μA)",
            (unsigned int)Test_jiejuo_jilu.zhidian_jingtai_gonghao,
            GONGHAO_KONGSHUI_MIN, GONGHAO_KONGSHUI_MAX);
      // 提供开阀到位信号（PA11输出低电平），让水表停止阀门动作
      daowei_1_ON(); // 开阀到位 = PA11 = daowei_2
      if (test_famen_type == 1) {
        send_none_valve_configuration();
        // configure_ultrasonic_valve_no_valve();
      } else if (test_famen_type == 0) {
        send_none_valve_configuration();
        // configure_ultrasonic_valve_no_valve();
      }
      log_i("输出开阀到位信号(PA11=低电平)");
      first_call = 1;  // 重置标志，下次重新开始
      retry_count = 0; // 重置重试计数器
    }
  }
}

void handle_test_qiehuan_manshui(void) {
  static uint8_t first_call = 1;

  if (first_call) {
    print_test_step_header("满水切换", "切换到满水状态");
    first_call = 0;
  }

  if (test_xieyi_jilu_Rec == w_get_manshui) {
    test_xieyi_jilu_Rec = No_Receive;
    Test_quanju_canshu_L.time_softdelay_ms = 0;
    Test_quanju_canshu_L.danbu_chaoshishijian_ms = 20000;
    Test_liucheng_L = w_gonghao_manshui_CHK;
    first_call = 1;
    print_test_step_footer("满水状态切换成功 ✓");
  } else {
    JILIAN_manshui_xieyi();
    Test_quanju_canshu_L.time_softdelay_ms = 3000;
  }
}

void handle_test_gonghao_manshui_chk(void) {
  static uint8_t first_call = 1;

  if (first_call) {
    print_test_step_header("满水功耗", "检测满水状态功耗");
    first_call = 0;
  }

  Test_jiejuo_jilu.zhidian_manshui_gonghao = static_power_check(
      200, 10, 200, 1); // 采样200ms,10次，只要有超过100uA快速失败 ;
  if (Test_jiejuo_jilu.zhidian_manshui_gonghao == 0xFFFE ||
      Test_jiejuo_jilu.zhidian_manshui_gonghao == 0xFFFF) {
    // 如果返回值为0xFFFE，表示采样进行中
    return; // 直接返回，等待下次调用继续采样
  }
  log_i("满水功耗测量值: %u μA",
        (unsigned int)Test_jiejuo_jilu.zhidian_manshui_gonghao);
  if (Test_jiejuo_jilu.zhidian_manshui_gonghao >= GONGHAO_MANSHUI_MIN &&
      Test_jiejuo_jilu.zhidian_manshui_gonghao <= GONGHAO_MANSHUI_MAX) {
    Test_quanju_canshu_L.time_softdelay_ms = 0;
    Test_quanju_canshu_L.danbu_chaoshishijian_ms = 20000;
    Test_liucheng_L = w_qiehuan_zoushui;
    first_call = 1;
    print_test_step_footer("满水功耗正常 ✓");
  } else {
    Test_quanju_canshu_L.time_softdelay_ms = 1000;
    log_i("功耗异常: %u μA (正常范围: %u-%u μA)",
          (unsigned int)Test_jiejuo_jilu.zhidian_manshui_gonghao,
          GONGHAO_MANSHUI_MIN, GONGHAO_MANSHUI_MAX);
  }
}

void handle_test_qiehuan_zoushui(void) {
  static uint8_t first_call = 1;

  if (first_call) {
    print_test_step_header("流水切换", "切换到流水状态");
    first_call = 0;
  }

  if (test_xieyi_jilu_Rec == w_get_zoushui) {
    test_xieyi_jilu_Rec = No_Receive;
    Test_quanju_canshu_L.time_softdelay_ms = 0;
    Test_quanju_canshu_L.danbu_chaoshishijian_ms = 20000;
    Test_liucheng_L = w_gonghao_zoushui_CHK;
    first_call = 1;
    print_test_step_footer("流水状态切换成功 ✓");
  } else {
    JILIAN_zoushui_xieyi();
    Test_quanju_canshu_L.time_softdelay_ms = 3000;
  }
}

void handle_test_gonghao_zoushui_chk(void) {
  static uint8_t first_call = 1;

  if (first_call) {
    print_test_step_header("流水功耗", "检测流水状态功耗");
    first_call = 0;
  }

  // 走水状态下水表有周期性采样脉冲，提高阈值和容错
  // 采样500ms,10次，连续3次超过500uA才快速失败
  Test_jiejuo_jilu.zhidian_zoushui_gonghao =
      static_power_check(200, 10, 500, 3);
  if (Test_jiejuo_jilu.zhidian_zoushui_gonghao == 0xFFFE ||
      Test_jiejuo_jilu.zhidian_zoushui_gonghao == 0xFFFF) {
    // 如果返回值为0xFFFE，表示采样进行中
    return; // 直接返回，等待下次调用继续采样
  }
  log_i("流水功耗测量值: %u μA",
        (unsigned int)Test_jiejuo_jilu.zhidian_zoushui_gonghao);
  // 走水状态功耗稍高，内部可能有flash读写，国内水表标准是低于40uA
  if (Test_jiejuo_jilu.zhidian_zoushui_gonghao >= GONGHAO_ZOUSHUI_MIN &&
      Test_jiejuo_jilu.zhidian_zoushui_gonghao <= GONGHAO_ZOUSHUI_MAX) {
    Test_quanju_canshu_L.time_softdelay_ms = 0;
    Test_quanju_canshu_L.danbu_chaoshishijian_ms = 20000;
    Test_liucheng_L = w_diancitie_huanxing_irda;
    first_call = 1;
    print_test_step_footer("流水功耗正常 ✓");
  } else {
    Test_quanju_canshu_L.time_softdelay_ms = 1000;
    log_i("功耗异常: %u μA (正常范围: %u-%u μA)",
          (unsigned int)Test_jiejuo_jilu.zhidian_zoushui_gonghao,
          GONGHAO_ZOUSHUI_MIN, GONGHAO_ZOUSHUI_MAX);
  }
}

void handle_test_diancitie_huanxing_irda(void) {
  print_test_step_header("红外初始化", "准备红外通信");

  // 设置默认表号，使用 Test_jiejuo_jilu.dangqian_biaohao
  memset(Test_jiejuo_jilu.dangqian_biaohao, 0xAA, 6);

  Test_quanju_canshu_L.time_softdelay_ms = 500;
  Test_quanju_canshu_L.danbu_chaoshishijian_ms = 30000; // 30秒超时
  biaohao_retry_count = 0;
  Test_liucheng_L = get_biaohao;
  print_test_step_footer("红外通信初始化完成 ✓");
}

void handle_test_get_biaohao(void) {
  static uint8_t first_call = 1;

  if (first_call) {
    print_test_step_header("表号查询", "获取水表表号");
    first_call = 0;
  }

  if (test_xieyi_jilu_Rec == w_get_biaohao) {
    test_xieyi_jilu_Rec = No_Receive;
    Test_quanju_canshu_L.time_softdelay_ms = 3000;
    shangg_chongfa = 0;
    Test_quanju_canshu_L.danbu_chaoshishijian_ms = 30000;
    Test_liucheng_L = w_famenceshi;
    first_call = 1;
    print_test_step_footer("表号查询成功 ✓");
  } else {
    find_biaohao_xieyi();
    Test_quanju_canshu_L.time_softdelay_ms = 5000;
    biaohao_retry_count++;

    if (biaohao_retry_count > 10) {
      log_i("表号查询重试次数已达上限 (%d次)", biaohao_retry_count);
      first_call = 1;
      print_test_step_footer("表号查询失败 ✗");
    }
  }
}

void handle_test_get_banbenhao(void) {
  static uint8_t first_call = 1;

  if (first_call) {
    print_test_step_header("版本查询", "获取水表版本号");
    first_call = 0;
  }

  if (test_xieyi_jilu_Rec == w_get_banbenhao) {
    test_xieyi_jilu_Rec = No_Receive;
    memcpy(Test_jiejuo_jilu.banbenhao, Test_linshi_cunchushuju_L.L_banbenhao,
           2);
    Test_quanju_canshu_L.time_softdelay_ms = 1000;
    shangg_chongfa = 0;
    Test_quanju_canshu_L.danbu_chaoshishijian_ms =
        60000; // 60秒超时，阀门测试流程较长
    Test_liucheng_L = w_read_accumulated_flux;
    first_call = 1;
    print_test_step_footer("版本号查询成功 ✓");
  } else {
    find_banbenhao_xieyi();
    Test_quanju_canshu_L.time_softdelay_ms = 5000;
  }
}

void handle_test_water_temp_chk(void) {
  static uint8_t first_call = 1;

  if (first_call) {
    print_test_step_header("温度配置", "设置水表温度参数");
    first_call = 0;
  }

  if (test_xieyi_jilu_Rec == w_set_water_temp) {
    test_xieyi_jilu_Rec = No_Receive;
    Test_quanju_canshu_L.time_softdelay_ms = 1000;
    shangg_chongfa = 0;
    Test_quanju_canshu_L.danbu_chaoshishijian_ms = 10000;
    Test_liucheng_L = get_banbenhao;
    first_call = 1;
    print_test_step_footer("温度配置成功 ✓");
  } else {
    set_water_temp();
    Test_quanju_canshu_L.time_softdelay_ms = 1500;
  }
}

void handle_test_zhudong_shangg(void) {
  static uint8_t first_call = 1;

  if (first_call) {
    print_test_step_header("主动上报", "触发水表上报数据");
    first_call = 0;
  }

  if (test_xieyi_jilu_Rec == w_get_shanggao || shangg_chongfa > 1) {
    test_xieyi_jilu_Rec = No_Receive;
    Test_jiejuo_jilu.hongwai_jiance = 1;
    Test_quanju_canshu_L.time_softdelay_ms = 0;
    Test_quanju_canshu_L.danbu_chaoshishijian_ms = 10000;
    Test_liucheng_L = w_water_temp_CHK;
    first_call = 1;
    print_test_step_footer(
        "主动上报触发成功,"
        "下一步不要着急查找上告数据，这个上告过程时间很长，做点别的 ✓");
  } else {
    shangg_chongfa++;
    send_shanggao_xieyi();
    Test_quanju_canshu_L.time_softdelay_ms = 3000;
  }
}

void handle_test_shangg_jieguo_start(void) {
  static uint8_t first_call = 1;

  if (first_call) {
    print_test_step_header("数据查询", "查询上报结果");
    first_call = 0;
  }

  if (test_xieyi_jilu_Rec == w_get_test_zhuanyong) {
    test_xieyi_jilu_Rec = No_Receive;
    Test_quanju_canshu_L.time_softdelay_ms = 1000;
    Test_quanju_canshu_L.danbu_chaoshishijian_ms = 60000;
    Test_liucheng_L = w_shangg_jieguo;
    first_call = 1;
    print_test_step_footer("开始查询上报结果 ✓");
  } else {
    Test_zhuanyong_xieyi();
    Test_quanju_canshu_L.time_softdelay_ms = 3000;
  }
}

void handle_test_shangg_jieguo(void) {
  static uint8_t first_call = 1;

  if (first_call) {
    print_test_step_header("数据解析", "解析上报数据");
    first_call = 0;
  }

  if (test_xieyi_jilu_Rec == w_get_test_zhuanyong &&
      (Test_linshi_cunchushuju_L.L_CSQ > 5 &&
       Test_linshi_cunchushuju_L.L_CSQ < 99 &&
       Test_linshi_cunchushuju_L.L_IMEI[0] != 0xFF &&
       Test_linshi_cunchushuju_L.L_IMSI[0] != 0xFF &&
       Test_linshi_cunchushuju_L.L_ICCID[0] != 0xFF &&
       Test_linshi_cunchushuju_L.L_IMEI[0] != 0x00 &&
       Test_linshi_cunchushuju_L.L_IMSI[0] != 0x00 &&
       Test_linshi_cunchushuju_L.L_ICCID[0] != 0x00)) {
    uint32_t liuliang_value = 0;
    test_xieyi_jilu_Rec = No_Receive;

    // 解析各种检测结果
    Test_jiejuo_jilu.CSQ = Test_linshi_cunchushuju_L.L_CSQ;
    Test_jiejuo_jilu.flash_jiance = Test_linshi_cunchushuju_L.L_flash_chk;
    Test_jiejuo_jilu.zhidian_dianya_huoqu =
        Test_linshi_cunchushuju_L.L_zhudian_dianya;
    Test_jiejuo_jilu.beidian_dianya_huoqu =
        Test_linshi_cunchushuju_L.L_beidian_dianya;
    Test_jiejuo_jilu.lanya_jiance = Test_linshi_cunchushuju_L.L_yali_CHK;
    Test_jiejuo_jilu.EEPROM_jiance = Test_linshi_cunchushuju_L.L_EEPROM_CHK;
    Test_jiejuo_jilu.GPSmozu_jiacne = Test_linshi_cunchushuju_L.L_GPRS_mozu_CHK;
    Test_jiejuo_jilu.GP30_dianya = Test_linshi_cunchushuju_L.L_GP30mozu_AD_CHK;

    // 复制标识信息并检测是否全为0（水表未上报）
    memcpy(Test_jiejuo_jilu.IMEI_CHK, Test_linshi_cunchushuju_L.L_IMEI, 15);
    memcpy(Test_jiejuo_jilu.IMSI_CHK, Test_linshi_cunchushuju_L.L_IMSI, 15);
    memcpy(Test_jiejuo_jilu.ICCID_CHK, Test_linshi_cunchushuju_L.L_ICCID, 20);

    // 检测IMEI/IMSI/ICCID是否全为0（只有全0才算未上报）
    uint8_t imei_all_zero = 1;
    uint8_t imsi_all_zero = 1;
    uint8_t iccid_all_zero = 1;
    int i;

    for (i = 0; i < 15; i++) {
      if (Test_jiejuo_jilu.IMEI_CHK[i] != 0) {
        imei_all_zero = 0;
        break;
      }
    }
    for (i = 0; i < 15; i++) {
      if (Test_jiejuo_jilu.IMSI_CHK[i] != 0) {
        imsi_all_zero = 0;
        break;
      }
    }
    for (i = 0; i < 20; i++) {
      if (Test_jiejuo_jilu.ICCID_CHK[i] != 0) {
        iccid_all_zero = 0;
        break;
      }
    }

    // 打印检测结果
    if (imei_all_zero) {
      log_i("IMEI: 水表未上报");
    } else {
      log_i("IMEI: 已获取 ✓");
    }
    if (imsi_all_zero) {
      log_i("IMSI: 水表未上报");
    } else {
      log_i("IMSI: 已获取 ✓");
    }
    if (iccid_all_zero) {
      log_i("ICCID: 水表未上报");
    } else {
      log_i("ICCID: 已获取 ✓");
    }

    // 处理温度数据
    water_temperature_value =
        Test_linshi_cunchushuju_L.L_water_temperature[0] +
        Test_linshi_cunchushuju_L.L_water_temperature[1] * 256;

    if (water_temperature_value > 200 && water_temperature_value < 300) {
      Test_jiejuo_jilu.water_temp = 1; // 1表示正常
      log_i("温度: %u.%u°C ✓", water_temperature_value / 10,
            water_temperature_value % 10);
    } else {
      Test_jiejuo_jilu.water_temp = 0; // 0表示异常
      log_i("温度异常: %u.%u°C ✗", water_temperature_value / 10,
            water_temperature_value % 10);
      // 温度异常时直接结束测试
      Test_quanju_canshu_L.test_status = TEST_ERROR_HARDWARE;
      if (imsi_all_zero && imei_all_zero && iccid_all_zero) {
        strcat(Test_quanju_canshu_L.status_description, "水表未上报; ");
        // 流程不处理，等待重试
      } else {
        Test_liucheng_L = w_end;
      }
      first_call = 1;
      print_test_step_footer("温度检测失败，测试结束 ✗");
      return; // 立即返回，避免继续执行后续代码
    }

    // 检查超声流量数据
    // memcpy(&liuliang_value,
    //         Test_linshi_cunchushuju_L.L_chaosheng_shunshiliuliang, 4);
    // if (liuliang_value > 0) {
    //  Test_jiejuo_jilu.jiliang_jiance = 1;
    // }

    // 启用阀门到位检测,提供高电平，实际水表那边默认可能是上拉
    daowei_1_OFF();
    daowei_2_OFF();

    // 数据解析完成，阀门测试已在 w_famenceshi 步骤完成
    // 直接进入测试结束流程
    Test_quanju_canshu_L.test_status = TEST_SUCCESS;
    Test_liucheng_L = w_end;
    shangg_chongfa = 0;
    first_call = 1;
    print_test_step_footer("数据解析完成 ✓");
  } else {
    Test_zhuanyong_xieyi();
    Test_quanju_canshu_L.time_softdelay_ms = 3500;
    shangg_chongfa++;

    if (shangg_chongfa == 10) {
      // 第一次重试上限，重新触发上报，计数器设为11表示已重试过一轮
      log_i("数据查询重试次数达到 %d 次，重新触发上报", shangg_chongfa);
      shangg_chongfa = 11; // 标记已经重试过一次上报
      Test_liucheng_L = w_zhudong_shangg;
      first_call = 1;
      print_test_step_footer("数据查询失败，重新上报 ✗");
    } else if (shangg_chongfa > 20) {
      // 第二轮重试也失败，彻底失败
      log_i("数据查询重试次数已达上限 (%d次)，测试失败", shangg_chongfa);
      Test_quanju_canshu_L.test_status = TEST_ERROR_PROTOCOL;
      Test_liucheng_L = w_end;
      first_call = 1;
      print_test_step_footer("数据查询失败 ✗");
    }
    // 其他情况继续重试查询
  }
}

void handle_test_famenceshi(void) {
  static uint8_t first_call = 1;
  if (first_call) {
    print_test_step_header("阀门功能测试", "阀门到位检测");
    first_call = 0;
  }
  if (test_xieyi_jilu_Rec == w_get_famen_dongzuo_L) {
    // 阀门动作测试完成，设置成功状态
    test_xieyi_jilu_Rec = No_Receive;
    Test_quanju_canshu_L.danbu_chaoshishijian_ms = 10000;
    Test_quanju_canshu_L.test_status = TEST_SUCCESS; // 阀门测试成功完成
    Test_liucheng_L = w_end;
  } else {
    log_i("阀门状态 FM1=%u FM2=%u", (unsigned int)Test_jiejuo_jilu.FM1,
          (unsigned int)Test_jiejuo_jilu.FM2);
    log_i("开始阀门测试");
    FM_Open_xieyi(); // 发送阀门开启协议
    Test_quanju_canshu_L.time_softdelay_ms = 8000;
  }
}

void handle_test_end(void) {
  static uint8_t first_call = 1;

  // Bug2修复: 强制停止阀门测试组件，防止异常结束时组件还在运行
  // 这可以避免资源冲突、GPIO干扰和死锁问题
  extern volatile uint8_t enable_valve_test;
  if (ValveCtrl_IsRunning()) {
    ValveCtrl_Stop();
  }
  enable_valve_test = 0; // 确保 main.c 中的标志位也被清除

  // 检查是否需要重置首次调用标志
  if (reset_first_call_flags) {
    first_call = 1;
    reset_first_call_flags = 0; // 在最后一个步骤重置全局标志
  }

  if (first_call) {
    print_test_step_header("测试结束", "测试流程结束");

    // 根据不同的测试状态输出对应的结束信息
    switch (Test_quanju_canshu_L.test_status) {
    case TEST_SUCCESS:
      log_i("测试正常完成 ✓");
      break;
    case TEST_TIMEOUT_GLOBAL:
      log_i("测试因全局超时而结束 ✗");
      break;
    case TEST_TIMEOUT_STEP:
      log_i("测试因单步超时而结束 ✗");
      break;
    case TEST_MANUAL_STOP:
      log_i("测试被手动停止 ⏹");
      break;
    case TEST_ERROR_HARDWARE:
      log_i("测试因硬件错误而结束 ✗");
      break;
    case TEST_ERROR_PROTOCOL:
      log_i("测试因协议错误而结束 ✗");
      break;
    default:
      log_i("测试结束，状态未知");
      break;
    }

    // 打印测试结果统计
    log_i("测试结果统计:");
    log_i("- 电压: %lu mV",
          (unsigned long)Test_jiejuo_jilu.zhidian_dianya_gongdian);
    log_i("- 空水功耗: %u μA",
          (unsigned int)Test_jiejuo_jilu.zhidian_jingtai_gonghao);
    log_i("- 满水功耗: %u μA",
          (unsigned int)Test_jiejuo_jilu.zhidian_manshui_gonghao);
    log_i("- 流水功耗: %u μA",
          (unsigned int)Test_jiejuo_jilu.zhidian_zoushui_gonghao);
    log_i("- 水温: %u.%u°C", water_temperature_value / 10,
          water_temperature_value % 10);
    log_i("- 阀门状态: FM1=%d, FM2=%d", Test_jiejuo_jilu.FM1,
          Test_jiejuo_jilu.FM2);
    log_i("- 红外通信: %s", Test_jiejuo_jilu.hongwai_jiance ? "正常" : "异常");

    // 恢复硬件初始状态
    zhudian_gongdian_On();
    beidian_gongdian_OFF();
    huuoer_1_OFF();
    huuoer_2_OFF();

    print_test_step_footer("测试流程完成，系统返回待机状态");
    first_call = 0;
  }

  // 保持在结束状态，不再循环
  // Test_liucheng_L = w_wait;  // 注释掉自动重启，避免重复显示
}

/**
 * @brief 静态功耗检测（在定时器中断中周期调用）
 *
 * @param total_time  总的检测功耗时间 (ms)
 * @param total_count  采样次数
 * @param early_fail_threshold  快速失败阈值（uA），0表示不启用
 * @param early_fail_count  连续多少次超过阈值后快速失败（如3表示连续3次）
 *                          如果前N次采样值都超过阈值，直接返回0xFFFF表示快速失败
 * @return uint16_t
 * 功耗值（采样进行中返回0xFFFE，完成返回平均值，快速失败返回0xFFFF）
 *
 * @note 使用去极值滤波：去掉最高1个和最低1个采样值后计算平均
 *       可有效滤除偶发的高值（如flash写入期间）和低值干扰
 */
uint16_t static_power_check(uint16_t total_time, uint16_t total_count,
                            uint16_t early_fail_threshold,
                            uint8_t early_fail_count) {
  static uint16_t count = 0;            // 当前采样次数
  static uint16_t samples[32];          // 采样值数组（最多32次）
  static uint32_t last_sample_time = 0; // 上次采样时间（毫秒）
  static uint32_t start_time = 0;       // 测量开始时间（毫秒）
  static uint8_t is_measuring = 0;      // 是否正在测量中
  static uint8_t fail_count = 0;        // 连续失败次数计数器

  // 参数检查
  if (total_count == 0 || total_time == 0) {
    return 0;
  }

  // 限制最大采样次数
  if (total_count > 32) {
    total_count = 32;
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
    fail_count = 0;
    start_time = current_time;
    last_sample_time = current_time;
    memset(samples, 0, sizeof(samples));
    log_i("开始静态功耗测试：总时间=%dms, 采样次数=%d, 间隔=%dms (去极值滤波)",
          total_time, total_count, period);
    return 0xFFFE; // 第一次调用，开始测量（采样进行中）
  }

  // 检查是否超时（给予足够的容错时间：至少10秒或total_count*500ms，取大值）
  // 因为主循环调用频率可能较低，每次采样间隔可能远大于理论值
  uint32_t timeout_ms = total_count * 500; // 每次采样最多500ms
  if (timeout_ms < 10000)
    timeout_ms = 10000; // 最少10秒超时

  if (current_time - start_time > timeout_ms) {
    // 超时，重置状态
    log_i("静态功耗测试超时（经过%dms），重置", current_time - start_time);
    is_measuring = 0;
    count = 0;
    fail_count = 0;
    return 0xFFFE; // 超时重置，需要重新开始（采样进行中）
  }

  // 到达采样时间点
  if (current_time - last_sample_time >= period) {
    last_sample_time = current_time; // 更新上次采样时间

    // 执行采样
    uint16_t current_power = Current_CHK_Func(INA219_CH_MAIN);
    samples[count] = current_power;
    count++;
    log_i("静态功耗采样 %d/%d: 当前功耗=%d uA", count, total_count,
          current_power);

    // 快速失败检测（如果启用）
    if (early_fail_threshold > 0 && early_fail_count > 0) {
      if (current_power > early_fail_threshold) {
        fail_count++;
        log_i("静态功耗采样第%d次超过阈值%d uA，连续失败%d/%d次", count,
              early_fail_threshold, fail_count, early_fail_count);

        // 达到连续失败次数，快速失败
        if (fail_count >= early_fail_count) {
          log_i("连续%d次采样超过阈值，快速失败", early_fail_count);
          // 重置状态
          is_measuring = 0;
          count = 0;
          fail_count = 0;
          return 0xFFFF; // 返回特殊值表示快速失败
        }
      } else {
        // 本次采样正常，重置失败计数器
        if (fail_count > 0) {
          log_i("本次采样正常，重置失败计数器");
          fail_count = 0;
        }
      }
    }

    // 检查是否完成所有采样
    if (count >= total_count) {
      uint16_t avg_power;

      // 根据采样次数选择滤波方式
      if (total_count >= 4) {
        // 采样次数>=4时，去掉最高1个和最低1个
        avg_power = util_filter_remove_extreme(samples, total_count, 1, 1);
        log_i("静态功耗测试完成：去极值平均功耗=%d uA (去掉最高最低各1个)",
              avg_power);
      } else {
        // 采样次数<4时，直接计算平均
        avg_power = util_filter_average(samples, total_count);
        log_i("静态功耗测试完成：简单平均功耗=%d uA", avg_power);
      }

      // 重置状态，准备下次测量
      is_measuring = 0;

      return avg_power; // 返回平均功耗
    }
  }

  return 0xFFFE; // 采样进行中
}

/**
 * @brief 配置阀门相关参数（仅适用于机械表阀门，超声波表未验证）
 * @param pipe_diameter_mm 阀门管径（单位：mm），常见值：15、20、25、32
 * @param valve_type 阀门类型：
 *        - 0: 无阀门
 *        - 1: 5线阀
 *        - 2: 2线阀
 * @param timeout_seconds 阀门动作超时时间（单位：秒），建议值：12~20
 * @param stall_current_ma 堵转电流阈值（单位：mA），典型值：230
 * @note 数据按小端格式发送（低位在前，高位在后）
 * @example 配置为无阀门状态（用于测试低功耗）：
 *          configure_valve_parameters(15, 0, 20, 230);
 *          // 15mm管径, 无阀门, 20秒超时, 230mA堵转电流
 */
/*============ 阀门配置函数（结构体版本） ============*/

/**
 * @brief 配置机械表阀门参数
 * @param config 机械表阀门配置结构体指针
 * @note 发送协议 0x2604，数据按小端格式
 */
void configure_mechanical_valve(const MechanicalValveConfig *config) {
  if (config == NULL)
    return;

  uint8_t valve_config_data[] = {
      (uint8_t)(config->pipe_diameter_mm & 0xFF),        // 管径低位
      (uint8_t)((config->pipe_diameter_mm >> 8) & 0xFF), // 管径高位
      config->valve_type,                                // 阀门类型
      config->timeout_seconds,                           // 超时时间
      (uint8_t)(config->stall_current_ma & 0xFF),        // 堵转电流低位
      (uint8_t)((config->stall_current_ma >> 8) & 0xFF)  // 堵转电流高位
  };

  TONGXIN_xieyifasong(0x2604, 0x04, Test_jiejuo_jilu.dangqian_biaohao,
                      valve_config_data, 6);
}

/**
 * @brief 配置超声波表阀门参数
 * @param config 超声波表阀门配置结构体指针
 * @note 发送协议 0x2036，数据按小端格式
 */
void configure_ultrasonic_valve(const UltrasonicValveConfig *config) {
  if (config == NULL)
    return;

  uint8_t ultrasonic_config_data[] = {
      config->pipe_type,                               // 管道类型
      config->transducer_type,                         // 换能器类型
      (uint8_t)(config->pipe_size_type & 0xFF),        // 管径类型低位
      (uint8_t)((config->pipe_size_type >> 8) & 0xFF), // 管径类型高位
      config->valve_type,                              // 阀门类型
      config->module_type                              // 模块类型
  };

  TONGXIN_xieyifasong(0x2036, 0x04, Test_jiejuo_jilu.dangqian_biaohao,
                      ultrasonic_config_data, 6);
}

/**
 * @brief 使用默认参数快速配置机械表阀门
 * @param pipe_diameter_mm 管径（mm）
 * @param valve_type 阀门类型：0=无阀, 1=5线阀, 2=2线阀
 */
void configure_mechanical_valve_default(uint16_t pipe_diameter_mm,
                                        uint8_t valve_type) {
  MechanicalValveConfig config = {
      .pipe_diameter_mm = pipe_diameter_mm,
      .valve_type = valve_type,
      .timeout_seconds = 12,  // 默认12秒超时
      .stall_current_ma = 230 // 默认230mA堵转电流
  };
  configure_mechanical_valve(&config);
}

/**
 * @brief 使用默认参数快速配置超声波表阀门
 * @param valve_type 阀门类型
 */
void configure_ultrasonic_valve_default(uint8_t valve_type) {
  UltrasonicValveConfig config = {
      .pipe_type = 0,       // 默认管道类型
      .transducer_type = 0, // 默认换能器类型
      .pipe_size_type = 15, // 默认15mm管径
      .valve_type = valve_type,
      .module_type = 0 // 默认模块类型
  };
  configure_ultrasonic_valve(&config);
}

/**
 * @brief  配置超声波无阀门状态（用于测试低功耗）
 */
void configure_ultrasonic_valve_no_valve(void) {
  UltrasonicValveConfig config = {
      .pipe_type = 2, // 管道类型,必须配置，否则水表判定非法
      .transducer_type = 0, // 默认换能器类型
      .pipe_size_type = 15, // 默认15mm管径
      .valve_type = 0x00,   // 无阀门
      .module_type =
          Test_jiejuo_jilu.ultrasonic_config
              .module_type, // 模块类型,不能配置 错误，如果错了，上告可能失败
  };
  configure_ultrasonic_valve(&config);
}

void send_none_valve_configuration(void) {
  // 根据阀门类型发送无阀门配置
  if (Test_jiejuo_jilu.biaoju_leixing_famen == 0) {
    // 机械表
    configure_mechanical_valve_default(15, 0); // 15mm管径, 无阀门
  } else {
    // 超声波表
    configure_ultrasonic_valve_no_valve();
  }
}

/**
 * @brief 从字节数组解析机械表阀门配置（用于解析上位机协议）
 * @param data 6字节数据数组（小端格式）
 * @param config 输出的配置结构体
 * @note
 * 字节顺序：[管径低][管径高][阀门类型][超时秒数][堵转电流低][堵转电流高]
 */
void parse_mechanical_config_from_bytes(const uint8_t *data,
                                        MechanicalValveConfig *config) {
  if (data == NULL || config == NULL)
    return;

  config->pipe_diameter_mm = data[0] | (data[1] << 8); // 小端：低位在前
  config->valve_type = data[2];
  config->timeout_seconds = data[3];
  config->stall_current_ma = data[4] | (data[5] << 8); // 小端：低位在前
}

/**
 * @brief 从字节数组解析超声波表阀门配置（用于解析上位机协议）
 * @param data 6字节数据数组
 * @param config 输出的配置结构体
 * @note
 * 字节顺序：[管道类型][换能器类型][管径类型低][管径类型高][阀门类型][模块类型]
 */
void parse_ultrasonic_config_from_bytes(const uint8_t *data,
                                        UltrasonicValveConfig *config) {
  if (data == NULL || config == NULL)
    return;

  config->pipe_type = data[0];
  config->transducer_type = data[1];
  config->pipe_size_type = data[2] | (data[3] << 8); // 小端：低位在前
  config->valve_type = data[4];
  config->module_type = data[5];
}

void handle_read_accumulated_flux(void) {
  static uint8_t first_call = 1;
  if (first_call == 1) {
    print_test_step_header("累计流量读取", "读取水表累计流量数据");
    first_call = 0;
  }
  if (test_xieyi_jilu_Rec == w_get_accumulated_flux) {
    test_xieyi_jilu_Rec = No_Receive;
    // 解析累计流量数据，判断是否有走水
    uint32_t accumulated_flux =
        ((uint32_t)(Test_linshi_cunchushuju_L
                        .L_ultrasonic_accumulated_flux[0]) |
         ((uint32_t)(Test_linshi_cunchushuju_L.L_ultrasonic_accumulated_flux[1])
          << 8) |
         ((uint32_t)(Test_linshi_cunchushuju_L.L_ultrasonic_accumulated_flux[2])
          << 16) |
         ((uint32_t)(Test_linshi_cunchushuju_L.L_ultrasonic_accumulated_flux[3])
          << 24)) /
        1000; // 数据单位为10^-3，除以1000转换为升
    if (accumulated_flux > 0) {
      // 累计流量大于0，说明有走水，计量正常
      log_i("累计流量数据正常,当前值: %lu", (unsigned long)accumulated_flux);
      Test_jiejuo_jilu.jiliang_jiance = 1;
    } else {
      log_i("累计流量数据异常,没有走水");
      Test_jiejuo_jilu.jiliang_jiance = 0;
    }
    Test_quanju_canshu_L.time_softdelay_ms = 0;
    Test_quanju_canshu_L.danbu_chaoshishijian_ms = 10000;
    Test_liucheng_L = w_shangg_jieguo_start;
    first_call = 1;
    print_test_step_footer("累计流量读取成功 ✓");
  } else {
    // 这个步骤失败了就不重试了，说明计量模块有问题
    read_accumulated_flux_protocol();
    Test_quanju_canshu_L.time_softdelay_ms = 2000;
  }
}
