/**********************************************************************
*
* File:         m16_timer.h
*
* Author(s):    Loay Ashraf <loay.ashraf.96@gmail.com>
*
* Date created: 20/09/2021
*
* Description:  contains definitions, configurations, typedefs
*               and function declarations for 8-bit timer module.
*
**********************************************************************/

#ifndef M16_TIMER_H_
#define M16_TIMER_H_

/*------------------------------INCLUDES-----------------------------*/

#include "hal/mcu/sys/io.h"
#include "service/include/std_types.h"

/*-------------------DEFINITIONS AND CONFIGURATIONS------------------*/

#define TIMER0_OC_PIN    PB3_M
#define TIMER0_T_PIN     PB0_M
#define TIMER2_OC_PIN    PD7_M
#define TIMER2_T1_PIN    PC6_M
#define TIMER2_T2_PIN    PC7_M

/*--------------------------------ENUMS------------------------------*/

typedef enum{TIMER0_M,TIMER2_M}timer_t;
typedef enum{T_NORMAL,T_PHASE_CORRECT=0x40,T_CTC=0x08,T_FAST_PWM=0x48}timermode_t;
typedef enum{T_DIV1=0x01,T_DIV8,T_DIV32,T_DIV64,T_DIV128,T_DIV256,T_DIV1024,T_EXT_FALLING=0x06,T_EXT_RISING}timerprescaler_t;
typedef enum{T_OFF,T_TOGGLE=0x10,T_CLEAR=0x20,T_SET=0x30}timerocmode_t;        
typedef enum{T_OVF,T_COMP}timerinterrupt_t;

/*------------------------FUNCTION DECLARATIONS----------------------*/    

void timer_start(timer_t a_timer,timerprescaler_t a_timerPrescaler);
void timer_stop(timer_t a_timer);
void timer_setMode(timer_t a_timer, timermode_t a_timerMode);
void timer_setOCMode(timer_t a_timer, timerocmode_t a_timerocmode);
void timer_setOCR(timer_t a_timer, uint8_t a_ocr);
uint8_t timer_getTCNT(timer_t a_timer);
bool_t timer_checkOverflow(timer_t a_timer);
void timer_enableInterrupt(timer_t a_timer, timerinterrupt_t a_timerInterrupt);
void timer_disableInterrupt(timer_t a_timer, timerinterrupt_t a_timerInterrupt);
void timer_setISRCallback(timer_t a_timer, timerinterrupt_t a_timerInterrupt, ISRcallback_t a_ISRCallback);

#endif /* M16_TIMER_H_ */
