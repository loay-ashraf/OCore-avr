/**********************************************************************
*
* File:         lcd.c
*
* Author(s):    Loay Ashraf <loay.ashraf.96@gmail.com>
*
* Date created: 15/01/2018
*
* Description:	contains function definitions for LCD module.
*
**********************************************************************/

/*------------------------------INCLUDES-----------------------------*/

#include "lcd.h"
#include "hal/ecu/lcd/common/lcd_macros.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
 
/*--------------------------GLOBAL VARIABLES-------------------------*/

/**********************************************************************
*
* Variable:    g_LCD
*
* Description: LCD object stores frame buffer, cursor position and
*              status register.
*
* Notes:
*
* Scope:       lcd.c.
*
**********************************************************************/

static lcd_t g_LCD;

/*------------------------FUNCTION PROTOTYPES------------------------*/

static void _initBuffer(void);
static void _putBuffer(void);	
static void _clearBuffer(void);
static void _setFrameBufferUpdateFlag(void);
static void _clearFrameBufferUpdateFlag(void);
static void _shiftBuffer(lcddirection_t a_direction, uint8_t a_offset, uint8_t a_width);
static void _scrollBuffer(lcddirection_t a_direction, uint8_t a_offset, uint8_t a_width);
static void _resetCursorConfig(void);
static void _saveCursorConfig(bool_t a_cursorVisible, bool_t a_cursorBlinking);
static void _restoreCursorConfig(void);
static void _resetTextDirectionConfig(void);
static void _saveTextDirectionConfig(bool_t a_leftToRight);
static void _restoreTextDirectionConfig(void);
static void _shiftCursorPosition(lcddirection_t a_dir);
static void _saveCursorPosition(uint8_t a_row, uint8_t a_column);	
static void _restoreCursorPosition(lcddirection_t a_dir, uint8_t a_offset, uint8_t a_width);
static void _setCursorPositionUpdateFlag(void);
static void _clearCursorPositionUpdateFlag(void);

/*-----------------------FUNCTION DEFINITIONS------------------------*/

/**********************************************************************
*
* Function:    LCD_init
*
* Description: Initializes LCD module.
*
* Notes:
*
* Returns:     None.
*
**********************************************************************/

void LCD_init(bool_t a_backlightON, bool_t a_cursorVisible, bool_t a_cursorBlinking, bool_t a_leftToRight){
	 
	_initBuffer();		/* initialize frame buffer */
	
	_saveCursorConfig(a_cursorVisible,a_cursorBlinking);
	_saveTextDirectionConfig(a_leftToRight);
	
	_setCursorPositionUpdateFlag();
	_setFrameBufferUpdateFlag();
		
	LCD_INIT(a_backlightON,a_cursorVisible,a_cursorBlinking,a_leftToRight);
	
}

/**********************************************************************
*
* Function:    LCD_sendInstruction
*
* Description: Sends instructions to LCD module.
*
* Notes:
*
* Returns:     None.
*
**********************************************************************/

void LCD_sendInstruction(ubyte_t a_instruction){

	LCD_SEND_INSTRUCTION(a_instruction);
   
}

/**********************************************************************
*
* Function:    LCD_clearUnit
*
* Description: Clears unit(character space) of LCD module.
*
* Notes:
*
* Returns:     None.
*
**********************************************************************/
 
void LCD_clearUnit(uint8_t a_row, uint8_t a_column){
	
	_resetCursorConfig();						/* turn off cursor */
	
	LCD_setCursorPosition(a_row, a_column);		/* unit position */
	
	LCD_putc(' ');								/* clear */
	
	LCD_setCursorPosition(a_row, a_column);		/* unit position */
	
	_restoreCursorConfig();						/* restore cursor configuration */
	
}

/**********************************************************************
*
* Function:    LCD_clearRow
*
* Description: Clears row of LCD module.
*
* Notes:
*
* Returns:     None.
*
**********************************************************************/
 
void LCD_clearRow(uint8_t a_row, uint8_t a_offset){
	 
	ubyte_t i;
	
	_resetCursorConfig();						/* turn off cursor */
	_resetTextDirectionConfig();
	
	LCD_setCursorPosition(a_row,a_offset);		/* row starting position */
	
	for(i = a_offset; i < LCD_COLUMNS; i++)
		
		LCD_putc(' ');							/* clear */
	 	 
	LCD_setCursorPosition(a_row,a_offset);		/* row starting position */
	
	_restoreCursorConfig();						/* restore cursor configuration */
	_restoreTextDirectionConfig();
	
}

