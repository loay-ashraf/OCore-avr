/*
 * delay.h
 *
 * Created: 5/5/2019 3:52:49 PM
 *  Author: Loay Ashraf
 */ 


#ifndef DELAY_H_
#define DELAY_H_

#include "cpu_config.h"
#include <util/delay.h>

#define DELAY_MS(MS)	_delay_ms(MS) 
#define DELAY_US(US)	_delay_us(US)

#endif /* DELAY_H_ */