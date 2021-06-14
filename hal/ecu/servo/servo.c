/**********************************************************************
*
* File:         servo.c
*
* Author(s):    Loay Ashraf <loay.ashraf.96@gmail.com>
*
* Date created: 12/03/2019
*
* Description:	contains function definitions for servo motor
*               module.
*
**********************************************************************/

/*------------------------------INCLUDES-----------------------------*/

#include "servo.h"
#include "hal/mcu/peripheral/timer16/timer16.h"
#include "service/include/map.h"
#include "service/include/delay_var.h"

/*--------------------------GLOBAL VARIABLES-------------------------*/

/**********************************************************************
*
* Variable:    g_servoSpeed
*
* Description: Stores current speed for each of servo motor
*              module channels.
*
* Notes:
*
* Scope:       servo.c.
*
**********************************************************************/

static uint16_t g_servoSpeed[SERVO_CHANNELS_NUMBER];

/**********************************************************************
*
* Variable:    g_servoDelay
*
* Description: Stores current delay required to achieve current speed 
*              for each of servo motor module channels.
*
* Notes:
*
* Scope:       servo.c.
*
**********************************************************************/

static uint16_t g_servoDelay[SERVO_CHANNELS_NUMBER];

/**********************************************************************
*
* Variable:    g_servoPosition
*
* Description: Stores current shaft position (in degrees) for each 
*              of servo motor module channels.
*
* Notes:
*
* Scope:       servo.c.
*
**********************************************************************/

static uint8_t g_servoPosition[SERVO_CHANNELS_NUMBER];

/*-----------------------FUNCTION DEFINITIONS------------------------*/

/**********************************************************************
*
* Function:    Servo_enable
*
* Description: Enables servo motor module.
*
* Notes:
*
* Returns:     None.
*
**********************************************************************/

void Servo_enable(void){
	
	timer16_setMode(SERVO_TIMER,SERVO_TIMER_MODE);
	timer16_setICR(SERVO_TIMER,SERVO_TIMER_TOP);
	timer16_start(SERVO_TIMER,SERVO_TIMER_PRESCALER);
	
}

/**********************************************************************
*
* Function:    Servo_disable
*
* Description: Disables servo motor module.
*
* Notes:
*
* Returns:     None.
*
**********************************************************************/

void Servo_disable(void){
	
	timer16_stop(SERVO_TIMER);
	
}

/**********************************************************************
*
* Function:    Servo_activateChannel
*
* Description: Activates servo motor module channel.
*
* Notes:       This function activates 16-bit TIMER1 module.
*
* Returns:     None.
*
**********************************************************************/

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

/**********************************************************************
*
* Function:    Servo_deactivateChannel
*
* Description: Deactivates servo motor module channel.
*
* Notes:       This function doesn't deactivate 16-bit TIMER1 module.
*
* Returns:     None.
*
**********************************************************************/

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

/**********************************************************************
*
* Function:    Servo_setSpeed
*
* Description: Sets speed (in deg/sec) for servo motor module channel.
*
* Notes:
*
* Returns:     None.
*
**********************************************************************/

void Servo_setSpeed(servochannel_t a_servoChannel, uint16_t a_speed){
		
	if(a_speed > SERVO_SPEED_MAX)
		a_speed = SERVO_SPEED_MAX;
	
	g_servoSpeed[a_servoChannel] = a_speed;
	g_servoDelay[a_servoChannel] = (1.0/a_speed)*1000;
	
}

/**********************************************************************
*
* Function:    Servo_setPositionDirect
*
* Description: Moves shaft at maximum possible speed to desired 
*              position for servo motor module channel.
*
* Notes:
*
* Returns:     None.
*
**********************************************************************/

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

/**********************************************************************
*
* Function:    Servo_setPositionGrad
*
* Description: Moves shaft at current speed to desired position 
*              for servo motor module channel.
*
* Notes:
*
* Returns:     None.
*
**********************************************************************/

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
					
					delayVarms(g_servoDelay[SER_CH0]-delayOffset);	
					Servo_setPositionDirect(SER_CH0,g_servoPosition[SER_CH0]+1);
					
				}
				
			}else if(numSteps < 0){
		
				while(numSteps++ < 0){
						
				    delayVarms(g_servoDelay[SER_CH0]-delayOffset);
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
					
					delayVarms(g_servoDelay[SER_CH1]-delayOffset);
					Servo_setPositionDirect(SER_CH1,g_servoPosition[SER_CH1]+1);
					
				}
				
			}else if(numSteps < 0){
				
				while(numSteps++ < 0){
					
					delayVarms(g_servoDelay[SER_CH1]-delayOffset);
					Servo_setPositionDirect(SER_CH1,g_servoPosition[SER_CH1]-1);
					
				}
				
			}
		}
		break;
	}
	
}
