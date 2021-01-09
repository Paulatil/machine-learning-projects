/*----------------------------------------------------------------------------
 * CMSIS-RTOS 'main' function template
 *---------------------------------------------------------------------------*/
 
#include "RTE_Components.h"
#include  CMSIS_device_header
#include "cmsis_os2.h"
#include "main.h"
#include "Hardware_Init.h"
#include "Interrupt_Handler.h"
#include "gpio_input.h"
#include "tones.h"
# include "threads.h"
#include "Perif_RTOS.h"
#include "Timer.h"

/* variables ----------------------------------------------------------------- */
TIM_HandleTypeDef	       	htim7;
osThreadId_t							gpio_Id;
osThreadId_t		          master_Id;
osThreadId_t							hwsim_Id;
osThreadId_t							mtimer_Id;

/*----------------------------------------------------------------------------
 *                     Application main thread
 *---------------------------------------------------------------------------*/ 

int main (void) {
 
	HAL_Init();

	TIM7_Init();

	SystemClock_Config();

	GPIO_Init();

	BSP_LCD_GLASS_Init();
	
	USART2_UART_Init();
	
	BSP_GYRO_Init();

	__enable_irq();
	
	
	Init_audio(); 
  	
	
	// System Initialization
  SystemCoreClockUpdate();
  
 
  osKernelInitialize();                 // Initialize CMSIS-RTOS
 
  gpio_Id =   osThreadNew(joystick_buttons, NULL, NULL);
	master_Id = osThreadNew(Master_thread, NULL, NULL);
	hwsim_Id =  osThreadNew(hardwareSimulationThreadFunction, NULL, NULL);
	mtimer_Id = osThreadNew(Init_Timer, NULL, NULL); 
            	osThreadNew(Gyro_Inputs, NULL, NULL);    // gyro thread  
            	osThreadNew(lock_thread, NULL, NULL);     // lock mode thread
	osKernelStart();                      // Start thread execution
  for (;;) {}
}
