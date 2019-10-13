/**********************************************************************
*
* File:         spi.c
*
* Author(s):    Loay Ashraf <loay.ashraf.96@gmail.com>
*
* Date created: 05/09/2019
*
* Description:	contains function definitions for serial peripheral 
*               interface module.
*
**********************************************************************/ 

/*------------------------------INCLUDES-----------------------------*/

#include "spi.h"
#include "hal/mcu/hw/driver/gpio/gpio.h"
#include "hal/mcu/io/io_macros.h"
#include "hal/mcu/sys/interrupt.h"

/*--------------------------GLOBAL VARIABLES-------------------------*/

/**********************************************************************
*
* Variable:    g_spiISRCallback
*
* Description: Holds address of interrupt callback function.
*
* Notes:
*
* Scope:       spi.c.
*
**********************************************************************/

static ISRcallback_t g_spiISRCallback;

/*-----------------------FUNCTION DEFINITIONS------------------------*/

/**********************************************************************
*
* Function:    spi_enable
*
* Description: Enables serial peripheral interface module.
*
* Notes:
*
* Returns:     None.
*
**********************************************************************/

void spi_enable(void){
	
	SRI(SPCR,(SPI_MASTER<<MSTR));
	
	#if (SPI_MASTER)
		
		SBI(SPCR,MSTR);
		gpio_setPinDirection(SPI_MOSI_PIN,IO_OUTPUT);
		gpio_setPinDirection(SPI_SCK_PIN,IO_OUTPUT);
		gpio_enablePinPullUp(SPI_SS_PIN);
		
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

/**********************************************************************
*
* Function:    spi_disable
*
* Description: Disables serial peripheral interface module.
*
* Notes:
*
* Returns:     None.
*
**********************************************************************/

void spi_disable(void){
	
	CBI(SPCR,SPE);
	
}

/**********************************************************************
*
* Function:    spi_selectSlave
*
* Description: Selects a slave by pulling corresponding pin LOW.
*
* Notes:
*
* Returns:     None.
*
**********************************************************************/

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

/**********************************************************************
*
* Function:    spi_releaseSlave
*
* Description: Releases a slave by pulling corresponding pin HIGH.
*
* Notes:
*
* Returns:     None.
*
**********************************************************************/

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

/**********************************************************************
*
* Function:    spi_transmitByte
*
* Description: Transmits byte via serial peripheral interface module.
*
* Notes:
*
* Returns:     None.
*
**********************************************************************/

void spi_transmitByte(ubyte_t a_byte){
	
	SPDR = a_byte;
	while(!RBI(SPSR,SPIF));
	
}

/**********************************************************************
*
* Function:    spi_receiveByte
*
* Description: Receives byte via serial peripheral interface module.
*
* Notes:
*
* Returns:     SPDR register value.
*
**********************************************************************/

ubyte_t spi_receiveByte(void){
	
	while(!RBI(SPSR,SPIF));
	return SPDR;
	
}

/**********************************************************************
*
* Function:    spi_transreceive
*
* Description: Transmits and receives bytes simultaneously via
*              serial peripheral interface module.
*
* Notes:
*
* Returns:     SPDR register value.
*
**********************************************************************/

ubyte_t spi_transreceive(ubyte_t a_byte){
	
	SPDR = a_byte;
	while(!RBI(SPSR,SPIF));
	return SPDR;
	
}

/**********************************************************************
*
* Function:    spi_enableInterrupt
*
* Description: Enables transfer complete interrupt request for 
*              serial peripheral interface module.
*
* Notes:       This function enables global interrupts if disabled.
*
* Returns:     None.
*
**********************************************************************/

void spi_enableInterrupt(void){
	
	if(!RBI(SREG,I_BIT))
		ENABLE_GLOBAL_INTERRUPTS;
	SBI(SPCR,SPIE);
	
}

/**********************************************************************
*
* Function:    spi_disableInterrupt
*
* Description: Disables transfer complete interrupt request for
*              serial peripheral interface module.
*
* Notes:       This function doesn't disable global interrupts.
*
* Returns:     None.
*
**********************************************************************/

void spi_disableInterrupt(void){
	
	CBI(SPCR,SPIE);
	
}

/**********************************************************************
*
* Function:    spi_setISRCallback
*
* Description: Sets transfer complete interrupt callback function 
*              for serial peripheral interface module.
*
* Notes:
*
* Returns:     None.
*
**********************************************************************/

void spi_setISRCallback(ISRcallback_t a_spiISRCallback){
	
	g_spiISRCallback = a_spiISRCallback;
	
}

/*---------------------------------ISR-------------------------------*/

ISR(SPI_STC_vect){
	
	g_spiISRCallback();
	
}