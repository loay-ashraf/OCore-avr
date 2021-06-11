/**********************************************************************
*
* File:         delay_var.c
*
* Author(s):    Loay Ashraf <loay.ashraf.96@gmail.com>
*
* Date created: 11/06/2021
*
* Description:	contains function definitions for variable delay
*               functions.
*
**********************************************************************/

/*------------------------------INCLUDES-----------------------------*/

#include "service/include/delay_var.h"
#include "hal/mcu/sys/delay.h"

/*-----------------------FUNCTION DEFINITIONS------------------------*/

/**********************************************************************
*
* Function:    delayVarms
*
* Description: Halts CPU cycles for a number of milliseconds.
*
* Notes:
*
* Returns:     None.
*
**********************************************************************/

void delayVarms(uint16_t a_ms){
	
	while(a_ms--)
		DELAY_MS(1);
	
}

/**********************************************************************
*
* Function:    delayVarus
*
* Description: Halts CPU cycles for a number of microseconds.
*
* Notes:
*
* Returns:     None.
*
**********************************************************************/

void delayVarus(uint16_t a_us){
	
	while(a_us--)
		DELAY_US(1);
	
}