/*
 * dac_fade.c
 *
 * Created: 15/09/2019 11:49:21 PM
 *  Author: Loay Ashraf
 */ 

#include "../include/examples.h"

void ex_dac_fade(void){
	
	int8_t duty;	/* counter variable */
	
	while(1){		/* loop forever */
		
		/************************************************/
		/* first loop to fade LED up to full brightness */
		/************************************************/
		
		for(duty=0;duty<=100;duty++){
			
			dac_setDuty(DA_CH0,duty);
			DELAY_MS(30);
			
		}
		
		/***************************************************/
		/* second loop to fade LED down to zero brightness */
		/***************************************************/
		
		for(duty=100;duty>=0;duty--){
			
			dac_setDuty(DA_CH0,duty);
			DELAY_MS(30);
			
		}
	}
	
}