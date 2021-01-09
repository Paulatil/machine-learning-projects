/* ======================================================================
 -------- This program was written by Paul Atilola ---------------
 
This file is the driver program wirtten to implement:
* The features that aid the automation of a coffee maker
* This program demonstrates the following operations:
** Set the temperature and pressure during brewing
** Set the maximum brewing time
** Dsiplay the current temperature and pressure when boiling and brewing
** Indicate when brewing 
** keep the valve open when brewing
** keep the valve closed when boiling
** keep the valve venting when after brewing
=========================================================================*/
/* INCLUDES -------- -------------------------------------------------------*/
#include "Perif.h"
#include "main.h"
#include "stm32l4xx_it.h"
#include <string.h>
#include <stdio.h>

/* DEFINES -------------------------------------------------------------------*/
#define buffer_size			50

/* TYPEDEFS -----------------------------------------------------------------*/
static SAI_HandleTypeDef   SaiHandle;
static DMA_HandleTypeDef   hSaiDma;
static AUDIO_DrvTypeDef    *audio_drv;
static LCD_HandleTypeDef   hlcd;
static TIM_HandleTypeDef   htim7;


/* VARIABLES ----------------------------------------------------------------*/
static uint16_t  Playbuffer[buffer_size];

static int16_t sound_file[]={0, 4107, 8149, 12062, 15786, 19260, 22431, 25247, 27666, 29648, 31163, 32187,
32702, 32702, 32187, 31163, 29648, 27666, 25247, 22431, 19260, 15786, 12062, 8149, 4107, 0, -4107,
-8149, -12062, -15786, -19260, -22431, -25247, -27666, -29648, -31163, -32187, -32702, -32702, -32187,
-31163, -29648, -27666, -25247, -22430, -19260, -15786, -12062, -8149, -4107 };

static uint8_t brew_time[4] = {48,53,48,48};           // default = 05:00
static uint8_t temp_value[4] = {48,55,53,67};       // default = 75 C
static uint8_t press_value[5] = {48,57,98,97,114};      // default  = 9 bar
static int clear_flag = 0;
static int list = 0;
static int list_flag = 0;
static int inc_temp = 0;
static int dec_temp = 0;
static int inc_press = 0;
static int dec_press = 0;
static int inc_brew_time = 0;
static int dec_brew_time = 0;
static int decrement_flag = 0;
static int debounce_flag = 0;
static int brew_flag = 0;
static int warmup_flag = 0;
static uint32_t set_temp = 0;
static uint32_t set_press = 0;
static uint32_t current_temp = 0;
static uint32_t current_press = 0;
static int a2i_t[5] = {0};
static int a2i_p[5] = {0};
static char str2[10] = "";
static char str3[10] = "";
static uint8_t temp_t = 0;
static uint8_t temp_p = 0;
static uint32_t brew_count_down = 0;
static int a2i_b[5] = {0};
static uint8_t temp_b = 0;
static char str4[10] = "";

/* FUNCTIONS PROTOYTPE ----------------------------*/
void SystemClock_Config(void);
static void Playback_Init(void);
static void GPIO_Init(void);
static void LCD_Init(void);
static void TIM7_Init(void);
void BSP_LCD_GLASS_Init(void);
void load_buffer(void);
void display_brew(void);
void display_temp(void);
void display_press(void);
void play_sound(void); 
void pause_sound(void);
void resume_play(void);
void stop_sound(void);
void menu_list(void);
void joystick_up(void);
void joystick_centre(void);
void joystick_left(void);
void joystick_right(void);
void joystick_down(void);
void warm_up(void);
void brew_coffee(void);
uint32_t StringToInt_t(uint8_t* value, int size);
uint32_t StringToInt_p(uint8_t* value, int size);
uint32_t StringToInt_b(uint8_t* value, int size);
void init_state(void);
void display_values(void);

/* ==========================================================
      DISPLAY AND SOUND FUNCTIONS DEFINITIONS HERE 
=============================================================*/
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

uint32_t StringToInt_t(uint8_t* value, int size){
	  /* conver ASCII to int */
	int i = 0;
	int t = 0;
	for(; i < size; i++){
			temp_t = value[i];
			if(temp_t == 48){
				a2i_t[i] = 0;
			}
			else if(temp_t == 49){
				a2i_t[i] = 1;
			}
			else if(temp_t == 50){
				a2i_t[i] = 2;
			}
			else if(temp_t == 51){
				a2i_t[i] = 3;
			}
			else if(temp_t == 52){
				a2i_t[i] = 4;
			}
			else if(temp_t == 53){
				a2i_t[i] = 5;
			}
			else if(temp_t == 54){
				a2i_t[i] = 6;
			}
			else if(temp_t == 55){
				a2i_t[i] = 7;
			}
			else if(temp_t == 56){
				a2i_t[i] = 8;
			}
			else if(temp_t == 57){
				a2i_t[i] = 9;
			}	
		}
	     /* convert char to int */
	i = 0;	
	while( i < size ){
		t = (t * 10) + a2i_t[i];
		i++;
	}	
	
	return (uint32_t)t;
}	

