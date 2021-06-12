/*
 * rs232_transmit.c
 *
 * Created: 15/09/2019 05:24:29 PM
 *  Author: Loay Ashraf
 */ 

#include "app/examples/include/examples.h"
#include "hal/ecu/lcd/api/lcd.h"
#include "hal/ecu/keypad/keypad.h"
#include "hal/mcu/hw/driver/usart/usart.h"
#include "hal/mcu/sys/delay.h"

static char keyMap[KEYPAD_ROWS][KEYPAD_COLUMNS] = {{'1','2','3','A'},{'4','5','6','B'},{'7','8','9','C'},{'*','0','#','D'}};				/* Keypad key map array */
static usartconfig_t usartConfig = {.mode=US_ASYNC,.frameSize=US_EIGHT_BITS,.parity=US_EVEN,.speed=US_NORMAL,.stopBit=US_ONE_BIT};			/* USART configuration structure */

void ex_rs232_transmit(void){
	
	char key = 0;	/* variable to hold value of pressed key */
	
	/***********************************************/
	/* initialize LCD, keypad and USART interfaces */
	/***********************************************/
	
	LCD_init(TRUE,TRUE,TRUE,TRUE);
	Keypad_setKeyMap(keyMap);
	Keypad_init();
	usart_config(USART0_M,&usartConfig);
	usart_setBaudrate(USART0_M,US_9600);
	usart_enable(USART0_M);
	
	while(1){		/* loop forever */
		
		key = Keypad_scan();	/* check if key is pressed */
		
		if(key){	/* if pressed */
			
			if(key == 'D'){		/* key to send data on the screen */
				
				/*****************************************************************/
				/* read data on the screen and put it in one array of characters */
				/*****************************************************************/
				
				uint8_t row,col;
				char frameBuffer[LCD_ROWS][LCD_COLUMNS+1];
				char transmitString[LCD_ROWS*LCD_COLUMNS];
				lcdposition_t cursorPosition;
				
				cursorPosition = LCD_getCursorPosition();
				LCD_copyBuffer(frameBuffer);
				uint8_t transmitStringLength = (cursorPosition.row*LCD_COLUMNS)+cursorPosition.column;
				
				for(row=0;row<=cursorPosition.row;row++){
					
					for(col=0;col<LCD_COLUMNS;col++){
						
						transmitString[(LCD_COLUMNS*row)+col] = frameBuffer[row][col];
						
						if((row == cursorPosition.row) && (col == cursorPosition.column))
							break;
					
					}
				}
				
				transmitString[transmitStringLength] = '\0';	/* terminate string */
				
				usart_transmitString(USART0_M,transmitString,US_LF);		/* transmit string */
				LCD_clearDisplay();								/* clear display */
				
			}else{		/* otherwise */
			
				LCD_putc(key);		/* print pressed key to LCD screen */
			
			}
		}
		
	}
	
}