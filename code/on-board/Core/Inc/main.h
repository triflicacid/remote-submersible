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

void HAL_TIM_MspPostInit(TIM_HandleTypeDef *htim);

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define B1_Pin GPIO_PIN_13
#define B1_GPIO_Port GPIOC
#define B1_EXTI_IRQn EXTI15_10_IRQn
#define STEPPER_BLACK_Pin GPIO_PIN_0
#define STEPPER_BLACK_GPIO_Port GPIOC
#define STEPPER_BLUE_Pin GPIO_PIN_1
#define STEPPER_BLUE_GPIO_Port GPIOC
#define STEPPER_GREEN_Pin GPIO_PIN_2
#define STEPPER_GREEN_GPIO_Port GPIOC
#define STEPPER_RED_Pin GPIO_PIN_3
#define STEPPER_RED_GPIO_Port GPIOC
#define DC_MOTOR_IN1_Pin GPIO_PIN_0
#define DC_MOTOR_IN1_GPIO_Port GPIOA
#define DC_MOTOR_IN2_Pin GPIO_PIN_1
#define DC_MOTOR_IN2_GPIO_Port GPIOA
#define DC_MOTOR_IN3_Pin GPIO_PIN_2
#define DC_MOTOR_IN3_GPIO_Port GPIOA
#define DC_MOTOR_IN4_Pin GPIO_PIN_3
#define DC_MOTOR_IN4_GPIO_Port GPIOA
#define DC_MOTOR_EN1_2_Pin GPIO_PIN_5
#define DC_MOTOR_EN1_2_GPIO_Port GPIOA
#define RFM_RESET_Pin GPIO_PIN_0
#define RFM_RESET_GPIO_Port GPIOB
#define RFM_DIO_Pin GPIO_PIN_1
#define RFM_DIO_GPIO_Port GPIOB
#define RFM_DIO_EXTI_IRQn EXTI1_IRQn
#define DC_MOTOR_EN3_4_Pin GPIO_PIN_10
#define DC_MOTOR_EN3_4_GPIO_Port GPIOB
#define RFM_SELECT_Pin GPIO_PIN_12
#define RFM_SELECT_GPIO_Port GPIOB
#define ELECTROMAGNET_Pin GPIO_PIN_3
#define ELECTROMAGNET_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
