/*=========================================================
*						THREADS source file
* This source file contains functions that implement
* temp bring up, brewing, user_input poll, and lock_mode
==========================================================*/

/* includes ----------------------------------------- */
#include "cmsis_os2.h"
#include "main.h"
#include "Perif_RTOS.h"
#include "gpio_input.h"
#include "tones.h"
#include "threads.h"
#include "Hardware_Init.h"
#include "Timer.h"

/* variables ----------------------------------------- */
static uint32_t          current_temp = 0;
static uint32_t          current_press = 0;        
static uint32_t	         bs = 0;	             //brew status
static char              string1[6]="";             
static char              string2[6]="";
uint32_t                 start_brew_timer = 0;
uint32_t   						   brew_time_complete = 0;
uint32_t 								 status_flag = 0x0;

/* prototype functions ----------------------------------- */
void temp_bring_up(void);
void brew_stage(void);
void Master_thread(void *arg);      // handles temp bring up and brew states 
void lock_thread(void *arg);        // waits for 12345 from UART               
void Gyro_Inputs(void *arg);    // handles the gyro inputs  


/*----------------------------------------------------
* 						FUNCTION DEFINITIONS
*---------------------------------------------------- */

void temp_bring_up(void){
	
// close the valve 
	valveClose();

	current_temp = readTemperature();      // get the temp and pressure readings
	current_press = readPressure();
	
 // boil the water to set temperature 
	if(current_temp >= set_temp){
		boilerTurnOff();
		bs = 1;	  // enter the brew state.
	}		
	
	else{
		boilerTurnOn();
		
	// display the current temp and press readings on LCD
	BSP_LCD_GLASS_Clear();
	sprintf(string1, "%dC", current_temp);
	BSP_LCD_GLASS_DisplayString((uint8_t*) string1);
	osDelay(1000);
	
	BSP_LCD_GLASS_Clear();
	sprintf(string2, "%dBAR", current_press);
	BSP_LCD_GLASS_DisplayString((uint8_t*) string2);
	osDelay(1000);	
	  }
	
 }

 
 void brew_stage(void){
  
	if(bs == 1){ 
	 current_temp = readTemperature();
	 current_press = readPressure();
	 
		if(current_press <= set_press){ 
		  pumpTurnOn(); 
		  start_brew_timer = 1;  //start the timer count-down
		  BSP_LED_On(LED4);
			brew_start_tone();  // play brewing start and process tone
		}
		 
		else if(current_press > set_press){
			 pumpTurnOff();
			 valveOpen();
		}
		
	 // display current temp and press readings 
	BSP_LCD_GLASS_Clear();
	sprintf(string1, "%dC", current_temp);
	BSP_LCD_GLASS_DisplayString((uint8_t*) string1);
	osDelay(1000);
	
	BSP_LCD_GLASS_Clear();
	sprintf(string2, "%dBAR", current_press);
	BSP_LCD_GLASS_DisplayString((uint8_t*) string2);
	osDelay(1000);
	 } 
}
 

__NO_RETURN void Master_thread(void *arg){
	(void) arg;
	 uint32_t flag;
	
	while(1){
		
		flag = status_flag;    // status flag set by joystick_buttons thread
	
		 
	  if((flag & START) == START){
			//start the temperature bring up phase 
			temp_bring_up();
			
			// start the brew stage; 
			//this only gets implemented when the bs value is set in the temp bring up phase 
			brew_stage();
			
			if(brew_time_complete == 1){
				pumpTurnOff();
				boilerTurnOff();
			  valveVent();
        BSP_LED_Off(LED4);				
				Init_upon_brew_complete(); 
				brew_time_complete = 0;
				osThreadYield();
			   }
			
			
			flag = NULL;	 
			}
		
		else if ((flag & INIT) == INIT){
			  pumpTurnOff();
				boilerTurnOff();
			  valveVent();
				BSP_LED_Off(LED4);
			  BSP_LCD_GLASS_Clear();
			  flag = NULL;
			  osThreadYield();     //suspend thread 
		}
		
	flag = NULL;	
	osDelay(500);
	
	} //emd fo while loop 

}



/*------- LOCK MODE FUNCTION -------------------------------------- 
*  this function implements locking the device when it gets a 5-digit "12345" password
*  displays ERROR on the LCD, turns off the boiler and pump. Can only release
*  upon resetting the device
------------------------------------------------------------------------- */
__NO_RETURN void lock_thread(void *arg){
	(void) arg;
	char recv_buff[1];
	uint8_t iter = '1';
	
		while(1){
			
		// listen on the UART COM port for particular 5-digit password credentials
		while(HAL_UART_Receive(&huart2, (uint8_t*)recv_buff, sizeof(recv_buff), UART_timeOut) != HAL_TIMEOUT){
		
    // check if the user input is the 5-digits looking for   
		if(recv_buff[0] != iter){
			if(recv_buff[0] != '1'){    //ensure the first chsr is 1
				iter = '1';
			}	
			else {iter = '2';}
		}
		
		else if(iter == '5'){    // ensure the last char is 5
		
		// suspend other threads (master thread, joystick_buttons)
		osThreadSuspend(master_Id);
		osThreadSuspend(gpio_Id);
		osTimerStop(brew_time_id);
			
		// display ERROR on the LCD
		BSP_LCD_GLASS_Clear();
		osDelay(200);
		BSP_LCD_GLASS_DisplayString((uint8_t*) "ERROR!");
		
		// play unending beep sound
		play_lock_beep();
		
		// trun off the boiler and pump and vent valve
		pumpTurnOff();
		boilerTurnOff();
		valveVent();
		
		osThreadExit();	
		}
		
		else {iter ++;}
	} 
	osDelay(200);	
	} //end of while loop

}



/*get gyro iputs and broadcast over COM port */
__NO_RETURN void Gyro_Inputs(void *arg){
	(void) arg;
	char trans_buff[100];
	float buff[3] = {0};
	int32_t x_val = 0;
	int32_t y_val = 0;
	int32_t z_val = 0;
	
	
	while(1){
		
		//read gyro X, Y and Z axis information
		BSP_GYRO_GetXYZ(buff);
		
		//process the readings
		x_val = (int32_t) buff[0];
		y_val = (int32_t) buff[1];
		z_val = (int32_t) buff[2];
		
		if(x_val > GYRO_THRESHOLD_DETECTION && y_val > GYRO_THRESHOLD_DETECTION && z_val > GYRO_THRESHOLD_DETECTION){
			sprintf(trans_buff, " position X:%d Y:%d Z:%d \r\n",x_val, y_val, z_val);
				}
		
		// boradcast the readings over the terminal every 500ms		
		osDelay(500);
		
		HAL_UART_Transmit(&huart2, (uint8_t*)trans_buff, sizeof(trans_buff), HAL_MAX_DELAY);
		
	osDelay(100);
	} // end of while loop 
}

