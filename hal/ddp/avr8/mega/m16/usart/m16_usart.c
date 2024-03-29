/**********************************************************************
*
* File:         m16_usart.c
*
* Author(s):    Loay Ashraf <loay.ashraf.96@gmail.com>
*
* Date created: 17/06/2021
*
* Description:  contains function definitions for usart interface
*               module.
*
**********************************************************************/

/*------------------------------INCLUDES-----------------------------*/

#include "m16_usart.h"
#include "hal/mcu/sys/cpu_config.h"
#include "hal/ddp/avr8/mega/m16/m16_interrupt.h"
#include "service/include/register_macros.h"
 
/*--------------------------GLOBAL VARIABLES-------------------------*/

/**********************************************************************
*
* Variable:    g_mode
*
* Description: Stores current mode of usart module (synchronous or
*              asynchronous).
*
* Notes:
*
* Scope:       m16_usart.c.
*
**********************************************************************/

static usartmode_t g_mode;

/**********************************************************************
*
* Variable:    g_frameSize
*
* Description: Stores current data frame size.
*
* Notes:
*
* Scope:       m16_usart.c.
*
**********************************************************************/

static usartframsize_t g_frameSize;

/**********************************************************************
*
* Variable:    g_usartISRCallback
*
* Description: Holds addresses of interrupt callback functions.
*
* Notes:
*
* Scope:       m16_usart.c.
*
**********************************************************************/

static ISRcallback_t g_usartISRCallback[3];

/*-----------------------FUNCTION DEFINITIONS------------------------*/

/**********************************************************************
*
* Function:    m16_usart.config
*
* Description: Configures usart interface module.
*
* Notes:
*
* Returns:     None.
*
**********************************************************************/

void usart_config(usart_t a_usart, usartconfig_t * a_usartConfig){
    
    g_mode = a_usartConfig->mode; 
    g_frameSize = a_usartConfig->frameSize;
    
    if(!a_usartConfig->mode){
    
        if(a_usartConfig->speed == US_DOUBLE)
            SBI(UCSRA,U2X);
        else
            CBI(UCSRA,U2X);
        
        if(a_usartConfig->frameSize != US_NINE_BITS && a_usartConfig->frameSize >= US_FIVE_BITS){
            
            WRI(UCSRC,((1<<URSEL)|((a_usartConfig->frameSize-5)<<UCSZ0)|(a_usartConfig->parity<<UPM0)|(a_usartConfig->stopBit<<USBS)));

        }else if (a_usartConfig->frameSize == US_NINE_BITS){
            
            // set number of bits to 9 in the data frame
            SBI(UCSRB,UCSZ2);
            WRI(UCSRC,((1<<URSEL)|(3<<UCSZ0)|(a_usartConfig->parity<<UPM0)|(a_usartConfig->stopBit<<USBS)));
            
        }

    }else if (a_usartConfig->mode == US_SYNC){
    
        if(a_usartConfig->frameSize != US_NINE_BITS && a_usartConfig->frameSize >= US_FIVE_BITS){
            
            WRI(UCSRC,((1<<URSEL)|(1<<UMSEL)|((a_usartConfig->frameSize-5)<<UCSZ0)|(a_usartConfig->parity<<UPM0)|(a_usartConfig->stopBit<<USBS)));

        }else if (a_usartConfig->frameSize == US_NINE_BITS){
            
            // set number of bits to 9 in the data frame
            SBI(UCSRB,UCSZ2);
            WRI(UCSRC,((1<<URSEL)|(1<<UMSEL)|(3<<UCSZ0)|(a_usartConfig->parity<<UPM0)|(a_usartConfig->stopBit<<USBS)));
            
        }
    
    }

}

/**********************************************************************
*
* Function:    usart_setBaudrate
*
* Description: Sets baud rate (bits/sec) for usart interface module.
*
* Notes:
*
* Returns:     None.
*
**********************************************************************/

