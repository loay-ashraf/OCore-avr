/**********************************************************************
*
* File:         HC_SR04.h
*
* Author(s):    Loay Ashraf <loay.ashraf.96@gmail.com>
*
* Date created: 11/06/2021
*
* Description:  contains definitions, typedefs and function
*               declarations for HC-SR04 controller module.
*
**********************************************************************/

#ifndef HC_SR04_H_
#define HC_SR04_H_

/*------------------------------INCLUDES-----------------------------*/

#include "service/include/std_types.h"

/*-------------------DEFINITIONS AND CONFIGURATIONS------------------*/

#define HC_SR04_PULSE_WIDTH         10
#define HC_SR04_DITANCE_CONSTANT    0.01715    

/*------------------------FUNCTION DECLARATIONS----------------------*/

uint16_t HC_SR04_read(void);

#endif /* HC_SR04_H_ */
