#define LOG_TAG "diaphragm_board_debug"
#include "NB_18_DiaphragmGas_board_debug.h"
/**
 * @file NB_18_DiaphragmGas_board_debug.c
 * @brief
 * 这个是膜式燃气表测试工装的调试相关代码，比如上位机发送控制指令开电源，关电源，实时测试当前的功耗等等;
 * 这个文件和测试流程无关，但是可以强制设置测试流程终止
 * @version 1.0.0
 * @author Noah
 * @date 2026-02-05
 *
 * 本文件通过 port 层接口实现调试功能，不直接操作底层硬件。
 */
#include "NB_18_DiaphragmGas_port.h" // 使用 port 层标准接口
#include <elog.h>
#include <string.h>

#ifdef __cplusplus
extern "C" {
#endif

#include "time_manager.h" // TM_GetTick

/*============ 调试控制状态结构 ============*/

// 采集任务配置
typedef struct {
  uint8_t enable;         // 使能标志
  uint16_t interval_ms;   // 采集间隔(ms)
  uint8_t avg_count;      // 平均次数
  uint8_t print_interval; // 打印间隔(秒)
  uint8_t print_count;    // 打印次数限制，0=持续
  uint8_t mode;           // 模式 (功耗: 0正常/1低功耗)

  // 运行时状态
  uint32_t last_sample_tick; // 上次采集时间
  uint32_t last_print_tick;  // 上次打印时间
  int32_t sum;               // 累计值
  uint8_t sample_cnt;        // 当前采集计数
  uint8_t print_done_cnt;    // 已打印次数
} SampleTaskConfig;

// 定时输出控制
typedef struct {
  uint8_t enable;       // 使能标志
  uint8_t duration_sec; // 持续时间(秒)，0=一直开启
  uint32_t start_tick;  // 开始时间
} TimedOutputConfig;

// 全局调试状态
static struct {
  uint8_t control_mode_active; // 控制模式激活标志: 0=退出, 1=进入

  SampleTaskConfig pwr_test;   // 功耗测试
  SampleTaskConfig valve_volt; // 阀门电压
  SampleTaskConfig all_volt;   // 各路电压

  TimedOutputConfig pos1;  // 到位信号1
  TimedOutputConfig pos2;  // 到位信号2
  TimedOutputConfig hall1; // 霍尔1
  TimedOutputConfig hall2; // 霍尔2
  TimedOutputConfig hall3; // 霍尔3
} s_debug_state;

/*============ 内部函数声明 ============*/
static void handle_power_control(uint8_t main_power, uint8_t aux_power);
static void handle_pwr_test_config(const uint8_t *data);
static void handle_valve_volt_config(const uint8_t *data);
static void handle_all_volt_config(const uint8_t *data);
static void handle_pos_signal_config(uint8_t pos1_level, uint8_t pos1_dur,
                                     uint8_t pos2_level, uint8_t pos2_dur);
static void handle_hall_config(uint8_t hall1_en, uint8_t hall1_dur,
                               uint8_t hall2_en, uint8_t hall2_dur,
                               uint8_t hall3_en, uint8_t hall3_dur);

/*============ 主控制命令处理 ============*/

/**
 * @brief 处理PC端发来的工装调试命令
 * @param data 完整帧数据 (36字节)
 * @param length 数据长度
 */
void Protocol_PC_BoardDebug_Handle(const uint8_t *data, uint16_t length) {
  if (data == NULL || length < 36) {
    log_w("调试命令数据无效, len=%d", length);
    return;
  }

  log_i("========== 解析工装调试命令 ==========");

  /*---------------------------------------------------------------------------
   * 控制模式: 0=退出控制模式, 1=进入控制模式
   *---------------------------------------------------------------------------*/
  uint8_t control_status = data[4];
  if (control_status == 0) {
    s_debug_state.control_mode_active = 0;
    log_i("[控制模式] 退出控制模式");
  } else if (control_status == 1) {
    s_debug_state.control_mode_active = 1;
    log_i("[控制模式] 进入控制模式");
  }
  // 其他值不改变当前控制模式状态

  /*---------------------------------------------------------------------------
   * 功能1-2: 电源控制
   *---------------------------------------------------------------------------*/
  uint8_t main_power = data[5]; // 0=关, 1=开, 0xFF=不操作
  uint8_t aux_power = data[6];
  handle_power_control(main_power, aux_power);

  /*---------------------------------------------------------------------------
   * 功能3: 静态功耗测试
   *---------------------------------------------------------------------------*/
  handle_pwr_test_config(data);

  /*---------------------------------------------------------------------------
   * 功能4: 阀门电压采集
   *---------------------------------------------------------------------------*/
  handle_valve_volt_config(data);

  /*---------------------------------------------------------------------------
   * 功能5: 各路电压显示
   *---------------------------------------------------------------------------*/
  handle_all_volt_config(data);

  /*---------------------------------------------------------------------------
   * 功能6-7: 到位信号输出
   *---------------------------------------------------------------------------*/
  handle_pos_signal_config(data[25], data[26], data[27], data[28]);

  /*---------------------------------------------------------------------------
   * 功能8-10: 霍尔/电磁铁控制
   *---------------------------------------------------------------------------*/
  handle_hall_config(data[29], data[30], data[31], data[32], data[33],
                     data[34]);

  log_i("========================================");
}

/*============ 电源控制 ============*/

static void handle_power_control(uint8_t main_power, uint8_t aux_power) {
  // 主电源控制
  if (main_power != 0xFF) {
    if (main_power) {
      power_on_main_power();
      log_i("[电源] 主电源 -> 开启");
    } else {
      power_off_main_power();
      log_i("[电源] 主电源 -> 关闭");
    }
  }

  // 从电源控制
  if (aux_power != 0xFF) {
    if (aux_power) {
      // 当前我们没有从电，这里暂时注释掉，后续如果有需求，可以开启
      // power_on_backup_power();
      log_i("[电源] 从电源 -> 开启");
    } else {
      // power_off_backup_power();
      log_i("[电源] 从电源 -> 关闭");
    }
  }
}

/*============ 功耗测试配置 ============*/

static void handle_pwr_test_config(const uint8_t *data) {
  uint8_t pwr_test_en = data[7];
  uint16_t pwr_interval = data[8] | (data[9] << 8);
  uint8_t pwr_avg_cnt = data[10];
  uint8_t pwr_print_int = data[11];
  uint8_t pwr_print_cnt = data[12]; // 打印次数，0 就是持续打印

  if (pwr_test_en == 0) {
    // 停止功耗测试
    s_debug_state.pwr_test.enable = 0;
    // 功耗测量停止，切换回普通供电模式（但如果用户明确要关闭电源则不重开）
    uint8_t main_power = data[5];
    power_off_main_power();
    if (main_power != 0) { // 只有用户没要求关电时才重新开启
      power_on_main_power();
    }
    log_i("[功耗] 测试已停止");
  } else {
    // 配置并启动功耗测试
    s_debug_state.pwr_test.enable = 1;
    s_debug_state.pwr_test.mode =
        (pwr_test_en == 2) ? 1 : 0; // 1=低功耗, 2=正常
    s_debug_state.pwr_test.interval_ms =
        (pwr_interval > 0) ? pwr_interval : 100;
    s_debug_state.pwr_test.avg_count = (pwr_avg_cnt > 0) ? pwr_avg_cnt : 1;
    s_debug_state.pwr_test.print_interval = pwr_print_int;
    s_debug_state.pwr_test.print_count = pwr_print_cnt;

    // 重置运行状态
    s_debug_state.pwr_test.last_sample_tick = TM_GetTick();
    s_debug_state.pwr_test.last_print_tick = TM_GetTick();
    s_debug_state.pwr_test.sum = 0;
    s_debug_state.pwr_test.sample_cnt = 0;
    s_debug_state.pwr_test.print_done_cnt = 0;

    // 【重要】清理所有可能导致功耗异常的IO状态
    // 确保阀门到位信号处于高阻态，避免电流通过气表上拉电阻灌入
    output_valve_position_signal_1(0); // PA12 释放
    output_valve_position_signal_2(0); // PA11 释放
    // 关闭霍尔信号输出
    output_hall_signal_1(0);
    output_hall_signal_2(0);
    // 关闭开盖检测电磁铁
    output_open_cover_signal(0);
    // 恢复119报警信号为高电平（非触发状态）
    output_alarm_119_signal(1);
    log_i("[功耗] 已清理所有IO状态，避免影响功耗测量");

    // 启动功耗测量模式
    power_on_main_power_measure_mode();
    log_i("[功耗] 测试已启动: 模式=%s, 间隔=%dms, 平均=%d次, 打印间隔=%ds, "
          "打印次数=%d",
          s_debug_state.pwr_test.mode ? "低功耗" : "正常",
          s_debug_state.pwr_test.interval_ms, s_debug_state.pwr_test.avg_count,
          s_debug_state.pwr_test.print_interval,
          s_debug_state.pwr_test.print_count);
  }
}

/*============ 阀门电压采集配置 ============*/

static void handle_valve_volt_config(const uint8_t *data) {
  uint8_t valve_en = data[13];
  uint16_t valve_interval = data[14] | (data[15] << 8);
  uint8_t valve_avg_cnt = data[16];
  uint8_t valve_print_int = data[17];
  uint8_t valve_print_cnt = data[18];

  if (valve_en == 0) {
    s_debug_state.valve_volt.enable = 0;
    log_i("[阀门电压] 采集已停止");
  } else {
    s_debug_state.valve_volt.enable = 1;
    s_debug_state.valve_volt.interval_ms =
        (valve_interval > 0) ? valve_interval : 100;
    s_debug_state.valve_volt.avg_count =
        (valve_avg_cnt > 0) ? valve_avg_cnt : 1;
    s_debug_state.valve_volt.print_interval = valve_print_int;
    s_debug_state.valve_volt.print_count = valve_print_cnt;

    // 重置运行状态
    s_debug_state.valve_volt.last_sample_tick = TM_GetTick();
    s_debug_state.valve_volt.last_print_tick = TM_GetTick();
    s_debug_state.valve_volt.sum = 0;
    s_debug_state.valve_volt.sample_cnt = 0;
    s_debug_state.valve_volt.print_done_cnt = 0;

    log_i("[阀门电压] 采集已启动: 间隔=%dms, 平均=%d次",
          s_debug_state.valve_volt.interval_ms,
          s_debug_state.valve_volt.avg_count);
  }
}

/*============ 各路电压显示配置 ============*/

static void handle_all_volt_config(const uint8_t *data) {
  uint8_t volt_en = data[19];
  uint16_t volt_interval = data[20] | (data[21] << 8);
  uint8_t volt_avg_cnt = data[22];
  uint8_t volt_print_int = data[23];
  uint8_t volt_print_cnt = data[24];

  if (volt_en == 0) {
    s_debug_state.all_volt.enable = 0;
    log_i("[各路电压] 显示已停止");
  } else {
    s_debug_state.all_volt.enable = 1;
    s_debug_state.all_volt.interval_ms =
        (volt_interval > 0) ? volt_interval : 500;
    s_debug_state.all_volt.avg_count = (volt_avg_cnt > 0) ? volt_avg_cnt : 1;
    s_debug_state.all_volt.print_interval = volt_print_int;
    s_debug_state.all_volt.print_count = volt_print_cnt;

    // 重置运行状态
    s_debug_state.all_volt.last_sample_tick = TM_GetTick();
    s_debug_state.all_volt.last_print_tick = TM_GetTick();
    s_debug_state.all_volt.sum = 0;
    s_debug_state.all_volt.sample_cnt = 0;
    s_debug_state.all_volt.print_done_cnt = 0;

    log_i("[各路电压] 显示已启动: 间隔=%dms, 平均=%d次",
          s_debug_state.all_volt.interval_ms, s_debug_state.all_volt.avg_count);
  }
}

/*============ 到位信号控制 ============*/

static void handle_pos_signal_config(uint8_t pos1_level, uint8_t pos1_dur,
                                     uint8_t pos2_level, uint8_t pos2_dur) {
  // 到位信号1
  if (pos1_level) {
    output_valve_position_signal_1(1);
    s_debug_state.pos1.enable = 1;
    s_debug_state.pos1.duration_sec = pos1_dur;
    s_debug_state.pos1.start_tick = TM_GetTick();
    log_i("[到位1] 已开启, 持续=%d秒 (0=一直)", pos1_dur);
  } else {
    output_valve_position_signal_1(0);
    s_debug_state.pos1.enable = 0;
    log_i("[到位1] 已关闭");
  }

  // 到位信号2
  if (pos2_level) {
    output_valve_position_signal_2(1);
    s_debug_state.pos2.enable = 1;
    s_debug_state.pos2.duration_sec = pos2_dur;
    s_debug_state.pos2.start_tick = TM_GetTick();
    log_i("[到位2] 已开启, 持续=%d秒 (0=一直)", pos2_dur);
  } else {
    output_valve_position_signal_2(0);
    s_debug_state.pos2.enable = 0;
    log_i("[到位2] 已关闭");
  }
}

/*============ 霍尔/电磁铁控制 ============*/

static void handle_hall_config(uint8_t hall1_en, uint8_t hall1_dur,
                               uint8_t hall2_en, uint8_t hall2_dur,
                               uint8_t hall3_en, uint8_t hall3_dur) {
  // 限制最大持续时间为15秒，防止烧毁
  if (hall1_dur > 15)
    hall1_dur = 15;
  if (hall2_dur > 15)
    hall2_dur = 15;
  if (hall3_dur > 15)
    hall3_dur = 15;

  // 霍尔1
  if (hall1_en) {
    output_hall_signal_1(1);
    s_debug_state.hall1.enable = 1;
    s_debug_state.hall1.duration_sec = hall1_dur;
    s_debug_state.hall1.start_tick = TM_GetTick();
    log_i("[霍尔1] 已开启, 持续=%d秒", hall1_dur);
  } else {
    output_hall_signal_1(0);
    s_debug_state.hall1.enable = 0;
    log_i("[霍尔1] 已关闭");
  }

  // 霍尔2
  if (hall2_en) {
    output_hall_signal_2(1);
    s_debug_state.hall2.enable = 1;
    s_debug_state.hall2.duration_sec = hall2_dur;
    s_debug_state.hall2.start_tick = TM_GetTick();
    log_i("[霍尔2] 已开启, 持续=%d秒", hall2_dur);
  } else {
    output_hall_signal_2(0);
    s_debug_state.hall2.enable = 0;
    log_i("[霍尔2] 已关闭");
  }

  // 霍尔3 (同时可能是开盖信号)
  // 注意: port层暂无hall3接口，可根据实际硬件扩展
  if (hall3_en) {
    // output_hall_signal_3(1); // 如有此接口则启用
    s_debug_state.hall3.enable = 1;
    s_debug_state.hall3.duration_sec = hall3_dur;
    s_debug_state.hall3.start_tick = TM_GetTick();
    log_i("[霍尔3] 已开启, 持续=%d秒 (未实现)", hall3_dur);
  } else {
    // output_hall_signal_3(0);
    s_debug_state.hall3.enable = 0;
    log_i("[霍尔3] 已关闭");
  }
}

/*============ 周期性任务处理 (需在主循环中调用) ============*/

/**
 * @brief 调试任务周期处理
 * @note 需要在主循环中周期调用此函数
 */
void BoardDebug_Task_Process(void) {
  uint32_t now = TM_GetTick();

  /*---
   * 功耗测试采集，这里不要做补偿，这里是调试的，有时候需要不接负载查看当前功耗，后续作为补偿标准
   * ---*/
  if (s_debug_state.pwr_test.enable) {
    // 检查打印次数限制
    if (s_debug_state.pwr_test.print_count > 0 &&
        s_debug_state.pwr_test.print_done_cnt >=
            s_debug_state.pwr_test.print_count) {
      s_debug_state.pwr_test.enable = 0;
      // 停止功耗测量，恢复正常供电
      power_off_main_power();
      power_on_main_power();
      log_i("[功耗] 测试完成, 共打印%d次",
            s_debug_state.pwr_test.print_done_cnt);
    } else if (now - s_debug_state.pwr_test.last_sample_tick >=
               s_debug_state.pwr_test.interval_ms) {
      s_debug_state.pwr_test.last_sample_tick = now;

      // 通过port层接口采集电流
      int32_t current = 0;
      read_main_power_consumption_ua(&current);
      s_debug_state.pwr_test.sum += current;
      s_debug_state.pwr_test.sample_cnt++;

      // 达到平均次数后计算并打印
      if (s_debug_state.pwr_test.sample_cnt >=
          s_debug_state.pwr_test.avg_count) {
        int32_t avg =
            s_debug_state.pwr_test.sum / s_debug_state.pwr_test.sample_cnt;

        // 检查打印间隔
        if (s_debug_state.pwr_test.print_interval == 0 ||
            (now - s_debug_state.pwr_test.last_print_tick) >=
                (s_debug_state.pwr_test.print_interval * 1000)) {
          s_debug_state.pwr_test.last_print_tick = now;
          s_debug_state.pwr_test.print_done_cnt++;

          if (avg < 1000) {
            log_i("[功耗] 平均电流(未做补偿): %d uA", avg);
          } else {
            log_i("[功耗] 平均电流: %d.%03d mA", avg / 1000, avg % 1000);
          }
        }

        // 重置累计
        s_debug_state.pwr_test.sum = 0;
        s_debug_state.pwr_test.sample_cnt = 0;
      }
    }
  }

  /*--- 阀门电压采集 ---*/
  if (s_debug_state.valve_volt.enable) {
    if (s_debug_state.valve_volt.print_count > 0 &&
        s_debug_state.valve_volt.print_done_cnt >=
            s_debug_state.valve_volt.print_count) {
      s_debug_state.valve_volt.enable = 0;
      log_i("[阀门电压] 采集完成");
    } else if (now - s_debug_state.valve_volt.last_sample_tick >=
               s_debug_state.valve_volt.interval_ms) {
      s_debug_state.valve_volt.last_sample_tick = now;

      uint16_t volt = 0;
      read_main_voltage_mv(&volt);
      s_debug_state.valve_volt.sum += volt;
      s_debug_state.valve_volt.sample_cnt++;

      if (s_debug_state.valve_volt.sample_cnt >=
          s_debug_state.valve_volt.avg_count) {
        int32_t avg =
            s_debug_state.valve_volt.sum / s_debug_state.valve_volt.sample_cnt;

        if (s_debug_state.valve_volt.print_interval == 0 ||
            (now - s_debug_state.valve_volt.last_print_tick) >=
                (s_debug_state.valve_volt.print_interval * 1000)) {
          s_debug_state.valve_volt.last_print_tick = now;
          s_debug_state.valve_volt.print_done_cnt++;
          log_i("[阀门电压] 平均: %d mV", avg);
        }

        s_debug_state.valve_volt.sum = 0;
        s_debug_state.valve_volt.sample_cnt = 0;
      }
    }
  }

  /*--- 各路电压采集 ---*/
  if (s_debug_state.all_volt.enable) {
    if (s_debug_state.all_volt.print_count > 0 &&
        s_debug_state.all_volt.print_done_cnt >=
            s_debug_state.all_volt.print_count) {
      s_debug_state.all_volt.enable = 0;
      log_i("[各路电压] 显示完成");
    } else if (now - s_debug_state.all_volt.last_sample_tick >=
               s_debug_state.all_volt.interval_ms) {
      s_debug_state.all_volt.last_sample_tick = now;
      s_debug_state.all_volt.sample_cnt++;

      if (s_debug_state.all_volt.sample_cnt >=
          s_debug_state.all_volt.avg_count) {
        if (s_debug_state.all_volt.print_interval == 0 ||
            (now - s_debug_state.all_volt.last_print_tick) >=
                (s_debug_state.all_volt.print_interval * 1000)) {
          s_debug_state.all_volt.last_print_tick = now;
          s_debug_state.all_volt.print_done_cnt++;

          // 通过port层接口读取各路电压
          uint16_t main_v = 0, backup_v = 0, board_5v = 0, board_3v3 = 0,
                   gprs_v = 0, rtc_v = 0;
          read_main_voltage_mv(&main_v);
          read_backup_voltage_mv(&backup_v);
          read_board_5v_voltage_mv(&board_5v);
          read_board_3v3_voltage_mv(&board_3v3);
          read_gprs_3v6_voltage_mv(&gprs_v);
          read_rtc_battery_voltage_mv(&rtc_v);

          log_i("[各路电压] 主电=%dmV, 备电=%dmV, 板5V=%dmV, 板3V3=%dmV, "
                "GPRS=%dmV, RTC=%dmV",
                main_v, backup_v, board_5v, board_3v3, gprs_v, rtc_v);
        }
        s_debug_state.all_volt.sample_cnt = 0;
      }
    }
  }

  /*--- 到位信号1超时检测 ---*/
  if (s_debug_state.pos1.enable && s_debug_state.pos1.duration_sec > 0) {
    if ((now - s_debug_state.pos1.start_tick) >=
        (s_debug_state.pos1.duration_sec * 1000)) {
      output_valve_position_signal_1(0); // 恢复高阻态（释放信号）
      s_debug_state.pos1.enable = 0;
      log_i("[到位1] 超时自动关闭");
    }
  }

  /*--- 到位信号2超时检测 ---*/
  if (s_debug_state.pos2.enable && s_debug_state.pos2.duration_sec > 0) {
    if ((now - s_debug_state.pos2.start_tick) >=
        (s_debug_state.pos2.duration_sec * 1000)) {
      output_valve_position_signal_2(0);
      s_debug_state.pos2.enable = 0;
      log_i("[到位2] 超时自动关闭");
    }
  }

  /*--- 霍尔1超时检测 ---*/
  if (s_debug_state.hall1.enable && s_debug_state.hall1.duration_sec > 0) {
    if ((now - s_debug_state.hall1.start_tick) >=
        (s_debug_state.hall1.duration_sec * 1000)) {
      output_hall_signal_1(0);
      s_debug_state.hall1.enable = 0;
      log_i("[霍尔1] 超时自动关闭 (保护)");
    }
  }

  /*--- 霍尔2超时检测 ---*/
  if (s_debug_state.hall2.enable && s_debug_state.hall2.duration_sec > 0) {
    if ((now - s_debug_state.hall2.start_tick) >=
        (s_debug_state.hall2.duration_sec * 1000)) {
      output_hall_signal_2(0);
      s_debug_state.hall2.enable = 0;
      log_i("[霍尔2] 超时自动关闭 (保护)");
    }
  }

  /*--- 霍尔3超时检测 ---*/
  if (s_debug_state.hall3.enable && s_debug_state.hall3.duration_sec > 0) {
    if ((now - s_debug_state.hall3.start_tick) >=
        (s_debug_state.hall3.duration_sec * 1000)) {
      // output_hall_signal_3(0);
      s_debug_state.hall3.enable = 0;
      log_i("[霍尔3] 超时自动关闭 (保护)");
    }
  }
}

/*============ 外部入口函数 ============*/

void diaphragm_board_debug_handle_command(const uint8_t *data,
                                          uint16_t length) {
  if (data == NULL || length == 0) {
    log_w("工装调试命令参数无效");
    return;
  }
  Protocol_PC_BoardDebug_Handle(data, length);
}

/*============ 调试状态查询API ============*/

/**
 * @brief 获取功耗测试使能状态
 */
uint8_t BoardDebug_IsPwrTestEnabled(void) {
  return s_debug_state.pwr_test.enable;
}

/**
 * @brief 停止所有调试任务
 */
void BoardDebug_StopAllTasks(void) {
  // 停止采集任务
  s_debug_state.pwr_test.enable = 0;
  s_debug_state.valve_volt.enable = 0;
  s_debug_state.all_volt.enable = 0;

  // 关闭输出
  if (s_debug_state.pos1.enable) {
    output_valve_position_signal_1(0);
    s_debug_state.pos1.enable = 0;
  }
  if (s_debug_state.pos2.enable) {
    output_valve_position_signal_2(0);
    s_debug_state.pos2.enable = 0;
  }
  if (s_debug_state.hall1.enable) {
    output_hall_signal_1(0);
    s_debug_state.hall1.enable = 0;
  }
  if (s_debug_state.hall2.enable) {
    output_hall_signal_2(0);
    s_debug_state.hall2.enable = 0;
  }
  s_debug_state.hall3.enable = 0;

  // 停止功耗测量模式
  power_off_main_power();
  power_on_main_power();
  log_i("[调试] 所有任务已停止");
}

/*============ 控制模式API ============*/

/**
 * @brief 查询是否处于控制模式
 * @return 1=处于控制模式, 0=未处于控制模式
 */
uint8_t BoardDebug_IsControlModeActive(void) {
  return s_debug_state.control_mode_active;
}

/**
 * @brief 退出控制模式
 */
void BoardDebug_ExitControlMode(void) {
  s_debug_state.control_mode_active = 0;
  log_i("[控制模式] 强制退出控制模式");
}

/**
 * @brief 控制模式循环
 * @param uart_process_callback 串口数据处理回调函数，用于在循环中处理新的命令
 * @note 调用此函数后会进入控制模式循环，直到control_mode_active被设为0
 *       在循环中会持续执行功耗测试、电压采集等任务
 *       同时通过回调函数处理串口数据以接收退出命令
 */
void BoardDebug_ControlMode_Loop(void (*uart_process_callback)(void)) {
  if (!s_debug_state.control_mode_active) {
    log_w("[控制模式] 未进入控制模式，无需循环");
    return;
  }

  log_i("[控制模式] ========== 进入控制模式循环 ==========");

  while (s_debug_state.control_mode_active) {
    // 执行调试任务处理
    BoardDebug_Task_Process();

    // 处理串口数据（包括可能的退出命令）
    if (uart_process_callback != NULL) {
      uart_process_callback();
    }

    // 适当延时，避免CPU占用过高
    // 这里假设有delay函数，如果没有可以用空循环或其他方式
    // FL_DelayMs(1); // 根据实际情况调整
  }

  log_i("[控制模式] ========== 退出控制模式循环 ==========");
}

/**
 * @brief 控制模式循环（带超时）
 * @param uart_process_callback 串口数据处理回调函数
 * @param timeout_ms 超时时间(毫秒), 0=无超时
 * @return 0=正常退出(收到退出命令), 1=超时退出
 */
uint8_t BoardDebug_ControlMode_Loop_Timeout(void (*uart_process_callback)(void),
                                            uint32_t timeout_ms) {
  if (!s_debug_state.control_mode_active) {
    log_w("[控制模式] 未进入控制模式，无需循环");
    return 0;
  }

  log_i("[控制模式] ========== 进入控制模式循环 (超时=%dms) ==========",
        timeout_ms);

  uint32_t start_tick = TM_GetTick();

  while (s_debug_state.control_mode_active) {
    // 执行调试任务处理
    BoardDebug_Task_Process();

    // 处理串口数据（包括可能的退出命令）
    if (uart_process_callback != NULL) {
      uart_process_callback();
    }

    // 检查超时
    if (timeout_ms > 0 && (TM_GetTick() - start_tick) >= timeout_ms) {
      log_w("[控制模式] 超时退出");
      s_debug_state.control_mode_active = 0;
      return 1; // 超时退出
    }
  }

  log_i("[控制模式] ========== 正常退出控制模式循环 ==========");
  return 0; // 正常退出
}

#ifdef __cplusplus
}
#endif
