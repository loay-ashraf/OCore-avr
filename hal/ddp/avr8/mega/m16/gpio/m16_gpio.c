/**********************************************************************
*
* File:         m16_gpio.c
*
* Author(s):    Loay Ashraf <loay.ashraf.96@gmail.com>
*
* Date created: 12/06/2021
*
* Description:  contains function definitions for gpio module.
*
**********************************************************************/

/*------------------------------INCLUDES-----------------------------*/

#include "m16_gpio.h"
#include "hal/ddp/avr8/mega/m16/m16_interrupt.h"
#include "service/include/register_macros.h"

/*--------------------------GLOBAL VARIABLES-------------------------*/

/**********************************************************************
*
* Variable:    g_gpioISRCallback
*
* Description: Holds addresses of interrupt callback functions.
*
* Notes:
*
* Scope:       gpio.c.
*
**********************************************************************/

static ISRcallback_t g_gpioISRCallback[3];

/*-----------------------FUNCTION DEFINITIONS------------------------*/

/**********************************************************************
*
* Function:    gpio_setPinDirection
*
* Description: Sets data direction for specific pin.
*
* Notes:
*
* Returns:     None.
*
**********************************************************************/

void gpio_setPinDirection(pin_t a_pin, datadirection_t a_dataDirection){
    
    if(a_pin < 8){
        
        if(a_dataDirection)
        SBI(DDRA,a_pin);
        else
        CBI(DDRA,a_pin);
        
    }else if(a_pin > 7 && a_pin < 16){
        
        if(a_dataDirection)
        SBI(DDRB,(a_pin-8));
        else
        CBI(DDRB,(a_pin-8));
        
    }else if(a_pin > 15 && a_pin < 24){
        
        if(a_dataDirection)
        SBI(DDRC,(a_pin-16));
        else
        CBI(DDRC,(a_pin-16));
        
    }else if(a_pin > 23 && a_pin < 32){
        
        if(a_dataDirection)
        SBI(DDRD,(a_pin-24));
        else
        CBI(DDRD,(a_pin-24));
        
    }
    
}

/**********************************************************************
*
* Function:    gpio_setPortDirection
*
* Description: Sets data direction for specific port.
*
* Notes:
*
* Returns:     None.
*
**********************************************************************/

void gpio_setPortDirection(port_t a_port, portmask_t a_portMask, datadirection_t a_dataDirection){
    
    switch (a_port){
        
        case PORTA_M:    {
            if(a_dataDirection)
            SRI(DDRA,a_portMask);
            else
            CRI(DDRA,a_portMask);
        }
        break;
        
        case PORTB_M:    {
            if(a_dataDirection)
            SRI(DDRB,a_portMask);
            else
            CRI(DDRB,a_portMask);
        }
        break;
        
        case PORTC_M:    {
            if(a_dataDirection)
            SRI(DDRC,a_portMask);
            else
            CRI(DDRC,a_portMask);
        }
        break;
        
        case PORTD_M:    {
            if(a_dataDirection)
            SRI(DDRD,a_portMask);
            else
            CRI(DDRD,a_portMask);
        }
        break;
        
    }
    
}

/**********************************************************************
*
* Function:    gpio_enablePinPullUp
*
* Description: Enables internal pull-up resistors for specific pin.
*
* Notes:
*
* Returns:     None.
*
**********************************************************************/

void gpio_enablePinPullUp(pin_t a_pin){
    
    gpio_setPinDirection(a_pin,IO_INPUT);
    gpio_setPin(a_pin);
    
}

/**********************************************************************
*
* Function:    gpio_enablePortPullUp
*
* Description: Enables internal pull-up resistors for specific port.
*
* Notes:
*
* Returns:     None.
*
**********************************************************************/

void gpio_enablePortPullUp(port_t a_port, portmask_t a_portMask){
    
    gpio_setPortDirection(a_port,a_portMask,IO_INPUT);
    gpio_setPort(a_port,a_portMask);
    
}

/**********************************************************************
*
* Function:    gpio_setPin
*
* Description: Outputs HIGH logic level on specific pin.
*
* Notes:       Pin should be set as an output first.
*
* Returns:     None.
*
**********************************************************************/

void gpio_setPin(pin_t a_pin){
    
    if(a_pin < 8)
    SBI(PORTA,a_pin);
    else if(a_pin > 7 && a_pin < 16)
    SBI(PORTB,(a_pin-8));
    else if(a_pin > 15 && a_pin < 24)
    SBI(PORTC,(a_pin-16));
    else if(a_pin > 23 && a_pin < 32)
    SBI(PORTD,(a_pin-24));
    
}

/**********************************************************************
*
* Function:    gpio_clearPin
*
* Description: Outputs LOW logic level on specific pin.
*
* Notes:       Pin should be set as an output first.
*
* Returns:     None.
*
**********************************************************************/

