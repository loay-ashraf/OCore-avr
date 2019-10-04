/*
 * ST7920.c
 *
 * Created: 25/09/2019 07:37:20 PM
 *  Author: Loay Ashraf
 */ 

#include "ST7920.h"
#include "ST7920_config.h"
#include "ST7920_fonts.h"
#include "hal/ecu/glcd/common/glcd_config.h"
#include "hal/mcu/hw/driver/gpio/gpio.h"
#include "hal/mcu/hw/driver/spi/spi.h"
#include "hal/mcu/sys/delay.h"
#include <avr/pgmspace.h>
#include <math.h>
#include <stdlib.h>

static uint8_t g_pixelX,g_pixelY;
static glcdfont_t g_font;

static void _resetDisplay(void);
static void _sendNibble(ubyte_t a_data, st7920transmissiontype_t a_transType);
static ubyte_t _readNibble(void);
static void _putc(char a_char);
static void _putw(uword_t a_word);
#ifdef ST7920_RW_PIN
static uword_t _getw(void);
#endif
static void _enableGraphics(void);
static uint16_t _map(uint8_t a_input, uint16_t a_inputMin, uint16_t a_inputMax, uint16_t a_outputMin, uint16_t a_outputMax);

void ST7920_init(bool_t a_backlightON){
	
	gpio_setPinDirection(ST7920_PSB_PIN,IO_OUTPUT);
	gpio_setPinDirection(ST7920_RS_PIN,IO_OUTPUT);
	
	#ifdef ST7920_BL_PIN
	
		gpio_setPinDirection(ST7920_BL_PIN,IO_OUTPUT);
		
		ST7920_configBacklight(a_backlightON);
	
	#endif
	
	#ifdef ST7920_RST_PIN
	
		gpio_setPinDirection(ST7920_RST_PIN,IO_OUTPUT);
		gpio_setPin(ST7920_RST_PIN);
	
	#endif
	
	#if (ST7920_INTERFACE == 1)
	
		gpio_setPin(ST7920_PSB_PIN);
		
		#ifdef ST7920_RW_PIN
		
			gpio_setPinDirection(ST7920_RW_PIN,IO_OUTPUT);
		
		#endif
		
		gpio_setPinDirection(ST7920_EN_PIN,IO_OUTPUT);								
		gpio_setPortDirection(ST7920_DATA_PORT,ST7920_DATA_PORT_MASK,IO_OUTPUT);		// set data port direction register
	
	#elif (ST7920_INTERFACE == 0)
	
		gpio_clearPin(ST7920_PSB_PIN);
		spi_enable();
	
	#endif
	
	_resetDisplay();
	
	#if (ST7920_INTERFACE == 1)
	
		#if (ST7920_DATA_MODE == 4)
	
			//------------CONFIGURE ST7920 BEHAVIOUR------------//
			ST7920_sendInstruction(ST7920_4BIT_MODE);									// 4-bit interface, 2-line mode, 5x8 dots format
			ST7920_sendInstruction(ST7920_DISPLAY_ON);									// display ON, cursor OFF, blink OFF
			ST7920_sendInstruction(ST7920_ENTRY_MODE);								// cursor moves to the right, no display shift
			ST7920_sendInstruction(ST7920_CLEAR_DISPLAY);								// clear display
	
		#elif (ST7920_DATA_MODE == 8)
	
			//------------CONFIGURE ST7920 BEHAVIOUR------------//
	
			ST7920_sendInstruction(ST7920_8BIT_MODE);									// 8-bit interface, 2-line mode, 5x8 dots format
			ST7920_sendInstruction(ST7920_DISPLAY_ON);									// display ON, cursor OFF, blink OFF
			ST7920_sendInstruction(ST7920_ENTRY_MODE);								// cursor moves to the right, no display shift
			ST7920_sendInstruction(ST7920_CLEAR_DISPLAY);								// clear display
	
		#endif
	
	#elif (ST7920_INTERFACE == 0)
	
		//------------CONFIGURE ST7920 BEHAVIOUR------------//
		ST7920_sendInstruction(ST7920_8BIT_MODE);										// 8-bit interface, 2-line mode, 5x8 dots format
		ST7920_sendInstruction(ST7920_DISPLAY_ON);										// display ON, cursor OFF, blink OFF
		ST7920_sendInstruction(ST7920_ENTRY_MODE);									// cursor moves to the right, no display shift
		ST7920_sendInstruction(ST7920_CLEAR_DISPLAY);									// clear display
	
	#endif
	
	DELAY_MS(10);
	
	_enableGraphics();
	
	g_font = GLCD_5X8;
	
}

