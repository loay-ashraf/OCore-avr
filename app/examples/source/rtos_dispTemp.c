/*
 * rtos_dispTemp.c
 *
 * Created: 16/09/2019 02:46:18 PM
 *  Author: Loay Ashraf
 */ 

#include "../include/examples.h"
#include <stdlib.h>
#include <string.h>

/* EEPROM routines used only with the WinAVR compiler. */
#include <avr/eeprom.h>

/* An address in the EEPROM used to count resets.  This is used to check that
the demo application is not unexpectedly resetting. */
#define mainRESET_COUNT_ADDRESS			( (void *) 0x50 )

/* task functions prototypes */

static void readTemp(void *pvParameters);
static void updateLCD(void *pvParameters);
static void vErrorChecks(void *pvParameters);

/*
 * Checks the unique counts of other tasks to ensure they are still operational.
 * Flashes an LED if everything is okay.
 */
static void prvCheckOtherTasksAreStillRunning(void);

/*
 * Called on boot to increment a count stored in the EEPROM.  This is used to
 * ensure the CPU does not reset unexpectedly.
 */
static void prvIncrementResetCount(void);

/*-----------------------------------------------------------*/

static volatile uint16_t temp;											/* variable to hold value of temperature */
static volatile bool_t readTemp_isRunning,updateLCD_isRunning;	/* variables used to check if LCD and Keypad tasks are running */

void ex_rtos_dispTemp(void){	
	
	prvIncrementResetCount();
	
	/***************************************************/
	/* initialize error LED, LCD and keypad interfaces */
	/***************************************************/
	
	gpio_setPinDirection(PD2_M,IO_OUTPUT);
	LCD_init(FALSE,FALSE);
	adc_config(AD_DIV16,AD_AVCC,FALSE);
	adc_enable();
	
	xTaskCreate(readTemp,"readTemp",configMINIMAL_STACK_SIZE,NULL,3,NULL);			/* task to read temperature every 10 ms */
	xTaskCreate(updateLCD,"updateLCD",configMINIMAL_STACK_SIZE,NULL,2,NULL);		/* task to update LCD every 500 ms */
	xTaskCreate(vErrorChecks,"Check",configMINIMAL_STACK_SIZE,NULL,1,NULL);			/* task to check if tasks are running every 2000 ms */

	vTaskStartScheduler();		/* start scheduler in preemptive mode */

}
/*-----------------------------------------------------------*/

static void readTemp(void *pvParameters){
	
	(void) pvParameters;
	
	for(;;){	/* loop forever */
		
		vTaskDelay(10);
		
		temp = adc_read(AD_CH0);
		
		readTemp_isRunning = TRUE;
		
	}
	
}

static void updateLCD(void *pvParameters){
	
	(void) pvParameters;
	
	for(;;){	/* loop forever */
		
		vTaskDelay(500);
		
		LCD_setCursorPosition(0,0);
		LCD_putf(temp*0.488);
		LCD_putc(' ');
		LCD_putc(223);
		LCD_putc('C');
		
		LCD_setCursorPosition(1,0);
		LCD_putf((temp*0.488*1.8)+32);
		LCD_putc(' ');
		LCD_putc(223);
		LCD_putc('F');
		
		updateLCD_isRunning = TRUE;
		
	}
	
}

static void vErrorChecks(void *pvParameters){
	
	(void) pvParameters;
	
	for(;;){	/* loop forever */
		
		vTaskDelay(2000);

		prvCheckOtherTasksAreStillRunning();
	}
}
/*-----------------------------------------------------------*/

static void prvCheckOtherTasksAreStillRunning(void){
	
static portBASE_TYPE xErrorHasOccurred = pdFALSE;
	
	if(updateLCD_isRunning && readTemp_isRunning){
		
		xErrorHasOccurred = pdFALSE;	
		updateLCD_isRunning = FALSE;
		readTemp_isRunning = FALSE;
		
	}else{
		
		xErrorHasOccurred = pdTRUE;	
		
	}
	
	if(xErrorHasOccurred == pdFALSE){
		
		/* Toggle the LED if everything is okay so we know if an error occurs even if not
		using console IO. */
		gpio_togglePin(PD2_M);
		
	}
}
/*-----------------------------------------------------------*/

static void prvIncrementResetCount(void){
	
unsigned char ucCount;

	eeprom_read_block(&ucCount,mainRESET_COUNT_ADDRESS,sizeof(ucCount));
	ucCount++;
	eeprom_write_byte(mainRESET_COUNT_ADDRESS,ucCount);
	
}