#ifndef __TONGXIN_XIEYI_CTRL_H__
#define __TONGXIN_XIEYI_CTRL_H__
#include "main.h"
void TONGXIN_xieyijiexi(uint8_t zufuchua[], uint16_t lenth);
void TONGXIN_xieyifasong(uint16_t shujubiaoshi, uint8_t kongzhima,
                         uint8_t w_biaohao[], uint8_t shujuyu[],
                         uint16_t shujuyu_changdu);
void set_famen_xieyi(void);
void find_biaohao_xieyi(void);
void send_shanggao_xieyi(void);
void FM_Open_xieyi(void);
void FM_Close_xieyi(void);
void Test_zhuanyong_xieyi(void);
void valve_position_status_read_protocol(void);
void find_banbenhao_xieyi(void);
// 设置为无阀门协议，清理掉测试的水表阀门数据，以便于后续正常使用无阀门水表协议
void reset_water_meter_to_none_valve();
void read_current_water_usage_protocol();

/**
 * @brief  配置阀门相关参数（仅适用于机械表阀门，超声波表未验证）
 * @param pipe_diameter_mm   阀门管径（单位：mm），常见值：15、20、25、32
 * @param valve_type         阀门类型：0=无阀门, 2=2线阀, 5=5线阀
 * @param timeout_seconds    阀门动作超时时间（单位：秒）
 * @param stall_current_ma   堵转电流阈值（单位：mA）
 * @example configure_valve_parameters(15, 0, 20, 230);  // 15mm, 无阀门,
 * 20秒, 230mA
 */
void configure_valve_parameters(uint16_t pipe_diameter_mm, uint8_t valve_type,
                                uint8_t timeout_seconds,
                                uint16_t stall_current_ma);

// 使用结构体配置阀门参数
struct MechanicalValveConfig;
struct UltrasonicValveConfig;
void configure_mechanical_valve(const void *config);
void configure_ultrasonic_valve(const void *config);
#endif
