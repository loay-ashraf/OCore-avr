/*
 * glcd_text.c
 *
 * Created: 28/09/2019 09:35:30 AM
 *  Author: Loay Ashraf
 */ 

#include "app/examples/include/examples.h"
#include "hal/ecu/glcd/api/glcd.h"

void ex_glcd_text(void){
    
    uint8_t y = 0;
    
    /******************************************************/
    /* initialize GLCD interface and enable graphics mode */
    /******************************************************/
    
    GLCD_init(TRUE);
    
    /***************************/
    /* display 8 lines of text */
    /***************************/
    
    while(y<8){
        
        GLCD_setCursorPosition(0,y*8);
        GLCD_puts("I'M IN GFX MODE!");
        y++;
        
    }
    
}

