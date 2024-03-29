/**********************************************************************
*
* File:         HD44780.c
*
* Author(s):    Loay Ashraf <loay.ashraf.96@gmail.com>
*
* Date created: 23/09/2019
*
* Description:  contains function definitions for HD44780 controller
*               module.
*
**********************************************************************/

/*------------------------------INCLUDES-----------------------------*/

#include "HD44780.h"
#include "HD44780_config.h"
#include "hal/ecu/display/lcd/common/lcd_config.h"
#include "hal/mcu/peripheral/gpio.h"
#include "hal/mcu/sys/delay.h"

/*------------------------FUNCTION PROTOTYPES------------------------*/

static void _resetDisplay(void);
static void _sendByte(ubyte_t a_data, hd44780transmissiontype_t a_transType);
static ubyte_t _readByte(void);

/*-----------------------FUNCTION DEFINITIONS------------------------*/

/**********************************************************************
*
* Function:    HD44780_init
*
* Description: Initializes HD44780 controller module.
*
* Notes:
*
* Returns:     None.
*
**********************************************************************/

void HD44780_init(bool_t a_backlightON, bool_t a_cursorVisible, bool_t a_cursorBlinking, bool_t a_leftToRight){
    
    #ifdef HD44780_BL_PIN
    
        GPIO_SET_PIN_DIRECTION(HD44780_BL_PIN,IO_OUTPUT);
        
        HD44780_configBacklight(a_backlightON);
        
    #endif    
    
    #ifdef HD44780_RW_PIN
    
        GPIO_SET_PIN_DIRECTION(HD44780_RW_PIN,IO_OUTPUT);
    
    #endif
    
    GPIO_SET_PIN_DIRECTION(HD44780_EN_PIN,IO_OUTPUT);                                                /* set control port direction register */
    GPIO_SET_PIN_DIRECTION(HD44780_RS_PIN,IO_OUTPUT);
    GPIO_SET_PORT_DIRECTION(HD44780_DATA_PORT,HD44780_DATA_PORT_MASK,IO_OUTPUT);                    /* set data port direction register */
    
    _resetDisplay();
    
    #if (HD44780_DATA_MODE == 4)
    
        /*------------CONFIGURE HD44780 BEHAVIOUR------------*/
    
        HD44780_sendInstruction(HD44780_4BIT_MODE);                                                /* 4-bit interface, 2-line mode, 5x8 dots format */
        HD44780_sendInstruction(HD44780_DISPLAY_ON|(a_cursorVisible<<1)|a_cursorBlinking);        /* display ON, cursor OFF, blink OFF */
        HD44780_sendInstruction(HD44780_CLEAR_DISPLAY);                                            /* clear display */
        HD44780_sendInstruction(HD44780_ENTRY_MODE|(a_leftToRight<<1));                            /* no display shift */
    
    #elif (HD44780_DATA_MODE == 8)
    
        /*------------CONFIGURE HD44780 BEHAVIOUR------------*/
    
        HD44780_sendInstruction(HD44780_8BIT_MODE);                                                /* 8-bit interface, 2-line mode, 5x8 dots format */
        HD44780_sendInstruction(HD44780_DISPLAY_ON|(a_cursorVisible<<1)|a_cursorBlinking);        /* display ON, cursor OFF, blink OFF */
        HD44780_sendInstruction(HD44780_CLEAR_DISPLAY);                                            /* clear display */
        HD44780_sendInstruction(HD44780_ENTRY_MODE|(a_leftToRight<<1));                            /* no display shift */
        
    #endif
    
    
}

/**********************************************************************
*
* Function:    HD44780_sendInstruction
*
* Description: Sends instructions to HD44780 controller module.
*
* Notes:
*
* Returns:     None.
*
**********************************************************************/

void HD44780_sendInstruction(ubyte_t a_instruction){
    
    #if (HD44780_DATA_MODE == 4)
    
        #if (HD44780_DATA_PORT_MASK == 0x0F)
    
            /*------------SEND HIGH NIBBLE------------*/
    
            _sendByte((a_instruction>>4),HD44780_INSTRUCTION);

            /*------------SEND LOW NIBBLE------------*/

            _sendByte(a_instruction,HD44780_INSTRUCTION);
    
        #elif (HD44780_DATA_PORT_MASK == 0xF0)
    
            /*------------SEND HIGH NIBBLE------------*/
    
            _sendByte(a_instruction,HD44780_INSTRUCTION);

            /*------------SEND LOW NIBBLE------------*/

            _sendByte((a_instruction<<4),HD44780_INSTRUCTION);
    
        #endif
    
    #elif (HD44780_DATA_MODE == 8)
    
        /*------------SEND 8-BIT COMMAND------------*/
    
        _sendByte(a_instruction,HD44780_INSTRUCTION);
    
    #endif
    
}

