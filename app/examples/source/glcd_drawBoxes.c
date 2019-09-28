/*
 * glcd_drawBoxes.c
 *
 * Created: 26/09/2019 03:51:48 PM
 *  Author: Loay Ashraf
 */ 

#include "app/examples/include/examples.h"
#include "hal/ecu/glcd/api/glcd.h"

void ex_glcd_drawBoxes(void){
	
	uint8_t x,y,width,length,numBoxes;	// variables for drawing five boxes
	
	/******************************************************/
	/* initialize GLCD interface and enable graphics mode */
	/******************************************************/
	
	GLCD_init(FALSE,FALSE);
	GLCD_enableGraphics();
	
	/*************************************/
	/* draw five boxes inside each other */
	/*************************************/
	
	x = 0;
	y = 0;
	width = GLCD_PIXELS_X;
	length = GLCD_PIXELS_Y;
	numBoxes = 0;
	
	while(numBoxes++ < 5){
		
		GLCD_drawBox(x,y,width,length);
		
		width /= 2.0;
		length /= 2.0;
		x += (width+1)/2.0;
		y += (length+1)/2.0;
		
	}
	
}