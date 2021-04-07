/**********************************************************************
*
* File:         ST7920_config.h
*
* Author(s):	Loay Ashraf <loay.ashraf.96@gmail.com>
*
* Date created: 25/09/2019
*
* Description:	contains definitions and configurations for 
*               ST7920 controller module.
*
**********************************************************************/

#ifndef ST7920_CONFIG_H_
#define ST7920_CONFIG_H_

/*-------------------DEFINITIONS AND CONFIGURATIONS------------------*/

#define ST7920_X_PAGE_SIZE		8
#define ST7920_Y_PAGE_SIZE		1
#define ST7920_BL_PIN			PA3_M
#define ST7920_PSB_PIN			PA5_M
/*#define ST7920_RST_PIN			PA1_M*/			/* uncomment this if RST pin is connected */
/*#define ST7920_RW_PIN			SPI_MOSI_PIN*/		/* uncomment this if RW pin is connected */
#define ST7920_EN_PIN			PA6_M
#define ST7920_RS_PIN			PA7_M
#define ST7920_DATA_PORT		PORTB_M
#define ST7920_INTERFACE		1		/* 1 for parallel interface and 0 for serial interface */
#define ST7920_DATA_MODE		8
#if (ST7920_DATA_MODE == 8)
#define ST7920_DATA_PORT_MASK	0XFF
#else
#define ST7920_DATA_PORT_MASK	0xF0
#endif
#define ST7920_BL_MODE			1		/* 1 for non-inverted mode and 0 for inverted mode */

#endif /* ST7920_CONFIG_H_ */