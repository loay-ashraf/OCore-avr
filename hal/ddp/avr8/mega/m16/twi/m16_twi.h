/**********************************************************************
*
* File:         m16_twi.h
*
* Author(s):    Loay Ashraf <loay.ashraf.96@gmail.com>
*
* Date created: 16/06/2021
*
* Description:  contains definitions and function declarations
*               for two-wire interface module.
*
**********************************************************************/

#ifndef M16_TWI_H_
#define M16_TWI_H_

/*------------------------------INCLUDES-----------------------------*/

#include "hal/ddp/avr8/mega/m16/m16_io.h"
#include "service/include/std_types.h"

/*-------------------DEFINITIONS AND CONFIGURATIONS------------------*/

#define TWI0_M_SDA_PIN    PC1_M
#define TWI0_M_SCL_PIN    PC0_M

#define ERROR             FALSE
#define PASS              TRUE
#define WRITE             FALSE
#define READ              TRUE
#define START             0x08
#define R_START           0x10
#define SLA_W_ACK         0x18
#define SLA_R_ACK         0x40
#define TX_ACK_MASTER     0x28
#define RX_ACK_MASTER     0x50
#define TX_ACK_SLAVE      0xC8

/*--------------------------------ENUMS------------------------------*/

typedef enum{TWI0_M}twi_t;
typedef enum{TWI_DIV1,TWI_DIV4,TWI_DIV16,TWI_DIV64}twiprescaler_t;

/*-------------------------------STRUCTS-----------------------------*/

typedef struct{

    twiprescaler_t prescaler;
    uint32_t frequency;
    uint8_t address;
    bool_t generalcall;
    bool_t ackbit;
    
}twiconfig_t;

/*------------------------FUNCTION DECLARATIONS----------------------*/    

void twi_config(twi_t a_twi, twiconfig_t * a_twiConfig);
void twi_enable(twi_t a_twi);
void twi_disable(twi_t a_twi);
bool_t twi_transmitStart(twi_t a_twi, ubyte_t a_address, bool_t a_readWrite);
void twi_transmitStop(twi_t a_twi);
bool_t twi_transmitByte(twi_t a_twi, ubyte_t a_data);
ubyte_t twi_receiveByte(twi_t a_twi);
void twi_enableInterrupt(twi_t a_twi);
void twi_disableInterrupt(twi_t a_twi);
void twi_setISRCallback(twi_t a_twi, ISRcallback_t a_twiISRCallback);
ubyte_t twi_getStatus(twi_t a_twi);

#endif /* M16_TWI_H_ */
