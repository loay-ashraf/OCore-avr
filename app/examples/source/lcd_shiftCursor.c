/*
 * lcd_shiftCursor.c
 *
 * Created: 24/09/2019 12:58:00 AM
 *  Author: Loay Ashraf
 */ 

#include "hal/ecu/lcd/api/lcd.h"
#include "hal/mcu/sys/delay.h"

void ex_lcd_shiftCursor(void){
	
	uint8_t counter;		/* general purpose counter */
	
	LCD_init(TRUE,TRUE,TRUE);	/* initialize LCD interface */
	
	DELAY_MS(1000);
	
	/************************************/
	/* shift cursor right across screen */
	/************************************/
	
	for(counter=0;counter<LCD_COLUMNS*LCD_ROWS;counter++){
		
		DELAY_MS(1000);
		LCD_shiftCursor(LCD_RIGHT);
		
	}
	
	DELAY_MS(1000);
	
	/***********************************/
	/* shift cursor left across screen */
	/***********************************/
	
	for(counter=0;counter<LCD_COLUMNS*LCD_ROWS;counter++){
		
		DELAY_MS(1000);
		LCD_shiftCursor(LCD_LEFT);
		
	}
	
	DELAY_MS(1000);
	
	/*********************************/
	/* shift cursor up across screen */
	/*********************************/
	
	for(counter=0;counter<LCD_ROWS;counter++){
		
		DELAY_MS(1000);
		LCD_shiftCursor(LCD_UP);
		
	}
	
	DELAY_MS(1000);
	
	/***********************************/
	/* shift cursor down across screen */
	/***********************************/
	
	for(counter=0;counter<LCD_ROWS;counter++){
		
		DELAY_MS(1000);
		LCD_shiftCursor(LCD_DOWN);
		
	}
	
	DELAY_MS(1000);
	
}