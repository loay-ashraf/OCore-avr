/**********************************************************************
*
* File:         glcd.c
*
* Author(s):    Loay Ashraf <loay.ashraf.96@gmail.com>
*
* Date created: 09/09/2019
*
* Description:	contains function definitions for GLCD module.
*
**********************************************************************/

/*------------------------------INCLUDES-----------------------------*/

 #include "glcd.h"
 #include "hal/ecu/glcd/common/glcd_macros.h"
 #include <math.h>
 #include <stdlib.h>
 #include <stdio.h>

/*-----------------------FUNCTION DEFINITIONS------------------------*/

/**********************************************************************
*
* Function:    GLCD_init
*
* Description: Initializes GLCD module.
*
* Notes:
*
* Returns:     None.
*
**********************************************************************/

void GLCD_init(bool_t a_backlightON){
	
	GLCD_INIT(a_backlightON);
	
}

/**********************************************************************
*
* Function:    GLCD_sendInstruction
*
* Description: Sends instructions to GLCD module.
*
* Notes:
*
* Returns:     None.
*
**********************************************************************/

void GLCD_sendInstruction(ubyte_t a_instruction){
	
	GLCD_SEND_INSTRUCTION(a_instruction);
   
}

/**********************************************************************
*
* Function:    GLCD_clearDisplay
*
* Description: Clears display of GLCD module.
*
* Notes:
*
* Returns:     None.
*
**********************************************************************/
 
void GLCD_clearDisplay(void){
	
	GLCD_CLEAR_DISLAY;

}

/**********************************************************************
*
* Function:    GLCD_configBacklight
*
* Description: Configures backlight of GLCD module.
*
* Notes:
*
* Returns:     None.
*
**********************************************************************/
 
void GLCD_configBacklight(bool_t a_backlightON){
	 
	GLCD_CONFIG_BACKLIGHT(a_backlightON);
	 
}

/**********************************************************************
*
* Function:    GLCD_setCursorPosition
*
* Description: Sets cursor position (X & Y) of GLCD module.
*
* Notes:
*
* Returns:     None.
*
**********************************************************************/

void GLCD_setCursorPosition(uint8_t a_x, uint8_t a_y){
		
	GLCD_SET_CURSOR_POSITION(a_x,a_y);
		
}

/**********************************************************************
*
* Function:    GLCD_setFont
*
* Description: Sets text font for GLCD module.
*
* Notes:
*
* Returns:     None.
*
**********************************************************************/
 
void GLCD_setFont(glcdfont_t a_font){
	 
	GLCD_SET_FONT(a_font);
	 
}

/**********************************************************************
*
* Function:    GLCD_scrollDisplay
*
* Description: Scrolls display in specific direction of GLCD module.
*
* Notes:
*
* Returns:     None.
*
**********************************************************************/
 
void GLCD_scrollDisplay(glcddirection_t a_direction){
	 
	GLCD_SCROLL_DISPLAY(a_direction);
	 
}

/**********************************************************************
*
* Function:    GLCD_putc
*
* Description: Prints character to display of GLCD module.
*
* Notes:
*
* Returns:     None.
*
**********************************************************************/
	
void GLCD_putc(char a_char){
	
	GLCD_PUTC(a_char);
	
}

/**********************************************************************
*
* Function:    GLCD_putw
*
* Description: Prints word-sized value to display of GLCD module.
*
* Notes:
*
* Returns:     None.
*
**********************************************************************/

void GLCD_putw(uword_t a_word){
	
	GLCD_putc((a_word>>8)&0xFF);
	GLCD_putc(a_word&0xFF);
	
}

/**********************************************************************
*
* Function:    GLCD_puts
*
* Description: Prints string to display of GLCD module.
*
* Notes:
*
* Returns:     None.
*
**********************************************************************/

void GLCD_puts(const char * a_data){
		
	GLCD_PUTS(a_data);
	
}

/**********************************************************************
*
* Function:    GLCD_puti
*
* Description: Prints integer number to display of GLCD module.
*
* Notes:
*
* Returns:     None.
*
**********************************************************************/
 
void GLCD_puti(udword_t a_data, glcdnumberbase_t a_numberBase){
	 
	char buffer[GLCD_BUFFER_SIZE];
	itoa(a_data,buffer,a_numberBase);
	
	if(a_numberBase==GLCD_BASE16)	 
		GLCD_puts("0x");
	else if(a_numberBase==GLCD_BASE2)	
		GLCD_puts("0b"); 
		
	GLCD_puts(buffer);
	 
}

/**********************************************************************
*
* Function:    GLCD_putf
*
* Description: Prints float number to display of GLCD module.
*
* Notes:
*
* Returns:     None.
*
**********************************************************************/
 
