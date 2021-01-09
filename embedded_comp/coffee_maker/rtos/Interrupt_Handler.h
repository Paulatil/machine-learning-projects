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
#define JOYSTICK_SEL					(0x00000001)     // select_mode and interrupt evnt flags idx 0             
#define JOYSTICK_UP						(0x00000002)     //  select_mode and interrupt evnt flags idx 1
#define JOYSTICK_DWN					(0x00000004)     //  select_mode and interrupt evnt flags idx 2
#define JOYSTICK_LEFT					(0x00000008)     //  interrupt evnt flag idx 3
#define JOYSTICK_RIGHT				(0x00000010)     //  interrupt evnt flag idx 4
#define GYRO_EVNT							(0x00000020)     //  evnt evnt flag idx 5
#define JOYSTICKUP_EVNT				(0x00000040)     //  evnt evnt flag idx 6
#define JOYSTICKDWN_EVNT			(0x00000100)     //  evnt evnt flag idx 8
#define JOYSTICKLEFT_EVNT			(0x00000200)     //  evnt evnt flag idx 9
#define JOYSTICKRIGHT_EVNT		(0x00000400)     //   evnt evnt flag idx 10
#define MIC_EVNT							(0x00000080)		 //  evnt evnt flag idx 7

 /* Exported Variables ----------------------------------------*/
extern osEventFlagsId_t  Interrupts_id;
extern osEventFlagsId_t  evnt_id;
extern uint32_t          interrupt_flag, joy_flag;
extern uint32_t          evnt_flags;
extern DMA_HandleTypeDef 				     hDfsdmDma; 
extern SAI_HandleTypeDef 				     SaiHandle;
 /* Expoerted functions ------------------------------------- */
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
