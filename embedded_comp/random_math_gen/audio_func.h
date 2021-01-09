/*=============================================
        Timer header file
 This file icludes threadID and flag, 
 it implements the game duration and 
 game end alarm thread creation  
==============================================*/

/* Define to prevent recursive  inclusion ----------------- */
#ifndef   __AUDIO_FUNC_H
#define   __AUDIO_FUNC_H

#ifdef  __cplusplus
 extern "C" {
 #endif
 
 /* includes ----------------------------------------*/
 #include "cmsis_os2.h"
 
 /* defines ---------------------------------------- */
 #define  quarter_beep_flag				   (0x00000001)
 #define 	correct_answer_flag        (0x00000002)
 #define incorrect_answer_flag       (0x00000004)
 #define game_over_flag							 (0x00000001)
 #define WAVE_FILE_SIZE								50
#define PLAY_BUFF_SIZE								50 
 
 /* variables ---------------------------------------- */
extern  osThreadId_t                        correctanswer_toneId;
 extern osThreadId_t                        Incorrectanswer_toneId;
 extern osThreadId_t                        quarter_toneId;
 extern osMutexId_t													mute_id;
 extern osStatus_t													status;
 extern osThreadId_t												 timeOutId;
 extern uint16_t                            PlayBuff[PLAY_BUFF_SIZE];
 extern struct    			                   sai pllsai;
 
 /* Exported functions ------------------------------- */
 void quarter_tone(void * argument);
 void correct_answer_tone(void * argument);
 void Incorrect_answer_tone(void * argument);
 void timeOut_tone(void *argument);
 void Playback_Init(void);
 
 
 
 #ifdef __cplusplus
}
#endif


#endif    /* __AUDIO_FUNC_H  */
