/*
 * ST7920.h
 *
 * Created: 25/09/2019 07:37:34 PM
 *  Author: Loay Ashraf
 */ 


#ifndef ST7920_H_
#define ST7920_H_

#include "hal/mcu/sys/std_types.h"

/*------------DEFINE ST7920 COMMANDS------------*/

#define ST7920_4BIT_MODE		0x20
#define ST7920_8BIT_MODE		0x30
#define ST7920_DISPLAY_ON		0x0C
#define ST7920_CURSOR_RIGHT		0x06
#define ST7920_CLEAR_DISPLAY	0x01
#define ST7920_GO_HOME			0x02
#define ST7920_SCROLL_DISPLAY	0x18
#define ST7920_SHIFT_CURSOR		0x10
#define ST7920_EXT_INSTRUCTION	0x04
#define ST7920_GRAPHICS_ON		0x06

/*------------DEFINE ST7920 ROW ADDRESSES------------*/

#define ST7920_ROW_ZERO			0x80
#define ST7920_ROW_ONE			0x90
#define ST7920_ROW_TWO			0x88
#define ST7920_ROW_THREE		0x98

/*------------TYPEDEF TANSMISSION TYPE ENUM------------*/

typedef enum{ST7920_INSTRUCTION,ST7920_DATA}st7920transmissiontype_t;
	
/*------------FUNCTION DECLARATIONS------------*/	

void ST7920_init(bool_t a_cursorVisible, bool_t a_cursorBlinking);
void ST7920_enableGraphics(void);
void ST7920_disableGraphics(void);
void ST7920_sendInstruction(ubyte_t a_instruction);
void ST7920_clearDisplay(void);
void ST7920_configCursor(bool_t a_cursorVisible, bool_t a_cursorBlinking);
void ST7920_setCursorPosition(ubyte_t a_row, ubyte_t a_col);
void ST7920_setPixelCursorPosition(ubyte_t a_row, ubyte_t a_col);
void ST7920_shiftCursor(bool_t a_dir);
void ST7920_scrollDisplay(bool_t a_dir);
void ST7920_defineCustomCharacter(ubyte_t a_characterIndex, uword_t a_characterArray[16]);
void ST7920_putc(char a_char);
char ST7920_readChar(void);
void ST7920_drawPixel(uint8_t a_x, uint8_t a_y);
void ST7920_fillDisplay(uword_t a_pattern);
void ST7920_putImageRAM(const ubyte_t * a_image);
void ST7920_putImageROM(const ubyte_t * a_image);

#endif /* ST7920_H_ */