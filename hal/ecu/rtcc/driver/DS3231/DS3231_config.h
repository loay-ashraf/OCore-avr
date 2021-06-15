/**********************************************************************
*
* File:         DS3231_config.h
*
* Author(s):    Loay Ashraf <loay.ashraf.96@gmail.com>
*
* Date created: 04/10/2019
*
* Description:  contains definitions and configurations for
*               DS3231 controller module.
*
**********************************************************************/

#ifndef DS3231_CONFIG_H_
#define DS3231_CONFIG_H_

/*-------------------DEFINITIONS AND CONFIGURATIONS------------------*/

#define DS3231_ADDRESS           0b01101000
#define DS3231_ALARM_INT         IO_INT0
#define DS3231_ALARM_INT_MODE    IO_FALLING_EDGE
#define DS3231_ALARM_INT_PIN     GPIO_INT0_PIN

#endif /* DS3231_CONFIG_H_ */
