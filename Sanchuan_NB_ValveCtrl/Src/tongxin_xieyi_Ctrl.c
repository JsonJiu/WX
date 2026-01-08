#include "tongxin_xieyi_Ctrl.h"
#include "GPIO.h"
#include "Test_List.h"
#include "uart0.h"
#include "uart1.h"
#include "valve_test.h"
// 信号函数
#define WaterMeterProtocol_VERSION_NUMBER 0x0A
uint8_t xieyi_zuzhen[200];
uint8_t kongshujuyu[1];
uint8_t moren_biaohao[6] = {0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA};

// 比较字符串要原样比较含\0是结尾
uint8_t bijiao_zifuchuan(uint8_t bijiao1[], uint8_t bijiao2[], uint16_t lenth) {
  uint16_t bijiao_lenth = 0;
  for (bijiao_lenth = 0; bijiao_lenth < lenth; bijiao_lenth++) {
    if (bijiao1[bijiao_lenth] != bijiao2[bijiao_lenth]) {
      return 0;
    }
  }
  return 1;
}

uint16_t user_cmd_get_crc(unsigned char *crcbuf, int len) {
  uint16_t x;
  uint16_t crc = 0;
  uint16_t i = 0;
  for (i = 0; i < len; i++) {
    x = ((crc >> 8) ^ (uint16_t)crcbuf[i]) & 0x0FF;
    x ^= (x >> 4);
    crc = (crc << 8) ^ (x << 12) ^ (x << 5) ^ x;
  }
  return crc & 0x0ffff;
}
// 通信  2031H  C=82H  读表号
//

