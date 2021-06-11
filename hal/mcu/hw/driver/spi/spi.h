/**********************************************************************
*
* File:         spi.h
*
* Author(s):	Loay Ashraf <loay.ashraf.96@gmail.com>
*
* Date created: 05/09/2019
*
* Description:	contains typedefs and function declarations for 
*               serial peripheral interface module.
*
**********************************************************************/

#ifndef SPI_H_
#define SPI_H_

/*------------------------------INCLUDES-----------------------------*/

#include "spi_config.h"
#include "hal/mcu/io/io_defs.h"
#include "service/include/std_types.h"

/*--------------------------------ENUMS------------------------------*/

typedef enum{SP_SLAVE0,SP_SLAVE1,SP_SLAVE2,SP_SLAVE3}spislave_t;

/*------------------------FUNCTION DECLARATIONS----------------------*/
					
void spi_enable(void);
void spi_disable(void);
void spi_selectSlave(spislave_t a_spiSlave);
void spi_releaseSlave(spislave_t a_spiSlave);
void spi_transmitByte(ubyte_t a_byte);
ubyte_t spi_receiveByte(void);
ubyte_t spi_transreceive(ubyte_t a_byte);
void spi_enableInterrupt(void);
void spi_disableInterrupt(void);
void spi_setISRCallback(ISRcallback_t a_spiISRCallback);

#endif /* SPI_H_ */