/**********************************************************************
*
* File:         m16_spi.c
*
* Author(s):    Loay Ashraf <loay.ashraf.96@gmail.com>
*
* Date created: 14/06/2021
*
* Description:	contains function definitions for serial peripheral 
*               interface module.
*
**********************************************************************/ 

/*------------------------------INCLUDES-----------------------------*/

#include "m16_spi.h"
#include "hal/ddp/avr8/mega/m16/gpio/m16_gpio.h"
#include "hal/ddp/avr8/mega/m16/m16_interrupt.h"
#include "service/include/register_macros.h"

/*--------------------------GLOBAL VARIABLES-------------------------*/

/**********************************************************************
*
* Variable:    g_spiISRCallback
*
* Description: Holds address of interrupt callback function.
*
* Notes:
*
* Scope:       m16_spi.c.
*
**********************************************************************/

static ISRcallback_t g_spiISRCallback;

/*-----------------------FUNCTION DEFINITIONS------------------------*/

/**********************************************************************
*
* Function:    spi_config
*
* Description: Configures serial peripheral interface module.
*
* Notes:
*
* Returns:     None.
*
**********************************************************************/

void spi_config(spi_t a_spi, spiconfig_t * a_spiConfig){
	
	SRI(SPCR,(a_spiConfig->mode<<MSTR)|(a_spiConfig->prescaler<<SPR0)|(a_spiConfig->dataorder<<DORD)|(a_spiConfig->datamode<<CPHA));
	SRI(SPSR,(a_spiConfig->speedmode<<SPI2X));
	
	if(a_spiConfig->mode == SPI_MASTER){
		
		gpio_setPinDirection(SPI0_MOSI_PIN,IO_OUTPUT);
		gpio_setPinDirection(SPI0_MISO_PIN,IO_INPUT);
		gpio_setPinDirection(SPI0_SCK_PIN,IO_OUTPUT);
		gpio_setPinDirection(SPI0_SS_PIN,IO_OUTPUT);
		gpio_setPinDirection(SPI_SLAVE0_PIN,IO_OUTPUT);
		gpio_setPinDirection(SPI_SLAVE1_PIN,IO_OUTPUT);
		gpio_setPinDirection(SPI_SLAVE2_PIN,IO_OUTPUT);
		gpio_setPinDirection(SPI_SLAVE3_PIN,IO_OUTPUT);
		
	}else if(a_spiConfig->mode == SPI_SLAVE){
		
		gpio_setPinDirection(SPI0_MOSI_PIN,IO_INPUT);
		gpio_setPinDirection(SPI0_MISO_PIN,IO_OUTPUT);
		gpio_setPinDirection(SPI0_SCK_PIN,IO_INPUT);
		gpio_setPinDirection(SPI0_SS_PIN,IO_INPUT);
		
	}
	
}

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

void spi_enable(spi_t a_spi){
	
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

void spi_disable(spi_t a_spi){
	
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

void spi_selectSlave(spi_t a_spi, spislave_t a_spiSlave){
	
	switch (a_spiSlave){
		
		case SPI_SLAVE0: gpio_clearPin(SPI_SLAVE0_PIN);
		break;
		
		case SPI_SLAVE1: gpio_clearPin(SPI_SLAVE1_PIN);
		break;
		
		case SPI_SLAVE2: gpio_clearPin(SPI_SLAVE2_PIN);
		break;
		
		case SPI_SLAVE3: gpio_clearPin(SPI_SLAVE3_PIN);	
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

void spi_releaseSlave(spi_t a_spi, spislave_t a_spiSlave){
	
	switch (a_spiSlave){
		
		case SPI_SLAVE0: gpio_setPin(SPI_SLAVE0_PIN);	
		break;
		
		case SPI_SLAVE1: gpio_setPin(SPI_SLAVE1_PIN);
		break;
		
		case SPI_SLAVE2: gpio_setPin(SPI_SLAVE2_PIN);
		break;
		
		case SPI_SLAVE3: gpio_setPin(SPI_SLAVE3_PIN);
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

void spi_transmitByte(spi_t a_spi, ubyte_t a_byte){
	
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

ubyte_t spi_receiveByte(spi_t a_spi){
	
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

ubyte_t spi_transreceive(spi_t a_spi, ubyte_t a_byte){
	
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

void spi_enableInterrupt(spi_t a_spi)
{
	
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

void spi_disableInterrupt(spi_t a_spi)
{
	
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

void spi_setISRCallback(spi_t a_spi, ISRcallback_t a_spiISRCallback){
	
	g_spiISRCallback = a_spiISRCallback;
	
}

/*---------------------------------ISR-------------------------------*/

ISR(SPI_STC_vect){
	
	g_spiISRCallback();
	
}
