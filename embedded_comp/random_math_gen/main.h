/*=============================================
              Main header file
 This file includes all peripheral interfaces required
 in this project. 
==============================================*/

/* Define to prevent recursive  inclusion ----------------- */
#ifndef   __MAIN_H
#define   __MAIN_H

#ifdef  __cplusplus
 extern "C" {
 #endif
 
/* Includes ------------------------------------------------------------------*/
#include "stm32l4xx_hal.h"
#include "stm32l476g_discovery.h"
#include "audio.h"
#include "cs43l22.h"
#include "stm32l476g_discovery_glass_lcd.h"
#include "stdio.h"
#include "string.h"
#include "stdlib.h"
#include "stm32l476g_discovery_gyroscope.h"


/* variables ***********************************************************/
extern TIM_HandleTypeDef	       	htim7;


/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* Defines ------------------------------------------------------------------- */
#define AUDIO_SAIx                           SAI1_Block_A
#define AUDIO_SAIx_CLK_ENABLE()              __HAL_RCC_SAI1_CLK_ENABLE()
#define AUDIO_SAIx_FS_GPIO_PORT              GPIOE
#define AUDIO_SAIx_FS_AF                     GPIO_AF13_SAI1
#define AUDIO_SAIx_FS_PIN                    GPIO_PIN_4
#define AUDIO_SAIx_SCK_GPIO_PORT             GPIOE
#define AUDIO_SAIx_SCK_AF                    GPIO_AF13_SAI1
#define AUDIO_SAIx_SCK_PIN                   GPIO_PIN_5
#define AUDIO_SAIx_SD_GPIO_PORT              GPIOE
#define AUDIO_SAIx_SD_AF                     GPIO_AF13_SAI1
#define AUDIO_SAIx_SD_PIN                    GPIO_PIN_6
#define AUDIO_SAIx_MCLK_GPIO_PORT            GPIOE
#define AUDIO_SAIx_MCLK_AF                   GPIO_AF13_SAI1
#define AUDIO_SAIx_MCLK_PIN                  GPIO_PIN_2
#define AUDIO_RST_Pin						 						 GPIO_PIN_3
#define AUDIO_RST_GPIO_Port 								 GPIOE

#define MFX_IRQ_OUT_Pin 					    GPIO_PIN_13
#define MFX_IRQ_OUT_GPIO_Port 				GPIOC
#define MAG_INT_Pin 					      	GPIO_PIN_1
#define MAG_INT_GPIO_Port 					  GPIOC
#define MAG_DRDY_Pin 						      GPIO_PIN_2
#define MAG_DRDY_GPIO_Port 					  GPIOC
#define VLCD_Pin 							        GPIO_PIN_3
#define VLCD_GPIO_Port 						    GPIOC
#define JOYSTICK_CENTER_Pin 					GPIO_PIN_0
#define JOYSTICK_CENTER_GPIO_Port 		GPIOA
#define JOYSTICK_CENTER_EXTI_IRQn 		EXTI0_IRQn
#define JOYSTICK_LEFT_Pin 						GPIO_PIN_1
#define JOYSTICK_LEFT_GPIO_Port 			GPIOA
#define JOYSTICK_LEFT_EXTI_IRQn 			EXTI1_IRQn
#define JOYSTICK_RIGHT_Pin 						GPIO_PIN_2
#define JOYSTICK_RIGHT_GPIO_Port 			GPIOA
#define JOYSTICK_RIGHT_EXTI_IRQn      EXTI2_IRQn
#define JOYSTICK_UP_Pin 							GPIO_PIN_3
#define JOYSTICK_UP_GPIO_Port 				GPIOA
#define JOYSTICK_UP_EXTI_IRQn         EXTI3_IRQn
#define JOYSTICK_WAKEUP_Pin 				  GPIO_PIN_4
#define JOYSTICK_WAKEUP_GPIO_Port 	  GPIOA
#define JOYSTICK_DOWN_Pin 						GPIO_PIN_5
#define JOYSTICK_DOWN_GPIO_Port 			GPIOA
#define JOYSTICK_DOWN_EXTI_IRQn 			EXTI9_5_IRQn 

#define SEG23_Pin 							      GPIO_PIN_6
#define SEG23_GPIO_Port 					    GPIOA
#define SEG0_Pin 							        GPIO_PIN_7
#define SEG0_GPIO_Port 						    GPIOA
#define SEG22_Pin 							      GPIO_PIN_4
#define SEG22_GPIO_Port 					    GPIOC
#define SEG1_Pin 						        	GPIO_PIN_5
#define SEG1_GPIO_Port 						    GPIOC
#define SEG21_Pin 							      GPIO_PIN_0
#define SEG21_GPIO_Port 					    GPIOB
#define SEG2_Pin 							        GPIO_PIN_1
#define SEG2_GPIO_Port 						    GPIOB
#define LD_R_Pin 						       	  GPIO_PIN_2
#define LD_R_GPIO_Port 						    GPIOB
#define AUDIO_DIN_Pin 						    GPIO_PIN_7
#define AUDIO_DIN_GPIO_Port 				  GPIOE
#define LD_G_Pin 							        GPIO_PIN_8
#define LD_G_GPIO_Port 						    GPIOE
#define AUDIO_CLK_Pin 						    GPIO_PIN_9
#define AUDIO_CLK_GPIO_Port 				  GPIOE
#define QSPI_CLK_Pin 						      GPIO_PIN_10
#define QSPI_CLK_GPIO_Port 					  GPIOE
#define QSPI_CS_Pin 						      GPIO_PIN_11
#define QSPI_CS_GPIO_Port 				   	GPIOE
#define QSPI_D0_Pin 						      GPIO_PIN_12
#define QSPI_D0_GPIO_Port 					  GPIOE
#define QSPI_D1_Pin 						      GPIO_PIN_13
#define QSPI_D1_GPIO_Port 					  GPIOE
#define QSPI_D2_Pin 						      GPIO_PIN_14
#define QSPI_D2_GPIO_Port 					  GPIOE
#define QSPI_D3_Pin 						      GPIO_PIN_15
#define QSPI_D3_GPIO_Port 					  GPIOE
#define MFX_I2C_SLC_Pin 					    GPIO_PIN_10
#define MFX_I2C_SLC_GPIO_Port 				GPIOB
#define MFX_I2C_SDA_Pin 				    	GPIO_PIN_11
#define MFX_I2C_SDA_GPIO_Port 				GPIOB

#define SEG20_Pin 										GPIO_PIN_12
#define SEG20_GPIO_Port 							GPIOB
#define SEG3_Pin 											GPIO_PIN_13
#define SEG3_GPIO_Port 								GPIOB
#define SEG19_Pin 										GPIO_PIN_14
#define SEG19_GPIO_Port 							GPIOB
#define SEG4_Pin 											GPIO_PIN_15
#define SEG4_GPIO_Port 								GPIOB
#define SEG18_Pin 										GPIO_PIN_8
#define SEG18_GPIO_Port 							GPIOD
#define SEG5_Pin 											GPIO_PIN_9
#define SEG5_GPIO_Port 								GPIOD
#define SEG17_Pin 										GPIO_PIN_10
#define SEG17_GPIO_Port 							GPIOD
#define SEG6_Pin 											GPIO_PIN_11
#define SEG6_GPIO_Port 								GPIOD
#define SEG16_Pin 										GPIO_PIN_12
#define SEG16_GPIO_Port 							GPIOD
#define SEG7_Pin 											GPIO_PIN_13
#define SEG7_GPIO_Port 								GPIOD
#define SEG15_Pin 										GPIO_PIN_14
#define SEG15_GPIO_Port 							GPIOD
#define SEG8_Pin 											GPIO_PIN_15
#define SEG8_GPIO_Port 								GPIOD
#define SEG14_Pin 										GPIO_PIN_6
#define SEG14_GPIO_Port 							GPIOC
#define SEG9_Pin 											GPIO_PIN_7
#define SEG9_GPIO_Port 								GPIOC
#define SEG13_Pin 										GPIO_PIN_8
#define SEG13_GPIO_Port 							GPIOC
#define OTG_FS_PowerSwitchOn_Pin 			GPIO_PIN_9
#define OTG_FS_PowerSwitchOn_GPIO_Port 		GPIOC

#define COM0_Pin 											GPIO_PIN_8
#define COM0_GPIO_Port 								GPIOA
#define COM1_Pin 											GPIO_PIN_9
#define COM1_GPIO_Port 								GPIOA
#define COM2_Pin 											GPIO_PIN_10
#define COM2_GPIO_Port 								GPIOA
#define OTG_FS_DM_Pin 								GPIO_PIN_11
#define OTG_FS_DM_GPIO_Port 					GPIOA
#define OTG_FS_DP_Pin 								GPIO_PIN_12
#define OTG_FS_DP_GPIO_Port 					GPIOA
#define SWDIO_Pin 										GPIO_PIN_13
#define SWDIO_GPIO_Port 							GPIOA
#define SWCLK_Pin 										GPIO_PIN_14
#define SWCLK_GPIO_Port 							GPIOA
#define SEG10_Pin 										GPIO_PIN_15
#define SEG10_GPIO_Port 							GPIOA
#define OTG_FS_OverCurrent_Pin 				GPIO_PIN_10
#define OTG_FS_OverCurrent_GPIO_Port 		GPIOC
#define OTG_FS_VBUS_Pin 							GPIO_PIN_11
#define OTG_FS_VBUS_GPIO_Port 				GPIOC
#define EXT_RST_Pin 									GPIO_PIN_0
#define EXT_RST_GPIO_Port 						GPIOD
#define MEMS_SCK_Pin 									GPIO_PIN_1
#define MEMS_SCK_GPIO_Port 						GPIOD
#define GYRO_INT1_Pin 								GPIO_PIN_2
#define GYRO_INT1_GPIO_Port 					GPIOD
#define MEMS_MISO_Pin 								GPIO_PIN_3
#define MEMS_MISO_GPIO_Port 					GPIOD
#define MEMS_MOSI_Pin 								GPIO_PIN_4
#define MEMS_MOSI_GPIO_Port 					GPIOD
#define USART_TX_Pin 									GPIO_PIN_5
#define USART_TX_GPIO_Port 						GPIOD
#define USART_RX_Pin 									GPIO_PIN_6
#define USART_RX_GPIO_Port 						GPIOD

#define GYRO_CS_Pin 									GPIO_PIN_7
#define GYRO_CS_GPIO_Port 						GPIOD
#define M3V3_REG_ON_Pin 							GPIO_PIN_3
#define M3V3_REG_ON_GPIO_Port 				GPIOB
#define SEG11_Pin 										GPIO_PIN_4
#define SEG11_GPIO_Port				 				GPIOB
#define SEG12_Pin 										GPIO_PIN_5
#define SEG12_GPIO_Port 							GPIOB
#define I2C1_SCL_Pin 									GPIO_PIN_6
#define I2C1_SCL_GPIO_Port 						GPIOB
#define I2C1_SDA_Pin 									GPIO_PIN_7
#define I2C1_SDA_GPIO_Port 						GPIOB
#define GYRO_INT2_Pin 								GPIO_PIN_8
#define GYRO_INT2_GPIO_Port 					GPIOB
#define GYRO_INT2_EXTI_IRQn 					EXTI9_5_IRQn
#define COM3_Pin 											GPIO_PIN_9
#define COM3_GPIO_Port 								GPIOB
#define XL_CS_Pin 										GPIO_PIN_0
#define XL_CS_GPIO_Port 							GPIOE
#define XL_INT_Pin 										GPIO_PIN_1
#define XL_INT_GPIO_Port 							GPIOE


#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
