/*
 * DS3231.c
 *
 * Created: 04/10/2019 07:15:44 AM
 *  Author: Loay Ashraf
 */ 

#include "DS3231.h"
#include "DS3231_config.h"
#include "hal/mcu/hw/driver/gpio/gpio.h"
#include "hal/mcu/hw/driver/twi/twi.h"

static rtccclockmode_t g_clockMode;
static ISRcallback_t g_alarmHandlerCallback[2];

static void alarmISR(void);

void DS3231_init(void){
	
	twi_enable();
	twi_transmitStart(DS3231_ADDRESS,WRITE);
	twi_transmitByte(0x07);
	twi_transmitByte(0x00);
	twi_transmitStop();
	twi_enable();
	
}

void DS3231_enable(void){
	
	twi_enable();
	
}

void DS3231_setClockMode(rtccclockmode_t a_clockMode){
	
	g_clockMode = a_clockMode;
	ubyte_t hourRegister;
	twi_transmitStart(DS3231_ADDRESS,WRITE);
	twi_transmitByte(0x02);
	twi_transmitStart(DS3231_ADDRESS,READ);
	hourRegister = twi_receiveByte();
	
	if(a_clockMode==RT_TWELVE){
		
		twi_transmitStart(DS3231_ADDRESS,WRITE);
		twi_transmitByte(0x02);
		twi_transmitByte(0x40|hourRegister);
		
	}else if(a_clockMode==RT_TWENTY_FOUR){
		
		twi_transmitStart(DS3231_ADDRESS,WRITE);
		twi_transmitByte(0x02);
		twi_transmitByte(0xBF&hourRegister);
		
	}
	
	twi_transmitStop();
	
}

void DS3231_setClock(rtccclock_t a_clock){
	
	twi_transmitStart(DS3231_ADDRESS,WRITE);
	twi_transmitByte(0x00);
	twi_transmitByte(0x00);
	
	twi_transmitByte((a_clock.minute%10)|((a_clock.minute/10)<<4));
	
	if (g_clockMode == RT_TWELVE){
		
		a_clock.hour = ((a_clock.hour%10)|((a_clock.hour/10)<<4))|0x40;
		twi_transmitByte(a_clock.hour|(a_clock.AMPM<<4));
		
	}else if (g_clockMode == RT_TWENTY_FOUR){
		
		a_clock.hour = ((a_clock.hour%10)|((a_clock.hour/10)<<4));
		twi_transmitByte(a_clock.hour);
		
	}
	
	twi_transmitStop();
	
}

void DS3231_setCalendar(rtcccalendar_t a_calendar){
	
	twi_transmitStart(DS3231_ADDRESS,WRITE);
	twi_transmitByte(0x00);
	twi_transmitByte(0x00);
	
	twi_transmitByte((a_calendar.minute%10)|((a_calendar.minute/10)<<4));
	
	if (g_clockMode == RT_TWELVE){
		
		a_calendar.hour = ((a_calendar.hour%10)|((a_calendar.hour/10)<<4))|0x40;
		twi_transmitByte(a_calendar.hour|(a_calendar.AMPM<<4));
		
	}else if (g_clockMode == RT_TWENTY_FOUR){
		
		a_calendar.hour = ((a_calendar.hour%10)|((a_calendar.hour/10)<<4));
		twi_transmitByte(a_calendar.hour);
		
	}
	
	twi_transmitByte(a_calendar.weekDay);
	
	twi_transmitByte((a_calendar.date%10)|((a_calendar.date/10)<<4));
	
	twi_transmitByte((a_calendar.month%10)|((a_calendar.month/10)<<4));
	
	twi_transmitByte((a_calendar.year%10)|((a_calendar.year/10)<<4));
	
	twi_transmitStop();
	
}

