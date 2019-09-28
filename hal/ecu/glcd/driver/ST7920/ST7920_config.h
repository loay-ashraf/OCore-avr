/*
 * ST7920_config.h
 *
 * Created: 25/09/2019 07:37:48 PM
 *  Author: Loay Ashraf
 */ 


#ifndef ST7920_CONFIG_H_
#define ST7920_CONFIG_H_

#include "hal/mcu/hw/driver/spi/spi_config.h"

/*------------DEFINE ST7920 INTERFACE MODE------------*/

#define ST7920_PSB_PIN			PA5_M
#define ST7920_RST_PIN			PA1_M				// uncomment this if RST pin is connected
#define ST7920_RW_PIN			SPI_MOSI_PIN		// uncomment this if RW pin is connected
#define ST7920_EN_PIN			SPI_SCK_PIN
#define ST7920_RS_PIN			PA4_M
#define ST7920_DATA_PORT		PORTB_M

#define ST7920_INTERFACE		0
#define ST7920_DATA_MODE		8
#if (ST7920_DATA_MODE == 8)
#define ST7920_DATA_PORT_MASK	0XFF
#else
#define ST7920_DATA_PORT_MASK	0xF0
#endif



#endif /* ST7920_CONFIG_H_ */