/**********************************************************************
*
* Function:    HD44780_clearDisplay
*
* Description: Clears display of HD44780 controller module.
*
* Notes:
*
* Returns:     None.
*
**********************************************************************/

void HD44780_clearDisplay(void){
    
    HD44780_sendInstruction(HD44780_CLEAR_DISPLAY);            /* clear display command */

}

/**********************************************************************
*
* Function:    HD44780_configBacklight
*
* Description: Configures backlight of HD44780 controller module.
*
* Notes:
*
* Returns:     None.
*
**********************************************************************/

void HD44780_configBacklight(bool_t a_backlightON){
    
    #ifdef HD44780_BL_PIN
    
        #if (HD44780_BL_MODE == 1)
        
            if(a_backlightON)
            
                GPIO_SET_PIN(HD44780_BL_PIN);
            
            else
            
                GPIO_CLEAR_PIN(HD44780_BL_PIN);
        
        #elif (HD44780_BL_MODE == 0)
        
            if(a_backlightON)
            
                GPIO_CLEAR_PIN(HD44780_BL_PIN);
            
            else
            
                GPIO_SET_PIN(HD44780_BL_PIN);
        
        #endif
        
    #endif    
    
}

/**********************************************************************
*
* Function:    HD44780_configCursor
*
* Description: Configures cursor (visible/blinking) of HD44780 
*              controller module.
*
* Notes:
*
* Returns:     None.
*
**********************************************************************/

void HD44780_configCursor(bool_t a_cursorVisible, bool_t a_cursorBlinking){

    HD44780_sendInstruction(HD44780_DISPLAY_ON|(a_cursorVisible<<1)|a_cursorBlinking);

}

/**********************************************************************
*
* Function:    HD44780_configTextDirection
*
* Description: Configures text direction (L->R/R->L) of HD44780 
*              controller module.
*
* Notes:
*
* Returns:     None.
*
**********************************************************************/

void HD44780_configTextDirection(bool_t a_leftToRight){
        
    HD44780_sendInstruction(HD44780_ENTRY_MODE|(a_leftToRight<<1));
    
}

/**********************************************************************
*
* Function:    HD44780_setCursorPosition
*
* Description: Sets cursor position (row & column) of HD44780 
*              controller module.
*
* Notes:
*
* Returns:     None.
*
**********************************************************************/

void HD44780_setCursorPosition(uint8_t a_row, uint8_t a_col){
    
    if(a_col >= LCD_COLUMNS)                                        /* check column */
        a_col = 0;
    
    switch(a_row){

        case 0: HD44780_sendInstruction(HD44780_ROW_ZERO+a_col);    /* first row */
        break;
        case 1: HD44780_sendInstruction(HD44780_ROW_ONE+a_col);        /* second row */
        break;
        case 2: HD44780_sendInstruction(HD44780_ROW_TWO+a_col);        /* third row */
        break;
        case 3: HD44780_sendInstruction(HD44780_ROW_THREE+a_col);    /* fourth row */
        break;
        default: HD44780_sendInstruction(HD44780_ROW_ZERO+a_col);    /* default is first row */
        break;

    }
}

/**********************************************************************
*
* Function:    HD44780_shiftCursor
*
* Description: Shifts cursor position in specific direction of HD44780
*              controller module.
*
* Notes:
*
* Returns:     None.
*
**********************************************************************/

void HD44780_shiftCursor(lcddirection_t a_dir){
    
    if(a_dir == LCD_LEFT || a_dir == LCD_RIGHT)
    
        HD44780_sendInstruction(HD44780_SHIFT_CURSOR|(a_dir<<2));
    
}

/**********************************************************************
*
* Function:    HD44780_scrollDisplay
*
* Description: Scrolls display in specific direction of HD44780
*              controller module.
*
* Notes:
*
* Returns:     None.
*
**********************************************************************/