void usart_setBaudrate(usart_t a_usart, usartbaudrate_t a_baudrate){
   
    uint16_t UBValue = 0;
   
    usart_disable(USART0_M);                                // disable receiver and transmitter buffers

    if(!g_mode){

        if (!RBI(UCSRA,U2X))

            UBValue = ((F_CPU)/(16.0*a_baudrate))-1;    // calculate baud rate based on CPU clock frequency, equations on p. 143 in datasheet

        else if(RBI(UCSRA,U2X))

            UBValue = ((F_CPU)/(8.0*a_baudrate))-1;        // calculate baud rate based on CPU clock frequency, equations on p. 143 in datasheet

    }else if(g_mode == US_SYNC){
    
        UBValue = ((F_CPU)/(2.0*a_baudrate))-1;            // calculate baud rate based on CPU clock frequency, equations on p. 143 in datasheet
    }
   
   // set the baud rate
   WRI(UBRRH,(uint8_t) (UBValue>>8));
   WRI(UBRRL,(uint8_t) UBValue);

}

/**********************************************************************
*
* Function:    usart_enable
*
* Description: Enables usart interface module.      
*
* Notes:
*
* Returns:     None.
*
**********************************************************************/

void usart_enable(usart_t a_usart){

    SRI(UCSRB,((1<<TXEN)|(1<<RXEN)));        // enable receiver and transmitter buffers

}

/**********************************************************************
*
* Function:    usart_disable
*
* Description: Disables usart interface module.
*
* Notes:
*
* Returns:     None.
*
**********************************************************************/
 
void usart_disable(usart_t a_usart){

    CRI(UCSRB,((1<<TXEN)|(1<<RXEN)));        // enable receiver and transmitter buffers

}

/**********************************************************************
*
* Function:    usart_transmitCharacter
*
* Description: Transmits string via usart interface module.
*
* Notes:
*
* Returns:     None.
*
**********************************************************************/

void usart_transmitCharacter(usart_t a_usart, char a_char){

    while(!RBI(UCSRA,UDRE));            // wait for empty transmit buffer
    
    if(g_frameSize < US_NINE_BITS){

        WRI(UDR,a_char);                // start transmission

    }else{
      
        CBI(UCSRB,TXB8);                // clear ninth data bit
        
        if(a_char & 0x0100)
            SBI(UCSRB,TXB8);            // set ninth data bit if it's set in the data
      
        WRI(UDR,a_char);                // start transmission

    }
}

/**********************************************************************
*
* Function:    usart_transmitString
*
* Description: Transmits character via usart interface module.
*
* Notes:
*
* Returns:     None.
*
**********************************************************************/
 
void usart_transmitString(usart_t a_usart, const char * a_str, usartlineterm_t a_usartLineTerm){

    while(*a_str != '\0')
      
        usart_transmitCharacter(USART0_M,*a_str++);        // iterate through the array till the NULL terminator
    
    /*------------transmit line terminator-----------*/
    
    switch(a_usartLineTerm){
        
        case US_CR: usart_transmitCharacter(USART0_M,'\r');        
        break;
        
        case US_LF: usart_transmitCharacter(USART0_M,'\n');    
        break;
        
        case US_CRLF: {
            
            usart_transmitCharacter(USART0_M,'\r');
            usart_transmitCharacter(USART0_M,'\n');
            
        }
        break;
    
    }
    
}

/**********************************************************************
*
* Function:    usart_receiveCharacter
*
* Description: Receives character via usart interface module.
*
* Notes:
*
* Returns:     UDR register value.
*
**********************************************************************/

char usart_receiveCharacter(usart_t a_usart){
    
    while(!RBI(UCSRA,RXC));            // Wait for incoming data
   
    if (g_frameSize < US_NINE_BITS){
        
        return RRI(UDR);            // return UDR register if there's no ninth bit

    }else{

        uint8_t resh,resl;

        resh = RRI(UCSRB);
        resl = RRI(UDR);

        resh = (resh>>1)&0x01;        // process the ninth bit

        return ((resh<<8)|resl);    // return the combined 9-bit data
        
    } 
}
 
