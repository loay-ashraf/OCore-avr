/** @file LCD.h
 *  @brief This driver supports interfacing of parallel interface character
 *	LCD (both 16x2 and 20x4); this file includes function prototypes for LCD.c
 *  @author Loay Ashraf
 *	@version 1.0.0
 *  @pre include delay.h and stdlib.h in micro_config.h
 *  @bug no known bugs
 *	@warning data mode has to be selected here
 *  @see micro_config.h
 */


#ifndef LCD_H_
#define LCD_H_

/*------------INCLUDE LCD CONFIGURATION AND REQUIRED HEADER FILES------------*/

#include "lcd_config.h"
#include "hal/mcu/sys/std_types.h"

/*------------TYPEDEF LCD OBJECT STRUCT, SHIFT AND SCROLL DIRECTION ENUMS------------*/

typedef enum{LCD_LEFT,LCD_RIGHT,LCD_UP,LCD_DOWN,LCD_NONE}lcddirection_t;
typedef enum{LCD_BASE10=10,LCD_BASE16=16,LCD_BASE2=2}lcdnumberbase_t;
	
typedef struct{
	
	ubyte_t row;
	ubyte_t column;
	
}lcdposition_t;

typedef struct{
	
	ubyte_t value;
	
	struct{
		
		bool_t	cursorvisible			:1;
		bool_t	cursorblinking			:1;
		bool_t	cursorpositionupdate	:1;
		bool_t	framebufferupdate		:1;
		ubyte_t :4;
		
	};	
	
}lcdstatusregister_t;

typedef struct{
	
	char frameBuffer[LCD_ROWS][LCD_COLUMNS+1];
	lcdposition_t cursorPos;
	lcdstatusregister_t statusRegister;
	
}lcd_t;

/*------------FUNCTION DECLARATIONS------------*/

void LCD_init(bool_t a_cursorVisible, bool_t a_cursorBlinking);
void LCD_sendInstruction(ubyte_t a_instruction);
void LCD_clearUnit(ubyte_t a_row, ubyte_t a_column);
void LCD_clearRow(ubyte_t a_row, ubyte_t a_offset);
void LCD_clearColumn(ubyte_t a_column, ubyte_t a_offset);
void LCD_clearDisplay(void);
void LCD_configCursor(bool_t a_cursorVisible, bool_t a_cursorBlinking);
void LCD_setCursorPosition(ubyte_t a_row, ubyte_t a_col);
lcdposition_t LCD_getCursorPosition(void);
void LCD_shiftCursor(lcddirection_t a_dir);
void LCD_shiftDisplay(lcddirection_t a_dir, ubyte_t a_offset, ubyte_t a_width);
void LCD_scrollDisplay(lcddirection_t a_dir, ubyte_t a_offset, ubyte_t a_width);
void LCD_scrollDisplayNative(lcddirection_t a_dir);
void LCD_setBuffer(char a_buffer[LCD_ROWS][LCD_COLUMNS+1]);
void LCD_copyBuffer(char a_returnedBuffer[LCD_ROWS][LCD_COLUMNS+1]);
void LCD_defineCustomCharacter(ubyte_t a_characterIndex, ubyte_t a_characterArray[8]);
void LCD_putc(char a_data);
void LCD_puts(const char * a_data);
void LCD_puti(udword_t a_data, lcdnumberbase_t a_numberBase);
void LCD_putf(float a_data);
void LCD_putp(void * a_ptr);
char LCD_readChar(void);
char * LCD_readString(uint8_t a_row, uint8_t a_width);

#endif /* LCD_H_ */