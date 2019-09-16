/*
 * rtcc.h
 *
 * Created: 07/08/2019 12:27:51 PM
 *  Author: Loay Ashraf
 */ 


#ifndef RTCC_H_
#define RTCC_H_

#include "../../config/ecu_depend.h"
#include "rtcc_config.h"
#include <stdio.h>

typedef enum{TWELVE,TWENTY_FOUR}clockmode_t;
typedef enum{AM,PM=2}ampm_t;	
typedef enum{SAT=1,SUN,MON,TUE,WED,THU,FRI}weekday_t;
typedef enum{JAN=1,FEB,MAR,APR,MAY,JUN,JUL,AUG,SEP,OCT,NOV,DEC}month_t;
typedef enum{ALARM1=0x01,ALARM2=0}alarm_t;	
typedef enum{PER_SEC=0x0F,PER_MIN=0x0E,MATCH_SEC=0X0E,MATCH_MIN=0X0C,MATCH_SM=0X0C,MATCH_MH=0X08,MATCH_SMH=0X08,MATCH_SMHD=0X00}alarmmode_t;

typedef struct {
	
	uint8_t hour;
	ampm_t AMPM;
	uint8_t minute;
	uint8_t second;
	
}clock_t;
	
typedef struct {
	
	uint8_t century;
	uint16_t year;
	month_t month;
	uint8_t date;
	weekday_t weekDay;
	uint8_t hour;
	ampm_t AMPM;
	uint8_t minute;
	uint8_t second;
	
}calendar_t;

typedef struct {
	
	bool_t DYDT;
	uint8_t date;
	weekday_t weekDay;
	uint8_t hour;
	ampm_t AMPM;
	uint8_t minute;
	uint8_t second;
	alarmmode_t alarmMode;
	ISRcallback_t alarmHandlerCallback;
	
}alarmconfig_t;
	
void RTCC_init(void);
void RTCC_enable(void);
void RTCC_setClockMode(clockmode_t a_clockMode);
void RTCC_setClock(clock_t a_clock);	
void RTCC_setCalendar(calendar_t a_calendar);
clock_t RTCC_getClock(void);
char * RTCC_getClockStr(void);
calendar_t RTCC_getCalendar(void);	
void RTCC_setAlarm(alarm_t a_alarm, alarmconfig_t a_alarmConfig);
void RTCC_enableAlarm(alarm_t a_alarm);
void RTCC_disableAlarm(alarm_t a_alarm);
float RTCC_getTempCelsius(void);
float RTCC_getTempFahrenheit(void);

#endif /* RTCC_H_ */
