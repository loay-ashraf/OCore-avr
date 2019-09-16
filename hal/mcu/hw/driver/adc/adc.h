/** @file adc.h
 *  @brief This driver supports interfacing ADC modules of 8-bit AVR MCU;
 *	this file includes function prototypes for ADC.c
 *  @author Loay Ashraf
 *	@version 1.0.0
 *  @pre include delay.h and math.h in micro_config.h
 *  @bug no known bugs
 *  @see micro_config.h
 */


#ifndef ADC_H_
#define ADC_H_

//------------INCLUDE MCU CONFIGURATION------------//

#include "../../../io/io_defs.h"
#include "../../../io/io_macros.h"
#include "../../../sys/delay.h"
#include "../../../sys/interrupt.h"
#include "../../../sys/std_types.h"
#include "../gpio/gpio.h"

#define ADC_PORT	PORTA_M

//------------DEFINE ADC PARAETERS ABBREVIATIONS------------//

typedef enum{AD_CH0,AD_CH1,AD_CH2,AD_CH3,AD_CH4,AD_CH5,AD_CH6,AD_CH7}adcchannel_t;
typedef enum{AD_DIV1,AD_DIV2,AD_DIV4,AD_DIV8,AD_DIV16,AD_DIV32,AD_DIV64,AD_DIV128}adcprescaler_t;	
typedef enum{AD_AREF,AD_AVCC,AD_IREF}adcreference_t;	
typedef enum{AD_FR,AD_AC=0x20,AD_INT0,AD_T0_COMP,AD_T0_OVF,AD_T1_COMPB,AD_T1_OVF,AD_T1_CAPT}adcautotriggersource_t;	

//------------FUNCTION PROTOTYPES------------//

void adc_config(adcprescaler_t a_adcPrescaler, adcreference_t a_adcReference, bool_t a_leftAdjust);
void adc_enable(void);
void adc_disable(void);
uint16_t adc_read(adcchannel_t a_adcChannel);
void adc_enableAutoTrigger(adcautotriggersource_t a_adcAutoTriggerSource);
void adc_disableAutoTrigger(void);
void adc_enableInterrupt(void);
void adc_disableInterrupt(void);
void adc_setISRCallback(ISRcallback_t a_ISRCallback);

#endif /* ADC_H_ */