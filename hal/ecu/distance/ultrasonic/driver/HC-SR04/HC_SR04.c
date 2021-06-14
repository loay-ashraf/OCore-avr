/**********************************************************************
*
* File:         HC_SR04.c
*
* Author(s):    Loay Ashraf <loay.ashraf.96@gmail.com>
*
* Date created: 11/06/2021
*
* Description:	contains function definitions for HC-SR04 controller
*               module.
*
**********************************************************************/

/*------------------------------INCLUDES-----------------------------*/

#include "HC_SR04.h"
#include "HC_SR04_config.h"
#include "hal/mcu/api/pwmin/pwmin.h"
#include "hal/mcu/peripheral/gpio/gpio.h"
#include "hal/mcu/sys/delay.h"

/*-----------------------FUNCTION DEFINITIONS------------------------*/

/**********************************************************************
*
* Function:    HC_SR04_read
*
* Description: Reads distance value (in cm) from HC-SR04 controller
*              module.
*
* Notes:
*
* Returns:     Distance value (in centimeters).
*
**********************************************************************/

uint16_t HC_SR04_read(void){
	
	uint16_t echoPulseWidth;
	
	GPIO_SET_PIN_DIRECTION(HC_SR04_TRIG_PIN,IO_OUTPUT);
	GPIO_SET_PIN_DIRECTION(HC_SR04_ECHO_PIN,IO_INPUT);
	
	GPIO_CLEAR_PIN(HC_SR04_TRIG_PIN);
	DELAY_US(5);
	GPIO_SET_PIN(HC_SR04_TRIG_PIN);
	DELAY_US(HC_SR04_PULSE_WIDTH);
	GPIO_CLEAR_PIN(HC_SR04_TRIG_PIN);
	
	echoPulseWidth = PWMIN_getPulseWidth(HC_SR04_ECHO_PIN, PWMIN_RISING);
	
	if(echoPulseWidth <= 116 || echoPulseWidth > 23323)
		echoPulseWidth = 0;
	
	return echoPulseWidth*HC_SR04_DITANCE_CONSTANT;
	
}