uint32_t StringToInt_p(uint8_t* value, int size){
	  /* conver ASCII to int */
	int i = 0;
	int p = 0;
	
	for(; i < size; i++){
			temp_p = *(value + i);
			if(temp_p == 48){
				a2i_p[i] = 0;
			}
			else if(temp_p == 49){
				a2i_p[i] = 1;
			}
			else if(temp_p == 50){
				a2i_p[i] = 2;
			}
			else if(temp_p == 51){
				a2i_p[i] = 3;
			}
			else if(temp_p == 52){
				a2i_p[i] = 4;
			}
			else if(temp_p == 53){
				a2i_p[i] = 5;
			}
			else if(temp_p == 54){
				a2i_p[i] = 6;
			}
			else if(temp_p == 55){
				a2i_p[i] = 7;
			}
			else if(temp_p == 56){
				a2i_p[i] = 8;
			}
			else if(temp_p == 57){
				a2i_p[i] = 9;
			}	
		}
	     /* convert char to int */
	i = 0;	
	while( i < size ){
		p = (p * 10) + a2i_p[i];
		i++;
	}
		return (uint32_t)p;
}	

uint32_t StringToInt_b(uint8_t* value, int size){
	  /* conver ASCII to int */
	int i = 0;
	int p = 0;
	
	for(; i < size; i++){
			temp_b = *(value + i);
			if(temp_b == 48){
				a2i_b[i] = 0;
			}
			else if(temp_b == 49){
				a2i_b[i] = 1;
			}
			else if(temp_b == 50){
				a2i_b[i] = 2;
			}
			else if(temp_b == 51){
				a2i_b[i] = 3;
			}
			else if(temp_b == 52){
				a2i_b[i] = 4;
			}
			else if(temp_b == 53){
				a2i_b[i] = 5;
			}
			else if(temp_b == 54){
				a2i_b[i] = 6;
			}
			else if(temp_b == 55){
				a2i_b[i] = 7;
			}
			else if(temp_b == 56){
				a2i_b[i] = 8;
			}
			else if(temp_b == 57){
				a2i_b[i] = 9;
			}	
		}
	     /* convert char to int */
		
	
		p = (a2i_b[0] * 600) + (a2i_b[1] * 60) + (a2i_b[2] * 10) + (a2i_b[3] * 1);
		
	
		return (uint32_t)p;
}	

void init_state(void){
		valveVent();
		boilerTurnOff();
		pumpTurnOff();
}

void load_buffer(void){
	/* Initialize the data buffer */
  for(int i=0; i < buffer_size; i++)
  {
    Playbuffer[i]=*((__IO uint16_t*)(sound_file + i));
    }
}

void play_sound(void){
  
	// Start the playback 
  if(0 != audio_drv->Play(AUDIO_I2C_ADDRESS, NULL, 0))
  {
    Error_Handler();
  }
	// DMA transfer towards SAI
  if(HAL_OK != HAL_SAI_Transmit_DMA(&SaiHandle, (uint8_t *)Playbuffer, buffer_size))
  {
    Error_Handler();
  }
}

void pause_sound(void){
	
	// Call the Audio Codec Pause function 
	if (0 != audio_drv->Pause(AUDIO_I2C_ADDRESS))
  {
    Error_Handler();
  }

  // Pause DMA transfer towards the serial audio interface 
	if (HAL_OK != HAL_SAI_DMAPause(&SaiHandle))
  {
    Error_Handler();
  }
}

void resume_play(void){
	
	// Call the Audio Codec Resume function 
  if (0 != audio_drv->Resume(AUDIO_I2C_ADDRESS))
  {
     Error_Handler();
  }

  // Resume DMA transfer towards the serial audio interface 
  if (HAL_OK != HAL_SAI_DMAResume(&SaiHandle))
  {
     Error_Handler();
  }

}

void stop_sound(void){

	// Call Audio Codec Stop function 
  if (0 != audio_drv->Stop(AUDIO_I2C_ADDRESS, CODEC_PDWN_SW))
  {
     Error_Handler();
  }

    // Stop DMA transfer towards the serial audio interface 
  if (HAL_OK != HAL_SAI_DMAStop(&SaiHandle))
  {
     Error_Handler();
  }
}


