/*
 * timer16.c
 *
 * Created: 05/08/2019 03:13:39 PM
 *  Author: Loay Ashraf
 */ 

#include "timer16.h"
#include "hal/mcu/hw/driver/gpio/gpio.h"
#include "hal/mcu/io/io_macros.h"
#include "hal/mcu/sys/interrupt.h"

static ISRcallback_t g_timer16ISRCallback[10];

void timer16_start(timer16_t a_timer16,timer16prescaler_t a_timer16Prescaler){
	
	
	switch (a_timer16){
		
		case TIMER1_M: {
			
			WRI(TCNT1,0);
			SRI(TCCR1B,a_timer16Prescaler);
			
		}
		break;
		
		case TIMER3_M: {
			
			/*WRI(TCNT3,0);
			SRI(TCCR3B,a_timer16Prescaler);*/
			
		}
		break;
		
		default: break;
		
	}
	
}

void timer16_stop(timer16_t a_timer16){
	
	switch (a_timer16){
		
		case TIMER1_M: {
			
			CRI(TCCR1B,0x07);
			
		}
		break;
		
		case TIMER3_M: {
			
			/*CRI(TCCR3B,0x07);*/
			
		}
		break;
		
		default: break;
		
	}
	
}
void timer16_setMode(timer16_t a_timer16, timer16mode_t a_timer16Mode){
	
	
	switch (a_timer16){
		
		case TIMER1_M: {
			
			CRI(TCCR1A,0x03);
			SRI(TCCR1A,(a_timer16Mode&0x03));
			
			CRI(TCCR1B,0x18);
			SRI(TCCR1B,((a_timer16Mode&0x0C)<<1));
			
		}
		break;
		
		case TIMER3_M: {
			
			/*CRI(TCCR3A,0x03);
			SRI(TCCR3A,a_timer16Mode);
			
			CRI(TCCR3B,0x18);
			SRI(TCCR3B,((a_timer16Mode&0xC0)<<1));*/
			
		}
		break;
		
		default: break;
		
	}
	
}

void timer16_setOCAMode(timer16_t a_timer16, timer16ocmode_t a_timer16ocmode){
	
	switch (a_timer16){
		
		case TIMER1_M: {
			
			CRI(TCCR1A,0xC0);
			if(a_timer16ocmode)
				gpio_setPinDirection(TIMER1_OCA_PIN,IO_OUTPUT);
			SRI(TCCR1A,(a_timer16ocmode<<2));
			
		}
		break;
		
		case TIMER3_M: {
			
			/*CRI(TCCR3A,0xC0);
			if(a_timer16ocmode)
				gpio_setPinDirection(TIMER3_OCRA_PORT,TIMER3_OCRA_PIN,OUTPUT);
			SRI(TCCR3A,(a_timer16ocmode<<2));*/
			
		}
		break;
		
		default: break;
		
	}
	
}

void timer16_setOCBMode(timer16_t a_timer16, timer16ocmode_t a_timer16ocmode){
	
	switch (a_timer16){
		
		case TIMER1_M: {
			
			CRI(TCCR1A,0x30);
			if(a_timer16ocmode)
				gpio_setPinDirection(TIMER1_OCB_PIN,IO_OUTPUT);
			SRI(TCCR1A,a_timer16ocmode);
			
		}
		break;
		
		case TIMER3_M: {
			
			/*CRI(TCCR3A,0x30);
			if(a_timer16ocmode)
				gpio_setPinDirection(TIMER3_OCRB_PORT,TIMER3_OCRB_PIN,OUTPUT);
			SRI(TCCR3A,a_timer16ocmode);*/
			
		}
		break;
		
		default: break;
		
	}
	
}

/*
void timer16_setOCCMode(timer16_t a_timer16, timer16ocmode_t a_timer16ocmode){
	
	switch (a_timer16){
		
		case TIMER1_M: {
			
			CRI(TCCR1A,0x0C);
			if(a_timer16ocmode)
				gpio_setPinDirection(TIMER1_OCRC_PORT,TIMER1_OCRC_PIN,OUTPUT);
			SRI(TCCR1A,(a_timer16ocmode>>2));
			
		}
		break;
		
		case TIMER3_M: {
			
			CRI(TCCR3A,0x0C);
			if(a_timer16ocmode)
				gpio_setPinDirection(TIMER3_OCRC_PORT,TIMER3_OCRC_PIN,OUTPUT);
			SRI(TCCR3A,(a_timer16ocmode>>2));
			
		}
		break;
		
		default: break;
		
	}
	
}*/

