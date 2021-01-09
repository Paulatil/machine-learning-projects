/*=============================================
 Hardware initialization header file
 This file consists of relevant exported functions
 That initializes needed hardware peripherals 
 such GPIO, Audio, LCD, USART etc
==============================================*/

/* Define to prevent recursive  inclusion ----------------- */
#ifndef   __HARDWARE_INIT_H
#define   __HARDWARE_INIT_H

#ifdef  __cplusplus
 extern "C" {
 #endif
 
 
 /* External variables  --------------------------------------*/
extern UART_HandleTypeDef 					 huart2;
extern DFSDM_Channel_HandleTypeDef   DfsdmChannelHandle;

/* Expoerted functions ------------------------------------- */
 void SystemClock_Config(void);
 void GPIO_Init(void);
 void LCD_Init(void);
 void USART2_UART_Init(void);
 void DFSDM_Init(void);
 void TIM7_Init(void);
 
 #ifdef __cplusplus
}
#endif
#endif    /* __HARDWARE_INIT_H  */
