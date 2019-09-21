/** @file USART.c
 *  @brief includes function definition
 *  @author Loay Ashraf
 *	@version 1.0.0
 *  @pre include USART.h
 *  @bug no known bugs
 *  @see USART.h
 */

//------------INCLUDE DRIVER HEADER FILE------------//

 #include "usart.h"
 #include "hal/mcu/hw/driver/gpio/gpio.h"
 #include "hal/mcu/io/io_macros.h"
 #include "hal/mcu/sys/cpu_config.h"
 #include "hal/mcu/sys/delay.h"
 #include "hal/mcu/sys/interrupt.h"
 
//------------DECLARE LOCAL VARIABLES------------//

/** @var uint8_t mode_flag
 *  @brief indicates the current mode (Asynchronous or Synchronous)
 */

/** @var uint8_t u2x_flag
 *  @brief indicates the current speed (Normal speed or Double speed)
 */

/** @var uint8_t ninth_bit_flag
 *  @brief indicates if there's a ninth bit in the data frame
 */

 static usartmode_t g_mode;
 static usartspeed_t g_speed;
 static bool_t g_ninthBit;
 static ISRcallback_t g_usartISRCallback[3];

 /** @brief initializes the USART interface
  *  @param umode communication mode (Asynchronous or Synchronous).
  *  @param u2x speed mode (Normal speed or Double speed).
  *	 @param ch_size number of bits in the data frame.
  *	 @param parity_bit parity check mode (Parity or No parity).
  *	 @param stop_bit number of stop bits (one bit or two bits).
  *  @return none.
  */
 void usart_config(usartconfig_t a_usartConfig){
    
	g_mode = a_usartConfig.mode; 
	g_speed = a_usartConfig.speed;
    
	if(!a_usartConfig.mode){
	
		if(a_usartConfig.speed == US_DOUBLE)
			SBI(UCSRA,U2X);
		else
			CBI(UCSRA,U2X);
		
		if(a_usartConfig.frameSize != 9 && a_usartConfig.frameSize >= 5){
			
			g_ninthBit = 0;
			WRI(UCSRC,((1<<URSEL)|((a_usartConfig.frameSize-5)<<UCSZ0)|(a_usartConfig.parity<<UPM0)|(a_usartConfig.stopBit<<USBS)));

		}else if (a_usartConfig.frameSize == 9){

			g_ninthBit = 1;
			
			// set number of bits to 9 in the data frame
			SBI(UCSRB,UCSZ2);
			WRI(UCSRC,((1<<URSEL)|(3<<UCSZ0)));
			
		}

	}else if (a_usartConfig.mode == US_SYNC){
	
		
		if(a_usartConfig.frameSize != 9 && a_usartConfig.frameSize >= 5){
			
			g_ninthBit = 0;
			WRI(UCSRC,((1<<URSEL)|(1<<UMSEL)|((a_usartConfig.frameSize-5)<<UCSZ0)|(a_usartConfig.parity<<UPM0)|(a_usartConfig.stopBit<<USBS)));

		}else if (a_usartConfig.frameSize == 9){

			g_ninthBit = 1;
			
			// set number of bits to 9 in the data frame
			SBI(UCSRB,UCSZ2);
			WRI(UCSRC,((1<<URSEL)|(1<<UMSEL)|(3<<UCSZ0)));
			
		}
	
	}

 }

