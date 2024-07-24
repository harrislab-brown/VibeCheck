/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2024 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */

/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32h7xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define SPI4_INT1_Pin GPIO_PIN_3
#define SPI4_INT1_GPIO_Port GPIOE
#define SPI4_CS_Pin GPIO_PIN_4
#define SPI4_CS_GPIO_Port GPIOE
#define SPI4_INT2_Pin GPIO_PIN_13
#define SPI4_INT2_GPIO_Port GPIOC
#define BIG_RED_BUTTON_Pin GPIO_PIN_2
#define BIG_RED_BUTTON_GPIO_Port GPIOB
#define ACCEL_NCS1_Pin GPIO_PIN_12
#define ACCEL_NCS1_GPIO_Port GPIOB
#define ACCEL_SCK1_Pin GPIO_PIN_13
#define ACCEL_SCK1_GPIO_Port GPIOB
#define ACCEL_MISO1_Pin GPIO_PIN_14
#define ACCEL_MISO1_GPIO_Port GPIOB
#define ACCEL_MOSI1_Pin GPIO_PIN_15
#define ACCEL_MOSI1_GPIO_Port GPIOB
#define ACCEL_INTA1_Pin GPIO_PIN_8
#define ACCEL_INTA1_GPIO_Port GPIOD
#define ACCEL_INTB1_Pin GPIO_PIN_9
#define ACCEL_INTB1_GPIO_Port GPIOD
#define LED_ACCEL1_Pin GPIO_PIN_12
#define LED_ACCEL1_GPIO_Port GPIOD
#define LED_ACCEL2_Pin GPIO_PIN_13
#define LED_ACCEL2_GPIO_Port GPIOD
#define LED_ACCEL3_Pin GPIO_PIN_14
#define LED_ACCEL3_GPIO_Port GPIOD
#define LED_UI_Pin GPIO_PIN_15
#define LED_UI_GPIO_Port GPIOD
#define LED_STROBE_Pin GPIO_PIN_6
#define LED_STROBE_GPIO_Port GPIOC
#define USB_DM_Pin GPIO_PIN_11
#define USB_DM_GPIO_Port GPIOA
#define USB_DP_Pin GPIO_PIN_12
#define USB_DP_GPIO_Port GPIOA
#define SPI3_CS_Pin GPIO_PIN_0
#define SPI3_CS_GPIO_Port GPIOD
#define SPI3_INT1_Pin GPIO_PIN_1
#define SPI3_INT1_GPIO_Port GPIOD
#define SPI3_INT2_Pin GPIO_PIN_2
#define SPI3_INT2_GPIO_Port GPIOD

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
