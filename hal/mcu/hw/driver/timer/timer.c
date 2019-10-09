/**********************************************************************
*
* File:         timer.c
*
* Author(s):    Loay Ashraf <loay.ashraf.96@gmail.com>
*
* Date created: 16/05/2019
*
* Description:	contains function definitions for 8-bit timer
*               interface module.
*
**********************************************************************/

/*------------------------------INCLUDES-----------------------------*/

#include "timer.h"
#include "hal/mcu/hw/driver/gpio/gpio.h"
#include "hal/mcu/io/io_macros.h"
#include "hal/mcu/sys/interrupt.h"

/*--------------------------GLOBAL VARIABLES-------------------------*/

/**********************************************************************
*
* Variable:    g_timerISRCallback
*
* Description: Holds addresses of interrupt callback functions.
*
* Notes:
*
* Scope:       timer.c.
*
**********************************************************************/

static ISRcallback_t g_timerISRCallback[4];

/*-----------------------FUNCTION DEFINITIONS------------------------*/

/**********************************************************************
*
* Function:    timer_start
*
* Description: Starts 8-bit timer module.
*
* Notes:
*
* Returns:     None.
*
**********************************************************************/

void timer_start(timer_t a_timer,timerprescaler_t a_timerPrescaler){
	
	switch (a_timer){
		
		case TIMER0_M: {
			
			WRI(TCNT0,0);
			if(a_timerPrescaler == T_DIV64)
				SRI(TCCR0,T_DIV32);
			else if(a_timerPrescaler == T_DIV256)
				SRI(TCCR0,T_DIV64);	
			else if(a_timerPrescaler == T_DIV1024)
				SRI(TCCR0,T_DIV128);		
			else	
				SRI(TCCR0,a_timerPrescaler);
			
		}
		break;
		
		case TIMER2_M: {
			
			WRI(TCNT2,0);
			SRI(TCCR2,a_timerPrescaler);
			
		}
		break;
		
		default: break;
		
	}
	
}

/**********************************************************************
*
* Function:    timer_stop
*
* Description: Stops 8-bit timer module.
*
* Notes:
*
* Returns:     None.
*
**********************************************************************/

void timer_stop(timer_t a_timer){
	
	switch (a_timer){
		
		case TIMER0_M: {
			
			CRI(TCCR0,0x07);
			
		}
		break;
		
		case TIMER2_M: {
			
			CRI(TCCR2,0x07);
			
		}
		break;
		
		default: break;
		
	}
	
}

/**********************************************************************
*
* Function:    timer_setMode
*
* Description: Sets operation mode for 8-bit timer module.
*
* Notes:
*
* Returns:     None.
*
**********************************************************************/

void timer_setMode(timer_t a_timer, timermode_t a_timerMode){
	
	switch (a_timer){
		
		case TIMER0_M: {
			
			CRI(TCCR0,0x48);
			SRI(TCCR0,a_timerMode);
			
		}
		break;
		
		case TIMER2_M: {
		
			CRI(TCCR2,0x48);
			SRI(TCCR2,a_timerMode);
			
		}
		break;
		
		default: break;
		
	}
	
}

/**********************************************************************
*
* Function:    timer_setOCMode
*
* Description: Sets output compare pin operation mode for 
*              8-bit timer module.
*
* Notes:
*
* Returns:     None.
*
**********************************************************************/

void timer_setOCMode(timer_t a_timer, timerocmode_t a_timerocmode){
	
	switch (a_timer){
		
		case TIMER0_M: {
			
			CRI(TCCR0,0x30);
			if(a_timerocmode)
				gpio_setPinDirection(TIMER0_OC_PIN,IO_OUTPUT);
			SRI(TCCR0,a_timerocmode);
			
		}
		break;
		
		case TIMER2_M: {
			
			CRI(TCCR2,0x30);
			if(a_timerocmode)
				gpio_setPinDirection(TIMER2_OC_PIN,IO_OUTPUT);
			SRI(TCCR2,a_timerocmode);
			
		}
		break;
		
		default: break;
		
	}
	
}

/**********************************************************************
*
* Function:    timer_setOCR
*
* Description: Sets output compare register value for
*              8-bit timer module.
*
* Notes:
*
* Returns:     None.
*
**********************************************************************/

