/*
 * servo.h
 *
 * Created: 07/09/2019 12:55:12 PM
 *  Author: Loay Ashraf
 */ 

#ifndef SERVO_H_
#define SERVO_H_

/*#include "../../config/ecu_depend.h"*/
#include "servo_config.h"
#include "hal/mcu/sys/std_types.h"

typedef enum{SER_CH0,SER_CH1}servochannel_t;

void Servo_enable(void);
void Servo_disable(void);
void Servo_activateChannel(servochannel_t a_servoChannel, uint16_t a_initialSpeed, uint8_t a_initialPosition);
void Servo_deactivateChannel(servochannel_t a_servoChannel);
void Servo_setSpeed(servochannel_t a_servoChannel, uint16_t a_speed);
void Servo_setPositionDirect(servochannel_t a_servoChannel, uint8_t a_position);
void Servo_setPositionGrad(servochannel_t a_servoChannel, uint8_t a_position);

#endif /* SERVO_H_ */