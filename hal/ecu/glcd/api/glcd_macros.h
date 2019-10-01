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

	#define GLCD_INIT()													ST7920_init()
	#define GLCD_SEND_INSTRUCTION(INST)									ST7920_sendInstruction(INST)
	#define GLCD_CLEAR_DISLAY											ST7920_clearDisplay()
	#define GLCD_SET_CURSOR_POSITION(X,Y)								ST7920_setCursorPosition(X,Y)
	#define GLCD_SET_FONT(FONT)											ST7920_setFont(FONT)
	#define GLCD_SCROLL_DISPLAY(DIR)									ST7920_scrollDisplay(DIR)
	#define GLCD_PUTC(DATA)												ST7920_putc(DATA)
	#define GLCD_PUTS(STR)												ST7920_puts(STR)
	#define GLCD_GETC													ST7920_getc()
	#define GLCD_DRAW_PIXEL(X,Y)										ST7920_drawPixel(X,Y)
	#define GLCD_DRAW_VERTICAL_BAR(INDEX,VALUE,MIN_VALUE,MAX_VALUE)		ST7920_drawVerticalBar(INDEX,VALUE,MIN_VALUE,MAX_VALUE)
	#define GLCD_DRAW_HORIZONTAL_BAR(INDEX,VALUE,MIN_VALUE,MAX_VALUE)	ST7920_drawHorizontalBar(INDEX,VALUE,MIN_VALUE,MAX_VALUE)
	#define GLCD_FILL_DISPLAY(PATTERN)									ST7920_fillDisplay(PATTERN)
	#define GLCD_PUT_IMAGE_RAM(IMAGE_ARRAY)								ST7920_putImageRAM(IMAGE_ARRAY)
	#define GLCD_PUT_IMAGE_ROM(IMAGE_ARRAY)								ST7920_putImageROM(IMAGE_ARRAY)

#endif



#endif /* GLCD_MACROS_H_ */