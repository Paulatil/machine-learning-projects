/*=============================================
  This Module implements the display of 
	Game Modes and Game Time on LCD display 
==============================================*/
/* includes ------------------------------------*/
#include "main.h" 
#include "select_mode.h"
#include "gpio_func.h"
#include "lcd_display.h"
#include "timer.h"
#include "play_game.h"


/* initialize variables ------------------------------ */
uint8_t               set_time[6] = {48, 48, 48, 48, 48, 48};
 osThreadId_t         lcdId;
/* LCD display functions ----------------------------- */
void Lcd_display(void *argument){
uint32_t flag, flag1;    // recieves game mode & duration to display on LCD

	while(1){
		       // init timer if player wants to play again		
		flag1 = osThreadFlagsWait(init_flag, osFlagsWaitAny, 0);
		if(flag1 == init_flag){
			set_time[0] = 48;
			set_time[1] = 48;
			set_time[2] = 48;
			set_time[3] = 48;
			set_time[4] = 48;
			set_time[5] = 48;
			
			flag = NULL;     // clear flag
		}
		
		        // wait for flag to know what to display
		flag = osThreadFlagsWait(current_mode, osFlagsWaitAny, 0);
		
		    // display easy mode
		if(flag == easy_mode){
			BSP_LCD_GLASS_DisplayString((uint8_t*) " EASY ");
		}
		
		 // display medium mode 
		else if(flag == medium_mode){
			BSP_LCD_GLASS_DisplayString((uint8_t*) " MEDIUM ");
		}
		
		   // display hard mode 
		else if(flag == hard_mode){
			BSP_LCD_GLASS_DisplayString((uint8_t*) " HARD ");
		}
		
		     // display time to set for game duration
		else if(flag == game_duration){
				display_time();
		}
	
	}

}

/* define time display function ----------------------------*/
void display_time(void){
	BSP_LCD_GLASS_DisplayChar(&set_time[0], POINT_OFF,DOUBLEPOINT_OFF,LCD_DIGIT_POSITION_1);
	BSP_LCD_GLASS_DisplayChar(&set_time[1], POINT_OFF,DOUBLEPOINT_ON,LCD_DIGIT_POSITION_2);
	BSP_LCD_GLASS_DisplayChar(&set_time[2], POINT_OFF,DOUBLEPOINT_OFF,LCD_DIGIT_POSITION_3);
	BSP_LCD_GLASS_DisplayChar(&set_time[3], POINT_OFF,DOUBLEPOINT_ON,LCD_DIGIT_POSITION_4);
	BSP_LCD_GLASS_DisplayChar(&set_time[4], POINT_OFF,DOUBLEPOINT_OFF,LCD_DIGIT_POSITION_5);
	BSP_LCD_GLASS_DisplayChar(&set_time[5], POINT_OFF,DOUBLEPOINT_OFF,LCD_DIGIT_POSITION_6);
}
