/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
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
#include "stm32f4xx_hal.h"

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
#define SENSOR_RAIN_Pin GPIO_PIN_3
#define SENSOR_RAIN_GPIO_Port GPIOF
#define SW_PWDG_OFF_Pin GPIO_PIN_4
#define SW_PWDG_OFF_GPIO_Port GPIOF
#define SW_LED_OFF_Pin GPIO_PIN_5
#define SW_LED_OFF_GPIO_Port GPIOF
#define SS_REL_3_Pin GPIO_PIN_11
#define SS_REL_3_GPIO_Port GPIOF
#define SS_REL_2_Pin GPIO_PIN_12
#define SS_REL_2_GPIO_Port GPIOF
#define SS_REL_1_Pin GPIO_PIN_13
#define SS_REL_1_GPIO_Port GPIOF
#define GP_REL_3_Pin GPIO_PIN_14
#define GP_REL_3_GPIO_Port GPIOF
#define GP_REL_2_Pin GPIO_PIN_15
#define GP_REL_2_GPIO_Port GPIOF
#define GP_REL_1_Pin GPIO_PIN_0
#define GP_REL_1_GPIO_Port GPIOG
#define DIR_REL_Pin GPIO_PIN_1
#define DIR_REL_GPIO_Port GPIOG
#define LED_MOTOR_MOVE_Pin GPIO_PIN_11
#define LED_MOTOR_MOVE_GPIO_Port GPIOE
#define LED_OPEN_CLOSE_GATE_1_Pin GPIO_PIN_12
#define LED_OPEN_CLOSE_GATE_1_GPIO_Port GPIOE
#define LED_OPEN_CLOSE_GATE_2_Pin GPIO_PIN_13
#define LED_OPEN_CLOSE_GATE_2_GPIO_Port GPIOE
#define LED_RAIN_Pin GPIO_PIN_14
#define LED_RAIN_GPIO_Port GPIOE
#define LED_PWM_1_Pin GPIO_PIN_14
#define LED_PWM_1_GPIO_Port GPIOB
#define PWM1_Pin GPIO_PIN_15
#define PWM1_GPIO_Port GPIOB
#define CLOSE_1_Pin GPIO_PIN_10
#define CLOSE_1_GPIO_Port GPIOD
#define OPEN_1_Pin GPIO_PIN_11
#define OPEN_1_GPIO_Port GPIOD
#define CLOSE_2_Pin GPIO_PIN_12
#define CLOSE_2_GPIO_Port GPIOD
#define OPEN_2_Pin GPIO_PIN_13
#define OPEN_2_GPIO_Port GPIOD
#define CLOSE_3_Pin GPIO_PIN_14
#define CLOSE_3_GPIO_Port GPIOD
#define OPEN_3_Pin GPIO_PIN_15
#define OPEN_3_GPIO_Port GPIOD
#define CLOSE_4_Pin GPIO_PIN_2
#define CLOSE_4_GPIO_Port GPIOG
#define OPEN_4_Pin GPIO_PIN_3
#define OPEN_4_GPIO_Port GPIOG
#define HOME_Pin GPIO_PIN_4
#define HOME_GPIO_Port GPIOG
#define PWM2_Pin GPIO_PIN_7
#define PWM2_GPIO_Port GPIOC
#define PWM3_Pin GPIO_PIN_8
#define PWM3_GPIO_Port GPIOC
#define PWM4_Pin GPIO_PIN_8
#define PWM4_GPIO_Port GPIOA
#define RIGHT_CLOSE_Pin GPIO_PIN_0
#define RIGHT_CLOSE_GPIO_Port GPIOD
#define RIGHT_CLOSE_EXTI_IRQn EXTI0_IRQn
#define RIGHT_OPEN_Pin GPIO_PIN_1
#define RIGHT_OPEN_GPIO_Port GPIOD
#define RIGHT_OPEN_EXTI_IRQn EXTI1_IRQn
#define LEFT_CLOSE_Pin GPIO_PIN_2
#define LEFT_CLOSE_GPIO_Port GPIOD
#define LEFT_CLOSE_EXTI_IRQn EXTI2_IRQn
#define LEFT_OPEN_Pin GPIO_PIN_3
#define LEFT_OPEN_GPIO_Port GPIOD
#define LEFT_OPEN_EXTI_IRQn EXTI3_IRQn
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