void timer16_setICMode(timer16_t a_timer16, timer16icmode_t a_timer16icmode){
	
	switch (a_timer16){
		
		case TIMER1_M: {
			
			gpio_setPinDirection(TIMER1_ICP_PIN,IO_INPUT);
			CBI(TCCR1B,ICES1);
			if(a_timer16icmode)
				SBI(TCCR1B,ICES1);
			
		}
		break;
		
		case TIMER3_M: {
			
			/*gpio_setPinDirection(TIMER3_ICP_PORT,TIMER1_ICP_PIN,INPUT);
			CBI(TCCR3B,ICES3);
			if(a_timer16icmode)
				SBI(TCCR3B,ICES3);*/
			
		}
		break;
		
		default: break;
		
	}
	
}

void timer16_enableICNC(timer16_t a_timer16){
	
	switch (a_timer16){
		
		case TIMER1_M: {
			
			SBI(TCCR1B,ICNC1);
			
		}
		break;
		
		case TIMER3_M: {
			
			/*SBI(TCCR3B,ICNC3);*/
			
		}
		break;
		
		default: break;
		
	}
	
}

void timer16_disableICNC(timer16_t a_timer16){
	
	switch (a_timer16){
		
		case TIMER1_M: {
			
			CBI(TCCR1B,ICNC1);
			
		}
		break;
		
		case TIMER3_M: {
			
			/*CBI(TCCR3B,ICNC3);*/
			
		}
		break;
		
		default: break;
		
	}
	
}

void timer16_setOCRA(timer16_t a_timer16, uint16_t a_ocr){
	
	switch (a_timer16){
		
		case TIMER1_M: {
			
			OCR1A = a_ocr;
			
		}
		break;
		
		case TIMER3_M: {
			
			/*OCR3A = a_ocr;*/
			
		}
		break;
		
		default: break;
		
	}
	
}

void timer16_setOCRB(timer16_t a_timer16, uint16_t a_ocr){
	
	switch (a_timer16){
		
		case TIMER1_M: {
			
			OCR1B = a_ocr;
			
		}
		break;
		
		case TIMER3_M: {
			
			/*OCR3B = a_ocr;*/
			
		}
		break;
		
		default: break;
		
	}
	
}

/*
void timer16_setOCRC(timer16_t a_timer16, uint16_t a_ocr){
	
	switch (a_timer16){
		
		case TIMER1_M: {
			
			OCR1C = a_ocr;
			
		}
		break;
		
		case TIMER3_M: {
			
			OCR3C = a_ocr;
			
		}
		break;
		
		default: break;
		
	}
	
}*/

void timer16_setICR(timer16_t a_timer16, uint16_t a_icr){
	
	switch (a_timer16){
		
		case TIMER1_M: {
			
			ICR1 = a_icr;
			
		}
		break;
		
		case TIMER3_M: {
			
			/*ICR3 = a_icr;*/
			
		}
		break;
		
		default: break;
		
	}
	
}

uint16_t timer16_getTCNT(timer16_t a_timer16){
	
	switch (a_timer16){
		
		case TIMER1_M: {
			
			return TCNT1;
			
		}
		break;
		
		case TIMER3_M: {
			
			/*return TCNT3;*/
			
		}
		break;
		
		default: break;
		
	}
	return 0;
}

uint16_t timer16_getICR(timer16_t a_timer16){
	
	switch (a_timer16){
		
		case TIMER1_M: {
			
			return ICR1;
			
		}
		break;
		
		case TIMER3_M: {
			
			/*return ICR3;*/
			
		}
		break;
		
		default: break;
		
	}
	return 0;	
}

bool_t timer16_checkOverflow(timer16_t a_timer16){
	
	switch (a_timer16){
		
		case TIMER1_M: {
			
			if(RBI(TIFR,TOV1)){
				
				SBI(TIFR,TOV1);
				return TRUE;
				
			}else{
				
				return FALSE;	
				
			}
			
		}
		break;
		
		case TIMER3_M: {
			
			/*if(RBI(ETIFR,TOV3)){
				
				SBI(ETIFR,TOV3);
				return TRUE;
				
			}else{
			
				return FALSE;
				
			}*/
			
		}
		break;
		
		default: break;
		
	}
	return 0;
	
}