void TONGXIN_xieyijiexi(uint8_t zufuchua[], uint16_t lenth) {
  uint16_t pHead = 0;
  uint16_t zhen_lenth = 0;
  uint16_t CRC_jiaoyan = 0;
  uint16_t CRC_jiaoyan_jisuan = 0;
  uint16_t shujubiaoshi = 0;
  uint8_t dian1 = 0;
  uint8_t dian2 = 0;
  uint8_t dian3 = 0;
  uint16_t jiexi_changdu = 0;
  while (1) {
    // 协议最短为18
    if (pHead + 17 > lenth) {
      break;
    }
    if (zufuchua[pHead] == 0x68) {
      // 确认并获取帧长
      zhen_lenth = 0;
      zhen_lenth = zufuchua[pHead + 11];
      zhen_lenth = zhen_lenth << 8;
      zhen_lenth += zufuchua[pHead + 10];
      // 长度不正确
      if (pHead + zhen_lenth <= lenth) {
        // 校验是否正确
        if (zufuchua[pHead + zhen_lenth - 1] == 0x16) {
          // 读协议CRC校验值
          CRC_jiaoyan = 0;
          CRC_jiaoyan = zufuchua[pHead + zhen_lenth - 2];
          CRC_jiaoyan = CRC_jiaoyan << 8;
          CRC_jiaoyan += zufuchua[pHead + zhen_lenth - 3];
          // 执行CRC校验
          CRC_jiaoyan_jisuan =
              user_cmd_get_crc(&zufuchua[pHead], zhen_lenth - 3);
          // CRC校验通过为协议
          if (CRC_jiaoyan == CRC_jiaoyan_jisuan) {
            // 保存当前表号通过
            memcpy(Test_jiejuo_jilu.dangqian_biaohao, &zufuchua[pHead + 1], 6);
            // 协议类型
            switch (zufuchua[pHead + 7]) {
            case 0x00:
              // 协议版本号
              switch (zufuchua[pHead + 8]) {
              case WaterMeterProtocol_VERSION_NUMBER: // 添加对版本号0x0A的支持
                DeBug_print("当前版本号是：%d\r\n", zufuchua[pHead + 8]);
                // 控制码
                switch (zufuchua[pHead + 9]) {
                // 保留字段
                // 读方向也不直接回复表号
                case 0x81:
                  break;
                  // 读方向
                case 0x82:
                  // 数据标识
                  shujubiaoshi = 0;
                  shujubiaoshi = zufuchua[pHead + 13];
                  shujubiaoshi = shujubiaoshi << 8;
                  shujubiaoshi += zufuchua[pHead + 12];
                  // pHead+15
                  switch (shujubiaoshi) {
                    // 表号
                  case 0x2031:
                    // 表号获取
                    Test_quanju_canshu_L.time_softdelay_ms = 0;
                    test_xieyi_jilu_Rec = w_get_biaohao;
                    break;
                    // 上告
                  case 0xF001:
                    // 上告完成
                    Test_quanju_canshu_L.time_softdelay_ms = 0;
                    test_xieyi_jilu_Rec = w_get_shanggao;
                    break;
                    // 测试协议
                    // 版本号
                  case 0x2011:
                    // 读版本号，32位
                    // 寻找协议长度
                    while (1) {
                      if (zufuchua[pHead + 17 + jiexi_changdu] == 0)
                        break;
                      if (dian1 == 0) {
                        if (zufuchua[pHead + 17 + jiexi_changdu] == '.') {
                          dian1 = jiexi_changdu;
                        }
                      } else if (dian2 == 0) {
                        if (zufuchua[pHead + 17 + jiexi_changdu] == '.') {
                          dian2 = jiexi_changdu;
                        }
                      } else if (dian3 == 0) {
                        if (zufuchua[pHead + 17 + jiexi_changdu] == '.') {
                          dian3 = jiexi_changdu;
                        }
                      }
                      jiexi_changdu++;
                    }
                    if (dian1 == 1) {
                      Test_linshi_cunchushuju_L.L_banbenhao[0] =
                          zufuchua[pHead + 17] - '0';
                    } else if (dian1 == 2) {
                      Test_linshi_cunchushuju_L.L_banbenhao[0] =
                          (zufuchua[pHead + 17] - '0') << 4;
                      Test_linshi_cunchushuju_L.L_banbenhao[0] +=
                          zufuchua[pHead + 18] - '0';
                    }
                    if ((dian2 - dian1) == 2) {
                      Test_linshi_cunchushuju_L.L_banbenhao[1] =
                          (zufuchua[pHead + 17 + dian1 + 1] - '0') << 4;
                    }
                    if ((jiexi_changdu - dian2) == 2) {
                      Test_linshi_cunchushuju_L.L_banbenhao[1] +=
                          (zufuchua[pHead + 17 + dian2 + 1] - '0');
                    }
                    Test_quanju_canshu_L.time_softdelay_ms = 0;
                    test_xieyi_jilu_Rec = w_get_banbenhao;
                    break;
                  case 0xF003:
                    // flash检测
                    // 保存flash
                    Test_linshi_cunchushuju_L.L_flash_chk =
                        zufuchua[pHead + 17];
                    // 主电电压
                    Test_linshi_cunchushuju_L.L_zhudian_dianya =
                        zufuchua[pHead + 19];
                    Test_linshi_cunchushuju_L.L_zhudian_dianya =
                        Test_linshi_cunchushuju_L.L_zhudian_dianya << 8;
                    Test_linshi_cunchushuju_L.L_zhudian_dianya +=
                        zufuchua[pHead + 18];
                    // 备电电压
                    Test_linshi_cunchushuju_L.L_beidian_dianya =
                        zufuchua[pHead + 21];
                    Test_linshi_cunchushuju_L.L_beidian_dianya =
                        Test_linshi_cunchushuju_L.L_beidian_dianya << 8;
                    Test_linshi_cunchushuju_L.L_beidian_dianya +=
                        zufuchua[pHead + 20];
                    // 压力获取
                    Test_linshi_cunchushuju_L.L_yali_CHK = zufuchua[pHead + 22];
                    // EEPROM状态获取
                    Test_linshi_cunchushuju_L.L_EEPROM_CHK =
                        zufuchua[pHead + 23];
                    // 霍尔1状态获取
                    Test_linshi_cunchushuju_L.L_huoer1_CHK =
                        zufuchua[pHead + 24];
                    // GPRS状态获取
                    Test_linshi_cunchushuju_L.L_GPRS_mozu_CHK =
                        zufuchua[pHead + 25];
                    // 霍尔2状态获取
                    Test_linshi_cunchushuju_L.L_huoer2_CHK =
                        zufuchua[pHead + 26];
                    // 瞬时流量获取
                    memcpy(
                        Test_linshi_cunchushuju_L.L_chaosheng_shunshiliuliang,
                        &zufuchua[pHead + 27], 4);
                    // 走水状态获取
                    Test_linshi_cunchushuju_L.L_chaosheng_zousuizhuangtai =
                        zufuchua[pHead + 31];
                    // GP30状态获取
                    Test_linshi_cunchushuju_L.L_GP30mozu_AD_CHK =
                        zufuchua[pHead + 33];
                    Test_linshi_cunchushuju_L.L_GP30mozu_AD_CHK =
                        Test_linshi_cunchushuju_L.L_GP30mozu_AD_CHK << 8;
                    Test_linshi_cunchushuju_L.L_GP30mozu_AD_CHK +=
                        zufuchua[pHead + 32];
                    // IMEI获取
                    memcpy(Test_linshi_cunchushuju_L.L_IMEI,
                           &zufuchua[pHead + 34], 15);
                    // IMSI获取
                    memcpy(Test_linshi_cunchushuju_L.L_IMSI,
                           &zufuchua[pHead + 49], 15);
                    // ICCID获取
                    memcpy(Test_linshi_cunchushuju_L.L_ICCID,
                           &zufuchua[pHead + 64], 20);
                    // CSQ获取
                    Test_linshi_cunchushuju_L.L_CSQ = zufuchua[pHead + 84];
                    // lorakey(有的一些项目前只用一位)
                    // Test_linshi_cunchushuju_L.lorakey[0] =
                    // zufuchua[pHead+85];
                    memcpy(Test_linshi_cunchushuju_L.lorakey,
                           &zufuchua[pHead + 85], 16);
                    // 到位1获取
                    Test_linshi_cunchushuju_L.L_daowei1 = zufuchua[pHead + 101];
                    // 到位2获取
                    Test_linshi_cunchushuju_L.L_daowei2 = zufuchua[pHead + 102];
                    DeBug_print("到位1状态是%d，到位2状态是%d\r\n",
                                Test_linshi_cunchushuju_L.L_daowei1,
                                Test_linshi_cunchushuju_L.L_daowei2);
                    // 计量1获取
                    Test_linshi_cunchushuju_L.L_jilianghuoer1 =
                        zufuchua[pHead + 103];
                    // 计量2获取
                    Test_linshi_cunchushuju_L.L_jilianghuoer2 =
                        zufuchua[pHead + 104];
                    // 无磁计量获取
                    Test_linshi_cunchushuju_L.L_wucijiliang =
                        zufuchua[pHead + 105];
                    if (Test_linshi_cunchushuju_L.L_CSQ < 33 &&
                        Test_linshi_cunchushuju_L.L_CSQ > 10 &&
                        Test_liucheng_L == w_shangg_jieguo) {
                      Test_quanju_canshu_L.time_softdelay_ms = 0;
                    }
                    DeBug_print("收到数据\r\n");
                    DeBug_print("当前强磁状态是%d\t,计量1状态是%d\t,"
                                "计量2状态是%d\r\n",
                                Test_linshi_cunchushuju_L.L_huoer1_CHK,
                                Test_linshi_cunchushuju_L.L_jilianghuoer1,
                                Test_linshi_cunchushuju_L.L_jilianghuoer2);
                    // 设置接收标志，通知测试流程已收到测试专用协议数据
                    test_xieyi_jilu_Rec = w_get_test_zhuanyong;
                    // 通知阀门测试模块收到F003响应
                    valve_test_on_response(&g_valve_ctx, 0xF003);
                    // 收到有效数据，立即清除软延时让测试流程处理，并给单步超时一些缓冲
                    Test_quanju_canshu_L.time_softdelay_ms = 0;
                    if (Test_quanju_canshu_L.danbu_chaoshishijian_ms < 3000) {
                      Test_quanju_canshu_L.danbu_chaoshishijian_ms = 3000;
                    }
                    break;
                  default:
                    break;
                  }
                  break;
                  // 写方向
                case 0x83:
                  break;
                  // 写方向
                case 0x84:
                  // 数据标识
                  shujubiaoshi = 0;
                  shujubiaoshi = zufuchua[pHead + 13];
                  shujubiaoshi = shujubiaoshi << 8;
                  shujubiaoshi += zufuchua[pHead + 12];
                  // 通知阀门测试模块收到响应
                  DeBug_print("收到写响应0x84, 数据标识=0x%04X\r\n",
                              shujubiaoshi);
                  valve_test_on_response(&g_valve_ctx, shujubiaoshi);
                  // pHead+15
                  switch (shujubiaoshi) {
                  // 阀门协议
                  case 0xC022:
                    if (zufuchua[pHead + 15] == 0x00) {
                      test_xieyi_jilu_Rec = w_get_famen_dongzuo;
                      DeBug_print(
                          "收到设置阀门协议的响应，即将解析阀门的电压\r\n");
                    }
                    break;
                  case 0x2604:
                    if (zufuchua[pHead + 14] == 0x00) {
                      test_xieyi_jilu_Rec = w_set_famen;
                      Test_quanju_canshu_L.time_softdelay_ms = 0;
                    }
                    break;
                  case 0x2636:
                    if (zufuchua[pHead + 14] == 0x00) {
                      test_xieyi_jilu_Rec = w_set_famen;
                    }
                    break;
                  default:
                    break;
                  }
                  break;
                // 远程
                case 0x86:
                  break;
                default:
                  break;
                }
                break;
              default:
                break;
              }
              break;
            default:
              break;
            }
            // 协议完成后，将同一帧的一个协议
            pHead += zhen_lenth;
          }
        }
      }
    }
    pHead++;
  }
}
// 数据标识 控制码 表号 数据域长度
void TONGXIN_xieyifasong(uint16_t shujubiaoshi, uint8_t kongzhima,
                         uint8_t w_biaohao[], uint8_t shujuyu[],
                         uint16_t shujuyu_changdu) {
  uint16_t xieyi_zongchangdu = 0;
  uint16_t CRCjianyan = 0;
  // 帧开始
  xieyi_zuzhen[xieyi_zongchangdu++] = 0x68;
  // 表号
  xieyi_zuzhen[xieyi_zongchangdu++] = w_biaohao[0];
  xieyi_zuzhen[xieyi_zongchangdu++] = w_biaohao[1];
  xieyi_zuzhen[xieyi_zongchangdu++] = w_biaohao[2];
  xieyi_zuzhen[xieyi_zongchangdu++] = w_biaohao[3];
  xieyi_zuzhen[xieyi_zongchangdu++] = w_biaohao[4];
  xieyi_zuzhen[xieyi_zongchangdu++] = w_biaohao[5];
  // 协议类型
  xieyi_zuzhen[xieyi_zongchangdu++] = 0x00;
  // 协议版本号
  xieyi_zuzhen[xieyi_zongchangdu++] = WaterMeterProtocol_VERSION_NUMBER;
  // 控制码
  xieyi_zuzhen[xieyi_zongchangdu++] = kongzhima;
  // 帧长
  xieyi_zuzhen[xieyi_zongchangdu++] = (shujuyu_changdu + 18) & 0xFF;
  xieyi_zuzhen[xieyi_zongchangdu++] = ((shujuyu_changdu + 18) >> 8) & 0xFF;
  // 数据标识
  xieyi_zuzhen[xieyi_zongchangdu++] = (shujubiaoshi & 0xFF);
  xieyi_zuzhen[xieyi_zongchangdu++] = ((shujubiaoshi >> 8) & 0xFF);
  // 信息号，远程0
  xieyi_zuzhen[xieyi_zongchangdu++] = 0x00;
  // 数据域
  memcpy(&xieyi_zuzhen[xieyi_zongchangdu], shujuyu, shujuyu_changdu);
  xieyi_zongchangdu += shujuyu_changdu;
  // 校验码
  CRCjianyan = user_cmd_get_crc(xieyi_zuzhen, xieyi_zongchangdu);
  xieyi_zuzhen[xieyi_zongchangdu++] = CRCjianyan & 0xFF;
  xieyi_zuzhen[xieyi_zongchangdu++] = (CRCjianyan >> 8) & 0xFF;
  // 帧结束
  xieyi_zuzhen[xieyi_zongchangdu++] = 0x16;
  Uart1_Tx_Send_ok(xieyi_zuzhen, xieyi_zongchangdu);
  DeBug_print("发送给水表的协议是：\r\n");
  protocol_debug_print(xieyi_zuzhen, xieyi_zongchangdu);
}
// 读表号
void find_biaohao_xieyi() {
  TONGXIN_xieyifasong(0x2031, 0x02, moren_biaohao, kongshujuyu, 0);
}

