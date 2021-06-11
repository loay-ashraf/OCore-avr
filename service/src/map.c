/**********************************************************************
*
* File:         map.c
*
* Author(s):    Loay Ashraf <loay.ashraf.96@gmail.com>
*
* Date created: 11/06/2021
*
* Description:	contains function definitions for common map functions.
*
**********************************************************************/

/*------------------------------INCLUDES-----------------------------*/

#include "service/include/map.h"

/*-----------------------FUNCTION DEFINITIONS------------------------*/

/**********************************************************************
*
* Function:    map
*
* Description: Maps an integer value from a range to another one.
*
* Notes:
*
* Returns:     Mapped value (type: uint16_t).
*
**********************************************************************/

uint16_t map(uint8_t a_input, uint16_t a_inputMin, uint16_t a_inputMax, uint16_t a_outputMin, uint16_t a_outputMax){
	
	if(a_input < a_inputMin)
		a_input = a_inputMin;
	else if(a_input > a_inputMax)
		a_input = a_inputMax;
	
	float slope = (float)(a_outputMax-a_outputMin)/(float)(a_inputMax-a_inputMin);
	uint16_t output = a_outputMin+slope*(a_input-a_inputMin);
	
	return output;
	
}