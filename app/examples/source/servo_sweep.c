/*
 * servo_sweep.c
 *
 * Created: 15/09/2019 07:02:19 PM
 *  Author: Loay Ashraf
 */ 

#include "app/examples/include/examples.h"
#include "hal/ecu/servo/servo.h"
#include "hal/mcu/sys/delay.h"

void ex_servo_sweep(void){
    
    int16_t pos;    /* variable to hold servo position */
    
    /******************************/
    /* initialize Servo interface */
    /******************************/
    
    Servo_enable();
    Servo_activateChannel(SER_CH0,100,0);
    
    while(1){
        
        for (pos = 0; pos <= 180; pos += 1) {        // goes from 0 degrees to 180 degrees
            
            // in steps of 1 degree
            Servo_setPositionDirect(SER_CH0,pos);   // tell servo to go to position in variable 'pos'
            DELAY_MS(15);                            // waits 15ms for the servo to reach the position
            
        }
        
        for (pos = 180; pos >= 0; pos -= 1){        // goes from 180 degrees to 0 degrees
            
            Servo_setPositionDirect(SER_CH0,pos);   // tell servo to go to position in variable 'pos'
            DELAY_MS(15);                            // waits 15ms for the servo to reach the position
            
        }
    
    }
    
}