/*
 * RTCC.c
 *
 * Created: 07/08/2019 12:27:36 PM
 *  Author: Loay Ashraf
 */ 

#include "RTCC.h"

static clockmode_t g_clockMode;
static ISRcallback_t g_alarmHandlerCallback[2];

static void alarmISR(void);

void RTCC_init(void){
	
	twi_enable();
	twi_transmitStart(RTCC_ADDRESS,WRITE);
	twi_transmitByte(0x07);
	twi_transmitByte(0x00);
	twi_transmitStop();
	
}

void RTCC_enable(void){
	
	twi_enable();
	
}

void RTCC_setClockMode(clockmode_t a_clockMode){
	
	g_clockMode = a_clockMode; 
	ubyte_t hourRegister;
	twi_transmitStart(RTCC_ADDRESS,WRITE);
	twi_transmitByte(0x02);
	twi_transmitStart(RTCC_ADDRESS,READ);
	hourRegister = twi_receiveByte();
	
	if(a_clockMode==TWELVE){
		
		twi_transmitStart(RTCC_ADDRESS,WRITE);
		twi_transmitByte(0x02);
		twi_transmitByte(0x40|hourRegister);
		
	}else if(a_clockMode==TWENTY_FOUR){
		
		twi_transmitStart(RTCC_ADDRESS,WRITE);
		twi_transmitByte(0x02);
		twi_transmitByte(0xBF&hourRegister);
		
	}
	
	twi_transmitStop();
}

void RTCC_setClock(clock_t a_clock){
	
	twi_transmitStart(RTCC_ADDRESS,WRITE);
	twi_transmitByte(0x00);
	twi_transmitByte(0x00);
	
	twi_transmitByte((a_clock.minute%10)|((a_clock.minute/10)<<4));
	
	if (g_clockMode == TWELVE){
		
		a_clock.hour = ((a_clock.hour%10)|((a_clock.hour/10)<<4))|0x40;
		twi_transmitByte(a_clock.hour|(a_clock.AMPM<<4));
		
	}else if (g_clockMode == TWENTY_FOUR){
		
		a_clock.hour = ((a_clock.hour%10)|((a_clock.hour/10)<<4));
		twi_transmitByte(a_clock.hour);
		
	}
	
	twi_transmitStop();
}

void RTCC_setCalendar(calendar_t a_calendar){
	
	twi_transmitStart(RTCC_ADDRESS,WRITE);
	twi_transmitByte(0x00);
	twi_transmitByte(0x00);
	
	twi_transmitByte((a_calendar.minute%10)|((a_calendar.minute/10)<<4));
	
	if (g_clockMode == TWELVE){
		
		a_calendar.hour = ((a_calendar.hour%10)|((a_calendar.hour/10)<<4))|0x40;
		twi_transmitByte(a_calendar.hour|(a_calendar.AMPM<<4));
		
	}else if (g_clockMode == TWENTY_FOUR){
		
		a_calendar.hour = ((a_calendar.hour%10)|((a_calendar.hour/10)<<4));
		twi_transmitByte(a_calendar.hour);
		
	}
	
	twi_transmitByte(a_calendar.weekDay);
	
	twi_transmitByte((a_calendar.date%10)|((a_calendar.date/10)<<4));
		
	twi_transmitByte((a_calendar.month%10)|((a_calendar.month/10)<<4));
	
	a_calendar.year -= RTCC_MILLENNIUM;
	
	twi_transmitByte((a_calendar.year%10)|((a_calendar.year/10)<<4));
	
	twi_transmitStop();
}

clock_t RTCC_getClock(void){
	
	clock_t clock;
	
	twi_transmitStart(RTCC_ADDRESS,WRITE);
	twi_transmitByte(0x00);
	twi_transmitStart(RTCC_ADDRESS,READ);
	
	clock.second = twi_receiveByte();
	clock.minute = twi_receiveByte();
	clock.hour = twi_receiveByte();
	twi_transmitStop();
	
	if(g_clockMode == TWELVE)
		clock.AMPM = (clock.hour&0x20)>>4;
	else if(g_clockMode == TWENTY_FOUR)
		clock.AMPM = 0xFF;
	
	clock.second = (((clock.second&0xF0)>>4)*10)+(clock.second&0x0F);
	clock.minute = (((clock.minute&0xF0)>>4)*10)+(clock.minute&0x0F);
	
	if(g_clockMode == TWELVE)
		clock.hour = (((clock.hour&0x10)>>4)*10)+(clock.hour&0x0F);
	else if(g_clockMode == TWENTY_FOUR)
		clock.hour = (((clock.hour&0x30)>>4)*10)+(clock.hour&0x0F);
	
	return clock;
}