/**
 * @brief  读取当前水表用水量协议
 *
 *@note  该协议用于获取水表的当前用水量信息，便于监测和记录用户的用水情况。
 */
void read_current_water_usage_protocol() {
  TONGXIN_xieyifasong(0x90FF, 0x02, Test_jiejuo_jilu.dangqian_biaohao,
                      kongshujuyu, 0);
}

// 重置系统为无阀状态，方便测试地功耗，否则有的可能测试第一次测到后面进行上告，然后不成功，但是是有阀门的状态，结果导致下一次测试功耗时间增加
void reset_water_meter_to_none_valve() {
  uint8_t none_valve_data[] = {
      0x0F, // 管径低位字节
      0x00, // 管径高位字节
      0x00, // 无阀门类型
      0x14, // 超时时间20s
      0xE6, // 堵转电流低位字节
      0x00 // 堵转电流i高位字节,当前堵转电流是230mA，由于实际没有接负载，当前永远不会堵转，只会超时
  };
  TONGXIN_xieyifasong(0xC022, 0x04, moren_biaohao, none_valve_data, 6);
}

/**
 * @brief  配置阀门相关参数（仅适用于机械表阀门，超声波表未验证）
 *
 * @param pipe_diameter_mm   阀门管径（单位：mm），常见值：15、20、25、32
 * @param valve_type         阀门类型：
 *                           - 0: 无阀门
 *                           - 1: 5线阀
 *                           - 2: 2线阀
 * @param timeout_seconds    阀门动作超时时间（单位：秒），建议值：12~20
 * @param stall_current_ma   堵转电流阈值（单位：mA），典型值：230
 *
 * @note   数据按小端格式发送（低位在前，高位在后）
 *
 * @example 配置为无阀门状态（用于测试低功耗）：
 *          configure_valve_parameters(15, 0, 20, 230);
 *          // 15mm管径, 无阀门, 20秒超时, 230mA堵转电流
 */
