#ifndef __DRV_LG1608_H__
#define __DRV_LG1608_H__

#define CardDelayCount 1

#define TRUE    1
#define FALSE   0
#define WRITE 0
#define READ 1

#define CON_ZONE 8
#define USER_ZONE0 0
#define USER_ZONE1 1
#define USER_ZONE2 2
#define USER_ZONE3 3
#define USER_ZONE4 4
#define USER_ZONE5 5
#define USER_ZONE6 6
#define USER_ZONE7 7

unsigned char init_card(void);
unsigned char write_zone(unsigned char area, unsigned char addr, unsigned char *data, unsigned int len);
unsigned char read_zone(unsigned char area, unsigned char add, unsigned char *data, unsigned int len);
void set_userzone(unsigned char zone);
unsigned char verify_pass(unsigned char zone, unsigned char io, unsigned char *password);
void encrypt(unsigned char *passbuf, unsigned char *keyseed, unsigned char *skey1, unsigned char *skey2);
void E_encrypt(unsigned char *passbuf, const unsigned char *keyseed, const unsigned char *skey1, const unsigned char *skey2);
void read_cmc(unsigned char *cmc);
void read_ar(unsigned char *arbuf);
unsigned char read_pac(unsigned char area, unsigned char io);

extern void CardSclSet(void);
extern void CardSclClr(void);
extern void CardRstSet(void);
extern void CardRstClr(void);
extern void CardSdaSet(void);
extern void CardSdaClr(void);
extern void CardSdaOutput(void);
extern void CardSdaInput(void);
extern unsigned char CardSdaRead(void);
extern void DelayMs(unsigned int Ms);

#endif
