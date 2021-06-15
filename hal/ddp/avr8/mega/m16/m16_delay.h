/*
 * m16_delay.h
 *
 *  Created on: 15 Jun 2021
 *      Author: Loay Ashraf
 */

#ifndef M16_DELAY_H_
#define M16_DELAY_H_

/*------------------------------INCLUDES-----------------------------*/

#include "hal/mcu/sys/cpu_config.h"
#include <util/delay.h>

/*-------------------------------MACROS------------------------------*/

#define DELAY_MS(MS)	_delay_ms(MS)
#define DELAY_US(US)	_delay_us(US)

#endif /* M16_DELAY_H_ */
