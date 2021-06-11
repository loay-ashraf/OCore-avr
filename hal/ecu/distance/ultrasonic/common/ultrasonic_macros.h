/**********************************************************************
*
* File:         ultrasonic_macros.h
*
* Author(s):	Loay Ashraf <loay.ashraf.96@gmail.com>
*
* Date created: 11/06/2021
*
* Description:	contains function-like adapter macros for Ultrasonic
*               module.
*
**********************************************************************/

#ifndef ULTRASONIC_MACROS_H_
#define ULTRASONIC_MACROS_H_

/*------------------------------INCLUDES-----------------------------*/

#if (ULTRASONIC_CONTROLLER == ULTRASONIC_HC_SRO4)

#include "hal/ecu/distance/ultrasonic/driver/HC-SR04/HC_SR04.h"

#endif

/*-------------------DEFINITIONS AND CONFIGURATIONS------------------*/

#if defined(HC_SR04_H_)

	#define ULTRASONIC_READCM	HC_SR04_read()

#endif	

#endif /* ULTRASONIC_MACROS_H_ */