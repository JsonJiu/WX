/**
 * @file device_protocol.h
 * @brief 设备/下位机协议接口定义
 * @version 2.0.0
 * @date 2025-01-20
 *
 * @section intro 简介
 * 定义设备协议的公共接口和命令码。
 * 所有设备协议实现都应该包含此头文件。
 *
 * @section supported 支持的设备
 * - 膜式燃气表 (diaphragm_gas_meter) - 深燃膜表协议
 * - 水表 (water_meter) - 三川水表协议 (兼容旧代码)
 */

#ifndef __DEVICE_PROTOCOL_H__
#define __DEVICE_PROTOCOL_H__

#include "../protocol_def.h"

/*============ 膜式燃气表协议定义 ============*/

/**
 * @brief 膜式燃气表控制码
 */
typedef enum {
  DGM_CTRL_READ = 0x01,    // 读操作
  DGM_CTRL_WRITE = 0x04,   // 写操作
  DGM_CTRL_INSTALL = 0x05, // 安装/配置操作
  DGM_CTRL_LOADLINK = 0x08 // 加载链接
} DiaphragmGasMeterCtrl;

/**
 * @brief 膜式燃气表数据标识
 */
typedef enum {
  // 基础命令
  DGM_DI_TIME = 0xC621, // 时间

  // 测试相关命令
  DGM_DI_AUTOCHECK_FINISH = 0x1000,  // 自检完成
  DGM_DI_TEST_CONNECT = 0x1001,      // 测试连接
  DGM_DI_SET_IO_STATUS = 0x1002,     // 设置端口输出状态
  DGM_DI_CLOSE_IR = 0x1005,          // 关闭红外
  DGM_DI_READ_CHECK_STATUS = 0x1008, // 读取检测状态/星闪MAC

  // IMEI/IMSI/ICCID读取
  DGM_DI_READ_IMEI_IMSI = 0xC518,       // 读取IMEI/IMSI
  DGM_DI_READ_IMEI_IMSI_ICCID = 0xC525, // 读取IMEI/IMSI/ICCID

  // 上位机测试命令
  DGM_DI_START_TEST = 0xFC03,      // 启动测试
  DGM_DI_GET_CHECK_RESULT = 0xFC04 // 查询测试结果
} DiaphragmGasMeterDI;

/*============ 水表协议命令码定义 (保留兼容) ============*/

/**
 * @brief 水表协议数据标识(DI)
 *
 * 水表通信使用数据标识来区分不同操作
 */
typedef enum {
  // 读取类命令 (控制码 0x81)
  WM_DI_READ_METER_NO = 0x2031, // 读取表号
  WM_DI_READ_VOLTAGE = 0x2034,  // 读取电压
  WM_DI_READ_VERSION = 0x2040,  // 读取版本号
  WM_DI_READ_FLOW = 0x2011,     // 读取累计流量
  WM_DI_READ_TEMP = 0x2050,     // 读取温度
  WM_DI_READ_IMEI = 0x2060,     // 读取IMEI
  WM_DI_READ_IMSI = 0x2061,     // 读取IMSI
  WM_DI_READ_ICCID = 0x2062,    // 读取ICCID
  WM_DI_READ_CSQ = 0x2063,      // 读取信号强度

  // 写入类命令 (控制码 0x82)
  WM_DI_WRITE_METER_NO = 0x2031, // 写入表号
  WM_DI_WRITE_FLOW = 0x2011,     // 写入累计流量

  // 配置类命令
  WM_DI_CONFIG_VALVE_US = 0x2036,   // 超声波表阀门配置
  WM_DI_CONFIG_VALVE_MECH = 0x2604, // 机械表阀门配置

  // 控制类命令
  WM_DI_VALVE_CTRL = 0xC022,   // 阀门控制
  WM_DI_REPORT_START = 0xC030, // 启动上报
  WM_DI_REPORT_QUERY = 0xC031, // 查询上报结果

  // 检测类命令
  WM_DI_FLASH_CHECK = 0xD001,    // Flash检测
  WM_DI_METERING_CHECK = 0xD002, // 计量检测
  WM_DI_INFRARED_CHECK = 0xD003, // 红外检测

} WaterMeterDI;

/**
 * @brief 水表协议控制码
 */
typedef enum {
  WM_CTRL_READ = 0x81,   // 读数据
  WM_CTRL_WRITE = 0x82,  // 写数据
  WM_CTRL_CTRL = 0x83,   // 控制命令
  WM_CTRL_REPORT = 0x84, // 上报
} WaterMeterCtrl;

