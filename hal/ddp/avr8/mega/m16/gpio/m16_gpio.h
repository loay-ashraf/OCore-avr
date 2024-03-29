/**********************************************************************
*
* File:         m16_gpio.h
*
* Author(s):    Loay Ashraf <loay.ashraf.96@gmail.com>
*
* Date created: 12/06/2021
*
* Description:  contains definitions, configurations, typedefs
*               and function declarations for gpio module.
*
**********************************************************************/

#ifndef M16_GPIO_H_
#define M16_GPIO_H_

/*------------------------------INCLUDES-----------------------------*/

#include "hal/ddp/avr8/mega/m16/m16_io.h"
#include "service/include/std_types.h"

/*-------------------DEFINITIONS AND CONFIGURATIONS------------------*/

#define GPIO_INT0_PIN    PD2_M
#define GPIO_INT1_PIN    PD3_M
#define GPIO_INT2_PIN    PB2_M

/*--------------------------------ENUMS------------------------------*/

typedef enum{IO_LOWER_NIBBLE=0x0F,IO_UPPER_NIBBLE=0xF0,IO_FULLPORT=0xFF}portmask_t;
typedef enum{IO_INPUT,IO_OUTPUT}datadirection_t;
typedef enum{IO_LOW,IO_HIGH}pinstate_t;
typedef enum{IO_INT0,IO_INT1,IO_INT2}gpiointerrupt_t;
typedef    enum{IO_LOW_LEVEL,IO_LOGICAL_CHANGE,IO_FALLING_EDGE,IO_RISING_EDGE}gpiointerruptmode_t;

/*------------------------FUNCTION DECLARATIONS----------------------*/

void gpio_setPinDirection(pin_t a_pin, datadirection_t a_dataDirection);
void gpio_setPortDirection(port_t a_port, portmask_t a_portMask, datadirection_t a_dataDirection);
void gpio_enablePinPullUp(pin_t a_pin);
void gpio_enablePortPullUp(port_t a_port, portmask_t a_portMask);
void gpio_setPin(pin_t a_pin);
void gpio_clearPin(pin_t a_pin);
void gpio_togglePin(pin_t a_pin);
void gpio_writePort(port_t a_port, ubyte_t a_value);
void gpio_setPort(port_t a_port, portmask_t a_portMask);
void gpio_clearPort(port_t a_port, portmask_t a_portMask);
pinstate_t gpio_readPin(pin_t a_pin);
ubyte_t gpio_readPort(port_t a_port);
void gpio_enableInterrupt(gpiointerrupt_t a_interrupt);
void gpio_disableInterrupt(gpiointerrupt_t a_interrupt);
void gpio_setInterruptMode(gpiointerrupt_t a_interrupt, gpiointerruptmode_t a_interruptMode);
void gpio_clearInterruptFlag(gpiointerrupt_t a_interrupt);
void gpio_setISRCallback(gpiointerrupt_t a_interrupt, ISRcallback_t a_ISRCallback);

#endif /* M16_GPIO_H_ */
