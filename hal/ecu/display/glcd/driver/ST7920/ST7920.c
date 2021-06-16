/**********************************************************************
*
* File:         ST7920.c
*
* Author(s):    Loay Ashraf <loay.ashraf.96@gmail.com>
*
* Date created: 25/09/2019
*
* Description:  contains function definitions for ST7920 controller
*               module.
*
**********************************************************************/

/*------------------------------INCLUDES-----------------------------*/

#include "ST7920.h"
#include "ST7920_config.h"
#include "ST7920_fonts.h"
#include "hal/ecu/display/glcd/common/glcd_config.h"
#include "hal/mcu/peripheral/gpio.h"
#include "hal/mcu/peripheral/spi.h"
#include "hal/mcu/sys/delay.h"
#include "service/include/map.h"
#include <avr/pgmspace.h>
#include <math.h>
#include <stdlib.h>

/*--------------------------GLOBAL VARIABLES-------------------------*/

/**********************************************************************
*
* Variable:    g_pixelX
*
* Description: Stores current pixel position along X-axis.
*
* Notes:
*
* Scope:       ST7920.c.
*
**********************************************************************/

static uint8_t g_pixelX;

/**********************************************************************
*
* Variable:    g_pixelY
*
* Description: Stores current pixel position along Y-axis.
*
* Notes:
*
* Scope:       ST7920.c.
*
**********************************************************************/

static uint8_t g_pixelY;

/**********************************************************************
*
* Variable:    g_font
*
* Description: Stores current used font.
*
* Notes:
*
* Scope:       ST7920.c.
*
**********************************************************************/

static glcdfont_t g_font;

/*------------------------FUNCTION PROTOTYPES------------------------*/

static void _resetDisplay(void);
static void _sendByte(ubyte_t a_data, st7920transmissiontype_t a_transType);
static ubyte_t _readByte(void);
static void _putc(char a_char);
static void _putw(uword_t a_word);
#ifdef ST7920_RW_PIN
static uword_t _getw(void);
#endif
static void _enableGraphics(void);

/*-----------------------FUNCTION DEFINITIONS------------------------*/

/**********************************************************************
*
* Function:    ST7920_init
*
* Description: Initializes ST7920 controller module.
*
* Notes:
*
* Returns:     None.
*
**********************************************************************/

