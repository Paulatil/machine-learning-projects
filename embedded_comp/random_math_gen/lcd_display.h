/*=============================================
              LCD dsiplay header file
 This file includes ldc display thread and threadID,
 and set_time array
==============================================*/

/* Define to prevent recursive  inclusion ----------------- */
#ifndef   __LCD_DISPLAY_H
#define   __LCD_DISPLAY_H

#ifdef  __cplusplus
 extern "C" {
 #endif

/* includes -------------------------------------------------- */
#include "cmsis_os2.h" 


/* variables --------------------------------------------------- */
extern osThreadId_t         lcdId;      // Id for the lcd_display thread
extern uint8_t              set_time[6]; 

/* Exported functions ------------------------------------------ */
void Lcd_display(void *argument);
void display_time(void);
 
 
 
 #ifdef __cplusplus
}
#endif


#endif    /* __LCD_DISPLAY_H  */
