/*========================================================
*                TONES HEADER FILE
========================================================== */
/* Define to prevent recursive  inclusion ----------------- */
#ifndef   __TONES_H
#define   __TONES_H

#ifdef  __cplusplus
 extern "C" {
 #endif
 
 /* includes ------------------------------------- */
 #include "cmsis_os2.h"
 
 /* defines ------------------------------------------ */
#define WAVE_FILE_SIZE								50
#define PLAY_BUFF_SIZE								50 


 /* prototype functions ----------------------------- */
void			 play_lock_beep(void);
void       play_beep(void);
void       Playback_Init(void);
void       Init_audio(void);
void       long_playback(void);
void 			 brew_start_tone(void);
void       time_up_beep(void);

/* variables ------------------------------------------ */
 extern uint16_t                            PlayBuff[PLAY_BUFF_SIZE];
 extern struct    			                   sai pllsai;
 
 
  #ifdef __cplusplus
}
#endif
#endif    /* __TONES_H  */
