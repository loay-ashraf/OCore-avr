/**********************************************************************
*
* File:         dac_config.h
*
* Author(s):    Loay Ashraf <loay.ashraf.96@gmail.com>
*
* Date created: 15/09/2019
*
* Description:  contains definitions and configurations for
*               digital-analog converter module.
*
**********************************************************************/

#ifndef DAC_CONFIG_H_
#define DAC_CONFIG_H_

/*-------------------DEFINITIONS AND CONFIGURATIONS------------------*/

#define DAC_CH0_TIMER              TIMER0_M
#define DAC_CH0_TIMER_MODE         T_FAST_PWM
#define DAC_CH0_TIMER_OCMODE       T_CLEAR
#define DAC_CH0_TIMER_PRESCALER    T_DIV8
#define DAC_CH1_TIMER              TIMER1_M
#define DAC_CH1_TIMER_MODE         T16_FAST_PWM_ICR
#define DAC_CH1_TIMER_OCMODE       T16_CLEAR
#define DAC_CH1_TIMER_PRESCALER    T16_DIV8
#define DAC_CH1_TIMER_TOP          ((F_CPU/56000.0)-1)
#define DAC_CH2_TIMER              TIMER1_M
#define DAC_CH2_TIMER_MODE         T16_FAST_PWM_ICR
#define DAC_CH2_TIMER_OCMODE       T16_CLEAR
#define DAC_CH2_TIMER_PRESCALER    T16_DIV8
#define DAC_CH2_TIMER_TOP          ((F_CPU/56000.0)-1)
#define DAC_CH3_TIMER              TIMER2_M
#define DAC_CH3_TIMER_MODE         T_FAST_PWM
#define DAC_CH3_TIMER_OCMODE       T_CLEAR
#define DAC_CH3_TIMER_PRESCALER    T_DIV8
#define DAC_CH0_DUTY_MIN           0
#define DAC_CH0_DUTY_MAX           100
#define DAC_CH1_DUTY_MIN           0
#define DAC_CH1_DUTY_MAX           100
#define DAC_CH2_DUTY_MIN           0
#define DAC_CH2_DUTY_MAX           100
#define DAC_CH3_DUTY_MIN           0
#define DAC_CH3_DUTY_MAX           100


#endif /* DAC_CONFIG_H_ */
