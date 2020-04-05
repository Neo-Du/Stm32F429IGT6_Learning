/**
  ******************************************************************************
  * File Name          : LTDC.h
  * Description        : This file provides code for the configuration
  *                      of the LTDC instances.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; Copyright (c) 2020 STMicroelectronics.
  * All rights reserved.</center></h2>
  *
  * This software component is licensed by ST under BSD 3-Clause license,
  * the "License"; You may not use this file except in compliance with the
  * License. You may obtain a copy of the License at:
  *                        opensource.org/licenses/BSD-3-Clause
  *
  ******************************************************************************
  */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __ltdc_H
#define __ltdc_H
#ifdef __cplusplus
 extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

extern LTDC_HandleTypeDef hltdc;

/* USER CODE BEGIN Private defines */
typedef struct
{
    u32 pwidth;			//LCD面板的宽�?,固定参数,不随显示方向改变,如果�?0,说明没有任何RGB屏接�?
    u32 pheight;		//LCD面板的高�?,固定参数,不随显示方向改变
    u16 hsw;			//水平同步宽度
    u16 vsw;			//垂直同步宽度
    u16 hbp;			//水平后廊
    u16 vbp;			//垂直后廊
    u16 hfp;			//水平前廊
    u16 vfp;			//垂直前廊
    u8 activelayer;		//当前层编�?:0/1
    u8 dir;				//0,竖屏;1,横屏;
    u16 width;			//LCD宽度
    u16 height;			//LCD高度
    u32 pixsize;		//每个像素�?占字节数
} _ltdc_dev;

#define WHITE         	 0xFFFF
#define BLACK         	 0x0000
#define BLUE         	 0x001F
#define BRED             0XF81F
#define GRED 			 0XFFE0
#define GBLUE			 0X07FF
#define RED           	 0xF800
#define MAGENTA       	 0xF81F
#define GREEN         	 0x07E0
#define CYAN          	 0x7FFF
#define YELLOW        	 0xFFE0
#define BROWN 			 0XBC40 //棕色
#define BRRED 			 0XFC07 //棕红�?
#define GRAY  			 0X8430 //灰色

/* USER CODE END Private defines */

void MX_LTDC_Init(void);

/* USER CODE BEGIN Prototypes */
void LCD_Init (void);
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif
#endif /*__ ltdc_H */

/**
  * @}
  */

/**
  * @}
  */

/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
