/** @file Keypad.c
 *  @brief includes function definition
 *  @author Loay Ashraf
 *	@version 1.0.0
 *  @pre include Keypad.h
 *  @bug no known bugs
 *	@warning the functions LCD_print_int and LCD_print_float have maximum digit number of 7 and has to be modified manually
 *  @see LCD.h
 */

//------------INCLUDE DRIVER HEADER FILE------------//

 #include "keypad.h"
 
 static char g_keyMap[KEYPAD_ROWS][KEYPAD_COLUMNS];
 static keyhandler_t g_keyHandlerCallback;
 
 static void periodicScanISR(void);

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
 
 void Keypad_setKeyMap(char a_keyMap[KEYPAD_ROWS][KEYPAD_COLUMNS]){
	
	uint8_t rows,columns;
	
	for(rows = 0; rows < KEYPAD_ROWS; rows++){
		
		for(columns = 0; columns < KEYPAD_COLUMNS; columns++)
			
			g_keyMap[rows][columns] = a_keyMap[rows][columns];
				
	}  
 }
 
 bool_t Keypad_enablePeriodicScan(keyhandler_t a_keyHandlerCallback, uint8_t a_scanFrequency){
	  
	  g_keyHandlerCallback = a_keyHandlerCallback;
	  uint16_t ICR1Value = F_CPU/(64*a_scanFrequency);
	  
	  timer16_setMode(KEYPAD_SCAN_TIMER,KEYPAD_SCAN_TIMER_MODE);
	  timer16_enableInterrupt(KEYPAD_SCAN_TIMER,KEYPAD_SCAN_TIMER_INT);
	  timer16_setISRCallback(KEYPAD_SCAN_TIMER,KEYPAD_SCAN_TIMER_INT,&periodicScanISR);
	  timer16_setICR(KEYPAD_SCAN_TIMER,ICR1Value);
	  timer16_start(KEYPAD_SCAN_TIMER,KEYPAD_SCAN_TIMER_PRE);
	  
	  return TRUE;
 }
  
 void Keypad_disablePeriodicScan(void){
	  
	  timer16_disableInterrupt(KEYPAD_SCAN_TIMER,KEYPAD_SCAN_TIMER_INT);
	  timer16_setICR(KEYPAD_SCAN_TIMER,0);
	  timer16_stop(KEYPAD_SCAN_TIMER);
	  
 }

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
						
					gpio_clearPin((pin_t)(KEYPAD_ROW_PORT+rows));
					return g_keyMap[rows-KEYPAD_ROW_PORT_OFFSET][columns-KEYPAD_COLUMN_PORT_OFFSET];
					
				}
					
			#elif (KEYPAD_CONFIG == -1)
				
				if (!gpio_readPin((pin_t)(KEYPAD_COLUMN_PORT+columns))){
						
					gpio_setPin((pin_t)(KEYPAD_ROW_PORT+rows));
					return g_keyMap[rows-KEYPAD_ROW_PORT_OFFSET][columns-KEYPAD_COLUMN_PORT_OFFSET];
					
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
 
 static void periodicScanISR(void){
	 
	 char key = Keypad_scan();
	 
	 if(key != '\0')
		g_keyHandlerCallback(key);
	 
 }