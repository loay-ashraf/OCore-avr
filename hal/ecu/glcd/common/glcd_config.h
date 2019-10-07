/*
 * glcd_config.h
 *
 * Created: 10/09/2019 07:08:55 AM
 *  Author: Loay Ashraf
 */ 

#ifndef GLCD_CONFIG_H_
#define GLCD_CONFIG_H_

#include "hal/ecu/glcd/driver/glcd_driver.h"

/*------------DEFINE GLCD CONTROLLER------------*/

#define GLCD_CONTROLLER		GLCD_ST7920

/*------------DEFINE GLCD RESOLUTION------------*/

#define GLCD_X_PIXELS		128
#define GLCD_Y_PIXELS		64

/*------------DEFINE INTEGER AND FLOAT BUFFER SIZE------------*/

#define GLCD_BUFFER_SIZE	20
#define GLCD_FRACTION_SIZE	3

#endif /* GLCD_CONFIG_H_ */