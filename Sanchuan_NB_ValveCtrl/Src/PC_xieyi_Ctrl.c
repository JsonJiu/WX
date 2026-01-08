#include "PC_xieyi_Ctrl.h"
#include "GPIO.h"
#include "Test_List.h"
#include "uart0.h"
#include "uart1.h"
#define send_lenth 200
uint8_t xieyi1_fanhui[6] = {0x68, 0xAB, 0x06, 0x00, 0x19, 0x16};
uint8_t xieyi2_fanhui[send_lenth];
extern volatile uint8_t PassThrough_Mode;
// 开始测试设置成功后发送
void PC_xieyifasong_1() {
  if (PassThrough_Mode) {
    // 透传模式下不发送测试开始确认
    return;
  }
  xieyi1_fanhui[3] = Test_jiejuo_jilu.gongwei;
  xieyi1_fanhui[4] = 0x19 + xieyi1_fanhui[3];

  // 调试模式下打印协议数据
  if (Debug_Mode) {
    DeBug_print("确认测试开始: ");
    protocol_debug_print(xieyi1_fanhui, 6);
  }

  // 无论是否调试模式，都要发送数据给 PC
  PC_Chuankou_tongxin_send(xieyi1_fanhui, 6);
}
// 查询结果
void PC_xieyifasong_2() {
  uint16_t jishu_lenth = 0;
  uint16_t hejiaoyan = 0;
  memset(xieyi2_fanhui, 0x00, send_lenth);
  xieyi2_fanhui[jishu_lenth++] = 0x68;
  xieyi2_fanhui[jishu_lenth++] = 0xAD;
  xieyi2_fanhui[jishu_lenth++] = 0x00;
  xieyi2_fanhui[jishu_lenth++] = Test_jiejuo_jilu.gongwei;
  xieyi2_fanhui[jishu_lenth++] =
      (Test_jiejuo_jilu.zhidian_dianya_gongdian) & 0xFF;
  xieyi2_fanhui[jishu_lenth++] =
      ((Test_jiejuo_jilu.zhidian_dianya_gongdian) >> 8) & 0xFF;
  xieyi2_fanhui[jishu_lenth++] = (Test_jiejuo_jilu.zhidian_dianya_huoqu) & 0xFF;
  xieyi2_fanhui[jishu_lenth++] =
      ((Test_jiejuo_jilu.zhidian_dianya_huoqu) >> 8) & 0xFF;
  xieyi2_fanhui[jishu_lenth++] =
      (Test_jiejuo_jilu.zhidian_jingtai_gonghao) & 0xFF;
  xieyi2_fanhui[jishu_lenth++] =
      ((Test_jiejuo_jilu.zhidian_jingtai_gonghao) >> 8) & 0xFF;

  xieyi2_fanhui[jishu_lenth++] =
      (Test_jiejuo_jilu.zhidian_manshui_gonghao) & 0xFF;
  xieyi2_fanhui[jishu_lenth++] =
      ((Test_jiejuo_jilu.zhidian_manshui_gonghao) >> 8) & 0xFF;

  xieyi2_fanhui[jishu_lenth++] =
      (Test_jiejuo_jilu.zhidian_zoushui_gonghao) & 0xFF;
  xieyi2_fanhui[jishu_lenth++] =
      ((Test_jiejuo_jilu.zhidian_zoushui_gonghao) >> 8) & 0xFF;

  xieyi2_fanhui[jishu_lenth++] =
      (Test_jiejuo_jilu.beidian_dianya_gongdian) & 0xFF;
  xieyi2_fanhui[jishu_lenth++] =
      ((Test_jiejuo_jilu.beidian_dianya_gongdian) >> 8) & 0xFF;
  Test_jiejuo_jilu.beidian_dianya_huoqu = 3600;
  xieyi2_fanhui[jishu_lenth++] = (Test_jiejuo_jilu.beidian_dianya_huoqu) & 0xFF;
  xieyi2_fanhui[jishu_lenth++] =
      ((Test_jiejuo_jilu.beidian_dianya_huoqu) >> 8) & 0xFF;

  xieyi2_fanhui[jishu_lenth++] = (Test_jiejuo_jilu.beidian_gonghao) & 0xFF;
  xieyi2_fanhui[jishu_lenth++] =
      ((Test_jiejuo_jilu.beidian_gonghao) >> 8) & 0xFF;

  xieyi2_fanhui[jishu_lenth++] = Test_jiejuo_jilu.lanya_jiance;
  xieyi2_fanhui[jishu_lenth++] = Test_jiejuo_jilu.flash_jiance;
  xieyi2_fanhui[jishu_lenth++] = Test_jiejuo_jilu.jiliang_jiance;
  xieyi2_fanhui[jishu_lenth++] = Test_jiejuo_jilu.hongwai_jiance;
  memcpy(&xieyi2_fanhui[jishu_lenth], Test_jiejuo_jilu.IMEI_CHK, 15);
  jishu_lenth += 15;
  memcpy(&xieyi2_fanhui[jishu_lenth], Test_jiejuo_jilu.IMSI_CHK, 15);
  jishu_lenth += 15;
  memcpy(&xieyi2_fanhui[jishu_lenth], Test_jiejuo_jilu.ICCID_CHK, 20);
  jishu_lenth += 20;
  xieyi2_fanhui[jishu_lenth++] = Test_jiejuo_jilu.CSQ;
  xieyi2_fanhui[jishu_lenth++] = Test_jiejuo_jilu.FM;
  Test_jiejuo_jilu.FM_daowei = 1;
  xieyi2_fanhui[jishu_lenth++] = Test_jiejuo_jilu.FM_daowei;
  xieyi2_fanhui[jishu_lenth++] = Test_jiejuo_jilu.EEPROM_jiance;
  xieyi2_fanhui[jishu_lenth++] = (Test_jiejuo_jilu.GP30_dianya) & 0xFF;
  xieyi2_fanhui[jishu_lenth++] = ((Test_jiejuo_jilu.GP30_dianya) >> 8) & 0xFF;
  memcpy(&xieyi2_fanhui[jishu_lenth], Test_jiejuo_jilu.loraEUI, 16);
  jishu_lenth += 16;
  xieyi2_fanhui[jishu_lenth++] = Test_jiejuo_jilu.qiangci_jiance;
  Test_jiejuo_jilu.kaigai_jiance = 1;
  xieyi2_fanhui[jishu_lenth++] = Test_jiejuo_jilu.kaigai_jiance;
  xieyi2_fanhui[jishu_lenth++] = Test_jiejuo_jilu.GPSmozu_jiacne;
  xieyi2_fanhui[jishu_lenth++] = 0; // 无磁检测
  memcpy(&xieyi2_fanhui[jishu_lenth], Test_jiejuo_jilu.jiaoyanma,
         2); // 海外用校验码
  jishu_lenth += 2;
  memcpy(&xieyi2_fanhui[jishu_lenth], Test_jiejuo_jilu.banbenhao,
         2); // 程序版本号
  jishu_lenth += 2;
  // 对帧长度赋值
  xieyi2_fanhui[2] = jishu_lenth + 2;
  xieyi2_fanhui[jishu_lenth] = 0;
  for (hejiaoyan = 0; hejiaoyan < jishu_lenth; hejiaoyan++) {
    xieyi2_fanhui[jishu_lenth] += xieyi2_fanhui[hejiaoyan];
  }
  jishu_lenth++;
  xieyi2_fanhui[jishu_lenth++] = 0x16;

  // 调试模式下打印协议数据，非调试模式发送给 PC
  if (Debug_Mode) {
    DeBug_print("返回测试结果: ");
    protocol_debug_print(xieyi2_fanhui, jishu_lenth);
  } else {
    PC_Chuankou_tongxin_send(xieyi2_fanhui, jishu_lenth);
  }
}

