/*
 * File:   app_irdausart_protocl.h
 * Author: wb
 *
 * Created on November 23, 2016, 1:08 PM
 */

#ifndef APP_IRDAUSART_PROTOCL_H
#define	APP_IRDAUSART_PROTOCL_H

#ifdef	__cplusplus
extern "C" {
#endif

unsigned short IrdaUartProtocol_DataAanaly(unsigned char *buf, unsigned char *buf_temp);
unsigned short IrdaUartDataFormFrame(unsigned char ctlcode, unsigned short datamark);
void IrdaUartDataDealProcess(void);

#ifdef	__cplusplus
}
#endif

#endif	/* APP_IRDAUSART_PROTOCL_H */

