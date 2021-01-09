/*=================================================================
*    TONES source file. 
*		this file defines functions to implement short and logg beeps.
=================================================================== */
/* includes --------------------------------------------------*/
#include "cmsis_os2.h"
#include "main.h"
#include "tones.h"
#include "Interrupt_Handler.h"

/* varibales ----------------------------------------------- */
static int16_t sound_file[]={0, 4107, 8149, 12062, 15786, 19260, 22431, 25247, 27666, 29648, 31163, 32187,
32702, 32702, 32187, 31163, 29648, 27666, 25247, 22431, 19260, 15786, 12062, 8149, 4107, 0, -4107,
-8149, -12062, -15786, -19260, -22431, -25247, -27666, -29648, -31163, -32187, -32702, -32702, -32187,
-31163, -29648, -27666, -25247, -22430, -19260, -15786, -12062, -8149, -4107 };


static AUDIO_DrvTypeDef            *audio_drv;
static DMA_HandleTypeDef            hSaiDma;
uint16_t                            PlayBuff[PLAY_BUFF_SIZE];


/* structure ------------------------------------- */
struct sai
	{
		uint32_t  N;
		uint32_t  M;
} pllsai;


/* functions --------------------------------------- */
void       Init_audio(void);       
void			 play_lock_beep(void);   
void       play_beep(void);       
void       Playback_Init(void);      
void 			 long_playback(void);    
void       brew_start_tone(void);     
void       time_up_beep(void);      

/*  Initialize the AUDIO, SAI and DMA */
void Init_audio(){

	
	// initialize sai frequency tuners 
	pllsai.N = 9;
	pllsai.M = 17;
	
	Playback_Init();
	
	/* Initialize the data buffer */
  for(int wave = 0; wave < PLAY_BUFF_SIZE; wave++)
  {
    PlayBuff[wave]=*((__IO uint16_t*)(sound_file + wave));
  }	
	
	
	// setup play back 
	
	if(0 != audio_drv->Play(AUDIO_I2C_ADDRESS, NULL, 0))
		{
			Error_Handler();
		}
	
	if(HAL_OK != HAL_SAI_Transmit_DMA(&SaiHandle, (uint8_t *)PlayBuff, PLAY_BUFF_SIZE))
		{
			Error_Handler();
		}
		
		
		osDelay(500);
		
		if (HAL_OK != HAL_SAI_DMAPause(&SaiHandle))
		{
			Error_Handler();
			}
		
}

   


/* cancel beep */
void play_beep(){
	
	// play for 1sec
	
  if (HAL_OK != HAL_SAI_DMAResume(&SaiHandle))
		{
			Error_Handler();
			}
	
			
	osDelay(1000);    //wait for 1 sec
	
   			
	
	if (HAL_OK != HAL_SAI_DMAPause(&SaiHandle))
		{
			Error_Handler();
			}

}


/* lock mode beep */
void play_lock_beep(){

 	
	pllsai.N = 70;
	pllsai.M = 7;
	
	long_playback();

	
	// play indefintely 
  if (HAL_OK != HAL_SAI_DMAResume(&SaiHandle))
		{
			Error_Handler();
			}

}




/* tone to indicate start of brew */
void brew_start_tone(void){
	
	pllsai.N = 40;
	pllsai.M = 17;
	
	long_playback();
	
	if (HAL_OK != HAL_SAI_DMAResume(&SaiHandle))
		{
			Error_Handler();
			}
	
			
	osDelay(700);    //wait for 0.7 sec
	
   			
	
	if (HAL_OK != HAL_SAI_DMAPause(&SaiHandle))
		{
			Error_Handler();
			}
}



/*plays when brew time is up! */
void time_up_beep(){
	
	pllsai.N = 50;
	pllsai.M = 17;
	
	long_playback();
	
	if (HAL_OK != HAL_SAI_DMAResume(&SaiHandle))
		{
			Error_Handler();
			}
	
			
	osDelay(700);    //wait for 0.7 sec
	
   			
	
	if (HAL_OK != HAL_SAI_DMAPause(&SaiHandle))
		{
			Error_Handler();
			}
}




