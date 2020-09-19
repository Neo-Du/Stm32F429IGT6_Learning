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
#include "fatfs.h"
#include "sdio.h"
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
extern FATFS SDFatFS; /* File system object for SD logical drive */
extern FIL SDFile; /* File object for SD */
extern char SDPath[4];
FRESULT res;
uint8_t workBuffer[_MAX_SS];
uint32_t byteswritten, bytesread; /* File write/read counts */
uint8_t wtext[] = "This is STM32 working with FatFs SDIO_4bit"; /* File write buffer */
uint8_t rtext[100]; /* File read buffer */

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
    MX_SDIO_SD_Init ();
    MX_USART1_UART_Init ();
    MX_FATFS_Init ();
    /* USER CODE BEGIN 2 */
    printf ("Start\r\n");
    /*##-2- Register the file system object to the FatFs module ##############*/
    printf ("#2  %d\r\n", HAL_GetTick ());
    if (f_mount (&SDFatFS, (TCHAR const*) SDPath, 0) != FR_OK)
    {
	/* FatFs Initialization Error */
	Error_Handler ();
    }
    else
    {
	/*##-3- Create a FAT file system (format) on the logical drive #########*/
	/* WARNING: Formatting the uSD card will delete all content on the device */
	printf ("#3  %d\r\n", HAL_GetTick ());
	if (f_mkfs ((TCHAR const*) SDPath, FM_ANY, 0, workBuffer, sizeof(workBuffer)) != FR_OK)
	{
	    /* FatFs Format Error */
	    Error_Handler ();
	}
	else
	{
	    /*##-4- Create and Open a new text file object with write access #####*/
	    printf ("#4  %d\r\n", HAL_GetTick ());
	    if (f_open (&SDFile, "STM32.TXT", FA_CREATE_ALWAYS | FA_WRITE) != FR_OK)
	    {
		/* 'STM32.TXT' file Open for write Error */
		Error_Handler ();
	    }
	    else
	    {
		/*##-5- Write data to the text file ################################*/
		printf ("#5  %d\r\n", HAL_GetTick ());
		res = f_write (&SDFile, wtext, sizeof(wtext), (void*) &byteswritten);

		if ((byteswritten == 0) || (res != FR_OK))
		{
		    /* 'STM32.TXT' file Write or EOF Error */
		    Error_Handler ();
		}
		else
		{
		    /*##-6- Close the open text file #################################*/
		    printf ("#6  %d\r\n", HAL_GetTick ());
		    f_close (&SDFile);

		    /*##-7- Open the text file object with read access ###############*/
		    printf ("#7  %d\r\n", HAL_GetTick ());
		    if (f_open (&SDFile, "STM32.TXT", FA_READ) != FR_OK)
		    {
			/* 'STM32.TXT' file Open for read Error */
			Error_Handler ();
		    }
		    else
		    {
			/*##-8- Read data from the text file ###########################*/
			printf ("#8  %d\r\n", HAL_GetTick ());
			res = f_read (&SDFile, rtext, sizeof(rtext), (UINT*) &bytesread);

			if ((bytesread == 0) || (res != FR_OK))
			{
			    /* 'STM32.TXT' file Read or EOF Error */
			    Error_Handler ();
			}
			else
			{
			    /*##-9- Close the open text file #############################*/
			    printf ("#9  %d\r\n", HAL_GetTick ());
			    f_close (&SDFile);

			    /*##-10- Compare read data with the expected data ############*/
			    printf ("#10  %d\r\n", HAL_GetTick ());
			    if ((bytesread != byteswritten))
			    {
				/* Read data is different from the expected data */
				Error_Handler ();
			    }
			    else
			    {
				/* Success of the demo: no error occurrence */
				printf ("Success\r\n");
			    }
			}
		    }
		}
	    }
	}
    }

    /*##-11- Unlink the micro SD disk I/O driver ###############################*/
    FATFS_UnLinkDriver (SDPath);

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
    RCC_OscInitStruct.PLL.PLLQ = 8;
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

//int _write(int file, char *ptr, int len) {
//	int DataIdx;
//	for (DataIdx = 0; DataIdx < len; DataIdx++) {
//		ITM_SendChar(*ptr++);
//	}
//	return len;
//}
/* USER CODE END 4 */

/**
 * @brief  This function is executed in case of error occurrence.
 * @retval None
 */
void Error_Handler (void)
{
    /* USER CODE BEGIN Error_Handler_Debug */
    /* User can add his own implementation to report the HAL error return state */
    printf ("Wrong!!!\r\n");
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
