/**********************************************************************
*
* File:         keypad.c
*
* Author(s):    Loay Ashraf <loay.ashraf.96@gmail.com>
*
* Date created: 15/01/2018
*
* Description:	contains function definitions for matrix keypad
*               module.
*
**********************************************************************/

/*------------------------------INCLUDES-----------------------------*/

#include "keypad.h"
#include "hal/mcu/hw/driver/gpio/gpio.h"
#include "hal/mcu/hw/driver/timer16/timer16.h"
#include "hal/mcu/sys/delay.h"

/*--------------------------GLOBAL VARIABLES-------------------------*/

/**********************************************************************
*
* Variable:    g_keyMap
*
* Description: Stores the key map of connected keypad as 2D array.
*
* Notes:
*
* Scope:       keypad.c.
*
**********************************************************************/
 
static char g_keyMap[KEYPAD_ROWS][KEYPAD_COLUMNS];

/**********************************************************************
*
* Variable:    g_keyHandlerCallback
*
* Description: Holds address of key handler callback function.
*
* Notes:
*
* Scope:       keypad.c.
*
**********************************************************************/

static keyhandler_t g_keyHandlerCallback;

/*------------------------FUNCTION PROTOTYPES------------------------*/
 
static void _periodicScanISR(void);

/*-----------------------FUNCTION DEFINITIONS------------------------*/

/**********************************************************************
*
* Function:    Keypad_init
*
* Description: Initializes matrix keypad module.
*
* Notes:
*
* Returns:     None.
*
**********************************************************************/

void Keypad_init(void){
	
	#if (KEYPAD_CONFIG == 1)
	
		gpio_setPortDirection(KEYPAD_ROW_PORT,KEYPAD_ROW_PORT_MASK,IO_OUTPUT);
		gpio_setPortDirection(KEYPAD_COLUMN_PORT,KEYPAD_COLUMN_PORT_MASK,IO_INPUT);
		gpio_clearPort(KEYPAD_ROW_PORT,KEYPAD_ROW_PORT_MASK);
	
	#elif (KEYPAD_CONFIG == -1)

		gpio_setPortDirection(KEYPAD_ROW_PORT,KEYPAD_ROW_PORT_MASK,IO_OUTPUT);
		gpio_setPortDirection(KEYPAD_COLUMN_PORT,KEYPAD_COLUMN_PORT_MASK,IO_INPUT);
		gpio_setPort(KEYPAD_ROW_PORT,KEYPAD_ROW_PORT_MASK);	
	
	#endif			
  
}

/**********************************************************************
*
* Function:    Keypad_setKeyMap
*
* Description: Sets key map for connected matrix keypad.
*
* Notes:
*
* Returns:     None.
*
**********************************************************************/
 
void Keypad_setKeyMap(char a_keyMap[KEYPAD_ROWS][KEYPAD_COLUMNS]){
	
	uint8_t rows,columns;
	
	for(rows = 0; rows < KEYPAD_ROWS; rows++){
		
		for(columns = 0; columns < KEYPAD_COLUMNS; columns++)
			
			g_keyMap[rows][columns] = a_keyMap[rows][columns];
				
	}  
}

/**********************************************************************
*
* Function:    Keypad_enablePeriodicScan
*
* Description: Enables periodic scan for keypad matrix module
*              using 16-bit TIMER1 module.
*
* Notes:       This function enables global interrupts if disabled.
*
* Returns:     None.
*
**********************************************************************/
 
void Keypad_enablePeriodicScan(keyhandler_t a_keyHandlerCallback, uint8_t a_scanFrequency){
	  
	g_keyHandlerCallback = a_keyHandlerCallback;
	uint16_t ICR1Value = F_CPU/(64*a_scanFrequency);
	  
	timer16_setMode(KEYPAD_SCAN_TIMER,KEYPAD_SCAN_TIMER_MODE);
	timer16_enableInterrupt(KEYPAD_SCAN_TIMER,KEYPAD_SCAN_TIMER_INT);
	timer16_setISRCallback(KEYPAD_SCAN_TIMER,KEYPAD_SCAN_TIMER_INT,&_periodicScanISR);
	timer16_setICR(KEYPAD_SCAN_TIMER,ICR1Value);
	timer16_start(KEYPAD_SCAN_TIMER,KEYPAD_SCAN_TIMER_PRE);
	
}

