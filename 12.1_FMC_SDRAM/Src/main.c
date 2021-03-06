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
#include "gpio.h"
#include "fmc.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>

/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

#define SDRAM_BANK_ADDR     ((uint32_t)0XC0000000)
#define WRITE_READ_ADDR     ((uint32_t)0x0800)
#define SDRAM_SIZE 	8*1024*1024
//#define SDRAM_BANK_ADDR 0xC0000000
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

uint32_t SDRAM_BUF[SDRAM_SIZE] __attribute__((section(".ExtRAMData2"))); // 1024 * 1024 /4    //1MB / 4

/*
 #if (defined ( __CC_ARM ))
 U32 aMemory[GUI_NUMBYTES / 4] __attribute__((at(MEM_BASE)));
 #elif (defined (__ICCARM__))
 #pragma location = MEM_BASE
 __no_init U32 aMemory[GUI_NUMBYTES / 4];
 #elif defined ( __GNUC__ )
 U32 aMemory[GUI_NUMBYTES / 4] __attribute__((section(".ExtRAMData1")));
 #endif

 __CC_ARM 	-->  U32 aMemory[GUI_NUMBYTES / 4] __attribute__((at(MEM_BASE)));
 __ICCARM__ 	-->  _no_init U32 aMemory[GUI_NUMBYTES / 4];
 __GNUC__ 	-->  aMemory[GUI_NUMBYTES / 4] __attribute__((section(".ExtRAMData1")));

 */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config (void);
void speed_test ();
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
    MX_FMC_Init ();
    /* USER CODE BEGIN 2 */
    SDRAM_Init ();
    printf ("Start\r\n");

    HAL_GPIO_TogglePin (LED0_GPIO_Port, LED0_Pin);
    HAL_Delay (100);
    HAL_GPIO_TogglePin (LED0_GPIO_Port, LED0_Pin);
    HAL_Delay (100);
    HAL_GPIO_TogglePin (LED0_GPIO_Port, LED0_Pin);
    HAL_Delay (100);
    HAL_GPIO_TogglePin (LED0_GPIO_Port, LED0_Pin);
    HAL_Delay (100);
    HAL_GPIO_TogglePin (LED0_GPIO_Port, LED0_Pin);
    HAL_Delay (100);

    speed_test ();

//    uint32_t ts = 0;
//
//    uint32_t i = 0;
//    uint32_t temp = 0;
//    uint32_t sval = 0;
//    aMemory2[1] = 0;
//    for (i = 0; i < 262144; i++)
//    {
//	aMemory[i] = i;
//	aMemory1[i] = i * 2;
//    }
//
//    for (i = 0; i < 262144; i++)
//    {
//
//	temp = aMemory[i];
//	sval = aMemory1[i];
//    }

//    printf("start at %d \r\n",HAL_GetTick());
//    for (i=0;i<8*1024*1024;i++)
//    {
//	*(__IO uint32_t*) (Bank5_SDRAM_ADDR + WRITE_READ_ADDR + i ) = 0;
//    }
//    printf("step1 at %d \r\n",HAL_GetTick());
//    for (i=0;i<8*1024*1024;i+=4)
//    {
//	*(__IO uint32_t*) (Bank5_SDRAM_ADDR + WRITE_READ_ADDR + i ) = temp;
//	temp++;
//    }
//    printf("step2 at %d \r\n",HAL_GetTick());
//    for (i=0;i<8*1024*1024;i+=4)
//    {
//	printf ("read[%d]:%d\r\n", i,*(__IO uint32_t*)(Bank5_SDRAM_ADDR+ WRITE_READ_ADDR+i));
//    }
//    printf("step3 at %d \r\n",HAL_GetTick());
    /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    while (1)
    {

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

void speed_test ()
{
    uint32_t counter = 0;
    uint8_t ubReaddata_8b = 0;
    HAL_GPIO_WritePin (LED0_GPIO_Port, LED0_Pin, 1);
    HAL_Delay (500);
    HAL_GPIO_WritePin (LED0_GPIO_Port, LED0_Pin, 0);
    for (counter = 0x00; counter < SDRAM_SIZE; counter++)
    {
	*(__IO uint8_t*) (SDRAM_BANK_ADDR + counter) = (uint8_t) 0x0;
    }
    HAL_GPIO_WritePin (LED0_GPIO_Port, LED0_Pin, 1);

    HAL_Delay (500);
    HAL_GPIO_WritePin (LED0_GPIO_Port, LED0_Pin, 0);
    for (counter = 0x00; counter < SDRAM_SIZE; counter++)
    {
	 ubReaddata_8b = *(__IO uint8_t *)(SDRAM_BANK_ADDR + counter);
    }
    HAL_GPIO_WritePin (LED0_GPIO_Port, LED0_Pin, 1);

    HAL_Delay (500);
    HAL_GPIO_WritePin (LED0_GPIO_Port, LED0_Pin, 0);
    for (counter = 0x00; counter < SDRAM_SIZE; counter++)
    {
	ubReaddata_8b = 0x00;
    }
    HAL_GPIO_WritePin (LED0_GPIO_Port, LED0_Pin, 1);


//    for (counter = 0x00; counter < IS42S16400J_SIZE; counter++)
//    {
//	aMemory2[counter] = (uint8_t) 0x0;
//    }
//    HAL_GPIO_WritePin (LED0_GPIO_Port, LED0_Pin, 1);
//    HAL_Delay (500);
//    HAL_GPIO_WritePin (LED0_GPIO_Port, LED0_Pin, 0);
//    for (counter = 0x00; counter < IS42S16400J_SIZE; counter++)
//    {
//	aMemory2[counter] = (uint32_t) 1;
//    }
//    HAL_GPIO_WritePin (LED0_GPIO_Port, LED0_Pin, 1);
//
////    for (int32_t i = 0; i < 128000; i++)
////    {
////	test_array[i] = 1;
////    }
//    HAL_Delay (500);
//    HAL_GPIO_WritePin (LED0_GPIO_Port, LED0_Pin, 0);
//    HAL_SDRAM_Write_32b (&hsdram1, aMemory2, test_array, 128000);
//    HAL_GPIO_WritePin (LED0_GPIO_Port, LED0_Pin, 1);
//
//
//    HAL_Delay (500);
//    HAL_GPIO_WritePin (LED0_GPIO_Port, LED0_Pin, 0);
//    HAL_SDRAM_Read_32b (&hsdram1, aMemory2, test_array1, 1024*25);
//    HAL_GPIO_WritePin (LED0_GPIO_Port, LED0_Pin, 1);

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
