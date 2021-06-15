/*
 * rtos_keyentry.c
 *
 * Created: 15/09/2019 03:39:53 PM
 *  Author: Loay Ashraf
 */ 

#include "app/examples/include/examples.h"
#include "rtos/config/rtos_config.h"
#include "hal/ecu/lcd/api/lcd.h"
#include "hal/ecu/keypad/keypad.h"
#include "hal/mcu/peripheral/gpio.h"

/* EEPROM routines used only with the WinAVR compiler. */
#include <avr/eeprom.h>

/* An address in the EEPROM used to count resets.  This is used to check that
the demo application is not unexpectedly resetting. */
#define mainRESET_COUNT_ADDRESS            ( (void *) 0x50 )

/* task functions prototypes */

static void scanKeypad(void *pvParameters);
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
static char keyMap[4][4] = {{'1','2','3','A'},{'4','5','6','B'},{'7','8','9','C'},{'*','0','#','D'}};    /* Keypad key map array */

static volatile char key;                                            /* variable to hold value of pressed key */
static volatile bool_t scanKeypad_isRunning,updateLCD_isRunning;    /* variables used to check if LCD and Keypad tasks are running */

void ex_rtos_textEntry(void){    
    
    prvIncrementResetCount();
    
    /***************************************************/
    /* initialize error LED, LCD and keypad interfaces */
    /***************************************************/
    
    GPIO_SET_PIN_DIRECTION(PD2_M,IO_OUTPUT);
    LCD_init(TRUE,TRUE,TRUE,TRUE);
    Keypad_setKeyMap(keyMap);
    Keypad_init();
    
    xTaskCreate(scanKeypad,"scanKeypad",configMINIMAL_STACK_SIZE,NULL,3,NULL);        /* task to scan keypad every 10 ms */
    xTaskCreate(updateLCD,"updateLCD",configMINIMAL_STACK_SIZE,NULL,2,NULL);        /* task to update LCD every 150 ms */
    xTaskCreate(vErrorChecks,"Check",configMINIMAL_STACK_SIZE,NULL,1,NULL);            /* task to check if tasks are running every 2000 ms */

    vTaskStartScheduler();        /* start scheduler in preemptive mode */

}
/*-----------------------------------------------------------*/

static void scanKeypad(void *pvParameters){
    
    (void) pvParameters;
    
    for(;;){    /* loop forever */
        
        vTaskDelay(100);
        
        key = Keypad_scan();
        
        scanKeypad_isRunning = TRUE;
        
    }
    
}

static void updateLCD(void *pvParameters){
    
    (void) pvParameters;
    
    for(;;){    /* loop forever */
        
        vTaskDelay(200);
        
        if(key)
            LCD_putc(key);
        
        updateLCD_isRunning = TRUE;
        
    }
    
}

static void vErrorChecks(void *pvParameters){
    
    (void) pvParameters;
    
    for(;;){    /* loop forever */
        
        vTaskDelay(2000);

        prvCheckOtherTasksAreStillRunning();
    }
}
/*-----------------------------------------------------------*/

static void prvCheckOtherTasksAreStillRunning(void){
    
static portBASE_TYPE xErrorHasOccurred = pdFALSE;
    
    if(updateLCD_isRunning && scanKeypad_isRunning){
        
        xErrorHasOccurred = pdFALSE;    
        updateLCD_isRunning = FALSE;
        scanKeypad_isRunning = FALSE;
        
    }else{
        
        xErrorHasOccurred = pdTRUE;    
        
    }
    
    if(xErrorHasOccurred == pdFALSE){
        
        /* Toggle the LED if everything is okay so we know if an error occurs even if not
        using console IO. */
        GPIO_TOGGLE_PIN(PD2_M);
        
    }
}
/*-----------------------------------------------------------*/

static void prvIncrementResetCount(void){
    
unsigned char ucCount;

    eeprom_read_block(&ucCount,mainRESET_COUNT_ADDRESS,sizeof(ucCount));
    ucCount++;
    eeprom_write_byte(mainRESET_COUNT_ADDRESS,ucCount);
    
}
