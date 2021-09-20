/**********************************************************************
*
* File:         timer.h
*
* Author(s):    Loay Ashraf <loay.ashraf.96@gmail.com>
*
* Date created: 16/05/2019
*
* Description:  contains function-like adapter macros for timer module.
*
**********************************************************************/

#ifndef TIMER_H_
#define TIMER_H_
 
#include "hal/ddp/ddp_interface.h"
#include PATH_TO_TIMER

#define TIMER_START(TIMER,TIMER_PRESCALER)                           timer_start(TIMER,TIMER_PRESCALER)
#define TIMER_STOP(TIMER)                                            timer_stop(TIMER)
#define TIMER_SET_MODE(TIMER,TIMER_MODE)                             timer_setMode(TIMER,TIMER_MODE)
#define TIMER_SET_OC_MODE(TIMER,TIMER_OC_MODE)                       timer_setOCMode(TIMER,TIMER_OC_MODE)
#define TIMER_SET_OCR(TIMER,OCR)                                     timer_setOCR(TIMER,OCR)
#define TIMER_GET_TCNT(TIMER)                                        timer_getTCNT(TIMER)
#define TIMER_CHECK_OVERFLOW(TIMER)                                  timer_checkOverflow(TIMER)
#define TIMER_ENABLE_INTERRUPT(TIMER,TIMER_INTERRUPT)                timer_enableInterrupt(TIMER,TIMER_INTERRUPT)
#define TIMER_DISABLE_INTERRUPT(TIMER,TIMER_INTERRUPT)               timer_disableInterrupt(TIMER,TIMER_INTERRUPT)
#define TIMER_SET_ISR_CALLBACK(TIMER,TIMER_INTERRUPT,ISR_CALLBACK)   timer_setISRCallback(TIMER,TIMER_INTERRUPT,ISR_CALLBACK)

#endif /* TIMER_H_ */
