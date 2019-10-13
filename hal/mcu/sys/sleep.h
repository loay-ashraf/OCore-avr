/**********************************************************************
*
* File:         sleep.h
*
* Author(s):	Loay Ashraf <loay.ashraf.96@gmail.com>
*
* Date created: 12/05/2019
*
* Description:	contains CPU sleep instructions macros.
*
**********************************************************************/

#ifndef SLEEP_H_
#define SLEEP_H_

/*------------------------------INCLUDES-----------------------------*/

#include <avr/sleep.h>

/*-------------------------------MACROS------------------------------*/

#define SET_SLEEP_MODE(MODE)	set_sleep_mode(MODE)
#define SLEEP_MODE				sleep_mode()
#define SLEEP_ENABLE			sleep_enable()
#define SLEEP_DISABLE			sleep_disable()
#define SLEEP_CPU				sleep_cpu()

#endif /* SLEEP_H_ */