void gpio_clearPin(pin_t a_pin){

    if(a_pin < 8)
    CBI(PORTA,a_pin);
    else if(a_pin > 7 && a_pin < 16)
    CBI(PORTB,(a_pin-8));
    else if(a_pin > 15 && a_pin < 24)
    CBI(PORTC,(a_pin-16));
    else if(a_pin > 23 && a_pin < 32)
    CBI(PORTD,(a_pin-24));
    
}

/**********************************************************************
*
* Function:    gpio_togglePin
*
* Description: Toggles logic level on specific pin (LOW -> HIGH,
*              HIGH -> LOW).
*
* Notes:       Pin should be set as an output first.
*
* Returns:     None.
*
**********************************************************************/

void gpio_togglePin(pin_t a_pin){
    
    if(a_pin < 8)
    TBI(PORTA,a_pin);
    else if(a_pin > 7 && a_pin < 16)
    TBI(PORTB,(a_pin-8));
    else if(a_pin > 15 && a_pin < 24)
    TBI(PORTC,(a_pin-16));
    else if(a_pin > 23 && a_pin < 32)
    TBI(PORTD,(a_pin-24));
    
}

/**********************************************************************
*
* Function:    gpio_writePort
*
* Description: Writes specific value directly to PORTx register.
*
* Notes:       Port should be set as an output first.
*
* Returns:     None.
*
**********************************************************************/

void gpio_writePort(port_t a_port, ubyte_t a_value){
    
    switch (a_port){
        
        case PORTA_M:    WRI(PORTA,a_value);
        break;
        
        case PORTB_M:    WRI(PORTB,a_value);
        break;
        
        case PORTC_M:    WRI(PORTC,a_value);
        break;
        
        case PORTD_M:    WRI(PORTD,a_value);
        break;
        
    }
    
}

/**********************************************************************
*
* Function:    gpio_setPort
*
* Description: Outputs HIGH logic level on specific port.
*
* Notes:       Port should be set as an output first.
*
* Returns:     None.
*
**********************************************************************/

void gpio_setPort(port_t a_port, portmask_t a_portMask){
    
    switch (a_port){
        
        case PORTA_M:    SRI(PORTA,a_portMask);
        break;
        
        case PORTB_M:    SRI(PORTB,a_portMask);
        break;
        
        case PORTC_M:    SRI(PORTC,a_portMask);
        break;
        
        case PORTD_M:    SRI(PORTD,a_portMask);
        break;
    }
    
}

/**********************************************************************
*
* Function:    gpio_clearPort
*
* Description: Outputs LOW logic level on specific port.
*
* Notes:       Port should be set as an output first.
*
* Returns:     None.
*
**********************************************************************/

void gpio_clearPort(port_t a_port, portmask_t a_portMask){
    
    switch (a_port){
        
        case PORTA_M:    CRI(PORTA,a_portMask);
        break;
        
        case PORTB_M:    CRI(PORTB,a_portMask);
        break;
        
        case PORTC_M:    CRI(PORTC,a_portMask);
        break;
        
        case PORTD_M:    CRI(PORTD,a_portMask);
        break;
    }
    
}

/**********************************************************************
*
* Function:    gpio_readPin
*
* Description: Reads state of specific pin.
*
* Notes:       Pin should be set as an input first.
*
* Returns:     Pin state (HIGH or LOW).
*
**********************************************************************/

pinstate_t gpio_readPin(pin_t a_pin){
    
    if(a_pin < 8)
    return RBI(PINA,a_pin);
    else if(a_pin > 7 && a_pin < 16)
    return RBI(PINB,(a_pin-8));
    else if(a_pin > 15 && a_pin < 24)
    return RBI(PINC,(a_pin-16));
    else if(a_pin > 23 && a_pin < 32)
    return RBI(PIND,(a_pin-24));
    else
    return 0;
    
}

/**********************************************************************
*
* Function:    gpio_readPort
*
* Description: Reads state of specific port.
*
* Notes:       Port should be set as an input first.
*
* Returns:     PINx register value.
*
**********************************************************************/

ubyte_t gpio_readPort(port_t a_port){
    
    switch (a_port){
        
        case PORTA_M:    return RRI(PINA);
        break;
        
        case PORTB_M:    return RRI(PINB);
        break;
        
        case PORTC_M:    return RRI(PINC);
        break;
        
        case PORTD_M:    return RRI(PIND);
        break;
    }
    
    return 0;
    
}

/**********************************************************************
*
* Function:    gpio_enableInterrupt
*
* Description: Enables external interrupts (INT0,INT1,INT2).
*
* Notes:       This function enables global interrupts if disabled.
*
* Returns:     None.
*
**********************************************************************/

