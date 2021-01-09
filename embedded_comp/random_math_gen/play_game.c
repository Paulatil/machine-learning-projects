/*==============================================================
  This play_game file implements the tasks of generating 
	two random numbers and a random operator to form a question
  the user can answer. It compares the player's answer to its result
  Also, asks the player a random gesture question such as clap,
   press button, or shake tje board.	
================================================================ */
/* includes ---------------------------------------------- */
#include "main.h"
#include "play_game.h"
#include "timer.h"
#include "select_mode.h"
#include "Interrupt_Handler.h"
#include "audio_func.h"
#include "timer.h"
#include "lcd_display.h"
#include "gpio_func.h"


/* variables -------------------------------------------- */
char string[]= "GESTURES";                        // strong array for gesture questions 
 static UART_HandleTypeDef 	huart2;             
static uint32_t start_timer = 0;
osThreadId_t			actionId, gameId;
osThreadId_t      correctanswer_toneId;
osThreadId_t      Incorrectanswer_toneId;
uint32_t         set_mode;
uint32_t				 count;
uint8_t 				 trans_buff[3];
uint8_t 				 recv_buff[2];





void play_game(void *argument){
	uint32_t game_mode, gesture, timeout, evnt_flag;
	uint8_t A = 0;
  uint8_t	B = 0;
 	uint8_t user_guess = 0;
  uint8_t	expected = 0;
	uint8_t action = 0;
  char	op = 0;
	

	
	  // check the game mode
	game_mode = osThreadFlagsWait(set_mode, osFlagsWaitAny, osWaitForever);
	
	    /* create action checker thread */
	actionId = osThreadNew(check_action, NULL, NULL);
	
	while(game_mode != NULL ){
		for(int iter = 0; iter < 4; iter++){   // run this loop 3 times for 3 math questions
			count = 0;                            // checks if interrupt action takes place
			
			// generate random numbers based on the game mode
			switch(game_mode){
				case (easy_mode):                            // generate 1-digit values	
					A  = (uint8_t)(rand()% (9 - 1 + 1)) + 1;
					B =  (uint8_t)(rand()% (9 - 1 + 1)) + 1;		
					break;
		
				case(medium_mode):                         // generate 2-digits values
					A  = (uint8_t)(rand()% (99 - 10 + 1)) + 10;
					B =  (uint8_t)(rand()% (99 - 10 + 1)) + 10;
					break;
		
				case(hard_mode):                          // generate 3-digits values 
					A  = (uint8_t)(rand()% (999 - 100 + 1)) + 100;
					B =  (uint8_t)(rand()% (999 - 100 + 1)) + 100;
					break;
		
				default:
			
					break;
				}
	
			op = "+-*"[rand() %3];                   // generate math operator
	  
			switch(op){
				case ('+'):
					expected = (A + B);             // compute the expected result       
					trans_buff[0] = A;
					trans_buff[1] = op;
					trans_buff[2] = B;
				  break;
				case('-'):
					if(B > A){
					expected = (B - A);
					trans_buff[0] = B;
					trans_buff[1] = op;
					trans_buff[2] = A;
					}
					expected = A - B;
					trans_buff[0] = A;
					trans_buff[1] = op;
					trans_buff[2] = B;
					break;
				case('*'):
					expected = (A * B);
				  trans_buff[0] = A;
					trans_buff[1] = op;
					trans_buff[2] = B;
					break;
				default:
					break;		
				}
		  			
			// transmit A op B to terminal
		  while(HAL_UART_Transmit(&huart2, trans_buff, sizeof(trans_buff), HAL_MAX_DELAY)){		
      if((trans_buff[0] || trans_buff[1] || trans_buff[2]) == NULL){
				HAL_UART_Abort(&huart2); }
			}
		  start_timer = 1;                  // the timer starts here after first transmission 
      // recieve repsponse from user 
		 while(HAL_UART_Receive(&huart2, recv_buff, sizeof(recv_buff), UART_timeOut)){
		  if(recv_buff[0] == NULL){ HAL_UART_Abort(&huart2);}
		}
     user_guess = recv_buff[0]; 
		
			// compare results
     if(user_guess == expected){
		  // play right answer tone
			 correctanswer_toneId = osThreadNew(correct_answer_tone, NULL, NULL);
			 osThreadFlagsSet(correctanswer_toneId, correct_answer_flag);
			}	
     else{// play wrong answer tone 
			 Incorrectanswer_toneId = osThreadNew(Incorrect_answer_tone, NULL, NULL);
			 osThreadFlagsSet(Incorrectanswer_toneId, incorrect_answer_flag);}
       			      				
		}	// end of for loop
		
		
		/* Fourth question will be a gesture based one */
		gesture = rand()%6;
		
		switch(gesture){
			 case (0):  
				string[0] = 'C';
			  string[1] = 'L';
			  string[2] = 'A';
			  string[3] = 'p';
			  string[4] = '\0';
			  action = 0;
				break;
			
			case (1): 
        string[0] = 'S';
			  string[1] = 'H';
			  string[2] = 'A';
			  string[3] = 'K';
			  string[4] = 'E';
				string[5] = '\0';
			  action = 1;
				break;
	
			case(2):				
				string[0] = 'U';
			  string[1] = 'P';
			  string[2] = '\0';
			  action = 2;
        break;
			
			case(3):
				string[0] = 'D';
			  string[1] = 'O';
			  string[2] = 'W';
			  string[3] = 'N';
			  string[4] = '\0';
			  action = 3;
				break;
			
			case(4):
				string[0] = 'L';
			  string[1] = 'E';
			  string[2] = 'F';
			  string[3] = 'T';
			  string[4] = '\0';
			  action = 4;
        break;
			
			case(5):
				string[0] = 'R';
			  string[1] = 'I';
			  string[2] = 'G';
			  string[3] = 'H';
			  string[4] = 'T';
			  string[5] = '\0';
			  action = 5;
        break;
				} 
		 
		 // transmit gesture question to terminal
		while(HAL_UART_Transmit(&huart2, (uint8_t*)string, sizeof(strlen(string)), UART_timeOut)){
		 if((string[0] || string[1] || string[2] || string[3] || string[4] || string[5]) == NULL){
			HAL_UART_Abort(&huart2);}
		    }
		
		count  = 1;                                       // activates interrupt action 		
		// compare result of gesture 
		evnt_flag = osEventFlagsWait(evnt_id, evnt_flags, osFlagsWaitAny, osWaitForever);
		// play appropriate sound
		while(evnt_flag != NULL){		
		 if(action == 0){                          			// CLAP!
			 if(evnt_flag == mic_evnt){
				 correctanswer_toneId = osThreadNew(correct_answer_tone, NULL, NULL);
			   osThreadFlagsSet(correctanswer_toneId, correct_answer_flag);
				 evnt_flag = NULL;
			 }
			 else{
				 Incorrectanswer_toneId = osThreadNew(Incorrect_answer_tone, NULL, NULL);
			   osThreadFlagsSet(Incorrectanswer_toneId, incorrect_answer_flag);
				 evnt_flag = NULL;
			 }
			}
		 
		if(action == 1){                          			// SHAKE!
			 if(evnt_flag == gyro_evnt){
				 correctanswer_toneId = osThreadNew(correct_answer_tone, NULL, NULL);
			   osThreadFlagsSet(correctanswer_toneId, correct_answer_flag);
				 evnt_flag = NULL;
			 }
			 else{
				 Incorrectanswer_toneId = osThreadNew(Incorrect_answer_tone, NULL, NULL);
			   osThreadFlagsSet(Incorrectanswer_toneId, incorrect_answer_flag);
				 evnt_flag = NULL;
			 }
			}
		
		if(action == 2){                          			// UP!
			 if(evnt_flag == joystickUp_evnt){
				 correctanswer_toneId = osThreadNew(correct_answer_tone, NULL, NULL);
			   osThreadFlagsSet(correctanswer_toneId, correct_answer_flag);
				 evnt_flag = NULL;
			 }
			 else{
				 Incorrectanswer_toneId = osThreadNew(Incorrect_answer_tone, NULL, NULL);
			   osThreadFlagsSet(Incorrectanswer_toneId, incorrect_answer_flag);
				 evnt_flag = NULL;
			 }
			}
		
		if(action == 3){                          			// DOWN!
			 if(evnt_flag == joystickDwn_evnt){
				 correctanswer_toneId = osThreadNew(correct_answer_tone, NULL, NULL);
			   osThreadFlagsSet(correctanswer_toneId, correct_answer_flag);
				 evnt_flag = NULL;
			 }
			 else{
				 Incorrectanswer_toneId = osThreadNew(Incorrect_answer_tone, NULL, NULL);
			   osThreadFlagsSet(Incorrectanswer_toneId, incorrect_answer_flag);
				 evnt_flag = NULL;
			 }
			}	
		
		if(action == 4){                          			// LEFT!
			 if(evnt_flag == joystickLeft_evnt){
				 correctanswer_toneId = osThreadNew(correct_answer_tone, NULL, NULL);
			   osThreadFlagsSet(correctanswer_toneId, correct_answer_flag);
				 evnt_flag = NULL;
			 }
			 else{
				 Incorrectanswer_toneId = osThreadNew(Incorrect_answer_tone, NULL, NULL);
			   osThreadFlagsSet(Incorrectanswer_toneId, incorrect_answer_flag);
				 evnt_flag = NULL;
			 }
			}

		if(action == 5){                          			// RIGHT!
			 if(evnt_flag == joystickRight_evnt){
				 correctanswer_toneId = osThreadNew(correct_answer_tone, NULL, NULL);
			   osThreadFlagsSet(correctanswer_toneId, correct_answer_flag);
				 evnt_flag = NULL;
			 }
			 else{
				 Incorrectanswer_toneId = osThreadNew(Incorrect_answer_tone, NULL, NULL);
			   osThreadFlagsSet(Incorrectanswer_toneId, incorrect_answer_flag);
				 evnt_flag = NULL;
			 }
			}				
		} // end of gesture response check while loop 	  
    
    
    // end game if time is out 
		timeout = osThreadFlagsWait(timeout_flag, osFlagsWaitAny, 0);
		if(timeout != NULL){
			osThreadResume(sel_modeId);
			osThreadFlagsSet(sel_modeId, init_flag);
			osThreadFlagsSet(joystickId, init_flag);
			osThreadFlagsSet(lcdId, init_flag);
			osThreadTerminate(timerId);                                  // terminate timer thread
			osThreadTerminate(gameId);                                    // self terminate
		}
			} // end of while loop
	} 



	
	/* this function checks the player's use of the gyro, joysticks and microphone */
	void check_action(void *argument){
		uint32_t evnt_flag;
		if(start_timer == 1){             // start timer 
			osThreadFlagsSet(timerId, start_timer_flag);
		}
		
		while(1){
			evnt_flag = osEventFlagsWait(evnt_id,evnt_flags, osFlagsWaitAny, 0);
			
			while(evnt_flag != NULL){
				 // check if action was asked. 
				if(count  == 0){ 
					// play the incorrect answer tone.
					Incorrectanswer_toneId = osThreadNew(Incorrect_answer_tone, NULL, NULL);
					osThreadFlagsSet(Incorrectanswer_toneId, incorrect_answer_flag);}
				evnt_flag = NULL;	
		  }
  }		
}
	
	
/************************************ END OF FILE *************************************************************/

			

