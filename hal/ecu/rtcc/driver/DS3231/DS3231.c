/**********************************************************************
*
* File:         DS3231.c
*
* Author(s):    Loay Ashraf <loay.ashraf.96@gmail.com>
*
* Date created: 04/10/2019
*
* Description:	contains function definitions for DS3231 controller
*               module.
*
**********************************************************************/

/*------------------------------INCLUDES-----------------------------*/

#include "DS3231.h"
#include "DS3231_config.h"
#include "hal/mcu/peripheral/gpio.h"
#include "hal/mcu/peripheral/twi/twi.h"

/*--------------------------GLOBAL VARIABLES-------------------------*/

/**********************************************************************
*
* Variable:    g_clockMode
*
* Description: Stores current clock mode for DS3231 controller module.
*
* Notes:
*
* Scope:       DS3231.c.
*
**********************************************************************/

static rtccclockmode_t g_clockMode;

/**********************************************************************
*
* Variable:    g_alarmHandlerCallback
*
* Description: Holds addresses of alarm handler callback functions.
*
* Notes:
*
* Scope:       DS3231.c.
*
**********************************************************************/

static ISRcallback_t g_alarmHandlerCallback[2];

/*------------------------FUNCTION PROTOTYPES------------------------*/

static void _alarmISR(void);

/*-----------------------FUNCTION DEFINITIONS------------------------*/

/**********************************************************************
*
* Function:    DS3231_init
*
* Description: Initializes D3231 controller module.
*
* Notes:       This function is called only when using new DS3231
*              controller module.
*
* Returns:     None.
*
**********************************************************************/

void DS3231_init(void){
	
	twi_enable(TWI0_M);
	twi_transmitStart(TWI0_M,DS3231_ADDRESS,WRITE);
	twi_transmitByte(TWI0_M,0x07);
	twi_transmitByte(TWI0_M,0x00);
	twi_transmitStop(TWI0_M);
	
}

/**********************************************************************
*
* Function:    DS3231_enable
*
* Description: Enables DS3231 controller module.
*
* Notes:
*
* Returns:     None.
*
**********************************************************************/

void DS3231_enable(void){
	
	twiconfig_t twiConfig = {.prescaler=TWI_DIV16,.frequency=200000UL,.address=0b1110000,.generalcall=TRUE,.ackbit=FALSE};
	twi_config(TWI0_M,&twiConfig);	
	twi_enable(TWI0_M);
	
}

/**********************************************************************
*
* Function:    DS3231_setClockMode
*
* Description: Sets clock mode (12H/24H) for DS3231 controller module.
*
* Notes:
*
* Returns:     None.
*
**********************************************************************/

void DS3231_setClockMode(rtccclockmode_t a_clockMode){
	
	g_clockMode = a_clockMode;
	ubyte_t hourRegister;
	twi_transmitStart(TWI0_M,DS3231_ADDRESS,WRITE);
	twi_transmitByte(TWI0_M,0x02);
	twi_transmitStart(TWI0_M,DS3231_ADDRESS,READ);
	hourRegister = twi_receiveByte(TWI0_M);
	
	if(a_clockMode==RT_TWELVE){
		
		twi_transmitStart(TWI0_M,DS3231_ADDRESS,WRITE);
		twi_transmitByte(TWI0_M,0x02);
		twi_transmitByte(TWI0_M,0x40|hourRegister);
		
	}else if(a_clockMode==RT_TWENTY_FOUR){
		
		twi_transmitStart(TWI0_M,DS3231_ADDRESS,WRITE);
		twi_transmitByte(TWI0_M,0x02);
		twi_transmitByte(TWI0_M,0xBF&hourRegister);
		
	}
	
	twi_transmitStop(TWI0_M);
	
}

/**********************************************************************
*
* Function:    DS3231_setClock
*
* Description: Sets clock for DS3231 controller module.
*
* Notes:
*
* Returns:     None.
*
**********************************************************************/

void DS3231_setClock(rtccclock_t a_clock){
	
	twi_transmitStart(TWI0_M,DS3231_ADDRESS,WRITE);
	twi_transmitByte(TWI0_M,0x00);
	twi_transmitByte(TWI0_M,0x00);
	
	twi_transmitByte(TWI0_M,(a_clock.minute%10)|((a_clock.minute/10)<<4));
	
	if (g_clockMode == RT_TWELVE){
		
		a_clock.hour = ((a_clock.hour%10)|((a_clock.hour/10)<<4))|0x40;
		twi_transmitByte(TWI0_M,a_clock.hour|(a_clock.AMPM<<4));
		
	}else if (g_clockMode == RT_TWENTY_FOUR){
		
		a_clock.hour = ((a_clock.hour%10)|((a_clock.hour/10)<<4));
		twi_transmitByte(TWI0_M,a_clock.hour);
		
	}
	
	twi_transmitStop(TWI0_M);
	
}

