/**********************************************************************
*
* File:         lcd_config.h
*
* Author(s):	Loay Ashraf <loay.ashraf.96@gmail.com>
*
* Date created: 09/09/2019
*
* Description:	contains definitions and configurations for LCD module.
*
**********************************************************************/

#ifndef LCD_CONFIG_H_
#define LCD_CONFIG_H_

/*------------------------------INCLUDES-----------------------------*/

#include "hal/ecu/lcd/driver/lcd_driver.h"

/*-------------------DEFINITIONS AND CONFIGURATIONS------------------*/

#define LCD_CONTROLLER		LCD_HD44780
#define LCD_ROWS			4
#define LCD_COLUMNS			20
#define LCD_BUFFER_SIZE		20
#define LCD_FRACTION_SIZE	3

#endif /* LCD_CONFIG_H_ */