void ST7920_init(bool_t a_backlightON){
    
    GPIO_SET_PIN_DIRECTION(ST7920_PSB_PIN,IO_OUTPUT);
    GPIO_SET_PIN_DIRECTION(ST7920_RS_PIN,IO_OUTPUT);
    
    #ifdef ST7920_BL_PIN
    
        GPIO_SET_PIN_DIRECTION(ST7920_BL_PIN,IO_OUTPUT);
        
        ST7920_configBacklight(a_backlightON);
    
    #endif
    
    #ifdef ST7920_RST_PIN
    
        GPIO_SET_PIN_DIRECTION(ST7920_RST_PIN,IO_OUTPUT);
        GPIO_SET_PIN(ST7920_RST_PIN);
    
    #endif
    
    #if (ST7920_INTERFACE == 1)
    
        GPIO_SET_PIN(ST7920_PSB_PIN);
        
        #ifdef ST7920_RW_PIN
        
            GPIO_SET_PIN_DIRECTION(ST7920_RW_PIN,IO_OUTPUT);
        
        #endif
        
        GPIO_SET_PIN_DIRECTION(ST7920_EN_PIN,IO_OUTPUT);
        GPIO_SET_PORT_DIRECTION(ST7920_DATA_PORT,ST7920_DATA_PORT_MASK,IO_OUTPUT);        /* set data port direction register */
    
    #elif (ST7920_INTERFACE == 0)
        
        spiconfig_t spiConfig = {.mode = SPI_MASTER,.prescaler=SPI_DIV128,.speedmode=SPI_NORMAL,.dataorder=SPI_MSB_FIRST,.datamode=SPI_MODE3};
        GPIO_CLEAR_PIN(ST7920_PSB_PIN);
        SPI_CONFIG(SPI0_M,&spiConfig);
        SPI_ENABLE(SPI0_M);
    
    #endif
    
    _resetDisplay();
    
    #if (ST7920_INTERFACE == 1)
    
        #if (ST7920_DATA_MODE == 4)
    
            /*------------CONFIGURE ST7920 BEHAVIOUR------------*/

            ST7920_sendInstruction(ST7920_4BIT_MODE);        /* 4-bit interface, 2-line mode, 5x8 dots format */
            ST7920_sendInstruction(ST7920_DISPLAY_ON);       /* display ON, cursor OFF, blink OFF */
            ST7920_sendInstruction(ST7920_ENTRY_MODE);       /* cursor moves to the right, no display shift */
            ST7920_sendInstruction(ST7920_CLEAR_DISPLAY);    /* clear display */
    
        #elif (ST7920_DATA_MODE == 8)
    
            /*------------CONFIGURE ST7920 BEHAVIOUR------------*/
    
            ST7920_sendInstruction(ST7920_8BIT_MODE);        /* 8-bit interface, 2-line mode, 5x8 dots format */
            ST7920_sendInstruction(ST7920_DISPLAY_ON);       /* display ON, cursor OFF, blink OFF */
            ST7920_sendInstruction(ST7920_ENTRY_MODE);       /* cursor moves to the right, no display shift */
            ST7920_sendInstruction(ST7920_CLEAR_DISPLAY);    /* clear display */
    
        #endif
    
    #elif (ST7920_INTERFACE == 0)
    
        /*------------CONFIGURE ST7920 BEHAVIOUR------------*/

        ST7920_sendInstruction(ST7920_8BIT_MODE);        /* 8-bit interface, 2-line mode, 5x8 dots format */
        ST7920_sendInstruction(ST7920_DISPLAY_ON);       /* display ON, cursor OFF, blink OFF */
        ST7920_sendInstruction(ST7920_ENTRY_MODE);       /* cursor moves to the right, no display shift */
        ST7920_sendInstruction(ST7920_CLEAR_DISPLAY);    /* clear display */
    
    #endif
    
    DELAY_MS(10);
    
    _enableGraphics();
    
    g_font = GLCD_5X8;    /* by default use 5x8 font */
    
}

/**********************************************************************
*
* Function:    ST7920_sendInstruction
*
* Description: Sends instructions to ST7920 controller module.
*
* Notes:
*
* Returns:     None.
*
**********************************************************************/

void ST7920_sendInstruction(ubyte_t a_instruction){
    
    #if (ST7920_INTERFACE == 1)
    
        #if (ST7920_DATA_MODE == 4)
    
            #if (ST7920_DATA_PORT_MASK == 0x0F)
    
                /*------------SEND HIGH NIBBLE------------*/
    
                _sendByte((a_instruction>>4),ST7920_INSTRUCTION);

                /*------------SEND LOW NIBBLE------------*/

                _sendByte(a_instruction,ST7920_INSTRUCTION);
    
            #elif (ST7920_DATA_PORT_MASK == 0xF0)
    
                /*------------SEND HIGH NIBBLE------------*/
    
                _sendByte(a_instruction,ST7920_INSTRUCTION);

                /*------------SEND LOW NIBBLE------------*/

                _sendByte((a_instruction<<4),ST7920_INSTRUCTION);
    
            #endif
    
        #elif (ST7920_DATA_MODE == 8)
    
            /*------------SEND 8-BIT COMMAND------------*/
    
            _sendByte(a_instruction,ST7920_INSTRUCTION);
    
        #endif
    
    #elif (ST7920_INTERFACE == 0)
    
        _sendByte(a_instruction,ST7920_INSTRUCTION);
    
    #endif
    
}

/**********************************************************************
*
* Function:    ST7920_clearDisplay
*
* Description: Clears display of ST7920 controller module.
*
* Notes:
*
* Returns:     None.
*
**********************************************************************/

void ST7920_clearDisplay(void){

    ST7920_fillDisplay(0);    /* clear display command */
    
}

/**********************************************************************
*
* Function:    ST7920_configBacklight
*
* Description: Configures backlight of ST7920 controller module.
*
* Notes:
*
* Returns:     None.
*
**********************************************************************/

