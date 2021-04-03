/**********************************************************************
*
* File:         rtcc.c
*
* Author(s):    Loay Ashraf <loay.ashraf.96@gmail.com>
*
* Date created: 07/08/2019
*
* Description:	contains function definitions for RTCC module.
*
**********************************************************************/

/*------------------------------INCLUDES-----------------------------*/

#include "rtcc.h"
#include "hal/ecu/rtcc/common/rtcc_macros.h"
#include <stdio.h>

/*-----------------------FUNCTION DEFINITIONS------------------------*/

/**********************************************************************
*
* Function:    RTCC_init
*
* Description: Initializes RTCC module.
*
* Notes:       This function is called only when using new RTCC module.
*
* Returns:     None.
*
**********************************************************************/

void RTCC_init(void){
	
	RTCC_INIT;
	
}

/**********************************************************************
*
* Function:    RTCC_enable
*
* Description: Enables RTCC module.
*
* Notes:
*
* Returns:     None.
*
**********************************************************************/

void RTCC_enable(void){
	
	RTCC_ENABLE;
	
}

/**********************************************************************
*
* Function:    RTCC_setClockMode
*
* Description: Sets clock mode (12H/24H) for RTCC module.
*
* Notes:
*
* Returns:     None.
*
**********************************************************************/

void RTCC_setClockMode(rtccclockmode_t a_clockMode){
	
	RTCC_SET_CLOCK_MODE(a_clockMode);
	
}

/**********************************************************************
*
* Function:    RTCC_setClock
*
* Description: Sets clock for RTCC module.
*
* Notes:
*
* Returns:     None.
*
**********************************************************************/

void RTCC_setClock(rtccclock_t a_clock){

	RTCC_SET_CLOCK(a_clock);
	
}

/**********************************************************************
*
* Function:    RTCC_setCalendar
*
* Description: Sets calendar for RTCC module.
*
* Notes:
*
* Returns:     None.
*
**********************************************************************/

void RTCC_setCalendar(rtcccalendar_t a_calendar){
	
	a_calendar.year -= RTCC_MILLENNIUM;
	
	RTCC_SET_CALENDAR(a_calendar);
	
}

/**********************************************************************
*
* Function:    RTCC_getClock
*
* Description: Gets clock from RTCC module.
*
* Notes:
*
* Returns:     Current clock (type: rtccclock_t).
*
**********************************************************************/

rtccclock_t RTCC_getClock(void){
	
	return RTCC_GET_CLOCK;
	
}

/**********************************************************************
*
* Function:    RTCC_getClockStr
*
* Description: Gets clock as formated string from RTCC module.
*
* Notes:
*
* Returns:     Current clock (type: char *).
*
**********************************************************************/

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

/**********************************************************************
*
* Function:    RTCC_getCalendar
*
* Description: Gets calendar from RTCC module.
*
* Notes:
*
* Returns:     Current calendar (type: rtcccalendar_t).
*
**********************************************************************/

rtcccalendar_t RTCC_getCalendar(void){
	
	rtcccalendar_t calendar;
	
	calendar = RTCC_GET_CALENDAR;
	calendar.year += RTCC_MILLENNIUM;
	
	return calendar;
	
}

/**********************************************************************
*
* Function:    RTCC_setAlarm
*
* Description: Sets alarm for RTCC module.
*
* Notes:
*
* Returns:     None.
*
**********************************************************************/

void RTCC_setAlarm(rtccalarm_t a_alarm, rtccalarmconfig_t a_alarmConfig){
	
	RTCC_SET_ALARM(a_alarm,a_alarmConfig);
	
}

/**********************************************************************
*
* Function:    RTCC_enableAlarm
*
* Description: Enables alarm for RTCC module.
*
* Notes:       This function enables global interrupts if disabled.
*
* Returns:     None.
*
**********************************************************************/

void RTCC_enableAlarm(rtccalarm_t a_alarm){
	
	RTCC_ENABLE_ALARM(a_alarm);
	
}

/**********************************************************************
*
* Function:    RTCC_disableAlarm
*
* Description: Disables alarm for RTCC module.
*
* Notes:       This function doesn't disable global interrupts.
*
* Returns:     None.
*
**********************************************************************/

void RTCC_disableAlarm(rtccalarm_t a_alarm){
	
	RTCC_DISABLE_ALARM(a_alarm);
	
}