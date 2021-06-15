/**********************************************************************
*
* File:         glcd_config.h
*
* Author(s):    Loay Ashraf <loay.ashraf.96@gmail.com>
*
* Date created: 10/09/2019
*
* Description:  contains definitions and configurations for GLCD module.
*
**********************************************************************/

#ifndef GLCD_CONFIG_H_
#define GLCD_CONFIG_H_

/*------------------------------INCLUDES-----------------------------*/

#include "hal/ecu/glcd/driver/glcd_driver.h"

/*-------------------DEFINITIONS AND CONFIGURATIONS------------------*/

#define GLCD_CONTROLLER       GLCD_ST7920
#define GLCD_X_PIXELS         128
#define GLCD_Y_PIXELS         64
#define GLCD_BUFFER_SIZE      20
#define GLCD_FRACTION_SIZE    3

#endif /* GLCD_CONFIG_H_ */
