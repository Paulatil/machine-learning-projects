/* ================================================================================
 *                      THE MAIN SOURCE MODULE
			This module initializes the system, hardware, clocks, RTOS kernel,
       and run kernel start function to start thread executions.			
 *-================================================================================ */
#include "cmsis_os2.h"
#include "main.h"
#include "Threads.h"
#include "Hardware_Init.h"

 
 
 /* variables ----------------------------------------- */
 osThreadId_t               top_id;
 TIM_HandleTypeDef	       	htim7;
 
/*----------------------------------------------------------------------------
 * Application main thread
 *---------------------------------------------------------------------------*/
 
 int main (void) {
 
// Reset all peripherals, initialize the flash interface and systick
	HAL_Init();
	 
	SystemClock_Config(); 
	
	 TIM7_Init();
	HAL_TIM_Base_Start_IT(&htim7);
 
  // hardware initializations
	LCD_Init();
	BSP_LED_Init(LED4);
	BSP_LED_Init(LED5);
	BSP_LCD_GLASS_Init();
	GPIO_Init();
	USART2_UART_Init();
	DFSDM_Init();
	__enable_irq();
	 
   osKernelInitialize();    // initialize the kernel
 
 /* create the top level thread */
     top_id = osThreadNew(Init_Thread, NULL, NULL);		
			
  osKernelStart();                      // Start thread scheduler 
  
  for (;;) {
	 
	}
}

