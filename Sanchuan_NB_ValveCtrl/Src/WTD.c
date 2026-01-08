#include "WTD.h"

void WatchDog_Init() {
  FL_IWDT_InitTypeDef WTD_InitTypeDef[1];
  FL_IWDT_StructInit(WTD_InitTypeDef);
  FL_IWDT_Init(IWDT, WTD_InitTypeDef);
  FL_IWDT_ReloadCounter(IWDT);
}
