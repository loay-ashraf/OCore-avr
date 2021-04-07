/**********************************************************************
*
* File:         HD44780_config.h
*
* Author(s):	Loay Ashraf <loay.ashraf.96@gmail.com>
*
* Date created: 23/09/2019
*
* Description:	contains definitions and configurations for 
*               HD44780 controller module.
*
**********************************************************************/

#ifndef HD44780_CONFIG_H_
#define HD44780_CONFIG_H_

/*-------------------DEFINITIONS AND CONFIGURATIONS------------------*/

#define HD44780_BL_PIN			PA3_M
/*#define HD44780_RW_PIN			PA4_M*/		/* uncomment this if RW pin is connected */
#define HD44780_EN_PIN			PA6_M
#define HD44780_RS_PIN			PA7_M
#define HD44780_DATA_PORT		PORTB_M
#define HD44780_DATA_MODE		8
#if (HD44780_DATA_MODE == 8)
#define HD44780_DATA_PORT_MASK	0XFF
#else
#define HD44780_DATA_PORT_MASK	0xF0
#endif
#define HD44780_BL_MODE			1		/* 1 for non-inverted mode and 0 for inverted mode */

#endif /* HD44780_CONFIG_H_ */