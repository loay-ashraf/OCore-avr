/*
 * glcd_drawCircles.c
 *
 * Created: 26/09/2019 04:27:54 PM
 *  Author: Loay Ashraf
 */ 

#include "app/examples/include/examples.h"
#include "hal/ecu/display/glcd/api/glcd.h"

void ex_glcd_drawCircles(void){
    
    uint8_t x,radius,numCircles;
    
    /******************************************************/
    /* initialize GLCD interface and enable graphics mode */
    /******************************************************/
    
    GLCD_init(TRUE);
    
    /**************************************************/
    /* draw a chain of five circles beside each other */
    /**************************************************/
    
    radius = GLCD_X_PIXELS/6.0;
    x = radius+2;
    numCircles = 0;
    
    while(numCircles++ < 5){
        
        GLCD_drawCircle(x,31,radius);
        x += radius-1;
        
    }
    
}



