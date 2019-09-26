/*
 * ST7920.c
 *
 * Created: 25/09/2019 07:37:20 PM
 *  Author: Loay Ashraf
 */ 

#include "ST7920.h"
#include "ST7920_config.h"
#include "hal/mcu/hw/driver/gpio/gpio.h"
#include "hal/mcu/hw/driver/spi/spi.h"
#include "hal/mcu/sys/delay.h"
#include <avr/pgmspace.h>
#include <math.h>

static void putw(uword_t a_word);
static uword_t readWord(void);
static void resetDisplay(void);
static void sendNibble(ubyte_t a_data, st7920transmissiontype_t a_transType);
static ubyte_t readNibble(void);


void ST7920_init(bool_t a_cursorVisible, bool_t a_cursorBlinking){
	
	#ifdef ST7920_RW_PIN
	
		gpio_setPinDirection(ST7920_RW_PIN,IO_OUTPUT);
	
	#endif
	
	#ifdef ST7920_RST_PIN
	
		gpio_setPinDirection(ST7920_RST_PIN,IO_OUTPUT);
		gpio_setPin(ST7920_RST_PIN);
	
	#endif
	
	gpio_setPinDirection(ST7920_PSB_PIN,IO_OUTPUT);
	gpio_setPinDirection(ST7920_EN_PIN,IO_OUTPUT);										// set control port direction register
	gpio_setPinDirection(ST7920_RS_PIN,IO_OUTPUT);
	
	#if (ST7920_INTERFACE == 1)
	
		gpio_setPin(ST7920_PSB_PIN);
		gpio_setPortDirection(ST7920_DATA_PORT,ST7920_DATA_PORT_MASK,IO_OUTPUT);		// set data port direction register
	
	#elif (ST7920_INTERFACE == 0)
	
		gpio_clearPin(ST7920_PSB_PIN);
		spi_enable();
	
	#endif
	
	resetDisplay();
	
	#if (ST7920_INTERFACE == 1)
	
		#if (ST7920_DATA_MODE == 4)
	
			//------------CONFIGURE ST7920 BEHAVIOUR------------//
			ST7920_sendInstruction(ST7920_4BIT_MODE);												// 4-bit interface, 2-line mode, 5x8 dots format
			ST7920_sendInstruction(ST7920_DISPLAY_ON|(a_cursorVisible<<1)|a_cursorBlinking);		// display ON, cursor OFF, blink OFF
			ST7920_sendInstruction(ST7920_CURSOR_RIGHT);											// cursor moves to the right, no display shift
			ST7920_sendInstruction(ST7920_CLEAR_DISPLAY);											// clear display
	
		#elif (ST7920_DATA_MODE == 8)
	
			//------------CONFIGURE ST7920 BEHAVIOUR------------//
	
			ST7920_sendInstruction(ST7920_8BIT_MODE);												// 8-bit interface, 2-line mode, 5x8 dots format
			ST7920_sendInstruction(ST7920_DISPLAY_ON|(a_cursorVisible<<1)|a_cursorBlinking);		// display ON, cursor OFF, blink OFF
			ST7920_sendInstruction(ST7920_CURSOR_RIGHT);											// cursor moves to the right, no display shift
			ST7920_sendInstruction(ST7920_CLEAR_DISPLAY);											// clear display
	
		#endif
	
	#elif (ST7920_INTERFACE == 0)
	
		//------------CONFIGURE ST7920 BEHAVIOUR------------//
		ST7920_sendInstruction(ST7920_8BIT_MODE);												// 8-bit interface, 2-line mode, 5x8 dots format
		ST7920_sendInstruction(ST7920_DISPLAY_ON|(a_cursorVisible<<1)|a_cursorBlinking);		// display ON, cursor OFF, blink OFF
		ST7920_sendInstruction(ST7920_CURSOR_RIGHT);											// cursor moves to the right, no display shift
		ST7920_sendInstruction(ST7920_CLEAR_DISPLAY);											// clear display
	
	#endif
	
	DELAY_MS(10);
	
}

