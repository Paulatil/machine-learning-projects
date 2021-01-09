/*=============================================
        Select Mode header file
 This file includes thread id, mode flags, 
 mode and duration set flags and select mode thread  
==============================================*/

/* Define to prevent recursive  inclusion ----------------- */
#ifndef   __SELECT_MODE_H
#define   __SELECT_MODE_H

#ifdef  __cplusplus
 extern "C" {
 #endif
 
/* includes -------------------------------------------------- */
#include "cmsis_os2.h" 
 
/* defines ---------------------------------------------------- */
#define easy_mode 			(0x00000001)
#define medium_mode 		(0x00000002)
#define hard_mode 			(0x00000004)
#define game_duration 	(0x00000008)
#define mode_lock				(0x00000010)
#define duration_set		(0x00000020)

/* variables --------------------------------------------------- */
extern osThreadId_t sel_modeId;     // Id for select_mode thread
extern uint32_t lock_flag, joystick_flags, current_mode;
/* exported functions --------------------------------------------- */
void select_Mode(void *argument);

 #ifdef __cplusplus
}
#endif


#endif    /* __SELECT_MODE_H  */
