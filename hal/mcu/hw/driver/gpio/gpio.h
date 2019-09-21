/*
 * gpio.h
 *
 * Created: 3/31/2019 9:18:41 PM
 *  Author: Loay Ashraf
 */ 


#ifndef GPIO_H_
#define GPIO_H_

#include "hal/mcu/io/io_defs.h"
#include "hal/mcu/sys/std_types.h"

#define GPIO_INT0_PIN	PD2_M
#define GPIO_INT1_PIN	PD3_M
#define GPIO_INT2_PIN	PB2_M

typedef enum{IO_LOWER_NIBBLE=0x0F,IO_UPPER_NIBBLE=0xF0,IO_FULLPORT=0xFF}portmask_t;
typedef enum{IO_INPUT,IO_OUTPUT}datadirection_t;
typedef enum{IO_LOW,IO_HIGH}pinstate_t;
typedef enum{IO_INT0,IO_INT1,IO_INT2}gpiointerrupt_t;
typedef	enum{IO_LOW_LEVEL,IO_LOGICAL_CHANGE,IO_FALLING_EDGE,IO_RISING_EDGE}gpiointerruptmode_t;

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

#endif /* GPIO_H_ */