rtccclock_t DS3231_getClock(void){
	
	rtccclock_t clock;
	
	twi_transmitStart(DS3231_ADDRESS,WRITE);
	twi_transmitByte(0x00);
	twi_transmitStart(DS3231_ADDRESS,READ);
	
	clock.second = twi_receiveByte();
	clock.minute = twi_receiveByte();
	clock.hour = twi_receiveByte();
	twi_transmitStop();
	
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

rtcccalendar_t DS3231_getCalendar(void){
	
	rtcccalendar_t calendar;
	
	twi_transmitStart(DS3231_ADDRESS,WRITE);
	twi_transmitByte(0x00);
	twi_transmitStart(DS3231_ADDRESS,READ);
	
	calendar.second = twi_receiveByte();
	calendar.minute = twi_receiveByte();
	calendar.hour = twi_receiveByte();
	
	if(g_clockMode == RT_TWELVE)
		calendar.AMPM = (calendar.hour&0x20)>>4;
	else if(g_clockMode == RT_TWENTY_FOUR)
		calendar.AMPM = 0xFF;
	
	calendar.weekDay = twi_receiveByte();
	calendar.date = twi_receiveByte();
	calendar.month = twi_receiveByte();
	calendar.year = twi_receiveByte();
	twi_transmitStop();
	
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
			
			twi_transmitStart(DS3231_ADDRESS,WRITE);
			twi_transmitByte(0x07);
			
			twi_transmitByte(((a_alarmConfig.alarmMode&0x01)<<7)|a_alarmConfig.second);
			twi_transmitByte(((a_alarmConfig.alarmMode&0x02)<<6)|a_alarmConfig.minute);
			twi_transmitByte(((a_alarmConfig.alarmMode&0x04)<<5)|a_alarmConfig.hour);
			
			if(a_alarmConfig.DYDT)

				twi_transmitByte(((a_alarmConfig.alarmMode&0x08)<<4)|a_alarmConfig.weekDay);
			
			else
			
				twi_transmitByte(((a_alarmConfig.alarmMode&0x08)<<4)|a_alarmConfig.date);
			
			g_alarmHandlerCallback[0] = a_alarmConfig.alarmHandlerCallback;
			
		}
		break;
		
		case RT_ALARM2: {
			
			twi_transmitStart(DS3231_ADDRESS,WRITE);
			twi_transmitByte(0x0B);
			
			twi_transmitByte(((a_alarmConfig.alarmMode&0x02)<<6)|a_alarmConfig.minute);
			twi_transmitByte(((a_alarmConfig.alarmMode&0x04)<<5)|a_alarmConfig.hour);
			
			if(a_alarmConfig.DYDT)

				twi_transmitByte(((a_alarmConfig.alarmMode&0x08)<<4)|a_alarmConfig.weekDay);
			
			else
			
				twi_transmitByte(((a_alarmConfig.alarmMode&0x08)<<4)|a_alarmConfig.date);
			
			g_alarmHandlerCallback[1] = a_alarmConfig.alarmHandlerCallback;
			
		}
		break;
		
	}
	
	twi_transmitStop();
	
}

void DS3231_enableAlarm(rtccalarm_t a_alarm){
	
	ubyte_t ctrlReg;
	
	twi_transmitStart(DS3231_ADDRESS,WRITE);
	twi_transmitByte(0x0E);
	twi_transmitStart(DS3231_ADDRESS,READ);
	ctrlReg = twi_receiveByte();
	
	twi_transmitStart(DS3231_ADDRESS,WRITE);
	twi_transmitByte(0x0E);
	
	switch(a_alarm){
		
		case RT_ALARM1: twi_transmitByte(ctrlReg|0x05);
		break;
		
		case RT_ALARM2: twi_transmitByte(ctrlReg|0x06);
		break;
		
	}
	
	twi_transmitStop();
	
	gpio_enablePinPullUp(DS3231_ALARM_INT_PIN);
	gpio_setInterruptMode(DS3231_ALARM_INT,DS3231_ALARM_INT_MODE);
	gpio_setISRCallback(DS3231_ALARM_INT,&alarmISR);
	gpio_enableInterrupt(DS3231_ALARM_INT);
	
}

void DS3231_disableAlarm(rtccalarm_t a_alarm){
	
	ubyte_t ctrlReg;
	
	twi_transmitStart(DS3231_ADDRESS,WRITE);
	twi_transmitByte(0x0E);
	twi_transmitStart(DS3231_ADDRESS,READ);
	ctrlReg = twi_receiveByte();
	
	twi_transmitStart(DS3231_ADDRESS,WRITE);
	twi_transmitByte(0x0E);
	
	switch(a_alarm){
		
		case RT_ALARM1: twi_transmitByte(ctrlReg&0xFE);
		break;
		
		case RT_ALARM2: twi_transmitByte(ctrlReg&0xFD);
		break;
		
	}
	
	twi_transmitStop();
	
	gpio_disableInterrupt(DS3231_ALARM_INT);
	
}

float DS3231_getTempCelsius(void){
	
	uint8_t temp_i,temp_d;
	twi_transmitStart(DS3231_ADDRESS,WRITE);
	twi_transmitByte(0x11);
	twi_transmitStart(DS3231_ADDRESS,READ);
	temp_i = twi_receiveByte();
	temp_d = (twi_receiveByte()>>6)*25;
	twi_transmitStop();
	
	return (float)(temp_i+(temp_d/100.0));
	
}
/**
* does nothing
*
*/

float DS3231_getTempFahrenheit(void){
	
	return (float)((DS3231_getTempCelsius()*1.8)+32);
	
}

static void alarmISR(void){
	
	ubyte_t csReg;
	
	twi_transmitStart(DS3231_ADDRESS,WRITE);
	twi_transmitByte(0x0F);
	twi_transmitStart(DS3231_ADDRESS,READ);
	csReg = twi_receiveByte();
	
	if((csReg&0x01) && (csReg&0x02)){
		
		g_alarmHandlerCallback[0]();
		g_alarmHandlerCallback[1]();
		
	}else if(csReg & 0x01){
		
		g_alarmHandlerCallback[0]();
		
	}else if(csReg & 0x02){
		
		g_alarmHandlerCallback[1]();
		
	}
	
	twi_transmitStart(DS3231_ADDRESS,WRITE);
	twi_transmitByte(0x0F);
	twi_transmitByte(0x88);
	twi_transmitStop();
	
}