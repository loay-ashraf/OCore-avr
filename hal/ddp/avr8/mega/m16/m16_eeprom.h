/*
 * m16_eeprom.h
 *
 *  Created on: 15 Jun 2021
 *      Author: Loay Ashraf
 */

#ifndef M16_EEPROM_H_
#define M16_EEPROM_H_

/*------------------------------INCLUDES-----------------------------*/

#include <avr/eeprom.h>

/*-------------------------------MACROS------------------------------*/

#define EEPROM_READ_BYTE(ADDR)                eeprom_read_byte(ADDR)
#define EEPROM_READ_WORD(ADDR)                eeprom_read_word(ADDR)
#define EEPROM_READ_DWORD(ADDR)               eeprom_read_dword(ADDR)
#define EEPROM_READ_FLOAT(ADDR)               eeprom_read_float(ADDR)
#define EEPROM_READ_BLOCK(DEST,SRC,SIZE)      eeprom_read_block(DEST,SRC,VALUE)
#define EEPROM_WRITE_BYTE(ADDR,VALUE)         eeprom_write_byte(ADDR,VALUE)
#define EEPROM_WRITE_WORD(ADDR,VALUE)         eeprom_write_word(ADDR,VALUE)
#define EEPROM_WRITE_DWORD(ADDR,VALUE)        eeprom_write_dword(ADDR,VALUE)
#define EEPROM_WRITE_FLOAT(ADDR,VALUE)        eeprom_write_float(ADDR,VALUE)
#define EEPROM_WRITE_BLOCK(SRC,DEST,SIZE)     eeprom_write_block(SRC,DEST,SIZE)
#define EEPROM_UPDATE_BYTE(ADDR,VALUE)        eeprom_update_byte(ADDR,VALUE)
#define EEPROM_UPDATE_WORD(ADDR,VALUE)        eeprom_update_word(ADDR,VALUE)
#define EEPROM_UPDATE_DWORD(ADDR,VALUE)       eeprom_update_dword(ADDR,VALUE)
#define EEPROM_UPDATE_FLOAT(ADDR,VALUE)       eeprom_update_float(ADDR,VALUE)
#define EEPROM_UPDATE_BLOCK(SRC,DEST,SIZE)    eeprom_update_block(SRC,DEST,SIZE)

#endif /* M16_EEPROM_H_ */