void configure_valve_parameters(uint16_t pipe_diameter_mm, uint8_t valve_type,
                                uint8_t timeout_seconds,
                                uint16_t stall_current_ma) {
  uint8_t valve_config_data[] = {
      (uint8_t)(pipe_diameter_mm & 0xFF),        // 管径低位字节
      (uint8_t)((pipe_diameter_mm >> 8) & 0xFF), // 管径高位字节
      valve_type,                                // 阀门类型
      timeout_seconds,                           // 超时时间（秒）
      (uint8_t)(stall_current_ma & 0xFF),        // 堵转电流低位字节
      (uint8_t)((stall_current_ma >> 8) & 0xFF)  // 堵转电流高位字节
  };
  TONGXIN_xieyifasong(0x2604, 0x04, Test_jiejuo_jilu.dangqian_biaohao,
                      valve_config_data, 6);
}
// 上告
void send_shanggao_xieyi() {
  TONGXIN_xieyifasong(0xF001, 0x02, Test_jiejuo_jilu.dangqian_biaohao,
                      kongshujuyu, 0);
}
// 阀门协议 - 开阀
void FM_Open_xieyi() {
  uint8_t shujuyu[] = {0x1C}; // 0x1C表示开阀
  TONGXIN_xieyifasong(0xC022, 0x04, Test_jiejuo_jilu.dangqian_biaohao, shujuyu,
                      1);
}

