/**********************************************************************
*
* File:         map.h
*
* Author(s):	Loay Ashraf <loay.ashraf.96@gmail.com>
*
* Date created: 11/06/2021
*
* Description:	contains typedefs and function declarations for
*               common map functions.
*
**********************************************************************/

#ifndef MAP_H_
#define MAP_H_

/*------------------------------INCLUDES-----------------------------*/

#include "service/include/std_types.h"

/*------------------------FUNCTION DECLARATIONS----------------------*/

uint16_t map(uint8_t a_input, uint16_t a_inputMin, uint16_t a_inputMax, uint16_t a_outputMin, uint16_t a_outputMax);

#endif /* MAP_H_ */