void ST7920_configBacklight(bool_t a_backlightON){
    
    #ifdef ST7920_BL_PIN
    
        #if (ST7920_BL_MODE == 1)
        
            if(a_backlightON)
            
                GPIO_SET_PIN(ST7920_BL_PIN);
            
            else
            
                GPIO_CLEAR_PIN(ST7920_BL_PIN);
        
        #elif (ST7920_BL_MODE == 0)
        
            if(a_backlightON)
            
                GPIO_CLEAR_PIN(ST7920_BL_PIN);
            
            else
            
                GPIO_SET_PIN(ST7920_BL_PIN);
        
        #endif
    
    #endif
    
}

/**********************************************************************
*
* Function:    ST7920_setCursorPosition
*
* Description: Sets cursor position (X & Y) of ST7920 controller module.
*
* Notes:
*
* Returns:     None.
*
**********************************************************************/

void ST7920_setCursorPosition(uint8_t a_x, uint8_t a_y){
    
    if(a_y > GLCD_Y_PIXELS-1)
        a_y = GLCD_Y_PIXELS-1;
    if(a_x > (GLCD_X_PIXELS/ST7920_X_PAGE_SIZE*2)-1)
        a_x = (GLCD_X_PIXELS/ST7920_X_PAGE_SIZE*2)-1;    
    
    if(g_pixelX != a_x)    
        g_pixelX = a_x;
    if(g_pixelY != a_y)    
        g_pixelY = a_y;    
    
    if(a_y < 32){
        
        ST7920_sendInstruction(0x80+a_y);
        ST7920_sendInstruction(0x80+a_x);
        
    }else{
        
        ST7920_sendInstruction(0x80+(a_y-32));
        ST7920_sendInstruction(0x88+a_x);
        
    }
    
}

/**********************************************************************
*
* Function:    ST7920_setFont
*
* Description: Sets text font for ST7920 controller module.
*
* Notes:
*
* Returns:     None.
*
**********************************************************************/

void ST7920_setFont(glcdfont_t a_font){
    
    g_font = a_font;
    
}

/**********************************************************************
*
* Function:    ST7920_scrollDisplay
*
* Description: Scrolls display in specific direction of ST7920
*              controller module.
*
* Notes:
*
* Returns:     None.
*
**********************************************************************/

void ST7920_scrollDisplay(glcddirection_t a_direction){
    
    if(a_direction == GLCD_LEFT || a_direction == GLCD_RIGHT)
    
        ST7920_sendInstruction(ST7920_SCROLL_DISPLAY|(a_direction<<2));
    
}

/**********************************************************************
*
* Function:    ST7920_putc
*
* Description: Prints character to display of ST7920 controller module.
*
* Notes:
*
* Returns:     None.
*
**********************************************************************/

void ST7920_putc(char a_char){
    
    uint8_t y,y_end;
    
    a_char -= 32;
    y_end = g_pixelY+8;
    
    for(y=g_pixelY;y<y_end;y++){
        
        ST7920_setCursorPosition(g_pixelX,y);
        
        if(g_font == GLCD_5X8)
            _putc(pgm_read_byte(&font_5x8[(uint8_t)a_char][8-(y_end-y)]));
        
    }
    
}

/**********************************************************************
*
* Function:    ST7920_puts
*
* Description: Prints string to display of ST7920 controller module.
*
* Notes:
*
* Returns:     None.
*
**********************************************************************/

void ST7920_puts(const char * a_data){
    
    uint8_t y,y_end;
    char * charPointer;

    y_end = g_pixelY+8;
    
    for(y=g_pixelY;y<y_end;y++){
        
        charPointer = (char *)a_data;
        ST7920_setCursorPosition(g_pixelX,y);
        
        if(g_font == GLCD_5X8){
        
            while(*charPointer != '\0')
                _putc(pgm_read_byte(&font_5x8[(uint8_t)((*charPointer++)-32)][8-(y_end-y)]));
        
        }
    }
    
}

/**********************************************************************
*
* Function:    ST7920_getc
*
* Description: Reads character from display of ST7920 controller module.
*
* Notes:
*
* Returns:     Character at current cursor position (type: char).
*
**********************************************************************/

