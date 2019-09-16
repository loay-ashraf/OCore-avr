/*
 * pwm_in.c
 *
 * Created: 11/09/2019 12:37:15 PM
 *  Author: Loay Ashraf
 */ 
#include "pwmin.h"

static uint16_t gateWindow = 100;

static void delayVar(uint16_t delay);

uint16_t PWMIN_getPulseWidth(pin_t a_pin, uint8_t a_triggerEdge){
	
	if(a_triggerEdge == PWMIN_RISING){

		while (gpio_readPin(a_pin));
		while (!gpio_readPin(a_pin));
		
		timer_setMode(PWMIN_PULSE_TIMER,PWMIN_PULSE_TIMER_MODE);
		timer_start(PWMIN_PULSE_TIMER,PWMIN_PULSE_TIMER_PRE);

		while (gpio_readPin(a_pin));

		timer_stop(PWMIN_PULSE_TIMER);

	}else if(a_triggerEdge == PWMIN_FALLING){

		while (!gpio_readPin(a_pin));
		while (gpio_readPin(a_pin));

		timer_setMode(PWMIN_PULSE_TIMER,PWMIN_PULSE_TIMER_MODE);
		timer_start(PWMIN_PULSE_TIMER,PWMIN_PULSE_TIMER_PRE);

		while (!gpio_readPin(a_pin));

		timer_stop(PWMIN_PULSE_TIMER);

	}

	return timer_getTCNT(TIMER2_M)*(1024.0/F_CPU_M);
}

float PWMIN_getFreqHZ(void){
	
	return PWMIN_getFreqKHZ()*1000;
	
}

float PWMIN_getFreqKHZ(void){
	
	timer16_setMode(PWMIN_FREQ_TIMER,PWMIN_FREQ_TIMER_MODE);
	timer16_start(PWMIN_FREQ_TIMER,PWMIN_FREQ_TIMER_PRE);
	delayVar(gateWindow);
	timer16_stop(PWMIN_FREQ_TIMER);
	
	if(timer16_getTCNT(PWMIN_FREQ_TIMER) > 65400 || timer16_checkOverflow(PWMIN_FREQ_TIMER)){
		
		if(gateWindow == 1){
			return 0xFFFF;
		}else{
			gateWindow /= 10.0;
			return PWMIN_getFreqKHZ();
		}
		
	}else if(timer16_getTCNT(PWMIN_FREQ_TIMER) < 100){
		
		if(gateWindow == 1000){
			
			if(timer16_getTCNT(PWMIN_FREQ_TIMER) == 0)
				return 0;
			else
				return 0xFFFF;
				
		}else{
			
			gateWindow *= 10.0;
			return PWMIN_getFreqKHZ();
			
		}
		
	}else{
		
		return (float)timer16_getTCNT(PWMIN_FREQ_TIMER)/(float)gateWindow;
	
	}
	
}

float PWMIN_getFreqMHZ(void){
	
	return PWMIN_getFreqKHZ()/1000.0;
	
}

/************************************************************************/
/*removed duty cycle acquisition for instability and race conditions	*/
/*using DAC with voltage buffer and getting duty cycle via ADC is a     */
/*much more reliable and stable method!                                 */
/************************************************************************/
	
 static void delayVar(uint16_t a_ms){
	
	while(a_ms--)
		DELAY_MS(1);

}