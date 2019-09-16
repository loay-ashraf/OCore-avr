/*
 * wdt.c
 *
 * Created: 04/09/2019 05:22:32 PM
 *  Author: Loay Ashraf
 */ 

#include "wdt.h"

void wdt_enable(wdttimeout_t a_wdtTimeout){
	
	SRI(WDTCR,a_wdtTimeout);
	SBI(WDTCR,WDE);
	
}

void wdt_disable(void){
	
	SBI(WDTCR,WDTOE);
	CBI(WDTCR,WDE);
	
}