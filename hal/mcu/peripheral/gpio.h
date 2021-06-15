/**********************************************************************
*
* File:         gpio.h
*
* Author(s):    Loay Ashraf <loay.ashraf.96@gmail.com>
*
* Date created: 31/03/2019
*
* Description:  contains function-like adapter macros for gpio module.
*
**********************************************************************/

#ifndef GPIO_H_
#define GPIO_H_

#include "hal/ddp/ddp_interface.h"
#include PATH_TO_GPIO

#define GPIO_SET_PIN_DIRECTION(PIN,DATA_DIRECTION)                gpio_setPinDirection(PIN,DATA_DIRECTION)
#define GPIO_SET_PORT_DIRECTION(PORT,PORT_MASK,DATA_DIRECTION)    gpio_setPortDirection(PORT,PORT_MASK,DATA_DIRECTION)
#define GPIO_ENABLE_PIN_PULLUP(PIN)                               gpio_enablePinPullUp(PIN)
#define GPIO_ENABLE_PORT_PULLUP(PORT,PORT_MASK)                   gpio_enablePortPullUp(PORT,PORT_MASK)
#define GPIO_SET_PIN(PIN)                                         gpio_setPin(PIN)
#define GPIO_CLEAR_PIN(PIN)                                       gpio_clearPin(PIN)
#define GPIO_TOGGLE_PIN(PIN)                                      gpio_togglePin(PIN)
#define GPIO_WRITE_PORT(PORT,PORT_VALUE)                          gpio_writePort(PORT,PORT_VALUE)
#define GPIO_SET_PORT(PORT,PORT_MASK)                             gpio_setPort(PORT,PORT_MASK)
#define GPIO_CLEAR_PORT(PORT,PORT_MASK)                           gpio_clearPort(PORT,PORT_MASK)
#define GPIO_READ_PIN(PIN)                                        gpio_readPin(PIN)
#define GPIO_READ_PORT(PORT)                                      gpio_readPort(PORT)
#define GPIO_ENABLE_INTERRUPT(INTERRUPT)                          gpio_enableInterrupt(INTERRUPT)
#define GPIO_DISABLE_INTERRUPT(INTERRUPT)                         gpio_disableInterrupt(INTERRUPT)
#define GPIO_SET_INTERRUPT_MODE(INTERRUPT,INTERRUPT_MODE)         gpio_setInterruptMode(INTERRUPT,INTERRUPT_MODE)
#define GPIO_CLEAR_INTERRUPT_FLAG(INTERRUPT)                      gpio_clearInterruptFlag(INTERRUPT)
#define GPIO_SET_ISR_CALLBACK(INTERRUPT,ISR_CALLBACK)             gpio_setISRCallback(INTERRUPT,ISR_CALLBACK)

#endif /* GPIO_H_ */
