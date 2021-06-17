/**********************************************************************
*
* File:         wdt.h
*
* Author(s):    Loay Ashraf <loay.ashraf.96@gmail.com>
*
* Date created: 04/09/2019
*
* Description:  contains definitions, configurations, typedefs
*               and function declarations for watchdog timer module.
*
**********************************************************************/ 

#ifndef WDT_H_
#define WDT_H_
 
#include "hal/ddp/ddp_interface.h"
#include PATH_TO_WDT

#define WDT_ENABLE(WDT_TIMEOUT)    wdt_enable(WDT_TIMEOUT)
#define WDT_DISABLE                wdt_disable()

#endif /* WDT_H_ */
