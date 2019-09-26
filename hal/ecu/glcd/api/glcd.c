/*
 * glcd.c
 *
 * Created: 09/09/2019 07:04:18 AM
 *  Author: Loay Ashraf
 */ 
//------------INCLUDE DRIVER HEADER FILE------------//

 #include "glcd.h"
 #include "glcd_macros.h"
 #include <math.h>
 #include <stdlib.h>
 #include <stdio.h>
 #include <string.h>
 
//------------DECLARE LOCAL VARIABLES------------//
 
/** @var Port data_port_G
 *  @brief holds structure instance of the data port
 */

/** @var Port ctrl_port_G
 *  @brief holds structure instance of the control port
 */

/** @var uint8 cursor_pos
 *  @brief holds current cursor position (row, col) 
 */

static glcd_t g_GLCD;

static void initBuffer(void);
static void putBuffer(void);	
static void clearBuffer(void);
static void setFrameBufferUpdateFlag(void);
static void clearFrameBufferUpdateFlag(void);
static void shiftUnits(glcddirection_t a_direction, ubyte_t a_offset, ubyte_t a_width);
static void scrollUnits(glcddirection_t a_direction, ubyte_t a_offset, ubyte_t a_width);
static void resetCursorConfig(void);
static void saveCursorConfig(bool_t a_cursorVisible, bool_t a_cursorBlinking);
static void restoreCursorConfig(void);
static void updateCursorPosition(glcddirection_t a_dir);
static void saveCursorPosition(ubyte_t a_row, ubyte_t a_column);	
static void restoreCursorPosition(glcddirection_t a_dir, ubyte_t a_offset, ubyte_t a_width);
static void setCursorPositionUpdateFlag(void);
static void clearCursorPositionUpdateFlag(void);

 void GLCD_init(bool_t a_cursorVisible, bool_t a_cursorBlinking){
	 
	initBuffer();		// initialize frame buffer 
	
	saveCursorConfig(a_cursorVisible,a_cursorBlinking);
	
	setCursorPositionUpdateFlag();
	setFrameBufferUpdateFlag();
	
	GLCD_INIT(a_cursorVisible,a_cursorBlinking);
	
 }
 
 void GLCD_enableGraphics(void){
	 
	clearFrameBufferUpdateFlag();
	clearCursorPositionUpdateFlag();
	resetCursorConfig();
	GLCD_ENABLE_GRAPHICS;
	 
 }
 
 void GLCD_disableGraphics(void){
	 
	GLCD_DISABLE_GRAPHICS;
	 
	setFrameBufferUpdateFlag();
	setCursorPositionUpdateFlag();
	restoreCursorConfig();
	restoreCursorPosition(GLCD_NONE,0,0);
	 
 }

 void GLCD_sendInstruction(ubyte_t a_instruction){
	
	GLCD_SEND_INSTRUCTION(a_instruction);
   
 }
 
 void GLCD_clearUnit(ubyte_t a_row, ubyte_t a_column){
	
	resetCursorConfig();						// turn off cursor
	
	GLCD_setCursorPosition(a_row, a_column);		// unit position
	
	GLCD_putc(' ');						// clear
	
	GLCD_setCursorPosition(a_row, a_column);		// unit position
	
	restoreCursorConfig();			// restore cursor configuration
	
 }
 
 void GLCD_clearRow(ubyte_t a_row, ubyte_t a_offset){
	 
	ubyte_t i;
	
	resetCursorConfig();						// turn off cursor
	
	GLCD_setCursorPosition(a_row,a_offset);		// row starting position
	
	for(i=a_offset;i<GLCD_COLUMNS;i++)
		GLCD_putc(' ');					// clear
	 	 
	GLCD_setCursorPosition(a_row,a_offset);		// row starting position
	
	restoreCursorConfig();			// restore cursor configuration
	
 }
 
 void GLCD_clearColumn(ubyte_t a_column, ubyte_t a_offset){

	ubyte_t i;
	
	resetCursorConfig();						// turn off cursor
	
	for(i = a_offset; i < GLCD_ROWS; i++){
		
		GLCD_setCursorPosition(i,a_column);
		GLCD_putc(' ');					// clear
		
	}
	
	GLCD_setCursorPosition(a_offset,a_column);		// column starting position
	
	restoreCursorConfig();			// restore cursor configuration
	 
 }
 
 void GLCD_clearDisplay(void){
	
	GLCD_CLEAR_DISLAY;
	 
	clearBuffer();					// clear frame buffer
	 
	saveCursorPosition(0,0);		// default cursor position

 }
 
 void GLCD_configCursor(bool_t a_cursorVisible, bool_t a_cursorBlinking){
		
	/* store cursor configuration */
		
	saveCursorConfig(a_cursorVisible, a_cursorBlinking);
			
	GLCD_CONFIG_CURSOR(a_cursorVisible,a_cursorBlinking);		// configure cursor
		
 }

 void GLCD_setCursorPosition(ubyte_t a_row, ubyte_t a_col){
	
	if(a_row >= GLCD_ROWS)		// check row
		a_row = 0;
	
	if(a_col >= (GLCD_COLUMNS/2))	// check column
		a_col = 0;
	
	/* update cursor position */
	
	if(g_GLCD.statusRegister.cursorpositionupdate)
		saveCursorPosition(a_row,a_col*2);
	
	GLCD_SET_CURSOR_POSITION(a_row,a_col);
	
 }
  
 void GLCD_setPixelCursorPosition(ubyte_t a_row, ubyte_t a_col){
	 
	 GLCD_SET_PIXEL_CURSOR_POSITION(a_row,a_col);
	 
 }
 
 glcdposition_t GLCD_getCursorPosition(void){
	 
	 return g_GLCD.cursorPos;
	 
 }
 
 void GLCD_shiftCursor(glcddirection_t a_dir){
	 
	 updateCursorPosition(a_dir);
	 
	 switch (a_dir){
						   		   	
		case GLCD_LEFT: {
			
			#if (GLCD_CURSOR_SW_SHFT == 1)

				if(g_GLCD.cursorPos.column == GLCD_COLUMNS-1){
					
					clearCursorPositionUpdateFlag();
					GLCD_setCursorPosition(g_GLCD.cursorPos.row,g_GLCD.cursorPos.column);
					setCursorPositionUpdateFlag();
					
					}else{
					
					GLCD_SHIFT_CURSOR(a_dir);
					
				}
				
			#else
			
				GLCD_SHIFT_CURSOR(a_dir);
			
			#endif
				
		}
		break;
			
		case GLCD_RIGHT: {
			
			#if (GLCD_CURSOR_SW_SHFT == 1)
				
				if(g_GLCD.cursorPos.column == 0){
					
					clearCursorPositionUpdateFlag();
					GLCD_setCursorPosition(g_GLCD.cursorPos.row,g_GLCD.cursorPos.column);
					setCursorPositionUpdateFlag();
					
					}else{
					
					GLCD_SHIFT_CURSOR(a_dir);
					
				}
			
			#else
			
				GLCD_SHIFT_CURSOR(a_dir);
			
			#endif
			
		}
		break;
					   
		case GLCD_UP:	 
		case GLCD_DOWN: {
			
			clearCursorPositionUpdateFlag();
			GLCD_setCursorPosition(g_GLCD.cursorPos.row,g_GLCD.cursorPos.column);
			setCursorPositionUpdateFlag();
		
		}
		break;		 		 	 	 		 
		
		default: break;
	}	
 }

 void GLCD_shiftDisplay(glcddirection_t a_dir, ubyte_t a_offset, ubyte_t a_width){
		 
	switch(a_dir){
		 
		case GLCD_LEFT:
		case GLCD_RIGHT: {
			  
			if((a_offset+a_width)>GLCD_ROWS)
				a_width = GLCD_ROWS-a_offset;
			  
			shiftUnits(a_dir,a_offset,a_width);
			putBuffer();
			  
			switch(a_dir) {
				  
				case GLCD_LEFT: GLCD_setCursorPosition(a_offset,GLCD_COLUMNS-1);
				break;
				case GLCD_RIGHT: GLCD_setCursorPosition(a_offset,0);
				break;
				default: break;
				  
			  }	    
		}				 
		break;
		 		 
		case GLCD_UP:
		case GLCD_DOWN: {
			 
			if((a_offset+a_width)>GLCD_COLUMNS)
				a_width = GLCD_COLUMNS-a_offset;
			
			shiftUnits(a_dir, a_offset, a_width);
			putBuffer();
				
			switch(a_dir) {
					
				case GLCD_UP: GLCD_setCursorPosition(GLCD_ROWS-1,a_offset);
				break;	
				case GLCD_DOWN: GLCD_setCursorPosition(0,a_offset);	
				break;
				default: break;
				
			 }
		} 
		break;	
		
		default: break;						 
	}
 }

 void GLCD_scrollDisplay(glcddirection_t a_dir, ubyte_t a_offset, ubyte_t a_width){
	 
	switch(a_dir){
		 
		case GLCD_LEFT:	
		case GLCD_RIGHT: {
			 
			if((a_offset+a_width)>GLCD_ROWS)
				a_width = GLCD_ROWS-a_offset;
			 
		}
		break;
		 
		case GLCD_UP:
		case GLCD_DOWN: {
			 
			if((a_offset+a_width)>GLCD_COLUMNS)
				a_width = GLCD_COLUMNS-a_offset;
				
		} 
		break;
		 
		default: break;
	}  
	
	scrollUnits(a_dir, a_offset, a_width);  
	putBuffer();
	restoreCursorPosition(a_dir, a_offset, a_width);
	
 }
 
 void GLCD_scrollDisplayNative(glcddirection_t a_dir){
	 
	if(a_dir == GLCD_LEFT || a_dir == GLCD_RIGHT)
		GLCD_SCROLL_DISPLAY(a_dir);
	 
 }
 
 void GLCD_setBuffer(char a_buffer[GLCD_ROWS][GLCD_COLUMNS+1]){
	 
	 ubyte_t rows;
	 
	 for(rows = 0; rows < GLCD_ROWS; rows++)
	 
		strcpy(g_GLCD.frameBuffer[rows],a_buffer[rows]);
	 
	 putBuffer();
	 
 }
 
 void GLCD_copyBuffer(char a_returnedBuffer[GLCD_ROWS][GLCD_COLUMNS+1]){
	 
	ubyte_t rows;
	
	for(rows = 0; rows < GLCD_ROWS; rows++)
	
		strcpy(a_returnedBuffer[rows],g_GLCD.frameBuffer[rows]);
		
 }	
 
 void GLCD_defineCustomCharacter(ubyte_t a_characterIndex, uword_t a_characterArray[16]){
	
	clearCursorPositionUpdateFlag();
	clearFrameBufferUpdateFlag();
	
	GLCD_DEFINE_CUSTOM_CHARACTER(a_characterIndex,a_characterArray);
	
	setCursorPositionUpdateFlag();
	setFrameBufferUpdateFlag();
	
	GLCD_setCursorPosition(0,0);
	
 }
	
 void GLCD_putc(char a_char){
	
	GLCD_PUTC(a_char);
	
	//------------UPDATE DISPLAY BUFFER------------//
	
	if(g_GLCD.statusRegister.framebufferupdate)
		g_GLCD.frameBuffer[g_GLCD.cursorPos.row][g_GLCD.cursorPos.column] = a_char;
	
	//------------UPDATE CURSOR POSITION------------//
	
	if(g_GLCD.statusRegister.cursorpositionupdate){
		
		updateCursorPosition(GLCD_RIGHT);
		
		#if (LCD_CURSOR_SW_SHFT == 1)
		
			if(g_GLCD.cursorPos.column == 0){
				
				clearCursorPositionUpdateFlag();
				GLCD_setCursorPosition(g_GLCD.cursorPos.row,g_GLCD.cursorPos.column);
				setCursorPositionUpdateFlag();
				
			}
		
		#endif
		
	}
}

