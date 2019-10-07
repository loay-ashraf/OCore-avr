/*
 * glcd.h
 *
 * Created: 09/09/2019 07:04:31 AM
 *  Author: Loay Ashraf
 */ 


#ifndef GLCD_H_
#define GLCD_H_

/*------------INCLUDE MCU CONFIGURATION AND REQUIRED HEADER FILES------------*/

#include "hal/ecu/glcd/common/glcd_config.h"
#include "hal/ecu/glcd/common/glcd_types.h"

/*------------FUNCTION DECLARATIONS------------*/

void GLCD_init(bool_t a_backlightON);
void GLCD_sendInstruction(ubyte_t a_instruction);
void GLCD_clearDisplay(void);
void GLCD_configBacklight(bool_t a_backlightON);
void GLCD_setCursorPosition(uint8_t a_x, uint8_t a_y);
void GLCD_setFont(glcdfont_t a_font);
void GLCD_scrollDisplay(glcddirection_t a_direction);
void GLCD_putc(char a_char);
void GLCD_putw(uword_t a_word);
void GLCD_puts(const char * a_data);
void GLCD_puti(udword_t a_data, glcdnumberbase_t a_numberBase);
void GLCD_putf(float a_data);
void GLCD_putp(void * a_ptr);
char GLCD_getc(void);
uword_t GLCD_getw(void);
void GLCD_drawPixel(uint8_t a_x, uint8_t a_y);
void GLCD_drawLine(uint8_t a_startX, uint8_t a_startY, uint8_t a_endX, uint8_t a_endY);
void GLCD_drawBox(uint8_t a_startX, uint8_t a_startY, uint8_t a_width, uint8_t a_length);
void GLCD_drawFilledBox(uint8_t a_startX, uint8_t a_startY, uint8_t a_width, uint8_t a_length);
void GLCD_drawCircle(uint8_t a_centerX, uint8_t a_centerY, uint8_t a_radius);
void GLCD_drawVerticalBar(glcdbarindex_t a_barIndex, uint8_t a_value, uint8_t a_minValue, uint8_t a_maxValue);
void GLCD_drawHorizontalBar(glcdbarindex_t a_barIndex, uint8_t a_value, uint8_t a_minValue, uint8_t a_maxValue);
void GLCD_fillDisplay(ubyte_t a_pattern);
void GLCD_putImageRAM(const ubyte_t * a_image);
void GLCD_putImageROM(const ubyte_t * a_image);

#endif /* GGLCD_H_ */