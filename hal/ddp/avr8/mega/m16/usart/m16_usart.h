/**********************************************************************
*
* File:         m16_usart.h
*
* Author(s):    Loay Ashraf <loay.ashraf.96@gmail.com>
*
* Date created: 17/06/2021
*
* Description:  contains definitions, configurations, typedefs
*               and function declarations for usart interface
*               module.
*
**********************************************************************/

#ifndef M16_USART_H_
#define M16_USART_H_

/*------------------------------INCLUDES-----------------------------*/

#include "hal/ddp/avr8/mega/m16/m16_io.h"
#include "service/include/std_types.h"

/*-------------------DEFINITIONS AND CONFIGURATIONS------------------*/

#define USART_RX_PIN         PD0_M
#define USART_TX_PIN         PD1_M
#define USART_XCK_PIN        PB0_M

#define USART_BUFFER_SIZE    16
#define USART_TX_ERROR       "TXERR"
#define USART_RX_ERROR       "RXERR"

/*--------------------------------ENUMS------------------------------*/

typedef enum{USART0_M}usart_t;
typedef enum{US_ASYNC,US_SYNC}usartmode_t;
typedef enum{US_NORMAL,US_DOUBLE}usartspeed_t;
typedef enum{US_FIVE_BITS=5,US_SIX_BITS,US_SEVEN_BITS,US_EIGHT_BITS,US_NINE_BITS}usartframsize_t;    
typedef enum{US_EVEN=2,US_ODD=3}usartparity_t;
typedef enum{US_ONE_BIT,US_TWO_BITS}usartstopbit_t;
typedef enum{US_50=50,US_75=75,US_110=110,US_134=134,US_150=150,US_200=200,US_300=300,US_600=600,US_1200=1200,US_1800=1800,
            US_2400=2400,US_4800=4800,US_9600=9600,US_19200=19200,US_38400=38400,US_57600=57600,US_115200=115200}usartbaudrate_t;
typedef enum{US_CR,US_LF,US_CRLF}usartlineterm_t;    
typedef enum{US_RX=0x07,US_TX=0x06,US_DRE=0x05}usartinterrupt_t;        

/*-------------------------------STRUCTS-----------------------------*/
    
typedef struct{
    
    usartmode_t mode;
    usartspeed_t speed;
    usartframsize_t frameSize;
    usartparity_t parity;
    usartstopbit_t stopBit;
    
}usartconfig_t;            

/*------------------------FUNCTION DECLARATIONS----------------------*/    

void usart_config(usart_t a_usart, usartconfig_t * a_usartConfig);
void usart_setBaudrate(usart_t a_usart, usartbaudrate_t a_baudrate);
void usart_enable(usart_t a_usart);
void usart_disable(usart_t a_usart);
void usart_transmitCharacter(usart_t a_usart, char a_char);
void usart_transmitString(usart_t a_usart, const char * a_str, usartlineterm_t a_usartLineTerm);
char usart_receiveCharacter(usart_t a_usart);
char * usart_receiveString(usart_t a_usart, usartlineterm_t a_usartLineTerm);
void usart_enableInterrupt(usart_t a_usart, usartinterrupt_t a_usartInterrupt);
void usart_disableInterrupt(usart_t a_usart, usartinterrupt_t a_usartInterrupt);
void usart_setISRCallback(usart_t a_usart, usartinterrupt_t a_usartInterrupt, ISRcallback_t a_ISRCallback);

#endif /* M16_USART_H_ */
