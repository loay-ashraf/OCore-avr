/**********************************************************************
*
* File:         interrupt.h
*
* Author(s):	Loay Ashraf <loay.ashraf.96@gmail.com>
*
* Date created: 30/07/2019
*
* Description:	contains global interrupts macros.
*
**********************************************************************/

#ifndef INTERRUPT_H_
#define INTERRUPT_H_

/*------------------------------INCLUDES-----------------------------*/

#include <avr/interrupt.h>

/*-------------------------------MACROS------------------------------*/

#define ENABLE_GLOBAL_INTERRUPTS	sei()
#define DISABLE_GLOBAL_INTERRUPTS	cli()

#endif /* INTERRUPT_H_ */