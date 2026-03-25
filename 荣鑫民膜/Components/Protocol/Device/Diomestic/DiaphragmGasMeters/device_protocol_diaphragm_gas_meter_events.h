/**
 * @file device_protocol_diaphragm_gas_meter_events.h
 * @brief 膜式燃气表协议事件定义
 * @version 1.0.0
 * @date 2025-01-24
 *
 * @details 定义协议层与测试层之间的事件接口
 *          协议层通过事件回调通知上层，不直接操作外部变量
 */

#ifndef __DEVICE_PROTOCOL_DIAPHRAGM_GAS_METER_EVENTS_H__
#define __DEVICE_PROTOCOL_DIAPHRAGM_GAS_METER_EVENTS_H__

#include <stdbool.h>
#include <stdint.h>

/*============================================================================
 *                          事件类型定义
 *===========================================================================*/

/**
 * @brief 膜式燃气表协议事件类型
 */
typedef enum {
  DGM_EVENT_NONE = 0,

  /* 自检/连接相关事件 */
  DGM_EVENT_SELF_CHECK_COMPLETE,    // 自检完成 (0x1000)
  DGM_EVENT_POWER_ON_INFO_RECEIVED, // 上告开机信息获取 (0x1001)
  DGM_EVENT_CONNECT_FAILED,         // 连接失败

  /* IO状态检测事件 */
  DGM_EVENT_IO_STATUS, // IO状态检测完成 (0x1002)

  /* 信息读取事件 */
  DGM_EVENT_IMEI_RECEIVED,     // 收到IMEI/IMSI/ICCID
  DGM_EVENT_STAR_MAC_RECEIVED, // 收到星闪MAC

  /* 控制相关事件 */
  DGM_EVENT_IR_CLOSED,   // 红外已关闭
  DGM_EVENT_TIME_SET_OK, // 时间设置成功
  DGM_EVENT_IO_CONFIGURED, // 端口状态已配置 (0x1007)，可以进行低功耗测试等等

  /* 错误事件 */
  DGM_EVENT_PARSE_ERROR,    // 解析错误
  DGM_EVENT_CHECKSUM_ERROR, // 校验和错误
  DGM_EVENT_TIMEOUT,        // 超时

  DGM_EVENT_MAX
} DgmEventType;

/*============================================================================
 *                          事件数据结构定义
 *===========================================================================*/

/**
 * @brief 自检完成数据 (0x1000响应, 1字节)
 */
typedef struct {
  uint8_t signal_strength; // [0] 信号强度 CSQ值 (0-33, 31=最强)
} DgmSelfCheckData;

/**
 * @brief 网络参数数据 (0xC525响应, 107字节)
 *
 * @note 来源: 《民用物联网表整机测试通讯协议》章节4.6.10
 * @note payload数据域内容:
 *   [0-14]    IMEI号 (15字节ASCII)
 *   [15-29]   IMSI号 (15字节ASCII)
 *   [30-49]   ICCID号 (20字节ASCII)
 *   [50]      CSQ值 (0-31)
 *   [51-52]   RSRP值 (小端, 有符号)
 *   [53-54]   SNR值 (小端, 有符号)
 *   [55]      ECL (0-6)
 *   [56-59]   CellID (小端)
 *   [60-79]   ICCID_2 (20字节ASCII)
 *   [80-94]   IMSI_2 (15字节ASCII)
 *   [95]      CSQ_2 (0-31)
 *   [96-101]  软件版本编译时间 (6字节BCD: YYMMDDhhmmss)
 *   [102]     外部压力传感器状态 (0=正常, 1=异常)
 *   [103-106] 外部压力传感器值 (小端, 2位小数)
 */
typedef struct {
  /* 主卡基本信息 */
  uint8_t imei[16];  // [0-14]   IMEI (15字节 + '\0')
  uint8_t imsi[16];  // [15-29]  IMSI (15字节 + '\0')
  uint8_t iccid[21]; // [30-49]  ICCID (20字节 + '\0')

  /* 信号参数 */
  uint8_t csq;      // [50]     CSQ值 (0-31)
  int16_t rsrp;     // [51-52]  RSRP值 (小端, 有符号, 单位dBm)
  int16_t snr;      // [53-54]  SNR值 (小端, 有符号, 单位dB)
  uint8_t ecl;      // [55]     覆盖等级 (0-6)
  uint32_t cell_id; // [56-59]  小区号 (小端)

  /* 备卡信息 */
  uint8_t iccid2[21]; // [60-79]  ICCID_2 (20字节 + '\0')
  uint8_t imsi2[16];  // [80-94]  IMSI_2 (15字节 + '\0')
  uint8_t csq2;       // [95]     CSQ_2 (0-31)

  /* 版本信息 */
  uint8_t build_time[6]; // [96-101] 软件版本编译时间 (BCD: YYMMDDhhmmss)

  /* 压力传感器 */
  uint8_t pressure_status; // [102]     外部压力传感器状态 (0=正常, 1=异常)
  uint32_t
      pressure_value; // [103-106] 外部压力传感器值 (小端, 2位小数, 单位0.01kPa)
} DgmImeiData;

/**
 * @brief 读取检测状态数据 (0x1008响应, 17字节)
 *
 * @note 来源: 《民用物联网表整机测试通讯协议》章节4.6.18
 * @note payload数据域内容:
 *   [0-1]   主电电压 (大端, 单位0.01V)
 *   [2-13]  星闪MAC地址 (12字节ASCII)
 *   [14]    星闪连接状态 (0=未连接, 1=已连接)
 *   [15]    星闪信号强度 (有符号8bit)
 *   [16]    按键状态 (0=未触发, 1=触发)
 */
