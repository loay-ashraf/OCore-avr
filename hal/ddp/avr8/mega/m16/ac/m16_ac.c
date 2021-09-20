/**********************************************************************
*
* File:         m16_ac.c
*
* Author(s):    Loay Ashraf <loay.ashraf.96@gmail.com>
*
* Date created: 13/06/2021
*
* Description:  contains function definitions for analog comparator
*               module.
*
**********************************************************************/

/*------------------------------INCLUDES-----------------------------*/

#include "m16_ac.h"
#include "hal/ddp/avr8/mega/m16/gpio/m16_gpio.h"
#include "hal/ddp/avr8/mega/m16/m16_interrupt.h"
#include "service/include/register_macros.h"

/*--------------------------GLOBAL VARIABLES-------------------------*/
 
/**********************************************************************
*
* Variable:    g_acISRCallback
*
* Description: Holds address of interrupt callback function.
*
* Notes:
*
* Scope:       m16_ac.c.
*
**********************************************************************/
 
static ISRcallback_t g_acISRCallback;

/*-----------------------FUNCTION DEFINITIONS------------------------*/

/**********************************************************************
*
* Function:    ac_enable
*
* Description: Enables analog comparator module.
*
* Notes:
*
* Returns:     None.
*
**********************************************************************/

void ac_enable(void){
    
    gpio_setPinDirection(AC_IN0_PIN,IO_INPUT);
    gpio_setPinDirection(AC_IN1_PIN,IO_INPUT);
    CBI(ACSR,ACD);
    
}

/**********************************************************************
*
* Function:    ac_disable
*
* Description: Disables analog comparator module.
*
* Notes:
*
* Returns:     None.
*
**********************************************************************/

void ac_disable(void){
    
    SBI(ACSR,ACD);
    
}

/**********************************************************************
*
* Function:    ac_enableBandGap
*
* Description: Enables band gap feature for analog comparator 
*              module.
*
* Notes:
*
* Returns:     None.
*
**********************************************************************/

void ac_enableBandGap(void){
    
    SBI(ACSR,ACBG);    
    
}

/**********************************************************************
*
* Function:    ac_disableBandGap
*
* Description: Disables band gap feature for analog comparator 
*              module.
*
* Notes:
*
* Returns:     None.
*
**********************************************************************/

void ac_disableBandGap(void){
    
    CBI(ACSR,ACBG);
    
}

/**********************************************************************
*
* Function:    ac_enableInterrupt
*
* Description: Enables interrupt request for analog comparator 
*              module.
*
* Notes:       This function enables global interrupts if disabled.
*
* Returns:     None.
*
**********************************************************************/

void ac_enableInterrupt(void){
    
    if(!RBI(SREG,I_BIT))
        ENABLE_GLOBAL_INTERRUPTS;
    SBI(ACSR,ACIE);    
    
}

/**********************************************************************
*
* Function:    ac_disableInterrupt
*
* Description: Disables interrupt request for analog comparator 
*              module.
*
* Notes:       This function doesn't disable global interrupts.
*
* Returns:     None.
*
**********************************************************************/

void ac_disableInterrupt(void){
    
    CBI(ACSR,ACIE);
    
}

/**********************************************************************
*
* Function:    ac_setInterruptMode
*
* Description: Sets interrupt trigger mode for analog comparator 
*              module.
*
* Notes:
*
* Returns:     None.
*
**********************************************************************/

void ac_setInterruptMode(acinterruptmode_t a_acInterruptMode){
    
    CRI(ACSR,0x03);
    SRI(ACSR,a_acInterruptMode);
    
}


/**********************************************************************
*
* Function:    ac_setISRCallback
*
* Description: Sets interrupt callback function for analog comparator 
*              module.
*
* Notes:
*
* Returns:     None.
*
**********************************************************************/
 
void ac_setISRCallback(ISRcallback_t a_ISRCallback){
     
     g_acISRCallback = a_ISRCallback;
     
}

/*---------------------------------ISR-------------------------------*/

ISR(ANA_COMP_vect){
    
    g_acISRCallback();
    
}
