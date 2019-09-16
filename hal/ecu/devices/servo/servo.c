/*
 * servo.c
 *
 * Created: 07/09/2019 12:54:48 PM
 *  Author: Loay Ashraf
 */ 

#include "servo.h"

static uint16_t g_servoSpeed[2];
static uint16_t g_servoDelay[2];
static uint8_t g_servoPosition[2];

static uint16_t map(uint8_t a_input, uint16_t a_inputMin, uint16_t a_inputMax, uint16_t a_outputMin, uint16_t a_outputMax);
static void delayVar(uint16_t a_ms);

void Servo_enable(void){
	
	timer16_setMode(SERVO_TIMER,SERVO_TIMER_MODE);
	timer16_setICR(SERVO_TIMER,SERVO_TIMER_TOP);
	timer16_start(SERVO_TIMER,SERVO_TIMER_PRESCALER);
	
}

void Servo_disable(void){
	
	timer16_stop(SERVO_TIMER);
	
}

void Servo_activateChannel(servochannel_t a_servoChannel, uint16_t a_initialSpeed, uint8_t a_initialPosition){
	
	switch (a_servoChannel){
		
		case SER_CH0: {
			
			if(a_initialSpeed > SERVO_SPEED_MAX)
				a_initialSpeed = SERVO_SPEED_MAX;
			
			if(a_initialPosition < SERVO_CH0_POSITION_MIN)
				a_initialPosition = SERVO_CH0_POSITION_MIN;
			else if(a_initialPosition > SERVO_CH0_POSITION_MAX)
				a_initialPosition = SERVO_CH0_POSITION_MAX;
			
			timer16_setOCRA(SERVO_TIMER,map(a_initialPosition,SERVO_CH0_POSITION_MIN,SERVO_CH0_POSITION_MAX,SERVO_TIMER_MIN,SERVO_TIMER_MAX));
			timer16_setOCAMode(SERVO_TIMER,SERVO_TIMER_OCMODE);
			g_servoSpeed[SER_CH0] = a_initialSpeed;
			g_servoDelay[SER_CH0] = (1.0/a_initialSpeed)*1000;
			g_servoPosition[SER_CH0] = a_initialPosition;
			
		}
		break;
		
		case SER_CH1: {
			
			if(a_initialSpeed > SERVO_SPEED_MAX)
				a_initialSpeed = SERVO_SPEED_MAX;
			
			if(a_initialPosition < SERVO_CH1_POSITION_MIN)
				a_initialPosition = SERVO_CH1_POSITION_MIN;
			else if(a_initialPosition > SERVO_CH1_POSITION_MAX)
				a_initialPosition = SERVO_CH1_POSITION_MAX;
			
			timer16_setOCRB(SERVO_TIMER,map(a_initialPosition,SERVO_CH1_POSITION_MIN,SERVO_CH1_POSITION_MAX,SERVO_TIMER_MIN,SERVO_TIMER_MAX));
			timer16_setOCBMode(SERVO_TIMER,SERVO_TIMER_OCMODE);	
			g_servoSpeed[SER_CH1] = a_initialSpeed;
			g_servoDelay[SER_CH1] = (1.0/a_initialSpeed)*1000;
			g_servoPosition[SER_CH1] = a_initialPosition;
			
		}
		break;
	}
	
}

void Servo_deactivateChannel(servochannel_t a_servoChannel){
	
	switch (a_servoChannel){
		
		case SER_CH0: {
			
			timer16_setOCAMode(SERVO_TIMER,T16_OFF);
			timer16_setOCRA(SERVO_TIMER,0);
			
		}
		break;
		
		case SER_CH1: {
			
			timer16_setOCBMode(SERVO_TIMER,T16_OFF);
			timer16_setOCRB(SERVO_TIMER,0);
			
		}
		break;
	}
	
}