void GLCD_putw(uword_t a_word){
	
	GLCD_putc((a_word>>8)&0xFF);
	GLCD_putc(a_word&0xFF);
	
}

 void GLCD_puts(const char * a_data){

	while(*a_data != '\0')
		GLCD_putc(*a_data++);

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
 
char GLCD_readChar(void){

	return GLCD_READ_CHAR;
	
}

uword_t GLCD_readWord(void){
	
	ubyte_t byte = 0;
	uword_t word = 0;
	byte = GLCD_READ_CHAR;		// dummy read
	byte = GLCD_READ_CHAR;
	word |= ((byte<<8)&0xFF00);
	byte = GLCD_READ_CHAR;
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
	
	if((a_centerX+a_radius-1) > (GLCD_PIXELS_X-1))
		a_radius = (GLCD_PIXELS_X-1)-a_centerX;
	else if((int8_t)(a_centerX-(a_radius-1)) < 0)	
		a_radius = a_centerX+1;
		
	if((a_centerY+a_radius-1) > (GLCD_PIXELS_Y-1))	
		a_radius = (GLCD_PIXELS_Y-1)-a_centerY;
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

void GLCD_fillDisplay(uword_t a_pattern){
	
	GLCD_FILL_DISPLAY(a_pattern);
	
}

void GLCD_putImageRAM(const ubyte_t * a_image){
	
	GLCD_PUT_IMAGE_RAM(a_image);
	
}

void GLCD_putImageROM(const ubyte_t * a_image){
	
	GLCD_PUT_IMAGE_ROM(a_image);
	
}
 
 static void initBuffer(void){
	
	ubyte_t rows;
	
	clearBuffer();
	
	for(rows = 0; rows < GLCD_ROWS; rows++)
		g_GLCD.frameBuffer[rows][GLCD_COLUMNS]='\0';
	 
 }
 
 static void putBuffer(void){
	
	clearFrameBufferUpdateFlag();
	clearCursorPositionUpdateFlag(); 
	resetCursorConfig();
	
	ubyte_t rows;
		
	for(rows = 0; rows < GLCD_ROWS; rows++){
		GLCD_setCursorPosition(rows,0);
		GLCD_puts(g_GLCD.frameBuffer[rows]);
	}
	
	setFrameBufferUpdateFlag();
	setCursorPositionUpdateFlag();
	restoreCursorConfig();
	
 }
 
 static void clearBuffer(void){
	 
	ubyte_t rows,cols;
	 
	for(rows = 0; rows < GLCD_ROWS; rows++){
		
		for(cols = 0; cols < GLCD_COLUMNS; cols++)
			 g_GLCD.frameBuffer[rows][cols]= ' ';
		 
	}
	  
 }
 
 static void setFrameBufferUpdateFlag(void){
	 
	 g_GLCD.statusRegister.framebufferupdate = TRUE;
	 
 }
 
 static void clearFrameBufferUpdateFlag(void){
	 
	 g_GLCD.statusRegister.framebufferupdate = FALSE;
	 
 }
 
 void shiftUnits(glcddirection_t a_direction, ubyte_t a_offset, ubyte_t a_width){
	  
	ubyte_t rows;
	  
	switch(a_direction){
		
		case GLCD_UP: {
			
			for(rows = 0; rows < GLCD_ROWS-1; rows++)
			
				strncpy(g_GLCD.frameBuffer[rows]+a_offset,g_GLCD.frameBuffer[rows+1]+a_offset,a_width);
			
			for(rows = a_offset; rows < a_offset+a_width; rows++)
			
				g_GLCD.frameBuffer[GLCD_ROWS-1][rows]=' ';
			
		}
		break;

		case GLCD_DOWN: {
			
			for(rows = GLCD_ROWS-1; rows > 0; rows--)
			
				strncpy(g_GLCD.frameBuffer[rows]+a_offset,g_GLCD.frameBuffer[rows-1]+a_offset,a_width);
			
			for(rows = a_offset; rows < a_offset+a_width; rows++)
			
				g_GLCD.frameBuffer[0][rows]=' ';
			
		}
		break;
		
		case GLCD_LEFT: {
			
			ubyte_t columns = 0;
			
			for(rows = a_offset; rows < a_offset+a_width; rows++){
				
				for(columns = 0; columns < GLCD_COLUMNS-1; columns++)
					
					g_GLCD.frameBuffer[rows][columns] = g_GLCD.frameBuffer[rows][columns+1];
				
				g_GLCD.frameBuffer[rows][GLCD_COLUMNS-1] = ' ';
			}
		}
		break;
		
		case GLCD_RIGHT: {
			
			ubyte_t columns = 0;
			
			for(rows = a_offset; rows < a_offset+a_width; rows++){
				
				for(columns = GLCD_COLUMNS-1; columns > 0; columns--)
					
					g_GLCD.frameBuffer[rows][columns] = g_GLCD.frameBuffer[rows][columns-1];
				
				g_GLCD.frameBuffer[rows][0] = ' ';
			}
		}
		break;
		
		default:
		break;
    }
 }
 
 void scrollUnits(glcddirection_t a_direction, ubyte_t a_offset, ubyte_t a_width){
	 
	ubyte_t rows = 0;
	
	switch(a_direction){
		 
		case GLCD_UP: {
			
			char tmp[GLCD_COLUMNS+1];
			tmp[GLCD_COLUMNS]='\0';
			
			strncpy(tmp+a_offset,g_GLCD.frameBuffer[0]+a_offset,a_width);
				
			for(rows = 0; rows < GLCD_ROWS-1; rows++)
		 
				strncpy(g_GLCD.frameBuffer[rows]+a_offset,g_GLCD.frameBuffer[rows+1]+a_offset,a_width);
		 
			strncpy(g_GLCD.frameBuffer[GLCD_ROWS-1]+a_offset,tmp+a_offset,a_width);
		 
		}
		break;
	
		case GLCD_DOWN: {
			
			char tmp[GLCD_COLUMNS+1];
			tmp[GLCD_COLUMNS]='\0';
				
			strncpy(tmp+a_offset,g_GLCD.frameBuffer[GLCD_ROWS-1]+a_offset,a_width);
		 
			for(rows = GLCD_ROWS-1; rows > 0; rows--)
		 
				strncpy(g_GLCD.frameBuffer[rows]+a_offset,g_GLCD.frameBuffer[rows-1]+a_offset,a_width);
		 
			strncpy(g_GLCD.frameBuffer[0]+a_offset,tmp+a_offset,a_width);
		 
		}
		break;	
	
		case GLCD_LEFT: {
			
			ubyte_t columns = 0;
			char tmp = 0;
			
			for(rows = a_offset; rows < a_offset+a_width; rows++){
				tmp = g_GLCD.frameBuffer[rows][0];
				for(columns = 0; columns < GLCD_COLUMNS-1; columns++)
					g_GLCD.frameBuffer[rows][columns] = g_GLCD.frameBuffer[rows][columns+1];
				 g_GLCD.frameBuffer[rows][GLCD_COLUMNS-1] = tmp;
			}
		}
		break;
		
		case GLCD_RIGHT: {
				
			 ubyte_t columns = 0;
			 char tmp = 0;
			 
			 for(rows = a_offset; rows < a_offset+a_width; rows++){
				 tmp = g_GLCD.frameBuffer[rows][GLCD_COLUMNS-1];
				 for(columns = GLCD_COLUMNS-1; columns > 0; columns--)
					 g_GLCD.frameBuffer[rows][columns] = g_GLCD.frameBuffer[rows][columns-1];
				 g_GLCD.frameBuffer[rows][0] = tmp;
			 }
		}
		break;
		
		default: break;
	}
 }
 
 static void resetCursorConfig(void){
	 
	 GLCD_CONFIG_CURSOR(FALSE,FALSE);
	 
 }
 
 static void saveCursorConfig(bool_t a_cursorVisible, bool_t a_cursorBlinking){
	 
	 g_GLCD.statusRegister.value &= ~0x03;
	 g_GLCD.statusRegister.cursorvisible = a_cursorVisible;
	 g_GLCD.statusRegister.cursorblinking = a_cursorBlinking;
	 
 }
 
 static void restoreCursorConfig(void){
	 
	 GLCD_CONFIG_CURSOR(g_GLCD.statusRegister.cursorvisible,g_GLCD.statusRegister.cursorblinking);
	 
 }
 
 static void updateCursorPosition(glcddirection_t a_dir){
	 
	switch (a_dir){
		  
		case GLCD_LEFT: {
	
			if(g_GLCD.cursorPos.column == 0){
				
				g_GLCD.cursorPos.column = GLCD_COLUMNS-1;
				
				if(g_GLCD.cursorPos.row == 0)
					g_GLCD.cursorPos.row = GLCD_ROWS-1;
				else
					g_GLCD.cursorPos.row--;
				 
			}else{
				
				g_GLCD.cursorPos.column--;
				
			 }
		}
		break;
		
		case GLCD_RIGHT: {
		
			if(g_GLCD.cursorPos.column == GLCD_COLUMNS-1){
				
				g_GLCD.cursorPos.column = 0;
				
				if(g_GLCD.cursorPos.row == GLCD_ROWS-1)
					g_GLCD.cursorPos.row = 0;
				else
					g_GLCD.cursorPos.row++;
					
			}else{
				
				g_GLCD.cursorPos.column++;
				
			}
		}
		break;
		
		case GLCD_UP: {
			
			if(g_GLCD.cursorPos.row == 0)
				g_GLCD.cursorPos.row = GLCD_ROWS-1;
			else
				g_GLCD.cursorPos.row--;
			
		}
		break;
		
		case GLCD_DOWN: {
			
			if(g_GLCD.cursorPos.row == GLCD_ROWS-1)
				g_GLCD.cursorPos.row = 0;
			else
				g_GLCD.cursorPos.row++;
			
		}
		break;
		
		default: break;
		
	}
 }
 
 static void saveCursorPosition(ubyte_t a_row, ubyte_t a_column){
	 
	g_GLCD.cursorPos.row = a_row;
	g_GLCD.cursorPos.column = a_column;
	 
 }
 
 static void restoreCursorPosition(glcddirection_t a_dir, ubyte_t a_offset, ubyte_t a_width){
	
	switch(a_dir){
		
		case GLCD_UP: {
		
			if(g_GLCD.cursorPos.column >= a_offset && g_GLCD.cursorPos.column < (a_offset+a_width)){
			
				if(g_GLCD.cursorPos.row == 0)
					GLCD_setCursorPosition(GLCD_ROWS-1,(g_GLCD.cursorPos.column/2));
				else
					GLCD_setCursorPosition(g_GLCD.cursorPos.row-1,(g_GLCD.cursorPos.column/2));
		
			}else{
		
				GLCD_setCursorPosition(g_GLCD.cursorPos.row,(g_GLCD.cursorPos.column/2));
		
			}
		}
		break;
	
		case GLCD_DOWN: {
		
			if(g_GLCD.cursorPos.column >= a_offset && g_GLCD.cursorPos.column < (a_offset+a_width)){
			
				if(g_GLCD.cursorPos.row == GLCD_ROWS-1)
					GLCD_setCursorPosition(0,(g_GLCD.cursorPos.column/2));
				else
					GLCD_setCursorPosition(g_GLCD.cursorPos.row+1,(g_GLCD.cursorPos.column/2));
			
			}else{
		
				GLCD_setCursorPosition(g_GLCD.cursorPos.row,(g_GLCD.cursorPos.column/2));
		
			}
		}
		break;

		case GLCD_LEFT: {
		
			if(g_GLCD.cursorPos.row >= a_offset && g_GLCD.cursorPos.row < (a_offset+a_width)){	
			
				if((g_GLCD.cursorPos.column/2) == 0)
					GLCD_setCursorPosition(g_GLCD.cursorPos.row,(GLCD_COLUMNS/2)-1);
				else
					GLCD_setCursorPosition(g_GLCD.cursorPos.row,(g_GLCD.cursorPos.column/2)-1);
				
			}else{
			
				GLCD_setCursorPosition(g_GLCD.cursorPos.row,(g_GLCD.cursorPos.column/2));
			
			}
		}
		break;

		case GLCD_RIGHT: {
		
			if(g_GLCD.cursorPos.row >= a_offset && g_GLCD.cursorPos.row < (a_offset+a_width)){
			
				if((g_GLCD.cursorPos.column/2) == (GLCD_COLUMNS/2)-1)
					GLCD_setCursorPosition(g_GLCD.cursorPos.row,0);
				else
					GLCD_setCursorPosition(g_GLCD.cursorPos.row,(g_GLCD.cursorPos.column/2)+1);
		
			}else{
			
				GLCD_setCursorPosition(g_GLCD.cursorPos.row,(g_GLCD.cursorPos.column/2));
			
			}
		}
		break;

		case GLCD_NONE: {
		
			GLCD_setCursorPosition(g_GLCD.cursorPos.row,(g_GLCD.cursorPos.column/2));
		
		}
		break;
	}
 }
 
 static void setCursorPositionUpdateFlag(void){
	 
	g_GLCD.statusRegister.cursorpositionupdate = TRUE;
	 
 }
 
 static void clearCursorPositionUpdateFlag(void){
	  
	g_GLCD.statusRegister.cursorpositionupdate = FALSE;
	  
 }