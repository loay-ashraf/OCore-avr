/*
 * HD44780.c
 *
 * Created: 23/09/2019 05:13:56 PM
 *  Author: Loay Ashraf
 */ 

#include "HD44780.h"
#include "HD44780_config.h"
#include "hal/mcu/hw/driver/gpio/gpio.h"
#include "hal/mcu/sys/delay.h"

static void resetDisplay(void);
static void sendNibble(ubyte_t a_data, hd44780transmissiontype_t a_transType);
static ubyte_t readNibble(void);

void HD44780_init(bool_t a_cursorVisible, bool_t a_cursorBlinking){
	
	#ifdef HD44780_RW_PIN
	
		gpio_setPinDirection(HD44780_RW_PIN,IO_OUTPUT);
	
	#endif
	
	gpio_setPinDirection(HD44780_EN_PIN,IO_OUTPUT);							// set control port direction register
	gpio_setPinDirection(HD44780_RS_PIN,IO_OUTPUT);
	gpio_setPortDirection(HD44780_DATA_PORT,HD44780_DATA_PORT_MASK,IO_OUTPUT);			// set data port direction register
	
	resetDisplay();
	
	#if (HD44780_DATA_MODE == 4)
	
		//------------CONFIGURE HD44780 BEHAVIOUR------------//
	
		HD44780_sendInstruction(HD44780_4BIT_MODE);												// 4-bit interface, 2-line mode, 5x8 dots format
		HD44780_sendInstruction(HD44780_DISPLAY_ON|(a_cursorVisible<<1)|a_cursorBlinking);		// display ON, cursor OFF, blink OFF
		HD44780_sendInstruction(HD44780_CURSOR_RIGHT);											// cursor moves to the right, no display shift
		HD44780_sendInstruction(HD44780_CLEAR_DISPLAY);											// clear display
	
	#elif (HD44780_DATA_MODE == 8)
	
		//------------CONFIGURE HD44780 BEHAVIOUR------------//
	
		HD44780_sendInstruction(HD44780_8BIT_MODE);												// 8-bit interface, 2-line mode, 5x8 dots format
		HD44780_sendInstruction(HD44780_DISPLAY_ON|(a_cursorVisible<<1)|a_cursorBlinking);		// display ON, cursor OFF, blink OFF
		HD44780_sendInstruction(HD44780_CURSOR_RIGHT);											// cursor moves to the right, no display shift
		HD44780_sendInstruction(HD44780_CLEAR_DISPLAY);											// clear display
	
	#endif
	
	
}

void HD44780_sendInstruction(ubyte_t a_instruction){
	
	#if (HD44780_DATA_MODE == 4)
	
		#if (HD44780_DATA_PORT_MASK == 0x0F)
	
			//------------SEND HIGH NIBBLE------------//
	
			sendNibble((a_instruction>>4),HD44780_INSTRUCTION);

			//------------SEND LOW NIBBLE------------//

			sendNibble(a_instruction,HD44780_INSTRUCTION);
	
		#elif (HD44780_DATA_PORT_MASK == 0xF0)
	
			//------------SEND HIGH NIBBLE------------//
	
			sendNibble(a_instruction,HD44780_INSTRUCTION);

			//------------SEND LOW NIBBLE------------//

			sendNibble((a_instruction<<4),HD44780_INSTRUCTION);
	
		#endif
	
	#elif (HD44780_DATA_MODE == 8)
	
		//------------SEND 8-BIT COMMAND------------//
	
		sendNibble(a_instruction,HD44780_INSTRUCTION);
	
	#endif
	
}

void HD44780_clearDisplay(void){
	
	HD44780_sendInstruction(HD44780_CLEAR_DISPLAY);			// clear display command

}

void HD44780_configCursor(bool_t a_cursorVisible, bool_t a_cursorBlinking){

	HD44780_sendInstruction(HD44780_DISPLAY_ON|(a_cursorVisible<<1)|a_cursorBlinking);		// configure cursor

}