void HD44780_scrollDisplay(lcddirection_t a_dir){
    
    if(a_dir == LCD_LEFT || a_dir == LCD_RIGHT)
    
        HD44780_sendInstruction(HD44780_SCROLL_DISPLAY|(a_dir<<2));
    
}

/**********************************************************************
*
* Function:    HD44780_defineCustomCharacter
*
* Description: Defines custom character for HD44780 controller module.
*
* Notes:
*
* Returns:     None.
*
**********************************************************************/

void HD44780_defineCustomCharacter(lcdcustomcharacter_t a_characterIndex, ubyte_t a_characterArray[8]){
    
    HD44780_sendInstruction(0x40+((uint8_t)a_characterIndex*8));
    HD44780_putc(a_characterArray[0]);
    HD44780_putc(a_characterArray[1]);
    HD44780_putc(a_characterArray[2]);
    HD44780_putc(a_characterArray[3]);
    HD44780_putc(a_characterArray[4]);
    HD44780_putc(a_characterArray[5]);
    HD44780_putc(a_characterArray[6]);
    HD44780_putc(a_characterArray[7]);
    
}

/**********************************************************************
*
* Function:    HD44780_putc
*
* Description: Prints character to display of HD44780 controller module.
*
* Notes:
*
* Returns:     None.
*
**********************************************************************/

void HD44780_putc(char a_data){
    
    #if (HD44780_DATA_MODE == 4)
    
        #if (HD44780_DATA_PORT_MASK == 0x0F)
    
            //------------SEND HIGH NIBBLE------------//
    
            _sendByte((a_data>>4),HD44780_DATA);

            //------------SEND LOW NIBBLE------------//

            _sendByte(a_data,HD44780_DATA);
    
        #elif (HD44780_DATA_PORT_MASK == 0xF0)
    
            //------------SEND HIGH NIBBLE------------//
    
            _sendByte(a_data,HD44780_DATA);

            //------------SEND LOW NIBBLE------------//

            _sendByte((a_data<<4),HD44780_DATA);
    
        #endif
    
    #elif (HD44780_DATA_MODE == 8)
    
        //------------SEND 8-BIT COMMAND------------//
    
        _sendByte(a_data,HD44780_DATA);
    
    #endif
    
}

/**********************************************************************
*
* Function:    HD44780_getc
*
* Description: Reads character from display of HD44780 controller module.
*
* Notes:
*
* Returns:     Character at current cursor position (type: char).
*
**********************************************************************/

char HD44780_getc(void){
    
    char data = 0;
    
    #if (HD44780_DATA_MODE == 4)
    
        #if (HD44780_DATA_PORT_MASK == 0x0F)
    
            //------------READ HIGH NIBBLE------------//
    
            data |= (_readByte()<<4);

            //------------READ LOW NIBBLE------------//

            data |= _readByte();
    
        #elif (HD44780_DATA_PORT_MASK == 0xF0)
    
            //------------READ HIGH NIBBLE------------//
    
            data |= _readByte();

            //------------READ LOW NIBBLE------------//

            data |= (_readByte()>>4);
    
        #endif
    
    #elif (HD44780_DATA_MODE == 8)
    
        //------------READ 8-BIT DATA------------//
    
        data |= _readByte();
    
    #endif

    return data;
}

/**********************************************************************
*
* Function:    _resetDisplay
*
* Description: Sends clear display instruction to HD44780
*              controller module.
*
* Notes:
*
* Returns:     None.
*
**********************************************************************/

static void _resetDisplay(void){
    
    #if (HD44780_DATA_MODE == 4)
    
        #if (HD44780_DATA_PORT_MASK == 0x0F)
    
            /*------------RESET THE HD44780------------*/
    
            _sendByte(0x03,HD44780_INSTRUCTION);
            DELAY_MS(1);
            _sendByte(0x03,HD44780_INSTRUCTION);
            DELAY_MS(1);
            _sendByte(0x03,HD44780_INSTRUCTION);
            DELAY_MS(1);
            _sendByte(0x02,HD44780_INSTRUCTION);
            DELAY_MS(1);
    
        #elif (HD44780_DATA_PORT_MASK == 0xF0)
    
            /*------------RESET THE HD44780------------*/
    
            _sendByte((0x03<<4),HD44780_INSTRUCTION);
            DELAY_MS(1);
            _sendByte((0x03<<4),HD44780_INSTRUCTION);
            DELAY_MS(1);
            _sendByte((0x03<<4),HD44780_INSTRUCTION);
            DELAY_MS(1);
            _sendByte((0x02<<4),HD44780_INSTRUCTION);
            DELAY_MS(1);

        #endif
    
    #elif (HD44780_DATA_MODE == 8)
    
        /*------------RESET THE HD44780------------*/
    
        _sendByte(0x03,HD44780_INSTRUCTION);
        DELAY_MS(1);
        _sendByte(0x03,HD44780_INSTRUCTION);
        DELAY_MS(1);
        _sendByte(0x03,HD44780_INSTRUCTION);
        DELAY_MS(1);
    
    #endif
    
}

