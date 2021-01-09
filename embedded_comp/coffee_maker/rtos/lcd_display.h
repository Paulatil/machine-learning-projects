/*========================================================
*                LCD_DISPLAY HEADER FILE
========================================================== */
/* Define to prevent recursive  inclusion ----------------- */
#ifndef   __LCD_DISPLAY_H
#define   __LCD_DISPLAY_H

#ifdef  __cplusplus
 extern "C" {
 #endif
 
 /* includes ------------------------------------- */
 #include "cmsis_os2.h"

/* defines ------------------------------------------ */
#define TEMP											(0x00000001)
#define BREW											(0x00000002)
#define PRESS											(0x00000004)
 
 /* prototype functions ----------------------------- */

void lcd_display(void *arg);

/* variables ------------------------------------------ */
extern osThreadId_t												lcd_Id;

  #ifdef __cplusplus
}
#endif
#endif    /* __LCD_DISPLAY_H  */
