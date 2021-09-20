/*
 * lcd_scrollDisplay.c
 *
 * Created: 24/09/2019 12:45:17 AM
 *  Author: Loay Ashraf
 */ 

#include "app/examples/include/examples.h"
#include "hal/ecu/display/lcd/api/lcd.h"
#include "hal/mcu/sys/delay.h"

void ex_lcd_scrollDisplay(void){
    
    uint8_t counter;        /* general purpose counter */
    
    LCD_init(TRUE,TRUE,TRUE,TRUE);    /* initialize LCD interface */
    
    /***************************************/
    /* print text to be scrolled on screen */
    /***************************************/
    
    #if (LCD_ROWS == 2)
    
        LCD_puts("Hello World!");
        LCD_setCursorPosition(1,0);
        LCD_puts("I'm ");
        LCD_puti(LCD_COLUMNS,LCD_BASE10);
        LCD_putc('x');
        LCD_puts("2 LCD");
        
    #elif (LCD_ROWS == 4)
        
        LCD_puts("Hello World!");
        LCD_setCursorPosition(1,0);
        LCD_puts("I'm ");
        LCD_puti(LCD_COLUMNS,LCD_BASE10);
        LCD_putc('x');
        LCD_puts("4 LCD");
        LCD_setCursorPosition(2,0);
        LCD_puts("I'm scrolling now");
        LCD_setCursorPosition(3,0);
        LCD_puts("Enjoy the show!");
        
    #endif    
    
    DELAY_MS(1000);
    
    while(1){        /* loop forever */
    
        /**************************************/
        /* scroll text on screen to the right */
        /**************************************/
    
        for(counter=0;counter<LCD_COLUMNS;counter++){
        
            DELAY_MS(1000);
            LCD_scrollDisplay(LCD_RIGHT,0,LCD_ROWS);
        
        }
    
        DELAY_MS(1000);
    
        /*************************************/
        /* scroll text on screen to the left */
        /*************************************/
    
        for(counter=0;counter<LCD_COLUMNS;counter++){
        
            DELAY_MS(1000);
            LCD_scrollDisplay(LCD_LEFT,0,LCD_ROWS);
        
        }
    
        DELAY_MS(1000);
    
        /*********************************/
        /* scroll text on screen upwards */
        /*********************************/
    
        for(counter=0;counter<LCD_ROWS;counter++){
        
            DELAY_MS(1000);
            LCD_scrollDisplay(LCD_UP,0,LCD_COLUMNS);
        
        }
    
        DELAY_MS(1000);
    
        /***********************************/
        /* scroll text on screen downwards */
        /***********************************/
    
        for(counter=0;counter<LCD_ROWS;counter++){
        
            DELAY_MS(1000);
            LCD_scrollDisplay(LCD_DOWN,0,LCD_COLUMNS);
        
        }
    
        DELAY_MS(1000);
        
    }
    
}
