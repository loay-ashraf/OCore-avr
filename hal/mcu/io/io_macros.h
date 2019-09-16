/*
 * io_macros.h
 *
 * Created: 5/15/2019 1:41:34 PM
 *  Author: Loay Ashraf
 */ 

#ifndef IO_MACROS_H_
#define IO_MACROS_H_

#define SBI(REG,BIT)	(REG|=_BV(BIT))
#define CBI(REG,BIT)	(REG&=~_BV(BIT))
#define TBI(REG,BIT)	(REG^=_BV(BIT))
#define RBI(REG,BIT)	((REG>>BIT)&0x01)
#define SRI(REG,MASK)	(REG|=MASK)
#define CRI(REG,MASK)	(REG&=~MASK)
#define WRI(REG,MASK)	(REG=MASK)
#define RRI(REG)		(REG)

#endif /* IO_MACROS_H_ */