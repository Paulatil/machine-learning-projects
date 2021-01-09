/* ==================================================
I/O Interrupt handlers source file that implements 
EXTI and DMA inerrupt requests for this project
Interrupt sources include SAI, JOYSTICK, GYRO and DFSDM 
==================================================== */
/* Includes ---------------------------------------*/
#include "main.h"
#include "Interrupt_Handler.h"
#include "audio_func.h"



/* variables  -------------------------------------- */
static DMA_HandleTypeDef 				hDfsdmDma;     // DFSDM handle
static SAI_HandleTypeDef 							SaiHandle;     // SAI handle 
osEventFlagsId_t                Interrupts_id;   // interrupt event ID
osEventFlagsId_t                evnt_id;         // re-initialization event ID
uint32_t                        interrupt_flags;  // interrupt flag set to interrupt handlers
uint32_t                        evnt_flags;      // event flag set when game time od 00:00:00



/*============================================= 
      SysTick and Peripherals Interrupt Handlers 
===============================================*/

/*This function handles EXTI line0 interrupt */
void EXTI0_IRQHandler(void)      // centre button press 
{  
		interrupt_flags = joystick_sel;
		osEventFlagsSet(Interrupts_id, interrupt_flags);
   
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_0); 
}


/*This function handles EXTI line1 interrupt */
void EXTI1_IRQHandler(void)        // left button press
{ 
		interrupt_flags = joystick_left;
		osEventFlagsSet(Interrupts_id, interrupt_flags);
      
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_1); 

}

/* This function handles EXTI line2 interrupt */
void EXTI2_IRQHandler(void)          // right button press
{  
		interrupt_flags = joystick_right;
		osEventFlagsSet(Interrupts_id, interrupt_flags);
   
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_2); 

}

/* This function handles EXTI line3 interrupt */

void EXTI3_IRQHandler(void)              // up button press
{ 
		interrupt_flags = joystick_up;
		osEventFlagsSet(Interrupts_id, interrupt_flags);
  
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_3); 

}


/*This function handles EXTI line[9:5] interrupts */

void EXTI9_5_IRQHandler(void)                // down button press | Gyro signal
{ 
		interrupt_flags = joystick_down;
		osEventFlagsSet(Interrupts_id, interrupt_flags);
  
   evnt_flags = gyro_evnt;
	osEventFlagsSet(evnt_id, evnt_flags);
			
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_5); 
  HAL_GPIO_EXTI_IRQHandler(GPIO_PIN_8); 

}


/* This function handles DFSDM DMA interrupt request */
void DMA1_Channel4_IRQHandler(void)               // DFSDM interrupt
{
	evnt_flags = mic_evnt;
	osEventFlagsSet(evnt_id, evnt_flags);
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
  HAL_IncTick();
  HAL_TIM_IRQHandler(&htim7);
  
}

