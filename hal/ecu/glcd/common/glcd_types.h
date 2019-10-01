/*
 * glcd_types.h
 *
 * Created: 30/09/2019 03:25:44 PM
 *  Author: Loay Ashraf
 */ 


#ifndef GLCD_TYPES_H_
#define GLCD_TYPES_H_

#include "glcd_config.h"
#include "hal/mcu/sys/std_types.h"

/*------------TYPEDEF SCROLL DIRECTION AND FONT ENUMS------------*/

typedef enum{GLCD_5X8}glcdfont_t;
typedef enum{GLCD_LEFT,GLCD_RIGHT,GLCD_UP,GLCD_DOWN}glcddirection_t;
typedef enum{GLCD_BASE10=10,GLCD_BASE16=16,GLCD_BASE2=2}glcdnumberbase_t;
typedef enum{GLCD_BAR0,GLCD_BAR1,GLCD_BAR2,GLCD_BAR3}glcdbarindex_t;


#endif /* GLCD_TYPES_H_ */