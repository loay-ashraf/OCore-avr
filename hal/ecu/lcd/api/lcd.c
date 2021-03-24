/** @file LCD.c
 *  @brief includes function definition
 *  @author Loay Ashraf
 *	@version 1.0.0
 *  @pre include LCD.h
 *  @bug no known bugs
 *	@warning the functions LCD_print_int and LCD_print_float have maximum digit number of 7 and has to be modified manually
 *  @see LCD.h
 */

//------------INCLUDE REQUIRED HEADER FILES------------//

 #include "lcd.h"
 #include "hal/ecu/lcd/common/lcd_macros.h"
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

static lcd_t g_LCD;

static void initBuffer(void);
static void putBuffer(void);	
static void clearBuffer(void);
static void setFrameBufferUpdateFlag(void);
static void clearFrameBufferUpdateFlag(void);
static void shiftBuffer(lcddirection_t a_direction, uint8_t a_offset, uint8_t a_width);
static void scrollBuffer(lcddirection_t a_direction, uint8_t a_offset, uint8_t a_width);
static void resetCursorConfig(void);
static void saveCursorConfig(bool_t a_cursorVisible, bool_t a_cursorBlinking);
static void restoreCursorConfig(void);
static void resetTextDirectionConfig(void);
static void saveTextDirectionConfig(bool_t a_leftToRight);
static void restoreTextDirectionConfig(void);
static void shiftCursorPosition(lcddirection_t a_dir);
static void saveCursorPosition(uint8_t a_row, uint8_t a_column);	
static void restoreCursorPosition(lcddirection_t a_dir, uint8_t a_offset, uint8_t a_width);
static void setCursorPositionUpdateFlag(void);
static void clearCursorPositionUpdateFlag(void);

 void LCD_init(bool_t a_backlightON, bool_t a_cursorVisible, bool_t a_cursorBlinking, bool_t a_leftToRight){
	 
	initBuffer();		// initialize frame buffer 
	
	saveCursorConfig(a_cursorVisible,a_cursorBlinking);
	saveTextDirectionConfig(a_leftToRight);
	
	setCursorPositionUpdateFlag();
	setFrameBufferUpdateFlag();
		
	LCD_INIT(a_backlightON,a_cursorVisible,a_cursorBlinking,a_leftToRight);
	
 }

 void LCD_sendInstruction(ubyte_t a_instruction){

	LCD_SEND_INSTRUCTION(a_instruction);
   
 }
 
 void LCD_clearUnit(uint8_t a_row, uint8_t a_column){
	
	resetCursorConfig();						// turn off cursor
	
	LCD_setCursorPosition(a_row, a_column);		// unit position
	
	LCD_putc(' ');								// clear
	
	LCD_setCursorPosition(a_row, a_column);		// unit position
	
	restoreCursorConfig();						// restore cursor configuration
	
 }
 
 void LCD_clearRow(uint8_t a_row, uint8_t a_offset){
	 
	ubyte_t i;
	
	resetCursorConfig();						// turn off cursor
	resetTextDirectionConfig();
	
	LCD_setCursorPosition(a_row,a_offset);		// row starting position
	
	for(i = a_offset; i < LCD_COLUMNS; i++)
		
		LCD_putc(' ');							// clear
	 	 
	LCD_setCursorPosition(a_row,a_offset);		// row starting position
	
	restoreCursorConfig();						// restore cursor configuration
	restoreTextDirectionConfig();
	
 }
 
 void LCD_clearColumn(uint8_t a_column, uint8_t a_offset){

	ubyte_t i;
	
	resetCursorConfig();						// turn off cursor
	
	for(i = a_offset; i < LCD_ROWS; i++){
		
		LCD_setCursorPosition(i,a_column);
		LCD_putc(' ');							// clear
		
	}
	
	LCD_setCursorPosition(a_offset,a_column);	// column starting position
	
	restoreCursorConfig();						// restore cursor configuration
	 
 }
 
 void LCD_clearDisplay(void){
	 
	LCD_CLEAR_DISLAY;
	 
	clearBuffer();								// clear frame buffer
	 
	saveCursorPosition(0,0);					// default cursor position

 }
 
 void LCD_configBacklight(bool_t a_backlightON){
	
	LCD_CONFIG_BACKLIGHT(a_backlightON); 
	 
 }
 
 void LCD_configCursor(bool_t a_cursorVisible, bool_t a_cursorBlinking){
		
		/* store cursor configuration */
		
		saveCursorConfig(a_cursorVisible,a_cursorBlinking);

		LCD_CONFIG_CURSOR(a_cursorVisible,a_cursorBlinking);		// configure cursor
	
 }
 
 void LCD_configTextDirection(bool_t a_leftToRight){
	 
	 saveTextDirectionConfig(a_leftToRight);
	 
	 LCD_CONFIG_TEXT_DIRECTION(a_leftToRight);
	 
 }

 void LCD_setCursorPosition(uint8_t a_row, uint8_t a_col){
	
	// update cursor position
	
	saveCursorPosition(a_row,a_col);		
	
	LCD_SET_CURSOR_POSITION(a_row,a_col);
	
  }
 
 lcdposition_t LCD_getCursorPosition(void){
	 
	 return g_LCD.primaryCursorPosition;
	 
 }
 
 void LCD_shiftCursor(lcddirection_t a_dir){
	
	shiftCursorPosition(a_dir);
	
	if(!(a_dir == LCD_UP || a_dir == LCD_DOWN || (a_dir == LCD_RIGHT && g_LCD.primaryCursorPosition.column == 0) || (a_dir == LCD_LEFT && g_LCD.primaryCursorPosition.column == LCD_COLUMNS-1)))
	
		LCD_SHIFT_CURSOR(a_dir);	 
			
 }

 void LCD_shiftDisplay(lcddirection_t a_dir, uint8_t a_offset, uint8_t a_width){
		 
	switch(a_dir){
		 
		case LCD_LEFT:
		case LCD_RIGHT: {
			  
			if((a_offset+a_width)>LCD_ROWS)
				a_width = LCD_ROWS-a_offset;
			  
			shiftBuffer(a_dir,a_offset,a_width);
			putBuffer();
			  
			switch(a_dir) {
				  
				case LCD_LEFT: LCD_setCursorPosition(a_offset,LCD_COLUMNS-1);
				break;
				case LCD_RIGHT: LCD_setCursorPosition(a_offset,0);
				break;
				default: break;
				  
			  }	    
		}				 
		break;
		 		 
		case LCD_UP:
		case LCD_DOWN: {
			 
			if((a_offset+a_width)>LCD_COLUMNS)
				a_width = LCD_COLUMNS-a_offset;
			
			shiftBuffer(a_dir, a_offset, a_width);
			putBuffer();
				
			switch(a_dir) {
					
				case LCD_UP: LCD_setCursorPosition(LCD_ROWS-1,a_offset);
				break;	
				case LCD_DOWN: LCD_setCursorPosition(0,a_offset);	
				break;
				default: break;
				
			 }
		} 
		break;	
		
		default: break;						 
	}
 }

 void LCD_scrollDisplay(lcddirection_t a_dir, uint8_t a_offset, uint8_t a_width){
	 
	switch(a_dir){
		 
		case LCD_LEFT:	
		case LCD_RIGHT: {
			 
			if((a_offset+a_width)>LCD_ROWS)
				a_width = LCD_ROWS-a_offset;
			 
		}
		break;
		 
		case LCD_UP:
		case LCD_DOWN: {
			 
			if((a_offset+a_width)>LCD_COLUMNS)
				a_width = LCD_COLUMNS-a_offset;
				
		} 
		break;
		 
		default: break;
	}  
	
	scrollBuffer(a_dir, a_offset, a_width);  
	putBuffer();
	restoreCursorPosition(a_dir, a_offset, a_width);
	
 }
 
 void LCD_scrollDisplayNative(lcddirection_t a_dir){
	 
	LCD_SCROLL_DISPLAY(a_dir);
	 
 }
 
 void LCD_setBuffer(char a_buffer[LCD_ROWS][LCD_COLUMNS+1]){
	 
	 ubyte_t rows;
	 
	 for(rows = 0; rows < LCD_ROWS; rows++)
		strcpy(g_LCD.frameBuffer[rows],a_buffer[rows]);
	 
	 putBuffer();
	 
 }
 
 void LCD_copyBuffer(char a_returnedBuffer[LCD_ROWS][LCD_COLUMNS+1]){
	 
	ubyte_t rows;
	
	for(rows = 0; rows < LCD_ROWS; rows++)
		strcpy(a_returnedBuffer[rows],g_LCD.frameBuffer[rows]);
		
 }	
 
 void LCD_defineCustomCharacter(lcdcustomcharacter_t a_characterIndex, ubyte_t a_characterArray[8]){
	
	clearCursorPositionUpdateFlag();
	clearFrameBufferUpdateFlag();
	
	LCD_DEFINE_CUSTOM_CHARACTER(a_characterIndex,a_characterArray);
	
	setCursorPositionUpdateFlag();
	setFrameBufferUpdateFlag();
	
	LCD_setCursorPosition(0,0);
	
 }
	
 void LCD_putc(char a_data){
	
	/*------------UPDATE DISPLAY BUFFER------------*/
	
	if(g_LCD.statusRegister.framebufferupdate){
		
		if(a_data == LCD_CCHAR0)
			
			g_LCD.frameBuffer[g_LCD.primaryCursorPosition.row][g_LCD.primaryCursorPosition.column] = '\n';
			
		else
		
			g_LCD.frameBuffer[g_LCD.primaryCursorPosition.row][g_LCD.primaryCursorPosition.column] = a_data;
	
	}
	
	/*------------UPDATE CURSOR POSITION AND PUT DATA ON SCREEN------------*/
	
	if(a_data == '\n')
		
		a_data = LCD_CCHAR0;
		
	LCD_PUTC(a_data);
	
	shiftCursorPosition((lcddirection_t)g_LCD.statusRegister.leftToRight);
			
}

 void LCD_puts(const char * a_data){

	while(*a_data != '\0')
		LCD_putc(*a_data++);

 }
 
 void LCD_puti(udword_t a_data, lcdnumberbase_t a_numberBase){
	 
	char buffer[LCD_BUFFER_SIZE];
	itoa(a_data,buffer,a_numberBase);
	
	if(a_numberBase==LCD_BASE16)	 
		LCD_puts("0x");
	else if(a_numberBase==LCD_BASE2)	
		LCD_puts("0b"); 
		
	LCD_puts(buffer);
	 
 }
 
 void LCD_putf(float a_data){
	 
	 char buffer[LCD_BUFFER_SIZE];
	 dtostrf(a_data, 0, LCD_FRACTION_SIZE, buffer);
	 LCD_puts(buffer);
	 
 }

 void LCD_putp(void * a_ptr){
	 
	 char buffer[LCD_BUFFER_SIZE];
	 sprintf(buffer,"%p",a_ptr);
	 LCD_puts(buffer);
	 
 }
 
 char LCD_getc(void){

	return LCD_GETC;
 }
 
 char * LCD_gets(uint8_t a_row, uint8_t a_width){
	 
	 static char rowData[LCD_COLUMNS+1];
	 uint8_t index;
	 
	 LCD_setCursorPosition(a_row,0); 
	 
	 for(index=0;index<a_width;index++)
		rowData[index] = LCD_getc();
		
	rowData[a_width] = '\0';
	
	return rowData; 
 }
 
 static void initBuffer(void){
	
	ubyte_t rows;
	
	clearBuffer();
	
	for(rows = 0; rows < LCD_ROWS; rows++)
		g_LCD.frameBuffer[rows][LCD_COLUMNS]='\0';
	 
 }
 
 static void putBuffer(void){
	
	clearFrameBufferUpdateFlag();
	clearCursorPositionUpdateFlag(); 
	resetCursorConfig();
	resetTextDirectionConfig();
		
	ubyte_t rows;
	
	LCD_setCursorPosition(0,0);
		
	for(rows = 0; rows < LCD_ROWS; rows++)

		LCD_puts(g_LCD.frameBuffer[rows]);
	
	setFrameBufferUpdateFlag();
	setCursorPositionUpdateFlag();
	restoreCursorConfig();
	restoreTextDirectionConfig();
	
 }
 
 static void clearBuffer(void){
	 
	ubyte_t rows,cols;
	 
	for(rows = 0; rows < LCD_ROWS; rows++){
		
		for(cols = 0; cols < LCD_COLUMNS; cols++)
			 g_LCD.frameBuffer[rows][cols]= ' ';
		 
	}
	  
 }
 
 static void setFrameBufferUpdateFlag(void){
	 
	 g_LCD.statusRegister.framebufferupdate = TRUE;
	 
 }
 
 static void clearFrameBufferUpdateFlag(void){
	 
	 g_LCD.statusRegister.framebufferupdate = FALSE;
	 
 }
 
 static void shiftBuffer(lcddirection_t a_direction, uint8_t a_offset, uint8_t a_width){
	  
	ubyte_t rows;
	  
	switch(a_direction){
		
		case LCD_UP: {
			
			for(rows = 0; rows < LCD_ROWS-1; rows++)
			
				strncpy(g_LCD.frameBuffer[rows]+a_offset,g_LCD.frameBuffer[rows+1]+a_offset,a_width);
			
			for(rows = a_offset; rows < a_offset+a_width; rows++)
			
				g_LCD.frameBuffer[LCD_ROWS-1][rows]=' ';
			
		}
		break;

		case LCD_DOWN: {
			
			for(rows = LCD_ROWS-1; rows > 0; rows--)
			
				strncpy(g_LCD.frameBuffer[rows]+a_offset,g_LCD.frameBuffer[rows-1]+a_offset,a_width);
			
			for(rows = a_offset; rows < a_offset+a_width; rows++)
			
				g_LCD.frameBuffer[0][rows]=' ';
			
		}
		break;
		
		case LCD_LEFT: {
			
			ubyte_t columns = 0;
			
			for(rows = a_offset; rows < a_offset+a_width; rows++){
				
				for(columns = 0; columns < LCD_COLUMNS-1; columns++)
					
					g_LCD.frameBuffer[rows][columns] = g_LCD.frameBuffer[rows][columns+1];
				
				g_LCD.frameBuffer[rows][LCD_COLUMNS-1] = ' ';
			}
		}
		break;
		
		case LCD_RIGHT: {
			
			ubyte_t columns = 0;
			
			for(rows = a_offset; rows < a_offset+a_width; rows++){
				
				for(columns = LCD_COLUMNS-1; columns > 0; columns--)
					
					g_LCD.frameBuffer[rows][columns] = g_LCD.frameBuffer[rows][columns-1];
				
				g_LCD.frameBuffer[rows][0] = ' ';
			}
		}
		break;
		
		default:
		break;
    }
 }
 
 static void scrollBuffer(lcddirection_t a_direction, uint8_t a_offset, uint8_t a_width){
	 
	ubyte_t rows = 0;
	
	switch(a_direction){
		 
		case LCD_UP: {
			
			char tmp[LCD_COLUMNS+1];
			tmp[LCD_COLUMNS]='\0';
			
			strncpy(tmp+a_offset,g_LCD.frameBuffer[0]+a_offset,a_width);
				
			for(rows = 0; rows < LCD_ROWS-1; rows++)
		 
				strncpy(g_LCD.frameBuffer[rows]+a_offset,g_LCD.frameBuffer[rows+1]+a_offset,a_width);
		 
			strncpy(g_LCD.frameBuffer[LCD_ROWS-1]+a_offset,tmp+a_offset,a_width);
		 
		}
		break;
	
		case LCD_DOWN: {
			
			char tmp[LCD_COLUMNS+1];
			tmp[LCD_COLUMNS]='\0';
				
			strncpy(tmp+a_offset,g_LCD.frameBuffer[LCD_ROWS-1]+a_offset,a_width);
		 
			for(rows = LCD_ROWS-1; rows > 0; rows--)
		 
				strncpy(g_LCD.frameBuffer[rows]+a_offset,g_LCD.frameBuffer[rows-1]+a_offset,a_width);
		 
			strncpy(g_LCD.frameBuffer[0]+a_offset,tmp+a_offset,a_width);
		 
		}
		break;	
	
		case LCD_LEFT: {
			
			ubyte_t columns = 0;
			char tmp = 0;
			
			for(rows = a_offset; rows < a_offset+a_width; rows++){
				tmp = g_LCD.frameBuffer[rows][0];
				for(columns = 0; columns < LCD_COLUMNS-1; columns++)
					g_LCD.frameBuffer[rows][columns] = g_LCD.frameBuffer[rows][columns+1];
				 g_LCD.frameBuffer[rows][LCD_COLUMNS-1] = tmp;
			}
		}
		break;
		
		case LCD_RIGHT: {
				
			 ubyte_t columns = 0;
			 char tmp = 0;
			 
			 for(rows = a_offset; rows < a_offset+a_width; rows++){
				 tmp = g_LCD.frameBuffer[rows][LCD_COLUMNS-1];
				 for(columns = LCD_COLUMNS-1; columns > 0; columns--)
					 g_LCD.frameBuffer[rows][columns] = g_LCD.frameBuffer[rows][columns-1];
				 g_LCD.frameBuffer[rows][0] = tmp;
			 }
		}
		break;
		
		default: break;
	}
 }
 
 static void resetCursorConfig(void){
	 
	LCD_CONFIG_CURSOR(FALSE,FALSE);
	 
 }
 
 static void saveCursorConfig(bool_t a_cursorVisible, bool_t a_cursorBlinking){
	 
	g_LCD.statusRegister.value &= ~0x03;
	g_LCD.statusRegister.cursorvisible = a_cursorVisible;
	g_LCD.statusRegister.cursorblinking = a_cursorBlinking;
	 
 }
 
 static void restoreCursorConfig(void){
	 
	LCD_CONFIG_CURSOR(g_LCD.statusRegister.cursorvisible,g_LCD.statusRegister.cursorblinking);
	 
 }
 
 static void resetTextDirectionConfig(void){
	  
	LCD_CONFIG_TEXT_DIRECTION(TRUE);
	  
 }
  
 static void saveTextDirectionConfig(bool_t a_leftToRight){
	  
	g_LCD.statusRegister.leftToRight = a_leftToRight;
	  
 }
  
 static void restoreTextDirectionConfig(void){
	  
	LCD_CONFIG_TEXT_DIRECTION(g_LCD.statusRegister.leftToRight);
	  
 }
 
 static void shiftCursorPosition(lcddirection_t a_dir){
	
	lcdposition_t cursorPosition;
	
	if(g_LCD.statusRegister.cursorpositionupdate)
		
		cursorPosition = g_LCD.primaryCursorPosition;
		
	else
	
		cursorPosition = g_LCD.secondaryCursorPosition;	
	 
	switch (a_dir){
		 
		case LCD_LEFT: {
			 
			if(cursorPosition.column == 0){
				 
				cursorPosition.column = LCD_COLUMNS-1;
				 
				if(cursorPosition.row == 0)
					cursorPosition.row = LCD_ROWS-1;
				else
					cursorPosition.row--;
				 
			}else{
				 
					cursorPosition.column--;
				 
			}
			 
		}
		break;
		 
		case LCD_RIGHT: {
			 
			if(cursorPosition.column == LCD_COLUMNS-1){
				 
				cursorPosition.column = 0;
				 
				if(cursorPosition.row == LCD_ROWS-1)
					cursorPosition.row = 0;
				else
					cursorPosition.row++;
				 
			}else{
				 
					cursorPosition.column++;
				 
			}
			 
		}
		break;
		 
		case LCD_UP: {
			 
			if(cursorPosition.row == 0)
				cursorPosition.row = LCD_ROWS-1;
			else
				cursorPosition.row--;
			 
		}
		break;
		 
		case LCD_DOWN: {
			 
			if(cursorPosition.row == LCD_ROWS-1)
				cursorPosition.row = 0;
			else
				cursorPosition.row++;
			 
		}
		break;
		 
		default: break;
		 
	}
	
	if(a_dir == LCD_UP || a_dir == LCD_DOWN || (a_dir == LCD_RIGHT && cursorPosition.column == 0) || (a_dir == LCD_LEFT && cursorPosition.column == LCD_COLUMNS-1))
		
		LCD_setCursorPosition(cursorPosition.row,cursorPosition.column);
		
	else
	
		saveCursorPosition(cursorPosition.row,cursorPosition.column);	
	
 }
 
 static void saveCursorPosition(uint8_t a_row, uint8_t a_column){
	
	if(g_LCD.statusRegister.cursorpositionupdate){
	 
		g_LCD.primaryCursorPosition.row = a_row;
		g_LCD.primaryCursorPosition.column = a_column;
		
	}else{
	
		g_LCD.secondaryCursorPosition.row = a_row;
		g_LCD.secondaryCursorPosition.column = a_column;
		
	}
	 
 }
 
 void restoreCursorPosition(lcddirection_t a_dir, uint8_t a_offset, uint8_t a_width){
	
	switch(a_dir){
		
		case LCD_UP: {
		
			if(g_LCD.primaryCursorPosition.column >= a_offset && g_LCD.primaryCursorPosition.column < (a_offset+a_width)){
			
				if(g_LCD.primaryCursorPosition.row == 0)
					LCD_setCursorPosition(LCD_ROWS-1,g_LCD.primaryCursorPosition.column);
				else
					LCD_setCursorPosition(g_LCD.primaryCursorPosition.row-1,g_LCD.primaryCursorPosition.column);
		
			}else{
		
				LCD_setCursorPosition(g_LCD.primaryCursorPosition.row,g_LCD.primaryCursorPosition.column);
		
			}
		}
		break;
	
		case LCD_DOWN: {
		
			if(g_LCD.primaryCursorPosition.column >= a_offset && g_LCD.primaryCursorPosition.column < (a_offset+a_width)){
			
				if(g_LCD.primaryCursorPosition.row == LCD_ROWS-1)
				LCD_setCursorPosition(0,g_LCD.primaryCursorPosition.column);
				else
				LCD_setCursorPosition(g_LCD.primaryCursorPosition.row+1,g_LCD.primaryCursorPosition.column);
			
			}else{
		
				LCD_setCursorPosition(g_LCD.primaryCursorPosition.row,g_LCD.primaryCursorPosition.column);
		
			}
		}
		break;

		case LCD_LEFT: {
		
			if(g_LCD.primaryCursorPosition.row >= a_offset && g_LCD.primaryCursorPosition.row < (a_offset+a_width)){	
			
				if(g_LCD.primaryCursorPosition.column == 0)
					LCD_setCursorPosition(g_LCD.primaryCursorPosition.row,LCD_COLUMNS-1);
				else
					LCD_setCursorPosition(g_LCD.primaryCursorPosition.row,g_LCD.primaryCursorPosition.column-1);
				
			}else{
			
				LCD_setCursorPosition(g_LCD.primaryCursorPosition.row,g_LCD.primaryCursorPosition.column);
			
			}
		}
		break;

		case LCD_RIGHT: {
		
			if(g_LCD.primaryCursorPosition.row >= a_offset && g_LCD.primaryCursorPosition.row < (a_offset+a_width)){
			
				if(g_LCD.primaryCursorPosition.column == LCD_COLUMNS-1)
					LCD_setCursorPosition(g_LCD.primaryCursorPosition.row,0);
				else
					LCD_setCursorPosition(g_LCD.primaryCursorPosition.row,g_LCD.primaryCursorPosition.column+1);
		
			}else{
			
				LCD_setCursorPosition(g_LCD.primaryCursorPosition.row,g_LCD.primaryCursorPosition.column);
			
			}
		}
		break;
	}
 }
 
 static void setCursorPositionUpdateFlag(void){
	 
	g_LCD.statusRegister.cursorpositionupdate = TRUE;
	 
 }
 
 static void clearCursorPositionUpdateFlag(void){
	  
	g_LCD.statusRegister.cursorpositionupdate = FALSE;
	  
 }