void HD44780_setCursorPosition(ubyte_t a_row, ubyte_t a_col){
	
	switch(a_row){

		case 0: HD44780_sendInstruction(HD44780_ROW_ZERO+a_col);	// first row
		break;
		case 1: HD44780_sendInstruction(HD44780_ROW_ONE+a_col);		// second row
		break;
		case 2: HD44780_sendInstruction(HD44780_ROW_TWO+a_col);		// third row
		break;
		case 3: HD44780_sendInstruction(HD44780_ROW_THREE+a_col);	// fourth row
		break;
		default: HD44780_sendInstruction(HD44780_ROW_ZERO+a_col);	// default is first row
		break;

	}
}

void HD44780_shiftCursor(bool_t a_dir){
	
	HD44780_sendInstruction(HD44780_SHIFT_CURSOR|(a_dir<<2));
	
}

void HD44780_scrollDisplay(bool_t a_dir){
	
	HD44780_sendInstruction(HD44780_SCROLL_DISPLAY|(a_dir<<2));
	
}

void HD44780_defineCustomCharacter(ubyte_t a_characterIndex, ubyte_t a_characterArray[8]){
	
	HD44780_sendInstruction(0x40+(a_characterIndex*8));
	HD44780_putc(a_characterArray[0]);
	HD44780_putc(a_characterArray[1]);
	HD44780_putc(a_characterArray[2]);
	HD44780_putc(a_characterArray[3]);
	HD44780_putc(a_characterArray[4]);
	HD44780_putc(a_characterArray[5]);
	HD44780_putc(a_characterArray[6]);
	HD44780_putc(a_characterArray[7]);
	
}

void HD44780_putc(char a_data){
	
	#if (HD44780_DATA_MODE == 4)
	
		#if (HD44780_DATA_PORT_MASK == 0x0F)
	
			//------------SEND HIGH NIBBLE------------//
	
			sendNibble((a_data>>4),HD44780_DATA);

			//------------SEND LOW NIBBLE------------//

			sendNibble(a_data,HD44780_DATA);
	
		#elif (HD44780_DATA_PORT_MASK == 0xF0)
	
			//------------SEND HIGH NIBBLE------------//
	
			sendNibble(a_data,HD44780_DATA);

			//------------SEND LOW NIBBLE------------//

			sendNibble((a_data<<4),HD44780_DATA);
	
		#endif
	
	#elif (HD44780_DATA_MODE == 8)
	
		//------------SEND 8-BIT COMMAND------------//
	
		sendNibble(a_data,HD44780_DATA);
	
	#endif
	
}

char HD44780_readChar(void){
	
	char data = 0;
	
	#if (HD44780_DATA_MODE == 4)
	
		#if (HD44780_DATA_PORT_MASK == 0x0F)
	
			//------------READ HIGH NIBBLE------------//
	
			data |= (readNibble()<<4);

			//------------READ LOW NIBBLE------------//

			data |= readNibble();
	
		#elif (HD44780_DATA_PORT_MASK == 0xF0)
	
			//------------READ HIGH NIBBLE------------//
	
			data |= readNibble();

			//------------READ LOW NIBBLE------------//

			data |= (readNibble()>>4);
	
		#endif
	
	#elif (HD44780_DATA_MODE == 8)
	
		//------------READ 8-BIT DATA------------//
	
		data |= readNibble();
	
	#endif

	return data;
}

