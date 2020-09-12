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
#include "dcmi.h"
#include "dma.h"
#include "dma2d.h"
#include "i2c.h"
#include "ltdc.h"
#include "gpio.h"
#include "fmc.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "LTDC_LCD.h"
#include "sdRAM.h"
#include "OV5640.h"
//#include "RGB565_240x130_1.h"
//#include "RGB565_320x240.h"
//#include"test_pic3.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define SDRAM_BANK_ADDR     ((uint32_t)0XC0000000)
#define jpeg_buf_size   30*1024*1024		//定义JPEG数据缓存jpeg_buf的大�???????????(1*4M字节)
#define jpeg_line_size	2*1024			//定义DMA接收数据�???????????,�???????????行数据的�???????????大�??

typedef int32_t s32;
typedef int16_t s16;
typedef int8_t s8;

typedef const int32_t sc32;
typedef const int16_t sc16;
typedef const int8_t sc8;

typedef __IO int32_t vs32;
typedef __IO int16_t vs16;
typedef __IO int8_t vs8;

typedef __I int32_t vsc32;
typedef __I int16_t vsc16;
typedef __I int8_t vsc8;

typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t u8;

typedef const uint32_t uc32;
typedef const uint16_t uc16;
typedef const uint8_t uc8;

typedef __IO uint32_t vu32;
typedef __IO uint16_t vu16;
typedef __IO uint8_t vu8;

typedef __I uint32_t vuc32;
typedef __I uint16_t vuc16;
typedef __I uint8_t vuc8;

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
uint32_t aMemory0[1200 * 800] __attribute__((section(".ExtRAMData"))); // 1024 * 1024 /4    //1MB / 4

//uint32_t dcmi_line_buf[2][jpeg_line_size];	//RGB屏时,摄像头采用一行一行读�???????????,定义行缓�???????????

//uint32_t line_buf[1024 * 4];

uint32_t pic_buf[2][160 * 120];

extern DCMI_HandleTypeDef hdcmi;
extern DMA_HandleTypeDef hdma_dcmi;

typedef struct
{
    vu16 LCD_REG;
    vu16 LCD_RAM;
} LCD_TypeDef;

#define LCD_BASE        ((u32)(0x60000000 | 0x0007FFFE))
#define LCD             ((LCD_TypeDef *) LCD_BASE)

u16 curline = 0;							//摄像头输出数�????????,当前行编�????????
u16 yoffset = 0;							//y方向的偏移量

volatile uint32_t temp2 = 0;
volatile uint32_t line_callback = 0;
volatile uint32_t error_callback = 0;
volatile uint32_t frame_callback = 0;
volatile uint32_t vsync_callback = 0;
volatile uint32_t dma_callback = 0;

volatile uint32_t m1 = 0;
volatile uint32_t m2 = 0;

uint32_t vsync_call = 0;
uint32_t hsync_call = 0;
uint32_t line_call = 0;
uint32_t temp_check = 0;

