/**********************************************************************
*
* File:         lcd_macros.h
*
* Author(s):	Loay Ashraf <loay.ashraf.96@gmail.com>
*
* Date created: 23/09/2019
*
* Description:	contains function-like adapter macros for LCD module.
*
**********************************************************************/

#ifndef LCD_MACROS_H_
#define LCD_MACROS_H_

/*------------------------------INCLUDES-----------------------------*/

#if (LCD_CONTROLLER == LCD_HD44780)

#include "hal/ecu/lcd/driver/HD44780/HD44780.h"

#endif

/*-------------------DEFINITIONS AND CONFIGURATIONS------------------*/

#if defined(HD44780_H_)
	
	#define LCD_INIT(BL_ON,VISIBLE,BLINK,LTR)			HD44780_init(BL_ON,VISIBLE,BLINK,LTR)
	#define LCD_SEND_INSTRUCTION(INST)					HD44780_sendInstruction(INST)
	#define LCD_CLEAR_DISLAY							HD44780_clearDisplay()
	#define LCD_CONFIG_BACKLIGHT(BL_ON)					HD44780_configBacklight(BL_ON)
	#define LCD_CONFIG_CURSOR(VISIBLE,BLINK)			HD44780_configCursor(VISIBLE,BLINK)
	#define LCD_CONFIG_TEXT_DIRECTION(LTR)				HD44780_configTextDirection(LTR)
	#define LCD_SET_CURSOR_POSITION(ROW,COL)			HD44780_setCursorPosition(ROW,COL)
	#define LCD_SHIFT_CURSOR(DIR)						HD44780_shiftCursor(DIR)
	#define LCD_SCROLL_DISPLAY(DIR)						HD44780_scrollDisplay(DIR)
	#define LCD_DEFINE_CUSTOM_CHARACTER(INDEX,ARRAY)	HD44780_defineCustomCharacter(INDEX,ARRAY)
	#define LCD_PUTC(DATA)								HD44780_putc(DATA)
	#define LCD_GETC									HD44780_getc()

#endif

#endif /* LCD_MACROS_H_ */