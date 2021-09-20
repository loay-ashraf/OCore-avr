/**********************************************************************
*
* File:         dac.h
*
* Author(s):    Loay Ashraf <loay.ashraf.96@gmail.com>
*
* Date created: 15/09/2019
*
* Description:  contains typedefs and function declarations for
*               digital-analog converter module.
*
**********************************************************************/

#ifndef DAC_H_
#define DAC_H_

/*------------------------------INCLUDES-----------------------------*/

#include "service/include/std_types.h"

/*--------------------------------ENUMS------------------------------*/

typedef enum{DA_CH0,DA_CH1,DA_CH2,DA_CH3}dacchannel_t;
    
/*------------------------FUNCTION DECLARATIONS----------------------*/

void dac_setDuty(dacchannel_t a_dacChannel, uint8_t a_duty);

#endif /* DAC_H_ */
