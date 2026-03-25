#ifndef __DOMESTIC_WATER_METER_TEST_H__
#define __DOMESTIC_WATER_METER_TEST_H__
#include "main.h"

/*============ 测试阈值宏定义 ============*/
// 功耗检测阈值 (单位: μA)
#define GONGHAO_KONGSHUI_MIN 10 // 空水功耗最小值
#define GONGHAO_KONGSHUI_MAX 40 // 空水功耗最大值
#define GONGHAO_MANSHUI_MIN 10  // 满水功耗最小值
#define GONGHAO_MANSHUI_MAX 40  // 满水功耗最大值
#define GONGHAO_ZOUSHUI_MIN 10  // 走水功耗最小值
#define GONGHAO_ZOUSHUI_MAX 40  // 走水功耗最大值

// 主电电压检测阈值 (单位: mV)
#define ZHUDIAN_DIANYA_MIN 3200 // 主电电压最小值
#define ZHUDIAN_DIANYA_MAX 4000 // 主电电压最大值

// 温度检测阈值 (单位: 0.1°C, 即600=60.0°C)
#define WENDU_MIN 1   // 温度最小值 (0.1°C)
#define WENDU_MAX 600 // 温度最大值 (60.0°C)

/*============ 阀门配置结构体定义 ============*/

// 机械表阀门配置参数
typedef struct {
  uint16_t pipe_diameter_mm; // 阀门管径（mm），常见值：15、20、25、32
  uint8_t valve_type;        // 阀门类型：0=无阀门, 1=5线阀, 2=2线阀
  uint8_t timeout_seconds;   // 动作超时时间（秒），建议：12~20
  uint16_t stall_current_ma; // 堵转电流阈值（mA），典型值：230
} MechanicalValveConfig;

// 超声波表阀门配置参数
typedef struct {
  uint8_t pipe_type;       // 管道类型
  uint8_t transducer_type; // 换能器类型
  uint16_t pipe_size_type; // 管径类型
  uint8_t valve_type;      // 阀门类型
  uint8_t module_type;     // 模块类型
} UltrasonicValveConfig;

// 测试完成状态枚举
enum Test_Status {
  TEST_RUNNING = 0,        // 测试进行中
  TEST_SUCCESS = 1,        // 测试正常完成
  TEST_TIMEOUT_GLOBAL = 2, // 全局超时中断
  TEST_TIMEOUT_STEP = 3,   // 单步超时中断
  TEST_ERROR_HARDWARE = 4, // 硬件错误中断
  TEST_ERROR_PROTOCOL = 5, // 协议错误中断
  TEST_MANUAL_STOP = 6     // 手动停止测试
};

// 测试流程枚举 - 移到前面以便在结构体中使用
enum Test_liucheng {
  w_wait = 0,
  // 3.3V VCC测试
  w_start,
  // 设置累计流量，方便后续直接读取，无需读取初始流量
  w_reset_accumulated_flux,
  // 5线阀关阀等待响应（上电后阀门会开阀，需要先关阀）
  w_close_valve_wait_response,
  // 切换为空水状态
  w_qiehuan_kongshui,
  // 功耗测试
  w_gonghao_CHK,
  // 切换为满水状态
  w_qiehuan_manshui,
  // 满水功耗测试
  w_gonghao_manshui_CHK,
  // 切换为走水状态
  w_qiehuan_zoushui,
  // 走水功耗测试
  w_gonghao_zoushui_CHK,
  // 读取累计流量
  w_read_accumulated_flux,
  // 电磁铁唤醒红外
  w_diancitie_huanxing_irda,
  // 获取表号
  get_biaohao,
  // 获取版本号
  get_banbenhao,
  // 设置阀门配置
  set_famen_peizhi,

  // 主动上报
  w_zhudong_shangg,
  // 第一次查询上报结果
  w_shangg_jieguo_start,
  // 查询上报结果等待上报完成期间检测阀门声音
  w_shangg_jieguo,
  // 阀门测试
  w_famenceshi,
  // 阀门测试子流程 - 开始测试
  w_valve_test_process_1,
  // 阀门测试子流程 - 电压检测
  w_valve_output_voltage_check,
  w_water_temp_CHK, // 水温检测
  w_end
};

