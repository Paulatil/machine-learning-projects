/*========================================================
*                TIMER HEADER FILE
========================================================== */
/* Define to prevent recursive  inclusion ----------------- */
#ifndef   __TIMER_H
#define   __TIMER_H

#ifdef  __cplusplus
 extern "C" {
 #endif
 
 /* includes ------------------------------------- */
 #include "cmsis_os2.h"
 
 /* defines ------------------------------------------ */

 /* prototype functions ----------------------------- */
void Init_Timer(void* arg);

/* variables ------------------------------------------ */
extern  osThreadId_t												mtimer_Id;
extern osThreadId_t													brew_time_id; 
  #ifdef __cplusplus
}
#endif
#endif    /* __TIMER_H  */
