/**********************************************************************
*
* File:         io_defs.h
*
* Author(s):	Loay Ashraf <loay.ashraf.96@gmail.com>
*
* Date created: 05/05/2019
*
* Description:	contains port, pin and peripherals definitions.
*
**********************************************************************/

#ifndef IO_H_
#define IO_H_

/*------------------------------INCLUDES-----------------------------*/

#include <avr/io.h>

/*--------------------------------ENUMS------------------------------*/

typedef enum{PORTA_M,PORTB_M=0x08,PORTC_M=0x10,PORTD_M=0x18}port_t;
typedef enum{PA0_M,PA1_M,PA2_M,PA3_M,PA4_M,PA5_M,PA6_M,PA7_M,PB0_M,PB1_M,PB2_M,PB3_M,PB4_M,PB5_M,PB6_M,PB7_M,
			PC0_M,PC1_M,PC2_M,PC3_M,PC4_M,PC5_M,PC6_M,PC7_M,PD0_M,PD1_M,PD2_M,PD3_M,PD4_M,PD5_M,PD6_M,PD7_M}pin_t;
typedef enum{TIMER0_M,TIMER2_M}timer_t;
typedef enum{TIMER1_M,TIMER3_M}timer16_t;

/*--------------------------FUNCTION POINTERS------------------------*/
	
typedef void (*ISRcallback_t) (void);	

/*-------------------DEFINITIONS AND CONFIGURATIONS------------------*/

#define I_BIT	0x07

#endif /* IO_H_ */