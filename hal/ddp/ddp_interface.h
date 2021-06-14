/*
 * ddp_interface.h
 *
 * Created: 13/06/2021 07:01:17 AM
 *  Author: Loay Ashraf
 */


#ifndef DDP_INTERFACE_H_
#define DDP_INTERFACE_H_

#if defined (__AVR__)
	#if defined (__AVR_ATmega16__)
 		#define PATH_TO_AC		"hal/ddp/avr8/mega/m16/ac/m16_ac.h"
		#define PATH_TO_ADC		"hal/ddp/avr8/mega/m16/adc/m16_adc.h"
		#define PATH_TO_GPIO	"hal/ddp/avr8/mega/m16/gpio/m16_gpio.h"
	#endif
#else
	#error "Platform not supported"
#endif

#endif /* DDP_INTERFACE_H_ */