/**********************************************************************
*
* Function:    usart_receiveString
*
* Description: Receives string via usart interface module.
*
* Notes:
*
* Returns:     Array of received characters.
*
**********************************************************************/

char * usart_receiveString(usart_t a_usart, usartlineterm_t a_usartLineTerm){
    
    static char input[USART_BUFFER_SIZE];
    uint8_t tmp, index = 0;
     
    switch(a_usartLineTerm){
         
        case US_CR: {
             
            while ((tmp = usart_receiveCharacter(USART0_M)) != '\r'){
                 
                input[index++] = tmp;                // iterate through the array till CR is received
                if(index == USART_BUFFER_SIZE)
                    break;                            // break loop if buffer size limit is reached
                 
            }
             
            if(tmp == '\r'){
                     
                input[index] = '\0';                // terminate the array with NULL terminator
                return input;                        // return pointer to the first element of the array
                 
            }else{
                 
                return USART_RX_ERROR;
                 
            }
            
        }
        break;
         
        case US_LF: {
             
            while ((tmp = usart_receiveCharacter(USART0_M)) != '\n'){
                
                if(tmp == '\r')
                    return USART_RX_ERROR;
                
                input[index++] = tmp;                // iterate through the array till LF is received
                if(index == USART_BUFFER_SIZE)
                    break;                            // break loop if buffer size limit is reached
                
            }
                
            if(tmp == '\n'){    
                
                input[index] = '\0';                // terminate the array with NULL terminator
                return input;                        // return pointer to the first element of the array
                
            }else{
                
                return USART_RX_ERROR;
                
            }
         
        }
        break;
         
        case US_CRLF: {
             
            while ((tmp = usart_receiveCharacter(USART0_M)) != '\r'){
                 
                input[index++] = tmp;                // iterate through the array till CR is received
                if(index == USART_BUFFER_SIZE)
                    break;                            // break loop if buffer size limit is reached
                 
            }
             
            if((tmp = usart_receiveCharacter(USART0_M)) == '\n'){
                 
                input[index] = '\0';                // terminate the array with NULL terminator
                return input;                        // return pointer to the first element of the array
                 
            }else{
                 
                return USART_RX_ERROR;
                 
            }
             
        }
        break; 
         
        default: return "NULL";
        break;
    }

}

/**********************************************************************
*
* Function:    usart_enableInterrupt
*
* Description: Enables interrupt request for usart interface module.
*
* Notes:       This function enables global interrupts if disabled.
*
* Returns:     None.
*
**********************************************************************/
 
void usart_enableInterrupt(usart_t a_usart, usartinterrupt_t a_usartInterrupt){
     
    if(!RBI(SREG,I_BIT))
        ENABLE_GLOBAL_INTERRUPTS;
        
    SBI(UCSRB,a_usartInterrupt);    
     
}

/**********************************************************************
*
* Function:    usart_disableInterrupt
*
* Description: Disables interrupt request for usart interface module.
*
* Notes:       This function doesn't disable global interrupts.
*
* Returns:     None.
*
**********************************************************************/
 
void usart_disableInterrupt(usart_t a_usart, usartinterrupt_t a_usartInterrupt){
     
    CBI(UCSRB,a_usartInterrupt);
     
}
 
void usart_setISRCallback(usart_t a_usart, usartinterrupt_t a_usartInterrupt, ISRcallback_t a_ISRCallback){
     
    g_usartISRCallback[a_usartInterrupt-5] = a_ISRCallback;
     
}
 
/*---------------------------------ISR-------------------------------*/
 
ISR(USART_UDRE_vect){
     
    g_usartISRCallback[0](); 
     
}
 
ISR(USART_TXC_vect){
     
    g_usartISRCallback[1](); 
      
}
 
ISR(USART_RXC_vect){
     
    g_usartISRCallback[2]();
     
}