char ST7920_getc(void){

    char data = 0;
    
    #if (ST7920_DATA_MODE == 4)

        #if (ST7920_DATA_PORT_MASK == 0x0F)

            /*------------READ HIGH NIBBLE------------*/

            data |= (_readByte()<<4);

            /*------------READ LOW NIBBLE------------*/

            data |= _readByte();

        #elif (ST7920_DATA_PORT_MASK == 0xF0)

            /*------------READ HIGH NIBBLE------------*/

            data |= _readByte();

            /*------------READ LOW NIBBLE------------*/

            data |= (_readByte()>>4);

        #endif

    #elif (ST7920_DATA_MODE == 8)

        /*------------READ 8-BIT DATA------------*/

        data |= _readByte();

    #endif
    
    return data;

}

/**********************************************************************
*
* Function:    ST7920_drawPixel
*
* Description: Draws pixel at current cursor position (X & Y) on display
*              of ST7920 controller module.
*
* Notes:
*
* Returns:     None.
*
**********************************************************************/

void ST7920_drawPixel(uint8_t a_x, uint8_t a_y){
    
    #ifdef ST7920_RW_PIN
        
        uint8_t bitNum,pageNum;
        uword_t currentData;
        
        pageNum = a_x/16.0;
        bitNum = a_x-(pageNum*16);
        
        ST7920_setCursorPosition(pageNum,a_y);
        currentData = _getw();
        ST7920_setCursorPosition(pageNum,a_y);
        _putw(currentData|(1<<(15-bitNum)));
    
    #endif    
         
}

/**********************************************************************
*
* Function:    ST7920_drawVerticalBar
*
* Description: Draws vertical bar on display of ST7920 controller
*              module.
*
* Notes:
*
* Returns:     None.
*
**********************************************************************/

void ST7920_drawVerticalBar(glcdbarindex_t a_barIndex, uint8_t a_value, uint8_t a_minValue, uint8_t a_maxValue){
    
    int8_t counter;
    uint8_t stopY = (45-map(a_value,a_minValue,a_maxValue,0,45))+18;
    char buffer[5];
    
    for(counter=63;counter>=stopY;counter--){
        
        ST7920_setCursorPosition(a_barIndex*2,counter);
        _putw(0x00FF);
        _putw(0xFF00);
        
    }
    
    for(;counter>=0;counter--){
        
        ST7920_setCursorPosition(a_barIndex*2,counter);
        _putw(0);
        _putw(0);
        
    }
    
    itoa(a_value,buffer,10);
    
    buffer[4] = '\0';
    buffer[3] = buffer[2];
    buffer[2] = buffer[1];
    buffer[1] = buffer[0];
    buffer[0] = ' ';
    
    ST7920_setCursorPosition(a_barIndex*2,5);
    ST7920_puts(buffer);
    
}

/**********************************************************************
*
* Function:    ST7920_drawHorizontalBar
*
* Description: Draws horizontal bar on display of ST7920 controller
*              module.
*
* Notes:
*
* Returns:     None.
*
**********************************************************************/

void ST7920_drawHorizontalBar(glcdbarindex_t a_barIndex, uint8_t a_value, uint8_t a_minValue, uint8_t a_maxValue){
    
    int8_t x,y;
    uint8_t stopX = map(a_value,a_minValue,a_maxValue,0,5);
    uint8_t stopY = (a_barIndex*16)+15;
    char buffer[5];
    
    for(y=a_barIndex*16;y<=stopY;y++){
        
        ST7920_setCursorPosition(0,y);
        
        for(x=0;x<=stopX;x++){
            
            if(y<(a_barIndex*16)+4)
                _putw(0);
            else if(y>(a_barIndex*16)+11)
                _putw(0);
            else
                _putw(0xFFFF);
            
        }
        
        for(;x<=7;x++)
            _putw(0);    
        
    }
    
    itoa(a_value,buffer,10);
    
    ST7920_setCursorPosition(6,(a_barIndex*16)+4);
    ST7920_puts(buffer);
    
}

/**********************************************************************
*
* Function:    ST7920_fillDisplay
*
* Description: Fills display of ST7920 controller module.       
*
* Notes:
*
* Returns:     None.
*
**********************************************************************/

void ST7920_fillDisplay(ubyte_t a_pattern){
    
    uint8_t x,y;
    
    for(y=0;y<64;y++){
        
        ST7920_setCursorPosition(0,y);
        
        for(x=0;x<8;x++){
            
            _putc(a_pattern);
            _putc(a_pattern);
            
        }
    }    
}

