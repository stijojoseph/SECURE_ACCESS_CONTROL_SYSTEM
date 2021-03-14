/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2021 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under Ultimate Liberty license
  * SLA0044, the "License"; You may not use this file except in compliance with
  * the License. You may obtain a copy of the License at:
  *                             www.st.com/SLA0044
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
#define SSID  "\"stijo\""         //set your wifi name here
#define PASSWORD "\"stijojoseph\"" //set your password for wifi
#define SD_CS_Pin GPIO_PIN_4
#define DOOR_OPEN GPIO_PIN_14
#define DOOR_CLOSE GPIO_PIN_13
#define DOOR_PORT GPIOB
#define SD_CS_GPIO_Port GPIOA
#define NAME_LENGTH 16    //max length of the name (for storing to SRAM on BOOTUP
#define NAME_SIZE 2       //no of names to be stored from SD CARD DATA BASE FILE to SRAM ON BOOTUP**** CAUTION: based on remaining available memory only increase this size****
#define HOUR 23
#define MIN 59
#define SECOND 52
#define DAY 28
#define MONTH RTC_MONTH_FEBRUARY
#define YEAR 21
#define TIMERESET 2  //change this value inorder to reset the date
#define JSON_NUM 2
#define sends   450
#define recieve 350
#define FILE_NAME "test.txt"   //store the database file names on this particular file
#define DATA_SEND_DELAY 20000  // time interval for sending data
/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
