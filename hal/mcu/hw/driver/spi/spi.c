/*
 * spi.c
 *
 * Created: 05/09/2019 12:25:35 AM
 *  Author: Loay Ashraf
 */ 

#include "spi.h"

static ISRcallback_t g_spiISRCallback;

void spi_enable(void){
	
	SRI(SPCR,(SPI_MASTER<<MSTR));
	
	#if (SPI_MASTER)
		
		SBI(SPCR,MSTR);
		gpio_setPinDirection(SPI_MOSI_PIN,IO_OUTPUT);
		gpio_setPinDirection(SPI_SCK_PIN,IO_OUTPUT);
		gpio_setPin(SPI_SS_PIN);
		
		gpio_setPinDirection(SPI_SLAVE0_PIN,IO_OUTPUT);
		gpio_setPinDirection(SPI_SLAVE1_PIN,IO_OUTPUT);
		gpio_setPinDirection(SPI_SLAVE2_PIN,IO_OUTPUT);
		gpio_setPinDirection(SPI_SLAVE3_PIN,IO_OUTPUT);
		
		gpio_setPin(SPI_SLAVE0_PIN);
		gpio_setPin(SPI_SLAVE1_PIN);
		gpio_setPin(SPI_SLAVE2_PIN);
		gpio_setPin(SPI_SLAVE3_PIN);
		
	#else
	
		CBI(SPCR,MSTR);
		gpio_setPinDirection(SPI_MISO_PIN,IO_OUTPUT);	
		
	#endif	
	
	#if (SPI_PRESCALER==4)
		
		CRI(SPCR,0x03);
	
	#elif (SPI_PRESCALER==16)
	
		SRI(SPCR,0x01);
	
	#elif (SPI_PRESCALER==64)
	
		SRI(SPCR,0x02);
	
	#elif (SPI_PRESCALER==128)
	
		SRI(SPCR,0x03); 
	
	#endif
	
	SRI(SPSR,(SPI_2X_MODE<<SPI2X));
	SRI(SPCR,(SPI_LSB_FRST<<DORD));
	
	#if (SPI_DATA_MODE == 0)
		
		CBI(SPCR,CPOL);
		CBI(SPCR,CPHA);
	
	#elif (SPI_DATA_MODE == 1)
		
		CBI(SPCR,CPOL);
		SBI(SPCR,CPHA);
	
	#elif (SPI_DATA_MODE == 2)
		
		SBI(SPCR,CPOL);
		CBI(SPCR,CPHA);
	
	#elif (SPI_DATA_MODE == 3)
		
		SBI(SPCR,CPOL);
		SBI(SPCR,CPHA);
	
	#endif
	
	SBI(SPCR,SPE);
	
}

void spi_disable(void){
	
	CBI(SPCR,SPE);
	
}

void spi_selectSlave(spislave_t a_spiSlave){
	
	switch (a_spiSlave){
		
		case SP_SLAVE0: gpio_clearPin(SPI_SLAVE0_PIN);
		break;
		
		case SP_SLAVE1: gpio_clearPin(SPI_SLAVE1_PIN);
		break;
		
		case SP_SLAVE2: gpio_clearPin(SPI_SLAVE2_PIN);
		break;
		
		case SP_SLAVE3: gpio_clearPin(SPI_SLAVE3_PIN);	
		break;
	}
	
}

void spi_releaseSlave(spislave_t a_spiSlave){
	
	switch (a_spiSlave){
		
		case SP_SLAVE0: gpio_setPin(SPI_SLAVE0_PIN);	
		break;
		
		case SP_SLAVE1: gpio_setPin(SPI_SLAVE1_PIN);
		break;
		
		case SP_SLAVE2: gpio_setPin(SPI_SLAVE2_PIN);
		break;
		
		case SP_SLAVE3: gpio_setPin(SPI_SLAVE3_PIN);
		break;
	}
	
}

void spi_transmitByte(ubyte_t a_byte){
	
	SPDR = a_byte;
	while(!RBI(SPSR,SPIF));
	
}

ubyte_t spi_receiveByte(void){
	
	while(!RBI(SPSR,SPIF));
	return SPDR;
	
}

ubyte_t spi_transreceive(ubyte_t a_byte){
	
	SPDR = a_byte;
	while(!RBI(SPSR,SPIF));
	return SPDR;
	
}

void spi_enableInterrupt(void){
	
	if(!RBI(SREG,I_BIT))
		ENABLE_GLOBAL_INTERRUPTS;
	SBI(SPCR,SPIE);
	
}

void spi_disableInterrupt(void){
	
	CBI(SPCR,SPIE);
	
}

void spi_setISRCallback(ISRcallback_t a_spiISRCallback){
	
	g_spiISRCallback = a_spiISRCallback;
	
}

ISR(SPI_STC_vect){
	
	g_spiISRCallback();
	
}