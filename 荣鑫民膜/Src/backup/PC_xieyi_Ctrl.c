#include "PC_xieyi_Ctrl.h"
#include "GPIO.h"
#include "Test_List.h"
#include "uart0.h"
#include "uart1.h"

// EasyLogger 日志标签
#define LOG_TAG "pc_proto"
#include <elog.h>

// 外部声明 - 透传模式变量 (定义在main.c中)
extern uint8_t PassThrough_Mode;

#define send_lenth 200
uint8_t xieyi1_fanhui[6] = {0x68, 0xAB, 0x06, 0x00, 0x19, 0x16};
uint8_t xieyi2_fanhui[send_lenth];
extern uint8_t test_famen_type;
extern uint8_t water_meter_type;
//添加断言确保协议字节正确
_Static_assert(sizeof(upgrade_cmd_t) == 13, "upgrade_cmd_t size error!");
_Static_assert(sizeof(start_response_t) == 6, "start_response_t size error!");
#pragma pack(1) //设置对对齐方式为1字节
// bootloader控制相关协议：
//  升级握手指令
typedef struct {
  uint8_t head;        // 0x68
  uint8_t cmd;         // 0xBA
  uint8_t length;      // 数据域长度
  uint8_t station_id;  // 工位号
  uint8_t mode;        // 00=手动, 01=自动(固定配置)
  uint8_t baud_cfg;    // 00=9600, 01=115200 (手动模式有效)
  uint8_t protocol;    // 00=Xmodem (手动模式有效)
  uint8_t timeout_sec; // 等待超时，同时也是倒计时显示值
  uint8_t log_enable;  // 00=关, 01=开 (手动模式有效)
  uint16_t fw_size;    // 固件大小，建议2字节，单位256B或直接KB
  uint8_t checksum;
  uint8_t tail; // 0x16
} upgrade_cmd_t;
upgrade_cmd_t upgrade_cmd_recv;

//开始测试和响应协议结构体，只有cmd和校验和不同
typedef struct {
  uint8_t head;
  uint8_t cmd;
  uint8_t length;
  uint8_t station_id;
  uint8_t checksum;
  uint8_t tail;
} start_response_t;
start_response_t start_response_recv = {.head = 0x68,
                                        .cmd = 0xAB,
                                        .length = 0x06,
                                        .station_id = 0x00, // 运行时设置
                                        .checksum = 0x19,
                                        .tail = 0x16};

//这个用于解析上位机发送的开始测试指令,国内水表专用，其他类型的不要用这个指令
typedef struct {
  uint8_t head;            // 0x68
  uint8_t cmd;             // 0xAA
  uint8_t length;          // 数据域长度
  uint8_t station_id;      // 工位号
  uint8_t meter_number[6]; //表具编号
  uint8_t meter_type;      //表具类型 0机械表 1超声波表
  //超声波6个字节参数，其中模块这个配置字节在超声波配置阀门的字节里面，如果是机械表，其实没关系，是超声波表就用配置阀门，那么有概率会影响超声波表的模块初始配置，错了就会上告异常
  uint8_t ultrasonic_meter_type; //超声波表计类型,默认1
  uint8_t ultrasonic_meter_;     //超声波表换能器类型,默认1
  uint16_t ultrasonic_valve_pipe_type; //超声波表管道类型,默认15 ,小端模式
  uint8_t ultrasonic_valve_type; //阀门类型，0x00无阀门，0x01五线阀,0x02两线阀
  uint8_t
      meter_module_type; //模块类型,0代表NB,1代表CAT1
                         //机械表参数配置,接下来6个字节都是机械表参数的数据域的配置,配置机械表的阀门也会用到这6个参数
  uint16_t mechanical_pipe; //小端模式，机械表管径，单位mm
  uint8_t mechanical_valve_type; //机械表阀门类型，0无阀，1五线阀，2两线阀
  uint8_t mechanical_timeout_seconds; //机械表阀门超时秒数
  uint16_t mechanical_stall_current_ma; //机械表堵转电流，单位mA，小端模式
  uint8_t checksum;                     //校验和
  uint8_t tail;                         // 0x16
} start_t start_cmd_send;
start_cmd_send.head = 0x68;
start_cmd_send.cmd = 0xAA;
start_cmd_send.length =
    0x19; //这个指令长度可能会变，协议现在是这样，以后可能会变更，变更了协议，请同步修改
start_cmd_send.tail = 0x16;

