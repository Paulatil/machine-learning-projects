/*===========================================================================
 *      This top-level module creates threads for different functions implementing
 *			different parts of the interactive game program. 
 *=========================================================================== */
#include "main.h"
#include "Threads.h"
#include "select_mode.h"
#include "gpio_func.h"
#include "lcd_display.h"
 
/* variable ------------------------------------------------ */


/*------------------------------------------------------------
*                 Threads creation
-------------------------------------------------------------- */

void Init_Thread (void* arg) {
 
  // create the select mode thread
	sel_modeid = osThreadNew(select_Mode, NULL, NULL); // select mode thread creates the game_play & timer threads
	if(!sel_modeid){
	  BSP_LED_On(LED4); 
	  }	
	joystickId = osThreadNew(joystick_buttons, NULL, NULL); //  thread handles jouysitck buttons press
	if(!joysitckId){
	  BSP_LED_On(LED4); 
	  }		
	lcdId = osThreadNew(Lcd_display, NULL, NULL);        // thread handles display on LCD 
	if(!lcdId){
	  BSP_LED_On(LED4); 
	  }	
	osDelay(osWaitForever);		// suspend init_thread
	  
     while(1){
         }
	}
 
