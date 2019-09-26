/*
 * glcd.h
 *
 * Created: 09/09/2019 07:04:31 AM
 *  Author: Loay Ashraf
 */ 


#ifndef GLCD_H_
#define GLCD_H_

/*------------INCLUDE MCU CONFIGURATION AND REQUIRED HEADER FILES------------*/

#include "glcd_config.h"
#include "hal/mcu/sys/std_types.h"

/*------------DEFINE GLCD CUSTOM CHARACTER CODES------------*/

#define GLCD_CCHARACTER_ZERO	0x00
#define GLCD_CCHARACTER_ONE		0x02
#define GLCD_CCHARACTER_TWO		0x04
#define GLCD_CCHARACTER_THREE	0x06

/*------------TYPEDEF GLCD OBJECT STRUCT, SHIFT AND SCROLL DIRECTION ENUMS------------*/

typedef enum{GLCD_LEFT,GLCD_RIGHT,GLCD_UP,GLCD_DOWN,GLCD_NONE}glcddirection_t;
typedef enum{GLCD_BASE10=10,GLCD_BASE16=16,GLCD_BASE2=2}glcdnumberbase_t;

typedef struct{
	
	ubyte_t row;
	ubyte_t column;
	
}glcdposition_t;

typedef struct{
	
	ubyte_t value;
	
	struct{
		
		bool_t	cursorvisible			:1;
		bool_t	cursorblinking			:1;
		bool_t	cursorpositionupdate	:1;
		bool_t	framebufferupdate		:1;
		ubyte_t :4;
		
	};
	
}glcdstatusregister_t;

typedef struct{
	
	char frameBuffer[GLCD_ROWS][GLCD_COLUMNS+1];
	glcdposition_t cursorPos;
	glcdstatusregister_t statusRegister;
	
}glcd_t;

/*------------FUNCTION DECLARATIONS------------*/

void GLCD_init(bool_t a_cursorVisible, bool_t a_cursorBlinking);
void GLCD_enableGraphics(void);
void GLCD_disableGraphics(void);
void GLCD_sendInstruction(ubyte_t a_instruction);
void GLCD_clearUnit(ubyte_t a_row, ubyte_t a_column);
void GLCD_clearRow(ubyte_t a_row, ubyte_t a_offset);
void GLCD_clearColumn(ubyte_t a_column, ubyte_t a_offset);
void GLCD_clearDisplay(void);
void GLCD_configCursor(bool_t a_cursorVisible, bool_t a_cursorBlinking);
void GLCD_setCursorPosition(ubyte_t a_row, ubyte_t a_col);
void GLCD_setPixelCursorPosition(ubyte_t a_row, ubyte_t a_col);
glcdposition_t GLCD_getCursorPosition(void);
void GLCD_shiftCursor(glcddirection_t a_dir);
void GLCD_shiftDisplay(glcddirection_t a_dir, ubyte_t a_offset, ubyte_t a_width);
void GLCD_scrollDisplay(glcddirection_t a_dir, ubyte_t a_offset, ubyte_t a_width);
void GLCD_scrollDisplayNative(glcddirection_t a_dir);
void GLCD_setBuffer(char a_buffer[GLCD_ROWS][GLCD_COLUMNS+1]);
void GLCD_copyBuffer(char a_returnedBuffer[GLCD_ROWS][GLCD_COLUMNS+1]);
void GLCD_defineCustomCharacter(ubyte_t a_characterIndex, uword_t a_characterArray[16]);
void GLCD_putc(char a_char);
void GLCD_putw(uword_t a_word);
void GLCD_puts(const char * a_data);
void GLCD_puti(udword_t a_data, glcdnumberbase_t a_numberBase);
void GLCD_putf(float a_data);
void GLCD_putp(void * a_ptr);
char GLCD_readChar(void);
uword_t GLCD_readWord(void);
void GLCD_drawPixel(uint8_t a_x, uint8_t a_y);
void GLCD_drawLine(uint8_t a_startX, uint8_t a_startY, uint8_t a_endX, uint8_t a_endY);
void GLCD_drawBox(uint8_t a_startX, uint8_t a_startY, uint8_t a_width, uint8_t a_length);
void GLCD_drawFilledBox(uint8_t a_startX, uint8_t a_startY, uint8_t a_width, uint8_t a_length);
void GLCD_drawCircle(uint8_t a_centerX, uint8_t a_centerY, uint8_t a_radius);
void GLCD_fillDisplay(uword_t a_pattern);
void GLCD_putImageRAM(const ubyte_t * a_image);
void GLCD_putImageROM(const ubyte_t * a_image);

#endif /* GGLCD_H_ */