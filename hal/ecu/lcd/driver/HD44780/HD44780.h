/**********************************************************************
*
* File:         HD44780.h
*
* Author(s):	Loay Ashraf <loay.ashraf.96@gmail.com>
*
* Date created: 23/09/2019
*
* Description:	contains definitions, typedefs and function 
*               declarations for HD44780 controller module.
*
**********************************************************************/

#ifndef HD44780_H_
#define HD44780_H_

/*------------------------------INCLUDES-----------------------------*/

#include "hal/ecu/lcd/common/lcd_types.h"

/*-------------------DEFINITIONS AND CONFIGURATIONS------------------*/

#define HD44780_4BIT_MODE		0x28
#define HD44780_8BIT_MODE		0x38
#define HD44780_DISPLAY_ON		0x0C
#define HD44780_ENTRY_MODE		0x04
#define HD44780_CLEAR_DISPLAY	0x01
#define HD44780_SCROLL_DISPLAY	0x18
#define HD44780_SHIFT_CURSOR	0x10
#define HD44780_ROW_ZERO		0x80
#define HD44780_ROW_ONE			0xC0
#define HD44780_ROW_TWO			0x94
#define HD44780_ROW_THREE		0xD4

/*--------------------------------ENUMS------------------------------*/

typedef enum{HD44780_INSTRUCTION,HD44780_DATA}hd44780transmissiontype_t;

/*------------------------FUNCTION DECLARATIONS----------------------*/

void HD44780_init(bool_t a_backlightON, bool_t a_cursorVisible, bool_t a_cursorBlinking, bool_t a_leftToRight);
void HD44780_sendInstruction(ubyte_t a_instruction);
void HD44780_clearDisplay(void);
void HD44780_configBacklight(bool_t a_backlightON);
void HD44780_configCursor(bool_t a_cursorVisible, bool_t a_cursorBlinking);
void HD44780_configTextDirection(bool_t a_leftToRight);
void HD44780_setCursorPosition(uint8_t a_row, uint8_t a_col);
void HD44780_shiftCursor(lcddirection_t a_dir);
void HD44780_scrollDisplay(lcddirection_t a_dir);
void HD44780_defineCustomCharacter(lcdcustomcharacter_t a_characterIndex, ubyte_t a_characterArray[8]);
void HD44780_putc(char a_data);
char HD44780_getc(void);

#endif /* HD44780_H_ */