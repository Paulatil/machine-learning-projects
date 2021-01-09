/*=============================================
 I/O Interrrupt handler header file
 This file consists of relevant exported I/O Handlers
 That handlers EXTI routines from I/Os like 
 DMA,GPIOs, DFSMD, Gyro, etc
==============================================*/

/* Define to prevent recursive  inclusion ----------------- */
#ifndef   __INTERRUPT_HANDLER_H
#define   __INTERRUPT_HANDLER_H

#ifdef  __cplusplus
 extern "C" {
 #endif
 /* includes -------------------------------------------------- */
 #include "cmsis_os2.h" 
 
/* defines ------------------------------------------------ */
#define joystick_sel					(0x00000001)     // center button flag for setting ame mode and duratioh            
#define joystick_up						(0x00000002)     // up button flag for navigating game mode options and setting game duration 
#define joystick_down					(0x00000004)     // down button flag for navigating game mode options and setting game duration
#define joystick_left					(0x00000008)     // left button flag for setting game duration (min values) 
#define joystick_right				(0x00000010)     // right button flag for setting game duration (sec vslues)
#define gyro_evnt							(0x00000020)     // gyro event flag for action question
#define joystickUp_evnt				(0x00000040)     // up button event flag for action question
#define joystickDwn_evnt			(0x00000100)     // down button event flag for action question
#define joystickLeft_evnt			(0x00000200)     // left button event flag for action question 
#define joystickRight_evnt		(0x00000400)     // right button event flag for action question  
#define mic_evnt							(0x00000080)		 //fsdm event flag for action question

 /* Exported Variables ----------------------------------------*/
extern osEventFlagsId_t  Interrupts_id;
extern osEventFlagsId_t  evnt_id;
extern uint32_t          interrupt_flags;
extern uint32_t          evnt_flags;
 
 /* Expoerted functions ------------------------------------- */
void SysTick_Handler(void);
void EXTI0_IRQHandler(void);
void EXTI1_IRQHandler(void);
void EXTI2_IRQHandler(void);
void EXTI3_IRQHandler(void);
void EXTI9_5_IRQHandler(void);
void TIM6_DAC_IRQHandler(void);
void TIM7_IRQHandler(void);  
void DMA2_Channel1_IRQHandler(void); // SAI DMA interrrupt request handler
void DMA1_Channel4_IRQHandler(void); // DFSDM DMA Interrupt request handler
 
 #ifdef __cplusplus
}
#endif


#endif    /* __INTERRUPT_HANDLER_H  */
