/**********************************************************************
*
* File:         usart.h
*
* Author(s):    Loay Ashraf <loay.ashraf.96@gmail.com>
*
* Date created: 05/03/2018
*
* Description:  contains definitions, configurations, typedefs
*               and function declarations for usart interface
*               module.
*
**********************************************************************/

#ifndef USART_H_
#define USART_H_
 
#include "hal/ddp/ddp_interface.h"
#include PATH_TO_USART

#define USART_CONFIG(USART,USART_CONFIG)                              usart_config(USART,USART_CONFIG)
#define USART_SET_BAUDRATE(USART,BAUDRATE)                            usart_setBaudrate(USART,BAUDRATE)
#define USART_ENABLE(USART)                                           usart_enable(USART)
#define USART_DISABLE(USART)                                          usart_disable(USART)
#define USART_TRANSMIT_CHARACTER(USART,CHARACTER)                     usart_transmitCharacter(USART,CHARACTER)
#define USART_TRANSMIT_STRING(USART,STR,LINE_TERM)                    usart_transmitString(USART,STR,LINE_TERM)
#define USART_RECEIVE_CHARACTER(USART)                                usart_receiveCharacter(USART)
#define USART_RECEIVE_STRING(USART,LINE_TERM)                         usart_receiveString(USART,LINE_TERM)
#define USART_ENABLE_INTERRUPT(USART,USART_INTERRUPT)                 usart_enableInterrupt(USART,USART_INTERRUPT)
#define USART_DISABLE_INTERRUPT(USART,USART_INTERRUPT)                usart_disableInterrupt(USART,USART_INTERRUPT)
#define USART_SET_ISR_CALLBACK(USART,USART_INTERRUPT,ISR_CALLBACK)    usart_setISRCallback(USART,USART_INTERRUPT,ISR_CALLBACK)

#endif /* USART_H_ */