void ST7920_enableGraphics(void){
	
	DELAY_MS(1); 
	
	#if (ST7920_INTERFACE == 1)
	
		#if (ST7920_DATA_MODE == 4)
	
			//------------CONFIGURE ST7920 BEHAVIOUR------------//
			ST7920_sendInstruction(ST7920_4BIT_MODE);
			DELAY_MS(1);
			ST7920_sendInstruction(ST7920_4BIT_MODE|ST7920_EXT_INSTRUCTION);
			DELAY_MS(1);
			ST7920_sendInstruction(ST7920_4BIT_MODE|ST7920_GRAPHICS_ON);
			DELAY_MS(1);
			ST7920_sendInstruction(ST7920_CLEAR_DISPLAY);
	
		#elif (ST7920_DATA_MODE == 8)
	
			//------------CONFIGURE ST7920 BEHAVIOUR------------//
	
			ST7920_sendInstruction(ST7920_8BIT_MODE);
			DELAY_MS(1);
			ST7920_sendInstruction(ST7920_8BIT_MODE|ST7920_EXT_INSTRUCTION);
			DELAY_MS(1);
			ST7920_sendInstruction(ST7920_8BIT_MODE|ST7920_GRAPHICS_ON);
			DELAY_MS(1);
			ST7920_sendInstruction(ST7920_CLEAR_DISPLAY);
	
		#endif
	
	#elif (ST7920_INTERFACE == 0)
	
		ST7920_sendInstruction(ST7920_8BIT_MODE);
		DELAY_MS(1);
		ST7920_sendInstruction(ST7920_8BIT_MODE|ST7920_EXT_INSTRUCTION);
		DELAY_MS(1);
		ST7920_sendInstruction(ST7920_8BIT_MODE|ST7920_GRAPHICS_ON);
		DELAY_MS(1);
		ST7920_sendInstruction(ST7920_CLEAR_DISPLAY);
	
	#endif
	
}

void ST7920_disableGraphics(void){
	
	#if (ST7920_INTERFACE == 1)
	
		#if (ST7920_DATA_MODE == 4)
	
			//------------CONFIGURE ST7920 BEHAVIOUR------------//

			ST7920_sendInstruction(ST7920_4BIT_MODE|ST7920_EXT_INSTRUCTION);
			DELAY_MS(1);
			ST7920_sendInstruction(ST7920_4BIT_MODE);
			DELAY_MS(1);
	
		#elif (ST7920_DATA_MODE == 8)
	
			//------------CONFIGURE ST7920 BEHAVIOUR------------//
	
			ST7920_sendInstruction(ST7920_8BIT_MODE|ST7920_EXT_INSTRUCTION);
			DELAY_MS(1);
			ST7920_sendInstruction(ST7920_8BIT_MODE);
			DELAY_MS(1);
	
		#endif
	
	#elif (ST7920_INTERFACE == 0)
	
		ST7920_sendInstruction(ST7920_8BIT_MODE|ST7920_EXT_INSTRUCTION);
		DELAY_MS(1);
		ST7920_sendInstruction(ST7920_8BIT_MODE);
		DELAY_MS(1);
	
	#endif
	
}


void ST7920_sendInstruction(ubyte_t a_instruction){
	
	#if (ST7920_INTERFACE == 1)
	
		#if (ST7920_DATA_MODE == 4)
	
			#if (ST7920_DATA_PORT_MASK == 0x0F)
	
				//------------SEND HIGH NIBBLE------------//
	
				sendNibble((a_instruction>>4),ST7920_INSTRUCTION);

				//------------SEND LOW NIBBLE------------//

				sendNibble(a_instruction,ST7920_INSTRUCTION);
	
			#elif (ST7920_DATA_PORT_MASK == 0xF0)
	
				//------------SEND HIGH NIBBLE------------//
	
				sendNibble(a_instruction,ST7920_INSTRUCTION);

				//------------SEND LOW NIBBLE------------//

				sendNibble((a_instruction<<4),ST7920_INSTRUCTION);
	
			#endif
	
		#elif (ST7920_DATA_MODE == 8)
	
			//------------SEND 8-BIT COMMAND------------//
	
			sendNibble(a_instruction,ST7920_INSTRUCTION);
	
		#endif
	
	#elif (ST7920_INTERFACE == 0)
	
		sendNibble(a_instruction,ST7920_INSTRUCTION);
	
	#endif
	
}

