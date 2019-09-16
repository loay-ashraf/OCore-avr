/** @file LCD.c
 *  @brief includes function definition
 *  @author Loay Ashraf
 *	@version 1.0.0
 *  @pre include LCD.h
 *  @bug no known bugs
 *	@warning the functions LCD_print_int and LCD_print_float have maximum digit number of 7 and has to be modified manually
 *  @see LCD.h
 */

//------------INCLUDE DRIVER HEADER FILE------------//

 #include "lcd.h"
 
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

static void resetDisplay(void);
static void sendNibble(ubyte_t a_data, lcdtransmissiontype_t a_transmissionType);
static ubyte_t readNibble(void);
static void initBuffer(void);
static void putBuffer(void);	
static void clearBuffer(void);
static void setFrameBufferUpdateFlag(void);
static void clearFrameBufferUpdateFlag(void);
static void shiftUnits(lcddirection_t a_direction, ubyte_t a_offset, ubyte_t a_width);
static void scrollUnits(lcddirection_t a_direction, ubyte_t a_offset, ubyte_t a_width);
static void resetCursorConfig(void);
static void saveCursorConfig(bool_t a_cursorVisible, bool_t a_cursorBlinking);
static void restoreCursorConfig(void);
static void updateCursorPosition(lcddirection_t a_dir);
static void saveCursorPosition(ubyte_t a_row, ubyte_t a_column);	
static void restoreCursorPosition(lcddirection_t a_dir, ubyte_t a_offset, ubyte_t a_width);
static void setCursorPositionUpdateFlag(void);
static void clearCursorPositionUpdateFlag(void);

 void LCD_init(bool_t a_cursorVisible, bool_t a_cursorBlinking){
	 
	initBuffer();		// initialize frame buffer 
	
	saveCursorConfig(a_cursorVisible,a_cursorBlinking);
	
	setCursorPositionUpdateFlag();
	setFrameBufferUpdateFlag();
	
	#ifdef LCD_RW_PIN
	
		gpio_setPinDirection(LCD_RW_PIN,IO_OUTPUT);
	
	#endif	
	
	gpio_setPinDirection(LCD_EN_PIN,IO_OUTPUT);							// set control port direction register
	gpio_setPinDirection(LCD_RS_PIN,IO_OUTPUT);	
	gpio_setPortDirection(LCD_DATA_PORT,LCD_DATA_PORT_MASK,IO_OUTPUT);			// set data port direction register
	
	resetDisplay();
	
	#if (LCD_DATA_MODE == 4)
		
		//------------CONFIGURE LCD BEHAVIOUR------------//
		
		LCD_sendInstruction(LCD_4BIT_MODE);												// 4-bit interface, 2-line mode, 5x8 dots format
		LCD_sendInstruction(LCD_DISPLAY_ON|(a_cursorVisible<<1)|a_cursorBlinking);		// display ON, cursor OFF, blink OFF
		LCD_sendInstruction(LCD_CURSOR_RIGHT);											// cursor moves to the right, no display shift
		LCD_sendInstruction(LCD_CLEAR_DISPLAY);											// clear display
		
	#elif (LCD_DATA_MODE == 8)
	
		//------------CONFIGURE LCD BEHAVIOUR------------//
	
		LCD_sendInstruction(LCD_8BIT_MODE);	// 8-bit interface, 2-line mode, 5x8 dots format
		LCD_sendInstruction(LCD_DISPLAY_ON|(a_cursorVisible<<1)|a_cursorBlinking);	// display ON, cursor OFF, blink OFF
		LCD_sendInstruction(LCD_CURSOR_RIGHT);	// cursor moves to the right, no display shift
		LCD_sendInstruction(LCD_CLEAR_DISPLAY);	// clear display
	
	#endif
	
 }

 void LCD_sendInstruction(ubyte_t a_instruction){
	
	#if (LCD_DATA_MODE == 4)
	
		#if (LCD_DATA_PORT_MASK == 0x0F)
			
			//------------SEND HIGH NIBBLE------------//
			
			sendNibble((a_instruction>>4),LCD_INSTRUCTION);

			//------------SEND LOW NIBBLE------------//

			sendNibble(a_instruction,LCD_INSTRUCTION);
			
		#elif (LCD_DATA_PORT_MASK == 0xF0)
		
			//------------SEND HIGH NIBBLE------------//
			
			sendNibble(a_instruction,LCD_INSTRUCTION);

			//------------SEND LOW NIBBLE------------//

			sendNibble((a_instruction<<4),LCD_INSTRUCTION);
				
		#endif		
	
	#elif (LCD_DATA_MODE == 8)
		
		//------------SEND 8-BIT COMMAND------------//
		
		sendNibble(a_instruction,LCD_INSTRUCTION);
	
	#endif
   
 }
 
 void LCD_clearUnit(ubyte_t a_row, ubyte_t a_column){
	
	resetCursorConfig();						// turn off cursor
	
	LCD_setCursorPosition(a_row, a_column);		// unit position
	
	LCD_putc(' ');						// clear
	
	LCD_setCursorPosition(a_row, a_column);		// unit position
	
	restoreCursorConfig();			// restore cursor configuration
	
 }
 
 void LCD_clearRow(ubyte_t a_row, ubyte_t a_offset){
	 
	ubyte_t i;
	
	resetCursorConfig();						// turn off cursor
	
	LCD_setCursorPosition(a_row,a_offset);		// row starting position
	
	for(i=a_offset;i<LCD_COLUMNS;i++)
		LCD_putc(' ');					// clear
	 	 
	LCD_setCursorPosition(a_row,a_offset);		// row starting position
	
	restoreCursorConfig();			// restore cursor configuration
	
 }
 
 void LCD_clearColumn(ubyte_t a_column, ubyte_t a_offset){

	ubyte_t i;
	
	resetCursorConfig();						// turn off cursor
	
	for(i = a_offset; i < LCD_ROWS; i++){
		
		LCD_setCursorPosition(i,a_column);
		LCD_putc(' ');					// clear
		
	}
	
	LCD_setCursorPosition(a_offset,a_column);		// column starting position
	
	restoreCursorConfig();			// restore cursor configuration
	 
 }
 
 void LCD_clearDisplay(void){
	 
	LCD_sendInstruction(LCD_CLEAR_DISPLAY);	// clear display command
	 
	clearBuffer();			// clear frame buffer
	 
	saveCursorPosition(0,0);		// default cursor position

 }
 
 void LCD_configCursor(bool_t a_cursorVisible, bool_t a_cursorBlinking){
	 
	if(a_cursorVisible < 2 && a_cursorBlinking < 2){
		
		// store cursor configuration
		
		saveCursorConfig(a_cursorVisible, a_cursorBlinking);

		LCD_sendInstruction(LCD_DISPLAY_ON|(a_cursorVisible<<1)|a_cursorBlinking);		// configure cursor
		
	}
 }

 void LCD_setCursorPosition(ubyte_t a_row, ubyte_t a_col){
	
	if(a_row >= LCD_ROWS)		// check row
		a_row = 0;
	
	if(a_col >= LCD_COLUMNS)	// check column
		a_col = 0;
	
	// update cursor position
	
	if(g_LCD.statusRegister.cursorpositionupdate){
	
		saveCursorPosition(a_row,a_col);
		
	}
	
	switch(a_row){

		case 0: LCD_sendInstruction(LCD_ROW_ZERO+a_col);	// first row
		break;
		case 1: LCD_sendInstruction(LCD_ROW_ONE+a_col);	// second row
		break;
		case 2: LCD_sendInstruction(LCD_ROW_TWO+a_col);	// third row
		break;
		case 3: LCD_sendInstruction(LCD_ROW_THREE+a_col);	// fourth row
		break;
		default: LCD_sendInstruction(LCD_ROW_ZERO+a_col);	// default is first row
		break;

	}
  }
 
 lcdposition_t LCD_getCursorPosition(void){
	 
	 return g_LCD.cursorPos;
	 
 }
 
 void LCD_shiftCursor(lcddirection_t a_dir){
	 
	 updateCursorPosition(a_dir);
	 
	 switch (a_dir){
		
		#if (LCD_ROWS == 2)
		
			case LCD_LEFT:
			case LCD_RIGHT: LCD_sendInstruction(LCD_SHIFT_CURSOR|(a_dir<<2));
			break;
						   		   	
		#elif (LCD_ROWS == 4)
			
			case LCD_LEFT: {

				if(g_LCD.cursorPos.row < LCD_ROWS-1 && g_LCD.cursorPos.column == LCD_COLUMNS-1){
					
					clearCursorPositionUpdateFlag();
					LCD_setCursorPosition(g_LCD.cursorPos.row,g_LCD.cursorPos.column);
					setCursorPositionUpdateFlag();
					
				}else{
					
					LCD_sendInstruction(LCD_SHIFT_CURSOR|(a_dir<<2));
				
				}
			}
			break;
			
			case LCD_RIGHT: {
				
				if(g_LCD.cursorPos.row > 0 && g_LCD.cursorPos.column == 0){
					
					clearCursorPositionUpdateFlag();
					LCD_setCursorPosition(g_LCD.cursorPos.row,g_LCD.cursorPos.column);
					setCursorPositionUpdateFlag();
					
				}else{
				
					LCD_sendInstruction(LCD_SHIFT_CURSOR|(a_dir<<2));
				
				}
			}
			break;
		
		#endif
					   
		case LCD_UP:	 
		case LCD_DOWN: {
			
			clearCursorPositionUpdateFlag();
			LCD_setCursorPosition(g_LCD.cursorPos.row,g_LCD.cursorPos.column);
			setCursorPositionUpdateFlag();
		
		}
		break;		 		 	 	 		 
		
		default: break;
	}	
 }

 void LCD_shiftDisplay(lcddirection_t a_dir, ubyte_t a_offset, ubyte_t a_width){
		 
	switch(a_dir){
		 
		case LCD_LEFT:
		case LCD_RIGHT: {
			  
			if((a_offset+a_width)>LCD_ROWS)
				a_width = LCD_ROWS-a_offset;
			  
			shiftUnits(a_dir,a_offset,a_width);
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
			
			shiftUnits(a_dir, a_offset, a_width);
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

 void LCD_scrollDisplay(lcddirection_t a_dir, ubyte_t a_offset, ubyte_t a_width){
	 
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
	
	scrollUnits(a_dir, a_offset, a_width);  
	putBuffer();
	restoreCursorPosition(a_dir, a_offset, a_width);
	
 }
 
 void LCD_scrollDisplayNative(lcddirection_t a_dir){
	 
	if(a_dir == LCD_LEFT || a_dir == LCD_RIGHT)
		LCD_sendInstruction(LCD_SCROLL_DISPLAY|(a_dir<<2));
	 
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
 
 void LCD_defineCustomCharacter(ubyte_t a_characterIndex, ubyte_t a_characterArray[8]){
	
	clearCursorPositionUpdateFlag();
	clearFrameBufferUpdateFlag();
	 
	LCD_sendInstruction(0x40+(a_characterIndex*8));
	LCD_putc(a_characterArray[0]);	 
	LCD_putc(a_characterArray[1]);	
	LCD_putc(a_characterArray[2]);	
	LCD_putc(a_characterArray[3]);	
	LCD_putc(a_characterArray[4]);	
	LCD_putc(a_characterArray[5]);	
	LCD_putc(a_characterArray[6]);	
	LCD_putc(a_characterArray[7]);	
	
	setCursorPositionUpdateFlag();
	setFrameBufferUpdateFlag();
	
	LCD_setCursorPosition(0,0);
	
 }
	
 void LCD_putc(char a_data){
   
	#if (LCD_DATA_MODE == 4)
   
		#if (LCD_DATA_PORT_MASK == 0x0F)
		   
			//------------SEND HIGH NIBBLE------------//
		   
			sendNibble((a_data>>4),LCD_DATA);

			//------------SEND LOW NIBBLE------------//

			sendNibble(a_data,LCD_DATA);
		   
		#elif (LCD_DATA_PORT_MASK == 0xF0)
		   
			//------------SEND HIGH NIBBLE------------//
		   
			sendNibble(a_data,LCD_DATA);

			//------------SEND LOW NIBBLE------------//

			sendNibble((a_data<<4),LCD_DATA);
		   
		#endif
   
	#elif (LCD_DATA_MODE == 8)
   
		//------------SEND 8-BIT COMMAND------------//
   
		sendNibble(a_data,LCD_DATA);
   
	#endif
	
	//------------UPDATE DISPLAY BUFFER------------//
	
	if(g_LCD.statusRegister.framebufferupdate)
		g_LCD.frameBuffer[g_LCD.cursorPos.row][g_LCD.cursorPos.column] = a_data;
	
	//------------UPDATE CURSOR POSITION------------//
	
	if(g_LCD.statusRegister.cursorpositionupdate){
		
		updateCursorPosition(LCD_RIGHT);
	
		#if (LCD_ROWS == 4)
		
			if(g_LCD.cursorPos.row > 0 && g_LCD.cursorPos.column == 0){
			
				clearCursorPositionUpdateFlag();
				LCD_setCursorPosition(g_LCD.cursorPos.row,g_LCD.cursorPos.column);
				setCursorPositionUpdateFlag();
			
			}
		
		#endif		
	}
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
	 
	 char buffer[8];
	 sprintf(buffer,"%p",a_ptr);
	 LCD_puts(buffer);
	 
 }
 
 char LCD_readChar(void){
	 
	char data = 0;
	 
	#if (LCD_DATA_MODE == 4)
	 
		#if (LCD_DATA_PORT_MASK == 0x0F)
	 
			//------------READ HIGH NIBBLE------------//
	 
			data |= (readNibble()<<4);

			//------------READ LOW NIBBLE------------//

			data |= readNibble();
	 
		#elif (LCD_DATA_PORT_MASK == 0xF0)
	 
			//------------READ HIGH NIBBLE------------//
	 
			data |= readNibble();

			//------------READ LOW NIBBLE------------//

			data |= (readNibble()>>4);
	 
		#endif
	 
	#elif (LCD_DATA_MODE == 8)
	 
		//------------READ 8-BIT DATA------------//
	 
		data |= readNibble();
	 
	#endif

	return data;
 }
 
 char * LCD_readString(uint8_t a_row, uint8_t a_width){
	 
	 static char rowData[LCD_COLUMNS+1];
	 uint8_t index;
	 
	 LCD_setCursorPosition(a_row,0); 
	 
	 for (index=0;index<a_width;index++)
		rowData[index] = LCD_readChar();
		
	rowData[a_width] = '\0';
	
	return rowData; 
 }
 
 static void resetDisplay(void){
	 
	 #if (LCD_DATA_MODE == 4)
		
		#if (LCD_DATA_PORT_MASK == 0x0F)
		
			/*------------RESET THE LCD------------*/
		
			sendNibble(0x03,LCD_INSTRUCTION);
			DELAY_MS(1);
			sendNibble(0x03,LCD_INSTRUCTION);
			DELAY_MS(1);
			sendNibble(0x03,LCD_INSTRUCTION);
			DELAY_MS(1);
			sendNibble(0x02,LCD_INSTRUCTION);
			DELAY_MS(1);
		
		#elif (LCD_DATA_PORT_MASK == 0xF0)
		
			/*------------RESET THE LCD------------*/
		
			sendNibble((0x03<<4),LCD_INSTRUCTION);
			DELAY_MS(1);
			sendNibble((0x03<<4),LCD_INSTRUCTION);
			DELAY_MS(1);
			sendNibble((0x03<<4),LCD_INSTRUCTION);
			DELAY_MS(1);
			sendNibble((0x02<<4),LCD_INSTRUCTION);
			DELAY_MS(1);

		#endif
		
	#elif (LCD_DATA_MODE == 8)
		
		/*------------RESET THE LCD------------*/
		
		sendNibble(0x03,LCD_INSTRUCTION);
		DELAY_MS(1);
		sendNibble(0x03,LCD_INSTRUCTION);
		DELAY_MS(1);
		sendNibble(0x03,LCD_INSTRUCTION);
		DELAY_MS(1);
	
	#endif		
	 
 }
 static void sendNibble(ubyte_t a_data, lcdtransmissiontype_t a_transType){
	
	//------------SEND A NIBBLE------------//
	
	if(a_transType == LCD_INSTRUCTION){
		
		gpio_setPin(LCD_EN_PIN);								// enable LCD interface for new data (EN signal), Command register is selected
		DELAY_US(800);
		gpio_setPort(LCD_DATA_PORT, (a_data & LCD_DATA_PORT_MASK));		// send a nibble of command
		DELAY_US(800);
		gpio_clearPin(LCD_EN_PIN);							// clear control port
		DELAY_US(200);
		gpio_clearPort(LCD_DATA_PORT, (a_data & LCD_DATA_PORT_MASK));		// clear data port
		
	}else if(a_transType == LCD_DATA){
		
		gpio_setPin(LCD_RS_PIN);								// select Data register (RS signal)
		DELAY_US(800);
		gpio_setPin(LCD_EN_PIN);								// enable LCD interface for new data (EN signal)
		DELAY_US(800);
		gpio_setPort(LCD_DATA_PORT, (a_data & LCD_DATA_PORT_MASK));		// send a nibble of data
		DELAY_US(800);
		gpio_clearPin(LCD_EN_PIN);			// clear EN pin
		gpio_clearPin(LCD_RS_PIN);			// clear RS pin
		DELAY_US(200);
		gpio_clearPort(LCD_DATA_PORT, (a_data & LCD_DATA_PORT_MASK));		// clear data port
		
	}
	 
 }
 
 static ubyte_t readNibble(void){
	 
	#ifdef LCD_RW_PIN
		
		ubyte_t data;
		
		gpio_setPortDirection(LCD_DATA_PORT,LCD_DATA_PORT_MASK,IO_INPUT);
	 
		gpio_setPin(LCD_RS_PIN);										// select Data register (RS signal)
		gpio_setPin(LCD_RW_PIN);										// select Data register (RW signal)
		DELAY_US(800);
		gpio_setPin(LCD_EN_PIN);										// enable GLCD interface for new data (EN signal)
		DELAY_US(800);
		data = (gpio_readPort(LCD_DATA_PORT)&LCD_DATA_PORT_MASK);		// read nibble of data
		DELAY_US(800);
		gpio_clearPin(LCD_EN_PIN);										// clear EN pin
		gpio_clearPin(LCD_RW_PIN);										// clear RW pin
		gpio_clearPin(LCD_RS_PIN);										// clear RS pin
		DELAY_US(200);
		
		gpio_setPortDirection(LCD_DATA_PORT,LCD_DATA_PORT_MASK,IO_OUTPUT);
	 
		return data;
	 
	#endif
	 
	return 0;
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
	
	LCD_setCursorPosition(0,0);
	
	#if (LCD_ROWS == 4)
		
		LCD_puts(g_LCD.frameBuffer[0]);
		LCD_puts(g_LCD.frameBuffer[2]);
		LCD_puts(g_LCD.frameBuffer[1]);
		LCD_puts(g_LCD.frameBuffer[3]);
		
	#else	
	
		ubyte_t rows;
		
		for(rows = 0; rows < LCD_ROWS; rows++)
			LCD_puts(g_LCD.frameBuffer[rows]);
	
	#endif
	
	setFrameBufferUpdateFlag();
	setCursorPositionUpdateFlag();
	restoreCursorConfig();
	
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
 
 void shiftUnits(lcddirection_t a_direction, ubyte_t a_offset, ubyte_t a_width){
	  
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
 
 void scrollUnits(lcddirection_t a_direction, ubyte_t a_offset, ubyte_t a_width){
	 
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
	 
	 LCD_sendInstruction(LCD_DISPLAY_ON); 
	 
 }
 
 static void saveCursorConfig(bool_t a_cursorVisible, bool_t a_cursorBlinking){
	 
	 g_LCD.statusRegister.value &= ~0x03;
	 g_LCD.statusRegister.cursorvisible = a_cursorVisible;
	 g_LCD.statusRegister.cursorblinking = a_cursorBlinking;
	 
 }
 
 static void restoreCursorConfig(void){
	 
	 LCD_sendInstruction(LCD_DISPLAY_ON|(g_LCD.statusRegister.cursorvisible<<1)|g_LCD.statusRegister.cursorblinking);
	 
 }
 
 static void updateCursorPosition(lcddirection_t a_dir){
	 
	switch (a_dir){
		  
		case LCD_LEFT: {
	
			if(g_LCD.cursorPos.column == 0){
				
				g_LCD.cursorPos.column = LCD_COLUMNS-1;
				
				if(g_LCD.cursorPos.row == 0)
					g_LCD.cursorPos.row = LCD_ROWS-1;
				else
					g_LCD.cursorPos.row--;
				 
			}else{
				
				g_LCD.cursorPos.column--;
				
			 }
		}
		break;
		
		case LCD_RIGHT: {
		
			if(g_LCD.cursorPos.column == LCD_COLUMNS-1){
				
				g_LCD.cursorPos.column = 0;
				
				if(g_LCD.cursorPos.row == LCD_ROWS-1)
					g_LCD.cursorPos.row = 0;
				else
					g_LCD.cursorPos.row++;
					
			}else{
				
				g_LCD.cursorPos.column++;
				
			}
		}
		break;
		
		case LCD_UP: {
			
			if(g_LCD.cursorPos.row == 0)
				g_LCD.cursorPos.row = LCD_ROWS-1;
			else
				g_LCD.cursorPos.row--;
			
		}
		break;
		
		case LCD_DOWN: {
			
			if(g_LCD.cursorPos.row == LCD_ROWS-1)
				g_LCD.cursorPos.row = 0;
			else
				g_LCD.cursorPos.row++;
			
		}
		break;
		
		default: break;
		
	}
 }
 
 static void saveCursorPosition(ubyte_t a_row, ubyte_t a_column){
	 
	g_LCD.cursorPos.row = a_row;
	g_LCD.cursorPos.column = a_column;
	 
 }
 
 static void restoreCursorPosition(lcddirection_t a_dir, ubyte_t a_offset, ubyte_t a_width){
	
	switch(a_dir){
		
		case LCD_UP: {
		
			if(g_LCD.cursorPos.column >= a_offset && g_LCD.cursorPos.column < (a_offset+a_width)){
			
				if(g_LCD.cursorPos.row == 0)
					LCD_setCursorPosition(LCD_ROWS-1,g_LCD.cursorPos.column);
				else
					LCD_setCursorPosition(g_LCD.cursorPos.row-1,g_LCD.cursorPos.column);
		
			}else{
		
				LCD_setCursorPosition(g_LCD.cursorPos.row,g_LCD.cursorPos.column);
		
			}
		}
		break;
	
		case LCD_DOWN: {
		
			if(g_LCD.cursorPos.column >= a_offset && g_LCD.cursorPos.column < (a_offset+a_width)){
			
				if(g_LCD.cursorPos.row == LCD_ROWS-1)
				LCD_setCursorPosition(0,g_LCD.cursorPos.column);
				else
				LCD_setCursorPosition(g_LCD.cursorPos.row+1,g_LCD.cursorPos.column);
			
			}else{
		
				LCD_setCursorPosition(g_LCD.cursorPos.row,g_LCD.cursorPos.column);
		
			}
		}
		break;

		case LCD_LEFT: {
		
			if(g_LCD.cursorPos.row >= a_offset && g_LCD.cursorPos.row < (a_offset+a_width)){	
			
				if(g_LCD.cursorPos.column == 0)
					LCD_setCursorPosition(g_LCD.cursorPos.row,LCD_COLUMNS-1);
				else
					LCD_setCursorPosition(g_LCD.cursorPos.row,g_LCD.cursorPos.column-1);
				
			}else{
			
				LCD_setCursorPosition(g_LCD.cursorPos.row,g_LCD.cursorPos.column);
			
			}
		}
		break;

		case LCD_RIGHT: {
		
			if(g_LCD.cursorPos.row >= a_offset && g_LCD.cursorPos.row < (a_offset+a_width)){
			
				if(g_LCD.cursorPos.column == LCD_COLUMNS-1)
					LCD_setCursorPosition(g_LCD.cursorPos.row,0);
				else
					LCD_setCursorPosition(g_LCD.cursorPos.row,g_LCD.cursorPos.column+1);
		
			}else{
			
				LCD_setCursorPosition(g_LCD.cursorPos.row,g_LCD.cursorPos.column);
			
			}
		}
		break;

		case LCD_NONE: {
		
			LCD_setCursorPosition(g_LCD.cursorPos.row,g_LCD.cursorPos.column);
		
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