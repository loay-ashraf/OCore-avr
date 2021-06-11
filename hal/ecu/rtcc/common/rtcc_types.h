/**********************************************************************
*
* File:         rtcc_types.h
*
* Author(s):	Loay Ashraf <loay.ashraf.96@gmail.com>
*
* Date created: 04/10/2019
*
* Description:	contains typedefs and common data types for
*               RTCC module.
*
**********************************************************************/

#ifndef RTCC_TYPES_H_
#define RTCC_TYPES_H_

/*------------------------------INCLUDES-----------------------------*/

#include "hal/mcu/io/io_defs.h"
#include "service/include/std_types.h"

/*--------------------------------ENUMS------------------------------*/

typedef enum{RT_TWELVE,RT_TWENTY_FOUR}rtccclockmode_t;
typedef enum{RT_AM,RT_PM=2}rtccampm_t;
typedef enum{RT_SAT=1,RT_SUN,RT_MON,RT_TUE,RT_WED,RT_THU,RT_FRI}rtccweekday_t;
typedef enum{RT_JAN=1,RT_FEB,RT_MAR,RT_APR,RT_MAY,RT_JUN,RT_JUL,RT_AUG,RT_SEP,RT_OCT,RT_NOV,RT_DEC}rtccmonth_t;
typedef enum{RT_ALARM1=0x01,RT_ALARM2=0}rtccalarm_t;
typedef enum{RT_PER_SEC=0x0F,RT_PER_MIN=0x0E,RT_MATCH_SEC=0X0E,RT_MATCH_MIN=0X0C,RT_MATCH_SM=0X0C,RT_MATCH_MH=0X08,RT_MATCH_SMH=0X08,RT_MATCH_SMHD=0X00}rtccalarmmode_t;

/*-------------------------------STRUCTS-----------------------------*/

typedef struct {
	
	uint8_t hour;
	rtccampm_t AMPM;
	uint8_t minute;
	uint8_t second;
	
}rtccclock_t;

typedef struct {
	
	uint8_t century;
	uint16_t year;
	rtccmonth_t month;
	uint8_t date;
	rtccweekday_t weekDay;
	uint8_t hour;
	rtccampm_t AMPM;
	uint8_t minute;
	uint8_t second;
	
}rtcccalendar_t;

typedef struct {
	
	bool_t DYDT;
	uint8_t date;
	rtccweekday_t weekDay;
	uint8_t hour;
	rtccampm_t AMPM;
	uint8_t minute;
	uint8_t second;
	rtccalarmmode_t alarmMode;
	ISRcallback_t alarmHandlerCallback;
	
}rtccalarmconfig_t;


#endif /* RTCC_TYPES_H_ */