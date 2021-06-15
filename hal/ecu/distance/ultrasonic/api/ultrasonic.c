/**********************************************************************
*
* File:         ultrasonic.c
*
* Author(s):    Loay Ashraf <loay.ashraf.96@gmail.com>
*
* Date created: 11/06/2021
*
* Description:  contains function definitions for Ultrasonic module.
*
**********************************************************************/

/*------------------------------INCLUDES-----------------------------*/

#include "ultrasonic.h"
#include "hal/ecu/distance/ultrasonic/common/ultrasonic_macros.h"

/*-----------------------FUNCTION DEFINITIONS------------------------*/

/**********************************************************************
*
* Function:    Ultrasonic_readcm
*
* Description: Reads distance value (in cm) from Ultrasonic module.        
*
* Notes:
*
* Returns:     Distance value (in centimeters).
*
**********************************************************************/

uint16_t Ultrasonic_readcm(void){
    
    return ULTRASONIC_READCM;
    
}

/**********************************************************************
*
* Function:    Ultrasonic_readm
*
* Description: Reads distance value (in m) from Ultrasonic module.
*
* Notes:
*
* Returns:     Distance value (in meters).
*
**********************************************************************/

uint8_t Ultrasonic_readm(void){
    
    return (ULTRASONIC_READCM/100.0);
    
}

/**********************************************************************
*
* Function:    Ultrasonic_readin
*
* Description: Reads distance value (in in) from Ultrasonic module.
*
* Notes:
*
* Returns:     Distance value (in inches).
*
**********************************************************************/

uint8_t Ultrasonic_readin(void){
    
    return (ULTRASONIC_READCM*0.393701);
    
}

/**********************************************************************
*
* Function:    Ultrasonic_readft
*
* Description: Reads distance value (in ft) from Ultrasonic module.
*
* Notes:
*
* Returns:     Distance value (in feet).
*
**********************************************************************/

uint8_t Ultrasonic_readft(void){
    
    return (ULTRASONIC_READCM*0.0328084);
    
}
