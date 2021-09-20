/**********************************************************************
*
* File:         keypad_config.h
*
* Author(s):    Loay Ashraf <loay.ashraf.96@gmail.com>
*
* Date created: 11/09/2019
*
* Description:  contains definitions and configurations for
*               matrix keypad module.
*
**********************************************************************/

#ifndef KEYPAD_CONFIG_H_
#define KEYPAD_CONFIG_H_

/*-------------------DEFINITIONS AND CONFIGURATIONS------------------*/

#define KEYPAD_ROWS                  4
#define KEYPAD_COLUMNS               4

#define KEYPAD_ROW_PORT              PORTC_M
#define KEYPAD_COLUMN_PORT           PORTD_M

#define KEYPAD_ROW_PORT_MASK         0xF0
#define KEYPAD_COLUMN_PORT_MASK      0xF0
#define KEYPAD_ROW_PORT_OFFSET       (uint8_t)(__builtin_ffs(KEYPAD_ROW_PORT_MASK)-1)
#define KEYPAD_COLUMN_PORT_OFFSET    (uint8_t)(__builtin_ffs(KEYPAD_COLUMN_PORT_MASK)-1)
#define KEYPAD_CONFIG                -1    /* 1 for positive logic (pull-down configuration), -1 for negative logic (pull-up configuration) */

#define    KEYPAD_SCAN_TIMER         TIMER1_M
#define    KEYPAD_SCAN_TIMER_PRE     T16_DIV64
#define    KEYPAD_SCAN_TIMER_MODE    T16_CTC_ICR
#define    KEYPAD_SCAN_TIMER_INT     T16_CAPT

#endif /* KEYPAD_CONFIG_H_ */
