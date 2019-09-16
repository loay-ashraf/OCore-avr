/*
 * dac.c
 *
 * Created: 15/09/2019 10:38:06 PM
 *  Author: Loay Ashraf
 */ 

#include "dac.h"

static bool_t g_isActivated[4];

static uint16_t map(uint8_t a_input, uint16_t a_inputMin, uint16_t a_inputMax, uint16_t a_outputMin, uint16_t a_outputMax);

void dac_setDuty(dacchannel_t a_dacChannel, uint8_t a_duty){
	
	switch (a_dacChannel){
		
		case DA_CH0: {
			
			if(a_duty < DAC_CH0_DUTY_MIN)
				a_duty = DAC_CH0_DUTY_MIN;
			else if(a_duty > DAC_CH0_DUTY_MAX)
				a_duty = DAC_CH0_DUTY_MAX;
				
			if(g_isActivated[DA_CH0]){
				
				timer_setOCR(DAC_CH0_TIMER,map(a_duty,DAC_CH0_DUTY_MIN,DAC_CH0_DUTY_MAX,0,0xFF));
				
				
			}else{
				
				timer_setMode(DAC_CH0_TIMER,DAC_CH0_TIMER_MODE);
				timer_setOCR(DAC_CH0_TIMER,map(a_duty,DAC_CH0_DUTY_MIN,DAC_CH0_DUTY_MAX,0,0xFF));
				timer_setOCMode(DAC_CH0_TIMER,DAC_CH0_TIMER_OCMODE);
				timer_start(DAC_CH0_TIMER,DAC_CH0_TIMER_PRESCALER);
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
			
				timer16_setOCRA(DAC_CH0_TIMER,map(a_duty,DAC_CH1_DUTY_MIN,DAC_CH1_DUTY_MAX,0,DAC_CH1_TIMER_TOP));
			
			}else{
				
				timer16_setMode(DAC_CH1_TIMER,DAC_CH1_TIMER_MODE);
				timer16_setICR(DAC_CH1_TIMER,DAC_CH1_TIMER_TOP);
				timer16_setOCRA(DAC_CH1_TIMER,map(a_duty,DAC_CH1_DUTY_MIN,DAC_CH1_DUTY_MAX,0,DAC_CH1_TIMER_TOP));
				timer16_setOCAMode(DAC_CH1_TIMER,DAC_CH1_TIMER_OCMODE);
				timer16_start(DAC_CH1_TIMER,DAC_CH1_TIMER_PRESCALER);
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
				
				timer16_setOCRB(DAC_CH2_TIMER,map(a_duty,DAC_CH2_DUTY_MIN,DAC_CH2_DUTY_MAX,0,DAC_CH2_TIMER_TOP));
				
			}else{
				
				timer16_setMode(DAC_CH2_TIMER,DAC_CH2_TIMER_MODE);
				timer16_setICR(DAC_CH2_TIMER,DAC_CH2_TIMER_TOP);
				timer16_setOCRB(DAC_CH2_TIMER,map(a_duty,DAC_CH2_DUTY_MIN,DAC_CH2_DUTY_MAX,0,DAC_CH2_TIMER_TOP));
				timer16_setOCBMode(DAC_CH2_TIMER,DAC_CH2_TIMER_OCMODE);
				timer16_start(DAC_CH2_TIMER,DAC_CH2_TIMER_PRESCALER);
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
				
				timer_setOCR(DAC_CH3_TIMER,map(a_duty,DAC_CH3_DUTY_MIN,DAC_CH3_DUTY_MAX,0,0xFF));
				
			}else{
				
				
				timer_setMode(DAC_CH3_TIMER,DAC_CH3_TIMER_MODE);
				timer_setOCR(DAC_CH3_TIMER,map(a_duty,DAC_CH3_DUTY_MIN,DAC_CH3_DUTY_MAX,0,0xFF));
				timer_setOCMode(DAC_CH3_TIMER,DAC_CH3_TIMER_OCMODE);
				timer_start(DAC_CH3_TIMER,DAC_CH3_TIMER_PRESCALER);
				g_isActivated[DA_CH3] = TRUE;
				
			}	
			
		}
		break;
	}
	
}

static uint16_t map(uint8_t a_input, uint16_t a_inputMin, uint16_t a_inputMax, uint16_t a_outputMin, uint16_t a_outputMax){
	
	if(a_input < a_inputMin)
		a_input = a_inputMin;
	else if(a_input > a_inputMax)
		a_input = a_inputMax;
	
	float slope = (float)(a_outputMax-a_outputMin)/(float)(a_inputMax-a_inputMin);
	uint16_t output = a_outputMin+slope*(a_input-a_inputMin);
	
	return output;
}