void GLCD_putf(float a_data){
	 
	char buffer[GLCD_BUFFER_SIZE];
	dtostrf(a_data, 0, GLCD_FRACTION_SIZE, buffer);
	GLCD_puts(buffer);
	 
}

/**********************************************************************
*
* Function:    GLCD_putp
*
* Description: Prints pointer value to display of GLCD module.
*
* Notes:
*
* Returns:     None.
*
**********************************************************************/

void GLCD_putp(void * a_ptr){
	 
	char buffer[8];
	sprintf(buffer,"%p",a_ptr);
	GLCD_puts(buffer);
	 
}

/**********************************************************************
*
* Function:    GLCD_getc
*
* Description: Reads character from display of GLCD module.
*
* Notes:
*
* Returns:     Character at current cursor position (type: char).
*
**********************************************************************/
 
char GLCD_getc(void){

	return GLCD_GETC;
	
}

/**********************************************************************
*
* Function:    GLCD_getw
*
* Description: Reads word-sized value from display of GLCD module.
*
* Notes:
*
* Returns:     Word at current cursor position (type: uword_t).
*
**********************************************************************/

uword_t GLCD_getw(void){
	
	ubyte_t byte = 0;
	uword_t word = 0;
	byte = GLCD_GETC;		/* dummy read */
	byte = GLCD_GETC;
	word |= ((byte<<8)&0xFF00);
	byte = GLCD_GETC;
	word |= (byte&0x00FF);
	
	return word;
	
}

/**********************************************************************
*
* Function:    GLCD_drawPixel
*
* Description: Draws pixel at current cursor position (X & Y) on display
*              of GLCD module.
*
* Notes:
*
* Returns:     None.
*
**********************************************************************/

void GLCD_drawPixel(uint8_t a_x, uint8_t a_y){
	
	GLCD_DRAW_PIXEL(a_x,a_y);
	
}

/**********************************************************************
*
* Function:    GLCD_drawLine
*
* Description: Draws line on display of GLCD module.
*
* Notes:
*
* Returns:     None.
*
**********************************************************************/

void GLCD_drawLine(uint8_t a_startX, uint8_t a_startY, uint8_t a_endX, uint8_t a_endY){
	
	int8_t dX,dY;
	
	dX = (int8_t)(a_endX-a_startX);
	dY = (int8_t)(a_endY-a_startY);
	
	if(dX == 0){
		
		int8_t Ycounter;
		
		if(dY > 0){
			
			for(Ycounter=a_startY;Ycounter<=a_endY;Ycounter++)
				GLCD_drawPixel(a_startX,Ycounter);
			
		}else if(dY < 0){
			
			for(Ycounter=a_startY;Ycounter>=a_endY;Ycounter--)
				GLCD_drawPixel(a_startX,Ycounter);
			
		}
		
	}else{
		
		int16_t x,y;
		float m,c;

		m = (float)(a_endY-a_startY)/(float)(a_endX-a_startX);
		c = a_startY-(m*a_startX);
		
		if(dX > 0){
			
			for(x=a_startX;x<=a_endX;x++){

				y = round((m*x)+c);
				GLCD_drawPixel(x,y);

			}
			
		}else if(dX < 0){
			
			for(x=a_startX;x>=a_endX;x--){

				y = round((m*x)+c);
				GLCD_drawPixel(x,y);

			}
			
		}
	}
	
}

/**********************************************************************
*
* Function:    GLCD_drawBox
*
* Description: Draws box on display of GLCD module.
*
* Notes:
*
* Returns:     None.
*
**********************************************************************/

void GLCD_drawBox(uint8_t a_startX, uint8_t a_startY, uint8_t a_width, uint8_t a_length){
	
	GLCD_drawLine(a_startX,a_startY,a_startX+a_width-1,a_startY);
	GLCD_drawLine(a_startX+a_width-1,a_startY,a_startX+a_width-1,a_startY+a_length-1);
	GLCD_drawLine(a_startX+a_width-1,a_startY+a_length-1,a_startX,a_startY+a_length-1);
	GLCD_drawLine(a_startX,a_startY+a_length-1,a_startX,a_startY);
	
}

/**********************************************************************
*
* Function:    GLCD_drawFilledBox
*
* Description: Draws filled box on display of GLCD module.
*
* Notes:
*
* Returns:     None.
*
**********************************************************************/

void GLCD_drawFilledBox(uint8_t a_startX, uint8_t a_startY, uint8_t a_width, uint8_t a_length){
	
	uint8_t x,y;
	
	for(x=a_startX;x<=a_startX+a_width;x++){
		
		for(y=a_startY;y<=a_startY+a_length;y++)
			GLCD_drawPixel(x,y);
		
	}
	
}