typedef struct {
  uint16_t voltage; // [0-1]  主电电压 (单位0.01V, 例: 360=3.6V)
  uint8_t mac[13];  // [2-13] 星闪MAC (12字节ASCII + '\0')
  uint8_t connected; // [14]   连接状态 (0=无设备连接, 1=有设备连接)
  int8_t signal; // [15]   信号强度 (有符号8bit, 有设备连接时有效)
  uint8_t key_status; // [16]   按键状态 (0=未触发, 1=触发)
} DgmStarMacData;

/**
 * @brief IO状态数据 (0x1002响应, 7字节)
 *
 * @note 来源: 《民用物联网表整机测试通讯协议》章节4.6.13
 * @note payload数据域内容:
 *   [0] 开到位  [1] 关到位  [2] 霍尔1状态
 *   [3] 霍尔2状态  [4] IC卡XB脚电平  [5] 119电平
 *   [6] IC卡ERR脚电平
 */
typedef struct {
  uint8_t high_low;    // 检测的是高电平(1)还是低电平(0)
  uint8_t open_pos;    // [0] 开到位状态 (valve open position)
  uint8_t close_pos;   // [1] 关到位状态 (valve close position)
  uint8_t hall1;       // [2] 霍尔1状态 (0=低, 1=高)
  uint8_t hall2;       // [3] 霍尔2状态 (0=低, 1=高)
  uint8_t ic_xb;       // [4] IC卡XB脚电平 (0=低, 1=高)
  uint8_t io_119;      // [5] 119电平 (0=低, 1=高)
  uint8_t ic_err;      // [6] IC卡ERR脚电平 (0=低, 1=高)

  /* 检测结果 */
  bool hall_ok;   // 霍尔检测是否通过
  bool ic_ok;     // IC卡检测是否通过
  bool io_119_ok; // 119检测是否通过
} DgmIoStatusData;

/**
 * @brief 上告开机信息数据 (0x1001响应, 26字节)
 *
 * @note payload数据域内容:
 *   [0]  表具类型     [1]  是否带附件   [2]  主控板电压
 *   [3]  模块状态     [4]  信号强度     [5]  连接状态
 *   [6]  SIM卡状态    [7]  EEPROM状态   [8]  计量状态
 *   [9]  软件版本1    [10] 软件版本2    [11] RTC状态
 *   [12] 温压状态     [13-22] 预留10字节
 *   [23] 开盖状态     [24] 倾斜状态     [25] 蓝牙状态
 */
typedef struct {
  /* 基本信息 */
  uint8_t meter_type;    // [0]  表具类型
  uint8_t has_addon;     // [1]  是否带附件
  uint8_t voltage;       // [2]  主控板电压 (需除以10得到实际电压)
  uint8_t module_status; // [3]  模块状态
  uint8_t signal;        // [4]  信号强度
  uint8_t connect_status; // [5]  连接状态
  uint8_t sim_ok;         // [6]  SIM卡状态
  uint8_t storage_ic_ok;  // [7]  EEPROM状态
  uint8_t measure_ok;     // [8]  计量状态
  uint8_t sw_ver1;        // [9]  软件版本1
  uint8_t sw_ver2;        // [10] 软件版本2
  uint8_t rtc_ok;         // [11] RTC状态
  uint8_t temp_press_ok;  // [12] 温压状态
  /* [13-22] 预留10字节 */
  uint8_t cover_open;   // [23] 开盖状态 (0=合盖, 1=开盖)
  uint8_t tilt_ok;      // [24] 倾斜状态
  uint8_t bluetooth_ok; // [25] 蓝牙状态

  /* 通信状态 */
  bool ir_comm_ok; // 红外通信是否正常
} DgmBoardInfoData;

/**
 * @brief 开盖检测数据
 */
typedef struct {
  bool cover_open;   // 开盖状态 (true=开, false=合)
  bool check_passed; // 检测是否通过
} DgmCoverCheckData;

/**
 * @brief 事件数据联合体
 */
typedef union {
  DgmSelfCheckData self_check;
  DgmImeiData imei;
  DgmStarMacData star_mac;
  DgmIoStatusData io_status;
  DgmBoardInfoData board_info;
  DgmCoverCheckData cover_check;
  uint8_t raw_data[160]; // 原始数据 (增大到160以容纳0xC525的107字节数据)
} DgmEventData;

/**
 * @brief 协议事件结构体
 */
typedef struct {
  DgmEventType type;  // 事件类型
  uint16_t data_mark; // 原始数据标识 (用于调试)
  DgmEventData data;  // 事件数据
} DgmProtocolEvent;

/*============================================================================
 *                          回调函数类型定义
 *===========================================================================*/

/**
 * @brief 膜式燃气表协议事件回调函数类型
 * @param event 协议事件
 */
typedef void (*DgmEventCallback)(const DgmProtocolEvent *event);

/*============================================================================
 *                          API函数声明
 *===========================================================================*/

/**
 * @brief 设置膜式燃气表协议事件回调
 * @param callback 回调函数
 */
void DGM_SetEventCallback(DgmEventCallback callback);

/**
 * @brief 获取事件类型名称 (用于调试日志)
 * @param type 事件类型
 * @return 事件名称字符串
 */
const char *DGM_GetEventName(DgmEventType type);

#endif /* __DEVICE_PROTOCOL_DIAPHRAGM_GAS_METER_EVENTS_H__ */