/**********************************************************************
*
* Function:    DS3231_setCalendar
*
* Description: Sets calendar for DS3231 controller module.
*
* Notes:
*
* Returns:     None.
*
**********************************************************************/

void DS3231_setCalendar(rtcccalendar_t a_calendar){
	
	twi_transmitStart(TWI0_M,DS3231_ADDRESS,WRITE);
	twi_transmitByte(TWI0_M,0x00);
	twi_transmitByte(TWI0_M,0x00);
	
	twi_transmitByte(TWI0_M,(a_calendar.minute%10)|((a_calendar.minute/10)<<4));
	
	if (g_clockMode == RT_TWELVE){
		
		a_calendar.hour = ((a_calendar.hour%10)|((a_calendar.hour/10)<<4))|0x40;
		twi_transmitByte(TWI0_M,a_calendar.hour|(a_calendar.AMPM<<4));
		
	}else if (g_clockMode == RT_TWENTY_FOUR){
		
		a_calendar.hour = ((a_calendar.hour%10)|((a_calendar.hour/10)<<4));
		twi_transmitByte(TWI0_M,a_calendar.hour);
		
	}
	
	twi_transmitByte(TWI0_M,a_calendar.weekDay);
	
	twi_transmitByte(TWI0_M,(a_calendar.date%10)|((a_calendar.date/10)<<4));
	
	twi_transmitByte(TWI0_M,(a_calendar.month%10)|((a_calendar.month/10)<<4));
	
	twi_transmitByte(TWI0_M,(a_calendar.year%10)|((a_calendar.year/10)<<4));
	
	twi_transmitStop(TWI0_M);
	
}

/**********************************************************************
*
* Function:    DS3231_getClock
*
* Description: Gets clock from DS3231 controller module.
*
* Notes:
*
* Returns:     Current clock (type: rtccclock_t).
*
**********************************************************************/

rtccclock_t DS3231_getClock(void){
	
	rtccclock_t clock;
	
	twi_transmitStart(TWI0_M,DS3231_ADDRESS,WRITE);
	twi_transmitByte(TWI0_M,0x00);
	twi_transmitStart(TWI0_M,DS3231_ADDRESS,READ);
	
	clock.second = twi_receiveByte(TWI0_M);
	clock.minute = twi_receiveByte(TWI0_M);
	clock.hour = twi_receiveByte(TWI0_M);
	twi_transmitStop(TWI0_M);
	
	if(g_clockMode == RT_TWELVE)
		clock.AMPM = (clock.hour&0x20)>>4;
	else if(g_clockMode == RT_TWENTY_FOUR)
		clock.AMPM = 0xFF;
	
	clock.second = (((clock.second&0xF0)>>4)*10)+(clock.second&0x0F);
	clock.minute = (((clock.minute&0xF0)>>4)*10)+(clock.minute&0x0F);
	
	if(g_clockMode == RT_TWELVE)
		clock.hour = (((clock.hour&0x10)>>4)*10)+(clock.hour&0x0F);
	else if(g_clockMode == RT_TWENTY_FOUR)
		clock.hour = (((clock.hour&0x30)>>4)*10)+(clock.hour&0x0F);
	
	return clock;
}

/**********************************************************************
*
* Function:    DS3231_getCalendar
*
* Description: Gets calendar from DS3231 controller module.
*
* Notes:
*
* Returns:     Current calendar (type: rtcccalendar_t).
*
**********************************************************************/

rtcccalendar_t DS3231_getCalendar(void){
	
	rtcccalendar_t calendar;
	
	twi_transmitStart(TWI0_M,DS3231_ADDRESS,WRITE);
	twi_transmitByte(TWI0_M,0x00);
	twi_transmitStart(TWI0_M,DS3231_ADDRESS,READ);
	
	calendar.second = twi_receiveByte(TWI0_M);
	calendar.minute = twi_receiveByte(TWI0_M);
	calendar.hour = twi_receiveByte(TWI0_M);
	
	if(g_clockMode == RT_TWELVE)
		calendar.AMPM = (calendar.hour&0x20)>>4;
	else if(g_clockMode == RT_TWENTY_FOUR)
		calendar.AMPM = 0xFF;
	
	calendar.weekDay = twi_receiveByte(TWI0_M);
	calendar.date = twi_receiveByte(TWI0_M);
	calendar.month = twi_receiveByte(TWI0_M);
	calendar.year = twi_receiveByte(TWI0_M);
	twi_transmitStop(TWI0_M);
	
	calendar.second = (((calendar.second&0xF0)>>4)*10)+(calendar.second&0x0F);
	calendar.minute = (((calendar.minute&0xF0)>>4)*10)+(calendar.minute&0x0F);
	
	if(g_clockMode == RT_TWELVE)
		calendar.hour = (((calendar.hour&0x10)>>4)*10)+(calendar.hour&0x0F);
	else if(g_clockMode == RT_TWENTY_FOUR)
		calendar.hour = (((calendar.hour&0x30)>>4)*10)+(calendar.hour&0x0F);
	
	calendar.date = (((calendar.date&0x30)>>4)*10)+(calendar.date&0x0F);
	calendar.month = (((calendar.month&0x10)>>4)*10)+(calendar.month&0x0F);
	calendar.year = (((calendar.year&0xF0)>>4)*10)+(calendar.year&0x0F);
	
	return calendar;
}

