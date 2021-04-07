/**********************************************************************
*
* File:         lcd.h
*
* Author(s):	Loay Ashraf <loay.ashraf.96@gmail.com>
*
* Date created: 15/01/2018
*
* Description:	contains function declarations for LCD module.
*
**********************************************************************/

#ifndef LCD_H_
#define LCD_H_

/*------------------------------INCLUDES-----------------------------*/

#include "hal/ecu/lcd/common/lcd_types.h"

/*------------------------FUNCTION DECLARATIONS----------------------*/

void LCD_init(bool_t a_backlightON, bool_t a_cursorVisible, bool_t a_cursorBlinking, bool_t a_leftToRight);
void LCD_sendInstruction(ubyte_t a_instruction);
void LCD_clearUnit(uint8_t a_row, uint8_t a_column);
void LCD_clearRow(uint8_t a_row, uint8_t a_offset);
void LCD_clearColumn(uint8_t a_column, uint8_t a_offset);
void LCD_clearDisplay(void);
void LCD_configBacklight(bool_t a_backlightON);
void LCD_configCursor(bool_t a_cursorVisible, bool_t a_cursorBlinking);
void LCD_configTextDirection(bool_t a_leftToRight);
void LCD_setCursorPosition(uint8_t a_row, uint8_t a_col);
lcdposition_t LCD_getCursorPosition(void);
void LCD_shiftCursor(lcddirection_t a_dir);
void LCD_shiftDisplay(lcddirection_t a_dir, uint8_t a_offset, uint8_t a_width);
void LCD_scrollDisplay(lcddirection_t a_dir, uint8_t a_offset, uint8_t a_width);
void LCD_scrollDisplayNative(lcddirection_t a_dir);
void LCD_setBuffer(char a_buffer[LCD_ROWS][LCD_COLUMNS+1]);
void LCD_copyBuffer(char a_returnedBuffer[LCD_ROWS][LCD_COLUMNS+1]);
void LCD_defineCustomCharacter(lcdcustomcharacter_t a_characterIndex, ubyte_t a_characterArray[8]);
void LCD_putc(char a_data);
void LCD_puts(const char * a_data);
void LCD_puti(udword_t a_data, lcdnumberbase_t a_numberBase);
void LCD_putf(float a_data);
void LCD_putp(void * a_ptr);
char LCD_getc(void);
char * LCD_gets(uint8_t a_row, uint8_t a_width);

#endif /* LCD_H_ */