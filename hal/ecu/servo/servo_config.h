/*
 * servo_config.h
 *
 * Created: 07/09/2019 01:28:34 PM
 *  Author: Loay Ashraf
 */ 


#ifndef SERVO_CONFIG_H_
#define SERVO_CONFIG_H_

#include "hal/mcu/sys/cpu_config.h"

#define SERVO_TIMER				TIMER1_M
#define SERVO_TIMER_MODE		T16_FAST_PWM_ICR
#define SERVO_TIMER_OCMODE		T16_CLEAR
#define SERVO_TIMER_PRESCALER	T16_DIV8
#define SERVO_TIMER_TOP			((F_CPU/400.0)-1)
#define SERVO_TIMER_MIN			(SERVO_TIMER_TOP*0.03)
#define SERVO_TIMER_MAX			(SERVO_TIMER_TOP*0.108)
#define SERVO_SPEED_MAX			1000
#define SERVO_CH0_POSITION_MIN	0
#define SERVO_CH0_POSITION_MAX	180
#define SERVO_CH1_POSITION_MIN	0
#define SERVO_CH1_POSITION_MAX	180


#endif /* SERVO_CONFIG_H_ */