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
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
#define SDRAM_BANK_ADDR     ((uint32_t)0XC0000000)
#define jpeg_buf_size   30*1024*1024		//定义JPEG数据缓存jpeg_buf的大小(1*4M字节)
#define jpeg_line_size	2*1024			//定义DMA接收数据时,一行数据的最大值

#define u8 uint8_t
#define u16 uint16_t
#define u32 uint32_t

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */
uint32_t aMemory0[1200 * 800] __attribute__((section(".ExtRAMData"))); // 1024 * 1024 /4    //1MB / 4

uint32_t dcmi_line_buf[2][jpeg_line_size];	//RGB屏时,摄像头采用一行一行读取,定义行缓存

uint32_t *ltdc_framebuf[2];					//LTDC LCD帧缓存数组指针,必须指向对应大小的内存区域
u16 curline = 0;							//摄像头输出数据,当前行编号
extern DCMI_HandleTypeDef hdcmi;
extern DMA_HandleTypeDef hdma_dcmi;

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config (void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */
void (*dcmi_rx_callback) (void);          //DCMI DMA接收回调函数

void LTDC_Color_Fill (u16 sx,u16 sy,u16 ex,u16 ey,u16*color)
{
    u32 psx, psy, pex, pey;	//以LCD面板为基准的坐标系,不随横竖屏变化而变化
    u32 timeout = 0;
    u16 offline;
    u32 addr;
    //坐标系转换
    if (1)	//横屏
    {
	psx = sx;
	psy = sy;
	pex = ex;
	pey = ey;
    }
    else			//竖屏
    {
	psx = sy;
	psy = 1024 - ex - 1;
	pex = ey;
	pey = 1024 - sx - 1;
    }
    offline = 600 - (pex - psx + 1);
    addr = ((u32) ltdc_framebuf[0] + 2 * (1024 * psy + psx));
    __HAL_RCC_DMA2D_CLK_ENABLE();	//使能DM2D时钟
    DMA2D->CR &= ~(DMA2D_CR_START);	//先停止DMA2D
    DMA2D->CR = DMA2D_M2M;			//存储器到存储器模式
    DMA2D->FGPFCCR = 0X02;	//设置颜色格式
    DMA2D->FGOR = 0;					//前景层行偏移为0
    DMA2D->OOR = offline;				//设置行偏移

    DMA2D->FGMAR = (u32) color;		//源地址
    DMA2D->OMAR = addr;				//输出存储器地址
    DMA2D->NLR = (pey - psy + 1) | ((pex - psx + 1) << 16);	//设定行数寄存器
    DMA2D->CR |= DMA2D_CR_START;					//启动DMA2D
    while ((DMA2D->ISR & (DMA2D_FLAG_TC)) == 0)		//等待传输完成
    {
	timeout++;
	if (timeout > 0X1FFFFF)
	    break;	//超时退出
    }
    DMA2D->IFCR |= DMA2D_FLAG_TC;				//清除传输完成标志
}

void rgblcd_dcmi_rx_callback (void)
{
    uint16_t *pbuf;
    if (hdma_dcmi.Instance->CR & (1 << 19))	//DMA使用buf1,读取buf0
    {
	pbuf = (uint16_t*) dcmi_line_buf[0];
    }
    else 						//DMA使用buf0,读取buf1
    {
	pbuf = (uint16_t*) dcmi_line_buf[1];
    }

    LTDC_Color_Fill (0, curline, 600 - 1, curline, pbuf); 						//DM2D填充

    if (curline < 1024)
	curline++;
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
    /* USER CODE BEGIN 2 */
    LCD_Init ();
    SDRAM_Init ();

    HAL_GPIO_WritePin (LTDC_BL_GPIO_Port, LTDC_BL_Pin, 1);

    OV5640_Init ();
    OV5640_Focus_Init ();
    OV5640_Focus_Constant (); //启动持续对焦

    HAL_DMA_Start (&hdma_dcmi, (uint32_t) & DCMI->DR, dcmi_line_buf[0], 300);

    OV5640_WR_Reg (0x3035, 0X51); //降低输出帧率，否则可能抖动
//    OV5640_OutSize_Set(4,0,lcddev.width,outputheight);		//满屏缩放显示

    //LCD_WriteRAM_Prepare ();		        //开始写入GRAM
    __HAL_DMA_ENABLE(&hdcmi); //使能DMA
    DCMI->CR |= DCMI_CR_CAPTURE;          //DCMI捕获使能

    dcmi_rx_callback = rgblcd_dcmi_rx_callback;          //RGB屏接收数据回调函数

    /* USER CODE END 2 */

    /* Infinite loop */
    /* USER CODE BEGIN WHILE */
    while (1)
    {
//	HAL_DMA2D_Start_IT (&hdma2d, 0x43CD80, 0xc0000000, 1024, 600);
//
//	HAL_Delay (1000);
//	HAL_DMA2D_Start_IT (&hdma2d, 0xFF69B4, 0xc0000000, 1024, 600);
//
//	HAL_Delay (1000);

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

/* USER CODE BEGIN 4 */

//DCMI中断服务函数
//void DCMI_IRQHandler (void)
//{
//    HAL_DCMI_IRQHandler (&hdma_dcmi);
//}


//DMA2数据流1中断服务函数
//void DMA2_Stream1_IRQHandler (void)
//{
//    if (__HAL_DMA_GET_FLAG(&hdma_dcmi,DMA_FLAG_TCIF1_5) != RESET)          //DMA传输完成
//    {
//	__HAL_DMA_CLEAR_FLAG(&hdma_dcmi, DMA_FLAG_TCIF1_5);          //清除DMA传输完成中断标志位
//	dcmi_rx_callback ();	//执行摄像头接收回调函数,读取数据等操作在这里面处理
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