/**
 * @brief 水表协议版本
 */
typedef enum {
  WM_VERSION_OLD = 0x00, // 旧版协议
  WM_VERSION_NEW = 0x0A, // 新版协议 (当前使用)
} WaterMeterVersion;

/*============ 水表配置参数结构体 ============*/

/**
 * @brief 超声波表阀门配置参数
 */
typedef struct {
  uint8_t pipe_type;       // 管道类型
  uint8_t transducer_type; // 换能器类型
  uint16_t pipe_size;      // 管径
  uint8_t valve_type;      // 阀门类型
  uint8_t module_type;     // 模块类型
} WM_UltrasonicConfig;

/**
 * @brief 机械表阀门配置参数
 */
typedef struct {
  uint16_t pipe_diameter_mm; // 管径(mm)
  uint8_t valve_type;        // 阀门类型
  uint8_t timeout_seconds;   // 超时时间
  uint16_t stall_current_ma; // 堵转电流
} WM_MechanicalConfig;

/**
 * @brief 阀门控制参数
 */
typedef struct {
  uint8_t action; // 动作: 0=关阀, 1=开阀
  uint8_t force;  // 强制: 0=正常, 1=强制
} WM_ValveCtrlParam;

/*============ 协议实例声明 ============*/

/**
 * @brief 膜式燃气表协议实例
 *
 * 在 device_protocol_diaphragm_gas_meter.c 中定义
 */
extern const ProtocolInterface diaphragm_gas_meter_protocol;

/**
 * @brief 三川水表协议实例 (兼容旧代码)
 *
 * 实际指向 diaphragm_gas_meter_protocol
 */
extern const ProtocolInterface water_meter_protocol;

/*============ 膜式燃气表公共API ============*/

/**
 * @brief 发送上告/开机信息获取命令 (0x1001)
 * @details 获取表具类型、附件状态、主控板电压、模块状态、信号强度、连接状态等
 */
void DGM_SendBoardInfoRequest(void);

/**
 * @brief 发送IO状态检测命令 (0x1002)
 * @param high_low 0=低电平检测, 1=高电平检测
 * @details 响应数据包含:
 * 开到位、关到位、霍尔1/2状态、IC卡XB脚、119电平、IC卡REE脚
 */
void DGM_SendIOStatusCheck(uint8_t data, uint8_t length);

/**
 * @brief 发送设置IO输出状态命令 (1002) - 单个控制
 * @param funtion_number 控制码数量 (通常为1)
 * @param function 控制码 (0x01~0x09)
 * @param io_status 控制状态 (0x00低/0x01高, 阀门: 0开/1关/2停止)
 */
void DGM_SendSetOutputIOStatus(uint8_t funtion_number, uint8_t function,
                               uint8_t io_status);

/**
 * @brief 发送设置IO输出状态命令 (1002) - 多个控制
 * @param count 控制数量
 * @param controls 控制数组，每个元素包含 [控制码, 控制状态]
 */
void DGM_SendSetOutputIOStatusMulti(uint8_t count, uint8_t controls[][2]);
/**
 * @brief  发送配置阀门命令，1002，开或者关，0是低，1是高，2是停止
 *
 * @param valve_status
 */
void DGM_SendConfigureValve(uint8_t valve_status);
/**
 * @brief 发送进入低功耗模式命令
 */
void DGM_SendEnterLowPower(void);

/**
 * @brief 发送读取IMEI/IMSI/ICCID命令
 */
void DGM_SendReadIMEI(void);

/**
 * @brief 发送关闭红外命令
 *
 */
void DGM_SendCloseIR(void);

/**
 * @brief 发送读取星闪MAC命令
 */
void DGM_SendReadStarMac(void);

/**
 * @brief 获取当前检测过程状态
 * @return 检测过程状态码
 */
uint8_t DGM_GetCheckProcess(void);

/**
 * @brief 设置当前检测过程状态
 * @param process 检测过程状态码
 */
void DGM_SetCheckProcess(uint8_t process);

/**
 * @brief 设置表号
 * @param meter_no 表号 (6字节)
 */
void DGM_SetMeterNumber(const uint8_t *meter_no);

/**
 * @brief 设置RTC时间
 * @param rtc_time RTC时间 (6字节, BCD格式)
 */
void DGM_SetRtcTime(const uint8_t *rtc_time);

#endif /* __DEVICE_PROTOCOL_H__ */
