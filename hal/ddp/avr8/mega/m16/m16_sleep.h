/*
 * m16_sleep.h
 *
 *  Created on: 15 Jun 2021
 *      Author: Loay Ashraf
 */

#ifndef M16_SLEEP_H_
#define M16_SLEEP_H_

/*------------------------------INCLUDES-----------------------------*/

#include <avr/sleep.h>

/*-------------------------------MACROS------------------------------*/

#define SET_SLEEP_MODE(MODE)    set_sleep_mode(MODE)
#define SLEEP_MODE              sleep_mode()
#define SLEEP_ENABLE            sleep_enable()
#define SLEEP_DISABLE           sleep_disable()
#define SLEEP_CPU               sleep_cpu()

#endif /* M16_SLEEP_H_ */