//默认自动升级配置
upgrade_cmd_t default_upgrade_cmd = {
    .head = 0x68,
    .cmd = 0xBA,
    .length = 0x09,
    .station_id = Test_jiejuo_jilu.gongwei, //当前实际的工位
    .mode = 0x01,                           // 自动模式
    .baud_cfg = 0x01,                       // 115200
    .protocol = 0x00,                       // Xmodem
    .timeout_sec = 1,                       // 1秒超时
    .log_enable = 0x00,                     // 关闭日志
    .fw_size =
        0x0000, // 固件大小,单位512B,主要方便bootloader进行计算，512B和Flash
                // 对齐，2个字节，小端模式，低字节在前，高字节在后，显示的数据后两位是小数部分
    .checksum = 0x00, // 后续计算
    .tail = 0x16};
//人工升级模式我们不设置默认值，因为需要上位机进行配置
// 工装收到上位机发送的开始测试指令的应答指令
#pragma pack()
void PC_xieyifasong_1() {
  // 组装开始测试应答协议
  start_response_recv.station_id = Test_jiejuo_jilu.gongwei;
  // 计算校验和
  uint8_t sum = 0;
  uint8_t protocol_length = 0;
  uint8_t checksum_range = 0;
  protocol_length = sizeof(start_response_recv);
  checksum_range = protocol_length - 4; //减去头尾和cmd,length两个字节
  for (uint8_t i = 0; i < checksum_range; i++) {
    sum += start_response_recv[i]; //校验和校验的是从头到校验和前一个字节
  }
  if (Debug_Mode == 0) {
    // Uart0_Tx_Send(xieyi1_fanhui,5);
    PC_Comm_Send(start_response_recv, protocol_length);
  } else {
    log_d("发送开始测试应答: ");
    hexdump((uint8_t *)&start_response_recv, protocol_length);
  }
}

