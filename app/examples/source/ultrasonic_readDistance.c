/*
 * ultrasonic_readDistance.c
 *
 * Created: 11/06/2021 08:43:27 AM
 *  Author: Loay Ashraf
 */ 

#include "app/examples/include/examples.h"
#include "hal/ecu/lcd/api/lcd.h"
#include "hal/ecu/distance/ultrasonic/api/ultrasonic.h"
#include "hal/mcu/sys/delay.h"

void ex_ultrasonic_readDistance(void){
	
	uint16_t distance;
	
	/****************************/
	/* initialize LCD interface */
	/****************************/
	
	LCD_init(TRUE,FALSE,FALSE,TRUE);
	LCD_setCursorPosition(0,0);
	LCD_puts("Distance in cm: ");
	LCD_setCursorPosition(1,0);
	LCD_puts("Distance in m: ");
	LCD_setCursorPosition(2,0);
	LCD_puts("Distance in in: ");
	LCD_setCursorPosition(3,0);
	LCD_puts("Distance in ft: ");
	
	while(1){	/* loop forever */
		
		distance = Ultrasonic_readcm();
		LCD_clearRow(0,16);
		LCD_setCursorPosition(0,16);
		LCD_puti(distance,LCD_BASE10);
		
		distance = Ultrasonic_readm();
		LCD_clearRow(1,15);
		LCD_setCursorPosition(1,15);
		LCD_puti(distance,LCD_BASE10);
		
		distance = Ultrasonic_readin();
		LCD_clearRow(2,16);
		LCD_setCursorPosition(2,16);
		LCD_puti(distance,LCD_BASE10);
		
		distance = Ultrasonic_readft();
		LCD_clearRow(3,16);
		LCD_setCursorPosition(3,16);
		LCD_puti(distance,LCD_BASE10);
		
		DELAY_MS(1000);
		
	}
	
}