/** @brief sets the baud rate for the USART interface
 *  @param baudrate set baud rate in bits/sec.
 *  @return none.
 */
 void usart_setBaudrate(usartbaudrate_t a_baudrate){
   
	uint16_t UBValue = 0;
   
	usart_disableTXRX();		// disable receiver and transmitter buffers

	if(!g_mode){

		if (g_speed == 0)

		UBValue = ((F_CPU)/(16.0*a_baudrate))-1;	// calculate baud rate based on CPU clock frequency, equations on p. 143 in datasheet

		else if(g_speed == 1)

		UBValue = ((F_CPU)/(8.0*a_baudrate))-1;		// calculate baud rate based on CPU clock frequency, equations on p. 143 in datasheet

	}else if(g_mode == US_SYNC){
    
		UBValue = ((F_CPU)/(2.0*a_baudrate))-1;		// calculate baud rate based on CPU clock frequency, equations on p. 143 in datasheet
	}
   
   // set the baud rate
   WRI(UBRRH,(uint8_t) (UBValue>>8));
   WRI(UBRRL,(uint8_t) UBValue);

 }
 
 /** @brief enables the USART interface
  *  @return none.
  */
 void usart_enableTXRX(void){

  SRI(UCSRB,((1<<TXEN)|(1<<RXEN)));		// enable receiver and transmitter buffers

 }
 
 void usart_disableTXRX(void){

	 CRI(UCSRB,((1<<TXEN)|(1<<RXEN)));		// enable receiver and transmitter buffers

 }

 /** @brief transmits 8-bit data 
  *	 @param data 8-bit data to be transmitted.
  *  @return none.
  */
 void usart_transmitCharacter(char a_char){

	while(!RBI(UCSRA,UDRE));		// wait for empty transmit buffer
	
	if(!g_ninthBit){

		WRI(UDR,a_char);				// start transmission

	}else{
	  
		CBI(UCSRB,TXB8);			// clear ninth data bit
		
		if(a_char & 0x0100)
			SBI(UCSRB,TXB8);		// set ninth data bit if it's set in the data
	  
		WRI(UDR,a_char);				// start transmission

	}
 }

 /** @brief transmits a string of 8-bit data 
  *	 @param data pointer to the fist element of the string.
  *  @return none.
  */
 void usart_transmitString(const char * a_str, usartlineterm_t a_usartLineTerm){

   while(*a_str != '\0'){
      
	  usart_transmitCharacter(*a_str++);		// iterate through the array till the NULL terminator

    }
	
	/*------------transmit line terminator-----------*/
	
	switch(a_usartLineTerm){
		
		case US_CR: usart_transmitCharacter('\r');		
		break;
		
		case US_LF: usart_transmitCharacter('\n');	
		break;
		
		case US_CRLF: {
			
			usart_transmitCharacter('\r');
			usart_transmitCharacter('\n');
			
		}
		break;
	
	}
	
 }

 /** @brief receives 8-bit data 
  *  @return received 8-bit data.
  */
 char usart_receiveCharacter(void){
    
	while(!RBI(UCSRA,RXC));			// Wait for incoming data
   
	if (!g_ninthBit){
		
		return RRI(UDR);			// return UDR register if there's no ninth bit

	}else{

		uint8_t resh,resl;

		resh = RRI(UCSRB);
		resl = RRI(UDR);

		resh = (resh>>1)&0x01;		// process the ninth bit

		return ((resh<<8)|resl);	// return the combined 9-bit data
	} 
 }
 
 /** @brief receives a string of 8-bit data 
  *  @return pointer to the fist element of the received string.
  */
 char * usart_receiveString(usartlineterm_t a_usartLineTerm){
	
	 static char input[USART_BUFFER_SIZE];
	 uint8_t tmp, index = 0;
	 
	 switch(a_usartLineTerm){
		 
		 case US_CR: {
			 
			 while ((tmp = usart_receiveCharacter()) != '\r'){
				 
				 input[index++] = tmp;				// iterate through the array till CR is received
				 if(index == USART_BUFFER_SIZE)
					break;							// break loop if buffer size limit is reached
				 
			 }
			 
			 if(tmp == '\r'){
					 
				input[index] = '\0';				// terminate the array with NULL terminator
				return input;						// return pointer to the first element of the array
				 
			}else{
				 
				 return USART_RX_ERROR;
				 
			}
			
		 }
		 break;
		 
		 case US_LF: {
			 
			while ((tmp = usart_receiveCharacter()) != '\n'){
				
				if(tmp == '\r')
					return USART_RX_ERROR;
				
				input[index++] = tmp;				// iterate through the array till LF is received
				if(index == USART_BUFFER_SIZE)
					break;							// break loop if buffer size limit is reached
				
			}
				
			if(tmp == '\n'){	
				
				input[index] = '\0';				// terminate the array with NULL terminator
				return input;						// return pointer to the first element of the array
				
			}else{
				
				return USART_RX_ERROR;
				
			}
		 
		 }
		 break;
		 
		 case US_CRLF: {
			 
			 while ((tmp = usart_receiveCharacter()) != '\r'){
				 
				 input[index++] = tmp;				// iterate through the array till CR is received
				 if(index == USART_BUFFER_SIZE)
					 break;							// break loop if buffer size limit is reached
				 
			 }
			 
			 if((tmp = usart_receiveCharacter()) == '\n'){
				 
				 input[index] = '\0';				// terminate the array with NULL terminator
				 return input;						// return pointer to the first element of the array
				 
			}else{
				 
				 return USART_RX_ERROR;
				 
			 }
			 
		 }
		 break; 
		 
		 default: return "NULL";
		 break;
	 }

 }
 
 void usart_enableInterrupt(usartinterrupt_t a_usartInterrupt){
	 
	 if(!RBI(SREG,I_BIT))
		ENABLE_GLOBAL_INTERRUPTS;
		
	SBI(UCSRB,a_usartInterrupt);	
	 
 }
 
 void usart_disableInterrupt(usartinterrupt_t a_usartInterrupt){
	 
	 CBI(UCSRB,a_usartInterrupt);
	 
 }
 
 void usart_setISRCallback(usartinterrupt_t a_usartInterrupt, ISRcallback_t a_ISRCallback){
	 
	 g_usartISRCallback[a_usartInterrupt-5] = a_ISRCallback;
	 
 }
 
 ISR(USART_UDRE_vect){
	 
	g_usartISRCallback[0](); 
	 
 }
 
 ISR(USART_TXC_vect){
	 
	g_usartISRCallback[1](); 
	  
 }
 
 ISR(USART_RXC_vect){
	 
	g_usartISRCallback[2]();
	 
 }