void menu_list(void){
	if(list == 0){
		BSP_LCD_GLASS_DisplayString((uint8_t *)" MENU");
		warmup_flag = 0;
	}
	if(list_flag == 1){
		list ++;
		if(list == 1){
			BSP_LCD_GLASS_Clear();
			HAL_Delay(500);
			display_brew();
			inc_brew_time = 1;
			dec_brew_time = 1;
			inc_press = 0;
			dec_press = 0;
			list_flag = 0;
			warmup_flag = 0;	
		}
		else if(list == 2){
			BSP_LCD_GLASS_Clear();
			HAL_Delay(500);
			display_temp();
			inc_brew_time = 0;
			dec_brew_time = 0;
			inc_temp = 1;
			dec_temp = 1;
			list_flag = 0;
			warmup_flag = 0;
	}
		else if(list  == 3){
			BSP_LCD_GLASS_Clear();
			HAL_Delay(500);
			display_press();
			inc_temp = 0;
			dec_temp = 0;
			inc_press = 1;
			dec_press = 1;
			list_flag = 0;
			warmup_flag = 0;
	   }
		else{
			HAL_Delay(500);
			BSP_LCD_GLASS_Clear();
			BSP_LCD_GLASS_DisplayString((uint8_t *)" SAVED ");
			HAL_Delay(1000);
			BSP_LCD_GLASS_Clear();
			BSP_LCD_GLASS_DisplayString((uint8_t *)" MENU");
			HAL_Delay(1000);
			BSP_LCD_GLASS_Clear();
			set_temp = StringToInt_t(temp_value, 3);
			set_press = StringToInt_p(press_value, 2);
			brew_count_down = StringToInt_b(brew_time, 4);
			list = 0;
			warmup_flag = 1;						
				}
		}
	}

	
void joystick_centre(void){
	if((GPIOA -> IDR & GPIO_IDR_IDR_0) == GPIO_IDR_IDR_0){
		if(clear_flag == 1){
			list_flag = 0;
			init_state();
			if(brew_flag == 1){
				GPIOB -> ODR &= ~(GPIO_ODR_OD2);}
			brew_flag = 0;
			list = 0;	
			play_sound();
			HAL_Delay(500);
			pause_sound();
			clear_flag = 0;
			menu_list();
	}
	list_flag = 1;
  }	
}

void joystick_up(void){
	if((GPIOA -> IDR & GPIO_IDR_IDR_3) == GPIO_IDR_IDR_3){
		if(debounce_flag == 1){
			if(clear_flag == 1){
				init_state();
				if(brew_flag == 1){
					GPIOB -> ODR &= ~(GPIO_ODR_OD2);}
				brew_flag = 0;
				list = 0;	
				play_sound();
				HAL_Delay(500);
				pause_sound();
				clear_flag = 0;
				menu_list();
					}
			
		else if(inc_brew_time == 1 ){
			debounce_flag = 0;
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
						if(brew_time[0] == 55){
							brew_time[0] = 48;
						}
					}
				}
			}
			display_brew();
			debounce_flag = 0;
		}
		else if(inc_temp == 1 ){
			debounce_flag = 0;
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
			debounce_flag = 0;
		}
		else if(inc_press == 1 && debounce_flag == 1){
			debounce_flag = 0;
			press_value[1]++;
			if(press_value[1] == 58){
				press_value[0]++;
				press_value[1] = 48;
				if(press_value[0] == 58){
					press_value[0] = 48;
				}
			}
			display_press();
			debounce_flag = 0;	
		}
	 }
 }
	HAL_Delay(200);
	debounce_flag = 1;
}

void joystick_left(){
	if((GPIOA -> IDR & GPIO_IDR_IDR_1) == GPIO_IDR_IDR_1){
		if(clear_flag == 1){
			if(brew_flag == 1){
				GPIOB -> ODR &= ~(GPIO_ODR_OD2);}
			brew_flag = 0;
			list = 0;	
			play_sound();
			HAL_Delay(500);
			pause_sound();
			clear_flag = 0;
			menu_list();
	 }
	}
}


void joystick_right(void){
   if((GPIOA -> IDR & GPIO_IDR_IDR_2) == GPIO_IDR_IDR_2){	
	if(clear_flag == 1){
		init_state();
		if(brew_flag == 1){
			GPIOB -> ODR &= ~(GPIO_ODR_OD2);}
		brew_flag = 0;
		play_sound();
		HAL_Delay(500);
		pause_sound();
		clear_flag = 0;
		menu_list();
	 }
   }
}