// 工装收到上位机的查询结果,发送这条指令返回相关数据
void PC_xieyifasong_2() {
  uint16_t jishu_lenth = 0;
  uint16_t hejiaoyan = 0;
  memset(xieyi2_fanhui, 0x00, send_lenth);
  xieyi2_fanhui[jishu_lenth++] = 0x68;
  xieyi2_fanhui[jishu_lenth++] = 0xAD;
  // 当前返回帧总字节长度，先进行初始化，赋值00
  xieyi2_fanhui[jishu_lenth++] = 0x00;
  xieyi2_fanhui[jishu_lenth++] = Test_jiejuo_jilu.gongwei; // 工位
  // 小端模式，先存放低位字节
  // 主电电压(供电电压)低字节
  xieyi2_fanhui[jishu_lenth++] =
      (Test_jiejuo_jilu.zhidian_dianya_gongdian) & 0xFF;
  // 主电电压(供电电压)高字节
  xieyi2_fanhui[jishu_lenth++] =
      ((Test_jiejuo_jilu.zhidian_dianya_gongdian) >> 8) & 0xFF;
  // 主电电压(协议获取)低字节
  xieyi2_fanhui[jishu_lenth++] = (Test_jiejuo_jilu.zhidian_dianya_huoqu) & 0xFF;
  // 主电电压(协议获取)高字节
  xieyi2_fanhui[jishu_lenth++] =
      ((Test_jiejuo_jilu.zhidian_dianya_huoqu) >> 8) & 0xFF;
  // 静态功耗低字节
  xieyi2_fanhui[jishu_lenth++] =
      (Test_jiejuo_jilu.zhidian_jingtai_gonghao) & 0xFF;
  // 静态功耗高字节
  xieyi2_fanhui[jishu_lenth++] =
      ((Test_jiejuo_jilu.zhidian_jingtai_gonghao) >> 8) & 0xFF;
  // 满水功耗低字节
  xieyi2_fanhui[jishu_lenth++] =
      (Test_jiejuo_jilu.zhidian_manshui_gonghao) & 0xFF;
  // 满水功耗高字节
  xieyi2_fanhui[jishu_lenth++] =
      ((Test_jiejuo_jilu.zhidian_manshui_gonghao) >> 8) & 0xFF;
  // 走水功耗低字节
  xieyi2_fanhui[jishu_lenth++] =
      (Test_jiejuo_jilu.zhidian_zoushui_gonghao) & 0xFF;
  // 走水功耗高字节
  xieyi2_fanhui[jishu_lenth++] =
      ((Test_jiejuo_jilu.zhidian_zoushui_gonghao) >> 8) & 0xFF;
  // 备电/NB/扩频电压(供电电压)低字节
  xieyi2_fanhui[jishu_lenth++] =
      (Test_jiejuo_jilu.beidian_dianya_gongdian) & 0xFF;
  // 备电/NB/扩频电压(供电电压)高字节
  xieyi2_fanhui[jishu_lenth++] =
      ((Test_jiejuo_jilu.beidian_dianya_gongdian) >> 8) & 0xFF;
  // 备电电压获取设置默认值为3600，没有备电就使用
  Test_jiejuo_jilu.beidian_dianya_huoqu = 3600;
  // 备电/NB/扩频电压(协议获取)低字节
  xieyi2_fanhui[jishu_lenth++] = (Test_jiejuo_jilu.beidian_dianya_huoqu) & 0xFF;
  /// 备电/NB/扩频电压(协议获取)高字节
  xieyi2_fanhui[jishu_lenth++] =
      ((Test_jiejuo_jilu.beidian_dianya_huoqu) >> 8) & 0xFF;
  // 备电/NB/扩频功耗低字节
  xieyi2_fanhui[jishu_lenth++] = (Test_jiejuo_jilu.beidian_gonghao) & 0xFF;
  // 备电/NB/扩频功耗高字节
  xieyi2_fanhui[jishu_lenth++] =
      ((Test_jiejuo_jilu.beidian_gonghao) >> 8) & 0xFF;
  // 蓝牙检测(预留,不检测) 0异常 1正常
  xieyi2_fanhui[jishu_lenth++] = Test_jiejuo_jilu.lanya_jiance;
  // flash检测 0异常 1正常
  xieyi2_fanhui[jishu_lenth++] = Test_jiejuo_jilu.flash_jiance;
  // 计量 检测
  xieyi2_fanhui[jishu_lenth++] = Test_jiejuo_jilu.jiliang_jiance;
  // 红外检测 0异常 1正常
  xieyi2_fanhui[jishu_lenth++] = Test_jiejuo_jilu.hongwai_jiance;
  // IMEI
  memcpy(&xieyi2_fanhui[jishu_lenth], Test_jiejuo_jilu.IMEI_CHK, 15);
  jishu_lenth += 15;
  // IMSI
  memcpy(&xieyi2_fanhui[jishu_lenth], Test_jiejuo_jilu.IMSI_CHK, 15);
  jishu_lenth += 15;
  // ICCID
  memcpy(&xieyi2_fanhui[jishu_lenth], Test_jiejuo_jilu.ICCID_CHK, 20);
  jishu_lenth += 20;
  // 信号强度
  xieyi2_fanhui[jishu_lenth++] = Test_jiejuo_jilu.CSQ;
  // 阀门
  xieyi2_fanhui[jishu_lenth++] = Test_jiejuo_jilu.FM;
  // 到位 - 使用实际测试结果，不再强制设为1
  xieyi2_fanhui[jishu_lenth++] = Test_jiejuo_jilu.FM_daowei;
  // EEPROM
  xieyi2_fanhui[jishu_lenth++] = Test_jiejuo_jilu.EEPROM_jiance;
  // GP30电压低字节
  xieyi2_fanhui[jishu_lenth++] = (Test_jiejuo_jilu.GP30_dianya) & 0xFF;
  // GP30电压高字节
  xieyi2_fanhui[jishu_lenth++] = ((Test_jiejuo_jilu.GP30_dianya) >> 8) & 0xFF;
  // LoraEUI
  memcpy(&xieyi2_fanhui[jishu_lenth], Test_jiejuo_jilu.loraEUI, 16);
  jishu_lenth += 16;
  // 强磁检测 0异常 1正常 ，没有就设置默认值1，正常
  Test_jiejuo_jilu.qiangci_jiance = 1;
  xieyi2_fanhui[jishu_lenth++] = Test_jiejuo_jilu.qiangci_jiance;
  // 开盖检测，没有就设置默认值1
  Test_jiejuo_jilu.kaigai_jiance = 1;
  xieyi2_fanhui[jishu_lenth++] = Test_jiejuo_jilu.kaigai_jiance;
  // GPS模组检测
  xieyi2_fanhui[jishu_lenth++] = Test_jiejuo_jilu.GPSmozu_jiacne;
  // 无磁检测
  xieyi2_fanhui[jishu_lenth++] = 0;
  // 校验码
  memcpy(&xieyi2_fanhui[jishu_lenth], Test_jiejuo_jilu.jiaoyanma, 2);
  jishu_lenth += 2;
  // 版本号
  memcpy(&xieyi2_fanhui[jishu_lenth], Test_jiejuo_jilu.banbenhao, 2);
  jishu_lenth += 2;
  // 水温检测
  xieyi2_fanhui[jishu_lenth++] = Test_jiejuo_jilu.water_temp;
  // 压力检测，暂时没有，设置默认值1,正常值
  Test_jiejuo_jilu.pressure_value = 1;
  xieyi2_fanhui[jishu_lenth++] = Test_jiejuo_jilu.pressure_value;

  // 对帧长度赋值
  xieyi2_fanhui[2] = jishu_lenth + 2;
  // 和校验 值初始化为0,后面会加到这里，不要赋值
  xieyi2_fanhui[jishu_lenth] = 0;

  for (hejiaoyan = 0; hejiaoyan < jishu_lenth; hejiaoyan++) {
    xieyi2_fanhui[jishu_lenth] += xieyi2_fanhui[hejiaoyan];
  }
  jishu_lenth++;
  xieyi2_fanhui[jishu_lenth++] = 0x16;

  log_d("当前发送数据到PC串口的是：");
  protocol_debug_print(xieyi2_fanhui, jishu_lenth);

  // Only send binary protocol data when not in debug mode to avoid garbled
  // output
  if (Debug_Mode == 0) {
    PC_Comm_Send(xieyi2_fanhui, jishu_lenth);
  } else {
    log_d("(调试模式：跳过二进制协议发送)");
  }
}

