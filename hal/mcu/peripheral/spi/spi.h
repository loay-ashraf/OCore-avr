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
 
#include "hal/ddp/ddp_interface.h"
#include PATH_TO_SPI

#define SPI_CONFIG(SPI,SPI_CONFIG)					spi_config(SPI,SPI_CONFIG)
#define SPI_ENABLE(SPI)								spi_enable(SPI)
#define SPI_DISABLE(SPI)							spi_disable(SPI)
#define SPI_SELECT_SLAVE(SPI,SPI_SLAVE)				spi_selectSlave(SPI,SPI_SLAVE)
#define SPI_RELEASE_SLAVE(SPI,SPI_SLAVE)			spi_releaseSlave(SPI,SPI_SLAVE)
#define SPI_TRANSMIT_BYTE(SPI,BYTE)					spi_transmitByte(SPI,BYTE)
#define SPI_RECEIVE_BYTE(SPI)						spi_receiveByte(SPI)
#define SPI_TRANSRECEIVE(SPI,BYTE)					spi_transReceive(SPI,BYTE)
#define SPI_ENABLE_INTERRUPT(SPI)					spi_enableInterrupt(SPI)
#define SPI_DISABLE_INTERRUPT(SPI)					spi_disableInterrupt(SPI)
#define SPI_SET_ISR_CALLBACK(SPI,SPI_ISR_CALLBACK)	spi_setISRCallback(SPI,SPI_ISR_CALLBACK)

#endif /* SPI_H_ */