/**********************************************************************
*
* Function:    ST7920_putImageRAM
*
* Description: Draws bitmap image on display of ST7920 controller 
*              module.
*
* Notes:       This function accepts bitmap images stored in RAM.
*
* Returns:     None.
*
**********************************************************************/

void ST7920_putImageRAM(const ubyte_t * a_image){
    
    uint8_t x,y;
    
    for(y=0;y<64;y++){
        
        ST7920_setCursorPosition(0,y);
        
        for(x=0;x<8;x++){
            
            _putc(a_image[(2*x)+(16*y)]);
            _putc(a_image[(2*x)+1+(16*y)]);
            
        }
    }
}

/**********************************************************************
*
* Function:    ST7920_putImageROM
*
* Description: Draws bitmap image on display of ST7920 controller 
*              module.
*
* Notes:       This function accepts bitmap images stored in ROM.
*
* Returns:     None.
*
**********************************************************************/

void ST7920_putImageROM(const ubyte_t * a_image){
    
    uint8_t x,y;
    uint8_t byte;
    
    for(y=0;y<64;y++){
        
        ST7920_setCursorPosition(0,y);
        
        for(x=0;x<8;x++){
            
            byte = pgm_read_byte(&a_image[(2*x)+(16*y)]);
            _putc(byte);
            
            byte = pgm_read_byte(&a_image[(2*x)+(16*y)+1]);
            _putc(byte);
            
        }
    }
}

/**********************************************************************
*
* Function:    _resetDisplay
*
* Description: Sends clear display instruction to ST7920
*              controller module.
*
* Notes:
*
* Returns:     None.
*
**********************************************************************/

static void _resetDisplay(void){
    
    #if (ST7920_INTERFACE == 1)
    
        #if (ST7920_DATA_MODE == 4)
    
            #if (ST7920_DATA_PORT_MASK == 0x0F)
    
                /*------------RESET THE ST7920------------*/
    
                _sendByte(0x03,ST7920_INSTRUCTION);
                DELAY_MS(1);
                _sendByte(0x03,ST7920_INSTRUCTION);
                DELAY_MS(1);
                _sendByte(0x03,ST7920_INSTRUCTION);
                DELAY_MS(1);
                _sendByte(0x02,ST7920_INSTRUCTION);
                DELAY_MS(1);
    
            #elif (ST7920_DATA_PORT_MASK == 0xF0)
    
                /*------------RESET THE ST7920------------*/
    
                _sendByte((0x03<<4),ST7920_INSTRUCTION);
                DELAY_MS(1);
                _sendByte((0x03<<4),ST7920_INSTRUCTION);
                DELAY_MS(1);
                _sendByte((0x03<<4),ST7920_INSTRUCTION);
                DELAY_MS(1);
                _sendByte((0x02<<4),ST7920_INSTRUCTION);
                DELAY_MS(1);

            #endif
    
        #elif (ST7920_DATA_MODE == 8)
    
            /*------------RESET THE ST7920------------*/
    
            _sendByte(0x03,ST7920_INSTRUCTION);
            DELAY_MS(1);
            _sendByte(0x03,ST7920_INSTRUCTION);
            DELAY_MS(1);
            _sendByte(0x03,ST7920_INSTRUCTION);
            DELAY_MS(1);
    
        #endif
    
    #elif (ST7920_INTERFACE == 0)
    
        /*------------RESET THE ST7920------------*/
    
        _sendByte(0x03,ST7920_INSTRUCTION);
        DELAY_MS(1);
        _sendByte(0x03,ST7920_INSTRUCTION);
        DELAY_MS(1);
        _sendByte(0x03,ST7920_INSTRUCTION);
        DELAY_MS(1);
    
    #endif
    
}

/**********************************************************************
*
* Function:    _sendByte
*
* Description: Sends byte to ST7920 controller module.      
*
* Notes:
*
* Returns:     None.
*
**********************************************************************/

