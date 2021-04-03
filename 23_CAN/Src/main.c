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
#include "can.h"
#include "usart.h"
#include "gpio.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
#ifdef __GNUC__
#define PUTCHAR_PROTOTYPE int __io_putchar(int ch)
#else
#define PUTCHAR_PROTOTYPE int fputc(int ch, FILE *f)
#endif
PUTCHAR_PROTOTYPE
{
    while ((USART1->SR & 0X40) == 0);
    USART1->DR = (uint8_t) ch;
    return ch;
}
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
extern CAN_HandleTypeDef hcan1;
CAN_FilterTypeDef sFilterConfig;
CAN_TxHeaderTypeDef TxHeader;
CAN_RxHeaderTypeDef RxHeader;
uint8_t TxData[8];
uint8_t RxData[8];
uint32_t TxMailbox;
int temp,x,y;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config (void);
/* USER CODE BEGIN PFP */
HAL_StatusTypeDef CAN_TX (CAN_HandleTypeDef*hcan,uint8_t*txBuf,uint8_t len,uint8_t StdID);
HAL_StatusTypeDef CAN_RX (CAN_HandleTypeDef*hcan,uint8_t*rxbuf,uint8_t*len);
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
    uint8_t lens = 0;
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
    MX_USART1_UART_Init ();
    MX_CAN1_Init ();
    /* USER CODE BEGIN 2 */
    printf ("Start\r\n");

    sFilterConfig.FilterBank = 0;
    sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;
    sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;
    sFilterConfig.FilterIdHigh = 0x0000;
    sFilterConfig.FilterIdLow = 0x0000;
    sFilterConfig.FilterMaskIdHigh = 0x0000;
    sFilterConfig.FilterMaskIdLow = 0x0000;
    sFilterConfig.FilterFIFOAssignment = CAN_RX_FIFO0;
    sFilterConfig.FilterActivation = ENABLE;
    sFilterConfig.SlaveStartFilterBank = 14;

    if (HAL_CAN_ConfigFilter (&hcan1, &sFilterConfig) != HAL_OK)
    {
	/* Filter configuration Error */
	printf ("config error\r\n");
    }

    /*##-3- Start the CAN peripheral ###########################################*/
    if (HAL_CAN_Start (&hcan1) != HAL_OK)
    {
	/* Start Error */
	printf ("start error\r\n");
    }


    TxData[1] = 0x22;
    TxData[2] = 0x33;
    TxData[3] = 0x44;
    TxData[4] = 0x55;
    TxData[5] = 0x66;
    TxData[6] = 0x77;
    TxData[7] = 0x88;

    /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    x = 0;
    y = 0;
    while (1)
    {
	if (x++ == 200)
	    x = 0;
	for (y = 0; y < 8; y++)
	{
	    TxData[y] = x+y;
	}

	CAN_TX (&hcan1, TxData, 8, 0x18);
	CAN_RX (&hcan1, RxData, &lens);
	printf ("send done\r\n");
	for (temp = 0; temp < 8; temp++)
	{
	    printf ("0X%x\t ", RxData[temp]);
	}
	printf ("\r\nreceive done\r\n");
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
HAL_StatusTypeDef CAN_TX (CAN_HandleTypeDef*hcan,uint8_t*txbuf,uint8_t len,uint8_t StdID)
{
    CAN_TxHeaderTypeDef txHeader;
    uint32_t txmailbox;

    txHeader.StdId = StdID;
    txHeader.RTR = CAN_RTR_DATA;
    txHeader.IDE = CAN_ID_STD;
    txHeader.DLC = len;
    TxHeader.TransmitGlobalTime = DISABLE;

    if (HAL_CAN_AddTxMessage (hcan, &txHeader, txbuf, &txmailbox) != HAL_OK)
    {
	printf ("add error\r\n");
	return HAL_ERROR;
    }

    while (HAL_CAN_GetTxMailboxesFreeLevel (&hcan1) != 3)
    {
    }
    return HAL_OK;
}

HAL_StatusTypeDef CAN_RX (CAN_HandleTypeDef*hcan,uint8_t*rxbuf,uint8_t*len)
{
    CAN_TxHeaderTypeDef rxHeader;

    if (HAL_CAN_GetRxFifoFillLevel (hcan, CAN_RX_FIFO0) != 1)
    {
	/* Reception Missing */
	printf ("rx error\r\n");
    }

    if (HAL_CAN_GetRxMessage (hcan, CAN_RX_FIFO0, &rxHeader, rxbuf) != HAL_OK)
    {
	/* Reception Error */
	printf ("get rx error\r\n");
    }
    *len = rxHeader.DLC;
    return HAL_OK;
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
