/** @file USART.h
 *  @brief This driver supports interfacing any device that
 *	supports serial communication via USART interface; baud 
 *	rate, data frame, parity check and number of stop bits
 *	can be set according to each system
 *  @author Loay Ashraf
 *	@version 1.0.0
 *  @bug no known bugs
 *  @see micro_config.h
 */

#ifndef USART_H_
#define USART_H_

//------------INCLUDE MCU CONFIGURATION------------//

#include "../../../io/io_defs.h"
#include "../../../io/io_macros.h"
#include "../../../sys/cpu_config.h"
#include "../../../sys/interrupt.h"
#include "../../../sys/std_types.h"

//------------DEFINE CONFIGURATION PARAMETERS ABBREVIATIONS------------//

#define USART_RX_PIN		PD0_M
#define USART_TX_PIN		PD1_M	
#define USART_XCK_PIN		PB0_M

#define USART_BUFFER_SIZE	16
#define USART_TX_ERROR		"TXERR"
#define USART_RX_ERROR		"RXERR"

typedef enum{US_ASYNC,US_SYNC}usartmode_t;
typedef enum{US_NORMAL,US_DOUBLE}usartspeed_t;
typedef enum{US_EVEN=2,US_ODD=3}usartparity_t;
typedef enum{US_ONE_BIT,US_TWO_BITS}usartstopbit_t;
typedef enum{US_50=50,US_75=75,US_110=110,US_134=134,US_150=150,US_200=200,US_300=300,US_600=600,US_1200=1200,US_1800=1800,
			US_2400=2400,US_4800=4800,US_9600=9600,US_19200=19200,US_38400=38400,US_57600=57600,US_115200=115200}usartbaudrate_t;
typedef enum{US_CR,US_LF,US_CRLF}usartlineterm_t;	
typedef enum{US_RX=0x07,US_TX=0x06,US_DRE=0x05}usartinterrupt_t;		
	
typedef struct{
	
	usartmode_t mode;
	usartspeed_t speed;
	uint8_t frameSize;
	usartparity_t parity;
	usartstopbit_t stopBit;
	
}usartconfig_t;			

//------------FUNCTION PROTOTYPES------------//

void usart_config(usartconfig_t a_usartConfig);
void usart_setBaudrate(usartbaudrate_t a_baudrate);
void usart_enableTXRX(void);
void usart_disableTXRX(void);
void usart_transmitCharacter(char a_char);
void usart_transmitString(const char * a_str, usartlineterm_t a_usartLineTerm);
char usart_receiveCharacter(void);
char * usart_receiveString(usartlineterm_t a_usartLineTerm);
void usart_enableInterrupt(usartinterrupt_t a_usartInterrupt);
void usart_disableInterrupt(usartinterrupt_t a_usartInterrupt);
void usart_setISRCallback(usartinterrupt_t a_usartInterrupt, ISRcallback_t a_ISRCallback);

#endif /* USART_H_ */