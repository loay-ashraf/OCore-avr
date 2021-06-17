/*
 * rs232_recieve.c
 *
 * Created: 15/09/2019 05:08:53 PM
 *  Author: Loay Ashraf
 */ 

#include "app/examples/include/examples.h"
#include "hal/ecu/display/lcd/api/lcd.h"
#include "hal/mcu/peripheral/usart.h"
#include <string.h>

static usartconfig_t usartConfig = {.mode=US_ASYNC,.frameSize=US_EIGHT_BITS,.parity=US_EVEN,.speed=US_NORMAL,.stopBit=US_ONE_BIT};    /* USART configuration structure */

/*****************************************************************************/
/* define callback function to be called upon triggering a receive interrupt */
/*****************************************************************************/

static void receiveISRCallback(void){
    
    char * RXString;
    RXString = USART_RECEIVE_STRING(USART0_M,US_CR);
    
    if(!strcmp(RXString,"clr"))        /* command to clear display */
        LCD_clearDisplay();        
    else    
        LCD_puts(RXString);            /* otherwise display received characters */
    
}

void ex_rs232_receive(void){
    
    /***************************************/
    /* initialize LCD and USART interfaces */
    /***************************************/
    
    LCD_init(TRUE,TRUE,TRUE,TRUE);
    USART_CONFIG(USART0_M,&usartConfig);
    
    USART_SET_BAUDRATE(USART0_M,US_9600);                            /* 9600 bits/sec baud rate */
    USART_ENABLE_INTERRUPT(USART0_M,US_RX);                        /* enable receive interrupt */
    USART_SET_ISR_CALLBACK(USART0_M,US_RX,&receiveISRCallback);        /* pass defined callback to the ISR to be called */
    USART_ENABLE(USART0_M);                                        /* enable USART interface */
    
}
