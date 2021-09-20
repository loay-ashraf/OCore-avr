/**********************************************************************
*
* File:         ST7920.h
*
* Author(s):    Loay Ashraf <loay.ashraf.96@gmail.com>
*
* Date created: 25/09/2019
*
* Description:  contains definitions, typedefs and function
*               declarations for ST7920 controller module.
*
**********************************************************************/

#ifndef ST7920_H_
#define ST7920_H_

/*------------------------------INCLUDES-----------------------------*/

#include "hal/ecu/display/glcd/common/glcd_types.h"

/*-------------------DEFINITIONS AND CONFIGURATIONS------------------*/

#define ST7920_4BIT_MODE          0x20
#define ST7920_8BIT_MODE          0x30
#define ST7920_DISPLAY_ON         0x0C
#define ST7920_ENTRY_MODE         0x06
#define ST7920_CLEAR_DISPLAY      0x01
#define ST7920_GO_HOME            0x02
#define ST7920_SCROLL_DISPLAY     0x18
#define ST7920_EXT_INSTRUCTION    0x04
#define ST7920_GRAPHICS_ON        0x06
#define ST7920_ROW_ZERO           0x80
#define ST7920_ROW_ONE            0x90
#define ST7920_ROW_TWO            0x88
#define ST7920_ROW_THREE          0x98

/*--------------------------------ENUMS------------------------------*/

typedef enum{ST7920_INSTRUCTION,ST7920_DATA}st7920transmissiontype_t;
    
/*------------------------FUNCTION DECLARATIONS----------------------*/    

void ST7920_init(bool_t a_backlightON);
void ST7920_sendInstruction(ubyte_t a_instruction);
void ST7920_clearDisplay(void);
void ST7920_configBacklight(bool_t a_backlightON);
void ST7920_setCursorPosition(uint8_t a_x, uint8_t a_y);
void ST7920_setFont(glcdfont_t a_font);
void ST7920_scrollDisplay(glcddirection_t a_direction);
void ST7920_putc(char a_char);
void ST7920_puts(const char * a_data);
char ST7920_getc(void);
void ST7920_drawPixel(uint8_t a_x, uint8_t a_y);
void ST7920_drawVerticalBar(glcdbarindex_t a_barIndex, uint8_t a_value, uint8_t a_minValue, uint8_t a_maxValue);
void ST7920_drawHorizontalBar(glcdbarindex_t a_barIndex, uint8_t a_value, uint8_t a_minValue, uint8_t a_maxValue);
void ST7920_fillDisplay(ubyte_t a_pattern);
void ST7920_putImageRAM(const ubyte_t * a_image);
void ST7920_putImageROM(const ubyte_t * a_image);

#endif /* ST7920_H_ */
