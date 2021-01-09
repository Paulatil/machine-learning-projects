/*========================================================
*                THREADS HEADER FILE
========================================================== */
/* Define to prevent recursive  inclusion ----------------- */
#ifndef   __THREADS_H
#define   __THREADS_H

#ifdef  __cplusplus
 extern "C" {
 #endif
 
 /* includes ------------------------------------- */
 #include "cmsis_os2.h"
 
 /* defines ------------------------------------------ */
#define UART_timeOut												(0xFFFFFFFF)
#define GYRO_THRESHOLD_DETECTION 						10000

 /* prototype functions ----------------------------- */
void temp_bring_up(void);
void brew_stage(void);
void Master_thread(void *arg);
void lock_thread(void *arg);
void Gyro_Inputs(void *arg);

/* variables ------------------------------------------ */
 extern uint32_t   						start_brew_timer;
 extern uint32_t   						brew_time_complete;
 extern uint32_t   						status_flag;
 extern osThreadId_t					master_Id;
 extern osThreadId_t					hwsim_Id;
 
  #ifdef __cplusplus
}
#endif
#endif    /* __THREADS_H  */