char * RTCC_getClockStr(void){
	
	uint8_t clock[4];
	static char clockStr[12];
	
	twi_transmitStart(RTCC_ADDRESS,WRITE);
	twi_transmitByte(0x00);
	twi_transmitStart(RTCC_ADDRESS,READ);
	
	clock[0] = twi_receiveByte();
	clock[1] = twi_receiveByte();
	clock[2] = twi_receiveByte();
	twi_transmitStop();
	
	if(g_clockMode == TWELVE)
		clock[3] = (clock[2]&0x20)>>4;
	else if(g_clockMode == TWENTY_FOUR)
		clock[3] = 0xFF;
		
	clock[0] = (((clock[0]&0xF0)>>4)*10)+(clock[0]&0x0F);
	clock[1] = (((clock[1]&0xF0)>>4)*10)+(clock[1]&0x0F);
	
	if(g_clockMode == TWELVE)
		clock[2] = (((clock[2]&0x10)>>4)*10)+(clock[2]&0x0F);
	else if(g_clockMode == TWENTY_FOUR)
		clock[2] = (((clock[2]&0x30)>>4)*10)+(clock[2]&0x0F);
	
	switch(clock[3]){
		
		case AM: sprintf(clockStr,RTCC_CLOCK_FORMAT,clock[2],clock[1],clock[0],"AM");
		break;
		
		case PM: sprintf(clockStr,RTCC_CLOCK_FORMAT,clock[2],clock[1],clock[0],"PM");
		break;
		
		default: sprintf(clockStr,"%02d:%02d:%02d",clock[2],clock[1],clock[0]);
		break;
		
	}
	
	clockStr[11] = '\0';
	return clockStr;
}

calendar_t RTCC_getCalendar(void){
	
	calendar_t calendar;
	
	twi_transmitStart(RTCC_ADDRESS,WRITE);
	twi_transmitByte(0x00);
	twi_transmitStart(RTCC_ADDRESS,READ);
	
	calendar.second = twi_receiveByte();
	calendar.minute = twi_receiveByte();
	calendar.hour = twi_receiveByte();
	
	if(g_clockMode == TWELVE)
	 calendar.AMPM = (calendar.hour&0x20)>>4;
	else if(g_clockMode == TWENTY_FOUR)
	 calendar.AMPM = 0xFF;
	 
	calendar.weekDay = twi_receiveByte();
	calendar.date = twi_receiveByte();
	calendar.month = twi_receiveByte();
	calendar.year = twi_receiveByte(); 
	twi_transmitStop();
	
	calendar.second = (((calendar.second&0xF0)>>4)*10)+(calendar.second&0x0F);
	calendar.minute = (((calendar.minute&0xF0)>>4)*10)+(calendar.minute&0x0F);
	
	if(g_clockMode == TWELVE)
		calendar.hour = (((calendar.hour&0x10)>>4)*10)+(calendar.hour&0x0F);
	else if(g_clockMode == TWENTY_FOUR)
		calendar.hour = (((calendar.hour&0x30)>>4)*10)+(calendar.hour&0x0F);
			
	calendar.date = (((calendar.date&0x30)>>4)*10)+(calendar.date&0x0F);
	calendar.month = (((calendar.month&0x10)>>4)*10)+(calendar.month&0x0F);
	calendar.year = (((calendar.year&0xF0)>>4)*10)+(calendar.year&0x0F);
	calendar.year += RTCC_MILLENNIUM;
	
	return calendar;
}

