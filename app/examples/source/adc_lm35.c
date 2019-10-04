/*
 * adc_lm35.c
 *
 * Created: 15/09/2019 07:17:06 PM
 *  Author: Loay Ashraf
 */ 

#include "app/examples/include/examples.h"
#include "hal/ecu/lcd/api/lcd.h"
#include "hal/mcu/hw/driver/adc/adc.h"
#include "hal/mcu/sys/delay.h"

void ex_adc_lm35(void){
	
	uint16_t adcValue;		// variable to hold adc value
	
	/*************************************/
	/* initialize LCD and ADC interfaces */
	/*************************************/
	
	adc_config(AD_DIV16,AD_AVCC,FALSE);
	adc_enable();
	LCD_init(TRUE,FALSE,FALSE,TRUE);
	
	while(1){		/* loop forever */
		
		adcValue = adc_read(AD_CH0);		/* read analog value from channel zero (PA0) */
		
		/****************************************/
		/* display temperature in Celsius scale */
		/****************************************/
		
		LCD_setCursorPosition(0,0);
		LCD_putf(adcValue*0.488);
		LCD_putc(' ');
		LCD_putc(223);
		LCD_putc('C');
		
		/*******************************************/
		/* display temperature in Fahrenheit scale */
		/*******************************************/
		
		LCD_setCursorPosition(1,0);
		LCD_putf((adcValue*0.488*1.8)+32);
		LCD_putc(' ');
		LCD_putc(223);
		LCD_putc('F');
		DELAY_MS(500);
		
	}
}