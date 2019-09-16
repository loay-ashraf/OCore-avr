/** @file Keypad.h
 *  @brief This driver supports interfacing of parallel interface character
 *	LCD (both 16x2 and 20x4); this file includes function prototypes for LCD.c
 *  @author Loay Ashraf
 *	@version 1.0.0
 *  @pre include delay.h and stdlib.h in micro_config.h
 *  @bug no known bugs
 *	@warning data mode has to be selected here
 *  @see micro_config.h
 */

#ifndef KEYPAD_H_
#define KEYPAD_H_

//------------INCLUDE MCU CONFIGURATION------------//

#include "../../config/ecu_depend.h"
#include "keypad_config.h"

typedef void (*keyhandler_t) (char);

//------------FUNCTION PROTOTYPES------------//

void Keypad_init(void);
void Keypad_setKeyMap(char a_keyMap[KEYPAD_ROWS][KEYPAD_COLUMNS]);
bool_t Keypad_enablePeriodicScan(keyhandler_t a_keyHandlerCallback, uint8_t a_scanFrequency);
void Keypad_disablePeriodicScan(void);
char Keypad_scan(void);

#endif /* KEYPAD_H_ */