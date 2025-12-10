/*
 * File:   app_pcusart_protocol.h
 * Author: wb
 *
 * Created on November 7, 2016, 6:51 PM
 */

#ifndef APP_PCUSART_PROTOCOL_H
#define	APP_PCUSART_PROTOCOL_H

#ifdef	__cplusplus
extern "C" {
#endif

unsigned short PCUartProtocol_DataAanaly(unsigned char *buf, unsigned char *buf_temp);
unsigned short PCUartDataFormFrame(unsigned char ctlcode, unsigned short datamark);
void PCUartDataDealProcess(void);

#ifdef	__cplusplus
}
#endif

#endif	/* APP_PCUSART_PROTOCOL_H */

