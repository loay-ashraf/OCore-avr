/**********************************************************************
*
* File:         twi_config.h
*
* Author(s):	Loay Ashraf <loay.ashraf.96@gmail.com>
*
* Date created: 05/09/2019
*
* Description:	contains definitions and configurations for
*               two-wire interface module.
*
**********************************************************************/

#ifndef TWI_CONFIG_H_
#define TWI_CONFIG_H_

/*-------------------DEFINITIONS AND CONFIGURATIONS------------------*/

#define	TWI_SDA_PIN		PC1_M
#define TWI_SCL_PIN		PC0_M

#define TWI_ADDRESS		0b1110000
#define TWI_FREQ_K		200.0
#define TWI_GC			0

#endif /* TWI_CONFIG_H_ */