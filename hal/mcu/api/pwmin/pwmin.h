/**********************************************************************
*
* File:         pwmin.h
*
* Author(s):	Loay Ashraf <loay.ashraf.96@gmail.com>
*
* Date created: 11/09/2019
*
* Description:	contains typedefs and function declarations for
*               PWMIN module.
*
**********************************************************************/

#ifndef PWM_IN_H_
#define PWM_IN_H_

/*------------------------------INCLUDES-----------------------------*/

#include "hal/mcu/sys/io.h"
#include "service/include/std_types.h"

/*--------------------------------ENUMS------------------------------*/

typedef enum{PWMIN_FALLING,PWMIN_RISING}pwminedge_t;

/*------------------------FUNCTION DECLARATIONS----------------------*/

uint16_t PWMIN_getPulseWidth(pin_t a_pin, uint8_t Edge);
float PWMIN_getFreqHZ(void);
float PWMIN_getFreqKHZ(void);
float PWMIN_getFreqMHZ(void);

#endif /* PWM_IN_H_ */
