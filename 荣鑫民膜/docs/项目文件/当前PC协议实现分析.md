# 当前PC协议实现字段分析

## 协议结构对比分析

基于 `PC_xieyi_Ctrl.c` 中 `PC_xieyifasong_2()` 函数的实际实现：

| 字节位置 | 字段名称 | 数据类型 | 字节数 | 描述 | 实现代码 |
|---------|---------|---------|-------|------|---------|
| 0 | 帧头1 | uint8_t | 1 | 固定值0x68 | `xieyi2_fanhui[0] = 0x68` |
| 1 | 帧头2 | uint8_t | 1 | 固定值0xAD | `xieyi2_fanhui[1] = 0xAD` |
| 2 | 长度 | uint8_t | 1 | 数据长度 | `xieyi2_fanhui[2] = 0x00` |
| 3 | 工位号 | uint8_t | 1 | 测试工位 | `Test_jiejuo_jilu.gongwei` |
| 4-5 | 指定电压(供电)低/高字节 | uint16_t | 2 | 小端格式 | `zhidian_dianya_gongdian` |
| 6-7 | 指定电压(获取)低/高字节 | uint16_t | 2 | 小端格式 | `zhidian_dianya_huoqu` |
| 8-9 | 指定静态功耗低/高字节 | uint16_t | 2 | 小端格式 | `zhidian_jingtai_gonghao` |
| 10-11 | 指定满水功耗低/高字节 | uint16_t | 2 | 小端格式 | `zhidian_manshui_gonghao` |
| 12-13 | 指定走水功耗低/高字节 | uint16_t | 2 | 小端格式 | `zhidian_zoushui_gonghao` |
| 14-15 | 备电电压(供电)低/高字节 | uint16_t | 2 | 小端格式 | `beidian_dianya_gongdian` |
| 16-17 | 备电电压(获取)低/高字节 | uint16_t | 2 | 小端格式 | `beidian_dianya_huoqu` (固定3600) |
| 18-19 | 备电功耗低/高字节 | uint16_t | 2 | 小端格式 | `beidian_gonghao` |
| 20 | 蓝牙检测 | uint8_t | 1 | 0异常 1正常 | `Test_jiejuo_jilu.lanya_jiance` |
| 21 | flash检测 | uint8_t | 1 | 0异常 1正常 | `Test_jiejuo_jilu.flash_jiance` |
| 22 | 计量检测 | uint8_t | 1 | 0异常 1正常 | `Test_jiejuo_jilu.jiliang_jiance` |
| 23 | 红外检测 | uint8_t | 1 | 0异常 1正常 | `Test_jiejuo_jilu.hongwai_jiance` |
| 24-38 | IMEI码 | uint8_t[15] | 15 | ASCII码 | `memcpy(IMEI_CHK, 15)` |
| 39-53 | IMSI码 | uint8_t[15] | 15 | ASCII码 | `memcpy(IMSI_CHK, 15)` |
| 54-73 | ICCID码 | uint8_t[20] | 20 | ASCII码 | `memcpy(ICCID_CHK, 20)` |
| 74 | 信号强度CSQ | uint8_t | 1 | 信号强度值 | `Test_jiejuo_jilu.CSQ` |
| 75 | FM功能 | uint8_t | 1 | FM状态 | `Test_jiejuo_jilu.FM` |
| 76 | FM到位 | uint8_t | 1 | 固定值1 | `Test_jiejuo_jilu.FM_daowei = 1` |
| 77 | EEPROM检测 | uint8_t | 1 | 0异常 1正常 | `Test_jiejuo_jilu.EEPROM_jiance` |
| 78-79 | GP30电压低/高字节 | uint16_t | 2 | 小端格式 | `GP30_dianya` |
| 80-95 | LoRa EUI | uint8_t[16] | 16 | EUI码 | `memcpy(loraEUI, 16)` |

## 关键发现

### 1. 字节序格式
- **数值字段**: 使用**小端格式** (低字节在前，高字节在后)
- **字节数组**: 使用**原始字节顺序** (memcpy直接复制)

### 2. IMEI/IMSI/ICCID传输顺序
```c
// 确认从数组第0个元素开始传输
memcpy(&xieyi2_fanhui[24], Test_jiejuo_jilu.IMEI_CHK, 15);  // IMEI[0]到IMEI[14]
memcpy(&xieyi2_fanhui[39], Test_jiejuo_jilu.IMSI_CHK, 15);  // IMSI[0]到IMSI[14]
memcpy(&xieyi2_fanhui[54], Test_jiejuo_jilu.ICCID_CHK, 20); // ICCID[0]到ICCID[19]
```

### 3. 数据来源追踪
- IMEI/IMSI/ICCID来自水表F003协议，透明传输
- 数值类数据在传输时转换为小端格式
- 部分字段有固定值 (如FM_daowei=1, beidian_dianya_huoqu=3600)

## 对比建议
请将此分析与您的ODS/XLTX表格对比：
1. 字节位置是否一致
2. 数据类型是否匹配
3. 字节序是否正确
4. IMEI等字段的传输顺序是否符合预期