/**********************************************************************
*
* Function:    _sendByte
*
* Description: Sends byte to HD44780 controller module.      
*
* Notes:
*
* Returns:     None.
*
**********************************************************************/

static void _sendByte(ubyte_t a_data, hd44780transmissiontype_t a_transType){
    
    /*------------SEND A BYTE------------*/
    
    if(a_transType == HD44780_INSTRUCTION){
        
        GPIO_SET_PIN(HD44780_EN_PIN);                                            /* enable HD44780 interface for new data (EN signal), Command register is selected */
        DELAY_US(800);
        GPIO_SET_PORT(HD44780_DATA_PORT, (a_data & HD44780_DATA_PORT_MASK));        /* send a nibble of command */
        DELAY_US(800);
        GPIO_CLEAR_PIN(HD44780_EN_PIN);                                            /* clear control port */
        DELAY_US(200);
        GPIO_CLEAR_PORT(HD44780_DATA_PORT, (a_data & HD44780_DATA_PORT_MASK));    /* clear data port */
        
    }else if(a_transType == HD44780_DATA){
        
        GPIO_SET_PIN(HD44780_RS_PIN);                                            /* select Data register (RS signal) */
        DELAY_US(800);
        GPIO_SET_PIN(HD44780_EN_PIN);                                            /* enable HD44780 interface for new data (EN signal) */
        DELAY_US(800);
        GPIO_SET_PORT(HD44780_DATA_PORT, (a_data & HD44780_DATA_PORT_MASK));        /* send a nibble of data */
        DELAY_US(800);
        GPIO_CLEAR_PIN(HD44780_EN_PIN);                                            /* clear EN pin */
        GPIO_CLEAR_PIN(HD44780_RS_PIN);                                            /* clear RS pin */
        DELAY_US(200);
        GPIO_CLEAR_PORT(HD44780_DATA_PORT, (a_data & HD44780_DATA_PORT_MASK));    /* clear data port */
        
    }
    
}

/**********************************************************************
*
* Function:    _readByte
*
* Description: Receives byte from HD44780 controller module.      
*
* Notes:
*
* Returns:     Data byte (type: ubyte_t).
*
**********************************************************************/

static ubyte_t _readByte(void){
    
    #ifdef HD44780_RW_PIN
    
        ubyte_t data;
    
        GPIO_SET_PORT_DIRECTION(HD44780_DATA_PORT,HD44780_DATA_PORT_MASK,IO_INPUT);
    
        GPIO_SET_PIN(HD44780_RS_PIN);                                        // select Data register (RS signal)
        GPIO_SET_PIN(HD44780_RW_PIN);                                        // select Data register (RW signal)
        DELAY_US(800);
        GPIO_SET_PIN(HD44780_EN_PIN);                                        // enable GHD44780 interface for new data (EN signal)
        DELAY_US(800);
        data = (GPIO_READ_PORT(HD44780_DATA_PORT)&HD44780_DATA_PORT_MASK);        // read nibble of data
        DELAY_US(800);
        GPIO_CLEAR_PIN(HD44780_EN_PIN);                                        // clear EN pin
        GPIO_CLEAR_PIN(HD44780_RW_PIN);                                        // clear RW pin
        GPIO_CLEAR_PIN(HD44780_RS_PIN);                                        // clear RS pin
        DELAY_US(200);
    
        GPIO_SET_PORT_DIRECTION(HD44780_DATA_PORT,HD44780_DATA_PORT_MASK,IO_OUTPUT);
    
        return data;
    
    #else
    
        return 0;
        
    #endif    
}
