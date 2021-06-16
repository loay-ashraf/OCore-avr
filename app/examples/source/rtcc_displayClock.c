/*
 * rtcc_displayClockStr.c
 *
 * Created: 12/09/2019 06:30:17 PM
 *  Author: Loay Ashraf
 */ 

#include "app/examples/include/examples.h"
#include "hal/ecu/display/lcd/api/lcd.h"
#include "hal/ecu/rtcc/api/rtcc.h"

void ex_rtcc_displayClock(void){
    
    /**************************************/
    /* initialize LCD and RTCC interfaces */
    /**************************************/
    
    LCD_init(TRUE,FALSE,FALSE,TRUE);
    RTCC_enable();
    
    while(1){        /* loop forever */
        
        /************************************/
        /* print current time to LCD screen */
        /************************************/
        
        LCD_setCursorPosition(0,0);
        LCD_puts(RTCC_getClockStr());
        
    }
    
}
