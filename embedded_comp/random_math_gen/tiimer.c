/*=========================================================
      Timer Module file keeps track of the current time left
			for the player of the game. It alarts the 
			player when the gmae time count to 0
===========================================================*/

/* include ---------------------------------------------------*/
#include "main.h"  
#include "audio_func.h"
#include "timer.h"
#include "lcd_display.h"
#include "play_game.h"

/*variables --------------------------------------------------*/
static uint32_t                     exec;       // argument for the timer call back function
osThreadId_t                        timerId;
osTimerId_t													time_id;
osThreadId_t												timeOutId;
osThreadId_t                        quarter_toneId;

// One-Shoot Timer Function
static void Timer_Callback (void const *arg) {
	
	if(set_time[5] == 48 && set_time[4] == 48 && set_time[3] == 48 && 
		set_time[2] == 48 && set_time[1] == 48 && set_time[0] == 48 ){
			timeOutId = osThreadNew(timeOut_tone, NULL, NULL);
			osThreadFlagsSet(gameId, timeout_flag);
			if(timeOutId != NULL){
			   osThreadFlagsSet(timeOutId, game_over_flag);
				 osThreadTerminate(quarter_toneId);                // stop the qaurter beep
			   osTimerStop(time_id);                             // stop the timer
				osTimerDelete(time_id);                             // delete timer 
			}
	  }			
	     // decrement timer 		
				set_time[5]--;
				if(set_time[5] == 47){
					set_time[4]--;
					set_time[5] = 57;
					if(set_time[4] == 47){
						set_time[3]--;
						set_time[4] = 57;
						if(set_time[3] == 47){
							set_time[2]--;
							set_time[3] = 57;
							if(set_time[2] == 47){
								set_time[1]--;
								set_time[2] = 53;
								if(set_time[1] == 47){
									set_time[0]--;
									set_time[1] = 57;
									if(set_time[0] == 47){
									set_time[0] = 53;
									}
								}
							}
						}
					}
				}
				
	    // play quarter tone
     if(set_time[3] == 48 || set_time[2] == 48 ){
			 osThreadFlagsSet(quarter_toneId, quarter_beep_flag);  
		 }				
}
 

 

 

// Create and Start timer
void Init_Timer (void *arg) {
  osStatus_t status1;                              // return status      
  uint32_t  flag1;                                  // return set flag
 // Create  timer
  exec = 1U;
  time_id = osTimerNew((osTimerFunc_t)&Timer_Callback, osTimerPeriodic, &exec, NULL);
  if (time_id != NULL) {  // One-shot timer created
    // start timer with delay 100ms
		flag1 = osThreadFlagsWait(start_timer_flag, osFlagsWaitAny, osWaitForever);
    if(flag1 != NULL){
		status1 = osTimerStart(time_id, 100U); 
    if (status1 != osOK) {
       quarter_toneId = osThreadNew(quarter_tone, NULL, NULL);
 		}
     } 		
   }
 }
 

