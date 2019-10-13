/**********************************************************************
*
* File:         delay.h
*
* Author(s):	Loay Ashraf <loay.ashraf.96@gmail.com>
*
* Date created: 15/05/2019
*
* Description:	contains delay macros (in ms and us).
*
**********************************************************************/

#ifndef DELAY_H_
#define DELAY_H_

/*------------------------------INCLUDES-----------------------------*/

#include "cpu_config.h"
#include <util/delay.h>

/*-------------------------------MACROS------------------------------*/

#define DELAY_MS(MS)	_delay_ms(MS) 
#define DELAY_US(US)	_delay_us(US)

#endif /* DELAY_H_ */