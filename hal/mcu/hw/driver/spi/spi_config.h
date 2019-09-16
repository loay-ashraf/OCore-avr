/*
 * spi_config.h
 *
 * Created: 05/09/2019 02:41:39 AM
 *  Author: Loay Ashraf
 */ 


#ifndef SPI_CONFIG_H_
#define SPI_CONFIG_H_

#define SPI_MOSI_PIN	PB5_M
#define SPI_MISO_PIN	PB6_M
#define SPI_SCK_PIN		PB7_M
#define SPI_SS_PIN		PB4_M

#define SPI_MASTER		TRUE
#define SPI_PRESCALER	128
#define SPI_2X_MODE		FALSE
#define SPI_LSB_FRST	FALSE
#define SPI_DATA_MODE	3

/************************************************************************/
/* comment section if configured as slave                               */
/************************************************************************/

#define SPI_SLAVE0_PIN	PD0_M
#define SPI_SLAVE1_PIN	PD1_M
#define SPI_SLAVE2_PIN	PD2_M
#define SPI_SLAVE3_PIN	PD3_M

#endif /* SPI_CONFIG_H_ */