/**********************************************************************
*
* Function:    DS3231_setAlarm
*
* Description: Sets alarm for DS3231 controller module.
*
* Notes:
*
* Returns:     None.
*
**********************************************************************/

void DS3231_setAlarm(rtccalarm_t a_alarm, rtccalarmconfig_t a_alarmConfig){
	
	if(a_alarm == RT_ALARM1)
		a_alarmConfig.second = (a_alarmConfig.second%10)|((a_alarmConfig.second/10)<<4);
	
	a_alarmConfig.minute = (a_alarmConfig.minute%10)|((a_alarmConfig.minute/10)<<4);
	
	if (g_clockMode == RT_TWELVE){
		
		a_alarmConfig.hour = ((a_alarmConfig.hour%10)|((a_alarmConfig.hour/10)<<4))|0x40;
		a_alarmConfig.hour = a_alarmConfig.hour|(a_alarmConfig.AMPM<<4);
		
	}else if (g_clockMode == RT_TWENTY_FOUR){
		
		a_alarmConfig.hour = ((a_alarmConfig.hour%10)|((a_alarmConfig.hour/10)<<4));
		
	}
	
	if(a_alarmConfig.DYDT)
		a_alarmConfig.weekDay = a_alarmConfig.weekDay|0x40;
	else
		a_alarmConfig.date = (a_alarmConfig.date%10)|((a_alarmConfig.date/10)<<4);
	
	switch(a_alarm){
		
		case RT_ALARM1: {
			
			twi_transmitStart(TWI0_M,DS3231_ADDRESS,WRITE);
			twi_transmitByte(TWI0_M,0x07);
			
			twi_transmitByte(TWI0_M,((a_alarmConfig.alarmMode&0x01)<<7)|a_alarmConfig.second);
			twi_transmitByte(TWI0_M,((a_alarmConfig.alarmMode&0x02)<<6)|a_alarmConfig.minute);
			twi_transmitByte(TWI0_M,((a_alarmConfig.alarmMode&0x04)<<5)|a_alarmConfig.hour);
			
			if(a_alarmConfig.DYDT)

				twi_transmitByte(TWI0_M,((a_alarmConfig.alarmMode&0x08)<<4)|a_alarmConfig.weekDay);
			
			else
			
				twi_transmitByte(TWI0_M,((a_alarmConfig.alarmMode&0x08)<<4)|a_alarmConfig.date);
			
			g_alarmHandlerCallback[0] = a_alarmConfig.alarmHandlerCallback;
			
		}
		break;
		
		case RT_ALARM2: {
			
			twi_transmitStart(TWI0_M,DS3231_ADDRESS,WRITE);
			twi_transmitByte(TWI0_M,0x0B);
			
			twi_transmitByte(TWI0_M,((a_alarmConfig.alarmMode&0x02)<<6)|a_alarmConfig.minute);
			twi_transmitByte(TWI0_M,((a_alarmConfig.alarmMode&0x04)<<5)|a_alarmConfig.hour);
			
			if(a_alarmConfig.DYDT)

				twi_transmitByte(TWI0_M,((a_alarmConfig.alarmMode&0x08)<<4)|a_alarmConfig.weekDay);
			
			else
			
				twi_transmitByte(TWI0_M,((a_alarmConfig.alarmMode&0x08)<<4)|a_alarmConfig.date);
			
			g_alarmHandlerCallback[1] = a_alarmConfig.alarmHandlerCallback;
			
		}
		break;
		
	}
	
	twi_transmitStop(TWI0_M);
	
}

/**********************************************************************
*
* Function:    DS3231_enableAlarm
*
* Description: Enables alarm for DS3231 controller module.
*
* Notes:       This function enables global interrupts if disabled.
*
* Returns:     None.
*
**********************************************************************/

