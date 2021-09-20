/**********************************************************************
*
* File:         register_macros.h
*
* Author(s):    Loay Ashraf <loay.ashraf.96@gmail.com>
*
* Date created: 15/05/2019
*
* Description:  contains commonly used macros for accessing
*               and modifying registers and bit manipulation.
*
**********************************************************************/

#ifndef REGISTER_MACROS_H_
#define REGISTER_MACROS_H_

/*-------------------------------MACROS------------------------------*/

#define SBI(REG,BIT)    (REG|=_BV(BIT))
#define CBI(REG,BIT)    (REG&=~_BV(BIT))
#define TBI(REG,BIT)    (REG^=_BV(BIT))
#define RBI(REG,BIT)    ((REG>>BIT)&0x01)
#define SRI(REG,MASK)    (REG|=MASK)
#define CRI(REG,MASK)    (REG&=~MASK)
#define WRI(REG,MASK)    (REG=MASK)
#define RRI(REG)        (REG)

#endif /* REGISTER_MACROS_H_ */