/**********************************************************************
*
* Function:    LCD_clearColumn
*
* Description: Clears column of LCD module.
*
* Notes:
*
* Returns:     None.
*
**********************************************************************/
 
void LCD_clearColumn(uint8_t a_column, uint8_t a_offset){

	ubyte_t i;
	
	_resetCursorConfig();						/* turn off cursor */
	
	for(i = a_offset; i < LCD_ROWS; i++){
		
		LCD_setCursorPosition(i,a_column);
		LCD_putc(' ');							/* clear */
		
	}
	
	LCD_setCursorPosition(a_offset,a_column);	/* column starting position */
	
	_restoreCursorConfig();						/* restore cursor configuration */
	 
}

/**********************************************************************
*
* Function:    LCD_clearDisplay
*
* Description: Clears display of LCD module.
*
* Notes:
*
* Returns:     None.
*
**********************************************************************/
 
void LCD_clearDisplay(void){
	 
	LCD_CLEAR_DISLAY;
	 
	_clearBuffer();								/* clear frame buffer */
	 
	_saveCursorPosition(0,0);					/* default cursor position */

}

/**********************************************************************
*
* Function:    LCD_configBacklight
*
* Description: Configures backlight of LCD module.
*
* Notes:
*
* Returns:     None.
*
**********************************************************************/
 
void LCD_configBacklight(bool_t a_backlightON){
	
	LCD_CONFIG_BACKLIGHT(a_backlightON); 
	 
}

/**********************************************************************
*
* Function:    LCD_configCursor
*
* Description: Configures cursor (visible/blinking) of LCD module.
*
* Notes:
*
* Returns:     None.
*
**********************************************************************/
 
void LCD_configCursor(bool_t a_cursorVisible, bool_t a_cursorBlinking){
		
	_saveCursorConfig(a_cursorVisible,a_cursorBlinking);			/* store cursor configuration */

	LCD_CONFIG_CURSOR(a_cursorVisible,a_cursorBlinking);		/* configure cursor */
	
}

/**********************************************************************
*
* Function:    LCD_configTextDirection
*
* Description: Configures text direction (L->R/R->L) of LCD module.
*
* Notes:
*
* Returns:     None.
*
**********************************************************************/
 
void LCD_configTextDirection(bool_t a_leftToRight){
	 
	_saveTextDirectionConfig(a_leftToRight);
	 
	LCD_CONFIG_TEXT_DIRECTION(a_leftToRight);
	 
}

/**********************************************************************
*
* Function:    LCD_setCursorPosition
*
* Description: Sets cursor position (row & column) of LCD module.
*
* Notes:
*
* Returns:     None.
*
**********************************************************************/

void LCD_setCursorPosition(uint8_t a_row, uint8_t a_col){
	
	_saveCursorPosition(a_row,a_col);		/* store cursor position */		
	
	LCD_SET_CURSOR_POSITION(a_row,a_col);	/* update cursor position */
	
}

/**********************************************************************
*
* Function:    LCD_getCursorPosition
*
* Description: Gets cursor position (row & column) from LCD module.
*
* Notes:
*
* Returns:     Current cursor position (type: lcdposition_t).
*
**********************************************************************/
 
lcdposition_t LCD_getCursorPosition(void){
	 
	return g_LCD.primaryCursorPosition;
	 
}

/**********************************************************************
*
* Function:    LCD_shiftCursor
*
* Description: Shifts cursor position in specific direction of LCD
*              module.
*
* Notes:
*
* Returns:     None.
*
**********************************************************************/
 
void LCD_shiftCursor(lcddirection_t a_dir){
	
	_shiftCursorPosition(a_dir);
	
	if(!(a_dir == LCD_UP || a_dir == LCD_DOWN || (a_dir == LCD_RIGHT && g_LCD.primaryCursorPosition.column == 0) || (a_dir == LCD_LEFT && g_LCD.primaryCursorPosition.column == LCD_COLUMNS-1)))
	
		LCD_SHIFT_CURSOR(a_dir);	 
			
}

/**********************************************************************
*
* Function:    LCD_shiftDisplay
*
* Description: Shifts display in specific direction of LCD module.
*
* Notes:
*
* Returns:     None.
*
**********************************************************************/

