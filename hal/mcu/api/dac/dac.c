/**********************************************************************
*
* File:         dac.c
*
* Author(s):    Loay Ashraf <loay.ashraf.96@gmail.com>
*
* Date created: 15/09/2019
*
* Description:  contains function definitions for digital-analog
*               converter module.
*
**********************************************************************/

/*------------------------------INCLUDES-----------------------------*/

#include "dac.h"
#include "dac_config.h"
#include "hal/mcu/peripheral/timer.h"
#include "hal/mcu/peripheral/timer16.h"
#include "hal/mcu/sys/cpu_config.h"
#include "service/include/map.h"

/*--------------------------GLOBAL VARIABLES-------------------------*/

/**********************************************************************
*
* Variable:    g_isActivated
*
* Description: Stores current state of digital-analog converter
*              channels (activated or not).
*
* Notes:
*
* Scope:       dac.c.
*
**********************************************************************/

static bool_t g_isActivated[4];

/*-----------------------FUNCTION DEFINITIONS------------------------*/

/**********************************************************************
*
* Function:    dac_setDuty
*
* Description: Sets duty cycle of specific channel of digital-analog
*              converter module.
*
* Notes:       This function enables 8-bit and 16-bit timers in 
*              fast PWM mode.
*              This function doesn't disable 8-bit or 16-bit timers
*              when duty is set to zero. 
*
* Returns:     None.
*
**********************************************************************/

void dac_setDuty(dacchannel_t a_dacChannel, uint8_t a_duty){
    
    switch (a_dacChannel){
        
        case DA_CH0: {
            
            if(a_duty < DAC_CH0_DUTY_MIN)
                a_duty = DAC_CH0_DUTY_MIN;
            else if(a_duty > DAC_CH0_DUTY_MAX)
                a_duty = DAC_CH0_DUTY_MAX;
                
            if(g_isActivated[DA_CH0]){
                
            	TIMER_SET_OCR(DAC_CH0_TIMER,map(a_duty,DAC_CH0_DUTY_MIN,DAC_CH0_DUTY_MAX,0,0xFF));
                
                
            }else{
                
            	TIMER_SET_MODE(DAC_CH0_TIMER,DAC_CH0_TIMER_MODE);
                TIMER_SET_OCR(DAC_CH0_TIMER,map(a_duty,DAC_CH0_DUTY_MIN,DAC_CH0_DUTY_MAX,0,0xFF));
                TIMER_SET_OC_MODE(DAC_CH0_TIMER,DAC_CH0_TIMER_OCMODE);
                TIMER_START(DAC_CH0_TIMER,DAC_CH0_TIMER_PRESCALER);
                g_isActivated[DA_CH0] = TRUE;
                
            }    
            
        }
        break;
        
        case DA_CH1: {
            
            if(a_duty < DAC_CH1_DUTY_MIN)
                a_duty = DAC_CH1_DUTY_MIN;
            else if(a_duty > DAC_CH1_DUTY_MAX)
                a_duty = DAC_CH1_DUTY_MAX;
                
            if(g_isActivated[DA_CH1]){
            
            	TIMER16_SET_OCRA(DAC_CH0_TIMER,map(a_duty,DAC_CH1_DUTY_MIN,DAC_CH1_DUTY_MAX,0,DAC_CH1_TIMER_TOP));
            
            }else{
                
            	TIMER16_SET_MODE(DAC_CH1_TIMER,DAC_CH1_TIMER_MODE);
            	TIMER16_SET_ICR(DAC_CH1_TIMER,DAC_CH1_TIMER_TOP);
                TIMER16_SET_OCRA(DAC_CH1_TIMER,map(a_duty,DAC_CH1_DUTY_MIN,DAC_CH1_DUTY_MAX,0,DAC_CH1_TIMER_TOP));
                TIMER16_SET_OCA_MODE(DAC_CH1_TIMER,DAC_CH1_TIMER_OCMODE);
                TIMER16_START(DAC_CH1_TIMER,DAC_CH1_TIMER_PRESCALER);
                g_isActivated[DA_CH1] = TRUE;
                
            }
            
        }
        break;
        
        case DA_CH2: {
            
            if(a_duty < DAC_CH2_DUTY_MIN)
                a_duty = DAC_CH2_DUTY_MIN;
            else if(a_duty > DAC_CH2_DUTY_MAX)
                a_duty = DAC_CH2_DUTY_MAX;
                
            if(g_isActivated[DA_CH2]){
                
            	TIMER16_SET_OCRB(DAC_CH2_TIMER,map(a_duty,DAC_CH2_DUTY_MIN,DAC_CH2_DUTY_MAX,0,DAC_CH2_TIMER_TOP));
                
            }else{
                
            	TIMER16_SET_MODE(DAC_CH2_TIMER,DAC_CH2_TIMER_MODE);
                TIMER16_SET_ICR(DAC_CH2_TIMER,DAC_CH2_TIMER_TOP);
                TIMER16_SET_OCRB(DAC_CH2_TIMER,map(a_duty,DAC_CH2_DUTY_MIN,DAC_CH2_DUTY_MAX,0,DAC_CH2_TIMER_TOP));
                TIMER16_SET_OCB_MODE(DAC_CH2_TIMER,DAC_CH2_TIMER_OCMODE);
                TIMER16_START(DAC_CH2_TIMER,DAC_CH2_TIMER_PRESCALER);
                g_isActivated[DA_CH2] = TRUE;
                
            }    
            
        }
        break;
        
        case DA_CH3: {
            
            if(a_duty < DAC_CH3_DUTY_MIN)
                a_duty = DAC_CH3_DUTY_MIN;
            else if(a_duty > DAC_CH3_DUTY_MAX)
                a_duty = DAC_CH3_DUTY_MAX;
                
            if(g_isActivated[DA_CH2]){
                
            	TIMER_SET_OCR(DAC_CH3_TIMER,map(a_duty,DAC_CH3_DUTY_MIN,DAC_CH3_DUTY_MAX,0,0xFF));
                
            }else{
                
                
            	TIMER_SET_MODE(DAC_CH3_TIMER,DAC_CH3_TIMER_MODE);
                TIMER_SET_OCR(DAC_CH3_TIMER,map(a_duty,DAC_CH3_DUTY_MIN,DAC_CH3_DUTY_MAX,0,0xFF));
                TIMER_SET_OC_MODE(DAC_CH3_TIMER,DAC_CH3_TIMER_OCMODE);
                TIMER_START(DAC_CH3_TIMER,DAC_CH3_TIMER_PRESCALER);
                g_isActivated[DA_CH3] = TRUE;
                
            }    
            
        }
        break;
    }
    
}
