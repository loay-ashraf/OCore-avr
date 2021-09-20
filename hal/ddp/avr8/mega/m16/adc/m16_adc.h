/**********************************************************************
*
* File:         m16_adc.h
*
* Author(s):    Loay Ashraf <loay.ashraf.96@gmail.com>
*
* Date created: 13/06/2021
*
* Description:  contains definitions, configurations, typedefs
*               and function declarations for analog-digital
*               converter module.
*
**********************************************************************/

#ifndef M16_ADC_H_
#define M16_ADC_H_

/*------------------------------INCLUDES-----------------------------*/

#include "hal/ddp/avr8/mega/m16/m16_io.h"
#include "service/include/std_types.h"

/*-------------------DEFINITIONS AND CONFIGURATIONS------------------*/

#define ADC_PORT    PORTA_M

/*--------------------------------ENUMS------------------------------*/

typedef enum{AD_CH0,AD_CH1,AD_CH2,AD_CH3,AD_CH4,AD_CH5,AD_CH6,AD_CH7}adcchannel_t;
typedef enum{AD_DIV1,AD_DIV2,AD_DIV4,AD_DIV8,AD_DIV16,AD_DIV32,AD_DIV64,AD_DIV128}adcprescaler_t;
typedef enum{AD_AREF,AD_AVCC,AD_IREF}adcreference_t;
typedef enum{AD_FR,AD_AC=0x20,AD_INT0,AD_T0_COMP,AD_T0_OVF,AD_T1_COMPB,AD_T1_OVF,AD_T1_CAPT}adcautotriggersource_t;

/*------------------------FUNCTION DECLARATIONS----------------------*/

void adc_config(adcprescaler_t a_adcPrescaler, adcreference_t a_adcReference, bool_t a_leftAdjust);
void adc_enable(void);
void adc_disable(void);
uint16_t adc_read(adcchannel_t a_adcChannel);
void adc_enableAutoTrigger(adcautotriggersource_t a_adcAutoTriggerSource);
void adc_disableAutoTrigger(void);
void adc_enableInterrupt(void);
void adc_disableInterrupt(void);
void adc_setISRCallback(ISRcallback_t a_ISRCallback);

#endif /* M16_ADC_H_ */
