/**********************************************************************
*
* File:         keypad.h
*
* Author(s):	Loay Ashraf <loay.ashraf.96@gmail.com>
*
* Date created: 15/01/2018
*
* Description:	contains typedefs and function declarations for
*               matrix keypad module.
*
**********************************************************************/

#ifndef KEYPAD_H_
#define KEYPAD_H_

/*------------------------------INCLUDES-----------------------------*/

#include "keypad_config.h"
#include "hal/mcu/sys/std_types.h"

/*--------------------------FUNCTION POINTERS------------------------*/

typedef void (*keyhandler_t) (char);

/*------------------------FUNCTION DECLARATIONS----------------------*/

void Keypad_init(void);
void Keypad_setKeyMap(char a_keyMap[KEYPAD_ROWS][KEYPAD_COLUMNS]);
void Keypad_enablePeriodicScan(keyhandler_t a_keyHandlerCallback, uint8_t a_scanFrequency);
void Keypad_disablePeriodicScan(void);
char Keypad_scan(void);

#endif /* KEYPAD_H_ */