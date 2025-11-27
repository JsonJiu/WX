#ifndef __USER_MANAGE_H
#define __USER_MANAGE_H
#include "lib_public.h"
#ifndef BIT0
#define BIT0         0x00000001U
#endif
#ifndef BIT1
#define BIT1         0x00000002U
#endif
#ifndef BIT2
#define BIT2         0x00000004U
#endif
#ifndef BIT3
#define BIT3         0x00000008U
#endif
#ifndef BIT4
#define BIT4         0x00000010U
#endif
#ifndef BIT5
#define BIT5         0x00000020U
#endif
#ifndef BIT6
#define BIT6         0x00000040U
#endif
#ifndef BIT7
#define BIT7         0x00000080U
#endif
#ifndef BIT8
#define BIT8         0x00000100U
#endif
#ifndef BIT9
#define BIT9         0x00000200U
#endif
#ifndef BIT10
#define BIT10        0x00000400U
#endif
#ifndef BIT11
#define BIT11        0x00000800U
#endif
#ifndef BIT12
#define BIT12        0x00001000U
#endif
#ifndef BIT13
#define BIT13        0x00002000U
#endif
#ifndef BIT14
#define BIT14        0x00004000U
#endif
#ifndef BIT15
#define BIT15        0x00008000U
#endif
#ifndef BIT16
#define BIT16        0x00010000U
#endif
#ifndef BIT17
#define BIT17        0x00020000U
#endif
#ifndef BIT18
#define BIT18        0x00040000U
#endif
#ifndef BIT19
#define BIT19        0x00080000U
#endif
#ifndef BIT20
#define BIT20        0x00100000U
#endif
#ifndef BIT21
#define BIT21        0x00200000U
#endif
#ifndef BIT22
#define BIT22        0x00400000U
#endif
#ifndef BIT23
#define BIT23        0x00800000U
#endif
#ifndef BIT24
#define BIT24        0x01000000U
#endif
#ifndef BIT25
#define BIT25        0x02000000U
#endif
#ifndef BIT26
#define BIT26        0x04000000U
#endif
#ifndef BIT27
#define BIT27        0x08000000U
#endif
#ifndef BIT28
#define BIT28        0x10000000U
#endif
#ifndef BIT29
#define BIT29        0x20000000U
#endif
#ifndef BIT30
#define BIT30        0x40000000U
#endif
#ifndef BIT31
#define BIT31        0x80000000U
#endif

extern void User_TimerCfg(unsigned char type, unsigned char len, IRQServerFT CallBack);
extern void User_TimerStart(unsigned char type);
extern void User_TimerStop(unsigned char type);

void Example(void);
void User_Init(void);
void LowPowerConsManage(void);
#endif
