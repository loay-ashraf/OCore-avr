/*
 * wdt.h
 *
 * Created: 04/09/2019 05:22:44 PM
 *  Author: Loay Ashraf
 */ 


#ifndef WDT_H_
#define WDT_H_

#include "hal/mcu/io/io_defs.h"
#include "hal/mcu/sys/std_types.h"

typedef enum{WD_16,WD_32,WD_65,WD_130,WD_260,WD_520,WD_1000,WD_2100}wdttimeout_t;

void wdt_enable(wdttimeout_t a_wdtTimeout);
void wdt_disable(void);

#endif /* WDT_H_ */