// 测试结果分析函数 - 根据测试配置显示检测值和合格判断
void PC_test_result_analysis(void) {
  extern uint16_t water_temperature_value;

  // 获取当前测试配置
  uint8_t has_valve = 0;
  const char *meter_type_str = "";
  const char *module_type_str = "";
  if (Debug_Mode == 0) {
    return;
  }

  if (water_meter_type == 0) {
    // 机械表
    meter_type_str = "机械表";
    has_valve = (Test_jiejuo_jilu.mechanical_config.valve_type != 0);
  } else {
    // 超声波表
    meter_type_str = "超声波表";
    has_valve = (Test_jiejuo_jilu.ultrasonic_config.valve_type != 0);
    module_type_str =
        (Test_jiejuo_jilu.ultrasonic_config.module_type == 0) ? "NB" : "CAT1";
  }

  log_d("\r\n========================================");
  log_d("           水表检测结果汇总");
  log_d("========================================");

  // 测试配置信息
  log_d("【测试配置】");
  log_d("  表类型: %s", meter_type_str);
  if (water_meter_type == 1) {
    log_d("  模块类型: %s", module_type_str);
    log_d("  管径: DN%d", Test_jiejuo_jilu.ultrasonic_config.pipe_size_type);
  }
  log_d("  阀门配置: %s", has_valve ? "有阀" : "无阀");
  log_d("----------------------------------------");

  // 测试状态
  log_d("【测试状态】");
  if (Test_quanju_canshu_L.test_status == TEST_SUCCESS) {
    log_d("  ✓ 测试成功");
  } else {
    log_d("  ✗ 测试失败");
  }
  log_d("----------------------------------------");

  // 电压检测 (必测项)
  log_d("【电压检测】");
  // 主电供电电压
  log_d("  主电供电电压: %lu mV - ",
        (unsigned long)Test_jiejuo_jilu.zhidian_dianya_gongdian);
  if (Test_jiejuo_jilu.zhidian_dianya_gongdian >= ZHUDIAN_DIANYA_MIN &&
      Test_jiejuo_jilu.zhidian_dianya_gongdian <= ZHUDIAN_DIANYA_MAX) {
    log_d("合格");
  } else {
    log_d("不合格");
  }
  // 主电协议电压
  log_d("  主电协议电压: %lu mV - ",
        (unsigned long)Test_jiejuo_jilu.zhidian_dianya_huoqu);
  if (Test_jiejuo_jilu.zhidian_dianya_huoqu >= ZHUDIAN_DIANYA_MIN &&
      Test_jiejuo_jilu.zhidian_dianya_huoqu <= ZHUDIAN_DIANYA_MAX) {
    log_d("合格");
  } else {
    log_d("不合格");
  }
  log_d("----------------------------------------");

  // 功耗检测 (必测项)
  log_d("【功耗检测】");
  log_d("  空水功耗: %u uA - ", Test_jiejuo_jilu.zhidian_jingtai_gonghao);
  if (Test_jiejuo_jilu.zhidian_jingtai_gonghao >= GONGHAO_KONGSHUI_MIN &&
      Test_jiejuo_jilu.zhidian_jingtai_gonghao <= GONGHAO_KONGSHUI_MAX) {
    log_d("合格");
  } else {
    log_d("不合格");
  }

  log_d("  满水功耗: %u uA - ", Test_jiejuo_jilu.zhidian_manshui_gonghao);
  if (Test_jiejuo_jilu.zhidian_manshui_gonghao >= GONGHAO_MANSHUI_MIN &&
      Test_jiejuo_jilu.zhidian_manshui_gonghao <= GONGHAO_MANSHUI_MAX) {
    log_d("合格");
  } else {
    log_d("不合格");
  }

  log_d("  走水功耗: %u uA - ", Test_jiejuo_jilu.zhidian_zoushui_gonghao);
  if (Test_jiejuo_jilu.zhidian_zoushui_gonghao >= GONGHAO_ZOUSHUI_MIN &&
      Test_jiejuo_jilu.zhidian_zoushui_gonghao <= GONGHAO_ZOUSHUI_MAX) {
    log_d("合格");
  } else {
    log_d("不合格");
  }
  log_d("----------------------------------------");

  // 温度检测
  log_d("【温度检测】");
  log_d("  水温: %u.%u°C - ", water_temperature_value / 10,
        water_temperature_value % 10);
  if (water_temperature_value >= WENDU_MIN &&
      water_temperature_value <= WENDU_MAX) {
    log_d("合格");
  } else {
    log_d("不合格");
  }
  log_d("----------------------------------------");

  // 设备标识信息
  log_d("【设备标识】");
  log_d("  IMEI: ");
  uint8_t imei_valid = 0;
  for (int i = 0; i < 15; i++) {
    if (Test_jiejuo_jilu.IMEI_CHK[i] >= '0' &&
        Test_jiejuo_jilu.IMEI_CHK[i] <= '9') {
      log_d("%c", Test_jiejuo_jilu.IMEI_CHK[i]);
      imei_valid = 1;
    } else if (Test_jiejuo_jilu.IMEI_CHK[i] != 0) {
      log_d(".");
    }
  }
  if (!imei_valid) {
    log_d("未获取");
  }
  log_d("");

  log_d("  IMSI: ");
  uint8_t imsi_valid = 0;
  for (int i = 0; i < 15; i++) {
    if (Test_jiejuo_jilu.IMSI_CHK[i] >= '0' &&
        Test_jiejuo_jilu.IMSI_CHK[i] <= '9') {
      log_d("%c", Test_jiejuo_jilu.IMSI_CHK[i]);
      imsi_valid = 1;
    } else if (Test_jiejuo_jilu.IMSI_CHK[i] != 0) {
      log_d(".");
    }
  }
  if (!imsi_valid) {
    log_d("未获取");
  }
  log_d("");

  log_d("  ICCID: ");
  uint8_t iccid_valid = 0;
  for (int i = 0; i < 20; i++) {
    if (Test_jiejuo_jilu.ICCID_CHK[i] >= '0' &&
        Test_jiejuo_jilu.ICCID_CHK[i] <= '9') {
      log_d("%c", Test_jiejuo_jilu.ICCID_CHK[i]);
      iccid_valid = 1;
    } else if (Test_jiejuo_jilu.ICCID_CHK[i] != 0) {
      log_d(".");
    }
  }
  if (!iccid_valid) {
    log_d("未获取");
  }
  log_d("");
  log_d("----------------------------------------");

  // 信号强度 (必测项)
  log_d("【信号检测】");
  if (Test_jiejuo_jilu.CSQ > 0 && Test_jiejuo_jilu.CSQ <= 31) {
    log_d("  信号强度(CSQ): %u - 合格", Test_jiejuo_jilu.CSQ);
  } else {
    log_d("  信号强度(CSQ): %u - 异常", Test_jiejuo_jilu.CSQ);
  }
  log_d("----------------------------------------");

  // 功能模块检测状态 (必测项)
  log_d("【功能模块检测】");
  log_d("  Flash检测: %s", Test_jiejuo_jilu.flash_jiance ? "合格" : "不合格");
  log_d("  计量检测: %s", Test_jiejuo_jilu.jiliang_jiance ? "合格" : "不合格");
  log_d("  红外检测: %s", Test_jiejuo_jilu.hongwai_jiance ? "合格" : "不合格");

  // 阀门检测 - 仅在有阀配置时显示
  if (has_valve) {
    log_d("  阀门检测: %s", Test_jiejuo_jilu.FM ? "合格" : "不合格");
    log_d("  阀门到位: %s", Test_jiejuo_jilu.FM_daowei ? "合格" : "不合格");
  } else {
    log_d("  阀门检测: 未配置(无阀模式)");
  }

  // EEPROM检测 - 当前配置不测试
  log_d("  EEPROM检测: 未配置");

  log_d("========================================");
}

