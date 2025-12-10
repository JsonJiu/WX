/* 
 * File:   app_ina219.h
 * Author: wb
 *
 * Created on November 8, 2016, 10:16 AM
 */

#ifndef APP_INA219_H
#define	APP_INA219_H

#ifdef	__cplusplus
extern "C" {
#endif


void INA219Delay(unsigned int num);    
void IIC_INA219_init(void);
void IIC_INA219_start(void);
void IIC_INA219_stop(void);
unsigned char IIC_INA219_ack();
void IIC_INA219_noack();
void IIC_INA219_write8bit(unsigned char input);
unsigned char IIC_INA219_read8bit();
unsigned char INA219Read(unsigned char RegAddr,unsigned char *tbuf);
unsigned char INA219Write(unsigned char RegAddr,unsigned char *buf);
unsigned int CurrentEve();
void CurrentInsert(unsigned int i);
void tstIna219(void);
void ArraySort(unsigned int *Array1,unsigned int *Array2,unsigned char len);
unsigned char GetCurrent();



#ifdef	__cplusplus
}
#endif

#endif	/* APP_INA219_H */