// 阀门协议 - 关阀
void FM_Close_xieyi() {
  uint8_t shujuyu[] = {0x1A}; // 0x1A表示关阀
  TONGXIN_xieyifasong(0xC022, 0x04, Test_jiejuo_jilu.dangqian_biaohao, shujuyu,
                      1);
}

// 测试协议
void Test_zhuanyong_xieyi() {
  TONGXIN_xieyifasong(0xF003, 0x02, Test_jiejuo_jilu.dangqian_biaohao,
                      kongshujuyu, 0);
}

// 到位状态读取协议
void valve_position_status_read_protocol(void) {
  TONGXIN_xieyifasong(0xF003, 0x02, Test_jiejuo_jilu.dangqian_biaohao,
                      kongshujuyu, 0);
}
// 设置协议
void set_famen_xieyi() {
  Test_jiejuo_jilu.jixiebiao_canshu[3] = 0x0C; // 超时时间12s
  if (Test_jiejuo_jilu.biaoju_leixing_famen == 0) {
    // Test_jiejuo_jilu.jixiebiao_canshu[0] = 0x0F; //阀发管径
    // Test_jiejuo_jilu.jixiebiao_canshu[1] = 0x00; //
    // Test_jiejuo_jilu.jixiebiao_canshu[2] = 0x02; //阀门类型 2线阀门
    // Test_jiejuo_jilu.jixiebiao_canshu[3] = 0x0C; //超时时间12s
    // Test_jiejuo_jilu.jixiebiao_canshu[4] = 0xE6; //堵转电流
    // Test_jiejuo_jilu.jixiebiao_canshu[5] = 0x00; //
    DeBug_print("发送阀门开关协议-机械表\r\n");
    TONGXIN_xieyifasong(0x2604, 0x04, Test_jiejuo_jilu.dangqian_biaohao,
                        Test_jiejuo_jilu.jixiebiao_canshu, 6);
  } else if (Test_jiejuo_jilu.biaoju_leixing_famen == 1) {
    DeBug_print("发送阀门开关协议-超声波表\r\n");
    TONGXIN_xieyifasong(0x2636, 0x04, Test_jiejuo_jilu.dangqian_biaohao,
                        Test_jiejuo_jilu.chaoshengbo_canshu, 6);
  }
}

