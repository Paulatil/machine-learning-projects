/*=================================================================
*                           GPIO_INPUT source file. 
*		this file defines functions to implement joystick buttons press.
=================================================================== */
/* includes --------------------------------------------------*/
#include "cmsis_os2.h"
#include "main.h"
#include "Interrupt_Handler.h"
#include "gpio_input.h"
#include "threads.h"
#include "tones.h"
#include "Perif_RTOS.h"

/* varibales ----------------------------------------------- */
uint8_t 	               brew_time[4] = {48,51,48,48};           // default = 03:00
uint8_t 	               temp_value[4] = {48,55,53,67};       // default = 75 C
uint8_t 	               press_value[5] = {48,57,98,97,114};      // default  = 9 bar
static uint32_t 	       curr_display;
uint32_t		             set_temp = 0;
uint32_t		             set_press = 0;
uint32_t   							 init = 0x0;
uint32_t   							 status = 0x0;
uint32_t 								 state = 0;

/* functions -------------------------------------------------- */
void display_brew(void);
void display_temp(void);
void display_press(void);
void joystick_buttons(void *arg);
uint32_t convert_ascii2int(uint8_t* value, int length);
void Init_upon_brew_complete(void);

/*============== FUNCTIONS DEFINITION ===============================*/

__NO_RETURN void joystick_buttons(void *arg){
	(void) arg;
	uint32_t   flag = 0x0;       	
	
	//on start up display the MENU on LCD
	BSP_LCD_GLASS_Clear();
	osDelay(200);
	BSP_LCD_GLASS_DisplayString((uint8_t*)" MENU ");
	
	while(1){
		// initialize to default values when any key is pressed		
		if(init == INIT){
			//set pressure to default value
			brew_time[0] = 48;
			brew_time[1] = 51;
			brew_time[2] = 48;
			brew_time[3] = 48;
			
			//set temperature to default value
			temp_value[0] = 48;
			temp_value[1] = 55;
			temp_value[2] = 53;
			temp_value[3] = 67;
			
		//set pressure to default value 
			press_value[0] = 48;
			press_value[1] = 57;
			press_value[2] = 98;
			press_value[3] = 97;
			press_value[4] = 114;
			
      state = 0;         
			status_flag = 0;    //ensure the main thread wait for its flags to be set again.
			status = 0;
			
			BSP_LED_Off(LED4);
			// set default screen to MENU
			BSP_LCD_GLASS_Clear();
			osDelay(200);
			BSP_LCD_GLASS_DisplayString((uint8_t*)" MENU ");
			//play tone
      play_beep();
			init = NULL;    //ensure this statemet block does not run twice.
		  osDelay(200);
		}
		
		
		flag  = joy_flag;      // get joystick button pressed
		
		if((flag & JOYSTICK_UP) == JOYSTICK_UP){
			if((status & START) == START){
				init = INIT;
				status_flag = INIT;
				curr_display = 0;
			 }
			
			if(curr_display == BREW){
				brew_time[3]++;
				if(brew_time[3] == 58){
					brew_time[2]++;
					brew_time[3] = 48;
					if(brew_time[2] == 54){
						brew_time[1]++;
						brew_time[2] = 48;
						if(brew_time[1] == 58){
							brew_time[0]++;
							brew_time[1] = 48;
							if(brew_time[0] == 54){
								brew_time[0] = 48;
									}
								}
							}
						}
				display_brew();			
			}
			
			else if(curr_display == TEMP){
				temp_value[2]++;
				if(temp_value[2] == 58){
					temp_value[1]++;
					temp_value[2] = 48;
					if(temp_value[1] == 58){
						temp_value[0]++;
						temp_value[1] = 48;
						if(temp_value[0] == 50){
							temp_value[0] = 48;
								}
							}
						}
				display_temp();
		   }
			
			else if(curr_display == PRESS){
				press_value[1]++;
				if(press_value[1] == 58){
					press_value[0]++;
					press_value[1] = 48;
					if(press_value[0] == 58){
						press_value[0] = 48;
								}
							}
				display_press();			
					} 
		flag = NULL;	
		}
		
		else if ((flag & JOYSTICK_DWN) == JOYSTICK_DWN){
			if((status & START) == START){
				init = INIT;
				status_flag = INIT;
				curr_display = 0;
			 }
			
			if(curr_display == BREW){
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
				display_brew();	
			}
			
			else if(curr_display == TEMP){
				temp_value[2]--;
				if(temp_value[2] == 47){
					temp_value[1]--;
					temp_value[2] = 57;
					if(temp_value[1] == 47){
						temp_value[0]--;
						temp_value[1] = 57;
						if(temp_value[0] == 47){
							temp_value[0] = 48;
											}
										}	
									}
				display_temp();
				}

			else if(curr_display == PRESS){
				press_value[1]--;
				if(press_value[1] == 47){
					press_value[0]--;
					press_value[1] = 57;
					if(press_value[0] == 47){
						press_value[0] = 57;
										}
									}
				display_press();
				}
			flag = NULL;
			}
		
			
		else if ((flag & JOYSTICK_LEFT) == JOYSTICK_LEFT){
			if((status & START) == START){
				init = INIT;
				status_flag = INIT;
				curr_display = 0;
			 }
			
			if(curr_display == BREW){
				brew_time[1]++;
				if(brew_time[1] == 58){
					brew_time[0]++;
					brew_time[1] = 48;
					if(brew_time[0] == 54){
						brew_time[0] = 48;
							}
						}
				display_brew();	
				}
			
			else if(curr_display == TEMP){
				temp_value[1]++;
				if(temp_value[1] == 58){
					temp_value[0]++;
					temp_value[1] = 48;
					if(temp_value[0] == 50){
						temp_value[0] = 48;
								}
							}
				display_temp();
				}
			
			else if(curr_display == PRESS){
				press_value[0]++;
				if(press_value[0] == 58){
					press_value[0] = 48;
						}
				display_press();
				}			
		}
		
		else if ((flag & JOYSTICK_RIGHT) == JOYSTICK_RIGHT){
			if((status & START) == START){
				init = INIT;
				status_flag = INIT;
				curr_display = 0;
				}
			
			if(curr_display == BREW){
				brew_time[2]++;
				if(brew_time[2] == 54){
					brew_time[1]++;
					brew_time[2] = 48;
					if(brew_time[1] == 58){
						brew_time[0]++;
						brew_time[1] = 48;
						if(brew_time[0] == 54){
							brew_time[0] = 48;
									}
								}
							}
				display_brew();			
			}	
		flag = NULL;		
		}

		
		else if ((flag & JOYSTICK_SEL) == JOYSTICK_SEL){
			if(state == 0){
				curr_display = BREW;
				BSP_LCD_GLASS_Clear();
				display_brew();	
				state ++;
				}
			else if(state == 1){
				curr_display = TEMP;
				BSP_LCD_GLASS_Clear();
				display_temp();
				state ++;	
				}
			else if(state == 2){
				curr_display = PRESS;
				BSP_LCD_GLASS_Clear();
				display_press();
				state ++;
				}
			
			else if(state == 3){
				// get the set temp and presure values in uint32_t  format 
				set_press = convert_ascii2int(press_value, 2);
			  set_temp = convert_ascii2int(temp_value, 3);
 				osDelay(200); 
				state++;
				status = START;
				status_flag = START;
				osThreadYield();
				}
			
			else if (state == 4){
					init = INIT;
				  status_flag  = INIT;
				}
				
		flag = NULL;		
		}
		
	joy_flag = NULL;
  osDelay(200);		
	}    // end of while loop 

}


