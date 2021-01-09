/*========================================================
*                GPIO INPUTS HEADER FILE
========================================================== */
/* Define to prevent recursive  inclusion ----------------- */
#ifndef   __GPIO_INPUT_H
#define   __GPIO_INPUT_H

#ifdef  __cplusplus
 extern "C" {
 #endif
 
 /* includes ------------------------------------- */
 #include "cmsis_os2.h"
 
 /* defines ------------------------------------------ */
#define TEMP											(0x00000001)
#define BREW											(0x00000002)
#define PRESS											(0x00000004)
#define INIT											(0x00000008)
#define START											(0x00000010)

 /* prototype functions ----------------------------- */
 void      display_brew(void);
void       display_temp(void);
void       display_press(void);
void       joystick_buttons(void *arg);
uint32_t   convert_ascii2int(uint8_t* value, int length);
void       Init_upon_brew_complete(void);


/* variables ------------------------------------------ */
extern osThreadId_t												gpio_Id;
extern uint8_t 														brew_time[4];
extern uint8_t 														temp_value[4];
extern uint8_t 														press_value[5];
extern uint32_t														set_temp;
extern uint32_t														set_press;
extern uint32_t   												init; 
 
  #ifdef __cplusplus
}
#endif
#endif    /* __GPIO_INPUT_H  */

