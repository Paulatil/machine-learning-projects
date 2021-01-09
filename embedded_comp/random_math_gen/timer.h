/*=============================================
        Timer header file
 This file icludes threadID and flag, 
 it implements the game duration and 
 game end alarm thread creation  
==============================================*/

/* Define to prevent recursive  inclusion ----------------- */
#ifndef   __TIMER_H
#define   __TIMER_H

#ifdef  __cplusplus
 extern "C" {
 #endif
 
 /* includes ----------------------------------------*/
 #include "cmsis_os2.h"
 
 /* defines ---------------------------------------- */
 #define   start_timer_flag										(0x00000001)														
 
 /* variables ---------------------------------------- */
 extern osThreadId_t                        timerId;
 extern osTimerId_t													time_id;
 /* Exported functions ------------------------------- */
 void timer(void * argument);
 void Init_Timer(void *arg);
 
 
 
 
 #ifdef __cplusplus
}
#endif


#endif    /* __TIMER_H  */
