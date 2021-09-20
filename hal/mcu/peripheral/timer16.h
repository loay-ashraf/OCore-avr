/**********************************************************************
*
* File:         timer16.h
*
* Author(s):    Loay Ashraf <loay.ashraf.96@gmail.com>
*
* Date created: 05/08/2019
*
* Description:  contains function-like adapter macros for
*               16-bit timer module.
*
**********************************************************************/ 

#ifndef TIMER16_H_
#define TIMER16_H_
 
#include "hal/ddp/ddp_interface.h"
#include PATH_TO_TIMER16

#define TIMER16_START(TIMER16,TIMER16_PRESCALER)                           timer16_start(TIMER16,TIMER16_PRESCALER)
#define TIMER16_STOP(TIMER16)                                              timer16_stop(TIMER16)
#define TIMER16_SET_MODE(TIMER16,TIMER16_MODE)                             timer16_setMode(TIMER16,TIMER16_MODE)
#define TIMER16_SET_OCA_MODE(TIMER16,TIMER16_OCA_MODE)                     timer16_setOCAMode(TIMER16,TIMER16_OCA_MODE)
#define TIMER16_SET_OCB_MODE(TIMER16,TIMER16_OCB_MODE)                     timer16_setOCAMode(TIMER16,TIMER16_OCB_MODE)
#define TIMER16_SET_IC_MODE(TIMER16,TIMER16_IC_MODE)                       timer16_setICMode(TIMER16,TIMER16_IC_MODE)
#define TIMER16_ENABLE_ICNC(TIMER16)                                       timer16_enableICNC(TIMER16)
#define TIMER16_DISABLE_ICNC(TIMER16)                                      timer16_disableICNC(TIMER16)
#define TIMER16_SET_OCRA(TIMER16,OCR)                                      timer16_setOCRA(TIMER16,OCR)
#define TIMER16_SET_OCRB(TIMER16,OCR)                                      timer16_setOCRB(TIMER16,OCR)
#define TIMER16_SET_ICR(TIMER16,ICR)                                       timer16_setICR(TIMER16,ICR)
#define TIMER16_GET_TCNT(TIMER16)                                          timer16_getTCNT(TIMER16)
#define TIMER16_GET_ICR(TIMER16)                                           timer16_getICR(TIMER16)
#define TIMER16_CHECK_OVERFLOW(TIMER16)                                    timer16_checkOverflow(TIMER16)
#define TIMER16_ENABLE_INTERRUPT(TIMER16,TIMER16_INTERRUPT)                timer16_enableInterrupt(TIMER16,TIMER16_INTERRUPT)
#define TIMER16_DISABLE_INTERRUPT(TIMER16,TIMER16_INTERRUPT)               timer16_disableInterrupt(TIMER16,TIMER16_INTERRUPT)
#define TIMER16_SET_ISR_CALLBACK(TIMER16,TIMER16_INTERRUPT,ISR_CALLBACK)   timer16_setISRCallback(TIMER16,TIMER16_INTERRUPT,ISR_CALLBACK)

#endif /* TIMER16_H_ */
