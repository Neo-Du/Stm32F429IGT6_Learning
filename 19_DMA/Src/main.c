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
#include "dma.h"
#include "usart.h"
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
#define SEND_BUF_SIZE 140000

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
uint8_t sendMsg[] = { "12345678900\r\n" }; // size = 14
uint8_t sendBuf[SEND_BUF_SIZE] = { }; // size = 14000
int reming = 0;
int flag = 0;
int dma_flag = 0;
int busy_flag = 0;

uint32_t start_time = 0;
volatile uint32_t current_Time = 0;
volatile cpt_flag = 0;
uint32_t DMA_time = 0;
uint32_t IT_time = 0;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config (void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

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
    MX_DMA_Init ();
    MX_USART1_UART_Init ();
    /* USER CODE BEGIN 2 */
    printf ("Start\r\n");

    int t = SEND_BUF_SIZE / sizeof(sendMsg);
    int k = 0;
    for (int i = 0; i < t; i++)
    {
	for (int j = 0; j < 14; j++)
	{
	    sendBuf[k++] = sendMsg[j];
	}
    }
    start_time = HAL_GetTick ();
    HAL_UART_Transmit_DMA (&huart1, sendBuf, SEND_BUF_SIZE);

    //HAL_DMA_Start(huart1.hdmatx, (uint32_t)sendBuf, (uint32_t)&huart1.Instance->DR, SEND_BUF_SIZE);
    //huart1.Instance->CR3 |= USART_CR3_DMAT;

//    reming = __HAL_DMA_GET_COUNTER(&hdma_usart1_tx);
//    while (reming)
//    {
//	reming = __HAL_DMA_GET_COUNTER(&hdma_usart1_tx);
//	dma_flag = __HAL_DMA_GET_FLAG(&hdma_usart1_tx, DMA_FLAG_TCIF3_7);
//    }


    while(cpt_flag == 0);
    DMA_time = current_Time - start_time;
    cpt_flag = 0;

    start_time = HAL_GetTick ();
    HAL_UART_Transmit_IT (&huart1, sendBuf, SEND_BUF_SIZE);
//    while(HAL_UART_GetState(&huart1) == HAL_UART_STATE_BUSY);
    while(cpt_flag == 0);
    IT_time = current_Time - start_time;
    /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    while (1)
    {

//	if(dma_flag)
//	{
//	    flag = 5000;
//	}else if(reming  == 0)
//	{
//	    flag = 2500;
//	}
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

void HAL_UART_TxCpltCallback (UART_HandleTypeDef*huart)
{
    if (huart->Instance == huart1.Instance)
    {
	//HAL_UART_DMAStop(&huart1);
	current_Time = HAL_GetTick ();
	cpt_flag = 1;
    }
}
void HAL_UART_TxHalfCpltCallback (UART_HandleTypeDef*huart)
{
    if (huart->Instance == huart1.Instance)
    {
    }
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