void joystick_down(void){
	if(decrement_flag == 1){
		if(clear_flag == 1){
			init_state();
			if(brew_flag == 1){
				GPIOB -> ODR &= ~(GPIO_ODR_OD2);}
			brew_flag = 0;
			list = 0;	
			play_sound();
			HAL_Delay(500);
			pause_sound();
			clear_flag = 0;
			decrement_flag = 0;
			menu_list();
	}

		else if(dec_brew_time == 1){
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
			decrement_flag = 0;
		}
		else if(dec_temp == 1){
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
			decrement_flag = 0;
		}
		else if(dec_press == 1){
			press_value[1]--;
			if(press_value[1] == 47){
				press_value[0]--;
				press_value[1] = 57;
				if(press_value[0] == 47){
					press_value[0] = 57;
				}
			}
			display_press();
			decrement_flag = 0;
		}
	}

}

void warm_up(void){
	if(warmup_flag == 1){
		clear_flag = 1;
		HAL_TIM_Base_Start_IT(&htim7);
		boilerTurnOn();
		valveClose();
		
		if(current_temp > set_temp){
			boilerTurnOff();
			brew_flag = 1;
		}
	}
}

void brew_coffee(void){
	if(brew_flag == 1){
		if(brew_count_down > 0){
		warmup_flag = 0;
		valveOpen();	
		pumpTurnOn();
		GPIOB -> ODR = GPIO_IDR_IDR_2;
		
		if(current_temp < set_temp){
			boilerTurnOn();
		}
		if(current_press > set_press){
			pumpTurnOff();
		}
	}
		else{ brew_flag = 0;
		      init_state();
					GPIOB -> ODR &= ~(GPIO_IDR_IDR_2);}
   }	
 }


void display_values(void){
	while(warmup_flag == 1 || brew_flag == 1){
	  BSP_LCD_GLASS_Clear(); 
    sprintf( str2, " %dBAR", current_press);
    BSP_LCD_GLASS_DisplayString((uint8_t*)str2); 
		HAL_Delay(1000);
		BSP_LCD_GLASS_Clear(); 
    sprintf(str3, " %dC", current_temp);
    BSP_LCD_GLASS_DisplayString((uint8_t*)str3);
		HAL_Delay(1000);
		BSP_LCD_GLASS_Clear(); 
    sprintf(str4, " %d", brew_count_down);
    BSP_LCD_GLASS_DisplayString((uint8_t*)str4);
		HAL_Delay(1000);
		if(warmup_flag == 0 && brew_flag == 0){ break;}
		}
	}
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim){
	if(htim == &htim7){
	if(brew_flag == 1){	
		brew_count_down --;
		if(brew_count_down == 0){
	HAL_TIM_Base_Stop_IT(&htim7);}
  }
 }
}
/*===============================================================
                    THE MAIN FUNCTION HERE
=================================================================*/
int main(void){
	/*--- initializations---- */
	HAL_Init();
	SystemClock_Config();
	GPIO_Init();
  LCD_Init();
  TIM7_Init();
  BSP_LCD_GLASS_Init();
	__enable_irq();	
	Playback_Init();
	load_buffer();
	init_state();
	
/*--display set default brew_time, temperature, and pressure values--*/  
	HAL_Delay(1000);
	display_brew();
	HAL_Delay(2000);
	BSP_LCD_GLASS_Clear();
	HAL_Delay(500);
	display_temp();
	HAL_Delay(2000);
	BSP_LCD_GLASS_Clear();
	HAL_Delay(500);
	display_press();
	HAL_Delay(2000);
	BSP_LCD_GLASS_Clear();
	HAL_Delay(500);
	
	while(1){
		menu_list();
		joystick_centre();
		joystick_down();
		joystick_up();
		warm_up();
		brew_coffee();
		display_values();
	}
	
}



/*=================================================== 
             DEFINE EVENT AND INTERRUPT HANDLERS
=====================================================*/ 

void SysTick_Handler(void){
	if(brew_flag == 1 || warmup_flag == 1){
		current_temp  = readTemperature();
		current_press = readPressure();
	}
		HAL_IncTick();
}


/* joystick DOWN interrupt request */
void EXTI9_5_IRQHandler(void){
	if((GPIOA -> IDR & GPIO_IDR_IDR_5) == GPIO_IDR_IDR_5){
		decrement_flag = 1;		

	}
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_5);
}

