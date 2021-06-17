/*
 * ddp_interface.h
 *
 * Created: 13/06/2021 07:01:17 AM
 *  Author: Loay Ashraf
 */

#ifndef DDP_INTERFACE_H_
#define DDP_INTERFACE_H_

#if defined (__AVR__)
    #if defined (__AVR_ATmega16__)
        #define PATH_TO_DELAY_H        "hal/ddp/avr8/mega/m16/m16_delay.h"
        #define PATH_TO_EEPROM_H       "hal/ddp/avr8/mega/m16/m16_eeprom.h"
        #define PATH_TO_INTERRUPT_H    "hal/ddp/avr8/mega/m16/m16_interrupt.h"
        #define PATH_TO_IO_H           "hal/ddp/avr8/mega/m16/m16_io.h"
        #define PATH_TO_SLEEP_H        "hal/ddp/avr8/mega/m16/m16_sleep.h"
        #define PATH_TO_AC             "hal/ddp/avr8/mega/m16/ac/m16_ac.h"
        #define PATH_TO_ADC            "hal/ddp/avr8/mega/m16/adc/m16_adc.h"
        #define PATH_TO_GPIO           "hal/ddp/avr8/mega/m16/gpio/m16_gpio.h"
        #define PATH_TO_SPI            "hal/ddp/avr8/mega/m16/spi/m16_spi.h"
        #define PATH_TO_TWI            "hal/ddp/avr8/mega/m16/twi/m16_twi.h"
        #define PATH_TO_USART          "hal/ddp/avr8/mega/m16/usart/m16_usart.h"
        #define PATH_TO_WDT            "hal/ddp/avr8/mega/m16/wdt/m16_wdt.h"
    #endif
#else
    #error "Architecture not supported yet"
#endif

#endif /* DDP_INTERFACE_H_ */