void display_brew(void){
	BSP_LCD_GLASS_DisplayChar(&brew_time[0], POINT_OFF,DOUBLEPOINT_OFF,LCD_DIGIT_POSITION_1);
	BSP_LCD_GLASS_DisplayChar(&brew_time[1], POINT_OFF,DOUBLEPOINT_ON,LCD_DIGIT_POSITION_2);
	BSP_LCD_GLASS_DisplayChar(&brew_time[2], POINT_OFF,DOUBLEPOINT_OFF,LCD_DIGIT_POSITION_3);
	BSP_LCD_GLASS_DisplayChar(&brew_time[3], POINT_OFF,DOUBLEPOINT_OFF,LCD_DIGIT_POSITION_4);
}

void display_temp(void){
	BSP_LCD_GLASS_DisplayChar(&temp_value[0], POINT_OFF,DOUBLEPOINT_OFF,LCD_DIGIT_POSITION_1);
	BSP_LCD_GLASS_DisplayChar(&temp_value[1], POINT_OFF,DOUBLEPOINT_OFF,LCD_DIGIT_POSITION_2);
	BSP_LCD_GLASS_DisplayChar(&temp_value[2], POINT_OFF,DOUBLEPOINT_OFF,LCD_DIGIT_POSITION_3);
	BSP_LCD_GLASS_DisplayChar(&temp_value[3], POINT_OFF,DOUBLEPOINT_OFF,LCD_DIGIT_POSITION_4);
}

