/**********************************************************************
*
* File:         spi.h
*
* Author(s):	Loay Ashraf <loay.ashraf.96@gmail.com>
*
* Date created: 05/09/2019
*
* Description:	contains definitions, configurations, typedefs
*               and function declarations for serial peripheral 
*               interface module.
*
**********************************************************************/

#ifndef SPI_H_
#define SPI_H_

/*------------------------------INCLUDES-----------------------------*/

#include "hal/mcu/io/io_defs.h"
#include "service/include/std_types.h"

/*-------------------DEFINITIONS AND CONFIGURATIONS------------------*/

#define SPI0_MOSI_PIN	PB5_M
#define SPI0_MISO_PIN	PB6_M
#define SPI0_SCK_PIN	PB7_M
#define SPI0_SS_PIN		PB4_M

/******************************************/
/* comment section if configured as slave */
/******************************************/

#define SPI_SLAVE0_PIN	PD0_M
#define SPI_SLAVE1_PIN	PD1_M
#define SPI_SLAVE2_PIN	PD2_M
#define SPI_SLAVE3_PIN	PD3_M

/*--------------------------------ENUMS------------------------------*/

typedef enum{SPI0_M}spi_t;
typedef enum{SPI_SLAVE,SPI_MASTER}spimode_t;
typedef enum{SPI_DIV4,SPI_DIV16,SPI_DIV64,SPI_DIV128}spiprescaler_t;
typedef enum{SPI_NORMAL,SPI_2X}spispeedmode_t;
typedef enum{SPI_MSB_FIRST,SPI_LSB_FIRST}spidataorder_t;
typedef enum{SPI_MODE0,SPI_MODE1,SPI_MODE2,SPI_MODE3}spidatamode_t;
typedef enum{SPI_SLAVE0,SPI_SLAVE1,SPI_SLAVE2,SPI_SLAVE3}spislave_t;
	
/*-------------------------------STRUCTS-----------------------------*/

typedef struct{
	
	spimode_t mode;
	spiprescaler_t prescaler;
	spispeedmode_t speedmode;
	spidataorder_t dataorder;
	spidatamode_t datamode;
	
}spiconfig_t;	

/*------------------------FUNCTION DECLARATIONS----------------------*/

void spi_config(spi_t a_spi, spiconfig_t * a_spiConfig);					
void spi_enable(spi_t a_spi);
void spi_disable(spi_t a_spi);
void spi_selectSlave(spi_t a_spi, spislave_t a_spiSlave);
void spi_releaseSlave(spi_t a_spi, spislave_t a_spiSlave);
void spi_transmitByte(spi_t a_spi, ubyte_t a_byte);
ubyte_t spi_receiveByte(spi_t a_spi);
ubyte_t spi_transreceive(spi_t a_spi, ubyte_t a_byte);
void spi_enableInterrupt(spi_t a_spi);
void spi_disableInterrupt(spi_t a_spi);
void spi_setISRCallback(spi_t a_spi, ISRcallback_t a_spiISRCallback);

#endif /* SPI_H_ */