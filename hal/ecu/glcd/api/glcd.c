/*
 * glcd.c
 *
 * Created: 09/09/2019 07:04:18 AM
 *  Author: Loay Ashraf
 */ 

/*------------INCLUDE DRIVER HEADER FILE------------*/

 #include "glcd.h"
 #include "glcd_macros.h"
 #include <math.h>
 #include <stdlib.h>
 #include <stdio.h>

 void GLCD_init(void){
	
	GLCD_INIT();
	
 }

 void GLCD_sendInstruction(ubyte_t a_instruction){
	
	GLCD_SEND_INSTRUCTION(a_instruction);
   
 }
 
 void GLCD_clearDisplay(void){
	
	GLCD_CLEAR_DISLAY;

 }

 void GLCD_setCursorPosition(uint8_t a_x, uint8_t a_y){
		
	GLCD_SET_CURSOR_POSITION(a_x,a_y);
		
 }
 
 void GLCD_setFont(glcdfont_t a_font){
	 
	GLCD_SET_FONT(a_font);
	 
 }
	
 void GLCD_putc(char a_char){
	
	GLCD_PUTC(a_char);
	
}

void GLCD_putw(uword_t a_word){
	
	GLCD_putc((a_word>>8)&0xFF);
	GLCD_putc(a_word&0xFF);
	
}

 void GLCD_puts(const char * a_data){
		
	GLCD_PUTS(a_data);
	
 }
 
 void GLCD_puti(udword_t a_data, glcdnumberbase_t a_numberBase){
	 
	char buffer[GLCD_BUFFER_SIZE];
	itoa(a_data,buffer,a_numberBase);
	
	if(a_numberBase==GLCD_BASE16)	 
		GLCD_puts("0x");
	else if(a_numberBase==GLCD_BASE2)	
		GLCD_puts("0b"); 
		
	GLCD_puts(buffer);
	 
 }
 
 void GLCD_putf(float a_data){
	 
	 char buffer[GLCD_BUFFER_SIZE];
	 dtostrf(a_data, 0, GLCD_FRACTION_SIZE, buffer);
	 GLCD_puts(buffer);
	 
 }

 void GLCD_putp(void * a_ptr){
	 
	 char buffer[8];
	 sprintf(buffer,"%p",a_ptr);
	 GLCD_puts(buffer);
	 
 }
 
char GLCD_getc(void){

	return GLCD_GETC;
	
}

uword_t GLCD_getw(void){
	
	ubyte_t byte = 0;
	uword_t word = 0;
	byte = GLCD_GETC;		// dummy read
	byte = GLCD_GETC;
	word |= ((byte<<8)&0xFF00);
	byte = GLCD_GETC;
	word |= (byte&0x00FF);
	
	return word;
	
}

void GLCD_drawPixel(uint8_t a_x, uint8_t a_y){
	
	GLCD_DRAW_PIXEL(a_x,a_y);
	
}

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

void GLCD_drawBox(uint8_t a_startX, uint8_t a_startY, uint8_t a_width, uint8_t a_length){
	
	GLCD_drawLine(a_startX,a_startY,a_startX+a_width-1,a_startY);
	GLCD_drawLine(a_startX+a_width-1,a_startY,a_startX+a_width-1,a_startY+a_length-1);
	GLCD_drawLine(a_startX+a_width-1,a_startY+a_length-1,a_startX,a_startY+a_length-1);
	GLCD_drawLine(a_startX,a_startY+a_length-1,a_startX,a_startY);
	
}

void GLCD_drawFilledBox(uint8_t a_startX, uint8_t a_startY, uint8_t a_width, uint8_t a_length){
	
	uint8_t x,y;
	
	for(x=a_startX;x<=a_startX+a_width;x++){
		
		for(y=a_startY;y<=a_startY+a_length;y++)
			GLCD_drawPixel(x,y);
		
	}
	
}

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
		 
		 // for each pixel we will
		 // draw all eight pixels
		 
		 x++;
		 
		 // check for decision parameter
		 // and correspondingly
		 // update d, x, y
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

void GLCD_drawVerticalBar(glcdbarindex_t a_barIndex, uint8_t a_value, uint8_t a_minValue, uint8_t a_maxValue){
	
	GLCD_DRAW_VERTICAL_BAR(a_barIndex,a_value,a_minValue,a_maxValue);
	
}

void GLCD_drawHorizontalBar(glcdbarindex_t a_barIndex, uint8_t a_value, uint8_t a_minValue, uint8_t a_maxValue){
	
	GLCD_DRAW_HORIZONTAL_BAR(a_barIndex,a_value,a_minValue,a_maxValue);
	
}

void GLCD_fillDisplay(ubyte_t a_pattern){
	
	GLCD_FILL_DISPLAY(a_pattern);
	
}

void GLCD_putImageRAM(const ubyte_t * a_image){
	
	GLCD_PUT_IMAGE_RAM(a_image);
	
}

void GLCD_putImageROM(const ubyte_t * a_image){
	
	GLCD_PUT_IMAGE_ROM(a_image);
	
}
 