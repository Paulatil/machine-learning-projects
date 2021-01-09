/* ==================================================
I/O Interrupt handlers source file that implements 
EXTI and DMA inerrupt requests for this project
Interrupt sources include SAI, JOYSTICK, GYRO and DFSDM 
==================================================== */
/* Includes ---------------------------------------*/
#include "main.h"
#include "Interrupt_Handler.h"
#include "threads.h"
#include "gpio_input.h"
#include "tones.h"

/* variables  -------------------------------------- */
DMA_HandleTypeDef 				     hDfsdmDma;            // DFSDM handle
SAI_HandleTypeDef 				     SaiHandle;            // SAI handle 
uint32_t 											joy_flag = 0;


/*============================================= 
      SysTick and Peripherals Interrupt Handlers 
===============================================*/
/**
  * @brief  Period elapsed callback in non blocking mode
  * @note   This function is called  when TIM7 interrupt took place, inside
  * HAL_TIM_IRQHandler(). It makes a direct call to HAL_IncTick() to increment
  * a global variable "uwTick" used as application time base.
  * @param  htim : TIM handle
  * @retval None
  */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
  if (htim->Instance == TIM7) {  
		
    HAL_IncTick();
  
	}
  
}


/*This function handles EXTI line0 interrupt */
void EXTI0_IRQHandler(void)      // centre button press 
{  
	joy_flag = JOYSTICK_SEL;
	//osEventFlagsSet(evnt_id, joy_flag);	
	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0); 
}


/*This function handles EXTI line1 interrupt */
void EXTI1_IRQHandler(void)        // left button press
{ 
	joy_flag = JOYSTICK_LEFT;
	//osEventFlagsSet(evnt_id, joy_flag);
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_1); 
}


/* This function handles EXTI line2 interrupt */
void EXTI2_IRQHandler(void)          // right button press
{  
	joy_flag = JOYSTICK_RIGHT;
	//osEventFlagsSet(evnt_id, joy_flag);
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_2); 

}

/* This function handles EXTI line3 interrupt */

void EXTI3_IRQHandler(void)              // up button press
{ 
	joy_flag = JOYSTICK_UP;
	//osEventFlagsSet(evnt_id, joy_flag);

 	HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_3); 

}


/*This function handles EXTI line[9:5] interrupts */

void EXTI9_5_IRQHandler(void)                // down button press | Gyro signal
{ 
	joy_flag = JOYSTICK_DWN;
	//osEventFlagsSet(evnt_id, joy_flag);
			
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_5); 
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_8); 

}


/* This function handles DFSDM DMA interrupt request */
void DMA1_Channel4_IRQHandler(void)               // DFSDM interrupt
{
//	evnt_flags = MIC_EVNT;
//	osEventFlagsSet(evnt_id, evnt_flags);
  HAL_DMA_IRQHandler(&hDfsdmDma);
}


/* This function handles SAI DMA interrupt request */
void DMA2_Channel1_IRQHandler(void)                // SAI interrupt
{ 
  HAL_DMA_IRQHandler(SaiHandle.hdmatx);
}


/* This function handles TIM7 global interrupt */
void TIM7_IRQHandler(void)                        // timer count down
{
 //HAL_IncTick();
  HAL_TIM_IRQHandler(&htim7);
  
}

