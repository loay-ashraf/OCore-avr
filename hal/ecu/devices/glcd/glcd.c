/*
 * glcd.c
 *
 * Created: 09/09/2019 07:04:18 AM
 *  Author: Loay Ashraf
 */ 
//------------INCLUDE DRIVER HEADER FILE------------//

 #include "glcd.h"
 
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

static void resetDisplay(void);
static void sendNibble(ubyte_t a_data, glcdtransmissiontype_t a_transmissionType);
static ubyte_t readNibble(void);
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
	
	#ifdef GLCD_RW_PIN
	
		gpio_setPinDirection(GLCD_RW_PIN,IO_OUTPUT);
	
	#endif
	
	#ifdef GLCD_RST_PIN
	
		gpio_setPinDirection(GLCD_RST_PIN,IO_OUTPUT);
		gpio_setPin(GLCD_RST_PIN);
	
	#endif 
	
	gpio_setPinDirection(GLCD_PSB_PIN,IO_OUTPUT);
	gpio_setPinDirection(GLCD_EN_PIN,IO_OUTPUT);										// set control port direction register
	gpio_setPinDirection(GLCD_RS_PIN,IO_OUTPUT);
	
	#if (GLCD_INTERFACE == 1)
		
		gpio_setPin(GLCD_PSB_PIN);
		gpio_setPortDirection(GLCD_DATA_PORT,GLCD_DATA_PORT_MASK,IO_OUTPUT);				// set data port direction register
		
	#elif (GLCD_INTERFACE == 0)
	
		gpio_clearPin(GLCD_PSB_PIN);
		spi_enable();	
	
	#endif	
	
	resetDisplay();
	
	#if (GLCD_INTERFACE == 1)
	
		#if (GLCD_DATA_MODE == 4)
	
			//------------CONFIGURE GLCD BEHAVIOUR------------//
	
			GLCD_sendInstruction(GLCD_4BIT_MODE);												// 4-bit interface, 2-line mode, 5x8 dots format
			GLCD_sendInstruction(GLCD_DISPLAY_ON|(a_cursorVisible<<1)|a_cursorBlinking);		// display ON, cursor OFF, blink OFF
			GLCD_sendInstruction(GLCD_CURSOR_RIGHT);											// cursor moves to the right, no display shift
			GLCD_sendInstruction(GLCD_CLEAR_DISPLAY);											// clear display
	
		#elif (GLCD_DATA_MODE == 8)
	
			//------------CONFIGURE GLCD BEHAVIOUR------------//
	
			GLCD_sendInstruction(GLCD_8BIT_MODE);												// 8-bit interface, 2-line mode, 5x8 dots format
			GLCD_sendInstruction(GLCD_DISPLAY_ON|(a_cursorVisible<<1)|a_cursorBlinking);		// display ON, cursor OFF, blink OFF
			GLCD_sendInstruction(GLCD_CURSOR_RIGHT);											// cursor moves to the right, no display shift
			GLCD_sendInstruction(GLCD_CLEAR_DISPLAY);											// clear display
	
		#endif
	
	#elif (GLCD_INTERFACE == 0)
	
		//------------CONFIGURE GLCD BEHAVIOUR------------//
		
		GLCD_sendInstruction(GLCD_8BIT_MODE);												// 8-bit interface, 2-line mode, 5x8 dots format
		GLCD_sendInstruction(GLCD_DISPLAY_ON|(a_cursorVisible<<1)|a_cursorBlinking);		// display ON, cursor OFF, blink OFF
		GLCD_sendInstruction(GLCD_CURSOR_RIGHT);											// cursor moves to the right, no display shift
		GLCD_sendInstruction(GLCD_CLEAR_DISPLAY);											// clear display
		
	#endif
	
	DELAY_MS(10); 
	
 }
 
 void GLCD_enableGraphics(void){
	 
	clearFrameBufferUpdateFlag();
	clearCursorPositionUpdateFlag();
	resetCursorConfig();
	DELAY_MS(1); 
	
	#if (GLCD_INTERFACE == 1)
		 
		#if (GLCD_DATA_MODE == 4)
	
		//------------CONFIGURE GLCD BEHAVIOUR------------//
			GLCD_sendInstruction(GLCD_4BIT_MODE);
			DELAY_MS(1);
			GLCD_sendInstruction(GLCD_4BIT_MODE|GLCD_EXT_INSTRUCTION);
			DELAY_MS(1);
			GLCD_sendInstruction(GLCD_4BIT_MODE|GLCD_GRAPHICS_ON);
			DELAY_MS(1);
			GLCD_sendInstruction(GLCD_CLEAR_DISPLAY);
	
		#elif (GLCD_DATA_MODE == 8)
	
		//------------CONFIGURE GLCD BEHAVIOUR------------//
		
			GLCD_sendInstruction(GLCD_8BIT_MODE);
			DELAY_MS(1);
			GLCD_sendInstruction(GLCD_8BIT_MODE|GLCD_EXT_INSTRUCTION);
			DELAY_MS(1);
			GLCD_sendInstruction(GLCD_8BIT_MODE|GLCD_GRAPHICS_ON);
			DELAY_MS(1);
			GLCD_sendInstruction(GLCD_CLEAR_DISPLAY);
	
		#endif
	
	#elif (GLCD_INTERFACE == 0)
	
		GLCD_sendInstruction(GLCD_8BIT_MODE);
		DELAY_MS(1);
		GLCD_sendInstruction(GLCD_8BIT_MODE|GLCD_EXT_INSTRUCTION);
		DELAY_MS(1);
		GLCD_sendInstruction(GLCD_8BIT_MODE|GLCD_GRAPHICS_ON);
		DELAY_MS(1);
		GLCD_sendInstruction(GLCD_CLEAR_DISPLAY);
	
	#endif
	 
 }
 
 void GLCD_disableGraphics(void){
	 
	 #if (GLCD_INTERFACE == 1)
	 
		 #if (GLCD_DATA_MODE == 4)
	 
		 //------------CONFIGURE GLCD BEHAVIOUR------------//

			GLCD_sendInstruction(GLCD_4BIT_MODE|GLCD_EXT_INSTRUCTION);
			DELAY_MS(1);	
			GLCD_sendInstruction(GLCD_4BIT_MODE);
			DELAY_MS(1);
		
		 #elif (GLCD_DATA_MODE == 8)
	 
		 //------------CONFIGURE GLCD BEHAVIOUR------------//
		
			GLCD_sendInstruction(GLCD_8BIT_MODE|GLCD_EXT_INSTRUCTION);
			DELAY_MS(1);
			GLCD_sendInstruction(GLCD_8BIT_MODE);
			DELAY_MS(1);
	 
		 #endif
	 
	 #elif (GLCD_INTERFACE == 0)
	 
		GLCD_sendInstruction(GLCD_8BIT_MODE|GLCD_EXT_INSTRUCTION);
		DELAY_MS(1);
		GLCD_sendInstruction(GLCD_8BIT_MODE);
		DELAY_MS(1);
		
	 #endif
	 
	 setFrameBufferUpdateFlag();
	 setCursorPositionUpdateFlag();
	 restoreCursorConfig();
	 restoreCursorPosition(GLCD_NONE,0,0);
	 
 }

 void GLCD_sendInstruction(ubyte_t a_instruction){
	
	#if (GLCD_INTERFACE == 1)
	
		#if (GLCD_DATA_MODE == 4)
	
			#if (GLCD_DATA_PORT_MASK == 0x0F)
			
				//------------SEND HIGH NIBBLE------------//
			
				sendNibble((a_instruction>>4),GLCD_INSTRUCTION);

				//------------SEND LOW NIBBLE------------//

				sendNibble(a_instruction,GLCD_INSTRUCTION);
			
			#elif (GLCD_DATA_PORT_MASK == 0xF0)
		
				//------------SEND HIGH NIBBLE------------//
			
				sendNibble(a_instruction,GLCD_INSTRUCTION);

				//------------SEND LOW NIBBLE------------//

				sendNibble((a_instruction<<4),GLCD_INSTRUCTION);
				
			#endif		
	
		#elif (GLCD_DATA_MODE == 8)
		
			//------------SEND 8-BIT COMMAND------------//
		
			sendNibble(a_instruction,GLCD_INSTRUCTION);
	
		#endif
	
	#elif (GLCD_INTERFACE == 0)
	
		sendNibble(a_instruction,GLCD_INSTRUCTION);
	
	#endif
   
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
	 
	GLCD_sendInstruction(GLCD_CLEAR_DISPLAY);	// clear display command
	 
	clearBuffer();					// clear frame buffer
	 
	saveCursorPosition(0,0);		// default cursor position

 }
 
 void GLCD_configCursor(bool_t a_cursorVisible, bool_t a_cursorBlinking){
	 
	if(a_cursorVisible < 2 && a_cursorBlinking < 2){
		
		// store cursor configuration
		
		saveCursorConfig(a_cursorVisible, a_cursorBlinking);

		GLCD_sendInstruction(GLCD_DISPLAY_ON|(a_cursorVisible<<1)|a_cursorBlinking);		// configure cursor
		
	}
 }

 void GLCD_setCursorPosition(ubyte_t a_row, ubyte_t a_col){
	
	if(a_row >= GLCD_ROWS)		// check row
		a_row = 0;
	
	if(a_col >= (GLCD_COLUMNS/2))	// check column
		a_col = 0;
	
	// update cursor position
	
	if(g_GLCD.statusRegister.cursorpositionupdate)
		saveCursorPosition(a_row,a_col*2);
	
	switch(a_row){

		case 0: GLCD_sendInstruction(GLCD_ROW_ZERO+a_col);	// first row
		break;
		case 1: GLCD_sendInstruction(GLCD_ROW_ONE+a_col);	// second row
		break;
		case 2: GLCD_sendInstruction(GLCD_ROW_TWO+a_col);	// third row
		break;
		case 3: GLCD_sendInstruction(GLCD_ROW_THREE+a_col);	// fourth row
		break;
		default: GLCD_sendInstruction(GLCD_ROW_ZERO+a_col);	// default is first row
		break;

	}
  }
 
 glcdposition_t GLCD_getCursorPosition(void){
	 
	 return g_GLCD.cursorPos;
	 
 }
 
 void GLCD_shiftCursor(glcddirection_t a_dir){
	 
	 updateCursorPosition(a_dir);
	 
	 switch (a_dir){
		
		#if (GLCD_ROWS == 2)
		
			case GLCD_LEFT:
			case GLCD_RIGHT: GLCD_sendInstruction(GLCD_SHIFT_CURSOR|(a_dir<<2));
			break;
						   		   	
		#elif (GLCD_ROWS == 4)
			
			case GLCD_LEFT: {

				if(g_GLCD.cursorPos.row < GLCD_ROWS-1 && g_GLCD.cursorPos.column == GLCD_COLUMNS-1){
					
					clearCursorPositionUpdateFlag();
					GLCD_setCursorPosition(g_GLCD.cursorPos.row,g_GLCD.cursorPos.column);
					setCursorPositionUpdateFlag();
					
				}else{
					
					GLCD_sendInstruction(GLCD_SHIFT_CURSOR|(a_dir<<2));
				
				}
			}
			break;
			
			case GLCD_RIGHT: {
				
				if(g_GLCD.cursorPos.row > 0 && g_GLCD.cursorPos.column == 0){
					
					clearCursorPositionUpdateFlag();
					GLCD_setCursorPosition(g_GLCD.cursorPos.row,g_GLCD.cursorPos.column);
					setCursorPositionUpdateFlag();
					
				}else{
				
					GLCD_sendInstruction(GLCD_SHIFT_CURSOR|(a_dir<<2));
				
				}
			}
			break;
		
		#endif
					   
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
		GLCD_sendInstruction(GLCD_SCROLL_DISPLAY|(a_dir<<2));
	 
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
	 
	GLCD_sendInstruction(0x40+(a_characterIndex*8));
	
	GLCD_putw(a_characterArray[0]); // 0
	GLCD_putw(a_characterArray[1]); // 1
	GLCD_putw(a_characterArray[2]); // 2
	GLCD_putw(a_characterArray[3]); // 3
	GLCD_putw(a_characterArray[4]); // 4
	GLCD_putw(a_characterArray[5]); // 5
	GLCD_putw(a_characterArray[6]); // 6
	GLCD_putw(a_characterArray[7]); // 7
	GLCD_putw(a_characterArray[8]); // 8
	GLCD_putw(a_characterArray[9]); // 9
	GLCD_putw(a_characterArray[10]); // A
	GLCD_putw(a_characterArray[11]); // B
	GLCD_putw(a_characterArray[12]); // C
	GLCD_putw(a_characterArray[13]); // D
	GLCD_putw(a_characterArray[14]); // E
	GLCD_putw(a_characterArray[15]); // F
	
	setCursorPositionUpdateFlag();
	setFrameBufferUpdateFlag();
	
	GLCD_setCursorPosition(0,0);
	
 }
	
 void GLCD_putc(char a_char){
	
	#if (GLCD_INTERFACE == 1)
	
		#if (GLCD_DATA_MODE == 4)
   
			#if (GLCD_DATA_PORT_MASK == 0x0F)
		   
				//------------SEND HIGH NIBBLE------------//
		   
				sendNibble((a_char>>4),GLCD_DATA);

				//------------SEND LOW NIBBLE------------//

				sendNibble(a_char,GLCD_DATA);
		   
			#elif (GLCD_DATA_PORT_MASK == 0xF0)
		   
				//------------SEND HIGH NIBBLE------------//
		   
				sendNibble(a_char,GLCD_DATA);

				//------------SEND LOW NIBBLE------------//

				sendNibble((a_char<<4),GLCD_DATA);
		   
			#endif
   
		#elif (GLCD_DATA_MODE == 8)
   
			//------------SEND 8-BIT DATA------------//
   
			sendNibble(a_char,GLCD_DATA);
   
		#endif
	
	#elif (GLCD_INTERFACE == 0)
	
		sendNibble(a_char,GLCD_DATA);
	
	#endif
	
	//------------UPDATE DISPLAY BUFFER------------//
	
	if(g_GLCD.statusRegister.framebufferupdate)
		g_GLCD.frameBuffer[g_GLCD.cursorPos.row][g_GLCD.cursorPos.column] = a_char;
	
	//------------UPDATE CURSOR POSITION------------//
	
	if(g_GLCD.statusRegister.cursorpositionupdate){
		
		updateCursorPosition(GLCD_RIGHT);
		
		#if (GLCD_ROWS == 4)
		
			if(g_GLCD.cursorPos.row >= 0 && g_GLCD.cursorPos.column == 0){
				
				clearCursorPositionUpdateFlag();
				GLCD_setCursorPosition(g_GLCD.cursorPos.row,g_GLCD.cursorPos.column);
				setCursorPositionUpdateFlag();
				
			}
		
		#endif
	}
}

void GLCD_putw(uword_t a_word) {
	
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
	
	char data = 0;
	
	#if (GLCD_DATA_MODE == 4)
	
		#if (GLCD_DATA_PORT_MASK == 0x0F)
	
			//------------READ HIGH NIBBLE------------//
	
			data |= (readNibble()<<4);

			//------------READ LOW NIBBLE------------//

			data |= readNibble();
	
		#elif (GLCD_DATA_PORT_MASK == 0xF0)
	
			//------------READ HIGH NIBBLE------------//
	
			data |= readNibble();

			//------------READ LOW NIBBLE------------//

			data |= (readNibble()>>4);
	
		#endif
	
	#elif (GLCD_DATA_MODE == 8)
	
		//------------READ 8-BIT DATA------------//
	
		data |= readNibble();
	
	#endif

	return data;
}

void GLCD_drawPixel(uint8_t a_x, uint8_t a_y){
	
	uint8_t pageNum;
	uint8_t subPageNum;
	bool_t secondSubPage;
	uint8_t bitNum; 
	
	pageNum = a_x/16;
	subPageNum = a_x/8;
	secondSubPage = subPageNum-(2*pageNum);
	bitNum = a_x-(subPageNum*8);
	
	if(a_y<32){
		
		GLCD_sendInstruction(0x80+a_y);
		GLCD_sendInstruction(0x80+pageNum);		
	
	}else{
		
		GLCD_sendInstruction(0x80+(a_y-32));
		GLCD_sendInstruction(0x88+pageNum);
		
	}
	
	if(secondSubPage){
		
		GLCD_putc(0x00);
		GLCD_putc(1<<(7-bitNum));
		
	}else{
		
		GLCD_putc(1<<(7-bitNum));
		GLCD_putc(0x00);
		
	}	
	
}

void GLCD_drawLine(uint8_t a_startX, uint8_t a_startY, uint8_t a_endX, uint8_t a_endY){
	
	uint8_t diffX,diffY;
	
	diffX = a_endX > a_startX? a_endX-a_startX : a_startX-a_endX;
	diffY = a_endY > a_startY? a_endY-a_startY : a_startY-a_endY;
	
	if(!diffY){
	
		bool_t bits[128];
		ubyte_t bytes[16];
		uint8_t arrayIndex;
	
		for(arrayIndex=0;arrayIndex<128;arrayIndex++){
		
			if((arrayIndex>=a_startX) && (arrayIndex<=a_endX))
				bits[arrayIndex] = TRUE;
			else
				bits[arrayIndex] = FALSE;	
			
		}
	
		for(arrayIndex=0;arrayIndex<16;arrayIndex++){
		
			bytes[arrayIndex] = 0;
			bytes[arrayIndex] |= (bits[0+(arrayIndex*8)]<<7);
			bytes[arrayIndex] |= (bits[1+(arrayIndex*8)]<<6);
			bytes[arrayIndex] |= (bits[2+(arrayIndex*8)]<<5);
			bytes[arrayIndex] |= (bits[3+(arrayIndex*8)]<<4);
			bytes[arrayIndex] |= (bits[4+(arrayIndex*8)]<<3);
			bytes[arrayIndex] |= (bits[5+(arrayIndex*8)]<<2);
			bytes[arrayIndex] |= (bits[6+(arrayIndex*8)]<<1);
			bytes[arrayIndex] |= (bits[7+(arrayIndex*8)]<<0);
		
		}
	
		if(a_startY<32){
		
			GLCD_sendInstruction(0x80+a_startY);
			GLCD_sendInstruction(0x80);
		
		}else{
		
			GLCD_sendInstruction(0x80+(a_startY-32));
			GLCD_sendInstruction(0x88);
		
		}
	
		for(arrayIndex=0;arrayIndex<16;arrayIndex++)
			GLCD_putc(bytes[arrayIndex]);
		
	}else if(!diffX){
		
		uint8_t Ycounter;
		
		for(Ycounter=a_startY;Ycounter<=a_endY;Ycounter++)
			GLCD_drawPixel(a_startX,Ycounter);
		
	}else{
		
		uint8_t x,y;
		float m,c;
		
		m = (float)(a_endY-a_startY)/(float)(a_endX-a_startX);
		c = a_startY-(m*a_startX);
		
		for(x=a_startX;x<=a_endX;x++){
			
			y = round((m*x)+c);
			GLCD_drawPixel(x,y);
			
		}	
	}
}

void GLCD_fillDisplay(uint8_t a_pattern){
	
	uint8_t x,y;
	
	for(y=0;y<64;y++){
		
		if(y<32){
			
			GLCD_sendInstruction(0x80+y);
			GLCD_sendInstruction(0x80);
			
			for(x=0;x<8;x++){
				
				GLCD_putc(a_pattern);
				GLCD_putc(a_pattern);
				
			}
			
		}else{
			
			GLCD_sendInstruction(0x80+(y-32));
			GLCD_sendInstruction(0x88);
			
			for(x=0;x<8;x++){
				
				GLCD_putc(a_pattern);
				GLCD_putc(a_pattern);
				
			}
		}
	}
		
}

void GLCD_putImageRAM(const uint8_t * a_image){
	
	uint8_t x,y;
	
	for(y=0;y<64;y++){
		
		if(y<32){
			
			for(x=0;x<8;x++){
				
				GLCD_sendInstruction(0x80|y);
				GLCD_sendInstruction(0x80|x);
				
				GLCD_putc(a_image[(2*x)+(16*y)]);
				GLCD_putc(a_image[(2*x)+1+(16*y)]);
				
			}
			
			}else{
			
			for(x=0;x<8;x++){
				
				GLCD_sendInstruction(0x80|(y-32));
				GLCD_sendInstruction(0x88|x);
				
				GLCD_putc(a_image[(2*x)+(16*y)]);
				GLCD_putc(a_image[(2*x)+1+(16*y)]);
				
			}
		}
	}
	
}

void GLCD_putImageROM(const uint8_t * a_image){
	
	uint8_t x,y;
	uint8_t byte;
	
	for(y=0;y<64;y++){
		
		if(y<32){
			
			GLCD_sendInstruction(0x80+y);
			GLCD_sendInstruction(0x80);
			
			for(x=0;x<8;x++){
				
				byte = pgm_read_byte(&a_image[(2*x)+(16*y)]);
				GLCD_putc(byte);
				
				byte = pgm_read_byte(&a_image[(2*x)+(16*y)+1]);
				GLCD_putc(byte);
				
			}
			
			}else{
			
			GLCD_sendInstruction(0x80+(y-32));
			GLCD_sendInstruction(0x88);
			
			for(x=0;x<8;x++){
				
				byte = pgm_read_byte(&a_image[(2*x)+(16*y)]);
				GLCD_putc(byte);

				byte = pgm_read_byte(&a_image[(2*x)+(16*y)+1]);
				GLCD_putc(byte);
				
			}
		}
	}

}

 static void resetDisplay(void){
	
	#ifdef GLCD_RST_PIN
	
		gpio_clearPin(GLCD_RST_PIN);
		DELAY_MS(100);
		gpio_setPin(GLCD_RST_PIN);
		
	#else	
		
		#if (GLCD_INTERFACE == 1)
		
			#if (GLCD_DATA_MODE == 4)
		
				#if (GLCD_DATA_PORT_MASK == 0x0F)
		
					/*------------RESET THE GLCD------------*/
		
					sendNibble(0x03,GLCD_INSTRUCTION);
					DELAY_MS(1);
					sendNibble(0x03,GLCD_INSTRUCTION);
					DELAY_MS(1);
					sendNibble(0x03,GLCD_INSTRUCTION);
					DELAY_MS(1);
					sendNibble(0x02,GLCD_INSTRUCTION);
					DELAY_MS(1);
		
				#elif (GLCD_DATA_PORT_MASK == 0xF0)
		
					/*------------RESET THE GLCD------------*/
		
					sendNibble((0x03<<4),GLCD_INSTRUCTION);
					DELAY_MS(1);
					sendNibble((0x03<<4),GLCD_INSTRUCTION);
					DELAY_MS(1);
					sendNibble((0x03<<4),GLCD_INSTRUCTION);
					DELAY_MS(1);
					sendNibble((0x02<<4),GLCD_INSTRUCTION);
					DELAY_MS(1);

				#endif
		
			#elif (GLCD_DATA_MODE == 8)
		
				/*------------RESET THE GLCD------------*/
		
				sendNibble(0x03,GLCD_INSTRUCTION);
				DELAY_MS(1);
				sendNibble(0x03,GLCD_INSTRUCTION);
				DELAY_MS(1);
				sendNibble(0x03,GLCD_INSTRUCTION);
				DELAY_MS(1);
		
			#endif
		
		#elif (GLCD_INTERFACE == 0)
		
			/*------------RESET THE GLCD------------*/
		
			sendNibble(0x03,GLCD_INSTRUCTION);
			DELAY_MS(1);
			sendNibble(0x03,GLCD_INSTRUCTION);
			DELAY_MS(1);
			sendNibble(0x03,GLCD_INSTRUCTION);
			DELAY_MS(1);
		
		#endif
	
	#endif	
	 
 }
 
 static void sendNibble(ubyte_t a_data, glcdtransmissiontype_t a_transType){
	
	#if (GLCD_INTERFACE == 1)
	
		//------------SEND A NIBBLE------------//
	
		if(a_transType == GLCD_INSTRUCTION){
		
			gpio_setPin(GLCD_EN_PIN);								// enable GLCD interface for new data (EN signal), Command register is selected
			DELAY_US(800);
			gpio_setPort(GLCD_DATA_PORT,(a_data & GLCD_DATA_PORT_MASK));		// send a nibble of command
			DELAY_US(800);
			gpio_clearPin(GLCD_EN_PIN);							// clear control port
			DELAY_US(200);
			gpio_clearPort(GLCD_DATA_PORT,(a_data & GLCD_DATA_PORT_MASK));		// clear data port
			DELAY_US(200);
		
		}else if(a_transType == GLCD_DATA){
		
			gpio_setPin(GLCD_RS_PIN);								// select Data register (RS signal)
			DELAY_US(800);
			gpio_setPin(GLCD_EN_PIN);								// enable GLCD interface for new data (EN signal)
			DELAY_US(800);
			gpio_setPort(GLCD_DATA_PORT,(a_data & GLCD_DATA_PORT_MASK));		// send a nibble of data
			DELAY_US(800);
			gpio_clearPin(GLCD_EN_PIN);			// clear EN pin
			gpio_clearPin(GLCD_RS_PIN);			// clear RS pin
			DELAY_US(200);
			gpio_clearPort(GLCD_DATA_PORT,(a_data & GLCD_DATA_PORT_MASK));		// clear data port
			DELAY_US(200);
		
		}
	
	#elif (GLCD_INTERFACE == 0)	
	
		gpio_setPin(GLCD_RS_PIN);								// select GLCD chip (SS signal)
		spi_transreceive(a_transType? 0xFA : 0xF8);
		spi_transreceive(a_data&0xF0);
		spi_transreceive((a_data<<4)&0xF0);
		gpio_clearPin(GLCD_RS_PIN);								// release GLCD chip (SS signal)
	
	#endif
	 
 }
 
 static ubyte_t readNibble(void){
	 
	 #ifdef GLCD_RW_PIN
	 
		 ubyte_t data;
	 
		 gpio_setPortDirection(GLCD_DATA_PORT,GLCD_DATA_PORT_MASK,IO_INPUT);
	 
		 gpio_setPin(GLCD_RS_PIN);										// select Data register (RS signal)
		 gpio_setPin(GLCD_RW_PIN);										// select Data register (RW signal)
		 DELAY_US(800);
		 gpio_setPin(GLCD_EN_PIN);										// enable GLCD interface for new data (EN signal)
		 DELAY_US(800);
		 data = (gpio_readPort(GLCD_DATA_PORT)&GLCD_DATA_PORT_MASK);		// read nibble of data
		 DELAY_US(800);
		 gpio_clearPin(GLCD_EN_PIN);										// clear EN pin
		 gpio_clearPin(GLCD_RW_PIN);										// clear RW pin
		 gpio_clearPin(GLCD_RS_PIN);										// clear RS pin
		 DELAY_US(200);
	 
		 gpio_setPortDirection(GLCD_DATA_PORT,GLCD_DATA_PORT_MASK,IO_OUTPUT);
	 
		 return data;
	 
	 #endif
	 
	 return 0;
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
	
	GLCD_setCursorPosition(0,0);
	
	#if (GLCD_ROWS == 4)
		
		GLCD_puts(g_GLCD.frameBuffer[0]);
		GLCD_puts(g_GLCD.frameBuffer[2]);
		GLCD_puts(g_GLCD.frameBuffer[1]);
		GLCD_puts(g_GLCD.frameBuffer[3]);
		
	#else	
	
		ubyte_t rows;
		
		for(rows = 0; rows < GLCD_ROWS; rows++)
			GLCD_puts(g_GLCD.frameBuffer[rows]);
	
	#endif
	
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
	 
	 GLCD_sendInstruction(GLCD_DISPLAY_ON); 
	 
 }
 
 static void saveCursorConfig(bool_t a_cursorVisible, bool_t a_cursorBlinking){
	 
	 g_GLCD.statusRegister.value &= ~0x03;
	 g_GLCD.statusRegister.cursorvisible = a_cursorVisible;
	 g_GLCD.statusRegister.cursorblinking = a_cursorBlinking;
	 
 }
 
 static void restoreCursorConfig(void){
	 
	 GLCD_sendInstruction(GLCD_DISPLAY_ON|(g_GLCD.statusRegister.cursorvisible<<1)|g_GLCD.statusRegister.cursorblinking);
	 
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