/* External timer interrupt request*/
void TIM7_IRQHandler(void)
{
	HAL_TIM_IRQHandler(&htim7);
}

 /*SAI DMA interrupt request*/
void DMA2_Channel1_IRQHandler(void)
{ 
  HAL_DMA_IRQHandler(SaiHandle.hdmatx);
}

/*=======================================================================
          SYSTEM CONFIGURATION AND PERIPHERALS DEFINITIONS HERE 
========================================================================*/

void SystemClock_Config(void)
{
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};

  /* MSI is enabled after System reset, activate PLL with MSI as source */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_MSI;
  RCC_OscInitStruct.MSIState = RCC_MSI_ON;
  RCC_OscInitStruct.MSIClockRange = RCC_MSIRANGE_6;
  RCC_OscInitStruct.MSICalibrationValue = RCC_MSICALIBRATION_DEFAULT;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_MSI;
  RCC_OscInitStruct.PLL.PLLM = 1;
  RCC_OscInitStruct.PLL.PLLN = 30;
  RCC_OscInitStruct.PLL.PLLR = 2;
  RCC_OscInitStruct.PLL.PLLP = 7;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if(HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    //Initialization Error 
    while(1);
  }
  
  /* Select PLL as system clock source and configure the HCLK, PCLK1 and PCLK2 clocks dividers */
  RCC_ClkInitStruct.ClockType = (RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2);
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV1;  
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;  
  if(HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_4) != HAL_OK)
  {
    // Initialization Error 
    while(1);
  }
}


