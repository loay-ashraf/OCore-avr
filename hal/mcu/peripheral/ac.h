/**********************************************************************
*
* File:         ac.h
*
* Author(s):    Loay Ashraf <loay.ashraf.96@gmail.com>
*
* Date created: 04/09/2019
*
* Description:  contains definitions, configurations, typedefs
*               and function declarations for analog comparator module.
*
**********************************************************************/

#ifndef AC_H_
#define AC_H_

#include "hal/ddp/ddp_interface.h"
#include PATH_TO_AC

#define AC_ENABLE                                ac_enable()
#define AC_DISABLE                               ac_disable()
#define AC_ENABLE_BAND_GAP                       ac_enableBandGap()
#define AC_DISABLE_BAND_GAP                      ac_disableBandGap()
#define AC_ENABLE_INTERRUPT                      ac_enableInterrupt()
#define AC_DISABLE_INTERRUPT                     ac_disableInterrupt()
#define AC_SET_INTERRUPT_MODE(INTERRUPT_MODE)    ac_setInterruptMode(INTERRUPT_MODE)
#define AC_SET_ISR_CALLBACK(ISR_CALLBACK)        ac_setISRCallback(ISR_CALLBACK)

#endif /* AC_H_ */
