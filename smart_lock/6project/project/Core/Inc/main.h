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
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "stdarg.h"
#include <string.h>
#include <stdint.h>
#include <stdio.h>
#include <stdbool.h>



//extern UART_HandleTypeDef huart3;

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
#define XPT2046_SPI_MOSI_Pin GPIO_PIN_2
#define XPT2046_SPI_MOSI_GPIO_Port GPIOE
#define XPT2046_PENIRQ_Pin GPIO_PIN_4
#define XPT2046_PENIRQ_GPIO_Port GPIOE
#define RFID_SS_Pin GPIO_PIN_4
#define RFID_SS_GPIO_Port GPIOA
#define LCD_G_Pin GPIO_PIN_0
#define LCD_G_GPIO_Port GPIOB
#define LCD_BK_Pin GPIO_PIN_12
#define LCD_BK_GPIO_Port GPIOD
#define XPT2046_SPI_CS_Pin GPIO_PIN_13
#define XPT2046_SPI_CS_GPIO_Port GPIOD
#define LED_Pin GPIO_PIN_5
#define LED_GPIO_Port GPIOB
#define RFID_IRQ_Pin GPIO_PIN_6
#define RFID_IRQ_GPIO_Port GPIOB
#define RFID_Reset_Pin GPIO_PIN_7
#define RFID_Reset_GPIO_Port GPIOB
#define XPT2046_SPI_CLK_Pin GPIO_PIN_0
#define XPT2046_SPI_CLK_GPIO_Port GPIOE
#define LCD_RST_Pin GPIO_PIN_1
#define LCD_RST_GPIO_Port GPIOE
/* USER CODE BEGIN Private defines */
typedef uint8_t u8;
typedef uint16_t u16;
typedef uint32_t u32;


/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
