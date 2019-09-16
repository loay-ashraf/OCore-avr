/*
 * std_types.h
 *
 *  Created on: 5/12/2019 5:34:59 PM
 *  Author: Loay Ashraf
 */

#ifndef STD_TYPES_H_
#define STD_TYPES_H_

#define NONE	0

typedef signed char byte_t;
typedef signed short word_t;
typedef signed long dword_t;
typedef unsigned char ubyte_t;
typedef unsigned short uword_t;
typedef unsigned long udword_t;

typedef enum{FALSE,TRUE}bool_t;

typedef union{
	
	byte_t value;
	
	struct{
		
		byte_t bit0 :1;
		byte_t bit1 :1;
		byte_t bit2 :1;
		byte_t bit3 :1;
		byte_t bit4 :1;
		byte_t bit5 :1;
		byte_t bit6 :1;
		byte_t bit7 :1;
		
	};
	
}registerbyte_t;

typedef union{
	
	word_t value;
	
	struct{
		
		byte_t bit0 :1;
		byte_t bit1 :1;
		byte_t bit2 :1;
		byte_t bit3 :1;
		byte_t bit4 :1;
		byte_t bit5 :1;
		byte_t bit6 :1;
		byte_t bit7 :1;
		byte_t bit8 :1;
		byte_t bit9 :1;
		byte_t bit10 :1;
		byte_t bit11 :1;
		byte_t bit12 :1;
		byte_t bit13 :1;
		byte_t bit14 :1;
		byte_t bit15 :1;
		
	};
	
}registerword_t;

typedef union{
	
	dword_t value;
	
	struct{
		
		byte_t bit0 :1;
		byte_t bit1 :1;
		byte_t bit2 :1;
		byte_t bit3 :1;
		byte_t bit4 :1;
		byte_t bit5 :1;
		byte_t bit6 :1;
		byte_t bit7 :1;
		byte_t bit8 :1;
		byte_t bit9 :1;
		byte_t bit10 :1;
		byte_t bit11 :1;
		byte_t bit12 :1;
		byte_t bit13 :1;
		byte_t bit14 :1;
		byte_t bit15 :1;
		byte_t bit16 :1;
		byte_t bit17 :1;
		byte_t bit18 :1;
		byte_t bit19 :1;
		byte_t bit20 :1;
		byte_t bit21 :1;
		byte_t bit22 :1;
		byte_t bit23 :1;
		byte_t bit24 :1;
		byte_t bit25 :1;
		byte_t bit26 :1;
		byte_t bit27 :1;
		byte_t bit28 :1;
		byte_t bit29 :1;
		byte_t bit30 :1;
		byte_t bit31 :1;
		
	};
	
}registerdword_t;

typedef union{
	
	ubyte_t value;
	
	struct{
		
		ubyte_t bit0 :1;
		ubyte_t bit1 :1;
		ubyte_t bit2 :1;
		ubyte_t bit3 :1;
		ubyte_t bit4 :1;
		ubyte_t bit5 :1;
		ubyte_t bit6 :1;
		ubyte_t bit7 :1;
		
	};
	
}registerubyte_t;

typedef union{
	
	uword_t value;
	
	struct{
		
		ubyte_t bit0 :1;
		ubyte_t bit1 :1;
		ubyte_t bit2 :1;
		ubyte_t bit3 :1;
		ubyte_t bit4 :1;
		ubyte_t bit5 :1;
		ubyte_t bit6 :1;
		ubyte_t bit7 :1;
		ubyte_t bit8 :1;
		ubyte_t bit9 :1;
		ubyte_t bit10 :1;
		ubyte_t bit11 :1;
		ubyte_t bit12 :1;
		ubyte_t bit13 :1;
		ubyte_t bit14 :1;
		ubyte_t bit15 :1;
		
	};
	
}registeruword_t;

typedef union{
	
	udword_t value;
	
	struct{
		
		ubyte_t bit0 :1;
		ubyte_t bit1 :1;
		ubyte_t bit2 :1;
		ubyte_t bit3 :1;
		ubyte_t bit4 :1;
		ubyte_t bit5 :1;
		ubyte_t bit6 :1;
		ubyte_t bit7 :1;
		ubyte_t bit8 :1;
		ubyte_t bit9 :1;
		ubyte_t bit10 :1;
		ubyte_t bit11 :1;
		ubyte_t bit12 :1;
		ubyte_t bit13 :1;
		ubyte_t bit14 :1;
		ubyte_t bit15 :1;
		ubyte_t bit16 :1;
		ubyte_t bit17 :1;
		ubyte_t bit18 :1;
		ubyte_t bit19 :1;
		ubyte_t bit20 :1;
		ubyte_t bit21 :1;
		ubyte_t bit22 :1;
		ubyte_t bit23 :1;
		ubyte_t bit24 :1;
		ubyte_t bit25 :1;
		ubyte_t bit26 :1;
		ubyte_t bit27 :1;
		ubyte_t bit28 :1;
		ubyte_t bit29 :1;
		ubyte_t bit30 :1;
		ubyte_t bit31 :1;
		
	};
	
}registerudword_t;

#endif /* STD_TYPES_H_ */
