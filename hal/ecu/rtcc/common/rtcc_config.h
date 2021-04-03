/**********************************************************************
*
* File:         rtcc_config.h
*
* Author(s):	Loay Ashraf <loay.ashraf.96@gmail.com>
*
* Date created: 13/09/2019
*
* Description:	contains definitions and configurations for RTCC module.
*
**********************************************************************/

#ifndef RTCC_CONFIG_H_
#define RTCC_CONFIG_H_

/*------------------------------INCLUDES-----------------------------*/

#include "hal/ecu/rtcc/driver/rtcc_driver.h"

/*-------------------DEFINITIONS AND CONFIGURATIONS------------------*/

#define RTCC_CONTROLLER		RTCC_DS3231
#define RTCC_MILLENNIUM		2000
#define RTCC_CLOCK_FORMAT	"%02d:%02d:%02d %s"

#endif /* RTCC_CONFIG_H_ */