/* setup the different playback tones */
void long_playback(void){
	RCC_PeriphCLKInitTypeDef RCC_PeriphCLKInitStruct;
  
  /* Configure and enable PLLSAI1 clock to generate 11.294MHz */
  RCC_PeriphCLKInitStruct.PeriphClockSelection    = RCC_PERIPHCLK_SAI1;
  RCC_PeriphCLKInitStruct.PLLSAI1.PLLSAI1Source   = RCC_PLLSOURCE_MSI;
  RCC_PeriphCLKInitStruct.PLLSAI1.PLLSAI1M        = 1;
  RCC_PeriphCLKInitStruct.PLLSAI1.PLLSAI1N        = pllsai.N;
  RCC_PeriphCLKInitStruct.PLLSAI1.PLLSAI1P        = pllsai.M;
  RCC_PeriphCLKInitStruct.PLLSAI1.PLLSAI1ClockOut = RCC_PLLSAI1_SAI1CLK;
  RCC_PeriphCLKInitStruct.Sai1ClockSelection      = RCC_SAI1CLKSOURCE_PLLSAI1;
  
  if(HAL_RCCEx_PeriphCLKConfig(&RCC_PeriphCLKInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
}





/* Playback Initialization file ----------------- */
void Playback_Init(void)

{
	RCC_PeriphCLKInitTypeDef RCC_PeriphCLKInitStruct;
  
  /* Configure and enable PLLSAI1 clock to generate 11.294MHz */
  RCC_PeriphCLKInitStruct.PeriphClockSelection    = RCC_PERIPHCLK_SAI1;
  RCC_PeriphCLKInitStruct.PLLSAI1.PLLSAI1Source   = RCC_PLLSOURCE_MSI;
  RCC_PeriphCLKInitStruct.PLLSAI1.PLLSAI1M        = 1;
  RCC_PeriphCLKInitStruct.PLLSAI1.PLLSAI1N        = pllsai.N;
  RCC_PeriphCLKInitStruct.PLLSAI1.PLLSAI1P        = pllsai.M;
  RCC_PeriphCLKInitStruct.PLLSAI1.PLLSAI1ClockOut = RCC_PLLSAI1_SAI1CLK;
  RCC_PeriphCLKInitStruct.Sai1ClockSelection      = RCC_SAI1CLKSOURCE_PLLSAI1;
  
  if(HAL_RCCEx_PeriphCLKConfig(&RCC_PeriphCLKInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /* Initialize SAI */
  __HAL_SAI_RESET_HANDLE_STATE(&SaiHandle);

  SaiHandle.Instance = AUDIO_SAIx;

  __HAL_SAI_DISABLE(&SaiHandle);

  SaiHandle.Init.AudioMode      = SAI_MODEMASTER_TX;
  SaiHandle.Init.Synchro        = SAI_ASYNCHRONOUS;
  SaiHandle.Init.OutputDrive    = SAI_OUTPUTDRIVE_ENABLE;
  SaiHandle.Init.NoDivider      = SAI_MASTERDIVIDER_ENABLE;
  SaiHandle.Init.FIFOThreshold  = SAI_FIFOTHRESHOLD_1QF;
  SaiHandle.Init.AudioFrequency = SAI_AUDIO_FREQUENCY_22K;
  SaiHandle.Init.Protocol       = SAI_FREE_PROTOCOL;
  SaiHandle.Init.DataSize       = SAI_DATASIZE_16;
  SaiHandle.Init.FirstBit       = SAI_FIRSTBIT_MSB;
  SaiHandle.Init.ClockStrobing  = SAI_CLOCKSTROBING_FALLINGEDGE;
  SaiHandle.Init.SynchroExt     = SAI_SYNCEXT_DISABLE;
  SaiHandle.Init.Mckdiv         = 0; /* N.U */
  SaiHandle.Init.MonoStereoMode = SAI_STEREOMODE;
  SaiHandle.Init.CompandingMode = SAI_NOCOMPANDING;
  SaiHandle.Init.TriState       = SAI_OUTPUT_NOTRELEASED;

  SaiHandle.FrameInit.FrameLength       = 32; 
  SaiHandle.FrameInit.ActiveFrameLength = 16;
  SaiHandle.FrameInit.FSDefinition      = SAI_FS_CHANNEL_IDENTIFICATION;
  SaiHandle.FrameInit.FSPolarity        = SAI_FS_ACTIVE_LOW;
  SaiHandle.FrameInit.FSOffset          = SAI_FS_BEFOREFIRSTBIT;

  SaiHandle.SlotInit.FirstBitOffset = 0;
  SaiHandle.SlotInit.SlotSize       = SAI_SLOTSIZE_DATASIZE;
  SaiHandle.SlotInit.SlotNumber     = 2; 
  SaiHandle.SlotInit.SlotActive     = (SAI_SLOTACTIVE_0 | SAI_SLOTACTIVE_1);

  if(HAL_OK != HAL_SAI_Init(&SaiHandle))
  {
    Error_Handler();
  }
  
  /* Enable SAI to generate clock used by audio driver */
  __HAL_SAI_ENABLE(&SaiHandle);
  
  /* Initialize audio driver */
  if(CS43L22_ID != cs43l22_drv.ReadID(AUDIO_I2C_ADDRESS))
  {
    Error_Handler();
  }
  
  audio_drv = &cs43l22_drv;
  audio_drv->Reset(AUDIO_I2C_ADDRESS);  
  if(0 != audio_drv->Init(AUDIO_I2C_ADDRESS, OUTPUT_DEVICE_HEADPHONE, 80, AUDIO_FREQUENCY_22K))
  {
    Error_Handler();
  } 
} 


/**
* @brief SAI MSP Initialization
* This function configures the hardware resources used in this project
* @param hsai: SAI handle pointer
* @retval None
*/

void HAL_SAI_MspInit(SAI_HandleTypeDef *hsai)
{
  GPIO_InitTypeDef  GPIO_Init;
 
	    
  /* Enable SAI1 clock */
  __HAL_RCC_SAI1_CLK_ENABLE();
  
  /* Configure GPIOs used for SAI1 */
  
  GPIO_Init.Mode      = GPIO_MODE_AF_PP;
  GPIO_Init.Pull      = GPIO_NOPULL;
  GPIO_Init.Speed     = GPIO_SPEED_FREQ_VERY_HIGH;
  
  GPIO_Init.Alternate = AUDIO_SAIx_FS_AF;
  GPIO_Init.Pin       = AUDIO_SAIx_FS_PIN;
  HAL_GPIO_Init(AUDIO_SAIx_FS_GPIO_PORT, &GPIO_Init);
  GPIO_Init.Alternate = AUDIO_SAIx_SCK_AF;
  GPIO_Init.Pin       = AUDIO_SAIx_SCK_PIN;
  HAL_GPIO_Init(AUDIO_SAIx_SCK_GPIO_PORT, &GPIO_Init);
  GPIO_Init.Alternate = AUDIO_SAIx_SD_AF;
  GPIO_Init.Pin       = AUDIO_SAIx_SD_PIN;
  HAL_GPIO_Init(AUDIO_SAIx_SD_GPIO_PORT, &GPIO_Init);
  GPIO_Init.Alternate = AUDIO_SAIx_MCLK_AF;
  GPIO_Init.Pin       = AUDIO_SAIx_MCLK_PIN;
  HAL_GPIO_Init(AUDIO_SAIx_MCLK_GPIO_PORT, &GPIO_Init);
  
  /* Configure DMA used for SAI1 */
  __HAL_RCC_DMA2_CLK_ENABLE();

  if(hsai->Instance == AUDIO_SAIx)
  {
  
    hSaiDma.Init.Request             = DMA_REQUEST_1;
    hSaiDma.Init.Direction           = DMA_MEMORY_TO_PERIPH;
    hSaiDma.Init.PeriphInc           = DMA_PINC_DISABLE;
    hSaiDma.Init.MemInc              = DMA_MINC_ENABLE;
    hSaiDma.Init.PeriphDataAlignment = DMA_PDATAALIGN_HALFWORD;
    hSaiDma.Init.MemDataAlignment    = DMA_MDATAALIGN_HALFWORD;
    hSaiDma.Init.Mode                = DMA_CIRCULAR;
    hSaiDma.Init.Priority            = DMA_PRIORITY_HIGH;

    /* Select the DMA instance to be used for the transfer : DMA2_Channel1 */
    hSaiDma.Instance                 = DMA2_Channel1;
  
    /* Associate the DMA handle */
    __HAL_LINKDMA(hsai, hdmatx, hSaiDma);

    /* Deinitialize the Stream for new transfer */
    HAL_DMA_DeInit(&hSaiDma);

    /* Configure the DMA channel */
    if (HAL_OK != HAL_DMA_Init(&hSaiDma))
    {
      Error_Handler();
    }
  }
	
  HAL_NVIC_SetPriority(DMA2_Channel1_IRQn, 0x01, 0);
  HAL_NVIC_EnableIRQ(DMA2_Channel1_IRQn);
}
