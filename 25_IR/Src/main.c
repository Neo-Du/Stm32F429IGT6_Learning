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
#define GET_RISING_EDGE 1
#define GET_FALLING_EDGE 0
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
extern TIM_HandleTypeDef htim1;

uint8_t level_flag = GET_RISING_EDGE;
uint8_t state_flag = 0;
uint16_t temp_time = 0;
uint16_t temp1 = 0;
uint16_t temp2 = 0;
uint32_t rec_IR = 0;
uint16_t count_IR = 0;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config (void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void HAL_TIM_IC_CaptureCallback (TIM_HandleTypeDef*htim)
{
    if (htim->Instance == TIM1)
    {
	if (level_flag == GET_RISING_EDGE) // rising edge
	{
	    TIM_RESET_CAPTUREPOLARITY(&htim1, TIM_CHANNEL_1);
	    TIM_SET_CAPTUREPOLARITY(&htim1, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_FALLING);
	    __HAL_TIM_SET_COUNTER(&htim1, 0);
	    level_flag = GET_FALLING_EDGE;
	    temp_time = 0;
	    state_flag |= 0x10;
	}
	else // falling edge
	{
	    temp_time = HAL_TIM_ReadCapturedValue (&htim1, TIM_CHANNEL_1);
	    TIM_RESET_CAPTUREPOLARITY(&htim1, TIM_CHANNEL_1);
	    TIM_SET_CAPTUREPOLARITY(&htim1, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_RISING);
	    level_flag = GET_RISING_EDGE;
	    if (state_flag & 0x10)
	    {
		if (state_flag & 0x80)
		{
		    if (temp_time > 300 && temp_time < 800)
		    {
			rec_IR <<= 1;
			rec_IR |= 0;
		    }
		    else if (temp_time > 1400 && temp_time < 1800)
		    {
			rec_IR <<= 1;
			rec_IR |= 1;
		    }
		    else if (temp_time > 2200 && temp_time < 2600)
		    {
			count_IR++;
			state_flag &= 0XF0;
		    }
		}
		else if (temp_time > 4200 && temp_time < 4700)
		{
		    state_flag |= 1 << 7;
		    count_IR = 0;
		}
	    }
	    state_flag &= ~(1 << 4);
	}
    }
}
void HAL_TIM_PeriodElapsedCallback (TIM_HandleTypeDef*htim)
{
    if (htim->Instance)
    {
	if (state_flag & 0x80)
	{
	    state_flag &= ~0x10;
	    if (state_flag & 0x0f)
	    {
		state_flag |= 1 << 6;
	    }
	    if ((state_flag & 0x0f) < 14)
	    {
		state_flag++;
	    }
	    else
	    {
		state_flag &= ~(1 << 7);
		state_flag &= 0xf0;
	    }
	}
    }
}

uint8_t IR_Scan ()
{
    uint8_t sta = 0;
    uint8_t t1, t2;
    if (state_flag & (1 << 6))
    {
	t1 = rec_IR >> 24;
	t2 = (rec_IR >> 16) & 0xff;
	if ((t1 == (uint8_t) ~t2) && t1 == 0)
	{
	    t1 = rec_IR >> 8;
	    t2 = rec_IR;
	    if (t1 == (uint8_t) ~t2)
		sta = t1;
	}
	if ((sta == 0) || ((state_flag & 0X80) == 0))
	{
	    state_flag &= ~(1 << 6);
	    count_IR = 0;
	}
    }
    return sta;
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
    MX_TIM1_Init ();
    /* USER CODE BEGIN 2 */
    printf ("Start\r\n");
    HAL_Delay (10);
    HAL_TIM_IC_Start_IT (&htim1, TIM_CHANNEL_1);
    __HAL_TIM_ENABLE_IT(&htim1, TIM_IT_UPDATE);

    /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    while (1)
    {
	uint8_t key = IR_Scan();
	if(key)
	{
	    printf("%d\t%d\r\n",key,count_IR);
	}
	HAL_Delay(10);
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
    RCC_OscInitTypeDef RCC_OscInitStruct = { 0 };
    RCC_ClkInitTypeDef RCC_ClkInitStruct = { 0 };

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
    RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
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
