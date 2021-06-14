/*
 * servo_knob.c
 *
 * Created: 15/09/2019 06:50:15 PM
 *  Author: Loay Ashraf
 */ 

#include "app/examples/include/examples.h"
#include "hal/ecu/servo/servo.h"
#include "hal/mcu/peripheral/adc/adc.h"

/*****************************************************************/
/* map function to interpolate a value from one range to another */
/*****************************************************************/

static float map(uint16_t input, uint16_t input_start, uint16_t input_end, uint16_t output_start, uint16_t output_end){
	
	if(input < input_start)
	input = input_start;
	else if(input > input_end)
	input = input_end;
	
	float slope = (float)(output_end-output_start)/(float)(input_end-input_start);
	float output = output_start + (slope * (input - input_start));
	
	return output;
}

void ex_servo_knob(void){
	
	/***************************************/
	/* initialize ADC and Servo interfaces */
	/***************************************/
	
	ADC_CONFIG(AD_DIV16,AD_AVCC,FALSE);
	ADC_ENABLE;
	Servo_enable();
	Servo_activateChannel(SER_CH0,100,0);
	
	while(1)		/* loop forever */
		Servo_setPositionDirect(SER_CH0,map(ADC_READ(AD_CH0),0,1023,SERVO_CH0_POSITION_MIN,SERVO_CH0_POSITION_MAX));	/* set servo position according to the knob position */
	
}