void RTCC_setAlarm(alarm_t a_alarm, alarmconfig_t a_alarmConfig){
	
	if(a_alarm == ALARM1)
		a_alarmConfig.second = (a_alarmConfig.second%10)|((a_alarmConfig.second/10)<<4);
	
	a_alarmConfig.minute = (a_alarmConfig.minute%10)|((a_alarmConfig.minute/10)<<4);
	
	if (g_clockMode == TWELVE){
		
		a_alarmConfig.hour = ((a_alarmConfig.hour%10)|((a_alarmConfig.hour/10)<<4))|0x40;
		a_alarmConfig.hour = a_alarmConfig.hour|(a_alarmConfig.AMPM<<4);
		
	}else if (g_clockMode == TWENTY_FOUR){
		
		a_alarmConfig.hour = ((a_alarmConfig.hour%10)|((a_alarmConfig.hour/10)<<4));
		
	}
	
	if(a_alarmConfig.DYDT)
		a_alarmConfig.weekDay = a_alarmConfig.weekDay|0x40;
	else
		a_alarmConfig.date = (a_alarmConfig.date%10)|((a_alarmConfig.date/10)<<4);
	
	switch(a_alarm){
		
		case ALARM1: {
			
			twi_transmitStart(RTCC_ADDRESS,WRITE);
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
		
		case ALARM2: {
			
			twi_transmitStart(RTCC_ADDRESS,WRITE);
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

void RTCC_enableAlarm(alarm_t a_alarm){
	
	ubyte_t ctrlReg;
	
	twi_transmitStart(RTCC_ADDRESS,WRITE);
	twi_transmitByte(0x0E);
	twi_transmitStart(RTCC_ADDRESS,READ);
	ctrlReg = twi_receiveByte();
	
	twi_transmitStart(RTCC_ADDRESS,WRITE);
	twi_transmitByte(0x0E);
	
	switch(a_alarm){
		
		case ALARM1: twi_transmitByte(ctrlReg|0x05);
		break;
		
		case ALARM2: twi_transmitByte(ctrlReg|0x06);
		break;
		
	}
	
	twi_transmitStop();
	
	gpio_enablePinPullUp(RTCC_ALARM_INT_PIN);
	gpio_setInterruptMode(RTCC_ALARM_INT,RTCC_ALARM_INT_MODE);
	gpio_setISRCallback(RTCC_ALARM_INT,&alarmISR);
	gpio_enableInterrupt(RTCC_ALARM_INT);
	
}

void RTCC_disableAlarm(alarm_t a_alarm){
	
	ubyte_t ctrlReg;
	
	twi_transmitStart(RTCC_ADDRESS,WRITE);
	twi_transmitByte(0x0E);
	twi_transmitStart(RTCC_ADDRESS,READ);
	ctrlReg = twi_receiveByte();
	
	twi_transmitStart(RTCC_ADDRESS,WRITE);
	twi_transmitByte(0x0E);
	
	switch(a_alarm){
		
		case ALARM1: twi_transmitByte(ctrlReg&0xFE);
		break;
		
		case ALARM2: twi_transmitByte(ctrlReg&0xFD);
		break;
		
	}
	
	twi_transmitStop();
	
	gpio_disableInterrupt(RTCC_ALARM_INT);
	
}

float RTCC_getTempCelsius(void){
	
	uint8_t temp_i,temp_d;
	twi_transmitStart(RTCC_ADDRESS,WRITE);
	twi_transmitByte(0x11);
	twi_transmitStart(RTCC_ADDRESS,READ);
	temp_i = twi_receiveByte();
	temp_d = (twi_receiveByte()>>6)*25;
	twi_transmitStop();
	
	return (float)(temp_i+(temp_d/100.0));
	
}
/**
* does nothing
*
*/

float RTCC_getTempFahrenheit(void){
	
	return (float)((RTCC_getTempCelsius()*1.8)+32);
	
}

static void alarmISR(void){
	
	ubyte_t csReg;
	
	twi_transmitStart(RTCC_ADDRESS,WRITE);
	twi_transmitByte(0x0F);
	twi_transmitStart(RTCC_ADDRESS,READ);
	csReg = twi_receiveByte();
	
	if((csReg&0x01) && (csReg&0x02)){
	
		g_alarmHandlerCallback[0]();
		g_alarmHandlerCallback[1]();
		
	}else if(csReg & 0x01){	
	
		g_alarmHandlerCallback[0]();
	
	}else if(csReg & 0x02){
	
		g_alarmHandlerCallback[1]();
	
	}
	
	twi_transmitStart(RTCC_ADDRESS,WRITE);
	twi_transmitByte(0x0F);
	twi_transmitByte(0x88);
	twi_transmitStop();
	
}