/**********************************************************************
*
* File:         m16_wdt.h
*
* Author(s):    Loay Ashraf <loay.ashraf.96@gmail.com>
*
* Date created: 17/06/2021
*
* Description:  contains definitions, configurations, typedefs
*               and function declarations for watchdog timer module.
*
**********************************************************************/ 

#ifndef M16_WDT_H_
#define M16_WDT_H_

/*------------------------------INCLUDES-----------------------------*/

#include "service/include/std_types.h"

/*--------------------------------ENUMS------------------------------*/

typedef enum{WD_16,WD_32,WD_65,WD_130,WD_260,WD_520,WD_1000,WD_2100}wdttimeout_t;

/*------------------------FUNCTION DECLARATIONS----------------------*/    

void wdt_enable(wdttimeout_t a_wdtTimeout);
void wdt_disable(void);

#endif /* M16_WDT_H_ */
