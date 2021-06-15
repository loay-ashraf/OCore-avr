/**********************************************************************
*
* File:         adc.h
*
* Author(s):    Loay Ashraf <loay.ashraf.96@gmail.com>
*
* Date created: 12/01/2018
*
* Description:    contains function-like adapter macros for
*               analog-digital converter module.
*
**********************************************************************/

#ifndef ADC_H_
#define ADC_H_

#include "hal/ddp/ddp_interface.h"
#include PATH_TO_ADC

#define ADC_CONFIG(ADC_PRESCALER,ADC_REFERENCE,ADC_LEFT_ADJUST)    adc_config(ADC_PRESCALER,ADC_REFERENCE,ADC_LEFT_ADJUST)
#define ADC_ENABLE                                                 adc_enable()
#define ADC_DISABLE                                                adc_disable()
#define ADC_READ(ADC_CHANNEL)                                      adc_read(ADC_CHANNEL)
#define ADC_ENABLE_AUTO_TRIGGER(AUTO_TRIGGER_SOURCE)               adc_enableAutoTrigger(AUTO_TRIGGER_SOURCE)
#define ADC_DISABLE_AUTO_TRIGGER                                   adc_disableAutoTrigger()
#define ADC_ENABLE_INTERRUPT                                       adc_enableInterrupt()
#define ADC_DISABLE_INTERRUPT                                      adc_disableInterrupt()
#define ADC_SET_ISR_CALLBACK(ISR_CALLBACK)                         adc_setISRCallback(ISR_CALLBACK)

#endif /* ADC_H_ */
