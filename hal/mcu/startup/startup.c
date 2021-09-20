/**********************************************************************
*
* File:         startup.c
*
* Author(s):    Loay Ashraf <loay.ashraf.96@gmail.com>
*
* Date created: 05/06/2019
*
* Description:    contains MCU initialization function definition.
*
**********************************************************************/

/*------------------------------INCLUDES-----------------------------*/

#include "startup.h"

/*------------------------FUNCTION PROTOTYPES------------------------*/

/*void __attribute__((constructor)) startup();*/    // uncomment if need startup code before calling main()

/*-----------------------FUNCTION DEFINITIONS------------------------*/

/**********************************************************************
*
* Function:    mcu_boot
*
* Description: Gets called before main() is called during MCU boot 
*              process.
*
* Notes:       This function should contain calls to initialization 
*              functions.
*
* Returns:     None.
*
**********************************************************************/

void startup(void){

    /*call initializers here*/

}
