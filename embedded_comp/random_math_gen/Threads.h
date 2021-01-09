/*========================================================
*	             This the Threads header file 
===========================================================*/
/* Define to prevent recursive  inclusion ----------------- */
#ifndef   __THREADS_H
#define   __THREADS_H

#ifdef  __cplusplus
 extern "C" {
 #endif

/* includes ----------------------------------------------*/
#include "cmsis_os2.h"

/* defines ------------------------------------------------ */


/* variables ------------------------------------------------- */
extern osThreadId_t               top_id;        

/* Exported functions ----------------------------------------- */
void Init_Thread(void* arg);


  #ifdef __cplusplus
}
#endif


#endif    /* __THREADS_H  */
