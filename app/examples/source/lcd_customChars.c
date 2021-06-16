/*
 * lcd_customChars.c
 *
 * Created: 12/09/2019 06:37:07 PM
 *  Author: Loay Ashraf
 */ 

#include "app/examples/include/examples.h"
#include "hal/ecu/display/lcd/api/lcd.h"

static ubyte_t Heart[] = {0x00,0x0A,0x1F,0x1F,0x0E,0x04,0x00,0x00};
static ubyte_t Bell[] = {0x04,0x0E,0x0E,0x0E,0x1F,0x00,0x04,0x00};
static ubyte_t Alien[] = {0x1F,0x15,0x1F,0x1F,0x0E,0x0A,0x1B,0x00};
static ubyte_t Check[] = {0x00,0x01,0x03,0x16,0x1C,0x08,0x00,0x00};
static ubyte_t Speaker[] = {0x01,0x03,0x0F,0x0F,0x0F,0x03,0x01,0x00};
static ubyte_t Sound[] = {0x01,0x03,0x05,0x09,0x09,0x0B,0x1B,0x18};
static ubyte_t Skull[] = {0x00,0x0E,0x15,0x1B,0x0E,0x0E,0x00,0x00};
static ubyte_t Lock[] = {0x0E,0x11,0x11,0x1F,0x1B,0x1B,0x1F,0x00};
    
void ex_lcd_customChars(void){
    
    LCD_init(TRUE,FALSE,FALSE,TRUE);        /* initialize LCD interface */
    
    /********************************/
    /* define LCD custom characters */
    /********************************/
    
    LCD_defineCustomCharacter(LCD_CCHAR0,Heart);
    LCD_defineCustomCharacter(LCD_CCHAR1,Bell);
    LCD_defineCustomCharacter(LCD_CCHAR2,Alien);
    LCD_defineCustomCharacter(LCD_CCHAR3,Check);
    LCD_defineCustomCharacter(LCD_CCHAR4,Speaker);
    LCD_defineCustomCharacter(LCD_CCHAR5,Sound);
    LCD_defineCustomCharacter(LCD_CCHAR6,Skull);
    LCD_defineCustomCharacter(LCD_CCHAR7,Lock);
    
    /***********************************/
    /* put custom characters on screen */
    /***********************************/
    
    #if (LCD_COLUMNS == 16)
    
        LCD_setCursorPosition(0,0);
        LCD_puts("Custom Character");
        
        LCD_setCursorPosition(1,0);
        LCD_putc(LCD_CCHAR0);
        LCD_setCursorPosition(1,2);
        LCD_putc(LCD_CCHAR1);
        LCD_setCursorPosition(1,4);
        LCD_putc(LCD_CCHAR2);
        LCD_setCursorPosition(1,6);
        LCD_putc(LCD_CCHAR3);
        LCD_setCursorPosition(1,8);
        LCD_putc(LCD_CCHAR4);
        LCD_setCursorPosition(1,10);
        LCD_putc(LCD_CCHAR5);
        LCD_setCursorPosition(1,12);
        LCD_putc(LCD_CCHAR6);
        LCD_setCursorPosition(1,14);
        LCD_putc(LCD_CCHAR7);
        
    #elif (LCD_COLUMNS == 20)    
    
        LCD_setCursorPosition(1,2);
        LCD_puts("Custom Character");
        
        LCD_setCursorPosition(2,2);
        LCD_putc(LCD_CCHAR0);
        LCD_setCursorPosition(2,4);
        LCD_putc(LCD_CCHAR1);
        LCD_setCursorPosition(2,6);
        LCD_putc(LCD_CCHAR2);
        LCD_setCursorPosition(2,8);
        LCD_putc(LCD_CCHAR3);
        LCD_setCursorPosition(2,10);
        LCD_putc(LCD_CCHAR4);
        LCD_setCursorPosition(2,12);
        LCD_putc(LCD_CCHAR5);
        LCD_setCursorPosition(2,14);
        LCD_putc(LCD_CCHAR6);
        LCD_setCursorPosition(2,16);
        LCD_putc(LCD_CCHAR7);
    
    #endif
    
}    
