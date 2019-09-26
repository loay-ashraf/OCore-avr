/*
 * lcd_scrollDisplay.c
 *
 * Created: 24/09/2019 12:45:17 AM
 *  Author: Loay Ashraf
 */ 

#include "hal/ecu/lcd/api/lcd.h"
#include "hal/mcu/sys/delay.h"

void ex_lcd_scrollDisplay(void){
	
	uint8_t counter;		/* general purpose counter */
	
	LCD_init(TRUE,TRUE);	/* initialize GLCD interface */
	
	/***************************************/
	/* print text to be scrolled on screen */
	/***************************************/
	
	#if (LCD_ROWS == 2)
	
		LCD_puts("Hello World!");
		LCD_setCursorPosition(1,0);
		LCD_puts("I'm 16x2 LCD");
		
	#elif (LCD_ROWS == 4)
		
		LCD_puts("Hello World!");
		LCD_setCursorPosition(1,0);
		LCD_puts("I'm 20x4 LCD");
		LCD_setCursorPosition(2,0);
		LCD_puts("I'm scrolling now");
		LCD_setCursorPosition(3,0);
		LCD_puts("Enjoy the show!");
		
	#endif	
	
	DELAY_MS(1000);
	
	/**************************************/
	/* scroll text on screen to the right */
	/**************************************/
	
	for(counter=0;counter<LCD_COLUMNS;counter++){
		
		DELAY_MS(1000);
		LCD_scrollDisplay(LCD_RIGHT,0,LCD_ROWS);
		
	}
	
	DELAY_MS(1000);
	
	/*************************************/
	/* scroll text on screen to the left */
	/*************************************/
	
	for(counter=0;counter<LCD_COLUMNS;counter++){
		
		DELAY_MS(1000);
		LCD_scrollDisplay(LCD_LEFT,0,LCD_ROWS);
		
	}
	
	DELAY_MS(1000);
	
	/*********************************/
	/* scroll text on screen upwards */
	/*********************************/
	
	for(counter=0;counter<LCD_ROWS;counter++){
		
		DELAY_MS(1000);
		LCD_scrollDisplay(LCD_UP,0,LCD_COLUMNS);
		
	}
	
	DELAY_MS(1000);
	
	/***********************************/
	/* scroll text on screen downwards */
	/***********************************/
	
	for(counter=0;counter<LCD_ROWS;counter++){
		
		DELAY_MS(1000);
		LCD_scrollDisplay(LCD_DOWN,0,LCD_COLUMNS);
		
	}
	
	DELAY_MS(1000);
	
}