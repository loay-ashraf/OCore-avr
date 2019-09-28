/*
 * glcd_macros.h
 *
 * Created: 25/09/2019 07:38:24 PM
 *  Author: Loay Ashraf
 */ 


#ifndef GLCD_MACROS_H_
#define GLCD_MACROS_H_

#include "hal/ecu/glcd/driver/ST7920/ST7920.h"

#if defined(ST7920_H_)

	#define GLCD_INIT(VISIBLE,BLINK)									ST7920_init(VISIBLE,BLINK)
	#define GLCD_ENABLE_GRAPHICS										ST7920_enableGraphics()
	#define GLCD_DISABLE_GRAPHICS										ST7920_disableGraphics()
	#define GLCD_SEND_INSTRUCTION(INST)									ST7920_sendInstruction(INST)
	#define GLCD_CLEAR_DISLAY											ST7920_clearDisplay()
	#define GLCD_CONFIG_CURSOR(VISIBLE,BLINK)							ST7920_configCursor(VISIBLE,BLINK)
	#define GLCD_SET_CURSOR_POSITION(ROW,COL)							ST7920_setCursorPosition(ROW,COL)
	#define GLCD_SET_PIXEL_CURSOR_POSITION(ROW,COL)						ST7920_setPixelCursorPosition(ROW,COL)
	#define GLCD_SHIFT_CURSOR(DIR)										ST7920_shiftCursor(DIR)
	#define GLCD_SCROLL_DISPLAY(DIR)									ST7920_scrollDisplay(DIR)
	#define GLCD_DEFINE_CUSTOM_CHARACTER(INDEX,ARRAY)					ST7920_defineCustomCharacter(INDEX,ARRAY)
	#define GLCD_PUTC(DATA)												ST7920_putc(DATA)
	#define GLCD_GETC													ST7920_getc()
	#define GLCD_PUTC_GFX(DATA)											ST7920_putcGFX(DATA)
	#define GLCD_PUTS_GFX(DATA_ARRAY)									ST7920_putsGFX(DATA_ARRAY)
	#define GLCD_DRAW_PIXEL(X,Y)										ST7920_drawPixel(X,Y)
	#define GLCD_DRAW_VERTICAL_BAR(INDEX,VALUE,MIN_VALUE,MAX_VALUE)		ST7920_drawVerticalBar(INDEX,VALUE,MIN_VALUE,MAX_VALUE)
	#define GLCD_DRAW_HORIZONTAL_BAR(INDEX,VALUE,MIN_VALUE,MAX_VALUE)	ST7920_drawHorizontalBar(INDEX,VALUE,MIN_VALUE,MAX_VALUE)
	#define GLCD_FILL_DISPLAY(PATTERN)									ST7920_fillDisplay(PATTERN)
	#define GLCD_PUT_IMAGE_RAM(IMAGE_ARRAY)								ST7920_putImageRAM(IMAGE_ARRAY)
	#define GLCD_PUT_IMAGE_ROM(IMAGE_ARRAY)								ST7920_putImageROM(IMAGE_ARRAY)

#endif



#endif /* GLCD_MACROS_H_ */