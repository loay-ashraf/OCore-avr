/*
 * glcd_config.h
 *
 * Created: 10/09/2019 07:08:55 AM
 *  Author: Loay Ashraf
 */ 


#ifndef GLCD_CONFIG_H_
#define GLCD_CONFIG_H_

/*------------DEFINE GLCD DIMENSIONS------------*/

#define GLCD_ROWS			4
#define GLCD_COLUMNS		16

/*------------DEFINE GLCD PIXEL COUNT------------*/

#define GLCD_PIXELS_X		128
#define GLCD_PIXELS_Y		64

/*------------DEFINE INTEGER AND FLOAT BUFFER SIZE------------*/

#define GLCD_BUFFER_SIZE	20
#define GLCD_FRACTION_SIZE	3

/*------------CONFIGURE SW POSITION CORRECTION OF CURSOR------------*/

#define GLCD_CURSOR_SW_SHFT	1


#endif /* GLCD_CONFIG_H_ */