void DS3231_enableAlarm(rtccalarm_t a_alarm){
	
	ubyte_t ctrlReg;
	
	twi_transmitStart(TWI0_M,DS3231_ADDRESS,WRITE);
	twi_transmitByte(TWI0_M,0x0E);
	twi_transmitStart(TWI0_M,DS3231_ADDRESS,READ);
	ctrlReg = twi_receiveByte(TWI0_M);
	
	twi_transmitStart(TWI0_M,DS3231_ADDRESS,WRITE);
	twi_transmitByte(TWI0_M,0x0E);
	
	switch(a_alarm){
		
		case RT_ALARM1: twi_transmitByte(TWI0_M,ctrlReg|0x05);
		break;
		
		case RT_ALARM2: twi_transmitByte(TWI0_M,ctrlReg|0x06);
		break;
		
	}
	
	twi_transmitStop(TWI0_M);
	
	GPIO_ENABLE_PIN_PULLUP(DS3231_ALARM_INT_PIN);
	GPIO_SET_INTERRUPT_MODE(DS3231_ALARM_INT,DS3231_ALARM_INT_MODE);
	GPIO_SET_ISR_CALLBACK(DS3231_ALARM_INT,&_alarmISR);
	GPIO_ENABLE_INTERRUPT(DS3231_ALARM_INT);
	
}

/**********************************************************************
*
* Function:    DS3231_disableAlarm
*
* Description: Disables alarm for DS3231 controller module.
*
* Notes:       This function doesn't disable global interrupts.
*
* Returns:     None.
*
**********************************************************************/

void DS3231_disableAlarm(rtccalarm_t a_alarm){
	
	ubyte_t ctrlReg;
	
	twi_transmitStart(TWI0_M,DS3231_ADDRESS,WRITE);
	twi_transmitByte(TWI0_M,0x0E);
	twi_transmitStart(TWI0_M,DS3231_ADDRESS,READ);
	ctrlReg = twi_receiveByte(TWI0_M);
	
	twi_transmitStart(TWI0_M,DS3231_ADDRESS,WRITE);
	twi_transmitByte(TWI0_M,0x0E);
	
	switch(a_alarm){
		
		case RT_ALARM1: twi_transmitByte(TWI0_M,ctrlReg&0xFE);
		break;
		
		case RT_ALARM2: twi_transmitByte(TWI0_M,ctrlReg&0xFD);
		break;
		
	}
	
	twi_transmitStop(TWI0_M);
	
	GPIO_DISABLE_INTERRUPT(DS3231_ALARM_INT);
	
}

/**********************************************************************
*
* Function:    DS3231_getTempCelsius
*
* Description: Gets temperature in Celsius from DS3231 controller
*              module.
*
* Notes:
*
* Returns:     Current temperature in Celsius (type: float).
*
**********************************************************************/

float DS3231_getTempCelsius(void){
	
	uint8_t temp_i,temp_d;
	twi_transmitStart(TWI0_M,DS3231_ADDRESS,WRITE);
	twi_transmitByte(TWI0_M,0x11);
	twi_transmitStart(TWI0_M,DS3231_ADDRESS,READ);
	temp_i = twi_receiveByte(TWI0_M);
	temp_d = (twi_receiveByte(TWI0_M)>>6)*25;
	twi_transmitStop(TWI0_M);
	
	return (float)(temp_i+(temp_d/100.0));
	
}

/**********************************************************************
*
* Function:    DS3231_getTempFahrenheit
*
* Description: Gets temperature in Fahrenheit from DS3231 controller
*              module.
*
* Notes:
*
* Returns:     Current temperature in Fahrenheit (type: float).
*
**********************************************************************/

float DS3231_getTempFahrenheit(void){
	
	return (float)((DS3231_getTempCelsius()*1.8)+32);
	
}

/**********************************************************************
*
* Function:    _alarmISR
*
* Description: Checks which alarm is triggered and calls corresponding
*              g_alarmHandlerCallback function.
*
* Notes:
*
* Returns:     None.
*
**********************************************************************/

static void _alarmISR(void){
	
	ubyte_t csReg;
	
	twi_transmitStart(TWI0_M,DS3231_ADDRESS,WRITE);
	twi_transmitByte(TWI0_M,0x0F);
	twi_transmitStart(TWI0_M,DS3231_ADDRESS,READ);
	csReg = twi_receiveByte(TWI0_M);
	
	if((csReg&0x01) && (csReg&0x02)){
		
		g_alarmHandlerCallback[0]();
		g_alarmHandlerCallback[1]();
		
	}else if(csReg & 0x01){
		
		g_alarmHandlerCallback[0]();
		
	}else if(csReg & 0x02){
		
		g_alarmHandlerCallback[1]();
		
	}
	
	twi_transmitStart(TWI0_M,DS3231_ADDRESS,WRITE);
	twi_transmitByte(TWI0_M,0x0F);
	twi_transmitByte(TWI0_M,0x88);
	twi_transmitStop(TWI0_M);
	
}