// 解析上位机发送的指令，比如开始测试指令，比如查询指令
void PC_xieyijiexi(uint8_t zufuchua[], uint16_t lenth) {
  uint16_t pHead = 0;
  uint8_t hejiaoyan = 0;
  uint16_t zhenchangdu = 0;
  uint16_t linshichangdu = 0;

  // 发送接收到数据的确认
  log_d("PC Protocol Start");
  log_d("开始解析协议，数据长度=%d", lenth);

  while (1) {
    // 确保是我们当前需要的数据，数据长度最起码达标
    if (pHead + 5 >= lenth) {
      log_d("到达数据末尾，pHead=%d", pHead);
      break;
    }
    // log_d("检查位置pHead=%d，数据=0x%02X", pHead, zufuchua[pHead]);
    if (zufuchua[pHead] == 0x68) {
      log_d("找到帧头0x68,继续验证帧的类型%02X", zufuchua[pHead + 1]);

      //先查看协议类型， 是开始测试指令还是查询结果指令，还是其他指令
      if (zufuchua[pHead + 1] == start_cmd_send.cmd) {
        log_d("当前是开始测试指令，对开始测试指令进行解析");
        //验证帧长度和尾帧
        if ((zufuchua[pHead + 2]) != start_cmd_send.length &&
            zufuchua[pHead + start_cmd_send.length - 1] != 0x16) {
          if ((zufuchua[pHead + 2]) != start_cmd_send.length) {
            log_d("开始测试指令长度错误，期望=%d，实际=%d",
                  start_cmd_send.length, zhenchangdu);
            else if (zufuchua[pHead + start_cmd_send.length - 1] != 0x16) {
              log_d("开始测试指令尾帧错误，期望=0x16，实际=0x%02X",
                    zufuchua[pHead + start_cmd_send.length - 1]);
            }
            pHead += zhenchangdu + 4; //跳过当前帧，继续解析后续数据
            continue;
          } else if ((zufuchua[pHead + 2]) == start_cmd_send.length &&
                     zufuchua[pHead + start_cmd_send.length - 1] == 0x16) {
            log_d("开始测试指令长度和尾帧验证通过");
          }
        } else {
          log_i("当前不是开始测试指令，跳过");
          pHead += 1; //继续寻找下一个帧头
          continue;
        }
        log_d("校验和计算=0x%02X，接收=0x%02X", hejiaoyan,
              zufuchua[pHead + zhenchangdu - 1]);
        if (hejiaoyan == zufuchua[pHead + zhenchangdu - 1]) {
          // 协议校验完成，发送确认消息
          log_d("Start Test CMD OK");
          // 进入主程序处理
          memcpy(Test_jiejuo_jilu.mokuaihao, &zufuchua[pHead + 4], 6);
          Test_jiejuo_jilu.biaoju_leixing_famen = zufuchua[pHead + 10];
          if (Test_jiejuo_jilu.biaoju_leixing_famen == 0) {
            log_d("当前测试水表类型为机械表");
            water_meter_type = 0;
          } else {
            log_d("当前测试水表类型为超声波表");
            water_meter_type = 1;
          }

          // 解析阀门配置参数（使用结构体）
          parse_ultrasonic_config_from_bytes(
              &zufuchua[pHead + 11], &Test_jiejuo_jilu.ultrasonic_config);
          parse_mechanical_config_from_bytes(
              &zufuchua[pHead + 17], &Test_jiejuo_jilu.mechanical_config);

          // 打印解析后的配置信息
          log_d("超声波表配置: 管道=%d, 换能器=%d, 管径=%d, 阀门=%d, "
                "模块=%d\r\n",
                Test_jiejuo_jilu.ultrasonic_config.pipe_type,
                Test_jiejuo_jilu.ultrasonic_config.transducer_type,
                Test_jiejuo_jilu.ultrasonic_config.pipe_size_type,
                Test_jiejuo_jilu.ultrasonic_config.valve_type,
                Test_jiejuo_jilu.ultrasonic_config.module_type);
          log_d("机械表配置: 管径=%dmm, 阀门=%d, 超时=%ds, 堵转=%dmA",
                Test_jiejuo_jilu.mechanical_config.pipe_diameter_mm,
                Test_jiejuo_jilu.mechanical_config.valve_type,
                Test_jiejuo_jilu.mechanical_config.timeout_seconds,
                Test_jiejuo_jilu.mechanical_config.stall_current_ma);

          // 判断当前有阀测试还是无阀测试（根据对应表类型的阀门配置）
          if (water_meter_type == 0) {
            // 机械表：检查机械表配置中的阀门类型
            test_famen_type =
                (Test_jiejuo_jilu.mechanical_config.valve_type != 0) ? 1 : 0;
          } else {
            // 超声波表：检查超声波表配置中的阀门类型
            test_famen_type =
                (Test_jiejuo_jilu.ultrasonic_config.valve_type != 0) ? 1 : 0;
          }
          log_d("当前测试水表阀门类型为%s", test_famen_type ? "有阀" : "无阀");

          test_start();
          PC_xieyifasong_1();
          pHead += zhenchangdu;
        } else {
          // 校验和错误，发送错误消息
          log_d("校验和错误");
        }
      } else {
        log_d("帧验证失败");
      }
    } else {
      log_d("帧长度过长：%d", zhenchangdu);
    }
  }
  //如果是查询结果指令
  else if (zufuchua[pHead + 1] == 0xAC &&
           zufuchua[pHead + 3] == Test_jiejuo_jilu.gongwei &&
           zufuchua[pHead + 5] == 0x16) {
    log_d("当前是查询结果指令，即将返回测试结果");
    if (zufuchua[pHead + 1] == 0xAC) {
      // 获取协议长度
      zhenchangdu = zufuchua[pHead + 2];
      // 长度应低于100个字节
      if (zhenchangdu < 100) {
        // 校验工位与尾帧是否正确
        if (zufuchua[pHead + 3] == Test_jiejuo_jilu.gongwei &&
            zufuchua[pHead + zhenchangdu - 1] == 0x16) {
          // 进行和校验
          hejiaoyan = 0;
          for (linshichangdu = 0; linshichangdu < zhenchangdu - 2;
               linshichangdu++) {
            hejiaoyan += zufuchua[pHead + linshichangdu];
          }
          if (hejiaoyan == zufuchua[pHead + zhenchangdu - 2]) {
            // 协议校验完成，发送确认消息
            log_d("Query CMD OK");
            // 总是返回结果，用于调试
            if (Test_liucheng_L == w_end) {

              PC_xieyifasong_2();
            }
            pHead += zhenchangdu;
          } else {
            // 校验和错误，发送错误消息
            log_d("Query Checksum Error");
          }
        }
      }
    }
  }
  // 这个协是设置开启或者关闭log日志的协议，或者开启或者关闭透传模式的协议
  // 68 AE 08 00 00 00 1E 16
  // 这个是关闭日志和透传模式的指令，08后面第一个是工位，第二个00是开关日志模式，第二个00是开关透传模式
  else if (zufuchua[pHead + 1] == 0xAE) {
    // 校验工位与尾帧是否正确
    // 先验工位，如工位不对，说明协议错了，跳过
    if (zufuchua[pHead + 2] != 0x08 ||
        zufuchua[pHead + 3] != Test_jiejuo_jilu.gongwei ||
        zufuchua[pHead + 7] != 0x16) {
      log_d("设置日志或透传模式指令，帧验证失败，跳过");
      pHead++;
      continue; // 这里continue，跳过当前循环，继续下一个位置寻找帧头
    }
    // 进行累加和校验
    hejiaoyan = 0;
    for (linshichangdu = 0; linshichangdu < 6; linshichangdu++) {
      hejiaoyan += zufuchua[pHead + linshichangdu];
    }
    if (hejiaoyan != zufuchua[pHead + 6]) {
      log_d("设置日志或透传模式指令，校验和错误，跳过");
      pHead++;
      continue; // 这里continue，跳过当前循环，继续下一个位置寻找帧头
    }
    log_d("收到设置日志或透传模式指令，正在处理...");
    if (zufuchua[pHead + 4] == 0x00) {
      Debug_Mode = 0;
      log_d("已关闭调试日志模式");
    } else {
      Debug_Mode = 1;
      log_d("已开启调试日志模式");
    }
    if (zufuchua[pHead + 5] == 0x00) {
      PassThrough_Mode = 0;
      log_d("已关闭透传模式");
    } else {
      PassThrough_Mode = 1;
      log_d("已开启透传模式");
    }
    // 发送设置成功的响应帧
    PC_xieyifasong_3();

    pHead += 8; // 跳过当前完整帧
    continue;
  }
  //开启App程序的升级指令，解析到这个标志位会进入App升级流程
  //具体指令如下：
  //帧头：0x68
  //功能码：0xBA
  //长度：整个帧长度，这里暂定
  //工位：工位号
  //数据域0：升级模式，00是人工模式，01是自动模式，自动模式会很快升级，默认会配置115200波特率，使用xmodem协议升级，关闭bootloader日志,固件大小要验证准确的值，手动模式不需要验证准确的值，比最大的小就可以
  //数据域1：升级相关参数，00：默认波特率9600，01：115200,这个设置会配置Bootloader的升级的波特率;如果是人手动发送升级，建议用9600,但是也支持115200升级
  //如果设置了115200波特率，当前仍然是9600波特率返回相关的响应，等reset之后，bootloader会使用115200波特率,但是app
  //永远默认使用9600波特率，所有切换波特率行为都是在bootloader阶段
  //数据域2：00 默认使用xmodem协议升级，01
  //数据域3:等待app程序的发送时间，单位秒，默认3秒,如果是人手动发送，可以设置长一点时间，比如20秒，人手动如果切换115200波特率，需要时间准备
  //数据域4：当前9600波特率下可以设置bootloader的日志是否开启，00关闭，01开启
  //数据域5：当前9600波特率下开启倒计时log，这个参数是倒计时的时间，和上面的等待app发送时间无关，当前是什么波特率，就是什么波特率下的倒计时log
  //数据域6：固件大小，单位是kb，这个bootloader会检验是否超过Flash大小，超过会拒绝升级
  //数据域名
  //使用其他协议升级,暂未定义，后续开放
  //校验和：前面所有字节累加和
  //帧尾：0x16
  else if (zufuchua[pHead + 1] == 0xBA) {
    // 校验工位与尾帧是否正确
    // 先验工位，如工位不对，说明协议错了，跳过
    if (zufuchua[pHead + 2] != 0x08 ||
        zufuchua[pHead + 3] != Test_jiejuo_jilu.gongwei ||
        zufuchua[pHead + 7] != 0x16) {
      log_d("设置日志或透传模式指令，帧验证失败，跳过");
      pHead++;
      continue; // 这里continue，跳过当前循环，继续下一个位置寻找帧头
    }
    // 进行累加和校验
    hejiaoyan = 0;
    for (linshichangdu = 0; linshichangdu < 6; linshichangdu++) {
      hejiaoyan += zufuchua[pHead + linshichangdu];
    }
    if (hejiaoyan != zufuchua[pHead + 6]) {
      log_d("设置日志或透传模式指令，校验和错误，跳过");
      pHead++;
      continue; // 这里continue，跳过当前循环，继续下一个位置寻找帧头
    }
    log_d("收到设置日志或透传模式指令，正在处理...");
    if (zufuchua[pHead + 4] == 0x00) {
      Debug_Mode = 0;
      log_d("已关闭调试日志模式");
    } else {
      Debug_Mode = 1;
      log_d("已开启调试日志模式");
    }
    if (zufuchua[pHead + 5] == 0x00) {
      PassThrough_Mode = 0;
      log_d("已关闭透传模式");
    } else {
      PassThrough_Mode = 1;
      log_d("已开启透传模式");
    }
    // 发送设置成功的响应帧
    PC_xieyifasong_3();

    pHead += 8; // 跳过当前完整帧
    continue;
  }
}
pHead++;
}
}