// 读版本号
void find_banbenhao_xieyi() {
  TONGXIN_xieyifasong(0x2011, 0x02, Test_jiejuo_jilu.dangqian_biaohao,
                      kongshujuyu, 0);
}

// 使用结构体配置机械表阀门
void configure_mechanical_valve(const void *config) {
  const MechanicalValveConfig *cfg = (const MechanicalValveConfig *)config;
  uint8_t valve_config_data[] = {
      (uint8_t)(cfg->pipe_diameter_mm & 0xFF),        // 管径低位字节
      (uint8_t)((cfg->pipe_diameter_mm >> 8) & 0xFF), // 管径高位字节
      cfg->valve_type,                                // 阀门类型
      cfg->timeout_seconds,                           // 超时时间（秒）
      (uint8_t)(cfg->stall_current_ma & 0xFF),       // 堵转电流低位字节
      (uint8_t)((cfg->stall_current_ma >> 8) & 0xFF) // 堵转电流高位字节
  };
  DeBug_print(
      "发送机械表阀门配置: 管径=%dmm, 类型=%d, 超时=%ds, 堵转电流=%dmA\r\n",
      cfg->pipe_diameter_mm, cfg->valve_type, cfg->timeout_seconds,
      cfg->stall_current_ma);
  TONGXIN_xieyifasong(0x2604, 0x04, Test_jiejuo_jilu.dangqian_biaohao,
                      valve_config_data, 6);
}

// 使用结构体配置超声波表阀门
void configure_ultrasonic_valve(const void *config) {
  const UltrasonicValveConfig *cfg = (const UltrasonicValveConfig *)config;
  uint8_t valve_config_data[] = {
      (uint8_t)(cfg->pipe_diameter_mm & 0xFF),        // 管径低位字节
      (uint8_t)((cfg->pipe_diameter_mm >> 8) & 0xFF), // 管径高位字节
      cfg->valve_type,                                // 阀门类型
      cfg->timeout_seconds,                           // 超时时间（秒）
      (uint8_t)(cfg->stall_current_ma & 0xFF),       // 堵转电流低位字节
      (uint8_t)((cfg->stall_current_ma >> 8) & 0xFF) // 堵转电流高位字节
  };
  DeBug_print(
      "发送超声波表阀门配置: 管径=%dmm, 类型=%d, 超时=%ds, 堵转电流=%dmA\r\n",
      cfg->pipe_diameter_mm, cfg->valve_type, cfg->timeout_seconds,
      cfg->stall_current_ma);
  TONGXIN_xieyifasong(0x2036, 0x04, Test_jiejuo_jilu.dangqian_biaohao,
                      valve_config_data, 6);
}