void ST7920_clearDisplay(void){

	ST7920_sendInstruction(ST7920_CLEAR_DISPLAY);	// clear display command
	
}

void ST7920_configCursor(bool_t a_cursorVisible, bool_t a_cursorBlinking){
	
	ST7920_sendInstruction(ST7920_DISPLAY_ON|(a_cursorVisible<<1)|a_cursorBlinking);		// configure cursor
	
}

void ST7920_setCursorPosition(ubyte_t a_row, ubyte_t a_col){
	
	switch(a_row){

		case 0: ST7920_sendInstruction(ST7920_ROW_ZERO+a_col);	// first row
		break;
		case 1: ST7920_sendInstruction(ST7920_ROW_ONE+a_col);	// second row
		break;
		case 2: ST7920_sendInstruction(ST7920_ROW_TWO+a_col);	// third row
		break;
		case 3: ST7920_sendInstruction(ST7920_ROW_THREE+a_col);	// fourth row
		break;
		default: ST7920_sendInstruction(ST7920_ROW_ZERO+a_col);	// default is first row
		break;

	}
	
}

void ST7920_setPixelCursorPosition(ubyte_t a_row, ubyte_t a_col){
	
	if(a_row > 63)
		a_row = 63;
	if(a_col > 7)
		a_col = 7;	
	
	if(a_row < 32){
		
		ST7920_sendInstruction(0x80+a_row);
		ST7920_sendInstruction(0x80+a_col);
		
	}else{
		
		ST7920_sendInstruction(0x80+(a_row-32));
		ST7920_sendInstruction(0x88+a_col);
		
	}
	
}

void ST7920_shiftCursor(bool_t a_dir){
	
	ST7920_sendInstruction(ST7920_SHIFT_CURSOR|(a_dir<<2));
	
}

void ST7920_scrollDisplay(bool_t a_dir){
	
	ST7920_sendInstruction(ST7920_SCROLL_DISPLAY|(a_dir<<2));
	
}

void ST7920_defineCustomCharacter(ubyte_t a_characterIndex, uword_t a_characterArray[16]){
	
	ST7920_sendInstruction(0x40+(a_characterIndex*8));
	putw(a_characterArray[0]); // 0
	putw(a_characterArray[1]); // 1
	putw(a_characterArray[2]); // 2
	putw(a_characterArray[3]); // 3
	putw(a_characterArray[4]); // 4
	putw(a_characterArray[5]); // 5
	putw(a_characterArray[6]); // 6
	putw(a_characterArray[7]); // 7
	putw(a_characterArray[8]); // 8
	putw(a_characterArray[9]); // 9
	putw(a_characterArray[10]); // A
	putw(a_characterArray[11]); // B
	putw(a_characterArray[12]); // C
	putw(a_characterArray[13]); // D
	putw(a_characterArray[14]); // E
	putw(a_characterArray[15]); // F
	
}

void ST7920_putc(char a_char){
	
	#if (ST7920_INTERFACE == 1)
	
		#if (ST7920_DATA_MODE == 4)
	
			#if (ST7920_DATA_PORT_MASK == 0x0F)
	
				//------------SEND HIGH NIBBLE------------//
	
				sendNibble((a_char>>4),ST7920_DATA);

				//------------SEND LOW NIBBLE------------//

				sendNibble(a_char,ST7920_DATA);
	
			#elif (ST7920_DATA_PORT_MASK == 0xF0)
	
				//------------SEND HIGH NIBBLE------------//
	
				sendNibble(a_char,ST7920_DATA);

				//------------SEND LOW NIBBLE------------//

				sendNibble((a_char<<4),ST7920_DATA);
	
			#endif
	
		#elif (ST7920_DATA_MODE == 8)
	
			//------------SEND 8-BIT DATA------------//
	
			sendNibble(a_char,ST7920_DATA);
	
		#endif
	
	#elif (ST7920_INTERFACE == 0)
	
		sendNibble(a_char,ST7920_DATA);
	
	#endif
	
}