void timer16_enableInterrupt(timer16_t a_timer16, timer16interrupt_t a_timer16Interrupt){
	
	if(!RBI(SREG,I_BIT))
		ENABLE_GLOBAL_INTERRUPTS;
	
	switch (a_timer16){
		
		case TIMER1_M: {
			
			if(a_timer16Interrupt == T16_COMPC){
				
				/*if(RBI(ETIFR,a_timer16Interrupt))
					SBI(ETIFR,a_timer16Interrupt);
				SBI(ETIMSK,a_timer16Interrupt);*/
				
			}else{
				
				if(RBI(TIFR,a_timer16Interrupt))
					SBI(TIFR,a_timer16Interrupt);
				SBI(TIMSK,a_timer16Interrupt);
			
			}
		}
		break;
		
		case TIMER3_M: {
			
			/*if(a_timer16Interrupt == COMPC){
				
				if(RBI(ETIFR,(a_timer16Interrupt+1)))
					SBI(ETIFR,(a_timer16Interrupt+1));
				SBI(ETIMSK,(a_timer16Interrupt+1));
				
			}else{
			
				if(RBI(ETIFR,a_timer16Interrupt))
					SBI(ETIFR,a_timer16Interrupt);
				SBI(ETIMSK,a_timer16Interrupt);
			
			}*/
		}
		break;
		
		default: break;
		
	}
	
}

void timer16_disableInterrupt(timer16_t a_timer16, timer16interrupt_t a_timer16Interrupt){
	
	switch (a_timer16){
		
		case TIMER1_M: {
			
			if(a_timer16Interrupt == T16_COMPC){
				
				/*CBI(ETIMSK,a_timer16Interrupt);*/
				
			}else{
				
				CBI(TIMSK,a_timer16Interrupt);
				
			}
		}
		break;
		
		case TIMER3_M: {
			
			/*if(a_timer16Interrupt == COMPC){
				
				CBI(ETIMSK,(a_timer16Interrupt+1));
				
			}else{
				

				CBI(ETIMSK,a_timer16Interrupt);
				
			}*/
		}
		break;
		
		default: break;
		
	}
	
}
void timer16_setISRCallback(timer16_t a_timer16, timer16interrupt_t a_timer16Interrupt, ISRcallback_t a_ISRCallback){
	
	switch (a_timer16){
		
		case TIMER1_M: {
			
			if(a_timer16Interrupt == T16_COMPC)
				g_timer16ISRCallback[a_timer16Interrupt] = a_ISRCallback;
			else
				g_timer16ISRCallback[a_timer16Interrupt-1] = a_ISRCallback;
			
		}
		break;
		
		case TIMER3_M: {
			
			if(a_timer16Interrupt == T16_COMPC)
				g_timer16ISRCallback[a_timer16Interrupt+5] = a_ISRCallback;
			else
				g_timer16ISRCallback[a_timer16Interrupt+4] = a_ISRCallback;
			
		}
		break;
		
		default: break;
		
	}
	
}

ISR(TIMER1_OVF_vect){
	
	g_timer16ISRCallback[1]();
	
}

ISR(TIMER1_COMPA_vect){
	
	g_timer16ISRCallback[3]();
	
}

ISR(TIMER1_COMPB_vect){
	
	g_timer16ISRCallback[2]();
	
}

/*
ISR(TIMER1_COMPC_vect){
	
	g_timer16ISRCallbacks[0]();
	
}*/

ISR(TIMER1_CAPT_vect){
	
	g_timer16ISRCallback[4]();
	
}

/*
ISR(TIMER3_OVF_vect){
	
	g_timer16ISRCallbacks[6]();
	
}

ISR(TIMER3_COMPA_vect){
	
	g_timer16ISRCallbacks[8]();
	
}

ISR(TIMER3_COMPB_vect){
	
	g_timer16ISRCallbacks[7]();
	
}

ISR(TIMER3_COMPC_vect){
	
	g_timer16ISRCallbacks[5]();
	
}

ISR(TIMER3_CAPT_vect){
	
	g_timer16ISRCallbacks[9]();
	
}*/