/*
 * ac.h
 *
 * Created: 04/09/2019 06:08:10 PM
 *  Author: Loay Ashraf
 */ 


#ifndef AC_H_
#define AC_H_

#include "../../../io/io_defs.h"
#include "../../../io/io_macros.h"
#include "../../../sys/interrupt.h"
#include "../gpio/gpio.h"

#define AC_IN0_PIN		PB2_M
#define AC_IN1_PIN		PB3_M

typedef enum{AC_TOGGLE,AC_FALLING_EDGE=0x02,AC_RISING_EDGE}acinterruptmode_t;

void ac_enable(void);
void ac_disable(void);
void ac_enableBandGap(void);
void ac_disableBandGap(void);
void ac_enableInterrupt(void);
void ac_disableInterrupt(void);
void ac_setInterruptMode(acinterruptmode_t a_acInterruptMode);
void ac_setISRCallback(ISRcallback_t a_acISRCalback);

#endif /* AC_H_ */