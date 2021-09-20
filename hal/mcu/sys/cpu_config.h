/**********************************************************************
*
* File:         cpu_config.h
*
* Author(s):    Loay Ashraf <loay.ashraf.96@gmail.com>
*
* Date created: 07/08/2019
*
* Description:  contains CPU clock definitions (in HZ, KHZ and MHZ).
*
**********************************************************************/

#ifndef CPU_CONFIG_H_
#define CPU_CONFIG_H_

/*-------------------DEFINITIONS AND CONFIGURATIONS------------------*/

#ifndef F_CPU
#define F_CPU      16000000UL //16MHz Clock frequency
#endif
#define F_CPU_K    16000
#define F_CPU_M    16

#endif /* CPU_CONFIG_H_ */