char ST7920_readChar(void){

	char data = 0;
	
	#if (ST7920_DATA_MODE == 4)

		#if (ST7920_DATA_PORT_MASK == 0x0F)

			//------------READ HIGH NIBBLE------------//

			data |= (readNibble()<<4);

			//------------READ LOW NIBBLE------------//

			data |= readNibble();

		#elif (ST7920_DATA_PORT_MASK == 0xF0)

			//------------READ HIGH NIBBLE------------//

			data |= readNibble();

			//------------READ LOW NIBBLE------------//

			data |= (readNibble()>>4);

		#endif

	#elif (ST7920_DATA_MODE == 8)

		//------------READ 8-BIT DATA------------//

		data |= readNibble();

	#endif
	
	return data;

}

void ST7920_drawPixel(uint8_t a_x, uint8_t a_y){
	
	#ifdef ST7920_RW_PIN
		
		uint8_t bitNum,pageNum;
		uword_t currentData;
		
		pageNum = a_x/16.0;
		bitNum = a_x - (pageNum*16);
		
		ST7920_setPixelCursorPosition(a_y,pageNum);
		currentData = readWord();
		ST7920_setPixelCursorPosition(a_y,pageNum);
		putw(currentData|(1<<(15-bitNum)));
	
	#endif	
		 
}

void ST7920_fillDisplay(uword_t a_pattern){
	
	uint8_t x,y;
	
	for(y=0;y<64;y++){
		
		ST7920_setPixelCursorPosition(y,0);
		
		for(x=0;x<8;x++){
			
			putw(a_pattern);
			
		}
	}	
}

void ST7920_putImageRAM(const ubyte_t * a_image){
	
	uint8_t x,y;
	
	for(y=0;y<64;y++){
		
		ST7920_setPixelCursorPosition(y,0);
		
		for(x=0;x<8;x++){
			
			ST7920_putc(a_image[(2*x)+(16*y)]);
			ST7920_putc(a_image[(2*x)+1+(16*y)]);
			
		}
	}
}

void ST7920_putImageROM(const ubyte_t * a_image){
	
	uint8_t x,y;
	uint8_t byte;
	
	for(y=0;y<64;y++){
		
		ST7920_setPixelCursorPosition(y,0);
		
		for(x=0;x<8;x++){
			
			byte = pgm_read_byte(&a_image[(2*x)+(16*y)]);
			ST7920_putc(byte);
			
			byte = pgm_read_byte(&a_image[(2*x)+(16*y)+1]);
			ST7920_putc(byte);
			
		}
	}
}

static void putw(uword_t a_word){
	
	ST7920_putc((a_word>>8)&0xFF);
	ST7920_putc(a_word&0xFF);
	
}

static uword_t readWord(void){
	
	ubyte_t byte = 0;
	uword_t word = 0;
	byte = ST7920_readChar();		// dummy read
	byte = ST7920_readChar();
	word |= ((byte<<8)&0xFF00);
	byte = ST7920_readChar();
	word |= (byte&0x00FF);
	
	return word;
	
}

static void resetDisplay(void){
	
	#if (ST7920_INTERFACE == 1)
	
		#if (ST7920_DATA_MODE == 4)
	
			#if (ST7920_DATA_PORT_MASK == 0x0F)
	
				/*------------RESET THE ST7920------------*/
	
				sendNibble(0x03,ST7920_INSTRUCTION);
				DELAY_MS(1);
				sendNibble(0x03,ST7920_INSTRUCTION);
				DELAY_MS(1);
				sendNibble(0x03,ST7920_INSTRUCTION);
				DELAY_MS(1);
				sendNibble(0x02,ST7920_INSTRUCTION);
				DELAY_MS(1);
	
			#elif (ST7920_DATA_PORT_MASK == 0xF0)
	
				/*------------RESET THE ST7920------------*/
	
				sendNibble((0x03<<4),ST7920_INSTRUCTION);
				DELAY_MS(1);
				sendNibble((0x03<<4),ST7920_INSTRUCTION);
				DELAY_MS(1);
				sendNibble((0x03<<4),ST7920_INSTRUCTION);
				DELAY_MS(1);
				sendNibble((0x02<<4),ST7920_INSTRUCTION);
				DELAY_MS(1);

			#endif
	
		#elif (ST7920_DATA_MODE == 8)
	
			/*------------RESET THE ST7920------------*/
	
			sendNibble(0x03,ST7920_INSTRUCTION);
			DELAY_MS(1);
			sendNibble(0x03,ST7920_INSTRUCTION);
			DELAY_MS(1);
			sendNibble(0x03,ST7920_INSTRUCTION);
			DELAY_MS(1);
	
		#endif
	
	#elif (ST7920_INTERFACE == 0)
	
		/*------------RESET THE ST7920------------*/
	
		sendNibble(0x03,ST7920_INSTRUCTION);
		DELAY_MS(1);
		sendNibble(0x03,ST7920_INSTRUCTION);
		DELAY_MS(1);
		sendNibble(0x03,ST7920_INSTRUCTION);
		DELAY_MS(1);
	
	#endif
	
}

