/*
 * glcd_spaceInvaders.c
 *
 * Created: 12/09/2019 06:37:07 PM
 *  Author: Loay Ashraf
 */ 

#include "app/examples/include/examples.h"
#include "hal/ecu/glcd/api/glcd.h"
#include "hal/mcu/sys/delay.h"

static uword_t spaceInvader1[16] = {0x0000, 0x0000, 0x0000, 0x0000, 0x0410, 0x0220, 0x07F0, 0x0DD8, 0x1FFC, 0x17F4, 0x1414, 0x0360, 0x0000, 0x0000, 0x0000, 0x0000};	/* space invader char set for GLCD Module */
static uword_t spaceInvader2[16] =	{0x0000, 0x0000, 0x0000, 0x0000, 0x0410, 0x1224, 0x17F4, 0x1DDC, 0x1FFC, 0x0FF8, 0x0410, 0x0808, 0x0000, 0x0000, 0x0000, 0x0000};	/* space invader char set for GLCD Module */ 	
	
void ex_glcd_spaceInvaders(void){
	
	int8_t verticalPosition;	/* vertical position counter */
	
	GLCD_init(FALSE,FALSE);		/* initialize GLCD interface */
	
	/*******************************************/
	/* define space invaders custom characters */
	/*******************************************/
	
	GLCD_defineCustomCharacter(GLCD_CCHARACTER_ZERO,spaceInvader1);
	GLCD_defineCustomCharacter(GLCD_CCHARACTER_ONE,spaceInvader2);
	GLCD_setCursorPosition(2,3);
	GLCD_puts("SI");
	
	while(1){		/* loop forever */
		
		
		/*******************************************************************************/
		/* two loops to cycle the two space invaders characters up and down the screen */
		/*******************************************************************************/
		
		for(verticalPosition=1;verticalPosition<GLCD_ROWS;verticalPosition++){
			
			GLCD_setCursorPosition(verticalPosition,2);
			GLCD_putw(GLCD_CCHARACTER_ZERO);
			GLCD_setCursorPosition((GLCD_ROWS-verticalPosition-1),5);
			GLCD_putw(GLCD_CCHARACTER_ONE);
			DELAY_MS(1000);
			GLCD_setCursorPosition(verticalPosition,2);
			GLCD_putw(GLCD_CCHARACTER_ONE);
			GLCD_setCursorPosition((GLCD_ROWS-verticalPosition-1),5);
			GLCD_putw(GLCD_CCHARACTER_ZERO);
			DELAY_MS(1000);
			
			GLCD_clearDisplay();
			
		}
		
		for(verticalPosition=GLCD_ROWS-2;verticalPosition>=0;verticalPosition--){
			
			GLCD_setCursorPosition(verticalPosition,2);
			GLCD_putw(GLCD_CCHARACTER_ZERO);
			GLCD_setCursorPosition((GLCD_ROWS-verticalPosition-1),5);
			GLCD_putw(GLCD_CCHARACTER_ONE);
			DELAY_MS(1000);
			GLCD_setCursorPosition(verticalPosition,2);
			GLCD_putw(GLCD_CCHARACTER_ONE);
			GLCD_setCursorPosition((GLCD_ROWS-verticalPosition-1),5);
			GLCD_putw(GLCD_CCHARACTER_ZERO);
			DELAY_MS(1000);
			
			GLCD_clearDisplay();
			
		}
		
	}
	
}	