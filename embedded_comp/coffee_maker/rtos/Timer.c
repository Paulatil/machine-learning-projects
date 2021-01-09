/*===========================================================================
*					TIMER SOURCE FILE
  Implements the timer callback function to count down the brew time  
============================================================================== */

#include "cmsis_os2.h"                          // CMSIS RTOS header file
 #include "main.h"
 #include "threads.h"
 #include "gpio_input.h"
 #include "Timer.h"
 
/*----------------------------------------------------------------------------
 *      Timer: timer functions
 *---------------------------------------------------------------------------*/
 

 
/*----- Periodic Timer Example -----*/
osTimerId_t 								brew_time_id;                            // timer id

 

// Periodic Timer Function
static void Timer_Callback (void const *arg) {
  if(start_brew_timer == 1){
			if(brew_time[0] == 48 && brew_time[1] == 48 && brew_time[2] == 48 && brew_time[3] == 48){
				 brew_time_complete = 1;
				 start_brew_timer = 0;
				
			}
			// count down brew time.
			  brew_time[3]--;
				if(brew_time[3] == 47){
					brew_time[2]--;
					brew_time[3] = 57;
					if(brew_time[2] == 47){
						brew_time[1]--;
						brew_time[2] = 53;
						if(brew_time[1] == 47){
							brew_time[0]--;
							brew_time[1] = 57;
							if(brew_time[0] == 47){
								brew_time[0] = 53;
											}
										}
									}
							}	
			    }
	}
 


// Example: Create and Start timers
__NO_RETURN void Init_Timer (void *arg) {
  (void) arg;
	osStatus_t	 status;                            // function return status
 
  
 
  // Create periodic timer
  
  brew_time_id = osTimerNew((osTimerFunc_t)&Timer_Callback, osTimerPeriodic, NULL, NULL);
  if (brew_time_id != NULL) {  // Periodic timer created
    // start timer with periodic 100ms interval
    status = osTimerStart(brew_time_id, 100U);            
    if (status != osOK) {
			BSP_LED_Toggle(LED5);
    }
  }
  
}