/**********************************************************************
*
* Function:    GLCD_drawCircle
*
* Description: Draws circle on display of GLCD module.
*
* Notes:
*
* Returns:     None.
*
**********************************************************************/

void GLCD_drawCircle(uint8_t a_centerX, uint8_t a_centerY, uint8_t a_radius){
	
	if((a_centerX+a_radius-1) > (GLCD_X_PIXELS-1))
		a_radius = (GLCD_X_PIXELS-1)-a_centerX;
	else if((int8_t)(a_centerX-(a_radius-1)) < 0)	
		a_radius = a_centerX+1;
		
	if((a_centerY+a_radius-1) > (GLCD_Y_PIXELS-1))	
		a_radius = (GLCD_Y_PIXELS-1)-a_centerY;
	else if((int8_t)(a_centerY-(a_radius-1)) < 0)
		a_radius = a_centerY+1;	
		
	uint8_t x,y;
	int8_t d;	
	
	x = 0;
	y = a_radius-1;
	d = 3-(2*a_radius);
	
	 GLCD_drawPixel(a_centerX+x,a_centerY+y);
	 GLCD_drawPixel(a_centerX-x,a_centerY+y);
	 GLCD_drawPixel(a_centerX+x,a_centerY-y);
	 GLCD_drawPixel(a_centerX-x,a_centerY-y);
	 GLCD_drawPixel(a_centerX+y,a_centerY+x);
	 GLCD_drawPixel(a_centerX-y,a_centerY+x);
	 GLCD_drawPixel(a_centerX+y,a_centerY-x);
	 GLCD_drawPixel(a_centerX-y,a_centerY-x);
	 
	 while (y >= x){
		 
		 /* for each pixel we will draw all eight pixels */
		 
		 x++;
		 
		 /* check for decision parameter and correspondingly update d, x, y */

		 if (d > 0){
			 
			 y--;
			 d = d+(4*(x-y))+10;
			 
		 }
		 else
			d = d+(4*x)+6;
			
		 GLCD_drawPixel(a_centerX+x,a_centerY+y);
		 GLCD_drawPixel(a_centerX-x,a_centerY+y);
		 GLCD_drawPixel(a_centerX+x,a_centerY-y);
		 GLCD_drawPixel(a_centerX-x,a_centerY-y);
		 GLCD_drawPixel(a_centerX+y,a_centerY+x);
		 GLCD_drawPixel(a_centerX-y,a_centerY+x);
		 GLCD_drawPixel(a_centerX+y,a_centerY-x);
		 GLCD_drawPixel(a_centerX-y,a_centerY-x);
		
	 }
	
}

/**********************************************************************
*
* Function:    GLCD_drawVerticalBar
*
* Description: Draws vertical bar on display of GLCD module.
*
* Notes:
*
* Returns:     None.
*
**********************************************************************/

void GLCD_drawVerticalBar(glcdbarindex_t a_barIndex, uint8_t a_value, uint8_t a_minValue, uint8_t a_maxValue){
	
	GLCD_DRAW_VERTICAL_BAR(a_barIndex,a_value,a_minValue,a_maxValue);
	
}

/**********************************************************************
*
* Function:    GLCD_drawHorizontalBar
*
* Description: Draws horizontal bar on display of GLCD module.
*
* Notes:
*
* Returns:     None.
*
**********************************************************************/

void GLCD_drawHorizontalBar(glcdbarindex_t a_barIndex, uint8_t a_value, uint8_t a_minValue, uint8_t a_maxValue){
	
	GLCD_DRAW_HORIZONTAL_BAR(a_barIndex,a_value,a_minValue,a_maxValue);
	
}

/**********************************************************************
*
* Function:    GLCD_fillDisplay
*
* Description: Fills display of GLCD module.       
*
* Notes:
*
* Returns:     None.
*
**********************************************************************/

void GLCD_fillDisplay(ubyte_t a_pattern){
	
	GLCD_FILL_DISPLAY(a_pattern);
	
}

/**********************************************************************
*
* Function:    GLCD_putImageRAM
*
* Description: Draws bitmap image on display of GLCD module.
*
* Notes:       This function accepts bitmap images stored in RAM.
*
* Returns:     None.
*
**********************************************************************/

void GLCD_putImageRAM(const ubyte_t * a_image){
	
	GLCD_PUT_IMAGE_RAM(a_image);
	
}

/**********************************************************************
*
* Function:    GLCD_putImageROM
*
* Description: Draws bitmap image on display of GLCD module.
*
* Notes:       This function accepts bitmap images stored in ROM.
*
* Returns:     None.
*
**********************************************************************/

void GLCD_putImageROM(const ubyte_t * a_image){
	
	GLCD_PUT_IMAGE_ROM(a_image);
	
}
 