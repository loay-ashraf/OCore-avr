/**********************************************************************
*
* File:         ultrasonic.h
*
* Author(s):    Loay Ashraf <loay.ashraf.96@gmail.com>
*
* Date created: 11/06/2021
*
* Description:  contains function declarations for Ultrasonic module.
*
**********************************************************************/

#ifndef ULTRASONIC_H_
#define ULTRASONIC_H_

/*------------------------------INCLUDES-----------------------------*/

#include "service/include/std_types.h"

/*------------------------FUNCTION DECLARATIONS----------------------*/

uint16_t Ultrasonic_readcm(void);
uint8_t Ultrasonic_readm(void);
uint8_t Ultrasonic_readin(void);
uint8_t Ultrasonic_readft(void);

#endif /* ULTRASONIC_H_ */