void gpio_enableInterrupt(gpiointerrupt_t a_interrupt){
    
    if(!RBI(SREG,I_BIT))
    ENABLE_GLOBAL_INTERRUPTS;
    
    switch(a_interrupt){
        
        case IO_INT0: SBI(GICR,INT0);
        break;
        
        case IO_INT1: SBI(GICR,INT1);
        break;
        
        case IO_INT2: SBI(GICR,INT2);
        break;
        
    }
    
}

/**********************************************************************
*
* Function:    gpio_disableInterrupt
*
* Description: Disables external interrupts (INT0,INT1,INT2).
*
* Notes:       This function doesn't disable global interrupts.
*
* Returns:     None.
*
**********************************************************************/

void gpio_disableInterrupt(gpiointerrupt_t a_interrupt){
    
    switch(a_interrupt){
        
        case IO_INT0: CBI(GICR,INT0);
        break;
        
        case IO_INT1: CBI(GICR,INT1);
        break;
        
        case IO_INT2: CBI(GICR,INT2);
        break;
        
    }
    
}

/**********************************************************************
*
* Function:    gpio_setInterruptMode
*
* Description: Sets interrupt trigger mode for specific external
*              interrupt.
*
* Notes:
*
* Returns:     None.
*
**********************************************************************/

void gpio_setInterruptMode(gpiointerrupt_t a_interrupt, gpiointerruptmode_t a_interruptMode){
    
    
    switch(a_interrupt){
        
        case IO_INT0: {
            if(a_interruptMode == IO_LOW_LEVEL){
                
                CBI(MCUCR,ISC00);
                CBI(MCUCR,ISC01);
                
                }else if(a_interruptMode == IO_LOGICAL_CHANGE){
                
                SBI(MCUCR,ISC00);
                CBI(MCUCR,ISC01);
                
                }else if(a_interruptMode == IO_FALLING_EDGE){
                
                CBI(MCUCR,ISC00);
                SBI(MCUCR,ISC01);
                
                }else if(a_interruptMode == IO_RISING_EDGE){
                
                SBI(MCUCR,ISC00);
                SBI(MCUCR,ISC01);
                
            }
        }
        break;
        
        case IO_INT1: {
            if(a_interruptMode == IO_LOW_LEVEL){
                
                CBI(MCUCR,ISC10);
                CBI(MCUCR,ISC11);
                
                }else if(a_interruptMode == IO_LOGICAL_CHANGE){
                
                SBI(MCUCR,ISC10);
                CBI(MCUCR,ISC11);
                
                }else if(a_interruptMode == IO_FALLING_EDGE){
                
                CBI(MCUCR,ISC10);
                SBI(MCUCR,ISC11);
                
                }else if(a_interruptMode == IO_RISING_EDGE){
                
                SBI(MCUCR,ISC10);
                SBI(MCUCR,ISC11);
                
            }
        }
        break;
        
        case IO_INT2: {
            if(a_interruptMode == IO_FALLING_EDGE){
                
                CBI(MCUCSR,ISC2);
                
                }else if(a_interruptMode == IO_RISING_EDGE){
                
                SBI(MCUCSR,ISC2);
                
            }
        }
        break;
        
    }
    
}

/**********************************************************************
*
* Function:    gpio_clearInterruptFlag
*
* Description: Clears interrupt flag for specific external
*              interrupt.
*
* Notes:
*
* Returns:     None.
*
**********************************************************************/

void gpio_clearInterruptFlag(gpiointerrupt_t a_interrupt){
    
    switch(a_interrupt){
        
        case IO_INT0: SBI(GIFR,INTF0);
        break;
        
        case IO_INT1: SBI(GIFR,INTF1);
        break;
        
        case IO_INT2: SBI(GIFR,INTF2);
        break;
        
    }
    
}

/**********************************************************************
*
* Function:    gpio_setISRCallback
*
* Description: Sets interrupt callback function for specific external
*              interrupt.
*
* Notes:
*
* Returns:     None.
*
**********************************************************************/

void gpio_setISRCallback(gpiointerrupt_t a_interrupt, ISRcallback_t a_ISRCallback){
    
    switch(a_interrupt){
        
        case IO_INT0: g_gpioISRCallback[0] = a_ISRCallback;
        break;
        
        case IO_INT1: g_gpioISRCallback[1] = a_ISRCallback;
        break;
        
        case IO_INT2: g_gpioISRCallback[2] = a_ISRCallback;
        break;
        
    }
}

/*---------------------------------ISR-------------------------------*/

ISR(INT0_vect){
    
    g_gpioISRCallback[0]();
    
}

ISR(INT1_vect){
    
    g_gpioISRCallback[1]();
    
}

ISR(INT2_vect){
    
    g_gpioISRCallback[2]();
    
}