void ST7920_sendInstruction(ubyte_t a_instruction){
	
	#if (ST7920_INTERFACE == 1)
	
		#if (ST7920_DATA_MODE == 4)
	
			#if (ST7920_DATA_PORT_MASK == 0x0F)
	
				//------------SEND HIGH NIBBLE------------//
	
				_sendNibble((a_instruction>>4),ST7920_INSTRUCTION);

				//------------SEND LOW NIBBLE------------//

				_sendNibble(a_instruction,ST7920_INSTRUCTION);
	
			#elif (ST7920_DATA_PORT_MASK == 0xF0)
	
				//------------SEND HIGH NIBBLE------------//
	
				_sendNibble(a_instruction,ST7920_INSTRUCTION);

				//------------SEND LOW NIBBLE------------//

				_sendNibble((a_instruction<<4),ST7920_INSTRUCTION);
	
			#endif
	
		#elif (ST7920_DATA_MODE == 8)
	
			//------------SEND 8-BIT COMMAND------------//
	
			_sendNibble(a_instruction,ST7920_INSTRUCTION);
	
		#endif
	
	#elif (ST7920_INTERFACE == 0)
	
		_sendNibble(a_instruction,ST7920_INSTRUCTION);
	
	#endif
	
}

void ST7920_clearDisplay(void){

	ST7920_fillDisplay(0);		// clear display command
	
}

void ST7920_configBacklight(bool_t a_backlightON){
	
	#ifdef ST7920_BL_PIN
	
		#if (ST7920_BL_MODE == 1)
		
			if(a_backlightON)
			
				gpio_setPin(ST7920_BL_PIN);
			
			else
			
				gpio_clearPin(ST7920_BL_PIN);
		
		#elif (ST7920_BL_MODE == 0)
		
			if(a_backlightON)
			
				gpio_clearPin(ST7920_BL_PIN);
			
			else
			
				gpio_setPin(ST7920_BL_PIN);
		
		#endif
	
	#endif
	
}

void ST7920_setCursorPosition(uint8_t a_x, uint8_t a_y){
	
	if(a_y > GLCD_Y_PIXELS-1)
		a_y = GLCD_Y_PIXELS-1;
	if(a_x > (GLCD_X_PIXELS/ST7920_X_PAGE_SIZE*2)-1)
		a_x = (GLCD_X_PIXELS/ST7920_X_PAGE_SIZE*2)-1;	
	
	if(g_pixelX != a_x)	
		g_pixelX = a_x;
	if(g_pixelY != a_y)	
		g_pixelY = a_y;	
	
	if(a_y < 32){
		
		ST7920_sendInstruction(0x80+a_y);
		ST7920_sendInstruction(0x80+a_x);
		
	}else{
		
		ST7920_sendInstruction(0x80+(a_y-32));
		ST7920_sendInstruction(0x88+a_x);
		
	}
	
}

void ST7920_setFont(glcdfont_t a_font){
	
	g_font = a_font;
	
}

void ST7920_scrollDisplay(glcddirection_t a_direction){
	
	if(a_direction == GLCD_LEFT || a_direction == GLCD_RIGHT);
	
		ST7920_sendInstruction(ST7920_SCROLL_DISPLAY|(a_direction<<2));
	
}

void ST7920_putc(char a_char){
	
	uint8_t y,y_end;
	
	a_char -= 32;
	y_end = g_pixelY+8;
	
	for(y=g_pixelY;y<y_end;y++){
		
		ST7920_setCursorPosition(g_pixelX,y);
		
		if(g_font == GLCD_5X8)
			_putc(pgm_read_byte(&font_5x8[(uint8_t)a_char][8-(y_end-y)]));
		
	}
	
}

void ST7920_puts(const char * a_data){
	
	uint8_t y,y_end;
	char * charPointer;

	y_end = g_pixelY+8;
	
	for(y=g_pixelY;y<y_end;y++){
		
		charPointer = (char *)a_data;
		ST7920_setCursorPosition(g_pixelX,y);
		
		if(g_font == GLCD_5X8){
		
			while(*charPointer != '\0')
				_putc(pgm_read_byte(&font_5x8[(uint8_t)((*charPointer++)-32)][8-(y_end-y)]));
		
		}
	}
	
}

