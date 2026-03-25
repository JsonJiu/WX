#define LOG_TAG "meter"
#include "tongxin_xieyi_Ctrl.h"
#include "GPIO.h"
#include "Test_List.h"
#include "uart0.h"
#include "uart1.h"
#include "uart3.h"
#include "utility.h"
#include "valve_ctrl.h"
#include <elog.h>

// 协议版本定义
#define PROTOCOL_VERSION_OLD 0x00                     // 旧版本协议
#define PROTOCOL_VERSION_NEW 0x0A                     // 新版本协议
#define PROTOCOL_VERSION_CURRENT PROTOCOL_VERSION_NEW // 当前使用的版本

// ��������ź���ͨ��

uint8_t xieyi_zuzhen[200];
uint8_t kongshujuyu[1];
uint8_t moren_biaohao[6] = {0xAA, 0xAA, 0xAA, 0xAA, 0xAA, 0xAA};
uint8_t water_temperture[2] = {0, 0}; // 温度值 两个字节 ，初始化为0
// protocol_response_param 已在 main.c 中定义
extern uint32_t protocol_response_param;
// extern uint8_t qiandao[1210];
//  ����д�Ƚ��ַ�����Ҫ��ԭ���Ƚϻ�ʹ\0���ǽ�������
uint8_t bijiao_zifuchuan(uint8_t bijiao1[], uint8_t bijiao2[], uint16_t lenth) {
  uint16_t bijiao_lenth = 0;
  for (bijiao_lenth = 0; bijiao_lenth < lenth; bijiao_lenth++) {
    if (bijiao1[bijiao_lenth] != bijiao2[bijiao_lenth]) {
      return 0;
    }
  }
  return 1;
}

