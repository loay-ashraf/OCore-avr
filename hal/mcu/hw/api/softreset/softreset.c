/*
 * softreset.c
 *
 * Created: 13/09/2019 03:41:02 PM
 *  Author: Loay Ashraf
 */ 

#include "softreset.h"

void softReset(void){
	
	wdt_enable(WD_16);
	while(1);
	
}