void timer_setOCR(timer_t a_timer, uint8_t a_ocr){
	
	switch (a_timer){
		
		case TIMER0_M: {
			
			OCR0 = a_ocr;
			
		}
		break;
		
		case TIMER2_M: {
			
			OCR2 = a_ocr;
			
		}
		break;
		
		default: break;
		
	}
	
}

/**********************************************************************
*
* Function:    timer_getTCNT
*
* Description: Gets counter value for 8-bit timer module.
*
* Notes:
*
* Returns:     TCNTx register value.
*
**********************************************************************/

uint8_t timer_getTCNT(timer_t a_timer){
	
	switch (a_timer){
		
		case TIMER0_M: {
			
			return TCNT0;
			
		}
		break;
		
		case TIMER2_M: {
			
			return TCNT2;
			
		}
		break;
		
		default: return 0;
		break;
		
	}
}

/**********************************************************************
*
* Function:    timer_checkOverflow
*
* Description: Checks if counter overflow has occurred for 
*              8-bit timer module.
*
* Notes:
*
* Returns:     Boolean value (TRUE or FALSE).
*
**********************************************************************/

bool_t timer_checkOverflow(timer_t a_timer){
	
	switch (a_timer){
		
		case TIMER0_M: {
			
			if(RBI(TIFR,TOV0)){
				
				SBI(TIFR,TOV0);
				return TRUE;
				
			}else{
				
				return FALSE;	
				
			}
			
		}
		break;
		
		case TIMER2_M: {
			
			if(RBI(TIFR,TOV2)){
				
				SBI(TIFR,TOV2);
				return TRUE;
				
			}else{
			
				return FALSE;
				
			}
			
		}
		break;
		
		default: break;
		
	}
	return 0;
	
}

/**********************************************************************
*
* Function:    timer_enableInterrupt
*
* Description: Enables interrupt request for 8-bit timer module.
*
* Notes:       This functions enables global interrupts if disabled.
*
* Returns:     None.
*
**********************************************************************/

void timer_enableInterrupt(timer_t a_timer, timerinterrupt_t a_timerInterrupt){
	
	if(!RBI(SREG,I_BIT))
		ENABLE_GLOBAL_INTERRUPTS;
	
	switch (a_timer){
		
		case TIMER0_M: {
			
			if(RBI(TIFR,a_timerInterrupt))
				SBI(TIFR,a_timerInterrupt);
			SBI(TIMSK,a_timerInterrupt);
			
		}
		break;
		
		case TIMER2_M: {
			
			if(RBI(TIFR,(a_timerInterrupt+6)))
				SBI(TIFR,(a_timerInterrupt+6));
			SBI(TIMSK,(a_timerInterrupt+6));
			
		}
		break;
		
		default: break;
		
	}
	
}

/**********************************************************************
*
* Function:    timer_disableInterrupt
*
* Description: Disables interrupt request for 8-bit timer module.
*
* Notes:       This functions doesn't disable global interrupts.
*
* Returns:     None.
*
**********************************************************************/

void timer_disableInterrupt(timer_t a_timer, timerinterrupt_t a_timerInterrupt){
	
	switch (a_timer){
		
		case TIMER0_M: {
			
			CBI(TIMSK,a_timerInterrupt);
			
		}
		break;
		
		case TIMER2_M: {
			
			CBI(TIMSK,(a_timerInterrupt+6));
			
		}
		break;
		
		default: break;
		
	}
	
}

/**********************************************************************
*
* Function:    timer_setISRCallback
*
* Description: Sets interrupt callback function for 8-bit 
*              timer module.
*
* Notes:
*
* Returns:     None.
*
**********************************************************************/

void timer_setISRCallback(timer_t a_timer, timerinterrupt_t a_timerInterrupt, ISRcallback_t a_ISRCallback){
	
	switch (a_timer){
		
		case TIMER0_M: {
			
			g_timerISRCallback[a_timerInterrupt] = a_ISRCallback;
			
		}
		break;
		
		case TIMER2_M: {
			
			g_timerISRCallback[a_timerInterrupt+2] = a_ISRCallback;
			
		}
		break;
		
		default: break;
		
	}
	
}

/*---------------------------------ISR-------------------------------*/

ISR(TIMER0_OVF_vect){
	
	g_timerISRCallback[0]();
	
}

ISR(TIMER0_COMP_vect){
	
	g_timerISRCallback[1]();
	
}

ISR(TIMER2_OVF_vect){
	
	g_timerISRCallback[2]();
	
}

ISR(TIMER2_COMP_vect){
	
	g_timerISRCallback[3]();
	
}