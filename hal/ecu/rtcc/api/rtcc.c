/*
 * RTCC.c
 *
 * Created: 07/08/2019 12:27:36 PM
 *  Author: Loay Ashraf
 */ 

#include "rtcc.h"
#include "hal/ecu/rtcc/common/rtcc_config.h"
#include "hal/ecu/rtcc/common/rtcc_macros.h"
#include <stdio.h>

void RTCC_init(void){
	
	RTCC_INIT;
	
}

void RTCC_enable(void){
	
	RTCC_ENABLE;
	
}

void RTCC_setClockMode(rtccclockmode_t a_clockMode){
	
	RTCC_SET_CLOCK_MODE(a_clockMode);
	
}

void RTCC_setClock(rtccclock_t a_clock){

	RTCC_SET_CLOCK(a_clock);
	
}

void RTCC_setCalendar(rtcccalendar_t a_calendar){
	
	a_calendar.year -= RTCC_MILLENNIUM;
	
	RTCC_SET_CALENDAR(a_calendar);
	
}

rtccclock_t RTCC_getClock(void){
	
	return RTCC_GET_CLOCK;
	
}

char * RTCC_getClockStr(void){
	
	rtccclock_t clock;
	static char clockStr[12];
	
	clock = RTCC_GET_CLOCK;
	
	switch(clock.AMPM){
		
		case RT_AM: sprintf(clockStr,RTCC_CLOCK_FORMAT,clock.hour,clock.minute,clock.second,"AM");
		break;
		
		case RT_PM: sprintf(clockStr,RTCC_CLOCK_FORMAT,clock.hour,clock.minute,clock.second,"PM");
		break;
		
		default: sprintf(clockStr,RTCC_CLOCK_FORMAT,clock.hour,clock.minute,clock.second," ");
		break;
		
	}
	
	clockStr[11] = '\0';
	return clockStr;
}

rtcccalendar_t RTCC_getCalendar(void){
	
	rtcccalendar_t calendar;
	
	calendar = RTCC_GET_CALENDAR;
	calendar.year += RTCC_MILLENNIUM;
	
	return calendar;
	
}

void RTCC_setAlarm(rtccalarm_t a_alarm, rtccalarmconfig_t a_alarmConfig){
	
	RTCC_SET_ALARM(a_alarm,a_alarmConfig);
	
}

void RTCC_enableAlarm(rtccalarm_t a_alarm){
	
	RTCC_ENABLE_ALARM(a_alarm);
	
}

void RTCC_disableAlarm(rtccalarm_t a_alarm){
	
	RTCC_DISABLE_ALARM(a_alarm);
	
}