void display_press(void){
	BSP_LCD_GLASS_DisplayChar(&press_value[0], POINT_OFF,DOUBLEPOINT_OFF,LCD_DIGIT_POSITION_1);
	BSP_LCD_GLASS_DisplayChar(&press_value[1], POINT_OFF,DOUBLEPOINT_OFF,LCD_DIGIT_POSITION_2);
	BSP_LCD_GLASS_DisplayChar(&press_value[2], POINT_OFF,DOUBLEPOINT_OFF,LCD_DIGIT_POSITION_3);
	BSP_LCD_GLASS_DisplayChar(&press_value[3], POINT_OFF,DOUBLEPOINT_OFF,LCD_DIGIT_POSITION_4);
	BSP_LCD_GLASS_DisplayChar(&press_value[4], POINT_OFF,DOUBLEPOINT_OFF,LCD_DIGIT_POSITION_5);
}


uint32_t convert_ascii2int(uint8_t * value, int length){
	//create an array dynamically
	uint8_t temp;
	uint32_t res;
	uint32_t a2i[length];
	
	for(int i = 0; i < length; i++){
		temp = value[i];
			if(temp == 48){
				a2i[i] = 0;
			}
			else if(temp == 49){
				a2i[i] = 1;
			}
			else if(temp == 50){
				a2i[i] = 2;
			}
			else if(temp == 51){
				a2i[i] = 3;
			}
			else if(temp == 52){
				a2i[i] = 4;
			}
			else if(temp == 53){
				a2i[i] = 5;
			}
			else if(temp == 54){
				a2i[i] = 6;
			}
			else if(temp == 55){
				a2i[i] = 7;
			}
			else if(temp == 56){
				a2i[i] = 8;
			}
			else if(temp == 57){
				a2i[i] = 9;
			}	
		}
	     /* convert char to int */
	int j = 0;	
	while(j < length){
		res = (res * 10) + a2i[j];
		j++;
	 }	
	
	return res;
}


void Init_upon_brew_complete(void){
	    brew_time[0] = 48;
			brew_time[1] = 51;
			brew_time[2] = 48;
			brew_time[3] = 48;
			
			//set temperature to default value
			temp_value[0] = 48;
			temp_value[1] = 55;
			temp_value[2] = 53;
			temp_value[3] = 67;
			
		//set pressure to default value 
			press_value[0] = 48;
			press_value[1] = 57;
			press_value[2] = 98;
			press_value[3] = 97;
			press_value[4] = 114;
			
      state = 0;
			status_flag = 0;    //ensure the main thread wait for its flags to be set again.
			status = 0;
			
			// set default screen to MENU
			BSP_LCD_GLASS_Clear();
			osDelay(200);
			BSP_LCD_GLASS_DisplayString((uint8_t*)" MENU ");
			time_up_beep();      
			init = NULL;    //ensure this statemet block does not run twice.
		  osDelay(200);
}
