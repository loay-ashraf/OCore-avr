/*
 * m16_io.h
 *
 *  Created on: 15 Jun 2021
 *      Author: Loay Ashraf
 */

#ifndef M16_IO_H_
#define M16_IO_H_

/*------------------------------INCLUDES-----------------------------*/

#include <avr/io.h>

/*--------------------------------ENUMS------------------------------*/

typedef enum{PORTA_M,PORTB_M=0x08,PORTC_M=0x10,PORTD_M=0x18}port_t;
typedef enum{PA0_M,PA1_M,PA2_M,PA3_M,PA4_M,PA5_M,PA6_M,PA7_M,PB0_M,PB1_M,PB2_M,PB3_M,PB4_M,PB5_M,PB6_M,PB7_M,
            PC0_M,PC1_M,PC2_M,PC3_M,PC4_M,PC5_M,PC6_M,PC7_M,PD0_M,PD1_M,PD2_M,PD3_M,PD4_M,PD5_M,PD6_M,PD7_M}pin_t;

/*-------------------DEFINITIONS AND CONFIGURATIONS------------------*/

#define RAM_SIZE        1024
#define ROM_SIZE        16384
#define EEPROM_SIZE     512
#define STACK_GROWTH    -1

#define PORTS_NUM       4
#define PINS_NUM        32
#define AC_NUM          1
#define ADC_NUM         1
#define ADC_CH_NUM      8
#define SPI_NUM         1
#define TIMER_NUM       2
#define TIMER16_NUM     1
#define TWI_NUM         1
#define USART_NUM       1
#define WDT_NUM         1

#define I_BIT           0x07

#endif /* M16_IO_H_ */