static void sendNibble(ubyte_t a_data, st7920transmissiontype_t a_transType){
	
	#if (ST7920_INTERFACE == 1)
	
		//------------SEND A NIBBLE------------//
	
		if(a_transType == ST7920_INSTRUCTION){
		
			gpio_setPin(ST7920_EN_PIN);								// enable ST7920 interface for new data (EN signal), Command register is selected
			DELAY_US(800);
			gpio_setPort(ST7920_DATA_PORT,(a_data & ST7920_DATA_PORT_MASK));		// send a nibble of command
			DELAY_US(800);
			gpio_clearPin(ST7920_EN_PIN);							// clear control port
			DELAY_US(200);
			gpio_clearPort(ST7920_DATA_PORT,(a_data & ST7920_DATA_PORT_MASK));		// clear data port
			DELAY_US(200);
		
		}else if(a_transType == ST7920_DATA){
		
			gpio_setPin(ST7920_RS_PIN);								// select Data register (RS signal)
			DELAY_US(800);
			gpio_setPin(ST7920_EN_PIN);								// enable ST7920 interface for new data (EN signal)
			DELAY_US(800);
			gpio_setPort(ST7920_DATA_PORT,(a_data & ST7920_DATA_PORT_MASK));		// send a nibble of data
			DELAY_US(800);
			gpio_clearPin(ST7920_EN_PIN);			// clear EN pin
			gpio_clearPin(ST7920_RS_PIN);			// clear RS pin
			DELAY_US(200);
			gpio_clearPort(ST7920_DATA_PORT,(a_data & ST7920_DATA_PORT_MASK));		// clear data port
			DELAY_US(200);
		
		}
	
	#elif (ST7920_INTERFACE == 0)
	
		gpio_setPin(ST7920_RS_PIN);								// select ST7920 chip (SS signal)
		spi_transreceive(a_transType? 0xFA : 0xF8);
		spi_transreceive(a_data&0xF0);
		spi_transreceive((a_data<<4)&0xF0);
		gpio_clearPin(ST7920_RS_PIN);								// release ST7920 chip (SS signal)
	
	#endif
	
}

static ubyte_t readNibble(void){
	
	#ifdef ST7920_RW_PIN
	
		ubyte_t data;
	
		gpio_setPortDirection(ST7920_DATA_PORT,ST7920_DATA_PORT_MASK,IO_INPUT);
	
		gpio_setPin(ST7920_RS_PIN);										// select Data register (RS signal)
		gpio_setPin(ST7920_RW_PIN);										// select Read mode (RW signal)
		DELAY_US(800);
		gpio_setPin(ST7920_EN_PIN);										// enable ST7920 interface for new data (EN signal)
		DELAY_US(800);
		data = (gpio_readPort(ST7920_DATA_PORT)&ST7920_DATA_PORT_MASK);		// read nibble of data
		DELAY_US(800);
		gpio_clearPin(ST7920_EN_PIN);										// clear EN pin
		gpio_clearPin(ST7920_RW_PIN);										// clear RW pin
		gpio_clearPin(ST7920_RS_PIN);										// clear RS pin
		DELAY_US(200);
	
		gpio_setPortDirection(ST7920_DATA_PORT,ST7920_DATA_PORT_MASK,IO_OUTPUT);
	
		return data;
	
	#else
	
		return 0;
		
	#endif	
	
}