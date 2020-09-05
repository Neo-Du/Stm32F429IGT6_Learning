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
#include "dma2d.h"
#include "ltdc.h"
#include "gpio.h"
#include "fmc.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include <stdio.h>
//#include "RGB565_240x130_1.h"
#include "L8_320X240.h"
#include "RGB565_320x240.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

#define SDRAM_BANK_ADDR     ((uint32_t)0XC0000000)

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
extern LTDC_HandleTypeDef hltdc;

uint8_t complate = 0;
//uint32_t aMemory[262144] __attribute__((section(".ExtRAMData")));
//uint32_t aMemory0[1200 * 800] __attribute__((section(".ExtRAMData"))); // 1024 * 1024 /4    //1MB / 4
//uint32_t aMemory1[1200 * 800] __attribute__((section(".ExtRAMData"))); // 1024 * 1024 /4    //1MB / 4

uint32_t aBlendedImage[(LAYER_SIZE_X * LAYER_SIZE_Y * LAYER_BYTE_PER_PIXEL) / 4];

volatile uint8_t ReloadFlag = 0;

//uint32_t aMemory1[262144] __attribute__((section(".ExtRAMData1"))); // 1024 * 1024 /4    //1MB / 4

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
    MX_LTDC_Init ();
    MX_DMA2D_Init ();
    /* USER CODE BEGIN 2 */
    SDRAM_Init ();
    //LCD_Init ();

    //u32 addr = &aMemory0[0];
    u32 timeout = 0;
    uint16_t index = 0;
    /*##-2- CLUT Configuration #################################################*/
    HAL_LTDC_ConfigCLUT (&hltdc, (uint32_t*) L8_320x240_CLUT, 256, 0);

    /*##-3- Enable CLUT For Layer 1 ############################################*/
    HAL_LTDC_EnableCLUT (&hltdc, 0);

    /*##-4- Configure line event ###############################################*/
    HAL_LTDC_ProgramLineEvent (&hltdc, 0);
    while (1)
    {
	for (index = 0; index < 350; index++)
	{
	    HAL_LTDC_SetWindowPosition_NoReload (&hltdc, index * 2, index, 0);
	    HAL_LTDC_SetWindowPosition_NoReload (&hltdc, 700 - index * 2, 350 - index, 1);

	    HAL_LTDC_Reload (&hltdc, LTDC_RELOAD_VERTICAL_BLANKING);

	    HAL_Delay(2);
	}
	HAL_Delay (500);
	for (index = 0; index < 350; index++)
	{
	    HAL_LTDC_SetWindowPosition_NoReload (&hltdc, index * 2, index, 1);
	    HAL_LTDC_SetWindowPosition_NoReload (&hltdc, 700 - index * 2, 350 - index, 0);

	    HAL_LTDC_Reload (&hltdc, LTDC_RELOAD_VERTICAL_BLANKING);
	    HAL_Delay(2);
	}
	HAL_Delay (500);
    }

    //HAL_DMA2D_Start (&hdma2d, (uint32_t) &RGB565_240x130_1, (uint32_t) &aMemory0, 240, 130);
    while (1);

//  __HAL_RCC_DMA2D_CLK_ENABLE();	//使能DM2D时钟
//  DMA2D->CR &= ~(DMA2D_CR_START);	//先停止DMA2D
//  DMA2D->CR = DMA2D_R2M;			//寄存器到存储器模�?????????
//  DMA2D->OPFCCR = 0X02;	//设置颜色格式
//  DMA2D->OOR = 2;				//设置行偏�?????????
//
//  DMA2D->OMAR = addr;				//输出存储器地�?????????
//  DMA2D->NLR = (1023 + 1) | ((599 + 1) << 16);	//设定行数寄存�?????????
//  DMA2D->OCOLR = BLUE;						//设定输出颜色寄存�?????????
//  DMA2D->CR |= DMA2D_CR_START;				//启动DMA2D
//  while ((DMA2D->ISR & (DMA2D_FLAG_TC)) == 0)	//等待传输完成
//  {
//	timeout++;
//	if (timeout > 0X1FFFFF)
//	    break;	//超时�?????????�?????????
//  }

//HAL_DMA2D_Start (&hdma2d, (uint32_t) BLUE, (uint32_t)&aMemory0, 1024, 600);

    /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    while (1)
    {
	printf ("Start %d \r\n", HAL_GetTick ());
	HAL_DMA2D_Start_IT (&hdma2d, 0x43CD80, 0xc0000000, 1024, 600);
	complate = 0;
	while (!complate)
	{
	}
	printf ("finish %d \r\n", HAL_GetTick ());
	HAL_Delay (500);

	HAL_DMA2D_Start_IT (&hdma2d, 0xFF69B4, 0xc0000000, 1024, 600);
	complate = 0;
	while (!complate)
	{
	}
	HAL_Delay (500);
	HAL_DMA2D_Start_IT (&hdma2d, 0xC0FF3E, 0xc0000000, 1024, 600);
	complate = 0;
	while (!complate)
	{
	}
	HAL_Delay (500);
	HAL_DMA2D_Start_IT (&hdma2d, 0x9B30FF, 0xc0000000, 1024, 600);
	complate = 0;
	while (!complate)
	{
	}

	HAL_Delay (500);

//	DMA2D->OMAR = addr;
//	DMA2D->NLR = (1023 + 1) | ((599 + 1) << 16);
//	DMA2D->OCOLR = YELLOW;
//	DMA2D->CR |= DMA2D_CR_START;
//	while ((DMA2D->ISR & (DMA2D_FLAG_TC)) == 0)
//	{
//
//	}
//	HAL_Delay (500);
//	DMA2D->OCOLR = BLUE;						//设定输出颜色寄存�?????????
//	DMA2D->CR |= DMA2D_CR_START;				//启动DMA2D
//	while ((DMA2D->ISR & (DMA2D_FLAG_TC)) == 0)	//等待传输完成
//	{
//
//	}
//	HAL_Delay (500);

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
    RCC_PeriphCLKInitTypeDef PeriphClkInitStruct =
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
    PeriphClkInitStruct.PeriphClockSelection = RCC_PERIPHCLK_LTDC;
    PeriphClkInitStruct.PLLSAI.PLLSAIN = 198;
    PeriphClkInitStruct.PLLSAI.PLLSAIR = 5;
    PeriphClkInitStruct.PLLSAIDivR = RCC_PLLSAIDIVR_2;
    if (HAL_RCCEx_PeriphCLKConfig (&PeriphClkInitStruct) != HAL_OK)
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

void HAL_LTDC_ReloadEventCallback (LTDC_HandleTypeDef*hltdc)
{
    ReloadFlag = 1;
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
