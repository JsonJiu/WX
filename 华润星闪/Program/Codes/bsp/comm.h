/* 
 * File:   comm.h
 * Author: wb
 *
 * Created on December 22, 2015, 5:16 PM
 */

#ifndef COMM_H
#define	COMM_H

#ifdef	__cplusplus
extern "C" {
#endif

unsigned char BcdToHex(unsigned char bcd);
unsigned short CheckAndFinishCommand(unsigned char *pCmd,unsigned short len);
unsigned short HexToAscii_Space(unsigned char* data_hex,unsigned char* data_ascii, unsigned short len_hex);


#ifdef	__cplusplus
}
#endif

#endif	/* COMM_H */

