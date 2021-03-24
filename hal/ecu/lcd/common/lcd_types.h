/*
 * lcd_types.h
 *
 * Created: 29/09/2019 06:52:57 AM
 *  Author: Loay Ashraf
 */ 


#ifndef LCD_TYPES_H_
#define LCD_TYPES_H_

#include "lcd_config.h"
#include "hal/mcu/sys/std_types.h"

/*------------TYPEDEF LCD OBJECT STRUCT, SHIFT AND SCROLL DIRECTION ENUMS------------*/

typedef enum{LCD_CCHAR0,LCD_CCHAR1,LCD_CCHAR2,LCD_CCHAR3,LCD_CCHAR4,LCD_CCHAR5,LCD_CCHAR6,LCD_CCHAR7}lcdcustomcharacter_t;
typedef enum{LCD_LEFT,LCD_RIGHT,LCD_UP,LCD_DOWN}lcddirection_t;
typedef enum{LCD_BASE10=10,LCD_BASE16=16,LCD_BASE2=2}lcdnumberbase_t;

typedef struct{
	
	uint8_t row;
	uint8_t column;
	
}lcdposition_t;

typedef struct{
	
	ubyte_t value;
	
	struct{
		
		bool_t	cursorvisible			:1;
		bool_t	cursorblinking			:1;
		bool_t  leftToRight				:1;
		bool_t	cursorpositionupdate	:1;
		bool_t	framebufferupdate		:1;
		ubyte_t :3;
		
	};
	
}lcdstatusregister_t;

typedef struct{
	
	char frameBuffer[LCD_ROWS][LCD_COLUMNS+1];
	lcdposition_t primaryCursorPosition;
	lcdposition_t secondaryCursorPosition;
	lcdstatusregister_t statusRegister;
	
}lcd_t;


#endif /* LCD_TYPES_H_ */