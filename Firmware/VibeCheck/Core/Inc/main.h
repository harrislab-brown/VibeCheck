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
#define ACCEL_SCK3_Pin GPIO_PIN_2
#define ACCEL_SCK3_GPIO_Port GPIOE
#define ACCEL_INTA3_Pin GPIO_PIN_3
#define ACCEL_INTA3_GPIO_Port GPIOE
#define ACCEL_INTA3_EXTI_IRQn EXTI3_IRQn
#define ACCEL_NCS3_Pin GPIO_PIN_4
#define ACCEL_NCS3_GPIO_Port GPIOE
#define ACCEL_MISO3_Pin GPIO_PIN_5
#define ACCEL_MISO3_GPIO_Port GPIOE
#define ACCEL_MOSI3_Pin GPIO_PIN_6
#define ACCEL_MOSI3_GPIO_Port GPIOE
#define ACCEL_INTB3_Pin GPIO_PIN_13
#define ACCEL_INTB3_GPIO_Port GPIOC
#define ACCEL_INTB3_EXTI_IRQn EXTI15_10_IRQn
#define GPIO_TIMING2_Pin GPIO_PIN_2
#define GPIO_TIMING2_GPIO_Port GPIOC
#define GPIO_TIMING1_Pin GPIO_PIN_0
#define GPIO_TIMING1_GPIO_Port GPIOA
#define MUTE_INDICATOR_Pin GPIO_PIN_1
#define MUTE_INDICATOR_GPIO_Port GPIOA
#define MUTE_BUTTON_Pin GPIO_PIN_2
#define MUTE_BUTTON_GPIO_Port GPIOA
#define MUTE_SIGNAL_Pin GPIO_PIN_3
#define MUTE_SIGNAL_GPIO_Port GPIOA
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
#define ACCEL_INTA1_EXTI_IRQn EXTI9_5_IRQn
#define ACCEL_INTB1_Pin GPIO_PIN_9
#define ACCEL_INTB1_GPIO_Port GPIOD
#define ACCEL_INTB1_EXTI_IRQn EXTI9_5_IRQn
#define LED_ACCEL_Pin GPIO_PIN_12
#define LED_ACCEL_GPIO_Port GPIOD
#define LED_STROBE_1_Pin GPIO_PIN_6
#define LED_STROBE_1_GPIO_Port GPIOC
#define LED_STROBE_2_Pin GPIO_PIN_7
#define LED_STROBE_2_GPIO_Port GPIOC
#define LED_STROBE_3_Pin GPIO_PIN_8
#define LED_STROBE_3_GPIO_Port GPIOC
#define DAC_EXT_Pin GPIO_PIN_9
#define DAC_EXT_GPIO_Port GPIOC
#define BIG_RED_BUTTON_Pin GPIO_PIN_15
#define BIG_RED_BUTTON_GPIO_Port GPIOA
#define ACCEL_SCK2_Pin GPIO_PIN_10
#define ACCEL_SCK2_GPIO_Port GPIOC
#define ACCEL_MISO2_Pin GPIO_PIN_11
#define ACCEL_MISO2_GPIO_Port GPIOC
#define ACCEL_MOSI2_Pin GPIO_PIN_12
#define ACCEL_MOSI2_GPIO_Port GPIOC
#define ACCEL_NCS2_Pin GPIO_PIN_0
#define ACCEL_NCS2_GPIO_Port GPIOD
#define ACCEL_INTA2_Pin GPIO_PIN_1
#define ACCEL_INTA2_GPIO_Port GPIOD
#define ACCEL_INTA2_EXTI_IRQn EXTI1_IRQn
#define ACCEL_INTB2_Pin GPIO_PIN_2
#define ACCEL_INTB2_GPIO_Port GPIOD
#define ACCEL_INTB2_EXTI_IRQn EXTI2_IRQn
#define VCP_TX_Pin GPIO_PIN_6
#define VCP_TX_GPIO_Port GPIOB
#define VCP_RX_Pin GPIO_PIN_7
#define VCP_RX_GPIO_Port GPIOB
#define RECORD_INDICATOR_Pin GPIO_PIN_8
#define RECORD_INDICATOR_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
