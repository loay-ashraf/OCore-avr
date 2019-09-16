/*
 * sleep.h
 *
 * Created: 5/12/2019 5:34:59 PM
 *  Author: Loay Ashraf
 */ 


#ifndef SLEEP_H_
#define SLEEP_H_

#include <avr/sleep.h>

#define SET_SLEEP_MODE(MODE)	set_sleep_mode(MODE)
#define SLEEP_MODE				sleep_mode()
#define SLEEP_ENABLE			sleep_enable()
#define SLEEP_DISABLE			sleep_disable()
#define SLEEP_CPU				sleep_cpu()

#endif /* SLEEP_H_ */