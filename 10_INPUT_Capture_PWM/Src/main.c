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
#define TIM_CLOCKS 90000000
/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
volatile uint32_t IC_Value_1 = 0;
volatile uint32_t IC_Value_2 = 0;
uint32_t Difference = 0;



volatile uint32_t PWM_IC_Rising_Value_1 = 0;
volatile uint32_t PWM_IC_Rising_Value_2 = 0;
volatile uint32_t PWM_IC_Rising_Value_Diff = 0;

volatile uint32_t PWM_IC_falling_Value_1 = 0;
volatile uint32_t PWM_IC_falling_Value_2 = 0;
volatile uint32_t PWM_IC_falling_Value_Diff = 0;

volatile float duty_cycle = 0.0;
volatile uint32_t Frenquency = 0;
uint8_t Is_duty_cycle = 0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
//	if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
//	{
//		PWM_IC_falling_Value_2 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
//
//
//		if(PWM_IC_falling_Value_2 > PWM_IC_falling_Value_1)
//		{
//			PWM_IC_falling_Value_Diff = PWM_IC_falling_Value_2 - PWM_IC_falling_Value_1;
//		}
//		else if(PWM_IC_falling_Value_2 < PWM_IC_falling_Value_1)
//		{
//			PWM_IC_falling_Value_Diff = (0xffff - PWM_IC_falling_Value_1 ) + PWM_IC_falling_Value_2 + 1;
//		}
//		else
//		{
//			Error_Handler();
//		}
//
//		PWM_IC_falling_Value_1 = PWM_IC_falling_Value_2;
//
//		Frenquency = TIM_CLOCKS / PWM_IC_falling_Value_Diff;
//
//		PWM_IC_Rising_Value_1 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_2);
//	}
//	else if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2)
//	{
//		PWM_IC_Rising_Value_2 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_2);
//		if(PWM_IC_Rising_Value_2 > PWM_IC_Rising_Value_1)
//		{
//			PWM_IC_Rising_Value_Diff = PWM_IC_Rising_Value_2 - PWM_IC_Rising_Value_1;
//		}
//		else if(PWM_IC_Rising_Value_2 < PWM_IC_Rising_Value_1)
//		{
//			PWM_IC_Rising_Value_Diff = (0xffff - PWM_IC_Rising_Value_1 ) + PWM_IC_Rising_Value_2 + 1;
//		}
//		else
//		{
//			Error_Handler();
//		}
//		duty_cycle = PWM_IC_Rising_Value_Diff * 100.0 /  PWM_IC_falling_Value_Diff  ;
//	}
//	PWM_IC_Rising_Value_1 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
//	PWM_IC_Rising_Value_2 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_2);
//	PWM_IC_Rising_Value_Diff = PWM_IC_Rising_Value_1 - PWM_IC_Rising_Value_2;


if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_1)
	IC_Value_1 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);
if(htim->Channel == HAL_TIM_ACTIVE_CHANNEL_2)
	IC_Value_2 = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_2);

}

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */

  /* USER CODE END 1 */
  

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */

  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_TIM4_Init();
  /* USER CODE BEGIN 2 */
  printf("Start\r\n");

  HAL_TIM_IC_Start_IT(&htim4,TIM_CHANNEL_1);
  HAL_TIM_IC_Start_IT(&htim4,TIM_CHANNEL_2);
  /* USER CODE END 2 */

  /* Infinite loop */
  while(1)
  {
	if (IC_Value_1 == 0)
	{
		duty_cycle = 0;
		Frenquency = 0;
	}
	else
	{
		duty_cycle = IC_Value_2 * 100 / IC_Value_1;
		Frenquency = 90000000 / IC_Value_1;
	}
  /* USER CODE BEGIN WHILE */
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */

  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};

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
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }
  /** Activate the Over-Drive mode 
  */
  if (HAL_PWREx_EnableOverDrive() != HAL_OK)
  {
    Error_Handler();
  }
  /** Initializes the CPU, AHB and APB busses clocks 
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV4;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV2;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_5) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */


int _write(int file, char *ptr, int len) {
	int DataIdx;
	for (DataIdx = 0; DataIdx < len; DataIdx++) {
		ITM_SendChar(*ptr++);
	}
	return len;
}

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
	while(1);
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