void Playback_Init(void)
{
  RCC_PeriphCLKInitTypeDef RCC_PeriphCLKInitStruct;
  
  /* Configure and enable PLLSAI1 clock to generate 11.294MHz */
  RCC_PeriphCLKInitStruct.PeriphClockSelection    = RCC_PERIPHCLK_SAI1;
  RCC_PeriphCLKInitStruct.PLLSAI1.PLLSAI1Source   = RCC_PLLSOURCE_MSI;
  RCC_PeriphCLKInitStruct.PLLSAI1.PLLSAI1M        = 1;
  RCC_PeriphCLKInitStruct.PLLSAI1.PLLSAI1N        = 24;
  RCC_PeriphCLKInitStruct.PLLSAI1.PLLSAI1P        = 17;
  RCC_PeriphCLKInitStruct.PLLSAI1.PLLSAI1ClockOut = RCC_PLLSAI1_SAI1CLK;
  RCC_PeriphCLKInitStruct.Sai1ClockSelection      = RCC_SAI1CLKSOURCE_PLLSAI1;
  
  if(HAL_RCCEx_PeriphCLKConfig(&RCC_PeriphCLKInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /* Initialize SAI */
  __HAL_SAI_RESET_HANDLE_STATE(&SaiHandle);

  SaiHandle.Instance = AUDIO_SAIx;

  __HAL_SAI_DISABLE(&SaiHandle);

  SaiHandle.Init.AudioMode      = SAI_MODEMASTER_TX;
  SaiHandle.Init.Synchro        = SAI_ASYNCHRONOUS;
  SaiHandle.Init.OutputDrive    = SAI_OUTPUTDRIVE_ENABLE;
  SaiHandle.Init.NoDivider      = SAI_MASTERDIVIDER_ENABLE;
  SaiHandle.Init.FIFOThreshold  = SAI_FIFOTHRESHOLD_1QF;
  SaiHandle.Init.AudioFrequency = SAI_AUDIO_FREQUENCY_22K;
  SaiHandle.Init.Protocol       = SAI_FREE_PROTOCOL;
  SaiHandle.Init.DataSize       = SAI_DATASIZE_16;
  SaiHandle.Init.FirstBit       = SAI_FIRSTBIT_MSB;
  SaiHandle.Init.ClockStrobing  = SAI_CLOCKSTROBING_FALLINGEDGE;
  SaiHandle.Init.SynchroExt     = SAI_SYNCEXT_DISABLE;            
  SaiHandle.Init.Mckdiv         = 0; /* N.U */
  SaiHandle.Init.MonoStereoMode = SAI_STEREOMODE;
  SaiHandle.Init.CompandingMode = SAI_NOCOMPANDING;
  SaiHandle.Init.TriState       = SAI_OUTPUT_NOTRELEASED;

  SaiHandle.FrameInit.FrameLength       = 32; 
  SaiHandle.FrameInit.ActiveFrameLength = 16;
  SaiHandle.FrameInit.FSDefinition      = SAI_FS_CHANNEL_IDENTIFICATION;
  SaiHandle.FrameInit.FSPolarity        = SAI_FS_ACTIVE_LOW;
  SaiHandle.FrameInit.FSOffset          = SAI_FS_BEFOREFIRSTBIT;

  SaiHandle.SlotInit.FirstBitOffset = 0;
  SaiHandle.SlotInit.SlotSize       = SAI_SLOTSIZE_DATASIZE;
  SaiHandle.SlotInit.SlotNumber     = 2; 
  SaiHandle.SlotInit.SlotActive     = (SAI_SLOTACTIVE_0 | SAI_SLOTACTIVE_1);

  if(HAL_OK != HAL_SAI_Init(&SaiHandle))
  {
    Error_Handler();
  }
  
  /* Enable SAI to generate clock used by audio driver */
  __HAL_SAI_ENABLE(&SaiHandle);
  
  /* Initialize audio driver */
  if(CS43L22_ID != cs43l22_drv.ReadID(AUDIO_I2C_ADDRESS))
  {
    Error_Handler();
  }
  
  audio_drv = &cs43l22_drv;
  audio_drv->Reset(AUDIO_I2C_ADDRESS);  
  if(0 != audio_drv->Init(AUDIO_I2C_ADDRESS, OUTPUT_DEVICE_HEADPHONE, 80, AUDIO_FREQUENCY_22K))
  {
    Error_Handler();
  } 
} 


void HAL_SAI_MspInit(SAI_HandleTypeDef *hsai)
{
  GPIO_InitTypeDef  GPIO_Init;
  
  /* Enable SAI1 clock */
  __HAL_RCC_SAI1_CLK_ENABLE();
  
  /* Configure GPIOs used for SAI1 */
  AUDIO_SAIx_MCLK_ENABLE();
  AUDIO_SAIx_SCK_ENABLE();
  AUDIO_SAIx_FS_ENABLE();
  AUDIO_SAIx_SD_ENABLE();
  
  GPIO_Init.Mode      = GPIO_MODE_AF_PP;
  GPIO_Init.Pull      = GPIO_NOPULL;
  GPIO_Init.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
  
  GPIO_Init.Alternate = AUDIO_SAIx_FS_AF;
  GPIO_Init.Pin       = AUDIO_SAIx_FS_PIN;
  HAL_GPIO_Init(AUDIO_SAIx_FS_GPIO_PORT, &GPIO_Init);
  GPIO_Init.Alternate = AUDIO_SAIx_SCK_AF;
  GPIO_Init.Pin       = AUDIO_SAIx_SCK_PIN;
  HAL_GPIO_Init(AUDIO_SAIx_SCK_GPIO_PORT, &GPIO_Init);
  GPIO_Init.Alternate = AUDIO_SAIx_SD_AF;
  GPIO_Init.Pin       = AUDIO_SAIx_SD_PIN;
  HAL_GPIO_Init(AUDIO_SAIx_SD_GPIO_PORT, &GPIO_Init);
  GPIO_Init.Alternate = AUDIO_SAIx_MCLK_AF;
  GPIO_Init.Pin       = AUDIO_SAIx_MCLK_PIN;
  HAL_GPIO_Init(AUDIO_SAIx_MCLK_GPIO_PORT, &GPIO_Init);
  
  /* Configure DMA used for SAI1 */
  __HAL_RCC_DMA2_CLK_ENABLE();

  if(hsai->Instance == AUDIO_SAIx)
  {
  
    hSaiDma.Init.Request             = DMA_REQUEST_1;
    hSaiDma.Init.Direction           = DMA_MEMORY_TO_PERIPH;
    hSaiDma.Init.PeriphInc           = DMA_PINC_DISABLE;
    hSaiDma.Init.MemInc              = DMA_MINC_ENABLE;
    hSaiDma.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
    hSaiDma.Init.MemDataAlignment    = DMA_MDATAALIGN_HALFWORD;
    hSaiDma.Init.Mode                = DMA_CIRCULAR;
    hSaiDma.Init.Priority            = DMA_PRIORITY_HIGH;

    /* Select the DMA instance to be used for the transfer : DMA2_Channel1 */
    hSaiDma.Instance                 = DMA2_Channel1;
  
    /* Associate the DMA handle */
    __HAL_LINKDMA(hsai, hdmatx, hSaiDma);

    /* Deinitialize the Stream for new transfer */
    HAL_DMA_DeInit(&hSaiDma);

    /* Configure the DMA channel */
    if (HAL_OK != HAL_DMA_Init(&hSaiDma))
    {
      Error_Handler();
    }
  }
	
  HAL_NVIC_SetPriority(DMA2_Channel1_IRQn, 0x01, 0);
  HAL_NVIC_EnableIRQ(DMA2_Channel1_IRQn);
}

 static void LCD_Init(void)
{

  /* USER CODE END LCD_Init 1 */
   hlcd.Instance = LCD;
  hlcd.Init.Prescaler = LCD_PRESCALER_1;
  hlcd.Init.Divider = LCD_DIVIDER_16;
  hlcd.Init.Duty = LCD_DUTY_1_4;
  hlcd.Init.Bias = LCD_BIAS_1_4;
  hlcd.Init.VoltageSource = LCD_VOLTAGESOURCE_INTERNAL;
  hlcd.Init.Contrast = LCD_CONTRASTLEVEL_0;
  hlcd.Init.DeadTime = LCD_DEADTIME_0;
  hlcd.Init.PulseOnDuration = LCD_PULSEONDURATION_0;
  hlcd.Init.MuxSegment = LCD_MUXSEGMENT_DISABLE;
  hlcd.Init.BlinkMode = LCD_BLINKMODE_OFF;
  hlcd.Init.BlinkFrequency = LCD_BLINKFREQUENCY_DIV8;
  hlcd.Init.HighDrive = LCD_HIGHDRIVE_DISABLE;
  if (HAL_LCD_Init(&hlcd) != HAL_OK)
  {
    Error_Handler();
  }
} 

static void TIM7_Init(void)
{

  TIM_MasterConfigTypeDef sMasterConfig = {0};

  htim7.Instance = TIM7;
  htim7.Init.Prescaler = 4 - 1;
  htim7.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim7.Init.Period = 10000 - 1;
  htim7.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_DISABLE;
  if (HAL_TIM_Base_Init(&htim7) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim7, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }

}

 static void GPIO_Init(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};
		
  /* GPIO Ports Clock Enable */  
  __HAL_RCC_GPIOE_CLK_ENABLE();
  __HAL_RCC_GPIOC_CLK_ENABLE();
  __HAL_RCC_GPIOH_CLK_ENABLE();  
  __HAL_RCC_GPIOA_CLK_ENABLE();
  __HAL_RCC_GPIOB_CLK_ENABLE();
  __HAL_RCC_GPIOD_CLK_ENABLE();  

   
  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOE, AUDIO_RST_Pin|LD_G_Pin|XL_CS_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GPIOB, LD_R_Pin|M3V3_REG_ON_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(OTG_FS_PowerSwitchOn_GPIO_Port, OTG_FS_PowerSwitchOn_Pin, GPIO_PIN_SET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(OTG_FS_VBUS_GPIO_Port, OTG_FS_VBUS_Pin, GPIO_PIN_RESET);

  /*Configure GPIO pin Output Level */
  HAL_GPIO_WritePin(GYRO_CS_GPIO_Port, GYRO_CS_Pin, GPIO_PIN_RESET);

/*Configure GPIO pin : AUDIO_RST_Pin */
  GPIO_InitStruct.Pin = AUDIO_RST_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
  HAL_GPIO_Init(AUDIO_RST_GPIO_Port, &GPIO_InitStruct); 

    /*Configure GPIO pins : MFX_IRQ_OUT_Pin OTG_FS_OverCurrent_Pin */
  GPIO_InitStruct.Pin = MFX_IRQ_OUT_Pin|OTG_FS_OverCurrent_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_EVT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct); 

    /*Configure GPIO pins : PC0 MAG_INT_Pin MAG_DRDY_Pin */
	GPIO_InitStruct.Pin = GPIO_PIN_0|MAG_INT_Pin|MAG_DRDY_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct); 

/*Configure GPIO pins : JOY_CENTER_Pin JOY_LEFT_Pin JOY_RIGHT_Pin JOY_UP_Pin */
  GPIO_InitStruct.Pin = JOY_CENTER_Pin|JOY_LEFT_Pin|JOY_RIGHT_Pin|JOY_UP_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /*Configure GPIO pin : MFX_WAKEUP_Pin */
  GPIO_InitStruct.Pin = MFX_WAKEUP_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_EVT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(MFX_WAKEUP_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : JOY_DOWN_Pin */
  GPIO_InitStruct.Pin = JOY_DOWN_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_IT_RISING;
  GPIO_InitStruct.Pull = GPIO_PULLDOWN;
  HAL_GPIO_Init(JOY_DOWN_GPIO_Port, &GPIO_InitStruct);

  /*Configure GPIO pin : LD_R_Pin */
  GPIO_InitStruct.Pin = LD_R_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(LD_R_GPIO_Port, &GPIO_InitStruct); 

/*Configure GPIO pin : AUDIO_CLK_Pin */
  GPIO_InitStruct.Pin = AUDIO_CLK_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  GPIO_InitStruct.Alternate = GPIO_AF13_SAI1;
  HAL_GPIO_Init(AUDIO_CLK_GPIO_Port, &GPIO_InitStruct); 

  /*Configure GPIO pins : QSPI_CLK_Pin QSPI_CS_Pin QSPI_D0_Pin QSPI_D1_Pin 
                           QSPI_D2_Pin QSPI_D3_Pin */
  GPIO_InitStruct.Pin = QSPI_CLK_Pin|QSPI_CS_Pin|QSPI_D0_Pin|QSPI_D1_Pin 
                          |QSPI_D2_Pin|QSPI_D3_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF10_QUADSPI;
  HAL_GPIO_Init(GPIOE, &GPIO_InitStruct); 

  /*Configure GPIO pins : MFX_I2C_SLC_Pin MFX_I2C_SDA_Pin */
  GPIO_InitStruct.Pin = MFX_I2C_SLC_Pin|MFX_I2C_SDA_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF4_I2C2;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct); 

  /*Configure GPIO pins : OTG_FS_PowerSwitchOn_Pin OTG_FS_VBUS_Pin */
  GPIO_InitStruct.Pin = OTG_FS_PowerSwitchOn_Pin|OTG_FS_VBUS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(GPIOC, &GPIO_InitStruct); 

  /*Configure GPIO pins : OTG_FS_DM_Pin OTG_FS_DP_Pin */
  GPIO_InitStruct.Pin = OTG_FS_DM_Pin|OTG_FS_DP_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF10_OTG_FS;
  HAL_GPIO_Init(GPIOA, &GPIO_InitStruct); 

  /*Configure GPIO pins : EXT_RST_Pin GYRO_INT1_Pin */
  GPIO_InitStruct.Pin = EXT_RST_Pin|GYRO_INT1_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_EVT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct); 

  /*Configure GPIO pins : MEMS_SCK_Pin MEMS_MISO_Pin MEMS_MOSI_Pin */
  GPIO_InitStruct.Pin = MEMS_SCK_Pin|MEMS_MISO_Pin|MEMS_MOSI_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF5_SPI2;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

  /*Configure GPIO pins : USART_TX_Pin USART_RX_Pin */
  GPIO_InitStruct.Pin = USART_TX_Pin|USART_RX_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF7_USART2;
  HAL_GPIO_Init(GPIOD, &GPIO_InitStruct);

   /*Configure GPIO pin : GYRO_CS_Pin */
  GPIO_InitStruct.Pin = GYRO_CS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  HAL_GPIO_Init(GYRO_CS_GPIO_Port, &GPIO_InitStruct);

   /*Configure GPIO pin : M3V3_REG_ON_Pin */
  GPIO_InitStruct.Pin = M3V3_REG_ON_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(M3V3_REG_ON_GPIO_Port, &GPIO_InitStruct);

    /*Configure GPIO pins : I2C1_SCL_Pin I2C1_SDA_Pin */
  GPIO_InitStruct.Pin = I2C1_SCL_Pin|I2C1_SDA_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_AF_OD;
  GPIO_InitStruct.Pull = GPIO_PULLUP;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_VERY_HIGH;
  GPIO_InitStruct.Alternate = GPIO_AF4_I2C1;
  HAL_GPIO_Init(GPIOB, &GPIO_InitStruct);

   /*Configure GPIO pin : GYRO_INT2_Pin */
  GPIO_InitStruct.Pin = GYRO_INT2_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_EVT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(GYRO_INT2_GPIO_Port, &GPIO_InitStruct);

   /*Configure GPIO pin : XL_CS_Pin */
  GPIO_InitStruct.Pin = XL_CS_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_OUTPUT_PP;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_LOW;
  HAL_GPIO_Init(XL_CS_GPIO_Port, &GPIO_InitStruct);

   /*Configure GPIO pin : XL_INT_Pin */
  GPIO_InitStruct.Pin = XL_INT_Pin;
  GPIO_InitStruct.Mode = GPIO_MODE_EVT_RISING;
  GPIO_InitStruct.Pull = GPIO_NOPULL;
  HAL_GPIO_Init(XL_INT_GPIO_Port, &GPIO_InitStruct);
	

  /* ---------EXTI interrupt init------------------*/
	
  HAL_NVIC_SetPriority(EXTI9_5_IRQn, 0, 0);
  HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);
}

void Error_Handler(void){

}
