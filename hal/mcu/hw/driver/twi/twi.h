/*
 * twi.h
 *
 * Created: 05/08/2019 09:38:07 PM
 *  Author: Loay Ashraf
 */ 


#ifndef TWI_H_
#define TWI_H_

#include "../../../io/io_defs.h"
#include "../../../io/io_macros.h"
#include "../../../sys/delay.h"
#include "../../../sys/interrupt.h"
#include "../../../sys/std_types.h"
#include "../gpio/gpio.h"
#include "twi_config.h"

#define ERROR			FALSE
#define PASS			TRUE
#define WRITE			FALSE
#define READ			TRUE
#define START			0x08
#define R_START			0x10
#define SLA_W_ACK		0x18
#define SLA_R_ACK		0x40
#define TX_ACK_MASTER	0x28
#define RX_ACK_MASTER	0x50
#define TX_ACK_SLAVE	0xC8

void twi_enable(void);
void twi_disable(void);
bool_t twi_transmitStart(ubyte_t a_address, bool_t a_readWrite);
void twi_transmitStop(void);
bool_t twi_transmitByte(ubyte_t a_data);
ubyte_t twi_receiveByte(void);
void twi_enableInterrupt(void);
void twi_disableInterrupt(void);
void twi_setISRCallback(ISRcallback_t a_twiISRCallback);
ubyte_t twi_getStatus(void);

#endif /* TWI_H_ */