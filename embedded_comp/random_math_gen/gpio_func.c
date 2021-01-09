/*============================================================
This Module implements GPIO related functions such as: 
 the joystick buttons press
===============================================================*/
/* includes --------------------------------------*/
 #include "main.h"
 #include "gpio_func.h"
 #include "Interrupt_Handler.h"
 #include "select_mode.h"
 #include "lcd_display.h"
 #include "play_game.h"
 #include "Threads.h"
 
 /* variables --------------------------------------- */
osThreadId_t 							joystickId;

 
 
 /* JOYSITCK BUTTONS PRESS function ----------------------------*/
 void joystick_buttons(void *argument){
	uint32_t button, set_lock, init;        // button recieves event flags ; lock receives mode set flag 
	while(1){
		init = osThreadFlagsWait(init_flag, osFlagsWaitAny, 0);
		if(init != NULL){
			button = 0;			
		}
		     // wait for interrupt signal from button press
		button = osEventFlagsWait(Interrupts_id, interrupt_flags, osFlagsWaitAny, 0);
		
		      // check if game_mode has been selected
		set_lock = osThreadFlagsWait(lock_flag, osFlagsWaitAny, 0); 
		  
		// up button pressed?
		if(button == joystick_up){
			if(set_lock == mode_lock){                    // if game_mode has been selected then set time
				//  set game duration
				set_time[5]++;
				if(set_time[5] == 57){
					set_time[4]++;
					set_time[5] = 48;
					if(set_time[4] == 57){
						set_time[3]++;
						set_time[4] = 48;
						if(set_time[3] == 57){
							set_time[2]++;
							set_time[3] = 48;
							if(set_time[2] == 53){
								set_time[1]++;
								set_time[2] = 48;
								if(set_time[1] == 57){
									set_time[0]++;
									set_time[1] = 48;
									if(set_time[0] == 53){
									set_time[0] = 48;
									}
								}
							}
						}
					}
				}
				display_time();
			}
			   // action question button press
			else if(set_lock == duration_set){
				evnt_flags = joystickUp_evnt;
				osEventFlagsSet(evnt_id, evnt_flags);
			  }
			
			      // naviagte select_mode if mode not set
			else{ joystick_flags = joystick_up;
				osThreadFlagsSet(sel_modeId, joystick_flags);}
			 
			osThreadFlagsClear(button);                     // clear flag
		}
		     
		    // down button pressed?
		else if(button == joystick_down){
			if(set_lock == mode_lock){                 // if game_mode has been selected then set time 
				// set game duration
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
				display_time();
			}
			
			// action question button press
			else if(set_lock == duration_set){
				evnt_flags = joystickDwn_evnt;
				osEventFlagsSet(evnt_id, evnt_flags);
			  }
			   // navigate select_mode if mode not set
			else{ joystick_flags = joystick_down;
				osThreadFlagsSet(sel_modeId, joystick_flags);}
			
			osThreadFlagsClear(button);    //clear flag
				}	
	      
				   // left button pressed?
		else if(button == joystick_left){
			
			// action question button press
			if(set_lock == duration_set){
				evnt_flags = joystickLeft_evnt;
				osEventFlagsSet(evnt_id, evnt_flags);
					}
			else if(set_lock == mode_lock){
				// increment the min values by 1min
				set_time[1]++;
				if(set_time[1] == 57){
					set_time[0]++;
					set_time[1] = 48;
					if(set_time[0] == 53){
						set_time[0] = 48;
					 }
				 }
				display_time();
			  }
		 	}
		       
		      //right button pressed?
		else if(button == joystick_right){
			
			// action question button press
		  if(set_lock == duration_set){
			 evnt_flags = joystickRight_evnt;
			 osEventFlagsSet(evnt_id, evnt_flags);
			  }
		  else if(set_lock == mode_lock){
				// increment the sec values by 10secs
				set_time[2]++;
				if(set_time[2] == 53){
					set_time[1]++;
					set_time[2] = 48;
					if(set_time[1] == 57){
						set_time[0]++;
						set_time[1] = 48;
						if(set_time[0] == 53){
							set_time[0] = 48;
						 }
					}
				}
			display_time();
		}
	    
		      // centre button pressed?   	
		else if(joystick_flags == joystick_sel){
			osThreadFlagsSet(sel_modeId, joystick_flags);
			osThreadFlagsClear(button);  // clear flag
     }	
		
			 } // end of while loop 
	   }
 
	 }
 
 