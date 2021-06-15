/*
 * m16_interrupt.h
 *
 *  Created on: 15 Jun 2021
 *      Author: Loay Ashraf
 */

#ifndef M16_INTERRUPT_H_
#define M16_INTERRUPT_H_

/*------------------------------INCLUDES-----------------------------*/

#include <avr/interrupt.h>

/*-------------------------------MACROS------------------------------*/

#define ENABLE_GLOBAL_INTERRUPTS	sei()
#define DISABLE_GLOBAL_INTERRUPTS	cli()

#endif /* INTERRUPT_H_ */