static void _sendByte(ubyte_t a_data, st7920transmissiontype_t a_transType){
    
    #if (ST7920_INTERFACE == 1)
    
        /*------------SEND A BYTE------------*/
    
        if(a_transType == ST7920_INSTRUCTION){
        
            GPIO_SET_PIN(ST7920_EN_PIN);                                           /* enable ST7920 interface for new data (EN signal), Command register is selected */
            DELAY_US(800);
            GPIO_SET_PORT(ST7920_DATA_PORT,(a_data & ST7920_DATA_PORT_MASK));      /* send a nibble of command */
            DELAY_US(800);
            GPIO_CLEAR_PIN(ST7920_EN_PIN);                                         /* clear control port */
            DELAY_US(200);
            GPIO_CLEAR_PORT(ST7920_DATA_PORT,(a_data & ST7920_DATA_PORT_MASK));    /* clear data port */
            DELAY_US(200);
        
        }else if(a_transType == ST7920_DATA){
        
            GPIO_SET_PIN(ST7920_RS_PIN);                                           /* select Data register (RS signal) */
            DELAY_US(800);
            GPIO_SET_PIN(ST7920_EN_PIN);                                           /* enable ST7920 interface for new data (EN signal) */
            DELAY_US(800);
            GPIO_SET_PORT(ST7920_DATA_PORT,(a_data & ST7920_DATA_PORT_MASK));      /* send a nibble of data */
            DELAY_US(800);
            GPIO_CLEAR_PIN(ST7920_EN_PIN);                                         /* clear EN pin */
            GPIO_CLEAR_PIN(ST7920_RS_PIN);                                         /* clear RS pin */
            DELAY_US(200);
            GPIO_CLEAR_PORT(ST7920_DATA_PORT,(a_data & ST7920_DATA_PORT_MASK));    /* clear data port */
            DELAY_US(200);
        
        }
    
    #elif (ST7920_INTERFACE == 0)
    
        GPIO_SET_PIN(ST7920_RS_PIN);      /* select ST7920 chip (SS signal) */
        DELAY_US(350);
        SPI_TRANSMIT_BYTE(SPI0_M,a_transType? 0xFA : 0xF8);
        SPI_TRANSMIT_BYTE(SPI0_M,a_data&0xF0);
        SPI_TRANSMIT_BYTE(SPI0_M,(a_data<<4)&0xF0);
        DELAY_US(350);
        GPIO_CLEAR_PIN(ST7920_RS_PIN);    /* release ST7920 chip (SS signal) */
    
    #endif
    
}

/**********************************************************************
*
* Function:    _readByte
*
* Description: Receives byte from ST7920 controller module.      
*
* Notes:
*
* Returns:     Data byte (type: ubyte_t).
*
**********************************************************************/

static ubyte_t _readByte(void){
    
    #ifdef ST7920_RW_PIN
    
        ubyte_t data;
    
        GPIO_SET_PORT_DIRECTION(ST7920_DATA_PORT,ST7920_DATA_PORT_MASK,IO_INPUT);
    
        GPIO_SET_PIN(ST7920_RS_PIN);                                        /* select Data register (RS signal) */
        GPIO_SET_PIN(ST7920_RW_PIN);                                        /* select Read mode (RW signal) */
        DELAY_US(800);
        GPIO_SET_PIN(ST7920_EN_PIN);                                        /* enable ST7920 interface for new data (EN signal) */
        DELAY_US(800);
        data = (GPIO_READ_PORT(ST7920_DATA_PORT)&ST7920_DATA_PORT_MASK);    /* read nibble of data */
        DELAY_US(800);
        GPIO_CLEAR_PIN(ST7920_EN_PIN);                                      /* clear EN pin */
        GPIO_CLEAR_PIN(ST7920_RW_PIN);                                      /* clear RW pin */
        GPIO_CLEAR_PIN(ST7920_RS_PIN);                                      /* clear RS pin */
        DELAY_US(200);
    
        GPIO_SET_PORT_DIRECTION(ST7920_DATA_PORT,ST7920_DATA_PORT_MASK,IO_OUTPUT);
    
        return data;
    
    #else
    
        return 0;
        
    #endif    
    
}

/**********************************************************************
*
* Function:    _putc
*
* Description: Sends 8-bit data to ST7920 controller module.      
*
* Notes:       This function is used to draw pixels on display.
*
* Returns:     None.
*
**********************************************************************/

