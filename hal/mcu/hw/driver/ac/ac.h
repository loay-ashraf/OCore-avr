/**********************************************************************
*
* File:         ac.h
*
* Author(s):    Loay Ashraf <loay.ashraf.96@gmail.com>
*
* Date created: 04/09/2019
*
* Description:  contains definitions, configurations, typedefs
*               and function declarations for analog comparator module.
*
**********************************************************************/

#ifndef AC_H_
#define AC_H_

/*------------------------------INCLUDES-----------------------------*/

#include "hal/mcu/io/io_defs.h"

/*-------------------DEFINITIONS AND CONFIGURATIONS------------------*/

#define AC_IN0_PIN		PB2_M
#define AC_IN1_PIN		PB3_M

/*--------------------------------ENUMS------------------------------*/

typedef enum{AC_TOGGLE,AC_FALLING_EDGE=0x02,AC_RISING_EDGE}acinterruptmode_t;
	
/*------------------------FUNCTION DECLARATIONS----------------------*/	

void ac_enable(void);
void ac_disable(void);
void ac_enableBandGap(void);
void ac_disableBandGap(void);
void ac_enableInterrupt(void);
void ac_disableInterrupt(void);
void ac_setInterruptMode(acinterruptmode_t a_acInterruptMode);
void ac_setISRCallback(ISRcallback_t a_ISRCallback);

#endif /* AC_H_ */