// 工装收到上位机发送的设置日志或透传模式指令的应答指令
void PC_xieyifasong_3(void) {
  uint8_t xieyi3_fanhui[8] = {0x68, 0xAF, 0x08, 0x00, 0x00, 0x00, 0x1F, 0x16};

  xieyi3_fanhui[3] = Test_jiejuo_jilu.gongwei;
  // 根据当前模式设置响应内容
  if (Debug_Mode == 0) {
    xieyi3_fanhui[4] = 0x00;
  } else {
    xieyi3_fanhui[4] = 0x01;
  }
  if (PassThrough_Mode == 0) {
    xieyi3_fanhui[5] = 0x00;
  } else {
    xieyi3_fanhui[5] = 0x01;
  }
  // 计算校验和
  xieyi3_fanhui[6] = 0;
  for (int i = 0; i < 6; i++) {
    xieyi3_fanhui[6] += xieyi3_fanhui[i];
  }

  log_d("发送设置日志或透传模式响应: ");
  protocol_debug_print(xieyi3_fanhui, 8);

  // Only send binary protocol data when not in debug mode to avoid garbled
  // output
  if (Debug_Mode == 0) {
    PC_Comm_Send(xieyi3_fanhui, 8);
  } else {
    log_d("(调试模式：跳过二进制协议发送)");
  }
}
