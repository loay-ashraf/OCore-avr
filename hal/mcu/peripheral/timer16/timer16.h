/**********************************************************************
*
* File:         timer16.h
*
* Author(s):	Loay Ashraf <loay.ashraf.96@gmail.com>
*
* Date created: 05/08/2019
*
* Description:	contains definitions, configurations, typedefs
*               and function declarations for 16-bit timer module.
*
**********************************************************************/ 

#ifndef TIMER16_H_
#define TIMER16_H_

/*------------------------------INCLUDES-----------------------------*/

#include "hal/mcu/io/io_defs.h"
#include "service/include/std_types.h"

/*-------------------DEFINITIONS AND CONFIGURATIONS------------------*/

#define TIMER1_OCA_PIN		PD5_M
#define TIMER1_OCB_PIN		PD4_M
/*#define TIMER1_OCC_PIN		PB7_M*/
#define TIMER1_ICP_PIN		PD6_M
#define TIMER1_T_PIN		PB1_M

/*#define TIMER3_OCA_PIN		PE3_M
#define TIMER3_OCB_PIN		PE4_M
#define TIMER3_OCC_PIN		PE5_M
#define TIMER3_ICP_PIN		PE7_M
#define TIMER3_T_PIN		PE6_M*/

/*--------------------------------ENUMS------------------------------*/

typedef enum{TIMER1_M,TIMER3_M}timer16_t;
typedef enum{T16_NORMAL,T16_PHASE_CORRECT_8,T16_PHASE_CORRECT_9,T16_PHASE_CORRECT_10,T16_CTC_OCRA,T16_FAST_PWM_8,T16_FAST_PWM_9,T16_FAST_PWM_10,T16_PHASE_FREQUENCY_CORRECT_ICR,
			T16_PHASE_FREQUENCY_CORRECT_OCRA,T16_PHASE_CORRECT_ICR,T16_PHASE_CORRECT_OCRA,T16_CTC_ICR,T16_FAST_PWM_ICR=0X0E,T16_FAST_PWM_OCRA}timer16mode_t;
typedef enum{T16_DIV1=0x01,T16_DIV8,T16_DIV64,T16_DIV256,T16_DIV1024,T16_EXT_FALLING,T16_EXT_RISING}timer16prescaler_t;
typedef enum{T16_OFF,T16_TOGGLE=0x10,T16_CLEAR=0x20,T16_SET=0x30}timer16ocmode_t;	
typedef enum{T16_FALLING_EDGE,T16_RISING_EDGE}timer16icmode_t;		
typedef	enum{T16_OVF=0x02,T16_COMPA=0x04,T16_COMPB=0x03,T16_COMPC=0x00,T16_CAPT=0x05}timer16interrupt_t;	
	
/*------------------------FUNCTION DECLARATIONS----------------------*/			
	
void timer16_start(timer16_t a_timer16,timer16prescaler_t a_timer16Prescaler);
void timer16_stop(timer16_t a_timer16);
void timer16_setMode(timer16_t a_timer16, timer16mode_t a_timer16Mode);
void timer16_setOCAMode(timer16_t a_timer16, timer16ocmode_t a_timer16ocmode);
void timer16_setOCBMode(timer16_t a_timer16, timer16ocmode_t a_timer16ocmode);
/*void timer16_setOCCMode(timer16_t a_timer16, timer16ocmode_t a_timer16ocmode);*/
void timer16_setICMode(timer16_t a_timer16, timer16icmode_t a_timer16icmode);
void timer16_enableICNC(timer16_t a_timer16);
void timer16_disableICNC(timer16_t a_timer16);
void timer16_setOCRA(timer16_t a_timer16, uint16_t a_ocr);
void timer16_setOCRB(timer16_t a_timer16, uint16_t a_ocr);
/*void timer16_setOCRC(timer16_t a_timer16, uint16_t a_ocr);*/
void timer16_setICR(timer16_t a_timer16, uint16_t a_icr);
uint16_t timer16_getTCNT(timer16_t a_timer16);
uint16_t timer16_getICR(timer16_t a_timer16);
bool_t timer16_checkOverflow(timer16_t a_timer16);
void timer16_enableInterrupt(timer16_t a_timer16, timer16interrupt_t a_timer16Interrupt);
void timer16_disableInterrupt(timer16_t a_timer16, timer16interrupt_t a_timer16Interrupt);
void timer16_setISRCallback(timer16_t a_timer16, timer16interrupt_t a_timer16Interrupt, ISRcallback_t a_ISRCallback);

#endif /* TIMER16_H_ */
