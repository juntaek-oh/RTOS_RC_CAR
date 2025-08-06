/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * File Name          : freertos.c
  * Description        : Code for freertos applications
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

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"
#include "stdbool.h"
#include "usart.h"
#include "string.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "ultrasonic.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
extern bool isAutoMode;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/
/* USER CODE BEGIN Variables */

/* USER CODE END Variables */
/* Definitions for Ultrasonic */
osThreadId_t UltrasonicHandle;
const osThreadAttr_t Ultrasonic_attributes = {
  .name = "Ultrasonic",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for Moving */
osThreadId_t MovingHandle;
const osThreadAttr_t Moving_attributes = {
  .name = "Moving",
  .stack_size = 128 * 4,
  .priority = (osPriority_t) osPriorityLow,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */

/* USER CODE END FunctionPrototypes */

void Ultrasonic_01(void *argument);
void Moving_01(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* add mutexes, ... */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* add semaphores, ... */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* start timers, add new ones, ... */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* add queues, ... */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of Ultrasonic */
  UltrasonicHandle = osThreadNew(Ultrasonic_01, NULL, &Ultrasonic_attributes);

  /* creation of Moving */
  MovingHandle = osThreadNew(Moving_01, NULL, &Moving_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* add threads, ... */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* add events, ... */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_Ultrasonic_01 */
/**
  * @brief  Function implementing the Ultrasonic thread.
  * @param  argument: Not used
  * @retval None
  */
/* USER CODE END Header_Ultrasonic_01 */
void Ultrasonic_01(void *argument)
{
  /* USER CODE BEGIN Ultrasonic_01 */
  /* Infinite loop */
  for(;;)
  {
      HCSR04_Trigger1();
      osDelay(10);

      HCSR04_Trigger2();

      osDelay(10);
      HCSR04_Trigger3();


    osDelay(10);
  }
  /* USER CODE END Ultrasonic_01 */
}

/* USER CODE BEGIN Header_Moving_01 */
/**
* @brief Function implementing the Moving thread.
* @param argument: Not used
* @retval None
*/
/* USER CODE END Header_Moving_01 */
void Moving_01(void *argument)
{
  /* USER CODE BEGIN Moving_01 */

  uint8_t receivedByte;
  void sendData(const char* data)
  {
      HAL_UART_Transmit(&huart6, (uint8_t*)data, strlen(data), 100);
  }
  /* Infinite loop */
  for (;;)
      {
//      static TickType_t lastInputTime = 0; // 마지막 입력 시간 저장
//          const TickType_t timeout = pdMS_TO_TICKS(5000); // 5초 타임아웃

      if (HAL_UART_Receive(&huart6, &receivedByte, sizeof(receivedByte), 10) == HAL_OK)
              {
//          lastInputTime = xTaskGetTickCount();

                  // Skip newline or carriage return
                  if (receivedByte == '\n' || receivedByte == '\r')
                  {
                      continue; // Skip to next iteration, don't exit
                  }

                  // Echo received character
                  char tmp[2] = {receivedByte, 0};
                  sendData(tmp);
                  sendData("\r\n");

                  // Process command
                  if (receivedByte == 'Z')
                  {
                      isAutoMode = true;
                      sendData("Auto mode enabled\r\n");
                  }
                  else
                  {
                      isAutoMode = false;
                      dir(receivedByte); // Manual command
                  }
              }
//
//      if (!isAutoMode && (xTaskGetTickCount() - lastInputTime >= timeout))
//          {
//              isAutoMode = true;
//              sendData("No input for 5 seconds, Auto mode enabled\r\n");
//          }
              // Run autonomous mode if enabled
              if (isAutoMode)
              {
                  moving();
              }

              // Small delay to prevent CPU hogging
              osDelay(90);
          }
      }

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */

/* USER CODE END Application */

