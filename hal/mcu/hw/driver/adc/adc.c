/** @file adc.c
 *  @brief includes function definitions
 *  @author Loay Ashraf
 *	@version 1.0.0
 *  @pre include ADC.h
 *  @bug no known bugs
 *  @see adc.h
 */

//------------INCLUDE DRIVER HEADER FILE------------//

 #include "adc.h"
 
 static ISRcallback_t g_adcISRCallback;

/** @brief initializes the ADC in single ended conversion mode
 *  @param Vref the reference voltage (AREF, AVCC or internal).
 *  @param Left_Adj result is left adjusted or right adjusted (FALSE for right, TRUE for left).
 *  @param Prescaler prescaler value for the ADC.
 *  @return none.
 */

 void adc_config(adcprescaler_t a_adcPrescaler, adcreference_t a_adcReference, bool_t a_leftAdjust){

	SRI(ADMUX,(a_adcReference<<6));		// set the reference voltage bits
	SRI(ADMUX,(a_leftAdjust<<5));		// set data adjustment bits

	SRI(ADCSRA,a_adcPrescaler);			// set prescaler bits

 }
 
 void adc_enable(void){
	 
	 SBI(ADCSRA,ADEN);					// enable the ADC
	 
 }
 
 void adc_disable(void){
	
	CBI(ADCSRA,ADEN);					// disable the ADC
	 
 }
 
/** @brief triggers instant conversion for the selected channel
 *  @param Channel the selected channel for the conversion.
 *  @return the conversion result (8-bit or 10-bit value).
 */

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
 
 void adc_enableAutoTrigger(adcautotriggersource_t a_adcAutoTriggerSource){
	 
	 CRI(SFIOR,0xE0);
	 SRI(SFIOR,a_adcAutoTriggerSource);
	 
	 SBI(ADCSRA,ADATE);
	 
 }
 
 void adc_disableAutoTrigger(void){
	 
	 CBI(ADCSRA,ADATE);
	 
 }
 
 void adc_enableInterrupt(void){
	 
	 if(!RBI(SREG,I_BIT))
		ENABLE_GLOBAL_INTERRUPTS;
	 SBI(ADCSRA,ADIE);
	 
 }
 
 void adc_disableInterrupt(void){
	 
	 CBI(ADCSRA,ADIE);
	 
 }
 
 void adc_setISRCallback(ISRcallback_t a_ISRCallback){
	 
	 g_adcISRCallback = a_ISRCallback;
	 
 }
 
 ISR(ADC_vect){
	 
	 g_adcISRCallback();
	 
 }