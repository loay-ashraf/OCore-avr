/*
 * rs232_recieve.c
 *
 * Created: 15/09/2019 05:08:53 PM
 *  Author: Loay Ashraf
 */ 

#include "app/examples/include/examples.h"
#include "hal/ecu/lcd/api/lcd.h"
#include "hal/mcu/hw/driver/usart/usart.h"
#include <string.h>

static usartconfig_t usartConfig = {.mode=US_ASYNC,.frameSize=US_NINE_BITS,.parity=US_EVEN,.speed=US_NORMAL,.stopBit=US_ONE_BIT};	/* USART configuration structure */

/*****************************************************************************/
/* define callback function to be called upon triggering a receive interrupt */
/*****************************************************************************/

static void receiveISRCallback(void){
	
	char * RXString;
	RXString = usart_receiveString(US_LF);
	
	if(!strcmp(RXString,"clr"))		/* command to clear display */
		LCD_clearDisplay();		
	else	
		LCD_puts(RXString);			/* otherwise display received characters */
	
}

void ex_rs232_receive(void){
	
	/***************************************/
	/* initialize LCD and USART interfaces */
	/***************************************/
	
	LCD_init(TRUE,TRUE,TRUE,TRUE);
	usart_config(usartConfig);
	
	usart_setBaudrate(US_9600);							/* 9600 bits/sec baud rate */
	usart_enableInterrupt(US_RX);						/* enable receive interrupt */
	usart_setISRCallback(US_RX,&receiveISRCallback);	/* pass defined callback to the ISR to be called */
	usart_enableTXRX();									/* enable USART interface */
	
}