/**********************************************************************
*
* Function:    Keypad_disablePeriodicScan
*
* Description: Disables periodic scan for keypad matrix module.
*
* Notes:       This function doesn't disable global interrupts.
*
* Returns:     None.
*
**********************************************************************/
  
void Keypad_disablePeriodicScan(void){
	  
	timer16_disableInterrupt(KEYPAD_SCAN_TIMER,KEYPAD_SCAN_TIMER_INT);
	timer16_setICR(KEYPAD_SCAN_TIMER,0);
	timer16_stop(KEYPAD_SCAN_TIMER);
	  
}

/**********************************************************************
*
* Function:    Keypad_scan
*
* Description: Scans if button is pressed in keypad matrix.
*
* Notes:
*
* Returns:     Pressed key value ('\0' if no key is pressed) 
*              (type: char).
*
**********************************************************************/

char Keypad_scan(void){

	uint8_t rows,columns;
 
	for(rows=KEYPAD_ROW_PORT_OFFSET;rows<KEYPAD_ROW_PORT_OFFSET+KEYPAD_ROWS;rows++){
	 
		#if (KEYPAD_CONFIG == 1)
			
			gpio_setPin((pin_t)(KEYPAD_ROW_PORT+rows));
		 
		#elif (KEYPAD_CONFIG == -1)
			
			gpio_clearPin((pin_t)(KEYPAD_ROW_PORT+rows));
		 
		#endif

		for (columns=KEYPAD_COLUMN_PORT_OFFSET;columns<KEYPAD_COLUMN_PORT_OFFSET+KEYPAD_COLUMNS;columns++){

			#if (KEYPAD_CONFIG == 1)
				
				if (gpio_readPin((pin_t)(KEYPAD_COLUMN_PORT+columns))){
					
					DELAY_MS(100);
					
					if (gpio_readPin((pin_t)(KEYPAD_COLUMN_PORT+columns))){	
						
						gpio_clearPin((pin_t)(KEYPAD_ROW_PORT+rows));
						return g_keyMap[rows-KEYPAD_ROW_PORT_OFFSET][columns-KEYPAD_COLUMN_PORT_OFFSET];
						
					}
				}
					
			#elif (KEYPAD_CONFIG == -1)
				
				if (!gpio_readPin((pin_t)(KEYPAD_COLUMN_PORT+columns))){
					
					DELAY_MS(100);
					
					if (!gpio_readPin((pin_t)(KEYPAD_COLUMN_PORT+columns))){
						
						gpio_setPin((pin_t)(KEYPAD_ROW_PORT+rows));
						return g_keyMap[rows-KEYPAD_ROW_PORT_OFFSET][columns-KEYPAD_COLUMN_PORT_OFFSET];
						
					}
				}
					
			#endif	
		}
	 
		#if (KEYPAD_CONFIG == 1)
		 
			gpio_clearPin((pin_t)(KEYPAD_ROW_PORT+rows));
		 
		#elif (KEYPAD_CONFIG == -1)
		 
			gpio_setPin((pin_t)(KEYPAD_ROW_PORT+rows));
		 
		#endif
	
	}
		
	return '\0';
}

/**********************************************************************
*
* Function:    _periodicScanISR
*
* Description: Initiates matrix keypad scan and calls 
*              g_keyHandlerCallback function at a defined rate (frequency).
*
* Notes:
*
* Returns:     None.
*
**********************************************************************/
 
static void _periodicScanISR(void){
	 
	char key = Keypad_scan();
	 
	if(key != '\0')
		g_keyHandlerCallback(key);
	 
}