#ifndef __IR_H__
#define __IR_H__
#include "main.h"
void Find_Device_ID(void);
void Start_Wmbus_Test(void);
void Set_true(void);
void Find_Version(void);
void Find_Result(void);
void Close_Commumication(void);
void Find_LoraEUI(void);
void Get_GP30_Voltage(void);
void Manual_Report_UP(void);
void Find_Report_Result(void);
void Find_Self_Check(void);
void simulate3WirePulse(uint8_t Dir);
void getLrdLoRaEUI(void);
void whiteAppkey(void);
void whiteAppEUI(void);
void whiteSave(void);
void whiteReset(void);
void loraSetFreq923(void);
void loraGetFreq(void);
void irProtocolParse(uint8_t *Data, uint32_t lenth);
void findLrdType(void);
void configWmbus(void);
void findWmbusConfig(void);
void setWmbusKey(void);
#endif