void LCD_shiftDisplay(lcddirection_t a_dir, uint8_t a_offset, uint8_t a_width){
		 
	switch(a_dir){
		 
		case LCD_LEFT:
		case LCD_RIGHT: {

			if((a_offset+a_width)>LCD_ROWS)
				a_width = LCD_ROWS-a_offset;
			  
			_shiftBuffer(a_dir,a_offset,a_width);
			_putBuffer();
			  
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
			
			_shiftBuffer(a_dir, a_offset, a_width);
			_putBuffer();
				
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

/**********************************************************************
*
* Function:    LCD_scrollDisplay
*
* Description: Scrolls display in specific direction of LCD module.
*
* Notes:
*
* Returns:     None.
*
**********************************************************************/

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
	
	_scrollBuffer(a_dir, a_offset, a_width);  
	_putBuffer();
	_restoreCursorPosition(a_dir, a_offset, a_width);
	
}

/**********************************************************************
*
* Function:    LCD_scrollDisplayNative
*
* Description: Scrolls display in specific direction of LCD module.
*
* Notes:       This function uses native function provided by driver.
*
* Returns:     None.
*
**********************************************************************/
 
void LCD_scrollDisplayNative(lcddirection_t a_dir){
	 
	LCD_SCROLL_DISPLAY(a_dir);
	 
}

/**********************************************************************
*
* Function:    LCD_setBuffer
*
* Description: Sets frame buffer of LCD module.
*
* Notes:
*
* Returns:     None.
*
**********************************************************************/
 
void LCD_setBuffer(char a_buffer[LCD_ROWS][LCD_COLUMNS+1]){
	 
	ubyte_t rows;
	 
	for(rows = 0; rows < LCD_ROWS; rows++)
		strcpy(g_LCD.frameBuffer[rows],a_buffer[rows]);
	 
	_putBuffer();
	 
}

/**********************************************************************
*
* Function:    LCD_copyBuffer
*
* Description: Copies frame buffer to given array of LCD module.
*
* Notes:
*
* Returns:     None.
*
**********************************************************************/
 
void LCD_copyBuffer(char a_returnedBuffer[LCD_ROWS][LCD_COLUMNS+1]){
	 
	ubyte_t rows;
	
	for(rows = 0; rows < LCD_ROWS; rows++)
		strcpy(a_returnedBuffer[rows],g_LCD.frameBuffer[rows]);
		
}

/**********************************************************************
*
* Function:    LCD_defineCustomCharacter
*
* Description: Defines custom character for LCD module.
*
* Notes:
*
* Returns:     None.
*
**********************************************************************/
 
void LCD_defineCustomCharacter(lcdcustomcharacter_t a_characterIndex, ubyte_t a_characterArray[8]){
	
	_clearCursorPositionUpdateFlag();
	_clearFrameBufferUpdateFlag();
	
	LCD_DEFINE_CUSTOM_CHARACTER(a_characterIndex,a_characterArray);
	
	_setCursorPositionUpdateFlag();
	_setFrameBufferUpdateFlag();
	
	LCD_setCursorPosition(0,0);
	
}

/**********************************************************************
*
* Function:    LCD_putc
*
* Description: Prints character to display of LCD module.
*
* Notes:
*
* Returns:     None.
*
**********************************************************************/
	
void LCD_putc(char a_data){
	
	/*------------UPDATE DISPLAY BUFFER------------*/
	
	if(g_LCD.statusRegister.framebufferupdate){
		
		if(a_data == LCD_CCHAR0)
			
			g_LCD.frameBuffer[g_LCD.primaryCursorPosition.row][g_LCD.primaryCursorPosition.column] = '\n';
			
		else
		
			g_LCD.frameBuffer[g_LCD.primaryCursorPosition.row][g_LCD.primaryCursorPosition.column] = a_data;
	
	}
	
	/*--------UPDATE CURSOR POSITION AND PUT DATA ON SCREEN--------*/

	if(a_data == '\n')
		
		a_data = LCD_CCHAR0;
		
	LCD_PUTC(a_data);
	
	_shiftCursorPosition((lcddirection_t)g_LCD.statusRegister.leftToRight);
			
}

/**********************************************************************
*
* Function:    LCD_puts
*
* Description: Prints string to display of LCD module.
*
* Notes:
*
* Returns:     None.
*
**********************************************************************/

void LCD_puts(const char * a_data){

	while(*a_data != '\0')
		LCD_putc(*a_data++);

}

/**********************************************************************
*
* Function:    LCD_puti
*
* Description: Prints integer number to display of LCD module.
*
* Notes:
*
* Returns:     None.
*
**********************************************************************/
 
void LCD_puti(udword_t a_data, lcdnumberbase_t a_numberBase){
	 
	char buffer[LCD_BUFFER_SIZE];
	itoa(a_data,buffer,a_numberBase);
	
	if(a_numberBase==LCD_BASE16)	 
		LCD_puts("0x");
	else if(a_numberBase==LCD_BASE2)	
		LCD_puts("0b"); 
		
	LCD_puts(buffer);
	 
}

/**********************************************************************
*
* Function:    LCD_putf
*
* Description: Prints float number to display of LCD module.
*
* Notes:
*
* Returns:     None.
*
**********************************************************************/
 
void LCD_putf(float a_data){
	 
	char buffer[LCD_BUFFER_SIZE];
	dtostrf(a_data, 0, LCD_FRACTION_SIZE, buffer);
	LCD_puts(buffer);
	 
}

/**********************************************************************
*
* Function:    LCD_putp
*
* Description: Prints pointer value to display of LCD module.
*
* Notes:
*
* Returns:     None.
*
**********************************************************************/

void LCD_putp(void * a_ptr){
	 
	char buffer[LCD_BUFFER_SIZE];
	sprintf(buffer,"%p",a_ptr);
	LCD_puts(buffer);
	 
}

/**********************************************************************
*
* Function:    LCD_getc
*
* Description: Reads character from display of LCD module.
*
* Notes:
*
* Returns:     Character at current cursor position (type: char).
*
**********************************************************************/

char LCD_getc(void){

	return LCD_GETC;

}

/**********************************************************************
*
* Function:    LCD_gets
*
* Description: Reads string from display of LCD module.
* 
* Notes:
*
* Returns:     Array of characters at current cursor position
*              (type: char *).
*
**********************************************************************/
 
char * LCD_gets(uint8_t a_row, uint8_t a_width){
	 
	static char rowData[LCD_COLUMNS+1];
	uint8_t index;
	 
	LCD_setCursorPosition(a_row,0); 
	 
	for(index=0;index<a_width;index++)
		rowData[index] = LCD_getc();
		
	rowData[a_width] = '\0';
	
	return rowData; 

}

/**********************************************************************
*
* Function:    _initBuffer
*
* Description: Initializes frame buffer of LCD module.
*
* Notes:
*
* Returns:     None.
*
**********************************************************************/
 
static void _initBuffer(void){
	
	ubyte_t rows;
	
	_clearBuffer();
	
	for(rows = 0; rows < LCD_ROWS; rows++)
		g_LCD.frameBuffer[rows][LCD_COLUMNS]='\0';

}

/**********************************************************************
*
* Function:    _putBuffer
*
* Description: Prints frame buffer to display of LCD module.
*
* Notes:
*
* Returns:     None.
*
**********************************************************************/
 
static void _putBuffer(void){
	
	_clearFrameBufferUpdateFlag();
	_clearCursorPositionUpdateFlag(); 
	_resetCursorConfig();
	_resetTextDirectionConfig();
		
	ubyte_t rows;
	
	LCD_setCursorPosition(0,0);
		
	for(rows = 0; rows < LCD_ROWS; rows++)

		LCD_puts(g_LCD.frameBuffer[rows]);
	
	_setFrameBufferUpdateFlag();
	_setCursorPositionUpdateFlag();
	_restoreCursorConfig();
	_restoreTextDirectionConfig();
	
}

/**********************************************************************
*
* Function:    _clearBuffer
*
* Description: Clears frame buffer of LCD module.
*
* Notes:
*
* Returns:     None.
*
**********************************************************************/
 
static void _clearBuffer(void){
	 
	ubyte_t rows,cols;
	 
	for(rows = 0; rows < LCD_ROWS; rows++){
		
		for(cols = 0; cols < LCD_COLUMNS; cols++)
			 g_LCD.frameBuffer[rows][cols]= ' ';
		 
	}
	  
}

/**********************************************************************
*
* Function:    _setFrameBufferUpdateFlag
*
* Description: Sets frame buffer update flag of LCD module.
*
* Notes:
*
* Returns:     None.
*
**********************************************************************/
 
static void _setFrameBufferUpdateFlag(void){
	 
	g_LCD.statusRegister.framebufferupdate = TRUE;
	 
}

/**********************************************************************
*
* Function:    _clearFrameBufferUpdateFlag
*
* Description: Clears frame buffer update flag of LCD module.
*
* Notes:
*
* Returns:     None.
*
**********************************************************************/
 
static void _clearFrameBufferUpdateFlag(void){
	 
	g_LCD.statusRegister.framebufferupdate = FALSE;
	 
}

/**********************************************************************
*
* Function:    _shiftBuffer
*
* Description: Shifts frame buffer in specific direction
*              of LCD module.
*
* Notes:
*
* Returns:     None.
*
**********************************************************************/
 
static void _shiftBuffer(lcddirection_t a_direction, uint8_t a_offset, uint8_t a_width){
	  
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

/**********************************************************************
*
* Function:    _scrollBuffer
*
* Description: Scrolls frame buffer in specific direction
*              of LCD module.
*
* Notes:
*
* Returns:     None.
*
**********************************************************************/
 
static void _scrollBuffer(lcddirection_t a_direction, uint8_t a_offset, uint8_t a_width){
	 
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

/**********************************************************************
*
* Function:    _resetCursorConfig
*
* Description: Resets cursor configuration of LCD module.
*
* Notes:
*
* Returns:     None.
*
**********************************************************************/
 
static void _resetCursorConfig(void){
	 
	LCD_CONFIG_CURSOR(FALSE,FALSE);
	 
}

/**********************************************************************
*
* Function:    _saveCursorConfig
*
* Description: Saves cursor configuration of LCD module.
*
* Notes:
*
* Returns:     None.
*
**********************************************************************/
 
static void _saveCursorConfig(bool_t a_cursorVisible, bool_t a_cursorBlinking){
	 
	g_LCD.statusRegister.value &= ~0x03;
	g_LCD.statusRegister.cursorvisible = a_cursorVisible;
	g_LCD.statusRegister.cursorblinking = a_cursorBlinking;
	 
}

/**********************************************************************
*
* Function:    _restoreCursorConfig
*
* Description: Restores cursor configuration of LCD module.
*
* Notes:
*
* Returns:     None.
*
**********************************************************************/
 
static void _restoreCursorConfig(void){
	 
	LCD_CONFIG_CURSOR(g_LCD.statusRegister.cursorvisible,g_LCD.statusRegister.cursorblinking);
	 
}

/**********************************************************************
*
* Function:    _resetTextDirectionConfig
*
* Description: Resets text direction configuration of LCD module.
*
* Notes:
*
* Returns:     None.
*
**********************************************************************/
 
static void _resetTextDirectionConfig(void){
	  
	LCD_CONFIG_TEXT_DIRECTION(TRUE);
	  
}

/**********************************************************************
*
* Function:    _saveTextDirectionConfig
*
* Description: Saves text direction configuration of LCD module.
*
* Notes:
*
* Returns:     None.
*
**********************************************************************/
  
static void _saveTextDirectionConfig(bool_t a_leftToRight){
	  
	g_LCD.statusRegister.leftToRight = a_leftToRight;
	  
}

/**********************************************************************
*
* Function:    _restoreTextDirectionConfig
*
* Description: Restores text direction configuration of LCD module.
*
* Notes:
*
* Returns:     None.
*
**********************************************************************/
  
static void _restoreTextDirectionConfig(void){
	  
	LCD_CONFIG_TEXT_DIRECTION(g_LCD.statusRegister.leftToRight);
	  
}

/**********************************************************************
*
* Function:    _shiftCursorPosition
*
* Description: Shifts cursor position of LCD module.
*
* Notes:
*
* Returns:     None.
*
**********************************************************************/
 
static void _shiftCursorPosition(lcddirection_t a_dir){
	
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
	
		_saveCursorPosition(cursorPosition.row,cursorPosition.column);	
	
}

/**********************************************************************
*
* Function:    _saveCursorPosition
*
* Description: Saves cursor position of LCD module.
*
* Notes:
*
* Returns:     None.
*
**********************************************************************/

static void _saveCursorPosition(uint8_t a_row, uint8_t a_column){
	
	if(g_LCD.statusRegister.cursorpositionupdate){
	 
		g_LCD.primaryCursorPosition.row = a_row;
		g_LCD.primaryCursorPosition.column = a_column;
		
	}else{
	
		g_LCD.secondaryCursorPosition.row = a_row;
		g_LCD.secondaryCursorPosition.column = a_column;
		
	}
	 
}

/**********************************************************************
*
* Function:    _restoreCursorPosition
*
* Description: Restores cursor position of LCD module.
*
* Notes:
*
* Returns:     None.
*
**********************************************************************/

static void _restoreCursorPosition(lcddirection_t a_dir, uint8_t a_offset, uint8_t a_width){
	
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

/**********************************************************************
*
* Function:    _setCursorPositionUpdateFlag
*
* Description: Sets cursor position update flag of LCD module.
*
* Notes:
*
* Returns:     None.
*
**********************************************************************/
 
static void _setCursorPositionUpdateFlag(void){
	 
	g_LCD.statusRegister.cursorpositionupdate = TRUE;
	 
}

/**********************************************************************
*
* Function:    _clearCursorPositionUpdateFlag
*
* Description: Clears cursor position update flag of LCD module.
*
* Notes:
*
* Returns:     None.
*
**********************************************************************/
 
static void _clearCursorPositionUpdateFlag(void){
	  
	g_LCD.statusRegister.cursorpositionupdate = FALSE;
	  
}