struct Test_quanju_canshu {
  volatile uint16_t time_softdelay_ms; // 软件延时计数器(ms) - 中断修改
  volatile uint32_t time_aroundtest_ms; // 全局测试超时计数器(ms) - 中断修改
  volatile uint16_t
      danbu_chaoshishijian_ms; // 单步测试超时计数器(ms) - 中断修改
  enum Test_Status test_status;   // 测试状态，替代原来的test_over
  enum Test_liucheng failed_step; // 记录失败时的测试步骤
  char status_description[64];    // 测试状态详细说明
};
extern struct Test_quanju_canshu Test_quanju_canshu_L;
//---------------------------------------------------------------

//---------------------------------------------------------------
struct Test_jieguo {
  uint8_t gongwei;
  uint32_t zhidian_dianya_gongdian;
  uint32_t zhidian_dianya_huoqu;
  uint16_t zhidian_jingtai_gonghao;
  uint16_t zhidian_manshui_gonghao;
  uint16_t zhidian_zoushui_gonghao;
  uint32_t beidian_dianya_gongdian;
  uint32_t beidian_dianya_huoqu;
  uint16_t beidian_gonghao;
  uint8_t lanya_jiance;
  uint8_t flash_jiance;
  uint8_t jiliang_jiance;
  uint8_t hongwai_jiance;
  uint8_t IMEI_CHK[15];
  uint8_t IMSI_CHK[15];
  uint8_t ICCID_CHK[20];
  uint8_t CSQ;
  uint8_t FM1;
  uint8_t FM2;
  uint8_t FM;
  uint8_t FM_daowei_1;
  uint8_t FM_daowei_2;
  uint8_t FM_daowei;
  uint8_t EEPROM_jiance;
  uint32_t GP30_dianya;
  uint8_t loraEUI[16];
  uint8_t qiangci_jiance;
  uint8_t qiangci_jiance_1;
  uint8_t qiangci_jiance_2;
  uint8_t kaigai_jiance;
  uint8_t GPSmozu_jiacne;
  uint8_t jiliang_1_1;
  uint8_t jiliang_1_2;
  uint8_t jiliang_2_1;
  uint8_t jiliang_2_2;
  uint8_t dangqian_biaohao[6];
  uint8_t mokuaihao[6];
  uint8_t biaoju_leixing_famen;
  // 阀门配置参数（从上位机PC接收）
  UltrasonicValveConfig ultrasonic_config; // 超声波表参数 (6字节)
  MechanicalValveConfig mechanical_config; // 机械表参数 (6字节)
  uint8_t jiaoyanma[2];
  uint8_t banbenhao[2];
  uint8_t water_temp;     // 新增，水温检测 结果
  uint8_t pressure_value; // 新增，压力状态值 ,没有默认返回1,正常
};
extern struct Test_jieguo Test_jiejuo_jilu;

struct Test_linshi_cunchushuju {
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
  uint8_t L_banbenhao[2];
  uint16_t L_water_temperature[2]; // 温度值，单位摄氏度,默认是20 ，0xC8 0x00
                                   // ,小端模式传输
  uint16_t L_Lora_rssi[2];                  // FF B9 代表-71 非 lora 默认 0
  uint16_t L_lora_isnr[2];                  // 00 05 代表 5 非 lora 默认 0
  uint8_t L_ultrasonic_accumulated_flux[4]; // 新增，超声波累计流量 (4字节)
  uint8_t L_StarMac[12];                    // 星闪MAC地址 (12字节)
};
extern struct Test_linshi_cunchushuju Test_linshi_cunchushuju_L;

extern enum Test_liucheng Test_liucheng_L;

enum test_xieyi_jilu {
  // 设置累计流量成功标识
  w_set_accumulated_flux,
  // 读取累计流量成功标识
  w_get_accumulated_flux,
  w_get_biaohao,
  // �ô��ڻ�ȡ����
  w_get_biaohao_chuankou,
  // �ϸ�
  w_get_shanggao,
  // �����ˮ״̬
  w_get_kongshui,
  // ������ˮ״̬
  w_get_manshui,
  // ������ˮ״̬
  w_get_zoushui,
  // ���÷���״̬
  w_set_famen,