static void resetDisplay(void){
	
	#if (HD44780_DATA_MODE == 4)
	
		#if (HD44780_DATA_PORT_MASK == 0x0F)
	
			/*------------RESET THE HD44780------------*/
	
			sendNibble(0x03,HD44780_INSTRUCTION);
			DELAY_MS(1);
			sendNibble(0x03,HD44780_INSTRUCTION);
			DELAY_MS(1);
			sendNibble(0x03,HD44780_INSTRUCTION);
			DELAY_MS(1);
			sendNibble(0x02,HD44780_INSTRUCTION);
			DELAY_MS(1);
	
		#elif (HD44780_DATA_PORT_MASK == 0xF0)
	
			/*------------RESET THE HD44780------------*/
	
			sendNibble((0x03<<4),HD44780_INSTRUCTION);
			DELAY_MS(1);
			sendNibble((0x03<<4),HD44780_INSTRUCTION);
			DELAY_MS(1);
			sendNibble((0x03<<4),HD44780_INSTRUCTION);
			DELAY_MS(1);
			sendNibble((0x02<<4),HD44780_INSTRUCTION);
			DELAY_MS(1);

		#endif
	
	#elif (HD44780_DATA_MODE == 8)
	
		/*------------RESET THE HD44780------------*/
	
		sendNibble(0x03,HD44780_INSTRUCTION);
		DELAY_MS(1);
		sendNibble(0x03,HD44780_INSTRUCTION);
		DELAY_MS(1);
		sendNibble(0x03,HD44780_INSTRUCTION);
		DELAY_MS(1);
	
	#endif
	
}

static void sendNibble(ubyte_t a_data, hd44780transmissiontype_t a_transType){
	
	//------------SEND A NIBBLE------------//
	
	if(a_transType == HD44780_INSTRUCTION){
		
		gpio_setPin(HD44780_EN_PIN);								// enable HD44780 interface for new data (EN signal), Command register is selected
		DELAY_US(800);
		gpio_setPort(HD44780_DATA_PORT, (a_data & HD44780_DATA_PORT_MASK));		// send a nibble of command
		DELAY_US(800);
		gpio_clearPin(HD44780_EN_PIN);							// clear control port
		DELAY_US(200);
		gpio_clearPort(HD44780_DATA_PORT, (a_data & HD44780_DATA_PORT_MASK));		// clear data port
		
	}else if(a_transType == HD44780_DATA){
		
		gpio_setPin(HD44780_RS_PIN);								// select Data register (RS signal)
		DELAY_US(800);
		gpio_setPin(HD44780_EN_PIN);								// enable HD44780 interface for new data (EN signal)
		DELAY_US(800);
		gpio_setPort(HD44780_DATA_PORT, (a_data & HD44780_DATA_PORT_MASK));		// send a nibble of data
		DELAY_US(800);
		gpio_clearPin(HD44780_EN_PIN);			// clear EN pin
		gpio_clearPin(HD44780_RS_PIN);			// clear RS pin
		DELAY_US(200);
		gpio_clearPort(HD44780_DATA_PORT, (a_data & HD44780_DATA_PORT_MASK));		// clear data port
		
	}
	
}

static ubyte_t readNibble(void){
	
	#ifdef HD44780_RW_PIN
	
		ubyte_t data;
	
		gpio_setPortDirection(HD44780_DATA_PORT,HD44780_DATA_PORT_MASK,IO_INPUT);
	
		gpio_setPin(HD44780_RS_PIN);										// select Data register (RS signal)
		gpio_setPin(HD44780_RW_PIN);										// select Data register (RW signal)
		DELAY_US(800);
		gpio_setPin(HD44780_EN_PIN);										// enable GHD44780 interface for new data (EN signal)
		DELAY_US(800);
		data = (gpio_readPort(HD44780_DATA_PORT)&HD44780_DATA_PORT_MASK);		// read nibble of data
		DELAY_US(800);
		gpio_clearPin(HD44780_EN_PIN);										// clear EN pin
		gpio_clearPin(HD44780_RW_PIN);										// clear RW pin
		gpio_clearPin(HD44780_RS_PIN);										// clear RS pin
		DELAY_US(200);
	
		gpio_setPortDirection(HD44780_DATA_PORT,HD44780_DATA_PORT_MASK,IO_OUTPUT);
	
		return data;
	
	#else
	
		return 0;
		
	#endif	
}