static void _putc(char a_char){
    
    #if (ST7920_INTERFACE == 1)
    
        #if (ST7920_DATA_MODE == 4)
    
            #if (ST7920_DATA_PORT_MASK == 0x0F)
    
                /*------------SEND HIGH NIBBLE------------*/
    
                _sendByte((a_char>>4),ST7920_DATA);

                /*------------SEND LOW NIBBLE------------*/

                _sendByte(a_char,ST7920_DATA);
    
            #elif (ST7920_DATA_PORT_MASK == 0xF0)
    
                /*------------SEND HIGH NIBBLE------------*/
    
                _sendByte(a_char,ST7920_DATA);

                /*------------SEND LOW NIBBLE------------*/

                _sendByte((a_char<<4),ST7920_DATA);
    
            #endif
    
        #elif (ST7920_DATA_MODE == 8)
    
            /*------------SEND 8-BIT DATA------------*/
    
            _sendByte(a_char,ST7920_DATA);
    
        #endif
    
    #elif (ST7920_INTERFACE == 0)
    
        _sendByte(a_char,ST7920_DATA);
    
    #endif
    
}

/**********************************************************************
*
* Function:    _putw
*
* Description: Sends 16-bit data to ST7920 controller module.      
*
* Notes:       This function is used to draw pixels on display.
*
* Returns:     None.
*
**********************************************************************/

static void _putw(uword_t a_word){
    
    _putc((a_word>>8)&0xFF);
    _putc(a_word&0xFF);
    
}

/**********************************************************************
*
* Function:    _getw
*
* Description: Receives 16-bit data from ST7920 controller module.      
*
* Notes:       This function is used to read pixels from display.
*
* Returns:     Data word (type: uword_t).
*
**********************************************************************/

#ifdef ST7920_RW_PIN
static uword_t _getw(void){
    
    ubyte_t byte = 0;
    uword_t word = 0;
    byte = ST7920_getc();        /* dummy read */
    byte = ST7920_getc();
    word |= ((byte<<8)&0xFF00);
    byte = ST7920_getc();
    word |= (byte&0x00FF);
    
    return word;
    
}
#endif

/**********************************************************************
*
* Function:    _enableGraphics
*
* Description: Enables graphics mode for ST7920 controller module.      
*
* Notes:       This function should be called after initialization.
*
* Returns:     None.
*
**********************************************************************/

static void _enableGraphics(void){
    
    DELAY_MS(1);
    
    #if (ST7920_INTERFACE == 1)
    
        #if (ST7920_DATA_MODE == 4)
    
            /*------------CONFIGURE ST7920 BEHAVIOUR------------*/

            ST7920_sendInstruction(ST7920_4BIT_MODE);
            DELAY_MS(1);
            ST7920_sendInstruction(ST7920_4BIT_MODE|ST7920_EXT_INSTRUCTION);
            DELAY_MS(1);
            ST7920_sendInstruction(ST7920_4BIT_MODE|ST7920_GRAPHICS_ON);
            DELAY_MS(1);
            ST7920_sendInstruction(ST7920_CLEAR_DISPLAY);
    
        #elif (ST7920_DATA_MODE == 8)
    
            /*------------CONFIGURE ST7920 BEHAVIOUR------------*/
    
            ST7920_sendInstruction(ST7920_8BIT_MODE);
            DELAY_MS(1);
            ST7920_sendInstruction(ST7920_8BIT_MODE|ST7920_EXT_INSTRUCTION);
            DELAY_MS(1);
            ST7920_sendInstruction(ST7920_8BIT_MODE|ST7920_GRAPHICS_ON);
            DELAY_MS(1);
            ST7920_sendInstruction(ST7920_CLEAR_DISPLAY);
    
        #endif
    
    #elif (ST7920_INTERFACE == 0)
    
        ST7920_sendInstruction(ST7920_8BIT_MODE);
        DELAY_MS(1);
        ST7920_sendInstruction(ST7920_8BIT_MODE|ST7920_EXT_INSTRUCTION);
        DELAY_MS(1);
        ST7920_sendInstruction(ST7920_8BIT_MODE|ST7920_GRAPHICS_ON);
        DELAY_MS(1);
        ST7920_sendInstruction(ST7920_CLEAR_DISPLAY);
    
    #endif
    
    ST7920_clearDisplay();
    
}