char ST7920_getc(void){

	char data = 0;
	
	#if (ST7920_DATA_MODE == 4)

		#if (ST7920_DATA_PORT_MASK == 0x0F)

			//------------READ HIGH NIBBLE------------//

			data |= (_readNibble()<<4);

			//------------READ LOW NIBBLE------------//

			data |= _readNibble();

		#elif (ST7920_DATA_PORT_MASK == 0xF0)

			//------------READ HIGH NIBBLE------------//

			data |= _readNibble();

			//------------READ LOW NIBBLE------------//

			data |= (_readNibble()>>4);

		#endif

	#elif (ST7920_DATA_MODE == 8)

		//------------READ 8-BIT DATA------------//

		data |= _readNibble();

	#endif
	
	return data;

}

void ST7920_drawPixel(uint8_t a_x, uint8_t a_y){
	
	#ifdef ST7920_RW_PIN
		
		uint8_t bitNum,pageNum;
		uword_t currentData;
		
		pageNum = a_x/16.0;
		bitNum = a_x-(pageNum*16);
		
		ST7920_setCursorPosition(pageNum,a_y);
		currentData = _getw();
		ST7920_setCursorPosition(pageNum,a_y);
		_putw(currentData|(1<<(15-bitNum)));
	
	#endif	
		 
}

void ST7920_drawVerticalBar(glcdbarindex_t a_barIndex, uint8_t a_value, uint8_t a_minValue, uint8_t a_maxValue){
	
	int8_t counter;
	uint8_t stopY = (45-_map(a_value,a_minValue,a_maxValue,0,45))+18;
	char buffer[5];
	
	for(counter=63;counter>=stopY;counter--){
		
		ST7920_setCursorPosition(a_barIndex*2,counter);
		_putw(0x00FF);
		_putw(0xFF00);
		
	}
	
	for(;counter>=0;counter--){
		
		ST7920_setCursorPosition(a_barIndex*2,counter);
		_putw(0);
		_putw(0);
		
	}
	
	itoa(a_value,buffer,10);
	
	buffer[4] = '\0';
	buffer[3] = buffer[2];
	buffer[2] = buffer[1];
	buffer[1] = buffer[0];
	buffer[0] = ' ';
	
	ST7920_setCursorPosition(a_barIndex*2,5);
	ST7920_puts(buffer);
	
}

void ST7920_drawHorizontalBar(glcdbarindex_t a_barIndex, uint8_t a_value, uint8_t a_minValue, uint8_t a_maxValue){
	
	int8_t x,y;
	uint8_t stopX = _map(a_value,a_minValue,a_maxValue,0,5);
	uint8_t stopY = (a_barIndex*16)+15;
	char buffer[5];
	
	for(y=a_barIndex*16;y<=stopY;y++){
		
		ST7920_setCursorPosition(0,y);
		
		for(x=0;x<=stopX;x++){
			
			if(y<(a_barIndex*16)+4)
				_putw(0);
			else if(y>(a_barIndex*16)+11)
				_putw(0);
			else
				_putw(0xFFFF);
			
		}
		
		for(;x<=7;x++)
			_putw(0);	
		
	}
	
	itoa(a_value,buffer,10);
	
	ST7920_setCursorPosition(6,(a_barIndex*16)+4);
	ST7920_puts(buffer);
	
}

void ST7920_fillDisplay(ubyte_t a_pattern){
	
	uint8_t x,y;
	
	for(y=0;y<64;y++){
		
		ST7920_setCursorPosition(0,y);
		
		for(x=0;x<8;x++){
			
			_putc(a_pattern);
			_putc(a_pattern);
			
		}
	}	
}

void ST7920_putImageRAM(const ubyte_t * a_image){
	
	uint8_t x,y;
	
	for(y=0;y<64;y++){
		
		ST7920_setCursorPosition(0,y);
		
		for(x=0;x<8;x++){
			
			_putc(a_image[(2*x)+(16*y)]);
			_putc(a_image[(2*x)+1+(16*y)]);
			
		}
	}
}

void ST7920_putImageROM(const ubyte_t * a_image){
	
	uint8_t x,y;
	uint8_t byte;
	
	for(y=0;y<64;y++){
		
		ST7920_setCursorPosition(0,y);
		
		for(x=0;x<8;x++){
			
			byte = pgm_read_byte(&a_image[(2*x)+(16*y)]);
			_putc(byte);
			
			byte = pgm_read_byte(&a_image[(2*x)+(16*y)+1]);
			_putc(byte);
			
		}
	}
}

