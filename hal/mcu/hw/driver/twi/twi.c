/*
 * twi.c
 *
 * Created: 05/08/2019 09:37:55 PM
 *  Author: Loay Ashraf
 */ 

#include "twi.h"

static ISRcallback_t g_twiISRCallback;

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

void twi_disable(void){
	
	gpio_clearPin(TWI_SDA_PIN);
	gpio_clearPin(TWI_SCL_PIN);
	
	WRI(TWCR,0x00);
	WRI(TWBR,0x00);
	WRI(TWSR,0xF8);
	WRI(TWDR,0xFF);
	WRI(TWAR,0xFE);
	
}

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

void twi_transmitStop(void){
	
	CBI(TWCR,TWEA);
	DELAY_US(100);
	SBI(TWCR,TWSTO);
	SBI(TWCR,TWINT);
	DELAY_US(100);
	SBI(TWCR,TWEA);
	
}

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

void twi_enableInterrupt(void){
	
	if(!RBI(SREG,I_BIT))
		ENABLE_GLOBAL_INTERRUPTS;
	SBI(TWCR,TWIE);
	
}

void twi_disableInterrupt(void){

	CBI(TWCR,TWIE);	
	
}

void twi_setISRCallback(ISRcallback_t a_twiISRCallback){
	
	g_twiISRCallback = a_twiISRCallback;
	
}

ubyte_t twi_getStatus(void){
	
	return (RRI(TWSR)&0xF8);
}

ISR(TWI_vect){
	
	g_twiISRCallback();
	
}