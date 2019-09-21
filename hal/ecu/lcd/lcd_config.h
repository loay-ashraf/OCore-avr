/*
 * lcd_config.h
 *
 * Created: 09/09/2019 07:14:45 AM
 *  Author: Loay Ashraf
 */ 


#ifndef LCD_CONFIG_H_
#define LCD_CONFIG_H_

/*------------DEFINE LCD DIMENSIONS------------*/

#define LCD_ROWS			4
#define LCD_COLUMNS			20

/*------------DEFINE LCD INTERFACE MODE------------*/

/*#define LCD_RW_PIN			PA3_M*/		// uncomment this if RW pin is connected
#define LCD_EN_PIN			PA6_M
#define LCD_RS_PIN			PA7_M
#define LCD_DATA_PORT		PORTB_M

#define LCD_DATA_MODE		8
#if (LCD_DATA_MODE == 8)
#define LCD_DATA_PORT_MASK	0XFF
#else
#define LCD_DATA_PORT_MASK	0xF0
#endif

/*------------DEFINE INTEGER AND FLOAT BUFFER SIZE------------*/

#define LCD_BUFFER_SIZE		20
#define LCD_FRACTION_SIZE	3

#endif /* LCD_CONFIG_H_ */