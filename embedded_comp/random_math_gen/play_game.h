/*==============================================
       play_game header file
 This file consists of relevant functions, defines,
 and variables as well as threadId and threadflags
==============================================*/

/* Define to prevent recursive  inclusion ----------------- */
#ifndef   __PLAY_GAME_H
#define   __PLAY_GAME_H

#ifdef  __cplusplus
 extern "C" {
 #endif
 
 /* includes ---------------------------------------- */
 #include "cmsis_os2.h"
 
 /* defines ---------------------------------------- */
 #define UART_timeOut												(0xFFFFFFFF)
 #define timeout_flag												(0x00000020)
 #define init_flag												(0x00000040)
 /* variables ----------------------------------------- */
 extern osThreadId_t     gameId;
 extern osThreadId_t		 actionId;
 extern uint32_t         set_mode;
 extern uint32_t				 count;
 extern uint8_t 				 trans_buff[3];
 extern uint8_t 				 recv_buff[2];
 extern char		  			 string[8];
 
 /* Exported functions -------------------------------- */
 void play_game(void *argument);
 void check_action(void *argument);
 
 
  #ifdef __cplusplus
}
#endif


#endif    /* __PLAY_GAME_H  */
