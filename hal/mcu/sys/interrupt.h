/*
 * interrupt.h
 *
 * Created: 30/07/2019 03:33:36 PM
 *  Author: Loay Ashraf
 */ 


#ifndef INTERRUPT_H_
#define INTERRUPT_H_

#include <avr/interrupt.h>

#define ENABLE_GLOBAL_INTERRUPTS	sei()
#define DISABLE_GLOBAL_INTERRUPTS	cli()


#endif /* INTERRUPT_H_ */