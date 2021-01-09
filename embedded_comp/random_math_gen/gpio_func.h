/*=============================================
              gpio_func header file
 This file includes joystick thread and ID
==============================================*/
/* Define to prevent recursive  inclusion ----------------- */
#ifndef   __GPIO_FUNC_H
#define   __GPIO_FUNC_H

#ifdef  __cplusplus
 extern "C" {
 #endif
 /* includes ------------------------------ */
 #include "cmsis_os2.h"


/* defines ------------------------------------- */

/* Expoerted functions ------------------------- */
 void joystick_buttons(void *argument);

/* variables ------------------------------------ */
extern osThreadId_t joystickId;


 #ifdef __cplusplus
}
#endif

#endif    /* __GPIO_FUNC_H  */
