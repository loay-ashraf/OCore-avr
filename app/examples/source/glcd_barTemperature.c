/*
 * glcd_barTemperature.c
 *
 * Created: 28/09/2019 09:26:18 AM
 *  Author: Loay Ashraf
 */ 

#include "app/examples/include/examples.h"
#include "hal/ecu/glcd/api/glcd.h"
#include "hal/mcu/peripheral/adc/adc.h"
#include "hal/mcu/sys/delay.h"

void ex_glcd_barTemperature(void){
	
	uint8_t temp = 0;
	
	/**************************************/
	/* initialize GLCD and ADC interfaces */
	/**************************************/
	
	GLCD_init(TRUE);
	adc_config(AD_DIV16,AD_AVCC,FALSE);
	adc_enable();
	
	
	while(1){		/* loop forever */
		
		/**********************************************************/
		/* read ADC channel 0 and update graph bar every 1 second */
		/**********************************************************/
		
		temp = adc_read(AD_CH0)*0.448;
		GLCD_drawVerticalBar(GLCD_BAR1,temp,0,60);
		DELAY_MS(1000);
		
	}
	
}
