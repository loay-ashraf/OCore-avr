/*
 * dac.h
 *
 * Created: 15/09/2019 10:38:16 PM
 *  Author: Loay Ashraf
 */ 


#ifndef DAC_H_
#define DAC_H_

#include "dac_config.h"
#include "hal/mcu/sys/std_types.h"

typedef enum{DA_CH0,DA_CH1,DA_CH2,DA_CH3}dacchannel_t;

void dac_setDuty(dacchannel_t a_dacChannel, uint8_t a_duty);

#endif /* DAC_H_ */