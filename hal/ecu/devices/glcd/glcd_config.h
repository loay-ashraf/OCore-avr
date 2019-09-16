/*
 * glcd_config.h
 *
 * Created: 10/09/2019 07:08:55 AM
 *  Author: Loay Ashraf
 */ 


#ifndef GLCD_CONFIG_H_
#define GLCD_CONFIG_H_

/*------------DEFINE GGLCD DIMENSIONS------------*/

#define GLCD_ROWS			4
#define GLCD_COLUMNS		16

/*------------DEFINE GGLCD INTERFACE MODE------------*/

#define GLCD_PSB_PIN		PA5_M
/*#define GLCD_RST_PIN		PA1_M*/		// uncomment this if RST pin is connected
/*#define GLCD_RW_PIN		PA3_M*/		// uncomment this if RW pin is connected
#define GLCD_EN_PIN			PA6_M
#define GLCD_RS_PIN			PA7_M
#define GLCD_DATA_PORT		PORTB_M

#define GLCD_INTERFACE		1
#define GLCD_DATA_MODE		8
#if (GLCD_DATA_MODE == 8)
#define GLCD_DATA_PORT_MASK	0XFF
#else
#define GLCD_DATA_PORT_MASK	0xF0
#endif

/*------------DEFINE INTEGER AND FLOAT BUFFER SIZE------------*/

#define GLCD_BUFFER_SIZE		20
#define GLCD_FRACTION_SIZE		3


#endif /* GLCD_CONFIG_H_ */