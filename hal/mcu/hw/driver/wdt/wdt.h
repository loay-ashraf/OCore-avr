/**********************************************************************
*
* File:         wdt.h
*
* Author(s):	Loay Ashraf <loay.ashraf.96@gmail.com>
*
* Date created: 04/09/2019
*
* Description:	contains definitions, configurations, typedefs
*               and function declarations for watchdog timer module.
*
**********************************************************************/ 

#ifndef WDT_H_
#define WDT_H_

/*------------------------------INCLUDES-----------------------------*/

#include "hal/mcu/io/io_defs.h"
#include "hal/mcu/sys/std_types.h"

/*--------------------------------ENUMS------------------------------*/

typedef enum{WD_16,WD_32,WD_65,WD_130,WD_260,WD_520,WD_1000,WD_2100}wdttimeout_t;

/*------------------------FUNCTION DECLARATIONS----------------------*/	

void wdt_enable(wdttimeout_t a_wdtTimeout);
void wdt_disable(void);

#endif /* WDT_H_ */