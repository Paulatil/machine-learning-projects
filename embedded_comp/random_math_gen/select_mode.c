/*=============================================
  This Module implements the Mode selection 
	and game duration for the Game play 
==============================================*/
/* includes ------------------------------------*/
#include "main.h"
#include "select_mode.h"
#include "gpio_func.h"
#include "lcd_display.h"
#include "play_game.h"
#include "timer.h"
#include "Interrupt_Handler.h"



/* variables ------------------------------------*/
osThreadId_t 					sel_modeId;
uint32_t lock_flag, joystick_flags, current_mode;

/* Select_Mode function ----------------------------*/
void select_Mode(void *argument){
	uint32_t flag1, mode, init;

	// default display of Easy Mode option at startup
  current_mode = easy_mode;
	mode  = osThreadFlagsSet(lcdId, current_mode);
	Interrupts_id = osEventFlagsNew(NULL);
 
	while(1){		
		init = osThreadFlagsWait(init_flag, osFlagsWaitAny, 0);
			if(init != NULL){
				flag1 = 0;
				mode = 0;
				lock_flag = 0;
				current_mode = easy_mode;
				}
		
		flag1 = osThreadFlagsWait(joystick_flags, osFlagsWaitAny, 0);
		
		// perform a task when button is pressed
		if(flag1 == joystick_up){
			if(mode == easy_mode){
				current_mode = medium_mode;
				mode = osThreadFlagsSet(lcdId, current_mode);
			}
			else if (mode == medium_mode){
				current_mode = hard_mode;
				mode = osThreadFlagsSet(lcdId, current_mode);
				}
			else if (mode == hard_mode){
				current_mode = game_duration;
				mode = osThreadFlagsSet(lcdId, current_mode);
				}
			osThreadFlagsClear(flag1);
		}
		
		else if(flag1 == joystick_down){
			if(mode == medium_mode){
				current_mode = easy_mode;
				mode  = osThreadFlagsSet(lcdId, current_mode);
				}	
			else if(mode  == hard_mode){
				current_mode = medium_mode;
				mode = osThreadFlagsSet(lcdId, current_mode);
				}
			else if(mode  == game_duration){
				current_mode = hard_mode;
				mode = osThreadFlagsSet(lcdId, current_mode);
				}
		osThreadFlagsClear(flag1);	
			}
		
		else if(flag1 == joystick_sel){
			if(mode == easy_mode || mode == medium_mode || mode == hard_mode){
				lock_flag = mode_lock;                                           // locks in the game mode 
				//set_mode  = mode;                                                // holds the game mode selected by plyaer 
				osThreadFlagsSet(joystickId, lock_flag);
				osThreadFlagsClear(flag1);
				}
			else if(mode == game_duration){
			lock_flag = duration_set;                            // sets the game duration
			osThreadFlagsSet(joystickId, lock_flag);	
			osThreadFlagsClear(flag1);
				}
			}
	
	else if(lock_flag == duration_set){
		//gameId = osThreadNew(play_game, NULL, NULL);
		//timerId = osThreadNew(Init_Timer, NULL, NULL);
		//osThreadFlagsSet(gameId, set_mode);		
	  
    // suspend thread
			osThreadSuspend(sel_modeId);		
	 }
  
	} //end of while loop

}
	
