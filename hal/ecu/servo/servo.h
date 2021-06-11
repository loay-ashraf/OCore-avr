/**********************************************************************
*
* File:         servo.h
*
* Author(s):	Loay Ashraf <loay.ashraf.96@gmail.com>
*
* Date created: 12/03/2019
*
* Description:	contains typedefs and function declarations for
*               servo motor module.
*
**********************************************************************/

#ifndef SERVO_H_
#define SERVO_H_

/*------------------------------INCLUDES-----------------------------*/

#include "servo_config.h"
#include "service/include/std_types.h"

/*--------------------------------ENUMS------------------------------*/

typedef enum{SER_CH0,SER_CH1}servochannel_t;
	
/*------------------------FUNCTION DECLARATIONS----------------------*/

void Servo_enable(void);
void Servo_disable(void);
void Servo_activateChannel(servochannel_t a_servoChannel, uint16_t a_initialSpeed, uint8_t a_initialPosition);
void Servo_deactivateChannel(servochannel_t a_servoChannel);
void Servo_setSpeed(servochannel_t a_servoChannel, uint16_t a_speed);
void Servo_setPositionDirect(servochannel_t a_servoChannel, uint8_t a_position);
void Servo_setPositionGrad(servochannel_t a_servoChannel, uint8_t a_position);

#endif /* SERVO_H_ */