  w_set_water_temp,
  // ��ȡ�汾��,
  w_get_banbenhao,
  // ���Ŷ���
  w_get_famen_dongzuo,
  // ���Ŷ���
  w_get_famen_dongzuo_L,
  // 测试专用
  w_get_test_zhuanyong,
  // 阀门测试结束标志
  w_valve_test_end,
  // 关阀命令响应
  w_close_valve_ack,
  // 信号强度
  // w_get_csq,

  // ===== 膜式燃气表专用枚举值 =====
  w_get_connect,    // 测试连接响应
  w_get_IO_status,  // IO状态检测响应
  w_get_close_IR,   // 关闭红外响应
  w_get_IMEI,       // IMEI/IMSI/ICCID读取响应
  w_get_self_check, // 自检完成响应

  No_Receive
};
extern enum test_xieyi_jilu test_xieyi_jilu_Rec;

enum test_xieyi_jilu_irda {
  // �ô��ڻ�ȡ����
  h_get_biaohao_chuankou,
  h_No_Receive
};
extern enum test_xieyi_jilu_irda test_xieyi_jilu_irda_rec;

void test_Loop_Func(void);

// 工位检测
void gongwei_jiance(void);
// 开始测试前恢复标志位
void test_start_Init(void);
// 开始测试
void test_start(void);
// 循环检测函数
void Test_loop_func(void);
void set_water_temp(void);

// 新增的便捷函数声明
enum Test_Status get_test_status(void); // 获取当前测试状态
uint8_t is_test_finished(void);         // 判断测试是否完成
uint8_t is_test_success(void);          // 判断测试是否成功完成
void set_test_status_with_description(
    enum Test_Status status, enum Test_liucheng step,
    const char *description);                  // 设置测试状态和说明
const char *get_test_status_description(void); // 获取测试状态说明
const char *get_test_step_name(enum Test_liucheng step); // 获取测试步骤名称

// 测试步骤日志和分割符函数
void print_test_step_header(const char *step_name, const char *description);
void print_test_step_separator(void);
void print_test_step_footer(const char *result);

// 重构后的测试步骤函数声明
void handle_test_wait(void);
void handle_test_start(void);
void handle_close_valve_wait_response(void);
void handle_reset_accumulated_flux(void);
void handle_test_qiehuan_kongshui(void);
void handle_test_gonghao_chk(void);
void handle_test_qiehuan_manshui(void);
void handle_test_gonghao_manshui_chk(void);
void handle_test_qiehuan_zoushui(void);
void handle_test_gonghao_zoushui_chk(void);
void handle_test_diancitie_huanxing_irda(void);
void handle_test_get_biaohao(void);
void handle_test_get_banbenhao(void);
void handle_test_set_famen_peizhi(void);
void handle_test_water_temp_chk(void);
void handle_test_zhudong_shangg(void);
void handle_test_shangg_jieguo_start(void);
void handle_test_shangg_jieguo(void);
void handle_test_famenceshi(void);
void handle_test_end(void);
void send_none_valve_configuration(void);
uint16_t static_power_check(uint16_t total_time, uint16_t total_count,
                            uint16_t early_fail_threshold,
                            uint8_t early_fail_count);
// 机械表阀门配置（使用结构体）
void configure_mechanical_valve(const MechanicalValveConfig *config);

// 超声波表阀门配置（使用结构体）
void configure_ultrasonic_valve(const UltrasonicValveConfig *config);

// 配置超声波无阀门状态（用于测试低功耗）
void configure_ultrasonic_valve_no_valve(void);

// 便捷函数：使用默认参数快速配置
void configure_mechanical_valve_default(uint16_t pipe_diameter_mm,
                                        uint8_t valve_type);
void configure_ultrasonic_valve_default(uint8_t valve_type);

// 从字节数组解析阀门配置结构体（用于解析上位机协议）
void parse_mechanical_config_from_bytes(const uint8_t *data,
                                        MechanicalValveConfig *config);
void parse_ultrasonic_config_from_bytes(const uint8_t *data,
                                        UltrasonicValveConfig *config);

#endif
