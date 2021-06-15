/**********************************************************************
*
* File:         softreset.c
*
* Author(s):    Loay Ashraf <loay.ashraf.96@gmail.com>
*
* Date created: 13/09/2019
*
* Description:  contains function definitions for software reset
*               module.
*
**********************************************************************/

/*------------------------------INCLUDES-----------------------------*/

#include "softreset.h"
#include "hal/mcu/peripheral/wdt/wdt.h"

/*-----------------------FUNCTION DEFINITIONS------------------------*/

/**********************************************************************
*
* Function:    softReset
*
* Description: Initiates MCU reset by enabling watchdog timer 
*              and entering infinite while loop.
*
* Notes:
*
* Returns:     None.
*
**********************************************************************/

void softReset(void){
    
    wdt_enable(WD_16);
    while(1);
    
}
