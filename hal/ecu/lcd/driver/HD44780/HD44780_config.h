/*
 * HD44780_config.h
 *
 * Created: 23/09/2019 05:13:42 PM
 *  Author: Loay Ashraf
 */ 


#ifndef HD44780_CONFIG_H_
#define HD44780_CONFIG_H_

/*------------DEFINE HD44780 DIMENSIONS------------*/

#define HD44780_ROWS			4
#define HD44780_COLUMNS			20

/*------------DEFINE HD44780 INTERFACE MODE------------*/

/*#define HD44780_RW_PIN			PA3_M*/		// uncomment this if RW pin is connected
#define HD44780_EN_PIN			PA6_M
#define HD44780_RS_PIN			PA7_M
#define HD44780_DATA_PORT		PORTB_M

#define HD44780_DATA_MODE		8
#if (HD44780_DATA_MODE == 8)
#define HD44780_DATA_PORT_MASK	0XFF
#else
#define HD44780_DATA_PORT_MASK	0xF0
#endif

/*------------DEFINE HD44780 CUSTOM CHARACTER CODES------------*/

#define HD44780_CCHAR_ZERO		0
#define HD44780_CCHAR_ONE		1
#define HD44780_CCHAR_TWO		2
#define HD44780_CCHAR_THREE		3
#define HD44780_CCHAR_FOUR		4
#define HD44780_CCHAR_FIVE		5
#define HD44780_CCHAR_SIX		6
#define HD44780_CCHAR_SEVEN		7


#endif /* HD44780_CONFIG_H_ */