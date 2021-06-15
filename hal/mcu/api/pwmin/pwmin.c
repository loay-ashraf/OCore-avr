/**********************************************************************
*
* File:         pwmin.c
*
* Author(s):    Loay Ashraf <loay.ashraf.96@gmail.com>
*
* Date created: 11/09/2019
*
* Description:	contains function definitions for PWMIN module.
*
**********************************************************************/ 

/*------------------------------INCLUDES-----------------------------*/

#include "pwmin.h"
#include "pwmin_config.h"
#include "hal/mcu/peripheral/gpio.h"
#include "hal/mcu/peripheral/timer/timer.h"
#include "hal/mcu/peripheral/timer16/timer16.h"
#include "hal/mcu/sys/delay.h"

/*--------------------------GLOBAL VARIABLES-------------------------*/

/**********************************************************************
*
* Variable:    g_timeWindow
*
* Description: Stores current time window width (in ms) for frequency
*              measurement.
*
* Notes:
*
* Scope:       pwmin.c.
*
**********************************************************************/

static uint16_t g_timeWindow = 100;

/*------------------------FUNCTION PROTOTYPES------------------------*/

static void delayVar(uint16_t delay);

/*-----------------------FUNCTION DEFINITIONS------------------------*/

/**********************************************************************
*
* Function:    PWMIN_getPulseWidth
*
* Description: Gets pulse width (in us) measured via specific pin
*              with specific trigger edge (rising or falling).
*
* Notes:
*
* Returns:     Pulse width in microseconds (type: uint16_t).
*
**********************************************************************/

uint16_t PWMIN_getPulseWidth(pin_t a_pin, uint8_t a_triggerEdge){
	
	if(a_triggerEdge == PWMIN_RISING){

		while(gpio_readPin(a_pin));
		while(!gpio_readPin(a_pin));
		
		timer_setMode(PWMIN_PULSE_TIMER,PWMIN_PULSE_TIMER_MODE);
		timer_start(PWMIN_PULSE_TIMER,PWMIN_PULSE_TIMER_PRE);

		while(gpio_readPin(a_pin));

		timer_stop(PWMIN_PULSE_TIMER);

	}else if(a_triggerEdge == PWMIN_FALLING){

		while(!gpio_readPin(a_pin));
		while(gpio_readPin(a_pin));

		timer_setMode(PWMIN_PULSE_TIMER,PWMIN_PULSE_TIMER_MODE);
		timer_start(PWMIN_PULSE_TIMER,PWMIN_PULSE_TIMER_PRE);

		while(!gpio_readPin(a_pin));

		timer_stop(PWMIN_PULSE_TIMER);

	}

	return timer_getTCNT(TIMER2_M)*(1024.0/F_CPU_M);
}

/**********************************************************************
*
* Function:    PWMIN_getFreqHZ
*
* Description: Gets frequency (in HZ) of input square wave.
*
* Notes:
*
* Returns:     Square wave frequency in hertz (type: float).
*
**********************************************************************/

float PWMIN_getFreqHZ(void){
	
	return PWMIN_getFreqKHZ()*1000;
	
}

/**********************************************************************
*
* Function:    PWMIN_getFreqKHZ
*
* Description: Gets frequency (in KHZ) of input square wave.
*
* Notes:       This function is implemented to be recursive
*              which makes this implementation a violation to 
*              MISRA-C 2004 mandatory rules.
*              An alternative implementation is planned for future
*              releases.
*
* Returns:     Square wave frequency in kilo-hertz (type: float).
*
**********************************************************************/

float PWMIN_getFreqKHZ(void){
	
	timer16_setMode(PWMIN_FREQ_TIMER,PWMIN_FREQ_TIMER_MODE);
	timer16_start(PWMIN_FREQ_TIMER,PWMIN_FREQ_TIMER_PRE);
	delayVar(g_timeWindow);
	timer16_stop(PWMIN_FREQ_TIMER);
	
	if(timer16_getTCNT(PWMIN_FREQ_TIMER) > 65400 || timer16_checkOverflow(PWMIN_FREQ_TIMER)){
		
		if(g_timeWindow == 1){
			return 0xFFFF;
		}else{
			g_timeWindow /= 10.0;
			return PWMIN_getFreqKHZ();
		}
		
	}else if(timer16_getTCNT(PWMIN_FREQ_TIMER) < 100){
		
		if(g_timeWindow == 1000){
			
			if(timer16_getTCNT(PWMIN_FREQ_TIMER) == 0)
				return 0;
			else
				return 0xFFFF;
				
		}else{
			
			g_timeWindow *= 10.0;
			return PWMIN_getFreqKHZ();
			
		}
		
	}else{
		
		return (float)timer16_getTCNT(PWMIN_FREQ_TIMER)/(float)g_timeWindow;
	
	}
	
}

/**********************************************************************
*
* Function:    PWMIN_getFreqMHZ
*
* Description: Gets frequency (in MHZ) of input square wave.
*
* Notes:
*
* Returns:     Square wave frequency in mega-hertz (type: float).
*
**********************************************************************/

float PWMIN_getFreqMHZ(void){
	
	return PWMIN_getFreqKHZ()/1000.0;
	
}

/***********************************************************************/
/* removed duty cycle acquisition for instability and race conditions. */
/* using DAC with voltage buffer and getting duty cycle via ADC is a   */
/* much more reliable and stable method!                               */
/***********************************************************************/

/**********************************************************************
*
* Function:    delayVar
*
* Description: Halts CPU cycles for a number of milliseconds.
*
* Notes:
*
* Returns:     None.
*
**********************************************************************/
	
static void delayVar(uint16_t a_ms){
	
	while(a_ms--)
		DELAY_MS(1);

}
