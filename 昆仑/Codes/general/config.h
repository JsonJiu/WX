/* 
 * File:   config.h
 * Author: wb
 *
 * Created on December 8, 2015, 9:28 AM
 */

#ifndef CONFIG_H
#define	CONFIG_H

#ifdef	__cplusplus
extern "C" {
#endif
#include <string.h>
#include <stdio.h>

#include "timer_manage.h"
#include "user_manage.h"
#include "task_manage.h"
  
#include    <p24Fxxxx.h>
#include "bsp_mc308.h"
#include "lib_public.h"
#include "temp.h"

#include "bord_config.h"  // ADD BY MJL

#include "drv_uart.h"
#include "app_MeterUsart_protocol.h"
#include "app_pcusart_protocol.h"
#include "app_autocheck.h"
#include "app_ina219.h"

/*=============================================================================*/


#ifdef	__cplusplus
}
#endif

#endif	/* CONFIG_H */