uint32_t t1, t2, t3;
uint32_t i, j;
/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config (void);
static void MX_NVIC_Init (void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

void HAL_DCMI_FrameEventCallback (DCMI_HandleTypeDef*hdcmi)
{
//    jpeg_data_process (); 						//jpeg数据处理
//    temp2++;
//    //重新使能帧中�????????,因为HAL_DCMI_IRQHandler()函数会关闭帧中断

    frame_callback = 1;
    __HAL_DCMI_ENABLE_IT(hdcmi, DCMI_IT_FRAME);
}

void HAL_DCMI_LineEventCallback (DCMI_HandleTypeDef*hdcmi)
{
    line_callback = 1;
}
void HAL_DCMI_ErrorCallback (DCMI_HandleTypeDef*hdcmi)
{
    error_callback++;
}

void HAL_DCMI_VsyncEventCallback (DCMI_HandleTypeDef*hdcmi)
{
    HAL_DMA2D_Abort (&hdma2d);
    if ((hdcmi->DMA_Handle->Instance->CR & DMA_SxCR_CT) == 0U)
    {
	m1++;
	HAL_DMA2D_Start (&hdma2d, (uint32_t) &pic_buf[1], (uint32_t) &aMemory0, 160, 120);
    }
    else
    {
	m2++;
	HAL_DMA2D_Start (&hdma2d, (uint32_t) &pic_buf[0], (uint32_t) &aMemory0, 160, 120);
    }
}
void dcmi_rx_callback ()
{
    dma_callback++;
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
    MX_DMA_Init ();
    MX_DCMI_Init ();
    MX_FMC_Init ();
    MX_LTDC_Init ();
    MX_DMA2D_Init ();
    MX_I2C2_Init ();

    /* Initialize interrupts */
    MX_NVIC_Init ();
    /* USER CODE BEGIN 2 */

    //LCD_Init ();
    SDRAM_Init ();

    HAL_GPIO_WritePin (LTDC_BL_GPIO_Port, LTDC_BL_Pin, 1);

    for (int i = 0; i < 19200; i++)
    {
	pic_buf[1][i] = 0xFF0000;
    }
    for (int j = 0; j < 19200; j++)
    {
	pic_buf[0][j] = 0x43CD80;
    }
//
//    while (1)
//    {
//	HAL_DMA2D_Abort(&hdma2d);
//	HAL_DMA2D_Start (&hdma2d, (uint32_t) &pic_buf[1], (uint32_t) &aMemory0, 160, 120);
//	HAL_Delay (500);
//	HAL_DMA2D_Abort(&hdma2d);
//	HAL_DMA2D_Start (&hdma2d, (uint32_t) &pic_buf[0], (uint32_t) &aMemory0, 160, 120);
//	HAL_Delay (500);
//    }

    OV5640_Init ();

    OV5640_RGB565_Mode ();
//    OV5640_Focus_Init ();
//    OV5640_Light_Mode (0);	//自动模式
//    OV5640_Color_Saturation (3);	//色彩饱和�???????????0
//    OV5640_Brightness (4);	//亮度0
//    OV5640_Contrast (3);		//对比�???????????0
//    OV5640_Sharpness (33);	//自动锐度
//    OV5640_Focus_Constant ();	//启动持续对焦
    OV5640_OutSize_Set (4, 0, 160, 120);

//    __HAL_UNLOCK(&hdma_dcmi);
//
//    HAL_DMAEx_MultiBufferStart (hdcmi.DMA_Handle, (uint32_t) &hdcmi.Instance->DR, &pic_buf[0], &pic_buf[1], 160 * 120);		//

    __HAL_LOCK(&hdcmi);
    hdcmi.State = HAL_DCMI_STATE_BUSY;
    __HAL_DCMI_ENABLE(&hdcmi);
    hdcmi.Instance->CR &= ~(DCMI_CR_CM);
    hdcmi.Instance->CR |= (uint32_t) (DCMI_MODE_CONTINUOUS);

    HAL_DMAEx_MultiBufferStart (hdcmi.DMA_Handle, (uint32_t) &hdcmi.Instance->DR, &pic_buf[0], &pic_buf[1], 160 * 120);		//
    hdcmi.Instance->CR |= DCMI_CR_CAPTURE;
    __HAL_UNLOCK(&hdcmi);

    while (1);
//
//    __HAL_DMA_ENABLE(&hdcmi); //使能DMA
//
//    hdcmi.Instance->CR &= 0xFFFE;
//    hdcmi.Instance->CR |= DCMI_CR_ENABLE;

//    HAL_DCMI_Start_DMA (&hdcmi, DCMI_MODE_CONTINUOUS, 0xC0000000, 160 * 120);

//    HAL_DMA2D_Start (&hdma2d, (uint32_t) &line_buf, (uint32_t) &aMemory0, 1024, 4);
    HAL_Delay (500);
    __HAL_DMA_ENABLE(&hdcmi);

    while (1)
    {
	HAL_DCMI_Start_DMA (&hdcmi, DCMI_MODE_SNAPSHOT, &pic_buf[0], 160 * 120);
	HAL_Delay (10);
	HAL_DMA2D_Abort (&hdma2d);
	HAL_DMA2D_Start (&hdma2d, (uint32_t) &pic_buf[0], (uint32_t) &aMemory0, 160, 120);
	HAL_Delay (10);
    }

    while (1)
    {
	if (vsync_callback)
	{
	    vsync_callback = 0;
	    vsync_call++;
	    temp_check = line_call;
	}
	if (frame_callback)
	{
	    frame_callback = 0;
	    hsync_call++;
	}
	if (line_callback)
	{
	    line_callback = 0;
	    line_call++;
	}
    }
//    dcmi_rx_callback = rgblcd_dcmi_rx_callback;          //RGB屏接收数据回调函�???????????
//
//    __HAL_UNLOCK(&hdma_dcmi);
//
//    __HAL_DMA_ENABLE(&hdma_dcmi); //使能DMA
//    DCMI->CR |= DCMI_CR_CAPTURE;          //DCMI捕获使能
//    DCMI->CR |= 1 << 14;
//HAL_DMAEx_MultiBufferStart (&hdma_dcmi, (u32) &DCMI->DR, (uint32_t) dcmi_line_buf[0], (uint32_t) dcmi_line_buf[1], 300);          //�??????????启双缓冲
//    HAL_DCMI_Start_DMA (&hdcmi, DCMI_MODE_CONTINUOUS, aMemory0, 1000 * 600);
//__HAL_DCMI_ENABLE_IT(&hdcmi, DCMI_IT_FRAME);

//    __HAL_DMA_ENABLE_IT(&hdma_dcmi, DMA_IT_TC);    //�??????????启传输完成中�??????????
//    HAL_NVIC_SetPriority (DMA2_Stream1_IRQn, 0, 0);        //DMA中断优先�??????????
//    HAL_NVIC_EnableIRQ (DMA2_Stream1_IRQn);
//
//    OV5640_OutSize_Set (4, 0, 600, 1024);		//满屏缩放显示
//
//    LCD_SetCursor (0, 0);
//    LCD_WriteRAM_Prepare ();		        //�??????????始写入GRAM

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
    RCC_PeriphCLKInitTypeDef PeriphClkInitStruct = { 0 };

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
    PeriphClkInitStruct.PLLSAI.PLLSAIN = 60;
    PeriphClkInitStruct.PLLSAI.PLLSAIR = 2;
    PeriphClkInitStruct.PLLSAIDivR = RCC_PLLSAIDIVR_2;
    if (HAL_RCCEx_PeriphCLKConfig (&PeriphClkInitStruct) != HAL_OK)
    {
	Error_Handler ();
    }
}

/**
 * @brief NVIC Configuration.
 * @retval None
 */
static void MX_NVIC_Init (void)
{
    /* DMA2_Stream1_IRQn interrupt configuration */
    HAL_NVIC_SetPriority (DMA2_Stream1_IRQn, 0, 0);
    HAL_NVIC_EnableIRQ (DMA2_Stream1_IRQn);
}

/* USER CODE BEGIN 4 */

//DCMI中断服务函数
//void DCMI_IRQHandler (void)
//{
//    HAL_DCMI_IRQHandler (&hdma_dcmi);
//}
//DMA2数据�???????????1中断服务函数
//void DMA2_Stream1_IRQHandler (void)
//{
//    if (__HAL_DMA_GET_FLAG(&hdma_dcmi,DMA_FLAG_TCIF1_5) != RESET)          //DMA传输完成
//    {
//	__HAL_DMA_CLEAR_FLAG(&hdma_dcmi, DMA_FLAG_TCIF1_5);          //清除DMA传输完成中断标志�???????????
//	dcmi_rx_callback ();	//执行摄像头接收回调函�???????????,读取数据等操作在这里面处�???????????
//    }
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