static void _resetDisplay(void){
	
	#if (ST7920_INTERFACE == 1)
	
		#if (ST7920_DATA_MODE == 4)
	
			#if (ST7920_DATA_PORT_MASK == 0x0F)
	
				/*------------RESET THE ST7920------------*/
	
				_sendNibble(0x03,ST7920_INSTRUCTION);
				DELAY_MS(1);
				_sendNibble(0x03,ST7920_INSTRUCTION);
				DELAY_MS(1);
				_sendNibble(0x03,ST7920_INSTRUCTION);
				DELAY_MS(1);
				_sendNibble(0x02,ST7920_INSTRUCTION);
				DELAY_MS(1);
	
			#elif (ST7920_DATA_PORT_MASK == 0xF0)
	
				/*------------RESET THE ST7920------------*/
	
				_sendNibble((0x03<<4),ST7920_INSTRUCTION);
				DELAY_MS(1);
				_sendNibble((0x03<<4),ST7920_INSTRUCTION);
				DELAY_MS(1);
				_sendNibble((0x03<<4),ST7920_INSTRUCTION);
				DELAY_MS(1);
				_sendNibble((0x02<<4),ST7920_INSTRUCTION);
				DELAY_MS(1);

			#endif
	
		#elif (ST7920_DATA_MODE == 8)
	
			/*------------RESET THE ST7920------------*/
	
			_sendNibble(0x03,ST7920_INSTRUCTION);
			DELAY_MS(1);
			_sendNibble(0x03,ST7920_INSTRUCTION);
			DELAY_MS(1);
			_sendNibble(0x03,ST7920_INSTRUCTION);
			DELAY_MS(1);
	
		#endif
	
	#elif (ST7920_INTERFACE == 0)
	
		/*------------RESET THE ST7920------------*/
	
		_sendNibble(0x03,ST7920_INSTRUCTION);
		DELAY_MS(1);
		_sendNibble(0x03,ST7920_INSTRUCTION);
		DELAY_MS(1);
		_sendNibble(0x03,ST7920_INSTRUCTION);
		DELAY_MS(1);
	
	#endif
	
}

static void _sendNibble(ubyte_t a_data, st7920transmissiontype_t a_transType){
	
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
		DELAY_US(350);
		spi_transmitByte(a_transType? 0xFA : 0xF8);
		spi_transmitByte(a_data&0xF0);
		spi_transmitByte((a_data<<4)&0xF0);
		DELAY_US(350);
		gpio_clearPin(ST7920_RS_PIN);								// release ST7920 chip (SS signal)
	
	#endif
	
}

static ubyte_t _readNibble(void){
	
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

static void _putc(char a_char){
	
	#if (ST7920_INTERFACE == 1)
	
		#if (ST7920_DATA_MODE == 4)
	
			#if (ST7920_DATA_PORT_MASK == 0x0F)
	
				//------------SEND HIGH NIBBLE------------//
	
				_sendNibble((a_char>>4),ST7920_DATA);

				//------------SEND LOW NIBBLE------------//

				_sendNibble(a_char,ST7920_DATA);
	
			#elif (ST7920_DATA_PORT_MASK == 0xF0)
	
				//------------SEND HIGH NIBBLE------------//
	
				_sendNibble(a_char,ST7920_DATA);

				//------------SEND LOW NIBBLE------------//

				_sendNibble((a_char<<4),ST7920_DATA);
	
			#endif
	
		#elif (ST7920_DATA_MODE == 8)
	
			//------------SEND 8-BIT DATA------------//
	
			_sendNibble(a_char,ST7920_DATA);
	
		#endif
	
	#elif (ST7920_INTERFACE == 0)
	
		_sendNibble(a_char,ST7920_DATA);
	
	#endif
	
}

static void _putw(uword_t a_word){
	
	_putc((a_word>>8)&0xFF);
	_putc(a_word&0xFF);
	
}

#ifdef ST7920_RW_PIN
static uword_t _getw(void){
	
	ubyte_t byte = 0;
	uword_t word = 0;
	byte = ST7920_getc();		// dummy read
	byte = ST7920_getc();
	word |= ((byte<<8)&0xFF00);
	byte = ST7920_getc();
	word |= (byte&0x00FF);
	
	return word;
	
}
#endif

static void _enableGraphics(void){
	
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
	
	ST7920_clearDisplay();
	
}

static uint16_t _map(uint8_t a_input, uint16_t a_inputMin, uint16_t a_inputMax, uint16_t a_outputMin, uint16_t a_outputMax){
	
	if(a_input < a_inputMin)
		a_input = a_inputMin;
	else if(a_input > a_inputMax)
		a_input = a_inputMax;
	
	float slope = (float)(a_outputMax-a_outputMin)/(float)(a_inputMax-a_inputMin);
	uint16_t output = a_outputMin+slope*(a_input-a_inputMin);
	
	return output;
}