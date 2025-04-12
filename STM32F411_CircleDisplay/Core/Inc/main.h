/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2025 STMicroelectronics.
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
#include <stdio.h>
#include <string.h>
#include "SD_Card.h"
#include "LCD_1inch28_test.h"

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
#define BLED_Pin GPIO_PIN_1
#define BLED_GPIO_Port GPIOC
#define TP_RST_Pin GPIO_PIN_4
#define TP_RST_GPIO_Port GPIOA
#define SPI1_CS_Pin GPIO_PIN_4
#define SPI1_CS_GPIO_Port GPIOC
#define SDIO_CD_Pin GPIO_PIN_5
#define SDIO_CD_GPIO_Port GPIOC
#define LCS_DC_Pin GPIO_PIN_0
#define LCS_DC_GPIO_Port GPIOB
#define LCS_RST_Pin GPIO_PIN_1
#define LCS_RST_GPIO_Port GPIOB
#define TP_INT_Pin GPIO_PIN_10
#define TP_INT_GPIO_Port GPIOB
#define TP_INT_EXTI_IRQn EXTI15_10_IRQn
#define TP_SDA_Pin GPIO_PIN_7
#define TP_SDA_GPIO_Port GPIOB
#define TP_SCL_Pin GPIO_PIN_8
#define TP_SCL_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */
void SD_mount_Fats(void);
/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
