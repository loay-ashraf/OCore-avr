/**********************************************************************
*
* File:         twi.h
*
* Author(s):    Loay Ashraf <loay.ashraf.96@gmail.com>
*
* Date created: 05/08/2019
*
* Description:  contains definitions and function declarations
*               for two-wire interface module.
*
**********************************************************************/

#ifndef TWI_H_
#define TWI_H_
 
#include "hal/ddp/ddp_interface.h"
#include PATH_TO_TWI

#define TWI_CONFIG(TWI,TWI_CONFIG)                    twi_config(TWI,TWI_CONFIG)
#define TWI_ENABLE(TWI)                               twi_enable(TWI)
#define TWI_DISABLE(TWI)                              twi_disbale(TWI)
#define TWI_TRANSMIT_START(TWI,ADDRESS,READ_WRITE)    twi_transmitStart(TWI,ADDRESS,READ_WRITE)
#define TWI_TRANSMIT_STOP(TWI)                        twi_transmitStop(TWI)
#define TWI_TRANSMIT_BYTE(TWI,DATA)                   twi_transmitByte(TWI,DATA)
#define TWI_RECEIVE_BYTE(TWI)                         twi_receiveByte(TWI)
#define TWI_ENABLE_INTERRUPT(TWI)                     twi_enableInterrupt(TWI)
#define TWI_DISABLE_INTERRUPT(TWI)                    twi_disableInterrupt(TWI)
#define TWI_SET_ISR_CALLBACK(TWI,ISR_CALLBACK)        twi_setISRCallback((TWI,ISR_CALLBACK)
#define TWI_GET_STATUS(TWI)                           twi_getStatus(TWI)

#endif /* TWI_H_ */
