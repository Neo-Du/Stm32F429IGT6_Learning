/* USER CODE BEGIN Header */
/**
 ******************************************************************************
 * @file           : main.c
 * @brief          : Main program body
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; Copyright (c) 2019 STMicroelectronics.
 * All rights reserved.</center></h2>
 *
 * This software component is licensed by ST under BSD 3-Clause license,
 * the "License"; You may not use this file except in compliance with the
 * License. You may obtain a copy of the License at:
 *                        opensource.org/licenses/BSD-3-Clause
 *
 ******************************************************************************
 */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "tim.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
uint32_t data[19] =
  { 0 };
uint32_t data1 = 0;
uint32_t data2 = 0;
volatile uint32_t timer = 0;
volatile uint32_t distance = 0;
volatile uint32_t distance_last = 0;
volatile uint32_t paulse = 0;
volatile int32_t paulse_setting = 0;
uint8_t stepper_state = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config (void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void HAL_TIM_PWM_PulseFinishedCallback (TIM_HandleTypeDef*htim)
{
  if (paulse++ == paulse_setting)
  {
    HAL_TIM_PWM_Stop_IT (&htim8, TIM_CHANNEL_1);
    HAL_GPIO_WritePin (EN_GPIO_Port, EN_Pin, 1);
    stepper_state = 0;
    paulse = 0;
  }
}

void HAL_TIM_PeriodElapsedCallback (TIM_HandleTypeDef*htim)
{
  timer++;
  distance = TIM5->CNT - distance_last;
  distance_last = TIM5->CNT;
}

/* USER CODE END 0 */

/**
 * @brief  The application entry point.
 * @retval int
 */
int main (void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init ();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config ();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init ();
  MX_TIM5_Init ();
  MX_TIM8_Init ();
  MX_TIM6_Init ();
  /* USER CODE BEGIN 2 */
  printf ("Start\r\n");

  HAL_TIM_Encoder_Start_IT (&htim5, TIM_CHANNEL_ALL);

  HAL_TIM_Base_Start_IT (&htim6);

  HAL_GPIO_WritePin (DIR_GPIO_Port, DIR_Pin, 1);
  HAL_GPIO_WritePin (EN_GPIO_Port, EN_Pin, 0);

  __HAL_TIM_SET_AUTORELOAD(&htim8, 2000);
  __HAL_TIM_SET_COMPARE(&htim8, TIM_CHANNEL_1, 1000);

  int i = 0;

  step_set_dir (1);
  step_set_steps (-1);
  step_start ();
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
    for (i = 500; i > 160; i -= 1)
    {
      step_set_speed (i);
      HAL_Delay (10);
    }

    HAL_Delay (1000);
    for (i = 160; i < 500; i += 1)
    {
      step_set_speed (i);
      HAL_Delay (10);
    }
    HAL_Delay (1000);
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
 * @brief System Clock Configuration
 * @retval None
 */
void SystemClock_Config (void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct =
    { 0 };
  RCC_ClkInitTypeDef RCC_ClkInitStruct =
    { 0 };

  /** Configure the main internal regulator output voltage
   */
  __HAL_RCC_PWR_CLK_ENABLE();
  __HAL_PWR_VOLTAGESCALING_CONFIG(PWR_REGULATOR_VOLTAGE_SCALE1);
  /** Initializes the CPU, AHB and APB busses clocks
   */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLM = 15;
  RCC_OscInitStruct.PLL.PLLN = 216;
  RCC_OscInitStruct.PLL.PLLP = RCC_PLLP_DIV2;
  RCC_OscInitStruct.PLL.PLLQ = 4;
  if (HAL_RCC_OscConfig (&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler ();
  }
  /** Activate the Over-Drive mode
   */
  if (HAL_PWREx_EnableOverDrive () != HAL_OK)
  {
    Error_Handler ();
  }
  /** Initializes the CPU, AHB and APB busses clocks
   */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK
      | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig (&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler ();
  }
}

/* USER CODE BEGIN 4 */

int _write (int file,char*ptr,int len)
{
  int DataIdx;
  for (DataIdx = 0; DataIdx < len; DataIdx++)
  {
    ITM_SendChar (*ptr++);
  }
  return len;
}

uint32_t get_encoder_value ()
{
  return TIM5->CNT;
}
uint8_t get_encoder_dir ()
{
  return TIM5->CR1 >> 4;
}

void step_run (uint8_t dir,uint16_t speed,uint32_t steps)
{
  paulse_setting = steps;
  stepper_state = 1;

  __HAL_TIM_SET_AUTORELOAD(&htim8, speed);
  __HAL_TIM_SET_COMPARE(&htim8, TIM_CHANNEL_1, speed / 2);

  HAL_GPIO_WritePin (DIR_GPIO_Port, DIR_Pin, dir);
  HAL_GPIO_WritePin (EN_GPIO_Port, EN_Pin, 0);
  HAL_TIM_PWM_Start_IT (&htim8, TIM_CHANNEL_1);
}

void step_set_dir (uint16_t dir)
{
  HAL_GPIO_WritePin (DIR_GPIO_Port, DIR_Pin, dir);
}

void step_set_speed (uint16_t speed)
{
  __HAL_TIM_SET_AUTORELOAD(&htim8, speed);
  __HAL_TIM_SET_COMPARE(&htim8, TIM_CHANNEL_1, speed / 2);
}

void step_set_steps (uint32_t steps)
{
  paulse_setting = steps;
}

void step_start ()
{
  HAL_TIM_PWM_Start_IT (&htim8, TIM_CHANNEL_1);
}

/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler (void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */

  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{ 
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     tex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
