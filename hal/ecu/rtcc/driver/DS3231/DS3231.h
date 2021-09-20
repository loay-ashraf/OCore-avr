/**********************************************************************
*
* File:         DS3231.h
*
* Author(s):    Loay Ashraf <loay.ashraf.96@gmail.com>
*
* Date created: 04/10/2019
*
* Description:  contains function declarations for DS3231
*               controller module.
*
**********************************************************************/

#ifndef DS3231_H_
#define DS3231_H_

/*------------------------------INCLUDES-----------------------------*/

#include "hal/ecu/rtcc/common/rtcc_types.h"

/*------------------------FUNCTION DECLARATIONS----------------------*/

void DS3231_init(void);
void DS3231_enable(void);
void DS3231_setClockMode(rtccclockmode_t a_clockMode);
void DS3231_setClock(rtccclock_t a_clock);
void DS3231_setCalendar(rtcccalendar_t a_calendar);
rtccclock_t DS3231_getClock(void);
rtcccalendar_t DS3231_getCalendar(void);
void DS3231_setAlarm(rtccalarm_t a_alarm, rtccalarmconfig_t a_alarmConfig);
void DS3231_enableAlarm(rtccalarm_t a_alarm);
void DS3231_disableAlarm(rtccalarm_t a_alarm);
float DS3231_getTempCelsius(void);
float DS3231_getTempFahrenheit(void);

#endif /* DS3231_H_ */
