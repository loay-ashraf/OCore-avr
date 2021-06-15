/**********************************************************************
*
* File:         twi.c
*
* Author(s):    Loay Ashraf <loay.ashraf.96@gmail.com>
*
* Date created: 05/08/2019
*
* Description:	contains function definitions for two-wire 
*               interface module.
*
**********************************************************************/

/*------------------------------INCLUDES-----------------------------*/

#include "twi.h"
#include "hal/mcu/peripheral/gpio.h"
#include "hal/mcu/sys/delay.h"
#include "hal/mcu/sys/interrupt.h"
#include "service/include/register_macros.h"
#include <math.h>

/*--------------------------GLOBAL VARIABLES-------------------------*/

/**********************************************************************
*
* Variable:    g_twiISRCallback
*
* Description: Holds address of interrupt callback function.
*
* Notes:
*
* Scope:       twi.c.
*
**********************************************************************/

static ISRcallback_t g_twiISRCallback;

/*-----------------------FUNCTION DEFINITIONS------------------------*/

/**********************************************************************
*
* Function:    twi_config
*
* Description: Configures two-wire interface module.
*
* Notes:
*
* Returns:     None.
*
**********************************************************************/

void twi_config(twi_t a_twi, twiconfig_t * a_twiConfig){
	
	gpio_enablePinPullUp(TWI0_M_SDA_PIN);
	gpio_enablePinPullUp(TWI0_M_SCL_PIN);
	
	uint8_t TWBRValue = ((F_CPU/a_twiConfig->frequency)-16)/(2*pow(4,a_twiConfig->prescaler));

	SRI(TWSR,a_twiConfig->prescaler);
	WRI(TWBR,TWBRValue);
	WRI(TWAR,(a_twiConfig->address<<1)|a_twiConfig->generalcall);
	SBI(TWCR,a_twiConfig->ackbit);
	
}

/**********************************************************************
*
* Function:    twi_enable
*
* Description: Enables two-wire interface module.
*
* Notes:
*
* Returns:     None.
*
**********************************************************************/

void twi_enable(twi_t a_twi){

	SBI(TWCR,TWEN);

}

/**********************************************************************
*
* Function:    twi_disable
*
* Description: Disables two-wire interface module.
*
* Notes:
*
* Returns:     None.
*
**********************************************************************/

void twi_disable(twi_t a_twi){
	
	gpio_clearPin(TWI0_M_SDA_PIN);
	gpio_clearPin(TWI0_M_SCL_PIN);
	
	CBI(TWCR,TWEN);
	
}

/**********************************************************************
*
* Function:    twi_transmitStart
*
* Description: Transmits START byte via two-wire interface module.
*
* Notes:
*
* Returns:     None.
*
**********************************************************************/

bool_t twi_transmitStart(twi_t a_twi, ubyte_t a_address, bool_t a_readWrite){
	
	SBI(TWCR,TWSTA);
	SBI(TWCR,TWINT);
	
	while(!RBI(TWCR,TWINT));
	
	if (twi_getStatus(TWI0_M) == START || twi_getStatus(TWI0_M) == R_START){
		
		WRI(TWDR,(a_address<<1)|a_readWrite);
		CBI(TWCR,TWSTA);
		SBI(TWCR,TWINT);

		while(!RBI(TWCR,TWINT));
		
		if(twi_getStatus(TWI0_M) != SLA_R_ACK && twi_getStatus(TWI0_M) != SLA_W_ACK)
			return ERROR;
		else
			return PASS;
	}else{
		return ERROR;
	}
}

/**********************************************************************
*
* Function:    twi_transmitStop
*
* Description: Transmits STOP byte via two-wire interface module.
*
* Notes:
*
* Returns:     None.
*
**********************************************************************/

void twi_transmitStop(twi_t a_twi){
	
	CBI(TWCR,TWEA);
	DELAY_US(100);
	SBI(TWCR,TWSTO);
	SBI(TWCR,TWINT);
	DELAY_US(100);
	SBI(TWCR,TWEA);
	
}

/**********************************************************************
*
* Function:    twi_transmitByte
*
* Description: Transmits data byte via two-wire interface module.
*
* Notes:
*
* Returns:     None.
*
**********************************************************************/

bool_t twi_transmitByte(twi_t a_twi, ubyte_t a_data){
	
	if(twi_getStatus(TWI0_M) == SLA_W_ACK || twi_getStatus(TWI0_M) == TX_ACK_MASTER || twi_getStatus(TWI0_M) == TX_ACK_SLAVE){
		
		WRI(TWDR,a_data);
		SBI(TWCR,TWINT);
		
		while(!RBI(TWCR,TWINT));
		
		if(twi_getStatus(TWI0_M) != TX_ACK_MASTER && twi_getStatus(TWI0_M) != TX_ACK_SLAVE)
			return ERROR;
		else
			return PASS;
	}else{
		return ERROR;
	}
}

/**********************************************************************
*
* Function:    twi_receiveByte
*
* Description: Receives data byte via two-wire interface module.
*
* Notes:
*
* Returns:     TWDR register value.
*
**********************************************************************/

ubyte_t twi_receiveByte(twi_t a_twi){
	
	if(twi_getStatus(TWI0_M) == SLA_R_ACK || twi_getStatus(TWI0_M) == RX_ACK_MASTER){
		
		SBI(TWCR,TWINT);
		while(!RBI(TWCR,TWINT));
		
		if(twi_getStatus(TWI0_M) != RX_ACK_MASTER)
			return ERROR;
		else
			return TWDR;
		
	}else{
		
		return ERROR;
		
	}
}     

/**********************************************************************
*
* Function:    twi_enableInterrupt
*
* Description: Enables interrupt request for two-wire interface
*              module.
*
* Notes:       This function enables global interrupts if disabled.
*
* Returns:     None.
*
**********************************************************************/    

void twi_enableInterrupt(twi_t a_twi){
	
	if(!RBI(SREG,I_BIT))
		ENABLE_GLOBAL_INTERRUPTS;
	SBI(TWCR,TWIE);
	
}

/**********************************************************************
*
* Function:    twi_disableInterrupt
*
* Description: Disables interrupt request for two-wire interface
*              module.
*
* Notes:       This function doesn't disable global interrupts.
*
* Returns:     None.
*
**********************************************************************/

void twi_disableInterrupt(twi_t a_twi){

	CBI(TWCR,TWIE);	
	
}

/**********************************************************************
*
* Function:    twi_setISRCallback
*
* Description: Sets interrupt callback function for two-wire
*              interface module.
*
* Notes:
*
* Returns:     None.
*
**********************************************************************/

void twi_setISRCallback(twi_t a_twi, ISRcallback_t a_twiISRCallback){
	
	g_twiISRCallback = a_twiISRCallback;
	
}

/**********************************************************************
*
* Function:    twi_getStatus
*
* Description: Gets current status of two-wire interface module.
*
* Notes:
*
* Returns:     TWSR register masked value.
*
**********************************************************************/

ubyte_t twi_getStatus(twi_t a_twi){
	
	return (RRI(TWSR)&0xF8);
}

/*---------------------------------ISR-------------------------------*/

ISR(TWI_vect){
	
	g_twiISRCallback();
	
}
