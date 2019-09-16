/*
 * dac.h
 *
 * Created: 15/09/2019 10:38:16 PM
 *  Author: Loay Ashraf
 */ 


#ifndef DAC_H_
#define DAC_H_

#include "../../../io/io_defs.h"
#include "../../../sys/delay.h"
#include "../../../sys/std_types.h"
#include "../../driver/gpio/gpio.h"
#include "../../driver/timer/timer.h"
#include "../../driver/timer16/timer16.h"
#include "dac_config.h"

typedef enum{DA_CH0,DA_CH1,DA_CH2,DA_CH3}dacchannel_t;

void dac_setDuty(dacchannel_t a_dacChannel, uint8_t a_duty);

#endif /* DAC_H_ */