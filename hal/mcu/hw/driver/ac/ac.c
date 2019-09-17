/*
 * ac.c
 *
 * Created: 04/09/2019 06:08:20 PM
 *  Author: Loay Ashraf
 */ 

#include "ac.h"

static ISRcallback_t g_acISRCallback;

void ac_enable(void){
	
	gpio_setPinDirection(AC_IN0_PIN,IO_INPUT);
	gpio_setPinDirection(AC_IN1_PIN,IO_INPUT);
	CBI(ACSR,ACD);
	
}

void ac_disable(void){
	
	SBI(ACSR,ACD);
	
}

void ac_enableBandGap(void){
	
	SBI(ACSR,ACBG);	
	
}

void ac_disableBandGap(void){
	
	CBI(ACSR,ACBG);
	
}

void ac_enableInterrupt(void){
	
	 if(!RBI(SREG,I_BIT))
		ENABLE_GLOBAL_INTERRUPTS;
	SBI(ACSR,ACIE);	
	
}

void ac_disableInterrupt(void){
	
	CBI(ACSR,ACIE);
	
}

void ac_setInterruptMode(acinterruptmode_t a_acInterruptMode){
	
	CRI(ACSR,0x03);
	SRI(ACSR,a_acInterruptMode);
	
}

void ac_setISRCallback(ISRcallback_t a_acISRCalback){
	
	g_acISRCallback = a_acISRCalback;
	
}

ISR(ANA_COMP_vect){
	
	g_acISRCallback();
	
}