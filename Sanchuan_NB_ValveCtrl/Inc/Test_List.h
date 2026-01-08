#ifndef __TEST_LIST_H__
#define __TEST_LIST_H__
#include "main.h"

// 机械表阀门配置结构体
typedef struct {
  uint16_t pipe_diameter_mm; // 管径 (mm)
  uint8_t valve_type;        // 阀门类型: 0=无阀门, 1=2线阀, 2=5线阀
  uint8_t timeout_seconds;   // 超时时间 (秒)
  uint16_t stall_current_ma; // 堵转电流 (mA)
} MechanicalValveConfig;

// 超声波表阀门配置结构体
typedef struct {
  uint16_t pipe_diameter_mm; // 管径 (mm)
  uint8_t valve_type;        // 阀门类型
  uint8_t timeout_seconds;   // 超时时间 (秒)
  uint16_t stall_current_ma; // 堵转电流 (mA)
} UltrasonicValveConfig;

struct Test_quanju_canshu {
  uint16_t time_softdelay_ms;
  uint32_t time_aroundtest_ms;
  uint16_t danbu_chaoshishijian_ms;
  uint8_t test_over;
};
extern struct Test_quanju_canshu Test_quanju_canshu_L;

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
  uint8_t qiangci_jiance_3;
  uint8_t kaigai_jiance;
  uint8_t GPSmozu_jiacne;
  // ��Ҫ�洢���μ�����ֵ���ڱȽ�
  uint8_t jiliang_1_1;
  uint8_t jiliang_1_2;
  uint8_t jiliang_1_3;
  uint8_t jiliang_2_1;
  uint8_t jiliang_2_2;
  uint8_t jiliang_2_3;
  // ����ָʾ���β����Ƿ����
  // ���ǵ�Э��ͨ����û��ͨ�ñ��ţ�������Ҫ��һ��
  uint8_t dangqian_biaohao[6];
  // ��ʼ�����Ի�ȡ������
  uint8_t mokuaihao[6];
  uint8_t biaoju_leixing_famen;
  uint8_t chaoshengbo_canshu[6];
  uint8_t jixiebiao_canshu[6];
  // 阀门配置结构体
  MechanicalValveConfig mechanical_config;
  UltrasonicValveConfig ultrasonic_config;
  // У����
  uint8_t jiaoyanma[2];
  // ����汾��
  uint8_t banbenhao[2];
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
  // ����汾��
  uint8_t L_banbenhao[2];
};
extern struct Test_linshi_cunchushuju Test_linshi_cunchushuju_L;

enum Test_liucheng {
  w_wait = 0,
  // 3.3V VCC检测
  w_start,
  // 发送关阀门指令（预关阀，确保阀门从关闭状态开始）
  w_close_valve_command_send,
  // 静态功耗测试
  w_gonghao_CHK,
  // ��������Ѻ���
  w_diancitie_huanxing_irda,
  // ��ȡ����
  get_biaohao,
  // 阀门和到位初始状态检测
  w_valve_initial_state_check_voltage,
  w_valve_initial_state_check_position,
  // 发送阀门配置指令，5线阀门，且验证当前F003的到位是0，0进入下一步，否则继续发送，直到超时或者次数超过限制
  w_valve_configuration,
  // 阀门测试（委托给外部状态机）
  w_famenceshi,
  // �����ϸ�
  w_zhudong_shangg,
  // ��һ�β�ѯ�ϸ�����
  w_shangg_jieguo_start,
  // ��ѯ�ϸ������ȴ��ϸ�������ڼ�����������
  w_shangg_jieguo,
  // 阀门测试过程1，刚刚发送测试指令，检测阀门电压，不做动作，但是要在1s内给到位信号，如果水表F003有返回且到位1检测到1，进入下一步，没有就继续发送，直到超时或者次数超过限制
  w_valve_test_process_1,
  // 阀门测试过程2，1s内检测阀门电压，检测到阀门电压和之前完全相反，就在1s内给到位信号2，如果水表F003有返回且到位2检测到1，进入下一步，没有就继续发送，直到超时或者次数超过限制
  w_valve_test_process_2,
  // 阀门测试过程3，1s内检测阀门电压，检测到阀门电压全部为0，阀门测试完成，进入下一步
  w_valve_test_process_3,
  // 获取水表固件版本号
  w_get_firmware_version,
  // 强磁检测和计量检测第一次采样
  w_jiliang_qiangci_CHK_1,
  // 强磁检测和计量检测第二次采样
  w_jiliang_qiangci_CHK_2,
  // 测试完成
  w_end
};
extern enum Test_liucheng Test_liucheng_L;

enum test_xieyi_jilu {
  w_get_biaohao,
  // 阀门到位信号初始状态正常
  w_get_valve_ini_position_OK,
  w_set_famen,
  w_get_shanggao,
  w_get_famen_dongzuo,
  w_get_famen_dongzuo_L,
  w_get_test_zhuanyong,
  w_get_banbenhao,
  No_Receive
};
extern enum test_xieyi_jilu test_xieyi_jilu_Rec;

void test_Loop_Func(void);

// 发送关阀命令
void send_close_valve_command(void);

// 工位检测
void gongwei_jiance(void);
// ��ʼ����ǰ�ָ���־λ
void test_start_Init(void);
// ��ʼ����
void test_start(void);
// 循环检测（原始版本 - 单次采样）
void Test_loop_func(void);
// 循环检测（改进版本 - 滑动窗口平均采样）
void Test_loop_func_avg(void);
uint16_t static_power_check(uint16_t total_time, uint16_t total_count,
                            uint16_t early_fail_threshold,
                            uint8_t early_fail_count);

uint32_t get_system_tick_ms(void);
// 电压采样处理函数（扩展版，支持多通道）
uint32_t process_voltage_reading_ex(uint8_t channel_id, uint16_t total_time_ms,
                                    uint16_t sample_count,
                                    uint32_t (*voltage_read_func)(void),
                                    uint32_t (*get_tick_func)(void),
                                    void (*delay_func)(uint32_t),
                                    uint8_t filter_mode);
// 电压采样处理函数
uint32_t process_voltage_reading(uint16_t total_time_ms, uint16_t sample_count,
                                 uint32_t (*voltage_read_func)(void),
                                 uint32_t (*get_tick_func)(void),
                                 void (*delay_func)(uint32_t),
                                 uint8_t filter_mode);
#endif
