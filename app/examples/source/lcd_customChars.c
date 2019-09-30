/*
 * lcd_customChars.c
 *
 * Created: 12/09/2019 06:37:07 PM
 *  Author: Loay Ashraf
 */ 

#include "app/examples/include/examples.h"
#include "hal/ecu/lcd/api/lcd.h"

static ubyte_t Heart[] = {0x00,0x0A,0x1F,0x1F,0x0E,0x04,0x00,0x00};
static ubyte_t Bell[] = {0x04,0x0E,0x0E,0x0E,0x1F,0x00,0x04,0x00};
static ubyte_t Alien[] = {0x1F,0x15,0x1F,0x1F,0x0E,0x0A,0x1B,0x00};
static ubyte_t Check[] = {0x00,0x01,0x03,0x16,0x1C,0x08,0x00,0x00};
static ubyte_t Speaker[] = {0x01,0x03,0x0F,0x0F,0x0F,0x03,0x01,0x00};
static ubyte_t Sound[] = {0x01,0x03,0x05,0x09,0x09,0x0B,0x1B,0x18};
static ubyte_t Skull[] = {0x00,0x0E,0x15,0x1B,0x0E,0x0E,0x00,0x00};
static ubyte_t Lock[] = {0x0E,0x11,0x11,0x1F,0x1B,0x1B,0x1F,0x00};
	
void ex_lcd_customChars(void){
	
	LCD_init(FALSE,FALSE,TRUE);		/* initialize LCD interface */
	
	/********************************/
	/* define LCD custom characters */
	/********************************/
	
	LCD_defineCustomCharacter(LCD_CCHARACTER_ZERO,Heart);
	LCD_defineCustomCharacter(LCD_CCHARACTER_ONE,Bell);
	LCD_defineCustomCharacter(LCD_CCHARACTER_TWO,Alien);
	LCD_defineCustomCharacter(LCD_CCHARACTER_THREE,Check);
	LCD_defineCustomCharacter(LCD_CCHARACTER_FOUR,Speaker);
	LCD_defineCustomCharacter(LCD_CCHARACTER_FIVE,Sound);
	LCD_defineCustomCharacter(LCD_CCHARACTER_SIX,Skull);
	LCD_defineCustomCharacter(LCD_CCHARACTER_SEVEN,Lock);
	
	/***********************************/
	/* put custom characters on screen */
	/***********************************/
	
	#if (LCD_COLUMNS == 16)
	
		LCD_setCursorPosition(0,0);
		LCD_puts("Custom Character");
		
		LCD_setCursorPosition(1,0);
		LCD_putc(LCD_CCHARACTER_ZERO);
		LCD_setCursorPosition(1,2);
		LCD_putc(LCD_CCHARACTER_ONE);
		LCD_setCursorPosition(1,4);
		LCD_putc(LCD_CCHARACTER_TWO);
		LCD_setCursorPosition(1,6);
		LCD_putc(LCD_CCHARACTER_THREE);
		LCD_setCursorPosition(1,8);
		LCD_putc(LCD_CCHARACTER_FOUR);
		LCD_setCursorPosition(1,10);
		LCD_putc(LCD_CCHARACTER_FIVE);
		LCD_setCursorPosition(1,12);
		LCD_putc(LCD_CCHARACTER_SIX);
		LCD_setCursorPosition(1,14);
		LCD_putc(LCD_CCHARACTER_SEVEN);
		
	#elif (LCD_COLUMNS == 20)	
	
		LCD_setCursorPosition(1,2);
		LCD_puts("Custom Character");
		
		LCD_setCursorPosition(2,2);
		LCD_putc(LCD_CCHARACTER_ZERO);
		LCD_setCursorPosition(2,4);
		LCD_putc(LCD_CCHARACTER_ONE);
		LCD_setCursorPosition(2,6);
		LCD_putc(LCD_CCHARACTER_TWO);
		LCD_setCursorPosition(2,8);
		LCD_putc(LCD_CCHARACTER_THREE);
		LCD_setCursorPosition(2,10);
		LCD_putc(LCD_CCHARACTER_FOUR);
		LCD_setCursorPosition(2,12);
		LCD_putc(LCD_CCHARACTER_FIVE);
		LCD_setCursorPosition(2,14);
		LCD_putc(LCD_CCHARACTER_SIX);
		LCD_setCursorPosition(2,16);
		LCD_putc(LCD_CCHARACTER_SEVEN);
	
	#endif
	
}	