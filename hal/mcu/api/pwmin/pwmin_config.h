/**********************************************************************
*
* File:         pwmin_config.h
*
* Author(s):	Loay Ashraf <loay.ashraf.96@gmail.com>
*
* Date created: 11/09/2019
*
* Description:	contains definitions and configurations for
*               PWMIN module.
*
**********************************************************************/

#ifndef PWM_IN_CONFIG_H_
#define PWM_IN_CONFIG_H_

/*-------------------DEFINITIONS AND CONFIGURATIONS------------------*/

#define PWMIN_PULSE_TIMER		TIMER2_M
#define PWMIN_PULSE_TIMER_MODE	T_NORMAL
#define PWMIN_PULSE_TIMER_PRE	T_DIV1024

#define PWMIN_FREQ_TIMER		TIMER1_M
#define PWMIN_FREQ_TIMER_MODE	T16_NORMAL
#define PWMIN_FREQ_TIMER_PRE	T16_EXT_RISING
#define PWMIN_FREQ_TIMER_PIN	TIMER1_T_PIN

#endif /* PWM_IN_CONFIG_H_ */