// ��ͨ�ŵ�ַ  2031H  C=82H  ��������
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
  log_d("开始解析协议，数据长度=%d", lenth);
  log_d("收到的水表协议内容是：");
  protocol_debug_print(zufuchua, lenth);
  while (1) {
    if (pHead + 17 > lenth) {
      break;
    }
    if (zufuchua[pHead] == 0x68) {
      zhen_lenth = 0;
      zhen_lenth = zufuchua[pHead + 11];
      zhen_lenth = zhen_lenth << 8;
      zhen_lenth += zufuchua[pHead + 10];
      if (pHead + zhen_lenth <= lenth) {
        if (zufuchua[pHead + zhen_lenth - 1] == 0x16) {
          CRC_jiaoyan = 0;
          CRC_jiaoyan = zufuchua[pHead + zhen_lenth - 2];
          CRC_jiaoyan = CRC_jiaoyan << 8;
          CRC_jiaoyan += zufuchua[pHead + zhen_lenth - 3];
          CRC_jiaoyan_jisuan =
              util_crc16_ccitt(&zufuchua[pHead], zhen_lenth - 3);
          if (CRC_jiaoyan == CRC_jiaoyan_jisuan) {
            memcpy(Test_jiejuo_jilu.dangqian_biaohao, &zufuchua[pHead + 1], 6);
            // log_d("获取到水表表号: ");
            // protocol_debug_print(Test_jiejuo_jilu.dangqian_biaohao, 6);
            switch (zufuchua[pHead + 7]) {
            case 0x00:
              switch (zufuchua[pHead + 8]) {
              case PROTOCOL_VERSION_OLD: // 支持旧版本0x00
              case PROTOCOL_VERSION_NEW: // 支持新版本0x0A
                switch (zufuchua[pHead + 9]) {
                case 0x81:
                  break;
                case 0x82:
                  shujubiaoshi = 0;
                  shujubiaoshi = zufuchua[pHead + 13];
                  shujubiaoshi = shujubiaoshi << 8;
                  shujubiaoshi += zufuchua[pHead + 12];
                  switch (shujubiaoshi) {
                  case 0x2031:
                    Test_quanju_canshu_L.time_softdelay_ms = 0;
                    test_xieyi_jilu_Rec = w_get_biaohao;
                    break;
                  case 0xF001:
                    Test_quanju_canshu_L.time_softdelay_ms = 0;
                    test_xieyi_jilu_Rec = w_get_shanggao;
                    break;
                  case 0x2011: {
                    // 防止死循环: 限制最大解析长度
                    uint16_t max_data_len =
                        (zhen_lenth > 20) ? (zhen_lenth - 20) : 0;
                    while (jiexi_changdu < max_data_len) {
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
                  } break;
                  case 0x9010: // 读取累计流量响应
                    if (zufuchua[pHead + 14] == 0x00) {
                      log_d("读取累计流量得到响应");
                      memcpy(Test_linshi_cunchushuju_L
                                 .L_ultrasonic_accumulated_flux,
                             &zufuchua[pHead + 15], 4);
                      test_xieyi_jilu_Rec = w_get_accumulated_flux;
                    } else {
                      log_d("读取累计流量响应异常");
                    }
                    break;
                  case 0xF003:
                    Test_linshi_cunchushuju_L.L_flash_chk =
                        zufuchua[pHead + 17];
                    Test_linshi_cunchushuju_L.L_zhudian_dianya =
                        zufuchua[pHead + 19];
                    Test_linshi_cunchushuju_L.L_zhudian_dianya =
                        Test_linshi_cunchushuju_L.L_zhudian_dianya << 8;
                    Test_linshi_cunchushuju_L.L_zhudian_dianya +=
                        zufuchua[pHead + 18];
                    // ����AD�������
                    Test_linshi_cunchushuju_L.L_beidian_dianya =
                        zufuchua[pHead + 21];
                    Test_linshi_cunchushuju_L.L_beidian_dianya =
                        Test_linshi_cunchushuju_L.L_beidian_dianya << 8;
                    Test_linshi_cunchushuju_L.L_beidian_dianya +=
                        zufuchua[pHead + 20];
                    // ѹ����������ȡ
                    Test_linshi_cunchushuju_L.L_yali_CHK = zufuchua[pHead + 22];
                    // EEPROM״̬��ȡ
                    Test_linshi_cunchushuju_L.L_EEPROM_CHK =
                        zufuchua[pHead + 23];
                    // ����1״̬��ȡ
                    Test_linshi_cunchushuju_L.L_huoer1_CHK =
                        zufuchua[pHead + 24];
                    // GPRS״̬��ȡ
                    Test_linshi_cunchushuju_L.L_GPRS_mozu_CHK =
                        zufuchua[pHead + 25];
                    // ����2״̬��ȡ
                    Test_linshi_cunchushuju_L.L_huoer2_CHK =
                        zufuchua[pHead + 26];
                    // ����˲ʱ������ȡ
                    memcpy(
                        Test_linshi_cunchushuju_L.L_chaosheng_shunshiliuliang,
                        &zufuchua[pHead + 27], 4);
                    // ��ˮ״̬��ȡ
                    Test_linshi_cunchushuju_L.L_chaosheng_zousuizhuangtai =
                        zufuchua[pHead + 31];
                    // GP30״̬��ȡ
                    Test_linshi_cunchushuju_L.L_GP30mozu_AD_CHK =
                        zufuchua[pHead + 33];
                    Test_linshi_cunchushuju_L.L_GP30mozu_AD_CHK =
                        Test_linshi_cunchushuju_L.L_GP30mozu_AD_CHK << 8;
                    Test_linshi_cunchushuju_L.L_GP30mozu_AD_CHK +=
                        zufuchua[pHead + 32];
                    // IMEI�Ż�ȡ
                    memcpy(Test_linshi_cunchushuju_L.L_IMEI,
                           &zufuchua[pHead + 34], 15);
                    // IMSI�Ż�ȡ
                    memcpy(Test_linshi_cunchushuju_L.L_IMSI,
                           &zufuchua[pHead + 49], 15);
                    // ICCID�Ż�ȡ
                    memcpy(Test_linshi_cunchushuju_L.L_ICCID,
                           &zufuchua[pHead + 64], 20);
                    // CSQ��ȡ
                    Test_linshi_cunchushuju_L.L_CSQ = zufuchua[pHead + 84];
                    // lorakey(�������һЩ���⣬Ŀǰֻ��һλ)
                    memcpy(Test_linshi_cunchushuju_L.lorakey,
                           &zufuchua[pHead + 85], 16);
                    // ��λ1�ź�
                    Test_linshi_cunchushuju_L.L_daowei1 = zufuchua[pHead + 101];
                    // ��λ2�ź�
                    Test_linshi_cunchushuju_L.L_daowei2 = zufuchua[pHead + 102];
                    // ��������1�źŻ�ȡ
                    Test_linshi_cunchushuju_L.L_jilianghuoer1 =
                        zufuchua[pHead + 103];
                    // ��������2�źŻ�ȡ
                    Test_linshi_cunchushuju_L.L_jilianghuoer2 =
                        zufuchua[pHead + 104];
                    // 无磁计量结果
                    Test_linshi_cunchushuju_L.L_wucijiliang =
                        zufuchua[pHead + 105];
                    // 温度结果，设置了10K电阻，默认25度
                    // 添加校验码，国内默认使用0x0000,两个字节
                    Test_linshi_cunchushuju_L.L_banbenhao[0] =
                        zufuchua[pHead + 106];
                    Test_linshi_cunchushuju_L.L_banbenhao[1] =
                        zufuchua[pHead + 107];
                    Test_linshi_cunchushuju_L.L_Lora_rssi[0] =
                        zufuchua[pHead + 108]; // 低字节, 说明：FF B9 代表-71 非
                                               //  lora 默认 0

                    Test_linshi_cunchushuju_L.L_Lora_rssi[1] =
                        zufuchua[pHead + 109]; // 高字节
                    Test_linshi_cunchushuju_L.L_lora_isnr[0] =
                        zufuchua[pHead + 110]; // 低字节，说明：00 05 代表 5 非
                                               //  lora 默认 0
                    Test_linshi_cunchushuju_L.L_lora_isnr[1] =
                        zufuchua[pHead + 111]; // 高字节
                    Test_linshi_cunchushuju_L.L_water_temperature[0] =
                        zufuchua[pHead + 112]; // 低字节, 默认是0x C8
                    Test_linshi_cunchushuju_L.L_water_temperature[1] =
                        zufuchua[pHead + 113]; // 高字节 默认00
                    log_d("当前温度是：0x%02X 0x%02X",
                          Test_linshi_cunchushuju_L.L_water_temperature[1],
                          Test_linshi_cunchushuju_L.L_water_temperature[0]);
                    if (Test_linshi_cunchushuju_L.L_CSQ < 33 &&
                        Test_linshi_cunchushuju_L.L_CSQ > 5) {
                      Test_quanju_canshu_L.time_softdelay_ms = 0;
                      // test_xieyi_jilu_Rec = w_get_csq;
                    }
                    log_d("已收到查询详细信息");
                    // 通知阀门测试模块收到F003响应
                    ValveCtrl_OnResponse(0xF003);
                    test_xieyi_jilu_Rec = w_get_test_zhuanyong;
                    break;
                  default:
                    break;
                  }
                  break;
                // ����������
                case 0x83:
                  break;
                // д����
                case 0x84:
                  // �������ݱ�ʶ����
                  shujubiaoshi = 0;
                  shujubiaoshi = zufuchua[pHead + 13];
                  shujubiaoshi = shujubiaoshi << 8;
                  shujubiaoshi += zufuchua[pHead + 12];
                  // pHead+15
                  // 协议响应参数赋值,当前只有阀门独立测试需要，其他协议暂时不需要
                  protocol_response_param = shujubiaoshi;
                  // 通知阀门测试模块收到响应
                  ValveCtrl_OnResponse(shujubiaoshi);
                  switch (shujubiaoshi) {
                  case 0xC021:
                    if (zufuchua[pHead + 15] == 0x00) {
                      log_d("收到0xC021响应, pHead+9=0x%02X",
                            zufuchua[pHead + 9]);
                      test_xieyi_jilu_Rec = w_set_accumulated_flux;
                    } else {
                      log_d("设置累计流量失败，错误码：0x%02X",
                            zufuchua[pHead + 15]);
                    }
                    break;
                  case 0xC022:
                    log_d("收到0xC022响应, pHead+9=0x%02X",
                          zufuchua[pHead + 9]);
                    if (zufuchua[pHead + 15] == 0x00) {
                      test_xieyi_jilu_Rec = w_get_famen_dongzuo;
                      if (Test_liucheng_L == w_close_valve_wait_response) {
                        // 软件超时清0，收到了响应
                        Test_quanju_canshu_L.time_softdelay_ms = 0;
                      }
                      log_d("已设置test_xieyi_jilu_Rec=w_get_famen_dongzuo");
                    } else {
                      log_d("阀门动作失败，错误码：0x%02X 0x%02X",
                            zufuchua[pHead + 15], zufuchua[pHead + 16]);
                    }
                    break;
                  case 0x2604:
                    if (zufuchua[pHead + 15] == 0x00) {
                      test_xieyi_jilu_Rec = w_set_famen;
                    }
                    break;
                  case 0x2636:
                    if (zufuchua[pHead + 15] == 0x00) {
                      test_xieyi_jilu_Rec = w_set_famen;
                    }
                    break;
                  case 0x2036:
                    // 超声波表阀门配置响应
                    if (zufuchua[pHead + 15] == 0x00) {
                      log_d("超声波表阀门配置成功");
                      test_xieyi_jilu_Rec = w_set_famen;
                    } else {
                      log_d("超声波表阀门配置失败，错误码：0x%02X",
                            zufuchua[pHead + 15]);
                    }
                    break;
                  case 0x2B00:
                    if (zufuchua[pHead + 15] == 0x00) {
                      test_xieyi_jilu_Rec = w_set_water_temp;
                      break;
                    }
                    break;
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
              // 协议解析完成后，跳到同一帧的下一个协议数据
              pHead += zhen_lenth;
              continue; // 跳过末尾的pHead++，避免跳过下一帧的帧头0x68
            }
          }
        }
      }
      pHead++;
    } else {
      pHead++;
    }
  }
}

// 数据标识 控制码 表号 数据域长度
// 帧长度总长度
// 固定帧开销: 帧头(1) + 表号(6) + 协议类型(1) + 版本(1) + 控制码(1) +
//            帧长度(2) + 数据标识(2) + 消息序号(1) + CRC(2) + 帧尾(1) = 18字节
#define FRAME_OVERHEAD 18
#define MAX_DATA_LEN (sizeof(xieyi_zuzhen) - FRAME_OVERHEAD)

void TONGXIN_xieyifasong(uint16_t shujubiaoshi, uint8_t kongzhima,
                         uint8_t w_biaohao[], uint8_t shujuyu[],
                         uint16_t shujuyu_changdu) {
  // 缓冲区溢出保护
  if (shujuyu_changdu > MAX_DATA_LEN) {
    log_e("数据域长度超限: %d > %d", shujuyu_changdu, (int)MAX_DATA_LEN);
    return;
  }

  uint16_t xieyi_zongchangdu = 0;
  uint16_t CRCjianyan = 0;
  // 帧开始标识
  xieyi_zuzhen[xieyi_zongchangdu++] = 0x68;
  // 表具标识
  xieyi_zuzhen[xieyi_zongchangdu++] = w_biaohao[0];
  xieyi_zuzhen[xieyi_zongchangdu++] = w_biaohao[1];
  xieyi_zuzhen[xieyi_zongchangdu++] = w_biaohao[2];
  xieyi_zuzhen[xieyi_zongchangdu++] = w_biaohao[3];
  xieyi_zuzhen[xieyi_zongchangdu++] = w_biaohao[4];
  xieyi_zuzhen[xieyi_zongchangdu++] = w_biaohao[5];
  // 协议类型，默认00
  xieyi_zuzhen[xieyi_zongchangdu++] = 0x00;
  // 协议版本号，使用宏定义统一管理,有可能是00，有可能是别的
  xieyi_zuzhen[xieyi_zongchangdu++] = PROTOCOL_VERSION_CURRENT;
  // 协议控制码
  xieyi_zuzhen[xieyi_zongchangdu++] = kongzhima;
  // 帧长度,小端序发送
  xieyi_zuzhen[xieyi_zongchangdu++] = (shujuyu_changdu + 18) & 0xFF;
  xieyi_zuzhen[xieyi_zongchangdu++] = ((shujuyu_changdu + 18) >> 8) & 0xFF;
  // 数据标识，小端序发送
  xieyi_zuzhen[xieyi_zongchangdu++] = (shujubiaoshi & 0xFF);
  xieyi_zuzhen[xieyi_zongchangdu++] = ((shujubiaoshi >> 8) & 0xFF);
  // 消息序号，默认是0
  xieyi_zuzhen[xieyi_zongchangdu++] = 0x00;
  // 数据域
  memcpy(&xieyi_zuzhen[xieyi_zongchangdu], shujuyu, shujuyu_changdu);
  xieyi_zongchangdu += shujuyu_changdu;
  // CRC校验
  CRCjianyan = util_crc16_ccitt(xieyi_zuzhen, xieyi_zongchangdu);
  xieyi_zuzhen[xieyi_zongchangdu++] = CRCjianyan & 0xFF;
  xieyi_zuzhen[xieyi_zongchangdu++] = (CRCjianyan >> 8) & 0xFF;
  // 帧尾
  xieyi_zuzhen[xieyi_zongchangdu++] = 0x16;
  Uart1_Tx_Send_ok(xieyi_zuzhen, xieyi_zongchangdu);
  log_d("当前发送给水表的协议是：");
  protocol_debug_print(xieyi_zuzhen, xieyi_zongchangdu);
}

// 获取当前表号
void find_biaohao_xieyi() {

  log_d("查询表号指令");
  TONGXIN_xieyifasong(0x2031, 0x02, moren_biaohao, kongshujuyu, 0);
}

// 上告协议
void send_shanggao_xieyi() {
  TONGXIN_xieyifasong(0xF001, 0x02, Test_jiejuo_jilu.dangqian_biaohao,
                      kongshujuyu, 0);
}

// 阀门动作协议
void FM_Open_xieyi() {
  uint8_t shujuyu[] = {0x1C}; // 开阀
  TONGXIN_xieyifasong(0xC022, 0x04, Test_jiejuo_jilu.dangqian_biaohao, shujuyu,
                      1);
}

// 关阀门
void FM_Close_xieyi() {
  uint8_t shujuyu[] = {0x1A}; // 关阀 (0x1A)
  TONGXIN_xieyifasong(0xC022, 0x04, Test_jiejuo_jilu.dangqian_biaohao, shujuyu,
                      1);
}

// 测试专用协议
void Test_zhuanyong_xieyi() {
  TONGXIN_xieyifasong(0xF003, 0x02, Test_jiejuo_jilu.dangqian_biaohao,
                      kongshujuyu, 0);
}

// 查询版本号协议
void find_banbenhao_xieyi() {
  TONGXIN_xieyifasong(0x2011, 0x02, Test_jiejuo_jilu.dangqian_biaohao,
                      kongshujuyu, 0);
}

// 设置阀门协议（使用结构体配置）
void set_famen_xieyi() {
  if (Test_jiejuo_jilu.biaoju_leixing_famen == 0) {
    // 机械表：发送 0x2604 配置命令
    configure_mechanical_valve(&Test_jiejuo_jilu.mechanical_config);
  } else if (Test_jiejuo_jilu.biaoju_leixing_famen == 1) {
    // 超声波表：发送 0x2036 配置命令
    configure_ultrasonic_valve(&Test_jiejuo_jilu.ultrasonic_config);
  }
}

// 设置开启水温检测
void set_water_temp(void) {
  uint16_t shujuyu[] = {0x0002};
  TONGXIN_xieyifasong(0x2B00, 0x04, Test_jiejuo_jilu.dangqian_biaohao,
                      (uint8_t *)shujuyu, 2);
}

// 写入水表底数，也就是写入水表当前的走水流量
// 示例写入0： 68 00 00 00 00 00 00 00 0A 04 16 00 21 C0 00 00 00 00 00 29 FF
// 16 rx:68 00 00 00 00 00 00 00 0A 84 14 00 21 C0 00 00 00 9E 39 16
//  写入12：68 00 00 00 00 00 00 00 0A 04 16 00 21 C0 00 E0 2E 00 00 04 E6 16
// rx:68 00 00 00 00 00 00 00 0A 84 14 00 21 C0 00 00 00 9E 39 16
// 写入1200： 68 00 00 00 00 00 00 00 0A 04 16 00 21 C0 00 80 4F 12 00 9C 76
// 16 rx:68 00 00 00 00 00 00 00 0A 84 14 00 21 C0 00 00 00 9E 39 16
// 低三位是小数值，5个字节数据域,数据标识c021H
void write_water_flux_data(uint8_t flux_data[5]) {
  TONGXIN_xieyifasong(0xC021, 0x04, Test_jiejuo_jilu.dangqian_biaohao,
                      flux_data, 5);
}
// 重置累计流量协议
void reset_accumulated_flux_protocol(void) {
  write_water_flux_data((uint8_t[]){0x00, 0x00, 0x00, 0x00, 0x00});
}

// 读取累计流量
// 示例：tx  68 00 00 00 00 00 00 00 0A 02 12 00 10 90 00 07 F3 16
// rx:68 00 00 00 00 00 00 00 0A 82 18 00 10 90 00 00 00 19 52 12 00 7B 2D 16
// 数据标识：9010H
// 数据格式U32*10-3，单位立方米，功能读取累计流量
void read_accumulated_flux_protocol(void) {
  TONGXIN_xieyifasong(0x9010, 0x02, Test_jiejuo_jilu.dangqian_biaohao,
                      kongshujuyu, 0);
}
