/**********************************************************************
*
* File:         m16_wdt.c
*
* Author(s):    Loay Ashraf <loay.ashraf.96@gmail.com>
*
* Date created: 17/06/2021
*
* Description:  contains function definitions for watchdog timer
*               module.
*
**********************************************************************/

/*------------------------------INCLUDES-----------------------------*/

#include "m16_wdt.h"
#include "hal/mcu/sys/io.h"
#include "service/include/register_macros.h"

/*-----------------------FUNCTION DEFINITIONS------------------------*/

/**********************************************************************
*
* Function:    wdt_enable
*
* Description: Enables watchdog timer module.
*
* Notes:
*
* Returns:     None.
*
**********************************************************************/

void wdt_enable(wdttimeout_t a_wdtTimeout){
    
    SRI(WDTCR,a_wdtTimeout);
    SBI(WDTCR,WDE);
    
}

/**********************************************************************
*
* Function:    wdt_disable
*
* Description: Disables watchdog timer module.
*
* Notes:
*
* Returns:     None.
*
**********************************************************************/

void wdt_disable(void){
    
    SBI(WDTCR,WDTOE);
    CBI(WDTCR,WDE);
    
}