void Servo_setSpeed(servochannel_t a_servoChannel, uint16_t a_speed){
		
	if(a_speed > SERVO_SPEED_MAX)
		a_speed = SERVO_SPEED_MAX;
	
	g_servoSpeed[a_servoChannel] = a_speed;
	g_servoDelay[a_servoChannel] = (1.0/a_speed)*1000;
	
}

void Servo_setPositionDirect(servochannel_t a_servoChannel, uint8_t a_position){
	
	switch (a_servoChannel){
		
		case SER_CH0: {
			
			if(a_position < SERVO_CH0_POSITION_MIN)
				a_position = SERVO_CH0_POSITION_MIN;
			else if(a_position > SERVO_CH0_POSITION_MAX)
				a_position = SERVO_CH0_POSITION_MAX;
			
			g_servoPosition[SER_CH0] = a_position;
			timer16_setOCRA(SERVO_TIMER,map(a_position,SERVO_CH0_POSITION_MIN,SERVO_CH0_POSITION_MAX,SERVO_TIMER_MIN,SERVO_TIMER_MAX));
			
		}
		break;
		
		case SER_CH1: {
			
			if(a_position < SERVO_CH1_POSITION_MIN)
				a_position = SERVO_CH1_POSITION_MIN;
			else if(a_position > SERVO_CH1_POSITION_MAX)
				a_position = SERVO_CH1_POSITION_MAX;
			
			g_servoPosition[SER_CH1] = a_position;
			timer16_setOCRB(SERVO_TIMER,map(a_position,SERVO_CH1_POSITION_MIN,SERVO_CH1_POSITION_MAX,SERVO_TIMER_MIN,SERVO_TIMER_MAX));
		
		}
		break;
	}
	
}

void Servo_setPositionGrad(servochannel_t a_servoChannel, uint8_t a_position){
	
	switch (a_servoChannel){
		
		case SER_CH0: {
			
			if(a_position < SERVO_CH0_POSITION_MIN)
				a_position = SERVO_CH0_POSITION_MIN;
			else if(a_position > SERVO_CH0_POSITION_MAX)
				a_position = SERVO_CH0_POSITION_MAX;
			
			int16_t numSteps = a_position-g_servoPosition[SER_CH0];
			uint8_t delayOffset;
			
			if(g_servoSpeed[SER_CH0] <= 10)
				delayOffset = 17;
			else
				delayOffset = 0;	
			
			if(numSteps > 0){
				
				while(numSteps--){
					
					delayVar(g_servoDelay[SER_CH0]-delayOffset);	
					Servo_setPositionDirect(SER_CH0,g_servoPosition[SER_CH0]+1);
					
				}
				
			}else if(numSteps < 0){
		
				while(numSteps++ < 0){
						
				    delayVar(g_servoDelay[SER_CH0]-delayOffset);
					Servo_setPositionDirect(SER_CH0,g_servoPosition[SER_CH0]-1);
						
				}	
				
			}					
		}
		break;
		
		case SER_CH1: {
			
			if(a_position < SERVO_CH1_POSITION_MIN)
				a_position = SERVO_CH1_POSITION_MIN;
			else if(a_position > SERVO_CH1_POSITION_MAX)
				a_position = SERVO_CH1_POSITION_MAX;
			
			int16_t numSteps = a_position-g_servoPosition[SER_CH1];
			uint8_t delayOffset;
			
			if(g_servoSpeed[SER_CH1] <= 10)
				delayOffset = 17;
			else
				delayOffset = 0;
			
			if(numSteps > 0){
				
				while(numSteps--){
					
					delayVar(g_servoDelay[SER_CH1]-delayOffset);
					Servo_setPositionDirect(SER_CH1,g_servoPosition[SER_CH1]+1);
					
				}
				
			}else if(numSteps < 0){
				
				while(numSteps++ < 0){
					
					delayVar(g_servoDelay[SER_CH1]-delayOffset);
					Servo_setPositionDirect(SER_CH1,g_servoPosition[SER_CH1]-1);
					
				}
				
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

static void delayVar(uint16_t a_ms){
	
	while(a_ms--)
		DELAY_MS(1);
	
}