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
#include "hal/mcu/hw/driver/gpio/gpio.h"
#include "hal/mcu/io/io_macros.h"
#include "hal/mcu/sys/delay.h"
#include "hal/mcu/sys/interrupt.h"

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
* Function:    twi_enable
*
* Description: Enables two-wire interface module.
*
* Notes:
*
* Returns:     None.
*
**********************************************************************/

void twi_enable(void){
	
	gpio_enablePinPullUp(TWI_SDA_PIN);
	gpio_enablePinPullUp(TWI_SCL_PIN);

	uint8_t TWBRValue = ((F_CPU_K/TWI_FREQ_K)-16)/32;
	SBI(TWSR,TWPS1);
	WRI(TWBR,TWBRValue);
	WRI(TWAR,(TWI_ADDRESS<<1)|TWI_GC);
	SBI(TWCR,TWEN);
	SBI(TWCR,TWEA);

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

void twi_disable(void){
	
	gpio_clearPin(TWI_SDA_PIN);
	gpio_clearPin(TWI_SCL_PIN);
	
	WRI(TWCR,0x00);
	WRI(TWBR,0x00);
	WRI(TWSR,0xF8);
	WRI(TWDR,0xFF);
	WRI(TWAR,0xFE);
	
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

bool_t twi_transmitStart(ubyte_t a_address, bool_t a_readWrite){
	
	SBI(TWCR,TWSTA);
	SBI(TWCR,TWINT);
	
	while(!RBI(TWCR,TWINT));
	
	if (twi_getStatus() == START || twi_getStatus() == R_START){
		
		WRI(TWDR,(a_address<<1)|a_readWrite);
		CBI(TWCR,TWSTA);
		SBI(TWCR,TWINT);

		while(!RBI(TWCR,TWINT));
		
		if(twi_getStatus() != SLA_R_ACK && twi_getStatus() != SLA_W_ACK)
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

void twi_transmitStop(void){
	
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

bool_t twi_transmitByte(ubyte_t a_data){
	
	if(twi_getStatus() == SLA_W_ACK || twi_getStatus() == TX_ACK_MASTER || twi_getStatus() == TX_ACK_SLAVE){
		
		WRI(TWDR,a_data);
		SBI(TWCR,TWINT);
		
		while(!RBI(TWCR,TWINT));
		
		if(twi_getStatus() != TX_ACK_MASTER && twi_getStatus() != TX_ACK_SLAVE)
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

ubyte_t twi_receiveByte(void){
	
	if(twi_getStatus() == SLA_R_ACK || twi_getStatus() == RX_ACK_MASTER){
		
		SBI(TWCR,TWINT);
		while(!RBI(TWCR,TWINT));
		
		if(twi_getStatus() != RX_ACK_MASTER)
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
* Notes:       This functions enables global interrupts if disabled.
*
* Returns:     None.
*
**********************************************************************/    

void twi_enableInterrupt(void){
	
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
* Notes:       This functions doesn't disable global interrupts.
*
* Returns:     None.
*
**********************************************************************/

void twi_disableInterrupt(void){

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

void twi_setISRCallback(ISRcallback_t a_twiISRCallback){
	
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

ubyte_t twi_getStatus(void){
	
	return (RRI(TWSR)&0xF8);
}

/*---------------------------------ISR-------------------------------*/

ISR(TWI_vect){
	
	g_twiISRCallback();
	
}