// 解析PC发送过来的协议
// 注意：透传模式下仍需解析 0xAE 指令（日志/透传切换），否则无法退出透传模式
void PC_xieyijiexi(uint8_t zufuchua[], uint16_t lenth) {
  uint16_t pHead = 0;
  uint8_t hejiaoyan = 0;
  uint16_t zhenchangdu = 0;
  uint16_t linshichangdu = 0;

  while (1) {
    if (pHead + 5 > lenth) {
      break;
    }
    if (zufuchua[pHead] == 0x68) {
      // 如果是开始测试指令，也就是一帧的第二位是0xAA
      // 透传模式下跳过测试相关协议，但继续检查0xAE切换指令
      if (zufuchua[pHead + 1] == 0xAA && !PassThrough_Mode) {
        // 获取协议长度
        zhenchangdu = zufuchua[pHead + 2] - 1;
        // 长度应低于100个字节
        if (zhenchangdu < 100) {
          // 校验工位与尾帧是否正确
          if (zufuchua[pHead + 3] == Test_jiejuo_jilu.gongwei &&
              zufuchua[pHead + zhenchangdu] == 0x16) {
            // 进行和校验
            hejiaoyan = 0;
            for (linshichangdu = 0; linshichangdu < zhenchangdu - 1;
                 linshichangdu++) {
              hejiaoyan += zufuchua[pHead + linshichangdu];
            }
            if (hejiaoyan == zufuchua[pHead + zhenchangdu - 1]) {
              // 协议校验完成，进入主程序处理
              memcpy(Test_jiejuo_jilu.mokuaihao, &zufuchua[pHead + 4], 6);
              Test_jiejuo_jilu.biaoju_leixing_famen = zufuchua[pHead + 10];
              memcpy(Test_jiejuo_jilu.chaoshengbo_canshu, &zufuchua[pHead + 11],
                     6);
              memcpy(Test_jiejuo_jilu.jixiebiao_canshu, &zufuchua[pHead + 17],
                     6);
              // 同步更新机械表阀门配置结构体
              Test_jiejuo_jilu.mechanical_config.pipe_diameter_mm =
                  Test_jiejuo_jilu.jixiebiao_canshu[0] |
                  (Test_jiejuo_jilu.jixiebiao_canshu[1] << 8);
              Test_jiejuo_jilu.mechanical_config.valve_type =
                  Test_jiejuo_jilu.jixiebiao_canshu[2];
              Test_jiejuo_jilu.mechanical_config.timeout_seconds =
                  Test_jiejuo_jilu.jixiebiao_canshu[3];
              Test_jiejuo_jilu.mechanical_config.stall_current_ma =
                  Test_jiejuo_jilu.jixiebiao_canshu[4] |
                  (Test_jiejuo_jilu.jixiebiao_canshu[5] << 8);
              // 同步更新超声波表阀门配置结构体
              Test_jiejuo_jilu.ultrasonic_config.pipe_diameter_mm =
                  Test_jiejuo_jilu.chaoshengbo_canshu[0] |
                  (Test_jiejuo_jilu.chaoshengbo_canshu[1] << 8);
              Test_jiejuo_jilu.ultrasonic_config.valve_type =
                  Test_jiejuo_jilu.chaoshengbo_canshu[2];
              Test_jiejuo_jilu.ultrasonic_config.timeout_seconds =
                  Test_jiejuo_jilu.chaoshengbo_canshu[3];
              Test_jiejuo_jilu.ultrasonic_config.stall_current_ma =
                  Test_jiejuo_jilu.chaoshengbo_canshu[4] |
                  (Test_jiejuo_jilu.chaoshengbo_canshu[5] << 8);
              DeBug_print(
                  "机械表管径是%02x%02x，阀门类型是%02x\r\n,超时时间是%02x秒,"
                  "堵转电流是%dmA\r\n",
                  Test_jiejuo_jilu.jixiebiao_canshu[1],
                  Test_jiejuo_jilu.jixiebiao_canshu[0],
                  Test_jiejuo_jilu.jixiebiao_canshu[2],
                  Test_jiejuo_jilu.jixiebiao_canshu[3],
                  Test_jiejuo_jilu.jixiebiao_canshu[4] +
                      (Test_jiejuo_jilu.jixiebiao_canshu[5] << 8));
              test_start();
              PC_xieyifasong_1();
              pHead += zhenchangdu;
            }
          }
        }
      } else if (zufuchua[pHead + 1] == 0xAC && !PassThrough_Mode &&
                 zufuchua[pHead + 3] == Test_jiejuo_jilu.gongwei &&
                 zufuchua[pHead + 5] == 0x16) {
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
                // 协议校验完成，进入主程序处理
                // 如果测试完成，则返回结果，未完成则不处理
                if (Test_quanju_canshu_L.test_over == 1) {
                  PC_xieyifasong_2();
                }
                pHead += zhenchangdu;
              }
            }
          }
        }
      }
      // 增加设置开启关闭日志和透传的协议，协议是
      // 工装日志和透传开启关闭指令
      // 5工位关闭日志：68 AE 08 05 00 00 23 16 5工位开启日志：68 AE 08 05 01 00
      // 24 16 5工位开启透传：68 AE 08 05 00 01 24 16 5工位开启透传和日志：68 AE
      // 08 05 01 01 25 16 响应是68 AF 08 05 01 00 25 16这种类型，主要依靠标志AF
      else if (zufuchua[pHead + 1] == 0xAE &&
               zufuchua[pHead + 3] == Test_jiejuo_jilu.gongwei) {
        // 获取协议长度,应该是0x08
        zhenchangdu = zufuchua[pHead + 2];
        // 检查帧尾
        if (zufuchua[pHead + zhenchangdu - 1] != 0x16) {
          pHead++;
          continue;
        }
        // 进行和校验
        hejiaoyan = 0;
        for (linshichangdu = 0; linshichangdu < zhenchangdu - 2;
             linshichangdu++) {
          hejiaoyan += zufuchua[pHead + linshichangdu];
        }
        // 和校验正确就进行处理
        if (hejiaoyan == zufuchua[pHead + zhenchangdu - 2]) {
          uint8_t log_flag = zufuchua[pHead + 4];
          uint8_t passthrough_flag = zufuchua[pHead + 5];
          if (log_flag == 1) {
            Debug_Mode = 1;
          } else {
            Debug_Mode = 0;
          }
          if (passthrough_flag == 1) {
            PassThrough_Mode = 1;
          } else {
            PassThrough_Mode = 0;
          }

          // 发送响应
          PC_Response_Log_Passthrough(Test_jiejuo_jilu.gongwei, log_flag,
                                      passthrough_flag);
        }
      }
    }
    // 无论是否匹配到协议，都要移动指针，避免死循环
    pHead++;
  }
}

/**
 * @brief   log和透传模式的协议的配置的返回响应
 *
 * @author Noah Smith
 * @param gongwei
 * @param log_flag
 * @param passthrough_flag
 */
void PC_Response_Log_Passthrough(uint8_t gongwei, uint8_t log_flag,
                                 uint8_t passthrough_flag) {
  uint8_t response[8];
  uint16_t hejiaoyan = 0;
  response[0] = 0x68;
  response[1] = 0xAF;
  response[2] = 0x08;
  response[3] = gongwei;
  response[4] = log_flag;
  response[5] = passthrough_flag;
  // 计算和校验
  for (int i = 0; i < 6; i++) {
    hejiaoyan += response[i];
  }
  response[6] = hejiaoyan;
  response[7] = 0x16;

  // 调试模式下打印协议数据
  if (Debug_Mode) {
    DeBug_print("日志和透传模式设置响应: ");
    protocol_debug_print(response, 8);
  }

  // 无论是否调试模式，都要发送数据给 PC
  PC_Chuankou_tongxin_send(response, 8);
}
