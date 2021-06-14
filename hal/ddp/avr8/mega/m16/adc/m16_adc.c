/**********************************************************************
*
* File:         m16_adc.c
*
* Author(s):    Loay Ashraf <loay.ashraf.96@gmail.com>
*
* Date created: 13/06/2021
*
* Description:	contains function definitions for analog-digital
*               converter module.
*
**********************************************************************/

/*------------------------------INCLUDES-----------------------------*/

#include "m16_adc.h"
#include "hal/ddp/avr8/mega/m16/gpio/m16_gpio.h"
#include "hal/mcu/io/io_macros.h"
#include "hal/mcu/sys/delay.h"
#include "hal/mcu/sys/interrupt.h"

/*--------------------------GLOBAL VARIABLES-------------------------*/

/**********************************************************************
*
* Variable:    g_adcISRCallback
*
* Description: Holds address of interrupt callback function.
*
* Notes:
*
* Scope:       adc.c.
*
**********************************************************************/

static ISRcallback_t g_adcISRCallback;

/*-----------------------FUNCTION DEFINITIONS------------------------*/

/**********************************************************************
*
* Function:    adc_config
*
* Description: Configures analog-digital converter module.
*
* Notes:
*
* Returns:     None.
*
**********************************************************************/

void adc_config(adcprescaler_t a_adcPrescaler, adcreference_t a_adcReference, bool_t a_leftAdjust){

	SRI(ADMUX,(a_adcReference<<6));		// set the reference voltage bits
	SRI(ADMUX,(a_leftAdjust<<5));		// set data adjustment bits

	SRI(ADCSRA,a_adcPrescaler);			// set prescaler bits

}

/**********************************************************************
*
* Function:    adc_enable
*
* Description: Enables analog-digital converter module.
*
* Notes:
*
* Returns:     None.
*
**********************************************************************/

void adc_enable(void){
	
	SBI(ADCSRA,ADEN);					// enable the ADC
	
}

/**********************************************************************
*
* Function:    adc_disable
*
* Description: Disables analog-digital converter module.
*
* Notes:
*
* Returns:     None.
*
**********************************************************************/

void adc_disable(void){
	
	CBI(ADCSRA,ADEN);					// disable the ADC
	
}

/**********************************************************************
*
* Function:    adc_read
*
* Description: Initiates analog-digital converter module
*              conversion and reads back the result.
*
* Notes:
*
* Returns:     value stored in ADC register.
*
**********************************************************************/

uint16_t adc_read(adcchannel_t a_adcChannel){
	
	gpio_setPinDirection((ADC_PORT+a_adcChannel),IO_INPUT);
	
	CRI(ADMUX,0x0F);					// clear channel multiplexer bits
	
	if (a_adcChannel < 8)
	SRI(ADMUX,a_adcChannel);		// set channel bits
	
	SBI(ADCSRA,ADSC);					// trigger ADC conversion
	
	while(!RBI(ADCSRA,ADIF));			// wait for the conversion to finish
	
	SBI(ADCSRA,ADIF);					// clear "conversion finished" flag

	DELAY_MS(1);
	return ADC;							// return the value in the ADC Data register

}

/**********************************************************************
*
* Function:    adc_enableAutoTrigger
*
* Description: Enables auto triggering of analog-digital converter
*              module.
*
* Notes:
*
* Returns:     None.
*
**********************************************************************/

void adc_enableAutoTrigger(adcautotriggersource_t a_adcAutoTriggerSource){
	
	CRI(SFIOR,0xE0);
	SRI(SFIOR,a_adcAutoTriggerSource);
	
	SBI(ADCSRA,ADATE);
	
}

/**********************************************************************
*
* Function:    adc_enableAutoTrigger
*
* Description: Disables auto triggering of analog-digital
*              converter module.
*
* Notes:
*
* Returns:     None.
*
**********************************************************************/

void adc_disableAutoTrigger(void){
	
	CBI(ADCSRA,ADATE);
	
}

/**********************************************************************
*
* Function:    adc_enableInterrupt
*
* Description: Enables conversion complete interrupt request for
*              analog-digital converter module.
*
* Notes:       This function enables global interrupts if disabled.
*
* Returns:     None.
*
**********************************************************************/

void adc_enableInterrupt(void){
	
	if(!RBI(SREG,I_BIT))
	ENABLE_GLOBAL_INTERRUPTS;
	SBI(ADCSRA,ADIE);
	
}

/**********************************************************************
*
* Function:    adc_disableInterrupt
*
* Description: Disables conversion complete interrupt request for
*              analog-digital converter module.
*
* Notes:       This function doesn't disable global interrupts.
*
* Returns:     None.
*
**********************************************************************/

void adc_disableInterrupt(void){
	
	CBI(ADCSRA,ADIE);
	
}

/**********************************************************************
*
* Function:    adc_setISRCallback
*
* Description: Sets conversion complete interrupt callback function
*              for analog-digital converter module.
*
* Notes:
*
* Returns:     None.
*
**********************************************************************/

void adc_setISRCallback(ISRcallback_t a_ISRCallback){
	
	g_adcISRCallback = a_ISRCallback;
	
}

/*---------------------------------ISR-------------------------------*/

ISR(ADC_vect){
	
	g_adcISRCallback();
	
}
