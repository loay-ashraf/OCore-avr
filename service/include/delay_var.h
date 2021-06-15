/**********************************************************************
*
* File:         servo.h
*
* Author(s):    Loay Ashraf <loay.ashraf.96@gmail.com>
*
* Date created: 11/06/2021
*
* Description:  contains typedefs and function declarations for
*               variable delay functions.
*
**********************************************************************/

#ifndef DELAY_VAR_H_
#define DELAY_VAR_H_

/*------------------------------INCLUDES-----------------------------*/

#include "service/include/std_types.h"

/*------------------------FUNCTION DECLARATIONS----------------------*/

void delayVarms(uint16_t a_ms);
void delayVarus(uint16_t a_us);

#endif /* DELAY_VAR_H_ */
