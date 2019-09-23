/*
 * lcd_config.h
 *
 * Created: 09/09/2019 07:14:45 AM
 *  Author: Loay Ashraf
 */ 


#ifndef LCD_CONFIG_H_
#define LCD_CONFIG_H_

#include "hal/ecu/lcd/driver/HD44780/HD44780_config.h"

#if defined(HD44780_CONFIG_H_)
	
	/*------------DEFINE LCD DIMENSIONS------------*/
	
	#define LCD_ROWS			HD44780_ROWS
	#define LCD_COLUMNS			HD44780_COLUMNS
	
	/*------------DEFINE LCD CUSTOM CHARACTER CODES------------*/
	
	#define LCD_CCHAR_ZERO		HD44780_CCHAR_ZERO
	#define LCD_CCHAR_ONE		HD44780_CCHAR_ONE	
	#define LCD_CCHAR_TWO		HD44780_CCHAR_TWO
	#define LCD_CCHAR_THREE		HD44780_CCHAR_THREE	
	#define LCD_CCHAR_FOUR		HD44780_CCHAR_FOUR	
	#define LCD_CCHAR_FIVE		HD44780_CCHAR_FIVE	
	#define LCD_CCHAR_SIX		HD44780_CCHAR_SIX	
	#define LCD_CCHAR_SEVEN		HD44780_CCHAR_SEVEN	
	
#endif

/*------------DEFINE BUFFER AND FRACTION SIZE------------*/

#define LCD_BUFFER_SIZE		20
#define LCD_FRACTION_SIZE	3

#endif /* LCD_CONFIG_H_ */