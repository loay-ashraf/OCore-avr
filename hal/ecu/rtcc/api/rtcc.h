/**********************************************************************
*
* File:         rtcc.h
*
* Author(s):	Loay Ashraf <loay.ashraf.96@gmail.com>
*
* Date created: 07/08/2019
*
* Description:	contains function declarations for RTCC module.
*
**********************************************************************/

#ifndef RTCC_H_
#define RTCC_H_

/*------------------------------INCLUDES-----------------------------*/

#include "hal/ecu/rtcc/common/rtcc_config.h"
#include "hal/ecu/rtcc/common/rtcc_types.h"

/*------------------------FUNCTION DECLARATIONS----------------------*/
	
void RTCC_init(void);
void RTCC_enable(void);
void RTCC_setClockMode(rtccclockmode_t a_clockMode);
void RTCC_setClock(rtccclock_t a_clock);	
void RTCC_setCalendar(rtcccalendar_t a_calendar);
rtccclock_t RTCC_getClock(void);
char * RTCC_getClockStr(void);
rtcccalendar_t RTCC_getCalendar(void);	
void RTCC_setAlarm(rtccalarm_t a_alarm, rtccalarmconfig_t a_alarmConfig);
void RTCC_enableAlarm(rtccalarm_t a_alarm);
void RTCC_disableAlarm(rtccalarm_t a_alarm);

#endif /* RTCC_H_ */
