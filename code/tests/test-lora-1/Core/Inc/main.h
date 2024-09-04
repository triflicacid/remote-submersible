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
#include "stm32l4xx_hal.h"

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

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define RadioSelect_Pin GPIO_PIN_2
#define RadioSelect_GPIO_Port GPIOA
#define RadioDIO_Pin GPIO_PIN_8
#define RadioDIO_GPIO_Port GPIOA
#define RadioDIO_EXTI_IRQn EXTI9_5_IRQn
#define RadioReset_Pin GPIO_PIN_9
#define RadioReset_GPIO_Port GPIOA
#define SendCode_Pin GPIO_PIN_10
#define SendCode_GPIO_Port GPIOC
#define SendCode_EXTI_IRQn EXTI15_10_IRQn
#define RequestCode_Pin GPIO_PIN_11
#define RequestCode_GPIO_Port GPIOC
#define RequestCode_EXTI_IRQn EXTI15_10_IRQn
#define ReleasePod_Pin GPIO_PIN_12
#define ReleasePod_GPIO_Port GPIOC
#define ReleasePod_EXTI_IRQn EXTI15_10_IRQn

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
